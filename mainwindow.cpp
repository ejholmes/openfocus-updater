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

void MainWindow::Log(QString text)
{
    ui->listLog->addItem(text);
    ui->listLog->count() - 1;
}

void MainWindow::on_btnConnect_clicked()
{
    qDebug("Connecting to device");
    if (OpenFocus::Helper::ConnectBootloader()) {
        QString PageSize, FlashSize, EEPROMSize;
        qDebug() << "Page Size:" << PageSize.setNum(bootloader->PageSize);
        qDebug() << "Flash Size:" <<FlashSize.setNum(bootloader->FlashSize);
        qDebug() << "EEPROM Size:" <<EEPROMSize.setNum(bootloader->EEPROMSize);

        Log(QString("Connected"));

        ui->btnConnect->setDisabled(true);
        ui->btnLocate->setEnabled(true);
    }
    else {
        Log(QString("Device not found"));
    }
}

void MainWindow::on_btnLocate_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, QString("Open firmware update"), QString(""), QString("Firmware Update Files (*.hex);;All files (*.*)"));

    if (filename != "") {
        FILE *fp = fopen(filename.toLocal8Bit().constData(), "r");
        record *records = IntelHexFile::Open(fp);
        flashdata = IntelHexFile::RecordsToFlashData(records);

        IntelHexFile::FreeRecords(records);
        QString filesize;
        filesize.setNum(flashdata->size);

        Log(QString("Ready to upload ") + filesize + QString(" bytes of data"));

        fclose(fp);

        ui->btnUpload->setEnabled(true);
    }
}

void MainWindow::on_btnUpload_clicked()
{
    if (flashdata != NULL) {
        unsigned char *data = flashdata->data;
        int length = flashdata->size;

        for (unsigned short address = 0; address < length; address += OpenFocus::Bootloader::PageSize) {
            Log(QString("Writing block ") + QString().setNum(address, 16) + QString(" ... ") + QString().setNum(address + OpenFocus::Bootloader::PageSize, 16));
            if (bootloader->WriteFlashBlock(address, data, OpenFocus::Bootloader::PageSize) <= 0)
            {
                Log(QString("An error occurred while writing data"));
                return;
            }
            data += OpenFocus::Bootloader::PageSize;
        }

        bootloader->Reboot();

        ui->btnConnect->setDisabled(true);
        ui->btnLocate->setDisabled(true);
        ui->btnUpload->setDisabled(true);
    }
    else {
        qDebug() << "An error occurred";
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (bootloader->IsConnected()) {
        bootloader->Reboot();
        bootloader->Disconnect();
    }
    event->accept();
}
