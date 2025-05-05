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
    QHBoxLayout* mainLayout;
    QLabel *coverLabel;
    MusicItem *musicItem;
    QString coverFileName;
    QPixmap coverPixMap;

    QNetworkAccessManager *manager;
    QNetworkRequest* coverRequest;
    QNetworkReply* coverReply;
private:
    void gotCover();
    void initLayout();
signals:
};

#endif // MUSICITEMWIDGET_H
