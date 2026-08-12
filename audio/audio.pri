# FFmpeg (vcpkg, MSVC x64)
FFMPEG_ROOT = D:/vcpkg/installed/x64-windows

HEADERS += \
    $$PWD/audioconvertmanager.h \
    $$PWD/audiotypes.h \
    $$PWD/audioprocessor.h \
    $$PWD/audioconverttask.h

SOURCES += \
    $$PWD/audioconvertmanager.cpp \
    $$PWD/audioprocessor.cpp \
    $$PWD/audioconverttask.cpp

INCLUDEPATH += $$PWD
INCLUDEPATH += $$FFMPEG_ROOT/include

win32-msvc {
    CONFIG(debug, debug|release) {
        FFMPEG_LIB_DIR = $$FFMPEG_ROOT/debug/lib
    } else {
        FFMPEG_LIB_DIR = $$FFMPEG_ROOT/lib
    }

    LIBS += "$$FFMPEG_LIB_DIR/avformat.lib"
    LIBS += "$$FFMPEG_LIB_DIR/avcodec.lib"
    LIBS += "$$FFMPEG_LIB_DIR/swresample.lib"
    LIBS += "$$FFMPEG_LIB_DIR/avutil.lib"
}
