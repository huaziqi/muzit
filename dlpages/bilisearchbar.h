#ifndef BILISEARCHBAR_H
#define BILISEARCHBAR_H

#include "common.h"
#include "bilitypes.h"
#include <QLineEdit>
#include <QComboBox>

class BiliSearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit BiliSearchBar(QWidget *parent = nullptr);

private:
    QHBoxLayout  *mainLayout;
    QButtonGroup *typeGroup;
    QPushButton  *keywordBtn;
    QPushButton  *bvidBtn;
    QLineEdit    *searchInput;
    QPushButton  *searchBtn;
    QComboBox    *pageSizeBox;

private slots:
    void onSearchClicked();

signals:
    void searchRequested(QString keyword, BiliSearchType type, int pageSize);
};

#endif // BILISEARCHBAR_H
