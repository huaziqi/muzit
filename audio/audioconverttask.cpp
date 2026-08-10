#include "audioconverttask.h"

AudioConvertTask::AudioConvertTask(
    const AudioConvertOptions &options,
    QObject *parent)
    : QObject{parent}, options{options}
{
}

void AudioConvertTask::start()
{
    emit failed(QStringLiteral("音频转换任务尚未实现"));
}

void AudioConvertTask::cancel()
{
    cancelRequested.store(true);
}
