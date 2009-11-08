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
    ui->treeView->setRootIndex(model->index("/"));
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
    if (model->isDir(index)) {
        ui->treeView->expand(index);
    } else {
    QString program = "/usr/local/bin/textedit";
    QStringList arguments;
    arguments << model->filePath(index);
    QMessageBox msgBox;
    msgBox.setText(arguments.first());
    msgBox.exec();
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);
    }
}

void MainWindow::on_action_Delete_triggered()
{
    QModelIndex index=ui->treeView->selectionModel()->currentIndex();
    QMessageBox msgBox;
    if (model->isDir(index)) {
        //msgBox.setText(tr("Cannot delete a directory!"));
        //msgBox.exec();
        bool retcode=model->fileInfo(index).absoluteDir().rmdir(model->filePath(index));
        if (!retcode) {
            msgBox.setText(tr("Cannot remove this file!"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    } else {
        msgBox.setText(tr("Do you really want to delete ") + model->fileName(index));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int ret=msgBox.exec();
        if (ret==QMessageBox::Yes) {
            bool retcode=model->fileInfo(index).absoluteDir().remove(model->fileName(index));
            if (!retcode) {
                msgBox.setText(tr("Cannot remove this file!"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
    }
}

void MainWindow::on_action_Copy_triggered()
{
    QModelIndex index=ui->treeView->selectionModel()->currentIndex();
    QMessageBox msgBox;
    if (model->isDir(index)) {
        msgBox.setText(tr("Cannot copy a directory!"));
        msgBox.exec();
    } else {
        clipindex=index;
        tempindex=2;
    }
}

void MainWindow::on_action_Paste_triggered()
{

    QModelIndex index=ui->treeView->selectionModel()->currentIndex();
    bool ret=QFile::copy(model->filePath(clipindex),(model->fileInfo(index).absolutePath() + model->fileName(clipindex)));
    qWarning("%d",ret);

}
