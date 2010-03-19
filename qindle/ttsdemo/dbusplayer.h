#ifndef DBUSPLAYER_H
#define DBUSPLAYER_H

#include <QObject>
#include <QString>
#include <QDBusInterface>

class dbusplayer : public QDBusInterface
{
    Q_OBJECT
public:
    dbusplayer();
    ~dbusplayer();
    int setControlInt(int i);
    int getControlInt();
    int setURIStr(QString URI);
    QString getURIStr();
    int getCmdIVolumeInt();
    int setCmdIVolumeInt(int i);
    int getSeekInt();
    int setSeekInt(int i);
    int setKillInt();

};
#endif // DBUSPLAYER_H
