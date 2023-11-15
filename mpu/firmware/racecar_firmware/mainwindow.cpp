#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        ui->cmbPort->addItem(port.portName());
    }

    connect(ui->btnOpen, &QPushButton::clicked, this, &MainWindow::btnOpen_clicked);
    connect(ui->btnClose, &QPushButton::clicked, this, &MainWindow::btnClose_clicked);
    connect(ui->btnRead, &QPushButton::clicked, this, &MainWindow::btnRead_clicked);
    connect(ui->btnWrite, &QPushButton::clicked, this, &MainWindow::btnWrite_clicked);
    connect(&port_, &QSerialPort::readyRead, this, &MainWindow::read_data);

    ui->btnWrite->setEnabled(false);
    ui->btnRead->setEnabled(false);
    ui->btnClose->setEnabled(false);
    ui->btnOpen->setEnabled(true);
    ui->cmbPort->setEnabled(true);
    ui->statusbar->showMessage(tr("Select A Serial Port and Open"));

    ui->ledKp->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledKi->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledKd->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledPIDFrequency->setValidator(new QIntValidator(1,100));

    pamameter_model_ = new ParameterModel(this);
    connect(pamameter_model_,&ParameterModel::parametersChanged,this,&MainWindow::update_values);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnOpen_clicked()
{

    if(port_.isOpen()){
        return;
    }
    port_.setPortName(ui->cmbPort->currentText());
    port_.setBaudRate(115200);
    port_.setDataBits(QSerialPort::Data8);
    port_.setParity(QSerialPort::NoParity);
    port_.setStopBits(QSerialPort::OneStop);
    if (port_.open(QIODevice::ReadWrite)) {
        ui->btnWrite->setEnabled(true);
        ui->btnRead->setEnabled(true);
        ui->btnClose->setEnabled(true);
        ui->btnOpen->setEnabled(false);
        ui->cmbPort->setEnabled(false);


//        QSerialPortInfo port_info(port_);
        ui->statusbar->showMessage(tr("Connected to %1")
                              .arg(ui->cmbPort->currentText()));

    } else {
        QMessageBox::critical(this, tr("Error"), port_.errorString());
        ui->statusbar->showMessage(tr("Open error"));
    }
}

void MainWindow::btnClose_clicked()
{
    if(!port_.isOpen())return;
    port_.close();
    ui->btnWrite->setEnabled(false);
    ui->btnRead->setEnabled(false);
    ui->btnClose->setEnabled(false);
    ui->btnOpen->setEnabled(true);
    ui->cmbPort->setEnabled(true);
    ui->statusbar->showMessage(tr("Select A Serial Port and Open"));
}

void MainWindow::btnRead_clicked()
{
    char header[] = {'x','i','l','i','n'};
    QByteArray byte_array(header,5);
    port_.write(byte_array);
}

void MainWindow::btnWrite_clicked()
{
    auto p = pamameter_model_->parameters();
    QByteArray byte_array((char*)(&p),sizeof(ParameterTypeDef));
    port_.write(byte_array);


}

void MainWindow::read_data()
{

    QByteArray data = port_.readAll();
    auto p = reinterpret_cast<ParameterTypeDef*>(data.data());
    pamameter_model_->setParameters(*p);
}

void MainWindow::update_values(const ParameterTypeDef& parameters)
{
    ui->ledKd->setText(QString::number(parameters.kd,'g',2));
    ui->ledKi->setText(QString::number(parameters.ki,'g',2));
    ui->ledKp->setText(QString::number(parameters.kp,'g',2));


}

