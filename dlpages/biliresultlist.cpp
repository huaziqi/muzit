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
