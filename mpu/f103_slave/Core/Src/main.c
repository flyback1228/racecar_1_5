/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"


#include "fonts.h"
#include "utility.h"

#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

//esc sensor data type define, real voltage = voltage/10.0, real current = current / 10.0
typedef struct{
	int16_t throttle;
	uint16_t temperature;
	uint32_t rpm;
	uint16_t voltage;
	uint16_t current;
} ESC_SensorTypeDef;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SPEED_BUFFER_SIZE 16
#define SPEED_PIN_COUNT 8
#define SPEED_MAX_INCREMENT 2
#define ESC_DATA_SIZE 32
#define RX_BUFFER_SIZE 16
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
//fonts
FontDef Font_5x5 = {5,5,95*5,Font5x5};
FontDef Font_7x10 = {7,10,95*10,Font7x10};
FontDef Font_11x18 = {11,18,95*18,Font11x18};
FontDef Font_16x26 = {16,26,95*26,Font16x26};

/*
"ACSR" + WHEEL ROTATING DIRECTION + SPEED_DATA + ESC_RECEIVE_LABEL + ESC_DATA
WHEEL ROTATING DIRECTION: 0 -- forward, 1 -- backward
SPEED_DATA:  containing SPEED_PIN_COUNT sets data, each data has two bytes
	1st byte -- the integer part of speed in Hz
	2nd byte -- the decimal part of speed in Hz
ESC_RECEIVE_LABEL : 0 -- no data, non-zeor -- has data
*/
uint8_t send_data[4 + 4 + SPEED_PIN_COUNT*2 + 1 + sizeof(ESC_SensorTypeDef)];

//Speed tracking buffers
uint8_t pulse_pointer[SPEED_PIN_COUNT]={0};
uint8_t increment_count[SPEED_PIN_COUNT]={0};
uint32_t pulse[SPEED_PIN_COUNT][SPEED_BUFFER_SIZE];


//esc data
uint8_t esc_receive[ESC_DATA_SIZE];
ESC_SensorTypeDef esc_sensor;
uint8_t esc_index=0;

// UART receive
uint8_t rx[RX_BUFFER_SIZE];
uint8_t rx_index = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
	esc_sensor.voltage = (uint16_t)(data[(15+start_index)%ESC_DATA_SIZE]);
	esc_sensor.current = ((uint16_t)(data[(16+start_index)%ESC_DATA_SIZE] <<8) | (data[(17+start_index)%ESC_DATA_SIZE]));
	esc_sensor.temperature = (uint16_t)(data[(18+start_index)%ESC_DATA_SIZE] <<8) | (data[(19+start_index)%ESC_DATA_SIZE]);

	esc_index = 0;
	memcpy(&send_data[SPEED_PIN_COUNT*2+9],&esc_sensor,sizeof(ESC_SensorTypeDef));

	return HAL_OK;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==huart1.Instance){
		read_ble_data(esc_receive);
	}else if(huart->Instance==huart2.Instance){
		if(rx[(rx_index+RX_BUFFER_SIZE)%RX_BUFFER_SIZE]=='t'
				&& rx[(rx_index+RX_BUFFER_SIZE-1)%RX_BUFFER_SIZE]=='e'
				&& rx[(rx_index+RX_BUFFER_SIZE-2)%RX_BUFFER_SIZE]=='s'
				&& rx[(rx_index+RX_BUFFER_SIZE-3)%RX_BUFFER_SIZE]=='e'
				&& rx[(rx_index+RX_BUFFER_SIZE-3)%RX_BUFFER_SIZE]=='r'){
			NVIC_SystemReset();
		}

		rx_index=(rx_index+1)%RX_BUFFER_SIZE;
		HAL_UART_Receive_DMA(&huart2, &rx[rx_index], 1);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if(huart->Instance==huart1.Instance) {
    	HAL_UART_Receive_DMA(&huart1, esc_receive, ESC_DATA_SIZE);
    }else if(huart->Instance==huart2.Instance){
    	HAL_UART_Receive_DMA(&huart2, rx, 1);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	uint8_t index;
	switch(GPIO_Pin){
	case S00_Pin:
		index = 0;
		break;
	case S01_Pin:
		index=1;
		break;
	case S04_Pin:
		index = 2;
		break;
	case S05_Pin:
		index = 3;
		break;
	case S08_Pin:
		index = 4;
		break;
	case S09_Pin:
		index = 5;
		break;
	case S12_Pin:
		index = 6;
		break;
	case S13_Pin:
		index = 7;
		break;
	}
	pulse[index][pulse_pointer[index]]=micros();
	++pulse_pointer[index];
	pulse_pointer[index]%=SPEED_BUFFER_SIZE;
	increment_count[index]=0;
}

void calcVelocity(){

	for(uint8_t i=0;i<SPEED_PIN_COUNT;++i){
		if(increment_count[i]>=SPEED_MAX_INCREMENT){
			send_data[2*i+8]=0;
			send_data[2*i+9]=0;
			continue;
		}
		uint32_t s=pulse[i][(pulse_pointer[i]+SPEED_BUFFER_SIZE-1)%SPEED_BUFFER_SIZE]-pulse[i][pulse_pointer[i]];
		if(s==0){
			send_data[2*i+8]=0;
			send_data[2*i+9]=0;
		}
		else{
			float speed = (SPEED_BUFFER_SIZE-1)*1000000.0/s;
			uint8_t speed_int = (uint8_t)speed;
			uint8_t speed_decimal = (uint8_t)((speed-speed_int)*100.0);
			send_data[2*i+8]=speed_int;
			send_data[2*i+9]=speed_decimal;
		}
	}

	/*
	 * need work
	 * label the rotating direction
	 */
	for(uint8_t i=0;i<4;++i){
		send_data[4+i]=0;
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	calcVelocity();
	if(esc_index++>9){
		send_data[SPEED_PIN_COUNT*2+8]=1;
		esc_index=10;
	}else{
		send_data[SPEED_PIN_COUNT*2+8]=0;
	}
	HAL_UART_Transmit(&huart2, send_data, SPEED_PIN_COUNT*2+9+sizeof(ESC_SensorTypeDef),10);

}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //speed_on_single_pin[0]=acsr;
  send_data[0]='A';
  send_data[1]='C';
  send_data[2]='S';
  send_data[3]='R';

  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_LED_GPIO_Port, LCD_LED_Pin, GPIO_PIN_SET);
  DWT_Init();
  ILI9341_Init();
  ILI9341_Fill_Screen(WHITE);
  ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
  ILI9341_Write_Text("FPS TEST, 40 loop 2 screens", 10, 10, &Font_11x18, BLACK, WHITE);
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_UART_Receive_DMA(&huart1, esc_receive, ESC_DATA_SIZE);
  HAL_UART_Receive_DMA(&huart2, rx, 1);

  for(int i=0;i<10;++i){
	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	  HAL_Delay(200);
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t t = millis();
  while (1)
  {
	for(uint8_t i=0;i<SPEED_PIN_COUNT;++i){
		increment_count[i]++;
	}
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

    uint32_t h = millis();
    while(h-t<500){
    	h = millis();
    	HAL_Delay(1);
    }
    t = h;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 35;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 50000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_LED_Pin|LCD_CS_Pin|LCD_RST_Pin|LCD_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LEDB12_GPIO_Port, LEDB12_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_LED_Pin LCD_CS_Pin LCD_RST_Pin LCD_DC_Pin */
  GPIO_InitStruct.Pin = LCD_LED_Pin|LCD_CS_Pin|LCD_RST_Pin|LCD_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : S00_Pin S04_Pin S08_Pin S12_Pin
                           S01_Pin S09_Pin S13_Pin */
  GPIO_InitStruct.Pin = S00_Pin|S04_Pin|S08_Pin|S12_Pin
                          |S01_Pin|S09_Pin|S13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LEDB12_Pin */
  GPIO_InitStruct.Pin = LEDB12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LEDB12_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : S05_Pin */
  GPIO_InitStruct.Pin = S05_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(S05_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
