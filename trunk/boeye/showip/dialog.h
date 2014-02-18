#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPaintEvent>
#include <QRect>
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
    void UpdateWindow(QRect region);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
    FBUpdate *fbupdate;
};

#endif // DIALOG_H
