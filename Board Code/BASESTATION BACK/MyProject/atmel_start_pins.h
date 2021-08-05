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

#define PA08 GPIO(GPIO_PORTA, 8)
#define Manual_1 GPIO(GPIO_PORTA, 9)
#define Manual_2 GPIO(GPIO_PORTA, 10)
#define Manual_3 GPIO(GPIO_PORTA, 11)
#define Right GPIO(GPIO_PORTA, 20)
#define Left GPIO(GPIO_PORTA, 21)
#define PA22 GPIO(GPIO_PORTA, 22)
#define PA23 GPIO(GPIO_PORTA, 23)
#define Manual_6 GPIO(GPIO_PORTB, 3)
#define Start_Cleaning GPIO(GPIO_PORTB, 8)
#define PB09 GPIO(GPIO_PORTB, 9)
#define PB12 GPIO(GPIO_PORTB, 12)
#define PB13 GPIO(GPIO_PORTB, 13)
#define Manual_7 GPIO(GPIO_PORTB, 16)
#define Direction GPIO(GPIO_PORTB, 17)
#define Manual_4 GPIO(GPIO_PORTB, 30)
#define Manual_5 GPIO(GPIO_PORTB, 31)

#endif // ATMEL_START_PINS_H_INCLUDED
