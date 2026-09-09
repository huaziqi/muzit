#include "audioprocessor.h"
#include "m4aremuxer.h"
#include "mp3transcoder.h"

std::unique_ptr<AudioConverter> AudioProcessor::createConverter(AudioOutputFormat format)
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
