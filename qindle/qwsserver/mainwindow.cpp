#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //右键菜单
    QAction *separatorAct = new QAction(this);
    separatorAct->setSeparator(true);
    ui->treeView->addAction(ui->action_Open);
    ui->treeView->addAction(ui->actionOpen_With);
    //ui->treeView->addAction(separatorAct);
    ui->treeView->addAction(ui->action_Cut);
    ui->treeView->addAction(ui->action_Copy);
    ui->treeView->addAction(ui->action_Paste);
    ui->treeView->addAction(separatorAct);
    ui->treeView->addAction(ui->action_Delete);
    //目录模型
    model=new QDirModel;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_File_Manager_triggered()
{
    ui->treeView->setModel(model);
    ui->stackedWidget->setCurrentIndex(0);
    ui->treeView->setFocus();
}

void MainWindow::on_treeView_expanded(QModelIndex index)
{
    ui->treeView->resizeColumnToContents(0);
}

void MainWindow::on_treeView_collapsed(QModelIndex index)
{
    ui->treeView->resizeColumnToContents(0);
}

void MainWindow::on_action_Program_Manager_triggered()
{
    QStringListModel *model=new QStringListModel;
    QStringList list;
    list << "a" << "b" << "c";
    model->setStringList(list);
    ui->listView->setModel(model);
    ui->stackedWidget->setCurrentIndex(1);
    ui->listView->setFocus();

}

void MainWindow::on_listView_activated(QModelIndex index)
{
    QMessageBox msgBox;
    msgBox.setText((index.data()).toString());
    msgBox.exec();
}

void MainWindow::on_action_Open_triggered()
{
    on_treeView_activated(ui->treeView->selectionModel()->currentIndex());
}

void MainWindow::on_treeView_activated(QModelIndex index)
{
    QString program = "/usr/local/bin/textedit";
    QStringList arguments;
    arguments << model->filePath(index);
    QMessageBox msgBox;
    msgBox.setText(arguments.first());
    msgBox.exec();
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);
}
