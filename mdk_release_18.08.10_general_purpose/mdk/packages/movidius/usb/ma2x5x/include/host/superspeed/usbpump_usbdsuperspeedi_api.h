/* usbpump_usbdsuperspeedi_api.h	Thu Feb 26 2009 10:28:22 djt */

/*

Module:  usbpump_usbdsuperspeedi_api.h

Function:
	Definition of API SuperSpeed functions for USBDI clients.

Version:
	V2.01a	Thu Feb 26 2009 10:28:22 djt	Edit level 1

Copyright notice:
	This file copyright (C) 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Debbie Totsline, MCCI Corporation	February 2009

Revision history:
   2.01a Thu Feb 26 2009 10:28:22 djt
	7416: Module created.

*/

#ifndef _USBPUMP_USBDSUPERSPEEDI_API_H_		/* prevent multiple includes */
#define _USBPUMP_USBDSUPERSPEEDI_API_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif


/****************************************************************************\
|
|	Functional API to USBDI components.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_USBDI_USBDSUPERSPEED_INIT_FN	
	UsbPumpUsbdSuperSpeed_Initialize;

__TMS_END_DECLS

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** end of usbpump_usbdsuperspeedi_api.h ****/
#endif /* _USBPUMP_USBDSUPERSPEEDI_API_H_ */
