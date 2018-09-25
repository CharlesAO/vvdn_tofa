/* usbpumplib_periodictimer.h	Tue Aug 26 2014 11:35:39 chwon */

/*

Module:  usbpumplib_periodictimer.h

Function:
	Definition of periodic timer

Version:
	V3.13b	Tue Aug 26 2014 11:35:39 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	August 2014

Revision history:
   3.13b  Wed Aug 20 2014 09:06:48 chwon
	18465: Module created.

   3.13b  Tue Aug 26 2014 11:35:39  chwon
	18465: Add type cloaking.

*/

#ifndef _USBPUMPLIB_PERIODICTIMER_H_	/* prevent multiple includes */
#define _USBPUMPLIB_PERIODICTIMER_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

/****************************************************************************\
|
|	Structure of USBPUMP_PERIODIC_TIMER
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_PERIODIC_TIMER);
struct __TMS_STRUCTNAME	(USBPUMP_PERIODIC_TIMER)
	{
	__TMS_UPLATFORM *		pPlatform;
	__TMS_USBPUMP_TIMER_TIMEOUT	PeriodicTime;
	__TMS_USBPUMP_TIMER		Timer;
	__TMS_UCALLBACKCOMPLETION	Callback;
	};


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_PERIODIC_TIMER *
UsbPumpLib_CreatePeriodicTimer(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_UCALLBACKFN *		/* pTimeoutCallbackFn */,
	__TMS_VOID *			/* pTimeoutCallbackCtx */,
	__TMS_USBPUMP_TIMER_TIMEOUT	/* nMillisecs */
	);

__TMS_VOID
UsbPumpLib_DeletePeriodicTimer(
	__TMS_USBPUMP_PERIODIC_TIMER *	/* pPeriodicTimer */
	);

__TMS_END_DECLS

/**** end of usbpumplib_periodictimer.h ****/
#endif /* _USBPUMPLIB_PERIODICTIMER_H_ */
