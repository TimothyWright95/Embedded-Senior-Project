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

struct adc_sync_descriptor Battery_lvl;

struct usart_sync_descriptor UART;

struct pwm_descriptor StepperDriver;

void Battery_lvl_PORT_init(void)
{

	// Disable digital pin circuitry
	gpio_set_pin_direction(PB09, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PB09, PINMUX_PB09B_ADC0_AIN3);
}

void Battery_lvl_CLOCK_init(void)
{
	hri_mclk_set_APBCMASK_ADC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, ADC0_GCLK_ID, CONF_GCLK_ADC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void Battery_lvl_init(void)
{
	Battery_lvl_CLOCK_init();
	Battery_lvl_PORT_init();
	adc_sync_init(&Battery_lvl, ADC0, _adc_get_adc_sync());
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

	// GPIO on PA20

	// Set pin direction to input
	gpio_set_pin_direction(up, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(up,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(up, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA21

	// Set pin direction to input
	gpio_set_pin_direction(down, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(down,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(down, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB14

	gpio_set_pin_level(Direction,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(Direction, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(Direction, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB15

	gpio_set_pin_level(Motor_ON_OFF,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(Motor_ON_OFF, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(Motor_ON_OFF, GPIO_PIN_FUNCTION_OFF);

	Battery_lvl_init();

	UART_init();

	StepperDriver_init();
}
