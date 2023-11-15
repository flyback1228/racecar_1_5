#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSerialPort>
#include "parametermodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showWriteError(const QString &message);
private:
    Ui::MainWindow *ui;

    QSerialPort port_;
    bool is_serial_port_open_ = false;
    ParameterModel* pamameter_model_;
    void btnOpen_clicked();
    void btnClose_clicked();
    void btnRead_clicked();
    void btnWrite_clicked();
    void read_data();
    void update_values(const ParameterTypeDef& parameters);
};
#endif // MAINWINDOW_H
