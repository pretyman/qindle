#include <QtGui/QApplication>
#include <QFile>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QCoreApplication::setOrganizationName("Qindle");
    QCoreApplication::setOrganizationDomain("qt.com");
    QCoreApplication::setApplicationName("QWSserver");

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
