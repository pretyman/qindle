#ifndef WEBDIALOG_H
#define WEBDIALOG_H

#include <QDialog>
#include <QPaintEvent>
#include <QRect>
#include "webfilewidget.h"
#include "webpagewidget.h"
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

public slots:
    void LoginSuccess();
    void showWeb();
    void FileNotFound();
    void WebPageFinished();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::webDialog *ui;
    resthandler *rhandler;
    WebFileWidget *filelist;
    WebPageWidget *webview;
};

#endif // WEBDIALOG_H
