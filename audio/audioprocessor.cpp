#include "audioprocessor.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/audio_fifo.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
}

namespace {
bool checkResult(
    bool failed,
    QString &error,
    const QString &errorMessage = "无法加载",
    AVFormatContext **inputContext = nullptr,
    AVFormatContext **outputContext = nullptr)
{
    if (!failed)
        return true;

    if (outputContext != nullptr) {
        avformat_free_context(*outputContext);
        outputContext = nullptr;
    }

    if (inputContext != nullptr)
        avformat_close_input(inputContext);

    error = errorMessage;
    return false;
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
    if(checkResult(result < 0, error, "无法打开输入文件"))
        return false;

    result = avformat_find_stream_info(inputContext, nullptr);
    if(checkResult(result < 0, error, "无法获取信息", &inputContext))
        return false;

    int audioStreamIndex = av_find_best_stream(inputContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if(checkResult(audioStreamIndex < 0, error, "未找到音频流", &inputContext))
        return false;

    AVStream *audioStream = inputContext->streams[audioStreamIndex];
    if(checkResult((audioStream->codecpar->codec_id != AV_CODEC_ID_AAC), error, "当前只支持 AAC 音频", &inputContext))
        return false;

    AVFormatContext *outputContext = nullptr;
    QByteArray outputPath = options.outputPath.toUtf8();
    result = avformat_alloc_output_context2(&outputContext, nullptr, "ipod", outputPath.constData());
    if(checkResult(result < 0, error, "无法创建 M4A 输出容器", &inputContext))
        return false;

    AVStream *outputStream = avformat_new_stream(outputContext, nullptr);
    if(checkResult(outputStream == nullptr, error, "无法创建输出音频流", &inputContext, &outputContext))
        return false;

    result = avcodec_parameters_copy(
        outputStream->codecpar,
        audioStream->codecpar
        );

    if(checkResult(result < 0, error, "无法复制音频参数", &inputContext, &outputContext))
        return false;


    avformat_free_context(outputContext);
    avformat_close_input(&inputContext);
    return true;

}
