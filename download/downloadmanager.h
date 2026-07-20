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
        taskInfo(const QString& _url, const QString& _savePath): url(_url), savePath(_savePath) {}
        QString url;
        QString savePath;
    };
    explicit DownloadManager(QObject *parent = nullptr);
    DownloadTask* addTask(const QString& url, const QString& savePath);
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
