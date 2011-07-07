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
/* If we're on windows, use system events to detect when the device is connected */
#ifdef __WIN32__
    connect = true;
    OpenFocus::Helper::ConnectBootloader(false);

    time_t timeout = time(NULL) + 2;

    while (connect) {
        if (time(NULL) >= timeout)
            goto notconnected; /* Timed out */
        QApplication::processEvents();
        if (bootloader->IsConnected())
            connect = false;
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

    /* Open the file and parse all the data */
    if (filename != "") {
        FILE *fp = fopen(filename.toLocal8Bit().constData(), "r");
        struct record *records = IntelHexFile::Open(fp);
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
            QApplication::processEvents();
            if (bootloader->WriteFlashBlock(address, data, OpenFocus::Bootloader::PageSize) <= 0)
            {
                Log(QString("An error occurred while writing data"));
                return;
            }
            data += OpenFocus::Bootloader::PageSize;
        }
        Log("Rebooting device");
        bootloader->Reboot();
        Log("Firmware update complete");

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
        Log(QString("No data uploaded...rebooting device"));
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
        if (connect && bootloader->Connect())
            connect = false;
    }

    return false;
}
#endif
