#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

#define KINDLE_ROOT "/mnt/us"
#define STYLESHEET "/usr/local/share/qindle/eink.qss"

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
    QProcess shell;
private slots:
    void on_changedir_clicked();
    void on_submit_clicked();

    void readdata(int i);
};

#endif // MAINWINDOW_H
