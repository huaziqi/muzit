QT       += core gui
QT += network
QT += sql
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    explorewidget.cpp \
    main.cpp \
    mainwidget.cpp \
    operateDb.cpp \
    playerwidget.cpp \
    sharewidget.cpp

HEADERS += \
    common.h \
    explorewidget.h \
    mainwidget.h \
    operateDb.h \
    playerwidget.h \
    sharewidget.h \
    structs.h

FORMS += \
    explorewidget.ui \
    mainwidget.ui \
    playerwidget.ui \
    sharewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc
