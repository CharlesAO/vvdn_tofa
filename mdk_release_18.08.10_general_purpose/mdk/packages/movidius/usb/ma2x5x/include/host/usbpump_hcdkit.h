/* usbpump_hcdkit.h	Fri Mar 15 2013 09:37:53 chwon */

/*

Module:  usbpump_hcdkit.h

Function:
	The API functions for the HCDKIT

Version:
	V3.11c	Fri Mar 15 2013 09:37:53 chwon	Edit level 6

Copyright notice:
	This file copyright (C) 2005-2006, 2009, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Fri Jun 17 2005 10:54:52  tmm
	Module created.

   1.91h  2005-08-31  tmm
	Write out definitions of UsbPumpHcdKit_InitializeHcd_V1() and
	UsbPumpHcdRequest_Complete, as these are part of the HCDKIT
	API.

   1.91h  Thu Sep  1 2005 05:41:47  tmm
	Remove UsbPumpHcdRequest_Complete() from this file, because it's not
	an HCDKIT API.  Moved it to usbpump_hcd.h.

   1.93e  Fri Jan 06 2006 16:04:49  chwon
	Added pPortStatusChangeArray in UsbPumpHcdKit_InitializeHcd_V1()

   2.01a  Mon Nov 16 2009 12:18:02  chwon
	9172: Remove pPortStatusArray, pPortStatusChangeArray input parameter.
	Change device speed input to device speed mask. 

   3.11c  Fri Mar 15 2013 09:37:53  chwon
	16861: Remove second usbpump_hcdkit_inswitch include.

*/

#ifndef _USBPUMP_HCDKIT_H_		/* prevent multiple includes */
#define _USBPUMP_HCDKIT_H_

#ifndef _USBPUMP_HCDKIT_TYPES_H_
# include "usbpump_hcdkit_types.h"
#endif

#ifndef _USBPUMP_HCDKIT_SWITCH_H_
# include "usbpump_hcdkit_switch.h"
#endif

#ifndef _USBPUMP_HCDKIT_HCD_H_
# include "usbpump_hcdkit_hcd.h"
#endif

/****************************************************************************\
|
|	Functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL
UsbPumpHcdKit_InitializeHcd_V1(
	__TMS_USBPUMP_HCDKIT_HCD *			/* pHcd */,
	__TMS_SIZE_T					/* HcdSize */,
	__TMS_USBPUMP_OBJECT_HEADER *			/* pParent */,
	__TMS_CONST __TMS_TEXT *			/* pHcdName */,
	__TMS_CONST __TMS_USBPUMP_HCDKIT_SWITCH *	/* pSwitch */,
	__TMS_BYTES					/* nRootPorts */,
	__TMS_USBPUMP_DEVICE_SPEED_MASK			/* SupportedSpeedMask */,
	__TMS_UINT32					/* InitialStatus */,
	__TMS_BYTES					/* DefaultMaxTransferSize */
	);

__TMS_END_DECLS

/**** end of usbpump_hcdkit.h ****/
#endif /* _USBPUMP_HCDKIT_H_ */
