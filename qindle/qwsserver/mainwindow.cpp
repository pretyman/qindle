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
#include "aboutkindle.h"
#include "hwinfo.h"
#include <QMessageBox>
#include <QDBusConnection>
#include <QWSServer>

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

    mevent=new menuevent();
    ui->treeView->installEventFilter(mevent);

    ui->tabWidget->setCurrentIndex(1);

    //DBus signal connection
    QDBusConnection::systemBus().connect(QString(), QString(), "com.lab126.powerd", "goingToScreenSaver", this,SLOT(screensaver()));
    QDBusConnection::systemBus().connect(QString(), QString(), "com.lab126.powerd", "outOfScreenSaver", this,SLOT(quitscreensaver()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mevent;
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
//        QMessageBox::information(0,tr("Debug"),program,QMessageBox::Ok);
//
//        QMessageBox::information(0,tr("Debug"),model.filePath(index),QMessageBox::Ok);
        fileutil.Filename=model.filePath(index);
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
//    QMessageBox::information(0,tr("Debug"),filepath,QMessageBox::Ok);

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


void MainWindow::on_tabWidget_currentChanged(int index)
{
    bool ret;
    hwinfo hwinfo;
    switch(index) {
    case 0: //file manager
        ui->treeView->setModel(&model);
        ui->treeView->setRootIndex(model.index(KINDLE_ROOT));
        ui->treeView->setFocus();
        break;
    case 1: //program manager
        ret=progdb.opendb();
        if (!ret) {
            QMessageBox::critical(0,tr("System Error"),tr("Cannot open program database!"),QMessageBox::Ok);
            break;
         };
        ui->tableView->setModel(&(progdb.ProgramModel));
        ui->tableView->setColumnHidden(2,true);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->selectRow(0);
        ui->tableView->setFocus();
        break;
    case 3:
        hwinfo.setTable(ui->tableWidget);
        break;

    }
}

void MainWindow::on_actionAbout_Kindle_triggered()
{
    aboutkindle a;
    a.exec();
}

void MainWindow::on_actionA_bout_Qt_triggered()
{
    QMessageBox::aboutQt(this,"About Qt");
}

void MainWindow::on_action_About_triggered()
{
    QString text=tr("This project is aimed at porting a Qt embedded framework to Amazon Kindle DX, and porting other useful applications to this framework.\n\nCopyright (C) 2009 Li Miao <lm3783@gmail.com>");
    QMessageBox::about(this,"About Qindle",text);
}

void MainWindow::screensaver()
{
    QStringList list;
    list << "-s" << "5";
    fileutil.exec("/usr/sbin/eips",list);
}

void MainWindow::quitscreensaver()
{
    QWSServer::instance()->refresh();
}
