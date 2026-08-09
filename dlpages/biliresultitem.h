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
    void setVideoInfo(const BiliVideoInfo &info);
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
    QPushButton *partSelectBtn = nullptr;
    QPushButton *quickDownloadBtn = nullptr;

    // 展开详情
    QWidget     *detailWidget;
    QHBoxLayout *detailLayout;
    QLabel      *bigCoverLabel;
    QLabel      *infoLabel;
    QPushButton *downloadBtn = nullptr;
    QPushButton *favoriteBtn;

    void initRow();
    void initDetail();
    void showPartSelectionDialog();
    void refreshPartSelectionState();
    static QString formatDuration(qint64 milliseconds);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void expandToggled(BiliResultItem *item);
    void partsSelectionChanged(BiliVideoInfo info);
    void downloadRequested(BiliVideoInfo info);
    void favoriteRequested(BiliVideoInfo info);
};

#endif // BILIRESULTITEM_H
