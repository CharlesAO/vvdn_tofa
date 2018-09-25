/* usbpump_applicationinit.h	Sat Sep 27 2014 21:12:48 tmm */

/*

Module:  usbpump_applicationinit.h

Function:
	Linkage for UsbPump_ApplicationInit()

Version:
	V3.13b	Sat Sep 27 2014 21:12:48 tmm	Edit level 1

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
   3.13b  Sat Sep 27 2014 21:12:48  tmm
	Module created.

*/

#ifndef _USBPUMP_APPLICATIONINIT_H_		/* prevent multiple includes */
#define _USBPUMP_APPLICATIONINIT_H_

#ifndef _UPLATFORM_H_
# include "uplatform.h"
#endif

/****************************************************************************\
|
|	The API definition
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL
UsbPump_ApplicationInit(
	__TMS_UPLATFORM *
	);

__TMS_END_DECLS

/**** end of usbpump_applicationinit.h ****/
#endif /* _USBPUMP_APPLICATIONINIT_H_ */
