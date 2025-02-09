#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "common.h"

#include "playerwidget.h"
#include "sharewidget.h"
#include "explorewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    int generateRandom(int low, int high);
    PlayerWidget *playerWidget = NULL; //
    ShareWidget *shareWidget = NULL;
    ExploreWidget *exploreWidget = NULL;
private slots:
    void receiveUrl(QList<MusicInfo> newMusics, QDir musicDir);
    void changeVolume(int volume){
        audioOutput->setVolume(volume * 1.0 / 100);
    }
    void on_play_lists_clicked();

private:
    //QDir musicDir;
    void nextMusic();
    void lastMusic();
    QMediaPlayer *mediaPlayer = NULL;
    QAudioOutput *audioOutput = NULL;
    void setPlayer();
    int musicIndex = -1, playMode = 0, inListNum = 0; //当前音乐索引， 播放模式（列表，单曲， 随机）， 列表中的歌曲数
    bool playStatus = false;
    Ui::MainWidget *ui;
    QList<MusicInfo> playLists;
};
#endif // MAINWIDGET_H

