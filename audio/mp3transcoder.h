#ifndef MP3TRANSCODER_H
#define MP3TRANSCODER_H

#include <QObject>
#include "audioconverter.h"

class Mp3Transcoder : public AudioConverter
{
    Q_OBJECT
public:
    Mp3Transcoder();
    bool convert(
        const AudioConvertOptions &option,
        const ProgressCallback &progress,
        const CancellationCallback &isCancelled,
        QString &error
    ) override;
};

#endif // MP3TRANSCODER_H
