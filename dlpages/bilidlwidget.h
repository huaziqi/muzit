#ifndef BILIDLWIDGET_H
#define BILIDLWIDGET_H

#include "common.h"
#include "download/downloadmanager.h"
#include "bilidltool.h"
#include "bilitypes.h"
#include "bilisearchbar.h"
#include "biliresultlist.h"
#include "bilisidepanel.h"


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

    void loadDemoData();

private slots:
    void onSearchRequested(const QString &keyword, BiliSearchType type, int pageSize);
    void onItemSelected(const BiliVideoInfo &info);
    void onDownloadRequested(const BiliVideoInfo &info);
    void onFavoriteRequested(const BiliVideoInfo &info);

signals:
};

#endif // BILIDLWIDGET_H
