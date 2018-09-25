/* usbpump_timer.h	Wed Feb 15 2017 16:30:06 chwon */

/*

Module:  usbpump_timer.h

Function:
	Definitions for the abstract TIMER facility for the DataPump

Version:
	V3.19b	Wed Feb 15 2017 16:30:06 chwon	Edit level 7

Copyright notice:
	This file copyright (C) 2005, 2009, 2015, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	July 2005

Revision history:
   1.91d  Fri Jul 01 2005 04:57:31  chwon
	Module created.

   1.91h  Fri Aug 26 2005 15:04:29  chwon
	Move some definitions to central types file.
	Add timer event handling and add UsbPumpTimer_TimerUpcallTick()

   1.91h  Tue Sep 13 2005 19:45:55  chwon
	Add timer tick start and stop methods

   2.01a  Wed Nov 04 2009 13:48:32  chwon
	9246: add USBPUMP_TIMER_TIMEOUT, USBPUMP_TIMER_QUEUE_INDEX type
	definition with compile time option for adjusting size.  Use these 
	types instead of USHORT.

   2.01a  Thu Dec 10 2009 19:47:01  maw
   	9590: Add timer resolution to context

   3.15c  Tue Aug 25 2015 14:57:04  tmm
	19019: improve documentation.

   3.19b  Wed Feb 15 2017 16:30:06  chwon
	20449: Define USBPUMP_TIMER_QUEUE_INDEX using USBPUMP_TIMER_TIMEOUT.

*/

#ifndef _USBPUMP_TIMER_H_		/* prevent multiple includes */
#define _USBPUMP_TIMER_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USTAT_H_
# include "ustat.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

/****************************************************************************\
|
|	Forward definitions
|
\****************************************************************************/


/*

Type:	USBPUMP_TIMER_TIMEOUT

Index:	Name:	USBPUMP_TIMER_QUEUE_INDEX

Function:
	The integral type for timeouts, and the matching type for
	timer queue indices.

Definition:
	#include "usbpump_timer.h"

	typedef UINT<n> USBPUMP_TIMER_TIMEOUT;
	typedef UINT<n> USBPUMP_TIMER_QUEUE_INDEX;

Description:
	USBPUMP_TIMER_TIMEOUT is used for specifying timeouts; and
	USBPUMP_TIMER_QUEUE_INDEX is used for selecting the appropriate
	timer queue when inserting a timer in the timer queues.

	Timeouts are specified in units of 1 millisecond. Timer objects are
	embecded in many data structures, so in order to minimize, these types
	are nominally half a pointer wide: UINT16 on 32-bit systems, thereby
	allowing a timer object to be stored in 4 dwords (16 bytes). On 
	64-bit systems, these types could be either 16 or 32 bits wide;
	because of padding, a timer object will typically be 32 bytes long
	in any case.

	For 64-bit systems, and in applications where it's awkward having 
	timeouts that are limited to 65 seconds, we have a compile time 
	option __TMS_USE_USBPUMP_TIMER_32BIT that selects 32-bit values for
	both these types. 

Notes:
	MCCI does not normally test use of 32-bit timeouts and queue-indices;
	we'll accept bug reports but we do not guarantee proper operation.

*/

/*
|| MCCI does not support users overriding changing to values of 32 bit timer.
*/
#ifndef	__TMS_USE_USBPUMP_TIMER_32BIT	/* PARAM */
# define __TMS_USE_USBPUMP_TIMER_32BIT	0
#endif

#if __TMS_USE_USBPUMP_TIMER_32BIT
__TMS_TYPE_DEF_ARG(USBPUMP_TIMER_TIMEOUT, UINT32);
#else
__TMS_TYPE_DEF_ARG(USBPUMP_TIMER_TIMEOUT, UINT16);
#endif

__TMS_TYPE_DEF_ARG(USBPUMP_TIMER_QUEUE_INDEX, USBPUMP_TIMER_TIMEOUT);

/*

Type:	USBPUMP_TIMER_DONE_FN

Function:
	Symbolic type for USB DataPump timer completion callback functions.

Definition:
	typedef VOID USBPUMP_TIMER_DONE_FN(
			UPLATFORM *pPlatform,
			USBPUMP_TIMER *pTimerObject,
			USBPUMP_MILLISECONDS CurrentTickCounter
			);

Description:
	The timer client provides a USBPUMP_TIMER_DONE_FN when calling
	UsbPumpTimer_Initialize() to set up the timer. Later, when the timer
	fires, the DataPump calls this function from the event loop (in
	DataPump context). pPlatform will be set to point to the UPLATFORM,
	pTimerObject points to the timer object that fired, and 
	CurrentTickCounter is set to the value of the real time clock at
	the time the timer fired.

Returns:
	No explicit result.

*/

__TMS_FNTYPE_DEF(USBPUMP_TIMER_DONE_FN,
__TMS_VOID,
	(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_USBPUMP_TIMER *		/* pTimerObject */,
	__TMS_USBPUMP_MILLISECONDS	/* CurrentTickCounter */
	));

/****************************************************************************\
|
|	The timer structure
|
\****************************************************************************/

/* in usbpump_types.h: __TMS_TYPE_DEF_STRUCT(USBPUMP_TIMER); */

struct __TMS_STRUCTNAME(USBPUMP_TIMER)
	{
	__TMS_USBPUMP_TIMER_DONE_FN *	pDoneFn;
	__TMS_USBPUMP_TIMER *		pNext;
	__TMS_USBPUMP_TIMER *		pLast;
	__TMS_USBPUMP_TIMER_QUEUE_INDEX	QueueIndex;
	__TMS_USBPUMP_TIMER_TIMEOUT	Ticks;
	};

/****************************************************************************\
|
|	The timer switch structure
|
\****************************************************************************/

/* in usbpump_types.h: __TMS_TYPE_DEF_STRUCT(USBPUMP_TIMER_SWITCH); */

/* Function for initializing the timer system. */
__TMS_FNTYPE_DEF(USBPUMP_TIMER_SYSTEM_INITIALIZE_FN,
__TMS_VOID *,
	(
	__TMS_UPLATFORM *		/* pPlatform */
	));

/* Notification function for timer updates */
__TMS_FNTYPE_DEF(USBPUMP_TIMER_UPCALL_TICK_FN,
__TMS_VOID,
	(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_USBPUMP_MILLISECONDS	/* CurrentTime */
	));

/* Method for UsbPumpTimer_Initialize */
__TMS_FNTYPE_DEF(USBPUMP_TIMER_INITIALIZE_FN,
__TMS_VOID,
	(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_USBPUMP_TIMER *		/* pTimerObject */,
	__TMS_USBPUMP_TIMER_DONE_FN *	/* pDoneFn */
	));

/* Method for UsbPumpTimer_Start */
__TMS_FNTYPE_DEF(USBPUMP_TIMER_START_FN,
__TMS_USTAT,
	(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_USBPUMP_TIMER *		/* pTimerObject */,
	__TMS_ARG_USBPUMP_TIMER_TIMEOUT	/* nMillisecs */,
	__TMS_USBPUMP_MILLISECONDS *	/* pStartTime -- OPTIONAL */
	));

/* Method for UsbPumpTimer_Cancel */
__TMS_FNTYPE_DEF(USBPUMP_TIMER_CANCEL_FN,
__TMS_BOOL,
	(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_USBPUMP_TIMER *		/* pTimerObject */
	));

/* Function for start timer tick interrupt */
__TMS_FNTYPE_DEF(USBPUMP_TIMER_TICK_START_FN,
__TMS_USBPUMP_MILLISECONDS,
	(
	__TMS_UPLATFORM *		/* pPlatform */
	));

/* Function for stop timer tick interrupt */
__TMS_FNTYPE_DEF(USBPUMP_TIMER_TICK_STOP_FN,
__TMS_VOID,
	(
	__TMS_UPLATFORM *		/* pPlatform */
	));


struct __TMS_STRUCTNAME(USBPUMP_TIMER_SWITCH)
	{
	__TMS_USBPUMP_TIMER_SYSTEM_INITIALIZE_FN *	pTimerSystemInitialize;
	__TMS_USBPUMP_TIMER_UPCALL_TICK_FN *		pTimerUpcallTick;
	__TMS_USBPUMP_TIMER_INITIALIZE_FN *		pTimerInitialize;
	__TMS_USBPUMP_TIMER_START_FN *			pTimerStart;
	__TMS_USBPUMP_TIMER_CANCEL_FN *			pTimerCancel;
	__TMS_USBPUMP_TIMER_TICK_START_FN *		pTimerTickStart;
	__TMS_USBPUMP_TIMER_TICK_STOP_FN *		pTimerTickStop;
	};

/*

Name:  USBPUMP_TIMER_SWITCH_INIT_V1()

Function:
	Initialize a USBPUMP_TIMER_SWITCH structure.

Definition:
	VOID USBPUMP_TIMER_SWITCH_INIT_V1(
		USBPUMP_TIMER_INITIALIZE_FN *	pTimerSystemInitialize,
		USBPUMP_TIMER_UPCALL_TICK_FN *	pTimerUpcallTick,
		USBPUMP_TIMER_INITIALIZE_FN *	pTimerInitialize,
		USBPUMP_TIMER_START_FN *	pTimerStart,
		USBPUMP_TIMER_CANCEL_FN *	pTimerCancel,
		USBPUMP_TIMER_TICK_START_FN *	pTimerTickStart,
		USBPUMP_TIMER_TICK_STOP_FN *	pTimerTickStop
		);

Description:
	This macro generates the code to initialize a USBPUMP_TIMER_SWITCH
	according to V1 semantics.  This macro should be used instead of
	initializing the USBPUMP_TIMER_SWITCH manually, because it will be
	revised to maintain upward compatibility whenever new fields are added
	to the USBPUMP_TIMER_SWITCH.

Returns:
	No explicit result.

*/

/* don't add parameters to this macro; instead create a V2 */
#define	__TMS_USBPUMP_TIMER_SWITCH_INIT_V1(		\
		pTimerSystemInitialize,			\
		pTimerUpcallTick,			\
		pTimerInitialize,			\
		pTimerStart,				\
		pTimerCancel,				\
		pTimerTickStart,			\
		pTimerTickStop				\
		)					\
		{					\
		(pTimerSystemInitialize),		\
		(pTimerUpcallTick),			\
		(pTimerInitialize),			\
		(pTimerStart),				\
		(pTimerCancel),				\
		(pTimerTickStart),			\
		(pTimerTickStop)			\
		}

/****************************************************************************\
|
|	DataPump standard timer implementation -- timer context structure
|
\****************************************************************************/

#ifndef	__TMS_USBPUMP_TIMER_QUEUE_SIZE	/* PARAM */
# define __TMS_USBPUMP_TIMER_QUEUE_SIZE	128
#endif

/*
|| Verify that the number of timer queue entries is a power of two
*/
__TMS_LIBPORT_C_ASSERT((__TMS_USBPUMP_TIMER_QUEUE_SIZE & (__TMS_USBPUMP_TIMER_QUEUE_SIZE-1)) == 0);

__TMS_TYPE_DEF_STRUCT(USBPUMP_TIMER_CONTEXT);

struct __TMS_STRUCTNAME(USBPUMP_TIMER_CONTEXT)
	{
	__TMS_USBPUMP_MILLISECONDS	CurrentTime;
	__TMS_USBPUMP_MILLISECONDS	TimerResolution;
	__TMS_ULONG			nTimerObject;
	__TMS_USBPUMP_TIMER *		TimerQueue[__TMS_USBPUMP_TIMER_QUEUE_SIZE];

	__TMS_UPLATFORM *		pPlatform;

	/* Timer event handling */
	__TMS_USBPUMP_MILLISECONDS	TimerEventTime;
	__TMS_UCALLBACKCOMPLETION	TimerEvent;
	};

/****************************************************************************\
|
|	Functions, etc.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/* DataPump Timer API */
__TMS_USBPUMP_TIMER_CANCEL_FN		UsbPumpTimer_Cancel;
__TMS_USBPUMP_TIMER_INITIALIZE_FN	UsbPumpTimer_Initialize;
__TMS_USBPUMP_TIMER_START_FN		UsbPumpTimer_Start;
__TMS_USBPUMP_TIMER_UPCALL_TICK_FN	UsbPumpTimer_TimerUpcallTick;

__TMS_BOOL	UsbPumpTimer_SystemInitialize(
			__TMS_UPLATFORM *			pPlatform,
			__TMS_CONST __TMS_USBPUMP_TIMER_SWITCH *pTimerSwitch
			);

/*
|| DataPump standard timer implementation -- Timer switch functions
||	DO NOT CALL THIS FUNCTION DIRECTLY!!!
*/
__TMS_USBPUMP_TIMER_SYSTEM_INITIALIZE_FN UsbPumpTimer_Sw_SystemInitialize;
__TMS_USBPUMP_TIMER_UPCALL_TICK_FN	UsbPumpTimer_Sw_TimerUpcallTick;
__TMS_USBPUMP_TIMER_CANCEL_FN		UsbPumpTimer_Sw_Cancel;
__TMS_USBPUMP_TIMER_INITIALIZE_FN	UsbPumpTimer_Sw_Initialize;
__TMS_USBPUMP_TIMER_START_FN		UsbPumpTimer_Sw_Start;

extern __TMS_CONST __TMS_USBPUMP_TIMER_SWITCH	gk_UsbPumpTimer_Switch;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USE_USBPUMP_TIMER_32BIT	\
   __TMS_USE_USBPUMP_TIMER_32BIT
# define USBPUMP_TIMER_SWITCH_INIT_V1(		\
		pTimerSystemInitialize,			\
		pTimerUpcallTick,			\
		pTimerInitialize,			\
		pTimerStart,				\
		pTimerCancel,				\
		pTimerTickStart,			\
		pTimerTickStop				\
		)	\
	__TMS_USBPUMP_TIMER_SWITCH_INIT_V1(		\
		pTimerSystemInitialize,			\
		pTimerUpcallTick,			\
		pTimerInitialize,			\
		pTimerStart,				\
		pTimerCancel,				\
		pTimerTickStart,			\
		pTimerTickStop				\
		)
# define USBPUMP_TIMER_QUEUE_SIZE	\
   __TMS_USBPUMP_TIMER_QUEUE_SIZE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_timer.h ****/
#endif /* _USBPUMP_TIMER_H_ */
