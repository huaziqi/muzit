#include "mp3transcoder.h"
#include <QDebug>

extern "C"{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
}

Mp3Transcoder::Mp3Transcoder() {}

bool Mp3Transcoder::convert(const AudioConvertOptions &options,
                            const ProgressCallback &progress,
                            const CancellationRequest &cancelRequest,
                            QString &error)
{
    if(progress){
        progress(0);
    }

    if(options.inputPath.isEmpty()){
        error = QStringLiteral("输入文件路径为空");
        return false;
    }
    if(options.outputPath.isEmpty()){

        error = QStringLiteral("输出文件路径为空");
        return false;
    }
    AVFormatContext *inputContext = nullptr;
    const QByteArray inputPath = options.inputPath.toUtf8();
    int result = avformat_open_input(&inputContext, inputPath.constData(), nullptr, nullptr);
    if(result < 0){
        avformat_close_input(&inputContext);
        error = QStringLiteral("无法打开文件");
        return false;
    }
    result = avformat_find_stream_info(inputContext, nullptr);
    if(result < 0){
        avformat_close_input(&inputContext);
        error = QStringLiteral("无法读取音频流信息");
        return false;
    }
    const int& streamIndex = av_find_best_stream(inputContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if(streamIndex < 0){
        avformat_close_input(&inputContext);
        error = QStringLiteral("没有音频流");
        return false;
    }
    AVStream *audioStream = inputContext->streams[streamIndex];
    qDebug() << "audio stream:" << streamIndex;
    qDebug() << "codec:" << audioStream->codecpar->codec_id;
    qDebug() << "sample rate:" << audioStream->codecpar->sample_rate;
    qDebug() << "channels:"
             << audioStream->codecpar->ch_layout.nb_channels;
    qDebug() << "bitrate:" << audioStream->codecpar->bit_rate;

    avformat_close_input(&inputContext);

    // 当前阶段还没有真正生成 MP3。
    error = QStringLiteral("已找到音频流，MP3 编码尚未实现");
    return false;

    return false;
}
