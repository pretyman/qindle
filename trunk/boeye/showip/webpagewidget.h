#ifndef WEBPAGEWIDGET_H
#define WEBPAGEWIDGET_H

#include <QtWebKit/QWebView>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QRect>

class WebPageWidget : public QWebView
{
    Q_OBJECT
public:
    explicit WebPageWidget(QWidget *parent = 0);

signals:
    void UpdateWindow(QRect region);

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent ( QKeyEvent * ev );

};

#endif // WEBPAGEWIDGET_H
