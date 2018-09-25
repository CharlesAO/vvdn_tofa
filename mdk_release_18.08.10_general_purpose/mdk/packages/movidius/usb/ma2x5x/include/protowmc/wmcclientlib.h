/* wmcclientlib.h	Mon March 8 2004 09:35:00  maw */

/*

Module:  wmcclientlib.h

Function:
	Prototypes for use by upper layer clients (os client drivers and
	the like).

Version:
	V1.87b	Mon March 8 2004 09:35:00	maw	Edit level 2

Copyright notice:
	This file copyright (C) 2002-2004 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Wed Jul 31 2002 17:53:13  tmm
	Module created.
	
   1.87b  Mon March 8 2004 09:35:00  maw
   	1226: WmcClientLib_CancelIn() and WmcClientLib_CancelOut() added

*/

#ifndef _WMCCLIENTLIB_H_		/* prevent multiple includes */
#define _WMCCLIENTLIB_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBIOCTL_MODEM_H_
# include "usbioctl_modem.h"
#endif

/****************************************************************************\
|
|	The client library functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_IOCTL_RESULT
WmcClientLib_GetPlaneInfo(
	__TMS_USBPUMP_OBJECT_HEADER *pIoObject,
	__TMS_BOOL *pfHasControlPlane,
	__TMS_UINT *pnDataPlanes
	);

__TMS_USBPUMP_IOCTL_RESULT 
WmcClientLib_QueueIn(
	__TMS_USBPUMP_OBJECT_HEADER *pIoObject,
	__TMS_UBUFQE *pListHead,
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR iPlane
	);

__TMS_USBPUMP_IOCTL_RESULT 
WmcClientLib_QueueOut(
	__TMS_USBPUMP_OBJECT_HEADER *pIoObject,
	__TMS_UBUFQE *pListHead,
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR iPlane
	);

__TMS_USBPUMP_IOCTL_RESULT 
WmcClientLib_CancelIn(
	__TMS_USBPUMP_OBJECT_HEADER *pIoObject,
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR iPlane,
	__TMS_USTAT Why
	);

__TMS_USBPUMP_IOCTL_RESULT 
WmcClientLib_CancelOut(
	__TMS_USBPUMP_OBJECT_HEADER *pIoObject,
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR iPlane,
	__TMS_USTAT Why
	);

__TMS_UINT
WmcClientLib_SetUartState(
	__TMS_USBPUMP_OBJECT_HEADER *pObject,
	__TMS_UINT ModemMask,
	__TMS_UINT ModemValue
	);

__TMS_END_DECLS

/**** end of wmcclientlib.h ****/
#endif /* _WMCCLIENTLIB_H_ */
