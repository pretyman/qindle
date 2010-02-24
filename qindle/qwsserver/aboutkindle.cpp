#include "aboutkindle.h"
#include "ui_aboutkindle.h"
#include "hwinfo.h"

aboutkindle::aboutkindle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutkindle)
{
    ui->setupUi(this);
    hwinfo hwinfo;
    hwinfo.setTable(ui->hdware);
    hwinfo.setTable(ui->batt);
}

aboutkindle::~aboutkindle()
{
    delete ui;
}

void aboutkindle::changeEvent(QEvent *e)
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
