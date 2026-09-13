#ifndef METADATATYPES_H
#define METADATATYPES_H

#include <QMetaType>
#include <QString>

struct AudioMetadata
{
    QString title;
    QString artist;
    QString album;
    QString coverUrl;
};

Q_DECLARE_METATYPE(AudioMetadata)

#endif // METADATATYPES_H
