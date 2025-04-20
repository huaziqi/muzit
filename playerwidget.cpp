#include "playerwidget.h"

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedHeight(100);
    this->setStyleSheet("border: 2px solid blue; background-color: black;");
}
