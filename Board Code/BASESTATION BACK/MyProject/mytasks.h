/*
 * mytasks.h
 *
 * Created: 4/27/2020 10:58:08 AM
 *  Author: yobro
 */ 
#include "driver_init.h"
#include "utils.h"

#ifndef MYTASKS_H_
#define MYTASKS_H_


void UART_task(void *p);
void PWM_task(void *p);
void STATE_task(void *p);
void Create_all_tasks();
void pwm_ramp();


#endif /* MYTASKS_H_ */