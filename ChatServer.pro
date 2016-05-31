#-------------------------------------------------
#
# Project created by QtCreator 2016-05-02T12:58:30
#
#-------------------------------------------------

QT       += core gui network sql

CONFIG += c++11


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cserver.cpp \
    cdatabase.cpp

HEADERS  += mainwindow.h \
    define.h \
    cserver.h \
    cdatabase.h

FORMS    += mainwindow.ui
