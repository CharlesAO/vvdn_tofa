/* usbpump_usbdi_types.h	Fri Mar 30 2012 10:16:52 cccho */

/*

Module:  usbpump_usbdi_types.h

Function:
	Provides the centralized scalar types and non-deferencable pointer
	types for USBDI

Version:
	V3.01f	Fri Mar 30 2012 10:16:52 cccho	Edit level 15

Copyright notice:
	This file copyright (C) 2005-2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Wed Aug 10 2005 19:13:23  tmm
	Module created.

   1.93c  Mon Nov 28 2005 05:01:20  tmm
	Add USBPUMP_USBDI_USBD_CONFIG as a placeholder for future
	possible configuration info.

   1.97j  Tue Aug 21 2007 14:00:44  djt
	3265: Added USBPUMP_USBDI_TT and USBPUMP_USBDI_USBDTT_CONFIG.

   1.97k  Thu Dec 13 2007 18:11:32  djt
	3049: Added CD stuff.

   1.97k  Fri Mar 14 2008 17:12:43  chwon
	3049: remove CD stuff

   1.97k  Fri Jul 11 2008 13:53:56  chwon
	4286: add USB initialization function type definition

   2.01a  Thu Mar 05 2009 10:10:15  djt
	7416: Added USBPUMP_USBDI_USBDSUPERSPEED_INIT_FN,
	USBPUMP_USBDI_USBDSUPERSPEED_CONFIG.

   3.01a  Wed Dec 23 2009 12:34:23  chwon
	9642: add USBPUMP_USBDI_DEVICE_ARRIVAL_INFO,
	USBPUMP_USBDI_NORMALIZED_ROUTE_STRING and
	USBPUMP_USBDI_DEVICE_ARRIVAL_LISTENER_FN

   3.01a  Sun Jan 03 2010 20:20:11  chwon
	9642: change return value of USBPUMP_USBDI_DEVICE_ARRIVAL_LISTENER_FN
	from BOOL to USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT.

   3.01a  Mon Jan 04 2010 23:21:11  chwon
	9688: add USBPUMP_USBDI_DEVICE_DEPARTURE_INFO and
	USBPUMP_USBDI_DEVICE_DEPARTURE_LISTENER_FN

   3.01c  Mon Mar 29 2010 10:08:25  chwon
	10082: add USBPUMP_USBDI_STOP_COMPLETION_LISTENER_FN.

   3.01c  Tue Aug 10 2010 18:44:47  chwon
	10911: move USBPUMP_USBDI_INSTANCE_INFO from usbpump_usbdi_funciton.h.

   3.01c  Mon Aug 16 2010 11:48:57  chwon
	10988: add USBPUMP_USBDI_POWER_STATE type.

   3.01d  Fri Mar 11 2011 09:27:26  chwon
	12923: add USBPUMP_USBDI_POWER_ACTION type.

   3.01f  Fri Mar 30 2012 10:16:52  cccho
	15053: use USBPUMP_POWER_STATE and USBPUMP_POWER_ACTION to define
	USBPUMP_USBDI_POWER_STATE and USBPUMP_USBDI_POWER_STATE respectively

*/

#ifndef _USBPUMP_USBDI_TYPES_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_TYPES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif /* _USBPUMP_TYPES_H_ */

/****************************************************************************\
|
|	Structure pointers, union pointers, scalar types etc.
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION	(USBPUMP_URB);
__TMS_TYPE_DEF_HANDLE	(USBPUMP_USBDI_DEVICE_HANDLE);

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_DRIVER_CLASS);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_DRIVER_CLASS_CONFIG);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION);

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_INSTANCE_INFO);
__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_FUNCTION);

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_INIT_MATCH_LIST);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY);

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_MATCH_LIST_HEADER);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBDI_MATCH_PRIORITY, UINT16);

__TMS_TYPE_DEF		(USBPUMP_USBDI_PIPE_HANDLE, 
			struct __TMS_STRUCTNAME(USBPUMP_USBDI_PIPE *)
			);

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_PORT);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBDI_PORT_KEY, UINT32);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_PORT_METHOD_TABLE);

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_USBD);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_USBD_CONFIG);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_USBD_IMPLEMENTATION);

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_TT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_USBDTT_CONFIG);

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_USBDSUPERSPEED_CONFIG);

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_DEVICE_ARRIVAL_INFO);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_DEVICE_DEPARTURE_INFO);
__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_NORMALIZED_ROUTE_STRING);

__TMS_TYPE_DEF_ARG	(USBPUMP_USBDI_POWER_STATE, USBPUMP_POWER_STATE);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBDI_POWER_ACTION, USBPUMP_POWER_ACTION);


/****************************************************************************\
|
|	Function types that depend on the above types....
|
\****************************************************************************/

__TMS_FNTYPE_DEF	(USBPUMP_URB_DONE_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_USBDI_PORT *,
		__TMS_USBPUMP_URB *,
		__TMS_VOID *		/* pDoneInfo */,
		__TMS_ARG_USTAT		/* the status */
		));


/****************************************************************************\
|
|	USBD initialization function type definition
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_USBD_INIT_FN,	/* 4286 */
__TMS_USBPUMP_OBJECT_HEADER *,
	(
	__TMS_USBPUMP_OBJECT_HEADER * /*pParent*/,
	__TMS_CONST __TMS_VOID * /*pConfigParam*/,
	__TMS_CONST __TMS_VOID * /*pImpementationParam*/
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_USBDTT_INIT_FN,	/* 4286 */
__TMS_BOOL,
	(
	__TMS_USBPUMP_OBJECT_HEADER * /*pParent*/,
	__TMS_USBPUMP_OBJECT_HEADER * /*pUsbd*/,
	__TMS_CONST __TMS_VOID * /*pConfigParam*/
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_USBDWIRELESS_INIT_FN,	/* 4286 */
__TMS_BOOL,
	(
	__TMS_USBPUMP_OBJECT_HEADER * /*pParent*/,
	__TMS_USBPUMP_OBJECT_HEADER * /*pUsbd*/,
	__TMS_CONST __TMS_VOID * /*pConfigParam*/
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_USBDSUPERSPEED_INIT_FN,
__TMS_BOOL,
	(
	__TMS_USBPUMP_OBJECT_HEADER * /*pParent*/,
	__TMS_USBPUMP_OBJECT_HEADER * /*pUsbd*/,
	__TMS_CONST __TMS_VOID * /*pConfigParam*/
	));

/****************************************************************************\
|
|	USBD device arrival function type definition
|
\****************************************************************************/

/*
|| USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT value are defined in the
|| usbpump_usbdi_device_arrival.h.
*/
__TMS_TYPE_DEF_ARG(USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT, UINT32);

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_DEVICE_ARRIVAL_LISTENER_FN,
__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT,
	(
	__TMS_CONST __TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO * /*pArrivalInfo*/
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_DEVICE_DEPARTURE_LISTENER_FN,
__TMS_VOID,
	(
	__TMS_CONST __TMS_USBPUMP_USBDI_DEVICE_DEPARTURE_INFO * /*pDepartureInfo*/
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_STOP_COMPLETION_LISTENER_FN,
__TMS_VOID,
	(
	__TMS_VOID *			/* pListenerContext */,
	__TMS_USBPUMP_USBDI_USBD *	/* pUsbd */
	));

/****************************************************************************\
|
|	Not really "types", but helpful to have in a central place
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_NAME(Name)	Name ".usbdi.mcci.com"

/****************************************************************************\
|
|	Uncloaked defs
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_NAME(Name)	__TMS_USBPUMP_USBDI_NAME(Name)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_types.h ****/
#endif /* _USBPUMP_USBDI_TYPES_H_ */
