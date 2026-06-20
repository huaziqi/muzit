#ifndef BILISIDEPANEL_H
#define BILISIDEPANEL_H

#include "common.h"
#include "bilitypes.h"
#include "config.h"

class BiliSidePanel : public QWidget
{
    Q_OBJECT
public:
    explicit BiliSidePanel(QWidget *parent = nullptr);
    void setSelectedSong(const BiliVideoInfo &info);
    BiliSaveSettings currentSettings() const;
    void addDownloadTask(const QString &title);

private:
    QVBoxLayout *mainLayout;

    // 保存设置区
    QLabel *pathLabel;
    QPushButton *browseBtn;
    QLineEdit *templateInput;
    QComboBox *qualityBox;
    const QString configGroup = "bilibili";


    // 收藏区
    QLabel *selectedSongLabel;
    QPushButton *favoriteBtn;

    // 下载队列区
    QScrollArea *queueArea;
    QWidget *queueWidget;
    QVBoxLayout *queueLayout;

    BiliVideoInfo currentInfo;

    void initSaveSettings();
    void initFavorites();
    void initDownloadQueue();
    void initConfig();
    QFrame *makeSeparator();

    //配置存储信息
    QString defaultPath;
    const QString defaultTemplate = "{title} - {author}";
    const QString defaultQaulity = "320kbps";

private slots:
    void onBrowseClicked();
    void onSettingsChanged(const BiliSaveSettings& settings);

signals:
    void saveSettingsChanged(const BiliSaveSettings& settings);
    void favoriteRequested(BiliVideoInfo info);
};

#endif // BILISIDEPANEL_H
