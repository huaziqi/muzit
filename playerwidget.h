#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include "structs.h"

#include "common.h"

#include "operateDb.h"

namespace Ui {
class PlayerWidget;
}

class PlayerWidget : public QWidget
{
    Q_OBJECT

public:
    int musicListNum;
    QDir dirPath;
    explicit PlayerWidget(QWidget *parent = nullptr);
    ~PlayerWidget();
    void scanfMusic(QList<QString> *musicPaths);
    void createItem(MusicInfo info);
    void init();
    void openMusicDir();
    void syncMusic();
    void showMusic();
    void showMusic(QString nowList);

private slots:
    void selectDirBtn_clicked();
    void menuList_clicked();
    void musicList_clicked();
    void on_playChosenBtn_clicked();
    void classedMenu_clicked();
    void on_delChosenBtn_clicked();
    void listPage_clicked();
    void on_searchBtn_clicked();
    void on_addToListBtn_clicked();
    void on_delListBtn_clicked();
    void on_updInfoBtn_clicked();

    void on_delFromListBtn_clicked();

signals:
    void sendUrl(QList<MusicInfo>, QDir);


private:
    QString shuffleString;
    int nowPage;
    QString nowList;
    QList<QString> listsName;
    QString classStd;
    QMap<QString, QList<MusicInfo>> classedInfo;
    void hideParts();
    void showParts();
    void gMenuList();
    void gAllMusic();
    void gClassedMusic();
    void gListPage();
    QList<QString> inDbPaths;
    QList<QString> listPaths;
    QMap <QString, MusicInfo> itemInfo;
    QList<QString> musicPaths;
    bool inited = false;
    DbInfo dbContent;
    QList<MusicInfo> musicInfo;
    void initDB();
    Ui::PlayerWidget *ui;
    QSqlDatabase DB;
    QSqlQuery *sqlQuery = NULL;

};

#endif // PLAYERWIDGET_H
