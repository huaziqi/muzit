#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QPushButton>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->shareWidget = new ShareWidget;//实例化
    this->playerWidget = new PlayerWidget;
    connect(playerWidget, SIGNAL(sendUrl(QList<MusicInfo>,QDir)), this, SLOT(receiveUrl(QList<MusicInfo>, QDir)));
    this->exploreWidget = new ExploreWidget;

    ui->stackedWidget->addWidget(this->exploreWidget);
    ui->stackedWidget->addWidget(this->playerWidget);
    ui->stackedWidget->addWidget(this->shareWidget);
    connect(ui->exploreMusicBtn, &QPushButton::clicked, this,  [=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->myMusicBtn, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->shareMusicBtn, &QPushButton::clicked,this,  [=](){
        ui->stackedWidget->setCurrentIndex(2);
    });
    audioOutput = new QAudioOutput(this);
    mediaPlayer = new QMediaPlayer(this);

    mediaPlayer->setAudioOutput(audioOutput);
    ui->playMusicBtn->setFixedSize(36, 36);
    ui->last_music->setFixedSize(36, 36);
    ui->next_music->setFixedSize(36, 36);
    ui->play_lists->setFixedSize(36, 36);
    ui->play_mode->setFixedSize(36, 36);
    setPlayer();

}

MainWidget::~MainWidget(){
    delete ui;
}

void MainWidget::receiveUrl(QList<MusicInfo> newMusics, QDir musicDir){
    for(int i = 0; i < newMusics.count(); i ++){
        for(int j = 0; j < playLists.count(); j ++)
            if(playLists.value(j).path == musicDir.path() + '/' + newMusics.value(i).path){
                playLists.removeAt(j);
                break;
            }
    }
    for(int i = 0; i < newMusics.count(); i ++){
        MusicInfo temInfo =  newMusics.value(i);
        temInfo.path = musicDir.path() + '/' + newMusics.value(i).path;
        playLists.insert(i, temInfo);
    }

    inListNum = playLists.count();
    musicIndex = 0;
    mediaPlayer->setSource(playLists.value(musicIndex).path);
    mediaPlayer->play();
    ui->playingMusic->setText(playLists.value(musicIndex).name);
    playStatus = true;
}

void MainWidget::setPlayer(){ //初始化播放器
    ui->audioSlider->setRange(0, 100);
    ui->audioSlider->setValue(50);
    audioOutput->setVolume(0.5);
    connect(ui->audioSlider, &QSlider::sliderMoved, this, &MainWidget::changeVolume);
    connect(mediaPlayer,&QMediaPlayer::mediaStatusChanged ,this,[=](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia && playStatus)
            nextMusic();
    });
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [=](qint64 duration){
        ui->total_time->setText(QString("%1:%2").arg(duration/1000/60).arg(duration/1000%60, 2, 10, QChar('0')));
        ui->musicSlider->setRange(0, duration);
    });
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [=](qint64 position){
        ui->current_time->setText(QString("%1:%2").arg(position/1000/60).arg(position/1000%60, 2, 10, QChar('0')));
        ui->musicSlider->setValue(position);
    });
    connect(ui->musicSlider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);
    connect(ui->playMusicBtn, &QPushButton::clicked, this, [=]{
        if(inListNum == 0)
            return;
        switch(mediaPlayer->playbackState()){
            case QMediaPlayer::PlaybackState::PausedState:{
                mediaPlayer->play();
                playStatus = true;
                break;
            }
            case QMediaPlayer::PlaybackState::PlayingState:{
                mediaPlayer->pause();
                playStatus = false;
                break;
            }
            case QMediaPlayer::PlaybackState::StoppedState:{
                nextMusic();
                break;
            }
        }
    });
    connect(ui->play_mode, &QPushButton::clicked, this, [=]{
        if(playMode == 0){
            ui->play_mode->setStyleSheet("background-image: url(:/assets/random_play.png);");
            playMode = 1;
        }
        else if(playMode == 1){
            ui->play_mode->setStyleSheet("background-image: url(:/assets/only_play.png);");
            playMode = 2;
        }
        else{
            ui->play_mode->setStyleSheet("background-image: url(:/assets/order_play.png);");
            playMode = 0;
        }
    });
    connect(ui->next_music, &QPushButton::clicked, this, &MainWidget::nextMusic);
    connect(ui->last_music, &QPushButton::clicked, this, &MainWidget::lastMusic);
}

void MainWidget::nextMusic(){
    inListNum = playLists.count();
    if(inListNum == 0)
        return;

    if(inListNum == 1){ //列表中只有一首歌曲时会出现问题，特判一下
        mediaPlayer->play();
    }
    else{
        if(playMode == 0)
            musicIndex = (musicIndex + 1) % inListNum;
        else if(playMode == 1)
            musicIndex = generateRandom(0, inListNum - 1);
        mediaPlayer->setSource(playLists.value(musicIndex).path);
        ui->playingMusic->setText(playLists.value(musicIndex).name);
        mediaPlayer->play();
    }
}

void MainWidget::lastMusic(){
    inListNum = playLists.count();
    if(inListNum == 0)
        return;
    if(playMode == 0)
        musicIndex = (musicIndex - 1 + inListNum) % inListNum;
    else if(playMode == 1)
        musicIndex = generateRandom(0, inListNum - 1);
        mediaPlayer->setSource(playLists.value(musicIndex).path);
        ui->playingMusic->setText(playLists.value(musicIndex).name);
        mediaPlayer->play();
}

int MainWidget::generateRandom(int low, int high){
    std::random_device rd;
    std::uniform_int_distribution<int> dist(low, high);
    return dist(rd);
}

void MainWidget::on_play_lists_clicked(){ //生成播放列表
    QDialog playList(this);
    QFormLayout form(&playList);

    QListWidget* playWidget = new QListWidget(&playList);
    playWidget->setSelectionMode(QAbstractItemView::MultiSelection); //设置多选
    QDialogButtonBox buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, &playList);

    connect(playWidget, &QListWidget::itemDoubleClicked, this, [=](){
        for(int j = 0; j < playLists.count(); j ++){
            if(playWidget->currentItem()->text() == playLists.value(j).name){
                musicIndex = j;
                break;
            }
        }
        mediaPlayer->setSource(playLists.value(musicIndex).path);
        ui->playingMusic->setText(playLists.value(musicIndex).name);
        mediaPlayer->play();
    });
    QLabel* label0 = new QLabel("播放列表");
    buttonBox.buttons().value(1)->setText("删除");
    for(int i = 0; i < playLists.count(); i ++)
        playWidget->addItem(playLists.value(i).name);
    form.addRow(label0, playWidget);
    form.addRow(&buttonBox);
    playList.setMinimumSize(200, 300);
    connect(buttonBox.button(QDialogButtonBox::Ok), &QPushButton::clicked, [&](){
        playList.reject();
    });
    connect(buttonBox.button(QDialogButtonBox::Cancel), &QPushButton::clicked, [&]() {
        auto items = playWidget->selectedItems();
        bool delNow = false;
        QList<MusicInfo> temInfo;
        QString nowName = playLists.value(musicIndex).name;

        for(int i = 0; i < playLists.count(); i ++){
            bool del = false;
            for(int j = 0; j < items.count(); j ++){
                if(items.value(j)->text() == playLists.value(i).name){
                    del = true;
                    if(items.value(j)->text() == nowName)
                        delNow = true;
                }
            }
            if(!del)
                temInfo.append(playLists.value(i));
        }
        playLists.clear();
        for(int i = 0; i < temInfo.count(); i ++){
            playLists.append(temInfo.value(i));
        }
        inListNum = playLists.count();
        if(delNow){
            if(inListNum == 0)
                mediaPlayer->stop(), playStatus = false, ui->playingMusic->setText(""), ui->total_time->setText("00:00"), ui->current_time->setText("00:00");
            else{
                musicIndex = generateRandom(0, inListNum - 1);
                mediaPlayer->setSource(playLists.value(musicIndex).path);
                ui->playingMusic->setText(playLists.value(musicIndex).name);
                mediaPlayer->play();
            }
        }
        else{
            for(int i = 0; i < playLists.count(); i ++)
                if(playLists.value(i).name == nowName){
                    musicIndex = i;
                    break;
                }
        }
        playList.close();
        on_play_lists_clicked();
    });
    playList.exec();
}

