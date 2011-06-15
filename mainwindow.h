#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "openfocus.h"
#include "intelhex.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Log(QString text);

private slots:
    void on_btnConnect_clicked();

    void on_btnLocate_clicked();

    void on_btnUpload_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    OpenFocus::Bootloader *bootloader;
    flash *flashdata;
};

#endif // MAINWINDOW_H
