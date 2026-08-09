#include "bilibiliapi.h"

namespace {
    QNetworkRequest createRequest(const QUrl &url)
    {
        QNetworkRequest request(url);

        request.setHeader(
            QNetworkRequest::UserAgentHeader,
            QByteArrayLiteral("Muzit/1.0")
            );

        request.setRawHeader(
            QByteArrayLiteral("Accept"),
            QByteArrayLiteral("application/json")
            );

        return request;
    }

}


QNetworkRequest BilibiliApi::weeklyPeriods()
{
    QUrl url(
        "https://api.bilibili.com/"
        "x/copyright-music-publicity/toplist/all_period"
        );

    QUrlQuery query;
    query.addQueryItem("list_type", "1");
    url.setQuery(query);

    return createRequest(url);
}

QNetworkRequest BilibiliApi::weeklySongs(int listId)
{
    QUrl url(
        "https://api.bilibili.com/"
        "x/copyright-music-publicity/toplist/music_list"
        );

    QUrlQuery query;
    query.addQueryItem("list_id", QString::number(listId));
    url.setQuery(query);

    return createRequest(url);
}

QNetworkRequest BilibiliApi::videoInfoRequest(QString bvid)
{
    QUrl url(
        "https://api.bilibili.com/"
        "x/web-interface/view"
        );
    QUrlQuery query;
    query.addQueryItem("bvid", bvid);
    url.setQuery(query);
    return createRequest(url);

}

QNetworkRequest BilibiliApi::playUrlRequest(const QString &bvid, qint64 cid)
{
    QUrl url(
        "https://api.bilibili.com/"
        "x/player/playurl"
        );

    QUrlQuery query;
    query.addQueryItem("bvid", bvid);
    query.addQueryItem("cid", QString::number(cid));
    query.addQueryItem("fnval", "16");
    query.addQueryItem("fnver", "0");
    query.addQueryItem("otype", "json");
    url.setQuery(query);

    QNetworkRequest request = createRequest(url);
    request.setRawHeader(
        QByteArrayLiteral("Referer"),
        QByteArrayLiteral("https://www.bilibili.com")
        );
    return request;
}
