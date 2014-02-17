#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    this->update = new FBUpdate();
    update->Init();
    connect(this, SIGNAL(UpdateWindow()), this->update,SLOT(Update()));

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
    this->update->Close();
    delete update;
}

void Dialog::paintEvent(QPaintEvent *)
{
    emit(this->UpdateWindow());

}
