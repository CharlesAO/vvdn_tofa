/* usbpump_rtems.h	Thu Dec 15 2016 16:50:12 chwon */

/*

Module:  usbpump_rtems.h

Function:
	Header file for glue between the MCCI USB DataPump and Rtems

Version:
	V3.19a	Thu Dec 15 2016 16:50:12 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2014-2016 by

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

   3.15a  Tue Feb 24 2015 14:33:53  chwon
	18534: Fix typo in UsbPumpRtems_UsbDeviceStart.

   3.17a  Mon Aug 29 2016 15:42:37  chwon
	20274: Change DataPump event IDs.

   3.19a  Thu Dec 15 2016 16:50:12  chwon
	20414: Add USBPUMP_RTEMS_CONFIG_DO_NOT_WAIT_DEBUG_FLUSH config flag.

*/

#ifndef _USBPUMP_RTEMS_H_		/* prevent multiple includes */
#define _USBPUMP_RTEMS_H_

#ifndef _BASEHIL_H_
# include "basehil.h"
#endif

#ifndef _UPLATFORM_H_
# include "uplatform.h"
#endif

#ifndef _USBAPPINIT_H_
# include "usbappinit.h"
#endif

#ifndef _USBPUMP_MEMPOOL_H_
# include "usbpump_mempool.h"
#endif

#ifndef _USBPUMP_ALLOCATION_H_
# include "usbpump_allocation.h"
#endif

#ifndef _USBPUMP_UHILAUX_H_
# include "usbpump_uhilaux.h"
#endif

/* Rtems header files */
#include <rtems.h>

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(UPLATFORM_RTEMS);
__TMS_TYPE_DEF_STRUCT(USBPUMP_RTEMS_CONFIGURATION_INFO);


/****************************************************************************\
|
|	On Rtems, the UPLATFORM object is embedded in a larger UPLATFORM_
|	RTEMS; the larger structure includes the extra info needed to bind
|	to Rtems.  For simplicity, we also have an "event flag" object
|	implemented using Rtems primitives.
|
\****************************************************************************/

/*

Type:	USBPUMP_RTEMS_DEBUG_PUTSTRING_FN

Function:
	Print debug string

Definition:
	typedef VOID USBPUMP_RTEMS_DEBUG_PUTSTRING_FN(
			UPLATFORM_RTEMS *	pPlatformRtems,
			CONST CHAR *		pString
			);

	typedef USBPUMP_RTEMS_DEBUG_PUTSTRING_FN
		*PUSBPUMP_RTEMS_DEBUG_PUTSTRING_FN;

Description:
	This method, if present in the OS configuration, is called whenever
	the DataPump prints debug string.

Returns:
	No explicit result.

*/

__TMS_FNTYPE_DEF(
	USBPUMP_RTEMS_DEBUG_PUTSTRING_FN,
	__TMS_VOID,
	(
	__TMS_UPLATFORM_RTEMS *		/* pPlatformRtems */,
	__TMS_CONST __TMS_CHAR *	/* pString */
	));

/*

Type:	USBPUMP_RTEMS_UHILAUX_CREATE_FN

Function:
	Create UHILAUX object

Definition:
	typedef VOID USBPUMP_RTEMS_UHILAUX_CREATE_FN(
			UPLATFORM_RTEMS *	pPlatformRtems,
			VOID *			pPortContext
			);

	typedef USBPUMP_RTEMS_UHILAUX_CREATE_FN
		*PUSBPUMP_RTEMS_UHILAUX_CREATE_FN;

Description:
	This method, if present in the OS configuration, is called to create
	platform specific UHILAUX object.

Returns:
	Pointer to USBPUMP_UHILAUX object.

*/

__TMS_FNTYPE_DEF(
USBPUMP_RTEMS_UHILAUX_CREATE_FN,
__TMS_USBPUMP_UHILAUX *,
	(
	__TMS_UPLATFORM_RTEMS *		/* pPlatformRtems */,
	__TMS_VOID *			/* pPortContext */
	));

/*

Type:	USBPUMP_RTEMS_CONFIGURATION_INFO

Function:
	C structure type; OS configuration information for Rtems.

Description:
	This structure represents a configuration information for the Rtems
	OS.  Client should pass this configuration information when initialize
	the DataPump by calling UsbPumpRtems_UsbPumpInit().

Contents:
	|UINT32 nEventQueue;|
		Number of the DataPump event queue element.

	|VOID *pMemoryPool;|
	|BYTES nMemoryPool;|
		Platform memory pool information.  The pMemoryPool is pointer
		of the platform memory pool and the nMemoryPool is size of the
		platform memory pool.  This is optional. If pMemoryPool is NULL
		or nMemoryPool is zero, it will not create platform abstract
		pool and it will use system memory allocation.

	|USBPUMP_RTEMS_DEBUG_PUTSTRING_FN *pDebugPutString;|
		User provided function pointer to print out debug message.

	|CONST UHIL_INTERRUPT_SYSTEM_INTERFACE *pInterruptSystem;|
		User provided pointer of the UHIL interrupt system interface.
		User can create UHIL interrupt system interface by using
		UsbPumpRtems_InitializeInterruptSystemInterface().

	|CONST USBPUMP_TIMER_SWITCH *pTimerSwitch;|
		Pointer to the DataPump system timer switch.  If user wants to
		use default DataPump system timer implementation, user can pass
		pointer of gk_UsbPumpRtems_TimerSwitchDefault.

	|CONST USBPUMP_ALLOCATION_TRACKING_CONFIG *pAllocationTrackCfg;|
		Pointer to the dynamic memory allocation tracking system
		configuration.  If user pass NULL, dynamic memory allocation
		tracking system will be disabled.  If user wants to use default
		dynamic memory allocation tracking system, user set pointer of
		gk_UsbPumpAllocationTrackingConfig_Default.

	|UPLATFORM_IOCTL_FN *pIoctlFn;|
		Pointer to the user supplied UPLATFORM ioctl handler.

	|USBPUMP_RTEMS_UHILAUX_CREATE_FN *pUhilAuxCreateFn;|
		Pointer to the UHILAUX object creation function.  If user does
		not want to create UHILAUX object, user can pass NULL.

	|UPLATFORM_CREATE_ABSTRACT_POOL_FN *pCreateAbstractPoolFn;|
		Pointer to the abstract pool creation function.  This is
		optional and user can pass NULL.  If user want to use default
		method, set UsbPumpPlatform_CreatePlatformAbstractPool().

	|UINT DataPumpTaskPriority;|
		DataPump task priority. The priority range from a high of 1 to
		a low of 255.

	|UINT DebugTaskPriority;|
		Debug task priority. The priority range from a high of 1 to
		a low of 255.  This debug task priority should be lower than
		DataPump task priority.

	|UINT32 bmOptions;|
		Bitmap of the Optional flags. Here is defined flags:
		|USBPUMP_RTEMS_CONFIG_WAIT_DATAPUMP_INIT|
			Wait until the DataPump task initialization done.

		|USBPUMP_RTEMS_CONFIG_DO_NOT_START_DEVICE|
			Do not start DCD operation.  If this flag set, user
			should call UsbPumpRtems_UsbDeviceStart() to start
			DCD operation later.

		|USBPUMP_RTEMS_CONFIG_WAIT_DEBUG_INIT|
			Wait until the DataPump debug task initialization done.

		|USBPUMP_RTEMS_CONFIG_DO_NOT_WAIT_DEBUG_FLUSH|
			Do not wait DataPump thread until all debug message
			flush out. So DataPump thread will not block while
			debug thread flush out debug message.

Notes:
	Instances of this object are normally initialized using either
	USBPUMP_RTEMS_CONFIGURATION_INFO_INIT_Vx() {compile time} or
	USBPUMP_RTEMS_CONFIGURATION_INFO_SETUP_Vx() {runtime}.

See Also:
	USBPUMP_RTEMS_DEBUG_PUTSTRING_FN
	USBPUMP_RTEMS_UHILAUX_CREATE_FN

*/

struct __TMS_STRUCTNAME(USBPUMP_RTEMS_CONFIGURATION_INFO)
	{
	/* DataPump Event Queue infromation */
	__TMS_UINT32					nEventQueue;

	/* Platform memory pool information */
	__TMS_VOID *					pMemoryPool;
	__TMS_BYTES					nMemoryPool;

	/* Debug print string function */
	__TMS_USBPUMP_RTEMS_DEBUG_PUTSTRING_FN *	pDebugPutString;

	/* Platform provided interrupt system interface */
	__TMS_CONST __TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE *
							pInterruptSystem;

	/* System timer switch */
	__TMS_CONST __TMS_USBPUMP_TIMER_SWITCH *	pTimerSwitch;

	/* Dynamic memory allocation tracking system configuration. */
	__TMS_CONST __TMS_USBPUMP_ALLOCATION_TRACKING_CONFIG *
							pAllocationTrackCfg;

	/* Platform IOCTL related function */
	__TMS_UPLATFORM_IOCTL_FN *			pIoctlFn;

	/* UHILAUX create function */
	__TMS_USBPUMP_RTEMS_UHILAUX_CREATE_FN *		pUhilAuxCreateFn;

	/* Abstract pool creation function */
	__TMS_UPLATFORM_CREATE_ABSTRACT_POOL_FN *	pCreateAbstractPoolFn;

	/* DataPump task priority */
	__TMS_UINT					DataPumpTaskPriority;
	__TMS_UINT					DebugTaskPriority;

	/* Options */
	__TMS_UINT32					bmOptions;
	};

/* definition of the bmOptions */
#define	__TMS_USBPUMP_RTEMS_CONFIG_WAIT_DATAPUMP_INIT	(1u << 0)
#define	__TMS_USBPUMP_RTEMS_CONFIG_DO_NOT_START_DEVICE	(1u << 1)
#define	__TMS_USBPUMP_RTEMS_CONFIG_WAIT_DEBUG_INIT	(1u << 2)
#define	__TMS_USBPUMP_RTEMS_CONFIG_DO_NOT_WAIT_DEBUG_FLUSH (1u << 3)

/*
|| initialization functions.  Don't change the calling sequence for any
|| macro ending in Vn; instead, create a macro ending in Vn+1, and
|| redefine the Vn macro to expand in an upward-compatible way.
*/
#define	__TMS_USBPUMP_RTEMS_CONFIGURATION_INFO_INIT_V1(			\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_pDebugPutString,					\
		a_pInterruptSystem,					\
		a_pTimerSwitch,						\
		a_pAllocationTrackCfg,					\
		a_pIoctlFn,						\
		a_pUhilAuxCreateFn,					\
		a_pCreateAbstractPoolFn,				\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_bmOptions						\
		)							\
	{								\
	(a_nEventQueue),						\
	(a_pMemoryPool),						\
	(a_nMemoryPool),						\
	(a_pDebugPutString),						\
	(a_pInterruptSystem),						\
	(a_pTimerSwitch),						\
	(a_pAllocationTrackCfg),					\
	(a_pIoctlFn),							\
	(a_pUhilAuxCreateFn),						\
	(a_pCreateAbstractPoolFn),					\
	(a_DataPumpTaskPriority),					\
	(a_DebugTaskPriority),						\
	(a_bmOptions)							\
	}

#define	__TMS_USBPUMP_RTEMS_CONFIGURATION_INFO_SETUP_V1(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_pDebugPutString,					\
		a_pInterruptSystem,					\
		a_pTimerSwitch,						\
		a_pAllocationTrackCfg,					\
		a_pIoctlFn,						\
		a_pUhilAuxCreateFn,					\
		a_pCreateAbstractPoolFn,				\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_bmOptions						\
		)							\
    do	{								\
	(a_pConfig)->nEventQueue = (a_nEventQueue);			\
	(a_pConfig)->pMemoryPool = (a_pMemoryPool);			\
	(a_pConfig)->nMemoryPool = (a_nMemoryPool);			\
	(a_pConfig)->pDebugPutString = (a_pDebugPutString);		\
	(a_pConfig)->pInterruptSystem = (a_pInterruptSystem);		\
	(a_pConfig)->pTimerSwitch = (a_pTimerSwitch);			\
	(a_pConfig)->pAllocationTrackCfg = (a_pAllocationTrackCfg);	\
	(a_pConfig)->pIoctlFn = (a_pIoctlFn);				\
	(a_pConfig)->pUhilAuxCreateFn = (a_pUhilAuxCreateFn);		\
	(a_pConfig)->pCreateAbstractPoolFn = (a_pCreateAbstractPoolFn);	\
	(a_pConfig)->DataPumpTaskPriority = (a_DataPumpTaskPriority);	\
	(a_pConfig)->DebugTaskPriority = (a_DebugTaskPriority);		\
	(a_pConfig)->bmOptions = (a_bmOptions);				\
	} while (0)


struct __TMS_STRUCTNAME(UPLATFORM_RTEMS)
	{
	/* the base platform structure */
	__TMS_UPLATFORM				urtems_Platform;

	/*
	|| The configuration information gets copied into the
	|| platform structure.
	*/
	__TMS_USBPUMP_RTEMS_CONFIGURATION_INFO	urtems_Config;

	/*
	|| The DataPump event system requres its own context block.
	|| Since all Rtems DataPump apps will use the same event system,
	|| we can put it here.
	*/
	__TMS_UEVENTCONTEXT			urtems_EventContext;
	__TMS_BOOL				urtems_fSendEvent;

	__TMS_BOOL				urtems_fTimerStart;

	/*
	|| UHILAUX interface
	*/
	__TMS_USBPUMP_UHILAUX *			urtems_pUhilAux;

	/*
	|| The platform state flags
	*/
	__TMS_UINT				urtems_fStateUp: 1;
	__TMS_UINT				urtems_fStateDead: 1;

	/*
	|| For convenience, we keep pointers to the init vectors here.
	*/
	__TMS_CONST __TMS_USB_DATAPUMP_PORT_INIT_VECTOR_HDR *
						urtems_pPortVecHdr;
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *
						urtems_pAppVecHdr;
	__TMS_UDEVICE **			urtems_pvpDevices;
	__TMS_UINT				urtems_nDevices;
	__TMS_UINT32				urtems_InitialDebugMask;

	/*
	|| Rtems OS related variables
	*/
	rtems_id				urtems_TaskId;
	rtems_id				urtems_CallerTaskId;
	rtems_id				urtems_TimerId;
	rtems_interval				urtems_TicksPerSecond;
	};

/**** sane names ****/
#define	urtems_nEventQueue		urtems_Config.nEventQueue
#define	urtems_pMemoryPool		urtems_Config.pMemoryPool
#define	urtems_nMemoryPool		urtems_Config.nMemoryPool
#define	urtems_pDebugPutString		urtems_Config.pDebugPutString
#define	urtems_pInterruptSystem		urtems_Config.pInterruptSystem
#define	urtems_pTimerSwitch		urtems_Config.pTimerSwitch
#define	urtems_pAllocationTrackCfg	urtems_Config.pAllocationTrackCfg
#define	urtems_pIoctlFn			urtems_Config.pIoctlFn
#define	urtems_pUhilAuxCreateFn		urtems_Config.pUhilAuxCreateFn
#define	urtems_pCreateAbstractPoolFn	urtems_Config.pCreateAbstractPoolFn
#define	urtems_DataPumpTaskPriority	urtems_Config.DataPumpTaskPriority
#define	urtems_DebugTaskPriority	urtems_Config.DebugTaskPriority
#define	urtems_bmOptions		urtems_Config.bmOptions

/*
|| Handy manipulation macros
*/
#define __TMS_UPLATFORM_TO_UPLATFORM_RTEMS(pPlatform) \
	__TMS_CONTAINER_OF(pPlatform, __TMS_UPLATFORM_RTEMS, urtems_Platform)

#define	__TMS_UPLATFORM_RTEMS_TO_UPLATFORM(pPlatform) \
	(&(pPlatform)->urtems_Platform)

/* RTEMS event id definition for internal use */
#ifndef	__TMS_USBPUMP_RTEMS_EVENT_DATAPUMP_START	/* PARAM */
# define __TMS_USBPUMP_RTEMS_EVENT_DATAPUMP_START	RTEMS_EVENT_13
#endif

#define	__TMS_USBPUMP_RTEMS_EVENT_DATAPUMP_EVENT	RTEMS_EVENT_12
#define	__TMS_USBPUMP_RTEMS_EVENT_DEBUG_FLUSH		RTEMS_EVENT_2
#define	__TMS_USBPUMP_RTEMS_EVENT_DEBUG_START		RTEMS_EVENT_3
#define	__TMS_USBPUMP_RTEMS_EVENT_DEBUG_EVENT		RTEMS_EVENT_4


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern
__TMS_CONST
__TMS_USBPUMP_TIMER_SWITCH
gk_UsbPumpRtems_TimerSwitchDefault;

__TMS_BOOL
UsbPumpRtems_UsbPumpInit(
	__TMS_UPLATFORM_RTEMS *				/* pPlatformRtems */,
	__TMS_CONST __TMS_USBPUMP_RTEMS_CONFIGURATION_INFO * /* pRtemsConfig */,
	__TMS_VOID *					/* pPortContext */,
	__TMS_CONST __TMS_USB_DATAPUMP_PORT_INIT_VECTOR_HDR * /* pPortInitVecHdr */,
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR * /* pAppInitVecHdr */,
	__TMS_UDEVICE **				/* pvpDevices */,
	__TMS_UINT					/* nDevices */,
	__TMS_UINT32					/* ulDebugMask */
	);

__TMS_VOID
UsbPumpRtems_UsbDeviceStart(
	__TMS_UPLATFORM_RTEMS *	/* pPlatformRtems */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_RTEMS_CONFIG_WAIT_DATAPUMP_INIT	\
   __TMS_USBPUMP_RTEMS_CONFIG_WAIT_DATAPUMP_INIT
# define USBPUMP_RTEMS_CONFIG_DO_NOT_START_DEVICE	\
   __TMS_USBPUMP_RTEMS_CONFIG_DO_NOT_START_DEVICE
# define USBPUMP_RTEMS_CONFIG_WAIT_DEBUG_INIT	\
   __TMS_USBPUMP_RTEMS_CONFIG_WAIT_DEBUG_INIT
# define USBPUMP_RTEMS_CONFIG_DO_NOT_WAIT_DEBUG_FLUSH	\
   __TMS_USBPUMP_RTEMS_CONFIG_DO_NOT_WAIT_DEBUG_FLUSH
# define USBPUMP_RTEMS_CONFIGURATION_INFO_INIT_V1(			\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_pDebugPutString,					\
		a_pInterruptSystem,					\
		a_pTimerSwitch,						\
		a_pAllocationTrackCfg,					\
		a_pIoctlFn,						\
		a_pUhilAuxCreateFn,					\
		a_pCreateAbstractPoolFn,				\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_bmOptions						\
		)	\
	__TMS_USBPUMP_RTEMS_CONFIGURATION_INFO_INIT_V1(			\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_pDebugPutString,					\
		a_pInterruptSystem,					\
		a_pTimerSwitch,						\
		a_pAllocationTrackCfg,					\
		a_pIoctlFn,						\
		a_pUhilAuxCreateFn,					\
		a_pCreateAbstractPoolFn,				\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_bmOptions						\
		)
# define USBPUMP_RTEMS_CONFIGURATION_INFO_SETUP_V1(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_pDebugPutString,					\
		a_pInterruptSystem,					\
		a_pTimerSwitch,						\
		a_pAllocationTrackCfg,					\
		a_pIoctlFn,						\
		a_pUhilAuxCreateFn,					\
		a_pCreateAbstractPoolFn,				\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_bmOptions						\
		)	\
	__TMS_USBPUMP_RTEMS_CONFIGURATION_INFO_SETUP_V1(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_pDebugPutString,					\
		a_pInterruptSystem,					\
		a_pTimerSwitch,						\
		a_pAllocationTrackCfg,					\
		a_pIoctlFn,						\
		a_pUhilAuxCreateFn,					\
		a_pCreateAbstractPoolFn,				\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_bmOptions						\
		)
# define UPLATFORM_TO_UPLATFORM_RTEMS(pPlatform)	\
   __TMS_UPLATFORM_TO_UPLATFORM_RTEMS(pPlatform)
# define UPLATFORM_RTEMS_TO_UPLATFORM(pPlatform)	\
   __TMS_UPLATFORM_RTEMS_TO_UPLATFORM(pPlatform)
# define USBPUMP_RTEMS_EVENT_DATAPUMP_START	\
   __TMS_USBPUMP_RTEMS_EVENT_DATAPUMP_START
# define USBPUMP_RTEMS_EVENT_DATAPUMP_EVENT	\
   __TMS_USBPUMP_RTEMS_EVENT_DATAPUMP_EVENT
# define USBPUMP_RTEMS_EVENT_DEBUG_FLUSH	\
   __TMS_USBPUMP_RTEMS_EVENT_DEBUG_FLUSH
# define USBPUMP_RTEMS_EVENT_DEBUG_START	\
   __TMS_USBPUMP_RTEMS_EVENT_DEBUG_START
# define USBPUMP_RTEMS_EVENT_DEBUG_EVENT	\
   __TMS_USBPUMP_RTEMS_EVENT_DEBUG_EVENT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_rtems.h ****/
#endif /* _USBPUMP_RTEMS_H_ */
