#ifndef WEBDIALOG_H
#define WEBDIALOG_H

#include <QDialog>

namespace Ui {
class webDialog;
}

class webDialog : public QDialog
{
    Q_OBJECT

public:
    explicit webDialog(QWidget *parent = 0);
    ~webDialog();

private:
    Ui::webDialog *ui;
};

#endif // WEBDIALOG_H
