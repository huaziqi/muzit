#ifndef MUSICITEMWIDGET_H
#define MUSICITEMWIDGET_H

#include "common.h"
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
    QLabel *coverLabel;
    MusicItem *musicItem;
    QString coverFileName;
    QPixmap coverPixMap, originCoverPixmap;
    int minWidth, maxWidth;
    int lastWidth;

    QNetworkAccessManager *manager;
    QNetworkRequest* coverRequest;
    QNetworkReply* coverReply;
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    void gotCover();
    void initLayout();
    void drawPixMap(int width, int height);
signals:
};

#endif // MUSICITEMWIDGET_H
