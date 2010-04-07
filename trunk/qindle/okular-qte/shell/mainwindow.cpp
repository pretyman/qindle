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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QMessageBox>
#include "indexviewer.h"
#include <QFileDialog>
#include <QUrl>
#include <QInputDialog>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller=0;

    //commandline argument
    QStringList arg=QCoreApplication::arguments();
    if (arg.count()==2 && !(arg.at(1).isEmpty())) {
        controller=new okular::Controller();
        connect(controller, SIGNAL(finished(int)), this, SLOT(controller_finished(int)));
        controller->openFile(arg.at(1));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_action_Open_triggered()
{
    if(controller)
        delete(controller);
    controller=new okular::Controller();
    connect(controller, SIGNAL(finished(int)), this, SLOT(controller_finished(int)));
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);

    QStringList filters;
    filters << "All readable files (*.pdf *.djvu *.epub *.chm)"
            << "PDF files (*.pdf)"
            << "DJVU files (*.djvu)"
            << "EPUB files (*.epub)"
            << "CHM files (*.chm)";
    dialog.setNameFilters(filters);

    QList<QUrl> urls;
    urls << QUrl::fromLocalFile("/mnt/us/documents")
            << QUrl::fromLocalFile("/mnt/us")
            << QUrl::fromLocalFile("/");
    dialog.setSidebarUrls(urls);

    dialog.setWindowTitle(tr("Open File"));
    connect(&dialog,SIGNAL(fileSelected(QString)),controller,SLOT(openFile(QString)));
    dialog.exec();

}

void MainWindow::on_action_Next_Page_triggered()
{
    controller->gotoPage(true);
}

void MainWindow::on_action_Contents_triggered()
{
    IndexViewer viewer;
    viewer.setModel(controller->getTOCModel());
    connect(&viewer,SIGNAL(gotoPage(int)),controller,SLOT(gotoPage(int)));
    connect(&viewer,SIGNAL(gotoPage(QString)),controller,SLOT(gotoPage(QString)));
    connect(&viewer, SIGNAL(changeBookmarkName(QString,QString)), controller, SLOT(changeBookmarkName(QString,QString)));
    connect(&viewer, SIGNAL(removeBookmark(QString)), controller, SLOT(removeBookmark(QString)));

    viewer.exec();
}

void MainWindow::controller_finished(int ret)
{
    if(ret==0) {
        QWidget* viewer=controller->getView(ui->centralWidget);
        ui->verticalLayout->addWidget(viewer);
        viewer->resize(ui->centralWidget->width(),ui->centralWidget->height());
        viewer->setFocusPolicy(Qt::StrongFocus);
        controller->mainwindowfinished();
        connect(this, SIGNAL(setDisplayType(int)), controller, SIGNAL(setDisplayType(int)));
        //Something is wrong...and I don't know why
        QTimer::singleShot(1000, viewer, SLOT(setFocus()));
//        QTimer::singleShot(3000, this, SLOT(on_actionFit_All_triggered()));
    } else {
        QMessageBox::warning(this, tr("File Error"), tr("Cannot open this file!"));
    }

}

void MainWindow::on_action_Privous_Page_triggered()
{
    controller->gotoPage(false);
}

void MainWindow::on_action_First_Page_triggered()
{
    controller->gotoPage(1);
}

void MainWindow::on_action_Last_Page_triggered()
{
    int page=controller->getTotalPage();
    if(page>1)
        controller->gotoPage(page);
    else
        QMessageBox::warning(this, tr("Error"), tr("This File doesn't support the function. Use index to locate."));
}

void MainWindow::on_action_Title_Page_triggered()
{
    controller->gotoPage(0);
}

void MainWindow::on_action_Goto_Page_triggered()
{
    int totalpage=controller->getTotalPage();
    if(totalpage<1) {
        QMessageBox::warning(this, tr("Error"), tr("This file doesn't support the function. Use index to locate."));
        return;
    };

    bool ok;
    int page=QInputDialog::getInt(this, tr("Goto Page"), tr("Enter Page Number(from 1 to %1)").arg(QString::number(totalpage)), controller->getCurrentPageNo(), 1, totalpage,1, &ok);
    if(ok)
        controller->gotoPage(page);
}

void MainWindow::on_action_Zoom_triggered()
{
    bool ok;
    int percent=QInputDialog::getInt(this, tr("Zoom To"), tr("Enter the percentage you want to zoom to"), 100, 10, 300, 10, &ok);
    if(ok)
        controller->setZoomFactor((qreal)percent / 100);
}

void MainWindow::on_actionRotate_Left_triggered()
{
    controller->setRotate(-90);
}

void MainWindow::on_actionRotate_Right_triggered()
{
    controller->setRotate(90);
}

void MainWindow::on_action_Original_triggered()
{
    controller->setRotate(0);
}

void MainWindow::on_action_Trim_Margens_triggered()
{
    emit(setDisplayType(6));
}

void MainWindow::on_actionFit_Fit_triggered()
{
    emit(setDisplayType(0));
}

void MainWindow::on_actionFit_All_triggered()
{
    emit(setDisplayType(1));
}

void MainWindow::on_actionSplit_Half_triggered()
{
    emit(setDisplayType(2));
}

void MainWindow::on_actionSplit_Quarter_triggered()
{
    emit(setDisplayType(4));
}

void MainWindow::on_actionSplit_One_Sixth_triggered()
{
    emit(setDisplayType(5));
}

void MainWindow::on_actionSplit_Half_Vertically_triggered()
{
    emit(setDisplayType(3));
}

void MainWindow::on_action_Add_Bookmark_triggered()
{

    QString name=QInputDialog::getText(this, tr("Add Bookmark"), tr("Enter bookmark name"));

        if(name.isEmpty())
            name=QString::number(controller->getCurrentPageNo());
        bool ret=controller->addBookmark(name);
        if(!ret)
            QMessageBox::warning(this, tr("Database Error"), tr("Cannot add bookmark."));
}

void MainWindow::on_actionGB18030_triggered()
{
    controller->setGBKencoding();
}
