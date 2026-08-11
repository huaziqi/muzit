#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "common.h"
#include "downloadtask.h"

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    struct taskInfo
    {
        taskInfo(const QNetworkRequest& _request, const QString& _savePath)
            : request(_request), savePath(_savePath) {}
        QNetworkRequest request;
        QString savePath;
    };
    explicit DownloadManager(QObject *parent = nullptr);
    DownloadTask* addTask(const QNetworkRequest& request, const QString& savePath);
    QNetworkReply* fetch(const QNetworkRequest& request);

private:
    QNetworkAccessManager *nam;
    QQueue<DownloadTask*> PendingQueue;
    qint64 curRTask = 0;
    const qint64 maxRTask = 3;

    void tryStart();

signals:

private slots:
    void taskFinished(DownloadTask* task);
};

#endif // DOWNLOADMANAGER_H
