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


template <int N>
struct Acsr_PWM_TypeDef{
	uint32_t arr;
	uint32_t ccr[N];
	uint32_t pre_ccr[N];
	uint32_t pre_ic[N];
};

#ifdef __cplusplus
 extern "C" {
#endif

//esc sensor data structure
typedef struct{
	int16_t throttle;
	uint16_t temperature;
	uint32_t rpm;
	uint16_t voltage;
	uint16_t current;
} ESC_SensorTypeDef;




//writable parameter structure, stored in W25Q
typedef struct{
	//header for handshake,"acsr"
	char header[4];

	//firmware version
	uint8_t version;
	uint8_t subversion;
	uint8_t pid_frequency;
	uint8_t publish_frequency;

	uint8_t esc_set_precision;
	uint8_t allow_reverse; //a bool
	uint8_t servo_set_precision;
	uint8_t upload_speed;

	//pid parameters
	float kp;
	float ki;
	float kd;


	//ros parameters


	//esc parameters
	float esc_rpm_to_speed_ratio;
	float esc_offset;
	float esc_max;
	float esc_min;




	//steering servo parameters
	float steering_esc_pwm_frequency;
	float steering_offset;
	float steering_to_dutycycle_ratio;
	float steering_max;
	float steering_min;



	//force parameters
	float force_ratio[8];
	float force_offset[8];

	//brake parameters
	float brake_pwm_frequency;

	float wheel_speed_difference_warning;



	//tailer for handshake,"b401"
	char tailer[4];

} ParameterTypeDef;


typedef struct{
	//header for handshake,"acsr"
	char header[4];

	float current_speed;
	float setpoint;
	float output;

	char tailer[4];

} SpeedTypeDef;


//input mode, can be set via external switch or software
typedef enum
{
	INPUT_MODE_SOFTWARE    = 0,
	INPUT_MODE_CONTROLLER = 1
}InputMode_TypeDef;


#define SPEED_PIN_COUNT 8

void set_esc_duty_cycle(float );
void set_servo_duty_cycle(float );
void set_brake(uint8_t*);

void setup(void);
void loop(void);

#ifdef __cplusplus
}
#endif


#endif /* MAINPP_H_ */
