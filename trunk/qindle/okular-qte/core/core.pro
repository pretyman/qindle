# #####################################################################
# Automatically generated by qmake (2.01a) ?? 3? 20 11:31:56 2010
# #####################################################################
QT += sql \
    webkit \
    network \
    xml
DEPENDPATH += .
INCLUDEPATH += .
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += ../include
INCLUDEPATH += /home/nemo/temp/gplrelease/quazip-0.2.3/quazip
include(../3rdparty/3rdparty.pri)

# Input
HEADERS += AbstractModel.h \
    CHMModel.h \
    DJVUModel.h \
    EPUBModel.h \
    ImageBasedModel.h \
    ImageBasedView.h \
    Metadata.h \
    PDFModel.h \
    TextBasedModel.h \
    TextBasedView.h \
    TOCModel.h \
    controller.h \
    chmreply.h \
    epubreply.h
SOURCES += CHMModel.cpp \
    DJVUModel.cpp \
    EPUBModel.cpp \
    Metadata.cpp \
    PDFModel.cpp \
    TOCModel.cpp \
    controller.cpp \
    ImageBasedView.cpp \
    TextBasedView.cpp \
    chmreply.cpp \
    epubreply.cpp
