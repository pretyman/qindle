#include "webdialog.h"
#include "ui_webdialog.h"

#include <QtWebKit/QWebView>
#include <QTreeWidget>
#include <QUrl>

webDialog::webDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::webDialog)
{
    ui->setupUi(this);
    rhandler = new resthandler(this);
    connect(rhandler, SIGNAL(LoginSuccess()), this, SLOT(LoginSuccess()));
    connect(rhandler, SIGNAL(LoginFailure()), this, SLOT(showWeb()));
    rhandler->getToken();
}

webDialog::~webDialog()
{
    delete ui;
    delete rhandler;
}

void webDialog::LoginSuccess()
{
    QTreeWidget *filelist = new QTreeWidget(this);
    this->ui->stackedWidget->addWidget(filelist);
    this->rhandler->getFileList(QDir("/"));

}

void webDialog::paintEvent(QPaintEvent *event)
{
    QRect region = event->rect();
    emit(this->UpdateWindow(region));

}

void webDialog::showWeb()
{
    QWebView *view = new QWebView(this);
    view->load(this->rhandler->LoginPage());

    this->ui->stackedWidget->addWidget(view);
    view->show();
    view->setFocus();
    connect(view, SIGNAL(urlChanged(QUrl)), rhandler, SLOT(getURL(QUrl)));
}
