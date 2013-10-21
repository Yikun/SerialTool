#-------------------------------------------------
#
# Project created by QtCreator 2013-07-18T14:54:16
#
#-------------------------------------------------

QT       += core gui

TARGET = SerialTool
TEMPLATE = app

DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/qextserial/qextserialport_unix.cpp \
    src/qextserial/qextserialport.cpp

HEADERS  += src/mainwindow.h \
    src/qextserial/qextserialport_global.h \
    src/qextserial/qextserialport.h

FORMS    += ui/mainwindow.ui
