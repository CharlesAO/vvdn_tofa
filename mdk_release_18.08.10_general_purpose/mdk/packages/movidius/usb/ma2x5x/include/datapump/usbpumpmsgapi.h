/* usbpumpmsgapi.h	Tue May 31 2005 10:51:56 maw */

/*

Module:  usbpumpmsgapi.h

Function:
	API functions for working with messages.  These are mostly for
	use by higher level clients, and so are separated from the base
	type definitions.

Version:
	V1.91d	Tue May 31 2005 10:51:56 maw	Edit level 2

Copyright notice:
	This file copyright (C) 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Mats Webjörn, MCCI Corporation	April 2005

Revision history:
   1.91b  Mon Apr 04 2005 15:50:37 maw
	Module created.

   1.91d  Tue May 31 2005 10:51:56 maw
   	Added process handle to UsbPumpMsg_DoMessage
   	
*/

#ifndef _USBPUMPMSGAPI_H_		/* prevent multiple includes */
#define _USBPUMPMSGAPI_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _UTASK_TYPES_H_
# include "utask_types.h"
#endif


/****************************************************************************\
|
|	Library functions for working with objects.
|
\****************************************************************************/


__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpMsg_DoMessage(
	__TMS_UPLATFORM *		pPlatform,
	__TMS_UTASK_PROCESS_HANDLE	hProc,
	__TMS_UTASK_MSG_HANDLE		hMsg
	);

__TMS_END_DECLS


/**** end of usbpumpmsgapi.h ****/
#endif /* _USBPUMPMSGAPI_H_ */
