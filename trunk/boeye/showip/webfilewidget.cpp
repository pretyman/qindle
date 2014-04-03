#include "webfilewidget.h"
#include <QMenu>
#include <QTreeWidgetItemIterator>

WebFileWidget::WebFileWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    QStringList labels;
    labels.append("Name");
    labels.append("Size");
    labels.append("Operation");
    labels.append("Selection");
    this->setHeaderLabels(labels);
}

void WebFileWidget::connectHandler(resthandler *handler)
{
    this->rhandler = handler;
    connect(this, SIGNAL(getFolderContent(QDir)), rhandler, SLOT(getFileList(QDir)));
    connect(rhandler, SIGNAL(gotFileList(QList<QString>*)), this, SLOT(setFolderContent(QList<QString>*)));
    //connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)), SLOT(itemExpand(QTreeWidgetItem*)));
    connect(this, SIGNAL(itemActivated(QTreeWidgetItem*,int)), SLOT(itemClick(QTreeWidgetItem*,int)));
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(ShowMenu(QTreeWidgetItem*,int)));
    connect(rhandler, SIGNAL(ProcessComplete()), this, SLOT(getChildItem()));
    emit(getFolderContent(QDir()));
}

//void WebFileWidget::paintEvent(QPaintEvent *event)
//{
//    QRect region = event->rect();
//    emit(this->UpdateWindow(region));

//}

void WebFileWidget::setFolderContent(QList<QString> *list)
{
    QTreeWidgetItem* root = this->currentItem();
    if(root && list->isEmpty()) {
        root->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
        return;
    }
    while(not list->isEmpty()) {
        QString str = list->takeFirst();
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(str));
        str = list->takeFirst();
        item->setText(1,str);
        if(root) {
            root->addChild(item);
            item->setCheckState(3,root->checkState(3));
        } else {
            this->addTopLevelItem(item);
            item->setCheckState(3,Qt::Unchecked);
        }
        str = list->takeFirst();
        if(str == "1") {
            item->setText(1,"");
            item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        }
    }
    this->resizeColumnToContents(0);
}

void WebFileWidget::itemExpand(QTreeWidgetItem *item)
{
    if(item->childCount() == 0) {
        QString path = item->text(0);
        QTreeWidgetItem *parent = item->parent();
        while(parent) {
            path.prepend(parent->text(0) + "/");
            parent = parent->parent();
        }
        emit(getFolderContent(QDir(path)));
    }
}

void WebFileWidget::itemClick(QTreeWidgetItem *item, int)
{
    if(item->checkState(3) == Qt::Unchecked) {
        this->itemCheckState(item, Qt::Checked);
    } else {
        this->itemCheckState(item, Qt::Unchecked);
    }
}


void WebFileWidget::itemDownload(QTreeWidgetItem *item)
{

    QString path = item->text(0);
    QTreeWidgetItem *parent = item->parent();
    while(parent) {
        path.prepend(parent->text(0) + "/");
        parent = item->parent();
    }
    rhandler->DownloadFile(QDir(path));
}

void WebFileWidget::itemCheckState(QTreeWidgetItem *item, Qt::CheckState state)
{
    item->setCheckState(3,state);
    if(item->childCount() > 0) {
        int i;
        for(i=0;i<item->childCount();i++) {
            QTreeWidgetItem *childitem = item->child(i);
            this->itemCheckState(childitem, state);
        }
    }
}

void WebFileWidget::ShowMenu( QTreeWidgetItem*, int)
{
     QMenu menu(this);
     menu.addAction("Download Selected", this, SLOT(itemDownload(QTreeWidgetItem*)));
     menu.exec(this->mapToGlobal(QPoint(0,0)));
}

void WebFileWidget::getChildItem()
{
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if((*it)->childCount() == 0 && (*it)->childIndicatorPolicy() == QTreeWidgetItem::ShowIndicator) {
            this->setCurrentItem((*it));
            this->itemExpand((*it));
            return;
        }
        ++it;
    }
    this->collapseAll();
    this->setCurrentItem(this->topLevelItem(0));
    disconnect(rhandler, SIGNAL(ProcessComplete()), this, SLOT(getChildItem()));
}
