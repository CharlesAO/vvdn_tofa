/* usbpumplib_autoremotewakeup.h	Fri Oct 02 2015 12:22:57 chwon */

/*

Module:  usbpumplib_autoremotewakeup.h

Function:
	Definition of auto remote wakeup

Version:
	V3.15d	Fri Oct 02 2015 12:22:57 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2014-2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	August 2014

Revision history:
   3.13b  Sat Aug 16 2014 19:05:37 chwon
	18465: Module created.

   3.15d  Fri Oct 02 2015 12:22:58  chwon
	19506: Add function wakeup support.

*/

#ifndef _USBPUMPLIB_AUTOREMOTEWAKEUP_H_	/* prevent multiple includes */
#define _USBPUMPLIB_AUTOREMOTEWAKEUP_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

#ifndef _UEVENTNODE_H_
# include "ueventnode.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

/****************************************************************************\
|
|	Types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_AUTO_REMOTE_WAKEUP_CONTEXT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_AUTO_REMOTE_WAKEUP_FUNCTION);


/****************************************************************************\
|
|	Structure of USBPUMP_AUTO_REMOTE_WAKEUP_CONTEXT
|
\****************************************************************************/

struct __TMS_STRUCTNAME	(USBPUMP_AUTO_REMOTE_WAKEUP_CONTEXT)
	{
	__TMS_UPLATFORM *				pPlatform;
	__TMS_UDEVICE *					pDevice;
	__TMS_USBPUMP_TIMER_TIMEOUT			DelayAfterSuspend;

	__TMS_USBPUMP_TIMER_TIMEOUT			CallbackCount;

	__TMS_UINT					fAutoWakeEnabled: 1;
	__TMS_UINT					fTimerRunning: 1;

	__TMS_UEVENTNODE				DeviceEventNode;
	__TMS_USBPUMP_TIMER				Timer;
	__TMS_UCALLBACKCOMPLETION			Callback;

	__TMS_UINTERFACE *				pInterface;
	__TMS_USBPUMP_AUTO_REMOTE_WAKEUP_FUNCTION *	pFunctions;
	};


/****************************************************************************\
|
|	Structure of USBPUMP_AUTO_REMOTE_WAKEUP_FUNCTION
|
\****************************************************************************/

struct __TMS_STRUCTNAME	(USBPUMP_AUTO_REMOTE_WAKEUP_FUNCTION)
	{
	__TMS_USBPUMP_AUTO_REMOTE_WAKEUP_CONTEXT *	pAutoWakeupCtx;

	__TMS_USBPUMP_AUTO_REMOTE_WAKEUP_FUNCTION *	pNext;
	__TMS_USBPUMP_AUTO_REMOTE_WAKEUP_FUNCTION *	pLast;

	__TMS_UINTERFACE *				pInterface;
	__TMS_UEVENTNODE				EventNode;
	};


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID *
UsbPumpLib_CreateAutoRemoteWakeup(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_USBPUMP_MILLISECONDS	/* DelayAfterSuspend */
	);

__TMS_VOID
UsbPumpLib_DeleteAutoRemoteWakeup(
	__TMS_VOID *			/* pAutoWakeupContext */
	);

__TMS_END_DECLS

/**** end of usbpumplib_autoremotewakeup.h ****/
#endif /* _USBPUMPLIB_AUTOREMOTEWAKEUP_H_ */
