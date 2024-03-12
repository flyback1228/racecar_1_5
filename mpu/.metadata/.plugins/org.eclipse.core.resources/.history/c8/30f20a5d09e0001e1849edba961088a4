#ifndef _ROS_stm32_sensor_msgs_Stm32SensorStamped_h
#define _ROS_stm32_sensor_msgs_Stm32SensorStamped_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace stm32_sensor_msgs
{

  class Stm32SensorStamped : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      float sensor[38];
      typedef uint8_t _eccor_type;
      _eccor_type eccor;

    Stm32SensorStamped():
      header(),
      sensor(),
      eccor(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      for( uint32_t i = 0; i < 38; i++){
      union {
        float real;
        uint32_t base;
      } u_sensori;
      u_sensori.real = this->sensor[i];
      *(outbuffer + offset + 0) = (u_sensori.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sensori.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_sensori.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_sensori.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->sensor[i]);
      }
      *(outbuffer + offset + 0) = (this->eccor >> (8 * 0)) & 0xFF;
      offset += sizeof(this->eccor);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      for( uint32_t i = 0; i < 38; i++){
      union {
        float real;
        uint32_t base;
      } u_sensori;
      u_sensori.base = 0;
      u_sensori.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sensori.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_sensori.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_sensori.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->sensor[i] = u_sensori.real;
      offset += sizeof(this->sensor[i]);
      }
      this->eccor =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->eccor);
     return offset;
    }

    virtual const char * getType() override { return "stm32_sensor_msgs/Stm32SensorStamped"; };
    virtual const char * getMD5() override { return "c92f7ff83f3acebb9c8c4224d69a466d"; };

  };

}
#endif
