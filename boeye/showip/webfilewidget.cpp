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
    this->DownloadingItem = 0;
}

void WebFileWidget::connectHandler(resthandler *handler)
{
    this->rhandler = handler;
    connect(this, SIGNAL(getFolderContent(QDir)), rhandler, SLOT(getFileList(QDir)));
    connect(rhandler, SIGNAL(gotFileList(QStringList*)), this, SLOT(setFolderContent(QStringList*)));
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

void WebFileWidget::setFolderContent(QStringList *list)
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
        str = list->takeFirst();
        item->setText(2,str);
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
        parent = parent->parent();
    }
    if(item->text(2) == "Download") {
        if(item->text(1) == "") {
            rhandler->ProcessFile(path, 2);
        } else {
            item->setText(2, "Downloading");
            connect(rhandler, SIGNAL(bytesWritten(qint64)), this, SLOT(DownloadProgress(qint64)));
            rhandler->ProcessFile(path, 1);
        }
    } else if(item->text(2) == "Delete") {
        if(item->text(1) == "") {
            rhandler->ProcessFile(path, 4);
        } else {
            rhandler->ProcessFile(path, 3);
        }
    }
}

void WebFileWidget::itemCheckState(QTreeWidgetItem *item, Qt::CheckState state)
{
    item->setCheckState(3,state);
    if(item->childCount() > 0) {
        this->expandItem(item);
        int i;
        for(i=0;i<item->childCount();i++) {
            QTreeWidgetItem *childitem = item->child(i);
            this->itemCheckState(childitem, state);
        }
    }
}

void WebFileWidget::ShowMenu( QTreeWidgetItem* item, int)
{
     QMenu menu(this);
     QPoint point = this->visualItemRect(item).bottomLeft();
     menu.addAction("Select All", this, SLOT(SelectAll()));
     menu.addAction("Select None", this, SLOT(SelectNone()));
     menu.addAction("Sync Selected", this, SLOT(ProcessSelected()));
     menu.addAction("Stop Download", this, SLOT(StopDownload()));
     menu.exec(this->mapToGlobal(point));
}

void WebFileWidget::DownloadProgress(qint64 bytes)
{
    int percentage = int((float(bytes) / this->DownloadingItem->text(1).toFloat()) * 100);
    QString progress = "Downloading(" + QString().setNum(percentage) + "%)";
    this->DownloadingItem->setText(2, progress);
    this->resizeColumnToContents(0);
}

void WebFileWidget::SelectAll()
{
    int i;
    for(i=0;i<this->topLevelItemCount();i++) {
        this->itemCheckState(this->topLevelItem(i), Qt::Checked);
    }
}

void WebFileWidget::SelectNone()
{
    int i;
    for(i=0;i<this->topLevelItemCount();i++) {
        this->itemCheckState(this->topLevelItem(i), Qt::Unchecked);
    }
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
    this->setCurrentItem(this->topLevelItem(0));
    disconnect(rhandler, SIGNAL(ProcessComplete()), this, SLOT(getChildItem()));
}

void WebFileWidget::ProcessSelected()
{
    if(this->DownloadingItem) {
        disconnect(rhandler, SIGNAL(bytesWritten(qint64)), this, SLOT(DownloadProgress(qint64)));
        this->DownloadingItem->setText(2, "Finished");

    } else {
        this->DownloadingItem = this->currentItem();
        connect(rhandler, SIGNAL(ProcessComplete()), this, SLOT(ProcessSelected()));
    }
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if((*it)->checkState(3) == Qt::Checked) {
            if((*it)->text(2) == "Finished") {
                (*it)->setCheckState(3,Qt::Unchecked);
            } else {
                this->DownloadingItem = (*it);
                emit(itemDownload((*it)));
                return;
            }
        }
        ++it;
    }
    this->DownloadingItem = 0;
    disconnect(rhandler, SIGNAL(ProcessComplete()), this, SLOT(ProcessSelected()));
}

void WebFileWidget::StopDownload()
{
    disconnect(rhandler, SIGNAL(ProcessComplete()), this, SLOT(ProcessSelected()));
}
