#ifndef APIPARSER_H
#define APIPARSER_H

#include "platforms/bilibili/bilitypes.h"

#include <QByteArray>
#include <QString>

namespace ApiParser {

    QString audioQualityDescription(int id);

    bool parseVideoInfo(
        const QByteArray &byteArray,
        BiliVideoInfo &info,
        QString &error);

    bool parsePlayUrlInfo(
        const QByteArray &byteArray,
        BiliPlayUrlInfo &playUrlInfo,
        QString &error);
}

#endif // APIPARSER_H
