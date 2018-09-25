/* usbpump_mdk_uhilaux.h	Wed Dec 24 2014 11:44:14 chwon */

/*

Module:  usbpump_mdk_uhilaux.h

Function:
	Definition of UHILAUX internals

Version:
	V3.13b	Wed Dec 24 2014 11:44:14 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	December 2014

Revision history:
   3.13b  Wed Dec 24 2014 11:44:14  chwon
	18574: Module created.

*/

#ifndef _USBPUMP_MDK_UHILAUX_H_	/* prevent multiple includes */
#define _USBPUMP_MDK_UHILAUX_H_

#ifndef _USBPUMP_RTEMS_H_
# include "usbpump_rtems.h"
#endif

#ifndef _USBPUMP_UHILAUX_H_
# include "usbpump_uhilaux.h"
#endif

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_RTEMS_UHILAUX_CREATE_FN
UsbPumpMdk_Rtems_UhilAuxCreate;

__TMS_END_DECLS

/**** end of usbpump_mdk_uhilaux.h ****/
#endif /* _USBPUMP_MDK_UHILAUX_H_ */
