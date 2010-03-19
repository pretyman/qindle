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
