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

#include "ImageBasedView.h"
#include <QPixmap>
#include <QMessageBox>
#include <QKeyEvent>

using namespace okular;

ImageBasedView::ImageBasedView(QWidget *p) : QLabel(p)
{
    scalemode=Qt::KeepAspectRatioByExpanding;
    m_machine.m_type=Fit;
    m_machine.m_state=Begin;
}

int ImageBasedView::loadImage(QImage image)
{
    backbuffer=QPixmap::fromImage(image);
    this->setPixmap(backbuffer.copy(viewerpoint));
    return 0;
}

void ImageBasedView::updateSize()
{
    viewerpoint.setSize(this->size());
    setZoomFactor(1);
}

void ImageBasedView::setZoomFactor(qreal factor)
{
    zoomFactor=factor;
    backSize.setHeight((int)(viewerpoint.height() * zoomFactor));
    backSize.setWidth((int)(viewerpoint.width() * zoomFactor));

}
//This part of code is too ugly to refine....
void ImageBasedView::keyPressEvent(QKeyEvent *ev)
{
    QLabel::keyPressEvent(ev);
    //-1 means privout state, 1 means next state
    int action=0;
    switch(m_machine.m_state) {
    case Begin:
        switch(ev->key()) {
        case Qt::Key_PageDown:
            action=1;
            break;
        case Qt::Key_PageUp:
            m_machine.m_state=End;
            viewerpoint.setX(backbuffer.width()-viewerpoint.width());
            viewerpoint.setY(backbuffer.height()-viewerpoint.height());
            viewerpoint.setSize(this->size());
            emit(gotoPage(false));
            break;
        }
        break;

    case End:
        switch(ev->key()) {
        case Qt::Key_PageDown:
            m_machine.m_state=Begin;
            viewerpoint.setX(0);
            viewerpoint.setY(0);
            viewerpoint.setSize(this->size());
            emit(gotoPage(true));
            break;
        case Qt::Key_PageUp:
            action=-1;
            break;
        }
        break;

    case Second:
        switch(ev->key()) {
        case Qt::Key_PageUp:
            m_machine.m_state=Begin;
            viewerpoint.adjust(0, -(viewerpoint.height()), 0, -(viewerpoint.height()));
            break;
        case Qt::Key_PageDown:
            m_machine.m_state=Third;
            viewerpoint.adjust(viewerpoint.width(), -(viewerpoint.height()), viewerpoint.width(), -(viewerpoint.height()));
            break;
        }
        emit(refresh());
        break;

    case Third:
        switch(ev->key()) {
        case Qt::Key_PageDown:
            if(m_machine.m_type==Split4) {
                m_machine.m_state=End;
            } else if(m_machine.m_type==Split6) {
                m_machine.m_state=Fourth;
            };
            viewerpoint.adjust(0, viewerpoint.height(), 0, viewerpoint.height());
            break;
        case Qt::Key_PageUp:
            m_machine.m_state=Second;
            viewerpoint.adjust(-(viewerpoint.width()), viewerpoint.height(), -(viewerpoint.width()), viewerpoint.height());
            break;
        }
        emit(refresh());
        break;

    case Fourth:
        switch(ev->key()) {
        case Qt::Key_PageUp:
            m_machine.m_state=Third;
            viewerpoint.adjust(0, -(viewerpoint.height()), 0, -(viewerpoint.height()));
            break;
        case Qt::Key_PageDown:
            m_machine.m_state=Fifth;
            viewerpoint.adjust(viewerpoint.width(), -(viewerpoint.height()), viewerpoint.width(), -(viewerpoint.height()));
            break;
        }
        emit(refresh());
        break;

    case Fifth:
        switch(ev->key()) {
        case Qt::Key_PageDown:
            m_machine.m_state=End;
            viewerpoint.adjust(0, viewerpoint.height(), 0, viewerpoint.height());
            break;
        case Qt::Key_PageUp:
            m_machine.m_state=Fourth;
            viewerpoint.adjust(-(viewerpoint.width()), viewerpoint.height(), -(viewerpoint.width()), viewerpoint.height());
            break;
        }
        emit(refresh());
        break;

    case None:
        switch(ev->key()) {
        case Qt::Key_PageUp:
            emit(gotoPage(false));
            break;
        case Qt::Key_PageDown:
            emit(gotoPage(true));
            break;
        }
        break;

    case setTrim:
        *marginmap=backbuffer;
        MarginPainter->begin(marginmap);
        switch(ev->key()) {
        case Qt::Key_Up:
            margin->adjust(0,-10,0,-10);
            MarginPainter->drawRect(*margin);
            MarginPainter->end();
            this->setPixmap(*marginmap);
            break;
        case Qt::Key_Down:
            margin->adjust(0,10,0,10);
            MarginPainter->drawRect(*margin);
            MarginPainter->end();
            this->setPixmap(*marginmap);
            break;
        case Qt::Key_Left:
            margin->adjust(-10,0,-10,0);
            MarginPainter->drawRect(*margin);
            MarginPainter->end();
            this->setPixmap(*marginmap);
            break;
        case Qt::Key_Right:
            margin->adjust(10,0,10,0);
            MarginPainter->drawRect(*margin);
            MarginPainter->end();
            this->setPixmap(*marginmap);
            break;
        case Qt::Key_PageUp:
            margin->adjust(10,10,-10,-10);
            MarginPainter->drawRect(*margin);
            MarginPainter->end();
            this->setPixmap(*marginmap);
            break;
        case Qt::Key_PageDown:
            margin->adjust(-10,-10,10,10);
            MarginPainter->drawRect(*margin);
            MarginPainter->end();
            this->setPixmap(*marginmap);
            break;
        case Qt::Key_Enter:
            this->setDisplayType(activeTrim);
            break;
        case Qt::Key_Return:
            this->setDisplayType(activeTrim);
            break;
        }
    }

    if(action==-1) {
        switch(m_machine.m_type) {
        case Fit:
            m_machine.m_state=Begin;
            viewerpoint.setX(0);
            viewerpoint.setY(0);
            viewerpoint.setSize(this->size());
            break;
        case Split2H:
            m_machine.m_state=Begin;
            viewerpoint.adjust(-(viewerpoint.width()), 0, -(viewerpoint.width()), 0);
            break;
        case Split2V:
            m_machine.m_state=Begin;
            viewerpoint.adjust(0, -(viewerpoint.height()), 0, -(viewerpoint.height()));
            break;
        case Split4:
            m_machine.m_state=Third;
            viewerpoint.adjust(0, -(viewerpoint.height()), 0, -(viewerpoint.height()));
            break;
        case Split6:
            m_machine.m_state=Fifth;
            viewerpoint.adjust(0, -(viewerpoint.height()), 0, -(viewerpoint.height()));
        }
        emit(refresh());
    } else if(action==1) {
        switch(m_machine.m_type) {
        case Fit:
            m_machine.m_state=End;
            viewerpoint.setX(backbuffer.width()-viewerpoint.width());
            viewerpoint.setY(backbuffer.height()-viewerpoint.height());
            viewerpoint.setSize(this->size());
            break;
        case Split2H:
            m_machine.m_state=End;
            viewerpoint.adjust(viewerpoint.width(), 0, viewerpoint.width(), 0);
            break;
        case Split2V:
            m_machine.m_state=End;
            viewerpoint.adjust(0, viewerpoint.height(), 0, viewerpoint.height());
            break;
        case Split4:
            m_machine.m_state=Second;
            viewerpoint.adjust(0, viewerpoint.height(), 0, viewerpoint.height());
            break;
        case Split6:
            m_machine.m_state=Second;
            viewerpoint.adjust(0, viewerpoint.height(), 0, viewerpoint.height());
        }
        emit(refresh());
    }
}

void ImageBasedView::setDisplayType(int newtype)
{
    //These are the most common set. Can be changed if necessary.
    m_machine.m_state=Begin;
    scalemode=Qt::KeepAspectRatio;
    setZoomFactor(1);
    viewerpoint.setX(0);
    viewerpoint.setY(0);
    viewerpoint.setSize(this->size());

    m_machine.m_type=(Type)newtype;
    switch(m_machine.m_type) {
    case Fit:
        scalemode=Qt::KeepAspectRatioByExpanding;
        backSize=viewerpoint.size();
        break;
    case All:
        m_machine.m_state=None;
        backSize=viewerpoint.size();
        break;
    case Split2H:
        backSize.setHeight(viewerpoint.height() * 2);
        backSize.setWidth(viewerpoint.width());
        break;
    case Split2V:
        backSize.setHeight(viewerpoint.height());
        backSize.setWidth(viewerpoint.width() * 2);
        break;
    case Split4:
        backSize.setHeight(viewerpoint.height() *2);
        backSize.setWidth(viewerpoint.width() * 2);
        break;
    case Split6:
        backSize.setHeight(viewerpoint.height() *2);
        backSize.setWidth(viewerpoint.width() * 3);
        break;
    case prepareTrim:
        m_machine.m_state=setTrim;
        backSize=viewerpoint.size();
        emit(refresh());
        QMessageBox::information(this, tr("Set Margin"), tr("After closing this message, use directional key to move the rectangle, and PageUp/PagrDown to zoom. When it fits the text, press enter to confirm."));

        marginmap=new QPixmap(backbuffer);
        margin=new QRect();
        margin->setSize(viewerpoint.size());

        margin->adjust(100,100,-100,-100);
        MarginPainter=new QPainter(marginmap);
        MarginPainter->setPen(Qt::SolidLine);
        MarginPainter->drawRect(*margin);
        MarginPainter->end();
        this->setPixmap(*marginmap);
        return;
    case activeTrim:
        m_machine.m_state=None;
        this->setZoomFactor((qreal)viewerpoint.height() / (qreal)margin->height());
        viewerpoint.setX((int)(margin->x() * zoomFactor));
        viewerpoint.setY((int)(margin->y() * zoomFactor));
        viewerpoint.setSize(this->size());
        delete margin;
        delete MarginPainter;
        delete marginmap;
    };
    emit(refresh());
}

