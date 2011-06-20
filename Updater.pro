#-------------------------------------------------
#
# Project created by QtCreator 2011-06-09T22:25:26
#
#-------------------------------------------------

QT       += core gui

TARGET = updater
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

LIBS += -lopenfocus -lusb
win32:LIBS += '-L../OpenFocusUpdater/LibOpenFocus-build-desktop/debug' '-L../OpenFocusUpdater/LibOpenFocus/libusb'
macx|unix:LIBS += '-L../OpenFocusUpdater/LibOpenFocus-build-desktop'
unix:LIBS += $$system(libusb-config --libs)
win32:RC_FILE += Win32.rc
