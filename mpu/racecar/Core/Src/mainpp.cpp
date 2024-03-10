/*
 * mainpp.cpp

 *
 *  Created on: 2023/11/17
 *      Author: xli185
 */
#include <mainpp.h>
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>


#include "main.h"
#include "stm32h7xx_hal_rcc.h"
//#include "w25q64jv.h"
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
std_msgs::Float32MultiArray sensor_msg;
ros::Publisher ros_pub("stm32_sensor", &sensor_msg);

//esc senso data
ESC_SensorTypeDef esc_sensor;
uint8_t esc_receive[ESC_DATA_SIZE];

const uint8_t wheel_speed_size=SPEED_PIN_COUNT;
const uint8_t force_size = 8;
const uint8_t vesc_size = 5;
const uint8_t imu_size = 9;

uint8_t error_code = 0;
uint8_t esc_receive_indicator = 0;
uint8_t f103_receive_indicator = 0;

//speed data from STM32F103, the first four elements are "acsr"
uint8_t speed_receive[2*SPEED_PIN_COUNT+4];
//speed, represent by frequency
float speed[SPEED_PIN_COUNT];

//uint8_t pwm_generator_indicator,pre_pwm_generator_indicator;
uint8_t is_frequency_set;
uint32_t force_raw[8];

//PIDMode_TypeDef
PIDMode_TypeDef pid_mode = PID_MODE_MANUAL;

uint8_t pid_its;
float esc_duty_cycle_set;
float speed_set = 0.0;
float duty_cycle_output = 0.0;
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


uint32_t servo_duty = 0;
uint32_t esc_duty = 0;
uint32_t input_freq;

uint8_t jy901_data_length = 11;
uint8_t jy901_data[11];
CJY901 jy901(&huart8);

ParameterTypeDef parameters ={
		.header={'a','c','s','r'},
		.kp = 2.0,
		.ki = 1.0,
		.kd = 0.0,
		.publish_frequency = 20,
		.pid_frequency = 10,

		.esc_rpm_to_speed_ratio = 100.0,

		.steering_esc_pwm_frequency = 64.5,
		.steering_offset=1500,
		.steering_ratio=100.0,
		.steering_max = 17.0*3.14159/180,
		.steering_min = -17.0*3.14159/180,

		.esc_offset=1500,
		.esc_max = 1750,
		.esc_min = 1250,

		.brake_pwm_frequency=100,

		.tailer={'b','4','0','1'}
};

PID<float> pid(&current_esc_speed,&duty_cycle_output,&speed_set,parameters.kp,parameters.ki,parameters.kd);

//uint8_t publish_frequency = 20;
//uint8_t pid_frequency = 10;


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

	for(int j=0;j<SPEED_PIN_COUNT;++j){
		speed[j] = data[(2*j+i+4)%(2*SPEED_PIN_COUNT+4)]+data[(2*j+1+i+4)%(2*SPEED_PIN_COUNT+4)]/100.0;
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

	uint32_t temp_freq;
	if(htim->Instance==TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		temp_freq = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		uint32_t temp_servo_duty = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		int32_t diff = temp_servo_duty - servo_duty;
		if(abs(diff) > 2){
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1,temp_servo_duty);
			servo_duty = temp_servo_duty;
		}

		__HAL_TIM_SetCounter(htim,0);
		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);

	}else if(htim->Instance==TIM15 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){

		temp_freq = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		uint32_t temp_esc_duty = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		int32_t diff = temp_esc_duty - esc_duty;
		if(abs(diff) >2){
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2,temp_esc_duty);
			esc_duty = temp_esc_duty;
		}

		__HAL_TIM_SetCounter(htim,0);

		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);

	}
	int32_t diff = temp_freq-input_freq;
	if(input_freq<1000 || abs(diff)>50){
		input_freq=temp_freq;
		__HAL_TIM_SET_AUTORELOAD(&htim3,temp_freq);
		esc_servo_arr = 1000000/input_freq-1;
	}


}

//timer callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	//ros publish
	if(htim->Instance==TIM16){
		memcpy(&sensor_msg.data[0],speed,wheel_speed_size*sizeof(float));
		for(uint8_t i=0;i<force_size;++i){
			sensor_msg.data[wheel_speed_size+i]=(float)force_raw[i]*3.3/0xFFFF;
		}
		sensor_msg.data[wheel_speed_size+force_size]=esc_sensor.throttle/100.0;
		sensor_msg.data[wheel_speed_size+force_size + 1] = esc_sensor.voltage;
		sensor_msg.data[wheel_speed_size+force_size + 2] = esc_sensor.current;
		sensor_msg.data[wheel_speed_size+force_size + 3] = (float)esc_sensor.temperature;
		sensor_msg.data[wheel_speed_size+force_size + 4] = (float)esc_sensor.rpm;

		sensor_msg.data[wheel_speed_size + force_size + vesc_size + 0] = jy901.getAccX();
		sensor_msg.data[wheel_speed_size + force_size + vesc_size + 1] = jy901.getAccY();
		sensor_msg.data[wheel_speed_size + force_size + vesc_size + 2] = jy901.getAccZ();
		sensor_msg.data[wheel_speed_size + force_size + vesc_size + 3] = jy901.getGyroX();
		sensor_msg.data[wheel_speed_size + force_size + vesc_size + 4] = jy901.getGyroY();
		sensor_msg.data[wheel_speed_size + force_size + vesc_size + 5] = jy901.getGyroZ();
		sensor_msg.data[wheel_speed_size + force_size + vesc_size + 6] = jy901.getRoll();
		sensor_msg.data[wheel_speed_size + force_size + vesc_size + 7] = jy901.getPitch();
		sensor_msg.data[wheel_speed_size + force_size + vesc_size + 8] = jy901.getYaw();

		ros_pub.publish(&sensor_msg);
		nh.spinOnce();
	}else if(htim->Instance==TIM6)//pid computation
	{
		if(input_mode == INPUT_MODE_CONTROLLER || pid_mode == PID_MODE_MANUAL)return;

		/*
		uint32_t esc_pulse=esc_duty_cycle_set*(parameters.esc_max-parameters.esc_offset)+parameters.esc_offset;

		if(pre_esc_pulse!=esc_pulse){
			pre_esc_pulse=esc_pulse;
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,pre_esc_pulse);
		}*/


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

//ros speed subscriber callback
void speed_callback(const std_msgs::Float32& msg){
	if(input_mode == INPUT_MODE_CONTROLLER || pid_mode == PID_MODE_MANUAL)return;
	pid_its=0;
	speed_set=msg.data;
}

//ros duty cycle subscriber callback
void duty_cycle_callback(const std_msgs::Float32& msg){
	if(input_mode == INPUT_MODE_CONTROLLER || pid_mode == PID_MODE_AUTOMATIC)return;
	duty_cycle_output = msg.data;
	int32_t esc_count = duty_cycle_output * esc_servo_arr;
	if(abs(esc_count - pre_esc_ccr)>5){
		pre_esc_ccr = esc_count;
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,esc_count);
	}
}

//ros steering subscriber callback
void steering_callback(const std_msgs::Float32& msg){
	if(input_mode == INPUT_MODE_CONTROLLER)return;
	int32_t steering_pulse = parameters.steering_ratio*msg.data + parameters.steering_offset;
	if(abs(steering_pulse - pre_servo_ccr)>5){
		pre_servo_ccr = steering_pulse;
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,steering_pulse);
	}
}

//ros brake subscriber callback
void brake_callback(const std_msgs::Float32MultiArray& msg){
	uint32_t c = msg.data[0]*brake_arr;
	if(c!=pre_brake[0]){
		pre_brake[0]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,c);
	}
	c = msg.data[1]*brake_arr;
	if(c!=pre_brake[1]){
		pre_brake[1]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,c);
	}
	c = msg.data[2]*brake_arr;
	if(c!=pre_brake[2]){
		pre_brake[2]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,c);
	}
	c = msg.data[3]*brake_arr;
	if(c!=pre_brake[3]){
		pre_brake[3]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4,c);
	}
}

void input_mode_callback(const std_msgs::Bool& msg){
	HAL_GPIO_WritePin(Manual_Output_GPIO_Port, Manual_Output_Pin, (GPIO_PinState)msg.data);

}

void pid_mode_callback(const std_msgs::Bool& msg){
	if(msg.data != (bool)pid_mode){
		pid_mode = (PIDMode_TypeDef)msg.data;
		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, (GPIO_PinState)msg.data);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_9) {
	  auto state = HAL_GPIO_ReadPin(PID_MODE_INPUT_GPIO_Port, PID_MODE_INPUT_Pin);
	  if(state==GPIO_PIN_SET){
		  pid_mode = PID_MODE_AUTOMATIC;
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
	__HAL_TIM_SET_PRESCALER(&htim6,9999);
	__HAL_TIM_SET_AUTORELOAD(&htim6,uint32_t(10000/parameters.pid_frequency-1));
	HAL_TIM_Base_Start_IT(&htim6);

	//start esc and steering servo pwm output
	input_freq = parameters.steering_esc_pwm_frequency;
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
	//this timer should start as the last one.
	HAL_TIM_Base_Start_IT(&htim16);


}


ros::Subscriber<std_msgs::Float32> speed_sub("Commands/speed", &speed_callback );
ros::Subscriber<std_msgs::Float32> duty_cycle_sub("Commands/duty_cycle", &duty_cycle_callback );
ros::Subscriber<std_msgs::Float32> steering_sub("Commands/steering", &steering_callback );
ros::Subscriber<std_msgs::Float32MultiArray> brake_sub("Commands/brakes", &brake_callback );
ros::Subscriber<std_msgs::Bool> input_mode_sub("Commands/input_mode", &input_mode_callback );
ros::Subscriber<std_msgs::Bool> pid_mode_sub("Commands/pid_mode", &pid_mode_callback );

void ros_setup(){

	//setup the sensor message, the first 16 data are speed, next 8 data are force, next
	sensor_msg.data_length = wheel_speed_size + force_size + vesc_size + imu_size;
	sensor_msg.data = new std_msgs::Float32MultiArray::_data_type[sensor_msg.data_length];

	nh.initNode();

	nh.subscribe(speed_sub);
	nh.subscribe(input_mode_sub);
	nh.subscribe(brake_sub);
	nh.subscribe(steering_sub);
	nh.subscribe(duty_cycle_sub);
	nh.subscribe(pid_mode_sub);

	nh.advertise(ros_pub);
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


void setup(void)
{
	DWT_Init();
	read_parameters();
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

