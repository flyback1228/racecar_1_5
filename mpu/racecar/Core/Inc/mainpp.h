/*
 * mainpp.h

 *
 *  Created on: 2023/11/17
 *      Author: xli185
 */

#ifndef MAINPP_H_
#define MAINPP_H_

#define W25QWRITE

#include "stm32h7xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif

//esc sensor data structure
typedef struct{
	int16_t throttle;
	uint32_t rpm;
	float voltage;
	float current;
	uint16_t temperature;
} ESC_SensorTypeDef;

//writable parameter structure, stored in W25Q
typedef struct{
	//header for handshake,"acsr"
	char header[4];

	//firmware version
	uint8_t version;
	uint8_t subversion;

	//pid parameters
	float kp;
	float ki;
	float kd;
	uint8_t pid_frequency;

	//ros parameters
	uint8_t publish_frequency;

	//esc parameters
	float esc_rpm_to_speed_ratio;
	float esc_offset;
	float esc_max;
	float esc_min;

	uint8_t esc_set_precision;
	uint8_t allow_reverse; //a bool


	//steering servo parameters
	float steering_esc_pwm_frequency;
	float steering_offset;
	float steering_to_dutycycle_ratio;
	float steering_max;
	float steering_min;

	uint8_t servo_set_precision;

	//force parameters
	float force_ratio[8];
	float force_offset[8];

	//brake parameters
	float brake_pwm_frequency;

	float wheel_speed_difference_warning;

	//tailer for handshake,"b401"
	char tailer[4];

} ParameterTypeDef;


//pid mode
//typedef enum
//{
//	PID_MODE_AUTOMATIC = 0,
//	PID_MODE_MANUAL    = 1
//
//}PIDMode_TypeDef;

//input mode, can be set via external switch or software
typedef enum
{
	INPUT_MODE_SOFTWARE    = 0,
	INPUT_MODE_CONTROLLER = 1
}InputMode_TypeDef;

//
#define ESC_DATA_SIZE 32
#define SPEED_PIN_COUNT 8

//extern

void set_esc_duty_cycle(float );
void set_servo_duty_cycle(float );
void set_brake(uint8_t*);

void setup(void);
void loop(void);

#ifdef __cplusplus
}
#endif


#endif /* MAINPP_H_ */
