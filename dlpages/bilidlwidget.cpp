#include "bilidlwidget.h"
#include <algorithm>
#include <QRegularExpression>

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
    connect(resultList, &BiliResultList::partsSelectionChanged,
            this, &BiliDLWidget::onPartsSelectionChanged);
    connect(biliDLTool, &BiliDLTool::videoInfoReady,
            this, &BiliDLWidget::onVideoInfoReady);
    connect(biliDLTool, &BiliDLTool::partAudioStreamsReady,
            this, &BiliDLWidget::onPartAudioStreamsReady);
    connect(biliDLTool, &BiliDLTool::partAudioStreamsFailed,
            this, &BiliDLWidget::onPartAudioStreamsFailed);

    // 加载演示数据
    //loadDemoData();
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
    BiliPlayUrlInfo firstPart;
    firstPart.cid = 1;
    firstPart.page = 1;
    firstPart.title = first.title;
    firstPart.durationMilliseconds = first.durationMilliseconds;
    first.parts.append(firstPart);
    demo.append(first);

    BiliVideoInfo second;
    second.bvid = "BV1xx411x7bb";
    second.title = "晴天 - 周杰伦（钢琴版）";
    second.author = "钢琴君";
    second.durationMilliseconds = 255000;
    second.description = "钢琴改编，高品质录制";
    second.playCount = 886000;
    BiliPlayUrlInfo secondPart;
    secondPart.cid = 2;
    secondPart.page = 1;
    secondPart.title = second.title;
    secondPart.durationMilliseconds = second.durationMilliseconds;
    second.parts.append(secondPart);
    demo.append(second);

    BiliVideoInfo third;
    third.bvid = "BV1xx411x7cc";
    third.title = "稻香 官方MV 完整版";
    third.author = "Jay Official";
    third.durationMilliseconds = 238000;
    third.playCount = 2340000;
    BiliPlayUrlInfo thirdPart;
    thirdPart.cid = 3;
    thirdPart.page = 1;
    thirdPart.title = third.title;
    thirdPart.durationMilliseconds = third.durationMilliseconds;
    third.parts.append(thirdPart);
    demo.append(third);
    resultList->setResults(demo);
}

void BiliDLWidget::onSearchRequested(const QString &keyword, BiliSearchType type, int pageSize)
{
    curPageSize = pageSize;
    Q_UNUSED(type); Q_UNUSED(pageSize);
    if(type == BiliSearchType::BvId){
        biliDLTool->getVideoInfo(keyword);
    }
}

void BiliDLWidget::onVideoInfoReady(const BiliVideoInfo &info)
{
    searchBar->searchFinished();
    resultList->setResults({info});
    loadSelectedPartAudio(info);
}

void BiliDLWidget::onItemSelected(const BiliVideoInfo &info)
{
    sidePanel->setSelectedSong(info);
}

void BiliDLWidget::onPartsSelectionChanged(const BiliVideoInfo &info)
{
    resultList->updateVideoInfo(info);
    sidePanel->setSelectedSong(info);
    loadSelectedPartAudio(info);
}

void BiliDLWidget::loadSelectedPartAudio(BiliVideoInfo info)
{
    if (audioRequestInProgress) {
        queuedAudioInfo.append(info);
        hasQueuedAudioInfo = true;
        return;
    }

    pendingAudioInfo = info;
    pendingAudioIndex.clear();
    int i = 0;
    for (const BiliPlayUrlInfo &part : pendingAudioInfo.parts) {
        if (part.selected && part.audioStreams.isEmpty())
            pendingAudioIndex.append(i);
        i ++;
    }
    if (pendingAudioIndex.isEmpty()) {
        return;
    }
    audioRequestInProgress = true;
    requestNextPartAudio();
}

void BiliDLWidget::requestNextPartAudio()
{
    if (pendingAudioIndex.isEmpty()) {
        finishPartAudioLoading();
        return;
    }

    const qint64 index = pendingAudioIndex.takeFirst();
    biliDLTool->getPartAudioStreams(pendingAudioInfo.parts[index], pendingAudioInfo.bvid, index);
}

void BiliDLWidget::onPartAudioStreamsReady(const BiliPlayUrlInfo &info, qint64 index)
{
    pendingAudioInfo.parts[index] = info;
    requestNextPartAudio();
}

void BiliDLWidget::onPartAudioStreamsFailed(qint64 cid, const QString &error)
{
    qWarning() << "Failed to load audio streams for CID" << cid << error;
    requestNextPartAudio();
}

void BiliDLWidget::finishPartAudioLoading()
{
    audioRequestInProgress = false;
    resultList->updateVideoInfo(pendingAudioInfo);

    if (!hasQueuedAudioInfo)
        return;

    BiliVideoInfo nextInfo = queuedAudioInfo.takeFirst();
    if(queuedAudioInfo.isEmpty())
        hasQueuedAudioInfo = false;

    loadSelectedPartAudio(nextInfo);
}

void BiliDLWidget::onDownloadRequested(const BiliVideoInfo &info)
{
    const BiliSaveSettings settings = sidePanel->currentSettings();
    const int qualityId = settings.quality.toInt();
    const int selectedPartCount = std::count_if(
        info.parts.cbegin(), info.parts.cend(),
        [](const BiliPlayUrlInfo &part) { return part.selected; });

    for (const BiliPlayUrlInfo &part : info.parts) {
        if (!part.selected)
            continue;

        const auto audioIt = std::find_if(
            part.audioStreams.cbegin(), part.audioStreams.cend(),
            [qualityId](const BiliAudioStream &audio) {
                return audio.id == qualityId;
            });
        if (audioIt == part.audioStreams.cend()) {
            qWarning() << "Selected quality is unavailable for CID" << part.cid;
            continue;
        }

        QString fileName = settings.fileNameTemplate;
        fileName.replace("{title}", info.title);
        fileName.replace("{author}", info.author);
        if (selectedPartCount > 1 || info.parts.size() > 1) {
            fileName += QStringLiteral(" - P%1 %2")
                            .arg(part.page)
                            .arg(part.title);
        }
        fileName.replace(QRegularExpression(R"([<>:"/\\|?*])"), "_");
        fileName = fileName.trimmed();
        if (fileName.isEmpty())
            fileName = info.bvid + QStringLiteral("-P%1").arg(part.page);

        const QString savePath =
            QDir(settings.savePath).filePath(fileName + ".m4s");
        DownloadTask *task = biliDLTool->downloadAudio(audioIt->url, savePath);
        sidePanel->addDownloadTask(
            QStringLiteral("P%1 %2 · %3")
                .arg(part.page)
                .arg(part.title)
                .arg(audioIt->qualityDescription),
            task);
    }
}

void BiliDLWidget::onFavoriteRequested(const BiliVideoInfo &info)
{
    // 占位：后续接入收藏逻辑
    qDebug() << "[BiliDLWidget] favorite:" << info.bvid;
}
