#ifndef EXPLOREWIDGET_H
#define EXPLOREWIDGET_H

#include "common.h"
#include "musicitem.h"
#include "musicitemwidget.h"


struct WeeklyInfo{
    int ID; // list_id
    int period; //期号
    int time; //发布时间戳
};

class ExploreWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreWidget(QWidget *parent = nullptr);
private:

    QNetworkAccessManager *manager;
    QVBoxLayout *mainLayout;
    QFont mainFont;

    QNetworkRequest *weeklyIdRequest;
    QNetworkReply *weeklyIdReply;
    QFrame* weeklyMusicWidget;

    int minWeeklyMusicLayoutHeight, maxWeeklyMusicLayoutHeight;
    QVBoxLayout *weeklyMusicLayout; //每周音乐榜
    QHBoxLayout *weeklyMusicTopLayout; //顶部布局
    QLabel* weeklyInfoLabel;
    QMenu* chooseWeekMenu;
    QVector<WeeklyInfo> weeklyIdVector;
    int currentWeekId = -1;
//每周音乐部分控件
    QNetworkRequest *currentRankRequest;
    QNetworkReply *currentRankReply;
    QVector<MusicItem*> currentRankSongs;
    QScrollArea* currentWeekSongsArea;
    QWidget* currentWeekSongsWidget;
    QHBoxLayout* currentWeekSongsLayout;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    //初始化函数
    void initWeeklyMusic();
    void initCurrentWeekMusic();
    void getWeeklyHTMLInfo();
    void getCurrentWeekRank();
signals:
};

#endif // EXPLOREWIDGET_H
