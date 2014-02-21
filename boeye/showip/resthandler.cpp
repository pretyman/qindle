#include "resthandler.h"
#include "appid.h"

#include <QScriptEngine>
#include <QScriptValue>
#include <QNetworkRequest>
#include <QScriptValueIterator>
#include <QFileInfo>

resthandler::resthandler(QObject *parent) :
    QObject(parent)
{
    this->settings = new QSettings(ORG_NAME, APP_NAME, this);
    this->manager = new QNetworkAccessManager(this);
}

resthandler::~resthandler()
{
    delete this->settings;
    delete this->manager;
}

int resthandler::getToken()
{
    QString queryname = "access_token";
    if(settings->contains(queryname)) {
        QUrl tokeninfo("https://openapi.baidu.com/oauth/2.0/tokeninfo");
        tokeninfo.addQueryItem(queryname, settings->value(queryname).toString());
        QString test = tokeninfo.toString();
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(LoginReplyFinished(QNetworkReply*)));
        manager->get(QNetworkRequest(tokeninfo));
    } else {
        emit(LoginFailure());
    }
    return 0;
}

QUrl resthandler::LoginPage()
{
    QUrl url("https://openapi.baidu.com/oauth/2.0/authorize");
    url.addQueryItem("client_id", OAUTH_CLIENT_ID);
    url.addQueryItem("response_type", "token");
    url.addQueryItem("redirect_uri", "oob");
    url.addQueryItem("scope", "netdisk");
    url.addQueryItem("display", "mobile");
    return url;
}

int resthandler::DownloadFile(QDir path)
{
    QDir abspath(APP_ROOT_PATH);
    QUrl url("https://d.pcs.baidu.com/rest/2.0/pcs/file");
    url.addQueryItem("method", "download");
    url.addQueryItem("access_token", settings->value("access_token").toString());
    url.addQueryItem("path", abspath.absoluteFilePath(path.path()));
    QString str = url.toString();
    if(this->CreateFile(path)) {
        downloadreply = manager->get(QNetworkRequest(url));
        connect(downloadreply, SIGNAL(finished()), this, SLOT(FileFinished()));
        return 1;
    } else {
        return 0;
    }
}

void resthandler::getFileList(QDir path)
{
    QDir abspath(APP_ROOT_PATH);
    QUrl url("https://pcs.baidu.com/rest/2.0/pcs/file");
    url.addQueryItem("method", "list");
    url.addQueryItem("access_token", settings->value("access_token").toString());
    url.addQueryItem("path", abspath.absoluteFilePath(path.path()));
    QString str = url.toString();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ListReplyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(url));
}

void resthandler::FileBytesAvailable()
{
    downloadfile->write(downloadreply->readAll());
}

void resthandler::FileFinished()
{
    if(downloadreply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 302) {
        QString strUrl = downloadreply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
        downloadreply->close();
        downloadreply->deleteLater();
        downloadreply = manager->get(QNetworkRequest(strUrl));
        connect(downloadreply, SIGNAL(readyRead()), this, SLOT(FileBytesAvailable()));
        connect(downloadreply, SIGNAL(finished()), this, SLOT(FileFinished()));

    } else {
        downloadfile->close();
        delete downloadfile;
        downloadreply->close();
        downloadreply->deleteLater();
    }
}

int resthandler::CreateFile(QDir path)
{
    QDir diskpath(DISK_ROOT_PATH);
    QFileInfo fileinfo = diskpath.absoluteFilePath(path.path());
    QString dirname = fileinfo.path();
    if(diskpath.mkpath(dirname)) {
        downloadfile = new QFile(fileinfo.absoluteFilePath());
        if(downloadfile->open(QIODevice::WriteOnly)) {
            return 1;
        } else {
            return 0;
        }
    } else{
        return 0;
    }
}

void resthandler::getURL(QUrl url)
{
    if(url.path().contains("login_success")) {
        url.setEncodedQuery(url.encodedFragment());
        QString token = url.queryItemValue("access_token");
        this->settings->setValue("access_token", token);
        emit(LoginSuccess());
    }
}

void resthandler::LoginReplyFinished(QNetworkReply *reply)
{

    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 200) {
        QString jsonString = QString(reply->readAll());
        QScriptEngine engine;
        QScriptValue value = engine.evaluate("JSON.parse").call(QScriptValue(), QScriptValueList() << jsonString);
        //
        QString str = value.property("client_id").toString();
        if(str == OAUTH_CLIENT_ID) {
            emit(LoginSuccess());
        } else {
            emit(LoginFailure());
        }
    } else {
        emit(LoginFailure());
    }
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(LoginReplyFinished(QNetworkReply*)));
    reply->close();
    reply->deleteLater();
}

void resthandler::ListReplyFinished(QNetworkReply *reply)
{
    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 200) {
        QString jsonString = QString(reply->readAll());
        QScriptEngine engine;
        QScriptValue value = engine.evaluate("JSON.parse").call(QScriptValue(), QScriptValueList() << jsonString);
        QScriptValueIterator iterator(value.property("list"));
        QList<QString> *filelist = new QList<QString>();
        QString str;
        while(iterator.hasNext()) {
            iterator.next();
            if(iterator.name() != "length") {
                str = iterator.value().property("path").toString().section("/", -1);
                filelist->append(str);
                str = iterator.value().property("size").toString();
                filelist->append(str);
                str = iterator.value().property("isdir").toString();
                filelist->append(str);
            }
        }
        emit(gotFileList(filelist));
    } else {
        emit(LoginFailure());
    }
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ListReplyFinished(QNetworkReply*)));
    reply->close();
    reply->deleteLater();

}

