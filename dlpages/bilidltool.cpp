#include "bilidltool.h"

BiliDLTool::BiliDLTool(
    DownloadManager *_downloadManager,
    QObject *parent)
    : QObject{parent}, downloadManager(_downloadManager)
{
}

void BiliDLTool::getVideoInfo(const QString &bvid)
{
    QNetworkReply *reply =
        downloadManager->fetch(BilibiliApi::videoInfoRequest(bvid));

    connect(reply, &QNetworkReply::finished, this, [this, reply]{
        if (reply->error() != QNetworkReply::NoError) {
            emit videoInfoFailed(reply->errorString());
            reply->deleteLater();
            return;
        }

        BiliVideoInfo info;
        QString error;
        if (!ApiParser::parseVideoInfo(reply->readAll(), info, error)) {
            emit videoInfoFailed(error);
            reply->deleteLater();
            return;
        }

        reply->deleteLater();
        emit videoInfoReady(info);
    });
}

void BiliDLTool::getPartAudioStreams(BiliPlayUrlInfo playUrlInfo, QString bvid, qint64 index)
{
    QNetworkReply *reply = downloadManager->fetch(
        BilibiliApi::playUrlRequest(bvid, playUrlInfo.cid));

    connect(reply, &QNetworkReply::finished, this,
            [this, reply, playUrlInfo, index]() mutable {
        if (reply->error() != QNetworkReply::NoError) {
            emit partAudioStreamsFailed(playUrlInfo.cid, reply->errorString());
            reply->deleteLater();
            return;
        }

        QString error;
        if (!ApiParser::parsePlayUrlInfo(
                reply->readAll(), playUrlInfo, error)) {
            emit partAudioStreamsFailed(playUrlInfo.cid, error);
            reply->deleteLater();
            return;
        }

        reply->deleteLater();
        emit partAudioStreamsReady(playUrlInfo, index);
    });
}

DownloadTask *BiliDLTool::downloadAudio(const AudioDownloadJob &job)
{
    return downloadManager->addTask(
        BilibiliApi::audioDownloadRequest(QUrl(job.source.url)),
        job.temporaryPath);
}
