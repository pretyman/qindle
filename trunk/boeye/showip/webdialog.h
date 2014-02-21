#ifndef WEBDIALOG_H
#define WEBDIALOG_H

#include <QDialog>
#include <QPaintEvent>
#include <QRect>
#include <QTreeWidget>
#include "resthandler.h"

namespace Ui {
class webDialog;
}

class webDialog : public QDialog
{
    Q_OBJECT

public:
    explicit webDialog(QWidget *parent = 0);
    ~webDialog();

signals:
    void UpdateWindow(QRect region);
    void getFolderContent(QDir path);

public slots:
    void LoginSuccess();
    void showWeb();
    void setFolderContent(QList<QString> *list);
    void itemExpanded(QTreeWidgetItem *item);
    void itemClicked(QTreeWidgetItem * item, int);
    void itemDownload(QTreeWidgetItem *item);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::webDialog *ui;
    resthandler *rhandler;
    QTreeWidget *filelist;
};

#endif // WEBDIALOG_H
