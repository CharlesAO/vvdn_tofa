/* usbpump_rtems_intr.h	Fri Oct 31 2014 12:18:46 chwon */

/*

Module:  usbpump_rtems_intr.h

Function:
	RTEMS interrupt system internal definitions.

Version:
	V3.13b	Fri Oct 31 2014 12:18:46 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	September 2014

Revision history:
   3.13b  Mon Sep 15 2014 12:33:40  chwon
	18534: Module created.

   3.13b  Fri Oct 31 2014 12:18:46  chwon
	18534: Change interrupt resource handling marco input parameter order.

*/

#ifndef _USBPUMP_RTEMS_INTR_H_		/* prevent multiple includes */
#define _USBPUMP_RTEMS_INTR_H_

#ifndef _USBPUMP_RTEMS_H_
# include "usbpump_rtems.h"
#endif

/****************************************************************************\
|
|	Interrupt resource handles are defined (for RTEMS)
|
\****************************************************************************/

#define	__TMS_USBPUMP_RTEMS_MAKE_INTERRUPT_RESOURCE_HANDLE(a_Resource)	\
	((__TMS_UHIL_INTERRUPT_RESOURCE_HANDLE) &a_Resource)

#define	__TMS_USBPUMP_RTEMS_DECLARE_INTERRUPT_RESOURCE(			\
	a_Resource,							\
	a_Vector							\
	)								\
	/* Declare interrupt resource variable */			\
	__TMS_USBPUMP_RTEMS_INTERRUPT_RESOURCE	a_Resource;		\
									\
	/* Interrupt service routine */					\
	static rtems_isr						\
	UsbpumpRtemsI_Isr_##a_Vector(					\
		rtems_vector_number	no				\
		)							\
		{							\
		if (a_Resource.uir_Vector == no && a_Resource.uir_pIsr)	\
			{						\
			(*a_Resource.uir_pIsr)(a_Resource.uir_pIsrCtx);	\
			}						\
		}

#define	__TMS_USBPUMP_RTEMS_INITIALIZE_INTERRUPT_RESOURCE(		\
	a_Resource,							\
	a_Vector							\
	)								\
	UsbPumpRtems_InterruptResourceInit(				\
		&a_Resource,						\
		a_Vector,						\
		UsbpumpRtemsI_Isr_##a_Vector				\
		)


/****************************************************************************\
|
|	Low level interrupt control routine
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
UPLATFORM_RTEMS_INTERRUPT_ENABLE_FN,
__TMS_BOOL,
	(
	__TMS_VOID *					/* pPortContext */,
	__TMS_UHIL_INTERRUPT_RESOURCE_HANDLE		/* hResource */,
	__TMS_BOOL					/* fEnable */
	));

__TMS_FNTYPE_DEF(
UPLATFORM_RTEMS_INTERRUPT_REGISTER_FN,
__TMS_BOOL,
	(
	__TMS_VOID *					/* pPortContext */,
	__TMS_UHIL_INTERRUPT_RESOURCE_HANDLE		/* hResource */,
	__TMS_UHIL_INTERRUPT_SERVICE_ROUTINE_FN *	/* pIsrFn */,
	__TMS_VOID *					/* pIsrCtx */
	));


/****************************************************************************\
|
|	The structures used for managing interrupts.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_RTEMS_INTERRUPT_CONNECTION);
__TMS_TYPE_DEF_STRUCT(USBPUMP_RTEMS_INTERRUPT_RESOURCE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_RTEMS_INTERRUPT_SYSTEM_INTERFACE);

struct __TMS_STRUCTNAME(USBPUMP_RTEMS_INTERRUPT_CONNECTION)
	{
	__TMS_UHIL_INTERRUPT_RESOURCE_HANDLE		uic_hResource;
	__TMS_USBPUMP_RTEMS_INTERRUPT_SYSTEM_INTERFACE *uic_pRtemsIntSys;
	__TMS_UINT					uic_fRegistered: 1;
	__TMS_UINT					uic_fEnabled: 1;
	};

#define	__TMS_UHIL_HANDLE_TO_USBPUMP_RTEMS_INTERRUPT_CONNECTION(Handle)	\
	((__TMS_USBPUMP_RTEMS_INTERRUPT_CONNECTION *) (Handle))

#define	__TMS_USBPUMP_RTEMS_TO_UHIL_INTERRUPT_CONNECTION_HANDLE(Connection) \
	((__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE) (Connection))


struct __TMS_STRUCTNAME(USBPUMP_RTEMS_INTERRUPT_RESOURCE)
	{
	rtems_vector_number				uir_Vector;
	rtems_isr_entry					uir_MyIsr;
	rtems_isr_entry					uir_OldIsr;

	/* DataPump ISR information */
	__TMS_UHIL_INTERRUPT_SERVICE_ROUTINE_FN *	uir_pIsr;
	__TMS_VOID *					uir_pIsrCtx;
	};

#define	__TMS_UHIL_HANDLE_TO_USBPUMP_RTEMS_INTERRUPT_RESOURCE(Handle)	\
	((__TMS_USBPUMP_RTEMS_INTERRUPT_RESOURCE *) (Handle))


struct __TMS_STRUCTNAME(USBPUMP_RTEMS_INTERRUPT_SYSTEM_INTERFACE)
	{
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE		UhilIntSys;
	__TMS_UPLATFORM_RTEMS *				pPlatformRtems;
	__TMS_VOID *					pPortContext;
	__TMS_UPLATFORM_RTEMS_INTERRUPT_ENABLE_FN *	pInterruptEnableFn;
	__TMS_UPLATFORM_RTEMS_INTERRUPT_REGISTER_FN *	pInterruptRegisterFn;
	};

#define	__TMS_UHIL_TO_USBPUMP_RTEMS_INTERRUPT_SYSTEM_INTERFACE(pIntSys)	\
	__TMS_CONTAINER_OF(						\
		pIntSys,						\
		__TMS_USBPUMP_RTEMS_INTERRUPT_SYSTEM_INTERFACE,		\
		UhilIntSys)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_CONST __TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE *
UsbPumpRtems_InitializeInterruptSystemInterface(
	__TMS_UPLATFORM_RTEMS *				pPlatformRtems,
	__TMS_USBPUMP_RTEMS_INTERRUPT_SYSTEM_INTERFACE *pRtemsIntSys,
	__TMS_VOID *					pPortContext,
	__TMS_UPLATFORM_RTEMS_INTERRUPT_ENABLE_FN *	pInterruptEnableFn,
	__TMS_UPLATFORM_RTEMS_INTERRUPT_REGISTER_FN *	pInterruptRegisterFn
	);

__TMS_VOID
UsbPumpRtems_InterruptResourceInit(
	__TMS_USBPUMP_RTEMS_INTERRUPT_RESOURCE *	pResource,
	rtems_vector_number				Vector,
	rtems_isr_entry					IsrFn
	);

__TMS_UPLATFORM_RTEMS_INTERRUPT_ENABLE_FN
UsbPumpRtems_InterruptEnable;

__TMS_UPLATFORM_RTEMS_INTERRUPT_REGISTER_FN
UsbPumpRtems_InterruptRegister;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_RTEMS_MAKE_INTERRUPT_RESOURCE_HANDLE(a_Resource)	\
   __TMS_USBPUMP_RTEMS_MAKE_INTERRUPT_RESOURCE_HANDLE(a_Resource)
# define USBPUMP_RTEMS_DECLARE_INTERRUPT_RESOURCE(			\
	a_Resource,							\
	a_Vector							\
	)	\
	__TMS_USBPUMP_RTEMS_DECLARE_INTERRUPT_RESOURCE(			\
	a_Resource,							\
	a_Vector							\
	)
# define USBPUMP_RTEMS_INITIALIZE_INTERRUPT_RESOURCE(		\
	a_Resource,							\
	a_Vector							\
	)	\
	__TMS_USBPUMP_RTEMS_INITIALIZE_INTERRUPT_RESOURCE(		\
	a_Resource,							\
	a_Vector							\
	)
# define UHIL_HANDLE_TO_USBPUMP_RTEMS_INTERRUPT_CONNECTION(Handle)	\
   __TMS_UHIL_HANDLE_TO_USBPUMP_RTEMS_INTERRUPT_CONNECTION(Handle)
# define USBPUMP_RTEMS_TO_UHIL_INTERRUPT_CONNECTION_HANDLE(Connection)	\
   __TMS_USBPUMP_RTEMS_TO_UHIL_INTERRUPT_CONNECTION_HANDLE(Connection)
# define UHIL_HANDLE_TO_USBPUMP_RTEMS_INTERRUPT_RESOURCE(Handle)	\
   __TMS_UHIL_HANDLE_TO_USBPUMP_RTEMS_INTERRUPT_RESOURCE(Handle)
# define UHIL_TO_USBPUMP_RTEMS_INTERRUPT_SYSTEM_INTERFACE(pIntSys)	\
   __TMS_UHIL_TO_USBPUMP_RTEMS_INTERRUPT_SYSTEM_INTERFACE(pIntSys)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_rtems_intr.h ****/
#endif /* _USBPUMP_RTEMS_INTR_H_ */
