#include "mainwidget.h"

#define PADDING 5

MainWidget::MainWidget(QWidget *parent)
    : FramelessWidget(parent){
    Config config;
    config.readConfig();
    common::loadFont(":/fonts/resources/font/VonwaonBitmap.ttf");
    titleBar->setTitle("Muzit");
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    this->setMinimumSize(QSize(200, 150));
    this->resize(800, 600);
    sideBarFont = new QFont("VonwaonBitmap 16px", 16);
    initSidebar();
    initRight();


}



void MainWidget::initSidebar()
{
    sidebarWidget = new QWidget(this);
    sidebarLayout = new QVBoxLayout();
    sidebarWidget->setLayout(sidebarLayout);
    sidebarWidget->setFixedWidth(200);

    contentLayout->addWidget(sidebarWidget);
    contentLayout->setAlignment(Qt::AlignLeft);
    sideTitle = new QLabel(this);
    sideTitle->setAlignment(Qt::AlignCenter);
    sidebarLayout->setAlignment(Qt::AlignTop);
    sideTitle->setText("Muzit");
    sideTitle->setFont(*sideBarFont);
    sidebarLayout->addWidget(sideTitle);

    sidebarButtons = new QButtonGroup();
    exploreButton = new QPushButton("探索音乐");
    localMusicButton = new QPushButton("本地音乐");
    exploreButton->setFixedSize(160, 50);
    localMusicButton->setFixedSize(160, 50);



    exploreButton->setFont(*sideBarFont);
    localMusicButton->setFont(*sideBarFont);
    exploreButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    localMusicButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    sidebarButtons->addButton(exploreButton, 0);
    sidebarButtons->addButton(localMusicButton, 1);
    sidebarLayout->addWidget(exploreButton);
    sidebarLayout->addWidget(localMusicButton);

    sidebarLayout->addStretch(1);
}



void MainWidget::initRight()
{
    rightLayout = new QVBoxLayout();
    rightWidget = new QWidget(this);
    contentLayout->addWidget(rightWidget);
    rightWidget->setLayout(rightLayout);
    funcWidget = new QStackedWidget();
    exploreWidget = new ExploreWidget();
    funcWidget->addWidget(exploreWidget);
    localWidget = new LocalWidget();
    funcWidget->addWidget(localWidget);
    funcWidget->setCurrentWidget(0);
    playerWidget = new PlayerWidget();
    rightLayout->addWidget(funcWidget, 1);
    rightLayout->addWidget(playerWidget);
}

void MainWidget::resizeEvent(QResizeEvent *event)
{


}

MainWidget::~MainWidget() {}
