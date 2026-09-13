#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include "ui/uicommon.h"
#include "bilibili/bilidlwidget.h"
#include "core/download/downloadmanager.h"

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
