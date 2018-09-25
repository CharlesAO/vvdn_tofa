/* mscsubclasslib.h	Fri Oct 18 2002 12:00:00  maw */

/*

Module:  mscsubclasslib.h

Function:
	API functions for subclass implementations.

Version:
	V1.79b	Fri Oct 18 2002 12:00:00  maw	Edit level 1

Copyright notice:
	This file copyright (C) 2002 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Mats Webjörn, MCCI Corporation	Oct 2002

Revision history:
   1.79b  Fri Oct 18 2002 12:00:00  maw
	Module created.

*/

#ifndef _MSCSUBCLASSLIB_H_		/* prevent multiple includes */
#define _MSCSUBCLASSLIB_H_

#ifndef _PROTOMSC_H_
# include "protomsc.h"
#endif

#ifndef _USBIOCTLEDGECODES_H_
# include "usbioctledgecodes.h"
#endif

#ifndef _USBIOCTL_STORAGE_H_
# include "usbioctl_storage.h"
#endif

/****************************************************************************\
|
|	IOCTLs
|
\****************************************************************************/


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS


__TMS_END_DECLS


/**** end of mscsubclasslib.h ****/
#endif /* _MSCSUBCLASSLIB_H_ */
