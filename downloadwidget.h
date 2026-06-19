#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include "common.h"
#include "dlpages/bilidlwidget.h"

class DownloadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadWidget(QWidget *parent = nullptr);

private:

    QTabWidget* dlChannel;
    QVBoxLayout* mainLayout;

    BiliDLWidget* biliDlWidget;
signals:
};

#endif // DOWNLOADWIDGET_H
