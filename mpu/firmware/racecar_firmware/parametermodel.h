#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <QObject>
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
    void set_publish_frequency(const QString &text);
    void set_pid_frequency(const QString &text);
    void set_steering_esc_pwm_frequency(const QString &text);
    void set_steering_offset(const QString &text);
    void set_steering_ratio(const QString &text);
    void set_steering_max(const QString &text);
    void set_steering_min(const QString &text);
    void set_esc_offset(const QString &text);
    void set_esc_max(const QString &text);
    void set_esc_min(const QString &text);
    void set_brake_pwm_frequency(const QString &text);




private:
    ParameterTypeDef parameters_;
signals:
    void parametersChanged(const ParameterTypeDef& parameters);


};

#endif // PARAMETERMODEL_H
