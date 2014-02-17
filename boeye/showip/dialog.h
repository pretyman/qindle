#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostAddress>
#include <QtDBus/QDBusInterface>
#include "fbupdate.h"
#include "webdialog.h"

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

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
    FBUpdate* update;
};

#endif // DIALOG_H
