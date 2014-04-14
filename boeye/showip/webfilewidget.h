#ifndef WEBFILEWIDGET_H
#define WEBFILEWIDGET_H

#include <QTreeWidget>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QRect>
#include <QTimer>
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
    void itemSelect();
    void ShowMenu();
    void SelectAll();
    void SelectNone();
    void DownloadProgress();

private:
    resthandler *rhandler;
    QTreeWidgetItem *DownloadingItem;
    QTimer *timer;
    void itemCheckState(QTreeWidgetItem *item, Qt::CheckState state);
    void itemExpand(QTreeWidgetItem *item);
    void itemDownload(QTreeWidgetItem *item);

};

#endif // WEBFILEWIDGET_H
