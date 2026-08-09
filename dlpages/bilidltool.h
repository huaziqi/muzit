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

    explicit BiliDLTool(DownloadManager *_downloadManager, QObject *parent = nullptr);
    void getVideoCid(QString bvid);
signals:
    void cidReady(qint64 cid);
    void cidFailed(QString error);
};

#endif // BILIDLTOOL_H
