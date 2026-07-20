#include "downloadwidget.h"

DownloadWidget::DownloadWidget(DownloadManager *_downloadManager, QWidget *parent)
    : QWidget{parent}, downloadManager(_downloadManager)
{
    mainLayout = new QVBoxLayout(this);
    dlChannel = new QTabWidget();
    mainLayout->addWidget(dlChannel);
    biliDlWidget = new BiliDLWidget(downloadManager);
    dlChannel->addTab(biliDlWidget, "bilibili");

}

