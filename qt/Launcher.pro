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
    shadoweffect.cpp

HEADERS  += mainwindow.h \
    stylesheet.h \
    shadoweffect.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11 -O3
