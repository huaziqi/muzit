#include "audioconverttask.h"

AudioConvertTask::AudioConvertTask(
    const AudioConvertOptions &options,
    QObject *parent)
    : QObject{parent}, options{options}
{
}

void AudioConvertTask::start()
{
    QString error;

}





void AudioConvertTask::cancel()
{
    cancelRequested.store(true);
}
