/*
 * mainpp.h
 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */

#ifndef MAINPP_H_
#define MAINPP_H_

#define W25QWRITE

#include "stm32h7xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif


typedef struct{
	int16_t throttle;
	uint32_t rpm;
	float voltage;
	float current;
	uint16_t temperature;
} ESC_SensorTypeDef;

typedef struct{
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

} ParameterTypeDef;

enum pid_mode_enum{
	PID_Auto,
	PID_Manual
};

#define ESC_DATA_SIZE 32
#define SPEED_PIN_COUNT 16

extern

void setup(void);
void loop(void);

#ifdef __cplusplus
}
#endif


#endif /* MAINPP_H_ */
