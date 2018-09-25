/* xbyte.h	Mon Oct  2 2000 01:00:39 tmm */

/*

Module:  xbyte.h

Function:
	Bind the XBYTE macro to this target's instance.

Version:
	V2.10a	Mon Oct  2 2000 01:00:39 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 1997-2000 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	June 1997

Revision history:
   2.10a  Sun Jun 29 1997 20:30:08  tmm
	Module created.

   3.00b  10/2/2000  tmm
	Get rid of metabracketed #includes for more portability.

*/

#ifndef _DEF_XBYTE_H_		/* prevent multiple includes */
#define _DEF_XBYTE_H_

#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif

/**** the xbyte macro ****/
#define XBYTE(lv, n, t)	__TMS_XBYTE(lv, n, t)

/**** end of xbyte.h ****/
#endif /* _DEF_XBYTE_H_ */
