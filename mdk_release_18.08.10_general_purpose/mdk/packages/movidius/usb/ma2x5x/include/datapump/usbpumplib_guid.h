/* usbpumplib_guid.h	Sun Jul 17 2005 03:45:20 tmm */

/*

Module:  usbpumplib_guid.h

Function:
	GUID manipulation routines.

Version:
	V1.91d	Sun Jul 17 2005 03:45:20 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2005

Revision history:
   1.91d  Sun Jul 17 2005 03:45:20  tmm
	Module created.

*/

#ifndef _USBPUMPLIB_GUID_H_		/* prevent multiple includes */
#define _USBPUMPLIB_GUID_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

#ifndef _USBUUID_H_
# include "usbuuid.h"
#endif

/****************************************************************************\
|
|	Function prototypes -- for historical reasons, the real names of
|	these functions are UHIL_..., but they should be UsbPumpLib_...
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BYTES
UsbPumpLib_GuidToString(
	__TMS_TEXT *			/*pBuffer*/,
	__TMS_BYTES			/*nBuffer*/,
	__TMS_BYTES			/*iBuffer*/,
	__TMS_CONST __TMS_USBGUID *	/*pGuid*/
	);

__TMS_END_DECLS


/**** end of usbpumplib_guid.h ****/
#endif /* _USBPUMPLIB_GUID_H_ */
