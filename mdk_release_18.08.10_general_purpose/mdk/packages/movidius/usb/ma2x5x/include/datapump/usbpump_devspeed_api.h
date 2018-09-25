/* usbpump_devspeed_api.h	Tue May 26 2009 11:05:51 tmm */

/*

Module:  usbpump_devspeed_api.h

Function:
	APIs for working with device speed.

Version:
	V2.01a	Tue May 26 2009 11:05:51 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	May 2009

Revision history:
   2.01a  Tue May 26 2009 11:05:51  tmm
	Module created.

*/

#ifndef _USBPUMP_DEVSPEED_API_H_		/* prevent multiple includes */
#define _USBPUMP_DEVSPEED_API_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif


/****************************************************************************\
|
|	API functions.
|
\****************************************************************************/


__TMS_BEGIN_DECLS

__TMS_CONST __TMS_TEXT *
UsbPumpDeviceSpeed_Name(
	__TMS_USBPUMP_DEVICE_SPEED SpeedCode
	);


__TMS_END_DECLS


/**** end of usbpump_devspeed_api.h ****/
#endif /* _USBPUMP_DEVSPEED_API_H_ */
