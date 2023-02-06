#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _deviceManager = new DeviceManager(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startScanButton_clicked()
{
    _deviceManager->startScan();
}


void MainWindow::on_stopScanButton_clicked()
{
    _deviceManager->stopScan();
}

