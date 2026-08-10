#ifndef AUDIOCONVERTTASK_H
#define AUDIOCONVERTTASK_H

#include "audiotypes.h"

#include <QObject>
#include <atomic>

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
    AudioConvertOptions options;
    std::atomic_bool cancelRequested{false};
};

#endif // AUDIOCONVERTTASK_H
