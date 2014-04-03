#include "webdialog.h"
#include "ui_webdialog.h"

#include <QtWebKit/QWebView>
#include <QUrl>
#include <QTreeWidgetItem>

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
    this->filelist = new WebFileWidget(this);
    filelist->connectHandler(rhandler);
    this->ui->stackedWidget->setCurrentIndex(this->ui->stackedWidget->addWidget(filelist));
    connect(filelist, SIGNAL(UpdateWindow(QRect)), this, SIGNAL(UpdateWindow(QRect)));
    filelist->setFocus();
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

