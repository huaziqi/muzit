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
    this->resize(840, 600);
}

QPushButton* MainWidget::createButton(const QString& name){
    QPushButton* button = new QPushButton(name);
    button->setFixedSize(160, 50);
    button->setFont(*sideBarFont);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sidebarLayout->addWidget(button);
    stackButtonGroup->addButton(button, curStackButtonID ++);
    button->setCheckable(true);
    connect(button, &QPushButton::clicked, this, &MainWidget::onStackButtonClicked);
    return button;
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
//设置sideTitle
    sideTitle = new QLabel(this);
    sideTitle->setAlignment(Qt::AlignCenter);
    sideTitle->setText("Muzit");
    sideTitle->setFont(*sideBarFont);
    sidebarLayout->addWidget(sideTitle);
//设置sidebar的按钮
    stackButtonGroup = new QButtonGroup();
    exploreButton = createButton("探索");
    downloadMusicButton = createButton("下载");
    localMusicButton = createButton("本地");

    exploreButton->setChecked(true);
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

    downloadWidget = new DownloadWidget();
    qDebug() << "rest";
    exploreWidget = new ExploreWidget();
    localWidget = new LocalWidget();

//添加到StackedWidget
    funcWidget->addWidget(exploreWidget);
    funcWidget->addWidget(downloadWidget);
    funcWidget->addWidget(localWidget);
    funcWidget->setCurrentIndex(0);

//设置funcArea
    funcArea = new QScrollArea();
    funcArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    funcArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    funcArea->setWidget(funcWidget);
    funcWidget->setFixedWidth(funcArea->viewport()->width());
    funcArea->setWidgetResizable(true); //
    funcArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    rightLayout->addWidget(funcArea);

//添加播放器
    playerWidget = new PlayerWidget();
    rightLayout->addWidget(playerWidget);
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    funcWidget->setFixedWidth(funcArea->viewport()->width());
}

void MainWidget::onStackButtonClicked()
{
    int stackId = stackButtonGroup->checkedId();
    funcWidget->setCurrentIndex(stackId);
}

MainWidget::~MainWidget() {}
