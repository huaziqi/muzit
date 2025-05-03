#include "explorewidget.h"

ExploreWidget::ExploreWidget(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);
    manager = new QNetworkAccessManager();
    mainFont = common::vonwaoFont;
    initWeeklyMusic();
    getWeeklyHTMLInfo();
}
void ExploreWidget::initWeeklyMusic()
{
    weeklyMusicWidget = new QFrame(this);
    weeklyMusicWidget->setFrameShape(QFrame::Box);
    mainLayout->addWidget(weeklyMusicWidget);
    weeklyMusicLayout = new QVBoxLayout();
    weeklyMusicWidget->setLayout(weeklyMusicLayout);

    weeklyMusicTopLayout = new QHBoxLayout();
    weeklyMusicLayout->addLayout(weeklyMusicLayout);

    weeklyMusicLayout->addLayout(weeklyMusicTopLayout);

    weeklyInfoLabel = new QLabel("bilibili音乐榜");
    weeklyMusicTopLayout->addWidget(weeklyInfoLabel);
    mainFont.setPixelSize(20);
    weeklyInfoLabel->setFont(mainFont);

}



void ExploreWidget::getWeeklyHTMLInfo()
{
    weeklyIdRequest = new QNetworkRequest(QUrl("https://api.bilibili.com/x/copyright-music-publicity/toplist/all_period?list_type=1")); //获取所有的list_id
    weeklyIdReply = manager->get(*weeklyIdRequest);
    connect(weeklyIdReply, &QNetworkReply::downloadProgress, [](qint64 bytesReceived, qint64 bytesTotal){
        if(bytesTotal > 0){
            qDebug() << bytesReceived << "/" << bytesTotal;
            if(bytesReceived == bytesTotal)
                qDebug() << "已完成下载";
        }
    });
    connect(weeklyIdReply, &QNetworkReply::finished, [this]{ //处理获取到的数据
        QByteArray byteArray = weeklyIdReply->readAll();
        QJsonDocument weeklyIdDoc = QJsonDocument::fromJson(byteArray);
        if(weeklyIdDoc.isObject()){
            QJsonObject weeklyObj = weeklyIdDoc.object();
            weeklyObj = weeklyObj["data"].toObject();
            weeklyObj = weeklyObj["list"].toObject();
            for(auto key : weeklyObj.keys()){
                QJsonArray yearlyArray = weeklyObj[key].toArray();
                for(int i = 0; i < yearlyArray.count(); i ++){
                    QJsonObject eachPriod = yearlyArray[i].toObject();
                    weeklyIdVector.append({eachPriod["ID"].toInt(), eachPriod["priod"].toInt(), eachPriod["publish_time"].toInt()});
                }
            }
            std::sort(weeklyIdVector.begin(), weeklyIdVector.end(), [](const WeeklyInfo& a, const WeeklyInfo& b){
                return a.period < b.period;
            });
        }
        weeklyIdReply->deleteLater();
    });



}
