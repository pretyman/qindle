#include "resthandler.h"
#include "appid.h"

#include <QScriptEngine>
#include <QScriptValue>
#include <QNetworkRequest>
#include <QScriptValueIterator>
#include <QFileInfo>
#include <QFileInfoList>

resthandler::resthandler(QObject *parent) :
    QObject(parent)
{
    this->settings = new QSettings(ORG_NAME, APP_NAME, this);
    this->manager = new QNetworkAccessManager(this);
    downloadfile = 0;
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
    QDir qdir;
    qdir.mkpath(DISK_ROOT_PATH);
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

int resthandler::ProcessFile(QString path, int mode)
{
    if(mode == 1) {
        QDir abspath(APP_ROOT_PATH);
        QUrl url("https://d.pcs.baidu.com/rest/2.0/pcs/file");
        url.addQueryItem("method", "download");
        url.addQueryItem("access_token", settings->value("access_token").toString());
        url.addQueryItem("path", abspath.absoluteFilePath(path));
        QString str = url.toString();
        if(this->CreateFile(path)) {
            downloadreply = manager->get(QNetworkRequest(url));
            connect(downloadreply, SIGNAL(finished()), this, SLOT(FileFinished()));
            return 1;
        } else {
            return 0;
        }
    } else {
        if(mode == 2) {
            QDir root(DISK_ROOT_PATH);
            root.mkpath(path);
        } else if(mode == 3) {
            QDir root(DISK_ROOT_PATH);
            root.remove(path);
        } else if(mode == 4) {
            this->RemovePath(path);

        }
        emit(ProcessComplete());
    }
    return 1;
}

int resthandler::getDownloadPos()
{
    if(downloadfile) {
        return downloadfile->pos();
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
        downloadfile = 0;
        downloadreply->close();
        downloadreply->deleteLater();
        emit(ProcessComplete());
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

QString resthandler::getSyncOperation(QString path, QString size)
{
    QString ret;
    QDir rootpath(APP_ROOT_PATH);
    QString relpath = rootpath.relativeFilePath(path);
    QDir diskpath(DISK_ROOT_PATH);
    QFileInfo fileinfo = diskpath.absoluteFilePath(relpath);
    if(fileinfo.exists()) {
        if(fileinfo.isDir()) {
            ret = "Finished";
        } else {
            if(fileinfo.size() == size.toInt()) {
                ret = "Finished";
            } else {
                ret = "Download";
            }
        }
    } else {
        ret = "Download";
    }
    return ret;
}

void resthandler::AppendLocalList(QString path, QStringList remotelist, QStringList *finallist)
{
    QDir rootpath(APP_ROOT_PATH);
    QString relpath = rootpath.relativeFilePath(path);
    QDir diskpath(DISK_ROOT_PATH);
    QDir currentpath = QFileInfo(diskpath.absoluteFilePath(relpath)).path();
    if(!currentpath.exists()) {
        return;
    }
    QFileInfoList filelist = currentpath.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    while(!filelist.isEmpty()) {
        QFileInfo file = filelist.takeFirst();
        if(!remotelist.contains(file.fileName())){
            finallist->append(file.fileName());
            finallist->append(QString().setNum(file.size()));
            finallist->append("Delete");
            finallist->append(QString().setNum(file.isDir()));
        }

    }
}

void resthandler::RemovePath(QString path)
{
    QDir root(DISK_ROOT_PATH);
    QDir currentpath = QDir(root.absoluteFilePath(path));
    QFileInfoList filelist = currentpath.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    while(!filelist.isEmpty()) {
        QFileInfo file = filelist.takeFirst();
        if(file.isFile()) {
            root.remove(file.filePath());
        }else if(file.isDir()) {
            this->RemovePath(file.filePath());
        }
    }
    root.rmpath(currentpath.path());
}

void resthandler::getURL(QUrl url)
{
    QString test = url.path();
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
        QStringList *filelist = new QStringList();
        QStringList remotelist;
        QString path;
        QString size;
        QString str;
        while(iterator.hasNext()) {
            iterator.next();
            if(iterator.name() != "length") {
                path = iterator.value().property("path").toString();
                filelist->append(path.section("/", -1));
                remotelist.append(path.section("/", -1));
                size = iterator.value().property("size").toString();
                filelist->append(size);
                str = this->getSyncOperation(path, size);
                filelist->append(str);
                str = iterator.value().property("isdir").toString();
                filelist->append(str);
            }
        }
        if(!filelist->isEmpty()) {
            this->AppendLocalList(path, remotelist, filelist);
        }
        emit(gotFileList(filelist));
    } else {
        emit(FileNotFound());
    }
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ListReplyFinished(QNetworkReply*)));
    reply->close();
    reply->deleteLater();
    emit(ProcessComplete());
}

