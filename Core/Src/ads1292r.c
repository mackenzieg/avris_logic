#include "ads1292r.h"
#include "ads129x_reg.h"

void ads1292r_init(ads1292r_handle_t* handle)
{
	handle->mode = IDLE;

	// Delay for 32ms
	for (int i = 0; i < 32*7000; ++i){
		asm("nop");
	}

	// Stop conversions
	ads1292r_chip_start(handle, GPIO_PIN_RESET);
	// Reset device
	ads1292r_chip_reset(handle, GPIO_PIN_RESET);

	// Allow chip to go fully into reset
	//DWT_Delay_ms(1);
	for (int i = 0; i < 7000; ++i){
		asm("nop");
	}

	ads1292r_chip_reset(handle, GPIO_PIN_SET);

	// Allow chip to fully leave reset
	//DWT_Delay_ms(1);
	for (int i = 0; i < 32*7000; ++i){
		asm("nop");
	}

	// Get device ready for commands
	ads1292r_chip_select(handle, GPIO_PIN_RESET);

	// Turn off continuous conversion
	ads1292r_write_command_blocking(handle, SDATAC);

	for (int i = 0; i < 7000; ++i){
		asm("nop");
	}

	uint8_t *data = NULL;
	ads1292r_read_reg_single_blocking(handle, ID, &data);

	// Make sure device is connected
	if (data[0] == 0)
	{
		LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
		//TODO error
	}

	// Make sure device is ADS1292R
	if (data[0] != ID_ADS1292R)
	{
		LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
		//TODO error
	}

	// TODO move these into struct controlled configurations
	// TODO fix all these numbering
	ads1292r_write_reg_single_blocking(handle, CONFIG2, CONFIG2_CONST | PDB_REFBUF | INT_TEST | TEST_FREQ);

	ads1292r_write_reg_single_blocking(handle, CONFIG1, CONFIG1_CONST | SAMP_1_KSPS);

	ads1292r_write_reg_single_blocking(handle, LOFF, LOFF_CONST);

	ads1292r_write_reg_single_blocking(handle, CH1SET, CH1SET_CONST | GAIN_6X | TEST_SIGNAL);
	ads1292r_write_reg_single_blocking(handle, CH2SET, CH2SET_CONST | GAIN_6X | TEST_SIGNAL);


	ads1292r_write_reg_single_blocking(handle, RLD_SENS, RLD_SENS_CONST | PDB_RLD | RLD2N | RLD2P);

	ads1292r_write_reg_single_blocking(handle, LOFF_SENS, LOFF_SENS_CONST);

	ads1292r_write_reg_single_blocking(handle, LOFF_STAT, LOFF_STAT_CONST);

	ads1292r_write_reg_single_blocking(handle, RESP1, RESP1_CONST | RESP_DEMOD_EN | RESP_MOD_EN);

	ads1292r_write_reg_single_blocking(handle, RESP2, RESP1_CONST | RLDREF_INT);

	// Disable chip select
	ads1292r_chip_select(handle, GPIO_PIN_SET);
}

void ads1292r_chip_select(ads1292r_handle_t* handle, GPIO_PinState state)
{
	if (state == GPIO_PIN_SET) {
		LL_GPIO_SetOutputPin(handle->cs_pin_port, handle->cs_pin);
	} else {
		LL_GPIO_ResetOutputPin(handle->cs_pin_port, handle->cs_pin);
	}
}

void ads1292r_chip_reset(ads1292r_handle_t* handle, GPIO_PinState state)
{
	if (state == GPIO_PIN_SET) {
		LL_GPIO_SetOutputPin(handle->reset_pin_port, handle->reset_pin);
	} else {
		LL_GPIO_ResetOutputPin(handle->reset_pin_port, handle->reset_pin);
	}
}

void ads1292r_chip_start(ads1292r_handle_t* handle, GPIO_PinState state)
{
	if (state == GPIO_PIN_SET) {
		LL_GPIO_SetOutputPin(handle->start_pin_port, handle->start_pin);
	} else {
		LL_GPIO_ResetOutputPin(handle->start_pin_port, handle->start_pin);
	}
}

void ads1292r_write_command_blocking(ads1292r_handle_t* handle, uint8_t command)
{
	handle->spi_buffer_tx[0] = command;

	spi_transmit(handle->spi_port, handle->spi_buffer_tx, 1);
}

// TODO make number_reg 1 based instead of 0
void ads1292r_read_reg_blocking(ads1292r_handle_t* handle, uint8_t reg, uint8_t number_reg, uint8_t** data)
{
	handle->spi_buffer_tx[0] = RREG | reg;
	handle->spi_buffer_tx[1] = number_reg;
	// Automatically zero out this part
	handle->spi_buffer_tx[2] = 0;

	// Size is 2 header bits + number of register (0 based) + 1 to 1 base
	spi_transmit_receive(handle->spi_port, handle->spi_buffer_tx, handle->spi_buffer_rx, 2 + number_reg + 1);

	*data = handle->spi_buffer_rx + 2;
}

void ads1292r_read_reg_single_blocking(ads1292r_handle_t* handle, uint8_t reg, uint8_t** data)
{
	ads1292r_read_reg_blocking(handle, reg, 0, data);
}

void ads1292r_write_reg_blocking(ads1292r_handle_t* handle, uint8_t reg, uint8_t number_reg, uint8_t* data)
{
	handle->spi_buffer_tx[0] = WREG | reg;
	handle->spi_buffer_tx[1] = number_reg;

	memcpy(handle->spi_buffer_tx + 1, data, number_reg + 1);

	spi_transmit(handle->spi_port, handle->spi_buffer_tx, 2 + number_reg + 1);
}

void ads1292r_write_reg_single_blocking(ads1292r_handle_t* handle, uint8_t reg, uint8_t data)
{
	ads1292r_write_reg_blocking(handle, reg, 0, &data);
}

void ads1292r_start_cont_mode(ads1292r_handle_t* handle)
{
	if (handle->mode == CONTINUOUS)
		return;

	handle->mode = CONTINUOUS;

	ads1292r_write_command_blocking(handle, RDATAC);

	ads1292r_chip_start(handle, GPIO_PIN_SET);
}

void ads1292r_stop_cont_mode(ads1292r_handle_t* handle)
{
	if (handle->mode != CONTINUOUS)
		return;

	handle->mode = SLEEP;

	ads1292r_chip_start(handle, GPIO_PIN_RESET);

	ads1292r_write_command_blocking(handle, SDATAC);
}

void ads1292r_drdy(ads1292r_handle_t* handle)
{
	if (handle->mode != CONTINUOUS)
		return;

	memset(handle->spi_buffer_tx, 0, 9);

	handle->mode = READ;
	// Enable chip select
	ads1292r_chip_select(handle, GPIO_PIN_RESET);
	spi_transmit_receive_dma(handle->spi_port, DMA1, LL_DMA_CHANNEL_3, LL_DMA_CHANNEL_2,
							 handle->spi_buffer_tx, handle->spi_buffer_rx, 9);
}

void ads1292r_dma_irq(ads1292r_handle_t* handle, uint32_t *status, int32_t *ch1, int32_t *ch2)
{
	// TODO maybe should put a wait here
	LL_SPI_Disable(handle->spi_port);

	// Last read is complete go back to continuous
	handle->mode = CONTINUOUS;
	ads1292r_chip_select(handle, GPIO_PIN_SET);

	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);

	(*status) = (handle->spi_buffer_rx[0] << 16 | handle->spi_buffer_rx[1] << 8 | handle->spi_buffer_rx[2] << 0);

	(*ch1)    = (handle->spi_buffer_rx[3] << 24 | handle->spi_buffer_rx[4] << 16 | handle->spi_buffer_rx[5] << 8) >> 8;
	(*ch2)    = (handle->spi_buffer_rx[6] << 24 | handle->spi_buffer_rx[7] << 16 | handle->spi_buffer_rx[8] << 8) >> 8;
}
