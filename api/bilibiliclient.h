#ifndef BILIBILICLIENT_H
#define BILIBILICLIENT_H

#include "common.h"
#include "dlpages/bilitypes.h"

#include <QDateTime>
#include <QMap>
#include <QNetworkCookieJar>

class BilibiliClient : public QObject
{
    Q_OBJECT

public:
    explicit BilibiliClient(QObject *parent = nullptr);

    void initialize();
    void searchVideos(const QString &keyword,
                      int page = 1,
                      int pageSize = 20,
                      const QString &order = QStringLiteral("totalrank"));
    bool isReady() const;

signals:
    void initialized();
    void initializationFailed(QString error);
    void searchFinished(QVector<BiliVideoInfo> results,
                        int page,
                        int totalResults);
    void searchFailed(QString error);

private:
    struct SearchRequest
    {
        QString keyword;
        int page = 1;
        int pageSize = 20;
        QString order;
        bool mayRetry = true;
    };

    QNetworkAccessManager *networkManager = nullptr;
    QNetworkCookieJar *cookieJar = nullptr;
    QString imgKey;
    QString subKey;
    QDateTime keysFetchedAt;
    bool initializing = false;
    QVector<SearchRequest> pendingSearches;

    void fetchBuvid();
    void fetchWbiKeys();
    void performSearch(const SearchRequest &search);
    void failInitialization(const QString &error);
    void flushPendingSearches();

    QNetworkRequest createRequest(const QUrl &url) const;
    QUrl signedUrl(const QUrl &baseUrl,
                   const QMap<QString, QString> &parameters) const;
    QString mixinKey() const;

    static QString keyFromUrl(const QString &url);
    static QString sanitizedValue(QString value);
    static QByteArray encodedQuery(const QMap<QString, QString> &parameters);
    static bool parseSearchResponse(const QByteArray &data,
                                    QVector<BiliVideoInfo> &results,
                                    int &page,
                                    int &totalResults,
                                    bool &signatureRejected,
                                    QString &error);
};

#endif // BILIBILICLIENT_H
