#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include "audiotypes.h"

#include <QString>
#include <functional>
#include <QDebug>

class AudioProcessor
{
public:
    using ProgressCallback = std::function<void(qint64&)>;
    using CancellationCallback = std::function<bool()>;

    bool process(
        const AudioConvertOptions &options,
        const ProgressCallback &progressCallback,
        const CancellationCallback &isCanceled,
        QString &error);
};

#endif // AUDIOPROCESSOR_H
