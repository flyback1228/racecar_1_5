/*
 * mainpp.h
 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */

#ifndef MAINPP_H_
#define MAINPP_H_
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
