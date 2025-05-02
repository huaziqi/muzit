#ifndef EXPLOREWIDGET_H
#define EXPLOREWIDGET_H

#include "common.h"

class ExploreWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreWidget(QWidget *parent = nullptr);
private:
    QVBoxLayout *mainLayout;
    QFont mainFont;

    QFrame* weeklyMusicWidget;
    QVBoxLayout *weeklyMusicLayout; //每周音乐榜
    QHBoxLayout *weeklyMusicTopLayout; //顶部布局
    QLabel* weeklyInfoLabel;


    //初始化函数
    void initWeeklyMusic();
signals:
};

#endif // EXPLOREWIDGET_H
