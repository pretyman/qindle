#include "hwinfo.h"
#include <QDate>
#include <QTime>

hwinfo::hwinfo()
{   
}

QString hwinfo::getInfo(QString name)
{
    QString value;
    if (name=="Date")
        value=QDate::currentDate().toString();
    else if (name=="Time")
        value=QTime::currentTime().toString();
    else if (name=="Battery")
        value=getbyLipc("com.lab126.powerd","battLevel",1);

    else if (name=="Wireless")
        value=getbyLipc("com.lab126.wan","powerState",1);
    else if (name=="Temperature")
        value=getbyFile("/proc/eink_fb/temperature");
    else if (name=="Free Space")
        value=getbyLipc("com.lab126.volumd","userstoreFreeSpace",1);
    else if (name=="Waveform Version")
        value=getbyLipc("com.lab126.system","waveformversion",0);
    else if (name=="Version")
        value=getbyLipc("com.lab126.system","version",0);
    else if (name=="USID")
        value=getbyLipc("com.lab126.system","usid",0);
    else if (name=="Charge In Percent")
        value=getbyProgram("gasgauge-info",QStringList("-s"));
    else if (name=="battery voltage")
        value=getbyProgram("gasgauge-info",QStringList("-v"));
    else if (name=="battery current")
        value=getbyProgram("gasgauge-info",QStringList("-l"));
    else if (name=="battery temperature")
        value=getbyProgram("gasgauge-info",QStringList("-k"));
    else if (name=="battery available capacity")
        value=getbyProgram("gasgauge-info",QStringList("-m"));
    else
        value="0";

    return value;
}

void hwinfo::setTable(QTableWidget* table)
{
    int count,i;
    QString name;
    QString value;
    QTableWidgetItem* item;
    count=table->rowCount();
    for (i=0;i<count;i++) {
        name=table->verticalHeaderItem(i)->text();
        value=getInfo(name);
        item=new QTableWidgetItem(value,0);
        table->setItem(i,0,item);
        table->resizeColumnToContents(0);
    }
}
QString hwinfo::getbyLipc(QString publisher, QString property, int type)
{
    QStringList args;
    if (type)
        args << "-i";
    else
        args << "-s";
    args << publisher << property;
    return getbyProgram("lipc-get-prop",args);
}
QString hwinfo::getbyProgram(QString name, QStringList args)
{
    process.start(name,args);
    process.closeWriteChannel();
    process.waitForFinished(1000);
    QByteArray result = process.readLine(80);
    process.close();
    return QString(result);
}
QString hwinfo::getbyFile(QString filename)
{
    QFile* file=new QFile(filename);
    file->open(QIODevice::ReadOnly);
    QByteArray result = file->readAll();
    file->close();
    delete(file);
    return QString(result);
}
