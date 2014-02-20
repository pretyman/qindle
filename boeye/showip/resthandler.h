#ifndef RESTHANDLER_H
#define RESTHANDLER_H

#include <QObject>
#include <QSettings>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDir>

class resthandler : public QObject
{
    Q_OBJECT
public:
    explicit resthandler(QObject *parent = 0);
    ~resthandler();
    int getToken();
    QUrl LoginPage();
    void getFileList(QDir path);

signals:
    void LoginSuccess();
    void LoginFailure();

public slots:
    void getURL(QUrl url);
    void LoginReplyFinished(QNetworkReply *reply);
    void ListReplyFinished(QNetworkReply *reply);

private:
    QSettings *settings;
    QNetworkAccessManager *manager;
};

#endif // RESTHANDLER_H
