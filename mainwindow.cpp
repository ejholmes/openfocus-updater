#include <QtGui>
#include <stdio.h>

#ifdef __WIN32__
#include <windows.h>
#include <time.h>
#endif

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
    connect = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Log(QString text)
{
    ui->listLog->addItem(text);
    ui->listLog->scrollToBottom();
}

void MainWindow::on_btnConnect_clicked()
{
    qDebug("Connecting to device");
#ifdef __WIN32__
    connect = true;
    OpenFocus::Helper::ConnectBootloader(false);

    time_t t = time(NULL) + 2;

    while (connect) {
        if (time(NULL) >= t)
            goto notconnected; /* Timed out */
        QApplication::processEvents();
    }
    goto connected;
#else
    if (OpenFocus::Helper::ConnectBootloader())
        goto connected;
    else
        goto notconnected;
#endif
connected:
    {
        QString PageSize, FlashSize, EEPROMSize;
        qDebug() << "Page Size:" << PageSize.setNum(bootloader->PageSize);
        qDebug() << "Flash Size:" << FlashSize.setNum(bootloader->FlashSize);
        qDebug() << "EEPROM Size:" << EEPROMSize.setNum(bootloader->EEPROMSize);

        Log(QString("Connected"));

        ui->btnConnect->setDisabled(true);
        ui->btnLocate->setEnabled(true);
        return;
    }
notconnected:
    Log(QString("Device not found"));
    return;
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

        for (unsigned short address = 0; address < length; address += bootloader->PageSize) {
            Log(QString("Writing block ") + QString().setNum(address, 16) + QString(" ... ") + QString().setNum(address + bootloader->PageSize, 16));
            if (bootloader->WriteFlashBlock(address, data, bootloader->PageSize) <= 0)
            {
                Log(QString("An error occurred while writing data"));
                return;
            }
            data += bootloader->PageSize;
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

#ifdef __WIN32__
bool MainWindow::winEvent(MSG *message, long *result)
{
    const unsigned int DBT_DEVNODES_CHANGED = 0x0007;
    if (message->message == WM_DEVICECHANGE && message->wParam == DBT_DEVNODES_CHANGED) {
        if (bootloader->IsConnected())
            connect = false;

        if (connect && bootloader->Connect())
            connect = false;
    }

    return false;
}
#endif
