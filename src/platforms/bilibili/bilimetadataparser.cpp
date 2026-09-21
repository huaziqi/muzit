#include "bilimetadataparser.h"

#include <QRegularExpression>

#include <algorithm>

namespace {


QString normolizeWhiteSpace(QString text)
{
    text.replace(QChar(0x3000), QLatin1Char(' ')); //将全角空格转换为半角
    return text.simplified(); //将换行，缩进和连续空格变成一个空格
}

QString withoutEdgeSeparators(QString text)
{
    static const QRegularExpression leading(
        QStringLiteral(R"(^\s*[-–—|｜:：·/]+\s*)"));
    static const QRegularExpression trailing(
        QStringLiteral(R"(\s*[-–—|｜:：·/]+\s*$)"));
    text.remove(leading);
    text.remove(trailing);
    return normolizeWhiteSpace(text);
}

QString cleanTitle(QString text)
{
    text = normolizeWhiteSpace(text);

    // Remove only decorations whose contents clearly describe the video,
    // preserving brackets that may be part of the actual song name.
    static const QString noise = QStringLiteral(
        R"(official|music\s*video|mv|pv|动态歌词|歌词版|完整版|纯享版?|现场版?|live|4k|8k|高清|无损|高音质|中字|中英字幕|字幕版|自制|搬运|补档)"
    );
    const QList<QRegularExpression> decorations = {
        QRegularExpression(QStringLiteral(R"(【[^】]*(?:%1)[^】]*】)").arg(noise),
                           QRegularExpression::CaseInsensitiveOption),
        QRegularExpression(QStringLiteral(R"(\[[^\]]*(?:%1)[^\]]*\])").arg(noise),
                           QRegularExpression::CaseInsensitiveOption),
        QRegularExpression(QStringLiteral(R"(（[^）]*(?:%1)[^）]*）)").arg(noise),
                           QRegularExpression::CaseInsensitiveOption),
        QRegularExpression(QStringLiteral(R"(\([^)]*(?:%1)[^)]*\))").arg(noise),
                           QRegularExpression::CaseInsensitiveOption)
    };
    for (const QRegularExpression &decoration : decorations)
        text.remove(decoration);

    static const QRegularExpression leadingPart(
        QStringLiteral(R"(^\s*(?:P|p|第)\s*\d+\s*(?:集|首|段)?\s*[-:：.]?\s*)"));
    text.remove(leadingPart);

    static const QRegularExpression trailingNoise(
        QStringLiteral(
            R"(\s*(?:[-|｜]\s*)?(?:official\s*(?:music\s*)?video|music\s*video|动态歌词|歌词版|完整版|纯享版?|MV|PV)\s*$)"),
        QRegularExpression::CaseInsensitiveOption);
    text.remove(trailingNoise);
    return withoutEdgeSeparators(text);
}

QString cleanField(QString text)
{
    text = cleanTitle(text);
    static const QRegularExpression label(
        QStringLiteral(R"(^\s*(?:歌名|歌曲名|曲名|歌手|艺人|演唱|artist|title)\s*[:：]\s*)"),
        QRegularExpression::CaseInsensitiveOption);
    text.remove(label);
    return withoutEdgeSeparators(text);
}

QString comparisonKey(QString text)
{
    text = normolizeWhiteSpace(text).toCaseFolded();
    static const QRegularExpression punctuation(
        QStringLiteral(R"([\s\p{P}\p{S}]+)"));
    text.remove(punctuation);
    return text;
}

bool resemblesAuthor(const QString &text, const QString &author)
{
    const QString textKey = comparisonKey(text);
    const QString authorKey = comparisonKey(author);
    return !textKey.isEmpty() && !authorKey.isEmpty()
        && (textKey == authorKey
            || textKey.contains(authorKey)
            || authorKey.contains(textKey));
}

void appendCandidate(QVector<BiliMetadataCandidate> &candidates,
                     QString title,
                     QString artist,
                     int confidence,
                     const QString &matchedRule)
{
    title = cleanField(title);
    artist = cleanField(artist);
    if (title.isEmpty() || artist.isEmpty())
        return;

    const QString titleKey = comparisonKey(title);
    const QString artistKey = comparisonKey(artist);
    for (BiliMetadataCandidate &candidate : candidates) {
        if (comparisonKey(candidate.metadata.title) == titleKey
            && comparisonKey(candidate.metadata.artist) == artistKey) {
            if (confidence > candidate.confidence) {
                candidate.confidence = confidence;
                candidate.matchedRule = matchedRule;
            }
            return;
        }
    }

    BiliMetadataCandidate candidate;
    candidate.metadata.title = title;
    candidate.metadata.artist = artist;
    candidate.confidence = std::clamp(confidence, 0, 100);
    candidate.matchedRule = matchedRule;
    candidates.append(candidate);
}

void appendLabelledCandidate(QVector<BiliMetadataCandidate> &candidates,
                             const QString &source,
                             int sourceBonus)
{
    static const QRegularExpression titleThenArtist(
        QStringLiteral(
            R"((?:歌名|歌曲名|曲名|title)\s*[:：]\s*(.+?)\s+(?:歌手|艺人|演唱|artist)\s*[:：]\s*(.+)$)"),
        QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression artistThenTitle(
        QStringLiteral(
            R"((?:歌手|艺人|演唱|artist)\s*[:：]\s*(.+?)\s+(?:歌名|歌曲名|曲名|title)\s*[:：]\s*(.+)$)"),
        QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = titleThenArtist.match(source);
    if (match.hasMatch()) {
        appendCandidate(candidates, match.captured(1), match.captured(2),
                        98 + sourceBonus, QStringLiteral("explicit-labels"));
        return;
    }

    match = artistThenTitle.match(source);
    if (match.hasMatch()) {
        appendCandidate(candidates, match.captured(2), match.captured(1),
                        98 + sourceBonus, QStringLiteral("explicit-labels"));
    }
}

void appendBookTitleCandidate(QVector<BiliMetadataCandidate> &candidates,
                              const QString &source,
                              const QString &author,
                              int sourceBonus)
{
    static const QRegularExpression quoted(QStringLiteral(R"(《([^》]+)》)"));
    const QRegularExpressionMatch match = quoted.match(source);
    if (!match.hasMatch())
        return;

    const QString songTitle = match.captured(1);
    const QString before = withoutEdgeSeparators(source.left(match.capturedStart()));
    const QString after = withoutEdgeSeparators(source.mid(match.capturedEnd()));

    if (!before.isEmpty())
        appendCandidate(candidates, songTitle, before, 92 + sourceBonus,
                        QStringLiteral("book-title-prefix"));
    if (!after.isEmpty())
        appendCandidate(candidates, songTitle, after, 90 + sourceBonus,
                        QStringLiteral("book-title-suffix"));
    appendCandidate(candidates, songTitle, author, 84 + sourceBonus,
                    QStringLiteral("book-title-author"));
}

void appendSeparatedCandidates(QVector<BiliMetadataCandidate> &candidates,
                               const QString &source,
                               const QString &author,
                               int sourceBonus)
{
    // Slash is deliberately excluded because it often appears inside titles.
    // Its more conservative rule is one of the extension points below.
    static const QRegularExpression separator(
        QStringLiteral(R"(\s*[-–—|｜]\s*|\s+·\s+)"));
    const QRegularExpressionMatch match = separator.match(source);
    if (!match.hasMatch())
        return;

    const QString left = cleanField(source.left(match.capturedStart()));
    const QString right = cleanField(source.mid(match.capturedEnd()));
    if (left.isEmpty() || right.isEmpty())
        return;

    const bool leftIsAuthor = resemblesAuthor(left, author);
    const bool rightIsAuthor = resemblesAuthor(right, author);

    int artistTitleScore = 78 + sourceBonus;
    int titleArtistScore = 74 + sourceBonus;
    if (leftIsAuthor)
        artistTitleScore += 12;
    if (rightIsAuthor)
        titleArtistScore += 12;

    appendCandidate(candidates, right, left, artistTitleScore,
                    QStringLiteral("separator-artist-title"));
    appendCandidate(candidates, left, right, titleArtistScore,
                    QStringLiteral("separator-title-artist"));
}

} // namespace

QVector<BiliMetadataCandidate> BiliMetadataParser::parseCandidates(
    const QString &videoTitle,
    const QString &author,
    const QString &partTitle)
{
    QVector<BiliMetadataCandidate> candidates;
    const QString cleanAuthor = cleanField(author);

    struct Source {
        QString title;
        int bonus;
    };
    QVector<Source> sources;
    if (!partTitle.trimmed().isEmpty())
        sources.append(Source{partTitle, 2});
    if (!videoTitle.trimmed().isEmpty()
        && comparisonKey(videoTitle) != comparisonKey(partTitle)) {
        sources.append(Source{videoTitle, 0});
    }

    for (const Source &source : sources) {
        const QString cleaned = cleanTitle(source.title);
        if (cleaned.isEmpty())
            continue;

        appendLabelledCandidate(candidates, cleaned, source.bonus);
        appendBookTitleCandidate(candidates, cleaned, cleanAuthor, source.bonus);
        appendSeparatedCandidates(candidates, cleaned, cleanAuthor, source.bonus);

        appendJapaneseQuoteCandidates(
            candidates, cleaned, cleanAuthor, source.bonus);
        appendRoleBasedCandidates(
            candidates, cleaned, cleanAuthor, source.bonus);
        appendSlashSeparatedCandidates(
            candidates, cleaned, cleanAuthor, source.bonus);

        appendCandidate(candidates, cleaned, cleanAuthor, 42 + source.bonus,
                        source.bonus > 0
                            ? QStringLiteral("part-title-fallback")
                            : QStringLiteral("video-title-fallback"));
    }

    std::stable_sort(candidates.begin(), candidates.end(),
                     [](const BiliMetadataCandidate &left,
                        const BiliMetadataCandidate &right) {
        return left.confidence > right.confidence;
    });
    return candidates;
}

void BiliMetadataParser::appendJapaneseQuoteCandidates(
    QVector<BiliMetadataCandidate> &candidates,
    const QString &sourceTitle,
    const QString &author,
    int sourceBonus)
{
    Q_UNUSED(candidates)
    Q_UNUSED(sourceTitle)
    Q_UNUSED(author)
    Q_UNUSED(sourceBonus)
    // TODO: Parse forms such as YOASOBI「アイドル」 and 『歌名』／歌手.
}

void BiliMetadataParser::appendRoleBasedCandidates(
    QVector<BiliMetadataCandidate> &candidates,
    const QString &sourceTitle,
    const QString &author,
    int sourceBonus)
{
    Q_UNUSED(candidates)
    Q_UNUSED(sourceTitle)
    Q_UNUSED(author)
    Q_UNUSED(sourceBonus)
    // TODO: Parse 原唱/翻唱/演唱/feat. and emit both original and cover artists.
}

void BiliMetadataParser::appendSlashSeparatedCandidates(
    QVector<BiliMetadataCandidate> &candidates,
    const QString &sourceTitle,
    const QString &author,
    int sourceBonus)
{
    Q_UNUSED(candidates)
    Q_UNUSED(sourceTitle)
    Q_UNUSED(author)
    Q_UNUSED(sourceBonus)
    // TODO: Split spaced '/' or '／' without breaking titles that contain a slash.
}
