#ifndef BILIDLWIDGET_H
#define BILIDLWIDGET_H

#include "ui/uicommon.h"
#include "core/download/downloadmanager.h"
#include "platforms/bilibili/bilidltool.h"
#include "platforms/bilibili/bilitypes.h"
#include "bilisearchbar.h"
#include "biliresultlist.h"
#include "bilisidepanel.h"
#include "platforms/bilibili/api/bilibiliclient.h"


class BiliDLWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BiliDLWidget(DownloadManager *_downloadManager, QWidget *parent = nullptr);

private:
    QVBoxLayout  *mainLayout;
    BiliSearchBar  *searchBar;
    BiliResultList *resultList;
    BiliSidePanel  *sidePanel;
    DownloadManager *downloadManager;

    BiliDLTool *biliDLTool;
    BilibiliClient *bilibiliClient;

    BiliVideoInfo pendingAudioInfo;
    QVector<BiliVideoInfo> queuedAudioInfo;
    QVector<qint64> pendingAudioIndex;
    bool audioRequestInProgress = false;
    bool hasQueuedAudioInfo = false;
    bool keywordSearchActive = false;

    int curPageSize = -1;

    void loadSelectedPartAudio(BiliVideoInfo info);
    void requestNextPartAudio();
    void finishPartAudioLoading();

private slots:
    void onSearchRequested(const QString &keyword, BiliSearchType type, int pageSize);
    void onKeywordSearchFinished(const QVector<BiliVideoInfo> &results,
                                 int page,
                                 int totalResults);
    void onKeywordSearchFailed(const QString &error);
    void onVideoInfoFailed(const QString &error);
    void onItemSelected(const BiliVideoInfo &info);
    void onPartsSelectionChanged(const BiliVideoInfo &info);
    void onVideoInfoReady(const BiliVideoInfo &info);
    void onPartAudioStreamsReady(const BiliPlayUrlInfo &info, qint64 index);
    void onPartAudioStreamsFailed(qint64 cid, const QString &error);
    void onDownloadRequested(const BiliVideoInfo &info);
    void onFavoriteRequested(const BiliVideoInfo &info);

signals:
};

#endif // BILIDLWIDGET_H
