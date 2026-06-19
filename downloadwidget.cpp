#include "downloadwidget.h"

DownloadWidget::DownloadWidget(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QVBoxLayout(this);
    dlChannel = new QTabWidget();
    mainLayout->addWidget(dlChannel);
    biliDlWidget = new BiliDLWidget();
    dlChannel->addTab(biliDlWidget, "bilibili");

}
