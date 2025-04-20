#include "titlebar.h"

TitleBar::TitleBar(QWidget *parent)
    : QWidget{parent}
{

    mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);
    mainLayout->setSpacing(2);
    mainLayout->setContentsMargins(0, 0, 0, 0);


    titleLabel = new QLabel(this);
    mainLayout->addWidget(titleLabel);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 15px;");

    minimunButton = new QPushButton(this);
    minimunButton->setObjectName("miniButton");
    minimunButton->setFixedSize(15, 15);
    mainLayout->addWidget(minimunButton);
    minimunButton->setStyleSheet("border-radius: 12px;"
                                 "background: black;");

    maxinumButton = new QPushButton(this);
    maxinumButton->setObjectName("maxiButton");
    maxinumButton->setFixedSize(15, 15);
    mainLayout->addWidget(maxinumButton);
    maxinumButton->setStyleSheet("border-radius: 12px;"
                                 "background: black;");

    closeButton = new QPushButton(this);
    closeButton->setObjectName("closeButton");
    closeButton->setFixedSize(15, 15);
    mainLayout->addWidget(closeButton);

    QIcon closeButtonIcon(":/UI/Button/resources/closeButtonNormal.png");
    closeButton->setIcon(closeButtonIcon);
    closeButton->setIconSize(QSize(15, 15));
    closeButton->setStyleSheet("border-radius: 12px;");
    connect(closeButton, &QPushButton::clicked, this, &TitleBar::onButtonClicked);
    connect(minimunButton, &QPushButton::clicked, this, &TitleBar::onButtonClicked);
    connect(maxinumButton, &QPushButton::clicked, this, &TitleBar::onButtonClicked);

    this->autoFillBackground();

}

void TitleBar::setTitle(const QString &title)
{
    titleLabel->setText(title);
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


