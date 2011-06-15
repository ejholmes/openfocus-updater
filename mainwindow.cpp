#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "OpenFocus.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    device = new OpenFocus::Device();
    bootloader = new OpenFocus::Bootloader();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    qDebug("Connecting to device");
    if (OpenFocus::Helper::ConnectBootloader()) {
        QString PageSize, FlashSize, EEPROMSize;
        PageSize.setNum(bootloader->PageSize);
        FlashSize.setNum(bootloader->FlashSize);
        EEPROMSize.setNum(bootloader->EEPROMSize);

        ui->listWidget->addItem(QString("Connected"));
        ui->listWidget->addItem(QString("Page size: ") + PageSize);
        ui->listWidget->addItem(QString("Flash size: ") + FlashSize);
        ui->listWidget->addItem(QString("EEPROM size: ") + EEPROMSize);

        ui->pushButton->setDisabled(true);
        ui->pushButton_2->setEnabled(true);
    }
    else {
        ui->listWidget->addItem(QString("Device not found"));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, QString("Open firmware update"), QString(""), QString("Firmware Update Files (*.hex);;All files (*.*)"));

    qDebug() << filename;
}

void MainWindow::on_pushButton_3_clicked()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //bootloader->Reboot();
    bootloader->Disconnect();
    event->accept();
}
