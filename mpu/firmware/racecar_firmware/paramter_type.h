#ifndef PARAMTER_TYPE_H
#define PARAMTER_TYPE_H

#include <cstdint>
#include <math.h>
struct ParameterTypeDef{
    char header[4];
    float kp;
    float ki;
    float kd;
    uint8_t publish_frequency;
    uint8_t pid_frequency;

    float steering_esc_pwm_frequency;
    uint16_t steering_offset;
    float steering_ratio;
    float steering_max;
    float steering_min;

    uint16_t esc_offset;
    uint16_t esc_max;
    uint16_t esc_min;

    float brake_pwm_frequency;

    char tailer[4];

    bool operator==(const ParameterTypeDef& other) const
    {
        return (abs(kp - other.kp)<0.01) && (abs(ki - other.ki)<0.01)  && (abs(kd - other.kd)<0.01)
            && publish_frequency == other.publish_frequency && pid_frequency==other.pid_frequency
            && abs(steering_esc_pwm_frequency-other.steering_esc_pwm_frequency)<0.01 && steering_offset== other.steering_offset
            && abs(steering_ratio-other.steering_ratio)<0.01 && abs(steering_max-other.steering_max)<0.01 && abs(steering_min-other.steering_min)<0.01
            && esc_offset==other.esc_offset && esc_max==other.esc_max && esc_min==other.esc_min
            && abs(brake_pwm_frequency-other.brake_pwm_frequency)<0.01;
    }

} ;

#endif // PARAMTER_TYPE_H
