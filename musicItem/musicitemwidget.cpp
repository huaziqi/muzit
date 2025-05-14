#include "musicitemwidget.h"

MusicItemWidget::MusicItemWidget(MusicItem* _musicItem, QWidget *parent)
    : QWidget{parent}, infoMinHeight(110)
{
    this->setAttribute(Qt::WA_StyledBackground);

    labelFont = common::vonwaoFont;
    labelFont.setPixelSize(10);

    musicItem = _musicItem;
    musicItem->setWidget(this);
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 0);

    manager = new QNetworkAccessManager;

    //this->setStyleSheet("background-color: #000000;");
    gotCover();
}

MusicItemWidget::~MusicItemWidget(){

}

void MusicItemWidget::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);

    int parentWidth = this->parentWidget()->parentWidget()->width();
    // qDebug() << this->parentWidget()->parentWidget()->parentWidget();

    int lineLabelNum = common::getColumn(parentWidth);
    // qDebug() << "in music item:" << parentWidth;
    int pixWidth = parentWidth / lineLabelNum - 200;
    this->resize(pixWidth + 200, qMax(infoMinHeight, static_cast<int>(pixWidth * aspectRadio)));
    if(!coverPixMap.isNull()){
        if(qAbs(lastWidth - pixWidth) > 8){
            coverPixMap = originCoverPixmap.scaled(pixWidth, pixWidth * aspectRadio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            coverLabel->setPixmap(coverPixMap);
            lastWidth = pixWidth;
        }
    }
    // qDebug() << parentWidth;
    // qDebug() << "resized: " << this->width() <<" " << pixWidth;
    // qDebug() << "";
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
    coverPixMap = QPixmap(coverFileName).scaled(197, 197 * aspectRadio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
    infoLayout->setSpacing(2);
    infoLayout->setContentsMargins(0, 10, 0, 5);

    titleLabel = new QLabel(musicItem->title);
    titleLabel->setFixedWidth(170);
    titleLabel->setWordWrap(true);
    authorLabel = new QLabel(musicItem->author);
    authorLabel->setMargin(0);
    playedNumLabel = new QLabel(QString::number(musicItem->playedNum));
    playedNumLabel->setMargin(0);

    infoLayout->addWidget(titleLabel);
    infoLayout->addStretch(5);
    infoLayout->addWidget(authorLabel);
    infoLayout->addWidget(playedNumLabel);
}

