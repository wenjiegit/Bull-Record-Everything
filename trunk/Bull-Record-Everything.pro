#-------------------------------------------------
#
# Project created by QtCreator 2017-09-05T17:50:11
#
#-------------------------------------------------

QT       += core gui multimedia

DESTDIR += $$PWD/bin
INCLUDEPATH += $$PWD/libRtAudio/

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mux-test
TEMPLATE = app
DEFINES += __WINDOWS_DS__
LIBS +=  -ldsound -luser32  -lole32

INCLUDEPATH += $$PWD/src $$PWD/src/mux $$PWD/src/source \
               $$PWD/external/libRtAudio

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/source/AudioSource.cpp \
    src/mux/MuxApi.cpp \
    src/mux/MuxThread.cpp \
    src/source/VideoSurface.cpp \
    external/libRtAudio/RtAudio.cpp

HEADERS  += src/MainWindow.h \
    src/source/AudioSource.h \
    src/mux/MuxApi.h \
    src/mux/MuxThread.h \
    src/source/VideoSurface.h \
    external/libRtAudio/RtAudio.h \
    external/libRtAudio/RtError.h

FORMS    += src/MainWindow.ui
