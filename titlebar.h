#ifndef TITLEBAR_H
#define TITLEBAR_H

#include "common.h"

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);
    void setTitle(const QString& title);
private:
    QHBoxLayout *mainLayout;
    QWidget* frontWidget;
    QLabel *titleLabel;
    QPushButton *closeButton, *maxnumButton, *minmunButton;
    QPoint pntMouseOffSet;
signals:
    void buttonEvent(const QString& signal);
protected:

private slots:
    void onButtonClicked();
private:
    //bool eventFilter(QObject *watched, QEvent *event);
};

#endif // TITLEBAR_H
