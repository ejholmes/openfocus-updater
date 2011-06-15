#include <QtGui>
#include <stdio.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openfocus.h"
#include "intelhex.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bootloader = new OpenFocus::Bootloader();
    flashdata = NULL;
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

    if (filename != "") {
        FILE *fp = fopen(filename.toLocal8Bit().constData(), "r");
        record *records = IntelHexFile::Open(fp);
        flashdata = IntelHexFile::RecordsToFlashData(records);

        IntelHexFile::FreeRecords(records);
        fclose(fp);

        ui->pushButton_3->setEnabled(true);
    }

    qDebug() << filename;
}

void MainWindow::on_pushButton_3_clicked()
{
    if (flashdata != NULL) {
        if (!bootloader->WriteFlash(flashdata->data, flashdata->size)) {
            qDebug() << "An error occured while trying to write flash data";
            return;
        }

        bootloader->Reboot();

        ui->pushButton->setDisabled(true);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
    }
    else {
        qDebug() << "An error occured";
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //bootloader->Reboot();
    bootloader->Disconnect();
    event->accept();
}
