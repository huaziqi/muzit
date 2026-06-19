#ifndef BILISIDEPANEL_H
#define BILISIDEPANEL_H

#include "common.h"
#include "bilitypes.h"
#include <QLineEdit>
#include <QComboBox>
#include <QProgressBar>

class BiliSidePanel : public QWidget
{
    Q_OBJECT
public:
    explicit BiliSidePanel(QWidget *parent = nullptr);
    void             setSelectedSong(const BiliVideoInfo &info);
    BiliSaveSettings currentSettings() const;
    void             addDownloadTask(const QString &title);

private:
    QVBoxLayout *mainLayout;

    // 保存设置区
    QLabel      *pathLabel;
    QPushButton *browseBtn;
    QLineEdit   *templateInput;
    QComboBox   *qualityBox;

    // 收藏区
    QLabel      *selectedSongLabel;
    QPushButton *favoriteBtn;

    // 下载队列区
    QScrollArea *queueArea;
    QWidget     *queueWidget;
    QVBoxLayout *queueLayout;

    BiliVideoInfo currentInfo;

    void initSaveSettings();
    void initFavorites();
    void initDownloadQueue();
    QFrame *makeSeparator();

private slots:
    void onBrowseClicked();

signals:
    void saveSettingsChanged(BiliSaveSettings settings);
    void favoriteRequested(BiliVideoInfo info);
};

#endif // BILISIDEPANEL_H
