#ifndef BILIDLTOOL_H
#define BILIDLTOOL_H

#include "common.h"
#include "download/downloadmanager.h"
#include "api/bilibiliapi.h"
#include "api/apiparser.h"

class BiliDLTool : public QObject
{
    Q_OBJECT
private:
    DownloadManager *downloadManager;

public:
    explicit BiliDLTool(
        DownloadManager *_downloadManager,
        QObject *parent = nullptr);

    void getVideoInfo(const QString &bvid);
    void getPartAudioStreams(BiliPlayUrlInfo playUrlInfo, QString bvid, qint64 index);
    DownloadTask *downloadAudio(const AudioDownloadJob &job);

signals:
    void videoInfoReady(BiliVideoInfo info);
    void videoInfoFailed(QString error);
    void partAudioStreamsReady(BiliPlayUrlInfo playUrlInfo, qint64 index);
    void partAudioStreamsFailed(qint64 cid, QString error);
};

#endif // BILIDLTOOL_H
