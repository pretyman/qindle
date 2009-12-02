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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QStringListModel>
#include <QMessageBox>
#include <QFileInfo>
#include <QDirModel>
#include "fileutil.h"
#include "programdb.h"

#define KINDLE_ROOT "/mnt/us"
#define STYLESHEET "./eink.qss"

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
    FileUtil fileutil;
    ProgramDB progdb;
    QDirModel model;
    QPoint MousePos;
    QModelIndex clipindex;
    bool rmori;

private slots:
    void on_action_Setting_triggered();
    void on_tableView_activated(QModelIndex index);
    void on_action_Cut_triggered();
    void on_action_Paste_triggered();
    void on_action_Copy_triggered();
    void on_action_Delete_triggered();
    void on_treeView_activated(QModelIndex index);
    void on_action_Open_triggered();
    void on_action_Program_Manager_triggered();
    void on_treeView_collapsed(QModelIndex index);
    void on_treeView_expanded(QModelIndex index);
    void on_action_File_Manager_triggered();
};

#endif // MAINWINDOW_H
