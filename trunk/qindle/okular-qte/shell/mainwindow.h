/* 功能：主窗体。
 * Copyright (C) 2010 Li Miao <lm3783@gmail.com>
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

#include <QMainWindow>
#include "controller.h"

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
    okular::Controller* controller;

private slots:
    void on_action_Add_Bookmark_triggered();
    void on_actionSplit_Half_Vertically_triggered();
    void on_actionSplit_One_Sixth_triggered();
    void on_actionSplit_Quarter_triggered();
    void on_actionSplit_Half_triggered();
    void on_actionFit_All_triggered();
    void on_actionFit_Fit_triggered();
    void on_action_Trim_Margens_triggered();
    void on_action_Original_triggered();
    void on_actionRotate_Right_triggered();
    void on_actionRotate_Left_triggered();
    void on_action_Zoom_triggered();
    void on_action_Goto_Page_triggered();
    void on_action_Title_Page_triggered();
    void on_action_Last_Page_triggered();
    void on_action_First_Page_triggered();
    void on_action_Privous_Page_triggered();
    void on_action_Contents_triggered();
    void on_action_Next_Page_triggered();
    void on_action_Open_triggered();
    void controller_finished(int ret);
signals:
    void setDisplayType(int newtype);
};

#endif // MAINWINDOW_H
