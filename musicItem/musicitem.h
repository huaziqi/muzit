#ifndef MUSICITEM_H
#define MUSICITEM_H

#include "common.h"

class MusicItem : public QObject
{
    Q_OBJECT
public:
    explicit MusicItem(QString _BVId, QString _title, QString _author, QString _coverUrl, int _playedNum, int _duration, int _rank = -1, QObject *parent = nullptr);;
    //virtual ~MusicItem();

public:
    void output();
protected:

    QString BVId;
    QString title, author;
    QString coverUrl;
    int playedNum, duration;
    int rank;
signals:
};

#endif // MUSICITEM_H
