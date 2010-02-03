/* 功能：连接程序数据库
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
#ifndef PROGRAMDB_H
#define PROGRAMDB_H

#include <QObject>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>

#define DATAFILE "/usr/local/share/qindle/system.db"
class ProgramDB : public QObject
{
    Q_OBJECT
public:
    ProgramDB();
    ~ProgramDB();
    bool opendb();
    QSqlQueryModel ProgramModel;    //程序列表
    QString ProgramName(QString suffix);        //根据文件后缀选择打开程序
private:
    QSqlDatabase db;
};

#endif // PROGRAMDB_H
