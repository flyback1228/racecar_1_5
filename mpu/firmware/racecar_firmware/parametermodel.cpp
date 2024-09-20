#include "parametermodel.h"

#include <QFile>
#include <QDir>

ParameterModel::ParameterModel(QObject *parent)
    : QObject{parent}//,timer(new QTimer(this)),save_label(false)
{
    //timer = new QTimer(this);
    //connect(timer, &QTimer::timeout, this, &ParameterModel::save);
    //timer->start(5000);
    if(!QDir(QDir::tempPath()+"/racecar").exists()){
        QDir().mkdir(QDir::tempPath()+"/racecar");
    }

    default_file_name = QDir::tempPath()+"/racecar/default.rcc";

/*
#ifdef __linux__
    default_file_name = "./racecar/default.rcc";
#elif _WIN32
    default_file_name = "./racecar/default.rcc";
#endif
*/


}

void ParameterModel::read_default_file(){
    if(QFile::exists(default_file_name)){
        QFile file(default_file_name);
        if(!file.open(QIODevice::ReadOnly))
            return;
        QDataStream load(&file);
        load >> parameters_;
        emit parametersChanged(parameters_);
    }
}

void ParameterModel::set_kp(const QString &text)
{
    auto kp = string_to_float(text);
    if(abs(kp-parameters_.kp)>std::numeric_limits<float>::epsilon()){
        parameters_.kp = kp;
        save();
    }
}

void ParameterModel::set_ki(const QString &text)
{
    auto ki=string_to_float(text);
    if(abs(parameters_.ki-ki)>  std::numeric_limits<float>::epsilon()){
        parameters_.ki=ki;
        save();
    }
}

void ParameterModel::set_kd(const QString &text)
{
    auto kd = string_to_float(text);
    if(abs(parameters_.kd -kd)>std::numeric_limits<float>::epsilon()){
        parameters_.kd=kd;
        save();
    }
}

void ParameterModel::set_publish_frequency(const QString &text)
{
    auto f = string_to_uint8(text);
    if(parameters_.publish_frequency != f){
        parameters_.publish_frequency = f;
        save();
    }
}

void ParameterModel::set_esc_rpm_to_speed_ratio(const QString &text)
{
    auto r = string_to_float(text);
    if(abs(parameters_.esc_rpm_to_speed_ratio - r)>std::numeric_limits<float>::epsilon()){
        parameters_.esc_rpm_to_speed_ratio=r;
        save();
    }
}

void ParameterModel::set_pid_frequency(const QString &text)
{
    auto f = string_to_uint8(text);
    if(parameters_.pid_frequency != f){
        parameters_.pid_frequency = f;
        save();
    }
}

void ParameterModel::set_steering_esc_pwm_frequency(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.steering_esc_pwm_frequency - f)>std::numeric_limits<float>::epsilon()){
        parameters_.steering_esc_pwm_frequency=f;
        save();
    }
}

void ParameterModel::set_steering_offset(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.steering_offset - f)>std::numeric_limits<float>::epsilon()){
        parameters_.steering_offset=f;
        save();
    }
}

void ParameterModel::set_steering_ratio(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.steering_to_dutycycle_ratio - f)>std::numeric_limits<float>::epsilon()){
        parameters_.steering_to_dutycycle_ratio=f;
        save();
    }
}

void ParameterModel::set_steering_max(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.steering_max - f)>std::numeric_limits<float>::epsilon()){
        parameters_.steering_max=f;
        save();
    }
}

void ParameterModel::set_steering_min(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.steering_min - f)>std::numeric_limits<float>::epsilon()){
        parameters_.steering_min=f;
        save();
    }
}

void ParameterModel::set_force_offset0(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_offset[0] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_offset[0]=f;
        save();
    }
}
void ParameterModel::set_force_offset1(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_offset[1] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_offset[1]=f;
        save();
    }
}
void ParameterModel::set_force_offset2(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_offset[2] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_offset[2]=f;
        save();
    }
}
void ParameterModel::set_force_offset3(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_offset[3] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_offset[3]=f;
        save();
    }
}
void ParameterModel::set_force_offset4(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_offset[4] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_offset[4]=f;
        save();
    }
}
void ParameterModel::set_force_offset5(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_offset[5] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_offset[5]=f;
        save();
    }
}
void ParameterModel::set_force_offset6(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_offset[6] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_offset[6]=f;
        save();
    }
}
void ParameterModel::set_force_offset7(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_offset[7] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_offset[7]=f;
        save();
    }
}

void ParameterModel::set_force_ratio0(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_ratio[0] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_ratio[0]=f;
        save();
    }
}
void ParameterModel::set_force_ratio1(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_ratio[1] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_ratio[1]=f;
        save();
    }
}
void ParameterModel::set_force_ratio2(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_ratio[2] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_ratio[2]=f;
        save();
    }
}
void ParameterModel::set_force_ratio3(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_ratio[3] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_ratio[3]=f;
        save();
    }
}
void ParameterModel::set_force_ratio4(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_ratio[4] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_ratio[4]=f;
        save();
    }
}
void ParameterModel::set_force_ratio5(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_ratio[5] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_ratio[5]=f;
        save();
    }
}
void ParameterModel::set_force_ratio6(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_ratio[6] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_ratio[6]=f;
        save();
    }
}
void ParameterModel::set_force_ratio7(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.force_ratio[7] - f)>std::numeric_limits<float>::epsilon()){
        parameters_.force_ratio[7]=f;
        save();
    }
}

void ParameterModel::set_esc_offset(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.esc_offset - f)>std::numeric_limits<float>::epsilon()){
        parameters_.esc_offset=f;
        save();
    }
}

void ParameterModel::set_esc_max(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.esc_max - f)>std::numeric_limits<float>::epsilon()){
        parameters_.esc_max=f;
        save();
    }
}

void ParameterModel::set_esc_min(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.esc_min - f)>std::numeric_limits<float>::epsilon()){
        parameters_.esc_min=f;
        save();
    }
}

void ParameterModel::set_esc_reverse(bool value)
{
    if(parameters_.allow_reverse!=value){
        parameters_.allow_reverse=value;
        save();
    }
}

void ParameterModel::set_esc_precision(const QString &text)
{
    auto v = string_to_uint8(text);
    if(parameters_.esc_set_precision != v){
        parameters_.esc_set_precision = v;
        save();
    }
}

void ParameterModel::set_servo_precision(const QString &text)
{
    auto v = string_to_uint8(text);
    if(parameters_.servo_set_precision != v){
        parameters_.servo_set_precision = v;
        save();
    }
}

void ParameterModel::set_brake_pwm_frequency(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.brake_pwm_frequency - f)>std::numeric_limits<float>::epsilon()){
        parameters_.brake_pwm_frequency=f;
        save();
    }
}

void ParameterModel::set_speed_difference_warning(const QString &text)
{
    auto f = string_to_float(text);
    if(abs(parameters_.wheel_speed_difference_warning - f)>std::numeric_limits<float>::epsilon()){
        parameters_.wheel_speed_difference_warning=f;
        save();
    }
}

void ParameterModel::set_upload_speed(bool value){
    if(parameters_.upload_speed!=value){
        parameters_.upload_speed=value;
        save();
    }
}

void ParameterModel::save()
{
    QFile file(default_file_name);
    if(!file.open(QIODevice::WriteOnly))
        return;
    QDataStream save(&file);
    save << parameters_;
    file.close();
}



float ParameterModel::string_to_float(const QString &text)
{
    bool ok;
    auto v = text.toFloat(&ok);
    if(ok){        
        return v;
    }
    return 0.0f;
}

uint8_t ParameterModel::string_to_uint8(const QString &text)
{
    bool ok;
    auto v = text.toUInt(&ok);
    if(ok){        
        return (uint8_t)v;
    }
    else return 0;
}

