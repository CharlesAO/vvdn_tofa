/* usbpump_hcd_inswitch.h	Tue Sep 10 2013 11:44:58 chwon */

/*

Module:  usbpump_hcd_inswitch.h

Function:
	Inbound switch contents for HCD API.

Version:
	V3.11d	Tue Sep 10 2013 11:44:58 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2005, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Wed Jun  8 2005 22:06:24  tmm
	Module created.

   1.91h  Fri Sep  2 2005 00:16:14  tmm
	Rename USBPUMP_HCD_CANCEL_REQUEST_FN to 
	USBPUMP_HCD_INSWITCH_CANCEL_REQUEST_FN and 
	USBPUMP_HCD_SUBMIT_REQUEST_FN to USBPUMP_HCD_INSWITCH_SUBMIT_REQUEST_FN

   3.11d  Tue Sep 10 2013 11:44:58  chwon
	17686: Fixed typo; USPBUMP should be USBPUMP. Fixed company name.

*/

#ifndef _USBPUMP_HCD_INSWITCH_H_		/* prevent multiple includes */
#define _USBPUMP_HCD_INSWITCH_H_

#ifndef _USBPUMP_HCD_TYPES_H_
# include "usbpump_hcd_types.h"
#endif

/****************************************************************************\
|
|	The pipe-based requests.
|
\****************************************************************************/


/* 
|| this function type is used for submitting a data transfer reqeust
|| to the HCD.
*/
__TMS_FNTYPE_DEF(
USBPUMP_HCD_INSWITCH_SUBMIT_REQUEST_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCD *			/* pHcd */,
	__TMS_USBPUMP_HCD_REQUEST *		/* pHead */,
	__TMS_USBPUMP_HCD_REQUEST_DONE_FN *	/* pDoneFn */,
	__TMS_VOID *				/* pDoneInfo */
	));

/*
|| this function type is used for the function that cancels the HCD
|| request.
*/
__TMS_FNTYPE_DEF(
USBPUMP_HCD_INSWITCH_CANCEL_REQUEST_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCD *,
	__TMS_USBPUMP_HCD_REQUEST *
	));

__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_INSWITCH);
struct __TMS_STRUCTNAME	(USBPUMP_HCD_INSWITCH)
	{
	__TMS_USBPUMP_HCD_INSWITCH_SUBMIT_REQUEST_FN	*pSubmitRequestFn;
	__TMS_USBPUMP_HCD_INSWITCH_CANCEL_REQUEST_FN	*pCancelRequestFn;
	};

#define	__TMS_USBPUMP_HCD_INSWITCH_INIT_V1(				\
	a_pSubmitRequestFn,						\
	a_pCancelRequestFn						\
	)								\
	{								\
	(a_pSubmitRequestFn),						\
	(a_pCancelRequestFn)						\
	}

#define __TMS_USBPUMP_HCD_INSWITCH_SETUP_V1(				\
	a_pSwitch,							\
	a_pSubmitRequestFn,						\
	a_pCancelRequestFn						\
	)								\
    do	{								\
	(a_pSwitch)->pSubmitRequestFn = (a_pSubmitRequestFn);		\
	(a_pSwitch)->pCancelRequestFn = (a_pCancelRequestFn);		\
	} while (0)

#define	__TMS_USBPUMP_HCD_INSWITCH_DECLARE_FUNCTIONS_V1(		\
	a_pSubmitRequestFn,						\
	a_pCancelRequestFn						\
	)								\
	__TMS_USBPUMP_HCD_INSWITCH_SUBMIT_REQUEST_FN a_pSubmitRequestFn; \
	__TMS_USBPUMP_HCD_INSWITCH_CANCEL_REQUEST_FN a_pCancelRequestFn; \
	}

#define	__TMS_USBPUMP_HCD_INSWITCH_DECLARE_FUNCTIONS_STD_V1(		\
	Prefix								\
	)								\
	__TMS_USBPUMP_HCD_INSWITCH_DECLARE_FUNCTIONS_STD_V1(		\
		__TMS_CONCAT(Prefix, _SubmitRequest),			\
		__TMS_CONCAT(Prefix, _CancelRequest)			\
		)

/****************************************************************************\
|
|	Cloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_HCD_INSWITCH_INIT_V1(				\
	a_pSubmitRequestFn,						\
	a_pCancelRequestFn						\
	)	\
	__TMS_USBPUMP_HCD_INSWITCH_INIT_V1(				\
	a_pSubmitRequestFn,						\
	a_pCancelRequestFn						\
	)
# define USBPUMP_HCD_INSWITCH_SETUP_V1(				\
	a_pSwitch,							\
	a_pSubmitRequestFn,						\
	a_pCancelRequestFn						\
	)	\
	__TMS_USBPUMP_HCD_INSWITCH_SETUP_V1(				\
	a_pSwitch,							\
	a_pSubmitRequestFn,						\
	a_pCancelRequestFn						\
	)
# define USBPUMP_HCD_INSWITCH_DECLARE_FUNCTIONS_V1(		\
	a_pSubmitRequestFn,						\
	a_pCancelRequestFn						\
	)	\
	__TMS_USBPUMP_HCD_INSWITCH_DECLARE_FUNCTIONS_V1(		\
	a_pSubmitRequestFn,						\
	a_pCancelRequestFn						\
	)
# define USBPUMP_HCD_INSWITCH_DECLARE_FUNCTIONS_STD_V1(		\
	Prefix								\
	)	\
	__TMS_USBPUMP_HCD_INSWITCH_DECLARE_FUNCTIONS_STD_V1(		\
	Prefix								\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_hcd_inswitch.h ****/
#endif /* _USBPUMP_HCD_INSWITCH_H_ */
