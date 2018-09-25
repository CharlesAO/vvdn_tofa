/* usbpump_hcd_types.h	Sat Sep 24 2005 00:18:07 chwon */

/*

Module:  usbpump_hcd_types.h

Function:
	Types specific to HCD-level objects.

Version:
	V1.91i	Sat Sep 24 2005 00:18:07 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	May 2005

Revision history:
   1.91d  Tue May 31 2005 00:27:42  tmm
	Module created.

   1.91h  2005-08-31  tmm
	Move to central place, as some highly cross-coupled DCD/HCD hardware
	really needs to be able to refer to this even if the user's not using
	the DCD.

   1.91i  Sat Sep 24 2005 00:18:07  chwon
	Move USBPUMP_HCD type from usbpump_types.h.

*/

#ifndef _USBPUMP_HCD_TYPES_H_		/* prevent multiple includes */
#define _USBPUMP_HCD_TYPES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	Declare common central types used by any HCD implementation, so
|	that boundary header files can refer to the types (even if they
|	can't be used without the full DataPump host stack.
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION(USBPUMP_HCD);
__TMS_TYPE_DEF_STRUCT(USBPUMP_HCD_DEVICE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_HCD_PIPE);
__TMS_TYPE_DEF_UNION(USBPUMP_HCD_REQUEST);

/*

Type:	USBPUMP_HCD_REQUEST_DONE_FN

Index:	Type:	PUSBPUMP_HCD_REQUEST_DONE_FN

Function:
	Called to report that an HCD request has completed

Definition:
	typedef VOID USBPUMP_HCD_REQUEST_DONE_FN(
			USBPUMP_HCD *pHcd,
			USBPUMP_HCD_REQUEST *pRequest,
			VOID *pDoneInfo,
			ARG_USTAT Status
			);

	typedef USBPUMP_HCD_REQUEST_DONE_FN
		*PUSBPUMP_HCD_REQUEST_DONE_FN;

Description:
	Every request passed to an HCD for processing must have a non-NULL
	pointer to the completion function in its Hdr.pDoneFn.  This
	function is called after the HCD has completed all transfer-related
	processing for the 

Returns:
	No explicit result.

*/

__TMS_FNTYPE_DEF(
USBPUMP_HCD_REQUEST_DONE_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_HCD *		/* the HCD involved */,
		__TMS_USBPUMP_HCD_REQUEST *	/* pHcdTransfer */,
		__TMS_VOID *			/* pDoneInfo */,
		__TMS_ARG_USTAT			/* completion status */
		)
	);

/*

Type:	USBPUMP_HCD_REQUEST_CANCEL_FN

Index:	Type:	PUSBPUMP_HCD_REQUEST_CANCEL_FN

Function:
	Function type for request cancellation functions.

Definition:
	typedef USTAT USBPUMP_HCD_REQUEST_CANCEL_FN(
			USBPUMP_HCD_REQUEST *pRequest,
			VOID *pDoneInfo
			);

	typedef USBPUMP_HCD_REQUEST_CANCEL_FN
		*PUSBPUMP_HCD_REQUEST_CANCEL_FN;

Description:
	When an USBPUMP_HCD_REQUEST packet is in a queue, the HCD may
	optionally set a cancel routine to be called if the operation
	has to be cancelled.  If possible, this routine must remove the
	request from whatever queue it's in, and prepare to complete the 
	request.  However, normally completing the request is handled
	by the caller.

Returns:
	USTAT_BUSY (0) --> the request cannot be completed now.
	any other status --> the request is completed with that status.

Notes:
	Normally, the cancel routine should return USTAT_KILL to indicate
	that the request was cancelled.

	Prior to calling this function, the cancel routine is reset.
	Also, prior to calling this function, Hdr.Flags bit
	USBPUMP_HCD_REQUEST_INTERNAL_FLAG_CANCELLED is set.

	If the request is to be completed, any pending timeout is cancelled.

*/

__TMS_FNTYPE_DEF(
USBPUMP_HCD_REQUEST_CANCEL_FN,
	__TMS_USTAT,
		(
		__TMS_USBPUMP_HCD_REQUEST *	/* pHcdTransfer */,
		__TMS_VOID *			/* pCancelInfo */
		)
	);

/**** end of usbpump_hcd_types.h ****/
#endif /* _USBPUMP_HCD_TYPES_H_ */
