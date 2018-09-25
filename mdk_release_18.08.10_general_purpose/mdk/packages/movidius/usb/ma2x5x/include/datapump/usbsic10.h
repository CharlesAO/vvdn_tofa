/* usbsic10.h	Wed Oct 27 2004 17:33:30  maw */

/*

Module:  usbsic10.h

Function:
	USB-IF Still Image Capture Class - Bulk Only protocol 
	constants and definitions.

Version:
	V1.89c	Wed Oct 27 2004 17:33:30 maw    Edit level 1

Copyright notice:
	This file copyright (C) 2004 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Mats Webjorn, MCCI Corporation	Oct 2004

Revision history:
   1.89c  Wed Oct 27 2004 17:33:30 maw
	Module created.
  
*/


#ifndef _USBSIC10_H_		/* prevent multiple includes */
#define _USBSIC10_H_

#ifndef _USBIF_H_
# include "usbif.h"
#endif

/****************************************************************************\
|
|	Class, subclass, and protocol codes.
|
\****************************************************************************/

/* Image class device */
#define	USB_bInterfaceClass_Image			0x06

/* Image class interface constants */
#define	USB_bInterfaceSubClass_ImageSIC			0x01

/* A table initializer */
#define	USB_bInterfaceSubClass_Image_INIT \
	{ USB_bInterfaceSubClass_ImageSIC, "Still Image Capture" }

/* Image class Protocol codes */
#define USB_bInterfaceProtocol_ImageBulkOnly		0x01

/* A table initializer */
#define	USB_bInterfaceProtocol_Image_INIT \
	{ USB_bInterfaceProtocol_ImageBulkOnly, "Bulk-Only Transport" }


/* Still Image Capture class-specific requests */
#define USB_bRequest_ImageSIC_CANCEL			0x64
#define USB_bRequest_ImageSIC_GET_EXTENDED_EVENT_DATA	0x65
#define USB_bRequest_ImageSIC_DEVICE_RESET		0x66
#define USB_bRequest_ImageSIC_GET_DEVICE_STATUS		0x67

/* A table initializer */
#define	USB_bRequest_ImageSIC_INIT \
	{ USB_bRequest_ImageSIC_CANCEL, "CANCEL" }, \
	{ USB_bRequest_ImageSIC_GET_EXTENDED_EVENT_DATA, "GET_EXTENDED_EVENT_DATA" }, \
	{ USB_bRequest_ImageSIC_DEVICE_RESET, "DEVICE_RESET" }, \
	{ USB_bRequest_ImageSIC_GET_DEVICE_STATUS, "GET_DEVICE_STATUS" }


/**** end of usbsic10.h ****/
#endif /* _USBSIC10_H_ */
