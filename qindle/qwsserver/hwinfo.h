#ifndef HWINFO_H
#define HWINFO_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QFile>
#include <QTableWidgetItem>
#include <QTableWidget>

class hwinfo : public QObject
{
    Q_OBJECT
public:
    hwinfo();
    QString getInfo(QString name);
    void setTable(QTableWidget* table);

private:
    QProcess process;
    QString getbyProgram(QString name, QStringList args);
    QString getbyFile(QString filename);
    QString getbyLipc(QString publisher, QString property, int type);
};

#endif // HWINFO_H
