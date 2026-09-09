#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include "audioconverter.h"

class AudioProcessor
{
public:
    std::unique_ptr<AudioProcessor> createConverter(AudioOutputFormat format);
};

#endif // AUDIOPROCESSOR_H
