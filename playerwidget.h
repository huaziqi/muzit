#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include "common.h"

class PlayerWidget : public QWidget
{
    Q_OBJECT

private:
    QHBoxLayout *mainLayout;

    QPushButton *showPlayList;


public:
    explicit PlayerWidget(QWidget *parent = nullptr);

signals:
};

#endif // PLAYERWIDGET_H
