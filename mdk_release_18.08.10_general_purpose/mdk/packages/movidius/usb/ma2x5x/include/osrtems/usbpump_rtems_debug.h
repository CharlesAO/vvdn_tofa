/* usbpump_rtems_debug.h	Mon Sep 15 2014 12:33:40 chwon */

/*

Module:  usbpump_rtems_debug.h

Function:
	Debug print definitions.

Version:
	V3.13b	Mon Sep 15 2014 12:33:40 chwon	Edit level 1

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

*/

#ifndef _USBPUMP_RTEMS_DEBUG_H_	/* prevent multiple includes */
#define _USBPUMP_RTEMS_DEBUG_H_

#ifndef _USBPUMP_RTEMS_H_
# include "usbpump_rtems.h"
#endif

#ifndef _UHILDBG_H_
# include "uhildbg.h"
#endif

/****************************************************************************\
|
|	Rtems debug context definition
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT  (USBPUMP_RTEMS_DEBUG_CONTEXT);
struct __TMS_STRUCTNAME(USBPUMP_RTEMS_DEBUG_CONTEXT)
	{
	__TMS_UPLATFORM_DEBUG_PRINT_CONTROL_HDR;

	__TMS_UPLATFORM_RTEMS *		pPlatformRtems;

	__TMS_UINT			fInitialized: 1;
	__TMS_UINT			fEnabled: 1;
	__TMS_UINT			fSendEmptyEvent: 1;

	__TMS_CHAR *			pHead;
	__TMS_CHAR *			pTail;
	__TMS_CHAR *			pBase;
	__TMS_CHAR *			pEnd;

	/* OS interface variables */
	rtems_id			TaskId;
	};


#define	__TMS_UPLATFORM_TO_USBPUMP_RTEMS_DEBUG(pPlatform)		\
	__TMS_CONTAINER_OF(						\
		&(pPlatform)->upf_pDebugPrintControl->upfdbg_hdr,	\
		__TMS_USBPUMP_RTEMS_DEBUG_CONTEXT,			\
		upfdbg_hdr						\
		)

#define	__TMS_USBPUMP_RTEMS_DEBUG_TO_UPLATFORM_DEBUG_PRINT_CONTROL(pCtx) \
	__TMS_CONTAINER_OF(						\
		&(pCtx)->upfdbg_hdr,					\
		__TMS_UPLATFORM_DEBUG_PRINT_CONTROL,			\
		upfdbg_hdr						\
		)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL
UsbPumpRtems_DebugPrintInit(
	__TMS_UPLATFORM *pPlatform
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPLATFORM_TO_USBPUMP_RTEMS_DEBUG(pPlatform)	\
   __TMS_UPLATFORM_TO_USBPUMP_RTEMS_DEBUG(pPlatform)
# define USBPUMP_RTEMS_DEBUG_TO_UPLATFORM_DEBUG_PRINT_CONTROL(p)	\
   __TMS_USBPUMP_RTEMS_DEBUG_TO_UPLATFORM_DEBUG_PRINT_CONTROL(p)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_rtems_debug.h ****/
#endif /* _USBPUMP_RTEMS_DEBUG_H_ */
