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
