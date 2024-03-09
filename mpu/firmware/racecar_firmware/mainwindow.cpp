#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),timer_(new QTimer(this))
{
    ui->setupUi(this);

    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        ui->cmbPort->addItem(port.portName());
    }

    connect(ui->btnOpen, &QPushButton::clicked, this, &MainWindow::btnOpen_clicked);
    connect(ui->btnClose, &QPushButton::clicked, this, &MainWindow::btnClose_clicked);
    connect(ui->btnRead, &QPushButton::clicked, this, &MainWindow::btnRead_clicked);
    connect(ui->btnWrite, &QPushButton::clicked, this, &MainWindow::btnWrite_clicked);
    connect(ui->btnLoad,&QPushButton::clicked, this, &MainWindow::btnLoad_clicked);
    connect(ui->btnSave,&QPushButton::clicked, this, &MainWindow::btnSave_clicked);

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
    ui->ledEscOffset->setValidator(new QIntValidator(1,20000,this));

    ui->ledSteeringRate->setValidator( new QDoubleValidator(0, 10000, 2, this) );
    ui->ledSteeringMax->setValidator( new QIntValidator(0, 90,this) );
    ui->ledSteeringMin->setValidator( new QIntValidator(-90, 0, this) );
    ui->ledSteeringOffset->setValidator(new QIntValidator(1,20000,this));

    ui->ledRosFrequency->setValidator( new QDoubleValidator(0, 50, 2, this) );
    ui->ledBrakeFrequency->setValidator( new QDoubleValidator(50, 2000, 2, this) );

    pamameter_model_ = new ParameterModel(this);

    connect(ui->ledKp,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_kp);
    connect(ui->ledKi,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_ki);
    connect(ui->ledKd,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_kd);
    connect(ui->ledPIDFrequency,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_pid_frequency);

    connect(ui->ledEscFrequency,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_steering_esc_pwm_frequency);
    connect(ui->ledEscMax,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_esc_max);
    connect(ui->ledEscMin,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_esc_min);
    connect(ui->ledEscOffset,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_esc_offset);

    connect(ui->ledSteeringOffset,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_steering_offset);
    connect(ui->ledSteeringMax,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_steering_max);
    connect(ui->ledSteeringMin,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_steering_min);
    connect(ui->ledSteeringOffset,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_steering_offset);

    connect(ui->ledRosFrequency,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_publish_frequency);
    connect(ui->ledBrakeFrequency,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_brake_pwm_frequency);

    connect(pamameter_model_,&ParameterModel::parametersChanged,this,&MainWindow::update_values);

    update_values(pamameter_model_->parameters());


    file_path_ = "/";
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
    data.clear();
    begin_reading_parameter_=true;
    char header[] = {'x','i','l','i','n'};
    QByteArray byte_array(header,5);
    port_.write(byte_array);

    ui->btnRead->setEnabled(false);
    QTimer::singleShot(500, this, &MainWindow::read_timeout);
}

void MainWindow::btnWrite_clicked()
{
    auto p = pamameter_model_->parameters();
    QByteArray byte_array((char*)(&p),sizeof(ParameterTypeDef));
    port_.write(byte_array);

}

void MainWindow::btnLoad_clicked()
{
    auto filter = tr("Config Files (*.rcc);;files (*)");
    auto fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Config File"), file_path_, filter);
    file_path_ = QFileInfo(fileName).path();
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QDataStream load(&file);
    ParameterTypeDef para;
    load >> para;
    pamameter_model_->setParameters(para);
}

void MainWindow::btnSave_clicked()
{
    auto filter = tr("Config Files (*.rcc);;files (*)");
    auto fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Save Config File"), file_path_, filter, &filter );
    file_path_ = QFileInfo(fileName).path();

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
        return;
    QDataStream save(&file);
    save << pamameter_model_->parameters();
    file.close();
}

void MainWindow::read_data()
{
    QByteArray d = port_.readAll();
    if(begin_reading_parameter_){
        data.append(d.begin(),d.size());
    }else{
        show_message(QString(d));
    }

}

void MainWindow::show_message(QString msg){
    ui->pteMessage->moveCursor (QTextCursor::End);
    ui->pteMessage->insertPlainText (msg);
    ui->pteMessage->moveCursor (QTextCursor::End);
}

void MainWindow::read_timeout()
{
    begin_reading_parameter_ = false;
    ui->btnRead->setEnabled(true);
    if(data.size()==sizeof(ParameterTypeDef)){
        auto p = reinterpret_cast<ParameterTypeDef*>(data.data());
        if(QString(p->header)=="acsr" && QString(p->tailer)=="b401"){
            pamameter_model_->setParameters(*p);
            qDebug()<<data<<'\n';
            show_message("Read Data From MPU Complete!\n");
        }else{
            qDebug()<<data<<'\n';
            show_message("Read Data From MPU Fail!\n");
        }
    }else{
        show_message("Read Data From MPU Fail!\n");
    }

}

void MainWindow::update_values(const ParameterTypeDef& parameters)
{
    ui->ledVersion->setText(tr("%1.%2").arg(parameters.version).arg(parameters.subversion));

    ui->ledKd->setText(QString::number(parameters.kd));
    ui->ledKi->setText(QString::number(parameters.ki));
    ui->ledKp->setText(QString::number(parameters.kp));
    ui->ledPIDFrequency->setText(QString::number(parameters.pid_frequency));

    ui->ledEscFrequency->setText(QString::number(parameters.steering_esc_pwm_frequency));
    ui->ledEscMax->setText(QString::number(parameters.esc_max));
    ui->ledEscMin->setText(QString::number(parameters.esc_min));
    ui->ledEscOffset->setText(QString::number(parameters.esc_offset));
    ui->ledSteeringOffset->setText(QString::number(parameters.steering_offset));
    ui->ledSteeringMax->setText(QString::number(parameters.steering_max));
    ui->ledSteeringMin->setText(QString::number(parameters.steering_min));
    ui->ledSteeringRate->setText(QString::number(parameters.steering_ratio));

    ui->ledRosFrequency->setText(QString::number(parameters.publish_frequency));
    ui->ledBrakeFrequency->setText(QString::number(parameters.brake_pwm_frequency));

}

