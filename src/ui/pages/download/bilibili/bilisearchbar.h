#ifndef BILISEARCHBAR_H
#define BILISEARCHBAR_H

#include "ui/uicommon.h"
#include "platforms/bilibili/bilitypes.h"

class BiliSearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit BiliSearchBar(QWidget *parent = nullptr);
    void searchFinished(const QString& error = "");

private:
    QHBoxLayout *mainLayout;
    QButtonGroup *typeGroup;
    QPushButton *keywordBtn;
    QPushButton *bvidBtn;
    QLineEdit *searchInput;
    QPushButton *searchBtn;
    QComboBox *pageSizeBox;

private slots:
    void onSearchClicked();


signals:
    void searchRequested(const QString &keyword, BiliSearchType type, int pageSize);
};

#endif // BILISEARCHBAR_H
