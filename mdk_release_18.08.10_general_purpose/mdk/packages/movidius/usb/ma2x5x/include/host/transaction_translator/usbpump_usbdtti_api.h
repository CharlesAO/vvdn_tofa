/* usbpump_usbdtti_api.h	Fri Jul 11 2008 13:52:32 chwon */

/*

Module:  usbpump_usbdtti_api.h

Function:
	Definition of API TT functions for USBDI clients.

Version:
	V1.97k	Fri Jul 11 2008 13:52:32 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2007-2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Debbie Totsline, MCCI Corporation	September 2007

Revision history:
   1.97j  Mon Sep 24 2007 18:47:39 djt
	3265: Module created.

   1.97k  Fri Jul 11 2008 13:52:32  chwon
	4286: use USBPUMP_USBDI_USBDTT_INIT_FN

*/

#ifndef _USBPUMP_USBDTTI_API_H_		/* prevent multiple includes */
#define _USBPUMP_USBDTTI_API_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

/****************************************************************************\
|
|	Functional API to USBDI components.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_USBDI_USBDTT_INIT_FN	/* 4286 */
	UsbPumpUsbdTT_Initialize;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/


/**** end of usbpump_usbdtti_api.h ****/
#endif /* _usbpump_usbdtti_api_H_ */
