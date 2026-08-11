#ifndef AUDIOTYPES_H
#define AUDIOTYPES_H

#include <QString>
#include <QMetaType>

enum class AudioOutputFormat
{
    M4a = 0,
    Mp3 = 1,
    Flac = 2
};

struct AudioConvertOptions
{
    QString inputPath;
    QString outputPath;
    AudioOutputFormat format = AudioOutputFormat::M4a;
    int mp3Bitrate = 192000;
    int flacCompressionLevel = 5;
    bool removeSourceAfterSuccess = false;
};

struct AudioInfo
{
    QString codecName;
    qint64 durationMilliseconds = 0;
    qint64 bitrate = 0;
    int sampleRate = 0;
    int channels = 0;
};

struct AudioMetadata
{
    QString title;
    QString artist;
    QString album;
    QString coverUrl;
};


Q_DECLARE_METATYPE(AudioOutputFormat);
Q_DECLARE_METATYPE(AudioConvertOptions);
Q_DECLARE_METATYPE(AudioInfo);
Q_DECLARE_METATYPE(AudioMetadata);

#endif // AUDIOTYPES_H
