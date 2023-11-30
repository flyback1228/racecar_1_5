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

#include <vesc_msgs/VescState.h>
#include "main.h"
#include "stm32h7xx_hal_rcc.h"
#include "w25q64jv.h"
#include "string.h"
#include <cstdio>
#include <pid.h>

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim15;

//communicate with ESC
extern UART_HandleTypeDef hlpuart1;

//communicate with NVIDIA Jetson, used as ROS port
extern UART_HandleTypeDef huart4;

//communicate with STM32F103
extern UART_HandleTypeDef huart5;

//to cp2102, usb port
extern UART_HandleTypeDef huart7;

//communicate with BNO085
extern UART_HandleTypeDef huart3;

extern DMA_HandleTypeDef hdma_uart7_rx;

extern ADC_HandleTypeDef hadc1;

#define huart_ros huart4
#define huart_esc hlpuart1
#define huart_f103 huart5
#define huart_usb huart7
#define huart_bno huart3


ros::NodeHandle nh;

//esc message and publisher
vesc_msgs::VescState vesc_state;
ros::Publisher vesc_pub("vesc_sensor", &vesc_state);

//force message and publisher
std_msgs::Float32MultiArray forces;
ros::Publisher force_pub("forces", &forces);

//wheel speed message and publisher
std_msgs::Float32MultiArray wheel_speed;
ros::Publisher wheel_speed_pub("forces", &wheel_speed);


ESC_SensorTypeDef esc_sensor;
uint8_t esc_receive[ESC_DATA_SIZE];


const uint32_t acsr = ('A'<<24) | ('C'<<16) | ('S'<<8) | 'R';//speed head
uint32_t speed_receive[SPEED_PIN_COUNT+1];//speed data from STM32F103, the first element is acsr
uint32_t speed[SPEED_PIN_COUNT];//speed frequency

uint8_t pwm_generator_indicator,pre_pwm_generator_indicator;
uint8_t is_frequency_set;
uint16_t force_raw[8];

PIDMode_TypeDef pid_mode = PID_MODE_MANUAL;
uint8_t pid_its;

uint8_t usb_buf[100];

float esc_duty_cycle_set;
float speed_set;

float duty_cycle_output;
//bool input_mode;

uint32_t tim2_arr;

uint32_t pre_steering_pulse=0;
uint32_t pre_esc_pulse=0;
uint32_t pre_brake[]={0,0,0,0};

ParameterTypeDef parameters ={
		.header={'a','c','s','r'},
		.kp = 2.0,
		.ki = 1.0,
		.kd = 0.0,
		.publish_frequency = 20,
		.pid_frequency = 10,
		.steering_esc_pwm_frequency = 64.5,
		.steering_offset=1500,
		.steering_ratio=1.0/1000,
		.steering_max = 17.0*3.14159/180,
		.steering_min = -17.0*3.14159/180,

		.esc_offset=1500,
		.esc_max = 1750,
		.esc_min = 1250,

		.tailer={'b','4','0','1'}

};

//uint8_t publish_frequency = 20;
//uint8_t pid_frequency = 10;



//char hello[] = "Hello world from STM32!";


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

	return HAL_OK;
}

HAL_StatusTypeDef read_speed_data(uint32_t* data){
	uint8_t i;
	for(i = 0;i<SPEED_PIN_COUNT+1;++i){
		if(data[i]==acsr)break;
	}
	if(i==SPEED_PIN_COUNT+1)return HAL_ERROR;

	memcpy(speed,&data[i+1],(SPEED_PIN_COUNT-i)*sizeof(uint32_t));
	if(i){
		memcpy(&speed[SPEED_PIN_COUNT-i],data,i*sizeof(uint32_t));
	}
	return HAL_OK;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart==&huart4){
		nh.getHardware()->flush();
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart==&huart4){
		nh.getHardware()->reset_rbuf();
	}else if(huart==&huart_esc){
		read_ble_data(esc_receive);
	}else if(huart==&huart_f103){
		read_speed_data(speed_receive);
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if (huart->Instance == UART7)
	{
		if(usb_buf[0]=='x' && usb_buf[1]=='i' && usb_buf[2]=='l' && usb_buf[3]=='i' && usb_buf[4]=='n'){
			HAL_UART_Transmit(&huart7, (uint8_t*)(&parameters) , sizeof(ParameterTypeDef), 10);
		}else if(usb_buf[0]=='a' && usb_buf[1]=='c' && usb_buf[2]=='s' && usb_buf[3]=='r'){
			uint32_t i = sizeof(ParameterTypeDef)-4;
			if(usb_buf[i]!='b' || usb_buf[i+1]!='4'|| usb_buf[i+2]!='0'|| usb_buf[i+3]!='1'){
				uint8_t data[]= "Receive Wrong Data\n";
				HAL_UART_Transmit(&huart7,data, sizeof(data), 10);
			}else{
				memcpy(&parameters,usb_buf,sizeof(ParameterTypeDef));
				uint8_t data[]= "Write the Configuration Complete!\n";
				HAL_UART_Transmit(&huart7,data, sizeof(data), 10);
				QSPI_W25Q64JV_Write((uint8_t*)(&parameters),0x0,sizeof(ParameterTypeDef));

			}

		}else{
			uint8_t data[]= "Receive Wrong Data\n";
			HAL_UART_Transmit(&huart7,data, sizeof(data), 10);
		}
		HAL_UARTEx_ReceiveToIdle_DMA(&huart7, (uint8_t *) usb_buf, 100);
		__HAL_DMA_DISABLE_IT(&hdma_uart7_rx, DMA_IT_HT);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
    if(UartHandle->Instance==LPUART1) {
    	HAL_UART_Receive_DMA(&huart_esc, esc_receive, ESC_DATA_SIZE);
    }else if(UartHandle->Instance==UART5) {
    	HAL_UART_Receive_DMA(&huart5, (uint8_t*)speed_receive, sizeof(uint32_t)*(SPEED_PIN_COUNT+1));
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){

//	if(!pwm_generator_indicator)return;
//
//	int32_t temp_freq;
//	if(htim->Instance==TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
//
//		temp_freq = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
//		servo_duty = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
//
//		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1,servo_duty);
//		__HAL_TIM_SetCounter(htim,0);
//
//		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
//		HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
//
//	}else if(htim->Instance==TIM15 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
//
//		temp_freq = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
//		esc_duty = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
//
//		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2,esc_duty);
//		__HAL_TIM_SetCounter(htim,0);
//
//		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
//		HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);
//
//	}
//	if(freq<1000 || abs(temp_freq-freq)>50){
//		freq=temp_freq;
//		__HAL_TIM_SET_AUTORELOAD(&htim3,temp_freq);
//	}


}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
//	if(htim->Instance==TIM3){
//		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1,servo_duty);
//		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2,esc_duty);
//
//	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance==TIM6){
		vesc_state.duty_cycle = esc_sensor.throttle/100.0;
		vesc_state.voltage_input = esc_sensor.voltage;
		vesc_state.current_input = esc_sensor.current;
		vesc_state.temperature_pcb = esc_sensor.temperature;
		vesc_state.speed = esc_sensor.rpm;

		vesc_pub.publish(&vesc_state);
		wheel_speed_pub.publish(&wheel_speed);

		for(uint8_t i=0;i<8;++i){
			forces.data[i]=(float)force_raw[i]*3.3/0xFFFF;
		}
		force_pub.publish(&forces);

		nh.spinOnce();
	}else if(htim->Instance==TIM7){
		//no esc topic received
		if(pid_its++>10){
			pid_its=10;
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,parameters.esc_offset);
			return;
		}

		if(pid_mode==PID_MODE_MANUAL){
			uint32_t esc_pulse=esc_duty_cycle_set*(parameters.esc_max-parameters.esc_offset)+parameters.esc_offset;
			//if(pre_esc_pulse==esc_pulse) no action needed.
			if(pre_esc_pulse!=esc_pulse){
				pre_esc_pulse=esc_pulse;
				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,pre_esc_pulse);
			}
		}else{
			//apply pid
		}

	}

//	HAL_TIM_IC_Start_DMA(htim, Channel, pData, Length)
//	if(htim->Instance==TIM5){
//		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
//		HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
//	}
//	if(htim->Instance==TIM15){
//		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
//		HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);
//	}
}



void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim){
	/*
	if(htim->Instance==TIM5){
		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
	}
	if(htim->Instance==TIM15){
		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);
	}*/
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	switch(GPIO_Pin){
	case GPIO_PIN_5:
		pwm_generator_indicator = !pwm_generator_indicator;
		if(!pwm_generator_indicator){
//			freq=0;
			HAL_TIM_IC_Stop_IT(&htim5, TIM_CHANNEL_1);
			HAL_TIM_IC_Stop(&htim5, TIM_CHANNEL_2);
			HAL_TIM_IC_Stop_IT(&htim15, TIM_CHANNEL_1);
			HAL_TIM_IC_Stop(&htim15, TIM_CHANNEL_2);

			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
		}else{
			HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
			HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
			HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
			HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);

			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
		}
		break;
	default:
		break;
	}
}

void speed_callback(const std_msgs::Float32& msg){
	pid_mode = PID_MODE_AUTOMATIC;
	pid_its=0;
	speed_set=msg.data;
}

void duty_cycle_callback(const std_msgs::Float32& msg){
	pid_mode=PID_MODE_MANUAL;
	pid_its=0;
	esc_duty_cycle_set = msg.data;
}

void steering_callback(const std_msgs::Float32& msg){
	uint32_t steering_pulse = parameters.steering_ratio*(msg.data-parameters.steering_offset);
	if(steering_pulse != pre_steering_pulse){
		pre_steering_pulse = steering_pulse;
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,pre_steering_pulse);
	}

}

void brake_callback(const std_msgs::Float32MultiArray& msg){
	uint32_t c = msg.data[0]*tim2_arr;
	if(c!=pre_brake[0]){
		pre_brake[0]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,c);
	}
	c = msg.data[1]*tim2_arr;
	if(c!=pre_brake[1]){
		pre_brake[1]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,c);
	}
	c = msg.data[2]*tim2_arr;
	if(c!=pre_brake[2]){
		pre_brake[2]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,c);
	}
	c = msg.data[3]*tim2_arr;
	if(c!=pre_brake[3]){
		pre_brake[3]=c;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4,c);
	}
}

void input_mode_callback(const std_msgs::Bool& msg){
	HAL_GPIO_WritePin(Manual_Output_GPIO_Port, Manual_Output_Pin, (GPIO_PinState)msg.data);
}

void adc_setup(){
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)force_raw,8);
}

void uart_setup(){
	HAL_UART_Receive_DMA(&huart_esc, esc_receive, ESC_DATA_SIZE);
	HAL_UART_Receive_DMA(&huart_f103, (uint8_t*)speed_receive, sizeof(uint32_t)*(SPEED_PIN_COUNT+1));

	HAL_UARTEx_ReceiveToIdle_DMA(&huart7, usb_buf, 100);
	__HAL_DMA_DISABLE_IT(&hdma_uart7_rx, DMA_IT_HT);
}

void timer_setup(){
	//set tim6 ARR value based on topic publish frequency and start tim6
	__HAL_TIM_SET_AUTORELOAD(&htim16,uint32_t(10000/parameters.publish_frequency-1));
	HAL_TIM_Base_Start_IT(&htim16);

	//set tim7 ARR value based on PID calculation frequency and start tim7
	__HAL_TIM_SET_AUTORELOAD(&htim7,uint32_t(10000/parameters.pid_frequency-1));
	HAL_TIM_Base_Start_IT(&htim7);

	//start esc and steering servo pwm output
	__HAL_TIM_SET_AUTORELOAD(&htim3,uint32_t(1000000/parameters.steering_esc_pwm_frequency-1));
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,parameters.steering_offset);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,parameters.esc_offset);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	//set brake pwm
	tim2_arr = uint32_t(1000000/parameters.brake_pwm_frequency-1);
	__HAL_TIM_SET_AUTORELOAD(&htim2,tim2_arr);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4,0);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

}


ros::Subscriber<std_msgs::Float32> speed_sub("Commands/speed", &speed_callback );
ros::Subscriber<std_msgs::Float32> duty_cycle_sub("Commands/duty_cycle", &duty_cycle_callback );
ros::Subscriber<std_msgs::Float32> steering_sub("Commands/steering", &steering_callback );
ros::Subscriber<std_msgs::Float32MultiArray> brake_sub("Commands/brakes", &brake_callback );
ros::Subscriber<std_msgs::Bool> input_mode_sub("Commands/input_mode", &input_mode_callback );

void ros_setup(){
	forces.data = new std_msgs::Float32MultiArray::_data_type[8];
	forces.data_length = 8;


	wheel_speed.data = new std_msgs::Float32MultiArray::_data_type[16];
	wheel_speed.data_length = 16;



	nh.advertise(vesc_pub);
	nh.advertise(force_pub);
	nh.advertise(wheel_speed_pub);
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
	    while (1);
	}
	char str[]="Connect to ROM, ROM ID: [0x00,0x00]\n";
	sprintf(str,"Connect to ROM, ROM ID: [0x%02x,0x%02x]\n",id[0],id[1]);
	HAL_UART_Transmit(&huart7, (uint8_t*)str, sizeof(str), 10);

	char header[4];
	QSPI_W25Q64JV_Read((uint8_t*)header, 0x00, 4);
	if(header[0]!='a' || header[1]!='c' || header[2]!='s' || header[3]!='r'){
		char str[]="Read Parameters Head Fails\n";
		HAL_UART_Transmit(&huart7, (uint8_t*)header, 4, 10);
		HAL_UART_Transmit(&huart7, (uint8_t*)str, sizeof(str), 10);
		return;
	}

	QSPI_W25Q64JV_Read((uint8_t*)(&parameters), 0x00, sizeof(ParameterTypeDef));
	if(parameters.tailer[0]!='b' || parameters.tailer[1]!='4' || parameters.tailer[2]!='0' || parameters.tailer[3]!='1'){
		char str[]="Read Parameters Tailor Fails\n";
		HAL_UART_Transmit(&huart7, (uint8_t*)parameters.tailer, 4, 10);
		HAL_UART_Transmit(&huart7, (uint8_t*)str, sizeof(str), 10);
	}

//	QSPI_W25Q64JV_Write((uint8_t*)(),0x0,2*Font_7x10.size);

}


void setup(void)
{

	read_parameters();
  nh.initNode();
  uart_setup();
  timer_setup();
  ros_setup();

//  input_mode = true;


//  HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_2 );







  uint16_t inc = TIM4->ARR/38;

//  for(int i=0;i<32;++i){
//	  triangle1[i] = (i+1)*inc;
//  }
//  inc*=2;
//  for(int i=0;i<16;++i){
//    triangle2[i] = (i+1)*inc;
//  }
//  for(int i=16;i<32;++i){
//    triangle2[i] = (33-(i+1))*inc;
//  }

  pwm_generator_indicator = 0;
  is_frequency_set = 0;
  uint32_t clock = HAL_RCC_GetPCLK1Freq();
//  TIMER_CLOCK_FREQ = clock/(TIM5->PSC+1);
//
//  HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1 );
//  HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_2 );

//  HAL_TIM_PWM_

  HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
  HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);

  HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
  HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);



  HAL_GPIO_WritePin(ONBOARD_LED_GPIO_Port, ONBOARD_LED_Pin, GPIO_PIN_SET);



}

uint8_t loop_index = 0;

void loop(void)
{
	auto state = HAL_GPIO_ReadPin(Manual_Input_GPIO_Port, Manual_Input_Pin);

	if(state==GPIO_PIN_SET){
		pid_mode=PID_MODE_MANUAL;
	}else if(state==GPIO_PIN_RESET){
		pid_mode=PID_MODE_AUTOMATIC;
	}
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, state);

	loop_index++;
	HAL_Delay(50);
	if(loop_index==10){
		HAL_GPIO_TogglePin(ONBOARD_LED_GPIO_Port, ONBOARD_LED_Pin);
	}

//  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

//  str_msg.data = hello;
//  chatter.publish(&str_msg);

//  vesc_state.duty_cycle = esc_sensor.throttle/100.0;
//  vesc_state.voltage_input = esc_sensor.voltage;
//  vesc_state.current_input = esc_sensor.current;
//  vesc_state.temperature_pcb = esc_sensor.temperature;
//  vesc_state.speed = esc_sensor.rpm;
//
//
//	_index=(_index+1)%32;
////	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1,triangle1[_index]);
////	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2,triangle2[_index]);
//
//
//  for(int i=2;i<8;++i){
//	  forces.data[i] = 0.1*i;
//  }
//  for(int i=0;i<16;++i){
//  	  wheel_speed.data[i] = 10*i;
//  }

//  forces.data[0] = freq;
//  forces.data[1] = servo_duty;
//
//
//  forces.data[2] = freq;
//  forces.data[3] = esc_duty;
//
//  forces.data[4]=TIMER_CLOCK_FREQ;
//  forces.data[5]=pwm_generator_indicator;



//  freq1=0;
//  duty1=0;
//  freq2=1;
//  duty2=1;

}

