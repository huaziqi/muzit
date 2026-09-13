#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "config.h"
#include "ui/uicommon.h"
#include "ui/pages/explore/explorewidget.h"
#include "ui/widgets/playerwidget.h"
#include "ui/pages/local/localwidget.h"
#include "ui/window/framelesswidget.h"
#include "ui/pages/download/downloadwidget.h"
#include "core/download/downloadmanager.h"

class MainWidget : public FramelessWidget
{
    Q_OBJECT

private:
    QWidget *sidebarWidget, *rightWidget;
    QVBoxLayout* sidebarLayout, *rightLayout;
    QScrollArea* funcArea; //可滚动的功能区域
    QLabel *sideTitle;
    QButtonGroup *stackButtonGroup;
    int curStackButtonID = 0;
    QPushButton *exploreButton, *localMusicButton, *downloadMusicButton;
    QStackedWidget* funcWidget;
    ExploreWidget *exploreWidget;
    LocalWidget *localWidget;
    DownloadWidget* downloadWidget;

    PlayerWidget* playerWidget;
    QFont *sideBarFont;

    DownloadManager *downloadManager;
private:
    void initSidebar();
    void initRight();
    QPushButton* createButton(const QString& name);
protected:
    void resizeEvent(QResizeEvent* event);
private slots:
    void onStackButtonClicked();

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
};
#endif // MAINWIDGET_H
