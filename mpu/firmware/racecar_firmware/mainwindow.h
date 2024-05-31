#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSerialPort>
#include <QTimer>
#include <QtCharts>
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
    QTimer* timer_;

    QByteArray data;
    QByteArray speed_data;

    QSerialPort port_;
    bool is_serial_port_open_ = false;
    ParameterModel* pamameter_model_;
    void btnOpen_clicked();
    void btnClose_clicked();
    void btnRead_clicked();
    void btnWrite_clicked();
    void btnLoad_clicked();
    void btnSave_clicked();
    void btnReset_clicked();
    void btnSpeedStart_clicked();
    void btnSpeedStop_clicked();
    void read_data();
    void update_values(const ParameterTypeDef& parameters);
    void show_message(QString msg);
    void read_timeout();
    void read_speed_timeout();
    bool begin_reading_parameter_ = false;
    bool begin_reading_speed_ = false;
    QString file_path_;

    QLineSeries* series_;
    QDateTimeAxis* xAxis_;
    QValueAxis* yAxis_;
    float max_y_=0.0f, min_y_=0.0f;
    QVector<float> speed_vector;

};
#endif // MAINWINDOW_H
