#include "titlebar.h"

TitleBar::TitleBar(QWidget *parent)
    : QWidget{parent}
{

    mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);
    titleLabel = new QLabel(this);
    mainLayout->addWidget(titleLabel);


    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->autoFillBackground();

    closeButton = new QPushButton(this);
    closeButton->setFixedSize(25, 25);

    closeButton->setStyleSheet("QPushButton{"
                               "border-radius: 12px;"
                               "}");
    mainLayout->addWidget(closeButton);
    QIcon closeButtonIcon(":/UI/Button/resources/closeButtonNormal.png");
    closeButton->setIcon(closeButtonIcon);
    closeButton->setIconSize(QSize(25, 25));

}

void TitleBar::setTitle(const QString &title)
{
    titleLabel->setText(title);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    pntMouseOffSet = event->globalPosition().toPoint() - parentWidget()->frameGeometry().topLeft();
}


void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoveIn(event->globalPosition().toPoint());
    parentWidget()->move(event->globalPosition().toPoint() - pntMouseOffSet);
}
