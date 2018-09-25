/* vespwire.h	Mon May 21 2001 23:59:25 sgs */

/*

Module:  vespwire.h

Function:
	MCCI Vendor Ethernet Specific "on-the-wire" definitions for USB NIC Devices

Version:
	V3.17v	Mon May 21 2001 23:59:25 sgs	Edit level 1

Copyright notice:
	This file copyright (C) 2000-2001 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	June 1997

Revision history:
   3.12p  Sun Mar 19 2000 19:35:09 sgs
	Module created.

   3.17v  Mon May 21 2001 23:59:25 sgs
	Added VESP_bRequest_ENABLE_DEVICE_CRC_GENERATION vendor cmd.

*/

#ifndef _VESPWIRE_H_		/* prevent multiple includes */
#define _VESPWIRE_H_

#include "usbif.h"		/* get the USB interface data */

/****************************************************************************\
|
|	The request type codes for VESP control packets.
|
\****************************************************************************/

#define	VESP_bmRequestType_TYPE	USB_bmRequestType_TYPE_VENDOR

#define	VESP_bmRequestType_DEST	USB_bmRequestType_DEST_IFC

#define	VESP_bmRequestType_IN	(VESP_bmRequestType_TYPE | \
				 VESP_bmRequestType_DEST | \
				 USB_bmRequestType_DIR_TOHOST)
				 
#define	VESP_bmRequestType_OUT	(VESP_bmRequestType_TYPE | \
				 VESP_bmRequestType_DEST | \
				 USB_bmRequestType_DIR_TODEV)

/****************************************************************************\
|
|	The request codes for VESP control packets.
|
\****************************************************************************/


#define	VESP_bRequest_ENABLE_CDC_TRAILER_PAD	0x01	/* enable CDC trailer pad mode */
#define	VESP_bRequest_SET_ETHERNET_ADDRESS	0x02	/* set ethernet address */
#define	VESP_bRequest_ENABLE_DEVICE_CRC_GENERATION	0x03	/* enable device CRC generation and checking. */

/**** end of vespwire.h ****/
#endif /* _VESPWIRE_H_ */
