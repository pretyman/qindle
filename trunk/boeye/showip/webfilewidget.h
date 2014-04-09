#ifndef WEBFILEWIDGET_H
#define WEBFILEWIDGET_H

#include <QTreeWidget>
#include <QPaintEvent>
#include <QRect>
#include "resthandler.h"

class WebFileWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit WebFileWidget(QWidget *parent = 0);
    void connectHandler(resthandler *handler);

signals:
    void getFolderContent(QDir path);
    void UpdateWindow(QRect region);

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void setFolderContent(QStringList *list);
    void getChildItem();
    void ProcessSelected();
    void StopDownload();
    void UpdateMenu(QAction *action);

private slots:
    void itemSelect(QTreeWidgetItem * item, int);
    void ShowMenu();
    void SelectAll();
    void SelectNone();

private:
    resthandler *rhandler;
    QTreeWidgetItem *DownloadingItem;
    void itemCheckState(QTreeWidgetItem *item, Qt::CheckState state);
    void itemExpand(QTreeWidgetItem *item);
    void itemDownload(QTreeWidgetItem *item);

};

#endif // WEBFILEWIDGET_H
