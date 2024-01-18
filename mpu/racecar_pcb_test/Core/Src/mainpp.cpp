/*
 * mainpp.cpp
 *
 *  Created on: Oct 28, 2023
 *      Author: acsr2004
 */

#include "mainpp.h"
#include "main.h"

#include "stm32h7xx_hal.h"
#include "utility.h"
#include <stdio.h>
#include <cstring>

extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart7;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim16;
extern ADC_HandleTypeDef hadc1;
#define SPEED_PIN_COUNT 16

typedef enum{
	Auto=0,
	Manual
}InputMode;

InputMode input_mode;

const uint32_t acsr = ('A'<<24) | ('C'<<16) | ('S'<<8) | 'R';
uint32_t received_speed[SPEED_PIN_COUNT+1];
uint32_t speed[SPEED_PIN_COUNT];
//uint8_t speed_index=0;
char led_state;

uint8_t ch3_cnt=0,ch4_cnt=99;

uint32_t servo_duty,esc_duty,freq;
uint32_t force_raw[8];

HAL_StatusTypeDef read_speed_data(){
	uint8_t i;
	for(i = 0;i<SPEED_PIN_COUNT+1;++i){
		if(received_speed[i]==acsr)break;
	}
	if(i==SPEED_PIN_COUNT+1)return HAL_ERROR;

	memcpy(speed,&received_speed[i+1],(SPEED_PIN_COUNT-i)*sizeof(uint32_t));
	if(i){
		memcpy(&speed[SPEED_PIN_COUNT-i],received_speed,i*sizeof(uint32_t));
	}
	return HAL_OK;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==UART5){
		if(read_speed_data()==HAL_ERROR){
			HAL_UART_Receive_DMA(&huart5, (uint8_t*)received_speed, sizeof(uint32_t)*(SPEED_PIN_COUNT+1));
		}
	}else if(huart->Instance==UART7){
		switch(led_state){
		case 'r':
		case 'R':
			HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
			break;
		case 'b':
		case 'B':
			HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
			break;
		case 'y':
		case 'Y':
			HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
			break;
		default:
			break;
		}
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
    if(UartHandle->Instance==UART5) {
    	HAL_UART_Receive_DMA(&huart5, (uint8_t*)received_speed, sizeof(uint32_t)*(SPEED_PIN_COUNT+1));
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM16){
		if(input_mode==Manual){
			esc_duty=0;
			servo_duty=0;
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1,esc_duty);
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2,servo_duty);
		}
	}
}


/*
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	float force[8];
	for(uint8_t i=0;i<8;++i){
		force[i]=(float)force_raw[i]*3.3/4096.0;
	}
	char msg[100]={0};
	sprintf(msg,"Force: %0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f\n",force[0],force[1],force[2],force[3],force[4],force[5],force[6],force[7]);

}*/

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){

//	if(input_mode==Auto)return;

	uint32_t temp_freq;
	if(htim->Instance==TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){

		temp_freq = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		servo_duty = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1,servo_duty);
		__HAL_TIM_SetCounter(htim,0);

		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);

	}else if(htim->Instance==TIM15 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){

		temp_freq = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		esc_duty = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2,esc_duty);
		__HAL_TIM_SetCounter(htim,0);

		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);

	}
	if(freq<1000 || abs((int)temp_freq-(int)freq)>50){
		freq=temp_freq;
		__HAL_TIM_SET_AUTORELOAD(&htim3,temp_freq);
	}
	__HAL_TIM_SetCounter(&htim16,0);

}

void test_cp2101(){

}

void test_i2c(){

    uint8_t i;
    HAL_StatusTypeDef ret;
    uint8_t address[10];
    uint8_t length=0;
    for(i=1; i<128; i++)
    {
        ret = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i<<1), 3, 5);
        if(ret == HAL_OK)
        {
            address[length++]=i;
        }
    }
    uint8_t i2c_start_msg1[] = "I2C Scanning:\n";

	char buffer[25] = {0};
	HAL_UART_Transmit(&huart7, i2c_start_msg1, sizeof(i2c_start_msg1)-1, 10);

	if(length==2){
		sprintf(buffer, "\tI2C1 Address:0x%02X,0x%02X\n", address[0],address[1]);
		HAL_UART_Transmit(&huart7, (uint8_t*)buffer, 24, 10);
	}else if(length==1){
		sprintf(buffer, "\tI2C1 Address:0x%02X\n", address[0]);
		HAL_UART_Transmit(&huart7, (uint8_t*)buffer, 19, 10);
	}else if(length==0){
		HAL_UART_Transmit(&huart7, (uint8_t*)"\tNo Device Found In I2C1\n", 25, 10);
	}

    length=0;
    for(i=1; i<128; i++)
	{
		ret = HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(i<<1), 3, 5);
		if(ret == HAL_OK)
		{
			address[length++]=i;
		}
	}
    if(length==2){
		sprintf(buffer, "\tI2C2 Address:0x%02X,0x%02X\n", address[0],address[1]);
		HAL_UART_Transmit(&huart7, (uint8_t*)buffer, 24, 10);
	}else if(length==1){
		sprintf(buffer, "\tI2C2 Address:0x%02X\n", address[0]);
		HAL_UART_Transmit(&huart7, (uint8_t*)buffer, 19, 10);
	}else if(length==0){
		HAL_UART_Transmit(&huart7, (uint8_t*)"\tNo Device Found In I2C2\n", 25, 10);
	}

}

void test_speed(){
	char send_data[7];
	HAL_UART_Transmit(&huart7, (uint8_t*)"Speed: ", 7, 10);
	for(int i=0;i<SPEED_PIN_COUNT;++i){
		sprintf(send_data,"%04lu ",speed[i]);
		HAL_UART_Transmit(&huart7, (uint8_t*)send_data, 5, 10);
	}
	uint8_t lf[]={10};
	HAL_UART_Transmit(&huart7, lf, 1, 10);

}

void test_brake(){
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,ch3_cnt++);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,ch4_cnt--);
	if(ch3_cnt==99)ch3_cnt=0;
	if(ch4_cnt==0)ch4_cnt=99;
}


void input_test(){
	InputMode mode = HAL_GPIO_ReadPin(Manual_Input_GPIO_Port, Manual_Input_Pin)==GPIO_PIN_RESET?Auto:Manual;
	if(input_mode!=mode){
		input_mode=mode;
		if(input_mode==Manual){
			HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
			HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
			HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
			HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);

			HAL_TIM_Base_Start_IT(&htim16);
//			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
//			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
		}else{
			freq=0;
			HAL_TIM_IC_Stop_IT(&htim5, TIM_CHANNEL_1);
			HAL_TIM_IC_Stop(&htim5, TIM_CHANNEL_2);
			HAL_TIM_IC_Stop_IT(&htim15, TIM_CHANNEL_1);
			HAL_TIM_IC_Stop(&htim15, TIM_CHANNEL_2);

			HAL_TIM_Base_Stop_IT(&htim16);

			__HAL_TIM_SET_AUTORELOAD(&htim3,20000-1);
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1,5000-1);
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2,10000-1);
//			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
//			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
		}
	}
	uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim3);
	uint32_t esc_cnt = __HAL_TIM_GET_COMPARE(&htim3,TIM_CHANNEL_1);
	uint32_t servo_cnt = __HAL_TIM_GET_COMPARE(&htim3,TIM_CHANNEL_2);

	char str[200]={0};
	if(mode==Auto){
		sprintf(str,"Input Mode: Auto, Frequency: %lu Hz, ESC DutyCycle: %u%%, SERVO DutyCycle: %lu%%\n",1000000/arr,esc_cnt*100/arr,servo_cnt*100/arr);
	}else{
		sprintf(str,"Input Mode: Manual, Frequency: %lu Hz, ESC DutyCycle: %u%%, SERVO DutyCycle: %lu%%\n",1000000/arr,esc_cnt*100/arr,servo_cnt*100/arr);
	}
	uint8_t i=0;
	while(str[i]!=0 && i<200){
		HAL_UART_Transmit(&huart7, (uint8_t*)(&str[i++]), 1, 1);
	}

}

void force_test(){
	float force[8];
	for(uint8_t i=0;i<8;++i){
		force[i]=(float)force_raw[i]*3.3/0xFFFF;
	}
	char msg[100]={0};
	sprintf(msg,"Force: %0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f\n",force[0],force[1],force[2],force[3],force[4],force[5],force[6],force[7]);
	uint8_t i=0;
	while(msg[i]!=0 && i<100){
		HAL_UART_Transmit(&huart7, (uint8_t*)(&msg[i++]), 1, 1);
	}
}

void BNO085_setup(){
	HAL_GPIO_WritePin(BNO_P0_GPIO_Port, BNO_P0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BNO_P1_GPIO_Port, BNO_P1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BNO_BOOTN_GPIO_Port, BNO_BOOTN_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(BNO_NRST_GPIO_Port,BNO_NRST_Pin,GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(BNO_NRST_GPIO_Port,BNO_NRST_Pin,GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(BNO_NRST_GPIO_Port,BNO_NRST_Pin,GPIO_PIN_SET);

}

void setup(){
	DWT_Init();
//	HAL_ADC_Start_DMA(&hadc1, force_raw, 8);
	HAL_ADC_Start_DMA(&hadc1,force_raw,8);
	HAL_UART_Receive_DMA(&huart5, (uint8_t*)received_speed, sizeof(uint32_t)*(SPEED_PIN_COUNT+1));
	HAL_UART_Receive_DMA(&huart7, (uint8_t*)&led_state, 1);

	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,33);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,67);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,ch3_cnt++);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,ch4_cnt--);

	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);


	HAL_GPIO_WritePin(Manual_Output_GPIO_Port, Manual_Output_Pin, GPIO_PIN_RESET);
	freq = 0;
	input_mode = HAL_GPIO_ReadPin(Manual_Input_GPIO_Port, Manual_Input_Pin)==GPIO_PIN_RESET?Auto:Manual;
	if(input_mode==Manual){
		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_2);

		HAL_TIM_Base_Start_IT(&htim16);
	}

	__HAL_TIM_SET_AUTORELOAD(&htim3,20000-1);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1,5000-1);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2,10000-1);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	BNO085_setup();

}




void loop(){

	HAL_UART_Transmit(&huart7, (uint8_t*)("\n\n"), 2, 1);
	force_test();
	input_test();
	test_speed();
	test_i2c();
	test_brake();
	HAL_Delay(1000);


}


