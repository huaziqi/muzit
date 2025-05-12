#include "musicitemwidget.h"

MusicItemWidget::MusicItemWidget(MusicItem* _musicItem, QWidget *parent)
    : QWidget{parent}, pixMinWidth(300), pixMaxWidth(500)
{
    musicItem = _musicItem;
    musicItem->setWidget(this);
    mainLayout = new QHBoxLayout(this);
    manager = new QNetworkAccessManager;
    this->setAttribute(Qt::WA_StyledBackground);
    //this->setStyleSheet("background-color: #000000;");
    gotCover();
}

MusicItemWidget::~MusicItemWidget(){

}

void MusicItemWidget::resizeEvent(QResizeEvent *event){
    int parentWidth = this->parentWidget()->parentWidget()->width();

    int lineLabelNum = common::getColumn(parentWidth);
    int pixWidth = (parentWidth - 200 * lineLabelNum) / lineLabelNum;
    this->resize(pixWidth + 200, pixWidth * aspectRadio);
    if(!coverPixMap.isNull()){
        if(qAbs(lastWidth - pixWidth) > 15){
            coverPixMap = originCoverPixmap.scaled(pixWidth, pixWidth * aspectRadio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            coverLabel->setPixmap(coverPixMap);
            lastWidth = pixWidth;
        }
    }
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
        coverFileName = dirPath + "/" + QCryptographicHash::hash(musicItem->coverUrl.toUtf8(), QCryptographicHash::Md5).toHex();

        QFile coverFile(coverFileName);
        if(!coverFile.exists()){
            if(coverFile.open(QIODeviceBase::WriteOnly)){
                coverFile.write(coverReply->readAll());
                coverFile.close();
            }
        }
        initLayout();
    });
}

void MusicItemWidget::initLayout(){
    coverLabel = new QLabel();
    coverPixMap = QPixmap(coverFileName).scaled(200, 200 * aspectRadio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    lastWidth = 200;
    QPainter painter(&coverPixMap);
    painter.setRenderHint(QPainter::Antialiasing); //抗锯齿

    auto getDurationString = [](int duration) -> QString{
        int second = duration % 60;
        duration /= 60;
        if(duration == 0){
            return "00:" + QString::number(second).rightJustified(2, '0');
        }
        else{
            int minutes = duration % 60;
            duration /= 60;
            if(duration == 0)
                return QString::number(minutes) + ":" + QString::number(second).rightJustified(2, '0');
            else
                return QString::number(duration) + ":" + QString::number(minutes).rightJustified(2, '0') + QString::number(second).rightJustified(2, '0');
        }
    };
    int padding = 5;//内部间距
    int margin = 10;

    QString durationString = getDurationString(musicItem->duration);
    QFont font("VonwaonBitmap 16px", 12, QFont::Bold);
    painter.setFont(font);
    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(durationString);
    int textHeight = fm.height();

    QRect textRect(coverPixMap.width() - (margin + padding * 2 + textWidth),
                   coverPixMap.height() - (margin + padding * 2 + textHeight),
                   textWidth + padding * 2,
                   textHeight + padding * 2);

    painter.setBrush(QBrush(QColor(0, 0, 0, 122)));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(textRect, 3, 3);

    painter.setPen(Qt::white);
    painter.drawText(textRect, Qt::AlignCenter,durationString);
    painter.end();

    coverLabel->setPixmap(coverPixMap);
    coverLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    originCoverPixmap = coverPixMap;

    mainLayout->addWidget(coverLabel, 1);

    infoWidget = new QWidget();
    infoWidget->setFixedWidth(200);
    mainLayout->addWidget(infoWidget);
    initInfo();
}

void MusicItemWidget::initInfo()
{
    infoLayout = new QVBoxLayout(infoWidget);
    titleLabel = new QLabel(musicItem->title);
    titleLabel->setFixedWidth(170);
    titleLabel->setWordWrap(true);
    authorLabel = new QLabel(musicItem->author);
    playedNumLabel = new QLabel(QString::number(musicItem->playedNum));
    infoLayout->addWidget(titleLabel);
    infoLayout->addStretch(5);
    infoLayout->addWidget(authorLabel);
    infoLayout->addWidget(playedNumLabel);
}

