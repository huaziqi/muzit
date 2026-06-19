#ifndef BILITYPES_H
#define BILITYPES_H

#include <QString>

enum class BiliSearchType { Keyword, BvId };

struct BiliVideoInfo {
    QString bvId;
    QString title;
    QString author;
    QString coverUrl;
    int     duration;   // 秒
    int     partCount;
    QString description;
    int     playCount;
};

struct BiliSaveSettings {
    QString savePath;
    QString fileNameTemplate;  // 支持 {title} {author} {bvid}
    QString quality;           // "320kbps" / "192kbps" / "hires"
};

#endif // BILITYPES_H
