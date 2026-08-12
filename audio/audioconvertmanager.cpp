#include "audioconvertmanager.h"

AudioConvertManager::AudioConvertManager(QObject *parent)
    : QObject(parent)
{
}

AudioConvertTask *AudioConvertManager::addTask(
    const AudioConvertOptions &options)
{
    auto *task = new AudioConvertTask(options);

    pendingTasks.enqueue(task);
    tryStart();

    return task;
}

void AudioConvertManager::tryStart()
{
    while (runningCount < maximumConcurrentTasks
           && !pendingTasks.isEmpty()) {
        AudioConvertTask *task = pendingTasks.dequeue();
        auto *thread = new QThread(this);

        runningTasks.insert(task, thread);
        ++runningCount;

        task->moveToThread(thread);

        connect(thread, &QThread::started,
                task, &AudioConvertTask::start);

        connect(task, &AudioConvertTask::finished,
                this, [this, task](const QString &) {
                    taskEnded(task);
                });

        connect(task, &AudioConvertTask::failed,
                this, [this, task](const QString &) {
                    taskEnded(task);
                });

        connect(task, &AudioConvertTask::canceled,
                this, [this, task]() {
                    taskEnded(task);
                });

        connect(thread, &QThread::finished,
                task, &QObject::deleteLater);

        connect(thread, &QThread::finished,
                thread, &QObject::deleteLater);

        thread->start();
    }
}

void AudioConvertManager::taskEnded(AudioConvertTask *task)
{
    QThread *thread = runningTasks.take(task);
    if (!thread)
        return;

    --runningCount;
    thread->quit();

    // 等 thread 真正退出后再启动下一个，避免短时间超过并发数
    connect(thread, &QThread::finished,
            this, &AudioConvertManager::tryStart);
}
