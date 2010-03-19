#ifndef DBUSTTS_H
#define DBUSTTS_H
#include <QDBusInterface>
#include <QString>

class dbustts : public QDBusInterface
{
    Q_OBJECT
public:
    dbustts();
    ~dbustts();
    int setTtsSVoiceStr(QString voice);
    int setTtsISpeedInt(int i);
    int getTtsISpeedInt();
    int setTextToProcessInt(int i);
    int getTtsIPitchInt();
    int setTtsIPitchInt(int i);
};

#endif // DBUSTTS_H
