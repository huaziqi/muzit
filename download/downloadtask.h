#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H

#include "common.h"

class DownloadTask : public QObject
{
    Q_OBJECT
public:
    explicit DownloadTask(const QString& _url, const QString& _path, QObject *parent = nullptr);
public:
    void start(QNetworkAccessManager *manager);
    void cancel();

private:
    QString url;
    QString savePath;
    QNetworkReply *reply = nullptr;
    QSaveFile *file = nullptr;

signals:
    void finished();
    void failed(const QString& error);
    void progress(qint64 bytesReceived, qint64 bytesTotal);
};

#endif // DOWNLOADTASK_H
