/* 功能：显示目录与书签的对话框。
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

#ifndef INDEXVIEWER_H
#define INDEXVIEWER_H

#include <QDialog>
#include <QStandardItemModel>
#include "menuevent.h"

namespace Ui {
    class IndexViewer;
}

class IndexViewer : public QDialog {
    Q_OBJECT
public:
    IndexViewer(QWidget *parent = 0);
    ~IndexViewer();
    void setModel(QStandardItemModel* model);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::IndexViewer *ui;
    menuevent* mevent;
signals:
    void gotoPage(int page);
    void gotoPage(QString pagename);
    void removeBookmark(QString name);
    void changeBookmarkName(QString oldname, QString newname);

private slots:
    void on_action_Delete_triggered();
    void on_action_Rename_triggered();
    void on_treeView_activated(QModelIndex index);
    void on_treeView_expanded(QModelIndex index);
    void on_treeView_collapsed(QModelIndex index);
};

#endif // INDEXVIEWER_H
