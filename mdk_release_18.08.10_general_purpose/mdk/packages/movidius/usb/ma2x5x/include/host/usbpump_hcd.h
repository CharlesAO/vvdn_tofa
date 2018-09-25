/* usbpump_hcd.h	Mon Nov 28 2011 10:47:19 chwon */

/*

Module:  usbpump_hcd.h

Function:
	Home for USBPUMP_HCD and associated types.

Version:
	V3.01f	Mon Nov 28 2011 10:47:19 chwon	Edit level 8

Copyright notice:
	This file copyright (C) 2005, 2007-2011 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Wed Jun  8 2005 17:21:10  tmm
	Module created.

   1.91h  Thu Sep  1 2005 05:37:29  tmm
	Clean up commentary.  Add the CompletionQueue.  Move API functions
	that apply to all HCDs here (at least provisionally).

   1.97j  Fri Aug 10 2007 09:47:08  djt
	3265: Added HCD specific TT parameters.
	Fixed company name.
	bMaxPeriodicStartSplitsPerMicroFrame

   1.97k  Fri Feb 01 2008 15:01:52  djt
	6900: Change bMaxPeriodicStartSplitsPerMicroFrame to UINT8.
	Changed fEhciStartSplitRestriction to fEhciStartSplitOk.
	Added documentation.
	Added USBPUMP_HCD_SPLIT_MAX_BYTES.

   2.01a  Fri Nov 13 2009 21:16:46  chwon
	9172: Replace RootHubDevice to ppRootHubDevices for supporting
	root hub for full, high and super speed. Move RootHubStatusPipe to the
	USBPUMP_HCDKIT_ROOTHUB.

   3.01c  Tue May 04 2010 17:45:15  ssyang
	9816: removed unused bMaxPeriodicStartSplitsPerMicroFrame.

   3.01f  Mon Nov 28 2011 10:47:19  chwon
	14489: add fBandwidthAllocationCapable parameter in the USBPUMP_HCD.

*/

#ifndef _USBPUMP_HCD_H_		/* prevent multiple includes */
#define _USBPUMP_HCD_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBPUMP_HCD_NAME_H_
# include "usbpump_hcd_name.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBPUMP_HCD_TYPES_H_
# include "usbpump_hcd_types.h"
#endif

#ifndef _USBPUMP_HCD_DEVICE_H_
# include "usbpump_hcd_device.h"
#endif

#ifndef _USBPUMP_HCD_INSWITCH_H_
# include "usbpump_hcd_inswitch.h"
#endif

#ifndef _USBPUMP_HCD_REQUEST_QUEUE_H_
# include "usbpump_hcd_request_queue.h"
#endif

/****************************************************************************\
|
|	An init function is useful
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
USBPUMP_HCD_INIT_FN,
	__TMS_USBPUMP_HCD *,
		(
		__TMS_USBPUMP_OBJECT_HEADER *	/*pParent*/,
		__TMS_CONST __TMS_VOID *	/*pInitParam*/
		));

/****************************************************************************\
|
|	USBPUMP_HCD
|
\****************************************************************************/

/*

Type:	USBPUMP_HCD

Index:	Macro:	USBPUMP_HCD_CONTENTS__UNION
	Macro:	USBPUMP_HCD_CONTENTS__STRUCT
	Type:	union __TMS_USBPUMP_HCD
	Constant:	USBPUMP_HCD_TAG

Function:
	The generic object that represents a USB host controller.

Description:
	A USBPUMP_HCD is the central context object for a given USB host 
	controller instance within the DataPump.

Contents:
	USBPUMP_OBJECT_HEADER ObjectHeader;
		The standard object header.  The tag is |USBPUMP_HCD_TAG|.
		The IOCTL parent is the pointer to the next object closer
		to the |UPLATFORM| -- depending on the design of the HCD,
		there may be no objects, or several objects, in series 
		between this HCD and the UPLATFORM.

		This may also be referenced as Hcd.ObjectHeader.

	USBPUMP_HCD_CONTENTS Hcd;
		The common definitions for all HCDs, defined as follows.

	UPLATFORM *Hcd.pPlatform;
		Pointer to the platform object, for convenience.

	USBPUMP_HCD_INSWITCH Hcd.InSwitch;
		The high speed APIs.  This is inteded always to be
		supplied by the common HCD logic, but is separated out
		for API clarity and easy separation of functions.

	USBPUMP_HCD_DEVICE **ppRootHubDevices;
		The array of HCD device pointer. The device pointer for the
		root hub for this HCD.  Generally speaking, pRootHubDevices[0]
		is pointer for the full speed root hub, pRootHubDevices[1] is
		for high speed root hub, and pRootHubDevices[2] is for super
		speed root hub.

Private Contents:
	BYTES DefaultMaxTransferSize;
		A configuration parameter, sets the max transfer size to
		be used by default for pipes on this HCD.

	USBPUMP_HCD_REQUEST_QUEUE CompletionQueue;
		A queue used by UsbPumpHcdRequest_Complete for deferring
		completion of requests.

	UINT fEhciStartSplitOk: 1;
		A flag indicating this HCD can handle a StartSplit and 
		CompleteSplit in H-frame 1.

	UINT fLateCompleteSplitsOk: 1;
		A flag indicating that this HCD can handle Late Complete Splits.
		A Late Complete Split is a Complete Split that is in the 
		mini-frame following the mini-frame the StartSplit is in.

	UINT fBandwidthAllocationCapable: 1;
		A flag indicating that the host controller can support hardware
		bandwidth allocation. If this flag is TRUE, USBD will skip
		internal bandwidth allocation.
	
Notes:
	The USBPUMP_HCD object is intended to be embedded in a larger
	device-hardware specific object, just as a UDEVICE is.  The
	embedding discipline is slightly different, because we are assuming
	ANSI compilers now, which was not the case when the DataPump
	was created.

	In order to define a hardware specific HCD object drived from
	the base HCD, please follow these rules.  (NB: the following
	rules are for the usual case where the HCD implementation is
	using the common HCD library (HcdLib) as 

	    1)	define your top level HCD object as a union, using the
		following schema:

			__TMS_TYPE_DEF_UNION(my_hcd_name);

		example:
			__TMS_TYPE_DEF_UNION(USBPUMP_HCD_ISP1362);

	    2)  define a "contents" structure for your HCD object, using
			__TMS_TYPE_DEF_STRUCT(my_hcd_name_CONTENTS);
		example:
			__TMS_TYPE_DEF_STRUCT(USBPUMP_HCD_ISP1362_CONTENTS);

	    3)  define the body of your HCD object:

			struct __TMS_STRUCTNAME(my_hcd_name_CONTENTS)
				{
				__TMS_USBPUMP_HCD_CONTENTS__STRUCT;

				// specific contents here
				};

	    4)	define the overall union for your HCD object

			#define __TMS_my_hcd_name_CONTENTS__UNION	\
				__TMS_USBPUMP_HCD_CONTENTS__UNION; 	\
				__TMS_my_hcd_name_CONTENTS

			union __TMS_UNIONNAME(my_hcd_name)
				{
				__TMS_my_hcd_name_CONTENTS__UNION;
				};

		Example:

			#define __TMS_USBPUMP_HCD_ISP1362_CONTENTS__UNION \
				__TMS_USBPUMP_HCD_CONTENTS__UNION;	\
				__TMS_USBPUMP_HCD_ISP1362_CONTENTS	Isp1362

			union __TMS_UNIONNAME(USBPUMP_HCD_ISP1362)
				{
				__TMS_USBPUMP_HCD_ISP3162_CONTENTS__UNION;
				};

	    5)	Then refer to your contents using |pHcd->Isp1362.{whatever}|

	If you follow the above dicipline, you will further be able to
	create types that are based on the USBPUMP_HCD_my_hcd_name
	structure, and which allow code (especially in macros) to refer to
	fields by name without worrying too much about the type, and without
	using casts.

	Of course, you might want to follow type-cloaking rules.

	Full HCD drivers (as opposed to HCD drivers based on the HcdKit)
	must use the macros __TMS_USBPUMP_HCD_CONTENTS__UNION and
	__TMS_USBPUMP_HCD_CONTENTS__STRUCT.

See Also:
	USBPUMP_HCDKIT_HCD
	USBPUMP_HCD_REQUEST_SWITCH

*/

/* __TMS_TYPE_DEF_UNION(USBPUMP_HCD); */

__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_HCD_CONTENTS)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__STRUCT;

	__TMS_UPLATFORM *		pPlatform;

	/*
	|| Data plane and high-performance accesses come via the
	|| HCD switch.  
	*/
	__TMS_USBPUMP_HCD_INSWITCH	InSwitch;

	/*
	|| A representation of the root hub device (including the default pipe).
	*/
	__TMS_BYTES			nRootHubDevices;
	__TMS_USBPUMP_HCD_DEVICE **	ppRootHubDevices;

	/*
	|| The default max transfer size for this HCD
	*/
	__TMS_BYTES			DefaultMaxTransferSize;

	/*
	|| We have a queue for requests that we'll complete soon.
	*/
	__TMS_USBPUMP_HCD_REQUEST_QUEUE	CompletionQueue;

	/* 
	|| These are the HCD specific TT parameters.
	*/
	__TMS_UINT			fEhciStartSplitOk: 1; 
	__TMS_UINT			fLateCompleteSplitsOk: 1;

	/*
	|| Host controller specific parameters
	*/
	__TMS_UINT			fBandwidthAllocationCapable: 1;
	};

/* prefix your derived object this way */
#define	__TMS_USBPUMP_HCD_CONTENTS__STRUCT				\
	__TMS_USBPUMP_HCD_CONTENTS		Hcd

/* a macro that you can embed in your own union */
#define __TMS_USBPUMP_HCD_CONTENTS__UNION				\
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;				\
	__TMS_USBPUMP_HCD_CONTENTS		Hcd;			\
	__TMS_USBPUMP_HCD			HcdCast

union __TMS_UNIONNAME(USBPUMP_HCD)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;
	__TMS_USBPUMP_HCD_CONTENTS		Hcd;
	};

/*
|| Object related things
*/

/* the USB device object tag */
#define	__TMS_USBPUMP_HCD_TAG		__TMS_UHIL_MEMTAG('U', 'H', 'c', 'd')

/* the HCD class name */
#define	__TMS_USBPUMP_HCD_CLASS_NAME	"hcd.class.mcci.com"

/*
|| TT related things.
*/

/* This is the maximum split transaction payload per micro frame. */
#define __TMS_USBPUMP_HCD_SPLIT_MAX_BYTES	188

/* Validate root hub device */
#define __TMS_USBPUMP_HCD_IS_ROOT_HUB_DEVICE(pHcd, pHcdDevice)	\
	UsbPumpHcdI_IsRootHubDevice(pHcd, pHcdDevice)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL
UsbPumpHcdRequest_Complete(
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */,
	__TMS_USBPUMP_HCD_REQUEST **	/* pQueueHead */, 
	__TMS_USTAT 			/* Status */
	);

__TMS_USBPUMP_HCD_REQUEST_CANCEL_FN *
UsbPumpHcdRequest_SetCancelRoutine(
	__TMS_USBPUMP_HCD_REQUEST	      *	/* pRequest */,
	__TMS_USBPUMP_HCD_REQUEST_CANCEL_FN   *	/* pCancelFn */,
	__TMS_VOID			      *	/* pCancelInfo */
	);

__TMS_USBPUMP_HCD_PIPE *
UsbPumpHcdI_GetRootHubDeviceStatusPipe(
	__TMS_USBPUMP_HCD *		/* pHcd */,
	__TMS_USBPUMP_HCD_DEVICE *	/* pRootHubDevice */
	);

__TMS_BOOL
UsbPumpHcdI_IsRootHubDevice(
	__TMS_USBPUMP_HCD *		/* pHcd */,
	__TMS_USBPUMP_HCD_DEVICE *	/* pHcdDevice */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define my_hcd_name_CONTENTS__UNION	\
   __TMS_my_hcd_name_CONTENTS__UNION
# define USBPUMP_HCD_ISP1362_CONTENTS__UNION	\
   __TMS_USBPUMP_HCD_ISP1362_CONTENTS__UNION
# define USBPUMP_HCD_CONTENTS__STRUCT	\
   __TMS_USBPUMP_HCD_CONTENTS__STRUCT
# define USBPUMP_HCD_CONTENTS__UNION	\
   __TMS_USBPUMP_HCD_CONTENTS__UNION
# define USBPUMP_HCD_TAG	\
   __TMS_USBPUMP_HCD_TAG
# define USBPUMP_HCD_CLASS_NAME	\
   __TMS_USBPUMP_HCD_CLASS_NAME
# define USBPUMP_HCD_SPLIT_MAX_BYTES	\
   __TMS_USBPUMP_HCD_SPLIT_MAX_BYTES
# define USBPUMP_HCD_IS_ROOT_HUB_DEVICE(pHcd, pHcdDevice)	\
   __TMS_USBPUMP_HCD_IS_ROOT_HUB_DEVICE(pHcd, pHcdDevice)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_hcd.h ****/
#endif /* _USBPUMP_HCD_H_ */
