#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include "common.h"
#include "titlebar.h"


enum Location{
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    CENTER,
    NONE
};

class FramelessWidget : public QWidget
{
    Q_OBJECT
private:
    QScreen* wholeScreen;
    QVBoxLayout* mainLayout;
    QHBoxLayout* contentLayout;
    TitleBar *titleBar;
    QPoint pntMouseOffSet;
    bool bIsLeftPressed = false;
    Location location = NONE;
    QRect rectMain, wholeRect;
    QWidget* wi;
private:
    bool eventFilter(QObject *watched, QEvent *event);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

public:
    void setCursorShape(const QPoint& point);
    explicit FramelessWidget(QWidget *parent = nullptr);
    ~FramelessWidget();

signals:

private slots:
    void titleBarEvent(const QString& signal);
};

#endif // FRAMELESSWIDGET_H
