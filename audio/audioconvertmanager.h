#ifndef AUDIOCONVERTMANAGER_H
#define AUDIOCONVERTMANAGER_H

#include "audioconverttask.h"

#include <QObject>
#include <QQueue>
#include <QHash>
#include <QThread>

class AudioConvertManager : public QObject
{
    Q_OBJECT

public:
    explicit AudioConvertManager(QObject *parent = nullptr);

    AudioConvertTask *addTask(const AudioConvertOptions &options);

private:
    void tryStart();
    void taskEnded(AudioConvertTask *task);

    QQueue<AudioConvertTask *> pendingTasks;
    QHash<AudioConvertTask *, QThread *> runningTasks;

    int runningCount = 0;
    int maximumConcurrentTasks = 3;
};

#endif
