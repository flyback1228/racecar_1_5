#include "parametermodel.h"

#include <QFile>

ParameterModel::ParameterModel(QObject *parent)
    : QObject{parent},timer(new QTimer(this)),save_label(false)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ParameterModel::save);
    timer->start(5000);

    if(QFile::exists("default.rcc")){
        QFile file("default.rcc");
        if(!file.open(QIODevice::ReadOnly))
            return;
        QDataStream load(&file);
        load >> parameters_;
        emit parametersChanged(parameters_);
    }
}

void ParameterModel::set_kp(const QString &text)
{
    parameters_.kp = string_to_float(text);
}

void ParameterModel::set_ki(const QString &text)
{
    parameters_.ki = string_to_float(text);
}

void ParameterModel::set_kd(const QString &text)
{
    parameters_.kd = string_to_float(text);
}

void ParameterModel::set_publish_frequency(const QString &text)
{
    parameters_.publish_frequency = string_to_uint8(text);
}

void ParameterModel::set_esc_rpm_to_speed_ratio(const QString &text)
{
    parameters_.esc_rpm_to_speed_ratio = string_to_float(text);
}

void ParameterModel::set_pid_frequency(const QString &text)
{
    parameters_.pid_frequency = string_to_uint8(text);
}

void ParameterModel::set_steering_esc_pwm_frequency(const QString &text)
{
    parameters_.steering_esc_pwm_frequency = string_to_float(text);
}

void ParameterModel::set_steering_offset(const QString &text)
{
    parameters_.steering_offset = string_to_float(text);
}

void ParameterModel::set_steering_ratio(const QString &text)
{
   parameters_.steering_to_dutycycle_ratio = string_to_float(text);
}

void ParameterModel::set_steering_max(const QString &text)
{
    parameters_.steering_max = string_to_float(text);
}

void ParameterModel::set_steering_min(const QString &text)
{
    parameters_.steering_min = string_to_float(text);
}

void ParameterModel::set_force_offset0(const QString &text)
{
    parameters_.force_offset[0] = string_to_float(text);
}
void ParameterModel::set_force_offset1(const QString &text)
{
    parameters_.force_offset[1] = string_to_float(text);
}
void ParameterModel::set_force_offset2(const QString &text)
{
    parameters_.force_offset[2] = string_to_float(text);
}
void ParameterModel::set_force_offset3(const QString &text)
{
    parameters_.force_offset[3] = string_to_float(text);
}
void ParameterModel::set_force_offset4(const QString &text)
{
    parameters_.force_offset[4] = string_to_float(text);
}
void ParameterModel::set_force_offset5(const QString &text)
{
    parameters_.force_offset[5] = string_to_float(text);
}
void ParameterModel::set_force_offset6(const QString &text)
{
    parameters_.force_offset[6] = string_to_float(text);
}
void ParameterModel::set_force_offset7(const QString &text)
{
    parameters_.force_offset[7] = string_to_float(text);
}

void ParameterModel::set_force_ratio0(const QString &text)
{
    parameters_.force_ratio[0] = string_to_float(text);
}
void ParameterModel::set_force_ratio1(const QString &text)
{
    parameters_.force_ratio[1] = string_to_float(text);
}
void ParameterModel::set_force_ratio2(const QString &text)
{
    parameters_.force_ratio[2] = string_to_float(text);
}
void ParameterModel::set_force_ratio3(const QString &text)
{
    parameters_.force_ratio[3] = string_to_float(text);
}
void ParameterModel::set_force_ratio4(const QString &text)
{
    parameters_.force_ratio[4] = string_to_float(text);
}
void ParameterModel::set_force_ratio5(const QString &text)
{
    parameters_.force_ratio[5] = string_to_float(text);
}
void ParameterModel::set_force_ratio6(const QString &text)
{
    parameters_.force_ratio[6] = string_to_float(text);
}
void ParameterModel::set_force_ratio7(const QString &text)
{
    parameters_.force_ratio[7] = string_to_float(text);
}

void ParameterModel::set_esc_offset(const QString &text)
{
    parameters_.esc_offset = string_to_float(text);
}

void ParameterModel::set_esc_max(const QString &text)
{
    parameters_.esc_max = string_to_float(text);
}

void ParameterModel::set_esc_min(const QString &text)
{
    parameters_.esc_min = string_to_float(text);
}

void ParameterModel::set_esc_reverse(bool value)
{
    parameters_.allow_reverse=value;
}

void ParameterModel::set_esc_precision(const QString &text)
{
    parameters_.esc_set_precision = string_to_uint8(text);
}

void ParameterModel::set_servo_precision(const QString &text)
{
    parameters_.servo_set_precision = string_to_uint8(text);
}

void ParameterModel::set_brake_pwm_frequency(const QString &text)
{
    parameters_.brake_pwm_frequency = string_to_float(text);
}

void ParameterModel::set_speed_difference_warning(const QString &text)
{
    parameters_.wheel_speed_difference_warning = string_to_float(text);
}

void ParameterModel::save()
{
    if(save_label){
        //auto filter = tr("Config Files (*.rcc);;files (*)");
        //auto fileName = QFileDialog::getSaveFileName(this,
        //                                             tr("Save Config File"), file_path_, filter, &filter );
        //file_path_ = QFileInfo(fileName).path();

        QFile file("default.rcc");
        if(!file.open(QIODevice::WriteOnly))
            return;
        QDataStream save(&file);
        save << parameters_;
        file.close();
        save_label = false;
    }
}

void ParameterModel::set_upload_speed(int state){
    parameters_.upload_speed=state;
}

float ParameterModel::string_to_float(const QString &text)
{
    bool ok;
    auto v = text.toFloat(&ok);
    if(ok){
        save_label = true;
        return v;
    }
    return 0.0f;
}

uint8_t ParameterModel::string_to_uint8(const QString &text)
{
    bool ok;
    auto v = text.toUInt(&ok);
    if(ok){
        save_label = true;
        return (uint8_t)v;
    }
    else return 0;
}

//void ParameterModel::setParameters(ParameterTypeDef parameters)
//{
//    this->parameters_ = parameters;
//    emit valueChanged();
//}
