#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include "audioconverter.h"

class AudioProcessor
{
public:
    using ProgressCallback = std::function<void(int)>;
    using CancellationCallback = std::function<bool()>;

    bool process(
        const AudioConvertOptions &options,
        const ProgressCallback &progressCallback,
        const CancellationCallback &isCanceled,
        QString &error);
};

#endif // AUDIOPROCESSOR_H
