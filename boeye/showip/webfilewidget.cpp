#include "webfilewidget.h"
#include <QMenu>
#include <QTreeWidgetItemIterator>
#include <QShortcut>

WebFileWidget::WebFileWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    QStringList labels;
    labels.append(QString::fromUtf8("文件名"));
    labels.append(QString::fromUtf8("大小"));
    labels.append(QString::fromUtf8("操作"));
    labels.append(QString::fromUtf8("是否选择"));
    this->setHeaderLabels(labels);
    this->DownloadingItem = 0;
}

void WebFileWidget::connectHandler(resthandler *handler)
{
    this->rhandler = handler;
    connect(this, SIGNAL(getFolderContent(QDir)), rhandler, SLOT(getFileList(QDir)));
    connect(rhandler, SIGNAL(gotFileList(QStringList*)), this, SLOT(setFolderContent(QStringList*)));
    connect(rhandler, SIGNAL(ProcessComplete()), this, SLOT(getChildItem()));

    QShortcut *shortcut = new QShortcut(QKeySequence("F9"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(ShowMenu()));
    shortcut = new QShortcut(QKeySequence("Return"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(itemSelect()));

    emit(getFolderContent(QDir()));
}

void WebFileWidget::paintEvent(QPaintEvent *event)
{
    QTreeWidget::paintEvent(event);
    QRect region = event->rect();
    emit(this->UpdateWindow(region));

}

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
            if(item->text(2) == "Delete") {
                item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
            } else {
                item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
            }
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

void WebFileWidget::itemSelect()
{
    QTreeWidgetItem *item = this->currentItem();
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
            this->timer->start(2000);
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

void WebFileWidget::ShowMenu()
{
     QMenu menu(this);
     QTreeWidgetItem *item = this->currentItem();
     QPoint point = this->visualItemRect(item).bottomLeft();
     connect(&menu, SIGNAL(hovered(QAction*)), this, SLOT(UpdateMenu(QAction*)));
     menu.addAction(QString::fromUtf8("全选"), this, SLOT(SelectAll()));
     menu.addAction(QString::fromUtf8("取消选择"), this, SLOT(SelectNone()));
     menu.addAction(QString::fromUtf8("开始传输"), this, SLOT(ProcessSelected()));
     menu.addAction(QString::fromUtf8("停止传输"), this, SLOT(StopDownload()));
     menu.exec(this->mapToGlobal(point));
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

void WebFileWidget::DownloadProgress()
{
    int totalsize = this->DownloadingItem->text(1).toInt();
    int currentsize = rhandler->getDownloadPos();
    float percent = (float(currentsize) / float(totalsize)) * 100;
    QString string = "Downloading(" + QString().setNum(percent, 'g', 2) + "%)";
    this->DownloadingItem->setText(2, string);
    this->resizeColumnToContents(2);
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
        this->DownloadingItem->setText(2, "Finished");
        this->timer->stop();
    } else {
        this->DownloadingItem = this->currentItem();
        connect(rhandler, SIGNAL(ProcessComplete()), this, SLOT(ProcessSelected()));
        this->timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(DownloadProgress()));
    }
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if((*it)->checkState(3) == Qt::Checked) {
            if((*it)->text(2) == "Finished") {
                (*it)->setCheckState(3,Qt::Unchecked);
            } else {
                this->DownloadingItem = (*it);
                this->itemDownload((*it));
                return;
            }
        }
        ++it;
    }
    this->DownloadingItem = 0;
    disconnect(rhandler, SIGNAL(ProcessComplete()), this, SLOT(ProcessSelected()));
    delete this->timer;
}

void WebFileWidget::StopDownload()
{
    disconnect(rhandler, SIGNAL(ProcessComplete()), this, SLOT(ProcessSelected()));
}

void WebFileWidget::UpdateMenu(QAction *action)
{
    QRect region = action->parentWidget()->rect();
    emit(this->UpdateWindow(region));
}
