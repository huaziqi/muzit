#ifndef BILIMETADATAPARSER_H
#define BILIMETADATAPARSER_H

#include "metadatatypes.h"

#include <QString>
#include <QVector>

struct BiliMetadataCandidate
{
    AudioMetadata metadata;
    int confidence = 0;
    QString matchedRule;
};

class BiliMetadataParser
{
public:
    // Results are de-duplicated and ordered from most to least likely.
    static QVector<BiliMetadataCandidate> parseCandidates(
        const QString &videoTitle,
        const QString &author,
        const QString &partTitle = QString());

private:
    // Extension points intentionally left for additional matching rules.
    static void appendJapaneseQuoteCandidates(
        QVector<BiliMetadataCandidate> &candidates,
        const QString &sourceTitle,
        const QString &author,
        int sourceBonus);
    static void appendRoleBasedCandidates(
        QVector<BiliMetadataCandidate> &candidates,
        const QString &sourceTitle,
        const QString &author,
        int sourceBonus);
    static void appendSlashSeparatedCandidates(
        QVector<BiliMetadataCandidate> &candidates,
        const QString &sourceTitle,
        const QString &author,
        int sourceBonus);
};

#endif // BILIMETADATAPARSER_H
