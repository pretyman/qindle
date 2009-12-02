/* 功能：主窗口界面
 * Copyright (C) 2009 Li Miao <lm3783@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_File_Manager_triggered()
{
    ui->treeView->setModel(&model);
    ui->treeView->setRootIndex(model.index(KINDLE_ROOT));
    ui->stackedWidget->setCurrentIndex(0);
    ui->treeView->setFocus();
}



void MainWindow::on_action_Program_Manager_triggered()
{
    bool ret=progdb.opendb();
    if (!ret) {
        QMessageBox::critical(0,tr("System Error"),tr("Cannot open program database!"),QMessageBox::Ok);
        return;
     };
    ui->tableView->setModel(&(progdb.ProgramModel));
    ui->tableView->setColumnHidden(2,true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->selectRow(0);
    ui->stackedWidget->setCurrentIndex(1);
    ui->tableView->setFocus();

}

void MainWindow::on_action_Open_triggered()
{
    on_treeView_activated(ui->treeView->selectionModel()->currentIndex());
}

void MainWindow::on_treeView_activated(QModelIndex index)
{
    if (model.isDir(index)) {
        ui->treeView->expand(index);
    } else {
        QString suffix=model.fileInfo(index).completeSuffix();
        QString program = progdb.ProgramName(suffix);
        if (program.isEmpty()) {
            QMessageBox::critical(0,tr("System Error"),tr("Cannot find program being able to open this file!"),QMessageBox::Ok);
            return;
        };
        QMessageBox::information(0,tr("Debug"),program,QMessageBox::Ok);
        fileutil.Filename=model.filePath(index);
        QMessageBox::information(0,tr("Debug"),model.filePath(index),QMessageBox::Ok);
        QStringList list;
        list.clear();
        fileutil.FileOpen(program,list);
    }
}

void MainWindow::on_action_Delete_triggered()
{
    QModelIndex index=ui->treeView->selectionModel()->currentIndex();
    fileutil.Filename=model.filePath(index);
    fileutil.isDir=model.isDir(index);
    int ret=fileutil.FileDel();
    if (ret) {
        QMessageBox::critical(0,tr("System Error"),tr("Cannot remove the file!"),QMessageBox::Ok);
    } else {
                model.refresh();
                ui->treeView->update();
            };
}

void MainWindow::on_action_Copy_triggered()
{
    QModelIndex index=ui->treeView->selectionModel()->currentIndex();
    clipindex=index;
    rmori=0;
    ui->action_Paste->setEnabled(true);
}

void MainWindow::on_action_Paste_triggered()
{
    QModelIndex index=ui->treeView->selectionModel()->currentIndex();
    if (index==clipindex) {
        QMessageBox::warning(0,tr("System Error"),tr("Cannot paste to the same folder!"),QMessageBox::Ok);
        return;
    }
    fileutil.Filename=model.filePath(clipindex);
    if (model.isDir(index)) {
        fileutil.Distname=model.filePath(index)+"/";
    } else {
        fileutil.Distname=model.fileInfo(index).absolutePath();
    };
    fileutil.isDir=model.isDir(clipindex);
    int ret=fileutil.FileCopy();
    if (rmori==true && ret==0)
        fileutil.FileDel();
    ui->action_Paste->setEnabled(false);
    model.refresh();
    ui->treeView->update();
}

void MainWindow::on_action_Cut_triggered()
{
    QModelIndex index=ui->treeView->selectionModel()->currentIndex();
    clipindex=index;
    rmori=1;
    ui->action_Paste->setEnabled(true);
}

void MainWindow::on_tableView_activated(QModelIndex index)
{
    int row=index.row();
    QString filepath=progdb.ProgramModel.record(row).value(2).toString();
    QMessageBox::information(0,tr("Debug"),filepath,QMessageBox::Ok);

    fileutil.Filename.clear();
    QStringList list;
    list.clear();
    fileutil.FileOpen(filepath,list);
}

void MainWindow::on_treeView_expanded(QModelIndex index)
{
    ui->treeView->resizeColumnToContents(0);
}

void MainWindow::on_treeView_collapsed(QModelIndex index)
{
    ui->treeView->resizeColumnToContents(0);
}

void MainWindow::on_action_Setting_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}
