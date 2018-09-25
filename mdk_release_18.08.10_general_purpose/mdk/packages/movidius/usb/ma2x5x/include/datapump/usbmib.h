/* usbmib.h	Fri Oct 13 2000 17:25:26 tmm */

/*

Module:  usbmib.h

Function:
	Structures corresponding to the USB MIB (from draft-dolnik-usb-mib-01
	.txt)

Version:
	V1.60r	Fri Oct 13 2000 17:25:26 tmm	Edit level 3

Copyright notice:
	This file copyright (C) 2000 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	September 2000

Revision history:
   1.60n  Mon Sep 25 2000 08:55:34  tmm
	Module created.

   1.60p  10/6/2000  tmm
	Correct based on feedback from customer.

   1.60r  10/13/2000  tmm
	We were missing a few statistics masks.

*/

#ifndef _USBMIB_H_		/* prevent multiple includes */
#define _USBMIB_H_

/****************************************************************************\
|
|	The USBMIB_PORT_ENTRY structure describes a physical port
|	and corresponds to a usbPortEntry object.
|
\****************************************************************************/


#ifndef __TMS_CLOAKED_NAMES_ONLY
# define __TMS_CLOAKED_NAMES_ONLY 0
#endif

typedef struct __TMS_USBMIB_PORT_ENTRY_20000301
	{
	unsigned long	usbPortIndex;
	unsigned	usbPortType;
	unsigned	usbPortRate;
	} __TMS_USBMIB_PORT_ENTRY_20000301,
	*__TMS_PUSBMIB_PORT_ENTRY_20000301;

typedef __TMS_USBMIB_PORT_ENTRY_20000301	__TMS_USBMIB_PORT_ENTRY,
						*__TMS_PUSBMIB_PORT_ENTRY;

#if !__TMS_CLOAKED_NAMES_ONLY
typedef	__TMS_USBMIB_PORT_ENTRY_20000301	USBMIB_PORT_ENTRY_20000301,
					*PUSBMIB_PORT_ENTRY_20000301;
typedef	__TMS_USBMIB_PORT_ENTRY		USBMIB_PORT_ENTRY,
					*PUSBMIB_PORT_ENTRY;
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/*
|| The enumeration types
*/

/* the port type */
#define	__TMS_USBMIB_usbPortType_Host	1
#define	__TMS_USBMIB_usbPortType_Device	2
#define	__TMS_USBMIB_usbPortType_Hub	3

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBMIB_usbPortType_Host	__TMS_USBMIB_usbPortType_Host
# define USBMIB_usbPortType_Device	__TMS_USBMIB_usbPortType_Device
# define USBMIB_usbPortType_Hub		__TMS_USBMIB_usbPortType_Hub
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/* the port rate */
#define	__TMS_USBMIB_usbPortRate_LowSpeed	1
#define	__TMS_USBMIB_usbPortRate_FullSpeed	2
#define	__TMS_USBMIB_usbPortRate_HighSpeed	3

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBMIB_usbPortRate_LowSpeed	__TMS_USBMIB_usbPortRate_LowSpeed
# define USBMIB_usbPortRate_FullSpeed	__TMS_USBMIB_usbPortRate_FullSpeed
# define USBMIB_usbPortRate_HighSpeed	__TMS_USBMIB_usbPortRate_HighSpeed
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/****************************************************************************\
|
|	The USBMIB_DEVICE_ENTRY corresponds to the usbDeviceEntry object.
|
\****************************************************************************/

typedef struct __TMS_USBMIB_DEVICE_ENTRY_20000301
	{
	unsigned long	usbDeviceIndex;
	/* XXX devices don't have or not have power attributes -- this is config related */
	unsigned	usbDevicePower;
	unsigned short	usbDeviceVendorID;
	unsigned short	usbDeviceProductID;
	unsigned int	usbDeviceNumberConfigurations;
	/* XXX devices don't have active classes! */
	unsigned int	usbDeviceActiveClass;
	unsigned int	usbDeviceStatus;
	unsigned long	usbDeviceEnumCounter;
	/* XXX devices don't support or not support remote wakeup */
	unsigned int	usbDeviceRemoteWakeup;
	unsigned int	usbDeviceRemoteWakeupOn;
	} __TMS_USBMIB_DEVICE_ENTRY_20000301,
	 *__TMS_PUSBMIB_DEVICE_ENTRY_20000301;

typedef __TMS_USBMIB_DEVICE_ENTRY_20000301	__TMS_USBMIB_DEVICE_ENTRY,
						*__TMS_PUSBMIB_DEVICE_ENTRY;

#if !__TMS_CLOAKED_NAMES_ONLY
typedef	__TMS_USBMIB_DEVICE_ENTRY_20000301	USBMIB_DEVICE_ENTRY_20000301,
						*PUSBMIB_DEVICE_ENTRY_20000301;
typedef	__TMS_USBMIB_DEVICE_ENTRY		USBMIB_DEVICE_ENTRY,
						*PUSBMIB_DEVICE_ENTRY;
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/* 
|| the enumeration types 
*/

/* usbDevicePower */
/* XXX this is not right, because a device can be both -- should be a bit map */
#define	__TMS_USBMIB_usbDevicePower_Unknown	1
#define	__TMS_USBMIB_usbDevicePower_SelfPowered	2
#define	__TMS_USBMIB_usbDevicePower_BusPowered	3

#if !__TMS_CLOAKED_NAMES_ONLY
#define	USBMIB_usbDevicePower_Unknown		__TMS_USBMIB_usbDevicePower_Unknown
#define	USBMIB_usbDevicePower_SelfPowered	__TMS_USBMIB_usbDevicePower_SelfPowered
#define	USBMIB_usbDevicePower_BusPowered	__TMS_USBMIB_usbDevicePower_BusPowered
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/* usbDeviceActiveClass XXX this should be the actual class! */
#define	__TMS_USBMIB_usbDeviceActiveClass_Other	1
#define	__TMS_USBMIB_usbDeviceActiveClass_Cdc	2

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBMIB_usbDeviceActiveClass_Other	__TMS_USBMIB_usbDeviceActiveClass_Other
# define USBMIB_usbDeviceActiveClass_Cdc	__TMS_USBMIB_usbDeviceActiveClass_Cdc
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/* usbDeviceStatus */
#define	__TMS_USBMIB_usbDeviceStatus_Unattached	1
#define	__TMS_USBMIB_usbDeviceStatus_Attached	2
#define	__TMS_USBMIB_usbDeviceStatus_Powered	3
#define	__TMS_USBMIB_usbDeviceStatus_Default	4
#define	__TMS_USBMIB_usbDeviceStatus_Address	5
#define	__TMS_USBMIB_usbDeviceStatus_Configured	6
#define	__TMS_USBMIB_usbDeviceStatus_Suspended	7

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBMIB_usbDeviceStatus_Unattached	__TMS_USBMIB_usbDeviceStatus_Unattached
# define USBMIB_usbDeviceStatus_Attached	__TMS_USBMIB_usbDeviceStatus_Attached
# define USBMIB_usbDeviceStatus_Powered		__TMS_USBMIB_usbDeviceStatus_Powered
# define USBMIB_usbDeviceStatus_Default		__TMS_USBMIB_usbDeviceStatus_Default
# define USBMIB_usbDeviceStatus_Address		__TMS_USBMIB_usbDeviceStatus_Address
# define USBMIB_usbDeviceStatus_Configured	__TMS_USBMIB_usbDeviceStatus_Configured
# define USBMIB_usbDeviceStatus_Suspended	__TMS_USBMIB_usbDeviceStatus_Suspended
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/* usbDeviceRemoteWakeup */
#define __TMS_USBMIB_usbDeviceRemoteWakeup_Supported	1
#define __TMS_USBMIB_usbDeviceRemoteWakeup_Unsupported	2

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBMIB_usbDeviceRemoteWakeup_Supported		__TMS_USBMIB_usbDeviceRemoteWakeup_Supported
# define USBMIB_usbDeviceRemoteWakeup_Unsupported	__TMS_USBMIB_usbDeviceRemoteWakeup_Unsupported
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/* usbDeviceRemoteWakeupOn */
#define __TMS_USBMIB_usbDeviceRemoteWakeupOn_TRUE	1
#define __TMS_USBMIB_usbDeviceRemoteWakeupOn_FALSE	2

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBMIB_usbDeviceRemoteWakeupOn_TRUE	__TMS_USBMIB_usbDeviceRemoteWakeupOn_TRUE
# define USBMIB_usbDeviceRemoteWakeupOn_FALSE	__TMS_USBMIB_usbDeviceRemoteWakeupOn_FALSE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/****************************************************************************\
|
|	USBMIB_CDC_ENTRY
|
\****************************************************************************/

typedef struct __TMS_USBMIB_CDC_ENTRY_20000301
	{
	unsigned int	usbCDCIndex;
	unsigned int	usbCDCIfIndex;
	/* XXX this should be an octet; and the CDCProtocol should also be represented */
	unsigned int	usbCDCSubclass;
	unsigned short	usbCDCVersion;
	/* XXX there's one transfer type for each direction */
	unsigned int	usbCDCDataTransferType;
	unsigned int	usbCDCDataEndpoints;
	unsigned long	usbCDCStalls;
	} __TMS_USBMIB_CDC_ENTRY_20000301,
	  *__TMS_PUSBMIB_CDC_ENTRY_20000301;

typedef __TMS_USBMIB_CDC_ENTRY_20000301	__TMS_USBMIB_CDC_ENTRY,
					*__TMS_PUSBMIB_CDC_ENTRY;

#if !__TMS_CLOAKED_NAMES_ONLY
typedef	__TMS_USBMIB_CDC_ENTRY_20000301	USBMIB_CDC_ENTRY_20000301,
					*PUSBMIB_CDC_ENTRY_20000301;
typedef	__TMS_USBMIB_CDC_ENTRY		USBMIB_CDC_ENTRY,
					*PUSBMIB_CDC_ENTRY;
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/* 
|| the enumeration types 
*/

/* 
|| here are the legal values for the usbCDCsubclass: note that
|| vendor specific has to map to 'other'!
*/
#define	__TMS_USBMIB_usbCDCSubclass_Other		0
#define	__TMS_USBMIB_usbCDCSubclass_DirectLine		1
#define	__TMS_USBMIB_usbCDCSubclass_Acm			2
#define	__TMS_USBMIB_usbCDCSubclass_Telephony		3
#define	__TMS_USBMIB_usbCDCSubclass_Multichannel	4
#define	__TMS_USBMIB_usbCDCSubclass_Capi		5
#define	__TMS_USBMIB_usbCDCSubclass_Ethernet		6
#define	__TMS_USBMIB_usbCDCSubclass_Atm			7

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBMIB_usbCDCSubclass_Other		__TMS_USBMIB_usbCDCSubclass_Other
# define USBMIB_usbCDCSubclass_DirectLine	__TMS_USBMIB_usbCDCSubclass_DirectLine
# define USBMIB_usbCDCSubclass_Acm		__TMS_USBMIB_usbCDCSubclass_Acm
# define USBMIB_usbCDCSubclass_Telephony	__TMS_USBMIB_usbCDCSubclass_Telephony
# define USBMIB_usbCDCSubclass_Multichannel	__TMS_USBMIB_usbCDCSubclass_Multichannel
# define USBMIB_usbCDCSubclass_Capi		__TMS_USBMIB_usbCDCSubclass_Capi
# define USBMIB_usbCDCSubclass_Ethernet		__TMS_USBMIB_usbCDCSubclass_Ethernet
# define USBMIB_usbCDCSubclass_Atm		__TMS_USBMIB_usbCDCSubclass_Atm
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*** data transfer type */
#define	__TMS_USBMIB_usbCDCDataTransferType_Synchronous		1
#define	__TMS_USBMIB_usbCDCDataTransferType_Asynchronous	2

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBMIB_usbCDCDataTransferType_Synchronous	__TMS_USBMIB_usbCDCDataTransferType_Synchronous
# define USBMIB_usbCDCDataTransferType_Asynchronous	__TMS_USBMIB_usbCDCDataTransferType_Asynchronous
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/****************************************************************************\
|
|	USBMIB_CDCETHER_ENTRY
|
\****************************************************************************/

typedef struct __TMS_USBMIB_CDCETHER_ENTRY_20000301
	{
	unsigned int	usbCDCEtherIfIndex;
	unsigned char	usbCDCEtherMacAddress[6];
	unsigned int	usbCDCEtherPacketFilter;
	unsigned int	usbCDCEtherDataStatisticsCapabilities;
	/* XXX this statistic doesn't make sense */
	unsigned int	usbCDCEtherDataCheckErrors;
	} __TMS_USBMIB_CDCETHER_ENTRY_20000301,
	 *__TMS_PUSBMIB_CDCETHER_ENTRY_20000301;

typedef __TMS_USBMIB_CDCETHER_ENTRY_20000301	__TMS_USBMIB_CDCETHER_ENTRY,
						*__TMS_PUSBMIB_CDCETHER_ENTRY;

#if !__TMS_CLOAKED_NAMES_ONLY
typedef	__TMS_USBMIB_CDCETHER_ENTRY_20000301	USBMIB_CDCETHER_ENTRY_20000301,
						*PUSBMIB_CDCETHER_ENTRY_20000301;
typedef	__TMS_USBMIB_CDCETHER_ENTRY		USBMIB_CDCETHER_ENTRY,
						*PUSBMIB_CDCETHER_ENTRY;
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/* 
|| the enumeration types 
*/

/* the packet filter bits */
#define	__TMS_USBMIB_usbCDCEtherPacketFilter_packetPromiscuous	0x00000001ul
#define	__TMS_USBMIB_usbCDCEtherPacketFilter_packetAllMulticast	0x00000002ul
#define	__TMS_USBMIB_usbCDCEtherPacketFilter_packetDirected	0x00000004ul
#define	__TMS_USBMIB_usbCDCEtherPacketFilter_packetBroadcast	0x00000008ul
#define	__TMS_USBMIB_usbCDCEtherPacketFilter_packetMulticast	0x00000010ul

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBMIB_usbCDCEtherPacketFilter_packetPromiscuous	__TMS_USBMIB_usbCDCEtherPacketFilter_packetPromiscuous
# define USBMIB_usbCDCEtherPacketFilter_packetAllMulticast	__TMS_USBMIB_usbCDCEtherPacketFilter_packetAllMulticast
# define USBMIB_usbCDCEtherPacketFilter_packetDirected	__TMS_USBMIB_usbCDCEtherPacketFilter_packetDirected
# define USBMIB_usbCDCEtherPacketFilter_packetBroadcast	__TMS_USBMIB_usbCDCEtherPacketFilter_packetBroadcast
# define USBMIB_usbCDCEtherPacketFilter_packetMulticast	__TMS_USBMIB_usbCDCEtherPacketFilter_packetMulticast
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/* the statistics capabilities */
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameXmitOk	0x00000001ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameRcvOk	0x00000002ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameXmitErr	0x00000004ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameRcvErr	0x00000008ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_RcvNoBuff	0x00000010ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesXmitDirectOk	0x00000020ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesXmitDirectOk	0x00000040ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytexXmitMulticastOk	0x00000080ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesXmitMulticastOk	0x00000100ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesXmitBroadcastOk	0x00000200ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesXmitBroadcastOk	0x00000400ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesRcvDirectOk	0x00000800ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvDirectOk	0x00001000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesRcvMulticastOk	0x00002000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvMulticastOk	0x00004000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesRcvBroadcastOk	0x00008000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvBroadcastOk	0x00010000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvCrcErr	0x00020000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitQueueLen		0x00040000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_RcvErrAlignment	0x00080000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitOneCollision	0x00100000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitMoreCollisions	0x00200000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitDeferred		0x00400000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitMaxCollision	0x00800000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_RcvOverrun		0x01000000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitUnderrun		0x02000000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitHeartbeatFailure	0x04000000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitTimesCrsLost	0x08000000ul
#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitLateCollisions	0x10000000ul

#define	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_ALL			0x1FFFFFFFul

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameXmitOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameXmitOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameRcvOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameRcvOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameXmitErr	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameXmitErr
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameRcvErr	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FrameRcvErr
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_RcvNoBuff	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_RcvNoBuff
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesXmitDirectOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesXmitDirectOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesXmitDirectOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesXmitDirectOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_BytexXmitMulticastOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytexXmitMulticastOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesXmitMulticastOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesXmitMulticastOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesXmitBroadcastOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesXmitBroadcastOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesXmitBroadcastOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesXmitBroadcastOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesRcvDirectOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesRcvDirectOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvDirectOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvDirectOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesRcvMulticastOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesRcvMulticastOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvMulticastOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvMulticastOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesRcvBroadcastOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_BytesRcvBroadcastOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvBroadcastOk	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvBroadcastOk
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvCrcErr	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_FramesRcvCrcErr
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitQueueLen	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitQueueLen
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_RcvErrAlignment	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_RcvErrAlignment
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitOneCollision	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitOneCollision
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitMoreCollisions	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitMoreCollisions
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitDeferred	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitDeferred
#define	USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitMaxCollision	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitMaxCollision
#define	USBMIB_usbCDCEtherDataStatisticsCapabilities_RcvOverrun	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_RcvOverrun
#define	USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitUnderrun	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitUnderrun
#define	USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitHeartbeatFailure	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitHeartbeatFailure
#define	USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitTimesCrsLost	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitTimesCrsLost
#define	USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitLateCollisions	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_XmitLateCollisions
# define USBMIB_usbCDCEtherDataStatisticsCapabilities_ALL	__TMS_USBMIB_usbCDCEtherDataStatisticsCapabilities_ALL
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/****************************************************************************\
|
|	Although MCCI doesn't implement transmit multicast filters in
|	any of our code, we define the following object in case it's
|	needed.
|
\****************************************************************************/

typedef struct __TMS_USBMIB_CDCETHERXMITADDRESS_ENTRY_20000301
	{
	unsigned char	usbCDCEtherMacAddress[6];
	} __TMS_USBMIB_CDCETHERXMITADDRESS_ENTRY_20000301,
	 *__TMS_PUSBMIB_CDCETHERXMITADDRESS_ENTRY_20000301;

typedef __TMS_USBMIB_CDCETHERXMITADDRESS_ENTRY_20000301	__TMS_USBMIB_CDCETHERXMITADDRESS_ENTRY,
							*__TMS_PUSBMIB_CDCETHERXMITADDRESS_ENTRY;

#if !__TMS_CLOAKED_NAMES_ONLY
typedef	__TMS_USBMIB_CDCETHERXMITADDRESS_ENTRY_20000301	USBMIB_CDCETHERXMITADDRESS_ENTRY_20000301,
							*PUSBMIB_CDCETHERXMITADDRESS_ENTRY_20000301;
typedef	__TMS_USBMIB_CDCETHERXMITADDRESS_ENTRY		USBMIB_CDCETHERXMITADDRESS_ENTRY,
							*PUSBMIB_CDCETHERXMITADDRESS_ENTRY;
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/**** end of usbmib.h ****/
#endif /* _USBMIB_H_ */
