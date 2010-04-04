/*
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

#include "indexviewer.h"
#include "ui_indexviewer.h"
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QMessageBox>
#include <QInputDialog>

IndexViewer::IndexViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IndexViewer)
{
    ui->setupUi(this);
    ui->treeView->addAction(ui->action_Rename);
    ui->treeView->addAction(ui->action_Delete);

    mevent=new menuevent();
    ui->treeView->installEventFilter(mevent);
}

IndexViewer::~IndexViewer()
{
    delete ui;
}

void IndexViewer::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void IndexViewer::setModel(QStandardItemModel *model)
{
    ui->treeView->setModel(model);
    ui->treeView->resizeColumnToContents(0);
}

void IndexViewer::on_treeView_collapsed(QModelIndex index)
{
    ui->treeView->resizeColumnToContents(0);
}

void IndexViewer::on_treeView_expanded(QModelIndex index)
{
    ui->treeView->resizeColumnToContents(0);
}

void IndexViewer::on_treeView_activated(QModelIndex index)
{

    QAbstractItemModel* model=ui->treeView->model();
    int row=index.row();
    QModelIndex pageindex=model->sibling(row,2,index);
    QString url=pageindex.data().toString();
    if(!(url.isEmpty()))
        emit(gotoPage(url.toInt()));
    else {
        pageindex=model->sibling(row,-1,index);
        url=pageindex.data().toString();
        if(!(url.isEmpty()))
            emit(gotoPage(url));
    }
    this->close();
}

void IndexViewer::on_action_Rename_triggered()
{

    QString text = QInputDialog::getText(this, tr("Rename"), tr("Enter the new name:"));
    if (!text.isEmpty()) {
        QModelIndex currentindex=ui->treeView->selectionModel()->currentIndex();
        QModelIndex rootindex=currentindex.parent();
        if(rootindex.data().toString() == tr("Bookmarks")) {
            emit(changeBookmarkName(currentindex.data().toString(), text));
            this->close();
        } else
            QMessageBox::warning(this, tr("Error"), tr("Only bookmark can be renamed!"));
    }
}

void IndexViewer::on_action_Delete_triggered()
{
    QMessageBox::StandardButton ret=QMessageBox::warning(this, tr("Confirm"), tr("Do you really want to remove this bookmark?"), QMessageBox::Yes | QMessageBox::No);
    if(ret==QMessageBox::Yes) {
        QModelIndex currentindex=ui->treeView->selectionModel()->currentIndex();
        QModelIndex rootindex=currentindex.parent();
        if(rootindex.data().toString() == tr("Bookmarks")) {
            emit(removeBookmark(currentindex.data().toString()));
            this->close();
        } else
            QMessageBox::warning(this, tr("Error"), tr("Only bookmark can be removed!"));
    }
}

