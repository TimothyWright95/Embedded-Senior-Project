/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

/**
 * Example of using Battery_lvl to generate waveform.
 */
void Battery_lvl_example(void)
{
	uint8_t buffer[2];

	adc_sync_enable_channel(&Battery_lvl, 0);

	while (1) {
		adc_sync_read_channel(&Battery_lvl, 0, buffer, 2);
	}
}

/**
 * Example of using UART to write "Hello World" using the IO abstraction.
 */
void UART_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&UART, &io);
	usart_sync_enable(&UART);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using StepperDriver.
 */
void StepperDriver_example(void)
{
	pwm_set_parameters(&StepperDriver, 10000, 5000);
	pwm_enable(&StepperDriver);
}
