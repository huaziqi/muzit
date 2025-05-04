#ifndef MUSICITEM_H
#define MUSICITEM_H

#include "common.h"

class MusicItemWidget;

class MusicItem : public QObject
{
    Q_OBJECT
public:
    explicit MusicItem(QString _BVId, QString _title, QString _author, QString _coverUrl, int _playedNum, int _duration, int _rank = -1, QObject *parent = nullptr);;
    //virtual ~MusicItem();
    void setWidget(MusicItemWidget* widget);
    MusicItemWidget* getWidget();
public:
    void output();
public:
    QString BVId;
    QString title, author;
    QString coverUrl;
    int playedNum, duration;
    int rank;

    MusicItemWidget* widget;
signals:
};

#endif // MUSICITEM_H
