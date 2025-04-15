#include "mainwidget.h"

#define PADDING 5


MainWidget::MainWidget(QWidget *parent)
    : FramelessWidget(parent){
    Config config;
    config.readConfig();
}

MainWidget::~MainWidget() {}
