#ifndef APIPARSER_H
#define APIPARSER_H

#include "common.h"
#include "dlpages/bilitypes.h"

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
