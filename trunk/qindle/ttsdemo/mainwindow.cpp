#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QString>
#include <QDBusConnection>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player=new dbusplayer();
    tts=new dbustts();
    QDBusConnection::systemBus().connect(QString(), QString(), "com.lab126.audio", "audioAvailable", this,SLOT(AudioAvail()));
    //ui->volumebox->setValue(player->getCmdIVolumeInt());
    //ui->speedbox->setValue(tts->getTtsISpeedInt());
    //ui->pitchbox->setValue(tts->getTtsIPitchInt());
}

MainWindow::~MainWindow()
{
    delete ui;
    player->setKillInt();
    delete player;
    delete tts;


}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_okButton_clicked()
{
    ui->statusBar->showMessage(tr("Processing, please wait..."));
    player->setKillInt();
}

void MainWindow::AudioAvail()
{
    QString voice;
    if(ui->radiotom->isChecked())
        voice="Tom";
    else if(ui->radiosam->isChecked())
        voice="Samantha";
    tts->setTtsSVoiceStr(voice);
    tts->setTtsIPitchInt(ui->pitchbox->value());
    tts->setTtsISpeedInt(ui->speedbox->value());
    player->setCmdIVolumeInt(ui->volumebox->value());
    player->setURIStr("tts:///var/tmp/ttsUSFifo");
    QFile fifo("/var/tmp/ttsUSFifo");
    fifo.open(QFile::WriteOnly);
    int size=fifo.write(ui->lineEdit->text().toAscii());
    //tts->setTextToProcessInt(size);
    fifo.close();
    player->setControlInt(1);
}
