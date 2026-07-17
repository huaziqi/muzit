#include "downloadtask.h"

DownloadTask::DownloadTask(const QString& _url, const QString& _path, QObject *parent)
    : QObject{parent}, url(_url), savePath(_path)
{}

void DownloadTask::start(QNetworkAccessManager *manager)
{
    if(reply) return;

    QDir().mkpath(QFileInfo(savePath).absolutePath());
    file = new QSaveFile(savePath, this);
    if(!file->open(QIODevice::WriteOnly)){
        qDebug() << "文件保存在: " << savePath << "失败";
        emit failed("文件打开失败");
        file->deleteLater();
        file = nullptr;
        return;
    }

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");
    request.setRawHeader("Referer", "https://www.bilibili.com");

    reply = manager->get(request);

    connect(reply, &QNetworkReply::downloadProgress, this, &DownloadTask::progress);

    connect(reply, &QNetworkReply::readyRead, this, [this]{
        file->write(reply->readAll());
    });

    connect(reply, &QNetworkReply::finished, this, [this]{
        file->write(reply->readAll());
        if(reply->error() == QNetworkReply::NoError){
            if(!file->commit())
                emit failed("文件写入失败");
            else
                emit finished();
        } else{
            file->cancelWriting();
            emit failed(reply->errorString());
        }
        file->deleteLater();
        file = nullptr;
        reply->deleteLater();
        reply = nullptr;

    });

}

void DownloadTask::stop()
{

}

void DownloadTask::cancel()
{
    if(reply)
        reply->abort();
}
