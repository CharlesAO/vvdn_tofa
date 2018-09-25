/* uinterfacesetapi.h	Thu Jan 31 2013 08:50:29 tmm */

/*

Module:  uinterfacesetapi.h

Function:
	Methods for UINTERFACESET.

Version:
	V3.11b	Thu Jan 31 2013 08:50:29 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	January 2013

Revision history:
   3.11b  Thu Jan 31 2013 08:50:29  tmm
	16402: Module created.

*/

#ifndef _UINTERFACESETAPI_H_		/* prevent multiple includes */
#define _UINTERFACESETAPI_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

/****************************************************************************\
|
|	The API functions
|
\****************************************************************************/

__TMS_BOOL
UsbPumpInterfaceSet_IsPrimary(
	__TMS_UINTERFACESET *		/* pIfcSet */
	);

__TMS_BOOL
UsbPumpInterfaceSet_IsPrimaryOrSecondary(
	__TMS_UINTERFACESET *		/* pIfcSet */
	);

__TMS_BOOL
UsbPumpInterfaceSet_IsSecondary(
	__TMS_UINTERFACESET *		/* pIfcSet */
	);

__TMS_VOID
UsbPumpInterfaceSet_MarkAsPrimary(
	__TMS_UINTERFACESET *		/* pIfcSet */
	);

__TMS_VOID
UsbPumpInterfaceSet_MarkAsSecondary(
	__TMS_UINTERFACESET *		/* pIfcSet */
	);

/**** end of uinterfacesetapi.h ****/
#endif /* _UINTERFACESETAPI_H_ */
