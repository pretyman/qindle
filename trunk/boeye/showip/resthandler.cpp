#include "resthandler.h"
#include "appid.h"

#include <QScriptEngine>
#include <QScriptValue>
#include <QNetworkRequest>

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

void resthandler::getFileList(QDir path)
{
    QUrl url("https://pcs.baidu.com/rest/2.0/pcs/file");
    url.addQueryItem("method", "list");
    url.addQueryItem("access_token", settings->value("access_token").toString());
    url.addQueryItem("path", path.path());
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ListReplyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(url));
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
        //
        QString str = value.property("list").toString();
    } else {
        emit(LoginFailure());
    }
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ListReplyFinished(QNetworkReply*)));
    reply->close();
    reply->deleteLater();

}

