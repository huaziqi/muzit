#ifndef MP3TRANSCODER_H
#define MP3TRANSCODER_H

#include "audioconverter.h"

class Mp3Transcoder : public AudioConverter
{
public:
    Mp3Transcoder();
    bool convert(
        const AudioConvertOptions &options,
        const ProgressCallback &progress,
        const CancellationRequest &cancelRequest,
        QString &error
    ) override;
};

#endif // MP3TRANSCODER_H
