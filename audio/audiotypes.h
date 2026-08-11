#ifndef AUDIOTYPES_H
#define AUDIOTYPES_H

#include <QString>
#include <QMetaType>

enum class AudioOutputFormat
{
    M4a,
    Mp3,
    Flac
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

Q_DECLARE_METATYPE(AudioConvertOptions);

#endif // AUDIOTYPES_H
