/* usbpump_usbdi_port.h	Mon Nov 24 2014 14:26:31 tmm */

/*

Module:  usbpump_usbdi_port.h

Function:
	Definitions for USBPUMP_USBDI_PORT and related objects.

Version:
	V3.13b	Mon Nov 24 2014 14:26:31 tmm	Edit level 4

Copyright notice:
	This file copyright (C) 2005, 2007, 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Sat Aug 13 2005 06:23:57  tmm
	Module created.

   1.97j  Thu Sep 20 2007 09:12:23  djt
	3205: Fixed documentation for Port.pMethods.

   3.13b  Mon Nov 24 2014 14:26:31  tmm
	Fix documentation -- incorrect reference to USBPUMP_URB_DONE_FN.

*/

#ifndef _USBPUMP_USBDI_PORT_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_PORT_H_

# ifndef _USBPUMP_USBDI_TYPES_H_
#  include "usbpump_usbdi_types.h"
# endif

# ifndef _USBPUMPOBJECT_H_
#  include "usbpumpobject.h"
# endif

/****************************************************************************\
|
|	The USBDI_PORT object is used for submitting requests
|
\****************************************************************************/

/*

Type:	USBPUMP_USBDI_PORT

Function:
	Represents a port (attach point) of USBDI to a function driver.

Description:
	Each hub or pseudo-hub within USBDI manages a collection of
	objects that export a USBPUMP_USBDI_PORT view of themselves.
	This view is used by function drivers to send requests to their
	device, via the HCD.

Contents:
	USBPUMP_OBJECT_HEADER	ObjectHeader;
		The standard DataPump object header.

	USBPUMP_USBD Port.pUsbd;
		Pointer to the object that represents the USBD
		system to the code that implements the system.

	CONST USBPUMP_USBDI_PORT_METHOD_TABLE *Port.pMethods;
		The switch object that is used for direct I/O access
		into the port.  This switch contains:

		USBPUMP_USBDI_PORT_SUBMIT_REQUEST_FN *
		   Port.pMethods->pSubmitRequest;

			A pointer to the URB submission function.  This
			function has the prototype:

			VOID (*pSubmitRequest)(
				USBPUMP_USBDI_PORT *,
				USBPUMP_URB *,
				USBPUMP_URB_DONE_FN *pDoneFn,
				VOID *pDoneInfo
				);

		USBPUMP_USBDI_PORT_CANCEL_REQUEST_FN *
		   Port.pMethods->pCancelRequest;

			A pointer to the URB cancellation function.
			This function has the prototype:

			VOID (*pCancelRequest)(
				USBPUMP_USBDI_PORT *,
				USBPUMP_URB *
				);

			Note that if the URB is routed to a remote
			controller, the request might not be cancelled 
			right away, or might complete successfully before
			it can be cancelled.

Notes:
	The USBPUMP_USBDI_PORT object is a subset of the full object,
	which is maintained internally by USBDI.  The remaining content
	of the port may vary depending on the underlying connect
	technology (wired, wireless, network, etc.)

See Also:
	

*/


/*
|| in usbpump_usbdi_types.h:
|| __TMS_TYPE_DEF_UNION(USBPUMP_USBDI_PORT); 
*/
__TMS_FNTYPE_DEF(
USBPUMP_USBDI_PORT_SUBMIT_REQUEST_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_USBDI_PORT *,
	__TMS_USBPUMP_URB *,
	__TMS_USBPUMP_URB_DONE_FN *,
	__TMS_VOID *
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_PORT_CANCEL_REQUEST_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_USBDI_PORT *,
	__TMS_USBPUMP_URB *
	));


/* __TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_PORT_METHOD_TABLE); */
struct __TMS_STRUCTNAME(USBPUMP_USBDI_PORT_METHOD_TABLE)
	{
	__TMS_USBPUMP_USBDI_PORT_SUBMIT_REQUEST_FN	*pSubmitRequest;
	__TMS_USBPUMP_USBDI_PORT_CANCEL_REQUEST_FN	*pCancelRequest;
	};

#define	__TMS_USBPUMP_USBDI_PORT_METHOD_TABLE_INIT_V1(			\
	a_pSubmitRequest,						\
	a_pCancelRequest						\
	)								\
	{								\
	(a_pSubmitRequest),						\
	(a_pCancelRequest)						\
	}

#define	__TMS_USBPUMP_USBDI_PORT_METHOD_TABLE_SETUP_V1(			\
	a_pMethods,							\
	a_pSubmitRequest,						\
	a_pCancelRequest						\
	)								\
	{								\
	(a_pMethods)->pSubmitRequest = (a_pSubmitRequest);		\
	(a_pMethods)->pCancelRequest = (a_pCancelRequest);		\
	}

__TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_PORT_CONTENTS);
struct __TMS_STRUCTNAME(USBPUMP_USBDI_PORT_CONTENTS)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__STRUCT;
	__TMS_USBPUMP_USBDI_USBD		*pUsbd;
	__TMS_CONST __TMS_USBPUMP_USBDI_PORT_METHOD_TABLE	*pMethods;
	};

/*
|| Finally, the actual USBD port object
*/
union __TMS_UNIONNAME(USBPUMP_USBDI_PORT)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;
	__TMS_USBPUMP_USBDI_PORT_CONTENTS	Port;
	};

#define	__TMS_USBPUMP_USBDI_PORT_CONTENTS__STRUCT		\
	__TMS_USBPUMP_USBDI_PORT_CONTENTS	Port

#define	__TMS_USBPUMP_USBDI_PORT_CONTENTS__UNION		\
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;			\
	__TMS_USBPUMP_USBDI_PORT_CONTENTS	Port;		\
	__TMS_USBPUMP_USBDI_PORT		PortCast

/*
|| Object related things
*/

/* the USB port object tag */
#define	__TMS_USBPUMP_USBDI_PORT_TAG	__TMS_UHIL_MEMTAG('U', 'P', 'o', 'r')

/* the name generator */
#define	__TMS_USBPUMP_USBDI_PORT_NAME(x)	\
				x __TMS_USBPUMP_USBDI_NAME("port")

#define	__TMS_USBPUMP_USBDI_PORT_CLASS_NAME	\
				__TMS_USBPUMP_USBDI_NAME("port.class")

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_PORT_METHOD_TABLE_INIT_V1(			\
	a_pSubmitRequest,						\
	a_pCancelRequest						\
	)	\
	__TMS_USBPUMP_USBDI_PORT_METHOD_TABLE_INIT_V1(			\
	a_pSubmitRequest,						\
	a_pCancelRequest						\
	)
# define USBPUMP_USBDI_PORT_METHOD_TABLE_SETUP_V1(			\
	a_pMethods,							\
	a_pSubmitRequest,						\
	a_pCancelRequest						\
	)	\
	__TMS_USBPUMP_USBDI_PORT_METHOD_TABLE_SETUP_V1(			\
	a_pMethods,							\
	a_pSubmitRequest,						\
	a_pCancelRequest						\
	)
# define USBPUMP_USBDI_PORT_CONTENTS__STRUCT	\
   __TMS_USBPUMP_USBDI_PORT_CONTENTS__STRUCT
# define USBPUMP_USBDI_PORT_CONTENTS__UNION	\
   __TMS_USBPUMP_USBDI_PORT_CONTENTS__UNION
# define USBPUMP_USBDI_PORT_TAG	\
   __TMS_USBPUMP_USBDI_PORT_TAG
# define USBPUMP_USBDI_PORT_NAME(x)	\
   __TMS_USBPUMP_USBDI_PORT_NAME(x)
# define USBPUMP_USBDI_PORT_CLASS_NAME	\
   __TMS_USBPUMP_USBDI_PORT_CLASS_NAME
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_port.h ****/
#endif /* _USBPUMP_USBDI_PORT_H_ */
