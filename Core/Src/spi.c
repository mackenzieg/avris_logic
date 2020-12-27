/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* SPI1 init function */
void MX_SPI1_Init(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  /**SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA6   ------> SPI1_MISO
  PA7   ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = ECG_SPI_SCLK_Pin|ECG_SPI_MISO_Pin|ECG_SPI_MOSI_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* SPI1 DMA Init */

  /* SPI1_RX Init */
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMA_REQUEST_1);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MDATAALIGN_BYTE);

  /* SPI1_TX Init */
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_3, LL_DMA_REQUEST_1);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PRIORITY_HIGH);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MDATAALIGN_BYTE);

  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV128;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 7;

  LL_SPI_Init(SPI1, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
  LL_SPI_DisableNSSPulseMgt(SPI1);

  LL_SPI_SetRxFIFOThreshold(SPI1, LL_SPI_RX_FIFO_QUARTER_FULL);
}
/* SPI3 init function */
void MX_SPI3_Init(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  /**SPI3 GPIO Configuration
  PC10   ------> SPI3_SCK
  PC11   ------> SPI3_MISO
  PC12   ------> SPI3_MOSI
  */
  GPIO_InitStruct.Pin = INTERNAL_SPI3_SCK_Pin|INTERNAL_SPI3_MISO_Pin|INTERNAL_SPI3_MOSI_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV64;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 7;
  LL_SPI_Init(SPI3, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI3, LL_SPI_PROTOCOL_MOTOROLA);
  LL_SPI_EnableNSSPulseMgt(SPI3);

}

void spi_transmit_receive(SPI_TypeDef* spi, uint8_t *tx, uint8_t *rx, uint16_t size)
{
	if (size > 1)
	{
		LL_SPI_SetRxFIFOThreshold(spi, LL_SPI_RX_FIFO_TH_HALF);
	}
	else
	{
		LL_SPI_SetRxFIFOThreshold(spi, LL_SPI_RX_FIFO_TH_QUARTER);
	}

	if (!LL_SPI_IsEnabled(spi))
	{
		LL_SPI_Enable(spi);
	}

	uint8_t *rx_ptr = rx;
	uint8_t *tx_ptr = tx;

	uint16_t tx_count = size;
	uint16_t rx_count = size;

	while (tx_count > 0 || rx_count > 0) {
		if (tx_count > 0 && LL_SPI_IsActiveFlag_TXE(spi))
		{
			if (tx_count > 1)
			{
				LL_SPI_TransmitData16(spi, *((uint16_t*)tx_ptr));
				tx_ptr += sizeof(uint16_t);
				tx_count -= 2;
			}
			else
			{
				LL_SPI_TransmitData8(spi, *tx_ptr);
				tx_count -= 1;
			}
		}

		if (rx_count > 0 && LL_SPI_IsActiveFlag_RXNE(spi))
		{
			if (rx_count > 1)
			{
				*((uint16_t*) rx_ptr) = LL_SPI_ReceiveData16(spi);
				rx_ptr += sizeof(uint16_t);
				rx_count -= 2;

				if (rx_count <= 1)
				{
					// Switch to 8 bit mode for last 8 bits
					LL_SPI_SetRxFIFOThreshold(spi, LL_SPI_RX_FIFO_TH_QUARTER);
				}
			}
			else
			{
				*rx_ptr = LL_SPI_ReceiveData8(spi);
				rx_count -= 1;
			}
		}
	}

	while(LL_SPI_IsActiveFlag_BSY(spi)) {}

	LL_SPI_Disable(spi);
}

void spi_transmit_receive_dma(SPI_TypeDef* spi, DMA_TypeDef* dma, uint32_t channel_tx, uint32_t channel_rx, uint8_t *tx, uint8_t *rx, uint16_t size)
{
	LL_DMA_ConfigAddresses(dma,
						   channel_tx,
						   (uint32_t) tx,
						   LL_SPI_DMA_GetRegAddr(spi),
						   LL_DMA_GetDataTransferDirection(dma, channel_tx));
	LL_DMA_ConfigAddresses(dma,
						   channel_rx,
						   LL_SPI_DMA_GetRegAddr(spi),
						   (uint32_t) rx,
						   LL_DMA_GetDataTransferDirection(dma, channel_rx));
	LL_DMA_SetDataLength(dma, channel_tx, size);
	LL_DMA_SetDataLength(dma, channel_rx, size);

	if (size % 2 == 1)
	{
		LL_SPI_SetDMAParity_TX(spi, LL_SPI_DMA_PARITY_ODD);
		LL_SPI_SetDMAParity_RX(spi, LL_SPI_DMA_PARITY_ODD);
	}
	else
	{
		LL_SPI_SetDMAParity_TX(spi, LL_SPI_DMA_PARITY_EVEN);
		LL_SPI_SetDMAParity_RX(spi, LL_SPI_DMA_PARITY_EVEN);
	}


	LL_SPI_SetRxFIFOThreshold(spi, LL_SPI_RX_FIFO_TH_QUARTER);

	// Remember to turn this off when using SPI normally
	LL_SPI_EnableDMAReq_TX(spi);
	LL_SPI_EnableDMAReq_RX(spi);

	LL_DMA_EnableIT_TC(dma, channel_tx);
	LL_DMA_EnableIT_TC(dma, channel_rx);

	LL_DMA_EnableChannel(DMA1, channel_tx);
	LL_DMA_EnableChannel(DMA1, channel_rx);

	if (!LL_SPI_IsEnabled(spi))
	{
		LL_SPI_Enable(spi);
	}
}

void spi_transmit(SPI_TypeDef* spi, uint8_t *data, uint16_t size)
{
	spi_transmit_receive(spi, data, data, size);
}
void spi_transmit_it(SPI_TypeDef* spi, uint8_t *data, uint16_t size)
{

}
void spi_transmit_dma(SPI_TypeDef* spi, uint8_t *data, uint16_t size)
{

}

void spi_receive(SPI_TypeDef* spi, uint8_t *data, uint16_t size)
{
	spi_transmit_receive(spi, data, data, size);
}

void spi_receive_it(SPI_TypeDef* spi, uint8_t *data, uint16_t size)
{

}

void spi_receive_dma(SPI_TypeDef* spi, DMA_TypeDef* dma, uint32_t channel, uint8_t *data, uint16_t size)
{
	// TODO same as transmit and receive but without tx dma interrupt
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
