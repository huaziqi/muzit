#include "musicitemwidget.h"

MusicItemWidget::MusicItemWidget(MusicItem* _musicItem, QWidget *parent)
    : QWidget{parent}, infoMinHeight(110)
{
    this->setAttribute(Qt::WA_StyledBackground);

    labelFont = common::vonwaoFont;
    labelFont.setPixelSize(13);

    musicItem = _musicItem;
    musicItem->setWidget(this);
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 10);

    coverResizeTimer = new QTimer(this);
    coverResizeTimer->setSingleShot(true);
    coverResizeTimer->setInterval(80);
    connect(coverResizeTimer, &QTimer::timeout,
            this, &MusicItemWidget::updateCoverPixmap);

    manager = new QNetworkAccessManager(this);

    //this->setStyleSheet("background-color: #000000;");
    gotCover();
}

MusicItemWidget::~MusicItemWidget(){

}

void MusicItemWidget::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);

    const QMargins margins = mainLayout->contentsMargins();
    const int infoWidth = infoWidget ? infoWidget->width() : 200;
    pendingCoverWidth = qMax(1, width() - infoWidth - margins.left()
                                  - margins.right() - mainLayout->spacing());

    if (!originCoverPixmap.isNull() && qAbs(lastWidth - pendingCoverWidth) > 8)
        coverResizeTimer->start();
}

void MusicItemWidget::updateCoverPixmap()
{
    if (!coverLabel || originCoverPixmap.isNull() || pendingCoverWidth <= 0)
        return;

    coverPixMap = originCoverPixmap.scaled(
        pendingCoverWidth,
        qMax(1, static_cast<int>(pendingCoverWidth * aspectRadio)),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    coverLabel->setPixmap(coverPixMap);
    lastWidth = pendingCoverWidth;
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
        coverReply->deleteLater();
    });
}

void MusicItemWidget::initLayout(){
    coverLabel = new QLabel();
    coverPixMap = QPixmap(coverFileName).scaled(197, 197 * aspectRadio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    lastWidth = 197;
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
    pendingCoverWidth = qMax(1, width() - infoWidget->width()
                                  - mainLayout->contentsMargins().left()
                                  - mainLayout->contentsMargins().right()
                                  - mainLayout->spacing());
    coverResizeTimer->start();

}

void MusicItemWidget::initInfo()
{
    infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setSpacing(2);
    infoLayout->setContentsMargins(0, 10, 0, 5);

    titleLabel = new QLabel(musicItem->title);
    titleLabel->setFont(labelFont);
    titleLabel->setFixedWidth(170);
    titleLabel->setWordWrap(true);
    titleLabel->setAlignment(Qt::AlignTop);
    authorLabel = new QLabel(musicItem->author);
    authorLabel->setFont(labelFont);
    authorLabel->setMargin(0);
    playedNumLabel = new QLabel(QString::number(musicItem->playedNum));
    authorLabel->setFont(labelFont);
    playedNumLabel->setMargin(0);

    infoLayout->addWidget(titleLabel);
    infoLayout->addStretch(5);
    infoLayout->addWidget(authorLabel);
    infoLayout->addWidget(playedNumLabel);
}

