/* def/cctypes.h	Tue Sep 23 2014 14:04:14 chwon */

/*

Module:  def/cctypes.h

Function:
	Provides compiler-specific overrides for type information;
	called from <def/cdefs.h> after defining the compiler symbols.

Version:
	V3.13b	Tue Sep 23 2014 14:04:14 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2001, 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	March 2001

Revision history:
   3.02d  Sat Mar 10 2001 15:39:00  tmm
	Module created.

   3.13b  Tue Sep 23 2014 14:04:14  chwon
	18470: Add CPU endian check.

*/

#ifndef _CCTYPES_H_		/* prevent multiple includes */
#define _CCTYPES_H_

/****************************************************************************\
|
|	The standard integral types -- we can use the standard.
|
\****************************************************************************/

/* nothing defined here */

/****************************************************************************\
|
|	Architectural stuff
|
\****************************************************************************/

/* #define	__TMS_USE_SELECTORS 0	-- selectors AREN'T handles */
/* #define	__TMS_S(t)	t	*/ 
/* #define	__TMS_ARBPTR_DEFINED */
/* #define	__TMS_PHYSADDR_DEFINED */
/* #define	__TMS_ADDRBITS_DEFINED */
/* #define	__TMS_ADDRDIFF_DEFINED */
/* #define	__TMS_SELECTOR_DEFINED */

/**** secondary derived types ****/
/* #define __TMS_CPUFLAGS_DEFINED */
/* #define __TMS_PARAS_DEFINED */
/* #define __TMS_RECSIZE_DEFINED */
/* #define __TMS_IOEDITCH_DEFINED */

/**** conversions ****/
/* #define	__TMS_PARAStoADDRBITS(P)	((__TMS_ADDRBITS)(P)) */
/* #define	__TMS_ADDRBITStoPARAS(A)	((__TMS_PARAS)(A)) */
/* #define	__TMS_ADDRBITStoPARAS_ROUND(A)	((__TMS_PARAS)(A)) */

/*
|| atomic types -- 32-bit bus, so use defaults.
*/

/* I/O port addresses -- on a Sparc, these are pointers -- but we need to do arith */
/* #define __TMS_IOPORT_DEFINED */

/**** the endianness of this architecture ****/
#ifndef __TMS_ENDIAN
# if defined(__GNUC__)
#  if __LITTLE_ENDIAN__
#   define __TMS_ENDIAN	__TMS_ENDIAN_LITTLE
#  elif __BIG_ENDIAN__
#   define __TMS_ENDIAN	__TMS_ENDIAN_BIG
#  else
#   define __TMS_ENDIAN	__TMS_ENDIAN_BIG	/* assume big-endian */
#  endif
# else
#  define __TMS_ENDIAN	__TMS_ENDIAN_BIG	/* assume big-endian */
# endif
#endif

/**** The XBYTE macro -- use the default. ****/

/**** we must specify, per compiler, whether <limits.h> exists and is sane ***/
/* #define __TMS_USE_LIMITS_H	-- let the portable file figure it out */


/**** end of def/cctypes.h ****/
#endif /* _CCTYPES_H_ */
