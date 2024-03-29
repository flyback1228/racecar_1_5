#ifndef _ROS_SERVICE_GetPhysicsProperties_h
#define _ROS_SERVICE_GetPhysicsProperties_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "gazebo_msgs/ODEPhysics.h"
#include "geometry_msgs/Vector3.h"

namespace gazebo_msgs
{

static const char GETPHYSICSPROPERTIES[] = "gazebo_msgs/GetPhysicsProperties";

  class GetPhysicsPropertiesRequest : public ros::Msg
  {
    public:

    GetPhysicsPropertiesRequest()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
     return offset;
    }

    virtual const char * getType() override { return GETPHYSICSPROPERTIES; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class GetPhysicsPropertiesResponse : public ros::Msg
  {
    public:
      typedef double _time_step_type;
      _time_step_type time_step;
      typedef bool _pause_type;
      _pause_type pause;
      typedef double _max_update_rate_type;
      _max_update_rate_type max_update_rate;
      typedef geometry_msgs::Vector3 _gravity_type;
      _gravity_type gravity;
      typedef gazebo_msgs::ODEPhysics _ode_config_type;
      _ode_config_type ode_config;
      typedef bool _success_type;
      _success_type success;
      typedef const char* _status_message_type;
      _status_message_type status_message;

    GetPhysicsPropertiesResponse():
      time_step(0),
      pause(0),
      max_update_rate(0),
      gravity(),
      ode_config(),
      success(0),
      status_message("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        double real;
        uint64_t base;
      } u_time_step;
      u_time_step.real = this->time_step;
      *(outbuffer + offset + 0) = (u_time_step.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_time_step.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_time_step.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_time_step.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_time_step.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_time_step.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_time_step.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_time_step.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->time_step);
      union {
        bool real;
        uint8_t base;
      } u_pause;
      u_pause.real = this->pause;
      *(outbuffer + offset + 0) = (u_pause.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->pause);
      union {
        double real;
        uint64_t base;
      } u_max_update_rate;
      u_max_update_rate.real = this->max_update_rate;
      *(outbuffer + offset + 0) = (u_max_update_rate.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_max_update_rate.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_max_update_rate.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_max_update_rate.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_max_update_rate.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_max_update_rate.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_max_update_rate.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_max_update_rate.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->max_update_rate);
      offset += this->gravity.serialize(outbuffer + offset);
      offset += this->ode_config.serialize(outbuffer + offset);
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.real = this->success;
      *(outbuffer + offset + 0) = (u_success.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->success);
      uint32_t length_status_message = strlen(this->status_message);
      varToArr(outbuffer + offset, length_status_message);
      offset += 4;
      memcpy(outbuffer + offset, this->status_message, length_status_message);
      offset += length_status_message;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        double real;
        uint64_t base;
      } u_time_step;
      u_time_step.base = 0;
      u_time_step.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_time_step.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_time_step.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_time_step.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_time_step.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_time_step.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_time_step.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_time_step.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->time_step = u_time_step.real;
      offset += sizeof(this->time_step);
      union {
        bool real;
        uint8_t base;
      } u_pause;
      u_pause.base = 0;
      u_pause.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->pause = u_pause.real;
      offset += sizeof(this->pause);
      union {
        double real;
        uint64_t base;
      } u_max_update_rate;
      u_max_update_rate.base = 0;
      u_max_update_rate.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_max_update_rate.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_max_update_rate.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_max_update_rate.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_max_update_rate.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_max_update_rate.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_max_update_rate.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_max_update_rate.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->max_update_rate = u_max_update_rate.real;
      offset += sizeof(this->max_update_rate);
      offset += this->gravity.deserialize(inbuffer + offset);
      offset += this->ode_config.deserialize(inbuffer + offset);
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.base = 0;
      u_success.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->success = u_success.real;
      offset += sizeof(this->success);
      uint32_t length_status_message;
      arrToVar(length_status_message, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_status_message; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_status_message-1]=0;
      this->status_message = (char *)(inbuffer + offset-1);
      offset += length_status_message;
     return offset;
    }

    virtual const char * getType() override { return GETPHYSICSPROPERTIES; };
    virtual const char * getMD5() override { return "575a5e74786981b7df2e3afc567693a6"; };

  };

  class GetPhysicsProperties {
    public:
    typedef GetPhysicsPropertiesRequest Request;
    typedef GetPhysicsPropertiesResponse Response;
  };

}
#endif
