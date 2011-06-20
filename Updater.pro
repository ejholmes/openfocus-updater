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

OTHER_FILES += \
    Win32.rc

debug {
    CONFIG += console
}

RESOURCES += \
    Resources.qrc

INCLUDEPATH += 'LibOpenFocus'

win32:LIBS += -L'../OpenFocusUpdater/LibOpenFocus-build-desktop/debug' -L'../OpenFocusUpdater/LibOpenFocus/libusb' -lopenfocus -lusb
win32:RC_FILE += Win32.rc
