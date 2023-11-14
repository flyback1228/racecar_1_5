/*
 * main.cpp

 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */
#include <mainpp.h>
#include <ros.h>
//#include <std_msgs/String.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <vesc_msgs/VescState.h>
#include "main.h"
#include "stm32h7xx_hal_rcc.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim15;

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart3;

#define huart_ros huart4
#define huart_esc hlpuart1
#define huart_f103 huart5
#define huart_usb huart7
#define huart_bno huart3


ros::NodeHandle nh;

//std_msgs::String str_msg;
vesc_msgs::VescState vesc_state;
std_msgs::Float32MultiArray forces;
std_msgs::Float32MultiArray wheel_speed;

ros::Publisher vesc_pub("vesc_sensor", &vesc_state);
ros::Publisher force_pub("forces", &forces);
ros::Publisher wheel_speed_pub("forces", &wheel_speed);

ESC_SensorTypeDef esc_sensor;
uint8_t esc_receive[ESC_DATA_SIZE];

const uint32_t acsr = ('A'<<24) | ('C'<<16) | ('S'<<8) | 'R';
uint32_t speed_receive[SPEED_PIN_COUNT+1];
uint32_t speed[SPEED_PIN_COUNT];

//int32_t freq;
//uint16_t servo_duty,esc_duty;
//uint32_t triangle1[32];
//uint32_t triangle2[32];

uint32_t TIMER_CLOCK_FREQ;
uint32_t _index = 0;
uint8_t pwm_generator_indicator,pre_pwm_generator_indicator;
uint8_t is_frequency_set;
//float duty_cycle = 0.5,voltage = 0.1,current = 0.2 ,speed = 1000.0, temperature=24.5;

pid_mode_enum pid_mode = PID_Manual;
uint8_t pid_its;

float esc_duty_cycle_set;
float speed_set;

float duty_cycle_output;

uint32_t pre_steering_pulse=0;
uint32_t pre_esc_pulse=0;
//float kp = 2.0;
//float ki = 1.0;
//float kd = 0.0;

ParameterTypeDef paramters ={
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
		.esc_min = 1250


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
		force_pub.publish(&forces);
		nh.spinOnce();
	}else if(htim->Instance==TIM7){
		//no esc topic received
		if(pid_its++>10){
			pid_its=10;
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,paramters.esc_offset);
			return;
		}
		if(pid_mode==PID_Manual){
			uint32_t esc_pulse=esc_duty_cycle_set*(paramters.esc_max-paramters.esc_offset)+paramters.esc_offset;
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
	pid_mode = PID_Auto;
	pid_its=0;
	speed_set=msg.data;
}

void duty_cycle_callback(const std_msgs::Float32& msg){
	pid_mode=PID_Manual;
	pid_its=0;
	esc_duty_cycle_set = msg.data;
}

void steering_callback(const std_msgs::Float32& msg){
	uint32_t steering_pulse = paramters.steering_ratio*(msg.data-paramters.steering_offset);
	if(steering_pulse != pre_steering_pulse){
		pre_steering_pulse = steering_pulse;
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,pre_steering_pulse);
	}

}


void uart_setup(){
	HAL_UART_Receive_DMA(&huart_esc, esc_receive, ESC_DATA_SIZE);
	HAL_UART_Receive_DMA(&huart_f103, (uint8_t*)speed_receive, sizeof(uint32_t)*(SPEED_PIN_COUNT+1));
}

void timer_setup(){
	//set tim6 ARR value based on topic publish frequency and start tim6
	__HAL_TIM_SET_AUTORELOAD(&htim6,uint32_t(10000/paramters.publish_frequency-1));
	HAL_TIM_Base_Start_IT(&htim6);

	//set tim7 ARR value based on PID calculation frequency and start tim7
	__HAL_TIM_SET_AUTORELOAD(&htim7,uint32_t(10000/paramters.pid_frequency-1));
	HAL_TIM_Base_Start_IT(&htim7);

	//start esc and steering servo pwm output
	__HAL_TIM_SET_AUTORELOAD(&htim3,uint32_t(1000000/paramters.steering_esc_pwm_frequency-1));
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,paramters.steering_offset);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,paramters.esc_offset);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}


ros::Subscriber<std_msgs::Float32> speed_sub("Commands/speed", &speed_callback );
ros::Subscriber<std_msgs::Float32> duty_cycle_sub("Commands/duty_cycle", &duty_cycle_callback );
ros::Subscriber<std_msgs::Float32> steering_sub("Commands/steering", &steering_callback );

void ros_setup(){
	forces.data = new std_msgs::Float32MultiArray::_data_type[8];
	forces.data_length = 8;


	wheel_speed.data = new std_msgs::Float32MultiArray::_data_type[16];
	wheel_speed.data_length = 16;



	nh.advertise(vesc_pub);
	nh.advertise(force_pub);
	nh.advertise(wheel_speed_pub);
}


void setup(void)
{
  nh.initNode();
  uart_setup();
  timer_setup();
  ros_setup();

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
  TIMER_CLOCK_FREQ = clock/(TIM5->PSC+1);

  HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1 );
  HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_2 );

//  HAL_TIM_PWM_

  HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
  HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);

  HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
  HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);

//  HAL_TIM_Base_Start_IT(&htim6);



}



void loop(void)
{
//  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

//  str_msg.data = hello;
//  chatter.publish(&str_msg);

  vesc_state.duty_cycle = esc_sensor.throttle/100.0;
  vesc_state.voltage_input = esc_sensor.voltage;
  vesc_state.current_input = esc_sensor.current;
  vesc_state.temperature_pcb = esc_sensor.temperature;
  vesc_state.speed = esc_sensor.rpm;


	_index=(_index+1)%32;
//	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1,triangle1[_index]);
//	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2,triangle2[_index]);


  for(int i=2;i<8;++i){
	  forces.data[i] = 0.1*i;
  }
  for(int i=0;i<16;++i){
  	  wheel_speed.data[i] = 10*i;
  }

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

  HAL_Delay(1000);
}

