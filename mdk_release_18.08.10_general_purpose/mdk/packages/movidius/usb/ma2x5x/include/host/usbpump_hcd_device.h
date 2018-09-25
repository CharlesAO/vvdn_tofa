/* usbpump_hcd_device.h	Sun Apr 05 2015 00:18:57 tmm */

/*

Module:  usbpump_hcd_device.h

Function:
	Home for USBPUMP_HCD_DEVICE

Version:
	V3.15b	Sun Apr 05 2015 00:18:57 tmm	Edit level 22

Copyright notice:
	This file copyright (C) 2005-2006, 2008-2010, 2012-2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2005

Revision history:
   1.91d  Tue May 31 2005 16:29:05  tmm
	Module created.

   1.93e  Mon Jan 30 2006 01:33:31  tmm
	Fix the device-setup macro.

   1.97k  Thu Aug 07 2008 12:37:15  djt
	UDP-6317: Added USBPUMP_HCD_DEVICE_HUB_TTPERPORT and associated macros.
	
   1.97k  Tue Dec 09 2008 09:40:38  djt
	6318: Remove one sentence from notes.
	Fix copyright and company name.

   2.01a  Mon Feb 23 2009 09:53:51  djt
	7416: Added USB 3.0 fields

   2.01a  Fri Apr 03 2009 09:47:48  djt
	7416: Fixed compiler errors.
	
   2.01a  Mon Jun 15 2009 16:56:05  tmm
	8254:  fix default max packet size for superspeed.

   2.01a  Thu Oct 08 2009 13:15:55  skpark
	8796: Added USBPUMP_HCD_DEVICE_CLEAR_ROUTE_STRING to clear uRouteString
	
   2.01a  Thu Oct 08 2009 14:15:57  skpark
	8796: Removed unused parameter

   2.01a  Tue Dec 01 2009 11:48:21  chwon
	9357: Fixed doc-xml error in the USBPUMP_HCD_DEVICE_SETUP_V2 document.

   2.01a  Tue Dec 15 2009 19:50:06  chwon
	9538, 9569: Added device capabilities in the USBPUMP_HCD_DEVICE

   3.01a  Mon Dec 21 2009 14:04:34  chwon
	9538, 9569: Change device capabilities structure name and field name.

   3.01a  Fri Jan 22 2010 10:24:21  chwon
	9783: Change bMaxBurstSize default value from 0 to 1.

   3.01a  Mon Feb  8 2010 19:52:44  tmm
	9893:  rename bMaxPStreams to wMaxStreamID and resize.

   3.01c  Mon Mar 22 2010 22:39:29  chwon
	10029: add bU1DevExitLat and wU2DevExitLat in the device capability.

   3.01f  Thu May 31 2012 15:45:42  chwon
	14821: add LPM BESL related variables in the device capability.

   3.11b  Mon Sep 24 2012 11:54:09  cccho
	15952: add fHubMultipleTtCapable in the device capability.

   3.11b  Thu Oct 04 2012 16:57:52  cccho
	15952: move fHubMultipleTtCapable flag to hub fsm.

   3.11d  Tue Sep 10 2013 11:44:31  chwon
	17686: Fixed typo in documentation.

   3.15b  Sun Apr 05 2015 00:18:57  tmm
	19019: documentation cleanup.

*/

#ifndef _USBPUMP_HCD_DEVICE_H_		/* prevent multiple includes */
#define _USBPUMP_HCD_DEVICE_H_

#ifndef _USBPUMP_HCD_TYPES_H_
# include "usbpump_hcd_types.h"
#endif

#ifndef _USBPUMP_HCD_PIPE_H_
# include "usbpump_hcd_pipe.h"
#endif

/*

Type:	USBPUMP_HCD_DEVICE

Function:
	Represents an attached USB device to a common HCD driver.

Description:
	Part of the representaion of a transfer target to the
	USBPUMP_HCD is the device address, transfer speed, and possibly
	(for full-speed devs on high-speed HCs) the hub address and hub 
	port address of the TT on the high-speed bus.

	Since this info is the same for every pipe on a device, we centralize
	it in the USBPUMP_HCD_DEVICE object.  

Contents:
	UINT32 Tag;
		The tag for this datastructure, 'UHdv' arranged in byte
		order so it dumps as "UHdv" in the debugger.  This is
		filled in at init-time.

	USBPUMP_HCD *pHcd;
		The owning HCD.

	UINT32 uRouteString;
		0:19 	Route String as defined in USB 3.0
		23 	fHub, set if this device is a hub
		24:31	bHubPortNumber, root hub port used to access this device

	UINT8 bSpeed;
		USBPUMP_DEVICE_SPEED_{LOW, FULL, HIGH, etc}, forced into
		a UINT8.

		This is filled in at enum time for the device.

	UINT8 bAddress;
		The device address on its bus.

		This is filled in at enum time.

	UINT8 bHubAddress;
		For full-speed on high-speed only, this is the address of the 
		device on its hub. If bit 7 is set, then there's one TT 
		per port.

		This is filled in at enum time.

	UNIT8 bHubPort;
		For full-speed on high-speed only, this is the port of the 
		device on its hub.

		This is filled in at enum time.

	UINT8 bSlotID;
		Slot ID as defined in XHCI. (Note that this is used even 
		for legacy speeds.)

	UINT8 bTTHubSlotID;
		For full-speed on high-speed hub only, this is the slot ID of 
		the upstream high-speed hub.

	UINT16 wMaxExitLatency;
		The Maximum Exit Latency in milliseconds, indicates worst case
		time it takes to wake up all links in the path of this device.
		(SuperSpeed only).

	USBPUMP_HCD_DEVICE_CAPABILITIES Cap;
		Device capabilities. It contains hub device capability, USB 2.0
		extension capability, OTG device capability, wireless device
		capability and super speed device capability.

	USBPUMP_HCD_PIPE DefaultPipe;
		The default pipe for this device.

Notes:
	This is the lowest level of representation of a device.  The
	parent USBD may define additional fields in an encapsulating block.

	At the level of the core HCD miniport, there is currently no need
	for linkage from the device to its pipes; the miniports are supposed
	to be stateless.  
	
Bugs:
	It really would be convenient to provide for a way to traverse the
	list of known pipes for this device.  This data structure doesn't
	allow that.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_DEVICE_CAPABILITIES_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_HCD_DEVICE_CAPABILITIES_CONTENTS)
	{
	/*
	|| Hub device capability
	*/
	__TMS_UINT	fHubMultipleTT: 1;	/* Hub support multiple TT */
	__TMS_UINT	nHubPorts: 5;		/* Number of downstream ports */
	__TMS_UINT	HubTtThinkTime: 2;
					/*
					|| TT Think Time: bit times of inter
					|| transaction gap on a full-/low-speed
					|| downstream bus.
					|| 00: TT require at most  8 FS bit times
					|| 01: TT require at most 16 FS bit times
					|| 10: TT require at most 24 FS bit times
					|| 11: TT require at most 32 FS bit times
					*/

	/*
	|| USB 2.0 extension capability
	*/
	__TMS_UINT	fLPM: 1;		/* support LPM */
	__TMS_UINT	fBESL: 1;		/* support BESL & Alternate
						|| HIRD definition
						*/
	__TMS_UINT	fBaselineBESL: 1;	/* Valid baseline BESL */
	__TMS_UINT	fDeepBESL: 1;		/* Valid deep BESL */
	__TMS_UINT	BaselineBESL: 4;	/* Baseline BESL value */
	__TMS_UINT	DeepBESL: 4;		/* Deep BESL value */

	/*
	|| OTG device capability
	*/
	__TMS_UINT	fHNP: 1;		/* support HNP */
	__TMS_UINT	fSRP: 1;		/* support SRP */

	/*
	|| Wireless device capability
	*/
	__TMS_UINT	fP2P_DRD: 1;		/* peer to peer DRD capable */
	__TMS_UINT	fSelfBeacon: 1;
	__TMS_UINT	fDirectBeacon: 1;
	__TMS_UINT	fNoBeacon: 1;

	/*
	|| Super speed device capability
	*/
	__TMS_UINT	fLTM: 1;		/* LTM capable */
	__TMS_UINT	bU1DevExitLat: 4;	/* U1 Device Exit Latency */
	__TMS_UINT	wU2DevExitLat: 11;	/* U2 Device Exit Latency */
	};

__TMS_TYPE_DEF_UNION	(USBPUMP_HCD_DEVICE_CAPABILITIES);
union __TMS_UNIONNAME	(USBPUMP_HCD_DEVICE_CAPABILITIES)
	{
	__TMS_USBPUMP_HCD_DEVICE_CAPABILITIES_CONTENTS	Contents;
	__TMS_UINT64					RawBits;
	};

__TMS_LIBPORT_C_ASSERT(sizeof(__TMS_USBPUMP_HCD_DEVICE_CAPABILITIES_CONTENTS) <= sizeof(__TMS_UINT64));

struct __TMS_STRUCTNAME(USBPUMP_HCD_DEVICE)
	{
	__TMS_UINT32	Tag;

	/*
	|| from client:
	*/
	__TMS_USBPUMP_HCD *pHcd;
	__TMS_UINT32	uRouteString;
	__TMS_UINT8	bAddress; 
	__TMS_UINT8	bSpeed;
	__TMS_UINT8	bHubAddress; /* bit 7 set indicates TTPerPort */
	__TMS_UINT8	bHubPort;
	__TMS_UINT8	bSlotID;
	__TMS_UINT8	bTTHubSlotID;
	__TMS_UINT16	wMaxExitLatency;

	__TMS_USBPUMP_HCD_DEVICE_CAPABILITIES Capabilities;

	__TMS_USBPUMP_HCD_PIPE DefaultPipe;
	};

/*
|| Tags and so forth
*/
#define	__TMS_USBPUMP_HCD_DEVICE_TAG	__TMS_UHIL_MEMTAG('U', 'H', 'd', 'v')

#define __TMS_USBPUMP_HCD_DEVICE_ROUTE_STRING_MASK  0x000fffff 

#define __TMS_USBPUMP_HCD_DEVICE_GET_ROUTE_STRING(pHcdDevice)		\
	((pHcdDevice)->uRouteString & 					\
	 __TMS_USBPUMP_HCD_DEVICE_ROUTE_STRING_MASK)

#define __TMS_USBPUMP_HCD_DEVICE_SET_ROUTE_STRING(			\
		pHcdDevice, uRouteStr)					\
    do	{								\
	(pHcdDevice)->uRouteString &=					\
		~__TMS_USBPUMP_HCD_DEVICE_ROUTE_STRING_MASK;		\
 	(pHcdDevice)->uRouteString |=					\
		((uRouteStr) & __TMS_USBPUMP_HCD_DEVICE_ROUTE_STRING_MASK); \
	} while (0)

#define __TMS_USBPUMP_HCD_DEVICE_CLEAR_ROUTE_STRING(pHcdDevice)		\
    do	{								\
	(pHcdDevice)->uRouteString = 0;					\
	} while (0)
	
#define __TMS_USBPUMP_HCD_DEVICE_IS_HUB(pHcdDevice)			\
	((pHcdDevice)->uRouteString & 0x00800000)

#define __TMS_USBPUMP_HCD_DEVICE_SET_HUB(pHcdDevice)			\
    do	{								\
 	(pHcdDevice)->uRouteString |= 0x00800000;			\
	} while (0)

#define __TMS_USBPUMP_HCD_DEVICE_ROOT_HUB_PORT_NUMBER_MASK  0xff000000 

#define __TMS_USBPUMP_HCD_DEVICE_GET_ROOT_HUB_PORT_NUMBER(pHcdDevice)	\
	((pHcdDevice)->uRouteString >> 24)

#define __TMS_USBPUMP_HCD_DEVICE_SET_ROOT_HUB_PORT_NUMBER(		\
		pHcdDevice, bRootHubPortNumber)				\
    do	{								\
	(pHcdDevice)->uRouteString &=					\
		~__TMS_USBPUMP_HCD_DEVICE_ROOT_HUB_PORT_NUMBER_MASK;	\
	(pHcdDevice)->uRouteString |= ((bRootHubPortNumber) << 24);	\
	} while (0)

#define __TMS_USBPUMP_HCD_DEVICE_GET_HUB_ADDRESS(pHcdDevice)		\
	((pHcdDevice)->bHubAddress & 0x7f)

#define __TMS_USBPUMP_HCD_DEVICE_HUB_TTPERPORT 0x80

#define __TMS_USBPUMP_HCD_DEVICE_SET_HUB_TTPERPORT(pHcdDevice)		\
    do	{								\
        (pHcdDevice)->bHubAddress |= __TMS_USBPUMP_HCD_DEVICE_HUB_TTPERPORT; \
        } while (0)

#define __TMS_USBPUMP_HCD_DEVICE_IS_HUB_TTPERPORT(pHcdDevice)		\
	((pHcdDevice)->bHubAddress & __TMS_USBPUMP_HCD_DEVICE_HUB_TTPERPORT)

#define __TMS_USBPUMP_HCD_DEVICE_IS_ON_TT(pHcdDevice)			\
	((pHcdDevice)->bHubAddress != 0)


/*

Name:	USBPUMP_HCD_DEVICE_SETUP_V2()

Index:	Name:	USBPUMP_HCD_DEVICE_SETUP_V2()
	Name:	USBPUMP_HCD_DEVICE_SETUP_V1()

Function:
	Initialize USBPUMP_HCD_DEVICE structure

Definition:
	VOID USBPUMP_HCD_DEVICE_SETUP_V2(	
		USBPUMP_HCD_DEVICE *pHcdDevice,
		USBPUMP_HCD	*pHcd,
		UINT32		uRouteString,
		UINT8		bAddress,
		USBPUMP_DEVICE_SPEED bSpeed,
		UINT8		bHubAddress,
		UINT8		bHubPort,
		UINT8		bSlotID,
		UINT8		bTTHubSlotID,
		UINT16		wMaxExitLatency
		);

	VOID USBPUMP_HCD_DEVICE_SETUP_V1(
		USBPUMP_HCD_DEVICE *pHcdDevice,
		USBPUMP_HCD	*pHcd,
		UINT8		bAddress,
		USBPUMP_DEVICE_SPEED bSpeed,
		UINT8		bHubAddress,
		UINT8		bHubPort
		);

Description:
	USBPUMP_HCD_DEVICE_SETUP_V2() initializes pHcdDevice 
	as a proper USBPUMP_HCD_DEVICE, using the specified
	initial values. 
	
Notes:
	wMaxExitLatency in only for SuperSpeed devices.

	uRouteString is primarily for SuperSpeed devices, but the MCCI
	USBD will maintain it. The minihost USBD does not maintain it.
	
	bSlotID and bTTHubSlotID are only used with xHCI-style 
	host controllers; they indicate unique "device slots" associated
	with the host controller for the device and for the controlling
	hub.

	wMaxPacketSize of the default pipe in the USBPUMP_HCD_DEVICE 
	is initialized appropriate based on the device speed:  512 bytes
	for super speed, 64 for high speed, 8 bytes otherwise.

*/

#define __TMS_USBPUMP_HCD_DEVICE_SETUP_V2(				\
	a_pHcdDev,							\
	a_pHcd, a_uRouteString, a_bAddress, a_bSpeed, a_bHubAddress, 	\
	a_bHubPort, a_bSlotID, a_bTTHubSlotID, a_wMaxExitLatency	\
	)   do	{							\
		(a_pHcdDev)->Tag = __TMS_USBPUMP_HCD_DEVICE_TAG;	\
		(a_pHcdDev)->pHcd = (a_pHcd);				\
		(a_pHcdDev)->uRouteString = (a_uRouteString);		\
		(a_pHcdDev)->bAddress = (a_bAddress);			\
		(a_pHcdDev)->bSpeed = (a_bSpeed);			\
		(a_pHcdDev)->bHubAddress = (a_bHubAddress);		\
		(a_pHcdDev)->bHubPort = (a_bHubPort);			\
		(a_pHcdDev)->bSlotID = (a_bSlotID);			\
		(a_pHcdDev)->bTTHubSlotID = (a_bTTHubSlotID);		\
		(a_pHcdDev)->wMaxExitLatency = (a_wMaxExitLatency);	\
		(a_pHcdDev)->Capabilities.RawBits = 0;			\
		__TMS_USBPUMP_HCD_PIPE_SETUP_V2(			\
			&(a_pHcdDev)->DefaultPipe,			\
			(a_pHcdDev),					\
			/* max transfer size */				\
			   (a_pHcd)->Hcd.DefaultMaxTransferSize,	\
			/* interval */ 0,				\
			/* initial max packet size */			\
			__TMS_USBPUMP_IS_SUPER_SPEED(a_bSpeed)		\
				? 512 :					\
			__TMS_USBPUMP_IS_WIRELESS_SPEED(a_bSpeed)	\
				? 512 :					\
			__TMS_USBPUMP_IS_HIGH_SPEED(a_bSpeed)		\
				? 64					\
				: 8,					\
			/* ep addr */ 0,				\
			/* ep type */ USB_bmAttr_CONTROL,		\
			/* toggle */ 0,					\
			/* bExtraPackets */ 0,				\
			/* bMaxBurstSize */ 1,				\
			/* wMaxStreamID */ 0,				\
			/* wBytesPerInterval */ 0,			\
			/* bMult */ 0					\
			);						\
		} while (0)

#define __TMS_USBPUMP_HCD_DEVICE_SETUP_V1(				\
	a_pHcdDev,							\
	a_pHcd, a_bAddress, a_bSpeed, a_bHubAddress, a_bHubPort		\
	)   								\
	__TMS_USBPUMP_HCD_DEVICE_SETUP_V2(				\
		a_pHcdDev,						\
		a_pHcd, 0, a_bAddress, a_bSpeed, a_bHubAddress, 	\
		a_bHubPort, 0, 0, 0					\
		)

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_HCD_DEVICE_TAG	\
   __TMS_USBPUMP_HCD_DEVICE_TAG
# define USBPUMP_HCD_DEVICE_GET_ROUTE_STRING(pHcdDevice)	\
   __TMS_USBPUMP_HCD_DEVICE_GET_ROUTE_STRING(pHcdDevice)
# define USBPUMP_HCD_DEVICE_SET_ROUTE_STRING(pHcdDevice, uRouteString)	\
   __TMS_USBPUMP_HCD_DEVICE_SET_ROUTE_STRING(pHcdDevice, uRouteString)
#define  USBPUMP_HCD_DEVICE_CLEAR_ROUTE_STRING(pHcdDevice)\
   __TMS_USBPUMP_HCD_DEVICE_CLEAR_ROUTE_STRING(pHcdDevice)			
# define USBPUMP_HCD_DEVICE_IS_HUB(pHcdDevice)	\
   __TMS_USBPUMP_HCD_DEVICE_IS_HUB(pHcdDevice)
# define USBPUMP_HCD_DEVICE_SET_HUB(pHcdDevice)	\
   __TMS_USBPUMP_HCD_DEVICE_SET_HUB(pHcdDevice)
# define USBPUMP_HCD_DEVICE_GET_ROOT_HUB_PORT_NUMBER(pHcdDevice)	\
   __TMS_USBPUMP_HCD_DEVICE_GET_ROOT_HUB_PORT_NUMBER(pHcdDevice)
# define USBPUMP_HCD_DEVICE_SET_ROOT_HUB_PORT_NUMBER(pHcdDevice, bRootHubPortNumber)	\
   __TMS_USBPUMP_HCD_DEVICE_SET_ROOT_HUB_PORT_NUMBER(pHcdDevice, bRootHubPortNumber)
# define USBPUMP_HCD_DEVICE_GET_HUB_ADDRESS(pHcdDevice)	\
   __TMS_USBPUMP_HCD_DEVICE_GET_HUB_ADDRESS(pHcdDevice)
# define USBPUMP_HCD_DEVICE_HUB_TTPERPORT	\
   __TMS_USBPUMP_HCD_DEVICE_HUB_TTPERPORT
# define USBPUMP_HCD_DEVICE_SET_HUB_TTPERPORT(pHcdDevice)	\
   __TMS_USBPUMP_HCD_DEVICE_SET_HUB_TTPERPORT(pHcdDevice)
# define USBPUMP_HCD_DEVICE_IS_HUB_TTPERPORT(pHcdDevice)	\
   __TMS_USBPUMP_HCD_DEVICE_IS_HUB_TTPERPORT(pHcdDevice)
# define USBPUMP_HCD_DEVICE_IS_ON_TT(pHcdDevice)	\
   __TMS_USBPUMP_HCD_DEVICE_IS_ON_TT(pHcdDevice)
# define USBPUMP_HCD_DEVICE_SETUP_V2(				\
	a_pHcdDev,							\
	a_pHcd, a_uRouteString, a_bAddress, a_bSpeed, a_bHubAddress, 	\
	a_bHubPort, a_bSlotID, a_bTTHubSlotID, a_wMaxExitLatency	\
	)	\
	__TMS_USBPUMP_HCD_DEVICE_SETUP_V2(				\
	a_pHcdDev,							\
	a_pHcd, a_uRouteString, a_bAddress, a_bSpeed, a_bHubAddress, 	\
	a_bHubPort, a_bSlotID, a_bTTHubSlotID, a_wMaxExitLatency	\
	)
# define USBPUMP_HCD_DEVICE_SETUP_V1(				\
	a_pHcdDev,							\
	a_pHcd, a_bAddress, a_bSpeed, a_bHubAddress, a_bHubPort		\
	)	\
	__TMS_USBPUMP_HCD_DEVICE_SETUP_V1(				\
	a_pHcdDev,							\
	a_pHcd, a_bAddress, a_bSpeed, a_bHubAddress, a_bHubPort		\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_hcd_device.h ****/
#endif /* _USBPUMP_HCD_DEVICE_H_ */
