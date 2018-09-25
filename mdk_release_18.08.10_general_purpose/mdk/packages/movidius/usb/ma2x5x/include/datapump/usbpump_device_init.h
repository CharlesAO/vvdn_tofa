/* usbpump_device_init.h	Sat Sep 27 2014 20:53:01 tmm */

/*

Module:  usbpump_device_init.h

Function:
	API and declarations for UsbPump_GenericDeviceInit()

Version:
	V3.13b	Sat Sep 27 2014 20:53:01 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	September 2014

Revision history:
   3.13b  Sat Sep 27 2014 20:53:01  tmm
	Module created.

*/

#ifndef _USBPUMP_DEVICE_INIT_H_		/* prevent multiple includes */
#define _USBPUMP_DEVICE_INIT_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBAPPINIT_TYPES_H_
# include "usbappinit_types.h"
#endif

/****************************************************************************\
|
|	The API
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UINT
UsbPump_GenericDeviceInit(
	__TMS_UPLATFORM *,
	__TMS_CONST __TMS_USB_DATAPUMP_PORT_INIT_VECTOR_HDR *,
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *
	);

__TMS_END_DECLS


/**** end of usbpump_device_init.h ****/
#endif /* _USBPUMP_DEVICE_INIT_H_ */
