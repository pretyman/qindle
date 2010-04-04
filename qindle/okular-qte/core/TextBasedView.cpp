/*
 * Copyright (C) 2010 Li Miao <lm3783@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "TextBasedView.h"
#include <QUrl>
#include <QWebFrame>
#include <QKeyEvent>

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

void TextBasedView::keyPressEvent(QKeyEvent *ev)
{
    QWebFrame* frame=this->page()->currentFrame();
    int key=ev->key();

    if(key==Qt::Key_PageDown && frame->scrollBarValue(Qt::Vertical) == frame->scrollBarMaximum(Qt::Vertical))
        emit(gotoPage(true));
    else if(key==Qt::Key_PageUp && frame->scrollBarValue(Qt::Vertical) == frame->scrollBarMinimum(Qt::Vertical))
        emit(gotoPage(false));
    else
        QWebView::keyPressEvent(ev);

    //qWarning("%d,%d,%d", frame->scrollBarValue(Qt::Vertical), frame->scrollBarMaximum(Qt::Vertical), frame->scrollBarMinimum(Qt::Vertical));
}
