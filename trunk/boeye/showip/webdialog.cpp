#include "webdialog.h"
#include "ui_webdialog.h"

webDialog::webDialog(QWidget *parent, FBUpdate* update) :
    QDialog(parent),
    ui(new Ui::webDialog)
{
    ui->setupUi(this);

    this->update = update;
    connect(this, SIGNAL(UpdateWindow()), this->update,SLOT(Update()));

}

webDialog::~webDialog()
{
    delete ui;

}

void webDialog::paintEvent(QPaintEvent *)
{
    emit(this->UpdateWindow());

}
