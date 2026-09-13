#ifndef AUDIOCONVERTTASK_H
#define AUDIOCONVERTTASK_H

#include "audiotypes.h"
#include "audioprocessor.h"

#include <QObject>
#include <atomic>
#include <memory.h>

class AudioConvertTask : public QObject
{
    Q_OBJECT

public:
    explicit AudioConvertTask(
        const AudioConvertOptions &options,
        QObject *parent = nullptr);

    void start();
    void cancel();

signals:
    void progressChanged(int percent);
    void finished(const QString &outputPath);
    void failed(const QString &error);
    void canceled();

private:
    AudioConverter::ProgressCallback progressFunc;
    AudioConverter::CancellationRequest cancelRequest;
    AudioConvertOptions options;
    std::unique_ptr<AudioProcessor> mainProcessor;
    std::atomic_bool cancelRequested{false};
};

#endif // AUDIOCONVERTTASK_H
