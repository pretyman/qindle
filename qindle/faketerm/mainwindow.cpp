#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&shell,SIGNAL(finished(int)),this,SLOT(readdata(int)));
    shell.setWorkingDirectory(KINDLE_ROOT);
    ui->currentdir->setText(shell.workingDirectory());
    ui->command->setFocus();
}

MainWindow::~MainWindow()
{
    shell.close();
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

void MainWindow::readdata(int i)
{
    if (i) {
        ui->result->setText(tr("Error!"));
    } else {
        ui->result->setText(QString(shell.readAll()));
    };
    ui->command->selectAll();
}

void MainWindow::on_submit_clicked()
{
    shell.start("/bin/sh");
    shell.write(ui->command->text().toAscii());
    shell.closeWriteChannel();

}

void MainWindow::on_changedir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), shell.workingDirectory(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    shell.setWorkingDirectory(dir);
    ui->currentdir->setText(shell.workingDirectory());
}
