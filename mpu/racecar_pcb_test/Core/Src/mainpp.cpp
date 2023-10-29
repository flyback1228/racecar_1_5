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
extern UART_HandleTypeDef hlpuart1;
#define SPEED_PIN_COUNT 16

const uint32_t acsr = ('A'<<24) | ('C'<<16) | ('S'<<8) | 'R';
uint32_t received_speed[SPEED_PIN_COUNT+1];
uint32_t speed[SPEED_PIN_COUNT];
//uint8_t speed_index=0;

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
		if(read_speed_data()==HAL_OK){

		}
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
    if(UartHandle->Instance==UART5) {
    	HAL_UART_Receive_DMA(&huart5, (uint8_t*)received_speed, sizeof(uint32_t)*(SPEED_PIN_COUNT+1));
    }
}

void

void setup(){
	DWT_Init();
	HAL_UART_Receive_DMA(&huart5, (uint8_t*)received_speed, sizeof(uint32_t)*(SPEED_PIN_COUNT+1));
}



void loop(){

	char send_data[5];
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)"Speed ", 6, 10);
	for(int i=0;i<SPEED_PIN_COUNT;++i){
		sprintf(send_data,"%04lu ",speed[i]);
		HAL_UART_Transmit(&hlpuart1, (uint8_t*)send_data, 5, 10);
	}
	uint8_t lf[]={10};
	HAL_UART_Transmit(&hlpuart1, lf, 1, 10);
	HAL_Delay(500);

}


