#include "framelesswidget.h"

#define PADDING 5

FramelessWidget::FramelessWidget(QWidget *parent)
    : QWidget{parent}{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    titleBar = new TitleBar(this);
    mainLayout->addWidget(titleBar);
    titleBar->installEventFilter(this);

    contentLayout = new QHBoxLayout();
    mainLayout->addLayout(contentLayout, 1);
    contentLayout->setContentsMargins(0, 5, 0, 0);


    connect(titleBar, &TitleBar::buttonEvent, this, &FramelessWidget::titleBarEvent);


    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

}

FramelessWidget::~FramelessWidget()
{

}

void FramelessWidget::titleBarEvent(const QString &signal)
{
    if(signal == "closeWindow"){
        this->close();
    }
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

bool FramelessWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched){
        if (event->type() == QEvent::Enter) {

            this->setCursor(Qt::ArrowCursor);
            location = CENTER;
        }
    }
    return QWidget::eventFilter(watched, event);

}

void FramelessWidget::mousePressEvent(QMouseEvent *event){

    switch(event->button()){
    case Qt::LeftButton:

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
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(QColor(200, 200, 200)));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    painter.drawRoundedRect(rect, 4, 4);

}


