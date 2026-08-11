#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H

#include "common.h"

class DownloadTask : public QObject
{
    Q_OBJECT
public:
    explicit DownloadTask(
        const QNetworkRequest& request,
        const QString& path,
        QObject *parent = nullptr);
public:
    void start(QNetworkAccessManager *manager);
    void stop();
    void cancel();

private:
    QNetworkRequest request;
    QString savePath;
    QNetworkReply *reply = nullptr;
    QSaveFile *file = nullptr;

signals:
    void finished();
    void failed(const QString& error);
    void progress(qint64 bytesReceived, qint64 bytesTotal);
};

#endif // DOWNLOADTASK_H
