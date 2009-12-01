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

#include "programdb.h"

ProgramDB::ProgramDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/nemo/temp/test.db");
    db.open();
}

bool ProgramDB::opendb()
{
    if (db.isOpenError())
        return false;
    ProgramDB::ProgramModel.setQuery("SELECT fullname, description, filepath FROM program",db);
    ProgramModel.setHeaderData(0, Qt::Horizontal, tr("Name"));
    ProgramModel.setHeaderData(1, Qt::Horizontal, tr("Description"));
    return true;
}

ProgramDB::~ProgramDB()
{
    if (db.isOpen())
        db.close();
}

QString ProgramDB::ProgramName(QString suffix)
{
    QString ret;
    if (db.isOpenError()) {
        ret.clear();
        return ret;
    };
    QSqlQuery *progquery=new QSqlQuery(db);
    QString sql="SELECT program.filepath FROM mimetype INNER JOIN program ON mimetype.prog = program.uiname  WHERE suffix='" + suffix + "' ORDER BY prefered DESC";
    progquery->exec(sql);
    if (progquery->size()) {
        progquery->first();
        ret=progquery->value(0).toString();
    } else {
        ret.clear();
    };
    return ret;
}
