#include "framelesswidget.h"

#define PADDING 2

FramelessWidget::FramelessWidget(QWidget *parent)
    : QWidget{parent}{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(8, 8, 8, 8);

    titleBar = new TitleBar(this);
    mainLayout->addWidget(titleBar);
    titleBar->installEventFilter(this);
    mainLayout->installEventFilter(this);

    contentLayout = new QHBoxLayout();
    mainLayout->addLayout(contentLayout, 1);
    contentLayout->setContentsMargins(0, 6, 0, 0);

    wholeScreen = this->screen();
    wholeRect = wholeScreen->availableGeometry();
    connect(titleBar, &TitleBar::buttonEvent, this, &FramelessWidget::titleBarEvent);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground); //启用透明背景
    this->setMouseTracking(true);

    this->setMaximumSize(wholeRect.width(), wholeRect.height());

}

FramelessWidget::~FramelessWidget()
{

}

void FramelessWidget::titleBarEvent(const QString &signal)
{

    if(signal == "closeWindow"){
        this->close();
    }else if(signal == "miniWindow"){
        showMinimized();
    }else if(signal == "maxiWindow"){
        primaryRect = this->rect();
        this->resize(wholeRect.width(), wholeRect.height());
        this->move(0,0);
        maxied = true;
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

void FramelessWidget::showMinimized()
{
    beforeMaxiedRect = this->geometry();

    QPropertyAnimation *sizeAnimation = new QPropertyAnimation(this, "geometry");
    sizeAnimation->setDuration(100);
    sizeAnimation->setStartValue(beforeMaxiedRect);
    sizeAnimation->setEndValue(QRect(wholeRect.x() + wholeRect.width()/2 - 1, 1, 2, 1));
    connect(sizeAnimation, &QPropertyAnimation::finished, [this]() {
        QWidget::showMinimized();  // 真正最小化
    });
    sizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(this, "windowOpacity");
    opacityAnimation->setDuration(70);
    opacityAnimation->setStartValue(this->windowOpacity());
    opacityAnimation->setEndValue(0);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

}

bool FramelessWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched){
        if (event->type() == QEvent::Enter) {
            this->setCursor(Qt::ArrowCursor);
            location = CENTER;
        }
        else if(event->type() == QEvent::Leave){
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
        maxied = false;
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
        if(maxied){
            this->resize(primaryRect.width(), primaryRect.height());
            this->move(mousePoint.x() - (1.0 * mousePoint.x() / wholeRect.width()) * primaryRect.width(), mousePoint.y() - 1.0 * mousePoint.y() / wholeRect.height() * primaryRect.height());
            return;
        }
        else
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
    if(!hasRecordedFRect)
        lastRect = reMove, hasRecordedFRect = true;
    if(std::abs(lastRect.width() - reMove.width()) > 100 || std::abs(lastRect.height() - reMove.height()) > 100){
        location = CENTER;
        this->setCursor(Qt::ArrowCursor);
        hasRecordedFRect = false;
        return;
    }
    this->setGeometry(reMove);
    lastRect = reMove;
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

void FramelessWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        QWindowStateChangeEvent *stateEvent = static_cast<QWindowStateChangeEvent*>(event);
        if (stateEvent->oldState() & Qt::WindowMinimized) {
            this->resize(beforeMaxiedRect.width(), beforeMaxiedRect.height());
            this->move(beforeMaxiedRect.x(), beforeMaxiedRect.y());
            this->setWindowOpacity(1);
        }
    }
    QWidget::changeEvent(event);
}


