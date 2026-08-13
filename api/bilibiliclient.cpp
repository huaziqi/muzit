#include "bilibiliclient.h"

#include <QCryptographicHash>
#include <QJsonParseError>
#include <QNetworkCookie>
#include <QRegularExpression>

namespace {

constexpr int KeyCacheSeconds = 60 * 60;

const int MixinKeyEncTable[] = {
    46, 47, 18, 2, 53, 8, 23, 32,
    15, 50, 10, 31, 58, 3, 45, 35,
    27, 43, 5, 49, 33, 9, 42, 19,
    29, 28, 14, 39, 12, 38, 41, 13,
    37, 48, 7, 16, 24, 55, 40, 61,
    26, 17, 0, 1, 60, 51, 30, 4,
    22, 25, 54, 21, 56, 59, 6, 63,
    57, 62, 11, 36, 20, 34, 44, 52
};

qint64 durationMilliseconds(const QString &duration)
{
    const QStringList parts = duration.split(QLatin1Char(':'));
    qint64 seconds = 0;
    for (const QString &part : parts)
        seconds = seconds * 60 + part.toLongLong();
    return seconds * 1000;
}

QString plainTitle(QString title)
{
    title.remove(QRegularExpression(QStringLiteral("<[^>]*>")));
    return title.replace(QStringLiteral("&lt;"), QStringLiteral("<"))
        .replace(QStringLiteral("&gt;"), QStringLiteral(">"))
        .replace(QStringLiteral("&quot;"), QStringLiteral("\""))
        .replace(QStringLiteral("&#39;"), QStringLiteral("'"))
        .replace(QStringLiteral("&amp;"), QStringLiteral("&"));
}

}

BilibiliClient::BilibiliClient(QObject *parent)
    : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    cookieJar = new QNetworkCookieJar(networkManager);
    networkManager->setCookieJar(cookieJar);
}

void BilibiliClient::initialize()
{
    if (isReady()) {
        emit initialized();
        flushPendingSearches();
        return;
    }
    if (initializing)
        return;

    initializing = true;
    fetchBuvid();
}

void BilibiliClient::searchVideos(const QString &keyword,
                                  int page,
                                  int pageSize,
                                  const QString &order)
{
    SearchRequest search;
    search.keyword = keyword.trimmed();
    search.page = qMax(1, page);
    search.pageSize = qBound(1, pageSize, 50);
    search.order = order;

    if (search.keyword.isEmpty()) {
        emit searchFailed(QStringLiteral("搜索关键词不能为空"));
        return;
    }

    if (!isReady()) {
        pendingSearches.append(search);
        initialize();
        return;
    }

    performSearch(search);
}

bool BilibiliClient::isReady() const
{
    return !imgKey.isEmpty()
        && !subKey.isEmpty()
        && keysFetchedAt.secsTo(QDateTime::currentDateTimeUtc()) < KeyCacheSeconds;
}

void BilibiliClient::fetchBuvid()
{
    const QUrl url(QStringLiteral("https://api.bilibili.com/x/frontend/finger/spi"));
    QNetworkReply *reply = networkManager->get(createRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const QByteArray body = reply->readAll();
        if (reply->error() != QNetworkReply::NoError) {
            const QString error = reply->errorString();
            reply->deleteLater();
            failInitialization(QStringLiteral("获取 buvid 失败：%1").arg(error));
            return;
        }
        reply->deleteLater();

        const QJsonObject root = QJsonDocument::fromJson(body).object();
        const QJsonObject data = root.value(QStringLiteral("data")).toObject();
        const QString buvid3 = data.value(QStringLiteral("b_3")).toString();
        const QString buvid4 = data.value(QStringLiteral("b_4")).toString();
        if (buvid3.isEmpty()) {
            failInitialization(QStringLiteral("buvid 接口未返回 b_3"));
            return;
        }

        QList<QNetworkCookie> cookies;
        for (const auto &item : QList<QPair<QByteArray, QByteArray>>{
                 {QByteArrayLiteral("buvid3"), buvid3.toUtf8()},
                 {QByteArrayLiteral("buvid4"), buvid4.toUtf8()},
                 {QByteArrayLiteral("b_nut"), QByteArray::number(QDateTime::currentSecsSinceEpoch())}}) {
            if (item.second.isEmpty())
                continue;
            QNetworkCookie cookie(item.first, item.second);
            cookie.setDomain(QStringLiteral(".bilibili.com"));
            cookie.setPath(QStringLiteral("/"));
            cookies.append(cookie);
        }
        cookieJar->setCookiesFromUrl(cookies, QUrl(QStringLiteral("https://www.bilibili.com/")));
        fetchWbiKeys();
    });
}

void BilibiliClient::fetchWbiKeys()
{
    const QUrl url(QStringLiteral("https://api.bilibili.com/x/web-interface/nav"));
    QNetworkReply *reply = networkManager->get(createRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const QByteArray body = reply->readAll();
        if (reply->error() != QNetworkReply::NoError) {
            const QString error = reply->errorString();
            reply->deleteLater();
            failInitialization(QStringLiteral("获取 WBI Key 失败：%1").arg(error));
            return;
        }
        reply->deleteLater();

        const QJsonObject root = QJsonDocument::fromJson(body).object();
        const QJsonObject wbi = root.value(QStringLiteral("data"))
                                    .toObject()
                                    .value(QStringLiteral("wbi_img"))
                                    .toObject();
        imgKey = keyFromUrl(wbi.value(QStringLiteral("img_url")).toString());
        subKey = keyFromUrl(wbi.value(QStringLiteral("sub_url")).toString());
        if (imgKey.isEmpty() || subKey.isEmpty()) {
            failInitialization(QStringLiteral("nav 接口未返回有效的 WBI Key"));
            return;
        }

        keysFetchedAt = QDateTime::currentDateTimeUtc();
        initializing = false;
        emit initialized();
        flushPendingSearches();
    });
}

void BilibiliClient::performSearch(const SearchRequest &search)
{
    QMap<QString, QString> parameters;
    parameters.insert(QStringLiteral("keyword"), search.keyword);
    parameters.insert(QStringLiteral("search_type"), QStringLiteral("video"));
    parameters.insert(QStringLiteral("page"), QString::number(search.page));
    parameters.insert(QStringLiteral("page_size"), QString::number(search.pageSize));
    parameters.insert(QStringLiteral("order"), search.order);

    const QUrl url = signedUrl(
        QUrl(QStringLiteral("https://api.bilibili.com/x/web-interface/wbi/search/type")),
        parameters);
    QNetworkReply *reply = networkManager->get(createRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply, search]() {
        const QByteArray body = reply->readAll();
        if (reply->error() != QNetworkReply::NoError) {
            const QString error = reply->errorString();
            reply->deleteLater();
            emit searchFailed(QStringLiteral("搜索请求失败：%1").arg(error));
            return;
        }
        reply->deleteLater();

        QVector<BiliVideoInfo> results;
        int page = search.page;
        int totalResults = 0;
        bool signatureRejected = false;
        QString error;
        if (!parseSearchResponse(body, results, page, totalResults,
                                 signatureRejected, error)) {
            if (search.mayRetry && signatureRejected) {
                imgKey.clear();
                subKey.clear();
                SearchRequest retry = search;
                retry.mayRetry = false;
                pendingSearches.prepend(retry);
                initialize();
                return;
            }
            emit searchFailed(error);
            return;
        }

        emit searchFinished(results, page, totalResults);
    });
}

void BilibiliClient::failInitialization(const QString &error)
{
    initializing = false;
    pendingSearches.clear();
    emit initializationFailed(error);
    emit searchFailed(error);
}

void BilibiliClient::flushPendingSearches()
{
    const QVector<SearchRequest> searches = pendingSearches;
    pendingSearches.clear();
    for (const SearchRequest &search : searches)
        performSearch(search);
}

QNetworkRequest BilibiliClient::createRequest(const QUrl &url) const
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QByteArrayLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                                        "AppleWebKit/537.36 (KHTML, like Gecko) "
                                        "Chrome/124.0 Safari/537.36"));
    request.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("Referer"), QByteArrayLiteral("https://www.bilibili.com/"));
    return request;
}

QUrl BilibiliClient::signedUrl(const QUrl &baseUrl,
                               const QMap<QString, QString> &parameters) const
{
    QMap<QString, QString> signedParameters = parameters;
    signedParameters.insert(QStringLiteral("wts"),
                            QString::number(QDateTime::currentSecsSinceEpoch()));
    const QByteArray query = encodedQuery(signedParameters);
    const QByteArray digest = QCryptographicHash::hash(
        query + mixinKey().toUtf8(), QCryptographicHash::Md5).toHex();

    signedParameters.insert(QStringLiteral("w_rid"), QString::fromLatin1(digest));
    QUrl url(baseUrl);
    url.setQuery(QString::fromLatin1(encodedQuery(signedParameters)));
    return url;
}

QString BilibiliClient::mixinKey() const
{
    const QString rawKey = imgKey + subKey;
    QString result;
    result.reserve(32);
    for (int index : MixinKeyEncTable) {
        if (result.size() == 32)
            break;
        if (index >= 0 && index < rawKey.size())
            result.append(rawKey.at(index));
    }
    return result;
}

QString BilibiliClient::keyFromUrl(const QString &url)
{
    const QString fileName = QUrl(url).fileName();
    const int dot = fileName.lastIndexOf(QLatin1Char('.'));
    return dot > 0 ? fileName.left(dot) : fileName;
}

QString BilibiliClient::sanitizedValue(QString value)
{
    static const QString unwanted = QStringLiteral("!'()*");
    for (const QChar character : unwanted)
        value.remove(character);
    return value;
}

QByteArray BilibiliClient::encodedQuery(const QMap<QString, QString> &parameters)
{
    QByteArray result;
    for (auto it = parameters.cbegin(); it != parameters.cend(); ++it) {
        if (!result.isEmpty())
            result.append('&');
        result.append(QUrl::toPercentEncoding(it.key()));
        result.append('=');
        result.append(QUrl::toPercentEncoding(sanitizedValue(it.value())));
    }
    return result;
}

bool BilibiliClient::parseSearchResponse(const QByteArray &data,
                                         QVector<BiliVideoInfo> &results,
                                         int &page,
                                         int &totalResults,
                                         bool &signatureRejected,
                                         QString &error)
{
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        error = QStringLiteral("搜索响应不是有效 JSON：%1").arg(parseError.errorString());
        return false;
    }

    const QJsonObject root = document.object();
    const int code = root.value(QStringLiteral("code")).toInt(-1);
    const QJsonObject responseData = root.value(QStringLiteral("data")).toObject();
    signatureRejected = responseData.contains(QStringLiteral("v_voucher"));
    if (code != 0 || signatureRejected) {
        error = QStringLiteral("Bilibili 搜索失败（%1）：%2")
                    .arg(code)
                    .arg(root.value(QStringLiteral("message")).toString());
        if (signatureRejected)
            error += QStringLiteral("，WBI 签名已失效");
        return false;
    }

    page = responseData.value(QStringLiteral("page")).toInt(page);
    totalResults = responseData.value(QStringLiteral("numResults")).toInt();
    const QJsonArray items = responseData.value(QStringLiteral("result")).toArray();
    results.reserve(items.size());
    for (const QJsonValue &value : items) {
        if (!value.isObject())
            continue;
        const QJsonObject item = value.toObject();
        BiliVideoInfo info;
        info.bvid = item.value(QStringLiteral("bvid")).toString();
        info.title = plainTitle(item.value(QStringLiteral("title")).toString());
        info.author = item.value(QStringLiteral("author")).toString();
        info.coverUrl = item.value(QStringLiteral("pic")).toString();
        if (info.coverUrl.startsWith(QStringLiteral("//")))
            info.coverUrl.prepend(QStringLiteral("https:"));
        info.durationMilliseconds = durationMilliseconds(
            item.value(QStringLiteral("duration")).toString());
        info.description = item.value(QStringLiteral("description")).toString();
        info.playCount = item.value(QStringLiteral("play")).toVariant().toLongLong();
        if (!info.bvid.isEmpty())
            results.append(info);
    }
    return true;
}
