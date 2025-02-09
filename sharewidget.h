#ifndef SHAREWIDGET_H
#define SHAREWIDGET_H

#include <QWidget>

namespace Ui {
class ShareWidget;
}

class ShareWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShareWidget(QWidget *parent = nullptr);
    ~ShareWidget();

private:
    Ui::ShareWidget *ui;
};

#endif // SHAREWIDGET_H
