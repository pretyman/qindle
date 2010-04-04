/* 功能：将CHM中的内容转换成webkit需要的回应。
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

#ifndef CHMREPLY_H
#define CHMREPLY_H

#include <QNetworkReply>

#include "chm_lib.h"

class CHMReply : public QNetworkReply
{
    Q_OBJECT
public:
    CHMReply(QObject *parent, const QNetworkRequest &req, const QNetworkAccessManager::Operation op, chmFile* file);
    ~CHMReply();

    virtual void abort();
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 bytesAvailable () const;
private:
    qint64 bytesavail;
    struct chmFile* m_file;
    struct chmUnitInfo cui;

};

#endif // CHMREPLY_H
