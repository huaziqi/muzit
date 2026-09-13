QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += $$PWD/src

include($$PWD/src/app/app.pri)
include($$PWD/src/core/audio/audio.pri)
include($$PWD/src/core/download/download.pri)
include($$PWD/src/core/metadata/metadata.pri)
include($$PWD/src/platforms/bilibili/bilibili.pri)
include($$PWD/src/ui/ui.pri)

RESOURCES += $$PWD/resources.qrc



