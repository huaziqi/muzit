#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent)
    : QObject{parent}
{
    nam = new QNetworkAccessManager(this);

}

DownloadTask *DownloadManager::addTask(const QString &url, const QString &savePath)
{
    DownloadTask *task = new DownloadTask(url, savePath);
    if(curRTask < maxRTask){

        task->start(nam);
        curRTask += 1;
        connect(task, &DownloadTask::finished, this, [this, task]{
            taskFinished(task);
        });
    } else{
        PendingQueue.append(task);
    }

    return task;
}

void DownloadManager::tryStart()
{
    if(curRTask < maxRTask && !PendingQueue.empty()){
        PendingQueue.dequeue()->start(nam);
        curRTask += 1;
    }
}



void DownloadManager::taskFinished(DownloadTask *task)
{
    curRTask -= 1;
    task->deleteLater();
    tryStart();
}
