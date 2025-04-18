#include "mainwidget.h"

#define PADDING 5


MainWidget::MainWidget(QWidget *parent)
    : FramelessWidget(parent){
    Config config;
    config.readConfig();
    titleBar->setTitle("muzit");
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    this->setMinimumSize(QSize(200, 150));
}

MainWidget::~MainWidget() {}
