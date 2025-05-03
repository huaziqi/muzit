#ifndef MUSICITEM_H
#define MUSICITEM_H

#include "common.h"

class MusicItem : public QObject
{
    Q_OBJECT
public:
    explicit MusicItem(QString BV, QObject *parent = nullptr);;

protected:
    QString BVNumber;

signals:
};

#endif // MUSICITEM_H
