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
    QPoint pntMouseOffSet;
    Location location = NONE;
    QRect rectMain, wholeRect, primaryRect, beforeMaxiedRect;
    QRect lastRect;//为了防止突然变形
    bool hasRecordedFRect = false;
    bool maxied = false, bIsLeftPressed = false;
    bool lastPressed = false;//检测鼠标形状
protected:
    QVBoxLayout* mainLayout;
    QHBoxLayout* contentLayout;
    TitleBar *titleBar;
private:
    void showMinimized();
    bool eventFilter(QObject *watched, QEvent *event) override;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *event) override;
public:
    bool setCursorShape(const QPoint& point);
    explicit FramelessWidget(QWidget *parent = nullptr);
    ~FramelessWidget();

signals:

private slots:
    void titleBarEvent(const QString& signal);
};

#endif // FRAMELESSWIDGET_H
