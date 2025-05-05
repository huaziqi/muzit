#include "musicitemwidget.h"

MusicItemWidget::MusicItemWidget(MusicItem* _musicItem, QWidget *parent)
    : QWidget{parent}
{
    musicItem = _musicItem;
    musicItem->setWidget(this);
    mainLayout = new QHBoxLayout();
    this->setLayout(mainLayout);
    manager = new QNetworkAccessManager;
    this->setFixedHeight(200);
    this->setFixedWidth(400);
    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("background-color: #dddddd;");
    gotCover();
}

MusicItemWidget::~MusicItemWidget(){

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
        coverFileName = fileName;

        QFile coverFile(fileName);
        if(!coverFile.exists()){
            if(coverFile.open(QIODeviceBase::WriteOnly)){
                coverFile.write(coverReply->readAll());
            }
        }
        initLayout();
    });
}

void MusicItemWidget::initLayout(){
    coverLabel = new QLabel();
    coverPixMap = QPixmap(coverFileName);
    coverPixMap = coverPixMap.scaled(300, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    mainLayout->addWidget(coverLabel);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    QPainter painter(&coverPixMap);
    int duration = musicItem->duration;
    auto durationString = [](const int& duration){

    };

    coverLabel->setPixmap(coverPixMap);
}
