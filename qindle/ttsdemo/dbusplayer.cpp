#include "dbusplayer.h"
#include <QMessageBox>
#include <QDBusConnection>

dbusplayer::dbusplayer() : QDBusInterface("com.lab126.audio", "/default", "com.lab126", QDBusConnection::systemBus())
{

}

dbusplayer::~dbusplayer()
{

}

int dbusplayer::setControlInt(int i)
{
    QDBusMessage ret=call("setControlInt", i);
    return ret.arguments().at(0).toInt();
}

int dbusplayer::getControlInt()
{
    QDBusMessage ret=call("getControlInt");
    return ret.arguments().at(1).toInt();
}

int dbusplayer::setURIStr(QString URI)
{
    QDBusMessage ret=call("setURIStr",URI);
    return ret.arguments().at(0).toInt();
}

QString dbusplayer::getURIStr()
{
    QDBusMessage ret=call("getURIStr");
    return ret.arguments().at(1).toString();
}

int dbusplayer::setCmdIVolumeInt(int i)
{
    QDBusMessage ret=call("setCmdIVolumeInt", i);
    return ret.arguments().at(0).toInt();

}

int dbusplayer::getCmdIVolumeInt()
{

    QDBusMessage ret=call("getCmdIVolumeInt");
    return ret.arguments().at(1).toInt();
}

int dbusplayer::setSeekInt(int i)
{
    QDBusMessage ret=call("setSeekInt", i);
    return ret.arguments().at(0).toInt();
}

int dbusplayer::getSeekInt()
{
    QDBusMessage ret=call("getSeekInt");
    return ret.arguments().at(1).toInt();
}

int dbusplayer::setKillInt()
{
    QDBusMessage ret=call("setKillInt", 1);
    return ret.arguments().at(0).toInt();
}
