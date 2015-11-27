#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T23:52:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qvfo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    freqctrl.cpp \
    settingsdialog.cpp \
    vna_serial.cpp \
    dds_dg8saq_usb.cpp \
    usb_backend.cpp

HEADERS  += mainwindow.h \
    freqctrl.h \
    settingsdialog.h \
    dds.h \
    vna_serial.h \
    dds_dg8saq_usb.h \
    usb_backend.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

TRANSLATIONS = qvfo_pl.ts

RESOURCES += icon.qrc

unix:CONFIG += qtserialport link_pkgconfig
unix:PKGCONFIG += libusb-1.0
unix:INCLUDEPATH += /usr/include/QtAddOnSerialPort
unix:LIBS += -lSerialPort

DISTFILES += qvfo_pl.qm

# Install

isEmpty(PREFIX) {
    PREFIX = /usr/local
}
BINDIR  = $$PREFIX/bin
DATADIR = $$PREFIX/share
DEFINES += APP_DATA_DIR=\"$$PREFIX/share/qvfo\"

bin.files = qvfo
bin.path = $$PREFIX/bin

desktop.files = qvfo.desktop
desktop.path = $$DATADIR/applications

language.files = qvfo_pl.qm
language.path = $$DATADIR/qvfo

icon16.files = icons/i16/qvfo.png
icon16.path = $$DATADIR/icons/hicolor/16x16/apps

icon32.files = icons/i32/qvfo.png
icon32.path = $$DATADIR/icons/hicolor/32x32/apps

icon48.files = icons/i48/qvfo.png
icon48.path = $$DATADIR/icons/hicolor/48x48/apps

icon64.files = icons/i64/qvfo.png
icon64.path = $$DATADIR/icons/hicolor/64x64/apps

INSTALLS += bin desktop icon16 icon32 icon48 icon64 language
