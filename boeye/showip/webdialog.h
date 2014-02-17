#ifndef WEBDIALOG_H
#define WEBDIALOG_H

#include <QDialog>
#include "fbupdate.h"

namespace Ui {
class webDialog;
}

class webDialog : public QDialog
{
    Q_OBJECT

public:
    explicit webDialog(QWidget *parent = 0, FBUpdate *update = 0);
    ~webDialog();

signals:
    void UpdateWindow();

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::webDialog *ui;
    FBUpdate* update;

};

#endif // WEBDIALOG_H
