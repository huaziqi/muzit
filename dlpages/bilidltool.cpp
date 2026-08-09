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

void BiliDLTool::getPartAudioStreams(BiliVideoInfo videoInfo, qint64 cid)
{
    if (cid <= 0) {
        emit partAudioStreamsFailed(cid, QStringLiteral("CID无效"));
        return;
    }

    qsizetype partIndex = -1;
    for (qsizetype i = 0; i < videoInfo.parts.size(); ++i) {
        if (videoInfo.parts.at(i).cid == cid) {
            partIndex = i;
            break;
        }
    }

    if (partIndex < 0) {
        emit partAudioStreamsFailed(
            cid, QStringLiteral("VideoInfo中找不到对应的CID"));
        return;
    }

    QNetworkReply *reply = downloadManager->fetch(
        BilibiliApi::playUrlRequest(videoInfo.bvid, cid));

    connect(reply, &QNetworkReply::finished, this,
            [this, reply, videoInfo, partIndex, cid]() mutable {
        if (reply->error() != QNetworkReply::NoError) {
            emit partAudioStreamsFailed(cid, reply->errorString());
            reply->deleteLater();
            return;
        }

        QString error;
        if (!ApiParser::parsePlayUrlInfo(
                reply->readAll(), videoInfo.parts[partIndex], error)) {
            emit partAudioStreamsFailed(cid, error);
            reply->deleteLater();
            return;
        }

        reply->deleteLater();
        emit partAudioStreamsReady(videoInfo);
    });
}

DownloadTask *BiliDLTool::downloadAudio(
    const QString &audioUrl,
    const QString &savePath)
{
    return downloadManager->addTask(audioUrl, savePath);
}
