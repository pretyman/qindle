#ifndef ABOUTKINDLE_H
#define ABOUTKINDLE_H

#include <QDialog>

namespace Ui {
    class aboutkindle;
}

class aboutkindle : public QDialog {
    Q_OBJECT
public:
    aboutkindle(QWidget *parent = 0);
    ~aboutkindle();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::aboutkindle *ui;
};

#endif // ABOUTKINDLE_H
