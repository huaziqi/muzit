#ifndef BILITYPES_H
#define BILITYPES_H

#include <QMetaType>
#include <QString>
#include <QStringList>
#include <QVector>

#include "audio/audiotypes.h"

enum class BiliSearchType { Keyword, BvId };

struct BiliAudioStream
{
    int id = 0;
    QString qualityDescription;
    QString url;
    QStringList backupUrls;
    qint64 bandwidth = 0;
    QString mimeType;
    QString codecs;
};

struct BiliPlayUrlInfo
{
    qint64 cid = 0;
    int page = 0;
    QString title;
    qint64 durationMilliseconds = 0;
    bool selected = false;
    QVector<BiliAudioStream> audioStreams;
};

struct BiliVideoInfo
{
    QString bvid;
    QString title;
    QString author;
    QString coverUrl;
    qint64 durationMilliseconds = 0;
    QString description;
    qint64 playCount = 0;
    QVector<BiliPlayUrlInfo> parts;
};

struct AudioDownloadJob
{
    QString bvid;
    qint64 cid = 0;
    int page = 0;

    BiliAudioStream source;
    AudioMetadata metadata;

    AudioOutputFormat outputFormat = AudioOutputFormat::M4a;
    QString temporaryPath;
    QString outputPath;

    AudioConvertOptions toConvertOptions() const
    {
        AudioConvertOptions options;
        options.inputPath = temporaryPath;
        options.outputPath = outputPath;
        options.format = outputFormat;
        options.removeSourceAfterSuccess = true;
        return options;
    }
};

struct BiliSaveSettings
{
    QString savePath;
    static constexpr const char *SavePathKey = "savePath";
    QString fileNameTemplate;
    static constexpr const char *FileNameTemplateKey = "fileNameTemplate";
    QString quality;
    static constexpr const char *QualityKey = "quality";
    AudioOutputFormat outputFormat = AudioOutputFormat::M4a;
    static constexpr const char *OutputFormatKey = "outputFormat";
};


Q_DECLARE_METATYPE(BiliAudioStream)
Q_DECLARE_METATYPE(BiliPlayUrlInfo)
Q_DECLARE_METATYPE(BiliVideoInfo)
Q_DECLARE_METATYPE(AudioDownloadJob)
Q_DECLARE_METATYPE(BiliSaveSettings)

#endif // BILITYPES_H
