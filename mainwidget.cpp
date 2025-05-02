#include "mainwidget.h"

#define PADDING 5

MainWidget::MainWidget(QWidget *parent)
    : FramelessWidget(parent){
    Config config;
    config.readConfig();
    common::loadFont(":/fonts/resources/font/VonwaonBitmap.ttf");
    titleBar->setTitle("Muzit");
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    this->setMinimumSize(QSize(400, 300));
    this->resize(800, 600);
    sideBarFont = new QFont("VonwaonBitmap 16px", 16);
    initSidebar();
    initRight();
}



void MainWidget::initSidebar()
{
    sidebarWidget = new QWidget(this);
    sidebarWidget->setStyleSheet("background-color: #e0e0e0");
    sidebarLayout = new QVBoxLayout();
    sidebarLayout->setContentsMargins(2, 20, 2, 0);
    sidebarLayout->setSpacing(10);
    sidebarWidget->setLayout(sidebarLayout);
    sidebarWidget->setFixedWidth(200);

    contentLayout->addWidget(sidebarWidget);
    sideTitle = new QLabel(this);
    sideTitle->setAlignment(Qt::AlignCenter);
    sidebarLayout->setAlignment(Qt::AlignTop);
    sidebarLayout->setAlignment(Qt::AlignHCenter);
    sideTitle->setText("Muzit");
    sideTitle->setFont(*sideBarFont);
    sidebarLayout->addWidget(sideTitle);

    stackButtonGroup = new QButtonGroup();
    exploreButton = new QPushButton("探索音乐");
    localMusicButton = new QPushButton("本地音乐");
    exploreButton->setFixedSize(160, 50);
    localMusicButton->setFixedSize(160, 50);

    exploreButton->setFont(*sideBarFont);
    localMusicButton->setFont(*sideBarFont);
    exploreButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    localMusicButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    sidebarLayout->addWidget(exploreButton);
    sidebarLayout->addWidget(localMusicButton);
    stackButtonGroup->addButton(exploreButton, 0);
    stackButtonGroup->addButton(localMusicButton, 1);
    exploreButton->setCheckable(true);
    localMusicButton->setCheckable(true);

    connect(exploreButton, &QPushButton::clicked, this, &MainWidget::onStackButtonClicked);
    connect(localMusicButton, &QPushButton::clicked, this, &MainWidget::onStackButtonClicked);

    sidebarLayout->addStretch(1);
}



void MainWidget::initRight()
{
    rightLayout = new QVBoxLayout();
    funcArea = new QScrollArea();
    rightLayout->addWidget(funcArea);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);
    rightWidget = new QWidget(this);
    contentLayout->addWidget(rightWidget);
    rightWidget->setLayout(rightLayout);


    funcWidget = new QStackedWidget();
    exploreWidget = new ExploreWidget();
    funcWidget->addWidget(exploreWidget);
    localWidget = new LocalWidget();
    funcWidget->addWidget(localWidget);
    funcWidget->setCurrentIndex(0);
    exploreButton->setChecked(true);
    playerWidget = new PlayerWidget();
    rightLayout->addWidget(funcArea);
    funcArea->setWidget(funcWidget);
    rightLayout->addWidget(playerWidget);
}

void MainWidget::resizeEvent(QResizeEvent *event)
{


}

void MainWidget::onStackButtonClicked()
{
    int stackId = stackButtonGroup->checkedId();
    funcWidget->setCurrentIndex(stackId);
}

MainWidget::~MainWidget() {}
