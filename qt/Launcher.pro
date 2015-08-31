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
    settingswindow.cpp \
    stylecontainer.cpp \
    job.cpp \
    atomicbool.cpp \
    resultwidget.cpp \
    cleaner.cpp \
    globalshortcut.cpp \
    singleinstance.cpp

HEADERS += mainwindow.h \
    stylesheet.h \
    shadoweffect.h \
    utils.h \
    windowcontroller.h \
    worker.h \
    NcFramelessHelper.h \
    settingswindow.h \
    stylecontainer.hpp \
    job.h \
    atomicbool.h \
    resultwidget.h \
    cleaner.h \
    globalshortcut.h \
    singleinstance.h

FORMS += mainwindow.ui \
    settingswindow.ui \
    resultwidget.ui

LIBS += -lX11

QMAKE_CXXFLAGS += -std=c++11 -g
QMAKE_CFLAGS_RELEASE = -O3 -march=native

