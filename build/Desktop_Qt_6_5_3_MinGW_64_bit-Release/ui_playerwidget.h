/********************************************************************************
** Form generated from reading UI file 'playerwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYERWIDGET_H
#define UI_PLAYERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlayerWidget
{
public:
    QListWidget *listWidget;
    QPushButton *selectDirBtn;
    QWidget *layoutWidget;
    QVBoxLayout *rightPart;
    QPushButton *playChosenBtn;
    QPushButton *delFromListBtn;
    QGridLayout *gridLayout;
    QComboBox *choseListBox;
    QPushButton *addToListBtn;
    QPushButton *delListBtn;
    QPushButton *updInfoBtn;
    QPushButton *delChosenBtn;
    QWidget *layoutWidget1;
    QHBoxLayout *topPart;
    QLineEdit *searchLine;
    QPushButton *searchBtn;

    void setupUi(QWidget *PlayerWidget)
    {
        if (PlayerWidget->objectName().isEmpty())
            PlayerWidget->setObjectName("PlayerWidget");
        PlayerWidget->resize(785, 549);
        listWidget = new QListWidget(PlayerWidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(40, 60, 441, 341));
        selectDirBtn = new QPushButton(PlayerWidget);
        selectDirBtn->setObjectName("selectDirBtn");
        selectDirBtn->setEnabled(true);
        selectDirBtn->setGeometry(QRect(40, 20, 92, 28));
        selectDirBtn->setAutoDefault(false);
        selectDirBtn->setFlat(false);
        layoutWidget = new QWidget(PlayerWidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(500, 60, 180, 241));
        rightPart = new QVBoxLayout(layoutWidget);
        rightPart->setObjectName("rightPart");
        rightPart->setContentsMargins(0, 0, 0, 0);
        playChosenBtn = new QPushButton(layoutWidget);
        playChosenBtn->setObjectName("playChosenBtn");

        rightPart->addWidget(playChosenBtn);

        delFromListBtn = new QPushButton(layoutWidget);
        delFromListBtn->setObjectName("delFromListBtn");

        rightPart->addWidget(delFromListBtn);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        choseListBox = new QComboBox(layoutWidget);
        choseListBox->setObjectName("choseListBox");

        gridLayout->addWidget(choseListBox, 0, 0, 1, 1);

        addToListBtn = new QPushButton(layoutWidget);
        addToListBtn->setObjectName("addToListBtn");

        gridLayout->addWidget(addToListBtn, 1, 0, 1, 1);

        delListBtn = new QPushButton(layoutWidget);
        delListBtn->setObjectName("delListBtn");

        gridLayout->addWidget(delListBtn, 2, 0, 1, 1);


        rightPart->addLayout(gridLayout);

        updInfoBtn = new QPushButton(layoutWidget);
        updInfoBtn->setObjectName("updInfoBtn");

        rightPart->addWidget(updInfoBtn);

        delChosenBtn = new QPushButton(layoutWidget);
        delChosenBtn->setObjectName("delChosenBtn");

        rightPart->addWidget(delChosenBtn);

        layoutWidget1 = new QWidget(PlayerWidget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(240, 20, 243, 30));
        topPart = new QHBoxLayout(layoutWidget1);
        topPart->setObjectName("topPart");
        topPart->setContentsMargins(0, 0, 0, 0);
        searchLine = new QLineEdit(layoutWidget1);
        searchLine->setObjectName("searchLine");

        topPart->addWidget(searchLine);

        searchBtn = new QPushButton(layoutWidget1);
        searchBtn->setObjectName("searchBtn");

        topPart->addWidget(searchBtn);


        retranslateUi(PlayerWidget);

        selectDirBtn->setDefault(false);


        QMetaObject::connectSlotsByName(PlayerWidget);
    } // setupUi

    void retranslateUi(QWidget *PlayerWidget)
    {
        PlayerWidget->setWindowTitle(QCoreApplication::translate("PlayerWidget", "Form", nullptr));
        selectDirBtn->setText(QCoreApplication::translate("PlayerWidget", "\351\200\211\346\213\251\346\226\207\344\273\266\345\244\271", nullptr));
        playChosenBtn->setText(QCoreApplication::translate("PlayerWidget", "\346\222\255\346\224\276", nullptr));
        delFromListBtn->setText(QCoreApplication::translate("PlayerWidget", "\347\247\273\345\207\272\346\255\214\345\215\225", nullptr));
        addToListBtn->setText(QCoreApplication::translate("PlayerWidget", "\345\212\240\345\205\245\346\255\214\345\215\225", nullptr));
        delListBtn->setText(QCoreApplication::translate("PlayerWidget", "\345\210\240\351\231\244\346\255\214\345\215\225", nullptr));
        updInfoBtn->setText(QCoreApplication::translate("PlayerWidget", "\344\277\256\346\224\271\344\277\241\346\201\257", nullptr));
        delChosenBtn->setText(QCoreApplication::translate("PlayerWidget", "\345\210\240\351\231\244", nullptr));
        searchBtn->setText(QCoreApplication::translate("PlayerWidget", "\346\220\234\347\264\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlayerWidget: public Ui_PlayerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYERWIDGET_H
