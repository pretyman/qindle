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
    connect(this, SIGNAL(getFolderContent(QDir)), rhandler, SLOT(getFileList(QDir)));
    connect(rhandler, SIGNAL(gotFileList(QList<QString>*)), this, SLOT(setFolderContent(QList<QString>*)));
    rhandler->getToken();
}

webDialog::~webDialog()
{
    delete ui;
    delete rhandler;
}

void webDialog::LoginSuccess()
{
    this->filelist = new QTreeWidget(this);
    QStringList labels;
    labels.append("Name");
    labels.append("Size");
    labels.append("Operation");
    labels.append("Selection");
    filelist->setHeaderLabels(labels);
    this->ui->stackedWidget->addWidget(filelist);
    filelist->setFocus();
    connect(filelist, SIGNAL(itemExpanded(QTreeWidgetItem*)), SLOT(itemExpanded(QTreeWidgetItem*)));
    connect(filelist, SIGNAL(itemActivated(QTreeWidgetItem*,int)), SLOT(itemClicked(QTreeWidgetItem*,int)));
    emit(getFolderContent(QDir()));
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

void webDialog::setFolderContent(QList<QString> *list)
{
    QTreeWidgetItem* root = filelist->currentItem();
    while(not list->isEmpty()) {
        QString str = list->takeFirst();
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(str));
        str = list->takeFirst();
        item->setText(1,str);
        str = list->takeFirst();
        if(str == "1") {
            item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        }
        item->setCheckState(3,Qt::Unchecked);
        if(root) {
            root->addChild(item);
        } else {
            filelist->addTopLevelItem(item);
        }
        filelist->resizeColumnToContents(0);
    }
}

void webDialog::itemExpanded(QTreeWidgetItem *item)
{
    if(item->childCount() == 0) {
        QString path = item->text(0);
        QTreeWidgetItem *parent = item->parent();
        while(parent) {
            path.prepend(parent->text(0) + "/");
            parent = item->parent();
        }
        emit(getFolderContent(QDir(path)));
    }
}

void webDialog::itemClicked(QTreeWidgetItem *item, int)
{
    if(item->childIndicatorPolicy() == QTreeWidgetItem::ShowIndicator) {
        filelist->expandItem(item);
    }
    if(item->checkState(3) == Qt::Unchecked) {
        item->setCheckState(3,Qt::Checked);
    } else {
        item->setCheckState(3,Qt::Unchecked);
    }
    ////////////////
    this->itemDownload(item);
}

void webDialog::itemDownload(QTreeWidgetItem *item)
{

    QString path = item->text(0);
    QTreeWidgetItem *parent = item->parent();
    while(parent) {
        path.prepend(parent->text(0) + "/");
        parent = item->parent();
    }
    rhandler->DownloadFile(QDir(path));
}
