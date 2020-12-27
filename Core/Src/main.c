/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "ads1292r.h"
#include "dfsdm.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include "wwdg.h"

void SystemClock_Config(void);

ads1292r_handle_t ecg_handle;

#define BUFFER_SIZE 1024

int32_t ch1_data_buffer[BUFFER_SIZE];
int32_t ch2_data_buffer[BUFFER_SIZE];

uint32_t buff_idx;

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_DFSDM1_Init();
  MX_I2C2_Init();
  // Must be called before the SPI init
  MX_DMA_Init();
  MX_SPI3_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();

  ecg_handle.spi_port = SPI1;
  ecg_handle.cs_pin = ECG_SPI_CS_Pin;
  ecg_handle.cs_pin_port = ECG_SPI_CS_GPIO_Port;
  ecg_handle.reset_pin = RST_ECG_Pin;
  ecg_handle.reset_pin_port = RST_ECG_GPIO_Port;
  ecg_handle.start_pin = START_ECG_Pin;
  ecg_handle.start_pin_port = START_ECG_GPIO_Port;

  ads1292r_init(&ecg_handle);

  ecg_drdy_flag = 0;
  dma_tc_flag = 0;

  ads1292r_start_cont_mode(&ecg_handle);

  uint32_t status;
  int32_t ch1;
  int32_t ch2;

  buff_idx = 0;

  while (1)
  {
	  if (ecg_drdy_flag)
	  {
		  // Data ready from ECG
		  ecg_drdy_flag = 0;
		  //LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
		  ads1292r_drdy(&ecg_handle);
	  }

	  if (dma_tc_flag)
	  {
		  dma_tc_flag = 0;

		  ads1292r_dma_irq(&ecg_handle, &status, &ch1, &ch2);

		  ch1_data_buffer[buff_idx]   = ch1;
		  ch2_data_buffer[buff_idx++] = ch2;

		  if (buff_idx == BUFFER_SIZE)
		  {
			  while(1) {
					LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
			  }
		  }
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_4)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_MSI_Enable();

   /* Wait till MSI is ready */
  while(LL_RCC_MSI_IsReady() != 1)
  {

  }
  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
  LL_RCC_MSI_SetCalibTrimming(0);
  LL_PWR_EnableBkUpAccess();
  LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_LOW);
  LL_RCC_LSE_Enable();

   /* Wait till LSE is ready */
  while(LL_RCC_LSE_IsReady() != 1)
  {

  }
  LL_RCC_MSI_EnablePLLMode();
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_EnableDomain_SYS();
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_SetSystemCoreClock(80000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C2_CLKSOURCE_PCLK1);
  LL_RCC_SetDFSDMClockSource(LL_RCC_DFSDM_CLKSOURCE_PCLK);

  // https://deepbluembedded.com/stm32-delay-microsecond-millisecond-utility-dwt-delay-timer-delay/
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
