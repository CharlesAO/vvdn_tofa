/* descfns.h	Thu Aug 02 2012 23:50:34 chwon */

/*

Module:  descfns.h

Function:
	Descriptor processing functions.

Version:
	V3.11a	Thu Aug 02 2012 23:50:34 chwon	Edit level 13

Copyright notice:
	This file copyright (C) 1999-2005, 2007-2008, 2010-2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	September 1999

Revision history:
   1.50e  Tue Sep 28 1999 00:43:51  tmm
	Module created.

   1.50f  Thu March 30 2000 12:00:00 maw
        Declaration of UsbFindNextDescriptorInInterface added

   1.79a  5/29/2002  tmm
	EWO 600032 (1.1.2): the descriptor pointers are now in the UINTERFACE,
	so we can save ourselves some effort in places.

   1.79a  6/5/2002  tmm
	Cloak the types.

   1.91c  Mon Apr 18 2005 11:10:16  chwon
	Correct type cloaking; change from __BEGIN_DECLS to __TMS_BEGIN_DECLS

   1.93c  Wed Nov 23 2005 18:13:36  tmm
	As part of fixing bug 542, add global "dummy descriptors" to the
	library for use by the init code if they so choose.

   1.97j  Mon Apr 21 2008 15:49:36  balakumar
	UDP-6286: Add support for bMaxPacketSize0 = 255 for wireless USB.

   1.97j  Fri Nov 14 2008 11:19:32  sivaraj
	UDP-6286: Declared UsbPumpDeviceDescriptor_GetMaxPacketSize0().

   1.97k  Thu Dec 20 2007 16:26:31  djt
	3049: Added UsbParseNextIfcClassDescriptor().

   1.97k  Mon Jan 21 2008 08:23:06  djt
	3049: Changed name from UsbParseNextIfcClassDescriptor() to 
	UsbPumpConfigBundle_FindNextClassDescInInterface()

   3.01c  Mon Mar 22 2010 23:45:27  chwon
	10029: Add UsbFindNextDescriptorInBos().

   3.01d  Tue Feb 01 2011 14:35:43  chwon
	12571: add Speed parameter in UsbPumpDeviceDescriptor_GetMaxPacketSize0

   3.11a  Thu Aug 02 2012 23:50:35  chwon
	15684: Fixed type cloaking problem.

*/

#ifndef _DESCFNS_H_		/* prevent multiple includes */
#define _DESCFNS_H_

#ifndef _USBPUMP_H_
# include "usbpump.h"
#endif

#ifndef _USBDESC_H_
# include "usbdesc.h"
#endif

/****************************************************************************\
|
|	Functions for processing USB descriptors
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_CONST __TMS_USBIF_CFGDESC_WIRE *
UsbFindConfigurationDescriptor(
	__TMS_CONST __TMS_UDEVICE *pSelf,
	__TMS_UINT8 iCfg
	);

__TMS_CONST __TMS_USBIF_IFCDESC_WIRE *
UsbFindInterfaceDescriptor(
	__TMS_CONST __TMS_UDEVICE *pSelf,
	__TMS_CONST __TMS_UINTERFACE *pIfc
	);


/* although we have a function, we commonly use a macro */
#define __TMS_UsbFindInterfaceDescriptor_INLINE(pSelf, pIfc)	\
	((pIfc)->uifc_pIfcDesc)

#if !__TMS_CLOAKED_NAMES_ONLY
# define UsbFindInterfaceDescriptor_INLINE(pSelf, pIfc)	\
	__TMS_UsbFindInterfaceDescriptor_INLINE(pSelf, pIfc)
#endif

#if !__TMS_CLOAKED_NAMES_ONLY
#define	UsbFindInterfaceDescriptor(pSelf, pIfc)	\
	__TMS_UsbFindInterfaceDescriptor_INLINE(pSelf, pIfc)
#endif

__TMS_CONST __TMS_USBIF_DESC_WIRE *
UsbFindNextDescriptorInBos(
	__TMS_CONST __TMS_USBIF_BOSDESC_WIRE *BosDesc,
	__TMS_CONST __TMS_VOID *StartPosition /* OPTIONAL */,
	__TMS_INT DescriptorCode
	);

__TMS_CONST __TMS_USBIF_DESC_WIRE *
UsbFindNextDescriptorInConfig(
	__TMS_CONST __TMS_USBIF_CFGDESC_WIRE *ConfigDesc,
	__TMS_CONST __TMS_VOID *StartPosition /* OPTIONAL */,
	__TMS_INT DescriptorCode
	);

__TMS_CONST __TMS_USBIF_DESC_WIRE *
UsbFindNextDescriptorInInterface(
	__TMS_CONST __TMS_USBIF_CFGDESC_WIRE *ConfigDesc,
	__TMS_CONST __TMS_VOID *StartPosition /* OPTIONAL */,
	__TMS_INT DescriptorCode
	);

__TMS_CONST __TMS_USBIF_DESC_WIRE *
UsbPumpConfigBundle_FindNextClassDescInInterface(
	__TMS_CONST __TMS_USBIF_CFGDESC_WIRE *ConfigDesc,
	__TMS_CONST __TMS_VOID *StartPosition,
	__TMS_INT DescriptorType,
	__TMS_INT DescriptorSubType
	);

__TMS_CONST __TMS_USBIF_IFCDESC_WIRE *
UsbParseConfigurationDescriptor(
	__TMS_CONST __TMS_USBIF_CFGDESC_WIRE *ConfigDesc,
	__TMS_CONST __TMS_VOID *StartPosition /*OPTIONAL*/,
	__TMS_INT InterfaceNumber,
	__TMS_INT AlternateSetting,
	__TMS_INT InterfaceClass,
	__TMS_INT InterfaceSubClass,
	__TMS_INT InterfaceProtocol
	);

__TMS_USHORT
UsbPumpDeviceDescriptor_GetMaxPacketSize0(
	__TMS_CONST __TMS_USBIF_DEVDESC_WIRE *pDevDesc,
	__TMS_USBPUMP_DEVICE_SPEED Speed
	);

/* 
|| We declare dummy descriptors which can be used by USBRC V2.23e and
|| later during initialization.
*/
extern __TMS_CONST __TMS_USBIF_IFCDESC_WIRE
gk_UsbPump_DummyIfc0Desc;

extern __TMS_CONST __TMS_USBIF_EPDESC_WIRE
gk_UsbPump_DummyEp0Desc_8;

extern __TMS_CONST __TMS_USBIF_EPDESC_WIRE
gk_UsbPump_DummyEp0Desc_16;

extern __TMS_CONST __TMS_USBIF_EPDESC_WIRE
gk_UsbPump_DummyEp0Desc_32;

extern __TMS_CONST __TMS_USBIF_EPDESC_WIRE
gk_UsbPump_DummyEp0Desc_64;

extern __TMS_CONST __TMS_USBIF_EPDESC_WIRE
gk_UsbPump_DummyEp0Desc_512;

__TMS_END_DECLS

/**** end of descfns.h ****/
#endif /* _DESCFNS_H_ */
