#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QUrl>
#include <QInputDialog>
#include <QDBusConnection>
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player=new dbusplayer();
    //DBus signal connection
    QDBusConnection::systemBus().connect(QString(), QString(), "com.lab126.audio", "audioPlaying", this,SLOT(AudioPlay()));
    QDBusConnection::systemBus().connect(QString(), QString(), "com.lab126.audio", "audioStopped", this,SLOT(AudioStop()));
    QDBusConnection::systemBus().connect(QString(), QString(), "com.lab126.audio", "audioEOS", this,SLOT(AudioEOS()));
    QDBusConnection::systemBus().connect(QString(), QString(), "com.lab126.audio", "audioFailed", this,SLOT(AudioFailed()));
    //commandline argument
    QStringList arg=QCoreApplication::arguments();
    if (arg.count()==2)
        addFile(arg.at(1));
    //resume autosave
    settings=new QSettings();
    if (settings->value("pref/autosave",false).toBool()){
        ui->playlist->addItems(settings->value("pref/currentlist",QStringList()).toStringList());
        ui->playlist->setCurrentRow(settings->value("pref/currentrow",0).toInt());
    }
    //resume autoplay
    if (settings->value("pref/autoresume",false).toBool() && ui->playlist->count())
        on_action_Play_triggered();

}

MainWindow::~MainWindow()
{

    if (settings->value("pref/autosave",true).toBool()){
        int i;
        QStringList tmplist;
        for(i=0;i<ui->playlist->count();i++) {
            tmplist << ui->playlist->item(i)->text();
        }
        settings->setValue("pref/currentlist",tmplist);
        settings->setValue("pref/currentrow",ui->playlist->currentRow());

    };
    delete settings;
    delete ui;
    player->setControlInt(3);
    delete player;
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

void MainWindow::on_actionOpen_P_laylist_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Playlist"), QDir::currentPath(),tr("Playlist (*.m3u *.pls)"));
    if(fileName.isEmpty()) return;
    currentList.setFileName(fileName);
    QString suffix=QFileInfo(currentList).suffix();
    if (suffix=="m3u") {
        ui->playlist->clear();
        if (currentList.open(QFile::ReadOnly)) {
            char buf[256];
            while(!(currentList.atEnd())) {
                currentList.readLine(buf, sizeof(buf));
                ui->playlist->addItem(QString(buf).left(QString(buf).size()-1));
            }
        }
        currentList.close();
    } else if (suffix=="pls") {
        ui->playlist->clear();
        if (currentList.open(QFile::ReadOnly)) {
            char buf[256];
            while(!(currentList.atEnd())) {
                currentList.readLine(buf, sizeof(buf));
                QString line=QString(buf);
                if (line.startsWith("File")) {
                    ui->playlist->addItem(line.section("=",0));
                }
            }
        }
        currentList.close();
    } else {
        QMessageBox::critical(0,tr("System Error"),tr("Cannot read file."),QMessageBox::Ok);
    };
}


void MainWindow::on_action_Save_Playlist_triggered()
{
    QString suffix=QFileInfo(currentList).suffix();
    if (suffix=="m3u") {
        if (currentList.open(QFile::WriteOnly | QIODevice::Text)) {
            int i;
            for(i=0;i<ui->playlist->count();i++) {
                currentList.write(ui->playlist->item(i)->text().toAscii());
                currentList.write("\n");
            }
        }
        currentList.close();
    } else {
        on_actionSave_Playlist_As_triggered();
    }
}

void MainWindow::on_actionSave_Playlist_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Playlist As"), QDir::currentPath(),tr("Playlist (*.m3u)"));
    if(fileName.isEmpty()) return;
    if (!(fileName.endsWith(".m3u")))
        fileName.append(".m3u");
    currentList.setFileName(fileName);
    on_action_Save_Playlist_triggered();
}

void MainWindow::addFile(QString filename)
{
    ui->playlist->addItem(filename);
}

void MainWindow::on_actionA_dd_File_triggered()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", QDir::currentPath(), "MP3 Files (*.mp3)");
    QString str;
    foreach (str, files)
        ui->playlist->addItem(str);
}

void MainWindow::on_actionRe_move_File_triggered()
{
    ui->playlist->takeItem(ui->playlist->currentRow());
}

void MainWindow::on_action_Quit_triggered()
{
    on_action_Stop_triggered();
    this->close();
}

void MainWindow::on_action_Play_triggered()
{
    int i=player->getControlInt();
    if(i!=3 || URI!=player->getURIStr()) {
        if(URI.isEmpty())
            on_playlist_currentRowChanged(ui->playlist->currentRow());
        player->setURIStr(URI);
    };
    player->setControlInt(1);
}

void MainWindow::on_action_Stop_triggered()
{
    player->setControlInt(3);
}

void MainWindow::on_actionP_ause_triggered()
{
    player->setControlInt(2);
}

void MainWindow::on_action_Next_File_triggered()
{
    if(ui->playlist->currentRow() < ui->playlist->count()-1)
        ui->playlist->setCurrentRow(ui->playlist->currentRow()+1);
    else if (settings->value("pref/repeat",false).toBool())
        ui->playlist->setCurrentRow(0);
}

void MainWindow::on_actionP_rivous_File_triggered()
{
    if(ui->playlist->currentRow() > 0)
        ui->playlist->setCurrentRow(ui->playlist->currentRow()-1);
    else
        ui->playlist->setCurrentRow(ui->playlist->count()-1);
}

void MainWindow::on_actionF_irst_File_triggered()
{
    ui->playlist->setCurrentRow(0);
}

void MainWindow::on_action_Last_File_triggered()
{
    ui->playlist->setCurrentRow(ui->playlist->count()-1);
}


void MainWindow::on_action_Volume_Up_triggered()
{
    int i=player->getCmdIVolumeInt();
    player->setCmdIVolumeInt(i+1);
}

void MainWindow::on_actionV_olume_Down_triggered()
{
    int i=player->getCmdIVolumeInt();
    if(i>0)
        player->setCmdIVolumeInt(i-1);
}

void MainWindow::on_action_Move_Top_triggered()
{
    QListWidgetItem* item=ui->playlist->takeItem(ui->playlist->currentRow());
    ui->playlist->insertItem(0,item);
}


void MainWindow::on_actionMove_Up_triggered()
{
    int row=ui->playlist->currentRow();
    if(row>0) {
        QListWidgetItem* item=ui->playlist->takeItem(ui->playlist->currentRow());
        ui->playlist->insertItem(row-1,item);
    }
}

void MainWindow::on_actionMove_Down_triggered()
{
    int row=ui->playlist->currentRow();
    if(row< ui->playlist->count()-1) {
        QListWidgetItem* item=ui->playlist->takeItem(ui->playlist->currentRow());
        ui->playlist->insertItem(row+1,item);
    }
}

void MainWindow::on_actionMove_Bottom_triggered()
{
    QListWidgetItem* item=ui->playlist->takeItem(ui->playlist->currentRow());
    ui->playlist->insertItem(ui->playlist->count(),item);
}

void MainWindow::on_playlist_currentRowChanged(int currentRow)
{
    URI=QUrl::fromLocalFile(QFileInfo(QFileInfo(currentList).absoluteDir(),ui->playlist->item(currentRow)->text()).absoluteFilePath()).toString();

}

void MainWindow::on_playlist_activated(QModelIndex index)
{
    int i=player->getControlInt();
    if(i==4)
        on_actionP_ause_triggered();
    else
        on_action_Play_triggered();
}

void MainWindow::on_action_Foward_triggered()
{
    int location=player->getSeekInt();
    player->setSeekInt(location+settings->value("pref/seekstep",30).toInt());
}

void MainWindow::on_action_Reward_triggered()
{
    int location=player->getSeekInt();
    if(location>=settings->value("pref/seekstep",30).toInt())
        location-=settings->value("pref/seekstep",30).toInt();
    else
        location=0;
    player->setSeekInt(location);
}

void MainWindow::on_actionS_eek_triggered()
{
    bool ok;
    int i = QInputDialog::getInt(this,tr("Seek"),tr("Time(second):"), 0, 0, 3600, 1, &ok);
    if (ok)
        player->setSeekInt(i);
}
void MainWindow::AudioPlay()
{
    ui->statusBar->showMessage(tr("Playing"));
}

void MainWindow::AudioStop()
{
    ui->statusBar->showMessage(tr("Stopped"));
}
void MainWindow::AudioEOS()
{
    int playmode=settings->value("pref/playmode",2).toInt();
    int i;
    switch(playmode){
    case 1:
        on_action_Stop_triggered();
        if (settings->value("pref/repeat",false).toBool())
            on_action_Play_triggered();
        break;
    case 2:
        i=ui->playlist->currentRow();
        on_action_Next_File_triggered();
        if (i!=ui->playlist->currentRow())
            on_action_Play_triggered();
        break;
    case 3:
        i=qrand() % (ui->playlist->count());
        ui->playlist->setCurrentRow(i);
        on_action_Play_triggered();
    };

}
void MainWindow::AudioFailed()
{
    on_action_Stop_triggered();
    QMessageBox::StandardButton ret=QMessageBox::warning ( this, tr("File Error"), tr("Cannot play this file. \nRemove from playlist?"), QMessageBox::Yes | QMessageBox::No);
    if(ret==QMessageBox::Yes)
        on_actionRe_move_File_triggered();
}

void MainWindow::on_actionP_reference_triggered()
{
    Dialog dialog;
    dialog.exec();
}

void MainWindow::on_action_About_triggered()
{
    QString text=tr("Simple music player for Kindle.\nThis program is a part of project Qindle.\nCopyright (C) 2010 Li Miao <lm3783@gmail.com>");
    QMessageBox::about(this,"About Qindle",text);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this,"About Qt");
}
