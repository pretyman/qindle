#-------------------------------------------------
#
# Project created by QtCreator 2014-02-13T14:51:05
#
#-------------------------------------------------

QT       += core gui network dbus webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = showip
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    fbupdate.cpp \
    webdialog.cpp \
    resthandler.cpp

HEADERS  += dialog.h \
    fbupdate.h \
    webdialog.h \
    resthandler.h

FORMS    += dialog.ui \
    webdialog.ui
