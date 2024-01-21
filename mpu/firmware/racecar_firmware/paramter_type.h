#ifndef PARAMTER_TYPE_H
#define PARAMTER_TYPE_H

#include <cstdint>
#include <math.h>

#include <QString>
#include <QMetaType>

struct ParameterTypeDef{
    char header[4]={'a','c','s','r'};
    uint8_t version=1;
    uint8_t subversion=0;
    float kp=1;
    float ki=1;
    float kd=0;
    uint8_t publish_frequency=20;
    uint8_t pid_frequency=10;

    float steering_esc_pwm_frequency=64.5;
    uint16_t steering_offset=1500;
    float steering_ratio=3000;
    float steering_max=17;
    float steering_min=-17;

    uint16_t esc_offset=1500;
    uint16_t esc_max=80;
    uint16_t esc_min=50;

    float brake_pwm_frequency=1000;

    char tailer[4]={'b','4','0','1'};

    bool operator==(const ParameterTypeDef& other) const
    {
        return (abs(kp - other.kp)<0.01) && (abs(ki - other.ki)<0.01)  && (abs(kd - other.kd)<0.01)
            && publish_frequency == other.publish_frequency && pid_frequency==other.pid_frequency
            && abs(steering_esc_pwm_frequency-other.steering_esc_pwm_frequency)<0.01 && steering_offset== other.steering_offset
            && abs(steering_ratio-other.steering_ratio)<0.01 && abs(steering_max-other.steering_max)<0.01 && abs(steering_min-other.steering_min)<0.01
            && esc_offset==other.esc_offset && esc_max==other.esc_max && esc_min==other.esc_min
            && abs(brake_pwm_frequency-other.brake_pwm_frequency)<0.01;
    }

    friend QDataStream &operator<<(QDataStream &out, const ParameterTypeDef &rhs){
        out.writeRawData((char*)(&rhs),sizeof(ParameterTypeDef));
        return out;
    }
    friend QDataStream &operator>>(QDataStream &in, ParameterTypeDef &rhs){
        char *t = new char[sizeof(ParameterTypeDef)];
        in.readRawData(t,sizeof(ParameterTypeDef));
        auto p = reinterpret_cast<ParameterTypeDef*>(t);
        memcpy(&rhs,p,sizeof(ParameterTypeDef));
        return in;
    }

} ;

Q_DECLARE_METATYPE(ParameterTypeDef)

#endif // PARAMTER_TYPE_H
