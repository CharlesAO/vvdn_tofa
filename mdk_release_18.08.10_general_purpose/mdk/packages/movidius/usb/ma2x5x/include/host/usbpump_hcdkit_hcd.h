/* usbpump_hcdkit_hcd.h	Wed Dec 02 2015 14:28:03 chwon */

/*

Module:  usbpump_hcdkit_hcd.h

Function:
	Definitions for USBPUMP_HCDKIT_HCD

Version:
	V3.15d	Wed Dec 02 2015 14:28:03 chwon	Edit level 15

Copyright notice:
	This file copyright (C) 2005-2006, 2009-2011, 2013-2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Fri Jun 17 2005 13:06:31  tmm
	Module created.

   1.91h  2005-08-31  tmm
	Remove symbolic types for USBPUMP_HCDKIT_COMPLETEREQUEST_FN and
	USBPUMP_HCDKIT_HCDINIT_FN, as they are only used once and are not
	currently part of any API.

   1.91h  Mon Sep  5 2005 09:19:32  tmm
	Add pClientObject and pClientContext for supporting function opens.

   1.93e  Fri Jan 06 2006 16:04:49  chwon
	Added pRootPortStatusChange

    1.93e  Wed Mar 01 2006 16:10:22  ssyang
	Add pRootHubIntRequest, which is used to keep roothub interrupt request

   2.01a  Fri Nov 13 2009 21:17:01  chwon
	9172: Add USBPUMP_HCDKIT_ROOTHUB structure and root hub will be created
	for the each supported speed.
	9246: Use USBPUMP_TIMER_TIMEOUT instead USHORT.

   2.01a  Tue Dec 01 2009 16:06:10  chwon
	9172: Remove pClientObject and pClientContext and add OpenCount.
	Add USBPUMP_HCDKIT_ROOTHUBPORT structure for root hub port.

   3.01c  Tue Sep 21 2010 11:31:13  chwon
	10988: add HcdReserved2 in the USBPUMP_HCDKIT_ROOTHUBPORT

   3.01d  Wed Oct 13 2010 18:50:01  chwon
	11323: change bit-field and add HCD reserved flags in the ROOTHUBPORT.

   3.01d  Thu Feb 10 2011 13:45:45  chwon
	12655: add fDeviceNoneRemovable flag in the root hub port.

   3.01f  Sun Jun 12 2011 15:28:15  ssyang
	13589: add pRootPort, pWakeAlertFn and pWakeAlertContext into
	USBPUMP_HCDKIT_ROOTHUB.

   3.11c  Fri Mar 15 2013 09:40:27  chwon
	16861: Add pOtg in the USBPUMP_HCDKIT_HCD.

   3.11c  Thu Mar 14 2013 04:14:19  ssyang
	16893: add fSupportRootHubBOS and RootHubContainerID fields into
	USBPUMP_HCDKIT_HCD_CONTENTS..
	
   3.11c  Tue Jun 04 2013 13:04:49  chwon
	17320: Add USBPUMP_HCDKIT_ROOTHUBPORT::pUsbdiPort.

   3.13b  Fri Aug 15 2014 13:38:14  chwon
	18461: Add hUhilAux and pUhilAuxInCall in the USBPUMP_HCDKIT_HCD.

   3.15b  Fri Apr 03 2015 20:46:40  tmm
	19121: Define USBPUMP_HCDKIT_ROOTHUBPORT_STATE__MAX.

   3.15d  Wed Dec 02 2015 14:28:03  chwon
	19697: Add fUserReverved in the USBPUMP_HCDKIT_ROOTHUBPORT.

*/

#ifndef _USBPUMP_HCDKIT_HCD_H_		/* prevent multiple includes */
#define _USBPUMP_HCDKIT_HCD_H_

#ifndef _USBPUMP_HCD_H_
# include "usbpump_hcd.h"
#endif

#ifndef _USBPUMP_HCDKIT_TYPES_H_
# include "usbpump_hcdkit_types.h"
#endif

#ifndef _USBPUMP_HCDKIT_SWITCH_H_
# include "usbpump_hcdkit_switch.h"
#endif

#ifndef _USBPUMP_HCD_REQUEST_H_
# include "usbpump_hcd_request.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

#ifndef _USBPUMP_HCD_IOCTLS_H_
# include "usbpump_hcd_ioctls.h"
#endif

#ifndef _USBUUID_H_
# include "usbuuid.h"
#endif

/****************************************************************************\
|
|	Definitions for HCDKIT-based drivers (which is the usual case)
|
\****************************************************************************/

/*

Type:	USBPUMP_HCDKIT_HUBPORT_STATUS

Function:
	Represents the status and change indications for a single hub or port.

Description:
	USB Hubs and ports share a common status reporting structure: a 16=
	bit status word, followed by a 16 bit change word.

Contents:
	UINT16 wStatus;
		This contains a status mask, in native byte order.  The bits
		are as defined in "usbhub20.h", USB_Hub_HUB_STATUS_wStatus_... or
		USB_Hub_PORT_STATUS_wStatus_... as appropriate.

	UINT16 wChange;
		This contains a change mask.  Same bit definitions as for
		wStatus, but a set bit indicates that a status change has
		occurred.  Changes are reset using CLEAR_FEATURE.

Notes:
	This structure is intended to be packed on 2-byte boundaries, so
	that an array of structures per-port may be formed conveniently for
	use by HcdKit.

See Also:
	usbhub20.h

*/

/* __TMS_TYPE_DEF_STRUCT(USBPUMP_HCDKIT_HUBPORT_STATUS); */

#include "usbpump_pushpack2.h"

__TMS_LIBPORT_PACKED2_DEFINITION(
struct __TMS_STRUCTNAME(USBPUMP_HCDKIT_HUBPORT_STATUS)
)
	{
	__TMS_UINT16	wStatus;
	__TMS_UINT16	wChange;
	} __TMS_LIBPORT_PACKED2_SUFFIX;

#include "usbpump_poppack.h"


/*

Type:	USBPUMP_HCDKIT_ROOTHUBPORT

Function:
	Represents the root hub port information.

Description:
	HcdKit virtual root hub logic uses this structure to control root hub
	port.

Contents:
	USBPUMP_HCDKIT_ROOTHUB *pRootHub;
		The pointer for the root hub.

	USBPUMP_HCDKIT_HUBPORT_STATUS PortStatus;
		Root hub port status.

	UINT PortNumber: 8;
		Port number for this root hub port.

	UINT bSpeed: 4;
		Current port speed. It is same as connected device speed.

	UINT State: 8;
		Port state for this root hub port.  This will be used by HCD to
		maintain state of the root hub port.

	UINT fRunTimer: 1;
		TRUE if timer in running.

	UINT fDummyPort: 1;
		TRUE if root hub port is dummy port.

	UINT fDeviceNoneRemovable: 1;
		TRUE if root hub port is device none removable port.

	UINT fUserReverved: 1;
		Reverved flag for user. HCD doesn't change this flag.

	UINT fHcdReserved[1-8]: 1;
		Reserved flags for the HCD usage.

	USBPUMP_TIMER Timer;
		Timer object for this root hub port.

	USBPUMP_USBDI_PORT *pUsbdiPort;
		The USBDI port pointer matched with this root hub port.
		This can be used to send IOCTL to the USBDI port object.

	ADDRBITS_PTR_UNION HcdReserved;
	ADDRBITS_PTR_UNION HcdReserved2;
		Reserved for the HCD usage.

See Also:
	USBPUMP_HCDKIT_HCD

*/

/* __TMS_TYPE_DEF_STRUCT(USBPUMP_HCDKIT_ROOTHUBPORT); */
struct __TMS_STRUCTNAME	(USBPUMP_HCDKIT_ROOTHUBPORT)
	{
	__TMS_USBPUMP_HCDKIT_ROOTHUB *		pRootHub;
	__TMS_USBPUMP_HCDKIT_HUBPORT_STATUS	PortStatus;
	__TMS_UINT				PortNumber: 8;
	__TMS_UINT				State: 8;
	__TMS_UINT				bSpeed: 4;
	__TMS_UINT				fRunTimer: 1;
	__TMS_UINT				fDummyPort: 1;
	__TMS_UINT				fDeviceNoneRemovable: 1;
	__TMS_UINT				fUserReverved: 1;

	__TMS_UINT				fHcdReverved1: 1;
	__TMS_UINT				fHcdReverved2: 1;
	__TMS_UINT				fHcdReverved3: 1;
	__TMS_UINT				fHcdReverved4: 1;
	__TMS_UINT				fHcdReverved5: 1;
	__TMS_UINT				fHcdReverved6: 1;
	__TMS_UINT				fHcdReverved7: 1;
	__TMS_UINT				fHcdReverved8: 1;
	__TMS_USBPUMP_TIMER			Timer;
	__TMS_USBPUMP_USBDI_PORT *		pUsbdiPort;
	__TMS_ADDRBITS_PTR_UNION		HcdReserved;
	__TMS_ADDRBITS_PTR_UNION		HcdReserved2;
	};

/* Root hub port State */
#define	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_DISCONNECTED	0
#define	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_DISABLED		1
#define	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_RESETTING	2
#define	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_ENABLED		3
#define	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_SUSPENDED	4
#define	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_RESUMING		5
#define	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_CONNECTED	6
#define	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_POWEREDOFF	7
#define	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_TESTING		8
#define	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE__MAX		9	/* following convention, this is always "last+1". */ 


/*

Type:	USBPUMP_HCDKIT_ROOTHUB

Function:
	Represents the root hub device information.

Description:
	HcdKit virtual root hub logic uses this structure to control root hub
	device.

Contents:
	USBPUMP_HCD_DEVICE RootHubDevice;
		The device representation (and the default pipe) for the
		root hub for this HCD.

	USBPUMP_HCD_PIPE RootHubStatusPipe;
		The interrupt pipe for the root hub for this HCD.

	USBPUMP_HCDKIT_HUBPORT_STATUS RootHubStatus;
		The root hub status, expressed in the same form returned by the
		get-hub-status operation:  one word of status, one word of
		change.  Note, however, that the words are stored in local
		native order.

	UINT8 *pRootPortStatusChange;
		Base of array of port status change.

	USBPUMP_HCD_REQUEST *pRootHubRequest;
		The pointer of HCD request to get notification from root hub.

	USBPUMP_HCDKIT_ROOTHUBPORT *pRootHubPorts;
		Base of array of root hub port entries, indexed by root port
		number.

See Also:
	USBPUMP_HCDKIT_HCD

*/

/* __TMS_TYPE_DEF_STRUCT(USBPUMP_HCDKIT_ROOTHUB); */
struct __TMS_STRUCTNAME	(USBPUMP_HCDKIT_ROOTHUB)
	{
	/*
	|| A representation of the root hub (including the default pipe).
	*/
	__TMS_USBPUMP_HCD_DEVICE		RootHubDevice;
	__TMS_USBPUMP_HCD_PIPE			RootHubStatusPipe;

	/*
	|| HcdKit HCDs have standard root-hub support fields
	*/
	__TMS_USBPUMP_HCDKIT_HUBPORT_STATUS	RootHubStatus;
	__TMS_UINT8 *				pRootPortStatusChange;
	__TMS_USBPUMP_HCD_REQUEST *		pRootHubRequest;
	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT *	pRootHubPorts;
	
	/*
	|| To support rootport emulation.
	*/
	__TMS_USBPUMP_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ALERT_FN
						*pWakeAlertFn;
	__TMS_VOID 				*pWakeAlertContext;
	};


/*

Type:	USBPUMP_HCDKIT_HCD

Index:	Type:	union __TMS_USBPUMP_HCDKIT_HCD
	Macro:	USBPUMP_HCDKIT_HCD_CONTENTS__UNION
	Macro:	USBPUMP_HCDKIT_HCD_CONTENTS__STRUCT

Function:
	Represents an HcdKit based HCD instance object to HcdKit modules

Description:
	HcdKit common logic uses objects of type USBPUMP_HCDKIT_HCD to
	represent HCD instances.  These objects are unions, which combine
	a hierarchy of views.  In addition, the objects are always a proper subset
	of the total data structure; the terminating layer of the HCD
	implementation normaly requires additional data of its own.

Contents:
	USBPUMP_HCD HcdKit.HcdCast;
		A view of the object that is type-compatible with functions
		that require USBPUMP_HCD pointers.  This allows you to make
		the conversion without doing a cast.

	USBPUMP_OBJECT_HEADER ObjectHeader.*;
		The fields defined in the standard object header.

	USBPUMP_HCD_CONTENTS Hcd;
		The fields defined in the standard HCD.

	UINT32 HcdKit.OpenCount;
		Function open count for use by HcdKit.

	USBPUMP_HCDKIT_SWITCH HcdKit.HwSwitch;
		The methods provided by the hardware-specific code for use
		by HcdKit.

	UINT32 HcdKit.Status;
		Hardware status for use by HcdKit.  This is a bit-map.

		If USBPUMP_HCDKIT_STATUS_HW_UP is set, then the hardware is
		operational, and requests should be passed to the next
		hardware level for processing.  If reset, then the hardware is
		not operational, and all normal USB requests into the HCD from
		above will be rejected without being passed to the hardware-
		specific code.

	USBPUMP_TIMER_TIMEOUT HcdKit.DefaultTimeout
		Default HCD request timeout for use by HcdKit.

	BYTES nRootPorts;
		Base of array of status entries, indexed by root port number.
		nRootPorts gives the number of root ports.

	USBPUMP_HCDKIT_ROOTHUB *HcdKit.pRootHubs;
		The array of root hub device pointer.  The root hub device
		pointer for the root hub device for this HCD.

	The followings are reqiured for OTG function support. This information
	will be initialized by UsbPumpHcdKit_InitializeOtg_V1(). If HCD need to
	support OTG functionality, HCD initialization routine should call
	UsbPumpHcdKit_InitializeOtg_V1().

	USBPUMP_HCDKIT_OTG *pOtg;
		Pointer of USBPUMP_HCDKIT_OTG structure. This structure will be
		created by UsbPumpHcdKit_InitializeOtg_V1().  It saves OTG
		related information including USBPUMP_HCDKIT_SWITCH_OTG.

Notes:
	None.

See Also:
	USBPUMP_HCD
	USBPUMP_HCDKIT_SWITCH
	USBPUMP_HCDKIT_HUBPORT_STATUS

*/

/* hcdkit maps from the full-driver API onto the common DataPump HCD framework */
/* __TMS_TYPE_DEF_UNION(USBPUMP_HCDKIT_HCD); */
__TMS_TYPE_DEF_STRUCT(USBPUMP_HCDKIT_HCD_CONTENTS);

struct __TMS_STRUCTNAME(USBPUMP_HCDKIT_HCD_CONTENTS)
	{
	__TMS_USBPUMP_HCD_CONTENTS__STRUCT;

	/*
	|| We need to have the client open count.
	*/
	__TMS_UINT			OpenCount;

	/*
	|| The lower switch is only to be used internally.  It gives
	|| high-performance APIs and an IOCTL API.
	*/
	__TMS_USBPUMP_HCDKIT_SWITCH	HwSwitch;

	/*
	|| HcdKit HCDs have status that we can consider
	*/
	__TMS_UINT32			Status;

	/*
	|| HcdKit HCDs may have a default request timeout.
	*/
	__TMS_USBPUMP_TIMER_TIMEOUT	DefaultTimeout;

	/*
	|| HcdKit HCDs have standard root-hub support fields
	*/
	__TMS_BYTES			nRootPorts;
	__TMS_BYTES			PortStatusBytes;
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	pRootHubs;

	/*
	|| HcdKit HCDs have optional OTG support information
	*/
	__TMS_USBPUMP_HCDKIT_OTG *	pOtg;

	/*
	|| HcdKit HCDs can support container ID for roothub
	*/
	__TMS_USBUUID_WIRE		RootHubContainerID;

	/*
	|| UHILAUX
	*/
	__TMS_USBPUMP_SESSION_HANDLE	hUhilAux;
	__TMS_USBPUMP_UHILAUX_INCALL *	pUhilAuxInCall;
	};

/**** the bits in the HCDKIT status dword ****/
#define __TMS_USBPUMP_HCDKIT_HCD_STATUS_HW_UP		(1u << 0)
#define __TMS_USBPUMP_HCDKIT_HCD_STATUS_ROOTHUB_BOS	(1u << 1)

#define __TMS_USBPUMP_HCDKIT_HCD_SUPPORT_ROOTHUB_BOS(pHcdKit)	\
	((pHcdKit)->HcdKit.Status & __TMS_USBPUMP_HCDKIT_HCD_STATUS_ROOTHUB_BOS)

/* use this macro in structures derived from HCDKIT */
#define	__TMS_USBPUMP_HCDKIT_HCD_CONTENTS__STRUCT	\
	__TMS_USBPUMP_HCDKIT_HCD_CONTENTS HcdKit

/* a macro that you shoule embed in the definition of the HCD drivers' union */
#define	__TMS_USBPUMP_HCDKIT_HCD_CONTENTS__UNION			\
	__TMS_USBPUMP_HCD_CONTENTS__UNION;				\
	__TMS_USBPUMP_HCDKIT_HCD_CONTENTS	HcdKit;			\
	__TMS_USBPUMP_HCDKIT_HCD		HcdKitCast		\

union __TMS_UNIONNAME(USBPUMP_HCDKIT_HCD)
	{
	__TMS_USBPUMP_HCD_CONTENTS__UNION;
	__TMS_USBPUMP_HCDKIT_HCD_CONTENTS	HcdKit;
	};


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_HCDKIT_ROOTHUBPORT_STATE_DISCONNECTED	\
   __TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_DISCONNECTED
# define USBPUMP_HCDKIT_ROOTHUBPORT_STATE_DISABLED	\
   __TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_DISABLED
# define USBPUMP_HCDKIT_ROOTHUBPORT_STATE_RESETTING	\
   __TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_RESETTING
# define USBPUMP_HCDKIT_ROOTHUBPORT_STATE_ENABLED	\
   __TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_ENABLED
# define USBPUMP_HCDKIT_ROOTHUBPORT_STATE_SUSPENDED	\
   __TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_SUSPENDED
# define USBPUMP_HCDKIT_ROOTHUBPORT_STATE_RESUMING	\
   __TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_RESUMING
# define USBPUMP_HCDKIT_ROOTHUBPORT_STATE_CONNECTED	\
   __TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_CONNECTED
# define USBPUMP_HCDKIT_ROOTHUBPORT_STATE_POWEREDOFF	\
   __TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_POWEREDOFF
# define USBPUMP_HCDKIT_ROOTHUBPORT_STATE_TESTING	\
   __TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE_TESTING
# define USBPUMP_HCDKIT_ROOTHUBPORT_STATE__MAX	\
   __TMS_USBPUMP_HCDKIT_ROOTHUBPORT_STATE__MAX
# define USBPUMP_HCDKIT_HCD_STATUS_HW_UP	\
   __TMS_USBPUMP_HCDKIT_HCD_STATUS_HW_UP
# define USBPUMP_HCDKIT_HCD_STATUS_ROOTHUB_BOS	\
   __TMS_USBPUMP_HCDKIT_HCD_STATUS_ROOTHUB_BOS
# define USBPUMP_HCDKIT_HCD_SUPPORT_ROOTHUB_BOS(pHcdKit)	\
   __TMS_USBPUMP_HCDKIT_HCD_SUPPORT_ROOTHUB_BOS(pHcdKit)
# define USBPUMP_HCDKIT_HCD_CONTENTS__STRUCT	\
   __TMS_USBPUMP_HCDKIT_HCD_CONTENTS__STRUCT
# define USBPUMP_HCDKIT_HCD_CONTENTS__UNION	\
   __TMS_USBPUMP_HCDKIT_HCD_CONTENTS__UNION
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_hcdkit_hcd.h ****/
#endif /* _USBPUMP_HCDKIT_HCD_H_ */
