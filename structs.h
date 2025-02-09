#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>
#include <random>

struct MusicInfo{
    QString name, singer, album;
    QString style, path;
    int times;
    int weight;
};
struct DbInfo{
    int MusicListNum;
    QString chosenDir;
};


#endif // STRUCTS_H
