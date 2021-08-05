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
#define UP 0
#define DOWN 1
#define OFF 2


//responces
#define DONEMOVING 0
#define ALLDONE 0 
#define LOWBATTERY 0

void dock();
void send_message(int length);
void wait_for_responce();
void pwm(int dir);
int readbtn(int btn);
void reset_buffer(uint8_t *buffer, int length);
struct io_descriptor *io;
uint8_t UART_Buffer[100];
uint8_t UART_Send_Buffer[100];
int UART_Length = 0;

void STATE_task(void *p)
{
	int total_moves = 0;
	//setup
	usart_sync_get_io_descriptor(&UART, &io);
	usart_sync_enable(&UART);
	
	/*
	1 = undock
	2 = brush toggle
	3 = continue
	4 = move down
	5 = dock
	6 = move up infinite
	7 = move down infinite
	8 = full stop

*/
	UART_Send_Buffer[0] = 's';
	UART_Send_Buffer[1] = '0';
	UART_Send_Buffer[2] = '\0';
	send_message(3);

	while(1)
	{
		if(gpio_get_pin_level(down) == 0 ||gpio_get_pin_level(up) == 0 )
		{
			pwm(OFF);			
		}
		wait_for_responce();
		if(gpio_get_pin_level(down) == 0 ||gpio_get_pin_level(up) == 0 )
		{
			pwm(OFF);
		}
		if(UART_Buffer[0] == '1')
		{
			gpio_set_pin_level(Motor_ON_OFF, 1);
			os_sleep(1000);
			pwm(DOWN);
			os_sleep(18460*2);
			pwm(OFF); 
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '1';
			UART_Send_Buffer[2] = '\0';
			send_message(3);
		}
		else if(UART_Buffer[0] == '2')
		{
			gpio_toggle_pin_level(Motor_ON_OFF);
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '1';
			UART_Send_Buffer[2] = '\0';
			send_message(3);
		}
		else if(UART_Buffer[0] == '3')
		{
			pwm(DOWN);
			os_sleep(12308*total_moves*2);
			pwm(OFF);
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '1';
			UART_Send_Buffer[2] = '\0';
			send_message(3);
		}
		else if(UART_Buffer[0] == '4')
		{
			pwm(DOWN);
			for(int i = 0; i < 246 ; i++)
			{
				os_sleep(100);
				if(gpio_get_pin_level(down) == 0 ||gpio_get_pin_level(up) == 0 )
				{
					pwm(OFF);
					i=246;
				}
			}
			pwm(OFF);
			if(gpio_get_pin_level(down) == 0)
			{
				UART_Send_Buffer[0] = 's';
				UART_Send_Buffer[1] = '2';
				UART_Send_Buffer[2] = '\0';
			}
			else
			{	UART_Send_Buffer[0] = 's';
				UART_Send_Buffer[1] = '1';
				UART_Send_Buffer[2] = '\0';
			}
			send_message(3);
			total_moves = total_moves + 1;
		}
		else if(UART_Buffer[0] == '5')
		{
			pwm(UP);
			while(gpio_get_pin_level(up) == 1);
			pwm(OFF);
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '1';
			UART_Send_Buffer[2] = '\0';
			send_message(3);
		}
		else if(UART_Buffer[0] == '6')
		{
			int m_break = 0;
			pwm(UP);
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '1';
			UART_Send_Buffer[2] = '\0';
			send_message(3);
			while(gpio_get_pin_level(up) == 1 && m_break == 0)
			{
				wait_for_responce();
				if(UART_Buffer[0] == '0')
				{
					m_break = 1;
				}
			}
			pwm(OFF);
		}
		else if(UART_Buffer[0] == '7')
		{
			int m_break = 0;
			pwm(DOWN);
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '1';
			UART_Send_Buffer[2] = '\0';
			send_message(3);
			while(gpio_get_pin_level(down) == 1 && m_break == 0)
			{
				wait_for_responce();
				if(UART_Buffer[0] == '0')
				{
					m_break = 1;
				}
			}
			pwm(OFF);
		}
		else
		{
			pwm(OFF);
		}
		if(read_battery() == 1)
		{
			UART_Send_Buffer[0] = 's';
			UART_Send_Buffer[1] = '3';
			UART_Send_Buffer[2] = '\0';
			send_message(3);
		}
		os_sleep(100);
		
	}
	
	
}

void Create_all_tasks()
{
		
		if (xTaskCreate(
		STATE_task, "STATE", TASK_STACK_SIZE*20, NULL, TASK_STACK_PRIORITY, xCreatedTask[3])
		!= pdPASS) {
			while (1) {
				;
			}
		}

		vTaskStartScheduler();

		return;
}

void send_message(int length)
{
	io_write(io, (uint8_t *)&UART_Send_Buffer, length);
	os_sleep(100);
	
	do 
	{
		
		if(gpio_get_pin_level(down) == 0 ||gpio_get_pin_level(up) == 0 )
		{
			pwm(OFF);
		}
		if(usart_sync_is_rx_not_empty(&UART) !=0)
		{
			UART_Length = io_read(io, (uint8_t *)&UART_Buffer, 1);
		}
		os_sleep(100);
	} while (UART_Buffer[0] != 's');
	reset_buffer(UART_Buffer, UART_Length);
}
void wait_for_responce()
{
	UART_Length = 0;
	int offset = 0;
	UART_Buffer[0] = '0';
	//while(UART_Buffer[0] == '0' || UART_Buffer[0] == '\0')
	//{
		uint8_t temp[2] = "r\0";
		io_write(io, (uint8_t *)&temp, 2);
		while(usart_sync_is_rx_not_empty(&UART) ==0)
		{
			if(gpio_get_pin_level(down) == 0 ||gpio_get_pin_level(up) == 0 )
			{
				pwm(OFF);			
			}
		} //ERROR Potential breaking loop if the command is not received by the website module
		for(int i = 0; i < 1000; i++)
		{
			if(usart_sync_is_rx_not_empty(&UART) !=0)
			{
				UART_Length = io_read(io, (uint8_t *)&UART_Buffer + offset, 1);
				offset++;
				i = 0;
				if(gpio_get_pin_level(down) == 0 ||gpio_get_pin_level(up) == 0 )
				{
					pwm(OFF);			
				}
			}
			os_sleep(1);	
			if(gpio_get_pin_level(down) == 0 ||gpio_get_pin_level(up) == 0 )
			{
				pwm(OFF);			
			}		
		}
		offset = 0;
		if(UART_Buffer[0] == '0')
			reset_buffer(&UART_Buffer, UART_Length);
	//}
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
	
		pwm_set_parameters(&StepperDriver, 1600, 800);	
		pwm_enable(&StepperDriver);
		os_sleep(10);
		
}

void reset_buffer(uint8_t *buffer, int length)
{
	for(int i = 0; i < length; i++)
	{
		buffer[i] = '\0';
	}
}
int read_battery()
{
	uint8_t battery[2];
	
	adc_sync_enable_channel(&Battery_lvl, 0);
	adc_sync_read_channel(&Battery_lvl, 0, battery, 2);
	double battery_lvl = ((uint16_t)battery[1] << 8) | battery[0];
	adc_sync_disable_channel(&Battery_lvl, 0);

	if(battery_lvl < 1365)
	{	
		adc_sync_enable_channel(&Battery_lvl, 0);
		adc_sync_read_channel(&Battery_lvl, 0, battery, 2);
		battery_lvl = ((uint16_t)battery[1] << 8) | battery[0];
		adc_sync_disable_channel(&Battery_lvl, 0);
		if(battery_lvl < 1365)
			return 1;
	}
	else
		return 0;
}

