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

#include "epubreply.h"
#include <QUrl>
#include <QString>
#include <string>

EPUBReply::EPUBReply(QObject *parent, const QNetworkRequest &req, const QNetworkAccessManager::Operation op, QuaZip* zipfile, QString rootpath) : QNetworkReply(parent)
{
    this->setRequest(req);
    this->setOperation(op);
    this->setUrl(req.url());
    qRegisterMetaType<QNetworkReply::NetworkError>("QNetworkReply::NetworkError");
    //remove first third slash(quazip doesn't like the root '/') and add opf path
    QString filename=rootpath + req.url().toString(QUrl::RemoveScheme | QUrl::RemoveFragment).mid(3);
    m_file=new QuaZipFile(zipfile->getZipName(),filename,QuaZip::csInsensitive,this);
    if(m_file->open(QIODevice::ReadOnly)) {
        bytesavail=m_file->usize();
        this->setHeader(QNetworkRequest::ContentLengthHeader, bytesavail);
        this->setHeader(QNetworkRequest::ContentTypeHeader,"application/xhtml+xml");
        this->open( QIODevice::ReadOnly );
        QMetaObject::invokeMethod(this, "readyRead", Qt::QueuedConnection);
        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
    } else {
        this->setError(QNetworkReply::ContentNotFoundError, tr("404 not found"));
        QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection,  Q_ARG(QNetworkReply::NetworkError, QNetworkReply::ContentNotFoundError));
        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
    }
}
EPUBReply::~EPUBReply()
{
    if(m_file)
        delete m_file;
}

qint64 EPUBReply::bytesAvailable() const
{
    return bytesavail+QNetworkReply::bytesAvailable();
}

void EPUBReply::abort()
{
    qWarning("aborted");
    if(m_file)
        m_file->close();
    QNetworkReply::close();
}

qint64 EPUBReply::readData(char* data, qint64 maxlen)
{
    int i;
    i=m_file->read(data, maxlen);
    if( i>0 ) {
        bytesavail-=i;
        return i;
    };
    return -1;
}
