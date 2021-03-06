/**
 * \file
 *
 * \brief Generic CALENDAR functionality declaration.
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

#ifndef _HAL_CALENDER_OS_H_INCLUDED
#define _HAL_CALENDER_OS_H_INCLUDED

#include "hpl_calendar.h"
#include <utils_list.h>
#include <hal_rtos.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_calendar_os
 *
 *@{
 */

/** Default base year. */
#define DEFAULT_BASE_YEAR 1970

/** \brief Calendar HAL driver struct
 *
 */
struct calendar_os_descriptor {
	struct calendar_dev    device;
	struct list_descriptor alarms;
	sem_t                  alarm_sem;
};

/** \brief Prototype of callback on alarm match
 *  \param calendar Pointer to the HAL Calendar instance.
 */
typedef void (*calendar_os_cb_alarm_t)(struct calendar_os_descriptor *const calendar);

/** \brief Struct for alarm time
 */
struct calendar_os_alarm {
	struct list_element    elem;
	struct _calendar_alarm cal_alarm;
	calendar_os_cb_alarm_t callback;
};

/** \brief Initialize the Calendar HAL instance and hardware
 *
 *  \param calendar Pointer to the HAL Calendar instance.
 *  \param hw       Pointer to the hardware instance.
 *  \return Operation status of init
 *  \retval 0       Completed successfully.
 */
int32_t calendar_os_init(struct calendar_os_descriptor *const calendar, const void *hw);

/** \brief Reset the Calendar HAL instance and hardware
 *
 *  Reset Calendar instance to hardware defaults.
 *
 *  \param calendar Pointer to the HAL Calendar instance.
 *  \return Operation status of reset.
 *  \retval 0       Completed successfully.
 */
int32_t calendar_os_deinit(struct calendar_os_descriptor *const calendar);

/** \brief Configure the time for calendar HAL instance and hardware
 *
 *  \param calendar Pointer to the HAL Calendar instance.
 *  \param time Pointer to the time configuration.
 *  \retval 0       Completed successfully.
 */
int32_t calendar_os_set_date_time(struct calendar_os_descriptor *const calendar, struct calendar_date_time *const time);

/** \brief Get the time for calendar HAL instance and hardware
 *
 *  Retrieve the time from calendar instance.
 *
 *  \param calendar Pointer to the HAL Calendar instance.
 *  \param time Pointer to the value that will be filled with the current time.
 *  \return Operation status of time retrieve.
 *  \retval 0       Completed successfully.
 */
int32_t calendar_os_get_date_time(struct calendar_os_descriptor *const calendar, struct calendar_date_time *const time);

/** \brief Config the alarm time for calendar HAL instance and hardware
 *
 *  Set the alarm time to the calendar instance. If callback is NULL, remove
 *  the alarm if the alarm is already added, otherwise, ignore the alarm.
 *
 *  \param calendar Pointer to the HAL Calendar instance.
 *  \param alarm Pointer to the configuration.
 *  \param callback Pointer to the callback function.
 *  \return Operation status of alarm time set.
 *  \retval 0       Completed successfully.
 */
int32_t calendar_os_set_alarm(struct calendar_os_descriptor *const calendar, struct calendar_os_alarm *const alarm,
                              calendar_os_cb_alarm_t callback);

/** \brief Alarm process task
 *
 *  It should be called in the user thread or while loop.
 *
 *  \param calendar Pointer to the HAL calendar instance.
 *  \return Operation status of alarm task.
 *  \retval 0       Completed successfully.
 */
int32_t calendar_os_task(struct calendar_os_descriptor *const calendar);

/** \brief Retrieve the current driver version
 *  \return Current driver version.
 */
uint32_t calendar_os_get_version(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* _HAL_CALENDER_OS_H_INCLUDED */
