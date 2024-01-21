#include "parametermodel.h"

ParameterModel::ParameterModel(QObject *parent)
    : QObject{parent}
{

}

void ParameterModel::set_kp(const QString &text)
{
    bool ok;
    float v = text.toFloat(&ok);
    if(ok)parameters_.kp = v;
}

void ParameterModel::set_ki(const QString &text)
{
    bool ok;
    float v = text.toFloat(&ok);
    if(ok)parameters_.ki = v;
}

void ParameterModel::set_kd(const QString &text)
{
    bool ok;
    float v = text.toFloat(&ok);
    if(ok)parameters_.kd = v;
}

void ParameterModel::set_publish_frequency(const QString &text)
{
    bool ok;
    auto v = text.toUInt(&ok);
    if(ok)parameters_.publish_frequency = (uint8_t)v;
}

void ParameterModel::set_pid_frequency(const QString &text)
{
    bool ok;
    auto v = text.toUInt(&ok);
    if(ok)parameters_.pid_frequency = (uint8_t)v;
}

void ParameterModel::set_steering_esc_pwm_frequency(const QString &text)
{
    bool ok;
    auto v = text.toFloat(&ok);
    if(ok)parameters_.steering_esc_pwm_frequency = v;
}

void ParameterModel::set_steering_offset(const QString &text)
{
    bool ok;
    auto v = text.toUInt(&ok);
    if(ok)parameters_.steering_offset = (uint16_t)v;
}

void ParameterModel::set_steering_ratio(const QString &text)
{
    bool ok;
    auto v = text.toFloat(&ok);
    if(ok)parameters_.steering_ratio = v;
}

void ParameterModel::set_steering_max(const QString &text)
{
    bool ok;
    auto v = text.toFloat(&ok);
    if(ok)parameters_.steering_max = v;
}

void ParameterModel::set_steering_min(const QString &text)
{
    bool ok;
    auto v = text.toFloat(&ok);
    if(ok)parameters_.steering_min = v;
}

void ParameterModel::set_esc_offset(const QString &text)
{
    bool ok;
    auto v = text.toUInt(&ok);
    if(ok)parameters_.esc_offset = (uint16_t)v;
}

void ParameterModel::set_esc_max(const QString &text)
{
    bool ok;
    auto v = text.toUInt(&ok);
    if(ok)parameters_.esc_max = (uint16_t)v;
}

void ParameterModel::set_esc_min(const QString &text)
{
    bool ok;
    auto v = text.toUInt(&ok);
    if(ok)parameters_.esc_min = (uint16_t)v;
}

void ParameterModel::set_brake_pwm_frequency(const QString &text)
{
    bool ok;
    auto v = text.toFloat(&ok);
    if(ok)parameters_.brake_pwm_frequency = v;
}

//void ParameterModel::setParameters(ParameterTypeDef parameters)
//{
//    this->parameters_ = parameters;
//    emit valueChanged();
//}
