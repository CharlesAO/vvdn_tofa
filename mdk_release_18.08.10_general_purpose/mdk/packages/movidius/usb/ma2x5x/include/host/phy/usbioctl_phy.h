/* usbioctl_phy.h	Fri May 08 2015 10:27:09 chwon */

/*

Module:  usbioctl_phy.h

Function:
	IOCTLs for operating on USB Phys (called UTXD in the docs)

Version:
	V3.15b	Fri May 08 2015 10:27:09 chwon	Edit level 24

Copyright notice:
	This file copyright (C) 2005-2010, 2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Mon Aug 22 2005 00:44:13  tmm
	Module created.

   1.91h  Fri Aug 26 2005 11:21:35  chwon
	Add EDGE IOCTL codes.

   1.91h  Tue Aug 30 2005 23:50:59  chwon
	Fix REGISTER_HCD/DCD IOCTL definition; these are not async IOCTL.
	Remove including usbpump_hcd.h

   1.91h  Fri Sep  2 2005 00:00:02  tmm
	No need any more for usbpump_hcd_types.h

   1.91i  Mon Sep 26 2005 11:08:52  chwon
	Move USBPUMP_HCD type from usbpump_types.h to usbpump_hcd_types.h.

   1.93a  Thu Oct 13 2005 11:56:10  chwon
	Add CONNECT_USB_ISR IOCTL.

   1.93b  Wed Nov 09 2005 16:32:27  chwon
	Fix USBPUMP_IOCTL_USBPHY_CONNECT_USB_ISR_ARG type.

   1.93c  Wed Nov 30 2005 16:34:50  chwon
	Add USBPUMP_IOCTL_USBPHY_CLEAR_B_CONN and
	USBPUMP_IOCTL_USBPHY_CHECK_B_CONN IOCTL.

   1.93e  Tue Feb  7 2006 16:10:13  tmm
	Add USBPUMP_IOCTL_USBPHY_GLOBAL_CAPABILITY_CHECK_POWER and
	associated USBPUMP_IOCTL_USBPHY_CHECK_POWER, ..._BOOK_POWER and
	..._FREE_POWER IOCTLs.

   1.97j  Fri Sep 21 2007 16:50:43  chwon
	3291: add iPort in the USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ARG.
	Add HCD event -- USBPUMP_USBPHY_HCD_EVENT_POWER_INVALID

   1.97j  Thu Oct 04 2007 08:53:10  skpark
	3210: Add HCD event - USBPUMP_USBPHY_HCD_EVENT_REMOTE_WAKEUP

   1.97j  Mon Nov 19 2007 16:01:09  skpark
	3471: Add HCD event - USBPUMP_USBPHY_HCD_EVENT_PORT_IDLE

   1.97k  Tue Aug 05 2008 15:11:07  chwon
	UDP-6309: Add ISR function pointer in the HCD/DCD register IOCTL

   1.97k  Tue Aug 05 2008 16:16:36  chwon
	UDP-6309: Change ISR function prototype

   1.97k  Wed Jan 21 2009 16:47:17  chwon
	7283: change UDEVICE * and USBPUMP_HCD * to USBPUMP_OBJECT_HEADER *

   2.01a  Tue Nov 03 2009 12:30:53  chwon
	9234: move common type definition to usbpump_usbphy_types.h.
	Add new IOCTL to control USBPHY hardware:
		USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC
		USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC
		USBPUMP_IOCTL_USBPHY_START_ASYNC
		USBPUMP_IOCTL_USBPHY_STOP_ASYNC

   2.01a  Tue Nov 24 2009 15:45:08  wkkim
	9515: Fixed typo 
	(__TMS_USBUPMP_IOCTL_USBPHY -> __TMS_USBPUMP_IOCTL_USBPHY)

   3.01d  Sun Oct 24 2010 22:37:50  chwon
	11667: add port object pointer in the GET_PORT_CAPABILITIES async ioctl

   3.01d  Tue Oct 26 2010 21:01:53  skpark
	11736: Add new HCD events for SRP and HNP
	11667: Add new IOCTL to support SRP and HNP
		USBPUMP_IOCTL_USBPHY_HCD_START_HNP
		USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP
		USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE
		USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE

   3.01d  Mon Dec 13 2010 15:54:02  skpark
	11667: Add USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE

   3.11c  Mon Jun 03 2013 21:52:42  chwon
	17311: Add USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP ioctl.

   3.15b  Fri May 08 2015 10:27:09  chwon
	19161: Add USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE ioctl.

*/

#ifndef _USBIOCTL_PHY_H_		/* prevent multiple includes */
#define _USBIOCTL_PHY_H_

#ifndef _USBPUMP_USBPHY_TYPES_H_
# include "usbpump_usbphy_types.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBPUMPIOCTLS_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _UHILINTSYS_H_
# include "uhilintsys.h"
#endif

/****************************************************************************\
|
|	Define helper macros.
|
\****************************************************************************/

/**** helper macro for non-VOID USBPHY in-calls ****/
#define __TMS_USBPUMP_IOCTL_USBPHY(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_USBPHY,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_USBPHY_, NAME, _ARG)	\
		)

/**** helper macro for non-VOID USBPHY out-calls ****/
#define __TMS_USBPUMP_IOCTL_EDGE_USBPHY(i, t, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_USBPHY_EDGE,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_EDGE_USBPHY_, NAME, _ARG) \
		)


/****************************************************************************\
|
|	The USBPHY ioctl codes
|
\****************************************************************************/

#define	__TMS_USBPUMP_IOCTL_USBPHY_GET_GLOBAL_CAPABILITIES_ASYNC	\
	__TMS_USBPUMP_IOCTL_USBPHY(0, R_ASYNC, GET_GLOBAL_CAPABILITIES)

#define	__TMS_USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ASYNC		\
	__TMS_USBPUMP_IOCTL_USBPHY(1, RW_ASYNC, GET_PORT_CAPABILITIES)

#define	__TMS_USBPUMP_IOCTL_USBPHY_GLOBAL_POWER_ENABLE_ASYNC		\
	__TMS_USBPUMP_IOCTL_USBPHY(2, W_ASYNC, GLOBAL_POWER_ENABLE)

#define	__TMS_USBPUMP_IOCTL_USBPHY_PORT_POWER_ENABLE_ASYNC		\
	__TMS_USBPUMP_IOCTL_USBPHY(3, W_ASYNC, PORT_POWER_ENABLE)

#define	__TMS_USBPUMP_IOCTL_USBPHY_GET_GLOBAL_POWER_ASYNC		\
	__TMS_USBPUMP_IOCTL_USBPHY(4, R_ASYNC, GET_GLOBAL_POWER)

#define	__TMS_USBPUMP_IOCTL_USBPHY_GET_PORT_STATUS_ASYNC		\
	__TMS_USBPUMP_IOCTL_USBPHY(5, RW_ASYNC, GET_PORT_STATUS)

#define	__TMS_USBPUMP_IOCTL_USBPHY_REGISTER_HCD				\
	__TMS_USBPUMP_IOCTL_USBPHY(6, W, REGISTER_HCD)

#define	__TMS_USBPUMP_IOCTL_USBPHY_REGISTER_DCD				\
	__TMS_USBPUMP_IOCTL_USBPHY(7, W, REGISTER_DCD)

#define	__TMS_USBPUMP_IOCTL_USBPHY_RELEASE_PORT_ASYNC			\
	__TMS_USBPUMP_IOCTL_USBPHY(8, W_ASYNC, RELEASE_PORT)

#define	__TMS_USBPUMP_IOCTL_USBPHY_SUSPEND_PORT_ASYNC			\
	__TMS_USBPUMP_IOCTL_USBPHY(9, W_ASYNC, SUSPEND_PORT)

#define	__TMS_USBPUMP_IOCTL_USBPHY_CONNECT_USB_ISR			\
	__TMS_USBPUMP_IOCTL_USBPHY(10, RW, CONNECT_USB_ISR)

#define	__TMS_USBPUMP_IOCTL_USBPHY_CLEAR_B_CONN				\
	__TMS_USBPUMP_IOCTL_VOID(__TMS_USBPUMP_IOCTL_CLASS_USBPHY, 11)

#define	__TMS_USBPUMP_IOCTL_USBPHY_CHECK_B_CONN_ASYNC			\
	__TMS_USBPUMP_IOCTL_USBPHY(12, R_ASYNC, CHECK_B_CONN)

#define	__TMS_USBPUMP_IOCTL_USBPHY_CHECK_POWER				\
	__TMS_USBPUMP_IOCTL_USBPHY(13, RW, CHECK_POWER)

#define	__TMS_USBPUMP_IOCTL_USBPHY_BOOK_POWER				\
	__TMS_USBPUMP_IOCTL_USBPHY(14, RW, BOOK_POWER)

#define	__TMS_USBPUMP_IOCTL_USBPHY_FREE_POWER				\
	__TMS_USBPUMP_IOCTL_USBPHY(15, W, FREE_POWER)

#define	__TMS_USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC			\
	__TMS_USBPUMP_IOCTL_USBPHY(16, W_ASYNC, HARDWARE_INIT_ASYNC)

#define	__TMS_USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC		\
	__TMS_USBPUMP_IOCTL_USBPHY(17, W_ASYNC, HARDWARE_DEINIT_ASYNC)

#define	__TMS_USBPUMP_IOCTL_USBPHY_START_ASYNC				\
	__TMS_USBPUMP_IOCTL_USBPHY(18, W_ASYNC, START_ASYNC)

#define	__TMS_USBPUMP_IOCTL_USBPHY_STOP_ASYNC				\
	__TMS_USBPUMP_IOCTL_USBPHY(19, W_ASYNC, STOP_ASYNC)

#define __TMS_USBPUMP_IOCTL_USBPHY_HCD_START_HNP			\
	__TMS_USBPUMP_IOCTL_USBPHY(20, W, HCD_START_HNP)

#define __TMS_USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP			\
	__TMS_USBPUMP_IOCTL_USBPHY(21, W, HCD_FINISH_HNP)

#define __TMS_USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE			\
	__TMS_USBPUMP_IOCTL_USBPHY(22, W, HCD_WAIT_SRP_ENABLE)

#define __TMS_USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE			\
	__TMS_USBPUMP_IOCTL_USBPHY(23, W, DCD_AUTO_HNP_ENABLE)

#define __TMS_USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE		\
	__TMS_USBPUMP_IOCTL_USBPHY(24, W, HCD_SET_ABUS_REQUEST_MODE)

#define __TMS_USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP			\
	__TMS_USBPUMP_IOCTL_USBPHY(25, W, HCD_SET_ABUS_DROP)

#define __TMS_USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE			\
	__TMS_USBPUMP_IOCTL_USBPHY(26, W, SET_USB_ID_MODE)


/****************************************************************************\
|
|	The `EDGE' IOCTLs are sent outwards
|
\****************************************************************************/

#define	__TMS_USBPUMP_IOCTL_EDGE_USBPHY_HCD_GOT_DEVICE		\
	__TMS_USBPUMP_IOCTL_EDGE_USBPHY(0, W, HCD_GOT_DEVICE)

#define	__TMS_USBPUMP_IOCTL_EDGE_USBPHY_HCD_CABLE_DETACH	\
	__TMS_USBPUMP_IOCTL_EDGE_USBPHY(1, W, HCD_CABLE_DETACH)

#define	__TMS_USBPUMP_IOCTL_EDGE_USBPHY_DCD_START_SESSION	\
	__TMS_USBPUMP_IOCTL_EDGE_USBPHY(2, W, DCD_START_SESSION)

#define	__TMS_USBPUMP_IOCTL_EDGE_USBPHY_DCD_END_SESSION		\
	__TMS_USBPUMP_IOCTL_EDGE_USBPHY(3, W, DCD_END_SESSION)


/****************************************************************************\
|
|	Phy states
|
\****************************************************************************/

/* the states of the simple PHY */
#define	__TMS_USBPUMP_USBPHY_STATE_IDLE		0
#define	__TMS_USBPUMP_USBPHY_STATE_HOST		1
#define	__TMS_USBPUMP_USBPHY_STATE_DEVICE	2
#define	__TMS_USBPUMP_USBPHY_STATE_OTHER	3


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_EVENT_STATE_CHANGE_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_EVENT_STATE_CHANGE_INFO)
	{
	__TMS_UINT32			Port;
	__TMS_USBPUMP_USBPHY_STATE	OldState;
	__TMS_USBPUMP_USBPHY_STATE	NewState;
	};


/****************************************************************************\
|
|	HCD callback functions and messages
|
\****************************************************************************/

#define __TMS_USBPUMP_USBPHY_HCD_EVENT_GOT_DEVICE	0u
#define	__TMS_USBPUMP_USBPHY_HCD_EVENT_CABLE_DETACH	1u
#define	__TMS_USBPUMP_USBPHY_HCD_EVENT_STATE_CHANGE	2u
#define	__TMS_USBPUMP_USBPHY_HCD_EVENT_POWER_INVALID	3u
#define	__TMS_USBPUMP_USBPHY_HCD_EVENT_REMOTE_WAKEUP	4u
#define	__TMS_USBPUMP_USBPHY_HCD_EVENT_PORT_IDLE	5u
#define	__TMS_USBPUMP_USBPHY_HCD_EVENT_SRP_DETECT	6u
#define	__TMS_USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS	7u

#define __TMS_USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS_SUCCESS	0u
#define __TMS_USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS_TIMEOUT	1u
#define __TMS_USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS_FINISH	2u

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_HCD_EVENT_GOT_DEVICE_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_HCD_EVENT_GOT_DEVICE_INFO)
	{
	__TMS_UINT32			Port;
	__TMS_USBPUMP_DEVICE_SPEED	Speed;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_HCD_EVENT_CABLE_DETACH_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_HCD_EVENT_CABLE_DETACH_INFO)
	{
	__TMS_UINT32			Port;
	};

__TMS_TYPE_DEF(USBPUMP_USBPHY_HCD_EVENT_STATE_CHANGE_INFO,
	struct __TMS_STRUCTNAME(USBPUMP_USBPHY_EVENT_STATE_CHANGE_INFO));

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_HCD_EVENT_POWER_INVALID_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_HCD_EVENT_POWER_INVALID_INFO)
	{
	__TMS_UINT32			Port;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_HCD_EVENT_REMOTE_WAKEUP_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_HCD_EVENT_REMOTE_WAKEUP_INFO)
	{
	__TMS_UINT32			Port;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_HCD_EVENT_PORT_IDLE_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_HCD_EVENT_PORT_IDLE_INFO)
	{
	__TMS_UINT32			Port;
	__TMS_BOOL			fIdle;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_HCD_EVENT_SRP_DETECT_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_HCD_EVENT_SRP_DETECT_INFO)
	{
	__TMS_UINT32			Port;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS_INFO)
	{
	__TMS_UINT32			Port;
	__TMS_UINT32			HnpStatus;
	};
	
/****************************************************************************\
|
|	Device event functions and messages
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBPHY_DCD_EVENT_START_SESSION	0u
#define	__TMS_USBPUMP_USBPHY_DCD_EVENT_END_SESSION	1u
#define	__TMS_USBPUMP_USBPHY_DCD_EVENT_STATE_CHANGE	2u

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_DCD_EVENT_START_SESSION_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_DCD_EVENT_START_SESSION_INFO)
	{
	__TMS_UINT32			Port;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_DCD_EVENT_END_SESSION_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_DCD_EVENT_END_SESSION_INFO)
	{
	__TMS_UINT32			Port;
	};

__TMS_TYPE_DEF(USBPUMP_USBPHY_DCD_EVENT_STATE_CHANGE_INFO,
	struct __TMS_STRUCTNAME(USBPUMP_USBPHY_EVENT_STATE_CHANGE_INFO));


/****************************************************************************\
|
|	IOCTL Argument types and initialization macros
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_GET_GLOBAL_CAPABILITIES_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_GET_GLOBAL_CAPABILITIES_ARG)
	{
	/*
	|| root hubs can have up to 31 ports in this model, port numbers
	|| start at one.
	*/
	__TMS_UINT32	NumPorts;

	/*
	|| low word is same as standard USB hub characteristcs.  high word
	|| includes an OTG flag
	*/
	__TMS_UINT32	HubCharacteristics;

	/* power-on to power-good, in 1ms clicks */
	__TMS_UINT32	PowerOnToPowerGood;

	/* hub control current, in 1mA clicks, excluding embedded devs */
	__TMS_UINT32	HubControlCurrent;

	/*
	|| removable ports bitmask; bit 0 is reserved, since ports are
	|| 1-origin
	*/
	__TMS_UINT32	RemovablePorts;

	/*
	|| OTG ports bitmask.
	*/
	__TMS_UINT32	OtgPorts;

	/*
	|| Device-only ports bitmask
	*/
	__TMS_UINT32	DevOnlyPort;

	/*
	|| Ports that can run in special-purpose mode (eg. CarKit)
	*/
	__TMS_UINT32	SpecialPorts;
	};

/* if defined, the phy supports OTG */
#define	__TMS_USBPUMP_IOCTL_USBPHY_GLOBAL_CAPABILITY_OTG	(0x00010000u)

/* if defined, the phy implements check-/book-/free-power IOCTL set */
#define	__TMS_USBPUMP_IOCTL_USBPHY_GLOBAL_CAPABILITY_CHECK_POWER (0x00020000u)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ARG)
	{
	/* max power available on port, in 1mA ticks */
	__TMS_UINT32	iPort;		/* IN: port number start at 1 */
	__TMS_USBPUMP_OBJECT_HEADER *pPortObject;	/* IN: port objet */
	__TMS_UINT32	PortMaxPower;	/* OUT: port max power */
	};

#define __TMS_USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pPortObject,							\
	a_iPort								\
	)								\
   do	{								\
	(a_pArg)->iPort = (a_iPort);					\
	(a_pArg)->pPortObject = (a_pPortObject);			\
	(a_pArg)->PortMaxPower = 0;					\
	} while (0)

#define __TMS_USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ARG_SETUP_V1(	\
	a_pArg,								\
	a_iPort								\
	)								\
	__TMS_USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ARG_SETUP_V2(	\
	a_pArg,								\
	/* V2: pPortObject */ __TMS_NULL,				\
	a_iPort								\
	)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_GLOBAL_POWER_ENABLE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_GLOBAL_POWER_ENABLE_ARG)
	{
	__TMS_BOOL	fEnable;
	};

#define __TMS_USBPUMP_IOCTL_USBPHY_GLOBAL_POWER_ENABLE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_fEnable							\
	)								\
   do	{								\
	(a_pArg)->fEnable = (a_fEnable);				\
	} while (0)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_PORT_POWER_ENABLE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_PORT_POWER_ENABLE_ARG)
	{
	__TMS_UINT32	iPort;
	__TMS_BOOL	fEnable;
	};

#define __TMS_USBPUMP_IOCTL_USBPHY_PORT_POWER_ENABLE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_iPort,							\
	a_fEnable							\
	)								\
   do	{								\
	(a_pArg)->iPort = (a_iPort);					\
	(a_pArg)->fEnable = (a_fEnable);				\
	} while (0)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_GET_GLOBAL_POWER_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_GET_GLOBAL_POWER_ARG)
	{
	/* state of power to the PHY */
	__TMS_BOOL	fPower;
	/* see usbhub20.h -- state of the root hub: */
	__TMS_UINT16	wStatus;
	__TMS_UINT16	wChange;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_GET_PORT_STATUS_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_GET_PORT_STATUS_ARG)
	{
	__TMS_UINT32	iPort;
	/* output: */
	__TMS_USBPUMP_USBPHY_STATE	PhyState;

	/* see usbhub20.h -- status if in host state: otherwise signals disconnect */
	__TMS_UINT16	wStatus;
	__TMS_UINT16	wChange;

	/* in device mode, this reflects the attach state */
	__TMS_UINT32	DeviceFlags;
	};

#define __TMS_USBPUMP_USBPHY_DEVFLAG_VBUS	(0x00000001u)
#define	__TMS_USBPUMP_USBPHY_DEVFLAG_SUSPEND	(0x00000002u)
#define	__TMS_USBPUMP_USBPHY_DEVFLAG_HIGHSPEED	(0x00000004u)
#define	__TMS_USBPUMP_USBPHY_DEVFLAG_RESET	(0x00000008u)

#define __TMS_USBPUMP_IOCTL_USBPHY_GET_PORT_STATUS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_iPort								\
	)								\
   do	{								\
	UHIL_fill(&(a_pArg), sizeof(*(a_pArg)), 0);			\
	(a_pArg)->iPort = (a_iPort);					\
	} while (0)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_REGISTER_HCD_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_REGISTER_HCD_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER		*pHcdObject;
	__TMS_USBPUMP_USBPHY_HCD_EVENT_FN	*pHcdEventFn;
	__TMS_VOID				*pContext;
	__TMS_USBPUMP_USBPHY_HCD_ISR_FN		*pHcdIsrFn;	/* UDP-6309 */
	};

#define __TMS_USBPUMP_IOCTL_USBPHY_REGISTER_HCD_ARG_SETUP_V2(		\
	a_pArg,								\
	a_pHcdObject,							\
	a_pHcdEventFn,							\
	a_pContext,							\
	a_pHcdIsrFn							\
	)								\
   do	{								\
	(a_pArg)->pHcdObject = (a_pHcdObject);				\
	(a_pArg)->pHcdEventFn = (a_pHcdEventFn);			\
	(a_pArg)->pContext = (a_pContext);				\
	(a_pArg)->pHcdIsrFn = (a_pHcdIsrFn);				\
	} while (0)

#define __TMS_USBPUMP_IOCTL_USBPHY_REGISTER_HCD_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcdObject,							\
	a_pHcdEventFn,							\
	a_pContext							\
	)								\
	__TMS_USBPUMP_IOCTL_USBPHY_REGISTER_HCD_ARG_SETUP_V2(		\
	a_pArg,								\
	a_pHcdObject,							\
	a_pHcdEventFn,							\
	a_pContext,							\
	/* V2: pHcdIsrFn */ __TMS_NULL					\
	)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_REGISTER_DCD_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_REGISTER_DCD_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER		*pDcdObject;
	__TMS_USBPUMP_USBPHY_DCD_EVENT_FN	*pDcdEventFn;
	__TMS_VOID				*pContext;
	__TMS_USBPUMP_USBPHY_DCD_ISR_FN		*pDcdIsrFn;	/* UDP-6309 */
	};

#define __TMS_USBPUMP_IOCTL_USBPHY_REGISTER_DCD_ARG_SETUP_V2(		\
	a_pArg,								\
	a_pDcdObject,							\
	a_pDcdEventFn,							\
	a_pContext,							\
	a_pDcdIsrFn							\
	)								\
   do	{								\
	(a_pArg)->pDcdObject = (a_pDcdObject);				\
	(a_pArg)->pDcdEventFn = (a_pDcdEventFn);			\
	(a_pArg)->pContext = (a_pContext);				\
	(a_pArg)->pDcdIsrFn = (a_pDcdIsrFn);				\
	} while (0)

#define __TMS_USBPUMP_IOCTL_USBPHY_REGISTER_DCD_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pDcdObject,							\
	a_pDcdEventFn,							\
	a_pContext							\
	)								\
	__TMS_USBPUMP_IOCTL_USBPHY_REGISTER_DCD_ARG_SETUP_V2(		\
	a_pArg,								\
	a_pDcdObject,							\
	a_pDcdEventFn,							\
	a_pContext,							\
	/* V2: pDcdIsrFn */ __TMS_NULL					\
	)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_RELEASE_PORT_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_RELEASE_PORT_ARG)
	{
	__TMS_UINT32	iPort;
	};

#define __TMS_USBPUMP_IOCTL_USBPHY_RELEASE_PORT_ARG_SETUP_V1(		\
	a_pArg,								\
	a_iPort								\
	)								\
   do	{								\
	(a_pArg)->iPort = (a_iPort);					\
	} while (0)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_SUSPEND_PORT_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_SUSPEND_PORT_ARG)
	{
	__TMS_UINT32	iPort;
	__TMS_BOOL	fSuspend;
	};

#define __TMS_USBPUMP_IOCTL_USBPHY_SUSPEND_PORT_ARG_SETUP_V1(		\
	a_pArg,								\
	a_iPort,							\
	a_fSuspend							\
	)								\
   do	{								\
	(a_pArg)->iPort = (a_iPort);					\
	(a_pArg)->fSuspend = (a_fSuspend);				\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_EDGE_USBPHY_HCD_GOT_DEVICE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_EDGE_USBPHY_HCD_GOT_DEVICE_ARG)
	{
	__TMS_USBPUMP_USBPHY_HCD_EVENT_GOT_DEVICE_INFO	Info;
	};

#define __TMS_USBPUMP_IOCTL_EDGE_USBPHY_HCD_GOT_DEVICE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port,								\
	a_Speed								\
	)								\
   do	{								\
	(a_pArg)->Info.Port = (a_Port);					\
	(a_pArg)->Info.Speed = (a_Speed);				\
	} while (0)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_EDGE_USBPHY_HCD_CABLE_DETACH_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_EDGE_USBPHY_HCD_CABLE_DETACH_ARG)
	{
	__TMS_USBPUMP_USBPHY_HCD_EVENT_CABLE_DETACH_INFO	Info;
	};

#define __TMS_USBPUMP_IOCTL_EDGE_USBPHY_HCD_CABLE_DETACH_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port								\
	)								\
   do	{								\
	(a_pArg)->Info.Port = (a_Port);					\
	} while (0)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_EDGE_USBPHY_DCD_START_SESSION_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_EDGE_USBPHY_DCD_START_SESSION_ARG)
	{
	__TMS_USBPUMP_USBPHY_DCD_EVENT_START_SESSION_INFO	Info;
	};

#define __TMS_USBPUMP_IOCTL_EDGE_USBPHY_DCD_START_SESSION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port								\
	)								\
   do	{								\
	(a_pArg)->Info.Port = (a_Port);					\
	} while (0)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_EDGE_USBPHY_DCD_END_SESSION_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_EDGE_USBPHY_DCD_END_SESSION_ARG)
	{
	__TMS_USBPUMP_USBPHY_DCD_EVENT_END_SESSION_INFO		Info;
	};

#define __TMS_USBPUMP_IOCTL_EDGE_USBPHY_DCD_END_SESSION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port								\
	)								\
   do	{								\
	(a_pArg)->Info.Port = (a_Port);					\
	} while (0)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_CONNECT_USB_ISR_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_CONNECT_USB_ISR_ARG)
	{
	__TMS_UHIL_INTERRUPT_SERVICE_ROUTINE_FN	*pUsbIsr;
	__TMS_VOID				*pUsbIsrCtx;
	__TMS_UHIL_INTERRUPT_SERVICE_ROUTINE_FN	*pOldUsbIsr;
	__TMS_VOID				*pOldUsbIsrCtx;
	};

#define __TMS_USBPUMP_IOCTL_USBPHY_CONNECT_USB_ISR_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbIsr,							\
	a_pUsbIsrCtx							\
	)								\
   do	{								\
	(a_pArg)->pUsbIsr = (a_pUsbIsr);				\
	(a_pArg)->pUsbIsrCtx = (a_pUsbIsrCtx);				\
	(a_pArg)->pOldUsbIsr = __TMS_NULL;				\
	(a_pArg)->pOldUsbIsrCtx = __TMS_NULL;				\
	} while (0)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_CHECK_B_CONN_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_CHECK_B_CONN_ARG)
	{
	__TMS_BOOL	fB_CONN_State;	/* Output: B_CONN state */
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_CHECK_POWER_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_CHECK_POWER_ARG)
	{
	/* IN: the port number in question */
	__TMS_UINT32	Port;

	/* IN: the number of milliamperes of current */
	__TMS_UINT32	maxCurrent;

	/* OUT: the status */
	__TMS_USTAT	Status;
	};

#define	__TMS_USBPUMP_IOCTL_USBPHY_CHECK_POWER_ARG_SETUP_V1(		\
	a_pArg,								\
	a_Port,								\
	a_maxCurrent							\
	)								\
   do	{								\
	(a_pArg)->Port = (a_Port);					\
	(a_pArg)->maxCurrent = (a_maxCurrent);				\
	(a_pArg)->Status = 0;						\
	} while (0)

/* use this in implementations to set up the results */
#define	__TMS_USBPUMP_IOCTL_USBPHY_CHECK_POWER_ARG_RESULT_V1(		\
	a_pArg,								\
	a_Status							\
	)								\
   do	{								\
	(a_pArg)->Status = (a_Status);					\
	} while (0)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_BOOK_POWER_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_BOOK_POWER_ARG)
	{
	/* IN: the port number in question */
	__TMS_UINT32	Port;

	/* IN: the number of milliamperes of current */
	__TMS_UINT32	maxCurrent;

	/* OUT: the status */
	__TMS_USTAT	Status;
	};

#define	__TMS_USBPUMP_IOCTL_USBPHY_BOOK_POWER_ARG_SETUP_V1(		\
	a_pArg,								\
	a_Port,								\
	a_maxCurrent							\
	)								\
   do	{								\
	(a_pArg)->Port = (a_Port);					\
	(a_pArg)->maxCurrent = (a_maxCurrent);				\
	(a_pArg)->Status = 0;						\
	} while (0)

/* use this in implementations to set up the results */
#define	__TMS_USBPUMP_IOCTL_USBPHY_BOOK_POWER_ARG_RESULT_V1(		\
	a_pArg,								\
	a_Status							\
	)								\
   do	{								\
	(a_pArg)->Status = (a_Status);					\
	} while (0)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_FREE_POWER_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_FREE_POWER_ARG)
	{
	/* IN: the port number in question */
	__TMS_UINT32	Port;

	/* IN: the number of milliamperes of current */
	__TMS_UINT32	maxCurrent;
	};

#define	__TMS_USBPUMP_IOCTL_USBPHY_FREE_POWER_ARG_SETUP_V1(		\
	a_pArg,								\
	a_Port,								\
	a_maxCurrent							\
	)								\
   do	{								\
	(a_pArg)->Port = (a_Port);					\
	(a_pArg)->maxCurrent = (a_maxCurrent);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC

Index:	Type:	USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC_ARG
	Name:	USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC_ARG_SETUP_V1()

Function:
	Initialize USBPHY hardware

Input:
	USBPUMP_USBPHY *pUsbPhy;
		The USBPHY of interest.

Description:
	This IOCTL is sent from the client to initialize USBPHY hardware, 
	but without attaching to interrupt. 
	The USBPHY IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- Hardware is already in use (USBPUMP_IOCTL_RESULT_HARDWARE_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC_ARG *pArg,
		USBPUMP_USBPHY *pUsbPhy
		);

*/

/**** parameters to USBPHY_HARDWARE_INIT_ASYNC ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC_ARG)
	{
	__TMS_USBPUMP_USBPHY *	pUsbPhy;		/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbPhy							\
	)								\
    do	{								\
	(a_pArg)->pUsbPhy = (a_pUsbPhy);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC

Index:	Type:	USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC_ARG
	Name:	USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1()

Function:
	De-initialize USBPHY hardware

Input:
	USBPUMP_USBPHY *pUsbPhy;
		The USBPHY of interest.

Description:
	This IOCTL is sent from the client to de-initialize USBPHY hardware 
	and detach from interrupt if this hasn't been done. 
	The USBPHY IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- Hardware is not in use (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC_ARG *pArg,
		USBPUMP_USBPHY *pUsbPhy
		);

*/

/**** parameters to USBPHY_HARDWARE_DEINIT_ASYNC ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC_ARG)
	{
	__TMS_USBPUMP_USBPHY *	pUsbPhy;		/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbPhy							\
	)								\
    do	{								\
	(a_pArg)->pUsbPhy = (a_pUsbPhy);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBPHY_START_ASYNC

Index:	Type:	USBPUMP_IOCTL_USBPHY_START_ASYNC_ARG
	Name:	USBPUMP_IOCTL_USBPHY_START_ASYNC_ARG_SETUP_V1()

Function:
	Start USB PHY controller

Input:
	USBPUMP_USBPHY *pUsbPhy;
		The USBPHY of interest.

Description:
	This IOCTL is sent from the client to start USB PHY 
	controller, and attach to interrupt. 
	The USBPHY IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- USBPHY already started (USBPUMP_IOCTL_RESULT_ALREADY_STARTED)	
	- Hardware is not in use (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_USBPHY_START_ASYNC_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBPHY_START_ASYNC_ARG *pArg,
		USBPUMP_USBPHY *pUsbPhy
		);

*/

/**** parameters to USBPHY_START_ASYNC ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_START_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_START_ASYNC_ARG)
	{
	__TMS_USBPUMP_USBPHY *	pUsbPhy;		/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_USBPHY_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pUsbPhy						\
	)							\
    do	{							\
	(a_pArg)->pUsbPhy = (a_pUsbPhy);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBPHY_STOP_ASYNC

Index:	Type:	USBPUMP_IOCTL_USBPHY_STOP_ASYNC_ARG
	Name:	USBPUMP_IOCTL_USBPHY_STOP_ASYNC_ARG_SETUP_V1()

Function:
	Stop USB PHY controller

Input:
	USBPUMP_USBPHY *pUsbPhy;
		The USBPHY of interest.

Description:
	This IOCTL is sent from the client to stop USB PHY 
	controller, and detach from interrupt. 
	The USBPHY IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- USBPHY already stopped (USBPUMP_IOCTL_RESULT_ALREADY_STOPPED)	
	- Hardware is not in use (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_USBPHY_START_ASYNC_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBPHY_START_ASYNC_ARG *pArg,
		USBPUMP_USBPHY *pUsbPhy
		);

*/

/**** parameters to USBPHY_STOP_ASYNC ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_STOP_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_STOP_ASYNC_ARG)
	{
	__TMS_USBPUMP_USBPHY *	pUsbPhy;		/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_USBPHY_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pUsbPhy						\
	)							\
    do	{							\
	(a_pArg)->pUsbPhy = (a_pUsbPhy);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBPHY_HCD_START_HNP

Index:	Type:	USBPUMP_IOCTL_USBPHY_HCD_START_HNP_ARG
	Name:	USBPUMP_IOCTL_USBPHY_HCD_START_HNP_ARG_SETUP_V1()

Function:
	Start HNP on the A-device

Input:
	UINT32	Port;
		The port number of interest.

Description:
	USBPHY sends this IOCTL to HCD to start HNP. HCD will send
	USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION to the HNP port.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)		
	- No HNP capable port (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_SUPPORT)

Setup Macro:
	VOID USBPUMP_IOCTL_USBPHY_HCD_START_HNP_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBPHY_HCD_START_HNP_ARG *pArg,
		UINT32	iPort
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_HCD_START_HNP_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_HCD_START_HNP_ARG)
	{
	__TMS_UINT32	Port;				/* IN */
	};
#define __TMS_USBPUMP_IOCTL_USBPHY_HCD_START_HNP_ARG_SETUP_V1(	\
	a_pArg,							\
	a_Port							\
	)							\
   do	{							\
	(a_pArg)->Port = (a_Port);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP

Index:	Type:	USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP_ARG
	Name:	USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP_ARG_SETUP_V1()

Function:
	Finish HNP on the A-device

Input:
	UINT32	Port;
		The port number of interest.

Description:
	USBPHY sends this IOCTL to HCD to finish HNP. HCD will send
	USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION to the HNP port.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)		
	- HNP port missing (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_SUPPORT)

Setup Macro:
	VOID USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP_ARG *pArg,
		UINT32	iPort
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP_ARG)
	{
	__TMS_UINT32	Port;				/* IN */
	};
#define __TMS_USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP_ARG_SETUP_V1(	\
	a_pArg,							\
	a_Port							\
	)							\
   do	{							\
	(a_pArg)->Port = (a_Port);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE

Index:	Type:	USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE_ARG
	Name:	USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE_ARG_SETUP_V1()

Function:
	Configure the WaitSRP feature on the A-device

Input:
	UINT32	Port;
		The port number of interest.
	BOOL	fEnable
		True if the WaitSRP feature is turned on.

Description:
	USBPHY sends this IOCTL to HCD to configure the WaitSRP feature on 
	a given port. HCD will send 
	USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP ioctl to the port.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)		
	- No SRP capable port (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_SUPPORT)

Setup Macro:
	VOID USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE_ARG *pArg,
		UINT32	Port,
		BOOL	fEnable
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE_ARG)
	{
	__TMS_UINT32	Port;				/* IN */
	__TMS_BOOL	fEnable;			/* IN */
	};
#define __TMS_USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE_ARG_SETUP_V1(	\
	a_pArg,							\
	a_Port,							\
	a_fEnable						\
	)							\
   do	{							\
	(a_pArg)->Port = (a_Port);				\
	(a_pArg)->fEnable = (a_fEnable);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE

Index:	Type:	USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE_ARG
	Name:	USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE_ARG_SETUP_V1()

Function:
	Configure the AutoHNP feature on the B-device

Input:
	BOOL	fEnable
		True if the AutoHNP feature is turned on.

Description:
	USBPHY sends this IOCTL to DCD to configure fAutoHnpEnable on the
	DCD object. If fAutoHnpEnable is set, DCD will send 
	USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST after receiving B_HNP_ENABLE from
	host when this feature is on.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)

Setup Macro:
	VOID USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE_ARG *pArg,
		BOOL	fEnable
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE_ARG)
	{
	__TMS_BOOL	fEnable;			/* IN */
	};
#define __TMS_USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE_ARG_SETUP_V1(	\
	a_pArg,							\
	a_fEnable						\
	)							\
   do	{							\
	(a_pArg)->fEnable = (a_fEnable);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE

Index:	Type:	USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE_ARG
	Name:	USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE_ARG_SETUP_V1()

Function:
	Configure the A_BUS_REQUEST mode on the A-device

Input:
	UINT32	Port;
		The port number of interest.
	USBPUMP_USBPHY_ABUS_REQUEST_MODE	AbusRequestMode;
		HCD send A_BUS_REQUEST based on this mode.

Description:
	USBPHY sends this IOCTL to HCD to configure the A_BUS_REQUEST mode on
	a given port.

	<< automatic setting with ID detection >>
	USBPUMP_USBPHY_ABUS_REQUEST_MODE_AUTO: HCD sends A_BUS_REQUEST to PHY if
		ID becomes low and clears A_BUS_REQUEST if ID becomes high.

	<< manual setting >>
	USBPUMP_USBPHY_ABUS_REQUEST_MODE_ON: HCD sends A_BUS_REQUEST receiving
		upon this request.
	USBPUMP_USBPHY_ABUS_REQUEST_MODE_OFF: HCD clears A_BUS_REQUEST receiving
		upon this request.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or

	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)
	- No SRP capable port (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_SUPPORT)

Setup Macro:
	VOID USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE_ARG *pArg,
		UINT32					Port,
		USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE	AbusRequestMode
		);

*/

#define __TMS_USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE_AUTO			0
#define __TMS_USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE_ON			1
#define __TMS_USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE_OFF			2

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE_ARG)
	{
	__TMS_UINT32	Port;	/* IN */
	__TMS_USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE	AbusRequestMode; /* IN */
	};
#define __TMS_USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE_ARG_SETUP_V1( \
	a_pArg,							\
	a_Port,							\
	a_AbusRequestMode						\
	)							\
   do	{							\
	(a_pArg)->Port = (a_Port);				\
	(a_pArg)->AbusRequestMode = (a_AbusRequestMode);	\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP

Index:	Type:	USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP_ARG
	Name:	USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP_ARG_SETUP_V1()

Function:
	Enable / disable the A_BUS_DROP on the A-device

Input:
	UINT32	Port;
		The port number of interest.

	BOOL	fAbusDrop;
		The flag for controlling the A_BUS_DROP

Description:
	USBPHY sends this IOCTL to HCD to enable / disable the A_BUS_DROP on
	a given port.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or

	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)
	- No OTG capable port (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_SUPPORT)

Setup Macro:
	VOID USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP_ARG *pArg,
		UINT32	Port,
		BOOL	fAbusDrop
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP_ARG)
	{
	__TMS_UINT32	Port;		/* IN */
	__TMS_BOOL	fAbusDrop;	/* IN */
	};

#define __TMS_USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP_ARG_SETUP_V1( \
	a_pArg,							\
	a_Port,							\
	a_fAbusDrop						\
	)							\
   do	{							\
	(a_pArg)->Port = (a_Port);				\
	(a_pArg)->fAbusDrop = (a_fAbusDrop);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE

Index:	Type:	USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE_ARG
	Name:	USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE_ARG_SETUP_V1()

Function:
	Set USB ID mode

Input:
	USBPUMP_USBPHY *pUsbPhy;
		The USBPHY of interest.

	UINT IdMode;
		USB ID mode to set. Defined USB ID modes are

		USBPUMP_IOTCL_USBPHY_USB_ID_MODE_FORCE_LOW
			Force set ID state to low which is A-device mode.

		USBPUMP_IOTCL_USBPHY_USB_ID_MODE_FORCE_HIGH
			Force set ID state to high which is B-device mode.

		USBPUMP_IOTCL_USBPHY_USB_ID_MODE_NORMAL
			This is default mode and ID is controlled by ID signal.

Description:
	The client sends this IOCTL to USBPHY object to set USB ID mode.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or

	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)

Setup Macro:
	VOID USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE_ARG *pArg,
		USBPUMP_USBPHY *pUsbPhy,
		UINT IdMode
		);

*/

#define	__TMS_USBPUMP_IOTCL_USBPHY_USB_ID_MODE_FORCE_LOW	0
#define	__TMS_USBPUMP_IOTCL_USBPHY_USB_ID_MODE_FORCE_HIGH	1
#define	__TMS_USBPUMP_IOTCL_USBPHY_USB_ID_MODE_NORMAL		2

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE_ARG)
	{
	__TMS_USBPUMP_USBPHY *	pUsbPhy;	/* IN */
	__TMS_UINT		IdMode;		/* IN */
	};

#define __TMS_USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbPhy,							\
	a_IdMode							\
	)								\
    do	{								\
	(a_pArg)->pUsbPhy = (a_pUsbPhy);				\
	(a_pArg)->IdMode = (a_IdMode);					\
	} while (0)


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_USBPHY(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_USBPHY(i, t, NAME)
# define USBPUMP_IOCTL_EDGE_USBPHY(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_EDGE_USBPHY(i, t, NAME)
# define USBPUMP_IOCTL_USBPHY_GET_GLOBAL_CAPABILITIES_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_GET_GLOBAL_CAPABILITIES_ASYNC
# define USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ASYNC
# define USBPUMP_IOCTL_USBPHY_GLOBAL_POWER_ENABLE_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_GLOBAL_POWER_ENABLE_ASYNC
# define USBPUMP_IOCTL_USBPHY_PORT_POWER_ENABLE_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_PORT_POWER_ENABLE_ASYNC
# define USBPUMP_IOCTL_USBPHY_GET_GLOBAL_POWER_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_GET_GLOBAL_POWER_ASYNC
# define USBPUMP_IOCTL_USBPHY_GET_PORT_STATUS_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_GET_PORT_STATUS_ASYNC
# define USBPUMP_IOCTL_USBPHY_REGISTER_HCD	\
   __TMS_USBPUMP_IOCTL_USBPHY_REGISTER_HCD
# define USBPUMP_IOCTL_USBPHY_REGISTER_DCD	\
   __TMS_USBPUMP_IOCTL_USBPHY_REGISTER_DCD
# define USBPUMP_IOCTL_USBPHY_RELEASE_PORT_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_RELEASE_PORT_ASYNC
# define USBPUMP_IOCTL_USBPHY_SUSPEND_PORT_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_SUSPEND_PORT_ASYNC
# define USBPUMP_IOCTL_USBPHY_CONNECT_USB_ISR	\
   __TMS_USBPUMP_IOCTL_USBPHY_CONNECT_USB_ISR
# define USBPUMP_IOCTL_USBPHY_CLEAR_B_CONN	\
   __TMS_USBPUMP_IOCTL_USBPHY_CLEAR_B_CONN
# define USBPUMP_IOCTL_USBPHY_CHECK_B_CONN_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_CHECK_B_CONN_ASYNC
# define USBPUMP_IOCTL_USBPHY_CHECK_POWER	\
   __TMS_USBPUMP_IOCTL_USBPHY_CHECK_POWER
# define USBPUMP_IOCTL_USBPHY_BOOK_POWER	\
   __TMS_USBPUMP_IOCTL_USBPHY_BOOK_POWER
# define USBPUMP_IOCTL_USBPHY_FREE_POWER	\
   __TMS_USBPUMP_IOCTL_USBPHY_FREE_POWER
# define USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC
# define USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC
# define USBPUMP_IOCTL_USBPHY_START_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_START_ASYNC
# define USBPUMP_IOCTL_USBPHY_STOP_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBPHY_STOP_ASYNC
# define USBPUMP_IOCTL_USBPHY_HCD_START_HNP	\
   __TMS_USBPUMP_IOCTL_USBPHY_HCD_START_HNP
# define USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP	\
   __TMS_USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP
# define USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE	\
   __TMS_USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE
# define USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE	\
   __TMS_USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE
# define USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE	\
   __TMS_USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE
# define USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP	\
   __TMS_USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP
# define USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE	\
   __TMS_USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE
# define USBPUMP_IOCTL_EDGE_USBPHY_HCD_GOT_DEVICE	\
   __TMS_USBPUMP_IOCTL_EDGE_USBPHY_HCD_GOT_DEVICE
# define USBPUMP_IOCTL_EDGE_USBPHY_HCD_CABLE_DETACH	\
   __TMS_USBPUMP_IOCTL_EDGE_USBPHY_HCD_CABLE_DETACH
# define USBPUMP_IOCTL_EDGE_USBPHY_DCD_START_SESSION	\
   __TMS_USBPUMP_IOCTL_EDGE_USBPHY_DCD_START_SESSION
# define USBPUMP_IOCTL_EDGE_USBPHY_DCD_END_SESSION	\
   __TMS_USBPUMP_IOCTL_EDGE_USBPHY_DCD_END_SESSION
# define USBPUMP_USBPHY_STATE_IDLE	\
   __TMS_USBPUMP_USBPHY_STATE_IDLE
# define USBPUMP_USBPHY_STATE_HOST	\
   __TMS_USBPUMP_USBPHY_STATE_HOST
# define USBPUMP_USBPHY_STATE_DEVICE	\
   __TMS_USBPUMP_USBPHY_STATE_DEVICE
# define USBPUMP_USBPHY_STATE_OTHER	\
   __TMS_USBPUMP_USBPHY_STATE_OTHER
# define USBPUMP_USBPHY_HCD_EVENT_GOT_DEVICE	\
   __TMS_USBPUMP_USBPHY_HCD_EVENT_GOT_DEVICE
# define USBPUMP_USBPHY_HCD_EVENT_CABLE_DETACH	\
   __TMS_USBPUMP_USBPHY_HCD_EVENT_CABLE_DETACH
# define USBPUMP_USBPHY_HCD_EVENT_STATE_CHANGE	\
   __TMS_USBPUMP_USBPHY_HCD_EVENT_STATE_CHANGE
# define USBPUMP_USBPHY_HCD_EVENT_POWER_INVALID	\
   __TMS_USBPUMP_USBPHY_HCD_EVENT_POWER_INVALID
# define USBPUMP_USBPHY_HCD_EVENT_REMOTE_WAKEUP	\
   __TMS_USBPUMP_USBPHY_HCD_EVENT_REMOTE_WAKEUP
# define USBPUMP_USBPHY_HCD_EVENT_PORT_IDLE	\
   __TMS_USBPUMP_USBPHY_HCD_EVENT_PORT_IDLE
# define USBPUMP_USBPHY_HCD_EVENT_SRP_DETECT	\
   __TMS_USBPUMP_USBPHY_HCD_EVENT_SRP_DETECT
# define USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS	\
   __TMS_USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS
# define USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS_SUCCESS	\
   __TMS_USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS_SUCCESS
# define USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS_TIMEOUT	\
   __TMS_USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS_TIMEOUT
# define USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS_FINISH	\
   __TMS_USBPUMP_USBPHY_HCD_EVENT_HNP_STATUS_FINISH
# define USBPUMP_USBPHY_DCD_EVENT_START_SESSION	\
   __TMS_USBPUMP_USBPHY_DCD_EVENT_START_SESSION
# define USBPUMP_USBPHY_DCD_EVENT_END_SESSION	\
   __TMS_USBPUMP_USBPHY_DCD_EVENT_END_SESSION
# define USBPUMP_USBPHY_DCD_EVENT_STATE_CHANGE	\
   __TMS_USBPUMP_USBPHY_DCD_EVENT_STATE_CHANGE
# define USBPUMP_IOCTL_USBPHY_GLOBAL_CAPABILITY_OTG	\
   __TMS_USBPUMP_IOCTL_USBPHY_GLOBAL_CAPABILITY_OTG
# define USBPUMP_IOCTL_USBPHY_GLOBAL_CAPABILITY_CHECK_POWER	\
   __TMS_USBPUMP_IOCTL_USBPHY_GLOBAL_CAPABILITY_CHECK_POWER
# define USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pPortObject,							\
	a_iPort								\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pPortObject,							\
	a_iPort								\
	)
# define USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ARG_SETUP_V1(	\
	a_pArg,								\
	a_iPort								\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_GET_PORT_CAPABILITIES_ARG_SETUP_V1(	\
	a_pArg,								\
	a_iPort								\
	)
# define USBPUMP_IOCTL_USBPHY_GLOBAL_POWER_ENABLE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_fEnable							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_GLOBAL_POWER_ENABLE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_fEnable							\
	)
# define USBPUMP_IOCTL_USBPHY_PORT_POWER_ENABLE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_iPort,							\
	a_fEnable							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_PORT_POWER_ENABLE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_iPort,							\
	a_fEnable							\
	)
# define USBPUMP_USBPHY_DEVFLAG_VBUS	\
   __TMS_USBPUMP_USBPHY_DEVFLAG_VBUS
# define USBPUMP_USBPHY_DEVFLAG_SUSPEND	\
   __TMS_USBPUMP_USBPHY_DEVFLAG_SUSPEND
# define USBPUMP_USBPHY_DEVFLAG_HIGHSPEED	\
   __TMS_USBPUMP_USBPHY_DEVFLAG_HIGHSPEED
# define USBPUMP_USBPHY_DEVFLAG_RESET	\
   __TMS_USBPUMP_USBPHY_DEVFLAG_RESET
# define USBPUMP_IOCTL_USBPHY_GET_PORT_STATUS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_iPort								\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_GET_PORT_STATUS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_iPort								\
	)
# define USBPUMP_IOCTL_USBPHY_REGISTER_HCD_ARG_SETUP_V2(		\
	a_pArg,								\
	a_pHcdObject,							\
	a_pHcdEventFn,							\
	a_pContext,							\
	a_pHcdIsrFn							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_REGISTER_HCD_ARG_SETUP_V2(		\
	a_pArg,								\
	a_pHcdObject,							\
	a_pHcdEventFn,							\
	a_pContext,							\
	a_pHcdIsrFn							\
	)
# define USBPUMP_IOCTL_USBPHY_REGISTER_HCD_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcdObject,							\
	a_pHcdEventFn,							\
	a_pContext							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_REGISTER_HCD_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcdObject,							\
	a_pHcdEventFn,							\
	a_pContext							\
	)
# define USBPUMP_IOCTL_USBPHY_REGISTER_DCD_ARG_SETUP_V2(		\
	a_pArg,								\
	a_pDcdObject,							\
	a_pDcdEventFn,							\
	a_pContext,							\
	a_pDcdIsrFn							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_REGISTER_DCD_ARG_SETUP_V2(		\
	a_pArg,								\
	a_pDcdObject,							\
	a_pDcdEventFn,							\
	a_pContext,							\
	a_pDcdIsrFn							\
	)
# define USBPUMP_IOCTL_USBPHY_REGISTER_DCD_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pDcdObject,							\
	a_pDcdEventFn,							\
	a_pContext							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_REGISTER_DCD_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pDcdObject,							\
	a_pDcdEventFn,							\
	a_pContext							\
	)
# define USBPUMP_IOCTL_USBPHY_RELEASE_PORT_ARG_SETUP_V1(		\
	a_pArg,								\
	a_iPort								\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_RELEASE_PORT_ARG_SETUP_V1(		\
	a_pArg,								\
	a_iPort								\
	)
# define USBPUMP_IOCTL_USBPHY_SUSPEND_PORT_ARG_SETUP_V1(		\
	a_pArg,								\
	a_iPort,							\
	a_fSuspend							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_SUSPEND_PORT_ARG_SETUP_V1(		\
	a_pArg,								\
	a_iPort,							\
	a_fSuspend							\
	)
# define USBPUMP_IOCTL_EDGE_USBPHY_HCD_GOT_DEVICE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port,								\
	a_Speed								\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_USBPHY_HCD_GOT_DEVICE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port,								\
	a_Speed								\
	)
# define USBPUMP_IOCTL_EDGE_USBPHY_HCD_CABLE_DETACH_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port								\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_USBPHY_HCD_CABLE_DETACH_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port								\
	)
# define USBPUMP_IOCTL_EDGE_USBPHY_DCD_START_SESSION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port								\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_USBPHY_DCD_START_SESSION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port								\
	)
# define USBPUMP_IOCTL_EDGE_USBPHY_DCD_END_SESSION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port								\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_USBPHY_DCD_END_SESSION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_Port								\
	)
# define USBPUMP_IOCTL_USBPHY_CONNECT_USB_ISR_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbIsr,							\
	a_pUsbIsrCtx							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_CONNECT_USB_ISR_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbIsr,							\
	a_pUsbIsrCtx							\
	)
# define USBPUMP_IOCTL_USBPHY_CHECK_POWER_ARG_SETUP_V1(		\
	a_pArg,								\
	a_Port,								\
	a_maxCurrent							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_CHECK_POWER_ARG_SETUP_V1(		\
	a_pArg,								\
	a_Port,								\
	a_maxCurrent							\
	)
# define USBPUMP_IOCTL_USBPHY_CHECK_POWER_ARG_RESULT_V1(		\
	a_pArg,								\
	a_Status							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_CHECK_POWER_ARG_RESULT_V1(		\
	a_pArg,								\
	a_Status							\
	)
# define USBPUMP_IOCTL_USBPHY_BOOK_POWER_ARG_SETUP_V1(		\
	a_pArg,								\
	a_Port,								\
	a_maxCurrent							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_BOOK_POWER_ARG_SETUP_V1(		\
	a_pArg,								\
	a_Port,								\
	a_maxCurrent							\
	)
# define USBPUMP_IOCTL_USBPHY_BOOK_POWER_ARG_RESULT_V1(		\
	a_pArg,								\
	a_Status							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_BOOK_POWER_ARG_RESULT_V1(		\
	a_pArg,								\
	a_Status							\
	)
# define USBPUMP_IOCTL_USBPHY_FREE_POWER_ARG_SETUP_V1(		\
	a_pArg,								\
	a_Port,								\
	a_maxCurrent							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_FREE_POWER_ARG_SETUP_V1(		\
	a_pArg,								\
	a_Port,								\
	a_maxCurrent							\
	)
# define USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbPhy							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbPhy							\
	)
# define USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbPhy							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbPhy							\
	)
# define USBPUMP_IOCTL_USBPHY_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pUsbPhy						\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pUsbPhy						\
	)
# define USBPUMP_IOCTL_USBPHY_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pUsbPhy						\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pUsbPhy						\
	)
# define USBPUMP_IOCTL_USBPHY_HCD_START_HNP_ARG_SETUP_V1(	\
	a_pArg,							\
	a_Port							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_HCD_START_HNP_ARG_SETUP_V1(	\
	a_pArg,							\
	a_Port							\
	)
# define USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP_ARG_SETUP_V1(	\
	a_pArg,							\
	a_Port							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_HCD_FINISH_HNP_ARG_SETUP_V1(	\
	a_pArg,							\
	a_Port							\
	)
# define USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE_ARG_SETUP_V1(	\
	a_pArg,							\
	a_Port,							\
	a_fEnable						\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_HCD_WAIT_SRP_ENABLE_ARG_SETUP_V1(	\
	a_pArg,							\
	a_Port,							\
	a_fEnable						\
	)
# define USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE_ARG_SETUP_V1(	\
	a_pArg,							\
	a_fEnable						\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_DCD_AUTO_HNP_ENABLE_ARG_SETUP_V1(	\
	a_pArg,							\
	a_fEnable						\
	)
# define USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE_AUTO	\
   __TMS_USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE_AUTO
# define USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE_ON	\
   __TMS_USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE_ON
# define USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE_OFF	\
   __TMS_USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE_OFF
# define USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE_ARG_SETUP_V1( \
	a_pArg,							\
	a_Port,							\
	a_AbusRequestMode						\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_REQUEST_MODE_ARG_SETUP_V1( \
	a_pArg,							\
	a_Port,							\
	a_AbusRequestMode						\
	)
# define USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP_ARG_SETUP_V1( \
	a_pArg,							\
	a_Port,							\
	a_fAbusDrop						\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_HCD_SET_ABUS_DROP_ARG_SETUP_V1( \
	a_pArg,							\
	a_Port,							\
	a_fAbusDrop						\
	)
# define USBPUMP_IOTCL_USBPHY_USB_ID_MODE_FORCE_LOW	\
   __TMS_USBPUMP_IOTCL_USBPHY_USB_ID_MODE_FORCE_LOW
# define USBPUMP_IOTCL_USBPHY_USB_ID_MODE_FORCE_HIGH	\
   __TMS_USBPUMP_IOTCL_USBPHY_USB_ID_MODE_FORCE_HIGH
# define USBPUMP_IOTCL_USBPHY_USB_ID_MODE_NORMAL	\
   __TMS_USBPUMP_IOTCL_USBPHY_USB_ID_MODE_NORMAL
# define USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbPhy,							\
	a_IdMode							\
	)	\
	__TMS_USBPUMP_IOCTL_USBPHY_SET_USB_ID_MODE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pUsbPhy,							\
	a_IdMode							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_phy.h ****/
#endif /* _USBIOCTL_PHY_H_ */
