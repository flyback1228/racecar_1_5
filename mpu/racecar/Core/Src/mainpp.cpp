/*
 * mainpp.cpp

 *
 *  Created on: 2023/11/17
 *      Author: xli185
 */
#include <mainpp.h>
#include <ros.h>
#include <std_msgs/UInt8MultiArray.h>
#include "main.h"
#include "stm32h7xx_hal_rcc.h"
#include "string.h"
#include <cstdio>

#include "jy901/JY901_Serial.h"
#include "w25q64jv/w25q64jv.h"
#include "utility.h"
#include "pid/pid.h"

//watch dog
extern IWDG_HandleTypeDef hiwdg1;

//pwm for brakes, 4 channels in total
extern TIM_HandleTypeDef htim2;

//pwm for esc and servo, ch1 for servo, ch2 for esc
extern TIM_HandleTypeDef htim3;

//pwm input caputure, for servo
extern TIM_HandleTypeDef htim5;
//pwm input caputure, for esc
extern TIM_HandleTypeDef htim15;

//ros publish timer
extern TIM_HandleTypeDef htim16;

//pid computation timer
extern TIM_HandleTypeDef htim6;

//communicate with ESC
extern UART_HandleTypeDef hlpuart1;

//communicate with NVIDIA Jetson, used as ROS port
extern UART_HandleTypeDef huart4;

//communicate with STM32F103
extern UART_HandleTypeDef huart5;

//to cp2102, usb port
extern UART_HandleTypeDef huart7;

//communicate with imu
extern UART_HandleTypeDef huart8;

extern DMA_HandleTypeDef hdma_uart7_rx;

//force sensor adc
extern ADC_HandleTypeDef hadc1;

#define huart_ros huart4
#define huart_esc hlpuart1
#define huart_f103 huart5
#define huart_usb huart7
#define huart_imu huart8

ros::NodeHandle nh;

//sensor data message published by ros
std_msgs::UInt8MultiArray sensor_msg;
ros::Publisher ros_pub("stm32_sensor", &sensor_msg);


//esc senso data
ESC_SensorTypeDef esc_sensor;
uint8_t esc_receive[ESC_DATA_SIZE];

//size of messages
const uint8_t wheel_speed_size=SPEED_PIN_COUNT;
const uint8_t force_size = 8;
const uint8_t vesc_size = 5;
const uint8_t imu_size = 9;

//error code
//0x01: no esc data, 0x02: no speed data
uint8_t error_code = 0;


uint8_t esc_receive_indicator = 0;
uint8_t f103_receive_indicator = 0;

//speed data from STM32F103, the first four elements are "acsr"
uint8_t speed_receive[2*SPEED_PIN_COUNT+4];
//speed, represent by frequency
uint8_t speed[2*SPEED_PIN_COUNT];

//uint8_t pwm_generator_indicator,pre_pwm_generator_indicator;
uint8_t is_frequency_set;
uint32_t force_raw[8];

//PIDMode_TypeDef
PIDMode_TypeDef pid_mode = PID_MODE_MANUAL;

//pid varibles
float pid_speed_set = 0.0;
float pid_esc_duty_cycle_output = 0.0;
float current_esc_speed = 0.0;

//usb buffer to store the received data from usb port
uint8_t usb_buf[100];

//input mode, by code or by transmitter, can be set by ros topic or by button on the control board
InputMode_TypeDef input_mode = INPUT_MODE_SOFTWARE;

//auto reload value for timer, those values are initialized after reading the parameters
uint32_t brake_arr;
uint32_t esc_servo_arr;

//last data, to compare with the received data.
int32_t pre_servo_ccr=0;
int32_t pre_esc_ccr=0;
uint32_t pre_brake[]={0,0,0,0};

uint32_t pre_servo_ic = 0;
uint32_t pre_esc_ic = 0;
//uint32_t pre_ic_freq;

uint8_t jy901_data_length = 11;
uint8_t jy901_data[11];
CJY901 jy901(&huart8);

ParameterTypeDef parameters = {
		.header={'a','c','s','r'},
		.version = 1,
		.subversion = 1,

		.kp = 2.0,
		.ki = 1.0,
		.kd = 0.0,
		.pid_frequency = 10,

		.publish_frequency = 20,

		.esc_rpm_to_speed_ratio = 100.0,
		.esc_offset=0.125,
		.esc_max = 0.15,
		.esc_min = 0.1,

		.esc_set_precision = 5,
		.allow_reverse = 1,

		.steering_esc_pwm_frequency = 64.5,
		.steering_offset=0.125,
		.steering_to_dutycycle_ratio=100.0,
		.steering_max = 17.0,
		.steering_min = -17.0,

		.servo_set_precision = 5,

		.force_ratio ={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},

		.brake_pwm_frequency=100.0,

		.tailer={'b','4','0','1'}
};

PID<float> *pid_ptr;


//read esc data from ble
HAL_StatusTypeDef read_ble_data(uint8_t* data){
	static uint8_t start_index = 0;
	for(uint8_t i=start_index;i<start_index+ESC_DATA_SIZE;++i){
		if(data[i]==0xFE && data[(i+1)%ESC_DATA_SIZE]==0x01){
			start_index = i%ESC_DATA_SIZE;
			break;
		}
	}

	if(data[(2+start_index)%ESC_DATA_SIZE]!=0x00)return HAL_ERROR;
	if(data[(3+start_index)%ESC_DATA_SIZE]!=0x03)return HAL_ERROR;
	if(data[(4+start_index)%ESC_DATA_SIZE]!=0x30)return HAL_ERROR;
	if(data[(5+start_index)%ESC_DATA_SIZE]!=0x5c)return HAL_ERROR;
	if(data[(6+start_index)%ESC_DATA_SIZE]!=0x17)return HAL_ERROR;
	if(data[(7+start_index)%ESC_DATA_SIZE]!=0x06)return HAL_ERROR;

	esc_sensor.throttle = (int16_t)(data[(9+start_index)%ESC_DATA_SIZE]);
	if(data[(11+start_index)%ESC_DATA_SIZE]!=0x01)esc_sensor.throttle=-esc_sensor.throttle;

	esc_sensor.rpm = ((uint32_t)(data[(14+start_index)%ESC_DATA_SIZE] <<8) | (data[(13+start_index)%ESC_DATA_SIZE]))*10;
	esc_sensor.voltage = (float)(data[(15+start_index)%ESC_DATA_SIZE])/10.0f;
	esc_sensor.current = ((uint16_t)(data[(16+start_index)%ESC_DATA_SIZE] <<8) | (data[(17+start_index)%ESC_DATA_SIZE]))/10.0;
	esc_sensor.temperature = (uint16_t)(data[(18+start_index)%ESC_DATA_SIZE] <<8) | (data[(19+start_index)%ESC_DATA_SIZE]);

	current_esc_speed = esc_sensor.rpm/parameters.esc_rpm_to_speed_ratio;
	esc_receive_indicator = 0;
	return HAL_OK;
}

//read speed data from f103
//the first data are "acsr", then the fifth is the integer part of speed1, and the sixth is the decimal part of speed1..
HAL_StatusTypeDef read_speed_data(uint8_t* data){
	uint8_t i;
	for(i = 0;i<2*SPEED_PIN_COUNT;++i){
		if(data[i]=='A' && data[i+1]=='C'&& data[i+2]=='S'&& data[i+3]=='R')break;
	}
	if(i==2*SPEED_PIN_COUNT)return HAL_ERROR;

	if(i==0){
		memcpy(speed,&data[4],2*SPEED_PIN_COUNT);
	}else{
		memcpy(speed,&data[i+4],2*SPEED_PIN_COUNT-i);
		memcpy(&speed[2*SPEED_PIN_COUNT-i],data,i);
	}
	f103_receive_indicator = 0;
	return HAL_OK;
}

//ros structure required
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart==&huart4){
		nh.getHardware()->flush();
	}
}

//uart data receive callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==huart4.Instance){
		nh.getHardware()->reset_rbuf();
	}else if(huart->Instance==huart_esc.Instance){
		read_ble_data(esc_receive);
	}else if(huart->Instance==huart_f103.Instance){
		read_speed_data(speed_receive);
	}else if(huart->Instance==huart_imu.Instance){
		if(jy901_data[0]==0x55 && jy901_data_length==11){
			jy901.parseData(jy901_data);
			HAL_UART_Receive_DMA(&huart8, jy901_data, jy901_data_length);

		}else if(jy901_data_length!=11){
			//jy901.parseData(jy901_data);
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

//uart receive callback for cp2102
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if (huart->Instance == UART7)
	{
		if(usb_buf[0]=='x' && usb_buf[1]=='i' && usb_buf[2]=='l' && usb_buf[3]=='i' && usb_buf[4]=='n'){
			HAL_UART_Transmit(&huart7, (uint8_t*)(&parameters) , sizeof(ParameterTypeDef), 10);
		}else if(usb_buf[0]=='a' && usb_buf[1]=='c' && usb_buf[2]=='s' && usb_buf[3]=='r'){
			uint32_t i = sizeof(ParameterTypeDef)-4;
			if(usb_buf[i]!='b' || usb_buf[i+1]!='4'|| usb_buf[i+2]!='0'|| usb_buf[i+3]!='1'){
				printf("Receive Wrong Data\n");
			}else{
				memcpy(&parameters,usb_buf,sizeof(ParameterTypeDef));
				printf("Write the Configuration Complete!\n");
				QSPI_W25Q64JV_Write((uint8_t*)(&parameters),0x0,sizeof(ParameterTypeDef));
				NVIC_SystemReset();
			}

		}else{
			printf("Receive Wrong Data\n");
			//HAL_UART_Transmit(&huart7,data, sizeof(data), 10);
		}
		HAL_UARTEx_ReceiveToIdle_DMA(&huart7, (uint8_t *) usb_buf, 100);
		__HAL_DMA_DISABLE_IT(&hdma_uart7_rx, DMA_IT_HT);
	}
}

//error handle of uart
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
    if(UartHandle->Instance==huart_esc.Instance) {
    	HAL_UART_Receive_DMA(&huart_esc, esc_receive, ESC_DATA_SIZE);
    }else if(UartHandle->Instance==huart_f103.Instance) {
    	HAL_UART_Receive_DMA(&huart_f103, (uint8_t*)speed_receive, 2*SPEED_PIN_COUNT+1);
    }
    else if(UartHandle->Instance==huart_imu.Instance) {
		HAL_UART_Receive_DMA(&huart_imu, (uint8_t*)jy901_data, 11);
	}else if(UartHandle->Instance==UART7){
		HAL_UARTEx_ReceiveToIdle_DMA(&huart7, (uint8_t *) usb_buf, 100);
		__HAL_DMA_DISABLE_IT(&hdma_uart7_rx, DMA_IT_HT);
	}
}

//pwm input capture callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){

	if(input_mode == INPUT_MODE_SOFTWARE)return;

	uint32_t ic_freq;
	if(htim->Instance==TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		ic_freq = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		uint32_t servo_ic = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		int32_t diff = servo_ic - pre_servo_ic;
		if(abs(diff) > 2){
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1,servo_ic);
			pre_servo_ic = servo_ic;
		}

		__HAL_TIM_SetCounter(htim,0);
		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);

	}else if(htim->Instance==TIM15 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){

		ic_freq = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		uint32_t esc_ic = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		int32_t diff = esc_ic - pre_esc_ic;
		if(abs(diff) >2){
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2,esc_ic);
			pre_esc_ic = esc_ic;
		}

		__HAL_TIM_SetCounter(htim,0);

		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);

	}
	int32_t diff = ic_freq-esc_servo_arr;
	if(esc_servo_arr<1000 || abs(diff)>50){
		esc_servo_arr=ic_freq;
		__HAL_TIM_SET_AUTORELOAD(&htim3,ic_freq);
	}


}

//timer callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	//ros publish
	if(htim->Instance==TIM16){
		memcpy(&sensor_msg.data[0],speed,2*wheel_speed_size);
		uint8_t index =2*wheel_speed_size;
		memcpy(&sensor_msg.data[index],force_raw,4*force_size);
		index+=4*force_size;
		memcpy(&sensor_msg.data[index],&esc_sensor, sizeof(esc_sensor));
		index+=sizeof(esc_sensor);
		memcpy(&sensor_msg.data[index],&jy901.JY901_data.acc,sizeof(jy901.JY901_data.acc));
		index+=sizeof(jy901.JY901_data.acc);
		memcpy(&sensor_msg.data[index],&jy901.JY901_data.gyro,sizeof(jy901.JY901_data.gyro));
		index+=sizeof(jy901.JY901_data.gyro);
		memcpy(&sensor_msg.data[index],&jy901.JY901_data.angle,sizeof(jy901.JY901_data.angle));
		index+=sizeof(jy901.JY901_data.angle);

		sensor_msg.data[index] = error_code;
		ros_pub.publish(&sensor_msg);
		nh.spinOnce();
	}else if(htim->Instance==TIM6)//pid computation
	{
		if(input_mode == INPUT_MODE_CONTROLLER || pid_mode == PID_MODE_MANUAL || (error_code & 0x01))return;
		pid_ptr->compute();
		set_esc_duty_cycle(pid_esc_duty_cycle_output);
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
	float esc_duty_cycle = parameters.esc_offset + fake_duty_cycle * (parameters.esc_max-parameters.esc_min) / 2.0;
	//convert duty cycle to pwm ccr
	int32_t esc_ccr = esc_duty_cycle * esc_servo_arr;
	if(abs(esc_ccr - pre_esc_ccr)>=parameters.esc_set_precision){
		pre_esc_ccr = esc_ccr;
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,esc_ccr);
	}
}

//set the duty cycle of the esc,
//input: steering_angle, unit is degree
void set_servo_duty_cycle(float steering_angle){
	float servo_duty_cycle = parameters.steering_to_dutycycle_ratio*steering_angle + parameters.steering_offset;

	//convert duty cycle to pwm ccr
	int32_t servo_ccr = servo_duty_cycle*esc_servo_arr;
	if(abs(servo_ccr - pre_servo_ccr)>=parameters.servo_set_precision){
		pre_servo_ccr = servo_ccr;
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,servo_ccr);
	}
}

//set the brake,
//input: brake pwm, value from 0-255
void set_brake(uint8_t* data){
	uint32_t c = data[0]*brake_arr/255;
	if(c!=pre_brake[0]){
		pre_brake[0]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,c);
	}
	c = data[1]*brake_arr/255;
	if(c!=pre_brake[1]){
		pre_brake[1]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,c);
	}
	c = data[2]*brake_arr/255;
	if(c!=pre_brake[2]){
		pre_brake[2]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,c);
	}
	c = data[3]*brake_arr/255;
	if(c!=pre_brake[3]){
		pre_brake[3]=c;
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
	HAL_UART_Receive_DMA(&huart_esc, esc_receive, ESC_DATA_SIZE);
	HAL_UART_Receive_DMA(&huart_f103, (uint8_t*)speed_receive, 2*SPEED_PIN_COUNT+4);
	HAL_UART_Receive_DMA(&huart8, jy901_data, 11);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart7, usb_buf, 100);
	__HAL_DMA_DISABLE_IT(&hdma_uart7_rx, DMA_IT_HT);
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
	esc_servo_arr = 1000000/parameters.steering_esc_pwm_frequency-1;
	__HAL_TIM_SET_AUTORELOAD(&htim3,esc_servo_arr);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,parameters.steering_offset);
	pre_servo_ccr = parameters.steering_offset;

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,parameters.esc_offset);
	pre_esc_ccr = parameters.esc_offset;

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	//set brake pwm, prescaler 99, timer frequency is 100MHz
	__HAL_TIM_SET_PRESCALER(&htim2,99);
	//set tim2 ARR value based on brake frequency and start tim6, 1000000 = 100M/(99+1)
	brake_arr = uint32_t(1000000/parameters.brake_pwm_frequency-1);
	__HAL_TIM_SET_AUTORELOAD(&htim2,brake_arr);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	//initial value zeros
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4,0);

	//pwm input capture for servo and esc
	HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
	HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
	HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);

	//start ros publish
	//this timer should the last one to start.
	HAL_TIM_Base_Start_IT(&htim16);

}

//ros brake subscriber callback
void command_callback(const std_msgs::UInt8MultiArray& msg){
	//data[0] : input mode
	HAL_GPIO_WritePin(Manual_Output_GPIO_Port, Manual_Output_Pin, (GPIO_PinState)msg.data[0]);

	//data[1] : pid mode
	if(msg.data[1] != (uint8_t)pid_mode){
		pid_mode = (PIDMode_TypeDef)msg.data[1];
		pid_ptr->reset();
		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, (GPIO_PinState)msg.data[1]);
	}

	//data[2]~data[5]:brake, brake is represent by a uint8 0-255 correponding dutycycle 0-1
	set_brake(&msg.data[2]);

	//other commands are ignored if input mode is mannual
	if(msg.data[0]){
		return;
	}

	//data[6]: speed integer part if PID_MODE_AUTOMATIC, the high byte of int16 if PID_MODE_MANNUAL
	//data[7]: speed decimal part if PID_MODE_AUTOMATIC, the low byte of int16 if PID_MODE_MANNUAL
	//duty cycle shall convert to 0-1 by divided by 1000
	if(pid_mode == PID_MODE_AUTOMATIC){
		pid_speed_set = msg.data[6] + msg.data[7]/100.0;
	}else{
		auto dc = (int16_t) (msg.data[6]<<8) | msg.data[7];
		set_esc_duty_cycle(dc/1000.0);
	}

	//data[8]: steering angle integer part
	//data[9]: steering angle decimal part
	float steering = msg.data[8] + msg.data[9]/100.0;
	set_servo_duty_cycle(steering);

}

ros::Subscriber<std_msgs::UInt8MultiArray> command_sub("Commands/stm32_cmd", &command_callback );

void ros_setup(){

	nh.initNode();
	nh.subscribe(command_sub);

	sensor_msg.data_length = 2*wheel_speed_size + 4*force_size + sizeof(esc_sensor) + sizeof(jy901.JY901_data.acc) + sizeof(jy901.JY901_data.gyro)+sizeof(jy901.JY901_data.angle)+1;
	sensor_msg.data = new std_msgs::UInt8MultiArray::_data_type[sensor_msg.data_length];

	//ros_pub = new ros::Publisher("stm32_sensor", &sensor_msg);
	nh.advertise(ros_pub);
	HAL_Delay(1000);
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
	    printf("Initializing ROM\n");
	}
	printf("Connect to ROM, ROM ID: [0x%02x,0x%02x]\n",id[0],id[1]);

	char header[4];
	QSPI_W25Q64JV_Read((uint8_t*)header, 0x00, 4);
	if(header[0]!='a' || header[1]!='c' || header[2]!='s' || header[3]!='r'){
		printf("Reading parameters fails, use default parameters\n");
		return;
	}

	QSPI_W25Q64JV_Read((uint8_t*)(&parameters), 0x00, sizeof(ParameterTypeDef));
	if(parameters.tailer[0]!='b' || parameters.tailer[1]!='4' || parameters.tailer[2]!='0' || parameters.tailer[3]!='1'){
		printf("Reading parameters fails, use default parameters\n");
		return;
	}
}

void reset_pid(){
	//reset pid class
	if(pid_ptr)delete pid_ptr;
	pid_ptr = new PID<float>(&current_esc_speed,&pid_esc_duty_cycle_output,&pid_speed_set,parameters.kp,parameters.ki,parameters.kd);
	pid_ptr->set_output_limits(-1,+1);
}


void setup(void)
{
	DWT_Init();
	read_parameters();
	reset_pid();
	uart_setup();
	adc_setup();
	ros_setup();
	timer_setup();
	HAL_GPIO_WritePin(Manual_Output_GPIO_Port, Manual_Output_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ONBOARD_LED_GPIO_Port, ONBOARD_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, (GPIO_PinState)pid_mode);
}

uint8_t loop_index = 0;

void loop(void)
{
	auto state = HAL_GPIO_ReadPin(Manual_Input_GPIO_Port, Manual_Input_Pin);

	if(state==GPIO_PIN_SET){
		input_mode = INPUT_MODE_CONTROLLER;
	}else if(state==GPIO_PIN_RESET){
		input_mode = INPUT_MODE_SOFTWARE;
	}
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, state);

	loop_index++;

	if(esc_receive_indicator <=5 )esc_receive_indicator++;
	if(f103_receive_indicator<=5)f103_receive_indicator++;

	HAL_Delay(100);
	if(loop_index==5){
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		loop_index=0;

		if(esc_receive_indicator>5)error_code = error_code | 0b00000001;
		else error_code = error_code & 0b11111110;

		if(f103_receive_indicator>5)error_code = error_code | 0b00000010;
		else error_code = error_code & 0b11111101;


	}
	HAL_IWDG_Refresh(&hiwdg1);
}

