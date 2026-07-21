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
