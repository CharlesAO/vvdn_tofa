/* usbcami10.h	Thu Aug 28 2008 14:33:19 luxw */

/*

Module: usbcami10.h

Function:
	Definition of CAMI constants.
	CAMI: Cable Association Modle Interface - WUSB_AM_Spec_r10

Version:
	V1.97k	Thu Aug 28 2008 14:33:19 luxw	Edit level 2

Copyright notice:
	This file copyright (C) 2007 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Xiaowen Lu, MCCI Corporation	October 2007

Revision history:
   1.97j  Thu Oct 18 2007 14:07:17 luxw
	Module created.

   1.97k  Thu Aug 28 2008 14:33:19  luxw
	Added wValue of Cable Association class commands.

*/

#ifndef _USBCAMI10_H_		/* prevent multiple includes */
#define _USBCAMI10_H_


/****************************************************************************\
|
|	Class, SubClass, and Protocol Codes
|
\****************************************************************************/

#define USB_bInterfaceClass_Miscellaneous	0xEF

#define USB_bInterfaceClass_CableAssociation	USB_bInterfaceClass_Miscellaneous

#define USB_bInterfaceSubClass_CableAssociation	0x03

#define USB_bInterfaceProtocol_CableAssociation	0x01

/* CAMI class-specific requests */
#define USB_bRequest_CableAssociation_GET_ASSOCIATION_INFORMATION	0x01
#define USB_bRequest_CableAssociation_GET_ASSOCIATION_REQUEST		0x02
#define USB_bRequest_CableAssociation_SET_ASSOCIATION_RESPONSE		0x03


#define USB_wValue_CableAssociation_GET_ASSOCIATION_INFORMATION		0
#define USB_wValue_CableAssociation_GET_ASSOCIATION_REQUEST		0x0200
#define USB_wValue_CableAssociation_SET_ASSOCIATION_RESPONSE_HOST_INFO	0x0101
#define USB_wValue_CableAssociation_SET_ASSOCIATION_RESPONSE_CC		0x0201
/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/


/**** end of usbcami10.h ****/
#endif /* _USBCAMI10_H_ */
