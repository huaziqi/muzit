#include "explorewidget.h"

ExploreWidget::ExploreWidget(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    mainFont = common::vonwaoFont;
    initWeeklyMusic();
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
