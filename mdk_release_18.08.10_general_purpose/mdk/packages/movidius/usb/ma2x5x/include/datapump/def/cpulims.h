/* cpulims.h	Mon May 30 2005 00:11:35 tmm */

/*

Module:  cpulims.h

Function:
	Defines the hardware-related /usr/group standard parameters
	as used by MCCI code running on 32-bit Sparc platforms.

Version:
	V3.05g	Mon May 30 2005 00:11:35 tmm	Edit level 3

Copyright notice:
	This file copyright (C) 2001, 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2001

Revision history:
   3.02d  Sat Mar 10 2001 15:39:00  tmm
	Module created.

   3.05g  2005-05-30  tmm
	Add 64-bit support.

*/

#ifndef _SPARC_CPULIMS_H_		/* prevent multiple includes */
#define _SPARC_CPULIMS_H_

/****************************************************************************\
|
|		The /usr/group Standard Parameters 
|
|	We define here the hardware-related /usr/group parameters.  The
|	file-system related parameters are defined in fs.h.
|
\****************************************************************************/

#define	__TMS_CHAR_BIT	8		/* number of bits in a byte */
#define	  __TMS_LG2CHAR_BIT 3		/* log2 of above */
#define __TMS_WORD_BIT	32		/* bits per word */
#define	  __TMS_LG2WORD_BIT	5	/* log2 of above */
#define	__TMS_LONG_BIT	32		/* bits per long */
#define	  __TMS_LG2LONG_BIT	5	/* log2 of above */


#define	__TMS_INT_MAX	0x7fffffff		/* largest value in an int */
#define	__TMS_INT_MIN	(-__TMS_INT_MAX - 1)	/* smallest int */

#define	__TMS_LONG_MAX	(0x7fffffffL)		/* largest long */
#define	__TMS_LONG_MIN	(-__TMS_LONG_MAX - 1)	/* smallest long */

#define	__TMS_SHRT_MAX	(32767)			/* largest short */
#define	__TMS_SHRT_MIN	(-__TMS_SHRT_MAX - 1)	/* smallest short */

#define	__TMS_CHAR_MIN	(-128)			/* smallest plain char */
#define	__TMS_CHAR_MAX	(127)			/* largest plain char */

#define	__TMS_SCHAR_MIN	(-128)			/* smallest signed char */
#define	__TMS_SCHAR_MAX	(127)			/* largest signed char */

#define	__TMS_USI_MAX	(0xffffffffu)		/* largest unsigned int */
#define	__TMS_UINT_MAX	__TMS_USI_MAX		/* synonym for ANSI */
#define	__TMS_USL_MAX	(0xffffffffUL)		/* largest unsigned long */
#define	__TMS_ULONG_MAX	__TMS_USL_MAX		/* synonym for ANSI */
#define	__TMS_USS_MAX	(0xFFFF)		/* largest unsigned short */
#define	__TMS_USHRT_MAX	__TMS_USS_MAX		/* synonym for ANSI */
#define	__TMS_UCHAR_MAX	(0xFF)			/* largest unsigned char */

#define	__TMS_SSIZE_MIN	__TMS_INT_MIN		/* min value for an ssize_t */
#define	__TMS_SSIZE_MAX	__TMS_INT_MAX		/* max value for an ssize_t */
#define	__TMS_SIZE_T_MAX __TMS_UINT_MAX		/* max value for a size_t */

/* signed extrema */
#define	__TMS_INT8_MAX	__TMS_CHAR_MAX		/* map onto the types */
#define	__TMS_INT8_MIN	__TMS_CHAR_MIN
#define	__TMS_INT16_MAX	__TMS_SHRT_MAX
#define	__TMS_INT16_MIN	__TMS_SHRT_MIN
#define	__TMS_INT32_MAX	__TMS_INT_MAX
#define	__TMS_INT32_MIN	__TMS_INT_MIN
#define __TMS_INT64_MAX	__TMS_LIBPORT_INT64_C(9223372036854775807)
#define __TMS_INT64_MIN	(-__TMS_LIBPORT_INT64_C(9223372036854775807) - 1)

/* unsigned maxima (only) */
#define	__TMS_UINT8_MAX		__TMS_UCHAR_MAX	/* map onto the types */
#define	__TMS_UINT16_MAX	__TMS_USHRT_MAX
#define	__TMS_UINT32_MAX	__TMS_USI_MAX
#define __TMS_UINT64_MAX	__TMS_LIBPORT_UINT64_C(0xffffffffffffffff)

/* a few more extrema */
#define	__TMS_LLONG_MAX		__TMS_INT64_MAX
#define	__TMS_LLONG_MIN		__TMS_INT64_MIN
#define	__TMS_ULLONG_MAX	__TMS_UINT64_MAX

/**** end of cpulims.h ****/
#endif /* _SPARC_CPULIMS_H_ */
