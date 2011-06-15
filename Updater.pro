#-------------------------------------------------
#
# Project created by QtCreator 2011-06-09T22:25:26
#
#-------------------------------------------------

QT       += core gui

TARGET = App
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

OTHER_FILES +=

LIBS += -L../lib -lusb

CONFIG += console

RESOURCES += \
    Resources.qrc
