#include "webdialog.h"
#include "ui_webdialog.h"

#include <QtWebKit/QWebView>
#include <QUrl>

webDialog::webDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::webDialog)
{
    ui->setupUi(this);

    QWebView *view = new QWebView(parent);
    view->load(QUrl("http://www.baidu.com"));

    this->ui->stackedWidget->addWidget(view);
    view->show();
    view->setFocus();

}

webDialog::~webDialog()
{
    delete ui;

}

void webDialog::paintEvent(QPaintEvent *event)
{
    QRect region = event->rect();
    emit(this->UpdateWindow(region));

}
