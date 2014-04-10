#include "webpagewidget.h"

WebPageWidget::WebPageWidget(QWidget *parent) :
    QWebView(parent)
{
}

void WebPageWidget::paintEvent(QPaintEvent *event)
{
    QWebView::paintEvent(event);
    QRect region = event->rect();
    emit(this->UpdateWindow(region));
}

void WebPageWidget::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Up){
        ev = new QKeyEvent(ev->type(), Qt::Key_Tab, Qt::ShiftModifier);
    } else if(ev->key() == Qt::Key_Down) {
        ev = new QKeyEvent(ev->type(), Qt::Key_Tab, Qt::NoModifier);
    }
    QWebView::keyPressEvent(ev);
}
