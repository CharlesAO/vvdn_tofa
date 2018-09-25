/* usbpump_usbdi_port_api.h	Fri Apr 22 2016 17:07:27 chwon */

/*

Module:  usbpump_usbdi_port_api.h

Function:
	API function for usbdi port method functions.

Version:
	V3.17a	Fri Apr 22 2016 17:07:28 chwon	Edit level 15

Copyright notice:
	This file copyright (C) 2010-2013, 2015-2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChiaChu Cho, MCCI Corporation	June 2010

Revision history:
   3.01c  Tue Jun 22 2010 14:13:09 cccho
	10391: Module created.

   3.01d  Tue Jan 18 2011 11:33:39  cccho
	12222: adjsut USBPUMP_USBDI_PORT_CONNECTION_INFORMATION by removing
	redundant fields. add UsbPumpUsbdFunction_GetHubPortStatus() prototype.

   3.01f  Tue Jul 05 2011 14:28:37  chwon
	13723: add UsbPumpUsbdPort_GetPortState() prototype.

   3.01f  Mon Jun 18 2012 14:30:59  cccho
	15552: add UsbPumpUsbdPort_GetHubPortNumber() prototype.

   3.11b  Thu Sep 20 2012 19:09:43  cccho
	15952: add UsbPumpUsbdPort_GetHubInformation() and
	UsbPumpUsbdPort_GetHubStatusChangeEndpointDescriptor() prototype.

   3.11b  Tue Sep 25 2012 10:48:39  cccho
	15952: add SuperBuffer in USBPUMP_USBDI_HUB_INFORMATION_DESC_UNION for
	UHIL_cpybuf operation in UsbPumpUsbdPort_GetHubInformation().

   3.11b  Thu Oct 04 2012 17:47:04  cccho
	15952: pass the size of output buffer for validation.

   3.11b  Thu Oct 18 2012 11:33:54  luxw
	15819: add UsbPumpUsbdPort_ReenumeratePort() prototype.

   3.11c  Wed Apr 10 2013 12:05:10  chwon
	16985: 17015: add UsbPumpUsbdPort_GetDeviceDescBuffer(),
	UsbPumpUsbdPort_SendDeviceRequest() and
	UsbPumpUsbdPort_StartVendorHnp() prototype.

   3.11c  Thu Apr 25 2013 12:18:09  chwon
	16985: 17015: add UsbPumpUsbdPort_GetHnpCapableStatus() prototype.

   3.11c  Thu May 02 2013 21:27:00  chwon
	17069: add UsbPumpUsbdPort_GetHcdDevice() prototype.

   3.11c  Wed Jun 05 2013 09:57:53  chwon
	17320: add UsbPumpUsbdPort_SetPortPower() prototype.

   3.13a  Wed Jul 02 2014 13:29:57  chwon
	18367: add prototype for UsbPumpUsbdPort_GetParentPort(),
	UsbPumpUsbdHub_GetHubConfigInfo(), UsbPumpUsbdHub_ProcessHubRequest().

   3.15c  Wed Aug 19 2015 13:43:28  chwon
	19395: Add UsbPumpUsbdPort_GetRouteInfo().

   3.15d  Wed Oct 14 2015 17:07:17  chwon
	19562: Add UsbPumpUsbdPort_ResetPort() prototype.

   3.17a  Fri Apr 22 2016 17:07:28  chwon
	20067: Add UsbPumpUsbdPort_GetChildHubPort() prototype.

*/

#ifndef _USBPUMP_USBDI_PORT_API_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_PORT_API_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

#ifndef _USBPUMP_HCD_TYPES_H_
# include "usbpump_hcd_types.h"
#endif

#ifndef _USTAT_H_
# include "ustat.h"
#endif

#ifndef _USBHUB30_H_
# include "usbhub30.h"
#endif

#ifndef _USETUP_H_
# include "usetup.h"
#endif

/****************************************************************************\
|
|	USBPUMP_USBDI_HUB_CAPABILITIES
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_HUB_CAPABILITIES);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_HUB_CAPABILITIES)
	{
	/*
	|| fHubIsHighSpeedCapable indicates the hub is high speed–capable.
	|| This capability does not necessarily mean that the hub is operating
	|| at high speed.
	*/
	__TMS_UINT	fHubIsHighSpeedCapable: 1;

	/*
	|| fHubIsHighSpeed indicates the hub is high speed.
	*/
	__TMS_UINT	fHubIsHighSpeed: 1;

	/*
	|| fHubIsMultiTtCapable indicates the hub is capable of doing multiple
	|| transaction translations simultaneously
	*/
	__TMS_UINT	fHubIsMultiTtCapable: 1;

	/*
	|| fHubIsMultiTt indicates the hub is configured to perform multiple
	|| transaction translations simultaneously.
	*/
	__TMS_UINT	fHubIsMultiTt: 1;

	/*
	|| fHubIsRoot indicates the hub is the root hub.
	*/
	__TMS_UINT	fHubIsRoot: 1;

	/*
	|| fHubIsArmedWakeOnConnect indicates the hub is armed to wake when a
	|| device is connected to the hub
	*/
	__TMS_UINT	fHubIsArmedWakeOnConnect: 1;

	/*
	|| fHubIsUsb3 indicates whether the hub is connected by USB3, and
	|| would be true even on the high-speed instance.
	*/
	__TMS_UINT	fHubIsUsb3: 1;

	/*
	|| fHubIsSuperSpeed indicates the hub is actually in superspeed mode.
	*/
	__TMS_UINT	fHubIsSuperSpeed: 1;

	/* fHubIsWireless indicates the hub is a wireless USB root hub */
	__TMS_UINT	fHubIsWireless: 1;

	/* fLPM indicates the hub supports LPM */
	__TMS_UINT	fHubIsLpmCapable: 1;

	/*
	|| fHubIsBusPowered indicateds whether the hub is bus-powered.
	|| TURE, the hub is bhus-powered; FALSE, the hub is self-powered.
	*/
	__TMS_UINT	fHubIsBusPowered: 1;
	};

/****************************************************************************\
|
|	USBPUMP_USBDI_HUB_INFORMATION
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_HUB_INFORMATION_DESC_UNION);
union __TMS_UNIONNAME	(USBPUMP_USBDI_HUB_INFORMATION_DESC_UNION)
	{
	/*
	|| Viewed as hub descriptor:
	*/
	USBIF_HUBDESC_WIRE		HubDescWire;
	USBIF_SUPERSPEED_HUBDESC_WIRE	SuperSpeedHubDescrWire;

	/*
	|| Viewed as a buffer
	||
	|| Windows reserves room for 255 ports power control and removable
	|| bitmask
	*/
	__TMS_UINT8			Buffer[LENGTHOF_USBIF_HUBDESC_WIRE(255)];
	__TMS_UINT8			SuperBuffer[LENGTHOF_USBIF_SUPERSPEED_HUBDESC_WIRE];
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_HUB_INFORMATION);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_HUB_INFORMATION)
	{
	__TMS_UINT	fHubIsBusPowered: 1;
	__TMS_UINT	fHubIsSuperSpeed: 1;
	__TMS_USBPUMP_USBDI_HUB_INFORMATION_DESC_UNION	HubDescriptorUnion;
	};

/****************************************************************************\
|
|	USBPUMP_USBDI_USB_CONNECTION_STATUS
|
\****************************************************************************/

__TMS_TYPE_DEF_ARG(USBPUMP_USBDI_USB_CONNECTION_STATUS, UINT32);

/* Indicates that there is no device connected to the port */
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_NO_DEVICE_CONNECTED	0

/* Indicates that a device was successfully connected to the port. */
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_CONNECTED		1

/*
|| Indicates that an attempt was made to connect a device to the port, but the
|| enumeration of the device failed.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_FAILED_ENUMERATION	2

/*
|| Indicates that an attempt was made to connect a device to the port, but the
|| connection failed for unspecified reasons
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_GENERAL_FAILURE	3

/*
|| Indicates that an attempt was made to connect a device to the port, but
|| there was not enough power to drive the device, and the connection failed.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_CAUSED_OVERCURRENT	4

/*
|| Indicates that an attempt was made to connect a device to the port, but
|| there was not enough power to drive the device, and the connection failed.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_NOT_ENOUGH_POWER	5

/*
|| Indicates that an attempt was made to connect a device to the port, but
|| there was not enough bandwidth available for the device to function
|| properly, and the connection failed.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_NOT_ENOUGHT_BANDWIDTH 6

/*
|| Indicates that an attempt was made to connect a device to the port, but the
|| nesting of USB hubs was too deep, so the connection failed.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_HUB_NESTED_TOO_DEEPLY 7

/*
|| Indicates that an attempt was made to connect a device to the port of an
|| unsupported legacy hub, and the connection failed.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_IN_LEGACY_HUB	8

/*
|| USB 2.0: 7.1.20
|| Indicate a port transceiver must enter the high-speed J state and remain
|| in that state until the exit action is taken.
||
|| USB OTG and EH 2.0
|| Upon enumerating VID 0x1A0A/PID 0x0102, the host’s downstream port shall
|| enter a high-speed J state as described in Section 7.1.20 of [USB2.0] until
|| the host controller is reset.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_TEST_J			9

/*
|| USB 2.0: 7.1.20
|| Indicates a port transceiver must enter the high-speed K state and remain
|| in that state until the exit action is taken.
||
|| USB OTG and EH 2.0
|| Upon enumerating VID 0x1A0A/PID 0x0103, the host’s downstream port shall
|| enter a high-speed K state as described in Section 7.1.20 of [USB2.0] until
|| the host controller is reset.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_TEST_K			10

/*
|| USB 2.0: 7.1.20
|| Indicates a port transceiver must enter the high-speed receive mode
|| and remain in that mode until the exit action is taken.
||
|| USB OTG and EH 2.0
|| Upon enumerating VID 0x1A0A/PID 0x0101, the host’s downstream port shall
|| enter a high-speed receive mode as described in Section 7.1.20 [USB2.0] and
|| drives an SE0 until the controller is reset.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_TEST_SE0_NAK		11

/*
|| USB 2.0: 7.1.20
|| Indicates a port must repetitively transmit the following test packet until
|| the exit action is taken.
||
|| USB OTG and EH 2.0
|| Upon enumerating VID 0x1A0A/PID 0x0104, the host shall begin sending test
|| packets as described in Section 7.1.20 of [USB2.0] until the host
|| controller is reset.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_TEST_PACKET		12

/*
|| USB 2.0: 7.1.20
|| Indicates downstream facing hub ports (and only downstream
|| facing hub ports) must be enabled in high-speed mode, even if there is no
|| device attached.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_TEST_FORCE_ENABLE		13

/*
|| USB OTG and EH 2.0
|| Upon enumerating VID:0x1A0A/PID 0x0106, the host shall continue sending
|| SOFs for 15 seconds, then suspend the downstream port under test per
|| Section 7.1.7.6.1 of [USB2.0]. After 15 seconds has elapsed, the host shall
|| issue a ResumeK state on the bus, then continue sending SOFs.
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_HS_HOST_PORT_SUSPEND_RESUME 14

/*
|| USB OTG and EH 2.0
|| When the host discovers a device with VID:0x1A0A/PID 0x0107, the following
|| steps are executed by the host and the device.
||
||	1. The host enumerates the test device, reads VID:0x1A0A/PID 0x0107,
||	then completes its enumeration procedure.
||	2. The host issues SOFs for 15 seconds allowing the test engineer to
||	raise the scope trigger just above the SOF voltage level.
||	3. The host sends a complete GetDescriptor(Device) transfer
||	4. The device ACKs the request, triggering the scope.
||	(Note: SOFs continue.)
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_SINGLE_STEP_GET_DEV_DESC	15

/*
|| USB OTG and EH 2.0
|| When the host discovers a device with VID:0x1A0A/PID 0x0108, the following
|| steps are executed by the host and the device.
||
||	1. The host enumerates the test device and reads VID:0x1A0A/PID 0x0108,
||	then completes its enumeration procedure
||	2. After enumerating the device, the host sends GetDescriptor(Device)
||	3. The device ACKs the request
||	4. The host issues SOFs for 15 seconds allowing the test engineer to
||	raise the scope trigger just above the SOF voltage level
||	5. The host sends an IN packet
||	6. The device sends data in response to the IN packet, triggering the
||	scope
||	7. The host sends an ACK in response to the data. (Note: SOFs may
||	follow the IN transaction).
*/
#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS_SINGLE_STEP_GET_DEV_DESC_DATA 16


#define	__TMS_USBPUMP_USBDI_CONNECTION_STATUS__INIT	\
	"NO_DEVICE_CONNECTED",				\
	"DEVICE_CONNECTED",				\
	"DEVICE_FAILED_ENUMERATION",			\
	"DEVICE_GENERAL_FAILURE",			\
	"DEVICE_CAUSED_OVERCURRENT",			\
	"DEVICE_NOT_ENOUGH_POWER",			\
	"DEVICE_NOT_ENOUGHT_BANDWIDTH",			\
	"DEVICE_HUB_NESTED_TOO_DEEPLY",			\
	"DEVICE_IN_LEGACY_HUB"				\
	"TEST_J",					\
	"TEST_K",					\
	"TEST_SE0_NAK",					\
	"TEST_PACKET",					\
	"TEST_FORCE_ENABLE",				\
	"HS_HOST_PORT_SUSPEND_RESUME",			\
	"SINGLE_STEP_GET_DEV_DESC",			\
	"SINGLE_STEP_GET_DEV_DESC_DATA"


/****************************************************************************\
|
|	USBPUMP_USBDI_PORT_CONNECTION_INFORMATION
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_PORT_CONNECTION_INFORMATION);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_PORT_CONNECTION_INFORMATION)
	{
	__TMS_USBPUMP_USBDI_PORT *			pPort;

	__TMS_USBIF_DEVDESC_WIRE *			pDeviceDescriptor;
	__TMS_USBPUMP_USBDI_USB_CONNECTION_STATUS	ConnectionStatus;
	__TMS_UINT8					ucCurrentConfig;
	__TMS_UINT					fLPM: 1;
	};

#define	__TMS_USBPUMP_USBDI_PORT_CONNECTION_INFORMATION_SETUP_V1(	\
		a_pPortConnectionInfo,					\
		a_pPort,						\
		a_pDeviceDescriptor					\
		)							\
    do	{								\
	(a_pPortConnectionInfo)->pPort = (a_pPort);			\
	(a_pPortConnectionInfo)->pDeviceDescriptor = (a_pDeviceDescriptor); \
	(a_pPortConnectionInfo)->ConnectionStatus =			\
		__TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_CONNECTED;	\
	(a_pPortConnectionInfo)->ucCurrentConfig = 0;			\
	(a_pPortConnectionInfo)->fLPM = FALSE;				\
	} while (0)


/****************************************************************************\
|
|	USBPUMP_USBDI_PORT_STATE
|
\****************************************************************************/

__TMS_TYPE_DEF_ARG(USBPUMP_USBDI_PORT_STATE, UINT32);

#define	__TMS_USBPUMP_USBDI_PORT_STATE_CONNECTED	(1u << 0)
#define	__TMS_USBPUMP_USBDI_PORT_STATE_RUNNING		(1u << 1)
#define	__TMS_USBPUMP_USBDI_PORT_STATE_SUSPENDED	(1u << 2)


/****************************************************************************\
|
|	USBPUMP_USBDI_PORT_ROUTE_INFO
|
\****************************************************************************/

__TMS_TYPE_DEF_ARG(USBPUMP_USBDI_PORT_ROUTE_INFO, UINT32);

#define	__TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_ROOTPORT_MASK	0x0F000000u
#define	__TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_TIER_MASK		0x00700000u
#define	__TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_ROUTE_MASK		0x000FFFFFu

#define	__TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_GET_ROOTPORT(PortRouteInfo)	\
	(((PortRouteInfo) >> 24) & 15)
#define	__TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_SET_ROOTPORT(PortRouteInfo, Port) \
  do {	(PortRouteInfo) |= ((Port) & 15) << 24; } while (0)

#define	__TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_GET_TIER(PortRouteInfo)	\
	(((PortRouteInfo) >> 20) & 7)
#define	__TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_SET_TIER(PortRouteInfo, Tier) \
  do {	(PortRouteInfo) |= ((Tier) & 7) << 20; } while (0)

#define	__TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_GET_ROUTE(PortRouteInfo)	\
	((PortRouteInfo) & 0x000FFFFFu)
#define	__TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_SET_ROUTE(PortRouteInfo, Route) \
  do {	(PortRouteInfo) |= (Route) & 0x000FFFFFu; } while (0)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_USBDI_PORT *
UsbPumpUsbdPort_GetChildHubPort(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */,
	__TMS_UINT			/* PortNumber */
	);

__TMS_CONST __TMS_USBIF_DEVDESC_WIRE *
UsbPumpUsbdPort_GetDeviceDescBuffer(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */
	);

__TMS_USBPUMP_HCD_DEVICE *
UsbPumpUsbdPort_GetHcdDevice(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */
	);

__TMS_BOOL
UsbPumpUsbdPort_GetHnpCapableStatus(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */
	);

__TMS_USTAT
UsbPumpUsbdPort_GetHubCapabilities(
	__TMS_USBPUMP_USBDI_PORT *		/* pPort */,
	__TMS_USBPUMP_USBDI_HUB_CAPABILITIES *	/* pHubCapabilities */,
	__TMS_BYTES				/* szHubCapabilities */
	);

__TMS_BOOL
UsbPumpUsbdHub_GetHubConfigInfo(
	__TMS_USBPUMP_USBDI_FUNCTION *		/* pFunction */,
	__TMS_BOOL				/* fGetConfigDesc */,
	__TMS_VOID **				/* ppConfigDesc */,
	__TMS_BYTES *				/* pnConfigDesc */
	);

__TMS_USTAT
UsbPumpUsbdPort_GetHubInformation(
	__TMS_USBPUMP_USBDI_PORT *		/* pPort */,
	__TMS_USBPUMP_USBDI_HUB_INFORMATION *	/* pHubInformation */,
	__TMS_BYTES				/* szHubInformation */
	);

__TMS_UINT
UsbPumpUsbdPort_GetHubPortNumber(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */
	);

__TMS_USTAT
UsbPumpUsbdPort_GetHubStatusChangeEndpointDescriptor(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */,
	__TMS_USBIF_EPDESC *		/* pHubIntEpDesc */,
	__TMS_BYTES			/* szHubIntEpDesc */
	);

__TMS_USBPUMP_USBDI_PORT *
UsbPumpUsbdPort_GetParentPort(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */
	);

__TMS_USTAT
UsbPumpUsbdPort_GetPortConnectionInformation(
	__TMS_USBPUMP_USBDI_PORT *				/* pPort */,
	__TMS_USBPUMP_USBDI_PORT_CONNECTION_INFORMATION *	/* pNodeConnectionInfo */
	);

__TMS_USBPUMP_USBDI_PORT_STATE
UsbPumpUsbdPort_GetPortState(
	__TMS_USBPUMP_USBDI_PORT *		/* pPort */
	);

__TMS_USBPUMP_USBDI_PORT_ROUTE_INFO
UsbPumpUsbdPort_GetRouteInfo(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */
	);

__TMS_BOOL
UsbPumpUsbdFunction_GetHubPortStatus(
	__TMS_USBPUMP_USBDI_FUNCTION *			/* pFunction */,
	__TMS_UINT32					/* ulHubPortNumber */,
	__TMS_USBPUMP_USBDI_USB_CONNECTION_STATUS *	/* pConnectionStatus */
	);

__TMS_BOOL
UsbPumpUsbdHub_ProcessHubRequest(
	__TMS_USBPUMP_USBDI_FUNCTION *	/* pFunction */,
	__TMS_USETUP_WIRE *		/* pSetup */,
	__TMS_VOID *			/* pBuffer */,
	__TMS_BYTES			/* nBuffer */,
	__TMS_BYTES *			/* pnActual */
	);

__TMS_VOID
UsbPumpUsbdPort_ReenumeratePort(
	__TMS_USBPUMP_USBDI_PORT *		/* pPort */
	);

__TMS_VOID
UsbPumpUsbdPort_ResetPort(
	__TMS_USBPUMP_USBDI_PORT *		/* pPort */
	);

__TMS_BOOL
UsbPumpUsbdPort_SendDeviceRequest(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */,
	__TMS_UINT8			/* bmReqType */,
	__TMS_UINT8			/* bRequest */,
	__TMS_UINT16			/* wIndex */,
	__TMS_UINT16			/* wValue */,
	__TMS_VOID *			/* pBuffer */,
	__TMS_BYTES			/* wLength */,
	__TMS_USBPUMP_URB_DONE_FN *	/* pDoneFn */,
	__TMS_VOID *			/* pDoneInfo */
	);

__TMS_BOOL
UsbPumpUsbdPort_SetPortPower(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */,
	__TMS_BOOL			/* fPortPowerOn */
	);

__TMS_BOOL
UsbPumpUsbdPort_StartVendorHnp(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */,
	__TMS_BOOL			/* fDoVendorSpecificHnp */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_CONNECTION_STATUS_NO_DEVICE_CONNECTED	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_NO_DEVICE_CONNECTED
# define USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_CONNECTED	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_CONNECTED
# define USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_FAILED_ENUMERATION	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_FAILED_ENUMERATION
# define USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_GENERAL_FAILURE	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_GENERAL_FAILURE
# define USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_CAUSED_OVERCURRENT	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_CAUSED_OVERCURRENT
# define USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_NOT_ENOUGH_POWER	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_NOT_ENOUGH_POWER
# define USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_NOT_ENOUGHT_BANDWIDTH	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_NOT_ENOUGHT_BANDWIDTH
# define USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_HUB_NESTED_TOO_DEEPLY	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_HUB_NESTED_TOO_DEEPLY
# define USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_IN_LEGACY_HUB	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_DEVICE_IN_LEGACY_HUB
# define USBPUMP_USBDI_CONNECTION_STATUS_TEST_J	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_TEST_J
# define USBPUMP_USBDI_CONNECTION_STATUS_TEST_K	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_TEST_K
# define USBPUMP_USBDI_CONNECTION_STATUS_TEST_SE0_NAK	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_TEST_SE0_NAK
# define USBPUMP_USBDI_CONNECTION_STATUS_TEST_PACKET	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_TEST_PACKET
# define USBPUMP_USBDI_CONNECTION_STATUS_TEST_FORCE_ENABLE	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_TEST_FORCE_ENABLE
# define USBPUMP_USBDI_CONNECTION_STATUS_HS_HOST_PORT_SUSPEND_RESUME	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_HS_HOST_PORT_SUSPEND_RESUME
# define USBPUMP_USBDI_CONNECTION_STATUS_SINGLE_STEP_GET_DEV_DESC	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_SINGLE_STEP_GET_DEV_DESC
# define USBPUMP_USBDI_CONNECTION_STATUS_SINGLE_STEP_GET_DEV_DESC_DATA	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS_SINGLE_STEP_GET_DEV_DESC_DATA
# define USBPUMP_USBDI_CONNECTION_STATUS__INIT	\
   __TMS_USBPUMP_USBDI_CONNECTION_STATUS__INIT
# define USBPUMP_USBDI_PORT_CONNECTION_INFORMATION_SETUP_V1(	\
		a_pPortConnectionInfo,					\
		a_pPort,						\
		a_pDeviceDescriptor					\
		)	\
	__TMS_USBPUMP_USBDI_PORT_CONNECTION_INFORMATION_SETUP_V1(	\
		a_pPortConnectionInfo,					\
		a_pPort,						\
		a_pDeviceDescriptor					\
		)
# define USBPUMP_USBDI_PORT_STATE_CONNECTED	\
   __TMS_USBPUMP_USBDI_PORT_STATE_CONNECTED
# define USBPUMP_USBDI_PORT_STATE_RUNNING	\
   __TMS_USBPUMP_USBDI_PORT_STATE_RUNNING
# define USBPUMP_USBDI_PORT_STATE_SUSPENDED	\
   __TMS_USBPUMP_USBDI_PORT_STATE_SUSPENDED
# define USBPUMP_USBDI_PORT_ROUTE_INFO_ROOTPORT_MASK	\
   __TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_ROOTPORT_MASK
# define USBPUMP_USBDI_PORT_ROUTE_INFO_TIER_MASK	\
   __TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_TIER_MASK
# define USBPUMP_USBDI_PORT_ROUTE_INFO_ROUTE_MASK	\
   __TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_ROUTE_MASK
# define USBPUMP_USBDI_PORT_ROUTE_INFO_GET_ROOTPORT(PortRouteInfo)	\
   __TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_GET_ROOTPORT(PortRouteInfo)
# define USBPUMP_USBDI_PORT_ROUTE_INFO_SET_ROOTPORT(PortRouteInfo, Port)	\
   __TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_SET_ROOTPORT(PortRouteInfo, Port)
# define USBPUMP_USBDI_PORT_ROUTE_INFO_GET_TIER(PortRouteInfo)	\
   __TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_GET_TIER(PortRouteInfo)
# define USBPUMP_USBDI_PORT_ROUTE_INFO_SET_TIER(PortRouteInfo, Tier)	\
   __TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_SET_TIER(PortRouteInfo, Tier)
# define USBPUMP_USBDI_PORT_ROUTE_INFO_GET_ROUTE(PortRouteInfo)	\
   __TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_GET_ROUTE(PortRouteInfo)
# define USBPUMP_USBDI_PORT_ROUTE_INFO_SET_ROUTE(PortRouteInfo, Route)	\
   __TMS_USBPUMP_USBDI_PORT_ROUTE_INFO_SET_ROUTE(PortRouteInfo, Route)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_port_api.h ****/
#endif /* _USBPUMP_USBDI_PORT_API_H_ */
