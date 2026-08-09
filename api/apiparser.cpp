#include "apiparser.h"

namespace{


}

bool ApiParser::parseCid(const QByteArray& byteArray, qint64 &cid, QString &error)
{
    QJsonParseError parseError;
    const QJsonDocument doc =
        QJsonDocument::fromJson(byteArray, &parseError);

    if (parseError.error != QJsonParseError::NoError ||
        !doc.isObject()) {
        error = parseError.errorString();
        return false;
    }
    const QJsonObject root = doc.object();
    if(root.value("code").toInteger() != 0){
        error = "获取失败";
        return false;
    }
    QJsonObject data = root.value("data").toObject();
    QJsonArray pages = data.value("pages").toArray();
    QJsonObject pageInfo = pages.at(0).toObject();
    cid = pageInfo.value("cid").toInteger();
    return true;
}
