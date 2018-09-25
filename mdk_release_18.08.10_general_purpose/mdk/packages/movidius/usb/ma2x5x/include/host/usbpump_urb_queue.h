/* usbpump_urb_queue.h	Sun Aug 13 2006 20:37:01 tmm */

/*

Module:  usbpump_urb_queue.h

Function:
	Definitions related to USBPUMP_URB_QUEUEs.

Version:
	V1.97e	Sun Aug 13 2006 20:37:01 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2005-2006 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	September 2005

Revision history:
   1.91h  Tue Sep  6 2005 03:54:13  tmm
	Module created.

   1.97e  Sun Aug 13 2006 20:37:01  tmm
	Correct documentation typo

*/

#ifndef _USBPUMP_URB_QUEUE_H_		/* prevent multiple includes */
#define _USBPUMP_URB_QUEUE_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

/*

Type:	USBPUMP_URB_QUEUE

Index:	Type:	USBPUMP_URB_QUEUE_ARRIVAL_FN
	Type:	PUSBPUMP_URB_QUEUE_ARRIVAL_FN

Function:
	An object for automating serialization and deferral of processing
	of queues of URBs.

Contents:
	UCALLBACKCOMPLETION ArrivalClosure;
		An internal closure used for scheduling deferred
		processing via the DataPump event queue.

	UPLATFORM *pPlatform;
		The platform, for reference in doing event processing.

	USBPUMP_URB *pHead;
		The head of the queue of URBs.

	USBPUMP_URB_QUEUE_ARRIVAL_FN *pArrivalFn;
	VOID *pArrivalContext;
		Closure informationfor an arrival processing function.
		This function is scheduled for processing whenever the
		queue changes state from empty to non-empty.

	The symboic type for the arrival function is defined as

	typedef VOID USBPUMP_URB_QUEUE_ARRIVAL_FN(
			USBPUMP_URB_QUEUE *pQueue,
			VOID *pArrivalContext
			);

Notes:
	The queue object should be initialized using
	UsbPumpUrbQueue_Initialize_V1().

See Also:
	UsbPumpUrbQueue_Initialize_V1()
	UsbPumpUrbQueue_PutUrb()

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_URB_QUEUE);

__TMS_FNTYPE_DEF(
USBPUMP_URB_QUEUE_ARRIVAL_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_URB_QUEUE *,
	__TMS_VOID *
	));

struct __TMS_STRUCTNAME	(USBPUMP_URB_QUEUE)
	{
	__TMS_UCALLBACKCOMPLETION		ArrivalClosure;
	__TMS_UPLATFORM				*pPlatform;
	__TMS_USBPUMP_URB			*pHead;
	__TMS_USBPUMP_URB_QUEUE_ARRIVAL_FN	*pArrivalFn;
	__TMS_VOID				*pArrivalContext;
	};

/****************************************************************************\
|
|	Functional APIs
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UCALLBACKFN
UsbPumpUrbQueueI_ArrivalCallback;

__TMS_VOID
UsbPumpUrbQueue_Initialize_V1(
	__TMS_USBPUMP_URB_QUEUE * /*pQueue*/,
	__TMS_USBPUMP_URB_QUEUE_ARRIVAL_FN * /*pArrivalFn*/,
	__TMS_VOID * /*pArrivalContext*/,
	__TMS_UPLATFORM * /*pPlatform*/
	);

__TMS_VOID
UsbPumpUrbQueue_PutUrb(
	__TMS_USBPUMP_URB_QUEUE *,
	__TMS_USBPUMP_URB *
	);

__TMS_VOID
UsbPumpUrbQueue_PutUrbCancellable(
	__TMS_USBPUMP_URB_QUEUE *,
	__TMS_USBPUMP_URB *
	);

__TMS_END_DECLS

/**** end of usbpump_urb_queue.h ****/
#endif /* _USBPUMP_URB_QUEUE_H_ */
