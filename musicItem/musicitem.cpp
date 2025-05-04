#include "musicitem.h"



MusicItem::MusicItem(QString _BVId, QString _title, QString _author, QString _coverUrl, int _playedNum, int _duration, int _rank, QObject *parent) : \
    BVId(_BVId), title(_title), author(_author), coverUrl(_coverUrl), playedNum(_playedNum), duration(_duration), rank(_rank){

}

void MusicItem::output()
{
    qDebug() << BVId << " " << author << " " << title << " " << coverUrl << " " << playedNum << " " << duration << " " << rank;
}
