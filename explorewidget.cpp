#include "explorewidget.h"

ExploreWidget::ExploreWidget(QWidget *parent)
    : QWidget{parent}{
    mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);
    mainLayout->setAlignment(Qt::AlignTop);

    minWeeklyMusicLayoutHeight = 200, maxWeeklyMusicLayoutHeight = 350;

    manager = new QNetworkAccessManager();
    mainFont = common::vonwaoFont;
    initWeeklyMusic();
    mainLayout->addStretch(1);
}

void ExploreWidget::resizeEvent(QResizeEvent *event){
    int height = qMin(maxWeeklyMusicLayoutHeight, qMax(minWeeklyMusicLayoutHeight, this->width() / 3));
    weeklyMusicWidget->resize(this->width() - mainLayout->contentsMargins().right()- mainLayout->contentsMargins().left(), height);
    QWidget::resizeEvent(event);
}

void ExploreWidget::initWeeklyMusic(){
//初始化整体布局
    weeklyMusicWidget = new QFrame(this);
    weeklyMusicWidget->setMinimumHeight(minWeeklyMusicLayoutHeight);
    weeklyMusicWidget->setMaximumHeight(maxWeeklyMusicLayoutHeight);
    int height = qMin(maxWeeklyMusicLayoutHeight, qMax(minWeeklyMusicLayoutHeight, this->width() / 3));
    weeklyMusicWidget->resize(this->width() - mainLayout->contentsMargins().right() * 2- mainLayout->contentsMargins().left() * 2, height);
    weeklyMusicWidget->setFrameShape(QFrame::Box);
    mainLayout->addWidget(weeklyMusicWidget);
    weeklyMusicLayout = new QVBoxLayout();
    weeklyMusicLayout->setSpacing(10);
    weeklyMusicWidget->setLayout(weeklyMusicLayout);
//初始化顶部
    weeklyMusicTopLayout = new QHBoxLayout();
    weeklyMusicLayout->addLayout(weeklyMusicTopLayout);
    weeklyMusicLayout->setContentsMargins(0, 0, 0, 0);
    weeklyMusicLayout->setSpacing(0);
//初始化label
    weeklyInfoLabel = new QLabel("<a href = \"https://music.bilibili.com/pc/rank\">bilibili音乐榜</a>");
    weeklyInfoLabel->setOpenExternalLinks(true);
    weeklyMusicTopLayout->addWidget(weeklyInfoLabel);
    mainFont.setPixelSize(20);
    weeklyInfoLabel->setFont(mainFont);

//初始化menu
    chooseWeekMenu = new QMenu();
    //chooseWeekMenu->addAction(new QAction("hello"));
    weeklyMusicTopLayout->addWidget(chooseWeekMenu);
    getWeeklyHTMLInfo();

}

void ExploreWidget::initCurrentWeekMusic(){
    //初始化
    currentWeekSongsWidget = new QWidget();
    currentWeekSongsLayout = new QHBoxLayout();
    currentWeekSongsArea = new QScrollArea();

    currentWeekSongsWidget->setLayout(currentWeekSongsLayout);
    currentWeekSongsArea->setWidget(currentWeekSongsWidget);
    currentWeekSongsArea->setWidgetResizable(true);
    currentWeekSongsArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    currentWeekSongsArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    weeklyMusicLayout->addWidget(currentWeekSongsArea);
    for(int i = 0; i < currentRankSongs.count(); i ++){
        currentWeekSongsLayout->addWidget(new MusicItemWidget(currentRankSongs[i]));
    }

}

void ExploreWidget::getWeeklyHTMLInfo(){
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
        currentWeekId = weeklyIdVector.last().ID;
        getCurrentWeekRank();
        weeklyIdReply->deleteLater();
    });

}

void ExploreWidget::getCurrentWeekRank() //获取本周榜单音乐信息
{
    if(currentWeekId == -1)
        return;
    while(!currentRankSongs.empty()){
        MusicItem* tempItem = currentRankSongs.last();
        currentRankSongs.pop_back();
        delete tempItem;
    }
    currentRankRequest = new QNetworkRequest(QUrl("https://api.bilibili.com/x/copyright-music-publicity/toplist/music_list?list_id=" + QString::number(currentWeekId)));
    currentRankReply = manager->get(*currentRankRequest);
    connect(currentRankReply, &QNetworkReply::downloadProgress, [](qint64 bytesReceived, qint64 bytesTotal){
        if(bytesTotal > 0){
            qDebug() << bytesReceived << "/" << bytesTotal;
            if(bytesReceived == bytesTotal)
                qDebug() << "已完成下载";
        }
    });
    connect(currentRankReply, &QNetworkReply::finished, [this]{
        QByteArray byteArray = currentRankReply->readAll();
        QJsonDocument currentRankInfo = QJsonDocument::fromJson(byteArray);
        if(currentRankInfo.isObject()){
            QJsonObject weekObj = currentRankInfo.object();
            weekObj = weekObj["data"].toObject();
            QJsonArray weekArray = weekObj["list"].toArray();
            for(int i = 0; i < weekArray.count(); i ++){
                QJsonObject songObj = weekArray[i].toObject();
                currentRankSongs.append(new MusicItem(songObj["creation_bvid"].toString(), songObj["creation_title"].toString(), songObj["creation_nickname"].toString(),\
                            songObj["creation_cover"].toString(),songObj["creation_play"].toInt(), songObj["creation_duration"].toInt(), songObj["rank"].toInt()));
            }
        }
        initCurrentWeekMusic();
        currentRankReply->deleteLater();
    });

}
