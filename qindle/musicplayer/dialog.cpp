#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    int playmode=settings.value("pref/playmode",2).toInt();
    switch(playmode){
    case 1:
        ui->radiosingle->setChecked(true);
        break;
    case 2:
        ui->radioseq->setChecked(true);
        break;
    case 3:
        ui->radioshuffle->setChecked(true);
    };
    ui->chk_repeat->setChecked(settings.value("pref/repeat",false).toBool());
    ui->spinBox->setValue(settings.value("pref/seekstep",30).toInt());
    ui->chkautosave->setChecked(settings.value("pref/autosave",false).toBool());
    ui->chkresume->setChecked(settings.value("pref/autoresume",false).toBool());
}

Dialog::~Dialog()
{
    if(this->result()) {
        if(ui->radiosingle->isChecked())
            settings.setValue("pref/playmode",1);
        else if(ui->radioseq->isChecked())
            settings.setValue("pref/playmode",2);
        else if(ui->radioshuffle->isChecked())
            settings.setValue("pref/playmode",3);
        settings.setValue("pref/repeat",ui->chk_repeat->isChecked());
        settings.setValue("pref/seekstep",ui->spinBox->value());
        settings.setValue("pref/autosave",ui->chkautosave->isChecked());
        settings.setValue("pref/autoresume",ui->chkresume->isChecked());
    }
    delete ui;
}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
