/* vspclock.h	Fri Mar 28 2003 01:00:29  tmm */

/*

Module:  vspclock.h

Function:
	Home for MCCICLKRATE and MAKEBAUDDIV

Version:
	V1.83e	Fri Mar 28 2003 01:00:29	tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2003 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2003

Revision history:
   1.83e  Fri Mar 28 2003 01:00:29  tmm
	Module created.

*/

#ifndef _VSPCLOCK_H_		/* prevent multiple includes */
#define _VSPCLOCK_H_

/****************************************************************************\
|
|	Some macros -- just stand-alone
|
\****************************************************************************/


#define MCCICLKRATE	3686400		/* 3.6864 Mhz */
#define MAKEBAUDDIV(r)	((unsigned short)((((MCCICLKRATE * 2)/(r)) + 1) / 2))

/**** end of vspclock.h ****/
#endif /* _VSPCLOCK_H_ */
