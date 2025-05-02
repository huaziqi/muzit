#include "playerwidget.h"

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setAttribute(Qt::WA_StyledBackground); // 启用样式表背景
    this->setFixedHeight(100);
}
