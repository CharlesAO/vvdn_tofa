/* def/stdmacs.h	Thu Nov 13 2014 14:39:02 tmm */

/*

Module:  def/stdmacs.h

Function:
	Define the standard types and macros.  Implies use of def/stdtypes.h.

Version:
	V3.13b	Thu Nov 13 2014 14:39:02 tmm	Edit level 12

Copyright notice:
	This file copyright (C) 1997-2005, 2009, 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 1997

Revision history:
   2.10a  Sun Jun 29 1997 23:00:38  tmm
	Module created.

   3.00b  10/2/2000  tmm
	Get rid of metabracketed #includes for more portability.

   3.02f  6/17/2001   tmm
	Bug:  compile order caused ARGINT to get disclosed sometimes without
	a trailing ';'!

   3.05b  Tue Oct 1 2002 09:05:00 maw
   	Added MAX and MIN macros
   	Added USBPUMP_UNREFERENCED_PARAMETER

   3.05b  Fri Feb 28 2003 16:00:00 maw
   	Renamed __TMS_MAX and __TMS_MIN to __TMS_LIBPORT_MAX and 
	__TMS_LIBPORT_MIN. Renamed __TMS_USBPUMP_UNREFERENCED_PARAMETER to 
	__TMS_LIBPORT_UNREFERENCED_PARAMETER

   3.05h  Fri Dec 09 2005 00:51:58  chwon
   	Fixed TRUE and FALSE definition.

   3.09a  Tue Mar 03 2009 13:24:09  chwon
	3527: add LIBPORT_API_PARAMETER() macro.

   3.13b  Sat Aug 23 2014 14:18:29  tmm
	18362: Add BEGIN_DECLS, CONST_CODESPACE_STRING, CONTAINER_OF,
	END_DECLS, FORCEINLINE, INLINE, LIBPORT_C_ASSERT(),
	LIBPORT_CPU_HARVARD, LIBPORT_HAS_INLINE, LIBPORT_LENOF, RESTRICT, 
	TYPE_CODESPACE(), TYPE_DEF_CONST_CODESPACE(), 
	LIBPORT_UNREFERENCED_VARIABLE().  Clean up documentation a bit.

   3.13b  Thu Nov 13 2014 14:39:02  tmm
	18651: honor __TMS_CLOAKED_NAMES_ONLY.

*/

#ifndef _DEF_STDMACS_H_		/* prevent multiple includes */
#define _DEF_STDMACS_H_

#ifndef _DEF_STDTYPES_H_
# include "def/stdtypes.h"
#endif /* def/stdtypes.h */


/*
|| Assume type cloaking is off.
*/
#ifndef __TMS_CLOAKED_NAMES_ONLY
# define __TMS_CLOAKED_NAMES_ONLY 0
#endif

/*
|| for legacy reasons, if another file defines CONST, it must define all of 
|| the following.
||
|| (definitions are sorted in alphabetical order) 
*/
#ifndef CONST
# ifndef __TMS_ARGINT_DISCLOSED
#   define __TMS_ARGINT_DISCLOSED
    __TMS_TYPE_DEF_DISCLOSE_BASIC(ARGINT);
# endif
# if ! __TMS_CLOAKED_NAMES_ONLY
#   define ARGS(list)		__TMS_P(list)
#   define BSS			__TMS_BSS
#   define CONST		__TMS_const
#   define VOLATILE		__TMS_volatile
#   define ONEWAY(defn)		__TMS_ONEWAY(defn)
#   define ONEWAY_DEF(defn)	__TMS_ONEWAY_DEF(defn)
#   define ROM			__TMS_ROM
#   define SKIP()		__TMS_SKIP()
#   define TRYVAR		__TMS_TRYVAR
# endif /* __TMS_CLOAKED_NAMES_ONLY */
#endif /* ndef CONST */

/*
|| the remaiing are not conditioned on CONST, as there are no legacy
|| issues
*/
#if ! __TMS_CLOAKED_NAMES_ONLY

#  define	BEGIN_DECLS		__TMS_BEGIN_DECLS
#  define	CONST_CODESPACE_STRING(s) \
	  __TMS_CONST_CODESPACE_STRING(s)
#  define	CONTAINER_OF(p, t, f)	__TMS_CONTAINER_OF(p, t, f)
#  define	END_DECLS		__TMS_END_DECLS
#  define	FORCEINLINE		__TMS_forceinline
#  define	INLINE			__TMS_inline
#  define	LIBPORT_C_ASSERT(e)	__TMS_LIBPORT_C_ASSERT(e)
#  define	LIBPORT_CPU_HARVARD	__TMS_LIBPORT_CPU_HARVARD
#  define	 LIBPORT_HAS_INLINE	__TMS_LIBPORT_HAS_INLINE
#  define	LIBPORT_LENOF(a)	__TMS_LIBPORT_LENOF(a)
#  define	OFFSET(s,f)		__TMS_OFFSET(s,f)
#  define	RESTRICT		__TMS_restrict
#  define	TYPE_CODESPACE(e)	__TMS_TYPE_CODESPACE(e)
#  define	TYPE_DEF_CONST_CODESPACE(a_name, a_type)	\
 	  __TMS_TYPE_DEF_CONST_CODESPACE(a_name, a_type)

/*
|| For legacy reasons, LIBPORT_MAX, LIBPORT_MIN,
|| LIBPORT_UNREFERENCED_PARAMETER,and LIBPORT_API_PARAMTER are defined
|| with extra baggage.  To follow the pattern, LIBPORT_UNREFERENCED_VARIABLE
|| also has extra baggage.
*/

#  ifndef __LIBPORT_STDMACS_MAX_DEFINED
#   ifndef LIBPORT_MAX
#    define __LIBPORT_STDMACS_MAX_DEFINED
#    define LIBPORT_MAX(a,b) __TMS_LIBPORT_MAX(a,b)
#   endif
#  endif

#  ifndef __LIBPORT_STDMACS_MIN_DEFINED
#   ifndef LIBPORT_MIN
#    define __LIBPORT_STDMACS_MIN_DEFINED
#    define LIBPORT_MIN(a,b) __TMS_LIBPORT_MIN(a,b)
#   endif
#  endif

#  ifndef __LIBPORT_STDMACS_UNREFERENCED_PARAMETER_DEFINED
#   ifndef LIBPORT_UNREFERENCED_PARAMETER
#    define __LIBPORT_STDMACS_UNREFERENCED_PARAMETER_DEFINED
#    define LIBPORT_UNREFERENCED_PARAMETER(x) __TMS_LIBPORT_UNREFERENCED_PARAMETER(x)
#   endif
#  endif

#  ifndef __LIBPORT_STDMACS_API_PARAMETER_DEFINED
#   ifndef LIBPORT_API_PARAMETER
#    define __LIBPORT_STDMACS_API_PARAMETER_DEFINED
#    define LIBPORT_API_PARAMETER(x) __TMS_LIBPORT_API_PARAMETER(x)
#   endif
#  endif

#  ifndef __LIBPORT_STDMACS_UNREFERENCED_VARIABLE_DEFINED
#   ifndef LIBPORT_UNREFERENCED_VARIABLE
#    define __LIBPORT_STDMACS_UNREFERENCED_VARIABLE_DEFINED
#    define LIBPORT_UNREFERENCED_VARIABLE(x) __TMS_LIBPORT_UNREFERENCED_VARIABLE(x)
#   endif
#  endif

#  ifndef NULL
#   define NULL	__TMS_NULL
#  endif

/**** these are always desirable ****/
#  ifndef TRUE
#   define TRUE	__TMS_TRUE
#  endif
#  ifndef FALSE
#   define FALSE	__TMS_FALSE
#  endif

#  define	NE	!=
#  define NOT	!
#  define EQ	==

#endif /* ! __TMS_CLOAKED_NAMES_ONLY */

/**** end of def/stdmacs.h ****/
#endif /* _DEF_STDMACS_H_ */
