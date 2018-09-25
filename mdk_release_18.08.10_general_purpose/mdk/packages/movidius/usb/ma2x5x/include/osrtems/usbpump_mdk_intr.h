/* usbpump_mdk_intr.h	Fri Sep 25 2015 11:52:18 chwon */

/*

Module:  usbpump_mdk_intr.h

Function:
	Definition of RTEMS interrupt interface.

Version:
	V3.15d	Fri Sep 25 2015 11:52:19 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	January 2015

Revision history:
   3.13b  Thu Jan 08 2015 11:48:48 chwon
	18534: Module created.

   3.15d  Fri Sep 25 2015 11:52:19  chwon
	19424: Remove DrvIcb.h file

*/

#ifndef _USBPUMP_MDK_INTR_H_	/* prevent multiple includes */
#define _USBPUMP_MDK_INTR_H_

#ifndef _USBPUMP_RTEMS_INTR_V2_H_
# include "usbpump_rtems_intr_v2.h"
#endif

#include "bsp.h"

/****************************************************************************\
|
|	Interrupt resource handles are defined (for MDK)
|
\****************************************************************************/

#define	USBPUMP_MDK_RTEMS_INITIALIZE_INTERRUPT_RESOURCE(		\
	a_Resource,							\
	a_Vector,							\
	a_Options,							\
	a_pName,							\
	a_IntType,							\
	a_IntPriority							\
	)								\
    do	{								\
	__TMS_USBPUMP_RTEMS_INITIALIZE_INTERRUPT_RESOURCE_V2(		\
		a_Resource,						\
		a_Vector,						\
		a_Options,						\
		a_pName							\
		);							\
	BSP_Set_interrupt_type_priority(				\
		a_Vector,						\
		a_IntType,						\
		a_IntPriority						\
		);							\
	} while (0)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UPLATFORM_RTEMS_INTERRUPT_ENABLE_FN
UsbPumpMdk_Rtems_InterruptEnable;

/*
|| __TMS_UPLATFORM_RTEMS_INTERRUPT_REGISTER_FN
|| UsbPumpMdk_Rtems_InterruptRegister;
*/
#define	UsbPumpMdk_Rtems_InterruptRegister	\
	UsbPumpRtems_InterruptRegister_V2

__TMS_END_DECLS

/**** end of usbpump_mdk_intr.h ****/
#endif /* _USBPUMP_MDK_INTR_H_ */
