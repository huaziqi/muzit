#include "explorewidget.h"

ExploreWidget::ExploreWidget(QWidget *parent)
    : QWidget{parent}{

    mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setContentsMargins(80, 0, 80, 0);
    manager = new QNetworkAccessManager();
    mainFont = common::vonwaoFont;

    initWeeklyMusic();
}

void ExploreWidget::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
    weeklyMusicWidget->setFixedWidth(this->width() - mainLayout->contentsMargins().left() - mainLayout->contentsMargins().right());

    if(currentRankSongs.empty() | !weeklyMusicLayout) //如果没有初始化好就return
        return;

    currentWeekSongsWidget->setFixedWidth(currentWeekSongsArea->width());
    qDebug() << "in Explore :" << currentWeekSongsWidget->width();

    int column = common::getColumn(currentWeekSongsArea->width());
    if(column == currentGridColumn)
        return;

    currentGridColumn = column;
    rebuildGridLayout();//重构布局

}

void ExploreWidget::initWeeklyMenu()
{
    chooseWeekMenu = new QMenu(this);
    chooseWeekBtn = new QPushButton( QString("第 %1 期 ").arg(weeklyIdVector.last().period) );
    chooseWeekBtn->setMenu(chooseWeekMenu);
    weeklyMusicTopLayout->addWidget(chooseWeekBtn);

    for(int i = 0; i <= (weeklyIdVector.count() - 1) / 10; i ++){
        QMenu* tempMenu = new QMenu(QString("第%1期 ~ 第%2期").arg(i * 10 + 1).arg(qMin(weeklyIdVector.count(), (i + 1) * 10) ) );
        chooseWeekMenu->addMenu(tempMenu);
        for(int j = i * 10; j < qMin(weeklyIdVector.count(), (i + 1) * 10); j ++){
            QAction *action = new QAction(QString("第 %1 期 ").arg(weeklyIdVector.at(j).period));
            connect(action, &QAction::triggered, [j, this](){
                currentWeekId = weeklyIdVector.at(j).ID;
                //qDebug() << j << " " << currentWeekId;
                chooseWeekBtn->setText(QString("第 %1 期 ").arg(weeklyIdVector.at(j).period));
                getCurrentWeekRank();
            });
            tempMenu->addAction(action);
        }
    }
}
void ExploreWidget::rebuildGridLayout()
{
    QLayoutItem* item;
    while ((item = currentWeekSongsLayout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) { //当item->widget是QWidget时进入if，一个语法糖
            currentWeekSongsLayout->removeWidget(widget);
            widget->setParent(nullptr);
        }
        delete item;
    }

// 重新按新的列数添加 widgets
    for (int i = 0; i < currentRankSongs.count(); ++i) {
        auto widget = new MusicItemWidget(currentRankSongs[i]);
        int line = i % currentGridColumn;
        if(currentGridColumn == 1) //currentGridColumn=1时会出问题
            line = 0;
        currentWeekSongsLayout->addWidget(widget, i / currentGridColumn, line);
    }

    currentWeekSongsWidget->adjustSize();
    weeklyMusicWidget->adjustSize();
}


void ExploreWidget::initWeeklyMusic(){
//初始化weeklywidget
    weeklyMusicWidget = new QFrame(this);
    //weeklyMusicWidget->resize(this->width() - mainLayout->contentsMargins().right() * 2- mainLayout->contentsMargins().left() * 2, this->height());
    weeklyMusicWidget->setMinimumHeight(180);
    weeklyMusicWidget->setMaximumHeight(400);
    weeklyMusicWidget->setFixedWidth(this->width() - mainLayout->contentsMargins().left() - mainLayout->contentsMargins().right());


    weeklyMusicWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    weeklyMusicWidget->setFrameShape(QFrame::Box);
    mainLayout->addWidget(weeklyMusicWidget);

    weeklyMusicLayout = new QVBoxLayout(weeklyMusicWidget);
    weeklyMusicLayout->setSpacing(0);


//初始化顶部
    weeklyMusicTopLayout = new QHBoxLayout();
    weeklyMusicLayout->addLayout(weeklyMusicTopLayout);
    weeklyMusicLayout->setContentsMargins(5, 5, 5, 5);
    weeklyMusicLayout->setSpacing(10);
//初始化label
    weeklyInfoLabel = new QLabel("<a href = \"https://music.bilibili.com/pc/rank\">bilibili音乐榜</a>");
    weeklyInfoLabel->setOpenExternalLinks(true);
    weeklyMusicTopLayout->addWidget(weeklyInfoLabel);
    mainFont.setPixelSize(20);
    weeklyInfoLabel->setFont(mainFont);

//获取信息
    getWeeklyHTMLInfo();
}

void ExploreWidget::initCurrentWeekMusic(){
//初始化Layout和Widget
    currentWeekSongsWidget = new QWidget(this);
    currentWeekSongsLayout = new QGridLayout(currentWeekSongsWidget);
    currentWeekSongsLayout->setContentsMargins(0, 0, 0, 0);
    currentWeekSongsLayout->setSpacing(10);
//设置歌曲布局的scrollArea
    currentWeekSongsArea = new QScrollArea();
    currentWeekSongsArea->setContentsMargins(0, 0, 0, 0);
    //currentWeekSongsArea->setHorizontalScrollBar(nullptr);
    currentWeekSongsArea->setWidget(currentWeekSongsWidget);
    currentWeekSongsArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    currentWeekSongsArea->setWidgetResizable(true);
    currentWeekSongsArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    currentWeekSongsArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    weeklyMusicLayout->addWidget(currentWeekSongsArea, 1);

    currentWeekSongsWidget->setFixedWidth(currentWeekSongsArea->width());

//布局已经初始化
    weeklyMusicLayoutInited = true;
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
        currentWeekId = weeklyIdVector.last().ID; //ID是真实的期数而不是index
        initWeeklyMenu();
        getCurrentWeekRank();
        weeklyIdReply->deleteLater();
    });

}

//获取本周榜单音乐信息
void ExploreWidget::getCurrentWeekRank()
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
        if(!weeklyMusicLayoutInited)
            initCurrentWeekMusic();
        currentGridColumn = common::getColumn(this->width());
        rebuildGridLayout();
        currentRankReply->deleteLater();
    });

}
