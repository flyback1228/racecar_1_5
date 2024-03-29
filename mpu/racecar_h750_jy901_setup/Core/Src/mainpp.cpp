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
#include "jy901/JY901_Serial.h"

extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;

uint8_t jy901_data[22];

CJY901 jy901(&huart8);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==UART8){
		if(jy901_data[0]==0x55){
			jy901.parseData(jy901_data);
		}else{
			__HAL_UART_DISABLE_IT(&huart8, UART_IT_RXFF);
			uint8_t temp;
			HAL_UART_Receive(&huart8, &temp, 1, 10);
			while(temp!=0x55){
				HAL_UART_Receive(&huart8, &temp, 1, 10);
			}
			for(uint8_t i=0;i<10;++i){
				HAL_UART_Receive(&huart8, &temp, 1, 10);
			}
			__HAL_UART_ENABLE_IT(&huart8, UART_IT_RXFF);
			HAL_UART_Receive_DMA(&huart8, jy901_data, 11);
		}

	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
    if(UartHandle->Instance==UART8) {
    	HAL_UART_Receive_DMA(&huart8, jy901_data, 11);
    }
}

/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM16){
		if(input_mode==Manual){
			esc_duty=0;
			servo_duty=0;
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1,esc_duty);
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2,servo_duty);
		}
	}
}*/


void setup(){
	DWT_Init();

	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);

	jy901.setBaudRate(0x06);
	jy901.setReportRate(0x07);
	jy901.saveConf(1);

	uint8_t temp;
	HAL_UART_Receive(&huart8, &temp, 1, 10);
	while(temp!=0x55){
		HAL_UART_Receive(&huart8, &temp, 1, 10);
	}
	for(uint8_t i=0;i<10;++i){
		HAL_UART_Receive(&huart8, &temp, 1, 10);
	}

	HAL_UART_Receive_DMA(&huart8, jy901_data, 11);


}




void loop(){

	HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
	HAL_UART_Transmit(&huart7, (uint8_t*)"Acc\n", 4, 100);
	printf("acc: %5.2f,%5.2f,%5.2f\n",jy901.getAccX(),jy901.getAccY(),jy901.getAccZ());

	HAL_Delay(1000);


}


