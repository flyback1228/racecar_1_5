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
#include <vesc_msgs/VescState.h>
#include "main.h"
#include "stm32h7xx_hal_rcc.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim15;

ros::NodeHandle nh;

//std_msgs::String str_msg;
vesc_msgs::VescState vesc_state;
std_msgs::Float32MultiArray forces;
std_msgs::Float32MultiArray wheel_speed;


//ros::Publisher chatter("chatter", &str_msg);
ros::Publisher vesc_pub("vesc_sensor", &vesc_state);
ros::Publisher force_pub("forces", &forces);
ros::Publisher wheel_speed_pub("forces", &wheel_speed);

ESC_SensorTypeDef esc_sensor;

float freq,servo_duty,esc_duty;
uint32_t triangle1[32];
uint32_t triangle2[32];

uint32_t TIMER_CLOCK_FREQ;
uint32_t _index = 0;
//float duty_cycle = 0.5,voltage = 0.1,current = 0.2 ,speed = 1000.0, temperature=24.5;



//char hello[] = "Hello world from STM32!";

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart==&huart4){
		nh.getHardware()->flush();
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart==&huart4){
		nh.getHardware()->reset_rbuf();
	}

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance==TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		uint32_t cl = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		uint32_t ch = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);


		freq = (float) TIMER_CLOCK_FREQ / (cl + 1);
		servo_duty = (float) 100 * ch / cl;
		__HAL_TIM_SetCounter(htim,0);
		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
	}else if(htim->Instance==TIM15 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		uint32_t cl = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		uint32_t ch = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SetCounter(htim,0);
		freq = (float) TIMER_CLOCK_FREQ / (cl + 1);
		esc_duty = (float) 100 * ch / cl;
		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

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

/*
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM4){

		_index=(_index+1)%32;
		__HAL_TIM_SetCompare(htim, TIM_CHANNEL_1,triangle1[_index]);
		__HAL_TIM_SetCompare(htim, TIM_CHANNEL_2,triangle2[_index]);
	}
}*/

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




void setup(void)
{
  nh.initNode();
//  nh.advertise(chatter);


  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_SET);

  //float duty_cycle = 0.5,voltage = 0.1,current = 0.2 ,speed = 1000.0, temperature=24.5;
  esc_sensor.current = 0.2;
  esc_sensor.temperature = 24.5;
  esc_sensor.throttle = 50;
  esc_sensor.voltage = 0.1;
  esc_sensor.rpm = 1000;


  forces.data = new std_msgs::Float32MultiArray::_data_type[8];
  forces.data_length = 8;


  wheel_speed.data = new std_msgs::Float32MultiArray::_data_type[16];
  wheel_speed.data_length = 16;



  uint16_t inc = TIM4->ARR/38;

  for(int i=0;i<32;++i){
	  triangle1[i] = (i+1)*inc;
  }
  inc*=2;
  for(int i=0;i<16;++i){
    triangle2[i] = (i+1)*inc;
  }
  for(int i=16;i<32;++i){
    triangle2[i] = (33-(i+1))*inc;
  }

  uint32_t clock = HAL_RCC_GetPCLK1Freq();
  TIMER_CLOCK_FREQ = clock/(TIM5->PSC+1);

  HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1 );
  HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_2 );

//  HAL_TIM_PWM_

  HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
  HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);

  HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
  HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);

  nh.advertise(vesc_pub);
  nh.advertise(force_pub);
  nh.advertise(wheel_speed_pub);

}



void loop(void)
{
  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

//  str_msg.data = hello;
//  chatter.publish(&str_msg);

  vesc_state.duty_cycle = esc_sensor.throttle/100.0;
  vesc_state.voltage_input = esc_sensor.voltage;
  vesc_state.current_input = esc_sensor.current;
  vesc_state.temperature_pcb = esc_sensor.temperature;
  vesc_state.speed = esc_sensor.rpm;


	_index=(_index+1)%32;
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1,triangle1[_index]);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2,triangle2[_index]);


  for(int i=2;i<8;++i){
	  forces.data[i] = 0.1*i;
  }
  for(int i=0;i<16;++i){
  	  wheel_speed.data[i] = 10*i;
  }

  forces.data[0] = freq1;
  forces.data[1] = duty1;


  forces.data[2] = freq2;
  forces.data[3] = duty2;

  forces.data[4]=TIMER_CLOCK_FREQ;


  vesc_pub.publish(&vesc_state);
  wheel_speed_pub.publish(&wheel_speed);
  force_pub.publish(&forces);

  nh.spinOnce();

  freq1=0;
  duty1=0;
  freq2=1;
  duty2=1;

  HAL_Delay(1000);
}

