#ifndef MENUEVENT_H
#define MENUEVENT_H

#include <QObject>
#include <QEvent>

class menuevent : public QObject
{

protected:
     bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MENUEVENT_H
