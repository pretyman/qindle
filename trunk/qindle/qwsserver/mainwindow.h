#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QDirModel>
#include <QStringListModel>
#include <QMessageBox>
#include <QProcess>
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QDirModel *model;
    QPoint MousePos;

private slots:
    void on_treeView_activated(QModelIndex index);
    void on_action_Open_triggered();
    void on_listView_activated(QModelIndex index);
    void on_action_Program_Manager_triggered();
    void on_treeView_collapsed(QModelIndex index);
    void on_treeView_expanded(QModelIndex index);
    void on_action_File_Manager_triggered();
};

#endif // MAINWINDOW_H
