#-------------------------------------------------
#
# Project created by QtCreator 2015-01-23T01:17:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Launcher
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stylesheet.cpp \
    shadoweffect.cpp \
    utils.cpp \
    windowcontroller.cpp \
    worker.cpp \
    NcFramelessHelper.cpp \
    settingswindow.cpp

HEADERS  += mainwindow.h \
    stylesheet.h \
    shadoweffect.h \
    utils.h \
    windowcontroller.h \
    worker.h \
    NcFramelessHelper.h \
    settingswindow.h

FORMS    += mainwindow.ui \
    settingswindow.ui

QMAKE_CXXFLAGS += -std=c++11

QMAKE_CFLAGS_RELEASE = -O3 -march=native
