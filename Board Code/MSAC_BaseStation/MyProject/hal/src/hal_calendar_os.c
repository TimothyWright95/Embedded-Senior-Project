/**
 * \file
 *
 * \brief Generic CALENDAR functionality implementation.
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include "hal_calendar_os.h"
#include <utils.h>
#include <utils_assert.h>
#include <hal_atomic.h>

#define CALENDAR_VERSION 0x00000001u
#define SECS_IN_LEAP_YEAR 31622400
#define SECS_IN_NON_LEAP_YEAR 31536000
#define SECS_IN_31DAYS 2678400
#define SECS_IN_30DAYS 2592000
#define SECS_IN_29DAYS 2505600
#define SECS_IN_28DAYS 2419200
#define SECS_IN_DAY 86400
#define SECS_IN_HOUR 3600
#define SECS_IN_MINUTE 60

/** \brief leapyear check
 *  \retval false  not leapyear.
 *  \retval true  leapyear.
 */
static bool leap_year(uint16_t year)
{
	if (year & 3) {
		return false;
	} else {
		return true;
	}
}

/** \brief calculate the seconds in specified year/month
 *  \retval 0  month error.
 */
static uint32_t get_secs_in_month(uint32_t year, uint8_t month)
{
	uint32_t sec_in_month = 0;

	if (leap_year(year)) {
		switch (month) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			sec_in_month = SECS_IN_31DAYS;
			break;
		case 2:
			sec_in_month = SECS_IN_29DAYS;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			sec_in_month = SECS_IN_30DAYS;
			break;
		default:
			break;
		}
	} else {
		switch (month) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			sec_in_month = SECS_IN_31DAYS;
			break;
		case 2:
			sec_in_month = SECS_IN_28DAYS;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			sec_in_month = SECS_IN_30DAYS;
			break;
		default:
			break;
		}
	}

	return sec_in_month;
}

/** \brief convert timestamp to date/time
 */
static int32_t convert_timestamp_to_datetime(uint32_t ts, struct calendar_date_time *dt)
{
	uint32_t tmp, sec_in_year, sec_in_month;
	uint32_t tmp_year    = DEFAULT_BASE_YEAR;
	uint8_t  tmp_month   = 1;
	uint8_t  tmp_day     = 1;
	uint8_t  tmp_hour    = 0;
	uint8_t  tmp_minutes = 0;

	tmp = ts;

	/* Find year */
	while (true) {
		sec_in_year = leap_year(tmp_year) ? SECS_IN_LEAP_YEAR : SECS_IN_NON_LEAP_YEAR;

		if (tmp >= sec_in_year) {
			tmp -= sec_in_year;
			tmp_year++;
		} else {
			break;
		}
	}
	/* Find month of year */
	while (true) {
		sec_in_month = get_secs_in_month(tmp_year, tmp_month);

		if (tmp >= sec_in_month) {
			tmp -= sec_in_month;
			tmp_month++;
		} else {
			break;
		}
	}
	/* Find day of month */
	while (true) {
		if (tmp >= SECS_IN_DAY) {
			tmp -= SECS_IN_DAY;
			tmp_day++;
		} else {
			break;
		}
	}
	/* Find hour of day */
	while (true) {
		if (tmp >= SECS_IN_HOUR) {
			tmp -= SECS_IN_HOUR;
			tmp_hour++;
		} else {
			break;
		}
	}
	/* Find minute in hour */
	while (true) {
		if (tmp >= SECS_IN_MINUTE) {
			tmp -= SECS_IN_MINUTE;
			tmp_minutes++;
		} else {
			break;
		}
	}

	dt->date.year  = tmp_year;
	dt->date.month = tmp_month;
	dt->date.day   = tmp_day;
	dt->time.hour  = tmp_hour;
	dt->time.min   = tmp_minutes;
	dt->time.sec   = tmp;

	return 0;
}

/** \brief convert date/time to timestamp
 *  \return timestamp
 */
static uint32_t convert_datetime_to_timestamp(struct calendar_date_time *dt)
{
	uint32_t tmp = 0;
	uint32_t i   = 0;
	uint8_t  year, month, day, hour, minutes, seconds;

	year    = dt->date.year - DEFAULT_BASE_YEAR;
	month   = dt->date.month;
	day     = dt->date.day;
	hour    = dt->time.hour;
	minutes = dt->time.min;
	seconds = dt->time.sec;

	/* tot up year field */
	for (i = 0; i < year; ++i) {
		if (leap_year(DEFAULT_BASE_YEAR + i)) {
			tmp += SECS_IN_LEAP_YEAR;
		} else {
			tmp += SECS_IN_NON_LEAP_YEAR;
		}
	}

	/* tot up month field */
	for (i = 1; i < month; ++i) {
		tmp += get_secs_in_month(dt->date.year, i);
	}

	/* tot up day/hour/minute/second fields */
	tmp += (day - 1) * SECS_IN_DAY;
	tmp += hour * SECS_IN_HOUR;
	tmp += minutes * SECS_IN_MINUTE;
	tmp += seconds;

	return tmp;
}

/** \brief calibrate timestamp to make desired timestamp ahead of current timestamp
 */
static void calibrate_timestamp(struct calendar_os_alarm *alarm, struct calendar_os_alarm *current_dt)
{
	uint32_t alarm_ts;
	uint32_t current_ts = current_dt->cal_alarm.timestamp;

	alarm_ts = alarm->cal_alarm.timestamp;

	/* calibrate timestamp */
	switch (alarm->cal_alarm.option) {
	case CALENDAR_ALARM_MATCH_SEC:

		if (alarm_ts <= current_ts) {
			alarm_ts += SECS_IN_MINUTE;
		}

		break;
	case CALENDAR_ALARM_MATCH_MIN:

		if (alarm_ts <= current_ts) {
			alarm_ts += SECS_IN_HOUR;
		}

		break;
	case CALENDAR_ALARM_MATCH_HOUR:

		if (alarm_ts <= current_ts) {
			alarm_ts += SECS_IN_DAY;
		}

		break;
	case CALENDAR_ALARM_MATCH_DAY:

		if (alarm_ts <= current_ts) {
			alarm_ts += get_secs_in_month(current_dt->cal_alarm.datetime.date.year,
			                              current_dt->cal_alarm.datetime.date.month);
		}

		break;
	case CALENDAR_ALARM_MATCH_MONTH:

		if (alarm_ts <= current_ts) {
			if (leap_year(current_dt->cal_alarm.datetime.date.year)) {
				alarm_ts += SECS_IN_LEAP_YEAR;
			} else {
				alarm_ts += SECS_IN_NON_LEAP_YEAR;
			}
		}

		break;
	/* do nothing for year match */
	case CALENDAR_ALARM_MATCH_YEAR:
	default:
		break;
	}

	/* desired timestamp after calibration */
	alarm->cal_alarm.timestamp = alarm_ts;
}

/** \brief complete alarm to absolute date/time, then fill up the timestamp
 */
static void fill_alarm(struct calendar_os_descriptor *const calendar, struct calendar_os_alarm *alarm)
{
	struct calendar_os_alarm current_dt;
	uint32_t                 tmp, current_ts;

	/* get current date/time */
	current_ts = _calendar_get_counter(&calendar->device);
	convert_timestamp_to_datetime(current_ts, &current_dt.cal_alarm.datetime);

	current_dt.cal_alarm.timestamp = current_ts;

	/* complete alarm */
	switch (alarm->cal_alarm.option) {
	case CALENDAR_ALARM_MATCH_SEC:
		alarm->cal_alarm.datetime.date.year  = current_dt.cal_alarm.datetime.date.year;
		alarm->cal_alarm.datetime.date.month = current_dt.cal_alarm.datetime.date.month;
		alarm->cal_alarm.datetime.date.day   = current_dt.cal_alarm.datetime.date.day;
		alarm->cal_alarm.datetime.time.hour  = current_dt.cal_alarm.datetime.time.hour;
		alarm->cal_alarm.datetime.time.min   = current_dt.cal_alarm.datetime.time.min;
		break;
	case CALENDAR_ALARM_MATCH_MIN:
		alarm->cal_alarm.datetime.date.year  = current_dt.cal_alarm.datetime.date.year;
		alarm->cal_alarm.datetime.date.month = current_dt.cal_alarm.datetime.date.month;
		alarm->cal_alarm.datetime.date.day   = current_dt.cal_alarm.datetime.date.day;
		alarm->cal_alarm.datetime.time.hour  = current_dt.cal_alarm.datetime.time.hour;
		break;
	case CALENDAR_ALARM_MATCH_HOUR:
		alarm->cal_alarm.datetime.date.year  = current_dt.cal_alarm.datetime.date.year;
		alarm->cal_alarm.datetime.date.month = current_dt.cal_alarm.datetime.date.month;
		alarm->cal_alarm.datetime.date.day   = current_dt.cal_alarm.datetime.date.day;
		break;
	case CALENDAR_ALARM_MATCH_DAY:
		alarm->cal_alarm.datetime.date.year  = current_dt.cal_alarm.datetime.date.year;
		alarm->cal_alarm.datetime.date.month = current_dt.cal_alarm.datetime.date.month;
		break;
	case CALENDAR_ALARM_MATCH_MONTH:
		alarm->cal_alarm.datetime.date.year = current_dt.cal_alarm.datetime.date.year;
		break;
	case CALENDAR_ALARM_MATCH_YEAR:
		break;
	default:
		break;
	}

	/* fill up the timestamp */
	tmp                        = convert_datetime_to_timestamp(&alarm->cal_alarm.datetime);
	alarm->cal_alarm.timestamp = tmp;

	/* calibrate the timestamp */
	calibrate_timestamp(alarm, &current_dt);
	convert_timestamp_to_datetime(alarm->cal_alarm.timestamp, &alarm->cal_alarm.datetime);
}

/** \brief add new alarm into the list in ascending order
 */
static int32_t calendar_add_new_alarm(struct list_descriptor *list, struct calendar_os_alarm *alarm)
{
	struct calendar_os_descriptor *calendar = CONTAINER_OF(list, struct calendar_os_descriptor, alarms);
	struct calendar_os_alarm *     head, *it, *prev = NULL;

	/*get the head of alarms list*/
	head = (struct calendar_os_alarm *)list_get_head(list);

	/*if head is null, insert new alarm as head*/
	if (!head) {
		list_insert_as_head(list, alarm);
		_calendar_set_comp(&calendar->device, alarm->cal_alarm.timestamp);
		return 0;
	}

	/*insert the new alarm in accending order, the head will be invoked firstly */
	for (it = head; it; it = (struct calendar_os_alarm *)list_get_next_element(it)) {
		if (alarm->cal_alarm.timestamp <= it->cal_alarm.timestamp) {
			break;
		}

		prev = it;
	}

	/*insert new alarm into the list */
	if (it == head) {
		list_insert_as_head(list, alarm);
		/*get the head and set it into register*/
		_calendar_set_comp(&calendar->device, alarm->cal_alarm.timestamp);

	} else {
		list_insert_after(prev, alarm);
	}

	return 0;
}
/** \brief callback for alarm
 */
static void calendar_os_alarm(struct calendar_dev *const dev)
{
	struct calendar_os_descriptor *calendar = CONTAINER_OF(dev, struct calendar_os_descriptor, device);
	sem_up(&calendar->alarm_sem);
}

/** \brief Process alarm task
 */
int32_t calendar_os_task(struct calendar_os_descriptor *const calendar)
{

	struct calendar_os_alarm *head, *it, current_dt;

	if (sem_down(&calendar->alarm_sem, ~0) != 0) {
		return ERR_TIMEOUT;
	}

	/* get current timestamp */
	current_dt.cal_alarm.timestamp = _calendar_get_counter(&calendar->device);

	/* get the head */
	head = (struct calendar_os_alarm *)list_get_head(&calendar->alarms);
	if (head == NULL) {
		return ERR_ABORTED;
	}

	/* remove all alarms and invoke them*/
	for (it = head; it; it = (struct calendar_os_alarm *)list_get_head(&calendar->alarms)) {
		/* check the timestamp with current timestamp*/
		if (it->cal_alarm.timestamp <= current_dt.cal_alarm.timestamp) {
			list_remove_head(&calendar->alarms);
			it->callback(calendar);

			if (it->cal_alarm.mode == REPEAT) {
				calibrate_timestamp(it, &current_dt);
				convert_timestamp_to_datetime(it->cal_alarm.timestamp, &it->cal_alarm.datetime);
				calendar_add_new_alarm(&calendar->alarms, it);
			}
		} else {
			break;
		}
	}

	/*if no alarm in the list, register null */
	if (!it) {
		_calendar_register_callback(&calendar->device, NULL);
		return ERR_NONE;
	}

	/*put the new head into register */
	_calendar_set_comp(&calendar->device, it->cal_alarm.timestamp);

	return ERR_NONE;
}

/** \brief Initialize Calendar
 */
int32_t calendar_os_init(struct calendar_os_descriptor *const calendar, const void *hw)
{
	int32_t ret = 0;

	/* Sanity check arguments */
	ASSERT(calendar);

	ret = sem_init(&calendar->alarm_sem, 0);
	if (ret < 0) {
		return ret;
	}

	calendar->device.hw = (void *)hw;

	_calendar_init(&calendar->device);
	_calendar_enable(&calendar->device);
	calendar->alarms.head = NULL;

	return 0;
}

/** \brief Reset the Calendar
 */
int32_t calendar_os_deinit(struct calendar_os_descriptor *const calendar)
{
	/* Sanity check arguments */
	ASSERT(calendar);

	_calendar_disable(&calendar->device);
	_calendar_deinit(&calendar->device);
	calendar->alarms.head = NULL;
	sem_deinit(&calendar->alarm_sem);

	return 0;
}

/** \brief Set time for calendar
 */
int32_t calendar_os_set_date_time(struct calendar_os_descriptor *const calendar, struct calendar_date_time *const time)
{
	/* Sanity check arguments */
	uint32_t ts;
	ASSERT(calendar);
	ASSERT(time);

	ts = convert_datetime_to_timestamp(time);
	_calendar_set_counter(&calendar->device, ts);

	return 0;
}

/** \brief Get date/time for calendar
 */
int32_t calendar_os_get_date_time(struct calendar_os_descriptor *const calendar, struct calendar_date_time *const time)
{
	uint32_t current_ts;
	/* Sanity check arguments */
	ASSERT(calendar);
	ASSERT(time);

	/* convert current timestamp to date/time */
	current_ts = _calendar_get_counter(&calendar->device);
	convert_timestamp_to_datetime(current_ts, time);

	return 0;
}

/** \brief Set alarm for calendar
 */
int32_t calendar_os_set_alarm(struct calendar_os_descriptor *const calendar, struct calendar_os_alarm *const alarm,
                              calendar_os_cb_alarm_t callback)
{
	struct calendar_os_alarm *head;

	/* Sanity check arguments */
	ASSERT(calendar);
	ASSERT(alarm);

	alarm->callback = callback;

	fill_alarm(calendar, alarm);

	head = (struct calendar_os_alarm *)list_get_head(&calendar->alarms);

	if (head != NULL) {
		/* already added */
		if (is_list_element(&calendar->alarms, alarm)) {
			if (callback == NULL) {
				/* remove alarm */
				list_delete_element(&calendar->alarms, alarm);

				if (!list_get_head(&calendar->alarms)) {
					_calendar_register_callback(&calendar->device, NULL);
				}
			} else {
				/* re-add */
				list_delete_element(&calendar->alarms, alarm);
				calendar_add_new_alarm(&calendar->alarms, alarm);
			}
		} else if (callback != NULL) {
			calendar_add_new_alarm(&calendar->alarms, alarm);
		}

	} else if (callback != NULL) {
		/* if head is NULL, Register callback*/
		_calendar_register_callback(&calendar->device, calendar_os_alarm);
		calendar_add_new_alarm(&calendar->alarms, alarm);
	}

	return 0;
}

/** \brief Retrieve driver version
 *  \return Current driver version
 */
uint32_t calendar_os_get_version(void)
{
	return CALENDAR_VERSION;
}
