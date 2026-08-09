#include "apiparser.h"

namespace {
    QString streamUrl(const QJsonObject &object)
    {
        QString url = object.value("baseUrl").toString();
        if (url.isEmpty())
            url = object.value("base_url").toString();
        return url;
    }

    QStringList streamBackupUrls(const QJsonObject &object)
    {
        QJsonArray urls = object.value("backupUrl").toArray();
        if (urls.isEmpty())
            urls = object.value("backup_url").toArray();

        QStringList result;
        for (const QJsonValue &value : urls) {
            if (value.isString())
                result.append(value.toString());
        }
        return result;
    }

    BiliAudioStream parseAudioStream(const QJsonObject &object)
    {
        BiliAudioStream stream;
        stream.id = object.value("id").toInt();
        stream.qualityDescription = ApiParser::audioQualityDescription(stream.id);
        stream.url = streamUrl(object);
        stream.backupUrls = streamBackupUrls(object);
        stream.bandwidth = object.value("bandwidth").toInteger();
        stream.mimeType = object.value("mimeType").toString();
        if (stream.mimeType.isEmpty())
            stream.mimeType = object.value("mime_type").toString();
        stream.codecs = object.value("codecs").toString();
        return stream;
    }

    void appendAudioStreams(
        const QJsonArray &array,
        QVector<BiliAudioStream> &streams)
    {
        for (const QJsonValue &value : array) {
            if (!value.isObject())
                continue;

            BiliAudioStream stream = parseAudioStream(value.toObject());
            if (!stream.url.isEmpty())
                streams.append(stream);
        }
    }
}

QString ApiParser::audioQualityDescription(int id)
{
    switch (id) {
    case 30216:
        return QStringLiteral("64K");
    case 30232:
        return QStringLiteral("132K");
    case 30280:
        return QStringLiteral("192K");
    case 30250:
        return QStringLiteral("杜比全景声");
    case 30251:
        return QStringLiteral("Hi-Res无损");
    default:
        return QStringLiteral("未知音质");
    }
}

bool ApiParser::parseVideoInfo(
    const QByteArray &byteArray,
    BiliVideoInfo &info,
    QString &error)
{
    info = BiliVideoInfo{};
    error.clear();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(byteArray, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        error = parseError.error == QJsonParseError::NoError
                    ? QStringLiteral("视频信息响应不是JSON对象")
                    : parseError.errorString();
        return false;
    }

    const QJsonObject root = doc.object();
    const int code = root.value("code").toInt(-1);
    if (code != 0) {
        error = root.value("message").toString(
            QStringLiteral("获取视频信息失败，错误码：%1").arg(code));
        return false;
    }

    const QJsonValue dataValue = root.value("data");
    if (!dataValue.isObject()) {
        error = QStringLiteral("视频信息响应缺少data对象");
        return false;
    }

    const QJsonObject data = dataValue.toObject();
    info.bvid = data.value("bvid").toString();
    info.title = data.value("title").toString();
    info.author = data.value("owner").toObject().value("name").toString();
    info.coverUrl = data.value("pic").toString();
    info.durationMilliseconds = data.value("duration").toInteger() * 1000;
    info.description = data.value("desc").toString();
    info.playCount = data.value("stat").toObject().value("view").toInteger();

    const QJsonArray pages = data.value("pages").toArray();
    for (const QJsonValue &value : pages) {
        if (!value.isObject())
            continue;

        const QJsonObject pageObject = value.toObject();
        BiliPlayUrlInfo part;
        part.cid = pageObject.value("cid").toInteger();
        part.page = pageObject.value("page").toInt();
        part.title = pageObject.value("part").toString();
        part.durationMilliseconds =
            pageObject.value("duration").toInteger() * 1000;

        if (part.cid > 0)
            info.parts.append(part);
    }

    if (info.parts.isEmpty()) {
        error = QStringLiteral("视频没有可用的分P信息");
        return false;
    }

    if (info.parts.size() == 1)
        info.parts.first().selected = true;

    return true;
}

bool ApiParser::parsePlayUrlInfo(
    const QByteArray &byteArray,
    BiliPlayUrlInfo &playUrlInfo,
    QString &error)
{
    playUrlInfo.audioStreams.clear();
    error.clear();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(byteArray, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        error = parseError.error == QJsonParseError::NoError
                    ? QStringLiteral("播放流响应不是JSON对象")
                    : parseError.errorString();
        return false;
    }

    const QJsonObject root = doc.object();
    const int code = root.value("code").toInt(-1);
    if (code != 0) {
        error = root.value("message").toString(
            QStringLiteral("获取播放流失败，错误码：%1").arg(code));
        return false;
    }

    const QJsonValue dataValue = root.value("data");
    if (!dataValue.isObject()) {
        error = QStringLiteral("播放流响应缺少data对象");
        return false;
    }

    const QJsonObject data = dataValue.toObject();

    const QJsonValue dashValue = data.value("dash");
    if (!dashValue.isObject()) {
        error = QStringLiteral("接口没有返回DASH播放流");
        return false;
    }

    const QJsonObject dash = dashValue.toObject();
    appendAudioStreams(
        dash.value("audio").toArray(), playUrlInfo.audioStreams);

    const QJsonObject dolby = dash.value("dolby").toObject();
    appendAudioStreams(
        dolby.value("audio").toArray(), playUrlInfo.audioStreams);

    const QJsonObject flac = dash.value("flac").toObject();
    const QJsonValue flacAudio = flac.value("audio");
    if (flacAudio.isObject()) {
        BiliAudioStream stream = parseAudioStream(flacAudio.toObject());
        if (!stream.url.isEmpty())
            playUrlInfo.audioStreams.append(stream);
    }

    if (playUrlInfo.audioStreams.isEmpty()) {
        error = QStringLiteral("视频分P没有可用的音频流");
        return false;
    }

    return true;
}
