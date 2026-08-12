#include "audioprocessor.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/audio_fifo.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
}

namespace {

bool checkFail(
    bool failed,
    QString &error,
    const QString &errorMessage = "无法加载",
    AVFormatContext **inputContext = nullptr,
    AVFormatContext **outputContext = nullptr,
    AVIOContext **outputPb = nullptr,
    AVPacket **packet = nullptr)
{
    if (!failed)
        return false;

    if(packet != nullptr){
        av_packet_free(packet);
    }

    if(outputPb != nullptr){
        avio_closep(outputPb);
    }

    if (outputContext != nullptr && *outputContext != nullptr) {
        avformat_free_context(*outputContext);
        *outputContext = nullptr;
    }

    if (inputContext != nullptr)
        avformat_close_input(inputContext);

    error = errorMessage;
    return true;
}

}

bool AudioProcessor::process(
    const AudioConvertOptions &options,
    const ProgressCallback &progressCallback,
    const CancellationCallback &isCanceled,
    QString &error)
{
    Q_UNUSED(progressCallback)
    Q_UNUSED(isCanceled)
    AVFormatContext* inputContext = nullptr;
    QByteArray inputPath = options.inputPath.toUtf8();

    int result = avformat_open_input(&inputContext, inputPath.constData(), nullptr, nullptr);
    if(checkFail(result < 0, error, "无法打开输入文件"))
        return false;

    result = avformat_find_stream_info(inputContext, nullptr);
    if(checkFail(result < 0, error, "无法获取信息", &inputContext))
        return false;

    int audioStreamIndex = av_find_best_stream(inputContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if(checkFail(audioStreamIndex < 0, error, "未找到音频流", &inputContext))
        return false;

    AVStream *audioStream = inputContext->streams[audioStreamIndex];
    if(checkFail((audioStream->codecpar->codec_id != AV_CODEC_ID_AAC), error, "当前只支持 AAC 音频", &inputContext))
        return false;

    AVFormatContext *outputContext = nullptr;
    QByteArray outputPath = options.outputPath.toUtf8();
    result = avformat_alloc_output_context2(&outputContext, nullptr, "ipod", outputPath.constData());
    if(checkFail(result < 0 || outputContext == nullptr, error, "无法创建 M4A 输出容器", &inputContext, &outputContext))
        return false;

    AVStream *outputStream = avformat_new_stream(outputContext, nullptr);
    if(checkFail(outputStream == nullptr, error, "无法创建输出音频流", &inputContext, &outputContext))
        return false;
    result = avcodec_parameters_copy(
        outputStream->codecpar,
        audioStream->codecpar
        );
    if(checkFail(result < 0, error, "无法复制音频参数", &inputContext, &outputContext))
        return false;
    outputStream->codecpar->codec_tag = 0;
    outputStream->time_base = audioStream->time_base;

    result = avio_open(
        &outputContext->pb,
        outputPath.constData(),
        AVIO_FLAG_WRITE
        );
    if(checkFail(result < 0, error, "无法打开输出文件", &inputContext, &outputContext))
        return false;

    result = avformat_write_header(outputContext, nullptr);
    if(checkFail(result < 0, error, "无法写入 M4A 文件头", &inputContext, &outputContext, &outputContext->pb))
        return false;
    AVPacket *packet = av_packet_alloc();
    if(checkFail(packet == nullptr, error, "无法创建音频数据包", &inputContext, &outputContext, &outputContext->pb))
        return false;

    while (true) {


        result = av_read_frame(inputContext, packet);

        if (result == AVERROR_EOF)
            break;
        if(checkFail(result < 0, error, "读取音频数据失败", &inputContext, &outputContext, &outputContext->pb, &packet))
            return false;

        if (packet->stream_index == audioStreamIndex) {
            av_packet_rescale_ts(
                packet,
                audioStream->time_base,
                outputStream->time_base
                );

            packet->stream_index = outputStream->index;
            packet->pos = -1;

            result = av_interleaved_write_frame(
                outputContext,
                packet
                );

            if(checkFail(result < 0, error, "写入音频数据失败", &inputContext, &outputContext, &outputContext->pb, &packet))
                return false;
        }

        av_packet_unref(packet);
    }

    result = av_write_trailer(outputContext);
    if(checkFail(result < 0, error, "无法写入 M4A 文件尾", &inputContext, &outputContext, &outputContext->pb))
        return false;

    avio_closep(&outputContext->pb);
    avformat_free_context(outputContext);
    outputContext = nullptr;
    avformat_close_input(&inputContext);
    return true;

}
