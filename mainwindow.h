#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "OpenFocus.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    OpenFocus::Device *device;
    OpenFocus::Bootloader *bootloader;
};

#endif // MAINWINDOW_H
