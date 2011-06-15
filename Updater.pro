#-------------------------------------------------
#
# Project created by QtCreator 2011-06-09T22:25:26
#
#-------------------------------------------------

QT       += core gui

TARGET = Updater
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

OTHER_FILES +=

debug {
    CONFIG += console
}

RESOURCES += \
    Resources.qrc

INCLUDEPATH += 'LibOpenFocus'

win32:LIBS += -L'../App/LibOpenFocus-build-desktop/debug' -L'../App/LibOpenFocus/libusb' -lopenfocus -lusb
