/* usbpump_mdk_udevswitch.h	Wed Dec 17 2014 12:15:15 chwon */

/*

Module:  usbpump_mdk_udevswitch.h

Function:
	Definition of MDK UDEVICE switch function

Version:
	V3.13b	Wed Dec 17 2014 12:15:15 chwon	Edit level 1

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
   3.13b  Wed Dec 17 2014 12:15:15 chwon
	Module created.

*/

#ifndef _USBPUMP_MDK_UDEVSWITCH_H_	/* prevent multiple includes */
#define _USBPUMP_MDK_UDEVSWITCH_H_

#ifndef _UDEVSWITCH_H_
# include "udevswitch.h"
#endif

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UDEVALLOCATEFN
UsbPumpMdk_Udevice_Alloc;

__TMS_UDEVFREEFN
UsbPumpMdk_Udevice_Free;

__TMS_END_DECLS

/**** end of usbpump_mdk_udevswitch.h ****/
#endif /* _USBPUMP_MDK_UDEVSWITCH_H_ */
