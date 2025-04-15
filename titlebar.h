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
    void mouseMoveIn(const QPoint& point);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
signals:
};

#endif // TITLEBAR_H
