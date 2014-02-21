#ifndef RESTHANDLER_H
#define RESTHANDLER_H

#include <QObject>
#include <QSettings>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDir>
#include <QList>
#include <QFile>

class resthandler : public QObject
{
    Q_OBJECT
public:
    explicit resthandler(QObject *parent = 0);
    ~resthandler();
    int getToken();
    QUrl LoginPage();
    int DownloadFile(QDir path);

signals:
    void LoginSuccess();
    void LoginFailure();
    void gotFileList(QList<QString>*);

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

};

#endif // RESTHANDLER_H
