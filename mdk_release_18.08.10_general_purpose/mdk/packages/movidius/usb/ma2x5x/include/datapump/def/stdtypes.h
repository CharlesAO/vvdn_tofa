/* def/stdtypes.h	Mon May 11 2009 21:51:59 chwon */

/*

Module:  def/stdtypes.h

Function:
	Kernel demangling of standard types defined for a particular
	architecture.  This file is now portable!

Version:
	V3.09b	Mon May 11 2009 21:51:59 chwon	Edit level 8

Copyright notice:
	This file copyright (C) 1995-2002, 2005-2006, 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	December 1995

Revision history:
   1.00a  Sat Dec 30 1995 07:08:39  tmm
	Module created.

   2.00a  1/11/96  tmm
	Add some more types, and get rid of USHORTness of BITS/BYTES.
	Costs efficiency.  The problem is that we don't want prototyped
	versions of things to have different types; this costs us
	long pushes instead of short pushes in some places.

	Add ARG_xxx where needed for completeness.

   2.03b  12/2/96  tmm
	Change into a standardized type demangler.

   2.10a  6/29/97  tmm
	Move into portable library.

   2.10c  10/15/98  june
	Allow VOID to be defined in the ARCH level.

   3.00o  3/15/1999  tmm
	VOID needs to be defined here; it's __TMS_VOID that needs to be
	architecture-specific, and defined or typedef'ed.  But we now
	pass up a flag so we know how to unveil VOID -- either a macro
	or a typedef.  Typedef is better for many debuggers, so we
	try to do that....

   3.00b  10/2/2000  tmm
	Get rid of metabracketed #includes for more portability.

   3.05a  6/18/2002  tmm
	EWO 600032/1.1.34: Add SIZE_T to the list of disclosed types.

   3.05g  2005-06-28  tmm
	Disclose the 64-bit types

   3.05i  Thu Apr 13 2006 04:37:36  tmm
	Disclose the 128-bit union types and data types (if present).

   3.07b  Wed Dec 05 2007 15:45:03  lin
	Uncloak defines for INT128/UINT128, INT64/UINT64 init macros.

   3.09b  Mon May 11 2009 21:51:59  chwon
	7991: uncloak defines for UINT_PTR, UINT_VOIDPTR, UINT_FNPTR

*/

#ifndef _DEF_STDTYPES_H_		/* prevent multiple includes */
#define _DEF_STDTYPES_H_

/****************************************************************************\
|
|	The standard types....
|
\****************************************************************************/

#ifndef _DEF_CDEFS_H
# include "def/cdefs.h"		/* get the mangled definitions */
#endif

__TMS_TYPE_DEF_DISCLOSE_ARG(TEXT);
__TMS_TYPE_DEF_DISCLOSE_ARG(SCHAR);
__TMS_TYPE_DEF_DISCLOSE_ARG(CHAR);
__TMS_TYPE_DEF_DISCLOSE_ARG(UCHAR);
__TMS_TYPE_DEF_DISCLOSE_ARG(SHORT);
__TMS_TYPE_DEF_DISCLOSE_ARG(USHORT);
__TMS_TYPE_DEF_DISCLOSE(INT);
__TMS_TYPE_DEF_DISCLOSE(UINT);
__TMS_TYPE_DEF_DISCLOSE(LONG);
__TMS_TYPE_DEF_DISCLOSE(ULONG);

#ifndef __TMS_ARGINT_DISCLOSED
# define __TMS_ARGINT_DISCLOSED
__TMS_TYPE_DEF_DISCLOSE(ARGINT);
#endif

__TMS_TYPE_DEF_DISCLOSE_ARG(BOOL);
__TMS_TYPE_DEF_DISCLOSE_ARG(COUNT);
__TMS_TYPE_DEF_DISCLOSE_ARG(METACH);
__TMS_TYPE_DEF_DISCLOSE_ARG(BITS);
__TMS_TYPE_DEF_DISCLOSE_ARG(BYTES);
__TMS_TYPE_DEF_DISCLOSE_ARG(SIZE_T);
__TMS_TYPE_DEF_DISCLOSE_ARG(UINT_PTR);
__TMS_TYPE_DEF_DISCLOSE_ARG(UINT_VOIDPTR);
__TMS_TYPE_DEF_DISCLOSE_ARG(UINT_FNPTR);

#ifndef __TMS_VOID_DISCLOSED
# define __TMS_VOID_DISCLOSED
# ifndef __TMS_VOID_USE_MACRO
  __TMS_TYPE_DEF_DISCLOSE(VOID);
# else

#  if __TMS_VOID_USE_MACRO == 1
#    define VOID	void
#  else
#   if __TMS_VOID_USE_MACRO == 2
#    define VOID	int
#   else
#    if __TMS_VOID_USE_MACRO == 3
#     undef VOID
#     define VOID	__TMS_VOID
#    else
#     error __TMS_VOID_USE_MACRO has an illegal value.
#    endif
#   endif
#  endif

  typedef __TMS_PVOID	PVOID;
# endif
#endif

/* the following types are the smallest types that have exactly n bits */
/* these are ONLY to be used with external representations */
__TMS_TYPE_DEF_DISCLOSE_ARG(INT8);
__TMS_TYPE_DEF_DISCLOSE_ARG(SINT8);
__TMS_TYPE_DEF_DISCLOSE_ARG(UINT8);
__TMS_TYPE_DEF_DISCLOSE_ARG(INT16);
__TMS_TYPE_DEF_DISCLOSE_ARG(UINT16);
__TMS_TYPE_DEF_DISCLOSE_ARG(INT32);
__TMS_TYPE_DEF_DISCLOSE_ARG(UINT32);

__TMS_TYPE_DEF_DISCLOSE(INT64_S);
__TMS_TYPE_DEF_DISCLOSE(UINT64_S);
__TMS_TYPE_DEF_DISCLOSE(INT128_S);
__TMS_TYPE_DEF_DISCLOSE(UINT128_S);

#define LIBPORT_HAS_64BIT	__TMS_LIBPORT_HAS_64BIT
#define LIBPORT_HAS_128BIT	__TMS_LIBPORT_HAS_128BIT

#if __TMS_LIBPORT_HAS_64BIT
__TMS_TYPE_DEF_DISCLOSE_ARG(LLONG);
__TMS_TYPE_DEF_DISCLOSE_ARG(ULLONG);
__TMS_TYPE_DEF_DISCLOSE_ARG(INT64);
__TMS_TYPE_DEF_DISCLOSE_ARG(UINT64);

# define LIBPORT_INT64_C(x)	__TMS_LIBPORT_INT64_C(x)
# define LIBPORT_UINT64_C(x)	__TMS_LIBPORT_UINT64_C(x)
# define LIBPORT_PRId64	__TMS_LIBPORT_PRId64
# define LIBPORT_PRIi64	__TMS_LIBPORT_PRIi64
# define LIBPORT_PRIo64	__TMS_LIBPORT_PRIo64
# define LIBPORT_PRIu64	__TMS_LIBPORT_PRIu64
# define LIBPORT_PRIx64	__TMS_LIBPORT_PRIx64
# define LIBPORT_PRIX64	__TMS_LIBPORT_PRIX64
# define LIBPORT_SCNd64	__TMS_LIBPORT_SCNd64
# define LIBPORT_SCNi64	__TMS_LIBPORT_SCNi64
# define LIBPORT_SCNo64	__TMS_LIBPORT_SCNo64
# define LIBPORT_SCNu64	__TMS_LIBPORT_SCNu64
# define LIBPORT_SCNx64	__TMS_LIBPORT_SCNx64

#endif /* __TMS_LIBPORT_HAS_64BIT */

#if __TMS_LIBPORT_HAS_128BIT
__TMS_TYPE_DEF_DISCLOSE_ARG(LLLONG);
__TMS_TYPE_DEF_DISCLOSE_ARG(ULLLONG);
__TMS_TYPE_DEF_DISCLOSE_ARG(INT128);
__TMS_TYPE_DEF_DISCLOSE_ARG(UINT128);

# define LIBPORT_INT128_C(x)	__TMS_LIBPORT_INT128_C(x)
# define LIBPORT_UINT128_C(x)	__TMS_LIBPORT_UINT128_C(x)
# define LIBPORT_PRId128	__TMS_LIBPORT_PRId128
# define LIBPORT_PRIi128	__TMS_LIBPORT_PRIi128
# define LIBPORT_PRIo128	__TMS_LIBPORT_PRIo128
# define LIBPORT_PRIu128	__TMS_LIBPORT_PRIu128
# define LIBPORT_PRIx128	__TMS_LIBPORT_PRIx128
# define LIBPORT_PRIX128	__TMS_LIBPORT_PRIX128
# define LIBPORT_SCNd128	__TMS_LIBPORT_SCNd128
# define LIBPORT_SCNi128	__TMS_LIBPORT_SCNi128
# define LIBPORT_SCNo128	__TMS_LIBPORT_SCNo128
# define LIBPORT_SCNu128	__TMS_LIBPORT_SCNu128
# define LIBPORT_SCNx128	__TMS_LIBPORT_SCNx128

#endif /* __TMS_LIBPORT_HAS_128BIT */

# define LIBPORT_UINT64_S_INIT(x)	__TMS_LIBPORT_UINT64_S_INIT(x)
# define LIBPORT_INT64_S_INIT(x)	__TMS_LIBPORT_INT64_S_INIT(x)
# define LIBPORT_UINT128_S_INIT(x)	__TMS_LIBPORT_UINT128_S_INIT(x)
# define LIBPORT_INT128_S_INIT(x)	__TMS_LIBPORT_INT128_S_INIT(x)

/**** end of def/stdtypes.h ****/
#endif /* _DEF_STDTYPES_H_ */
