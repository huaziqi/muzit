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

    // 封面占位（54×30）
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

    quickDownloadBtn = new QPushButton("下载");
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

    downloadBtn = new QPushButton("加入下载");
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

    rowWidget->setStyleSheet(expanded ? "background: #eeeeea;" : "");

    // 展开/折叠动画
    QPropertyAnimation *anim = new QPropertyAnimation(detailWidget, "maximumHeight");
    anim->setDuration(150);
    if (expanded) {
        detailWidget->setVisible(true);
        detailWidget->setMaximumHeight(0);
        anim->setStartValue(0);
        anim->setEndValue(detailWidget->sizeHint().height());
        connect(anim, &QPropertyAnimation::finished, detailWidget, [this]() {
            detailWidget->setMaximumHeight(QWIDGETSIZE_MAX);
        });
    } else {
        anim->setStartValue(detailWidget->height());
        anim->setEndValue(0);
        connect(anim, &QPropertyAnimation::finished, detailWidget, [this]() {
            detailWidget->setVisible(false);
            detailWidget->setMaximumHeight(QWIDGETSIZE_MAX);
        });
    }
    anim->start(QAbstractAnimation::DeleteWhenStopped);
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
