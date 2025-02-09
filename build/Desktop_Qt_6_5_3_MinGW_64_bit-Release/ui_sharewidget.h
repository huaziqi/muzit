/********************************************************************************
** Form generated from reading UI file 'sharewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAREWIDGET_H
#define UI_SHAREWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShareWidget
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *ShareWidget)
    {
        if (ShareWidget->objectName().isEmpty())
            ShareWidget->setObjectName("ShareWidget");
        ShareWidget->resize(663, 487);
        pushButton = new QPushButton(ShareWidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(250, 220, 231, 28));

        retranslateUi(ShareWidget);

        QMetaObject::connectSlotsByName(ShareWidget);
    } // setupUi

    void retranslateUi(QWidget *ShareWidget)
    {
        ShareWidget->setWindowTitle(QCoreApplication::translate("ShareWidget", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("ShareWidget", "\345\260\232\346\234\252\345\256\236\347\216\260\357\274\214\346\225\254\350\257\267\346\234\237\345\276\205", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShareWidget: public Ui_ShareWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAREWIDGET_H
