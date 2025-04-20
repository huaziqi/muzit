#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "config.h"
#include "common.h"
#include "explorewidget.h"
#include "playerwidget.h"
#include "localwidget.h"
#include "framelesswidget.h"

class MainWidget : public FramelessWidget
{
    Q_OBJECT

private:
    QWidget *sidebarWidget, *rightWidget;
    QVBoxLayout* sidebarLayout, *rightLayout;
    QLabel *sideTitle;
    QButtonGroup *sidebarButtons;
    QPushButton *exploreButton, *localMusicButton;
    QStackedWidget* funcWidget;
    ExploreWidget *exploreWidget;
    LocalWidget *localWidget;

    PlayerWidget* playerWidget;

private:
    void initSidebar();
    void initRight();
protected:


public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
};
#endif // MAINWIDGET_H
