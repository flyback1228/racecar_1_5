/*
 * utility.h
 *
 *  Created on: Oct 9, 2023
 *      Author: acsr
 */

#ifndef INC_UTILITY_H_
#define INC_UTILITY_H_

#include "main.h"

static inline void DWT_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; //
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;   //
}

static inline void delay_us(uint32_t us)
{
	uint32_t us_count_tic =  us * (SystemCoreClock / 1000000U);
	DWT->CYCCNT = 0U;
	while(DWT->CYCCNT < us_count_tic);
}

static inline uint32_t micros(void){
	return  DWT->CYCCNT / (SystemCoreClock / 1000000U);
}

static inline uint32_t millis(void){
	return  DWT->CYCCNT / (SystemCoreClock / 1000U);
}



#endif /* INC_UTILITY_H_ */
