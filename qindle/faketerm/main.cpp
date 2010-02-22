#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QFile>

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

        w.setWindowState(Qt::WindowMaximized);

    w.show();
    return a.exec();
}
