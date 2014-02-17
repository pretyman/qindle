#-------------------------------------------------
#
# Project created by QtCreator 2014-02-13T14:51:05
#
#-------------------------------------------------

QT       += core gui network dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = showip
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    fbupdate.cpp \
    webdialog.cpp

HEADERS  += dialog.h \
    fbupdate.h \
    webdialog.h

FORMS    += dialog.ui \
    webdialog.ui
