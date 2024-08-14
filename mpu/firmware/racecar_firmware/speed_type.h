#ifndef SPEED_TYPE_H
#define SPEED_TYPE_H

#include <math.h>

#include <QString>
#include <QMetaType>
#include <QDataStream>

struct SpeedTypeDef{
    char header[4]={'a','c','s','r'};


    float current_speed=0.0;
    float setpoint=0.0;
    float output=0.0;

    char tailer[4]={'b','4','0','1'};

    bool operator==(const SpeedTypeDef& other) const
    {
        return (abs(current_speed - other.current_speed)<std::numeric_limits<float>::epsilon())
               && (abs(setpoint - other.setpoint)<std::numeric_limits<float>::epsilon())
               && (abs(output - other.output)<std::numeric_limits<float>::epsilon());
    }

    friend QDataStream &operator<<(QDataStream &out, const SpeedTypeDef &rhs){
        out.writeRawData((char*)(&rhs),sizeof(SpeedTypeDef));
        return out;
    }
    friend QDataStream &operator>>(QDataStream &in, SpeedTypeDef &rhs){
        char *t = new char[sizeof(SpeedTypeDef)];
        in.readRawData(t,sizeof(SpeedTypeDef));
        auto p = reinterpret_cast<SpeedTypeDef*>(t);
        memcpy(&rhs,p,sizeof(SpeedTypeDef));
        return in;
    }

} ;

Q_DECLARE_METATYPE(SpeedTypeDef)

#endif // SPEED_TYPE_H
