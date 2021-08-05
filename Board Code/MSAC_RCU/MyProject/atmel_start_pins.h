/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMC21 has 9 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7
#define GPIO_PIN_FUNCTION_I 8

#define up GPIO(GPIO_PORTA, 20)
#define down GPIO(GPIO_PORTA, 21)
#define PA22 GPIO(GPIO_PORTA, 22)
#define PA23 GPIO(GPIO_PORTA, 23)
#define PB09 GPIO(GPIO_PORTB, 9)
#define PB12 GPIO(GPIO_PORTB, 12)
#define PB13 GPIO(GPIO_PORTB, 13)
#define Direction GPIO(GPIO_PORTB, 14)
#define Motor_ON_OFF GPIO(GPIO_PORTB, 15)

#endif // ATMEL_START_PINS_H_INCLUDED
