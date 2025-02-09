/********************************************************************************
** Form generated from reading UI file 'mainwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWidget
{
public:
    QStackedWidget *stackedWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *exploreMusicBtn;
    QPushButton *myMusicBtn;
    QPushButton *shareMusicBtn;
    QSlider *musicSlider;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QPushButton *play_mode;
    QPushButton *play_lists;
    QPushButton *last_music;
    QPushButton *playMusicBtn;
    QPushButton *next_music;
    QWidget *layoutWidget2;
    QVBoxLayout *vertiLayout;
    QSlider *audioSlider;
    QLabel *label;
    QLabel *playingMusic;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *current_time;
    QSpacerItem *horizontalSpacer;
    QLabel *total_time;

    void setupUi(QWidget *MainWidget)
    {
        if (MainWidget->objectName().isEmpty())
            MainWidget->setObjectName("MainWidget");
        MainWidget->resize(800, 600);
        stackedWidget = new QStackedWidget(MainWidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(40, 60, 701, 431));
        layoutWidget = new QWidget(MainWidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(150, 20, 491, 30));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        exploreMusicBtn = new QPushButton(layoutWidget);
        exploreMusicBtn->setObjectName("exploreMusicBtn");

        horizontalLayout->addWidget(exploreMusicBtn);

        myMusicBtn = new QPushButton(layoutWidget);
        myMusicBtn->setObjectName("myMusicBtn");

        horizontalLayout->addWidget(myMusicBtn);

        shareMusicBtn = new QPushButton(layoutWidget);
        shareMusicBtn->setObjectName("shareMusicBtn");

        horizontalLayout->addWidget(shareMusicBtn);

        musicSlider = new QSlider(MainWidget);
        musicSlider->setObjectName("musicSlider");
        musicSlider->setGeometry(QRect(180, 490, 411, 20));
        musicSlider->setOrientation(Qt::Horizontal);
        layoutWidget1 = new QWidget(MainWidget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(200, 550, 361, 30));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        play_mode = new QPushButton(layoutWidget1);
        play_mode->setObjectName("play_mode");
        play_mode->setStyleSheet(QString::fromUtf8("background-image: url(:/assets/order_play.png);"));

        gridLayout->addWidget(play_mode, 0, 4, 1, 1);

        play_lists = new QPushButton(layoutWidget1);
        play_lists->setObjectName("play_lists");
        play_lists->setStyleSheet(QString::fromUtf8("background-image: url(:/assets/play_lists.png);"));

        gridLayout->addWidget(play_lists, 0, 0, 1, 1);

        last_music = new QPushButton(layoutWidget1);
        last_music->setObjectName("last_music");
        last_music->setStyleSheet(QString::fromUtf8("background-image: url(:/assets/play_last.png);"));

        gridLayout->addWidget(last_music, 0, 1, 1, 1);

        playMusicBtn = new QPushButton(layoutWidget1);
        playMusicBtn->setObjectName("playMusicBtn");
        playMusicBtn->setStyleSheet(QString::fromUtf8("background-image: url(:/assets/play_music.png);"));
        playMusicBtn->setIconSize(QSize(32, 32));
        playMusicBtn->setCheckable(false);

        gridLayout->addWidget(playMusicBtn, 0, 2, 1, 1);

        next_music = new QPushButton(layoutWidget1);
        next_music->setObjectName("next_music");
        next_music->setStyleSheet(QString::fromUtf8("background-image: url(:/assets/play_next.png);"));

        gridLayout->addWidget(next_music, 0, 3, 1, 1);

        layoutWidget2 = new QWidget(MainWidget);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(650, 490, 32, 111));
        vertiLayout = new QVBoxLayout(layoutWidget2);
        vertiLayout->setObjectName("vertiLayout");
        vertiLayout->setContentsMargins(0, 0, 0, 0);
        audioSlider = new QSlider(layoutWidget2);
        audioSlider->setObjectName("audioSlider");
        audioSlider->setOrientation(Qt::Vertical);

        vertiLayout->addWidget(audioSlider);

        label = new QLabel(layoutWidget2);
        label->setObjectName("label");

        vertiLayout->addWidget(label);

        playingMusic = new QLabel(MainWidget);
        playingMusic->setObjectName("playingMusic");
        playingMusic->setGeometry(QRect(230, 520, 321, 20));
        playingMusic->setAlignment(Qt::AlignCenter);
        playingMusic->setIndent(0);
        layoutWidget3 = new QWidget(MainWidget);
        layoutWidget3->setObjectName("layoutWidget3");
        layoutWidget3->setGeometry(QRect(130, 510, 501, 22));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        current_time = new QLabel(layoutWidget3);
        current_time->setObjectName("current_time");

        horizontalLayout_2->addWidget(current_time);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        total_time = new QLabel(layoutWidget3);
        total_time->setObjectName("total_time");

        horizontalLayout_2->addWidget(total_time);


        retranslateUi(MainWidget);

        QMetaObject::connectSlotsByName(MainWidget);
    } // setupUi

    void retranslateUi(QWidget *MainWidget)
    {
        MainWidget->setWindowTitle(QCoreApplication::translate("MainWidget", "MainWidget", nullptr));
        exploreMusicBtn->setText(QCoreApplication::translate("MainWidget", "\346\216\242\347\264\242\351\237\263\344\271\220", nullptr));
        myMusicBtn->setText(QCoreApplication::translate("MainWidget", "\346\210\221\347\232\204\351\237\263\344\271\220", nullptr));
        shareMusicBtn->setText(QCoreApplication::translate("MainWidget", "\345\210\206\344\272\253\351\237\263\344\271\220", nullptr));
        play_mode->setText(QString());
        play_lists->setText(QString());
        last_music->setText(QString());
        playMusicBtn->setText(QString());
        next_music->setText(QString());
        label->setText(QCoreApplication::translate("MainWidget", "\351\237\263\351\207\217", nullptr));
        playingMusic->setText(QString());
        current_time->setText(QCoreApplication::translate("MainWidget", "00:00", nullptr));
        total_time->setText(QCoreApplication::translate("MainWidget", "00:00", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H
