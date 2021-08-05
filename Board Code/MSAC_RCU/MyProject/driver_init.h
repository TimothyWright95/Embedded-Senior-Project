/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_adc_sync.h>

#include <hal_usart_sync.h>
#include <hal_pwm.h>
#include <hpl_tc_base.h>

extern struct adc_sync_descriptor Battery_lvl;

extern struct usart_sync_descriptor UART;

extern struct pwm_descriptor StepperDriver;

void Battery_lvl_PORT_init(void);
void Battery_lvl_CLOCK_init(void);
void Battery_lvl_init(void);

void UART_PORT_init(void);
void UART_CLOCK_init(void);
void UART_init(void);

void StepperDriver_PORT_init(void);
void StepperDriver_CLOCK_init(void);
void StepperDriver_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
