#ifndef MUSICITEMWIDGET_H
#define MUSICITEMWIDGET_H

#include "ui/uicommon.h"
#include "musicitem.h"

class MusicItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicItemWidget(MusicItem* _musicItem, QWidget *parent = nullptr);
    virtual ~MusicItemWidget();
private:
    //一些布局
    QHBoxLayout* mainLayout;
    QLabel *coverLabel = nullptr;
    MusicItem *musicItem;
    QString coverFileName;
    QPixmap coverPixMap, originCoverPixmap;
    int pixMinWidth, pixMaxWidth;
    int infoMinHeight;
    int lastWidth;
    int pendingCoverWidth = 0;
    double aspectRadio = 0.55; //pixMap长宽比
    QTimer *coverResizeTimer;

    QWidget* infoWidget = nullptr; //右侧信息栏
    QVBoxLayout *infoLayout;
    QLabel* titleLabel, *authorLabel, * playedNumLabel;
    QFont labelFont;

    QNetworkAccessManager *manager;
    QNetworkRequest* coverRequest;
    QNetworkReply* coverReply;
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    void gotCover();
    void initLayout();
    void initInfo();
    void drawPixMap(int width, int height);
    void updateCoverPixmap();
signals:
};

#endif // MUSICITEMWIDGET_H
