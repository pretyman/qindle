#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QFile>
#define KINDLE_ROOT "/mnt/us"
#define STYLESHEET "/usr/local/share/qindle/eink.qss"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFile styleSheet(STYLESHEET);
    if (!styleSheet.open(QIODevice::ReadOnly)) {
        qWarning("Unable to open style!");
    } else {
        a.setStyleSheet(styleSheet.readAll());
    };
    w.show();
    return a.exec();
}
