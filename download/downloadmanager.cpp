#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent)
    : QObject{parent}
{
    nam = new QNetworkAccessManager(this);
}

DownloadTask *DownloadManager::addTask(const QString &url, const QString &savePath)
{
    DownloadTask *task = new DownloadTask(url, savePath);
    connect(task, &DownloadTask::finished, this, [this, task]{
        taskFinished(task);
    });
    connect(task, &DownloadTask::failed, this, [this, task](const QString &){
        taskFinished(task);
    });

    if(curRTask < maxRTask){
        curRTask += 1;
        task->start(nam);
    } else{
        PendingQueue.append(task);
    }

    return task;
}

QNetworkReply *DownloadManager::fetch(const QNetworkRequest &request){
    return nam->get(request);
}

void DownloadManager::tryStart()
{
    if(curRTask < maxRTask && !PendingQueue.empty()){
        DownloadTask *task = PendingQueue.dequeue();
        curRTask += 1;
        task->start(nam);
    }
}

void DownloadManager::taskFinished(DownloadTask *task)
{
    curRTask -= 1;
    task->deleteLater();
    tryStart();
}
