#ifndef BILIDLTOOL_H
#define BILIDLTOOL_H

#include "common.h"
#include "download/downloadmanager.h"
#include "api/bilibiliapi.h"
#include "api/apiparser.h"
#include "audio/audioconvertmanager.h"


class BiliDLTool : public QObject
{
    Q_OBJECT
private:
    DownloadManager *downloadManager;
    AudioConvertManager *audioConvertManager;

public:
    explicit BiliDLTool(
        DownloadManager *_downloadManager,
        QObject *parent = nullptr);

    void getVideoInfo(const QString &bvid);
    void getPartAudioStreams(BiliPlayUrlInfo playUrlInfo, QString bvid, qint64 index);
    bool createAudioDownloadJob(
        const BiliVideoInfo &videoInfo,
        const BiliPlayUrlInfo &part,
        const BiliSaveSettings &settings,
        const AudioMetadata &metadata,
        AudioDownloadJob &job,
        QString &error) const;
    DownloadTask *downloadAudio(const AudioDownloadJob &job);


signals:
    void videoInfoReady(BiliVideoInfo info);
    void videoInfoFailed(QString error);
    void partAudioStreamsReady(BiliPlayUrlInfo playUrlInfo, qint64 index);
    void partAudioStreamsFailed(qint64 cid, QString error);
};

#endif // BILIDLTOOL_H
