#include "mainwidget.h"

#define PADDING 5

MainWidget::MainWidget(QWidget *parent)
    : FramelessWidget(parent){

    Config config;
    config.readConfig();
    common::loadFont(":/fonts/resources/font/VonwaonBitmap.ttf");
    titleBar->setTitle("Muzit");
    this->setMinimumSize(QSize(800, 600));

    sideBarFont = new QFont("VonwaonBitmap 16px", 16);
    initSidebar();
    initRight();
    this->resize(800, 600);
}


void MainWidget::initSidebar()
{
    sidebarWidget = new QWidget(this);
    sidebarWidget->setStyleSheet("background-color: #e0e0e0");
    sidebarWidget->setFixedWidth(200);
    contentLayout->addWidget(sidebarWidget);
    sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(2, 20, 2, 0);
    sidebarLayout->setSpacing(10);
    sidebarLayout->setAlignment(Qt::AlignTop);
    sidebarLayout->setAlignment(Qt::AlignHCenter);


    sideTitle = new QLabel(this);
    sideTitle->setAlignment(Qt::AlignCenter);
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
    exploreButton->setChecked(true);
    localMusicButton->setCheckable(true);

    connect(exploreButton, &QPushButton::clicked, this, &MainWidget::onStackButtonClicked);
    connect(localMusicButton, &QPushButton::clicked, this, &MainWidget::onStackButtonClicked);

    sidebarLayout->addStretch(1);
}

void MainWidget::initRight()
{
//布局设置
    rightLayout = new QVBoxLayout();
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);
//widget设置
    rightWidget = new QWidget(this);
    contentLayout->addWidget(rightWidget);
    rightWidget->setLayout(rightLayout);
//初始化widget
    funcWidget = new QStackedWidget();
    exploreWidget = new ExploreWidget();
    localWidget = new LocalWidget();

//添加到StackedWidget
    funcWidget->addWidget(exploreWidget);
    funcWidget->addWidget(localWidget);
    funcWidget->setCurrentIndex(0);


//设置funcArea

    funcArea = new QScrollArea();
    funcArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    funcArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    funcArea->setWidget(funcWidget);
    funcWidget->setFixedWidth(funcArea->width());
    funcArea->setWidgetResizable(true); //
    funcArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightLayout->addWidget(funcArea);

//添加播放器
    playerWidget = new PlayerWidget();
    rightLayout->addWidget(playerWidget);
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    funcWidget->setFixedWidth(funcArea->width());
}

void MainWidget::onStackButtonClicked()
{
    int stackId = stackButtonGroup->checkedId();
    funcWidget->setCurrentIndex(stackId);
}

MainWidget::~MainWidget() {}
