/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

void MX_SPI1_Init(void);
void MX_SPI3_Init(void);

void spi_transmit_receive(SPI_TypeDef* spi, uint8_t *tx, uint8_t *rx, uint16_t size);
void spi_transmit_receive_dma(SPI_TypeDef* spi, DMA_TypeDef* dma, uint32_t channel_tx,
							  uint32_t channel_rx, uint8_t *tx, uint8_t *rx, uint16_t size);

void spi_transmit(SPI_TypeDef* spi, uint8_t *data, uint16_t size);
void spi_transmit_it(SPI_TypeDef* spi, uint8_t *data, uint16_t size);
void spi_transmit_dma(SPI_TypeDef* spi, uint8_t *data, uint16_t size);

void spi_receive(SPI_TypeDef* spi, uint8_t *data, uint16_t size);
void spi_receive_it(SPI_TypeDef* spi, uint8_t *data, uint16_t size);
// TODO create a struct for this
void spi_receive_dma(SPI_TypeDef* spi, DMA_TypeDef* dma, uint32_t channel, uint8_t *data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
