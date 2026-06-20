#ifndef BILITYPES_H
#define BILITYPES_H

#include <QString>

enum class BiliSearchType { Keyword, BvId };

struct BiliVideoInfo {
    QString bvId;
    QString title;
    QString author;
    QString coverUrl;
    int duration;   // 秒
    int partCount;
    QString description;
    int playCount;
};

struct BiliSaveSettings {
    QString savePath;
    static constexpr const char* SavePathKey = "savePath";
    QString fileNameTemplate;  // 支持 {title} {author}
    static constexpr const char* FileNameTemplateKey = "fileNameTemplate";
    QString quality;           // "320kbps" / "192kbps" / "hires"
    static constexpr const char* QualityKey = "quality";
};

#endif // BILITYPES_H
