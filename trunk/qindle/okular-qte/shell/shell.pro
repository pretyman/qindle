# -------------------------------------------------
# Project created by QtCreator 2010-01-25T12:13:47
# -------------------------------------------------
QT += sql \
    webkit \
    network \
    xml
TARGET = okular-qte
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    indexviewer.cpp \
    menuevent.cpp
HEADERS += mainwindow.h \
    indexviewer.h \
    menuevent.h
FORMS += mainwindow.ui \
    indexviewer.ui
LIBS += -L../core \
    -lcore
INCLUDEPATH += ../core
DEPENDPATH += ../core
LIBS += -L../lib \
    -ldjvulibre \
    -lmupdf \
    -lfitz \
    -lfitzdraw \
    -lcmaps \
    -lfonts \
    -lchm \
    -lquazip \
    -ljbig2dec \
    -lopenjpeg
LIBS += -ljpeg
