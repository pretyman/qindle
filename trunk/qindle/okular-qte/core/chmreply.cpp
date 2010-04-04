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

#include "chmreply.h"
#include <QUrl>
#include <QString>

CHMReply::CHMReply(QObject *parent, const QNetworkRequest &req, const QNetworkAccessManager::Operation op, chmFile* file) : QNetworkReply(parent)
{
    this->setRequest(req);
    this->setOperation(op);
    this->setUrl(req.url());
    qRegisterMetaType<QNetworkReply::NetworkError>("QNetworkReply::NetworkError");

    m_file=file;
    QString filename=req.url().toString(QUrl::RemoveScheme | QUrl::RemoveFragment).mid(2); //remove first two slash

    int i=chm_resolve_object(m_file,filename.toAscii().data(),&cui);
    if (i==CHM_RESOLVE_SUCCESS) {
        bytesavail=cui.length;
        this->setHeader(QNetworkRequest::ContentLengthHeader, cui.length);
        this->setHeader(QNetworkRequest::ContentTypeHeader,"text/html");
        this->open( QIODevice::ReadOnly );
        QMetaObject::invokeMethod(this, "readyRead", Qt::QueuedConnection);
        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
    } else {
        this->setError(QNetworkReply::ContentNotFoundError, tr("404 not found"));
        QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection,  Q_ARG(QNetworkReply::NetworkError, QNetworkReply::ContentNotFoundError));
        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
    }
}
CHMReply::~CHMReply()
{
}

qint64 CHMReply::bytesAvailable() const
{
    return bytesavail+QNetworkReply::bytesAvailable();
}

void CHMReply::abort()
{
    //qWarning("aborted");
    QNetworkReply::close();
}

qint64 CHMReply::readData(char* data, qint64 maxlen)
{
    int i = chm_retrieve_object(m_file, &cui, (unsigned char *)data, cui.length-bytesavail, maxlen);
    //qWarning("total %d, available %d, reading %d",cui.length, bytesavail, i);
    if( i>0) {
        bytesavail-=i;
        return i;
    }
    return -1;
}
