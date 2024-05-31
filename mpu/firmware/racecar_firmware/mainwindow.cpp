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
    connect(ui->btnReset, &QPushButton::clicked, this, &MainWindow::btnReset_clicked);
    connect(ui->btnLoad,&QPushButton::clicked, this, &MainWindow::btnLoad_clicked);
    connect(ui->btnSave,&QPushButton::clicked, this, &MainWindow::btnSave_clicked);
    connect(ui->btnSpeedStart,&QPushButton::clicked, this, &MainWindow::btnSpeedStart_clicked);
    connect(ui->btnSpeedStop,&QPushButton::clicked, this, &MainWindow::btnSpeedStop_clicked);

    connect(&port_, &QSerialPort::readyRead, this, &MainWindow::read_data);

    connect(timer_, &QTimer::timeout,this, &MainWindow::read_speed_timeout);

    ui->btnWrite->setEnabled(false);
    ui->btnReset->setEnabled(false);
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

    ui->ledEscMax->setValidator( new QDoubleValidator(0, 1, 3, this) );
    ui->ledEscMin->setValidator( new QDoubleValidator(0, 1, 3, this) );
    ui->ledEscOffset->setValidator(new QDoubleValidator(0,1,3,this));
    ui->ledEscPrecision->setValidator(new QIntValidator(0, 100,this));
    ui->ledServoPrecision->setValidator(new QIntValidator(0, 100,this));

    ui->ledSteeringRate->setValidator( new QDoubleValidator(0, 10000, 2, this) );
    ui->ledSteeringMax->setValidator( new QDoubleValidator(0, 90,2,this) );
    ui->ledSteeringMin->setValidator( new QDoubleValidator(-90, 0,2, this) );
    ui->ledSteeringOffset->setValidator(new QDoubleValidator(1,20000,2,this));

    ui->ledForceOffset0->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceOffset1->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceOffset2->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceOffset3->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceOffset4->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceOffset5->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceOffset6->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceOffset7->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );

    ui->ledForceRatio0->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceRatio1->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceRatio2->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceRatio3->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceRatio4->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceRatio5->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceRatio6->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );
    ui->ledForceRatio7->setValidator( new QDoubleValidator(-100000, 100000, 2, this) );

    ui->ledRosFrequency->setValidator( new QDoubleValidator(0, 50, 2, this) );
    ui->ledSpeedDifferenceWarning->setValidator( new QDoubleValidator(0, 10, 2, this) );
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
    connect(ui->ledSpeedDifferenceWarning,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_speed_difference_warning);

    connect(ui->ledEscRate,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_esc_rpm_to_speed_ratio);
    connect(ui->ledEscPrecision,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_esc_precision);
    connect(ui->ledServoPrecision,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_servo_precision);
    connect(ui->ckbEscReverse,&QRadioButton::toggled,pamameter_model_,&ParameterModel::set_esc_reverse);

    connect(ui->ledForceOffset0,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_offset0);
    connect(ui->ledForceOffset1,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_offset1);
    connect(ui->ledForceOffset2,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_offset2);
    connect(ui->ledForceOffset3,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_offset3);
    connect(ui->ledForceOffset4,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_offset4);
    connect(ui->ledForceOffset5,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_offset5);
    connect(ui->ledForceOffset6,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_offset6);
    connect(ui->ledForceOffset7,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_offset7);

    connect(ui->ledForceRatio0,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_ratio0);
    connect(ui->ledForceRatio1,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_ratio1);
    connect(ui->ledForceRatio2,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_ratio2);
    connect(ui->ledForceRatio3,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_ratio3);
    connect(ui->ledForceRatio4,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_ratio4);
    connect(ui->ledForceRatio5,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_ratio5);
    connect(ui->ledForceRatio6,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_ratio6);
    connect(ui->ledForceRatio7,&QLineEdit::textChanged,pamameter_model_,&ParameterModel::set_force_ratio7);

    connect(pamameter_model_,&ParameterModel::parametersChanged,this,&MainWindow::update_values);

    update_values(pamameter_model_->parameters());


    //set the chart
    auto m_chart = ui->chartView->chart();

    series_ = new QLineSeries();
    m_chart->addSeries(series_);

    xAxis_ = new QDateTimeAxis();
    xAxis_->setFormat("mm:ss.z");
    xAxis_->setTitleText("Time");
    xAxis_->setTickCount(10);

    m_chart->addAxis(xAxis_, Qt::AlignBottom);
    series_->attachAxis(xAxis_);

    yAxis_ = new QValueAxis;
    yAxis_->setTitleText("Speed");
    m_chart->addAxis(yAxis_, Qt::AlignLeft);
    series_->attachAxis(yAxis_);


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
    port_.setBaudRate(230400);
    port_.setDataBits(QSerialPort::Data8);
    port_.setParity(QSerialPort::NoParity);
    port_.setStopBits(QSerialPort::OneStop);
    port_.setReadBufferSize(200);
    if (port_.open(QIODevice::ReadWrite)) {
        ui->btnWrite->setEnabled(true);
        ui->btnReset->setEnabled(true);
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
    ui->btnReset->setEnabled(false);
    ui->btnRead->setEnabled(false);
    ui->btnClose->setEnabled(false);
    ui->btnOpen->setEnabled(true);
    ui->cmbPort->setEnabled(true);
    ui->statusbar->showMessage(tr("Select A Serial Port and Open"));
}

void MainWindow::btnRead_clicked()
{
    // to stop the speed
    if(begin_reading_speed_){
        btnSpeedStop_clicked();
    }
    data.clear();
    begin_reading_parameter_=true;
    char header[] = {'x','i','l','i','n'};
    QByteArray byte_array(header,5);
    port_.write(byte_array);

    ui->btnRead->setEnabled(false);
    QTimer::singleShot(1500, this, &MainWindow::read_timeout);
}

void MainWindow::btnWrite_clicked()
{
    auto p = pamameter_model_->parameters();
    QByteArray byte_array((char*)(&p),sizeof(ParameterTypeDef));
    port_.write(byte_array);

}

void MainWindow::btnReset_clicked(){
    char header[] = {'r','e','s','e','t'};
    QByteArray byte_array(header,5);
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

//callback function of button(Start Plot) clicked
//step 1: send the "speed" to mpu
//step 2: start the timer to process the data from mpu

void MainWindow::btnSpeedStart_clicked()
{
    if(begin_reading_speed_){
        return;
    }
    speed_vector.clear();
    begin_reading_speed_=true;
    timer_->start(200);
    char header[] = {'s','p','e','e','d'};
    QByteArray byte_array(header,5);
    port_.write(byte_array);
    xAxis_->setMin(QDateTime::fromMSecsSinceEpoch(QDateTime::currentDateTime().toMSecsSinceEpoch()-1000));
}

//callback function of button(Stop Plot) clicked, send the "speed" to mpu
void MainWindow::btnSpeedStop_clicked()
{
    if(!begin_reading_speed_){
        return;
    }
    timer_->stop();
    begin_reading_speed_=false;
    char header[] = {'s','p','e','e','d'};
    QByteArray byte_array(header,5);
    port_.write(byte_array);
    //xAxis_->setMin(QDateTime::fromMSecsSinceEpoch(QDateTime::currentDateTime().toMSecsSinceEpoch()-1000));
}

void MainWindow::read_data()
{

    QByteArray d = port_.readAll();
    qDebug()<<d<<'\n';
    if(begin_reading_parameter_){
        data.append(d.begin(),d.size());
        if(data.size()>=sizeof(ParameterTypeDef))
            begin_reading_parameter_ = false;
    }else if(begin_reading_speed_){
        speed_data.append(d.begin(),d.size());
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

    ui->btnRead->setEnabled(true);
    qDebug()<<"data:"<<data.size()<<"\tparma:"<<sizeof(ParameterTypeDef)<<'\n';
    qDebug()<<QString(data)<<'\n';
    if(data.size()==sizeof(ParameterTypeDef)){
        auto p = reinterpret_cast<ParameterTypeDef*>(data.data());
        qDebug()<<QString(p->header)<<'\t'<<QString(p->tailer)<<'\n';
        if(QString(p->tailer)=="b401"){
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

//callback for the timeout function of a timer, to process the speed data and to plot
//each set of the data contains 5 bytes, the first three are 's', 'p' and 'd', the 4th byte is the integer part of a float, and the 5th byte is the decimal part multiplied by 100
void MainWindow::read_speed_timeout()
{
    if(!begin_reading_speed_)return;
    if(speed_data.size()<5)return;
    while(speed_data.size()>=5 && speed_data[0]!='s' && speed_data[1]!='p' && speed_data[2]!='d')
        speed_data.remove(0,1);

    float speed=0;
    int i=0;
    while(speed_data.size()>=5){
        speed = speed_data[3]+speed_data[4]/100.0;
        i++;
        speed_data.remove(0,5);
    }
    if(i==0)return;
    speed/=i;
    speed_vector.push_back(speed);
    if(speed_vector.size()==1){
        max_y_=speed;
        min_y_=speed-0.1;
    }else if(speed>max_y_){
        max_y_=speed;
        yAxis_->setMax(max_y_);
    }else if(speed<min_y_){
        min_y_=speed;
        yAxis_->setMin(min_y_);
    }
    auto t = QDateTime::currentDateTime();
    series_->append(t.toMSecsSinceEpoch(), speed);
    xAxis_->setMax(t);

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
    ui->ledSteeringRate->setText(QString::number(parameters.steering_to_dutycycle_ratio));

    ui->ledRosFrequency->setText(QString::number(parameters.publish_frequency));
    ui->ledBrakeFrequency->setText(QString::number(parameters.brake_pwm_frequency));

    ui->ckbEscReverse->setChecked(parameters.allow_reverse);
    ui->ledEscPrecision->setText(QString::number(parameters.esc_set_precision));
    ui->ledServoPrecision->setText(QString::number(parameters.servo_set_precision));
    ui->ledEscRate->setText(QString::number(parameters.esc_rpm_to_speed_ratio));

    ui->ledForceRatio0->setText(QString::number(parameters.force_ratio[0]));
    ui->ledForceRatio1->setText(QString::number(parameters.force_ratio[1]));
    ui->ledForceRatio2->setText(QString::number(parameters.force_ratio[2]));
    ui->ledForceRatio3->setText(QString::number(parameters.force_ratio[3]));
    ui->ledForceRatio4->setText(QString::number(parameters.force_ratio[4]));
    ui->ledForceRatio5->setText(QString::number(parameters.force_ratio[5]));
    ui->ledForceRatio6->setText(QString::number(parameters.force_ratio[6]));
    ui->ledForceRatio7->setText(QString::number(parameters.force_ratio[7]));

    ui->ledForceOffset0->setText(QString::number(parameters.force_offset[0]));
    ui->ledForceOffset1->setText(QString::number(parameters.force_offset[1]));
    ui->ledForceOffset2->setText(QString::number(parameters.force_offset[2]));
    ui->ledForceOffset3->setText(QString::number(parameters.force_offset[3]));
    ui->ledForceOffset4->setText(QString::number(parameters.force_offset[4]));
    ui->ledForceOffset5->setText(QString::number(parameters.force_offset[5]));
    ui->ledForceOffset6->setText(QString::number(parameters.force_offset[6]));
    ui->ledForceOffset7->setText(QString::number(parameters.force_offset[7]));

    ui->ledSpeedDifferenceWarning->setText(QString::number(parameters.wheel_speed_difference_warning));
}

