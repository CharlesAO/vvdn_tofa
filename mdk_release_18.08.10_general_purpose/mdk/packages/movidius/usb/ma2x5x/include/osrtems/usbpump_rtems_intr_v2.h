/* usbpump_rtems_intr_v2.h	Fri Jan 09 2015 09:45:59 chwon */

/*

Module:  usbpump_rtems_intr_v2.h

Function:
	Definition of 

Version:
	V3.13b	Fri Jan 09 2015 09:45:59 chwon	Edit level 1

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
   3.13b  Fri Jan 09 2015 09:45:59 chwon
	18534: Module created.

*/

#ifndef _USBPUMP_RTEMS_INTR_V2_H_	/* prevent multiple includes */
#define _USBPUMP_RTEMS_INTR_V2_H_

#ifndef _USBPUMP_RTEMS_INTR_H_
# include "usbpump_rtems_intr.h"
#endif

#include <rtems/irq-extension.h>

/****************************************************************************\
|
|	Interrupt resource handles are defined (for RTEMS)
|
\****************************************************************************/

#define	__TMS_USBPUMP_RTEMS_MAKE_INTERRUPT_RESOURCE_HANDLE_V2(a_Resource) \
	((__TMS_UHIL_INTERRUPT_RESOURCE_HANDLE) &a_Resource)

#define	__TMS_USBPUMP_RTEMS_DECLARE_INTERRUPT_RESOURCE_V2(		\
	a_Resource,							\
	a_Vector							\
	)								\
	/* Declare interrupt resource variable */			\
	__TMS_USBPUMP_RTEMS_INTERRUPT_RESOURCE_V2	a_Resource;	\
									\
	/* Interrupt service routine */					\
	static void							\
	UsbpumpRtemsI_Isr_##a_Vector(					\
		void *	pContext					\
		)							\
		{							\
		__TMS_USBPUMP_RTEMS_INTERRUPT_RESOURCE_V2 * __TMS_CONST	\
			pResource = pContext;				\
									\
		if (pResource->uir_pIsr)				\
			{						\
			(*pResource->uir_pIsr)(pResource->uir_pIsrCtx);	\
			}						\
		}

#define	__TMS_USBPUMP_RTEMS_INITIALIZE_INTERRUPT_RESOURCE_V2(		\
	a_Resource,							\
	a_Vector,							\
	a_Options,							\
	a_pName								\
	)								\
	UsbPumpRtems_InterruptResourceInit_V2(				\
		&a_Resource,						\
		a_Vector,						\
		a_Options,						\
		a_pName,						\
		UsbpumpRtemsI_Isr_##a_Vector				\
		)


/****************************************************************************\
|
|	The structures used for managing interrupts.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_RTEMS_INTERRUPT_RESOURCE_V2);

struct __TMS_STRUCTNAME(USBPUMP_RTEMS_INTERRUPT_RESOURCE_V2)
	{
	rtems_vector_number				uir_Vector;
	rtems_option					uir_Options;
	const char *					uir_pName;
	rtems_interrupt_handler				uir_MyIsr;

	/* DataPump ISR information */
	__TMS_UHIL_INTERRUPT_SERVICE_ROUTINE_FN *	uir_pIsr;
	__TMS_VOID *					uir_pIsrCtx;
	};

#define	__TMS_UHIL_HANDLE_TO_USBPUMP_RTEMS_INTERRUPT_RESOURCE_V2(Handle) \
	((__TMS_USBPUMP_RTEMS_INTERRUPT_RESOURCE_V2 *) (Handle))


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpRtems_InterruptResourceInit_V2(
	__TMS_USBPUMP_RTEMS_INTERRUPT_RESOURCE_V2 *	pResource,
	rtems_vector_number				Vector,
	rtems_option					Options,
	const char *					pName,
	rtems_interrupt_handler				IsrFn
	);

__TMS_UPLATFORM_RTEMS_INTERRUPT_ENABLE_FN
UsbPumpRtems_InterruptEnable_V2;

__TMS_UPLATFORM_RTEMS_INTERRUPT_REGISTER_FN
UsbPumpRtems_InterruptRegister_V2;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_RTEMS_MAKE_INTERRUPT_RESOURCE_HANDLE_V2(a_Resource)	\
   __TMS_USBPUMP_RTEMS_MAKE_INTERRUPT_RESOURCE_HANDLE_V2(a_Resource)
# define USBPUMP_RTEMS_DECLARE_INTERRUPT_RESOURCE_V2(		\
	a_Resource,							\
	a_Vector							\
	)	\
	__TMS_USBPUMP_RTEMS_DECLARE_INTERRUPT_RESOURCE_V2(		\
	a_Resource,							\
	a_Vector							\
	)
# define USBPUMP_RTEMS_INITIALIZE_INTERRUPT_RESOURCE_V2(		\
	a_Resource,							\
	a_Vector,							\
	a_Options,							\
	a_pName								\
	)	\
	__TMS_USBPUMP_RTEMS_INITIALIZE_INTERRUPT_RESOURCE_V2(		\
	a_Resource,							\
	a_Vector,							\
	a_Options,							\
	a_pName								\
	)
# define UHIL_HANDLE_TO_USBPUMP_RTEMS_INTERRUPT_RESOURCE_V2(Handle)	\
   __TMS_UHIL_HANDLE_TO_USBPUMP_RTEMS_INTERRUPT_RESOURCE_V2(Handle)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_rtems_intr_v2.h ****/
#endif /* _USBPUMP_RTEMS_INTR_V2_H_ */