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
//    void paintEvent(QPaintEvent *event);

public slots:
    void setFolderContent(QStringList *list);
    void getChildItem();
    void ProcessSelected();
    void StopDownload();

private slots:
    void itemExpand(QTreeWidgetItem *item);
    void itemClick(QTreeWidgetItem * item, int);
    void itemDownload(QTreeWidgetItem *item);
    void ShowMenu(QTreeWidgetItem *item, int);
    void DownloadProgress(qint64 bytes);
    void SelectAll();
    void SelectNone();

private:
    resthandler *rhandler;
    QTreeWidgetItem *DownloadingItem;
    void itemCheckState(QTreeWidgetItem *item, Qt::CheckState state);
};

#endif // WEBFILEWIDGET_H
