#include "framelesswidget.h"

#define PADDING 5

FramelessWidget::FramelessWidget(QWidget *parent)
    : QWidget{parent}{
    mainLayout = new QVBoxLayout(this);
    contentLayout = new QHBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    titleBar = new TitleBar();
    mainLayout->addWidget(titleBar);
    mainLayout->addLayout(contentLayout, 1);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    connect(titleBar, &TitleBar::mouseMoveIn, this, &FramelessWidget::setCursorShape);

}

FramelessWidget::~FramelessWidget()
{

}

void FramelessWidget::setCursorShape(const QPoint &point)
{

    QPoint topLeft = mapToGlobal(rectMain.topLeft());
    QPoint bottomRight = mapToGlobal(rectMain.bottomRight());
    int x = point.x(), y = point.y();
    if(x >= topLeft.x() && x <= topLeft.x() + PADDING && y >= topLeft.y() && y <= topLeft.y() + PADDING)
        location = TOP_LEFT, this->setCursor(Qt::SizeFDiagCursor);
    else if(x >= topLeft.x() + PADDING && x <= bottomRight.x() - PADDING && y >= topLeft.y() && y <= topLeft.y() + PADDING)
        location = TOP, this->setCursor(Qt::SizeVerCursor);
    else if(x >= bottomRight.x() - PADDING && x <= bottomRight.x() && y >= topLeft.y() && y <= topLeft.y() + PADDING)
        location = TOP_RIGHT, this->setCursor(Qt::SizeBDiagCursor);
    else if(x >= bottomRight.x() - PADDING && x <= bottomRight.x() && y >= topLeft.y() + PADDING && y <= bottomRight.y() - PADDING)
        location = RIGHT, this->setCursor(Qt::SizeHorCursor);
    else if(x >= bottomRight.x() - PADDING && x <= bottomRight.x() && y >= bottomRight.y() - PADDING && y <= bottomRight.y())
        location = BOTTOM_RIGHT, this->setCursor(Qt::SizeFDiagCursor);
    else if(x >= topLeft.x() + PADDING && x <= bottomRight.x() - PADDING && y >= bottomRight.y() - PADDING && y <= bottomRight.y())
        location = BOTTOM, this->setCursor(Qt::SizeVerCursor);
    else if(x >= topLeft.x() && x <= topLeft.x() + PADDING && y >= bottomRight.y() - PADDING && y <= bottomRight.y())
        location = BOTTOM_LEFT, this->setCursor(Qt::SizeBDiagCursor);
    else if(x >= topLeft.x() && x <= topLeft.x() + PADDING && y >= topLeft.y() + PADDING && y <= bottomRight.y() - PADDING)
        location = LEFT, this->setCursor(Qt::SizeHorCursor);
    else
        location = CENTER, this->setCursor(Qt::ArrowCursor);
}

void FramelessWidget::mousePressEvent(QMouseEvent *event){

    switch(event->button()){
    case Qt::LeftButton:
        qDebug() << location;
        if(location == CENTER)
            pntMouseOffSet = event->globalPosition().toPoint() - this->frameGeometry().topLeft();
        bIsLeftPressed = true;
        break;
    }
}

void FramelessWidget::mouseReleaseEvent(QMouseEvent *event){
    switch(event->button()){
    case Qt::LeftButton:
        bIsLeftPressed = false;
        break;
    }
}

void FramelessWidget::mouseMoveEvent(QMouseEvent *event){
    QPoint mousePoint = event->globalPosition().toPoint();
    rectMain = this->rect();
    QPoint topLeft = mapToGlobal(rectMain.topLeft()), bottomRight = mapToGlobal(rectMain.bottomRight());
    if(!bIsLeftPressed){ //没有点击设置形状
        this->setCursorShape(mousePoint);
        return;
    }
    if(location == CENTER){
        move(mousePoint - pntMouseOffSet);
        return;
    }
    QRect reMove(topLeft, bottomRight);
    switch (location){
    case TOP:
        if(bottomRight.y() - mousePoint.y()> this->minimumHeight())
            reMove.setTop(mousePoint.y());
        break;
    case BOTTOM:
        reMove.setHeight(mousePoint.y() - topLeft.y());
        break;
    case LEFT:
        if(bottomRight.x() - mousePoint.x() > this->minimumWidth())
            reMove.setLeft(mousePoint.x());
        break;
    case RIGHT:
        reMove.setWidth(mousePoint.x() - topLeft.x());
        break;
    case TOP_LEFT:
        if(bottomRight.y() - mousePoint.y()> this->minimumHeight())
            reMove.setTop(mousePoint.y());
        if(bottomRight.x() - mousePoint.x() > this->minimumWidth())
            reMove.setX(mousePoint.x());
        break;
    case BOTTOM_RIGHT:
        reMove.setHeight(mousePoint.y() - topLeft.y());
        reMove.setWidth(mousePoint.x() - topLeft.x());
        break;
    case TOP_RIGHT:
        if(bottomRight.y() - mousePoint.y()> this->minimumHeight())
            reMove.setTop(mousePoint.y());
        reMove.setWidth(mousePoint.x() - topLeft.x());
        break;
    case BOTTOM_LEFT:
        reMove.setHeight(mousePoint.y() - topLeft.y());
        if(bottomRight.x() - mousePoint.x() > this->minimumWidth())
            reMove.setX(mousePoint.x());
        break;
    }
    this->setGeometry(reMove);
}

void FramelessWidget::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 使绘制更加平滑
    painter.setBrush(QBrush(QColor(255, 255, 255)));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    painter.drawRoundedRect(rect, 4, 4);  // 绘制一个带有圆角的矩形窗口

}

