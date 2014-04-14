#include "dialog.h"
#include "ui_dialog.h"

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostAddress>
#include <QtDBus/QDBusInterface>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    this->fbupdate = new FBUpdate();
    fbupdate->Init();
    connect(this, SIGNAL(UpdateWindow(QRect)), this->fbupdate,SLOT(Update(QRect)));

    QDBusInterface splash( "com.sibrary.BoeyeServer", "/StartupSplash", "com.sibrary.Service.StartupSplash" );
    splash.call("closeSplash");
    splash.connection().send(QDBusMessage::createSignal("/USB", "com.sibrary.Service.USB", "umsConnected"));

    QString ipaddr = "ftp://";
        QList<QHostAddress> list = QNetworkInterface::allAddresses();
        foreach (QHostAddress address, list) {
            if(address.protocol() == QAbstractSocket::IPv4Protocol and address != QHostAddress::LocalHost) {
                ipaddr = ipaddr + address.toString();
                break;
            }
        }

    ui->label_2->setText(ipaddr);
}

Dialog::~Dialog()
{
    QDBusConnection::sessionBus().send(QDBusMessage::createSignal("/USB", "com.sibrary.Service.USB", "umsDisconnected"));
    delete ui;
    this->fbupdate->Close();
    delete fbupdate;
}

void Dialog::paintEvent(QPaintEvent *event)
{
    QRect region = event->rect();
    emit(this->UpdateWindow(region));

}

void Dialog::on_buttonBox_accepted()
{
    webDialog* download = new webDialog(this);
    connect(download, SIGNAL(UpdateWindow(QRect)), fbupdate, SLOT(Update(QRect)));
    download->show();
}
