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
#define ONBOARD_LED_Pin GPIO_PIN_3
#define ONBOARD_LED_GPIO_Port GPIOE
#define S04_Pin GPIO_PIN_4
#define S04_GPIO_Port GPIOE
#define ESC_Input_Pin GPIO_PIN_5
#define ESC_Input_GPIO_Port GPIOE
#define S06_Pin GPIO_PIN_6
#define S06_GPIO_Port GPIOE
#define ONBOARD_KEY1_Pin GPIO_PIN_13
#define ONBOARD_KEY1_GPIO_Port GPIOC
#define F06_Pin GPIO_PIN_0
#define F06_GPIO_Port GPIOC
#define F07_Pin GPIO_PIN_1
#define F07_GPIO_Port GPIOC
#define S02_Pin GPIO_PIN_2
#define S02_GPIO_Port GPIOC
#define S03_Pin GPIO_PIN_3
#define S03_GPIO_Port GPIOC
#define Servo_Input_Pin GPIO_PIN_0
#define Servo_Input_GPIO_Port GPIOA
#define PWM1_Pin GPIO_PIN_1
#define PWM1_GPIO_Port GPIOA
#define PWM2_Pin GPIO_PIN_2
#define PWM2_GPIO_Port GPIOA
#define PWM3_Pin GPIO_PIN_3
#define PWM3_GPIO_Port GPIOA
#define F03_Pin GPIO_PIN_4
#define F03_GPIO_Port GPIOA
#define S05_Pin GPIO_PIN_5
#define S05_GPIO_Port GPIOA
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
#define S07_Pin GPIO_PIN_7
#define S07_GPIO_Port GPIOE
#define LCD_SWITCH_Pin GPIO_PIN_9
#define LCD_SWITCH_GPIO_Port GPIOE
#define LCD_LED_Pin GPIO_PIN_10
#define LCD_LED_GPIO_Port GPIOE
#define LCD_CS_Pin GPIO_PIN_11
#define LCD_CS_GPIO_Port GPIOE
#define LCD_SCL_Pin GPIO_PIN_12
#define LCD_SCL_GPIO_Port GPIOE
#define LCD_WR_RS_Pin GPIO_PIN_13
#define LCD_WR_RS_GPIO_Port GPIOE
#define LCD_SDA_Pin GPIO_PIN_14
#define LCD_SDA_GPIO_Port GPIOE
#define LED_RED_Pin GPIO_PIN_15
#define LED_RED_GPIO_Port GPIOE
#define S10_Pin GPIO_PIN_10
#define S10_GPIO_Port GPIOB
#define S11_Pin GPIO_PIN_11
#define S11_GPIO_Port GPIOB
#define S12_Pin GPIO_PIN_12
#define S12_GPIO_Port GPIOB
#define S13_Pin GPIO_PIN_13
#define S13_GPIO_Port GPIOB
#define S14_Pin GPIO_PIN_14
#define S14_GPIO_Port GPIOB
#define S15_Pin GPIO_PIN_15
#define S15_GPIO_Port GPIOB
#define S08_Pin GPIO_PIN_8
#define S08_GPIO_Port GPIOD
#define S09_Pin GPIO_PIN_9
#define S09_GPIO_Port GPIOD
#define PCA9685_OE_Pin GPIO_PIN_10
#define PCA9685_OE_GPIO_Port GPIOD
#define Servo_Output_Pin GPIO_PIN_6
#define Servo_Output_GPIO_Port GPIOC
#define ESC_Output_Pin GPIO_PIN_7
#define ESC_Output_GPIO_Port GPIOC
#define PWM0_Pin GPIO_PIN_15
#define PWM0_GPIO_Port GPIOA
#define LED_BLUE_Pin GPIO_PIN_3
#define LED_BLUE_GPIO_Port GPIOD
#define LED_YELLOW_Pin GPIO_PIN_5
#define LED_YELLOW_GPIO_Port GPIOD
#define SPI1_CS_Pin GPIO_PIN_6
#define SPI1_CS_GPIO_Port GPIOD
#define Manual_Output_Pin GPIO_PIN_5
#define Manual_Output_GPIO_Port GPIOB
#define Manual_Input_Pin GPIO_PIN_7
#define Manual_Input_GPIO_Port GPIOB
#define S00_Pin GPIO_PIN_0
#define S00_GPIO_Port GPIOE
#define S01_Pin GPIO_PIN_1
#define S01_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
