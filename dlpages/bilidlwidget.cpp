#include "bilidlwidget.h"

BiliDLWidget::BiliDLWidget(DownloadManager *_downloadManager, QWidget *parent)
    : QWidget{parent}, downloadManager(_downloadManager)
{
    biliDLTool = new BiliDLTool(downloadManager);

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
    connect(biliDLTool, &BiliDLTool::videoInfoReady,
            this, [](const BiliVideoInfo &info) {
        qDebug() << info.title << info.author << info.parts.size();
    });

    // 加载演示数据
    loadDemoData();
}

void BiliDLWidget::loadDemoData()
{
    QVector<BiliVideoInfo> demo;
    BiliVideoInfo first;
    first.bvid = "BV1xx411x7aa";
    first.title = "青花瓷 翻唱版 - 完整版本";
    first.author = "某某某";
    first.durationMilliseconds = 222000;
    first.description = "高质量翻唱";
    first.playCount = 124000;
    first.parts.append({1, 1, first.title, first.durationMilliseconds});
    demo.append(first);

    BiliVideoInfo second;
    second.bvid = "BV1xx411x7bb";
    second.title = "晴天 - 周杰伦（钢琴版）";
    second.author = "钢琴君";
    second.durationMilliseconds = 255000;
    second.description = "钢琴改编，高品质录制";
    second.playCount = 886000;
    second.parts.append({2, 1, second.title, second.durationMilliseconds});
    demo.append(second);

    BiliVideoInfo third;
    third.bvid = "BV1xx411x7cc";
    third.title = "稻香 官方MV 完整版";
    third.author = "Jay Official";
    third.durationMilliseconds = 238000;
    third.playCount = 2340000;
    third.parts.append({3, 1, third.title, third.durationMilliseconds});
    demo.append(third);
    resultList->setResults(demo);
}

void BiliDLWidget::onSearchRequested(const QString &keyword, BiliSearchType type, int pageSize)
{
    Q_UNUSED(type); Q_UNUSED(pageSize);
    if(type == BiliSearchType::BvId){

        biliDLTool->getVideoInfo(keyword);
        connect(biliDLTool, &BiliDLTool::videoInfoReady, this, [=](const BiliVideoInfo& info){
            biliDLTool->getPartAudioStreams(info, info.parts[0].cid);
            connect(biliDLTool, &BiliDLTool::partAudioStreamsReady, this, [=](const BiliVideoInfo& info){
                qDebug() << info.parts[0].audioStreams[0].url;
            });
        });
    }
}

void BiliDLWidget::onItemSelected(const BiliVideoInfo &info)
{
    sidePanel->setSelectedSong(info);
}

void BiliDLWidget::onDownloadRequested(const BiliVideoInfo &info)
{
    // 占位：后续接入真实下载
    sidePanel->addDownloadTask(info.title);
    qDebug() << "[BiliDLWidget] download:" << info.bvid;
}

void BiliDLWidget::onFavoriteRequested(const BiliVideoInfo &info)
{
    // 占位：后续接入收藏逻辑
    qDebug() << "[BiliDLWidget] favorite:" << info.bvid;
}

void onSettingsChanged(BiliSaveSettings settings){

}
