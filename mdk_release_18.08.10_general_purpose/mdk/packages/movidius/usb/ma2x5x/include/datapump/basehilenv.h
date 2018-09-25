/* basehilenv.h	Mon May 27 2002 19:12:34  tmm */

/*

Module:  basehilenv.h

Function:
	Set up the compilation environment for basehil.h and subsets.

Version:
	V1.69a	Mon May 27 2002 19:12:34	tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2002 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.69a  Mon May 27 2002 19:12:34  tmm
	Module created.

*/

#ifndef _BASEHILENV_H_		/* prevent multiple includes */
#define _BASEHILENV_H_

#ifndef __TMS_CLOAKED_NAMES_ONLY
# define __TMS_CLOAKED_NAMES_ONLY 0
#endif

#ifndef _BASEHILTYPES_H_
# include "basehiltypes.h"
#endif /* _BASEHILTYPES_H_ */

#if !__TMS_CLOAKED_NAMES_ONLY
# include "def/kerntypes.h"
# include "def/archptrs.h"
# include "mccidefs.h"
#endif /* __TMS_CLOAKED_NAMES_ONLY */

/* 
|| for now, the deprecated interrupt support is in the header files by
|| default 
*/
#ifndef __TMS_BASEHIL_INCLUDE_V1_INTERRUPTS
# ifndef BASEHIL_INCLUDE_V1_INTERRUPTS
#  define __TMS_BASEHIL_INCLUDE_V1_INTERRUPTS 1
# else
   /* backwards compatilibity */
#  define __TMS_BASEHIL_INCLUDE_V1_INTERRUPTS BASEHIL_INCLUDE_V1_INTERRUPTS
# endif
#endif

#if !__TMS_CLOAKED_NAMES_ONLY
# ifndef BASEHIL_INCLUDE_V1_INTERRUPTS
#  define BASEHIL_INCLUDE_V1_INTERRUPTS __TMS_BASEHIL_INCLUDE_V1_INTERRUPTS
# endif
#endif

/**** end of basehilenv.h ****/
#endif /* _BASEHILENV_H_ */
