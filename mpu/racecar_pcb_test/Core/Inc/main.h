/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define F06_Pin GPIO_PIN_0
#define F06_GPIO_Port GPIOC
#define F07_Pin GPIO_PIN_1
#define F07_GPIO_Port GPIOC
#define F03_Pin GPIO_PIN_4
#define F03_GPIO_Port GPIOA
#define F00_Pin GPIO_PIN_6
#define F00_GPIO_Port GPIOA
#define F01_Pin GPIO_PIN_4
#define F01_GPIO_Port GPIOC
#define F04_Pin GPIO_PIN_5
#define F04_GPIO_Port GPIOC
#define F05_Pin GPIO_PIN_0
#define F05_GPIO_Port GPIOB
#define F02_Pin GPIO_PIN_1
#define F02_GPIO_Port GPIOB
#define BNO_INT_Pin GPIO_PIN_7
#define BNO_INT_GPIO_Port GPIOE
#define LED_RED_Pin GPIO_PIN_15
#define LED_RED_GPIO_Port GPIOE
#define BNO_NRST_Pin GPIO_PIN_14
#define BNO_NRST_GPIO_Port GPIOB
#define BNO_BOOTN_Pin GPIO_PIN_15
#define BNO_BOOTN_GPIO_Port GPIOB
#define BNO_P0_Pin GPIO_PIN_8
#define BNO_P0_GPIO_Port GPIOD
#define BNO_P1_Pin GPIO_PIN_9
#define BNO_P1_GPIO_Port GPIOD
#define LED_BLUE_Pin GPIO_PIN_3
#define LED_BLUE_GPIO_Port GPIOD
#define LED_YELLOW_Pin GPIO_PIN_5
#define LED_YELLOW_GPIO_Port GPIOD
#define Manual_Output_Pin GPIO_PIN_5
#define Manual_Output_GPIO_Port GPIOB
#define Manual_Input_Pin GPIO_PIN_7
#define Manual_Input_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
