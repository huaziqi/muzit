#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include "audiotypes.h"

#include <QString>
#include <functional>

class AudioConverter
{
public:
    using ProgressCallback = std::function<void(int)>;
    using CancellationCallback = std::function<bool()>;

    virtual ~AudioConverter() = default;

    virtual bool convert(
        const AudioConvertOptions &option,
        const ProgressCallback &progress,
        const CancellationCallback &isCancelled,
        QString &error
    ) = 0;
};

#endif // AUDIOCONVERTER_H
