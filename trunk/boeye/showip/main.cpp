#include "dialog.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setCursorFlashTime(0);
    a.setStyleSheet("*{color: black;"
                    "background-color: white;"
                    "selection-color: white;"
                    "selection-background-color: black;}");
    Dialog w;
    w.show();

    return a.exec();
}
