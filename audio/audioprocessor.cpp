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
    Q_UNUSED(options)
    Q_UNUSED(progressCallback)
    Q_UNUSED(isCanceled)

    error = QStringLiteral("音频处理功能尚未实现");
    return false;
}
