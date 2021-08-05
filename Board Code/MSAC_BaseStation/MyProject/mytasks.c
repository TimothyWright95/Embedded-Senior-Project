/*
 * mytasks.c
 *
 * Created: 4/27/2020 10:57:52 AM
 *  Author: yobro
 */ 
#include "mytasks.h"
#include <string.h>
#define TASK_STACK_SIZE (128 / sizeof(portSTACK_TYPE))
#define TASK_STACK_PRIORITY (tskIDLE_PRIORITY + 1)
static TaskHandle_t      xCreatedTask[10];
#define UNDOCK 0
#define DOCK 1
#define LEFT 0
#define RIGHT 1
#define OFF 2
//messages to send
#define BRUSH_ON "1"
#define CONTINUE "2"
#define MOVEDOWN "3"

//responces
#define DONEMOVING 0
#define ALLDONE 0 
#define LOWBATTERY 0

int check_schedule();
void manual_mode();
void idle(int sf, int lb);
int check_sensors();
void dock();
void send_message(int length);
void wait_for_responce();
void pwm(int dir);
int readbtn(int btn);
void reset_buffer(uint8_t *buffer, int length);
char sched[100] = {0};
struct io_descriptor *io;
uint8_t UART_Buffer[100];
uint8_t UART_Send_Buffer[100];
int UART_Length = 0;

/*
char sched[100] = {0};
struct io_descriptor *io;
uint8_t UART_Buffer[100];
uint8_t UART_Send_Buffer[100];
int UART_Length = 0;
*/
void STATE_task(void *p)
{
	int responce = 0;
	int sensor_failure = 0;
	int low_bat = 0;
	int continuing = 0;
	int done = 0;
	int dir = RIGHT;
	//setup
	usart_sync_get_io_descriptor(&UART, &io);
	usart_sync_enable(&UART);
	
	UART_Send_Buffer[0] = 's';
	UART_Send_Buffer[1] = '0';
	UART_Send_Buffer[2] = '\0';
	send_message(3);
	
	while(1)
	{
		////////
		idle(sensor_failure, low_bat);
		int sensor_failure = check_sensors();
		if(sensor_failure == 0)
		{
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '1';
			UART_Send_Buffer[2] = '\0';
			send_message(3); //************************************************************************************* UNDOCK
			//send message to RCU move down(92308 steps) 1khz out on pwm for os_sleep(9230)       *************************************************************************************************
			// 6154 steps/inch
			wait_for_responce();

			if(continuing)
			{
				UART_Send_Buffer[0] = 's';
				UART_Send_Buffer[1] = '3';
				UART_Send_Buffer[2] = '\0';
				send_message(3);//************************************************************************ CONTINUE
				wait_for_responce();
			}
			while(done == 0)
			{
				pwm(dir);
				if(dir == RIGHT)
				{
					while(readbtn(Right) == 1 )
					{
						os_sleep(100);
					}
				}
				else if(dir == LEFT)
				{
					while(readbtn(Left) == 1 )
					{
						os_sleep(100);
					}
				}
				pwm(OFF);
				UART_Send_Buffer[0] = 's';
				UART_Send_Buffer[1] = '4';
				UART_Send_Buffer[2] = '\0';
				send_message(3);//************************************************************************ MOVEDOWN
				wait_for_responce();
				if(UART_Buffer[0] == '1')
				{
					if(dir == LEFT) dir = RIGHT;
					else dir = LEFT;
				}
				else if(UART_Buffer[0] == '2')
				{
					done = 1;
					dock();
					
				}
				else if(UART_Buffer[0] == '3')
				{
					continuing = 1;
					low_bat = 1;
					dir = RIGHT;
					dock();
				}
			}
		}
		
		
	}
	
	
}

void Create_all_tasks()
{
		//Basestation
		if (xTaskCreate(
		STATE_task, "STATE", TASK_STACK_SIZE*20, NULL, TASK_STACK_PRIORITY, xCreatedTask[3])
		!= pdPASS) {
			while (1) {
				;
			}
		}
		
		//RCU
		/*
		if (xTaskCreate(
		STATE_task, "RCU", TASK_STACK_SIZE*20, NULL, TASK_STACK_PRIORITY, xCreatedTask[3])
		!= pdPASS) {
			while (1) {
				;
			}
		}
		
		
		*/

		vTaskStartScheduler();

		return;
}
void idle(int sf, int lb)//sf= sensor failure, lb = low battery
{
	if(lb)
	{
		//low battery sleep 1 hour
		for(int i = 0; i < 3600; i ++)
		{
			os_sleep(2000);
		}
		return;	
	}
	if(sf)
	{
		while(check_sensors() == 1);
		os_sleep(5000);
		return;
	}
	while(1)
	{ 
		if(readbtn(Manual_7) == 0)
		{
			while(readbtn(Manual_7) == 0);
			manual_mode();
		}
		if(check_schedule() == 1)
		{
			return;
		}
		if(readbtn(Start_Cleaning) == 0)
		{
			return;
		}
		os_sleep(10);
	}
}
int check_sensors() //DONE
{
		uint8_t temp[2];
		uint8_t wind[2];
		
		adc_sync_enable_channel(&WindSensor, 0);
		adc_sync_read_channel(&WindSensor, 0, wind, 2);
		double w = ((uint16_t)wind[1] << 8) | wind[0];
		w = (((w * 3300.0 / 4095.0) - 400) / 1600) * 50;
		adc_sync_disable_channel(&WindSensor, 0);
		
		adc_sync_enable_channel(&TempSensor, 0);
		adc_sync_read_channel(&TempSensor, 0, temp, 2);
		double c = ((uint16_t)temp[1] << 8) | temp[0];
		c  = c  * (3300/4095.0);
		c  = ( c  - 500) / 10;
		double f = (c *9 / 5)+32;
		adc_sync_disable_channel(&TempSensor, 0);
		if(f < 80 && w < 8)
			return 0;
		else
			return 1;
}
void dock()
{
	pwm(LEFT);
	while(readbtn(Left) == 1);
	pwm(OFF);
	UART_Send_Buffer[0] = 's';
	UART_Send_Buffer[1] = '5';
	UART_Send_Buffer[2] = '\0';
	send_message(3);//************************************************************************ DOCK
	
}
void send_message(int length)
{
	io_write(io, (uint8_t *)&UART_Send_Buffer, length);
	os_sleep(100);
	
	do 
	{
		if(usart_sync_is_rx_not_empty(&UART) !=0)
		{
			UART_Length = io_read(io, (uint8_t *)&UART_Buffer, 1);
		}
		os_sleep(500);
	} while (UART_Buffer[0] != 's');
	reset_buffer(UART_Buffer, UART_Length);
}
void wait_for_responce()
{
	UART_Length = 0;
	int offset = 0;
	UART_Buffer[0] = '0';
	while(UART_Buffer[0] == '0' || UART_Buffer[0] == '\0')
	{
		uint8_t temp[1] = {0};
		temp[0] = 'r';
		io_write(io, (uint8_t *)&temp, 1);
		for(int i = 0; i < 1000; i++)
		{
			if(usart_sync_is_rx_not_empty(&UART) !=0)
			{
				UART_Length = io_read(io, (uint8_t *)&UART_Buffer + offset, 1);
				offset++;
				i = 0;
			}
			os_sleep(1);			
		}
		offset = 0;
		if(UART_Buffer[0] == '0' || UART_Buffer[0] == 'E')
			reset_buffer(&UART_Buffer, UART_Length);
	}
}
void pwm(int dir)
{
	if(dir == 2)
	{
		pwm_disable(&StepperDriver);
		return;
	}
		pwm_disable(&StepperDriver);
		os_sleep(100);
		gpio_set_pin_level(Direction, dir);
		pwm_ramp();
}
int readbtn(int btn)
{
	return gpio_get_pin_level(btn);
	
}
void pwm_ramp()
{
	int pwm_params[16] = {2000, 1333, 1000, 800, 667, 571, 500, 444, 400, 364, 333, 308, 286, 267, 250, 235};
		pwm_set_parameters(&StepperDriver, pwm_params[3], pwm_params[3]/2);
		pwm_enable(&StepperDriver);
		os_sleep(10);
		pwm_set_parameters(&StepperDriver, pwm_params[4], pwm_params[4]/2);		
		os_sleep(10);
		pwm_set_parameters(&StepperDriver, pwm_params[5], pwm_params[5]/2);
		os_sleep(10);
		pwm_set_parameters(&StepperDriver, pwm_params[6], pwm_params[6]/2);
		os_sleep(10);
		pwm_set_parameters(&StepperDriver, pwm_params[7], pwm_params[7]/2);
		os_sleep(10);//*/
		//pwm_set_parameters(&StepperDriver, pwm_params[8], pwm_params[8]/2);
		//os_sleep(10);//*/
		//pwm_set_parameters(&StepperDriver, pwm_params[9], pwm_params[9]/2);
		//os_sleep(10);
}
void manual_mode()
{
		//int i[7];
	//while(1)
	//{
//
		//i[0] = gpio_get_pin_level(Manual_1);
		//i[1] = gpio_get_pin_level(Manual_2);
		//i[2] = gpio_get_pin_level(Manual_3);
		//i[3] = gpio_get_pin_level(Manual_4);
		//i[4] = gpio_get_pin_level(Manual_5);
		//i[5] = gpio_get_pin_level(Manual_6);
		//i[6] = gpio_get_pin_level(Manual_7);
	//}
	/*
		Manual_1	Left
		Manual_2	Right
		Manual_3	Up
		Manual_4	Down
		Manual_5	Brush on/off
		Manual_6	Dock
		Manual_7	Manual on
	
	*/
	int dir = RIGHT;
	while(gpio_get_pin_level(Manual_6) != 0)
	{
		

		if(dir == RIGHT)
		{
			if (gpio_get_pin_level(Right) == 0 )
			{
				pwm(OFF);
			}
		} 
		if(dir == LEFT)
		{
			if(gpio_get_pin_level(Left)== 0)
			{
				pwm(OFF);
			}
		}
		if(gpio_get_pin_level(Manual_1) == 0 && gpio_get_pin_level(Left)== 1)
		{
			dir = LEFT;
			pwm(OFF);
			pwm(LEFT);
			while(gpio_get_pin_level(Manual_1) == 0)
			{
				if(gpio_get_pin_level(Left)== 0)
				{
					pwm(OFF);
				}
			}
		}
		else if(gpio_get_pin_level(Manual_2) == 0 && gpio_get_pin_level(Right) == 1)
		{
			dir = RIGHT;
			pwm(OFF);
			pwm(RIGHT);
			while(gpio_get_pin_level(Manual_2) == 0)
			{
				if (gpio_get_pin_level(Right) == 0 )
				{
					pwm(OFF);
				}
			}
		}
		else if(gpio_get_pin_level(Manual_3) == 0)
		{
			pwm(OFF);
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '6';
			UART_Send_Buffer[2] = '\0';
			send_message(3);//************************************************************************ MOVEUP_INFINITE	
			while(gpio_get_pin_level(Manual_3) == 0);
		}
		else if(gpio_get_pin_level(Manual_4) == 0)
		{
			pwm(OFF);
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '7';
			UART_Send_Buffer[2] = '\0';
			send_message(3);//************************************************************************ MOVEDOWN_INFINITE
			while(gpio_get_pin_level(Manual_4) == 0);
		}
		else if(gpio_get_pin_level(Manual_5) == 0)
		{
			pwm(OFF);
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '2';
			UART_Send_Buffer[2] = '\0';
			send_message(3);//************************************************************************ ToggleBrush
			while(gpio_get_pin_level(Manual_5) == 0);
		}
		else if(gpio_get_pin_level(Manual_7) == 0)
		{
			pwm(OFF);
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '8';
			UART_Send_Buffer[2] = '\0';
			send_message(3);//************************************************************************ FULL_STOP
			while(gpio_get_pin_level(Manual_7) == 0);
		}
	}
	pwm(OFF);
	UART_Send_Buffer[0] = 's';
	UART_Send_Buffer[1] = '8';
	UART_Send_Buffer[2] = '\0';
	send_message(3);//************************************************************************ FULL_STOP
	dock();
}
int check_schedule()
{
	
	return 0;
}
void reset_buffer(uint8_t *buffer, int length)
{
	for(int i = 0; i < length; i++)
	{
		buffer[i] = '\0';
	}
}

