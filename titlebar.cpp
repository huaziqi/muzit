#include "titlebar.h"

TitleBar::TitleBar(QWidget *parent)
    : QWidget{parent}
{

    mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);


    titleLabel = new QLabel(this);
    mainLayout->addWidget(titleLabel);

    closeButton = new QPushButton(this);
    closeButton->setObjectName("closeButton");
    closeButton->setFixedSize(15, 15);
    mainLayout->addWidget(closeButton);

    QIcon closeButtonIcon(":/UI/Button/resources/closeButtonNormal.png");
    closeButton->setIcon(closeButtonIcon);
    closeButton->setIconSize(QSize(15, 15));
    closeButton->setStyleSheet("border-radius: 12px;");
    connect(closeButton, &QPushButton::clicked, this, &TitleBar::onButtonClicked);

    //this->setMouseTracking(true);
    this->autoFillBackground();

}

void TitleBar::setTitle(const QString &title)
{
    titleLabel->setText(title);
}


void TitleBar::onButtonClicked()
{

    QPushButton *sendedButton = qobject_cast<QPushButton*>(sender());
    qDebug() << sendedButton->objectName();
    if(sendedButton->objectName() == "closeButton"){
        emit buttonEvent("closeWindow");
    }else if(sendedButton->objectName() == "minimumButton"){
        emit buttonEvent("minimunWindow");
    }else if(sendedButton->objectName() == "maximumButton"){
        emit buttonEvent("maximunWindow");
    }


}


