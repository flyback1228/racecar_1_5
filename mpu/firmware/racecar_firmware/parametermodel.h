#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <QObject>
#include <qtimer.h>
#include "paramter_type.h"

class ParameterModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ParameterTypeDef parameters READ parameters WRITE setParameters NOTIFY parametersChanged)
public:
    explicit ParameterModel(QObject *parent = nullptr);

    ParameterTypeDef parameters() const{
        return parameters_;
    }

    void setParameters(const ParameterTypeDef& parameters){
        if(!(parameters_ == parameters)){
            this->parameters_=parameters;
            emit parametersChanged(parameters);
        }
    }

    virtual ~ParameterModel() {};

public slots:
    void set_kp(const QString &text);
    void set_ki(const QString &text);
    void set_kd(const QString &text);
    void set_pid_frequency(const QString &text);

    void set_publish_frequency(const QString &text);

    void set_esc_rpm_to_speed_ratio(const QString &text);
    void set_esc_offset(const QString &text);
    void set_esc_max(const QString &text);
    void set_esc_min(const QString &text);

    void set_esc_reverse(bool value);
    void set_esc_precision(const QString &text);
    void set_servo_precision(const QString &text);

    void set_steering_esc_pwm_frequency(const QString &text);
    void set_steering_offset(const QString &text);
    void set_steering_ratio(const QString &text);
    void set_steering_max(const QString &text);
    void set_steering_min(const QString &text);

    void set_force_offset0(const QString &text);
    void set_force_offset1(const QString &text);
    void set_force_offset2(const QString &text);
    void set_force_offset3(const QString &text);
    void set_force_offset4(const QString &text);
    void set_force_offset5(const QString &text);
    void set_force_offset6(const QString &text);
    void set_force_offset7(const QString &text);

    void set_force_ratio0(const QString &text);
    void set_force_ratio1(const QString &text);
    void set_force_ratio2(const QString &text);
    void set_force_ratio3(const QString &text);
    void set_force_ratio4(const QString &text);
    void set_force_ratio5(const QString &text);
    void set_force_ratio6(const QString &text);
    void set_force_ratio7(const QString &text);

    void set_brake_pwm_frequency(const QString &text);
    void set_speed_difference_warning(const QString &text);

    void save();


private:
    ParameterTypeDef parameters_;
    float string_to_float(const QString &text);
    uint8_t string_to_uint8(const QString &text);

    QTimer *timer;
    bool save_label;

signals:
    void parametersChanged(const ParameterTypeDef& parameters);


};

#endif // PARAMETERMODEL_H
