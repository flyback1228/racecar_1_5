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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w25q64jv.h"
#include <stdio.h>
#include "st7735.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define WRITE_DATA
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef hlpuart1;

QSPI_HandleTypeDef hqspi;

SD_HandleTypeDef hsd1;

SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN PV */

HAL_SD_CardCIDTypedef pCID;
HAL_SD_CardCSDTypedef pCSD;
HAL_SD_CardInfoTypeDef pCardInfo;

#ifdef WRITE_DATA
#include "fonts.h"
#include "testimg.h"
//FontDef Font_7x10 = {7,10,Font7x10};
//FontDef Font_11x18 = {11,18,Font11x18};
//FontDef Font_16x26 = {16,26,Font16x26};

FontDef Font_7x10 = {7,10,95*10,Font7x10};
FontDef Font_11x18 = {11,18,95*18,Font11x18};
FontDef Font_16x26 = {16,26,95*26,Font16x26};
uint16_t* img = (uint16_t*)test_img_128x128;
#else
uint16_t* Font7x10,Font11x18,Font16x26;
FontDef Font_7x10 = {7,10,95*10,NULL};
FontDef Font_11x18 = {11,18,95*18,NULL};
FontDef Font_16x26 = {16,26,95*26,NULL};
uint16_t* img = NULL;
#endif

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_SPI2_Init(void);
static void MX_SDMMC1_SD_Init(void);
/* USER CODE BEGIN PFP */

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&hlpuart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void loop() {
    // Check border
    ST7735_FillScreen(ST7735_BLACK);

    for(int x = 0; x < ST7735_WIDTH; x++) {
        ST7735_DrawPixel(x, 0, ST7735_RED);
        ST7735_DrawPixel(x, ST7735_HEIGHT-1, ST7735_RED);
    }

    for(int y = 0; y < ST7735_HEIGHT; y++) {
        ST7735_DrawPixel(0, y, ST7735_RED);
        ST7735_DrawPixel(ST7735_WIDTH-1, y, ST7735_RED);
    }

    HAL_Delay(3000);

    // Check fonts
    ST7735_FillScreen(ST7735_BLACK);


    ST7735_WriteString(0, 0, "Font_7x10, red on black, lorem ipsum dolor sit amet", Font_7x10, ST7735_RED, ST7735_BLACK);
    ST7735_WriteString(0, 3*10, "Font_11x18, green, lorem ipsum", Font_11x18, ST7735_GREEN, ST7735_BLACK);
    ST7735_WriteString(0, 3*10+3*18, "Font_16x26", Font_16x26, ST7735_BLUE, ST7735_BLACK);
    HAL_Delay(2000);

    // Check colors
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_WriteString(0, 0, "BLACK", Font_11x18, ST7735_WHITE, ST7735_BLACK);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_BLUE);
    ST7735_WriteString(0, 0, "BLUE", Font_11x18, ST7735_BLACK, ST7735_BLUE);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_RED);
    ST7735_WriteString(0, 0, "RED", Font_11x18, ST7735_BLACK, ST7735_RED);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_GREEN);
    ST7735_WriteString(0, 0, "GREEN", Font_11x18, ST7735_BLACK, ST7735_GREEN);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_CYAN);
    ST7735_WriteString(0, 0, "CYAN", Font_11x18, ST7735_BLACK, ST7735_CYAN);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_MAGENTA);
    ST7735_WriteString(0, 0, "MAGENTA", Font_11x18, ST7735_BLACK, ST7735_MAGENTA);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_YELLOW);
    ST7735_WriteString(0, 0, "YELLOW", Font_11x18, ST7735_BLACK, ST7735_YELLOW);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_WHITE);
    ST7735_WriteString(0, 0, "WHITE", Font_11x18, ST7735_BLACK, ST7735_WHITE);
    HAL_Delay(500);

#ifdef ST7735_IS_128X128
    // Display test image 128x128
    ST7735_DrawImage(0, 0, ST7735_WIDTH, ST7735_HEIGHT, (uint16_t*)img);

/*
    // Display test image 128x128 pixel by pixel
    for(int x = 0; x < ST7735_WIDTH; x++) {
        for(int y = 0; y < ST7735_HEIGHT; y++) {
            uint16_t color565 = test_img_128x128[y][x];
            // fix endiness
            color565 = ((color565 & 0xFF00) >> 8) | ((color565 & 0xFF) << 8);
            ST7735_DrawPixel(x, y, color565);
        }
    }
*/
    HAL_Delay(15000);
#endif // ST7735_IS_128X128

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
  MX_QUADSPI_Init();
  MX_LPUART1_UART_Init();
  MX_SPI2_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
  QSPI_W25Q64JV_Init();
  QSPI_W25Q64JV_Reset();
  ST7735_Init();

	HAL_SD_GetCardCID(&hsd1, &pCID);
	HAL_SD_GetCardCSD(&hsd1, &pCSD);
	HAL_SD_GetCardInfo(&hsd1, &pCardInfo);

	printf("card type: %u\n",pCardInfo.CardType);


  uint8_t id[2];
  if (QSPI_OK != QSPI_W25Q64JV_DeviceID(id)) {
    while (1);
  }
  printf("Initialize W25Q64JV Successfully, Device ID: %u,%u\n",id[0],id[1]);
  HAL_GPIO_WritePin(LCD_LED_GPIO_Port, LCD_LED_Pin, GPIO_PIN_SET);
  HAL_Delay(100);

#ifdef WRITE_DATA

  QSPI_W25Q64JV_Write((uint8_t*)(Font_7x10.data),0x0,2*Font_7x10.size);
  printf("write Font_7x10 success\n");
  QSPI_W25Q64JV_Write((uint8_t*)(Font_11x18.data),2*Font_7x10.size,2*Font_11x18.size);
  printf("write Font_11x18 success\n");
  QSPI_W25Q64JV_Write((uint8_t*)(Font_16x26.data),2*Font_7x10.size+2*Font_11x18.size,2*Font_16x26.size);
  printf("write Font_16x26 success\n");
  QSPI_W25Q64JV_Write((uint8_t*)(test_img_128x128),2*Font_7x10.size+2*Font_11x18.size+2*Font_16x26.size,2*128*128);
  printf("write image 128*128 success\n");

  uint16_t read_buf[100];
  QSPI_W25Q64JV_Read((uint8_t*)read_buf, 0x00, 200);
  printf("Read first 100 element of Font_7x10:\n");
	for(int i=0;i<100;++i){
		if(i%10==0)
			printf("\n");
		printf("0x%04x\t", read_buf[i]);
	}
	printf("\n");

	printf("Read first 100 element of Font_11x18:\n");
	QSPI_W25Q64JV_Read((uint8_t*)read_buf, 2*Font_7x10.size, 200);
	for(int i=0;i<100;++i){
		if(i%18==0)
			printf("\n");
		printf("0x%04x\t", read_buf[i]);
	}
	printf("\n");

	printf("Read first 100 element of Font_16x26:\n");
	QSPI_W25Q64JV_Read((uint8_t*)read_buf, 2*Font_7x10.size+2*Font_11x18.size, 200);
	for(int i=0;i<100;++i){
		if(i%26==0)
			printf("\n");
		printf("0x%04x\t", read_buf[i]);
	}
	printf("\n");

	printf("Read first 100 element of image_128*128:\n");
	QSPI_W25Q64JV_Read((uint8_t*)read_buf, 2*Font_7x10.size+2*Font_11x18.size+2*Font_16x26.size, 200);
	for(int i=0;i<100;++i){
		if(i%16==0)
			printf("\n");
		printf("0x%04x\t", read_buf[i]);
	}
	printf("\n");

#else
	Font_7x10.data = (uint16_t*)malloc(2*Font_7x10.size);;
	Font_11x18.data = (uint16_t*)malloc(2*Font_11x18.size);
	Font_16x26.data = (uint16_t*)malloc(2*Font_16x26.size);
	img = (uint16_t*)malloc(2*128*128);



	if(QSPI_W25Q64JV_Read((uint8_t*)Font_7x10.data, 0x00, 2*Font_7x10.size)==QSPI_OK){
	    printf("Read Font_7x10 data success. The first 100 element of Font_7x10 are:");
		for(int i=0;i<100;++i){
			if(i%10==0)
				printf("\n");
			printf("0x%04x\t", Font_7x10.data[i]);
		}
		printf("\n");
	}

	if(QSPI_W25Q64JV_Read((uint8_t*)Font_11x18.data, 2*Font_7x10.size, 2*Font_11x18.size)==QSPI_OK){
		printf("Read Font_11x18 data success. The first 100 element of Font_11x18 are:");
		for(int i=0;i<100;++i){
			if(i%18==0)
				printf("\n");
			printf("0x%04x\t", Font_11x18.data[i]);
		}
		printf("\n");
	}


	if(QSPI_W25Q64JV_Read((uint8_t*)Font_16x26.data, 2*Font_7x10.size+2*Font_11x18.size, 2*Font_16x26.size)==QSPI_OK){
		printf("Read Font_16x26 data success. The first 100 element of Font_16x26 are:");
		for(int i=0;i<100;++i){
			if(i%26==0)
				printf("\n");
			printf("0x%04x\t", Font_16x26.data[i]);
		}
		printf("\n");
	}
	if(QSPI_W25Q64JV_Read((uint8_t*)img, 2*Font_7x10.size+2*Font_11x18.size+2*Font_16x26.size,2*128*128)==QSPI_OK){
		printf("Read image_128x128 data success. The first 100 element of image_128x128 are:");
		for(int i=0;i<100;++i){
			if(i%26==0)
				printf("\n");
			printf("0x%04x\t", img[i]);
		}
		printf("\n");
	}


#endif







//  QSPI_W25Q64JV_Read(test_buf, 0x0, 100);
//  for (int i = 0; i < 100; i++) {
//    test_buf[i] = i;
//  }
//  QSPI_W25Q64JV_Write(test_buf, 0x0, 100);
//  QSPI_W25Q64JV_Read(read_buf, 0x0, 100);
//
//  for(int i=0;i<100;++i){
//	  printf("%d\n",read_buf[i]);
//  }


	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  loop();

	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//	  printf("hello\n");
//	  HAL_Delay(500);
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 10;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = 22;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0x0;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_LED_Pin|LCD_DC_Pin|LCD_CS_Pin|LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY2_Pin */
  GPIO_InitStruct.Pin = KEY2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_LED_Pin LCD_DC_Pin LCD_CS_Pin LCD_RST_Pin */
  GPIO_InitStruct.Pin = LCD_LED_Pin|LCD_DC_Pin|LCD_CS_Pin|LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
