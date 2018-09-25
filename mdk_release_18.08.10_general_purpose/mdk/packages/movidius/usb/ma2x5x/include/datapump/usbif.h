/* usbif.h	Wed Jan 04 2012 19:08:48 chwon */

/*

Module:  usbif.h

Function:
	Definitions from the USB-IF spec, version 1

Version:
	V3.01f	Wed Jan 04 2012 19:08:48 chwon	Edit level 12

Copyright notice:
	This file copyright (C) 1997-1998, 2003, 2005-2007, 2009, 2012 by

		MCCI Corporation.
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation.	April 1997

Revision history:
   1.00a  Sat Apr 19 1997 02:43:14  tmm
	Module created.

   1.00a  5/13/97  tmm
	Add a few more declarations for fields in bmRequestType.

   1.42a  5/24/1998  tmm
	Finish the bmRequestType matrix.

   1.85b  7/1/2003  tmm
	1095:  add the new descriptor types from the 2.0 specification,
	as amended by the IAD change notice.

   1.91h  2005-08-28  tmm
	Fix typo in name of USBFEATURE_DEVICE_A_HNP_SUPPORT: was
	USBFEATURE_DEVICE_A_NHP_SUPPORT

   1.93h  Mon Jan 30 2006 20:08:46  tmm
	Add USB_bmAttr_IS_PERIODIC, a macro that will tell you whether
	an endpoint is periodic.  Add USBFEATURE_ENDPOINT_HALT (which
	is the name we should have been using since 1.1 days) and
	deprecate USBFEATURE_ENDPOINT_STALL.

   1.97j  Sat Nov 03 2007 13:20:21  yssaroha
	3510: Add Descriptor type extended for WUSB 1.0

    1.97j  Tue Nov 20 2007 23:03:29  tmm
	Fix an error -- don't decloak __TMS_DATAPUMP_COMPAT.

   2.01a  Wed Feb 25 2009 11:33:07  djt
	2416: Added USB_bRequest_SET_SEL, USB_bRequest_ISOCH_DELAY, and
	USB_bDescType_SUPERSPEED_EP_COMPANION.
	Added new USBFEATUREs.

   3.01f  Wed Jan 04 2012 19:08:48  chwon
	14623: Added GET_STATUS value definition for USB 3.0.

*/

#ifndef _USBIF_H_		/* prevent multiple includes */
#define _USBIF_H_

/* for now, we maintain backwards compatibility */
#ifndef __TMS_DATAPUMP_COMPAT_V1_9
# define __TMS_DATAPUMP_COMPAT_V1_9 1
#endif

/***** the request types from chapter 9 *****/
/*	D...  means dev to host						    */
/*	H...  means host to dev						    */
/*	.S..  means standard						    */
/*	.C..  means class						    */
/*	.V..  means vendor						    */
/*	..DEV means device is target					    */
/*	..IFC means interface is target					    */
/*	..EP  means endpoint is target					    */
/*	..OTH means "other" is target					    */

#define USB_bmRequestType_DSDEV	0x80
#define USB_bmRequestType_HSDEV	0x00
#define USB_bmRequestType_DSIFC	0x81
#define USB_bmRequestType_HSIFC	0x01
#define USB_bmRequestType_DSEP	0x82
#define USB_bmRequestType_HSEP	0x02
#define USB_bmRequestType_DSOTH	0x83
#define USB_bmRequestType_HSOTH	0x03

#define USB_bmRequestType_DCDEV	0xA0
#define USB_bmRequestType_HCDEV	0x20
#define USB_bmRequestType_DCIFC	0xA1
#define USB_bmRequestType_HCIFC	0x21
#define USB_bmRequestType_DCEP	0xA2
#define USB_bmRequestType_HCEP	0x22
#define USB_bmRequestType_DCOTH	0xA3
#define USB_bmRequestType_HCOTH	0x23

#define USB_bmRequestType_DVDEV	0xC0
#define USB_bmRequestType_HVDEV	0x40
#define USB_bmRequestType_DVIFC	0xC1
#define USB_bmRequestType_HVIFC	0x41
#define USB_bmRequestType_DVEP	0xC2
#define USB_bmRequestType_HVEP	0x42
#define USB_bmRequestType_DVOTH	0xC3
#define USB_bmRequestType_HVOTH	0x43

/* now, break it down */
#define	USB_bmRequestType_DIR_SHIFT	(7)
#define	USB_bmRequestType_DIR_MASK	(1 << USB_bmRequestType_DIR_SHIFT)

#define	USB_bmRequestType_DIR_TODEV	(0 << USB_bmRequestType_DIR_SHIFT)
#define	USB_bmRequestType_DIR_TOHOST	(1 << USB_bmRequestType_DIR_SHIFT)

#define	USB_bmRequestType_TYPE_SHIFT	(5)
#define	USB_bmRequestType_TYPE_MASK	(3 << USB_bmRequestType_TYPE_SHIFT)
# define USB_bmRequestType_TYPE_STD	(0 << USB_bmRequestType_TYPE_SHIFT)
# define USB_bmRequestType_TYPE_CLASS	(1 << USB_bmRequestType_TYPE_SHIFT)
# define USB_bmRequestType_TYPE_VENDOR	(2 << USB_bmRequestType_TYPE_SHIFT)
# define USB_bmRequestType_TYPE_RSV3	(3 << USB_bmRequestType_TYPE_SHIFT)

#define	USB_bmRequestType_DEST_SHIFT	(0)
#define USB_bmRequestType_DEST_MASK	(0x1F << USB_bmRequestType_DEST_SHIFT)
# define USB_bmRequestType_DEST_DEVICE	(0x00 << USB_bmRequestType_DEST_SHIFT)
# define USB_bmRequestType_DEST_IFC	(0x01 << USB_bmRequestType_DEST_SHIFT)
# define USB_bmRequestType_DEST_EP	(0x02 << USB_bmRequestType_DEST_SHIFT)
# define USB_bmRequestType_DEST_OTHER	(0x03 << USB_bmRequestType_DEST_SHIFT)

/***** here are the chapter 9 USB endpoint-0 requests ******/
#define USB_bRequest_GET_STATUS		0
#define USB_bRequest_CLEAR_FEATURE	1
#define USB_bRequest_SET_FEATURE	3
#define USB_bRequest_SET_ADDRESS	5
#define USB_bRequest_GET_DESCRIPTOR	6
#define USB_bRequest_SET_DESCRIPTOR	7
#define USB_bRequest_GET_CONFIG		8
#define USB_bRequest_SET_CONFIG		9
#define USB_bRequest_GET_INTERFACE	10
#define USB_bRequest_SET_INTERFACE	11
#define USB_bRequest_SYNCH_FRAME	12
#define USB_bRequest_SET_SEL		48
#define USB_bRequest_ISOCH_DELAY	49

/***** Here are the Descriptor type codes *****/
#define USB_bDescType_DEVICE			1
#define USB_bDescType_CONFIG			2
#define USB_bDescType_STRING			3
#define USB_bDescType_INTERFACE			4
#define USB_bDescType_ENDPOINT			5
#define	USB_bDescType_DEVICE_QUALIFIER		6
#define	USB_bDescType_OTHER_SPEED_CONFIGURATION 7
#define	USB_bDescType_INTERFACE_POWER		8
#define	USB_bDescType_OTG			9
#define	USB_bDescType_DEBUG			10
#define	USB_bDescType_INTERFACE_ASSOCIATION	11
#define USB_bDescType_SECURITY			12
#define USB_bDescType_KEY			13
#define USB_bDescType_ENCRYPTION_TYPE		14
#define USB_bDescType_BOS			15
#define USB_bDescType_DEVICE_CAPABILITY		16
#define USB_bDescType_WEP_COMPANION		17
#define USB_bDescType_SUPERSPEED_EP_COMPANION	48

/***** Endpoint attribute codes *****/
#define USB_bmAttr_CONTROL	0
#define USB_bmAttr_ISO		1
#define USB_bmAttr_BULK		2
#define USB_bmAttr_INT		3

/* node that odd endpoint codes are periodic, so: */
#define	USB_bmAttr_IS_PERIODIC(x)	((x) & 1)

/**** Feature settings ****/
#define	USBFEATURE_ENDPOINT_HALT		0

#if __TMS_DATAPUMP_COMPAT_V1_9
#define	USBFEATURE_ENDPOINT_STALL		USBFEATURE_ENDPOINT_HALT
#endif

#define USBFEATURE_INTERFACE_FUNCTION_SUSPEND 	0

#define	USBFEATURE_DEVICE_REMOTE_WAKEUP		1
#define	USBFEATURE_DEVICE_TEST_MODE		2
#define	USBFEATURE_DEVICE_B_HNP_ENABLE		3
#define USBFEATURE_DEVICE_A_HNP_SUPPORT		4
#define	USBFEATURE_DEVICE_A_ALT_HNP_SUPPORT	5
#define USBFEATURE_DEVICE_U1_ENABLE		48
#define USBFEATURE_DEVICE_U2_ENABLE		49
#define USBFEATURE_DEVICE_LTM_ENABLE		50

/**** address masks ****/
#define	USB_bEndpointAddress_IN		0x80	/* if set, it's an input */
#define	USB_bEndpointAddress_PORTMASK	0x0F	/* the port number mask */

/**** the results for GET_STATUS on a device ****/
#define USBDEVSTAT_SELFPWR	(1 << 0)
#define USBDEVSTAT_RMWKUP	(1 << 1)
#define	USBDEVSTAT_U1_ENABLE	(1 << 2)
#define	USBDEVSTAT_U2_ENABLE	(1 << 3)
#define	USBDEVSTAT_LTM_ENABLE	(1 << 4)

/**** the results for GET_STATUS on an interface ****/
#define USBIFCSTAT_FUNCTION_REMOTE_WAKE_CAPABLE	(1 << 0)
#define USBIFCSTAT_FUNCTION_REMOTE_WAKEUP	(1 << 1)

/**** the results for GET_STATUS on an endpoint ****/
#define USBEPSTAT_STALL		(1 << 0)

/**** the values for interface function suspend (SET_FEATURE) ****/
#define	USBFEATURE_INTERFACE_FUNCTION_LOW_POWER_SUSPEND		(1 << 0)
#define	USBFEATURE_INTERFACE_FUNCTION_REMOTE_WAKE_ENABLE	(1 << 1)

/**** the values for test-mode selection (SET_FEATURE) ****/
#define	USBFEATURE_DEVICE_TEST_SELECT_J		0x01
#define	USBFEATURE_DEVICE_TEST_SELECT_K		0x02
#define	USBFEATURE_DEVICE_TEST_SELECT_SE0_NAK	0x03
#define	USBFEATURE_DEVICE_TEST_SELECT_PACKET	0x04
#define	USBFEATURE_DEVICE_TEST_SELECT_FORCE_EN	0x05
#define	USBFEATURE_DEVICE_TEST_SELECT_VS0	0xC0
#define	USBFEATURE_DEVICE_TEST_SELECT_VSMAX	0xFF
#define	USBFEATURE_DEVICE_TEST_SELECT_VSn(N)	(0xC0 + (N))

#define	USBFEATURE_DEVICE_TEST_wIndexToSelect(w) (((w) >> 8) & 0xFF)
#define	USBFEATURE_DEVICE_TEST_wIndexToAddress(w) ((w) & 0xFF)
#define	USBFEATURE_DEVICE_TEST_MakeIndex(s,a)	(((s) << 8) | (a))

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/* make sure cloaking control var is defined */
#ifndef __TMS_CLOAKED_NAMES_ONLY
# define __TMS_CLOAKED_NAMES_ONLY 0
#endif

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define DATAPUMP_COMPAT_V1_9	\
   __TMS_DATAPUMP_COMPAT_V1_9
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/**** end of usbif.h ****/
#endif /* _USBIF_H_ */
