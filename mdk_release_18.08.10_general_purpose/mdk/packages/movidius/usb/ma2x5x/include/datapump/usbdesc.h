/* usbdesc.h	Mon Feb 13 2012 22:22:11 chwon */

/*

Module:  usbdesc.h

Function:
	USB/WUSB/SUPERSPEED descriptor structures as per Chap. 9/ Chap7.

Version:
	V3.01f	Mon Feb 13 2012 22:22:11 chwon	Edit level 20

Copyright notice:
	This file copyright (C) 1997-1998, 2000-2003, 2006-2010, 2012 by

		MCCI Corporation.
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	John Keys, MCCI Corporation.	September 1997

Revision history:
   1.10d  Thu Sep  4 1997 14:04:59  johnk
	Module created.

   1.47e  12/19/1998  tmm
	Add wire-compatible definitions.

   1.60e  6/6/2000  tmm
	Eschew metabrackets on #include.

   1.61c  1/5/2001  tmm
	Fix a problem with portability:  sizeof() on an odd byte-length
	structure isn't portable, because compilers might not pack arrays
	of such structs.  Define LENGTHOF_... symbols for the various
	wire-lenth structures.

   1.64a 5/27/2002  tmm
	Factor out all the typedefs, so we can define them externally if
	we need to.

   1.85b  7/1/2003  tmm
	1095:  Add USB 2.0 descriptors

   1.93e  Tue Jan 24 2006 05:53:10  tmm
	For high-speed we need masks within the endpoint descriptor fields,
	becuase they pack in extra info.  Also, change to use of the
	automatic type-cloaking tool.

   1.93e  Fri Feb 17 2006 13:13:05  tmm
	Add string descriptor.... why wasn't that there before?

   1.97j  Sat Nov 03 2007 11:14:25  yssaroha
	3510: Add WUSB descriptor structures

   1.97j  Tue Nov 20 2007 23:23:13  tmm
	Fix cloaking problem.

   1.97j  Mon Nov 26 2007 10:20:54  chwon
	Correct misnamed macro definition.

   1.97k  Tue Sep 23 2008 15:05:48  cccho
	6683: add USB 2.0 extension descriptor structures.

   2.01a  Wed Feb 25 2009 10:55:56  djt
	7146: Added Usage Type definitions for interrupt endpoints for USB 3.0.
	Added USBIF_SUPERSPEED_EP_COMPANION_DESC. 
	Added USBIF_SUPERSPEED_DEVCAPDESC, USBIF_EXTENSION_DEVCAPDESC, 
	USBIF_CONTAINER_ID_DEVCAPDESC.

   2.01a  Tue May 26 2009 06:25:36  tmm
	8109: add USBIF_EPDESC_bmAttributes_IS_LOW_POWER_INT() and
	USBIF_EPDESC_bmAttributes_HAS_DATA_ADJUST().

   2.01a  Mon Aug 31 2009 14:22:05  chwon
	8814: fixed wrong wPHYRates definition

   2.01a  Fri Nov 20 2009 14:52:38  chwon
	9465: add USBIF_SUPERSPEED_EP_COMPANION_DESC_bMaxBurst_IS_VALID()

   2.01a  Wed Dec 09 2009 16:28:09  chwon
	9569: add USBIF_WUSB_DEVCAPDESC_bmAttributes_BEACON() and _MASK.
	Improve comments in the descriptor structure.

   3.01a  Mon Feb  8 2010 18:35:08  tmm
	9893: rename the MAXSTREAMS macro to 
	USBIF_SUPERSPEED_EP_COMPANION_DESC_bmAttributes_LG2MAXSTREAMS, as
	it returns the log2 of the maximum number of streams (more or less).
	Add USBIF_SUPERSPEED_EP_COMPANION_DESC_bmAttributes_MAXSTREAMS to
	return the numerical value.

   3.01f  Mon Feb 13 2012 22:22:11  chwon
	14821: add LPM attributes definition which are added in the errata.

*/

#ifndef _USBDESC_H_		/* prevent multiple includes */
#define _USBDESC_H_

/*
|| We need some basic types.
*/
#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif

/*
|| Defines, code, etc. are in usbif.h
*/
#ifndef _USBIF_H_
# include "usbif.h"
#endif

/*
|| The typenames
*/
#ifndef _USBDESC_TYPES_H_
# include "usbdesc_types.h"
#endif

#ifndef _USBUUID_H_
# include "usbuuid.h"
#endif

/*
|| The Device Descriptor
*/
struct __TMS_STRUCTNAME(USBIF_DEVDESC)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_DevDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_DEVICE		*/
	__TMS_UINT16	bcdUSB;			/* USB Spec. Release Number	*/
	__TMS_UINT8	bDeviceClass;		/* Class Code			*/
	__TMS_UINT8	bDeviceSubClass;	/* Subclass Code		*/
	__TMS_UINT8	bDeviceProtocol;	/* Protocol code		*/
	__TMS_UINT8	bMaxPacketSize0;	/* max packet size - ep0	*/
	__TMS_UINT16	idVendor;		/* vendor ID			*/
	__TMS_UINT16	idProduct;		/* product ID			*/
	__TMS_UINT16	bcdDevice;		/* Device release number	*/
	__TMS_UINT8	iManufacturer;		/* string desc index for mfg	*/
	__TMS_UINT8	iProduct;		/* string desc index for prod   */
	__TMS_UINT8	iSerialNumber;		/* string desc index for sn	*/
	__TMS_UINT8	bNumConfigurations;	/* number of possible configs   */
	};

struct __TMS_STRUCTNAME(USBIF_DEVDESC_WIRE)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_DevDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_DEVICE		*/
	__TMS_UINT8	bcdUSB[2];		/* USB Spec. Release Number	*/
	__TMS_UINT8	bDeviceClass;		/* Class Code			*/
	__TMS_UINT8	bDeviceSubClass;	/* Subclass Code		*/
	__TMS_UINT8	bDeviceProtocol;	/* Protocol code		*/
	__TMS_UINT8	bMaxPacketSize0;	/* max packet size - ep0	*/
	__TMS_UINT8	idVendor[2];		/* vendor ID			*/
	__TMS_UINT8	idProduct[2];		/* product ID			*/
	__TMS_UINT8	bcdDevice[2];		/* Device release number	*/
	__TMS_UINT8	iManufacturer;		/* string desc index for mfg	*/
	__TMS_UINT8	iProduct;		/* string desc index for prod   */
	__TMS_UINT8	iSerialNumber;		/* string desc index for sn	*/
	__TMS_UINT8	bNumConfigurations;	/* number of possible configs   */
	};

/* rather than try to derive it, we just use the number from the spec */
#define __TMS_LENGTHOF_USBIF_DEVDESC_WIRE	18

/*
|| The Configuration Descriptor
||
|| The Configuration descriptor is always sent as a package deal. Not only
|| does the configuration descriptor get sent, but so do all interface
|| and endpoint descriptors that describe this configuration. They are
|| returned in a specific order:
||	Config
||	Interface 0
||		Interface 0 endpoints
||		As needed {
||			Intrface 0 Alt Descriptor
||			Endpoint Descriptors
||			}
||	.
||	.
||	.
||	Interface X
||		Interface X endpoints
||		As needed {
||			Intrface 0 Alt Descriptor
||			Endpoint Descriptors
||			}
*/

struct __TMS_STRUCTNAME(USBIF_CFGDESC)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_CfgDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_CONFIG		*/
	__TMS_UINT16	wTotalLength;		/* sizeof desc. bundle		*/
	__TMS_UINT8	bNumInterfaces;		/* number of interfaces		*/
	__TMS_UINT8	bConfigurationValue;	/* idx of this config		*/
	__TMS_UINT8	iConfiguration;		/* str desc index of config str */
	__TMS_UINT8	bmAttributes;		/* config characteristics	*/
	__TMS_UINT8	MaxPower;		/* max power consumption	*/
	};

struct __TMS_STRUCTNAME(USBIF_CFGDESC_WIRE)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_CfgDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_CONFIG		*/
	__TMS_UINT8	wTotalLength[2];	/* sizeof desc. bundle		*/
	__TMS_UINT8	bNumInterfaces;		/* number of interfaces		*/
	__TMS_UINT8	bConfigurationValue;	/* idx of this config		*/
	__TMS_UINT8	iConfiguration;		/* str desc index of config str */
	__TMS_UINT8	bmAttributes;		/* config characteristics	*/
	__TMS_UINT8	MaxPower;		/* max power consumption	*/
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_CFGDESC_WIRE	9

/*
|| Some needed bit definitions
*/
#define	__TMS_USBIF_CFGDESC_bmAttributes_SelfPowered	0x40u
#define	__TMS_USBIF_CFGDESC_bmAttributes_RemoteWakeup	0x20u

/*
|| The Interface Descriptor
*/
struct __TMS_STRUCTNAME(USBIF_IFCDESC)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_IfcDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_INTERFACE	*/
	__TMS_UINT8	bInterfaceNumber;	/* idx of this interface	*/
	__TMS_UINT8	bAlternateSetting;	/* val to select alt setting	*/
	__TMS_UINT8	bNumEndpoints;		/* number of endpoints this ifc */
	__TMS_UINT8	bInterfaceClass;	/* class code			*/
	__TMS_UINT8	bInterfaceSubClass;	/* subclass code		*/
	__TMS_UINT8	bInterfaceProtocol;	/* protocol code		*/
	__TMS_UINT8	iInterface;		/* str desc idx of ifc string	*/
	};

struct __TMS_STRUCTNAME(USBIF_IFCDESC_WIRE)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_IfcDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_INTERFACE	*/
	__TMS_UINT8	bInterfaceNumber;	/* idx of this interface	*/
	__TMS_UINT8	bAlternateSetting;	/* val to select alt setting	*/
	__TMS_UINT8	bNumEndpoints;		/* number of endpoints this ifc */
	__TMS_UINT8	bInterfaceClass;	/* class code			*/
	__TMS_UINT8	bInterfaceSubClass;	/* subclass code		*/
	__TMS_UINT8	bInterfaceProtocol;	/* protocol code		*/
	__TMS_UINT8	iInterface;		/* str desc idx of ifc string	*/
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_IFCDESC_WIRE	9

/*
|| The Endpoint Descriptor
*/
struct __TMS_STRUCTNAME(USBIF_EPDESC)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_EpDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_ENDPOINT	*/
	__TMS_UINT8	bEndpointAddress;	/* USB address for this endpoint*/
	__TMS_UINT8	bmAttributes;		/* attributes of endpoint	*/
	__TMS_UINT16    wMaxPacketSize;		/* max packet size this ep	*/
	__TMS_UINT8	bInterval;		/* polling interval		*/
	};

struct __TMS_STRUCTNAME(USBIF_EPDESC_WIRE)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_EpDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_ENDPOINT	*/
	__TMS_UINT8	bEndpointAddress;	/* USB address for this endpoint*/
	__TMS_UINT8	bmAttributes;		/* attributes of endpoint	*/
	__TMS_UINT8	wMaxPacketSize[2];	/* max packet size this ep	*/
	__TMS_UINT8	bInterval;		/* polling interval		*/
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_EPDESC_WIRE	7

/*
|| wMaxPacketSize has two bit fields (as of USB 2.0):  bits 10..0 are the
|| size, and bits 12..11 are the # of "extra packets".
*/
#define	__TMS_USBIF_EPDESC_wMaxPacketSize_BASE(v)	((v) & 0x7FF)
#define	__TMS_USBIF_EPDESC_wMaxPacketSize_EXTRA(v)	(((v) & 0x1800) >> 11)

/*
|| bmAttributes has extra fields for isoch for HS, SS, and Wireless
*/
#define	__TMS_USBIF_EPDESC_bmAttributes_TRANSFER(v)	((v) & 0x03)
#define	__TMS_USBIF_EPDESC_bmAttributes_SYNCH(v)	(((v) & 0x0C) >> 2)
#define	__TMS_USBIF_EPDESC_bmAttributes_USAGE(v)	(((v) & 0x30) >> 4)

#define	__TMS_USBIF_EPDESC_bmAttributes_SYNCH_NONE	0
#define	__TMS_USBIF_EPDESC_bmAttributes_SYNCH_ASYNC	1
#define	__TMS_USBIF_EPDESC_bmAttributes_SYNCH_ADAPTIVE	2
#define	__TMS_USBIF_EPDESC_bmAttributes_SYNCH_SYNC	3

#define	__TMS_USBIF_EPDESC_bmAttributes_USAGE_DATA	0
#define	__TMS_USBIF_EPDESC_bmAttributes_USAGE_FEEDBACK	1
#define	__TMS_USBIF_EPDESC_bmAttributes_USAGE_IMPLICIT	2

/*
|| bmAttributes has extra fields for interrupt for HS, SS:
*/
#define	__TMS_USBIF_EPDESC_bmAttributes_USAGE_PERIODIC		0
#define	__TMS_USBIF_EPDESC_bmAttributes_USAGE_NOTIFICATION	1

/*
|| bmAttributes has two extra bits for Wireless; we treat them as
|| capabilities
*/
#define	__TMS_USBIF_EPDESC_bmAttributes_IS_LOW_POWER_INT(v)	\
							(((v) & 0x4) != 0)
#define	__TMS_USBIF_EPDESC_bmAttributes_HAS_DATA_ADJUST(v)	\
							(((v) & 0x80) != 0)



/*
|| The string descriptor is just a simple template so we can get to the
|| first byte of the string data without doing arightmetic.
*/
struct __TMS_STRUCTNAME(USBIF_STRINGDESC)
	{
	__TMS_UINT8	bLength;		/* length of descriptor */
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_STRING */
	__TMS_UINT16	bString[1];		/* USBIF name is bString,
						|| which is wrong, but
						|| that's standards for ya...
						*/
	};

/* an alternate view from the spec */
struct __TMS_STRUCTNAME(USBIF_STRINGDESC_LANGID)
	{
	__TMS_UINT8	bLength;		/* length of descriptor */
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_STRING */
	__TMS_UINT16	wLANGID[1];		/* this is the USBIF name */
	};

/* the wire version */
struct __TMS_STRUCTNAME(USBIF_STRINGDESC_WIRE)
	{
	__TMS_UINT8	bLength;		/* length of descriptor */
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_STRING */
	__TMS_UINT8	bString[1][2];		/* USBIF name is bString,
						|| which is wrong, but
						|| that's standards for ya...
						|| we code as 2 because it's
						|| got to be a multiple of 2
						*/
	};

struct __TMS_STRUCTNAME(USBIF_STRINGDESC_LANGID_WIRE)
	{
	__TMS_UINT8	bLength;		/* length of descriptor */
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_STRING */
	__TMS_UINT8	wLANGID[1][2];		/* this is the USBIF name */
	};

/* we need some macros to help us work with string descs */
#define	__TMS_USBIF_STRINGDESC_WIRE_BODY(pDesc)	((pDesc)->bString[0])

#define	__TMS_USBIF_STRINGDESC_CHARS(pDesc)				\
	(((pDesc)->bLength - sizeof(__TMS_USBIF_DESC_WIRE)) >> 1)
#define	__TMS_USBIF_STRINGDESC_ISVALID(pDesc)				\
	(((pDesc)->bLength >= sizeof(__TMS_USBIF_DESC_WIRE)) &&		\
	 (((pDesc)->bLength - sizeof(__TMS_USBIF_DESC_WIRE)) & 1) == 0)

/*
|| the device qualifier descriptor looks almost (but not quite) unlike
|| a device descriptor
*/
struct __TMS_STRUCTNAME(USBIF_DEVQUALDESC)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_DevDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_DEVICE		*/
	__TMS_UINT16	bcdUSB;			/* USB Spec. Release Number	*/
	__TMS_UINT8	bDeviceClass;		/* Class Code			*/
	__TMS_UINT8	bDeviceSubClass;	/* Subclass Code		*/
	__TMS_UINT8	bDeviceProtocol;	/* Protocol code		*/
	__TMS_UINT8	bMaxPacketSize0;	/* max packet size - ep0	*/
	__TMS_UINT8	bNumConfigurations;	/* number of possible configs   */
	__TMS_UINT8	bReserved;		/* reserved for future use	*/
	};

struct __TMS_STRUCTNAME(USBIF_DEVQUALDESC_WIRE)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_DevDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_DEVICE		*/
	__TMS_UINT8	bcdUSB[2];		/* USB Spec. Release Number	*/
	__TMS_UINT8	bDeviceClass;		/* Class Code			*/
	__TMS_UINT8	bDeviceSubClass;	/* Subclass Code		*/
	__TMS_UINT8	bDeviceProtocol;	/* Protocol code		*/
	__TMS_UINT8	bMaxPacketSize0;	/* max packet size - ep0	*/
	__TMS_UINT8	bNumConfigurations;	/* number of possible configs   */
	__TMS_UINT8	bReserved;		/* reserved for future use	*/
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_DEVQUALDESC_WIRE	10

/*
|| The other speed configuration descriptor looks exactly like a
|| configuration descriptor.
*/
struct __TMS_STRUCTNAME(USBIF_OSCFGDESC)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_CfgDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_OTHER_SPEED_CONFIG	*/
	__TMS_UINT16	wTotalLength;		/* sizeof desc. bundle		*/
	__TMS_UINT8	bNumInterfaces;		/* number of interfaces		*/
	__TMS_UINT8	bConfigurationValue;	/* idx of this config		*/
	__TMS_UINT8	iConfiguration;		/* str desc index of config str */
	__TMS_UINT8	bmAttributes;		/* config characteristics	*/
	__TMS_UINT8	MaxPower;		/* max power consumption	*/
	};

struct __TMS_STRUCTNAME(USBIF_OSCFGDESC_WIRE)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_CfgDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_OTHER_SPEED_CONFIG		*/
	__TMS_UINT8	wTotalLength[2];	/* sizeof desc. bundle		*/
	__TMS_UINT8	bNumInterfaces;		/* number of interfaces		*/
	__TMS_UINT8	bConfigurationValue;	/* idx of this config		*/
	__TMS_UINT8	iConfiguration;		/* str desc index of config str */
	__TMS_UINT8	bmAttributes;		/* config characteristics	*/
	__TMS_UINT8	MaxPower;		/* max power consumption	*/
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_OSCFGDESC_WIRE	9

/*
|| No layout for an interface power descriptor.
*/

/*
|| Layout for the OnTheGo descriptor
*/
struct __TMS_STRUCTNAME(USBIF_OTGDESC)
	{
	__TMS_UINT8	bLength;		/* LENGTHOF_USBIF_OTGDESC_WIRE	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_OTG		*/
	__TMS_UINT8	bmAttributes;		/* attributes */
	};

#define	__TMS_USBIF_OTGDESC_ATTR_SRP	(0x01)	/* if set, supports SRP */
#define	__TMS_USBIF_OTGDESC_ATTR_HNP	(0x02)	/* if set, supports HNP */

struct __TMS_STRUCTNAME(USBIF_OTGDESC_WIRE)
	{
	__TMS_UINT8	bLength;		/* LENGTHOF_USBIF_OTGDESC_WIRE	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_OTG		*/
	__TMS_UINT8	bmAttributes;		/* attributes */
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_OTGDESC_WIRE	3


/*
|| No published info on the DEBUG desc
*/

/*
|| Interface Association Descriptor
*/
struct __TMS_STRUCTNAME(USBIF_IADESC)
	{
	__TMS_UINT8	bLength;		/* LENGTHOF_USBIF_IADESC_WIRE	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_INTEFACE_ASSOCIATION */
	__TMS_UINT8	bFirstInterface;	/* interface number of first
						|| interface that is associated with
						|| this function
						*/
	__TMS_UINT8	bInterfaceCount;	/* number of contiguous interfaces
						|| that are associated with the
						|| function.
						*/
	__TMS_UINT8	bFunctionClass;		/* class code */
	__TMS_UINT8	bFunctionSubClass;	/* subclass code */
	__TMS_UINT8	bFunctionProtocol;	/* protocol code */
	__TMS_UINT8	iFunction;		/* index of string descriptor
						|| describing this function.
						*/
	};

struct __TMS_STRUCTNAME(USBIF_IADESC_WIRE)
	{
	__TMS_UINT8	bLength;		/* LENGTHOF_USBIF_IADESC_WIRE	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_INTERFACE_ASSOCIATION */
	__TMS_UINT8	bFirstInterface;	/* interface number of first
						|| interface that is associated with
						|| this function
						*/
	__TMS_UINT8	bInterfaceCount;	/* number of contiguous interfaces
						|| that are associated with the
						|| function.
						*/
	__TMS_UINT8	bFunctionClass;		/* class code */
	__TMS_UINT8	bFunctionSubClass;	/* subclass code */
	__TMS_UINT8	bFunctionProtocol;	/* protocol code */
	__TMS_UINT8	iFunction;		/* index of string descriptor
						|| describing this function.
						*/
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_IADESC_WIRE	8


/*
|| The generic Descriptor
*/
struct __TMS_STRUCTNAME(USBIF_DESC)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_DevDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_DEVICE		*/
	};

struct __TMS_STRUCTNAME(USBIF_DESC_WIRE)
	{
	__TMS_UINT8	bLength;		/* sizeof(struct USB_DevDesc)	*/
	__TMS_UINT8	bDescriptorType;	/* USB_bDescType_DEVICE		*/
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_DESC_WIRE	2


/* WUSB Binary device Object Store Descriptor */
struct __TMS_STRUCTNAME(USBIF_BOSDESC)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_BOSDESC) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_BOS */
	__TMS_UINT16	wTotalLength;	/*
					|| Total length of all the capabilities
					|| descriptors
					*/
	__TMS_UINT8	bNumDeviceCaps;	/*
					|| Count of device capability descriptor
					*/
	};

struct __TMS_STRUCTNAME(USBIF_BOSDESC_WIRE)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_BOSDESC_WIRE) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_BOS */
	__TMS_UINT8	wTotalLength[2];/*
					|| Total length of all the capabilities
					|| descriptors
					*/
	__TMS_UINT8	bNumDeviceCaps;	/*
					|| Count of device capability descriptor
					*/
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_BOSDESC_WIRE	5


/* WUSB The Security Descriptor */
struct __TMS_STRUCTNAME(USBIF_SECURITYDESC)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_SECURITYDESC) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_SECURITY */
	__TMS_UINT16	wTotalLength;		/*
						|| Total length of all the
						|| Encryption descriptors
						*/
	__TMS_UINT8	bNumEncryptionTypes;	/*
						|| Count of Encryption Types
						|| descriptor
						*/
	};

struct __TMS_STRUCTNAME(USBIF_SECURITYDESC_WIRE)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_SECURITYDESC_WIRE) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_SECURITY */
	__TMS_UINT8	wTotalLength[2];	/*
						|| Total length of all the
						|| Encryption descriptors
						*/
	__TMS_UINT8	bNumEncryptionTypes;	/*
						|| Count of Encryption Types
						|| descriptor
						*/
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_SECURITYDESC_WIRE	5

/* Encryption Type */
#define __TMS_USBIF_SECURITYDESC_bNumEncryptionTypes_UNSECURE	0
#define __TMS_USBIF_SECURITYDESC_bNumEncryptionTypes_WIRED	1
#define __TMS_USBIF_SECURITYDESC_bNumEncryptionTypes_CCM_1	2 /* AES_128 */


/* WUSB The Encryption Type Descriptor */
struct __TMS_STRUCTNAME(USBIF_ENCRYPYDESC)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_ENCRYPYDESC) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_ENCRYPTION_TYPE */
	__TMS_UINT8	bEncryptionType;  /* Encryption Type */
	__TMS_UINT8	bEncryptionValue; 	/*
						|| Value to be used to enable
						|| this encryption
						*/
	__TMS_UINT8	bAuthKeyIndex;		/*
						|| Non zero value can be
						|| specified to indicate that
						|| key can be used in Connection
						*/
	};

struct __TMS_STRUCTNAME(USBIF_ENCRYPYDESC_WIRE)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_ENCRYPYDESC_WIRE) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_ENCRYPTION_TYPE */
	__TMS_UINT8	bEncryptionType;  /* Encryption Type */
	__TMS_UINT8	bEncryptionValue;	/*
						|| Value to be used to enable
						|| this encryption
						*/
	__TMS_UINT8	bAuthKeyIndex;		/*
						|| Non zero value can be
						|| specified to indicate that
						|| key can be used in Connection
						*/
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_ENCRYPYDESC_WIRE	5


/* WUSB The Key Descriptor */
struct __TMS_STRUCTNAME(USBIF_KEYDESC)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_KEYDESC) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_KEY */
	__TMS_UINT32	tTKID;		  /*
					  || TKID if Present
					  || Its size is 3 bytes only.
					  || But in this structure we are using
					  || 4 bytes for ease of usage
					  */
	__TMS_UINT8	bReserved;
	__TMS_UINT8	bKeyData[1];	  /* Place holder for Key Data */
	};

struct __TMS_STRUCTNAME(USBIF_KEYDESC_WIRE)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_KEYDESC_WIRE) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_KEY */
	__TMS_UINT8	tTKID[3];	  /* TKID if Present */
	__TMS_UINT8	bReserved;
	__TMS_UINT8	bKeyData[1];	  /* Place holder for Key Data */
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
|| Note: The last byte has not been taken in to consideration for size
|| Actual size should be determined from the bLength Field
*/
#define __TMS_LENGTHOF_USBIF_KEYDESC_WIRE	6


/*
|| The Wireless Endpoint Companion Descriptor
|| This descriptor should immediately follow the
|| EP descriptor, it is associated with
|| EP 0 should not have Companion descriptor
*/
struct __TMS_STRUCTNAME(USBIF_WEP_COMPANION_DESC)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_WEP_COMPANION_DESC) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_WEP_COMPANION */
	__TMS_UINT8	bMaxBurst;
				/*
				|| For Bulk and ISO it can be 1-16
				|| For Interrupt and Control it should be 1
				*/
	__TMS_UINT8	bMaxSequence;  /* Should be a value between 2-32 */
	__TMS_UINT16	wMaxStreamDelay;
				/*
				|| For ISO it can be 1-65535
				|| For others it should be 0
				*/
	__TMS_UINT16	wOverTheAirPacketSize;
				/*
				|| For ISO it should have appropriate size
				|| if possible should be same
				|| as wMaxPacketSize
				|| For others it should be 0
				|| src WUSB 1.0, table 7-32
				*/
	__TMS_UINT8	bOverTheAirInterval;
				/*
				|| For ISO it will be 'X' unit of 128 micro
				|| second interval
				|| For others it should be 0
				|| src WUSB 1.0, table 7-32
				*/
	__TMS_UINT8	bmCompAttributes;
	};

struct __TMS_STRUCTNAME(USBIF_WEP_COMPANION_DESC_WIRE)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_WEP_COMPANION_DESC_WIRE) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_WEP_COMPANION */
	__TMS_UINT8	bMaxBurst;
				/*
				|| For Bulk and ISO it can be 1-16
				|| For Interrupt and Control it should be 1
				*/
	__TMS_UINT8	bMaxSequence;  /* Should be a value between 2-32 */
	__TMS_UINT8	wMaxStreamDelay[2];
				/*
				|| For ISO it can be 1-65535
				|| For others it should be 0
				*/
	__TMS_UINT8	wOverTheAirPacketSize[2];
				/*
				|| For ISO it should have appropriate size
				|| if possible should be same
				|| as wMaxPacketSize
				|| For others it should be 0
				|| src WUSB 1.0, table 7-32
				*/
	__TMS_UINT8	bOverTheAirInterval;
				/*
				|| For ISO it will be 'X' unit of 128 micro
				|| second interval
				|| For others it should be 0
				|| src WUSB 1.0, table 7-32
				*/
	__TMS_UINT8	bmCompAttributes;
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_WEP_COMPANION_DESC_WIRE	10

/***** Endpoint Companion attribute codes *****/
#define __TMS_USBIF_WEP_COMPANION_DESC_bmCompAttributes_NO_SWITCH	0
#define __TMS_USBIF_WEP_COMPANION_DESC_bmCompAttributes_DYNAMIC_SWITCH	1
#define __TMS_USBIF_WEP_COMPANION_DESC_bmCompAttributes_DYNAMIC_SWITCH_SCALABLE 2


/*
|| Device Capability Descriptor
*/
struct __TMS_STRUCTNAME(USBIF_DEVCAPDESC)
	{
	__TMS_UINT8	bLength;	     /* sizeof(struct USBIF_DEVCAPDESC) */
	__TMS_UINT8	bDescriptorType;     /* USB_bDescType_DEVICE_CAPABILITY */
	__TMS_UINT8	bDevCapabilityType;  
	__TMS_UINT8	bDevCapData[1];	     /* Place holder for Device Capability Data */
	};

struct __TMS_STRUCTNAME(USBIF_DEVCAPDESC_WIRE)
	{
	__TMS_UINT8	bLength;	     /* sizeof(struct USBIF_DEVCAPDESC_WIRE) */
	__TMS_UINT8	bDescriptorType;     /* USB_bDescType_DEVICE_CAPABILITY */
	__TMS_UINT8	bDevCapabilityType;  
	__TMS_UINT8	bDevCapData[1];	     /* Place holder for Device Capability Data */
	};

/***** Device Capability Type codes *****/
#define __TMS_USBIF_DEVCAPDESC_bDevCapabilityType_WUSB			0x01u
#define __TMS_USBIF_DEVCAPDESC_bDevCapabilityType_USB20EXTENSION	0x02u
#define __TMS_USBIF_DEVCAPDESC_bDevCapabilityType_SUPERSPEED_USB	0x03u
#define __TMS_USBIF_DEVCAPDESC_bDevCapabilityType_CONTAINER_ID		0x04u 

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
|| Note: The last byte has not been taken in to consideration for size
|| Actual size should be determined from the bLength Field
*/
#define __TMS_LENGTHOF_USBIF_DEVCAPDESC_WIRE	3


/*
|| The Wireless Device Capability Descriptor
*/
struct __TMS_STRUCTNAME(USBIF_WUSB_DEVCAPDESC)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_WUSB_DEVCAPDESC) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_DEVICE_CAPABILITY */
	__TMS_UINT8	bDevCapabilityType; /* USBIF_DEVCAPDESC_bDevCapabilityType_WUSB */
	__TMS_UINT8	bmAttributes;
					/* Bit 	Encoding
					|| 0	Must be 0
					|| 1	P2P -DRD Peer 2 Peer DRD capable
					|| 3:2
					||	00 - Reserved
					|| 	01 - Self-Beacon
					|| 	10 - Directed Beacon
					|| 	11 - No Beacon
					|| 7:4	0 Reserved
					|| src WUSB 1.0, table 7-29
					*/
	__TMS_UINT16	wPHYRates;
					/* BitPos Rate
					|| 0	53.3*
					|| 1	80
					|| 2	106.7*
					|| 3	160
					|| 4	200*
					|| 5	320
					|| 6	400
					|| 7	480
					|| 15:8 Reserved must be 0
					|| * Are mandatory to support
					|| src WUSB 1.0, table 7-29
					*/
	__TMS_UINT8	bmTFITXPowerInfo;
					/*
					|| PHY Transmit Power Info
					|| on TFI Channel
					|| Bit Pos
					|| 3:0	power level steps supported
					|| 7:4  Step Size (dB)
					||	0	1.0
					||	1	1.25
					||	2	1.5
					||	3	1.75
					||	4	2.0
					||	5	2.25
					||	6	2.5
					||	7	2.75
					||	8	3.0
					||	9	3.25
					||	10	3.5
					||	11	3.75
					||	12	4.0
					||	13	4.25
					||	14	4.50
					||	15	4.75
					|| src WUSB 1.0, table 7-29
					*/
	__TMS_UINT8	bmFFITXPowerInfo;
					/*
					|| PHY Transmit Power Info
					|| on FFI Channel
					|| same as above
					*/
	__TMS_UINT16	bmBandGroup;
					/*
					|| UWB band group supported
					|| Currently 5 band groups are supported
					|| If a band is supported, an implementation
					|| should support all the channel of that group
					|| Band group 1 is mandatory for
					|| any implementation (src WUSB 1.0 sec 3.2)
					|| Channel Number
					|| Range	    Band Group, TF code
					|| 0-8		      Reserved
					|| 9-15		      1, 1-7
					|| 17-23	      2, 1-7
					|| 25-31	      3, 1-7
					|| 33-39	      4, 1-7
					|| 45-46	      5, 5-6
					|| (src WUSB 1.0 table 5-12)
					||
					|| Bit Pos
					||  4:0		1 indicate all of the
					||		channel in associated
					||		PHY band are supported
					|| 15:5		Reserved - 0
					|| src WUSB 1.0, table 7-29
					*/
	__TMS_UINT8	bReserved;
	};

struct __TMS_STRUCTNAME(USBIF_WUSB_DEVCAPDESC_WIRE)
	{
	__TMS_UINT8	bLength;	  /* sizeof(struct USBIF_WUSB_DEVCAPDESC_WIRE) */
	__TMS_UINT8	bDescriptorType;  /* USB_bDescType_DEVICE_CAPABILITY */
	__TMS_UINT8	bDevCapabilityType; /* USBIF_DEVCAPDESC_bDevCapabilityType_WUSB */
	__TMS_UINT8	bmAttributes;
					/* Bit 	Encoding
					|| 0	Must be 0
					|| 1	P2P -DRD Peer 2 Peer DRD capable
					|| 3:2
					||	00 - Reserved
					|| 	01 - Self-Beacon
					|| 	10 - Directed Beacon
					|| 	11 - No Beacon
					|| 7:4	0 Reserved
					|| src WUSB 1.0, table 7-29
					*/
	__TMS_UINT8	wPHYRates[2];
					/* BitPos Rate
					|| 0	53.3*
					|| 1	80
					|| 2	106.7*
					|| 3	160
					|| 4	200*
					|| 5	320
					|| 6	400
					|| 7	480
					|| 15:8 Reserved must be 0
					|| * Are mandatory to support
					|| src WUSB 1.0, table 7-29
					*/
	__TMS_UINT8	bmTFITXPowerInfo;
					/*
					|| PHY Transmit Power Info
					|| on TFI Channel
					|| Bit Pos
					|| 3:0	power level steps supported
					|| 7:4  Step Size (dB)
					||	0	1.0
					||	1	1.25
					||	2	1.5
					||	3	1.75
					||	4	2.0
					||	5	2.25
					||	6	2.5
					||	7	2.75
					||	8	3.0
					||	9	3.25
					||	10	3.5
					||	11	3.75
					||	12	4.0
					||	13	4.25
					||	14	4.50
					||	15	4.75
					|| src WUSB 1.0, table 7-29
					*/
	__TMS_UINT8	bmFFITXPowerInfo;
					/*
					|| PHY Transmit Power Info
					|| on FFI Channel
					|| same as above
					*/
	__TMS_UINT8	bmBandGroup[2];
					/*
					|| UWB band group supported
					|| Currently 5 band groups are supported
					|| If a band is supported, an implementation
					|| should support all the channel of that group
					|| Band group 1 is mandatory for
					|| any implementation (src WUSB 1.0 sec 3.2)
					|| Channel Number
					|| Range	    Band Group, TF code
					|| 0-8		      Reserved
					|| 9-15		      1, 1-7
					|| 17-23	      2, 1-7
					|| 25-31	      3, 1-7
					|| 33-39	      4, 1-7
					|| 45-46	      5, 5-6
					|| (src WUSB 1.0 table 5-12)
					||
					|| Bit Pos
					||  4:0		1 indicate all of the
					||		channel in associated
					||		PHY band are supported
					|| 15:5		Reserved - 0
					|| src WUSB 1.0, table 7-29
					*/
	__TMS_UINT8	bReserved;
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_WUSB_DEVCAPDESC_WIRE	11

/*
|| Some needed bit definitions for WUSB device capability descriptor
*/
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_P2P_DRD	0x02u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_Beacon_MASK	0x0Cu
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_BEACON(v)	\
	((v) & __TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_Beacon_MASK)
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_Self_Beacon	0x04u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_Directed_Beacon 0x08u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_No_Beacon	0x0Cu

#define	__TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_53_3		0x01u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_80		0x02u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_106_7		0x04u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_160		0x08u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_200		0x10u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_320		0x20u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_400		0x40u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_480		0x80u

#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_0	0x00u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_25	0x10u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_5	0x20u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_75	0x30u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_0	0x40u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_25	0x50u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_5	0x60u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_75	0x70u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_0	0x80u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_25	0x90u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_5	0xA0u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_75	0xB0u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_0	0xC0u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_25	0xD0u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_5	0xE0u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_75	0xF0u

#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group1		0x01u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group2		0x02u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group3		0x04u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group4		0x08u
#define	__TMS_USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group5		0x10u

/*
|| SuperSpeed Endpoint Companion Descriptor
|| This descriptor should immediately follow the
|| EP descriptor it is associated with.
|| EP 0 should not have Companion descriptor.
*/
struct __TMS_STRUCTNAME(USBIF_SUPERSPEED_EP_COMPANION_DESC)
	{
	__TMS_UINT8	bLength;	 
	__TMS_UINT8	bDescriptorType;  
				/* USB_bDescType_SUPERSPEED_EP_COMPANION */
	__TMS_UINT8	bMaxBurst;
				/*
				|| Can be 0 to 15. 
				|| 0: it can burst 1 packet at a time.
				|| 15: it can burst 16 packets at a time.
				|| It shall be set to 0, for Control EP.
				*/
	__TMS_UINT8	bmAttributes;
				/*
				|| Bulk Endpoint:
				||  Bits Description
				||  4:0  MaxStreams. Valid values are 0 ~ 16.
				||       0 == indicates no streams.
				||       1~16 == 2 ^ MaxStreams
				||  7:5  Reserved and shall be set to zero.
				||
				|| Control or Interrupt Endpoint:
				||  Bits Description
				||  7:0  Reserved and shall be set to zero.
				||
				|| Isochronous Endpoint:
				||  Bits Description
				||  1:0  Mult. Maximum value is 2.
				||       Max number of packets =
				||           bMaxBurst * (Mult + 1)
				||  7:2  Reserved and shall be set to zero.
				*/
	__TMS_UINT16	wBytesPerInterval;
				/*
				|| Total number of bytes this EP will transfer
				|| every service interval. Only valid for 
				|| periodic endpoints.
				*/ 
	};

struct __TMS_STRUCTNAME(USBIF_SUPERSPEED_EP_COMPANION_DESC_WIRE)
	{
	__TMS_UINT8	bLength;	
	__TMS_UINT8	bDescriptorType;  
				/* USB_bDescType_SUPERSPEED_EP_COMPANION */ 
	__TMS_UINT8	bMaxBurst;
				/*
				|| Can be 0 to 15. 
				|| 0: it can burst 1 packet at a time.
				|| 15: it can burst 16 packets at a time.
				|| It shall be set to 0, for Control EP.
				*/
	__TMS_UINT8	bmAttributes;
				/*
				|| Bulk Endpoint:
				||  Bits Description
				||  4:0  MaxStreams. Valid values are 0 ~ 16.
				||       0 == indicates no streams.
				||       1~16 == 2 ^ MaxStreams
				||  7:5  Reserved and shall be set to zero.
				||
				|| Control or Interrupt Endpoint:
				||  Bits Description
				||  7:0  Reserved and shall be set to zero.
				||
				|| Isochronous Endpoint:
				||  Bits Description
				||  1:0  Mult. Maximum value is 2.
				||       Max number of packets =
				||           bMaxBurst * (Mult + 1)
				||  7:2  Reserved and shall be set to zero.
				*/
	__TMS_UINT8	wBytesPerInterval[2];
				/*
				|| Total number of bytes this EP will transfer
				|| every service interval. Only valid for 
				|| periodic endpoints.
				*/ 
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_SUPERSPEED_EP_COMPANION_DESC_WIRE	6

/***** SuperSpeed Endpoint Companion bMaxBurst validation *****/
#define __TMS_USBIF_SUPERSPEED_EP_COMPANION_DESC_bMaxBurst_MASK	0x0Fu
#define __TMS_USBIF_SUPERSPEED_EP_COMPANION_DESC_bMaxBurst_IS_VALID(v)	\
		((v) < 16)

/***** SuperSpeed Endpoint Companion attribute codes *****/
/* for Bulk EP */
#define __TMS_USBIF_SUPERSPEED_EP_COMPANION_DESC_bmAttributes_LG2MAXSTREAMS(v) \
		((v) & 0x1F)

#define __TMS_USBIF_SUPERSPEED_EP_COMPANION_DESC_LG2MAXSTREAMS_GETMAXID(v) \
	((v) == 0 ? 0 :							\
	 (v) < 16 ? 1 << (v) :						\
		    65533)

/* for Isochronous EP */
#define __TMS_USBIF_SUPERSPEED_EP_COMPANION_DESC_bmAttributes_MULT(v) \
		((v) & 0x03)

/*
|| The SuperSpeed USB Device Capability Descriptor
*/
struct __TMS_STRUCTNAME(USBIF_SUPERSPEED_DEVCAPDESC)
	{
	__TMS_UINT8	bLength;	    /* sizeof(struct USBIF_SUPERSPEED_DEVCAPDESC) */
	__TMS_UINT8	bDescriptorType;    /* USB_bDescType_DEVICE_CAPABILITY */
	__TMS_UINT8	bDevCapabilityType; /* USBIF_DEVCAPDESC_bDevCapabilityType_SUPERSPEED_USB */
	__TMS_UINT8	bmAttributes;
					/* 
					|| Bit 	  Encoding
					|| 0	  Reserved
					|| 1	  LTM Capable
					|| 7:2    Reserved
					*/
	__TMS_UINT16	wSpeedsSupported;
					/* 
					|| Bit    Rate
					|| 0      low-speed
					|| 1      full-speed
					|| 2      high-speed
					|| 3      5 Gbps
					|| 15:4   Reserved
					*/
	__TMS_UINT8	bFunctionalitySupport;
					/*
					|| The lowest speed at which all the
					|| functionality supported by the device
					|| is available to the user. Refer to
					|| wSpeedsSupported field for valid 
					|| values.
					*/
	__TMS_UINT8	bU1DevExitLat;
					/*
					|| U1 Device Exit Latency. Worst case
					|| latency to transition from U1 to U0,
					|| assuming the latency is limited only
					|| by the device and not the device's 
					|| link partner.
					|| This field applies only to the exit
					|| latency associated with an individual
					|| port, and does not apply to total
					|| latency through a hub (e.g. from
					|| downstream port to upstream port).
					||
					|| Value    Meaning
					|| 00H      Zero
					|| 01H      Less than 1 microsecond
					|| 02H      Less than 2 microseconds
					|| 03H      Less than 3 microseconds
					|| 04H      Less than 4 microseconds
					|| ...
					|| 0AH      Less than 10 micorseconds
					|| 0BH -    Reserved
					|| FFH
					||
					|| For a hub, this is the value for both
					|| its upstream and downstream ports.
					*/ 
	__TMS_UINT16	wU2DevExitLat;
					/*
					|| U2 Device Exit Latency. Worst case
					|| latency to transition from U2 to U0,
					|| assuming the latency is limited only
					|| by the device and not the device's 
					|| link partner. Applies to all ports on 
					|| a device.
					||
					|| Value    Meaning
					|| 00H      Zero
					|| 01H      Less than 1 microsecond
					|| 02H      Less than 2 microseconds
					|| 03H      Less than 3 microseconds
					|| 04H      Less than 4 microseconds
					|| ...
					|| 0AH      Less than 10 micorseconds
					|| 0BH -    Reserved
					|| FFH
					||
					|| For a hub, this is the value for both
					|| its upstream and downstream ports.
					*/
	};

struct __TMS_STRUCTNAME(USBIF_SUPERSPEED_DEVCAPDESC_WIRE)
	{
	__TMS_UINT8	bLength;	    /* sizeof(struct USBIF_SUPERSPEED_DEVCAPDESC) */
	__TMS_UINT8	bDescriptorType;    /* USB_bDescType_DEVICE_CAPABILITY */
	__TMS_UINT8	bDevCapabilityType; /* USBIF_DEVCAPDESC_bDevCapabilityType_SUPERSPEED_USB */
	__TMS_UINT8	bmAttributes;
					/* 
					|| Bit 	  Encoding
					|| 0	  Reserved
					|| 1	  LTM Capable
					|| 7:2    Reserved
					*/
	__TMS_UINT8	wSpeedsSupported[2];
					/* 
					|| Bit    Rate
					|| 0      low-speed
					|| 1      full-speed
					|| 2      high-speed
					|| 3      5 Gbps
					|| 15:4   Reserved
					*/
	__TMS_UINT8	bFunctionalitySupport;
					/*
					|| The lowest speed at which all the
					|| functionality supported by the device
					|| is available to the user. Refer to
					|| wSpeedsSupported field for valid 
					|| values.
					*/
	__TMS_UINT8	bU1DevExitLat;
					/*
					|| U1 Device Exit Latency. Worst case
					|| latency to transition from U1 to U0,
					|| assuming the latency is limited only
					|| by the device and not the device's 
					|| link partner.
					|| This field applies only to the exit
					|| latency associated with an individual
					|| port, and does not apply to total
					|| latency through a hub (e.g. from
					|| downstream port to upstream port).
					||
					|| Value    Meaning
					|| 00H      Zero
					|| 01H      Less than 1 microsecond
					|| 02H      Less than 2 microseconds
					|| 03H      Less than 3 microseconds
					|| 04H      Less than 4 microseconds
					|| ...
					|| 0AH      Less than 10 micorseconds
					|| 0BH -    Reserved
					|| FFH
					||
					|| For a hub, this is the value for both
					|| its upstream and downstream ports.
					*/ 
	__TMS_UINT8	wU2DevExitLat[2];
					/*
					|| U2 Device Exit Latency. Worst case
					|| latency to transition from U2 to U0,
					|| assuming the latency is limited only
					|| by the device and not the device's 
					|| link partner. Applies to all ports on 
					|| a device.
					||
					|| Value    Meaning
					|| 00H      Zero
					|| 01H      Less than 1 microsecond
					|| 02H      Less than 2 microseconds
					|| 03H      Less than 3 microseconds
					|| 04H      Less than 4 microseconds
					|| ...
					|| 0AH      Less than 10 micorseconds
					|| 0BH -    Reserved
					|| FFH
					||
					|| For a hub, this is the value for both
					|| its upstream and downstream ports.
					*/
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_SUPERSPEED_DEVCAPDESC_WIRE	10

/***** SuperSpeed Device Capability attribute codes *****/
#define __TMS_USBIF_SUPERSPEED_DEVCAPDESC_bmAttributes_LTM	0x02

/**** SuperSpeed Device Capability speeds supported ****/
#define __TMS_USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_LOW(v) \
		((v) & 0x01)
#define __TMS_USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_FULL(v) \
		((v) & 0x02)
#define __TMS_USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_HIGH(v) \
		((v) & 0x04)
#define __TMS_USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_SUPER(v) \
		((v) & 0x08)

/*
|| USB 2.0 Extension Device Capability Descriptor
*/
struct __TMS_STRUCTNAME(USBIF_EXTENSION_DEVCAPDESC)
	{
	__TMS_UINT8	bLength;	 
				/* USB_bDescType_DEVICE_CAPABILITY */
	__TMS_UINT8	bDescriptorType;  
				/* USBIF_DEVCAPDESC_bDevCapabilityType_USB20EXTENSION */
	__TMS_UINT8	bDevCapabilityType; 
				/* 
				|| Bit 	  Encoding
				|| 0	  Reserved
				|| 1	  LPM support
				|| 31:2   Reserved
				*/
	__TMS_UINT32	bmAttributes;
	};

struct __TMS_STRUCTNAME(USBIF_EXTENSION_DEVCAPDESC_WIRE)
	{
	__TMS_UINT8	bLength;	
				/* USB_bDescType_DEVICE_CAPABILITY */
	__TMS_UINT8	bDescriptorType;  
				/* USBIF_DEVCAPDESC_bDevCapabilityType_USB20EXTENSION */
	__TMS_UINT8	bDevCapabilityType;
				/* 
				|| Bit	 Encoding
				|| 0	 Reserved
				|| 1	 LPM support
				|| 2	 BESL & Alternate HIRD definition support
				|| 3	 Recommended Baseline BESL valid
				|| 4	 Recommended Deep BESL valid
				|| 11:8	 Recommended Baseline BESL value
				|| 15:12 Recommended Deep BESL value
				|| 31:16 Reserved
				*/
	__TMS_UINT8	bmAttributes[4];
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_EXTENSION_DEVCAPDESC_WIRE		7

/***** USB 2.0 Extension Device Capability attribute codes *****/
#define __TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_LPM	0x02
#define	__TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_BESL	0x04
#define	__TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_BASELINE_BESL 0x08
#define	__TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_DEEP_BESL	0x10
#define	__TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_BASELINE_BESL_VAL(v) \
	(((v) >> 8) & 0x0f)
#define	__TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_DEEP_BESL_VAL(v) \
	(((v) >> 12) & 0x0f)

/*
|| Container ID Device Capability Descriptor
*/
struct __TMS_STRUCTNAME(USBIF_CONTAINER_ID_DEVCAPDESC)
	{
	__TMS_UINT8	bLength;	 
	__TMS_UINT8	bDescriptorType;  
				/* USB_bDescType_DEVICE_CAPABILITY */
	__TMS_UINT8	bDevCapabilityType; 
				/* USBIF_DEVCAPDESC_bDevCapabilityType_CONTAINER_ID */
	__TMS_UINT8	bReserved;
	__TMS_USBUUID	ContainerID;
				/* This is a 128-bit number that is unique to a 
				|| device instance that is used to uniquely
				|| identify the device instance accross all 
				|| modes of operation. This same value may be
				|| provided over other technologies as well to
				|| allow the host to identify the device
				|| independent of means of connectivity.
				|| Refer to IETF RFC 1422 for details on 
				|| generation of UUID.
				*/
	};

struct __TMS_STRUCTNAME(USBIF_CONTAINER_ID_DEVCAPDESC_WIRE)
	{
	__TMS_UINT8	bLength;	 
	__TMS_UINT8	bDescriptorType;  
				/* USB_bDescType_DEVICE_CAPABILITY */
	__TMS_UINT8	bDevCapabilityType; 
				/* USBIF_DEVCAPDESC_bDevCapabilityType_CONTAINER_ID */
	__TMS_UINT8	bReserved;
	__TMS_USBUUID_WIRE	ContainerID;
				/* This is a 128-bit number that is unique to a 
				|| device instance that is used to uniquely
				|| identify the device instance accross all 
				|| modes of operation. This same value may be
				|| provided over other technologies as well to
				|| allow the host to identify the device
				|| independent of means of connectivity.
				|| Refer to IETF RFC 1422 for details on 
				|| generation of UUID.
				*/
	
	};

/*
|| rather than try to derive it, we just use the number from the spec --
|| sizeof() doesn't work due to compiler portability issues.
*/
#define __TMS_LENGTHOF_USBIF_CONTAINER_ID_DEVCAPDESC_WIRE		20

/****************************************************************************\
|
|	Uncloaked Names
|
\****************************************************************************/

/**** some compilers don't like it if this is undefined ****/
#ifndef __TMS_CLOAKED_NAMES_ONLY
# define __TMS_CLOAKED_NAMES_ONLY 0
#endif

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define LENGTHOF_USBIF_DEVDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_DEVDESC_WIRE
# define LENGTHOF_USBIF_CFGDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_CFGDESC_WIRE
# define USBIF_CFGDESC_bmAttributes_SelfPowered	\
   __TMS_USBIF_CFGDESC_bmAttributes_SelfPowered
# define USBIF_CFGDESC_bmAttributes_RemoteWakeup	\
   __TMS_USBIF_CFGDESC_bmAttributes_RemoteWakeup
# define LENGTHOF_USBIF_IFCDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_IFCDESC_WIRE
# define LENGTHOF_USBIF_EPDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_EPDESC_WIRE
# define USBIF_EPDESC_wMaxPacketSize_BASE(v)	\
   __TMS_USBIF_EPDESC_wMaxPacketSize_BASE(v)
# define USBIF_EPDESC_wMaxPacketSize_EXTRA(v)	\
   __TMS_USBIF_EPDESC_wMaxPacketSize_EXTRA(v)
# define USBIF_EPDESC_bmAttributes_TRANSFER(v)	\
   __TMS_USBIF_EPDESC_bmAttributes_TRANSFER(v)
# define USBIF_EPDESC_bmAttributes_SYNCH(v)	\
   __TMS_USBIF_EPDESC_bmAttributes_SYNCH(v)
# define USBIF_EPDESC_bmAttributes_USAGE(v)	\
   __TMS_USBIF_EPDESC_bmAttributes_USAGE(v)
# define USBIF_EPDESC_bmAttributes_SYNCH_NONE	\
   __TMS_USBIF_EPDESC_bmAttributes_SYNCH_NONE
# define USBIF_EPDESC_bmAttributes_SYNCH_ASYNC	\
   __TMS_USBIF_EPDESC_bmAttributes_SYNCH_ASYNC
# define USBIF_EPDESC_bmAttributes_SYNCH_ADAPTIVE	\
   __TMS_USBIF_EPDESC_bmAttributes_SYNCH_ADAPTIVE
# define USBIF_EPDESC_bmAttributes_SYNCH_SYNC	\
   __TMS_USBIF_EPDESC_bmAttributes_SYNCH_SYNC
# define USBIF_EPDESC_bmAttributes_USAGE_DATA	\
   __TMS_USBIF_EPDESC_bmAttributes_USAGE_DATA
# define USBIF_EPDESC_bmAttributes_USAGE_FEEDBACK	\
   __TMS_USBIF_EPDESC_bmAttributes_USAGE_FEEDBACK
# define USBIF_EPDESC_bmAttributes_USAGE_IMPLICIT	\
   __TMS_USBIF_EPDESC_bmAttributes_USAGE_IMPLICIT
# define USBIF_EPDESC_bmAttributes_USAGE_PERIODIC	\
   __TMS_USBIF_EPDESC_bmAttributes_USAGE_PERIODIC
# define USBIF_EPDESC_bmAttributes_USAGE_NOTIFICATION	\
   __TMS_USBIF_EPDESC_bmAttributes_USAGE_NOTIFICATION
# define USBIF_EPDESC_bmAttributes_IS_LOW_POWER_INT(v)	\
   __TMS_USBIF_EPDESC_bmAttributes_IS_LOW_POWER_INT(v)
# define USBIF_EPDESC_bmAttributes_HAS_DATA_ADJUST(v)	\
   __TMS_USBIF_EPDESC_bmAttributes_HAS_DATA_ADJUST(v)
# define USBIF_STRINGDESC_WIRE_BODY(pDesc)	\
   __TMS_USBIF_STRINGDESC_WIRE_BODY(pDesc)
# define USBIF_STRINGDESC_CHARS(pDesc)	\
   __TMS_USBIF_STRINGDESC_CHARS(pDesc)
# define USBIF_STRINGDESC_ISVALID(pDesc)	\
   __TMS_USBIF_STRINGDESC_ISVALID(pDesc)
# define LENGTHOF_USBIF_DEVQUALDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_DEVQUALDESC_WIRE
# define LENGTHOF_USBIF_OSCFGDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_OSCFGDESC_WIRE
# define USBIF_OTGDESC_ATTR_SRP	\
   __TMS_USBIF_OTGDESC_ATTR_SRP
# define USBIF_OTGDESC_ATTR_HNP	\
   __TMS_USBIF_OTGDESC_ATTR_HNP
# define LENGTHOF_USBIF_OTGDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_OTGDESC_WIRE
# define LENGTHOF_USBIF_IADESC_WIRE	\
   __TMS_LENGTHOF_USBIF_IADESC_WIRE
# define LENGTHOF_USBIF_DESC_WIRE	\
   __TMS_LENGTHOF_USBIF_DESC_WIRE
# define LENGTHOF_USBIF_BOSDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_BOSDESC_WIRE
# define LENGTHOF_USBIF_SECURITYDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_SECURITYDESC_WIRE
# define USBIF_SECURITYDESC_bNumEncryptionTypes_UNSECURE	\
   __TMS_USBIF_SECURITYDESC_bNumEncryptionTypes_UNSECURE
# define USBIF_SECURITYDESC_bNumEncryptionTypes_WIRED	\
   __TMS_USBIF_SECURITYDESC_bNumEncryptionTypes_WIRED
# define USBIF_SECURITYDESC_bNumEncryptionTypes_CCM_1	\
   __TMS_USBIF_SECURITYDESC_bNumEncryptionTypes_CCM_1
# define LENGTHOF_USBIF_ENCRYPYDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_ENCRYPYDESC_WIRE
# define LENGTHOF_USBIF_KEYDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_KEYDESC_WIRE
# define LENGTHOF_USBIF_WEP_COMPANION_DESC_WIRE	\
   __TMS_LENGTHOF_USBIF_WEP_COMPANION_DESC_WIRE
# define USBIF_WEP_COMPANION_DESC_bmCompAttributes_NO_SWITCH	\
   __TMS_USBIF_WEP_COMPANION_DESC_bmCompAttributes_NO_SWITCH
# define USBIF_WEP_COMPANION_DESC_bmCompAttributes_DYNAMIC_SWITCH	\
   __TMS_USBIF_WEP_COMPANION_DESC_bmCompAttributes_DYNAMIC_SWITCH
# define USBIF_WEP_COMPANION_DESC_bmCompAttributes_DYNAMIC_SWITCH_SCALABLE	\
   __TMS_USBIF_WEP_COMPANION_DESC_bmCompAttributes_DYNAMIC_SWITCH_SCALABLE
# define USBIF_DEVCAPDESC_bDevCapabilityType_WUSB	\
   __TMS_USBIF_DEVCAPDESC_bDevCapabilityType_WUSB
# define USBIF_DEVCAPDESC_bDevCapabilityType_USB20EXTENSION	\
   __TMS_USBIF_DEVCAPDESC_bDevCapabilityType_USB20EXTENSION
# define USBIF_DEVCAPDESC_bDevCapabilityType_SUPERSPEED_USB	\
   __TMS_USBIF_DEVCAPDESC_bDevCapabilityType_SUPERSPEED_USB
# define USBIF_DEVCAPDESC_bDevCapabilityType_CONTAINER_ID	\
   __TMS_USBIF_DEVCAPDESC_bDevCapabilityType_CONTAINER_ID
# define LENGTHOF_USBIF_DEVCAPDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_DEVCAPDESC_WIRE
# define LENGTHOF_USBIF_WUSB_DEVCAPDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_WUSB_DEVCAPDESC_WIRE
# define USBIF_WUSB_DEVCAPDESC_bmAttributes_P2P_DRD	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_P2P_DRD
# define USBIF_WUSB_DEVCAPDESC_bmAttributes_Beacon_MASK	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_Beacon_MASK
# define USBIF_WUSB_DEVCAPDESC_bmAttributes_BEACON(v)	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_BEACON(v)
# define USBIF_WUSB_DEVCAPDESC_bmAttributes_Self_Beacon	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_Self_Beacon
# define USBIF_WUSB_DEVCAPDESC_bmAttributes_Directed_Beacon	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_Directed_Beacon
# define USBIF_WUSB_DEVCAPDESC_bmAttributes_No_Beacon	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmAttributes_No_Beacon
# define USBIF_WUSB_DEVCAPDESC_wPHYRates_53_3	\
   __TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_53_3
# define USBIF_WUSB_DEVCAPDESC_wPHYRates_80	\
   __TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_80
# define USBIF_WUSB_DEVCAPDESC_wPHYRates_106_7	\
   __TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_106_7
# define USBIF_WUSB_DEVCAPDESC_wPHYRates_160	\
   __TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_160
# define USBIF_WUSB_DEVCAPDESC_wPHYRates_200	\
   __TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_200
# define USBIF_WUSB_DEVCAPDESC_wPHYRates_320	\
   __TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_320
# define USBIF_WUSB_DEVCAPDESC_wPHYRates_400	\
   __TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_400
# define USBIF_WUSB_DEVCAPDESC_wPHYRates_480	\
   __TMS_USBIF_WUSB_DEVCAPDESC_wPHYRates_480
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_0	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_0
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_25	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_25
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_5	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_5
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_75	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_1_75
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_0	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_0
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_25	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_25
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_5	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_5
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_75	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_2_75
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_0	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_0
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_25	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_25
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_5	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_5
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_75	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_3_75
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_0	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_0
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_25	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_25
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_5	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_5
# define USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_75	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmFITXPowerInfo_Step_4_75
# define USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group1	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group1
# define USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group2	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group2
# define USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group3	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group3
# define USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group4	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group4
# define USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group5	\
   __TMS_USBIF_WUSB_DEVCAPDESC_bmBandGroup_Group5
# define LENGTHOF_USBIF_SUPERSPEED_EP_COMPANION_DESC_WIRE	\
   __TMS_LENGTHOF_USBIF_SUPERSPEED_EP_COMPANION_DESC_WIRE
# define USBIF_SUPERSPEED_EP_COMPANION_DESC_bMaxBurst_MASK	\
   __TMS_USBIF_SUPERSPEED_EP_COMPANION_DESC_bMaxBurst_MASK
# define USBIF_SUPERSPEED_EP_COMPANION_DESC_bMaxBurst_IS_VALID(v)	\
   __TMS_USBIF_SUPERSPEED_EP_COMPANION_DESC_bMaxBurst_IS_VALID(v)
# define USBIF_SUPERSPEED_EP_COMPANION_DESC_bmAttributes_LG2MAXSTREAMS(v)	\
   __TMS_USBIF_SUPERSPEED_EP_COMPANION_DESC_bmAttributes_LG2MAXSTREAMS(v)
# define USBIF_SUPERSPEED_EP_COMPANION_DESC_LG2MAXSTREAMS_GETMAXID(v)	\
   __TMS_USBIF_SUPERSPEED_EP_COMPANION_DESC_LG2MAXSTREAMS_GETMAXID(v)
# define USBIF_SUPERSPEED_EP_COMPANION_DESC_bmAttributes_MULT(v)	\
   __TMS_USBIF_SUPERSPEED_EP_COMPANION_DESC_bmAttributes_MULT(v)
# define LENGTHOF_USBIF_SUPERSPEED_DEVCAPDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_SUPERSPEED_DEVCAPDESC_WIRE
# define USBIF_SUPERSPEED_DEVCAPDESC_bmAttributes_LTM	\
   __TMS_USBIF_SUPERSPEED_DEVCAPDESC_bmAttributes_LTM
# define USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_LOW(v)	\
   __TMS_USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_LOW(v)
# define USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_FULL(v)	\
   __TMS_USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_FULL(v)
# define USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_HIGH(v)	\
   __TMS_USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_HIGH(v)
# define USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_SUPER(v)	\
   __TMS_USBIF_SUPERSPEED_DEVCAPDESC_wSpeedsSupported_SUPER(v)
# define LENGTHOF_USBIF_EXTENSION_DEVCAPDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_EXTENSION_DEVCAPDESC_WIRE
# define USBIF_EXTENSION_DEVCAPDESC_bmAttributes_LPM	\
   __TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_LPM
# define USBIF_EXTENSION_DEVCAPDESC_bmAttributes_BESL	\
   __TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_BESL
# define USBIF_EXTENSION_DEVCAPDESC_bmAttributes_BASELINE_BESL	\
   __TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_BASELINE_BESL
# define USBIF_EXTENSION_DEVCAPDESC_bmAttributes_DEEP_BESL	\
   __TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_DEEP_BESL
# define USBIF_EXTENSION_DEVCAPDESC_bmAttributes_BASELINE_BESL_VAL(v)	\
   __TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_BASELINE_BESL_VAL(v)
# define USBIF_EXTENSION_DEVCAPDESC_bmAttributes_DEEP_BESL_VAL(v)	\
   __TMS_USBIF_EXTENSION_DEVCAPDESC_bmAttributes_DEEP_BESL_VAL(v)
# define LENGTHOF_USBIF_CONTAINER_ID_DEVCAPDESC_WIRE	\
   __TMS_LENGTHOF_USBIF_CONTAINER_ID_DEVCAPDESC_WIRE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbdesc.h ****/
#endif /* _USBDESC_H_ */
