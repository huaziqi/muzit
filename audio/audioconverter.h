#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include "audiotypes.h"

#include <QString>
#include <functional>

class AudioConverter
{
public:
    using ProcessCallback = std::function<void(int)>;
    using CancellationCallback = std::function<bool()>;

    virtual ~AudioConverter() = default;

    virtual bool convert(
        const AudioConvertOptions &option,
        const ProcessCallback &process,
        const CancellationCallback &cancel,
        const QString &error
    );
};

#endif // AUDIOCONVERTER_H
