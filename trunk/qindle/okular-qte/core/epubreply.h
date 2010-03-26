#ifndef EPUBREPLY_H
#define EPUBREPLY_H

#include <QNetworkReply>
#include "quazip.h"
#include "quazipfile.h"

class EPUBReply : public QNetworkReply
{
    Q_OBJECT
public:
    EPUBReply(QObject *parent, const QNetworkRequest &req, const QNetworkAccessManager::Operation op, QuaZip* zipfile, QString rootpath);
    ~EPUBReply();

    virtual void abort();
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 bytesAvailable () const;
private:
    QuaZipFile* m_file;
    qint64 bytesavail;
};

#endif // EPUBREPLY_H
