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
 * Example of using WindSensor to generate waveform.
 */
void WindSensor_example(void)
{
	uint8_t buffer[2];

	adc_sync_enable_channel(&WindSensor, 0);

	while (1) {
		adc_sync_read_channel(&WindSensor, 0, buffer, 2);
	}
}

/**
 * Example of using TempSensor to generate waveform.
 */
void TempSensor_example(void)
{
	uint8_t buffer[2];

	adc_sync_enable_channel(&TempSensor, 0);

	while (1) {
		adc_sync_read_channel(&TempSensor, 0, buffer, 2);
	}
}

/**
 * Example of using Scheduler.
 */
static struct calendar_os_alarm alarm1, alarm2;

static void alarm_cb(struct calendar_os_descriptor *const descr)
{
	/* alarm expired */
}
/**
 * The alarm task function(calendar_os_task) should be called
 * in user while-loop thread, so that the soft alarm could be processed and
 * the alarm_cb function could be called.
 */
void Scheduler_example_task(void)
{
	struct calendar_date_time dt;

	dt.date.year  = 2000;
	dt.date.month = 12;
	dt.date.day   = 31;

	dt.time.hour = 12;
	dt.time.min  = 59;
	dt.time.sec  = 59;

	calendar_os_set_date_time(&Scheduler, &dt);

	alarm1.cal_alarm.datetime.time.sec = 20;
	alarm1.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
	alarm1.cal_alarm.mode              = ONESHOT;

	calendar_os_set_alarm(&Scheduler, &alarm1, alarm_cb);

	alarm2.cal_alarm.datetime.time.sec = 40;
	alarm2.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
	alarm2.cal_alarm.mode              = REPEAT;
	calendar_os_set_alarm(&Scheduler, &alarm2, alarm_cb);

	while (1) {
		calendar_os_task(&Scheduler);
		/* do something with your code */
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
