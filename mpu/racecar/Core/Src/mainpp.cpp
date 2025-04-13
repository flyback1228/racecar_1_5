/*
 * mainpp.cpp

 *
 *  Created on: 2023/11/17
 *      Author: xli185
 */
#include <mainpp.h>
#include "main.h"
#include "stm32h7xx_hal_rcc.h"
#include "string.h"
#include <cstdio>

#include "jy901/JY901_Serial.h"
#include "w25q64jv/w25q64jv.h"
#include "utility.h"
#include "pid/pid.h"



#define MAX_PARAMETER_LENGTH 200
const uint8_t wheel_speed_size=SPEED_PIN_COUNT;
const uint8_t force_size = 8;
const uint8_t vesc_size = 5;
const uint8_t imu_size = 9;
const int32_t nvdia_buf_size = 40;

//timers
//pwm for brakes, 4 channels in total
extern TIM_HandleTypeDef htim2;
//pwm for esc and servo, ch1 for servo, ch2 for esc
extern TIM_HandleTypeDef htim3;
//pwm input caputure, for servo
extern TIM_HandleTypeDef htim5;
//pwm input caputure, for esc
extern TIM_HandleTypeDef htim15;
//timer for uploading data to host computer via serial
extern TIM_HandleTypeDef htim16;
//timer for pid computation
extern TIM_HandleTypeDef htim6;

//uarts
//communicate with ESC
extern UART_HandleTypeDef hlpuart1;
//communicate with NVIDIA Jetson
extern UART_HandleTypeDef huart4;
//communicate with STM32F103
extern UART_HandleTypeDef huart5;
//to cp2102, usb port, for setting parameters. work togather with the QT software
extern UART_HandleTypeDef huart7;
//communicate with imu
extern UART_HandleTypeDef huart8;


extern DMA_HandleTypeDef hdma_uart7_rx;
extern DMA_HandleTypeDef hdma_uart4_rx;

//force sensor adc
extern ADC_HandleTypeDef hadc1;

#define huart_nvidia huart4
#define huart_esc hlpuart1
#define huart_f103 huart5
#define huart_usb huart7
#define huart_imu huart8


//sensor data, uploading to
uint8_t* sensor_data;

//esc senso data
ESC_SensorTypeDef esc_sensor;

//error code
uint8_t error_code = 0;

//f103 data receive label
uint8_t f103_receive_indicator = 0;

//host nvdia receive label
uint8_t received_from_nvdia_indicator=0;

//data from STM32F103, the first four elements are "acsr"
uint8_t f103_receive[2*SPEED_PIN_COUNT+9+sizeof(ESC_SensorTypeDef)];

//parsed data of f103 data
uint8_t f103_data[2*SPEED_PIN_COUNT+5+sizeof(ESC_SensorTypeDef)];

//force data from ADC
uint32_t force_raw[8];

//PIDMode_TypeDef
PIDMode_TypeDef pid_mode = PID_MODE_MANUAL;

//pid varibles
float pid_speed_set = 0.0;
float pid_esc_duty_cycle_output = 0.0;
float current_esc_speed = 0.0;

//usb buffer to store the received data from usb port
uint8_t usb_buf[MAX_PARAMETER_LENGTH];

//input mode, by code or by transmitter, can be set by ros topic or by button on the control board
InputMode_TypeDef input_mode = INPUT_MODE_SOFTWARE;

//pwm structures. arr are initialized after reading the parameters
Acsr_PWM_TypeDef<4> brake_pwm;
Acsr_PWM_TypeDef<2> servo_pwm;

//imu
uint8_t jy901_data_length = 11;
uint8_t jy901_data[11];
CJY901 jy901(&huart8);


uint8_t nvidia_buf[nvdia_buf_size];

ParameterTypeDef parameters = {
		.header={'p','r','m','b'},

		.version = 1,
		.subversion = 1,
		.pid_frequency = 10,
		.publish_frequency = 20,

		.esc_set_precision = 5,
		.allow_reverse = 0,
		.servo_set_precision = 5,
		.upload_speed = false,

		.kp = 2.0,
		.ki = 1.0,
		.kd = 0.0,

		.esc_rpm_to_speed_ratio = 1800.0,
		.esc_offset=0.096,
		.esc_max = 0.124,
		.esc_min = 0.072,

		.steering_esc_pwm_frequency = 64.5,
		.steering_offset=0.097,
		.steering_to_dutycycle_ratio=653.8,
		.steering_max = 17.0,
		.steering_min = -17.0,

		.force_ratio ={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
		.force_offset ={0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0},

		.brake_pwm_frequency=100.0,

		.wheel_speed_difference_warning = 1.0,

		.tailer={'p','r','m','e'}
};

SpeedTypeDef speed_ = {
		.header={'s','p','d','b'},

		.current_speed = 2.0,
		.setpoint = 1.0,
		.output = 0.0,

		.tailer={'s','p','d','e'}
};

PID<float> *pid_ptr;

char usb_msg[200];

void send_usb_msg_to_host(char* str, uint8_t size){
	usb_msg[4]=size;
	memcpy(&usb_msg[5],str,size);
	usb_msg[5+size]='m';
	usb_msg[6+size]='s';
	usb_msg[7+size]='g';
	usb_msg[8+size]='e';
	HAL_UART_Transmit(&huart7, (uint8_t*)usb_msg, size+9, 10);
}

void command_process(uint8_t* data){
	//data[0] : input mode
	HAL_GPIO_WritePin(Manual_Output_GPIO_Port, Manual_Output_Pin, (GPIO_PinState)data[0]);

	//data[1] : pid mode
	if(data[1] != (uint8_t)pid_mode){
		pid_mode = (PIDMode_TypeDef)data[1];
		pid_ptr->reset();
		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, (GPIO_PinState)data[1]);
	}

	//data[2]~data[5]:brake, brake is represent by a uint8 0-255 correponding dutycycle 0-1
	set_brake(&data[2]);

	//other commands are ignored if input mode is mannual
	if(data[0] || input_mode == INPUT_MODE_CONTROLLER ){
		return;
	}

	//data[6]: speed integer part if PID_MODE_AUTOMATIC, the high byte of int16 if PID_MODE_MANNUAL
	//data[7]: speed decimal part if PID_MODE_AUTOMATIC, the low byte of int16 if PID_MODE_MANNUAL
	//duty cycle shall convert to 0-1 by divided by 1000
	if(pid_mode == PID_MODE_AUTOMATIC){
		pid_speed_set = data[6] + data[7]/100.0f;
	}else{
		auto dc = (int16_t) (data[6]<<8) | data[7];
		set_esc_duty_cycle(dc/1000.0);
	}

	//data[8]: steering angle integer part (the first bit indicates the sign)
	//data[9]: steering angle decimal part
	float steering = (data[8] & 0b01111111) + data[9]/100.0;
	if(data[8]>>7)steering=-steering;
	set_servo_duty_cycle(steering);
}


//read speed data from f103
//the first data are "acsr", then the fifth is the integer part of speed1, and the sixth is the decimal part of speed1..
HAL_StatusTypeDef parse_f103_data(uint8_t* data){
	uint8_t i;
	for(i = 0;i<2*SPEED_PIN_COUNT;++i){
		if(data[i]=='A' && data[i+1]=='C'&& data[i+2]=='S'&& data[i+3]=='R')break;
	}
	if(i==2*SPEED_PIN_COUNT)return HAL_ERROR;

	if(i==0){
		memcpy(f103_data,&data[4],2*SPEED_PIN_COUNT+5+sizeof(ESC_SensorTypeDef));
	}else{
		memcpy(f103_data,&data[i+4],2*SPEED_PIN_COUNT+5+sizeof(ESC_SensorTypeDef)-i);
		memcpy(&f103_data[2*SPEED_PIN_COUNT+5+sizeof(ESC_SensorTypeDef)-i],data,i);
	}

	memcpy(&esc_sensor,&f103_data[2*SPEED_PIN_COUNT+5], sizeof(esc_sensor));
	current_esc_speed = esc_sensor.rpm/parameters.esc_rpm_to_speed_ratio;

	if(abs((int)f103_data[4]*100+f103_data[5]-((int)f103_data[6]*100+f103_data[7]))>parameters.wheel_speed_difference_warning*100){
		error_code |= 0b10000000;
	}else{
		error_code &= 0b01111111;
	}

	if(abs((int)f103_data[8]*100+f103_data[9]-((int)f103_data[10]*100+f103_data[11]))>parameters.wheel_speed_difference_warning*100){
		error_code |= 0b01000000;
	}else{
		error_code &= 0b10111111;
	}

	if(abs((int)f103_data[12]*100+f103_data[13]-((int)f103_data[14]*100+f103_data[15]))>parameters.wheel_speed_difference_warning*100){
		error_code |= 0b00100000;
	}else{
		error_code &= 0b11011111;
	}

	if(abs((int)f103_data[16]*100+f103_data[17]-((int)f103_data[18]*100+f103_data[19]))>parameters.wheel_speed_difference_warning*100){
		error_code |= 0b00010000;
	}else{
		error_code &= 0b11101111;
	}

	if(f103_data[20]){
		error_code |= 0b00000001;
	}else{
		error_code &= 0b11111110;
	}

	f103_receive_indicator = 0;
	return HAL_OK;
}

//read speed data from f103
//the first data are "acsr", then the fifth is the integer part of speed1, and the sixth is the decimal part of speed1..
HAL_StatusTypeDef parse_cmd_data(uint8_t* data){
	uint8_t i;
	for(i = 0;i<2*SPEED_PIN_COUNT;++i){
		if(data[i]=='A' && data[i+1]=='C'&& data[i+2]=='S'&& data[i+3]=='R')break;
	}
	if(i==2*SPEED_PIN_COUNT)return HAL_ERROR;

	if(i==0){
		memcpy(f103_data,&data[4],2*SPEED_PIN_COUNT+5+sizeof(ESC_SensorTypeDef));
	}else{
		memcpy(f103_data,&data[i+4],2*SPEED_PIN_COUNT+5+sizeof(ESC_SensorTypeDef)-i);
		memcpy(&f103_data[2*SPEED_PIN_COUNT+5+sizeof(ESC_SensorTypeDef)-i],data,i);
	}

	memcpy(&esc_sensor,&f103_data[2*SPEED_PIN_COUNT+5], sizeof(esc_sensor));
	current_esc_speed = esc_sensor.rpm/parameters.esc_rpm_to_speed_ratio;

	if(abs((int)f103_data[4]*100+f103_data[5]-((int)f103_data[6]*100+f103_data[7]))>parameters.wheel_speed_difference_warning*100){
		error_code |= 0b10000000;
	}else{
		error_code &= 0b01111111;
	}

	if(abs((int)f103_data[8]*100+f103_data[9]-((int)f103_data[10]*100+f103_data[11]))>parameters.wheel_speed_difference_warning*100){
		error_code |= 0b01000000;
	}else{
		error_code &= 0b10111111;
	}

	if(abs((int)f103_data[12]*100+f103_data[13]-((int)f103_data[14]*100+f103_data[15]))>parameters.wheel_speed_difference_warning*100){
		error_code |= 0b00100000;
	}else{
		error_code &= 0b11011111;
	}

	if(abs((int)f103_data[16]*100+f103_data[17]-((int)f103_data[18]*100+f103_data[19]))>parameters.wheel_speed_difference_warning*100){
		error_code |= 0b00010000;
	}else{
		error_code &= 0b11101111;
	}

	if(f103_data[20]){
		error_code |= 0b00000001;
	}else{
		error_code &= 0b11111110;
	}

	f103_receive_indicator = 0;
	return HAL_OK;
}

//uart data receive callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==huart_f103.Instance){
		parse_f103_data(f103_receive);
	}else if(huart->Instance==huart_imu.Instance){
		if(jy901_data[0]==0x55 && jy901_data_length==11){
			jy901.parseData(jy901_data);
			HAL_UART_Receive_DMA(&huart8, jy901_data, jy901_data_length);

		}else if(jy901_data_length!=11){
			jy901_data_length = 11;
			HAL_UART_Receive_DMA(&huart8, jy901_data, jy901_data_length);
		}else{
			uint8_t i = 0;
			for(;i<11;++i){
				if(jy901_data[i]==0x55)break;
			}
			jy901_data_length = i;
			HAL_UART_Receive_DMA(&huart8, jy901_data, jy901_data_length);
		}
	}
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	//uart receive callback for cp2102
	if (huart->Instance == huart_usb.Instance)
	{
		if(strncmp((char *)usb_buf,"xilin",5)==0){
			HAL_UART_Transmit(&huart7, (uint8_t*)(&parameters) , sizeof(ParameterTypeDef), 10);
		}else if(strncmp((char *)usb_buf,parameters.header,4)==0){
			uint32_t i = sizeof(ParameterTypeDef)-4;
			if(strncmp((char *)(usb_buf+i),parameters.tailer,4)==0){
				char msg[]="Receive Wrong Data\n";
				send_usb_msg_to_host(msg,sizeof(msg));
			}else{
				memcpy(&parameters,usb_buf,sizeof(ParameterTypeDef));
				char msg[]="Write the Configuration Complete!\n";
				send_usb_msg_to_host(msg,sizeof(msg));
				QSPI_W25Q64JV_Write((uint8_t*)(&parameters),0x0,sizeof(ParameterTypeDef));
				NVIC_SystemReset();
			}

		}else if(strncmp((char *)usb_buf,"reset",5)==0){
			HAL_UART_Transmit(&huart_f103, usb_buf, 5, 100);
			NVIC_SystemReset();
		}
		else{
			char msg[]="Receive Wrong Data\n\n";
			send_usb_msg_to_host(msg,sizeof(msg));
		}

		HAL_UARTEx_ReceiveToIdle_DMA(&huart_usb, (uint8_t *) usb_buf, MAX_PARAMETER_LENGTH);
		__HAL_DMA_DISABLE_IT(&hdma_uart7_rx, DMA_IT_HT);
	}
	//uart receive callback for nvidia
	else if(huart->Instance == huart_nvidia.Instance){
		if(Size==13 && nvidia_buf[0] == 0xAA && nvidia_buf[1] == 0x55 && nvidia_buf[Size-1] == 0x0A){
			received_from_nvdia_indicator = 0;
			command_process(nvidia_buf+2);
		}
		HAL_UARTEx_ReceiveToIdle_DMA(&huart_nvidia, (uint8_t *) nvidia_buf, nvdia_buf_size);
		__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
	}
}

//error handle of uart
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
	if(UartHandle->Instance==huart_f103.Instance) {
    	HAL_UART_Receive_DMA(&huart_f103, f103_receive, 2*SPEED_PIN_COUNT+9+sizeof(ESC_SensorTypeDef));
    }
    else if(UartHandle->Instance==huart_imu.Instance) {
		HAL_UART_Receive_DMA(&huart_imu, (uint8_t*)jy901_data, 11);
	}else if(UartHandle->Instance==huart_usb.Instance){
		HAL_UARTEx_ReceiveToIdle_DMA(&huart_usb, (uint8_t *) usb_buf, MAX_PARAMETER_LENGTH);
		__HAL_DMA_DISABLE_IT(&hdma_uart7_rx, DMA_IT_HT);
	}else if(UartHandle->Instance==huart_nvidia.Instance){
		HAL_UARTEx_ReceiveToIdle_DMA(&huart_nvidia, (uint8_t *) nvidia_buf, 20);
		__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
	}
}

//pwm input capture callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(input_mode == INPUT_MODE_SOFTWARE)return;

	if(htim->Instance==TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		uint32_t steering_ic = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		int32_t diff = steering_ic - servo_pwm.pre_ic[0];
		if(abs(diff) > 2){
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1,steering_ic);
			servo_pwm.pre_ic[0] = steering_ic;
		}

		__HAL_TIM_SetCounter(htim,0);
		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);

	}else if(htim->Instance==TIM15 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){

		uint32_t esc_ic = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		int32_t diff = esc_ic - servo_pwm.pre_ic[1];
		if(abs(diff) >2){
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2,esc_ic);
			servo_pwm.pre_ic[1] = esc_ic;
		}

		__HAL_TIM_SetCounter(htim,0);
		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);

	}

}

void update_and_upload_speed(float esc_speed,float setpoint,float dutycycle){
	speed_.current_speed=esc_speed;
	speed_.setpoint=setpoint;
	speed_.output=dutycycle;
	HAL_UART_Transmit(&huart7, (uint8_t*)(&speed_), sizeof(SpeedTypeDef), 10);
}

//timer callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	//ros publish
	if(htim->Instance==TIM16){

		memcpy(sensor_data+2,f103_data,2*wheel_speed_size+4);
		uint8_t index =2 + 2*wheel_speed_size+4;
		memcpy(sensor_data+index,force_raw,4*force_size);
		index+=4*force_size;
		memcpy(sensor_data+index,&f103_data[2*SPEED_PIN_COUNT+5], sizeof(esc_sensor));
		//memcpy(&esc_sensor,&f103_data[2*SPEED_PIN_COUNT+5], sizeof(esc_sensor));
		index+=sizeof(esc_sensor);
		memcpy(sensor_data+index,&jy901.JY901_data.acc,sizeof(jy901.JY901_data.acc));
		index+=sizeof(jy901.JY901_data.acc);
		memcpy(sensor_data+index,&jy901.JY901_data.gyro,sizeof(jy901.JY901_data.gyro));
		index+=sizeof(jy901.JY901_data.gyro);
		memcpy(sensor_data+index,&jy901.JY901_data.angle,sizeof(jy901.JY901_data.angle));
		index+=sizeof(jy901.JY901_data.angle);

		sensor_data[index] = error_code;
		sensor_data[index+1] = 0xBB;
		sensor_data[index+2] = 0xCC;
		sensor_data[index+3] = '\n';

		HAL_UART_Transmit(&huart4, sensor_data, index+4, 100);
	}else if(htim->Instance==TIM6)//pid computation
	{
		//not pid mode
		if(input_mode == INPUT_MODE_CONTROLLER || pid_mode == PID_MODE_MANUAL){
			if(parameters.upload_speed){
				update_and_upload_speed(current_esc_speed, 0, esc_sensor.throttle);
			}
			return;
		}
		// no esc signal
		if(error_code & 0x01){
			set_esc_duty_cycle(0.0);
			if(parameters.upload_speed){
				update_and_upload_speed(0, 0, 0);
			}
			return;
		}
		pid_ptr->compute();
		set_esc_duty_cycle(pid_esc_duty_cycle_output);

		if(parameters.upload_speed){
			update_and_upload_speed(current_esc_speed, pid_speed_set, esc_sensor.throttle);
		}
	}
}


void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance==TIM16){
		HAL_TIM_Base_Start_IT(&htim16);
	}

	else if(htim->Instance==TIM5){
		__HAL_TIM_SetCounter(htim,0);
		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
	}
	else if(htim->Instance==TIM15){
		__HAL_TIM_SetCounter(htim,0);
		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);
	}
}


//set the duty cycle of the esc,
//input: fake_duty_cycle, a virtual duty cycle range from -1.0~1.0, where minus represents reverse
void set_esc_duty_cycle(float fake_duty_cycle){
	float esc_duty_cycle;
	if(fake_duty_cycle>0){
		esc_duty_cycle = parameters.esc_offset - fake_duty_cycle * (parameters.esc_offset-parameters.esc_min);
	}else{
		if(!parameters.allow_reverse){
			esc_duty_cycle=parameters.esc_offset;
		}
		else {
			esc_duty_cycle = parameters.esc_offset - fake_duty_cycle * (parameters.esc_max-parameters.esc_offset);
		}
	}

	//convert duty cycle to pwm ccr
	int32_t esc_ccr = esc_duty_cycle * servo_pwm.arr;
	if(abs(esc_ccr - (int)servo_pwm.pre_ccr[1])>=parameters.esc_set_precision){
		servo_pwm.pre_ccr[1] = esc_ccr;
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,esc_ccr);
	}
}

//set the duty cycle of the esc,
//input: steering_angle, unit is degree
void set_servo_duty_cycle(float steering_angle){
	steering_angle = MAX(steering_angle,parameters.steering_min);
	steering_angle = MIN(steering_angle,parameters.steering_max);
	float servo_duty_cycle = steering_angle/parameters.steering_to_dutycycle_ratio + parameters.steering_offset;

	//convert duty cycle to pwm ccr
	int32_t servo_ccr = servo_duty_cycle * servo_pwm.arr;
	if(abs(servo_ccr - (int)servo_pwm.pre_ccr[0])>=parameters.servo_set_precision){
		servo_pwm.pre_ccr[0] = servo_ccr;
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,servo_ccr);
	}
}

//set the brake,
//input: brake pwm, value from 0-255
void set_brake(uint8_t* data){
	uint32_t c = data[0]*brake_pwm.arr/255;
	if(c!=brake_pwm.pre_ccr[0]){
		brake_pwm.pre_ccr[0]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,c);
	}
	c = data[1]*brake_pwm.arr/255;
	if(c!=brake_pwm.pre_ccr[1]){
		brake_pwm.pre_ccr[1]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,c);
	}
	c = data[2]*brake_pwm.arr/255;
	if(c!=brake_pwm.pre_ccr[2]){
		brake_pwm.pre_ccr[2]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,c);
	}
	c = data[3]*brake_pwm.arr/255;
	if(c!=brake_pwm.pre_ccr[3]){
		brake_pwm.pre_ccr[3]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4,c);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_9) {
	  auto state = HAL_GPIO_ReadPin(PID_MODE_INPUT_GPIO_Port, PID_MODE_INPUT_Pin);
	  if(state==GPIO_PIN_SET){
		  pid_mode = PID_MODE_AUTOMATIC;
		  pid_ptr->reset();
	  }else{
		  pid_mode = PID_MODE_MANUAL;
	  }
	  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, state);
  }
}

void adc_setup(){
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)force_raw,8);
}

void uart_setup(){
	//HAL_UART_Receive_DMA(&huart_esc, esc_receive, ESC_DATA_SIZE);
	HAL_UART_Receive_DMA(&huart_f103, f103_receive, 2*SPEED_PIN_COUNT+9+sizeof(ESC_SensorTypeDef));
	HAL_UART_Receive_DMA(&huart8, jy901_data, 11);

	//huart7.RxXferSize=MAX_PARAMETER_LENGTH;
	//huart7.RxXferSize

	HAL_UARTEx_ReceiveToIdle_DMA(&huart7, (uint8_t *) usb_buf, MAX_PARAMETER_LENGTH);
	__HAL_DMA_DISABLE_IT(&hdma_uart7_rx, DMA_IT_HT);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart4, (uint8_t *) nvidia_buf, 20);
	__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
}

void timer_setup(){
	//set tim16 ARR value based on topic publish frequency and start tim16, 10000 = 100M/(9999+1), where 9999 is the prescale of timer16
	__HAL_TIM_SET_PRESCALER(&htim16,9999);
	__HAL_TIM_SET_AUTORELOAD(&htim16,10000/parameters.publish_frequency-1);

	//set tim6 ARR value based on PID calculation frequency and start tim7
	//__HAL_TIM_SET_PRESCALER(&htim6,9999);
	__HAL_TIM_SET_AUTORELOAD(&htim6,10000/parameters.pid_frequency-1);
	HAL_TIM_Base_Start_IT(&htim6);

	//start esc and steering servo pwm output
	servo_pwm.arr = 1000000/parameters.steering_esc_pwm_frequency-1;
	__HAL_TIM_SET_AUTORELOAD(&htim3,servo_pwm.arr);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,parameters.steering_offset);
	servo_pwm.pre_ccr[0] = parameters.steering_offset;

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,parameters.esc_offset);
	servo_pwm.pre_ccr[1] = parameters.esc_offset;

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	//set brake pwm, prescaler 99, timer frequency is 100MHz
	__HAL_TIM_SET_PRESCALER(&htim2,99);
	//set tim2 ARR value based on brake frequency and start tim6, 1000000 = 100M/(99+1)
	brake_pwm.arr = uint32_t(1000000/parameters.brake_pwm_frequency-1);
	__HAL_TIM_SET_AUTORELOAD(&htim2,brake_pwm.arr);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	//initial value zeros
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4,0);

	//start upload data to host
	//this timer should the last one to start.
	HAL_TIM_Base_Start_IT(&htim16);

}


void variable_init(){
	sensor_data = new uint8_t[2 + 2*wheel_speed_size + 4 + 4*force_size + sizeof(esc_sensor) + sizeof(jy901.JY901_data.acc) + sizeof(jy901.JY901_data.gyro)+sizeof(jy901.JY901_data.angle)+4];
	sensor_data[0] = 0x55;
	sensor_data[1] = 0xAA;
}

void gpio_setup(){
	//set default pid mode to manual
	HAL_GPIO_WritePin(Manual_Output_GPIO_Port, Manual_Output_Pin, GPIO_PIN_SET);
	//red led, used as power indicator
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	//blue and yellow led, default off
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
}

//w25q64
void read_parameters(){
	QSPI_W25Q64JV_Init();
	QSPI_W25Q64JV_Reset();

	uint8_t id[2];
	if (QSPI_OK != QSPI_W25Q64JV_DeviceID(id)) {
		char msg[]="Initializing ROM\n";
		send_usb_msg_to_host(msg, sizeof(msg));
	}
	char msg[50];
	sprintf(msg,"Connect to ROM, ROM ID: [0x%02x,0x%02x]\n",id[0],id[1]);
	send_usb_msg_to_host(msg,50);

	char header[4];
	QSPI_W25Q64JV_Read((uint8_t*)header, 0x00, 4);

	char error_message[]="Reading parameters fails, use default parameters\n";

	if(header[0]!='p' || header[1]!='r' || header[2]!='m' || header[3]!='b'){
		send_usb_msg_to_host(error_message, sizeof(error_message));
		QSPI_W25Q64JV_Write((uint8_t*)(&parameters),0x0,sizeof(ParameterTypeDef));
		return;
	}

	QSPI_W25Q64JV_Read((uint8_t*)(&parameters), 0x00, sizeof(ParameterTypeDef));
	if(parameters.tailer[0]!='p' || parameters.tailer[1]!='r' || parameters.tailer[2]!='m' || parameters.tailer[3]!='e'){
		QSPI_W25Q64JV_Write((uint8_t*)(&parameters),0x0,sizeof(ParameterTypeDef));
		send_usb_msg_to_host(error_message, sizeof(error_message));
		return;
	}
	char suc_msg[]="Reading parameters from ROM success\n";
	send_usb_msg_to_host(suc_msg,sizeof(suc_msg));
}

void reset_pid(){
	if(pid_ptr)delete pid_ptr;
	pid_ptr = new PID<float>(&current_esc_speed,&pid_esc_duty_cycle_output,&pid_speed_set,parameters.kp,parameters.ki,parameters.kd);

	if(parameters.allow_reverse)
		pid_ptr->set_output_limits(-1.0,+1.0);
	else
		pid_ptr->set_output_limits(0.0, 1.0);
}


void setup(void)
{
	DWT_Init();

	usb_msg[0] = 'm';
	usb_msg[1] = 's';
	usb_msg[2] = 'g';
	usb_msg[3] = 'b';

	read_parameters();
	reset_pid();
	uart_setup();
	adc_setup();
	variable_init();
	timer_setup();
	HAL_GPIO_WritePin(Manual_Output_GPIO_Port, Manual_Output_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ONBOARD_LED_GPIO_Port, ONBOARD_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, (GPIO_PinState)pid_mode);



	for(int i=0;i<10;++i){
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_Delay(200);
	}
}

uint8_t loop_index = 0;

void loop(void)
{
	auto state = HAL_GPIO_ReadPin(Manual_Input_GPIO_Port, Manual_Input_Pin);

	received_from_nvdia_indicator++;
	if(received_from_nvdia_indicator >= 2){
		//ros_received = 2;
		set_esc_duty_cycle(0.0);
		set_servo_duty_cycle(0.0);
	}

	if(state==GPIO_PIN_SET && input_mode == INPUT_MODE_SOFTWARE){
		//pwm input capture for servo and esc
		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);
		input_mode = INPUT_MODE_CONTROLLER;

	}else if(state==GPIO_PIN_RESET && input_mode == INPUT_MODE_CONTROLLER){
		HAL_TIM_IC_Stop_IT(&htim5, TIM_CHANNEL_1);
		HAL_TIM_IC_Stop(&htim5, TIM_CHANNEL_2);
		HAL_TIM_IC_Stop_IT(&htim15, TIM_CHANNEL_1);
		HAL_TIM_IC_Stop(&htim15, TIM_CHANNEL_2);

		input_mode = INPUT_MODE_SOFTWARE;
	}
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, state);

	loop_index++;

	if(f103_receive_indicator++>10){
		f103_receive_indicator=11;
		error_code = error_code | 0b00000011;
	}
	else {
		error_code = error_code & 0b11111101;
	}

	HAL_Delay(100);
	if(loop_index==5){
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		loop_index=0;

	}
	//HAL_IWDG_Refresh(&hiwdg1);
}

