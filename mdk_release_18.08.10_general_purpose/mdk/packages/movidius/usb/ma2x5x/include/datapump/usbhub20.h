/* usbhub20.h	Tue Jan 13 2015 20:28:43 tmm */

/*

Module:  usbhub20.h

Function:
	USB Hub Device Class (HUB) V2.0 constants from the core spec.

Version:
	V3.13b	Tue Jan 13 2015 20:28:43 tmm	Edit level 12

Copyright notice:
	This file copyright (C) 2005-2007, 2009-2010, 2012, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	Distribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistribution of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. MCCI's name may not be used to endorse or promote products
	   derived from this software without specific prior written
	   permission.

Disclaimer:
	THIS FILE IS PROVIDED BY MCCI "AS IS" AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED.  IN NO EVENT SHALL MCCI BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
	GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS FILE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
Author:
	Terry Moore, MCCI Corporation	June 2005

Notes:
	This header file should be included with "structure packing" turned
	on -- otherwise the structures defined herein may have spurious
	gaps introduced at the whim of the compiler.

Revision history:
   1.91d  Sat Jun 18 2005 10:18:02  tmm
	Module created.

   1.91h  2005-08-28  tmm
	Revise to match conventions in MCCI's usbif.h.

   1.91h  Mon Sep  5 2005 01:20:19  tmm
	Add maxes for features.

   1.93h  Sat Feb 18 2006 12:08:11  tmm
	Fix up the structure definition; and allow us to use the MCCI
	header files.

   1.93e  Thu Feb 23 2006 13:47:01  chwon
	Fix up missing include "usbpump_poppack.h"

   1.97e  Tue Aug 08 2006 14:16:04  tmm
	Bug 1894:  fix typos in definitions of 
	USB_Hub_FEATURE_HUB__SET_MASK and USB_Hub_FEATURE_HUB__CLEAR_MASK

   1.97j  Mon Sep 10 2007 10:43:20  djt
	3265: Added USB_Hub_wValue_ClearTTBuffer.
	Fixed company name and copyright.

   2.01a  Wed Dec 09 2009 15:21:09  chwon
	9569: Added LPM related request definition

   3.01a  Fri Jan 15 2010 12:24:15  ssyang
	9674: add port indicator bit into hub characteristics

   3.01f  Mon Feb 13 2012 22:19:05  chwon
	14821: Corrected USB_bRequest_Hub_SET_AND_TEST value and added
	USB_Hub_LPM_SET_AND_TEST_STATUS_STALL.

   3.01f  Wed Jun 13 2012 15:58:48  chwon
	15535: Added USB_Hub_FEATURE_PORT__INIT macro.
	
   3.13b  Tue Jan 13 2015 20:28:43  tmm
   	18845: Add USB_Hub_TestSelector__INIT macro.

*/

#ifndef _USBHUB20_H_		/* prevent multiple includes */
#define _USBHUB20_H_

/* it's more important that the structures be packed... */
#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif

/****************************************************************************\
|
|	Class, subclass, and protocol codes
|
\****************************************************************************/

/**** device class codes ****/
#define USB_bDeviceClass_Hub		9	/* the hub device class. */

/**** device protocol codes ****/
#define USB_bDeviceProto_Hub_FS		0	/* full speed hub  */
#define USB_bDeviceProto_Hub_HS_1TT	1	/* high-speed hub with 1 
						|| transaction translator 
						*/
#define USB_bDeviceProto_Hub_HS_ManyTT	2	/* high-speed hub with 
						|| multiple transaction 
						|| translators. 
						*/

/**** interface class codes ****/
#define USB_bInterfaceClass_Hub		9	/* the hub interface class */

/**** interface protocol codes ****/
#define USB_bInterfaceProto_Hub_FS	0	/* full speed hub  */
#define USB_bInterfaceProto_Hub_HS_1TT	1	/* high-speed hub in single
						|| transaction translator mode
						*/
#define USB_bInterfaceProto_Hub_HS_ManyTT  2	/* high-speed hub in multiple 
						|| transaction translator mode
						*/

/****************************************************************************\
|
|	Class request codes -- table 11-16, p 421
|
\****************************************************************************/

#define USB_bRequest_Hub_GET_STATUS	0	/* get 4-byte hub or port 
						|| status 
						*/
#define USB_bRequest_Hub_CLEAR_FEATURE	1	/* clear hub or port feature */
#define USB_bRequest_Hub_SET_FEATURE	3	/* set hub or port feature */
#define USB_bRequest_Hub_GET_DESCRIPTOR	6	/* get hub descriptor */
#define USB_bRequest_Hub_SET_DESCRIPTOR	7	/* set hub descriptor */
#define USB_bRequest_Hub_CLEAR_TT_BUFFER 8	/* clear the TT buffer */
#define USB_bRequest_Hub_RESET_TT	9	/* reset the TT */
#define USB_bRequest_Hub_GET_TT_STATE	10	/* get the TT state */
#define USB_bRequest_Hub_STOP_TT	11	/* stop the TT */
#define	USB_bRequest_Hub_SET_AND_TEST	12	/* Set and Test Port Feature */

/* for debug prints */
#define	USB_bRequest_Hub__INIT						\
	{ USB_bRequest_Hub_GET_STATUS, "GET_STATUS" },			\
	{ USB_bRequest_Hub_CLEAR_FEATURE, "CLEAR_FEATURE" },		\
	{ USB_bRequest_Hub_SET_FEATURE, "SET_FEATURE" },		\
	{ USB_bRequest_Hub_GET_DESCRIPTOR, "GET_DESCRIPTOR" },		\
	{ USB_bRequest_Hub_SET_DESCRIPTOR, "SET_DESCRIPTOR" },		\
	{ USB_bRequest_Hub_CLEAR_TT_BUFFER, "CLEAR_TT_BUFFER" },	\
	{ USB_bRequest_Hub_RESET_TT, "RESET_TT" },			\
	{ USB_bRequest_Hub_GET_TT_STATE, "GET_TT_STATE" },		\
	{ USB_bRequest_Hub_STOP_TT, "STOP_TT" },			\
	{ USB_bRequest_Hub_SET_AND_TEST, "SET_AND_TEST" }

/****************************************************************************\
|
|	Hub feature selector codes -- table 11-20, p 426
|
\****************************************************************************/

#define USB_Hub_FEATURE_C_HUB_LOCAL_POWER	0	/* change in hub local 
							|| power 
							*/
#define USB_Hub_FEATURE_C_HUB_OVER_CURRENT	1	/* change in hub 
							|| over-current  
							*/

#define USB_Hub_FEATURE_HUB__MAX		1	/* maximum hub feature */

#define	USB_Hub_FEATURE_HUB__CLEAR_MASK				\
	((1u << USB_Hub_FEATURE_C_HUB_LOCAL_POWER) |		\
	 (1u << USB_Hub_FEATURE_C_HUB_OVER_CURRENT))

#define	USB_Hub_FEATURE_HUB__SET_MASK				\
	((1u << USB_Hub_FEATURE_C_HUB_LOCAL_POWER) |		\
	 (1u << USB_Hub_FEATURE_C_HUB_OVER_CURRENT))

/* for debug prints */
#define	USB_Hub_FEATURE__INIT						\
	{ USB_Hub_FEATURE_C_HUB_LOCAL_POWER, "C_HUB_LOCAL_POWER" },	\
	{ USB_Hub_FEATURE_C_HUB_OVER_CURRENT, "C_HUB_OVER_CURRENT" }

/****************************************************************************\
|
|	Port feature selector codes
|
\****************************************************************************/


#define USB_Hub_FEATURE_PORT_CONNECTION	0	/* port connection status */

/*
|| Clearing the PORT_ENABLE feature causes the port to be placed in the
|| Disabled state.  If the port is in the Powered-off state, the hub should
|| treat this request as a functional no-operation.
*/
#define USB_Hub_FEATURE_PORT_ENABLE	1	/* port enable */

/*
|| Clearing the PORT_SUSPEND feature causes a host-initiated resume on the
|| specified port.  If the port is not in the Suspended state, the hub should
|| treat this request as a functional no-operation.
||
|| Setting the PORT_SUSPEND feature causes bus traffic to cease on that port 
|| and, consequently, the device to suspend.
*/
#define USB_Hub_FEATURE_PORT_SUSPEND	2	/* port suspend */
#define USB_Hub_FEATURE_PORT_OVER_CURRENT 3	/* port over-current status */

/*
|| Setting the reset feature PORT_RESET causes the hub to signal reset on that
|| port.  When the reset signaling is complete, the hub sets the C_PORT_RESET
|| status change and immediately enables the port.
*/
#define USB_Hub_FEATURE_PORT_RESET	4	/* port reset */

/*
|| Clearing the PORT_POWER feature causes the port to be placed in the
|| Powered-off state and may, subject to the constraints due to the hub's
|| method of power switching, result in power being removed from the port.
|| Refer to [USBCORE] Section 11.11 on rules for how this request is used with
|| ports that are gang-powered.
*/
#define USB_Hub_FEATURE_PORT_POWER	8	/* port power */
#define USB_Hub_FEATURE_PORT_LOW_SPEED	9	/* port is in low-speed mode */

/*
|| Clearing the PORT_L1 feature causes a host-initiated L1 exit (resume)
|| on the specified port. If the port is not in the L1 state, the hub must
|| treat this request as a functional no-operation.
*/
#define USB_Hub_FEATURE_PORT_L1		10	/* port is in L1 state */

#define USB_Hub_FEATURE_C_PORT_CONNECTION  16	/* Change in state of PORT_CONNECTION */
#define USB_Hub_FEATURE_C_PORT_ENABLE	17	/* Change in state of PORT_ENABLE */
#define USB_Hub_FEATURE_C_PORT_SUSPEND	18	/* Change in state of PORT_SUSPEND */
#define USB_Hub_FEATURE_C_PORT_OVER_CURRENT 19	/* Change in state of PORT_OVER_CURRENT */
#define USB_Hub_FEATURE_C_PORT_RESET	20	/* Change in state of PORT_RESET */

/*
|| When setting PORT_TEST, bits 15..8 of wIndex select the test mode, and bits
|| 7..0 select the port
*/
#define USB_Hub_FEATURE_PORT_TEST	21	/* Enable a test mode.  */

/*
|| When controlling PORT_INDICATOR features, bits 15..8 of wIndex select the
|| indicator mode, and bits 7..0 select the port.
*/
#define USB_Hub_FEATURE_PORT_INDICATOR  22	/* Set a port indictor state. */

#define USB_Hub_FEATURE_C_PORT_L1	23	/* Change in state of PORT_L1 */

#define USB_Hub_FEATURE_PORT__MAX	23	/* maximum hub feature */

#define	USB_Hub_FEATURE_PORT__VALID_MASK				\
	((1u << USB_Hub_FEATURE_PORT_CONNECTION) |			\
	 (1u << USB_Hub_FEATURE_PORT_ENABLE) |				\
	 (1u << USB_Hub_FEATURE_PORT_SUSPEND) |				\
	 (1u << USB_Hub_FEATURE_PORT_OVER_CURRENT) |			\
	 (1u << USB_Hub_FEATURE_PORT_RESET) |				\
	 (1u << USB_Hub_FEATURE_PORT_POWER) |				\
	 (1u << USB_Hub_FEATURE_PORT_LOW_SPEED) |			\
	 (1u << USB_Hub_FEATURE_PORT_L1) |				\
	 (1u << USB_Hub_FEATURE_C_PORT_CONNECTION) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_ENABLE) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_SUSPEND) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_OVER_CURRENT) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_RESET) |				\
	 (1u << USB_Hub_FEATURE_PORT_TEST) |				\
	 (1u << USB_Hub_FEATURE_PORT_INDICATOR) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_L1))


/* in the order from 11.24.2.2 */
#define	USB_Hub_FEATURE_PORT__CLEAR_MASK				\
	((1u << USB_Hub_FEATURE_PORT_ENABLE) |				\
	 (1u << USB_Hub_FEATURE_PORT_SUSPEND) |				\
	 (1u << USB_Hub_FEATURE_PORT_POWER) |				\
	 (1u << USB_Hub_FEATURE_PORT_INDICATOR) |			\
	 (1u << USB_Hub_FEATURE_PORT_L1) |				\
	 (1u << USB_Hub_FEATURE_C_PORT_CONNECTION) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_RESET) |				\
	 (1u << USB_Hub_FEATURE_C_PORT_ENABLE) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_SUSPEND) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_OVER_CURRENT) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_L1))

/* in the order from 11.24.2.13 */
#define	USB_Hub_FEATURE_PORT__SET_MASK					\
	((1u << USB_Hub_FEATURE_PORT_RESET) |				\
	 (1u << USB_Hub_FEATURE_PORT_SUSPEND) |				\
	 (1u << USB_Hub_FEATURE_PORT_POWER) |				\
	 (1u << USB_Hub_FEATURE_PORT_TEST) |				\
	 (1u << USB_Hub_FEATURE_PORT_INDICATOR) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_CONNECTION) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_RESET) |				\
	 (1u << USB_Hub_FEATURE_C_PORT_ENABLE) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_SUSPEND) |			\
	 (1u << USB_Hub_FEATURE_C_PORT_OVER_CURRENT))

/**** an initializer for debug prints ****/
#define	USB_Hub_FEATURE_PORT__INIT					\
	{ USB_Hub_FEATURE_PORT_CONNECTION, "PORT_CONNECTION" },		\
	{ USB_Hub_FEATURE_PORT_ENABLE, "PORT_ENABLE" },			\
	{ USB_Hub_FEATURE_PORT_SUSPEND, "PORT_SUSPEND" },		\
	{ USB_Hub_FEATURE_PORT_OVER_CURRENT, "PORT_OVER_CURRENT" },	\
	{ USB_Hub_FEATURE_PORT_RESET, "PORT_RESET" },			\
	{ USB_Hub_FEATURE_PORT_POWER, "PORT_POWER" },			\
	{ USB_Hub_FEATURE_PORT_LOW_SPEED, "PORT_LOW_SPEED" },		\
	{ USB_Hub_FEATURE_PORT_L1, "PORT_L1" },				\
	{ USB_Hub_FEATURE_C_PORT_CONNECTION, "C_PORT_CONNECTION" },	\
	{ USB_Hub_FEATURE_C_PORT_ENABLE, "C_PORT_ENABLE" },		\
	{ USB_Hub_FEATURE_C_PORT_SUSPEND, "C_PORT_SUSPEND" },		\
	{ USB_Hub_FEATURE_C_PORT_OVER_CURRENT, "C_PORT_OVER_CURRENT" },	\
	{ USB_Hub_FEATURE_C_PORT_RESET, "C_PORT_RESET" },		\
	{ USB_Hub_FEATURE_PORT_TEST, "PORT_TEST" },			\
	{ USB_Hub_FEATURE_PORT_INDICATOR, "PORT_INDICATOR" },		\
	{ USB_Hub_FEATURE_C_PORT_L1, "C_PORT_L1" }

/****************************************************************************\
|
|	Additional definitions for test mode
|
\****************************************************************************/

#define	USB_Hub_wIndex_ToTestSelector(wIndex)	(((wIndex) >> 8) & 0xFF)
#define	USB_Hub_wIndex_ToTestPort(wIndex)	((wIndex) & 0xFF)
#define	USB_Hub_wIndex_Test(sel, port)		(((sel) << 8) | (port))

#define	USB_Hub_TestSelector_J		0x01
#define	USB_Hub_TestSelector_K		0x02
#define	USB_Hub_TestSelector_SE0_NAK	0x03
#define	USB_Hub_TestSelector_PACKET	0x04
#define	USB_Hub_TestSelector_FORCE_ENABLE 0x05
#define	USB_Hub_TestSelector_VENDOR0	0xC0
#define	USB_Hub_TestSelector_VENDORMAX	0xFF
#define	USB_Hub_TestSelector_VENDORn(N)	(USB_Hub_TestSelector_VENDOR0 + (N))

/**** an initializer for debug prints ****/
#define	USB_Hub_TestSelector__INIT					\
	{ USB_Hub_TestSelector_J,		"J" },			\
	{ USB_Hub_TestSelector_K,		"K" },	                \
	{ USB_Hub_TestSelector_SE0_NAK,		"SEO_NAK" },	        \
	{ USB_Hub_TestSelector_PACKET,		"PACKET" },             \
	{ USB_Hub_TestSelector_FORCE_ENABLE, 	"FORCE_ENABLE" }


#define	USB_Hub_TestSelector__IS_VALID(N)				\
	((USB_Hub_TestSelector_J <= (N) &&				\
	  (N) <= USB_Hub_TestSelector_FORCE_ENABLE) ||			\
	 (USB_Hub_TestSelector_VENDOR0 <= (N) &&			\
	  (N) <= USB_Hub_TestSelector_VENDORMAX))

/****************************************************************************\
|
|	Additional definitions for indicator features
|
\****************************************************************************/

#define	USB_Hub_wIndex_INDICATOR_AUTO	0x0000
#define	USB_Hub_wIndex_INDICATOR_AMBER	0x0100
#define	USB_Hub_wIndex_INDICATOR_GREEN	0x0200
#define	USB_Hub_wIndex_INDICATOR_OFF	0x0300

#define	USB_Hub_IndicatorSelector_AUTO	0x00
#define	USB_Hub_IndicatorSelector_AMBER	0x01
#define	USB_Hub_IndicatorSelector_GREEN	0x02
#define	USB_Hub_IndicatorSelector_OFF	0x03

#define	USB_Hub_IndicatorSelector__VALID_MASK			\
	((1u << USB_Hub_IndicatorSelector_AUTO) |		\
	 (1u << USB_Hub_IndicatorSelector_AMBER) |		\
	 (1u << USB_Hub_IndicatorSelector_GREEN) |		\
	 (1u << USB_Hub_IndicatorSelector_OFF))

/****************************************************************************\
|
|	Additional definitions for Clear TT buffer
|
\****************************************************************************/

#define USB_Hub_wValue_ClearTTBuffer(epNum, deviceAddr, epType, dir)	\
	((epNum) | ((deviceAddr) << 4) | ((epType) << 11) | ((dir) << 15))

/****************************************************************************\
|
|	Additional definitions for LPM
|
\****************************************************************************/

#define	USB_Hub_wIndex_LpmClearFeature(PortNo, Hird)			\
	((((Hird) & 0x0F) << 4) |					\
	 ((PortNo) & 0x0F))

#define	USB_Hub_wIndex_LpmSetAndTest(PortNo, Hird, DevAddr, RemoteWakeupEnable) \
	((((RemoteWakeupEnable) ? 1 : 0) << 15) |			\
	 (((DevAddr) & 0x7F) << 8) |					\
	 (((Hird) & 0x0F) << 4) |					\
	 ((PortNo) & 0x0F))

#define	USB_Hub_wIndex_ToLpmSelector(wIndex)	(((wIndex) >> 8) & 0xFF)
#define	USB_Hub_wIndex_ToLpmPortNumber(wIndex)	((wIndex) & 0x0F)
#define	USB_Hub_wIndex_ToLpmBesl(wIndex)	(((wIndex) >> 4) & 0x0F)
/* define USB_Hub_wIndex_ToLpmHird() for backward compatibility */
#define	USB_Hub_wIndex_ToLpmHird(wIndex)	USB_Hub_wIndex_ToLpmBesl(wIndex)
#define	USB_Hub_wIndex_ToLpmDevAddr(wIndex)	(((wIndex) >> 8) & 0x7F)
#define	USB_Hub_wIndex_ToLpmRemoteWake(wIndex)	((wIndex) & 0x8000)

#define	USB_Hub_LPM_SET_AND_TEST_STATUS_SUCCESS	0x00
#define	USB_Hub_LPM_SET_AND_TEST_STATUS_RESERVE	0x01
#define	USB_Hub_LPM_SET_AND_TEST_STATUS_NYET	0x10
#define	USB_Hub_LPM_SET_AND_TEST_STATUS_TIMEOUT	0x11
#define	USB_Hub_LPM_SET_AND_TEST_STATUS_STALL	0x30

/****************************************************************************\
|
|	Descriptors
|
\****************************************************************************/

#define	USB_bDescriptorType_Hub_HUB_DESCRIPTOR	0x29

/* define the type */
typedef struct USB_Hub_HubDescriptor_Wire	
	USBIF_HUBDESC_WIRE, *PUSBIF_HUBDESC_WIRE;

/* from page 418 */
#include "usbpump_pushpack1.h"

__TMS_LIBPORT_PACKED1_DEFINITION(
struct USB_Hub_HubDescriptor_Wire
)
	{
	/*
	|| For consistency, we use bLength rather than bDescLength,
	|| which is what the spec uses in the table.
	*/
	unsigned char	bLength;
	unsigned char	bDescriptorType;

	/*
	|| Number of downstream-facing hub ports.  
	*/
	unsigned char	bNbrPorts;

	/*
	|| The hub characteristics
	*/
	unsigned char	wHubCharacteristics[2];

	/*
	|| power-on to power good, in 2ms clicks.
	*/
	unsigned char	bPwrOn2PwrGood;

	/*
	|| power taken by the electronics of the hub, in 1mA clicks.
	|| Doesn't include embedded devices.
	*/
	unsigned char	bHubContrCurrent;

	/*
	|| bit mask of removable devices on a per-port basis.
	|| Since port numbers start at 1, bitmap position zero is
	|| reserved.  Zero-padded to make a full byte.
	*/
	unsigned char	DeviceRemovable[1];

	/* 
	|| there is an additional PortPwrCtrlMask for 1.0 hubs but it's
	|| at a variable offset and we don't define it separately.  It's
	|| supposed to be a bitmap, one bit per port, always set.  We
	|| assume that we will return 0 in bit position 0, which ought
	|| to be reserved.  For refreence on 
	*/
	} __TMS_LIBPORT_PACKED1_SUFFIX;

#include "usbpump_poppack.h"

/*
|| Calculate proper length given number of downstream ports.  Take into
|| account the 1.0 compliance bits.
*/
#define	LENGTHOF_USBIF_HUBDESC_WIRE(bNbrPorts)		\
	(7 + 2 * (((bNbrPorts) + 8) / 8))

/*
||  bits in hub characteristics 
*/

/* logical power switching mode */
#define	USB_Hub_wHubCharacteristics_LPSM_MASK	0x0003
#define	USB_Hub_wHubCharacteristics_LPSM_SHIFT	0
#define	USB_Hub_wHubCharacteristics_LPSM_GANGED	0x0000	/* ganged */
#define	USB_Hub_wHubCharacteristics_LPSM_SINGLE	0x0001	/* port-by-port */
#define	USB_Hub_wHubCharacteristics_LPSM_NONE	0x0002	/* 1.0 only: none */
#define	USB_Hub_wHubCharacteristics_LPSM_NONE_A	0x0003	/* 1.0 only: none
							|| (alt encoding
							*/

/* compound device flag */
#define	USB_Hub_wHubCharacteristics_COMPOUND	0x0004	/* part of compound dev */

/* overcurrent-protection mode */
#define	USB_Hub_wHubCharacteristics_OCPM_MASK	0x0018
#define	USB_Hub_wHubCharacteristics_OCPM_SHIFT	3
#define	USB_Hub_wHubCharacteristics_OCPM_GLOBAL	0x0000	/* hub-global */
#define	USB_Hub_wHubCharacteristics_OCPM_SINGLE	0x0008	/* port-by-port */
#define	USB_Hub_wHubCharacteristics_OCPM_NONE	0x0010	/* none */
#define	USB_Hub_wHubCharacteristics_OCPM_NONE_A	0x0018	/* alt encoding for none */

/* TT think time */
#define	USB_Hub_wHubCharacteristics_TTTM_SHIFT	5
#define	USB_Hub_wHubCharacteristics_TTTM_MASK	0x0060
#define	USB_Hub_wHubCharacteristics_TTTM_8	0x0000	/* 8 fs bit times */
#define	USB_Hub_wHubCharacteristics_TTTM_16	0x0020	/* 16 fs bit times */
#define	USB_Hub_wHubCharacteristics_TTTM_24	0x0040	/* 24 fs bit times */
#define	USB_Hub_wHubCharacteristics_TTTM_32	0x0060	/* 32 fs bit times */
#define USB_Hub_wHubCharacteristics_TTTM(wHubCharacteristics)			\
	(((wHubCharacteristics) & USB_Hub_wHubCharacteristics_TTTM_MASK) >>	\
	 USB_Hub_wHubCharacteristics_TTTM_SHIFT)

/* port indicator */
#define	USB_Hub_wHubCharacteristics_PIS_SUPPORT		0x0080	/* supported */

/****************************************************************************\
|
|	Arguments for requests
|
\****************************************************************************/

#define	USB_bDescriptorType_Hub_HUB_DESCRIPTOR	0x29

/* define the type */
typedef struct USB_Hub_HUB_OR_PORT_STATUS_WIRE
	USBIF_HUB_OR_PORT_STATUS_WIRE, *PUSBIF_HUB_OR_PORT_STATUS_WIRE;

/* from page 418 */
#include "usbpump_pushpack1.h"

__TMS_LIBPORT_PACKED1_DEFINITION(
struct USB_Hub_HUB_OR_PORT_STATUS_WIRE
)
	{
	unsigned char	wStatus[2];
	unsigned char	wChange[2];
	} __TMS_LIBPORT_PACKED1_SUFFIX;

#include "usbpump_poppack.h"

#define	LENGTHOF_USBIF_HUB_OR_PORT_STATUS_WIRE	4

#define	USB_Hub_HUB_STATUS_wStatus_LocalPower	0x0001
#define	USB_Hub_HUB_STATUS_wStatus_OverCurrent	0x0002
#define	USB_Hub_HUB_STATUS_wStatus_ReservedMask 0xFFFC

#define	USB_Hub_PORT_STATUS_wStatus_Connect	0x0001
#define	USB_Hub_PORT_STATUS_wStatus_Enable	0x0002
#define	USB_Hub_PORT_STATUS_wStatus_Suspend	0x0004
#define	USB_Hub_PORT_STATUS_wStatus_OverCurrent	0x0008
#define	USB_Hub_PORT_STATUS_wStatus_Reset	0x0010
#define	USB_Hub_PORT_STATUS_wStatus_L1		0x0020
#define	USB_Hub_PORT_STATUS_wStatus_Power	0x0100
#define	USB_Hub_PORT_STATUS_wStatus_LowSpeed	0x0200
#define	USB_Hub_PORT_STATUS_wStatus_HighSpeed	0x0400
#define	USB_Hub_PORT_STATUS_wStatus_Test	0x0800
#define	USB_Hub_PORT_STATUS_wStatus_Manual	0x1000

#define	USB_Hub_PORT_STATUS_wStatus_ReservedMask 0xE0C0

#define	USB_Hub_PORT_STATUS_wChange_ReservedMask 0xFFC0

/**** end of usbhub20.h ****/
#endif /* _USBHUB20_H_ */
