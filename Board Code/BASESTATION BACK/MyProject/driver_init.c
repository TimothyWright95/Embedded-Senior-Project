/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>

#include <hpl_adc_base.h>
#include <hpl_adc_base.h>

struct adc_sync_descriptor WindSensor;

struct adc_sync_descriptor TempSensor;

struct calendar_os_descriptor Scheduler;

struct usart_sync_descriptor UART;

struct pwm_descriptor StepperDriver;

void WindSensor_PORT_init(void)
{

	// Disable digital pin circuitry
	gpio_set_pin_direction(PB09, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PB09, PINMUX_PB09B_ADC0_AIN3);
}

void WindSensor_CLOCK_init(void)
{
	hri_mclk_set_APBCMASK_ADC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, ADC0_GCLK_ID, CONF_GCLK_ADC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void WindSensor_init(void)
{
	WindSensor_CLOCK_init();
	WindSensor_PORT_init();
	adc_sync_init(&WindSensor, ADC0, _adc_get_adc_sync());
}

void TempSensor_PORT_init(void)
{

	// Disable digital pin circuitry
	gpio_set_pin_direction(PA08, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PA08, PINMUX_PA08B_ADC1_AIN10);
}

void TempSensor_CLOCK_init(void)
{
	hri_mclk_set_APBCMASK_ADC1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, ADC1_GCLK_ID, CONF_GCLK_ADC1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void TempSensor_init(void)
{
	TempSensor_CLOCK_init();
	TempSensor_PORT_init();
	adc_sync_init(&TempSensor, ADC1, _adc_get_adc_sync());
}

void Scheduler_CLOCK_init(void)
{
	hri_mclk_set_APBAMASK_RTC_bit(MCLK);
}

void Scheduler_init(void)
{
	Scheduler_CLOCK_init();
	calendar_os_init(&Scheduler, RTC);
}

void UART_PORT_init(void)
{

	gpio_set_pin_function(PA22, PINMUX_PA22C_SERCOM3_PAD0);

	gpio_set_pin_function(PA23, PINMUX_PA23C_SERCOM3_PAD1);
}

void UART_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM3_GCLK_ID_CORE, CONF_GCLK_SERCOM3_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM3_GCLK_ID_SLOW, CONF_GCLK_SERCOM3_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBCMASK_SERCOM3_bit(MCLK);
}

void UART_init(void)
{
	UART_CLOCK_init();
	usart_sync_init(&UART, SERCOM3, (void *)NULL);
	UART_PORT_init();
}

void StepperDriver_PORT_init(void)
{

	gpio_set_pin_function(PB12, PINMUX_PB12E_TC0_WO0);

	gpio_set_pin_function(PB13, PINMUX_PB13E_TC0_WO1);
}

void StepperDriver_CLOCK_init(void)
{
	hri_mclk_set_APBCMASK_TC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC0_GCLK_ID, CONF_GCLK_TC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void StepperDriver_init(void)
{
	StepperDriver_CLOCK_init();
	StepperDriver_PORT_init();
	pwm_init(&StepperDriver, TC0, _tc_get_pwm());
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA09

	// Set pin direction to input
	gpio_set_pin_direction(Manual_1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(Manual_1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(Manual_1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA10

	// Set pin direction to input
	gpio_set_pin_direction(Manual_2, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(Manual_2,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(Manual_2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA11

	// Set pin direction to input
	gpio_set_pin_direction(Manual_3, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(Manual_3,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(Manual_3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA20

	// Set pin direction to input
	gpio_set_pin_direction(Right, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(Right,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(Right, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA21

	// Set pin direction to input
	gpio_set_pin_direction(Left, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(Left,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(Left, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB03

	// Set pin direction to input
	gpio_set_pin_direction(Manual_6, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(Manual_6,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(Manual_6, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB08

	// Set pin direction to input
	gpio_set_pin_direction(Start_Cleaning, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(Start_Cleaning,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(Start_Cleaning, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB16

	// Set pin direction to input
	gpio_set_pin_direction(Manual_7, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(Manual_7,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(Manual_7, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB17

	gpio_set_pin_level(Direction,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(Direction, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(Direction, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB30

	// Set pin direction to input
	gpio_set_pin_direction(Manual_4, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(Manual_4,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(Manual_4, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB31

	// Set pin direction to input
	gpio_set_pin_direction(Manual_5, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(Manual_5,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(Manual_5, GPIO_PIN_FUNCTION_OFF);

	WindSensor_init();

	TempSensor_init();

	Scheduler_init();

	UART_init();

	StepperDriver_init();
}
