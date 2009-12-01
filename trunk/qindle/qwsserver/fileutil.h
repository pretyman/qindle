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

#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QObject>
#include <QString>
#include <QStringList>


class FileUtil : public QObject
{
    Q_OBJECT
public:
    FileUtil();
    int FileOpen(QString Program, QStringList Argument);
    int FileCopy();
    int FileDel();
    int FileMove();
    QString Filename;
    QString Distname;
    bool isDir;

private:
    int msgbox(int mode);
    int exec(QString Program, QStringList Argument);
};

#endif // FILEUTIL_H
