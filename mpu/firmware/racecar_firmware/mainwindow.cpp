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
    ui->ledPIDFrequency->setValidator(new QIntValidator(1,100,this));


    ui->ledEscFrequency->setValidator( new QDoubleValidator(0, 100, 2, this) );
    ui->ledEscMax->setValidator( new QIntValidator(0, 100,this) );
    ui->ledEscMin->setValidator( new QIntValidator(0, 100, this) );
    ui->ledEscOffset->setValidator(new QIntValidator(1,20000));

    ui->ledSteeringRate->setValidator( new QDoubleValidator(0, 10000, 2, this) );
    ui->ledSteeringMax->setValidator( new QIntValidator(0, 90,this) );
    ui->ledSteeringMin->setValidator( new QIntValidator(-90, 0, this) );
    ui->ledSteeringOffset->setValidator(new QIntValidator(1,20000));

    ui->ledRosFrequency->setValidator( new QDoubleValidator(0, 50, 2, this) );
    ui->ledBrakeFrequency->setValidator( new QDoubleValidator(50, 2000, 2, this) );

    pamameter_model_ = new ParameterModel(this);
    connect(pamameter_model_,&ParameterModel::parametersChanged,this,&MainWindow::update_values);
}

MainWindow::~MainWindow()
{
    delete pamameter_model_;
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
    ui->ledVersion->setText(tr("%d.%d").arg(parameters.version).arg(parameters.subversion));

    ui->ledKd->setText(QString::number(parameters.kd,'g',2));
    ui->ledKi->setText(QString::number(parameters.ki,'g',2));
    ui->ledKp->setText(QString::number(parameters.kp,'g',2));
    ui->ledPIDFrequency->setText(QString::number(parameters.pid_frequency));

    ui->ledEscFrequency->setText(QString::number(parameters.steering_esc_pwm_frequency,'g',2));
    ui->ledEscMax->setText(QString::number(parameters.esc_max));
    ui->ledEscMin->setText(QString::number(parameters.esc_min));
    ui->ledEscOffset->setText(QString::number(parameters.esc_offset));
    ui->ledSteeringOffset->setText(QString::number(parameters.steering_offset));
    ui->ledSteeringMax->setText(QString::number(parameters.steering_max,'g',2));
    ui->ledSteeringMin->setText(QString::number(parameters.steering_min,'g',2));
    ui->ledSteeringRate->setText(QString::number(parameters.steering_ratio,'g',2));

    ui->ledRosFrequency->setText(QString::number(parameters.publish_frequency));
    ui->ledBrakeFrequency->setText(QString::number(parameters.brake_pwm_frequency,'g',2));

}

