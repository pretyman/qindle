#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QFile>

#define KINDLE_ROOT "/mnt/us"
#define STYLESHEET "/usr/local/share/qindle/eink.qss"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Qindle");
        QCoreApplication::setOrganizationDomain("qt.com");
        QCoreApplication::setApplicationName("MusicPlayer");
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
