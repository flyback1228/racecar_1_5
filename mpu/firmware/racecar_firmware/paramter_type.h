#ifndef PARAMTER_TYPE_H
#define PARAMTER_TYPE_H

#include <cstdint>
#include <math.h>

#include <QString>
#include <QMetaType>
#include <QDataStream>

struct ParameterTypeDef{
    char header[4]={'a','c','s','r'};

    uint8_t version=1;
    uint8_t subversion=1;
    uint8_t pid_frequency=10;
    uint8_t publish_frequency=20;

    uint8_t esc_set_precision=5;
    uint8_t allow_reverse=false; //a bool
    uint8_t servo_set_precision=5;
    uint8_t upload_speed=false;

    float kp=1;
    float ki=1;
    float kd=0;


    float esc_rpm_to_speed_ratio=1800;
    float esc_offset=0.097;
    float esc_max=0.109;
    float esc_min=0.081;



    float steering_esc_pwm_frequency=64.5;
    float steering_offset=0.096;
    float steering_to_dutycycle_ratio=653.8;
    float steering_max=17;
    float steering_min=-17;



    //force parameters
    float force_ratio[8]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
    float force_offset[8]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    float brake_pwm_frequency=1000;

    float wheel_speed_difference_warning=1.0f;



    char tailer[4]={'b','4','0','1'};

    bool operator==(const ParameterTypeDef& other) const
    {
        bool v=true;
        for(auto i=0;i<8;++i)
            v&=(abs(force_ratio[i]-other.force_ratio[i])<0.01);
        if(!v)return false;

        return (version==other.version
                && subversion==other.subversion
                && abs(kp - other.kp)<std::numeric_limits<float>::epsilon())
                && (abs(ki - other.ki)<std::numeric_limits<float>::epsilon())
                && (abs(kd - other.kd)<std::numeric_limits<float>::epsilon())
                &&  pid_frequency==other.pid_frequency
                && publish_frequency == other.publish_frequency
                && abs(esc_rpm_to_speed_ratio-other.esc_rpm_to_speed_ratio)<std::numeric_limits<float>::epsilon()
                && esc_offset==other.esc_offset
                && esc_max == other.esc_max
                && esc_min == other.esc_min
                && esc_set_precision==other.esc_set_precision
                && allow_reverse==other.allow_reverse
               && abs(steering_esc_pwm_frequency-other.steering_esc_pwm_frequency)<std::numeric_limits<float>::epsilon()
                && steering_offset== other.steering_offset
                && abs(steering_to_dutycycle_ratio-other.steering_to_dutycycle_ratio)<std::numeric_limits<float>::epsilon()
                && abs(steering_max-other.steering_max)<std::numeric_limits<float>::epsilon()
                && abs(steering_min-other.steering_min)<std::numeric_limits<float>::epsilon()
                && servo_set_precision==other.servo_set_precision
                && abs(brake_pwm_frequency-other.brake_pwm_frequency)<std::numeric_limits<float>::epsilon()
                && abs(wheel_speed_difference_warning-other.wheel_speed_difference_warning)<std::numeric_limits<float>::epsilon()
                && upload_speed==other.upload_speed;
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
