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
