#ifndef RESTHANDLER_H
#define RESTHANDLER_H

#include <QObject>
#include <QSettings>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDir>
#include <QStringList>
#include <QFile>

class resthandler : public QObject
{
    Q_OBJECT
public:
    explicit resthandler(QObject *parent = 0);
    ~resthandler();
    int getToken();
    QUrl LoginPage();
    int ProcessFile(QString path, int mode);

signals:
    void LoginSuccess();
    void LoginFailure();
    void gotFileList(QStringList*);
    void FileNotFound();
    void ProcessComplete();
    void bytesWritten (qint64 bytes);

public slots:
    void getURL(QUrl url);
    void LoginReplyFinished(QNetworkReply *reply);
    void ListReplyFinished(QNetworkReply *reply);
    void getFileList(QDir path);
    void FileBytesAvailable();
    void FileFinished();

private:
    QSettings *settings;
    QNetworkAccessManager *manager;
    QNetworkReply *downloadreply;
    QFile *downloadfile;
    int CreateFile(QDir path);
    QString getSyncOperation(QString path, QString size);
    void AppendLocalList(QString path, QStringList remotelist, QStringList* finallist);

};

#endif // RESTHANDLER_H
