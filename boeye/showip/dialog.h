#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostAddress>
#include <QtDBus/QDBusInterface>
#include "fbupdate.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

signals:
    void UpdateWindow();

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::Dialog *ui;
    FBUpdate* update;
};

#endif // DIALOG_H
