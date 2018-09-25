/* usbprn10.h	Sat Apr 17 1999 13:33:31 tmm */

/*

Module:  usbprn10.h

Function:
	Definitions for the USB-IF Printer device class (wire-level things).

Version:
	V1.00b	Sat Apr 17 1999 13:33:31 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 1998-1999 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	May 1998

Revision history:
   1.00a  Sat May 23 1998 22:42:03  tmm
	Module created.

   1.00b  4/17/1999  tmm
	Update documentation.

*/

#ifndef _USBPRN10_H_		/* prevent multiple includes */
#define _USBPRN10_H_

#ifndef _USBIF_H_
# include "usbif.h"
#endif

/****************************************************************************\
|
|	Class, subclass, and protocol codes.
|
\****************************************************************************/

/* class code */
#define	USB_bInterfaceClass_Printer	0x07	/* the printer class */

/* subclass */
#define	USB_bInterfaceSubClass_Printer	0x01	/* the printer subclass */

/* protocols */
#define	USB_bInterfaceProtocol_Printer_Unidirectional	0x01
#define	USB_bInterfaceProtocol_Printer_Bidirectional	0x02

/****************************************************************************\
|
|	Requests -- printer class is somewhat unusual in its implementation.
|
\****************************************************************************/

/*
|| get-device ID takes wValue == configuration wIndex == ifc + (altsetting << 8)
||	but is marked as an interface-specific request.  But if you only have
||	one interface, you're OK.
*/
#define	USB_bRequest_Printer_GET_DEVICE_ID	0x00

/*
|| get port status is normal 
*/
#define	USB_bRequest_Printer_GET_PORT_STATUS	0x01

/*
|| SOFT RESET flushed all buffers and resets the bulk out and bulk in pipes.
||	Does it clear the data toggle?  In MCCI implementations, no.
||
||	Note that the 1.0 specification indicates that the bmRequestType
||	is 0,10,00011 which means class-specific other!  This means that
||	this protocol is not routable, and is arguably a bug.  On a single-
||	interface device, this is not a problem.   But on multi-interface
||	devices, this is a headache.  MCCI recommends recognizing either
||	0,10,00011 or 0,10,00001 (which would be directed to a specific
||	interface and is therefore routable).
*/
#define	USB_bRequest_Printer_SOFT_RESET		0x02

/****************************************************************************\
|
|	The printer status bits
|
\****************************************************************************/

#define	USB_Printer_Status_PaperEmpty	(1 << 5)
#define	USB_Printer_Status_Select	(1 << 4)
#define	USB_Printer_Status_NotError	(1 << 3)

#define	USB_Printer_Status_ReservedBits	(0xCF)

/**** end of usbprn10.h ****/
#endif /* _USBPRN10_H_ */
