#include "bilidlwidget.h"
#include "biliaudiometadatadialog.h"

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
    QVector<int> selectedPartIndexes;
    for (qsizetype i = 0; i < info.parts.size(); ++i) {
        if (info.parts.at(i).selected)
            selectedPartIndexes.append(static_cast<int>(i));
    }
    if (selectedPartIndexes.isEmpty())
        return;

    BiliAudioMetadataDialog dialog(info, selectedPartIndexes, this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    const QVector<AudioMetadata> metadata = dialog.metadata();

    for (qsizetype i = 0; i < selectedPartIndexes.size(); ++i) {
        const BiliPlayUrlInfo &part =
            info.parts.at(selectedPartIndexes.at(i));
        AudioDownloadJob job;
        QString error;
        if (!biliDLTool->createAudioDownloadJob(
                info, part, settings, metadata.at(i), job, error)) {
            qWarning() << "Failed to prepare audio download for CID"
                       << part.cid << error;
            continue;
        }

        DownloadTask *task = biliDLTool->downloadAudio(job);
        sidePanel->addDownloadTask(
            QStringLiteral("P%1 %2 · %3 · %4")
                .arg(part.page)
                .arg(job.metadata.title)
                .arg(job.metadata.artist)
                .arg(job.source.qualityDescription),
            task);
    }
}

void BiliDLWidget::onFavoriteRequested(const BiliVideoInfo &info)
{
    // 占位：后续接入收藏逻辑
    qDebug() << "[BiliDLWidget] favorite:" << info.bvid;
}
