/* usbpump_rtems_sync.h	Thu Aug 17 2017 13:43:31 chwon */

/*

Module:  usbpump_rtems_sync.h

Function:
	Definition of RTEMS synchronize

Version:
	V3.21c	Thu Aug 17 2017 13:43:31 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	August 2017

Revision history:
   3.21c  Thu Aug 17 2017 13:43:31 chwon
	20644: Module created.

*/

#ifndef _USBPUMP_RTEMS_SYNC_H_	/* prevent multiple includes */
#define _USBPUMP_RTEMS_SYNC_H_

#ifndef _USBPUMP_RTEMS_H_
# include "usbpump_rtems.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

#ifndef _UPLATFORMAPI_H_
# include "uplatformapi.h"
#endif

/****************************************************************************\
|
|	USBPUMP_RTEMS_SYNC_CONTEXT
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_RTEMS_SYNC_CONTEXT);
struct __TMS_STRUCTNAME	(USBPUMP_RTEMS_SYNC_CONTEXT)
	{
	__TMS_UPLATFORM *			pPlatform;
	__TMS_UCALLBACKFN *			pSynchFn;
	__TMS_VOID *				pSynchCtx;
	__TMS_UCALLBACKCOMPLETION		Callback;
	rtems_id				TaskId;
	};


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpRtems_SynchronizeToDataPump(
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_UCALLBACKFN *	/* pSynchFn */,
	__TMS_VOID *		/* pSynchCtx */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/


/**** end of usbpump_rtems_sync.h ****/
#endif /* _USBPUMP_RTEMS_SYNC_H_ */
