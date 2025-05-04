#ifndef MUSICITEMWIDGET_H
#define MUSICITEMWIDGET_H

#include "common.h"
#include "musicitem.h"

class MusicItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicItemWidget(MusicItem* _musicItem, QWidget *parent = nullptr);
private:
    QHBoxLayout* mainLayout;
    MusicItem *musicItem;

    QNetworkAccessManager *manager;
    QNetworkRequest* coverRequest;
    QNetworkReply* coverReply;
signals:
};

#endif // MUSICITEMWIDGET_H
