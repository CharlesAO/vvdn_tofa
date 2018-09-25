///
/// @file rtems.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup rtems rtems Driver
/// @{
/// @brief     RTEMS Layer Implementation
///


#ifndef __RTEMS_H__
#define __RTEMS_H__

#include <DrvRegUtils.h>
#include <DrvIcb.h>
#include <DrvTimer.h>
#include <DrvCpr.h>
#include <stdio.h>
#include <bsp.h>
#include <bsp/irq-generic.h>
#include <rtems/libio.h>
#include <rtems/libi2c.h>
#include <rtems/status-checks.h>
//#include "OsCross.h"

/*****************************************************************************************************************************/
/* Configuration  */
/*****************************************************************************************************************************/
#define OSCROSSIOMANAGERTABLEMAXDRIVERS																20 // IO manager max drivers
#define DRVCROSSLIBI2CMANAGERDEPTH																	10
#define DRVCROSSLIBI2CDEVICESDEPTH																	3
/*****************************************************************************************************************************/
/* Traslation Layer */
/*****************************************************************************************************************************/
/*
 * Utility macros
*/
#define OSCROSS_IRQ_SHARED																		osCrossInformation.irqShared
#define OSCROSS_UPDATE_IRQ_SHARED(event_)														OSCROSS_IRQ_SHARED = event_
#define OSCROSS_GET_IRQ_SHARED																	OSCROSS_IRQ_SHARED
#define OSCROSS_CLEAR_IRQ_SHARED(mask_)															OSCROSS_IRQ_SHARED &= ~mask_
#define OSCROSS_SET_IRQ_SHARED(mask_)															OSCROSS_IRQ_SHARED |= mask_


/*
 * RTEMS internal staff
*/
#define Clock_exit()
#define Clock_initialize(major_,minor_,arg_)
#define __BSP_CLOCK_CONFIG__																	osCrossBSPClockConfig_t
#define BSP_CLOCK_CONFIG																		osCrossBSPClockConfiguration

/*
 * Debug Staff
*/
#define printk(...)																				printf(__VA_ARGS__)

/*
* data types to map RTEMS
*/
#define rtems_id																				uint32_t
#define rtems_option                                                                            uint32_t
#define rtems_status_code       																uint32_t
#define rtems_event_set																			uint32_t
#define rtems_interval                                                                          uint32_t
#define true																					TRUE
#define rtems_interrupt_handler																	irq_handler
#define rtems_vector_number																		uint32_t
#define rtems_task																				uint32_t
#define rtems_device_major_number																uint32_t
#define rtems_device_minor_number																uint32_t
#define rtems_libio_rw_args_t																	osCrossLibIORWArgs_t
#define rtems_interrupt_level                                                                   uint32_t

/*
 * Return codes
*/
#define RTEMS_SUCCESSFUL																		0
#define RTEMS_TASK_EXITTED 																		1
#define RTEMS_MP_NOT_CONFIGURED 																2
#define RTEMS_INVALID_NAME 																		3
#define RTEMS_INVALID_ID 																		4
#define RTEMS_TOO_MANY 																			5
#define RTEMS_TIMEOUT 																			6
#define RTEMS_OBJECT_WAS_DELETED 																7
#define RTEMS_INVALID_SIZE 																		8
#define RTEMS_INVALID_ADDRESS 																	9
#define RTEMS_INVALID_NUMBER 																	10
#define RTEMS_NOT_DEFINED 																		11
#define RTEMS_RESOURCE_IN_USE 																	12
#define RTEMS_UNSATISFIED 																		13
#define RTEMS_INCORRECT_STATE 																	14
#define RTEMS_ALREADY_SUSPENDED 																15
#define RTEMS_ILLEGAL_ON_SELF 																	16
#define RTEMS_ILLEGAL_ON_REMOTE_OBJECT 															17
#define RTEMS_CALLED_FROM_ISR 																	18
#define RTEMS_INVALID_PRIORITY 																	19
#define RTEMS_INVALID_CLOCK 																	20
#define RTEMS_INVALID_NODE 																		21
#define RTEMS_NOT_CONFIGURED 																	22
#define RTEMS_NOT_OWNER_OF_RESOURCE 															23
#define RTEMS_NOT_IMPLEMENTED 																	24
#define RTEMS_INTERNAL_ERROR 																	25
#define RTEMS_NO_MEMORY 																		26
#define RTEMS_IO_ERROR 																			27
#define RTEMS_PROXY_BLOCKING 																	28

/*
 * Constants used in calls to directives
*/
#define RTEMS_NO_TIMEOUT																		0xFFFFFFFF
#define RTEMS_WAIT																				0
#define RTEMS_NO_WAIT																			1
#define RTEMS_EVENT_ALL																			0
#define RTEMS_EVENT_ANY																			2
#define RTEMS_PENDING_EVENTS																	0

/*
 * Directives to map RTEMS
*/

/*
 * Time directives
*/
#define rtems_clock_get_ticks_per_second()														OsCrossGetTicksperSecond()
#define rtems_clock_get_ticks_since_boot()                                                      0

/*
 * Semaphores / Mutexes
*/
#define rtems_semaphore_create(_name, count_, attributes_, ceiling_, id_)						OS_MYR_DRV_SUCCESS
#define rtems_semaphore_obtain(id_, option_, timeout_)											OS_MYR_DRV_SUCCESS
#define rtems_semaphore_release(id_)															OS_MYR_DRV_SUCCESS
#define rtems_semaphore_delete(id_)																OS_MYR_DRV_SUCCESS
/*
 * Event send/receive
*/
#define rtems_event_send(taskid_, event_)														OSCROSS_IRQ_SHARED |= event_
#define rtems_event_receive(events_, mode_, timeout_, eventsout_)								OsCrossWaitFlags(events_, mode_, timeout_, eventsout_, &OSCROSS_IRQ_SHARED)

/*
 * Task management
*/
#define rtems_task_self()																		0
#define rtems_task_wake_after(timeout_)															OsCrossUSleep(timeout_)
#define rtems_task_argument																		void *
#define rtems_task_start(id_, thread_, arg_)													OS_MYR_DRV_SUCCESS
#define rtems_build_name(a_,b_,c_,d_)															NULL
#define rtems_task_create(name_, prio_, stack_, mode_, attribute_, id_)							OS_MYR_DRV_SUCCESS
#define rtems_task_delete(id_)																	OS_MYR_DRV_SUCCESS
// TBD
#define RTEMS_FLOATING_POINT																	0
#define RTEMS_DEFAULT_ATTRIBUTES																0
#define RTEMS_PREEMPT 																			0
#define RTEMS_TIMESLICE																			0
#define RTEMS_INTERRUPT_LEVEL(level_)															0
#define RTEMS_MINIMUM_STACK_SIZE																0
#define RTEMS_SELF																				0

/*
 * IO Manager
*/
#define rtems_device_driver_entry																osCrossDeviceDriverEntry
#define rtems_driver_address_table																osCrossDriverAddressTable_t
#define rtems_io_register_driver(major_, driver_table_, registered_major_)						OsCrossIORegisterDriver(major_, driver_table_, registered_major_ )
#define rtems_io_unregister_driver(major_)														OsCrossIOUnRegisterDriver(major_)
#define rtems_io_initialize(major_,minor_,arg_)													OsCrossIOInitDriver(major_, minor_, arg_)
#define rtems_io_open(major_, minor_, arg_) 													OsCrossIOOpenDriver(major_, minor_, arg_)
#define rtems_io_close(major_, minor_, arg_) 													OsCrossIOCloseDriver(major_, minor_, arg_)
#define rtems_io_read(major_, minor_, arg_) 													OsCrossIOReadDriver(major_, minor_, arg_)
#define rtems_io_write(major_, minor_, arg_) 													OsCrossIOWriteDriver(major_, minor_, arg_)
#define rtems_io_control(major_, minor_, arg_) 													OsCrossIOCtldDriverDriver(major_, minor_, arg_)

/*
 * Libi2c
*/
#define rtems_libi2c_initialize()																OS_MYR_DRV_SUCCESS
#define rtems_libi2c_register_bus(name_, table_)												OsCrossLibI2CRegisterBus(name_, table_)
#define rtems_libi2c_register_drv(name_, table_, bus_, addr_)								    OsCrossLibI2CRegisterDevice(name_, table_, bus_, addr_)

#define rtems_libi2c_send_start(minor_) 														OsCrossLibI2CSendStart(minor_)
#define rtems_libi2c_send_stop(minor_)															OsCrossLibI2CSendStop(minor_)
#define rtems_libi2c_send_addr(minor_, arg_)													OsCrossLibI2CSendAddr(minor_, arg_)
#define rtems_libi2c_read_bytes(minor_, buffer_, count_)										OsCrossLibI2CReadBytes(minor_, buffer_, count_)
#define rtems_libi2c_write_bytes(minor_, buffer_, count_)										OsCrossLibI2CWriteBytes(minor_, buffer_, count_)

#define rtems_libi2c_bus_t_ 																	osCrossLibI2CBus_t_
#define rtems_libi2c_bus_t 																		osCrossLibI2CBus_t
#define rtems_libi2c_bus_ops_ 																	osCrossLibI2CBusOps_
#define rtems_libi2c_bus_ops_t 																	osCrossLibI2CBusOps_t
#define rtems_libi2c_drv_t																		osCrossLibI2CDrv_t
#define rtems_libi2c_drv_t_																		osCrossLibI2CDrv_t_
#define myr2I2cBusEntry_t																		osCrossLibI2CBusEntry_t
/*****************************************************************************************************************************/
/* Data Types/Structures required */
/*****************************************************************************************************************************/
/*
 * Extra structure needed
*/
typedef struct
{
	volatile uint32_t irqShared;
} osCrossInfo_t;
/*
 * Extra functions needed
*/
#ifdef __cplusplus
extern "C" {
#endif

int osCrossInit(void);

#ifdef __cplusplus
}
#endif

#endif

///@}
