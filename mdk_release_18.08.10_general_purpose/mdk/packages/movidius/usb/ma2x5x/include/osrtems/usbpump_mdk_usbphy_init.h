/* usbpump_mdk_usbphy_init.h	Thu Sep 03 2015 15:44:56 chwon */

/*

Module:  usbpump_mdk_usbphy_init.h

Function:
	Definition of USB PHY initialization

Version:
	V3.15d	Thu Sep 03 2015 15:44:56 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2014-2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	November 2014

Revision history:
   3.13b  Wed Nov 19 2014 09:05:38 chwon
	Module created.

   3.15a  Tue Jan 20 2015 12:01:06  chwon
	18516: Add fUseExternalClock parameter.

   3.15d  Thu Sep 03 2015 15:44:56  chwon
	19424: Move register definition to .c file.

*/

#ifndef _USBPUMP_MDK_USBPHY_INIT_H_	/* prevent multiple includes */
#define _USBPUMP_MDK_USBPHY_INIT_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpMdk_UsbPhy_Init(
	__TMS_BOOL	fEnableOtgBlock,
	__TMS_BOOL	fUseExternalClock
	);

__TMS_END_DECLS

/**** end of usbpump_mdk_usbphy_init.h ****/
#endif /* _USBPUMP_MDK_USBPHY_INIT_H_ */
