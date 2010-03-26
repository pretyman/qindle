#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller=new okular::Controller();
//    controller->openFile("/home/nemo/DjVu_Tech_Primer.djvu");
    controller->openFile("/home/nemo/2009_Gupta_et_al_JNN.pdf");
//    controller->openFile("/home/nemo/temp/gplrelease/everest.chm");
//    controller->openFile("/home/nemo/temp/gplrelease/VirtualBox.chm");
//    controller->openFile("/home/nemo/win/temp/alice.epub");
    ui->verticalLayout->addWidget(controller->getView(ui->centralWidget));

}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_action_Open_triggered()
{
    controller->gotoPage(0);
}

void MainWindow::on_action_Next_Page_triggered()
{
    int page=controller->getCurrentPageNo();
    QMessageBox::about(0,QString(),QString::number(page));

    controller->gotoPage(page+1);
}
