/********************************************************************************
** Form generated from reading UI file 'explorewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPLOREWIDGET_H
#define UI_EXPLOREWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExploreWidget
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QRadioButton *titleRadio;
    QRadioButton *bvRadio;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QLineEdit *searchContent;
    QPushButton *searchBtn;
    QPushButton *selectDirBtn;
    QPushButton *downloadBtn;
    QListWidget *titleWidget;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpinBox *numSpin;

    void setupUi(QWidget *ExploreWidget)
    {
        if (ExploreWidget->objectName().isEmpty())
            ExploreWidget->setObjectName("ExploreWidget");
        ExploreWidget->resize(770, 507);
        layoutWidget = new QWidget(ExploreWidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(30, 20, 113, 59));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        titleRadio = new QRadioButton(layoutWidget);
        titleRadio->setObjectName("titleRadio");
        titleRadio->setChecked(true);

        verticalLayout->addWidget(titleRadio);

        bvRadio = new QRadioButton(layoutWidget);
        bvRadio->setObjectName("bvRadio");
        bvRadio->setEnabled(true);
        bvRadio->setAcceptDrops(false);
        bvRadio->setChecked(false);
        bvRadio->setAutoRepeat(false);
        bvRadio->setAutoExclusive(true);

        verticalLayout->addWidget(bvRadio);

        layoutWidget1 = new QWidget(ExploreWidget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(150, 40, 501, 30));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        searchContent = new QLineEdit(layoutWidget1);
        searchContent->setObjectName("searchContent");

        horizontalLayout->addWidget(searchContent);

        searchBtn = new QPushButton(layoutWidget1);
        searchBtn->setObjectName("searchBtn");

        horizontalLayout->addWidget(searchBtn);

        selectDirBtn = new QPushButton(ExploreWidget);
        selectDirBtn->setObjectName("selectDirBtn");
        selectDirBtn->setEnabled(true);
        selectDirBtn->setGeometry(QRect(40, 90, 92, 28));
        selectDirBtn->setAutoDefault(false);
        selectDirBtn->setFlat(false);
        downloadBtn = new QPushButton(ExploreWidget);
        downloadBtn->setObjectName("downloadBtn");
        downloadBtn->setGeometry(QRect(580, 380, 92, 28));
        titleWidget = new QListWidget(ExploreWidget);
        titleWidget->setObjectName("titleWidget");
        titleWidget->setGeometry(QRect(150, 90, 411, 321));
        widget = new QWidget(ExploreWidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(570, 90, 119, 30));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName("label");

        horizontalLayout_2->addWidget(label);

        numSpin = new QSpinBox(widget);
        numSpin->setObjectName("numSpin");
        numSpin->setMinimum(1);
        numSpin->setMaximum(35);
        numSpin->setValue(5);

        horizontalLayout_2->addWidget(numSpin);


        retranslateUi(ExploreWidget);

        selectDirBtn->setDefault(false);


        QMetaObject::connectSlotsByName(ExploreWidget);
    } // setupUi

    void retranslateUi(QWidget *ExploreWidget)
    {
        ExploreWidget->setWindowTitle(QCoreApplication::translate("ExploreWidget", "Form", nullptr));
        titleRadio->setText(QCoreApplication::translate("ExploreWidget", "\346\240\207\351\242\230\346\220\234\347\264\242", nullptr));
        bvRadio->setText(QCoreApplication::translate("ExploreWidget", "BV\345\217\267\344\270\213\350\275\275", nullptr));
        searchBtn->setText(QCoreApplication::translate("ExploreWidget", "\346\220\234\347\264\242", nullptr));
        selectDirBtn->setText(QCoreApplication::translate("ExploreWidget", "\351\200\211\346\213\251\346\226\207\344\273\266\345\244\271", nullptr));
        downloadBtn->setText(QCoreApplication::translate("ExploreWidget", "\344\270\213\350\275\275", nullptr));
        label->setText(QCoreApplication::translate("ExploreWidget", "\347\273\223\346\236\234\344\270\252\346\225\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExploreWidget: public Ui_ExploreWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPLOREWIDGET_H
