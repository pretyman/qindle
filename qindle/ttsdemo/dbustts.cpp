#include "dbustts.h"
#include <QMessageBox>
#include <QDBusConnection>

dbustts::dbustts() : QDBusInterface("com.lab126.tts", "/default", "com.lab126", QDBusConnection::systemBus())
{
}

dbustts::~dbustts()
{

}

int dbustts::setTtsSVoiceStr(QString voice)
{
    QDBusMessage ret=call("setTtsSVoiceStr",voice);
    return ret.arguments().at(0).toInt();
}

int dbustts::setTtsISpeedInt(int i)
{
    QDBusMessage ret=call("setTtsISpeedInt", i);
    return ret.arguments().at(0).toInt();
}

int dbustts::getTtsISpeedInt()
{
    QDBusMessage ret=call("getTtsISpeedInt");
    return ret.arguments().at(1).toInt();
}

int dbustts::setTextToProcessInt(int i)
{
    QDBusMessage ret=call("setTextToProcessInt", i);
    return ret.arguments().at(0).toInt();
}

int dbustts::setTtsIPitchInt(int i)
{
    QDBusMessage ret=call("setTtsIPitchInt", i);
    return ret.arguments().at(0).toInt();
}

int dbustts::getTtsIPitchInt()
{
    QDBusMessage ret=call("getTtsIPitchInt");
    return ret.arguments().at(1).toInt();
}
