/* usbpump_mdk_usbphy_init_v2.h	Thu Sep 03 2015 15:17:21 chwon */

/*

Module:  usbpump_mdk_usbphy_init_v2.h

Function:
	Definition of USB PHY initialization

Version:
	V3.15d	Thu Sep 03 2015 15:17:21 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	September 2015

Revision history:
   3.15d  Thu Sep 03 2015 15:17:21 chwon
	19424: Module created.

*/

#ifndef _USBPUMP_MDK_USBPHY_INIT_V2_H_	/* prevent multiple includes */
#define _USBPUMP_MDK_USBPHY_INIT_V2_H_

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
UsbPumpMdk_UsbPhy_Init_V2(
	__TMS_BOOL	fEnableOtgBlock,
	__TMS_BOOL	fUseExternalClock,
	__TMS_UINT32	fsel,
	__TMS_UINT32	refclk_sel0
	);

__TMS_END_DECLS

/**** end of usbpump_mdk_usbphy_init_v2.h ****/
#endif /* _USBPUMP_MDK_USBPHY_INIT_V2_H_ */
