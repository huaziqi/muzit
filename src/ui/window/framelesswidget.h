#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include "ui/uicommon.h"
#include "titlebar.h"

class FramelessWidget : public QWidget
{
    Q_OBJECT
private:
    Qt::Edges resizeEdgesAt(const QPointF &position) const;
    void updateResizeCursor(const QPointF &position);
protected:
    QVBoxLayout* mainLayout;
    QHBoxLayout* contentLayout;
    TitleBar *titleBar;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *event) override;
public:
    explicit FramelessWidget(QWidget *parent = nullptr);
    ~FramelessWidget();

signals:

private slots:
    void titleBarEvent(const QString& signal);
};

#endif // FRAMELESSWIDGET_H
