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
	char header[4];
	uint8_t version;
	uint8_t subversion;
	float kp;
	float ki;
	float kd;
	uint8_t publish_frequency;
	uint8_t pid_frequency;

	float esc_rpm_to_speed_ratio;

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
#define SPEED_PIN_COUNT 16

//extern

void setup(void);
void loop(void);

#ifdef __cplusplus
}
#endif


#endif /* MAINPP_H_ */
