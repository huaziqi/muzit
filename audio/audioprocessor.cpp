#include "audioprocessor.h"
#include "m4aremuxer.h"
#include "mp3transcoder.h"

AudioProcessor::AudioProcessor(
    const AudioConvertOptions& options, QObject *parent)
    : QObject(parent), audioOptions(options){
    mainConverter = createConverter(audioOptions.format);
}

std::unique_ptr<AudioConverter> AudioProcessor::createConverter(const AudioOutputFormat& format)
{
    switch (format){
    case AudioOutputFormat::M4a:
        return std::make_unique<M4aRemuxer>();
    case AudioOutputFormat::Mp3:
        return std::make_unique<Mp3Transcoder>();
    default:
        return nullptr;
    }
}

void AudioProcessor::start(const AudioConverter::ProgressCallback& progressFunc,
                           const AudioConverter::CancellationRequest& cancelRequest,
                           QString& error)
{
    startConvert(audioOptions, progressFunc, cancelRequest, error);
}

void AudioProcessor::startConvert(const AudioConvertOptions& options,
                                  const AudioConverter::ProgressCallback& progressFunc,
                                  const AudioConverter::CancellationRequest& cancelRequest,
                                  QString& error)
{
    const bool isSuccess = mainConverter->convert(options, progressFunc, cancelRequest, error);
}
