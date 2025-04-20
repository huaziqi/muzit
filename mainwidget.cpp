#include "mainwidget.h"

#define PADDING 5

MainWidget::MainWidget(QWidget *parent)
    : FramelessWidget(parent){
    Config config;
    config.readConfig();
    titleBar->setTitle("Muzit");
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    this->setMinimumSize(QSize(200, 150));

    initSidebar();
    initRight();
    exploreWidget = new ExploreWidget();
    funcWidget->addWidget(exploreWidget);
    localWidget = new LocalWidget();
    funcWidget->addWidget(localWidget);
    funcWidget->setCurrentWidget(0);
}

void MainWidget::initSidebar()
{
    sidebarWidget = new QWidget(this);
    sidebarLayout = new QVBoxLayout();
    sidebarWidget->setLayout(sidebarLayout);

    contentLayout->addWidget(sidebarWidget);
    contentLayout->setAlignment(Qt::AlignLeft);
    sidebarWidget->setFixedWidth(100);
    sideTitle = new QLabel(this);

    sideTitle->setAlignment(Qt::AlignCenter);

    sidebarLayout->setAlignment(Qt::AlignTop);
    sideTitle->setText("Muzit");
    sidebarLayout->addWidget(sideTitle);

    sidebarButtons = new QButtonGroup();
    exploreButton = new QPushButton("探索音乐");

    localMusicButton = new QPushButton("本地音乐");

    sidebarButtons->addButton(exploreButton, 0);
    sidebarButtons->addButton(localMusicButton, 1);
    sidebarLayout->addWidget(exploreButton);
    sidebarLayout->addWidget(localMusicButton);

}

void MainWidget::initRight()
{

    rightLayout = new QVBoxLayout();
    rightWidget = new QWidget(this);
    contentLayout->addWidget(rightWidget);
    rightWidget->setLayout(rightLayout);
    funcWidget = new QStackedWidget();
    playerWidget = new PlayerWidget();
    rightLayout->addWidget(funcWidget, 1);
    rightLayout->addWidget(playerWidget);
    this->setStyleSheet("border: 2px solid blue;");
}

MainWidget::~MainWidget() {}
