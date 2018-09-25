/* def/mccilims.h	Tue Dec 25 2007 21:12:50 chwon */

/*

Module:  def/mccilims.h

Function:
	Maps MCCI limits names into sane names.

Version:
	V3.07b	Tue Dec 25 2007 21:12:50 chwon	Edit level 7

Copyright notice:
	This file copyright (C) 1997-2007 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	June 1997

Revision history:
   2.10a  Sat Jun 28 1997 20:41:19  tmm
	Module created.

   2.11b  6/7/2000  tmm
	It appears that we need SCHAR_MIN and SCHAR_MAX as we start
	supporting other architectures.

   3.00b  10/2/2000  tmm
	Get rid of metabracketed #includes for more portability.

   3.05h  Mon Jan 16 2006 21:09:34  tmm
	Sometimes people find this file and #include it, not realizing that
	it's not intended to be #included directly.  Add an interlock so that
	some setup is always required, which may help reduce those errors.

   3.05h  Wed Jan 18 2006 14:51:17  tmm
	Correct LIMITS_ANSI definition

   3.07b  Tue Dec 25 2007 21:12:50  chwon
	3650: correct definition of INT64_MIN, INT64_MAX, UINT64_MAX,
	ULLONG_MAX, LLONG_MIN and LLONG_MAX.

*/

/**** We expect and allow multiple includes of this file! ****/

/*
||	__TMS_LIMITS		1 ==> want all definitions in normal space.
||				0 ==> do not include the MCCI extras.
||	__TMS_LIMITS_POSIX	1 ==> want POSIX limits.h names
||				0 ==> do not want POSIX names
||	__TMS_LIMITS_ANSI	1 ==> want ANSI limits.h names
||				0 ==> do not want ANSI limits.h names.
||	__TMS_LIMITS_C99	1 ==> want C99 limits.h names (implies ANSI)
||				0 ==> do not want C99 limits.h names
||
||	Defaults  __TMS_LIMITS		1
||		  __TMS_LIMITS_POSIX	(__TMS_LIMITS)
||		  __TMS_LIMITS_ANSI	(__TMS_LIMITS_POSIX)
||		  __TMS_LIMITS_C99	(__TMS_LIMITS_ANSI)
*/

#ifndef __TMS_MCCILIMS_INCLUDE_IS_OK
# error "def/mccilims.h should not be #included directly; use def/limits.h"
#else
# undef __TMS_MCCILIMS_INCLUDE_IS_OK
#endif

/* get the CPU-specific __TMS_* values */
#include "def/cpulims.h"

/**** set defaults ****/
#ifndef __TMS_LIMITS
# define __TMS_LIMITS 1
#endif

#ifndef __TMS_LIMITS_POSIX
# define __TMS_LIMITS_POSIX	__TMS_LIMITS
#endif

#ifndef __TMS_LIMITS_ANSI
# ifndef __TMS_LIMITS_C99
#  define __TMS_LIMITS_ANSI	__TMS_LIMITS_POSIX
# else
#  define __TMS_LIMITS_ANSI	__TMS_LIMITS_C99
# endif
#endif

#ifndef __TMS_LIMITS_C99
# define __TMS_LIMITS_C99	__TMS_LIMITS_ANSI
#endif

/* define TrueTask/MCCI symbols */
#if __TMS_LIMITS
# ifndef __TMS_LIMITS_DEFINED
#  define __TMS_LIMITS_DEFINED
#  define	LG2CHAR_BIT	__TMS_LG2CHAR_BIT
#  define	LG2WORD_BIT	__TMS_LG2WORD_BIT
#  define	LG2LONG_BIT	__TMS_LG2LONG_BIT
#  define	USI_MAX		__TMS_USI_MAX
#  define	USL_MAX		__TMS_USL_MAX
#  define	USS_MAX		__TMS_USS_MAX
#  define	INT8_MAX	__TMS_INT8_MAX
#  define	INT8_MIN	__TMS_INT8_MIN
#  define	INT16_MAX	__TMS_INT16_MAX
#  define	INT16_MIN	__TMS_INT16_MIN
#  define	INT32_MAX	__TMS_INT32_MAX
#  define	INT32_MIN	__TMS_INT32_MIN
#  if __TMS_LIBPORT_HAS_64BIT
#   define	INT64_MIN	__TMS_INT64_MIN
#   define	INT64_MAX	__TMS_INT64_MAX
#  endif /* __TMS_LIBPORT_HAS_64BIT */
#  define	UINT8_MAX	__TMS_UINT8_MAX
#  define	UINT16_MAX	__TMS_UINT16_MAX
#  define	UINT32_MAX	__TMS_UINT32_MAX
#  if __TMS_LIBPORT_HAS_64BIT
#   define	UINT64_MAX	__TMS_UINT64_MAX
#  endif /* __TMS_LIBPORT_HAS_64BIT */
#  define	SSIZE_T_MIN	__TMS_SSIZE_T_MIN
#  define	SSIZE_T_MAX	__TMS_SSIZE_T_MAX
#  define	SIZE_T_MAX	__TMS_SIZE_T_MAX
# endif /* __TMS_LIMITS_DEFINED */
#endif /* __TMS_LIMITS */

/**** define POSIX symbols ****/
#if __TMS_LIMITS_POSIX
# ifndef __TMS_LIMITS_POSIX_DEFINED
#  define __TMS_LIMITS_POSIX_DEFINED
#  define	SIZE_MAX	__TMS_SIZE_T_MAX
#  define	WORD_BIT	__TMS_WORD_BIT
#  define	LONG_BIT	__TMS_LONG_BIT
# endif /* __TMS_LIMITS_POSIX_DEFINED */
#endif /* __TMS_LIMITS_POSIX */

/*
|| We duplicate the definitions from MCCI's limits.h because
|| this library has to live in any environment, not just TrueTask;
|| but also, for some compilers, we may want the additional posix or
|| MCCI names.
*/
#if __TMS_LIMITS_ANSI
# ifndef __TMS_LIMITS_ANSI_DEFINED
#  define __TMS_LIMITS_ANSI_DEFINED
/* see Harbison and Steele 5th ed p 127 */
#  define	CHAR_BIT	__TMS_CHAR_BIT
#  define	SCHAR_MIN	__TMS_SCHAR_MIN
#  define	SCHAR_MAX	__TMS_SCHAR_MAX
#  define	UCHAR_MAX	__TMS_UCHAR_MAX
#  define	SHRT_MIN	__TMS_SHRT_MIN
#  define	SHRT_MAX	__TMS_SHRT_MAX
#  define	USHRT_MAX	__TMS_USS_MAX
#  define	INT_MIN		__TMS_INT_MIN
#  define	INT_MAX		__TMS_INT_MAX
#  define	UINT_MAX	__TMS_USI_MAX
#  define	LONG_MIN	__TMS_LONG_MIN
#  define	LONG_MAX	__TMS_LONG_MAX
#  define	CHAR_MAX	__TMS_CHAR_MAX
#  define	CHAR_MIN	__TMS_CHAR_MIN
#  define	ULONG_MAX	__TMS_USL_MAX
# endif /* __TMS_LIMITS_ANSI_DEFINED */
#endif /* __TMS_LIMITS_ANSI */

#if __TMS_LIMITS_C99
# ifndef __TMS_LIMITS_C99_DEFINED
#  define __TMS_LIMITS_C99_DEFINED
#  define ULLONG_MAX	__TMS_ULLONG_MAX
#  define LLONG_MAX	__TMS_LLONG_MAX
#  define LLONG_MIN	__TMS_LLONG_MIN
# endif /* __TMS_LIMITS_C99_DEFINED */
#endif /* __TMS_LIMITS_C99 */

/**** end of mccilims.h ****/
