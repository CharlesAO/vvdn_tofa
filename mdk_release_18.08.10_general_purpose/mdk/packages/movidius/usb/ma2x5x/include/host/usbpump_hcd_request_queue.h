/* usbpump_hcd_request_queue.h	Tue Sep  6 2005 04:02:25 tmm */

/*

Module:  usbpump_hcd_request_queue.h

Function:
	Defines USBPUMP_HCD_REQUEST_QUEUEs.

Version:
	V1.91h	Tue Sep  6 2005 04:02:25 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	September 2005

Revision history:
   1.91h  Thu Sep  1 2005 03:26:33  tmm
	Module created.

   1.91h  Tue Sep  6 2005 04:02:25  tmm
	Fix unused argument.

*/

#ifndef _USBPUMP_HCD_REQUEST_QUEUE_H_		/* prevent multiple includes */
#define _USBPUMP_HCD_REQUEST_QUEUE_H_

#ifndef _USBPUMP_HCD_TYPES_H_
# include "usbpump_hcd_types.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

/*

Type:	USBPUMP_HCD_REQUEST_QUEUE

Index:	Type:	USBPUMP_HCD_REQUEST_QUEUE_ARRIVAL_FN
	Type:	PUSBPUMP_HCD_REQUEST_QUEUE_ARRIVAL_FN

Function:
	An object for automating serializing and processing queues of
	HCD requests.

Contents:
	UCALLBACKCOMPLETION ArrivalClosure;
		An internal closure used for scheduling deferred processing 
		to the DataPump event queue

	UPLATFORM *pPlatform;
		The platform, for reference in doing event processing.

	USBPUMP_HCD_REQUEST *pHead;
		The request queue.

	USBPUMP_HCD_REQUEST_QUEUE_ARRIVAL_FN *pArrivalFn;
	VOID *pArrivalContext;
		Closure information for an arrival processing function.
		This function is scheduled for processing whenever the
		queue changes state from empty to non-empty.  

	The symbolic type for the arrival function is defined as:

	typedef VOID USBPUMP_HCD_REQUEST_QUEUE_ARRIVAL_FN(
			USBPUMP_HCD_REQUEST_QUEUE *pQueue,
			VOID *pArrivalContext
			);

Notes:
	The queue object should be initialied using
	UsbPumpHcdRequestQueue_Initialize_V1().

See Also:
	UsbPumpHcdRequestQueue_Iniitialize_V1(),
	UsbPumpHcdRequestQueue_PutRequest().

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_QUEUE);

__TMS_FNTYPE_DEF(
USBPUMP_HCD_REQUEST_QUEUE_ARRIVAL_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCD_REQUEST_QUEUE * /*pQueue*/,
	__TMS_VOID * /*pArrivalContext*/
	));

/****************************************************************************\
|
|	Functional APIs
|
\****************************************************************************/


struct __TMS_STRUCTNAME	(USBPUMP_HCD_REQUEST_QUEUE)
	{
	__TMS_UCALLBACKCOMPLETION			ArrivalClosure;
	__TMS_UPLATFORM					*pPlatform;
	__TMS_USBPUMP_HCD_REQUEST			*pHead;
	__TMS_USBPUMP_HCD_REQUEST_QUEUE_ARRIVAL_FN	*pArrivalFn;
	__TMS_VOID					*pArrivalContext;
	};

__TMS_BEGIN_DECLS

__TMS_UCALLBACKFN
UsbPumpHcdRequestQueueI_ArrivalCallback;

__TMS_VOID
UsbPumpHcdRequestQueue_Initialize_V1(
	__TMS_USBPUMP_HCD_REQUEST_QUEUE * /*pQueue*/,
	__TMS_USBPUMP_HCD_REQUEST_QUEUE_ARRIVAL_FN * /*pArrivalFn*/,
	__TMS_VOID * /*pArrivalContext*/,
	__TMS_UPLATFORM * /*pPlatform*/
	);

__TMS_VOID
UsbPumpHcdRequestQueue_PutRequest(
	__TMS_USBPUMP_HCD_REQUEST_QUEUE * /*pQueue*/,
	__TMS_USBPUMP_HCD_REQUEST * /*pRequest*/
	);

__TMS_END_DECLS

/**** end of usbpump_hcd_request_queue.h ****/
#endif /* _USBPUMP_HCD_REQUEST_QUEUE_H_ */
