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
    AudioProcessor processor = createConverter(options.format);
}

std::unique_ptr<AudioConverter> createConverter(AudioOutputFormat format){
    switch(format){
    case AudioOutputFormat::M4a:
        return std::make_unique<M4aRemuxer>();
    case AudioOutputFormat::Mp3:
        return std::make_unique<Mp3Transcoder>();
    default:
        return nullptr;
    }
}



void AudioConvertTask::cancel()
{
    cancelRequested.store(true);
}
