/* def/limits.h	Mon Jan 16 2006 21:12:57 tmm */

/*

Module:  def/limits.h

Function:
	Portable library emulation of the TrueTask <def/limits.h>,
	which defines a policy and then calls def/mccilims.h.  This
	module is compiler and architecture neutral.

Version:
	V3.05h	Mon Jan 16 2006 21:12:57 tmm	Edit level 4

Copyright notice:
	This file copyright (C) 1997-2006 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	June 1997

Revision history:
   2.10a  Sat Jun 28 1997 21:55:03  tmm
	Module created.

   3.00b  10/2/2000  tmm
	Get rid of metabracketed #includes for more portability.

   3.05g  2005-05-29  tmm
	Add 64-bit and limited C99 support.

   3.05h  Mon Jan 16 2006 21:12:57  tmm
	New calling sequence for def/mccilims.h.

*/

/****************************************************************************\
|
|  See portable library's doc/notes.txt for theory of operation
|  of this module.  We are required to have the effect of <limits.h>;
|  but we are also required to make sure the POSIX and TrueTask symbols
|  are defined, as they are rather useful.  If __TMS_MCCI_LIMTS_H is
|  defined to 1, we are going to use the MCCI definitions for the ANSI
|  stuff; otherwise we'll use the C compiler's header for the ANSI
|  stuff.  This can be overridden from the CC command line or by the
|  architecture's def/cdefs.h.
|
\****************************************************************************/

#ifndef _DEF_LIMITS_H_		/* prevent multiple includes */
#define _DEF_LIMITS_H_

#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif /* _DEF_CDEFS_H_ */

#ifndef __TMS_MCCI_LIMITS_H
# define __TMS_MCCI_LIMITS_H	0
#endif

/**** first tell mccidefs whether to define the extra stuff ****/
#ifndef __TMS_LIMITS
#  define __TMS_LIMITS	1
#endif

#ifndef __TMS_LIMITS_POSIX
#  define __TMS_LIMITS_POSIX 1
#endif

/* by default, if the MCCI_LIMITS_H is false, use the compiler for ansi defs */
#ifndef __TMS_LIMITS_ANSI
#  define __TMS_LIMITS_ANSI	__TMS_MCCI_LIMITS_H
#endif

/* by default, if the MCCI_LIMITS_H is false, use the compiler for C99 defs */
#ifndef __TMS_LIMITS_C99
#  define __TMS_LIMITS_C99	__TMS_MCCI_LIMITS_H
#endif

/**** make the definitions ****/
#define __TMS_MCCILIMS_INCLUDE_IS_OK	/* mandatory for calling sequence */
#include "def/mccilims.h"

/**** get limits.h if needed ****/
#if !__TMS_MCCI_LIMITS_H
# include <limits.h>
#endif

/**** end of limits.h ****/
#endif /* _DEF_LIMITS_H_ */
