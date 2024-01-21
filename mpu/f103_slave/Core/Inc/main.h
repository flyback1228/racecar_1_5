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
#include "stm32f1xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define LCD_LED_Pin GPIO_PIN_0
#define LCD_LED_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_1
#define LCD_CS_GPIO_Port GPIOA
#define LCD_RST_Pin GPIO_PIN_4
#define LCD_RST_GPIO_Port GPIOA
#define LCD_SCK_Pin GPIO_PIN_5
#define LCD_SCK_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_6
#define LCD_DC_GPIO_Port GPIOA
#define LCD_MOSI_Pin GPIO_PIN_7
#define LCD_MOSI_GPIO_Port GPIOA
#define S00_Pin GPIO_PIN_0
#define S00_GPIO_Port GPIOB
#define S00_EXTI_IRQn EXTI0_IRQn
#define S01_Pin GPIO_PIN_1
#define S01_GPIO_Port GPIOB
#define S01_EXTI_IRQn EXTI1_IRQn
#define S02_Pin GPIO_PIN_2
#define S02_GPIO_Port GPIOB
#define S02_EXTI_IRQn EXTI2_IRQn
#define S10_Pin GPIO_PIN_10
#define S10_GPIO_Port GPIOB
#define S10_EXTI_IRQn EXTI15_10_IRQn
#define S11_Pin GPIO_PIN_11
#define S11_GPIO_Port GPIOB
#define S11_EXTI_IRQn EXTI15_10_IRQn
#define LEDB12_Pin GPIO_PIN_12
#define LEDB12_GPIO_Port GPIOB
#define S13_Pin GPIO_PIN_13
#define S13_GPIO_Port GPIOB
#define S13_EXTI_IRQn EXTI15_10_IRQn
#define S14_Pin GPIO_PIN_14
#define S14_GPIO_Port GPIOB
#define S14_EXTI_IRQn EXTI15_10_IRQn
#define S15_Pin GPIO_PIN_15
#define S15_GPIO_Port GPIOB
#define S15_EXTI_IRQn EXTI15_10_IRQn
#define S12_Pin GPIO_PIN_12
#define S12_GPIO_Port GPIOA
#define S12_EXTI_IRQn EXTI15_10_IRQn
#define S03_Pin GPIO_PIN_3
#define S03_GPIO_Port GPIOB
#define S03_EXTI_IRQn EXTI3_IRQn
#define S04_Pin GPIO_PIN_4
#define S04_GPIO_Port GPIOB
#define S04_EXTI_IRQn EXTI4_IRQn
#define S05_Pin GPIO_PIN_5
#define S05_GPIO_Port GPIOB
#define S05_EXTI_IRQn EXTI9_5_IRQn
#define S06_Pin GPIO_PIN_6
#define S06_GPIO_Port GPIOB
#define S06_EXTI_IRQn EXTI9_5_IRQn
#define S07_Pin GPIO_PIN_7
#define S07_GPIO_Port GPIOB
#define S07_EXTI_IRQn EXTI9_5_IRQn
#define S08_Pin GPIO_PIN_8
#define S08_GPIO_Port GPIOB
#define S08_EXTI_IRQn EXTI9_5_IRQn
#define S09_Pin GPIO_PIN_9
#define S09_GPIO_Port GPIOB
#define S09_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
