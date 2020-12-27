#ifndef ADS129X_H
#define ADS129X_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include "spi.h"
#include "main.h"

/*
 * Large enough to fit a STATUS + CH1 + CH2 packet
 * STATUS = 24bit
 * CH1 = 24bit
 * CH2 = 24bit
 */

/*
 * Assuming a 1kS/s => T=1ms
 * For a buffer of 10ms we want to buffer 10 samples therefore
 * size should be 9*10=90 bytes (9 is spi packet size).
 * Double this to allow for dma circular buffering
 * TODO add system to automatically calculate this
 */

#define ADS1292R_SPI_BUFFER_SIZE_RX 9
#define ADS1292R_SPI_BUFFER_SIZE_TX 4

// TODO add proper library
extern void DWT_Delay_us(volatile uint32_t au32_microseconds);

extern void DWT_Delay_ms(volatile uint32_t au32_milliseconds);

// TODO add states
enum ADS1292R_ECG_STATE {
	IDLE,
	CONFIGURATION,
	SLEEP,
	CONTINUOUS,
	READ
};

typedef struct {
	SPI_TypeDef* spi_port;

	uint32_t cs_pin;
	GPIO_TypeDef* cs_pin_port;

	uint32_t reset_pin;
	GPIO_TypeDef* reset_pin_port;

	uint32_t start_pin;
	GPIO_TypeDef* start_pin_port;

	enum ADS1292R_ECG_STATE mode;

	uint8_t spi_buffer_rx[ADS1292R_SPI_BUFFER_SIZE_RX];
	uint8_t spi_buffer_tx[ADS1292R_SPI_BUFFER_SIZE_TX];
} ads1292r_handle_t;

void ads1292r_init(ads1292r_handle_t* handle);

void ecg_drdy_irq(ads1292r_handle_t* handle);

void ecg_dma_cplt_callback(ads1292r_handle_t* handle);

// TODO make number_reg 1 based

void ads1292r_spi_transmit_blocking(ads1292r_handle_t* handle, uint8_t *data, uint16_t size);

void ads1292r_chip_select(ads1292r_handle_t* handle, GPIO_PinState state);

void ads1292r_chip_reset(ads1292r_handle_t* handle, GPIO_PinState state);

void ads1292r_chip_start(ads1292r_handle_t* handle, GPIO_PinState state);

void ads1292r_write_command_blocking(ads1292r_handle_t* handle, uint8_t command);

void ads1292r_read_reg_blocking(ads1292r_handle_t* handle, uint8_t reg, uint8_t number_reg, uint8_t** data);

void ads1292r_read_reg_single_blocking(ads1292r_handle_t* handle, uint8_t reg, uint8_t** data);

void ads1292r_write_reg_blocking(ads1292r_handle_t* handle, uint8_t reg, uint8_t number_reg, uint8_t* data);

void ads1292r_write_reg_single_blocking(ads1292r_handle_t* handle, uint8_t reg, uint8_t data);

void ads1292r_start_cont_mode(ads1292r_handle_t* handle);
void ads1292r_stop_cont_mode(ads1292r_handle_t* handle);

void ads1292r_drdy(ads1292r_handle_t* handle);

void ads1292r_dma_irq(ads1292r_handle_t* handle, uint32_t *status, int32_t *ch1, int32_t *ch2);



//void ads1292r_drdy_ittr(void);
//
//int ads1292r_start_conversion(ads1292r_handle_t* handle);
//int ads1292r_stop_conversion(ads1292r_handle_t* handle);
//
//int ads1292r_read_reg(ads1292r_handle_t* handle, u_int8_t reg, u_int8_t number_reg, u_int8_t** data);
//
//int ads1292r_write_reg_buff(ads1292r_handle_t* handle, u_int8_t reg, u_int8_t number_reg);
//int ads1292r_write_reg(ads1292r_handle_t* handle, u_int8_t reg, u_int8_t number_reg, u_int8_t* data);
//
//int ads1292r_write_reg_single(ads1292r_handle_t* handle, u_int8_t reg, u_int8_t data);
//
//int ads1292r_write_command(ads1292r_handle_t* handle, u_int8_t command);
//
//int ads1292r_poll(ads1292r_handle_t* handle, u_int32_t* status, int32_t* ch1, int32_t* ch2);
//
//u_int8_t get_bit(u_int8_t byte, uint8_t index);
//
//void ads1292r_parse_status(u_int32_t status, u_int8_t* loff_status, u_int8_t* gpio_status);

#ifdef __cplusplus
}
#endif

#endif
