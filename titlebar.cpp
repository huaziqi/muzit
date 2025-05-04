#include "titlebar.h"

TitleBar::TitleBar(QWidget *parent)
    : QWidget{parent}
{

    mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);
    mainLayout->setSpacing(4);
    mainLayout->setContentsMargins(0, 0, 0, 0);


    titleLabel = new QLabel(this);
    mainLayout->addWidget(titleLabel);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 15px;");


    //设置一些窗口操作按钮
    minimunButton = new QPushButton(this);
    minimunButton->setObjectName("miniButton");
    minimunButton->setFixedSize(18,18);
    mainLayout->addWidget(minimunButton);
    minimunButton->setStyleSheet("border-radius: 18px;");

    maximumButton = new QPushButton(this);
    maximumButton->setObjectName("maxiButton");
    maximumButton->setFixedSize(19, 19);
    mainLayout->addWidget(maximumButton);
    maximumButton->setStyleSheet("border-radius: 18px;");

    closeButton = new QPushButton(this);
    closeButton->setObjectName("closeButton");
    closeButton->setFixedSize(20, 20);
    mainLayout->addWidget(closeButton);
    closeButton->setStyleSheet("border-radius: 18px;");

    closeButtonIcon = QIcon(":/UI/Button/resources/closeButton_normal.png");
    minimumButtonIcon = QIcon(":/UI/Button/resources/minimumBotton_normal.png");
    maximumButtonIcon = QIcon(":/UI/Button/resources/maximumButton_normal.png");
    closeButton->setIconSize(QSize(20, 20));
    maximumButton->setIconSize(QSize(19, 19));
    minimunButton->setIconSize(QSize(18, 18));

    closeButton->setIcon(closeButtonIcon);
    minimunButton->setIcon(minimumButtonIcon);
    maximumButton->setIcon(maximumButtonIcon);
    closeButton->installEventFilter(this);
    connect(closeButton, &QPushButton::clicked, this, &TitleBar::onButtonClicked);
    connect(minimunButton, &QPushButton::clicked, this, &TitleBar::onButtonClicked);
    connect(maximumButton, &QPushButton::clicked, this, &TitleBar::onButtonClicked);

    this->autoFillBackground();

}

void TitleBar::setTitle(const QString &title)
{
    titleLabel->setText(title);
}

bool TitleBar::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == closeButton){
        if(event->type() == QEvent::Enter){
            closeButtonIcon = QIcon(":/UI/Button/resources/closeButton_hover.png");
            closeButton->setIcon(closeButtonIcon);
            return true;
        }
        else if(event->type() == QEvent::Leave){
            closeButtonIcon = QIcon(":/UI/Button/resources/closeButton_normal.png");
            closeButton->setIcon(closeButtonIcon);
            return true;
        }
        else if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton){
                closeButtonIcon = QIcon(":/UI/Button/resources/closeButton_clicked.png");
                closeButton->setIcon(closeButtonIcon);
            }
        }

    }
    return false;
}


void TitleBar::onButtonClicked()
{

    QPushButton *sendedButton = qobject_cast<QPushButton*>(sender());

    if(sendedButton->objectName() == "closeButton"){
        emit buttonEvent("closeWindow");
    }else if(sendedButton->objectName() == "miniButton"){
        emit buttonEvent("miniWindow");
    }else if(sendedButton->objectName() == "maxiButton"){
        emit buttonEvent("maxiWindow");
    }

}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isMousePressed = true;
        mouseStartPoint = event->globalPosition().toPoint();
        windowStartPoint = this->window()->frameGeometry().topLeft();
    }
    QWidget::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (isMousePressed) {
        QPoint offset = event->globalPosition().toPoint() - mouseStartPoint;
        this->window()->move(windowStartPoint + offset);
    }
    QWidget::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isMousePressed = false;
    }
    QWidget::mouseReleaseEvent(event);
}

