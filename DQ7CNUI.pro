#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T10:14:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DQ7CNUI
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    FPTFile.cpp \
    TXTEntity.cpp

HEADERS  += mainwindow.h \
    CodeHelper.hpp \
    FPTFile.hpp \
    TXTEntity.hpp

FORMS    += mainwindow.ui
