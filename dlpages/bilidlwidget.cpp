#include "bilidlwidget.h"

BiliDLWidget::BiliDLWidget(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 搜索栏
    searchBar = new BiliSearchBar();
    searchBar->setStyleSheet("background: #e8e8e2; border-bottom: 1px solid #ccc;");
    mainLayout->addWidget(searchBar);

    // 主体：结果列表 + 右侧面板
    QHBoxLayout *bodyLayout = new QHBoxLayout();
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    resultList = new BiliResultList();
    sidePanel  = new BiliSidePanel();
    sidePanel->setStyleSheet("background: #e8e6e0; border-left: 1px solid #ccc;");

    bodyLayout->addWidget(resultList, 1);
    bodyLayout->addWidget(sidePanel);
    mainLayout->addLayout(bodyLayout, 1);

    // 信号连接
    connect(searchBar,  &BiliSearchBar::searchRequested,
            this,       &BiliDLWidget::onSearchRequested);
    connect(resultList, &BiliResultList::itemSelected,
            this,       &BiliDLWidget::onItemSelected);
    connect(resultList, &BiliResultList::downloadRequested,
            this,       &BiliDLWidget::onDownloadRequested);
    connect(resultList, &BiliResultList::favoriteRequested,
            this,       &BiliDLWidget::onFavoriteRequested);

    // 加载演示数据
    loadDemoData();
}

void BiliDLWidget::loadDemoData()
{
    QVector<BiliVideoInfo> demo;
    demo.append({"BV1xx411x7aa", "青花瓷 翻唱版 - 完整版本", "某某某",
                 "", 222, 1, "高质量翻唱", 124000});
    demo.append({"BV1xx411x7bb", "晴天 - 周杰伦（钢琴版）", "钢琴君",
                 "", 255, 1, "钢琴改编，高品质录制", 886000});
    demo.append({"BV1xx411x7cc", "稻香 官方MV 完整版", "Jay Official",
                 "", 238, 1, "", 2340000});
    resultList->setResults(demo);
}

void BiliDLWidget::onSearchRequested(const QString &keyword, BiliSearchType type, int pageSize)
{
    Q_UNUSED(type); Q_UNUSED(pageSize);
    // 占位：后续接入真实 API
    qDebug() << "[BiliDLWidget] search:" << keyword;
}

void BiliDLWidget::onItemSelected(const BiliVideoInfo &info)
{
    sidePanel->setSelectedSong(info);
}

void BiliDLWidget::onDownloadRequested(const BiliVideoInfo &info)
{
    // 占位：后续接入真实下载
    sidePanel->addDownloadTask(info.title);
    qDebug() << "[BiliDLWidget] download:" << info.bvId;
}

void BiliDLWidget::onFavoriteRequested(const BiliVideoInfo &info)
{
    // 占位：后续接入收藏逻辑
    qDebug() << "[BiliDLWidget] favorite:" << info.bvId;
}
