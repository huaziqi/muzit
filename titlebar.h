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

    QLabel *titleLabel; //标题栏
    QPushButton *closeButton, *maximumButton, *minimunButton;
    QIcon closeButtonIcon, maximumButtonIcon, minimumButtonIcon;//按钮背景icon

    bool isMousePressed = false;
    QPoint mouseStartPoint;
    QPoint windowStartPoint;
signals:
    void buttonEvent(const QString& signal);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private slots:
    void onButtonClicked();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // TITLEBAR_H
