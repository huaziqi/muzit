QT += core
QT -= gui

CONFIG += console c++17
CONFIG -= app_bundle

TEMPLATE = app
TARGET = muzit_tests

SOURCES += \
    audioprocessortest.cpp \
    main.cpp

HEADERS += \
    audioprocessortest.h

include($$PWD/../../audio/audio.pri)
