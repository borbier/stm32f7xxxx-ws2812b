/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "ws2812b.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
WSLED wsled;
uint8_t palette_A[6][3] = {
	{0xac,0x46,0x84},
	{0x95,0x47,0x84},
	{0x6d,0x48,0x86},
	{0x50,0x48,0x87},
	{0x3a,0x48,0x87},
	{0x20,0x49,0x88},
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint32_t Wheel(uint8_t WheelPos);
uint8_t randomNumber(uint8_t lower, uint8_t upper);

void rainbowGradient(void);
void strobeGradient(void);
void wheelAll(void);
void randomColor(void);
void loopPalette(uint8_t color[6][3]);
void loopPalette2(uint8_t color[6][3]);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	wsled.led_init();
	wsled.led_set_color_all(0x0F, 0x0F, 0x0F);  
	wsled.led_display();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		randomColor();
		// 5 cycles of all colors on wheel

		//for (j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
		//	for (q=0; q < 3; q++) {
		//		for (i=0;i < LED_CFG_LEDS_CNT;i=i+3) {
		//			wsled.led_set_color_rgb(i+q, Wheel((i+j) % 255));    //turn every third pixel on
		//		}
		//		wsled.led_display();

		//		HAL_Delay(50);

		//		for (i=0;i < LED_CFG_LEDS_CNT;i=i+3) {
		//			wsled.led_set_color_rgb(i+q, 0);        //turn every third pixel off
		//		}
		//	}
		//}
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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint32_t Wheel(uint8_t WheelPos) 
{
  if(WheelPos < 85) 
  {
    //return wsled.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
		return (((uint32_t)(WheelPos * 3) << 16) | ((255 - (uint32_t)WheelPos * 3) <<  8) | 0) << 8;
  } 
  else 
  {
    if(WheelPos < 170) 
    {
     WheelPos -= 85;
     //return wsled.Color(255 - WheelPos * 3, 0, WheelPos * 3);
		 return (((255 - (uint32_t)WheelPos * 3) << 16) | (0 <<  8) | ((uint32_t)WheelPos * 3)) << 8;
    } 
    else 
    {
     WheelPos -= 170;
     //return wsled.Color(0, WheelPos * 3, 255 - WheelPos * 3);
		 return (((uint32_t)0 << 16) | (((uint32_t)WheelPos * 3) <<  8) | (255 - (uint32_t)WheelPos * 3)) << 8;
    }
  }
}

uint8_t randomNumber(uint8_t lower, uint8_t upper) {
	return (rand() % (upper - lower + 1)) + lower;
}

void rainbowGradient(void) {
	uint16_t i, j;
	
	for(j=0; j<256*5; j++) { 
		for(i=0; i< LED_CFG_LEDS_CNT; i++) {
			wsled.led_set_color_rgb(i, Wheel(((i * 255 / LED_CFG_LEDS_CNT) + j) & 255));
		}
		wsled.led_display();
		HAL_Delay(10);
	}
}

void strobeGradient(void) {
	uint16_t i, j, q;
	
	for (j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
		for (q=0; q < 3; q++) {
			for (i=0;i < LED_CFG_LEDS_CNT;i=i+3) {
				wsled.led_set_color_rgb(i+q, Wheel((i+j) % 255));    //turn every third pixel on
			}
			wsled.led_display();

			HAL_Delay(50);

			for (i=0;i < LED_CFG_LEDS_CNT;i=i+3) {
				wsled.led_set_color_rgb(i+q, 0);        //turn every third pixel off
			}
		}
	}
}

void wheelAll(void) {
	uint16_t i;
	
	for (i=0; i < 256; i++) {
		wsled.led_set_color_all_rgb(Wheel(i % 255));
		wsled.led_display();
		HAL_Delay(500);
	}
}

void randomColor(void) {
	uint16_t i;
	uint8_t color_r, color_g, color_b;
	
	for (i=0;i < LED_CFG_LEDS_CNT; i++) {
		color_r = randomNumber(0, 255);
		color_g = randomNumber(0, 255);
		color_b = randomNumber(0, 255);
		
		wsled.led_set_color(i, color_r, color_g, color_b);
	}
	wsled.led_display();
	HAL_Delay(500);
}

void loopPalette(uint8_t color[6][3]) {
	uint16_t i, j;
	uint8_t palettePos;
	
	for (j=0;j < 6; j++) {
		for (i=0;i < LED_CFG_LEDS_CNT; i++) {
			if ((i / 6) % 2 == 1) {
				palettePos = (i+j)%6;
			} else {
				palettePos = 5-(i+j)%6;
			}
			wsled.led_set_color(i, color[palettePos][0], color[palettePos][1], color[palettePos][2]);
		}
		wsled.led_display();
		HAL_Delay(500);
	}
}

void loopPalette2(uint8_t color[6][3]) {
	uint16_t i, j;
	uint8_t palettePos;
	
	for (j=0;j < 6;j++) {
		for (i=0;i < LED_CFG_LEDS_CNT;i++) {
			palettePos = ((i / 6)+j) % 6;
			wsled.led_set_color(i, color[palettePos][0], color[palettePos][1], color[palettePos][2]);
		}
		wsled.led_display();
		HAL_Delay(500);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
