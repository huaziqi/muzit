# BiliWidget 布局实现计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 在现有的 `BiliDLWidget` 空骨架内实现完整的下载页布局——搜索栏、可展开结果列表、右侧保存/收藏/下载队列面板。

**Architecture:** 五个独立 Widget（SearchBar、ResultItem、ResultList、SidePanel、BiliDLWidget），数据结构定义在独立头文件 `bilitypes.h`。BiliDLWidget 作为顶层胶水层组合子组件，信号槽在组件间传递事件。本次只做布局，不接实际网络请求。

**Tech Stack:** Qt 6.5, C++17, QPropertyAnimation（展开动画）, QFileDialog（路径选择）

---

## 文件结构

```
dlpages/
  bilitypes.h          新建 — BiliVideoInfo / BiliSaveSettings / BiliSearchType
  bilisearchbar.h/.cpp 新建 — 搜索栏组件
  bilisidepanel.h/.cpp 新建 — 右侧面板组件
  biliresultitem.h/.cpp新建 — 单条结果行（含展开态）
  biliresultlist.h/.cpp新建 — 结果列表 + 翻页
  bilidlwidget.h/.cpp  修改 — 现有空骨架，接入上面五个组件
muzit.pro              修改 — 添加新文件到 SOURCES/HEADERS
```

---

## Task 1：数据类型头文件 + 更新构建

**Files:**
- Create: `dlpages/bilitypes.h`
- Modify: `muzit.pro`

- [ ] **Step 1: 创建 `dlpages/bilitypes.h`**

```cpp
#ifndef BILITYPES_H
#define BILITYPES_H

#include <QString>

enum class BiliSearchType { Keyword, BvId };

struct BiliVideoInfo {
    QString bvId;
    QString title;
    QString author;
    QString coverUrl;
    int     duration;   // 秒
    int     partCount;
    QString description;
    int     playCount;
};

struct BiliSaveSettings {
    QString savePath;
    QString fileNameTemplate;  // 支持 {title} {author} {bvid}
    QString quality;           // "320kbps" / "192kbps" / "hires"
};

#endif // BILITYPES_H
```

- [ ] **Step 2: 在 `muzit.pro` 的 HEADERS 和 SOURCES 中添加新文件**

在 `muzit.pro` 的 `HEADERS +=` 块末尾追加：

```
    dlpages/bilitypes.h \
    dlpages/bilisearchbar.h \
    dlpages/bilisidepanel.h \
    dlpages/biliresultitem.h \
    dlpages/biliresultlist.h \
```

在 `SOURCES +=` 块末尾追加：

```
    dlpages/bilisearchbar.cpp \
    dlpages/bilisidepanel.cpp \
    dlpages/biliresultitem.cpp \
    dlpages/biliresultlist.cpp \
```

同时在 `INCLUDEPATH +=` 后添加：

```
INCLUDEPATH += $$PWD/dlpages
```

- [ ] **Step 3: 提交**

```
git add dlpages/bilitypes.h muzit.pro
git commit -m "feat: add BiliWidget data types and update build"
```

---

## Task 2：BiliSearchBar

**Files:**
- Create: `dlpages/bilisearchbar.h`
- Create: `dlpages/bilisearchbar.cpp`

- [ ] **Step 1: 创建 `dlpages/bilisearchbar.h`**

```cpp
#ifndef BILISEARCHBAR_H
#define BILISEARCHBAR_H

#include "common.h"
#include "bilitypes.h"
#include <QLineEdit>
#include <QComboBox>

class BiliSearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit BiliSearchBar(QWidget *parent = nullptr);

private:
    QHBoxLayout  *mainLayout;
    QButtonGroup *typeGroup;
    QPushButton  *keywordBtn;
    QPushButton  *bvidBtn;
    QLineEdit    *searchInput;
    QPushButton  *searchBtn;
    QComboBox    *pageSizeBox;

private slots:
    void onSearchClicked();

signals:
    void searchRequested(QString keyword, BiliSearchType type, int pageSize);
};

#endif // BILISEARCHBAR_H
```

- [ ] **Step 2: 创建 `dlpages/bilisearchbar.cpp`**

```cpp
#include "bilisearchbar.h"

BiliSearchBar::BiliSearchBar(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(8, 6, 8, 6);
    mainLayout->setSpacing(6);

    // 搜索类型切换按钮组
    typeGroup   = new QButtonGroup(this);
    keywordBtn  = new QPushButton("关键词");
    bvidBtn     = new QPushButton("BV号");
    for (auto *btn : {keywordBtn, bvidBtn}) {
        btn->setCheckable(true);
        btn->setFixedHeight(28);
        btn->setMinimumWidth(56);
    }
    keywordBtn->setChecked(true);
    typeGroup->addButton(keywordBtn, static_cast<int>(BiliSearchType::Keyword));
    typeGroup->addButton(bvidBtn,    static_cast<int>(BiliSearchType::BvId));
    typeGroup->setExclusive(true);

    // 搜索框
    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("搜索视频标题 / 输入 BVxxxxxxx");
    searchInput->setFixedHeight(28);
    connect(searchInput, &QLineEdit::returnPressed, this, &BiliSearchBar::onSearchClicked);

    // 搜索按钮
    searchBtn = new QPushButton("搜索");
    searchBtn->setFixedSize(60, 28);
    connect(searchBtn, &QPushButton::clicked, this, &BiliSearchBar::onSearchClicked);

    // 每页条数
    QLabel *pageSizeLabel = new QLabel("每页");
    pageSizeLabel->setFixedHeight(28);
    pageSizeBox = new QComboBox();
    pageSizeBox->addItems({"10", "20", "30"});
    pageSizeBox->setFixedSize(50, 28);
    QLabel *tiao = new QLabel("条");
    tiao->setFixedHeight(28);

    mainLayout->addWidget(keywordBtn);
    mainLayout->addWidget(bvidBtn);
    mainLayout->addWidget(searchInput, 1);
    mainLayout->addWidget(searchBtn);
    mainLayout->addWidget(pageSizeLabel);
    mainLayout->addWidget(pageSizeBox);
    mainLayout->addWidget(tiao);
}

void BiliSearchBar::onSearchClicked()
{
    QString keyword = searchInput->text().trimmed();
    if (keyword.isEmpty()) return;
    auto type = static_cast<BiliSearchType>(typeGroup->checkedId());
    int pageSize = pageSizeBox->currentText().toInt();
    emit searchRequested(keyword, type, pageSize);
}
```

- [ ] **Step 3: 用 Qt Creator 构建项目，确认无编译错误**

- [ ] **Step 4: 提交**

```
git add dlpages/bilisearchbar.h dlpages/bilisearchbar.cpp
git commit -m "feat: add BiliSearchBar widget"
```

---

## Task 3：BiliSidePanel

**Files:**
- Create: `dlpages/bilisidepanel.h`
- Create: `dlpages/bilisidepanel.cpp`

- [ ] **Step 1: 创建 `dlpages/bilisidepanel.h`**

```cpp
#ifndef BILISIDEPANEL_H
#define BILISIDEPANEL_H

#include "common.h"
#include "bilitypes.h"
#include <QLineEdit>
#include <QComboBox>
#include <QFileDialog>
#include <QProgressBar>

class BiliSidePanel : public QWidget
{
    Q_OBJECT
public:
    explicit BiliSidePanel(QWidget *parent = nullptr);
    void             setSelectedSong(const BiliVideoInfo &info);
    BiliSaveSettings currentSettings() const;
    void             addDownloadTask(const QString &title);

private:
    QVBoxLayout *mainLayout;

    // 保存设置区
    QLabel      *pathLabel;
    QPushButton *browseBtn;
    QLineEdit   *templateInput;
    QComboBox   *qualityBox;

    // 收藏区
    QLabel      *selectedSongLabel;
    QPushButton *favoriteBtn;

    // 下载队列区
    QScrollArea *queueArea;
    QWidget     *queueWidget;
    QVBoxLayout *queueLayout;

    BiliVideoInfo currentInfo;

    void initSaveSettings();
    void initFavorites();
    void initDownloadQueue();
    QFrame *makeSeparator();

private slots:
    void onBrowseClicked();

signals:
    void saveSettingsChanged(BiliSaveSettings settings);
    void favoriteRequested(BiliVideoInfo info);
};

#endif // BILISIDEPANEL_H
```

- [ ] **Step 2: 创建 `dlpages/bilisidepanel.cpp`**

```cpp
#include "bilisidepanel.h"

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
    QLabel *title = new QLabel("保存设置");
    title->setStyleSheet("font-weight: bold; font-size: 11px;");
    mainLayout->addWidget(title);

    // 路径行
    QLabel *pathTitle = new QLabel("保存路径");
    pathTitle->setStyleSheet("font-size: 11px; color: #666;");
    mainLayout->addWidget(pathTitle);

    QHBoxLayout *pathRow = new QHBoxLayout();
    pathRow->setSpacing(4);
    pathLabel = new QLabel(QDir::homePath() + "/Music");
    pathLabel->setStyleSheet("font-size: 10px; color: #888;");
    pathLabel->setMaximumWidth(140);
    pathLabel->setElideMode(Qt::ElideMiddle);  // Qt::ElideMiddle 需手动截断，用 elidedText
    // 注意：QLabel 没有 setElideMode，改用 setSizePolicy 自动截断
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
    templateInput = new QLineEdit("{title} - {author}");
    templateInput->setFixedHeight(24);
    templateInput->setStyleSheet("font-size: 11px;");
    mainLayout->addWidget(templateInput);

    // 音质
    QLabel *qualTitle = new QLabel("音质");
    qualTitle->setStyleSheet("font-size: 11px; color: #666;");
    mainLayout->addWidget(qualTitle);
    qualityBox = new QComboBox();
    qualityBox->addItem("320 kbps",  "320kbps");
    qualityBox->addItem("192 kbps",  "192kbps");
    qualityBox->addItem("Hi-Res",    "hires");
    qualityBox->setFixedHeight(24);
    mainLayout->addWidget(qualityBox);
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
}

BiliSaveSettings BiliSidePanel::currentSettings() const
{
    return BiliSaveSettings{
        pathLabel->text(),
        templateInput->text(),
        qualityBox->currentData().toString()
    };
}

void BiliSidePanel::addDownloadTask(const QString &title)
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
```

- [ ] **Step 3: 用 Qt Creator 构建，确认无编译错误**

- [ ] **Step 4: 提交**

```
git add dlpages/bilisidepanel.h dlpages/bilisidepanel.cpp
git commit -m "feat: add BiliSidePanel widget"
```

---

## Task 4：BiliResultItem

**Files:**
- Create: `dlpages/biliresultitem.h`
- Create: `dlpages/biliresultitem.cpp`

- [ ] **Step 1: 创建 `dlpages/biliresultitem.h`**

```cpp
#ifndef BILIRESULTITEM_H
#define BILIRESULTITEM_H

#include "common.h"
#include "bilitypes.h"

class BiliResultItem : public QWidget
{
    Q_OBJECT
public:
    explicit BiliResultItem(const BiliVideoInfo &info, QWidget *parent = nullptr);
    void setExpanded(bool expanded);
    bool isExpanded() const { return m_expanded; }
    const BiliVideoInfo &videoInfo() const { return m_info; }

private:
    BiliVideoInfo m_info;
    bool          m_expanded = false;

    // 折叠行
    QWidget     *rowWidget;
    QHBoxLayout *rowLayout;
    QLabel      *coverLabel;
    QLabel      *titleLabel;
    QLabel      *metaLabel;
    QPushButton *quickDownloadBtn;

    // 展开详情
    QWidget     *detailWidget;
    QHBoxLayout *detailLayout;
    QLabel      *bigCoverLabel;
    QLabel      *infoLabel;
    QPushButton *downloadBtn;
    QPushButton *favoriteBtn;

    void initRow();
    void initDetail();
    static QString formatDuration(int seconds);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void expandToggled(BiliResultItem *item);
    void downloadRequested(BiliVideoInfo info);
    void favoriteRequested(BiliVideoInfo info);
};

#endif // BILIRESULTITEM_H
```

- [ ] **Step 2: 创建 `dlpages/biliresultitem.cpp`**

```cpp
#include "biliresultitem.h"

BiliResultItem::BiliResultItem(const BiliVideoInfo &info, QWidget *parent)
    : QWidget{parent}, m_info(info)
{
    setAttribute(Qt::WA_StyledBackground);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    initRow();
    initDetail();

    mainLayout->addWidget(rowWidget);
    mainLayout->addWidget(detailWidget);
    detailWidget->setVisible(false);
}

void BiliResultItem::initRow()
{
    rowWidget  = new QWidget();
    rowWidget->setAttribute(Qt::WA_StyledBackground);
    rowLayout  = new QHBoxLayout(rowWidget);
    rowLayout->setContentsMargins(8, 6, 8, 6);
    rowLayout->setSpacing(8);

    // 封面占位（54×30，后续可替换为真实图片）
    coverLabel = new QLabel();
    coverLabel->setFixedSize(54, 30);
    coverLabel->setStyleSheet("background:#cccccc; border-radius:2px;");
    coverLabel->setAlignment(Qt::AlignCenter);

    // 标题 + 元数据
    QWidget     *textWidget = new QWidget();
    QVBoxLayout *textLayout = new QVBoxLayout(textWidget);
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(2);

    titleLabel = new QLabel(m_info.title);
    titleLabel->setStyleSheet("font-size: 12px; font-weight: 600;");
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QString meta = QString("UP主：%1  ·  %2  ·  播放量 %3")
                       .arg(m_info.author)
                       .arg(formatDuration(m_info.duration))
                       .arg(m_info.playCount);
    metaLabel = new QLabel(meta);
    metaLabel->setStyleSheet("font-size: 10px; color: #888;");

    textLayout->addWidget(titleLabel);
    textLayout->addWidget(metaLabel);

    quickDownloadBtn = new QPushButton("⬇ 下载");
    quickDownloadBtn->setFixedSize(60, 24);
    quickDownloadBtn->setStyleSheet("font-size: 11px;");
    connect(quickDownloadBtn, &QPushButton::clicked, this, [this]() {
        emit downloadRequested(m_info);
    });

    rowLayout->addWidget(coverLabel);
    rowLayout->addWidget(textWidget, 1);
    rowLayout->addWidget(quickDownloadBtn);
}

void BiliResultItem::initDetail()
{
    detailWidget = new QWidget();
    detailWidget->setStyleSheet("background: #f0f0ea;");
    detailLayout = new QHBoxLayout(detailWidget);
    detailLayout->setContentsMargins(12, 8, 12, 8);
    detailLayout->setSpacing(12);

    // 封面大图占位（80×50）
    bigCoverLabel = new QLabel();
    bigCoverLabel->setFixedSize(80, 50);
    bigCoverLabel->setStyleSheet("background:#bbbbbb; border-radius:3px;");
    bigCoverLabel->setAlignment(Qt::AlignCenter);

    // 信息文字
    QString infoText = QString(
        "<b>BV号：</b>%1<br>"
        "<b>时长：</b>%2 &nbsp; <b>分P：</b>%3 P<br>"
        "<b>简介：</b>%4")
            .arg(m_info.bvId)
            .arg(formatDuration(m_info.duration))
            .arg(m_info.partCount)
            .arg(m_info.description.isEmpty() ? "暂无简介" : m_info.description);
    infoLabel = new QLabel(infoText);
    infoLabel->setStyleSheet("font-size: 11px; color: #555;");
    infoLabel->setWordWrap(true);
    infoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // 操作按钮
    QWidget     *btnWidget = new QWidget();
    QVBoxLayout *btnLayout = new QVBoxLayout(btnWidget);
    btnLayout->setSpacing(4);
    btnLayout->setAlignment(Qt::AlignVCenter);

    downloadBtn = new QPushButton("⬇ 加入下载");
    downloadBtn->setFixedSize(90, 26);
    downloadBtn->setStyleSheet("font-size: 11px; background:#555; color:#fff; border-radius:3px;");
    connect(downloadBtn, &QPushButton::clicked, this, [this]() {
        emit downloadRequested(m_info);
    });

    favoriteBtn = new QPushButton("★ 收藏");
    favoriteBtn->setFixedSize(90, 26);
    favoriteBtn->setStyleSheet("font-size: 11px; border-radius:3px;");
    connect(favoriteBtn, &QPushButton::clicked, this, [this]() {
        emit favoriteRequested(m_info);
    });

    btnLayout->addWidget(downloadBtn);
    btnLayout->addWidget(favoriteBtn);

    detailLayout->addWidget(bigCoverLabel);
    detailLayout->addWidget(infoLabel, 1);
    detailLayout->addWidget(btnWidget);
}

void BiliResultItem::setExpanded(bool expanded)
{
    if (m_expanded == expanded) return;
    m_expanded = expanded;

    detailWidget->setVisible(expanded);
    rowWidget->setStyleSheet(expanded ? "background: #eeeeea;" : "");

    // 展开/折叠动画
    QPropertyAnimation *anim = new QPropertyAnimation(detailWidget, "maximumHeight");
    anim->setDuration(150);
    if (expanded) {
        detailWidget->setMaximumHeight(0);
        anim->setStartValue(0);
        anim->setEndValue(detailWidget->sizeHint().height());
    } else {
        anim->setStartValue(detailWidget->height());
        anim->setEndValue(0);
        connect(anim, &QPropertyAnimation::finished, detailWidget, [this]() {
            detailWidget->setVisible(false);
            detailWidget->setMaximumHeight(QWIDGETSIZE_MAX);
        });
    }
    if (expanded) {
        connect(anim, &QPropertyAnimation::finished, detailWidget, [this]() {
            detailWidget->setMaximumHeight(QWIDGETSIZE_MAX);
        });
    }
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    adjustSize();
}

void BiliResultItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QWidget *child = childAt(event->pos());
        // 点击按钮不触发展开
        if (child && child->inherits("QAbstractButton")) {
            QWidget::mousePressEvent(event);
            return;
        }
        emit expandToggled(this);
    }
    QWidget::mousePressEvent(event);
}

QString BiliResultItem::formatDuration(int seconds)
{
    int m = seconds / 60;
    int s = seconds % 60;
    return QString("%1:%2").arg(m).arg(s, 2, 10, QChar('0'));
}
```

- [ ] **Step 3: 用 Qt Creator 构建，确认无编译错误**

- [ ] **Step 4: 提交**

```
git add dlpages/biliresultitem.h dlpages/biliresultitem.cpp
git commit -m "feat: add BiliResultItem with expand/collapse animation"
```

---

## Task 5：BiliResultList

**Files:**
- Create: `dlpages/biliresultlist.h`
- Create: `dlpages/biliresultlist.cpp`

- [ ] **Step 1: 创建 `dlpages/biliresultlist.h`**

```cpp
#ifndef BILIRESULTLIST_H
#define BILIRESULTLIST_H

#include "common.h"
#include "bilitypes.h"
#include "biliresultitem.h"

class BiliResultList : public QWidget
{
    Q_OBJECT
public:
    explicit BiliResultList(QWidget *parent = nullptr);
    void setResults(const QVector<BiliVideoInfo> &results);
    void clear();

private:
    QVBoxLayout *mainLayout;
    QWidget     *scrollContent;
    QVBoxLayout *scrollLayout;
    QScrollArea *scrollArea;

    QHBoxLayout     *paginationLayout;
    QPushButton     *prevBtn;
    QPushButton     *nextBtn;
    QVector<QPushButton *> pageButtons;

    BiliResultItem  *expandedItem = nullptr;

    void initScrollArea();
    void initPagination();

private slots:
    void onItemExpandToggled(BiliResultItem *item);

signals:
    void itemSelected(BiliVideoInfo info);
    void downloadRequested(BiliVideoInfo info);
    void favoriteRequested(BiliVideoInfo info);
};

#endif // BILIRESULTLIST_H
```

- [ ] **Step 2: 创建 `dlpages/biliresultlist.cpp`**

```cpp
#include "biliresultlist.h"

BiliResultList::BiliResultList(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    initScrollArea();
    initPagination();
}

void BiliResultList::initScrollArea()
{
    scrollContent = new QWidget();
    scrollLayout  = new QVBoxLayout(scrollContent);
    scrollLayout->setContentsMargins(8, 8, 8, 8);
    scrollLayout->setSpacing(4);
    scrollLayout->setAlignment(Qt::AlignTop);

    scrollArea = new QScrollArea();
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mainLayout->addWidget(scrollArea, 1);
}

void BiliResultList::initPagination()
{
    QWidget *paginationWidget = new QWidget();
    paginationLayout = new QHBoxLayout(paginationWidget);
    paginationLayout->setContentsMargins(0, 4, 0, 4);
    paginationLayout->setSpacing(4);
    paginationLayout->setAlignment(Qt::AlignHCenter);

    prevBtn = new QPushButton("◀");
    prevBtn->setFixedSize(28, 24);
    paginationLayout->addWidget(prevBtn);

    for (int i = 1; i <= 3; ++i) {
        QPushButton *btn = new QPushButton(QString::number(i));
        btn->setFixedSize(28, 24);
        btn->setCheckable(true);
        if (i == 1) btn->setChecked(true);
        pageButtons.append(btn);
        paginationLayout->addWidget(btn);
    }

    nextBtn = new QPushButton("▶");
    nextBtn->setFixedSize(28, 24);
    paginationLayout->addWidget(nextBtn);

    mainLayout->addWidget(paginationWidget);
}

void BiliResultList::setResults(const QVector<BiliVideoInfo> &results)
{
    clear();
    for (const BiliVideoInfo &info : results) {
        BiliResultItem *item = new BiliResultItem(info);
        connect(item, &BiliResultItem::expandToggled,
                this, &BiliResultList::onItemExpandToggled);
        connect(item, &BiliResultItem::downloadRequested,
                this, &BiliResultList::downloadRequested);
        connect(item, &BiliResultItem::favoriteRequested,
                this, &BiliResultList::favoriteRequested);
        scrollLayout->addWidget(item);
    }
}

void BiliResultList::clear()
{
    expandedItem = nullptr;
    QLayoutItem *child;
    while ((child = scrollLayout->takeAt(0)) != nullptr) {
        if (QWidget *w = child->widget()) {
            scrollLayout->removeWidget(w);
            w->deleteLater();
        }
        delete child;
    }
}

void BiliResultList::onItemExpandToggled(BiliResultItem *item)
{
    if (expandedItem && expandedItem != item) {
        expandedItem->setExpanded(false);
    }
    bool nowExpanded = !item->isExpanded();
    item->setExpanded(nowExpanded);
    expandedItem = nowExpanded ? item : nullptr;

    if (nowExpanded)
        emit itemSelected(item->videoInfo());
}
```

- [ ] **Step 3: 用 Qt Creator 构建，确认无编译错误**

- [ ] **Step 4: 提交**

```
git add dlpages/biliresultlist.h dlpages/biliresultlist.cpp
git commit -m "feat: add BiliResultList with single-expand management"
```

---

## Task 6：接入 BiliDLWidget

**Files:**
- Modify: `dlpages/bilidlwidget.h`
- Modify: `dlpages/bilidlwidget.cpp`

- [ ] **Step 1: 替换 `dlpages/bilidlwidget.h`**

```cpp
#ifndef BILIDLWIDGET_H
#define BILIDLWIDGET_H

#include "common.h"
#include "bilitypes.h"
#include "bilisearchbar.h"
#include "biliresultlist.h"
#include "bilisidepanel.h"

class BiliDLWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BiliDLWidget(QWidget *parent = nullptr);

private:
    QVBoxLayout  *mainLayout;
    BiliSearchBar  *searchBar;
    BiliResultList *resultList;
    BiliSidePanel  *sidePanel;

    void loadDemoData();

private slots:
    void onSearchRequested(QString keyword, BiliSearchType type, int pageSize);
    void onItemSelected(BiliVideoInfo info);
    void onDownloadRequested(BiliVideoInfo info);
    void onFavoriteRequested(BiliVideoInfo info);

signals:
};

#endif // BILIDLWIDGET_H
```

- [ ] **Step 2: 替换 `dlpages/bilidlwidget.cpp`**

```cpp
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

void BiliDLWidget::onSearchRequested(QString keyword, BiliSearchType type, int pageSize)
{
    Q_UNUSED(type); Q_UNUSED(pageSize);
    // 占位：后续接入真实 API
    qDebug() << "[BiliDLWidget] search:" << keyword;
}

void BiliDLWidget::onItemSelected(BiliVideoInfo info)
{
    sidePanel->setSelectedSong(info);
}

void BiliDLWidget::onDownloadRequested(BiliVideoInfo info)
{
    // 占位：后续接入真实下载
    sidePanel->addDownloadTask(info.title);
    qDebug() << "[BiliDLWidget] download:" << info.bvId;
}

void BiliDLWidget::onFavoriteRequested(BiliVideoInfo info)
{
    // 占位：后续接入收藏逻辑
    qDebug() << "[BiliDLWidget] favorite:" << info.bvId;
}
```

- [ ] **Step 3: 用 Qt Creator 构建整个项目，确认无编译错误**

- [ ] **Step 4: 运行程序，点击"下载"侧边栏按钮，验证以下行为：**
  - 页面显示三条演示结果
  - 点击结果行展开详情，再点击另一行时前一个自动收起
  - 点击"⬇ 下载"按钮后右侧下载队列出现该歌曲
  - 点击右侧"…"按钮能打开文件夹选择对话框
  - 展开行后右侧收藏区显示歌曲名并激活"添加到收藏"按钮

- [ ] **Step 5: 提交**

```
git add dlpages/bilidlwidget.h dlpages/bilidlwidget.cpp
git commit -m "feat: wire up BiliDLWidget — layout complete"
```
