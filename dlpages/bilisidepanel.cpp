#include "bilisidepanel.h"
#include <QFileDialog>
#include <QSet>
#include <QStandardItemModel>
#include "download/downloadtask.h"

BiliSidePanel::BiliSidePanel(QWidget *parent)
    : QWidget{parent}
{
    setFixedWidth(200);
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 10, 8, 8);
    mainLayout->setSpacing(8);
    mainLayout->setAlignment(Qt::AlignTop);

    initSaveSettings();
    mainLayout->addWidget(makeSeparator());
    initFavorites();
    mainLayout->addWidget(makeSeparator());
    initDownloadQueue();
    mainLayout->addStretch(1);

    connect(this, &BiliSidePanel::saveSettingsChanged,
            this, &BiliSidePanel::onSettingsChanged);
}

QFrame *BiliSidePanel::makeSeparator()
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

void BiliSidePanel::initSaveSettings()
{
//默认设置
    defaultPath =  QDir::homePath() + "/Music";

    QLabel *title = new QLabel("保存设置");
    title->setStyleSheet("font-weight: bold; font-size: 11px;");
    mainLayout->addWidget(title);

    // 路径行
    QLabel *pathTitle = new QLabel("保存路径");
    pathTitle->setStyleSheet("font-size: 11px; color: #666;");
    mainLayout->addWidget(pathTitle);

    QHBoxLayout *pathRow = new QHBoxLayout();
    pathRow->setSpacing(4);
    pathLabel = new QLabel();
    pathLabel->setStyleSheet("font-size: 10px; color: #888;");
    pathLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    browseBtn = new QPushButton("…");
    browseBtn->setFixedSize(22, 22);
    connect(browseBtn, &QPushButton::clicked, this, &BiliSidePanel::onBrowseClicked);
    pathRow->addWidget(pathLabel, 1);
    pathRow->addWidget(browseBtn);
    mainLayout->addLayout(pathRow);

    // 文件名模板
    QLabel *tmplTitle = new QLabel("文件名模板");
    tmplTitle->setStyleSheet("font-size: 11px; color: #666;");
    mainLayout->addWidget(tmplTitle);
    templateInput = new QLineEdit();
    templateInput->setFixedHeight(24);
    templateInput->setStyleSheet("font-size: 11px;");
    mainLayout->addWidget(templateInput);
    connect(templateInput, &QLineEdit::textChanged, this, [this]() {
        emit saveSettingsChanged(currentSettings());
    });

    // 音质
    QLabel *qualTitle = new QLabel("音质");
    qualTitle->setStyleSheet("font-size: 11px; color: #666;");
    mainLayout->addWidget(qualTitle);
    qualityBox = new QComboBox();
    qualityBox->addItem("Hi-Res 无损", 30251);
    qualityBox->addItem("杜比全景声", 30250);
    qualityBox->addItem("192K", 30280);
    qualityBox->addItem("132K", 30232);
    qualityBox->addItem("64K", 30216);
    qualityBox->setFixedHeight(24);
    mainLayout->addWidget(qualityBox);
    connect(qualityBox, &QComboBox::currentIndexChanged, this, [this]() {
        emit saveSettingsChanged(currentSettings());
    });

    initConfig();
    updateAvailableQualities(BiliVideoInfo{});
}

void BiliSidePanel::initConfig(){
    pathLabel->setText(Config::instance().getValuewithGroup(configGroup, BiliSaveSettings::SavePathKey, defaultPath).toString());
    templateInput->setText(Config::instance().getValuewithGroup(configGroup, BiliSaveSettings::FileNameTemplateKey, defaultTemplate).toString());
    QString quality = Config::instance().getValuewithGroup(configGroup, BiliSaveSettings::QualityKey, defaultQaulity).toString();
    int idx = qualityBox->findData(quality.toInt());
    if(idx >= 0){
        qualityBox->setCurrentIndex(idx);
    }
    else{
        qualityBox->setCurrentIndex(0);
    }
}

void BiliSidePanel::initFavorites()
{
    QLabel *title = new QLabel("收藏");
    title->setStyleSheet("font-weight: bold; font-size: 11px;");
    mainLayout->addWidget(title);

    selectedSongLabel = new QLabel("未选中歌曲");
    selectedSongLabel->setStyleSheet("font-size: 10px; color: #999;");
    selectedSongLabel->setWordWrap(true);
    mainLayout->addWidget(selectedSongLabel);

    favoriteBtn = new QPushButton("★ 添加到收藏");
    favoriteBtn->setFixedHeight(26);
    favoriteBtn->setEnabled(false);
    connect(favoriteBtn, &QPushButton::clicked, this, [this]() {
        emit favoriteRequested(currentInfo);
    });
    mainLayout->addWidget(favoriteBtn);
}

void BiliSidePanel::initDownloadQueue()
{
    QLabel *title = new QLabel("下载队列");
    title->setStyleSheet("font-weight: bold; font-size: 11px;");
    mainLayout->addWidget(title);

    queueWidget = new QWidget();
    queueLayout = new QVBoxLayout(queueWidget);
    queueLayout->setContentsMargins(0, 0, 0, 0);
    queueLayout->setSpacing(4);
    queueLayout->setAlignment(Qt::AlignTop);

    queueArea = new QScrollArea();
    queueArea->setWidget(queueWidget);
    queueArea->setWidgetResizable(true);
    queueArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    queueArea->setFixedHeight(120);
    mainLayout->addWidget(queueArea);
}

void BiliSidePanel::setSelectedSong(const BiliVideoInfo &info)
{
    currentInfo = info;
    selectedSongLabel->setText(info.title);
    selectedSongLabel->setStyleSheet("font-size: 10px; color: #333;");
    favoriteBtn->setEnabled(true);
    updateAvailableQualities(info);
}

void BiliSidePanel::updateAvailableQualities(const BiliVideoInfo &info)
{
    QSet<int> commonQualities;
    bool hasSelectedPart = false;
    bool firstSelectedPart = true;

    for (const BiliPlayUrlInfo &part : info.parts) {
        if (!part.selected)
            continue;

        hasSelectedPart = true;
        QSet<int> partQualities;
        for (const BiliAudioStream &audio : part.audioStreams)
            partQualities.insert(audio.id);

        if (firstSelectedPart) {
            commonQualities = partQualities;
            firstSelectedPart = false;
        } else {
            commonQualities.intersect(partQualities);
        }
    }

    QStandardItemModel *model =
        qobject_cast<QStandardItemModel *>(qualityBox->model());
    int firstEnabledIndex = -1;
    for (int i = 0; i < qualityBox->count(); ++i) {
        const bool enabled =
            hasSelectedPart && commonQualities.contains(qualityBox->itemData(i).toInt());
        if (QStandardItem *item = model ? model->item(i) : nullptr)
            item->setEnabled(enabled);
        if (enabled && firstEnabledIndex < 0)
            firstEnabledIndex = i;
    }

    qualityBox->setEnabled(firstEnabledIndex >= 0);
    if (firstEnabledIndex >= 0) {
        const int currentId = qualityBox->currentData().toInt();
        if (!commonQualities.contains(currentId))
            qualityBox->setCurrentIndex(firstEnabledIndex);
    }
}

BiliSaveSettings BiliSidePanel::currentSettings() const
{
    return BiliSaveSettings{
        pathLabel->text(),
        templateInput->text(),
        QString::number(qualityBox->currentData().toInt())
    };
}

void BiliSidePanel::addDownloadTask(const QString &title, DownloadTask *task)
{
    QWidget *taskRow = new QWidget();
    QVBoxLayout *rowLayout = new QVBoxLayout(taskRow);
    rowLayout->setContentsMargins(2, 2, 2, 2);
    rowLayout->setSpacing(2);

    QLabel *taskLabel = new QLabel(title);
    taskLabel->setStyleSheet("font-size: 10px;");
    taskLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QProgressBar *bar = new QProgressBar();
    bar->setRange(0, 100);
    bar->setValue(0);
    bar->setFixedHeight(6);
    bar->setTextVisible(false);

    rowLayout->addWidget(taskLabel);
    rowLayout->addWidget(bar);
    queueLayout->addWidget(taskRow);

    connect(task, &DownloadTask::progress, taskRow,
            [bar](qint64 received, qint64 total) {
        if (total > 0)
            bar->setValue(static_cast<int>(received * 100 / total));
    });
    connect(task, &DownloadTask::finished, taskRow, [taskLabel, bar] {
        bar->setValue(100);
        taskLabel->setText(taskLabel->text() + QStringLiteral("  已完成"));
    });
    connect(task, &DownloadTask::failed, taskRow,
            [taskLabel](const QString &error) {
        taskLabel->setText(
            taskLabel->text() + QStringLiteral("  失败：") + error);
    });
}

void BiliSidePanel::onBrowseClicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this, "选择保存目录", pathLabel->text());
    if (!dir.isEmpty()) {
        pathLabel->setText(dir);
        emit saveSettingsChanged(currentSettings());
    }
}



void BiliSidePanel::onSettingsChanged(const BiliSaveSettings& settings){
    Config::instance().setValuewithGroup(configGroup, settings.SavePathKey, settings.savePath);
    Config::instance().setValuewithGroup(configGroup, settings.FileNameTemplateKey, settings.fileNameTemplate);
    Config::instance().setValuewithGroup(configGroup, settings.QualityKey, settings.quality);
}
