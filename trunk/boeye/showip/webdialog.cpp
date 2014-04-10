#include "webdialog.h"
#include "ui_webdialog.h"

#include <QUrl>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QKeyEvent>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>

webDialog::webDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::webDialog)
{
    ui->setupUi(this);
    rhandler = new resthandler(this);
    connect(rhandler, SIGNAL(LoginSuccess()), this, SLOT(LoginSuccess()));
    connect(rhandler, SIGNAL(LoginFailure()), this, SLOT(showWeb()));
    connect(rhandler, SIGNAL(FileNotFound()), this, SLOT(FileNotFound()));
    rhandler->getToken();
    this->filelist = 0;
    this->webview = 0;
}

webDialog::~webDialog()
{
    delete ui;
    delete rhandler;
    delete filelist;
    delete webview;
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
    if(this->filelist) {
        this->ui->stackedWidget->removeWidget(this->filelist);
        filelist->close();
        delete filelist;
        filelist = 0;
    }
    if(!this->webview) {
        webview = new WebPageWidget(this);
        webview->setTextSizeMultiplier(1.5);
        webview->load(this->rhandler->LoginPage());
        this->ui->stackedWidget->addWidget(webview);
        connect(webview, SIGNAL(UpdateWindow(QRect)), this, SIGNAL(UpdateWindow(QRect)));
        connect(webview, SIGNAL(urlChanged(QUrl)), rhandler, SLOT(getURL(QUrl)));
        connect(webview, SIGNAL(loadFinished(bool)), this, SLOT(WebPageFinished()));
    }
    webview->setFocus();
}

void webDialog::FileNotFound()
{
    QLabel *label = new QLabel(this);
    label->setText("Please go to My App Data->booksync folder and add your books.\n If the folders do not exist, you may need to create them.");
    this->ui->stackedWidget->setCurrentIndex(this->ui->stackedWidget->addWidget(label));
}

void webDialog::WebPageFinished()
{
    QWebElement el = webview->page()->currentFrame()->findFirstElement("input");
        if (!el.isNull()) {
            el.setFocus();
        }
    QKeyEvent *forwardTab = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
    QKeyEvent *backwardTab = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::ShiftModifier);
    QCoreApplication::postEvent(webview, forwardTab);
    QCoreApplication::postEvent(webview, backwardTab);
}

