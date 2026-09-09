#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include "audioconverter.h"
#include "m4aremuxer.h"
#include "mp3transcoder.h"

class AudioProcessor
{
public:
    std::unique_ptr<AudioConverter> createConverter(AudioOutputFormat format);
};

#endif // AUDIOPROCESSOR_H
