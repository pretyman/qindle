#ifndef WEBDIALOG_H
#define WEBDIALOG_H

#include <QDialog>
#include <QPaintEvent>
#include <QRect>

namespace Ui {
class webDialog;
}

class webDialog : public QDialog
{
    Q_OBJECT

public:
    explicit webDialog(QWidget *parent = 0);
    ~webDialog();

signals:
    void UpdateWindow(QRect region);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::webDialog *ui;

};

#endif // WEBDIALOG_H
