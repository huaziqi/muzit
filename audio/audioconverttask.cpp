#include "audioconverttask.h"

AudioConvertTask::AudioConvertTask(
    const AudioConvertOptions &options,
    QObject *parent)
    : QObject{parent}, options{options}
{
}

void AudioConvertTask::start()
{
    AudioProcessor processor;
    QString error;
    if(!processor.process(options,
                           [=](qint64 &process){}, [=](){return cancelRequested.load();}, error)){
        emit failed(error);
        return;
    }
    emit finished(options.outputPath);
}



void AudioConvertTask::cancel()
{
    cancelRequested.store(true);
}
