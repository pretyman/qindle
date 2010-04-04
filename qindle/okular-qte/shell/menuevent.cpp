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

#include "menuevent.h"
#include <QKeyEvent>
#include <QtGui/QApplication>
#include <QContextMenuEvent>
#include <QPoint>

bool menuevent::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //qDebug("Ate key press %d", keyEvent->key());
        if(keyEvent->key()==Qt::Key_Menu) {
            QContextMenuEvent menuevent(QContextMenuEvent::Keyboard, QPoint());
            QCoreApplication::sendEvent(obj,&menuevent);
            //qDebug("Ate key press");
            return true;
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);

}
