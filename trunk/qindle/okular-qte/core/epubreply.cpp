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
