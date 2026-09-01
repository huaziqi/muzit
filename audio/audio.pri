# FFmpeg (vcpkg, MSVC x64)
FFMPEG_ROOT = D:/vcpkg/installed/x64-windows

HEADERS += \
    $$PWD/audioconverter.h \
    $$PWD/audioconvertmanager.h \
    $$PWD/audiotypes.h \
    $$PWD/audioprocessor.h \
    $$PWD/audioconverttask.h \
    $$PWD/ffmpegutils.h \
    $$PWD/m4aremuxer.h \
    $$PWD/mp3transcoder.h

SOURCES += \
    $$PWD/audioconvertmanager.cpp \
    $$PWD/audioprocessor.cpp \
    $$PWD/audioconverttask.cpp \
    $$PWD/ffmpegutils.cpp \
    $$PWD/m4aremuxer.cpp \
    $$PWD/mp3transcoder.cpp

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
