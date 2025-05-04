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
    QScrollArea* funcArea; //可滚动的功能区域
    QLabel *sideTitle;
    QButtonGroup *stackButtonGroup;
    QPushButton *exploreButton, *localMusicButton;
    QStackedWidget* funcWidget;
    ExploreWidget *exploreWidget;
    LocalWidget *localWidget;

    PlayerWidget* playerWidget;
    QFont *sideBarFont;

private:
    void initSidebar();
    void initRight();
protected:
    void resizeEvent(QResizeEvent* event);
private slots:
    void onStackButtonClicked();

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
};
#endif // MAINWIDGET_H
