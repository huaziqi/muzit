#include "bilidltool.h"

#include <QRegularExpression>
#include <algorithm>

namespace {

QString outputSuffix(AudioOutputFormat format)
{
    switch (format) {
    case AudioOutputFormat::M4a:
        return QStringLiteral("m4a");
    case AudioOutputFormat::Mp3:
        return QStringLiteral("mp3");
    case AudioOutputFormat::Flac:
        return QStringLiteral("flac");
    }

    return QStringLiteral("m4a");
}

}

BiliDLTool::BiliDLTool(
    DownloadManager *_downloadManager,
    QObject *parent)
    : QObject{parent}, downloadManager(_downloadManager)
{
}

void BiliDLTool::getVideoInfo(const QString &bvid)
{
    QNetworkReply *reply =
        downloadManager->fetch(BilibiliApi::videoInfoRequest(bvid));

    connect(reply, &QNetworkReply::finished, this, [this, reply]{
        if (reply->error() != QNetworkReply::NoError) {
            emit videoInfoFailed(reply->errorString());
            reply->deleteLater();
            return;
        }

        BiliVideoInfo info;
        QString error;
        if (!ApiParser::parseVideoInfo(reply->readAll(), info, error)) {
            emit videoInfoFailed(error);
            reply->deleteLater();
            return;
        }

        reply->deleteLater();
        emit videoInfoReady(info);
    });
}

void BiliDLTool::getPartAudioStreams(BiliPlayUrlInfo playUrlInfo, QString bvid, qint64 index)
{
    QNetworkReply *reply = downloadManager->fetch(
        BilibiliApi::playUrlRequest(bvid, playUrlInfo.cid));

    connect(reply, &QNetworkReply::finished, this,
            [this, reply, playUrlInfo, index]() mutable {
        if (reply->error() != QNetworkReply::NoError) {
            emit partAudioStreamsFailed(playUrlInfo.cid, reply->errorString());
            reply->deleteLater();
            return;
        }

        QString error;
        if (!ApiParser::parsePlayUrlInfo(
                reply->readAll(), playUrlInfo, error)) {
            emit partAudioStreamsFailed(playUrlInfo.cid, error);
            reply->deleteLater();
            return;
        }

        reply->deleteLater();
        emit partAudioStreamsReady(playUrlInfo, index);
    });
}

bool BiliDLTool::createAudioDownloadJob(
    const BiliVideoInfo &videoInfo,
    const BiliPlayUrlInfo &part,
    const BiliSaveSettings &settings,
    const AudioMetadata &metadata,
    AudioDownloadJob &job,
    QString &error) const
{
    if (metadata.title.trimmed().isEmpty()
        || metadata.artist.trimmed().isEmpty()) {
        error = QStringLiteral("歌曲名和歌手不能为空");
        return false;
    }

    const int qualityId = settings.quality.toInt();
    const auto audioIt = std::find_if(
        part.audioStreams.cbegin(),
        part.audioStreams.cend(),
        [qualityId](const BiliAudioStream &audio) {
            return audio.id == qualityId;
        });
    if (audioIt == part.audioStreams.cend()) {
        error = QStringLiteral("所选音质不可用");
        return false;
    }

    QString fileName = settings.fileNameTemplate;
    fileName.replace(QStringLiteral("{title}"), metadata.title.trimmed());
    fileName.replace(QStringLiteral("{author}"), metadata.artist.trimmed());

    const int selectedPartCount = std::count_if(
        videoInfo.parts.cbegin(),
        videoInfo.parts.cend(),
        [](const BiliPlayUrlInfo &item) { return item.selected; });
    if (selectedPartCount > 1 || videoInfo.parts.size() > 1) {
        fileName += QStringLiteral(" - P%1 %2")
                        .arg(part.page)
                        .arg(part.title);
    }

    fileName.replace(QRegularExpression(R"([<>:"/\\|?*])"), "_");
    fileName = fileName.trimmed();
    if (fileName.isEmpty()) {
        fileName = videoInfo.bvid
            + QStringLiteral("-P%1").arg(part.page);
    }

    job = AudioDownloadJob{};
    job.bvid = videoInfo.bvid;
    job.cid = part.cid;
    job.page = part.page;
    job.source = *audioIt;
    job.metadata = metadata;
    if (job.metadata.coverUrl.isEmpty())
        job.metadata.coverUrl = videoInfo.coverUrl;
    job.outputFormat = settings.outputFormat;

    const QString temporaryDirectory =
        QDir(settings.savePath).filePath(QStringLiteral("temp"));
    job.temporaryPath =
        QDir(temporaryDirectory).filePath(fileName + QStringLiteral(".m4s"));
    job.outputPath = QDir(settings.savePath).filePath(
        fileName + QStringLiteral(".") + outputSuffix(job.outputFormat));
    return true;
}

DownloadTask *BiliDLTool::downloadAudio(const AudioDownloadJob &job)
{
    return downloadManager->addTask(
        BilibiliApi::audioDownloadRequest(QUrl(job.source.url)),
        job.temporaryPath);
}
