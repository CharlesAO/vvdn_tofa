/* usbhub30.h	Thu Oct 15 2009 13:07:53 chwon */

/*

Module:  usbhub30.h

Function:
	USB Hub Device Class (HUB) Additional V3.0 definitions

Version:
	V2.01a	Thu Oct 15 2009 13:07:53 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Debbie Totsline, MCCI Corporation	February 2009

Revision history:
   2.01a Fri Feb 27 2009 09:04:48 djt
	7416: Module created.

   2.01a  Fri Apr 03 2009 10:12:47  djt
	7416: Added USB_Hub_FEATURE_PORT_LINK_STATE_* and 
	USB_Hub_PORT_STATUS_wStatus_LinkState_*

   2.01a  Thu Oct 15 2009 13:07:53  chwon
	9105: Fixed typo -- USB_Hub_FEATURE_PORT_REMOTE_WAKE_MASK and
	add hub feature name init macro.

*/

#ifndef _USBHUB30_H_		/* prevent multiple includes */
#define _USBHUB30_H_

/* it's more important that the structures be packed... */
#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif

#ifndef _USBHUB20_H_
# include "usbhub20.h"
#endif

/****************************************************************************\
|
|	Protocol code
|
\****************************************************************************/

/**** device protocol code for SuperSpeed ****/
#define USB_bDeviceProto_Hub_SUPERSPEED		3	/* super-speed hub  */

/****************************************************************************\
|
|	Class request codes for 3.0 -- table 10-6
|
\****************************************************************************/

#define USB_bRequest_Hub_SET_HUB_DEPTH		12	/* set hub depth*/
#define USB_bRequest_Hub_GET_PORT_ERR_COUNT	13	/* get num link errs */

/* for debug prints */
#define	USB_bRequest_SuperSpeed_Hub__INIT				\
	{ USB_bRequest_Hub_SET_HUB_DEPTH, "SET_HUB_DEPTH" },		\
	{ USB_bRequest_Hub_GET_PORT_ERR_COUNT, "GET_PORT_ERR_COUNT" }

/****************************************************************************\
|
|	Hub feature selector codes for 3.0 -- table 10-7
|
\****************************************************************************/

/* 
|| Same as 2.0
|| C_HUB_LOCAL_POWER 0
|| C_HUB_OVER_CURRENT 1
*/

/****************************************************************************\
|
|	Additional port feature selector codes for 3.0 -- table 10-7
|
\****************************************************************************/

					/*
					|| Defined in 2.0 and used in 3.0
					|| PORT_CONNECTION 0
					|| PORT_OVER_CURRENT 3 
					|| PORT_RESET 4	
					*/

					/*
					|| When this feature is set, the hub
					|| will transition the link the 
					|| state specified in wIndex most
					|| significant byte (bits 15..8).
					*/
#define USB_Hub_FEATURE_PORT_LINK_STATE	5

					/*
					|| Defined in 2.0 and used in 3.0
					|| PORT_POWER 8
					|| C_PORT_CONNECTION 16
					|| C_PORT_OVER_CURRENT 19
					|| C_PORT_RESET 20
					*/

					/*
					|| When this feature is selected, the 
					|| most significant byte (bits 15..8) of
					|| the wIndex field specifies the 
					|| timeout.
					*/	
#define USB_Hub_FEATURE_PORT_U1_TIMEOUT 23
					/*
					|| When this feature is selected, the 
					|| most significant byte (bits 15..8) of
					|| the wIndex field specifies the 
					|| timeout.
					*/
#define USB_Hub_FEATURE_PORT_U2_TIMEOUT 24

					/*
					|| Set to one when the port's link
					|| completes a transition from the U3 
					|| state to the U0 state as a result of 
					|| SetPortFeature(Port_Link_State) 
					|| request or completes a transition
					|| from any of the U-states to 
					|| SS.Inactive. This bit is not set to 
					|| one due to transitions from U3 to U0
					|| as a result of remote wakeup 
					|| signalling received on a downstream
					|| facing port.
					*/
#define USB_Hub_FEATURE_C_PORT_LINK_STATE 25	
					/*
					|| Set to one if the link connected to
					|| the port could not be successfully
					|| configured, e.g. if two downstream 
					|| only capable ports are connected to 
					|| each other or if the link 
					|| configuration could not be completed.
					|| In addition, the port shall transition
					|| to the DSPORT.Disabled state when this 
					|| occurs.
					*/
#define USB_Hub_FEATURE_C_PORT_CONFIG_ERROR 26
					/*
					|| When this feature is set, the most
					|| significant byte (bits 15..8) of the
					|| wIndex field specifies the conditions 
					|| that would cause the hub to signal
					|| a remote wake event on its upstream
					|| port.
					||
					|| See table 10-14 for wIndex values.
					|| Note that if remote wake notifications
					|| for connect, disconnect, or over
					|| current events are disabled, these
					|| events are still captured and reported
					|| as port status change events after the
					|| host or hub is resumed.
					*/
#define USB_Hub_FEATURE_PORT_REMOTE_WAKE_MASK 27
					/*
					|| When this feature is selected, the 
					|| hub shall initiate a warm reset on 
					|| the port that is identified in wIndex.
					*/
#define USB_Hub_FEATURE_BH_PORT_RESET 28
					/*
					|| Set to one when the port transitions
					|| from DSPORT.Resetting state to the
					|| DSPORT.Enabled state for a Warm Reset
					|| only.
					*/
#define USB_Hub_FEATURE_C_BH_PORT_RESET 29
					/*
					|| When this feature is set, the 
					|| hub shall send Set Link Function LMP
					|| with the Force_LinkPM_Accept bit
					|| asserted. If the port is not in the
					|| Enabled state, the hub shall treat 
					|| this as a functional no-op.
					||
					|| When this feature is cleared, the
					|| hub shall send Set Link Function LMP
					|| with the Force_LinkPM_Accept bit
					|| de-asserted.
					*/
#define USB_Hub_FEATURE_FORCE_LINKPM_ACCEPT 30

					/* max port features for SuperSpeed */
#define USB_Hub_SUPERSPEED_FEATURE_PORT__MAX 30	

/**** an initializer for debug prints ****/
#define	USB_Hub_SUPERSPEED_FEATURE__INIT				\
	{ USB_Hub_FEATURE_PORT_LINK_STATE, "PORT_LINK_STATE" },		\
	{ USB_Hub_FEATURE_PORT_U1_TIMEOUT, "PORT_U1_TIMEOUT" },		\
	{ USB_Hub_FEATURE_PORT_U2_TIMEOUT, "PORT_U2_TIMEOUT" },		\
	{ USB_Hub_FEATURE_C_PORT_LINK_STATE, "C_PORT_LINK_STATE" },	\
	{ USB_Hub_FEATURE_C_PORT_CONFIG_ERROR, "C_PORT_CONFIG_ERROR" },	\
	{ USB_Hub_FEATURE_PORT_REMOTE_WAKE_MASK, "PORT_REMOTE_WAKE_MASK" }, \
	{ USB_Hub_FEATURE_BH_PORT_RESET, "BH_PORT_RESET" },		\
	{ USB_Hub_FEATURE_C_BH_PORT_RESET, "C_BH_PORT_RESET" },		\
	{ USB_Hub_FEATURE_FORCE_LINKPM_ACCEPT, "FORCE_LINKPM_ACCEPT" }


/* in the order from 10.14.2.2 */
#define	USB_Hub_SUPERSPEED_FEATURE_PORT__CLEAR_MASK			\
	((1u << USB_Hub_FEATURE_PORT_POWER) |				\
	 (1u << USB_Hub_FEATURE_C_PORT_CONNECTION) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_RESET) |				\
	 (1u << USB_Hub_FEATURE_C_PORT_OVER_CURRENT) |			\
         (1u << USB_Hub_FEATURE_C_PORT_LINK_STATE) |			\
         (1u << USB_Hub_FEATURE_C_PORT_CONFIG_ERROR) |			\
         (1u << USB_Hub_FEATURE_C_BH_PORT_RESET) |			\
         (1u << USB_Hub_FEATURE_FORCE_LINKPM_ACCEPT))			

/* in the order from 10.14.2.10 */
#define	USB_Hub_SUPERSPEED_FEATURE_PORT__SET_MASK			\
	((1u << USB_Hub_FEATURE_PORT_RESET) |				\
	 (1u << USB_Hub_FEATURE_BH_PORT_RESET) |			\
	 (1u << USB_Hub_FEATURE_PORT_POWER) |				\
	 (1u << USB_Hub_FEATURE_PORT_U1_TIMEOUT) |			\
	 (1u << USB_Hub_FEATURE_PORT_U2_TIMEOUT) |			\
	 (1u << USB_Hub_FEATURE_PORT_LINK_STATE) |			\
	 (1u << USB_Hub_FEATURE_PORT_REMOTE_WAKE_MASK) |		\
	 (1u << USB_Hub_FEATURE_FORCE_LINKPM_ACCEPT))

#define USB_Hub_FEATURE_PORT_LINK_STATE_U0		0x00
#define USB_Hub_FEATURE_PORT_LINK_STATE_U1		0x01
#define USB_Hub_FEATURE_PORT_LINK_STATE_U2		0x02
#define USB_Hub_FEATURE_PORT_LINK_STATE_U3		0x03
#define USB_Hub_FEATURE_PORT_LINK_STATE_SSDisabled	0x04
#define USB_Hub_FEATURE_PORT_LINK_STATE_RxDetect	0x05
#define USB_Hub_FEATURE_PORT_LINK_STATE_SSInactive	0x06
#define USB_Hub_FEATURE_PORT_LINK_STATE_Polling		0x07
#define USB_Hub_FEATURE_PORT_LINK_STATE_Recovery	0x08
#define USB_Hub_FEATURE_PORT_LINK_STATE_HotReset	0x09
#define USB_Hub_FEATURE_PORT_LINK_STATE_Compliance	0x0A
#define USB_Hub_FEATURE_PORT_LINK_STATE_Loopback	0x0B

#define USB_Hub_PORT_LINK_STATE__IS_VALID(State)	\
	((State) <= USB_Hub_FEATURE_PORT_LINK_STATE_Loopback)

#define	USB_Hub_FEATURE_PORT_REMOTE_WAKE_MASK_Conn	0x01
#define	USB_Hub_FEATURE_PORT_REMOTE_WAKE_MASK_Disconn	0x02
#define	USB_Hub_FEATURE_PORT_REMOTE_WAKE_MASK_OC	0x04

/* bit3-7: reserved and must be set to zero */
#define	USB_Hub_PORT_REMOTE_WAKE_MASK__IS_VALID(Mask)	\
	(((Mask) | 7u) == 7u)


/****************************************************************************\
|
|	Descriptors
|
\****************************************************************************/

#define	USB_bDescriptorType_Hub_SUPERSPEED_HUB_DESCRIPTOR	0x2A

/* define the type */
typedef struct USB_Hub_SuperSpeed_HubDescriptor_Wire	
	USBIF_SUPERSPEED_HUBDESC_WIRE, *PUSBIF_SUPERSPEED_HUBDESC_WIRE;

#include "usbpump_pushpack1.h"

__TMS_LIBPORT_PACKED1_DEFINITION(
struct USB_Hub_SuperSpeed_HubDescriptor_Wire
)
	{
	/*
	|| For consistency, we use bLength rather than bDescLength,
	|| which is what the spec uses in the table.
	*/
	unsigned char	bLength;

	/*
	|| Descriptor Type 2AH for SuperSpeed hub descriptor.
	*/
	unsigned char	bDescriptorType;

	/*
	|| Number of downstream-facing hub ports.
	|| Maximum is 15 for 3.0. 
	*/
	unsigned char	bNbrPorts;

	/*
	|| The hub characteristics
	|| Bits D4:D0 are the same as 2.0.
	|| Bits D7:D5 are used in 2.0, but are reserved in 3.0.
	*/
	unsigned char	wHubCharacteristics[2];

	/*
	|| power-on to power good, in 2ms clicks.
	|| Same as 2.0.
	*/
	unsigned char	bPwrOn2PwrGood;

	/*
	|| Maximum current requirements of the hub controller electronics, when 
	|| operating in both 2.0 and SuperSpeed mode - expressed in units of 
	|| aCurrentUnit (i.e. 50=50*aCurrentUnit mA). 
	||
	|| When operating only on USB 2.0, current requirement is reported in 
	|| the USB 2.0 hub descriptor.
	*/
	unsigned char	bHubContrCurrent;

	/*
	|| New field in 3.0. Hub Packet Header Decode Latency.
	|| Worst case latency for hubs whose upstream link is in U0 to decode
	|| the header of a downstream flowing TP or DP packet and initiate a 
	|| transition to U0 on the relevent downstream port. The time is 
	|| measured from receipt of the last symbol of the header packet by the 
	|| upstream port until the hubs starts LFPS on the intended downstream
	|| port. This field is used to calculate the total path exit latency
	|| through a hub.
	|| Value       Meaning
	|| 00H         Much lass than 0.1 microseconds
	|| 01H         0.1 microseconds
	|| 02H         0.2 microseconds
	|| 03H         0.3 microseconds
	|| 04H         0.4 microseconds
	|| 05H -       reserved
	|| FFH
	*/
	unsigned char	bHubHdrDecLat;

	/*
	|| New field in 3.0. 
	|| The field defines the average delay in nanoseconds a hub introduces
	|| on downstream flowing header packets, that it receives before 
	|| forwarding them, when both its upstream link and the downstream link, 
	|| on which it forwards the packet, are in the U0 state, and no Link 
	|| Comands are in flight. The time is measured from receipt of the last 
	|| symbol of the packet by the upstream port until the downstream port 
	|| sends the first framing symbol of the packet. 
	*/
	unsigned char   wHubDelay[2];

	/*
	|| Bit mask of removable devices on a per-port basis.
	|| Since port numbers start at 1, bitmap position zero is
	|| reserved.  Zero-padded to make a full byte.
	||
	|| Note that for SuperSpeed, there is a maximum of 15 ports, so
	|| only 2 bytes are required.
	*/ 
	unsigned char	DeviceRemovable[2];

	/* 
	|| there is not an additional PortPwrCtrlMask for SuperSpeed hubs.
	*/
	} __TMS_LIBPORT_PACKED1_SUFFIX;

#include "usbpump_poppack.h"

/*
|| Length of SuperSpeed Hub Descriptor Wire
*/
#define	LENGTHOF_USBIF_SUPERSPEED_HUBDESC_WIRE 12

/****************************************************************************\
|
|	Hub Status bits -- table 10-8
|
\****************************************************************************/
/*
||	Defined in 2.0 and used in 3.0
||	USB_Hub_HUB_STATUS_wStatus_LocalPower		0x0001
||	USB_Hub_HUB_STATUS_wStatus_OverCurrent		0x0002
*/

/****************************************************************************\
|
|	Port Status bits -- table 10-10
|
\****************************************************************************/
/*
||	Defined in 2.0 and used in 3.0
||	USB_Hub_PORT_STATUS_wStatus_Connect		0x0001
||	USB_Hub_PORT_STATUS_wStatus_Enable		0x0002
||	USB_Hub_PORT_STATUS_wStatus_OverCurrent		0x0008
||	USB_Hub_PORT_STATUS_wStatus_Reset		0x0010
*/

#define	USB_Hub_PORT_STATUS_wStatus_LinkState		0x01E0 /* bits 5-8 */
#define	USB_Hub_PORT_STATUS_wStatus_SuperSpeedPower	0x0200 /* bit 9, not 8 */
#define	USB_Hub_PORT_STATUS_wStatus_PortSpeed		0x1C00 /* bits 10-12 */

#define	USB_Hub_PORT_STATUS_wStatus_SuperSpeedReservedMask 0xE004

#define USB_Hub_PORT_STATUS_wStatus_LinkState_U0		0x00
#define USB_Hub_PORT_STATUS_wStatus_LinkState_U1		0x01
#define USB_Hub_PORT_STATUS_wStatus_LinkState_U2		0x02
#define USB_Hub_PORT_STATUS_wStatus_LinkState_U3		0x03
#define USB_Hub_PORT_STATUS_wStatus_LinkState_SSDisabled	0x04
#define USB_Hub_PORT_STATUS_wStatus_LinkState_RxDetect		0x05
#define USB_Hub_PORT_STATUS_wStatus_LinkState_SSInactive	0x06
#define USB_Hub_PORT_STATUS_wStatus_LinkState_Polling		0x07
#define USB_Hub_PORT_STATUS_wStatus_LinkState_Recovery		0x08
#define USB_Hub_PORT_STATUS_wStatus_LinkState_HotReset		0x09
#define USB_Hub_PORT_STATUS_wStatus_LinkState_Compliance	0x0A
#define USB_Hub_PORT_STATUS_wStatus_LinkState_Loopback		0x0B

/****************************************************************************\
|
|	Port Change bits -- table 10-11
|
\****************************************************************************/

#define	USB_Hub_PORT_STATUS_wChange_Connect		0x0001
#define	USB_Hub_PORT_STATUS_wChange_OverCurrent		0x0008
#define USB_Hub_PORT_STATUS_wChange_Reset		0x0010
#define USB_Hub_PORT_STATUS_wChange_BHReset		0x0020
#define	USB_Hub_PORT_STATUS_wChange_LinkState		0x0040
#define	USB_Hub_PORT_STATUS_wChange_ConfigError		0x0080

#define	USB_Hub_PORT_STATUS_wChange_SuperSpeedReservedMask 0xFF06

/**** end of usbhub30.h ****/
#endif /* _USBHUB30_H_ */
