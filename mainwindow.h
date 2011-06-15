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

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    OpenFocus::Bootloader *bootloader;
    flash *flashdata;
};

#endif // MAINWINDOW_H
