#include "musicitemwidget.h"

MusicItemWidget::MusicItemWidget(MusicItem* _musicItem, QWidget *parent)
    : QWidget{parent}, minWidth(300), maxWidth(500)
{
    musicItem = _musicItem;
    musicItem->setWidget(this);
    mainLayout = new QHBoxLayout();
    this->setLayout(mainLayout);
    manager = new QNetworkAccessManager;
    this->resize(400, 200);
    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("background-color: #dddddd;");
    gotCover();
}

MusicItemWidget::~MusicItemWidget(){

}

void MusicItemWidget::resizeEvent(QResizeEvent *event){
    int parentWidth = this->parentWidget()->parentWidget()->width();
    int width = qMin(maxWidth, qMax(minWidth, parentWidth / 4));
    this->resize(width, width / 2);
    if(!coverPixMap.isNull()){
        if(qAbs(lastWidth - width) / lastWidth > 0.1){
            coverPixMap = originCoverPixmap.scaled(width, width / 2, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            coverLabel->setPixmap(coverPixMap);
            lastWidth = width;
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
    coverPixMap = QPixmap(coverFileName).scaled(300, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    lastWidth = 300;
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
    originCoverPixmap = coverPixMap;

    mainLayout->addWidget(coverLabel);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignLeft);
}

