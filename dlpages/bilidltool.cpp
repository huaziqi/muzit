#include "bilidltool.h"

BiliDLTool::BiliDLTool(DownloadManager *_downloadManager, QObject *parent)
    : QObject{parent}, downloadManager(_downloadManager)
{

}

void BiliDLTool::getVideoCid(QString bvid)
{
    QNetworkReply* reply = downloadManager->fetch(BilibiliApi::videoInfoRequest(bvid));


    connect(reply, &QNetworkReply::finished, this, [=]{
        QByteArray byteArray = reply->readAll();
        qint64 cid = 0;
        QString error;
        if(ApiParser::parseCid(byteArray, cid, error)){
            emit cidFailed(error);
        }
        emit cidReady(cid);
        reply->deleteLater();
    });
}
