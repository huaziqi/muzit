#include "musicitemwidget.h"

MusicItemWidget::MusicItemWidget(MusicItem* _musicItem, QWidget *parent)
    : QWidget{parent}
{
    musicItem = _musicItem;
    musicItem->setWidget(this);
    mainLayout = new QHBoxLayout();
    manager = new QNetworkAccessManager;
    this->setFixedHeight(200);
    this->setFixedWidth(300);
    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("background-color: black;");
    gotCover();
}

void MusicItemWidget::gotCover()
{
    coverRequest = new QNetworkRequest(QUrl(musicItem->coverUrl));
    coverReply = manager->get(*coverRequest);
    connect(coverReply, &QNetworkReply::downloadProgress, [](qint64 bytesReceived, qint64 bytesTotal){

    });
    connect(coverReply, &QNetworkReply::finished, [=]{
        if(!QDir(common::cachePath + "/muzit").exists()){
            QDir().mkdir(common::cachePath + "/muzit");
        }
        QString dirPath = common::cachePath + "/muzit/cover";
        if(!QDir(dirPath).exists()){
            QDir().mkdir(dirPath);
        }
        if(!QDir(dirPath).exists()){
            qDebug() << "创建失败";
            return;
        }
        QString fileName = dirPath + "/" + QCryptographicHash::hash(musicItem->coverUrl.toUtf8(), QCryptographicHash::Md5).toHex();
        QFile coverFile(fileName);
        if(!coverFile.exists()){
            qDebug() << 1;
            if(coverFile.open(QIODeviceBase::WriteOnly)){
                coverFile.write(coverReply->readAll());
            }
        }

    });
}
