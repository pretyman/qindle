#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbusplayer.h"
#include "dbustts.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    dbusplayer* player;
    dbustts* tts;

private slots:
    void on_okButton_clicked();
    void AudioAvail();
};

#endif // MAINWINDOW_H
