#include "mp3transcoder.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/audio_fifo.h>
#include <libavutil/channel_layout.h>
#include <libavutil/mathematics.h>
#include <libswresample/swresample.h>
}

struct Mp3Transcoder::TranscodeContext
{
    AVFormatContext *inputFormat = nullptr;
    AVCodecContext *decoder = nullptr;
    AVStream *inputStream = nullptr;
    int inputStreamIndex = -1;

    AVFormatContext *outputFormat = nullptr;
    AVCodecContext *encoder = nullptr;
    AVStream *outputStream = nullptr;

    SwrContext *resampler = nullptr;
    AVAudioFifo *fifo = nullptr;

    AVPacket *inputPacket = nullptr;
    AVPacket *outputPacket = nullptr;
    AVFrame *decodedFrame = nullptr;

    int64_t nextOutputPts = 0;

    ~TranscodeContext()
    {
        av_frame_free(&decodedFrame);
        av_packet_free(&inputPacket);
        av_packet_free(&outputPacket);

        if (fifo)
            av_audio_fifo_free(fifo);

        swr_free(&resampler);
        avcodec_free_context(&decoder);
        avcodec_free_context(&encoder);
        avformat_close_input(&inputFormat);

        if (outputFormat) {
            if (outputFormat->pb)
                avio_closep(&outputFormat->pb);

            avformat_free_context(outputFormat);
        }
    }
};

Mp3Transcoder::Mp3Transcoder()
{
}

bool Mp3Transcoder::convert(const AudioConvertOptions &options,
                            const ProgressCallback &progress,
                            const CancellationRequest &cancelRequest,
                            QString &error)
{
    error.clear();

    if (progress)
        progress(0);

    TranscodeContext context;

    initializeInput(context, options);
    initializeOutput(context, options);
    initializeResampler(context);
    initializeBuffers(context);

    if (!transcodePackets(context, cancelRequest))
        return false;

    flushDecoder(context);
    flushResampler(context);
    encodeAvailableSamples(context, true);
    flushEncoder(context);

    av_write_trailer(context.outputFormat);

    if (progress)
        progress(100);

    return true;
}

void Mp3Transcoder::initializeInput(
    TranscodeContext &context,
    const AudioConvertOptions &options)
{
    const QByteArray inputPath = options.inputPath.toUtf8();

    avformat_open_input(
        &context.inputFormat,
        inputPath.constData(),
        nullptr,
        nullptr);

    avformat_find_stream_info(context.inputFormat, nullptr);

    context.inputStreamIndex = av_find_best_stream(
        context.inputFormat,
        AVMEDIA_TYPE_AUDIO,
        -1,
        -1,
        nullptr,
        0);

    context.inputStream =
        context.inputFormat->streams[context.inputStreamIndex];

    const AVCodec *decoder = avcodec_find_decoder(
        context.inputStream->codecpar->codec_id);

    context.decoder = avcodec_alloc_context3(decoder);

    avcodec_parameters_to_context(
        context.decoder,
        context.inputStream->codecpar);

    context.decoder->pkt_timebase = context.inputStream->time_base;

    avcodec_open2(context.decoder, decoder, nullptr);
}

void Mp3Transcoder::initializeOutput(
    TranscodeContext &context,
    const AudioConvertOptions &options)
{
    const QByteArray outputPath = options.outputPath.toUtf8();

    avformat_alloc_output_context2(
        &context.outputFormat,
        nullptr,
        "mp3",
        outputPath.constData());

    const AVCodec *encoder = avcodec_find_encoder(AV_CODEC_ID_MP3);
    context.encoder = avcodec_alloc_context3(encoder);

    const void *supportedSampleFormats = nullptr;
    avcodec_get_supported_config(
        context.encoder,
        encoder,
        AV_CODEC_CONFIG_SAMPLE_FORMAT,
        0,
        &supportedSampleFormats,
        nullptr);

    const auto *sampleFormats =
        static_cast<const AVSampleFormat *>(supportedSampleFormats);

    context.encoder->bit_rate = options.mp3Bitrate;
    context.encoder->sample_rate = context.decoder->sample_rate;
    context.encoder->sample_fmt = sampleFormats
        ? sampleFormats[0]
        : AV_SAMPLE_FMT_FLTP;
    context.encoder->time_base = AVRational{1, context.encoder->sample_rate};

    av_channel_layout_copy(
        &context.encoder->ch_layout,
        &context.decoder->ch_layout);

    if (context.outputFormat->oformat->flags & AVFMT_GLOBALHEADER)
        context.encoder->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    avcodec_open2(context.encoder, encoder, nullptr);

    context.outputStream = avformat_new_stream(
        context.outputFormat,
        nullptr);

    avcodec_parameters_from_context(
        context.outputStream->codecpar,
        context.encoder);

    context.outputStream->time_base = context.encoder->time_base;
    context.outputStream->codecpar->codec_tag = 0;

    avio_open(
        &context.outputFormat->pb,
        outputPath.constData(),
        AVIO_FLAG_WRITE);

    avformat_write_header(context.outputFormat, nullptr);
}

void Mp3Transcoder::initializeResampler(TranscodeContext &context)
{
    swr_alloc_set_opts2(
        &context.resampler,
        &context.encoder->ch_layout,
        context.encoder->sample_fmt,
        context.encoder->sample_rate,
        &context.decoder->ch_layout,
        context.decoder->sample_fmt,
        context.decoder->sample_rate,
        0,
        nullptr);

    swr_init(context.resampler);

    context.fifo = av_audio_fifo_alloc(
        context.encoder->sample_fmt,
        context.encoder->ch_layout.nb_channels,
        1);
}

void Mp3Transcoder::initializeBuffers(TranscodeContext &context)
{
    context.inputPacket = av_packet_alloc();
    context.outputPacket = av_packet_alloc();
    context.decodedFrame = av_frame_alloc();
}

bool Mp3Transcoder::transcodePackets(
    TranscodeContext &context,
    const CancellationRequest &cancelRequest)
{
    while (av_read_frame(context.inputFormat, context.inputPacket) >= 0) {
        if (cancelRequest && cancelRequest())
            return false;

        if (context.inputPacket->stream_index != context.inputStreamIndex) {
            av_packet_unref(context.inputPacket);
            continue;
        }

        avcodec_send_packet(context.decoder, context.inputPacket);
        av_packet_unref(context.inputPacket);
        receiveDecodedFrames(context);
    }

    return true;
}

void Mp3Transcoder::receiveDecodedFrames(TranscodeContext &context)
{
    while (avcodec_receive_frame(context.decoder, context.decodedFrame) == 0) {
        resampleToFifo(context, false);
        encodeAvailableSamples(context, false);
        av_frame_unref(context.decodedFrame);
    }
}

int Mp3Transcoder::resampleToFifo(
    TranscodeContext &context,
    bool draining)
{
    AVFrame *inputFrame = draining ? nullptr : context.decodedFrame;
    const int inputSampleCount = inputFrame ? inputFrame->nb_samples : 0;

    const int64_t delayedSamples = swr_get_delay(
        context.resampler,
        context.decoder->sample_rate);

    const int outputCapacity = static_cast<int>(av_rescale_rnd(
        delayedSamples + inputSampleCount,
        context.encoder->sample_rate,
        context.decoder->sample_rate,
        AV_ROUND_UP));

    AVFrame *convertedFrame = av_frame_alloc();
    convertedFrame->format = context.encoder->sample_fmt;
    convertedFrame->sample_rate = context.encoder->sample_rate;
    convertedFrame->nb_samples = outputCapacity;

    av_channel_layout_copy(
        &convertedFrame->ch_layout,
        &context.encoder->ch_layout);

    av_frame_get_buffer(convertedFrame, 0);

    const uint8_t **inputData = inputFrame
        ? (const uint8_t **)inputFrame->extended_data
        : nullptr;

    const int convertedSampleCount = swr_convert(
        context.resampler,
        convertedFrame->extended_data,
        outputCapacity,
        inputData,
        inputSampleCount);

    av_audio_fifo_realloc(
        context.fifo,
        av_audio_fifo_size(context.fifo) + convertedSampleCount);

    av_audio_fifo_write(
        context.fifo,
        (void **)convertedFrame->extended_data,
        convertedSampleCount);

    av_frame_free(&convertedFrame);
    return convertedSampleCount;
}

void Mp3Transcoder::encodeAvailableSamples(
    TranscodeContext &context,
    bool includeRemainingSamples)
{
    const int frameSize = context.encoder->frame_size;

    while (av_audio_fifo_size(context.fifo) >= frameSize)
        encodeOneFrame(context, frameSize);

    if (includeRemainingSamples && av_audio_fifo_size(context.fifo) > 0)
        encodeOneFrame(context, av_audio_fifo_size(context.fifo));
}

void Mp3Transcoder::encodeOneFrame(
    TranscodeContext &context,
    int sampleCount)
{
    AVFrame *frame = av_frame_alloc();
    frame->format = context.encoder->sample_fmt;
    frame->sample_rate = context.encoder->sample_rate;
    frame->nb_samples = sampleCount;

    av_channel_layout_copy(
        &frame->ch_layout,
        &context.encoder->ch_layout);

    av_frame_get_buffer(frame, 0);

    av_audio_fifo_read(
        context.fifo,
        (void **)frame->extended_data,
        sampleCount);

    frame->pts = context.nextOutputPts;
    context.nextOutputPts += sampleCount;

    avcodec_send_frame(context.encoder, frame);
    writeEncodedPackets(context);

    av_frame_free(&frame);
}

void Mp3Transcoder::writeEncodedPackets(TranscodeContext &context)
{
    while (avcodec_receive_packet(
               context.encoder,
               context.outputPacket) == 0) {
        av_packet_rescale_ts(
            context.outputPacket,
            context.encoder->time_base,
            context.outputStream->time_base);

        context.outputPacket->stream_index = context.outputStream->index;
        context.outputPacket->pos = -1;

        av_interleaved_write_frame(
            context.outputFormat,
            context.outputPacket);

        av_packet_unref(context.outputPacket);
    }
}

void Mp3Transcoder::flushDecoder(TranscodeContext &context)
{
    avcodec_send_packet(context.decoder, nullptr);
    receiveDecodedFrames(context);
}

void Mp3Transcoder::flushResampler(TranscodeContext &context)
{
    while (swr_get_delay(
               context.resampler,
               context.decoder->sample_rate) > 0) {
        const int convertedSamples = resampleToFifo(context, true);

        if (convertedSamples <= 0)
            break;

        encodeAvailableSamples(context, false);
    }
}

void Mp3Transcoder::flushEncoder(TranscodeContext &context)
{
    avcodec_send_frame(context.encoder, nullptr);
    writeEncodedPackets(context);
}
