#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "arduinocommander.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_startScanButton_clicked();

  void on_stopScanButton_clicked();

private:
  Ui::MainWindow *ui;
  ArduinoCommander _commander;
};
#endif // MAINWINDOW_H
