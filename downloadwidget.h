#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include "common.h"
#include "dlpages/bilidlwidget.h"
#include "download/downloadmanager.h"

class DownloadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadWidget(DownloadManager *_downloadManager, QWidget *parent = nullptr);

private:
    DownloadManager *downloadManager;
    QTabWidget* dlChannel;
    QVBoxLayout* mainLayout;

    BiliDLWidget* biliDlWidget;
signals:


};

#endif // DOWNLOADWIDGET_H
