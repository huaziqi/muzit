#ifndef EXPLOREWIDGET_H
#define EXPLOREWIDGET_H

#include "common.h"

struct WeeklyInfo{
    int ID, period, time;
};

class ExploreWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreWidget(QWidget *parent = nullptr);
private:
    QNetworkAccessManager *manager;
    QNetworkRequest *weeklyIdRequest;
    QNetworkReply *weeklyIdReply;

    QVBoxLayout *mainLayout;
    QFont mainFont;


    QFrame* weeklyMusicWidget;
    QVBoxLayout *weeklyMusicLayout; //每周音乐榜
    QHBoxLayout *weeklyMusicTopLayout; //顶部布局
    QLabel* weeklyInfoLabel;
    QVector<WeeklyInfo> weeklyIdVector;

private:
    //初始化函数
    void initWeeklyMusic();
    void getWeeklyHTMLInfo();
signals:
};

#endif // EXPLOREWIDGET_H
