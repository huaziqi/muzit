#ifndef APIPARSER_H
#define APIPARSER_H

#include "common.h"

namespace ApiParser {


    bool parseCid(
        const QByteArray& byteArray,
        qint64& cid,
        QString& error);
}

#endif // APIPARSER_H
