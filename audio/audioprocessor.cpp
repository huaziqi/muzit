#include "audioprocessor.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/audio_fifo.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
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
    if(result < 0){
        error = QStringLiteral("无法打开文件");
        return false;
    }
    result = avformat_find_stream_info(inputContext, nullptr);
    if(result < 0){
        error = QStringLiteral("无法获取信息");
        return false;
    }
    qDebug() << inputContext->nb_streams;
    qDebug() << inputContext->duration;

    avformat_close_input(&inputContext);
    return true;

}
