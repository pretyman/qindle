#include "TextBasedView.h"
#include <QUrl>

using namespace okular;

TextBasedView::TextBasedView(QWidget *p) : QWebView(p)
{

}

void TextBasedView::setManager(QNetworkAccessManager* manager)
{
    this->page()->setNetworkAccessManager(manager);
}

int TextBasedView::loadFile(QString filename)
{
    this->load(QUrl::fromLocalFile(filename));
    return 0;
}
