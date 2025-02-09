#ifndef EXPLOREWIDGET_H
#define EXPLOREWIDGET_H

#include "common.h"

namespace Ui {
class ExploreWidget;
}

class ExploreWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExploreWidget(QWidget *parent = nullptr);
    ~ExploreWidget();
private:
    QDir downPath = QDir("D:\\music");
    QMap<QString, QList<QString>> urls;
public slots:
    void startSearch();
    void downLoadByBv(QString title, QString bvId, QString cId);
    void searchTitleByBv(QString bvId);
    void searchByTitle(QString sTitle);
private slots:
    void on_selectDirBtn_clicked();
    void on_downloadBtn_clicked();

private:
    Ui::ExploreWidget *ui;
};

#endif // EXPLOREWIDGET_H
