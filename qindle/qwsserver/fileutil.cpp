/* 功能：提供打开、复制、删除文件的接口
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

#include "fileutil.h"
#include <QMessageBox>
#include <QProcess>

FileUtil::FileUtil()
{
}

int FileUtil::msgbox(int mode) {
    QMessageBox msgBox;
    QString msgbody;
    switch (mode) {
        case 1:
        msgbody=tr("Do you really want to delete %1?").arg(FileUtil::Filename);
        break;
        case 2:
        msgbody=tr("Do you really want to delete %1 and all of its contents?").arg(FileUtil::Filename);
        break;
        case 3:
        msgbody=tr("Do you really want to move %1?").arg(FileUtil::Filename);
        break;
        case 4:
        msgbody=tr("Do you really want to move %1 and all of its contents?").arg(FileUtil::Filename);
        break;
        default:
        msgbody=FileUtil::Filename;
    };
    if (mode<=4) {
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    } else {
        msgBox.setStandardButtons(QMessageBox::Ok);
    };
    msgBox.setText(msgbody);
    int ret=msgBox.exec();
    return ret;
}

int FileUtil::exec(QString Program, QStringList Argument) {
    QProcess *myProcess = new QProcess();
    myProcess->start(Program, Argument);
    return myProcess->exitCode();
}

int FileUtil::FileOpen(QString Program, QStringList Argument) {
    FileUtil::msgbox(5);
    Argument << FileUtil::Filename;
    int ret=FileUtil::exec(Program, Argument);
    return ret;
}

int FileUtil::FileCopy() {
    QString Program="/bin/cp";
    QStringList Argument;
    if (FileUtil::isDir)
        Argument << "-r";
    Argument << FileUtil::Filename << FileUtil::Distname;
    FileUtil::msgbox(5);
    int ret=FileUtil::exec(Program, Argument);
    return ret;
}

int FileUtil::FileDel() {

    QString Program="/bin/rm";
    QStringList Argument;
    if (FileUtil::isDir) {
        Argument << "-rf";
        FileUtil::msgbox(2);
    } else {
        Argument << "-f";
        FileUtil::msgbox(1);
    };
    Argument << FileUtil::Filename;
    FileUtil::msgbox(5);
    int ret=FileUtil::exec(Program, Argument);
    return ret;
}

int FileUtil::FileMove() {

    QString Program="/bin/mv";
    QStringList Argument;
    if (FileUtil::isDir) {
        FileUtil::msgbox(2);
    } else {
        FileUtil::msgbox(1);
    };
    Argument << FileUtil::Filename << FileUtil::Distname;
    FileUtil::msgbox(5);
    int ret=FileUtil::exec(Program, Argument);
    return ret;
}
