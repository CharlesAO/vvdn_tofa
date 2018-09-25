/* def/cdefs.h	Fri Jan 06 2017 12:37:50 chwon */

/*

Module:  def/cdefs.h

Function:
	This module is the central include file in LIBPORT -- it
	defines the basic types and orients us to the compiler and
	machine in use.

Version:
	V3.19a	Fri Jan 06 2017 12:37:50 chwon	Edit level 42

Copyright notice:
	This file copyright (C) 1996-2007, 2009-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	December 1996

Revision history:
   2.10a  Sun Dec  1 1996 23:07:23  tmm
	Module created.

   2.10a  7/13/97 johnk
	Added __TMS_ARG_CPUFLAGS. 

   2.10a  4/20/1998  tmm	
	Fix IOPORT so we can do arithmetic on it.

   2.10a  6/8/1998  tmm
	Bring back to TrueTask.

   2.10b  6/12/1998  tmm
	Move to the portable area; now all the ports must have
	def/ccdefs.h files.

   2.10b  6/19/1998  tmm
	Add BIT_ENDIAN support.

   2.10b  6/27/1998  tmm
	Add __TMS_SIG_ATOMIC_INT and friends.

   2.10o  8/20/1998  tmm
	We need a way to defeat the definition of size_t here, because it
	often conflicts with vendor compiler header files (in particular
	stddef.h)

   2.10v  3/17/1999  tmm
	Add arm sdk wrapper on GCC detection, to get rid of warnings.
	
   2.11a  6/3/1999  june
	Change INT8 from CHAR to SCHAR, since gcc-MCORE takes CHAR as 
	unsigned.

   2.11b  7/28/1999  tmm
	Add an "int-big-enough-to-hold-a-pointer" type.  Also add an
	"int-big-enough-to-hold-a-function-pointer" type.

   2.11b  1/12/2000  chwon
	Change define from __TMS_ADDRBITS_DEFINED to __TMS_ADDRDIFF_DEFINED.

   Usbkern 1.60n  10/2/2000  tmm
	Get rid of metabracketed #includes for more portability.

   3.05b  8/24/2002  tmm
	Need __TMS_VA_LIST_REF symbolic constant so we can pass va_lists
	around in a porable fashion.

   3.05b  Tue Oct 1 2002 09:05:00 maw
   	Added __TMS_MAX and __TMS_MIN macros
   	Added __TMS_USBPUMP_UNREFERENCED_PARAMETER macro

   3.05b  Fri Feb 28 2003 16:00:00 maw
   	Renamed __TMS_MAX and __TMS_MIN to __TMS_LIBPORT_MAX and 
	__TMS_LIBPORT_MIN
   	Renamed __TMS_USBPUMP_UNREFERENCED_PARAMETER to 
	__TMS_LIBPORT_UNREFERENCED_PARAMETER

   3.05c  Fri Feb 28 2003 21:50:48  tmm
	1015: Fix defaults values of ARG_USHORT, etc, to match ANSI.

   3.05g  2005-05-29  tmm
	Add __TMS_ADDRBITS_PTR_UNION, and commentary.

   3.05h  Fri Sep 09 2005 16:41:59  chwon
	Fix typo in __TMS_LIBPORT_INT64_C(); should be x ## ll, not x ## ull.

   3.05h  Thu Feb 23 2006 22:54:37  tmm
	Add __TMS_LIBPORT_PACKED1(), ...2(), ...4(), ...8()

   3.05i  Thu Apr 13 2006 04:36:45  tmm
	Add rudimentary 128-bit support; and for consistency add UINT64_S and
	INT64_S types.

   3.07a  Wed Nov 21 2007 04:51:53  tmm
	3516: Add __TMS_LIBPORT_VERSION(), __TMS_LIBPORT_VERSION_CALC(),
	__TMS_LIBPORT_VERSION_MAJOR(), __TMS_LIBPORT_VERSION_MINOR(),
	__TMS_LIBPORT_VERSION_LETTER(), __TMS_LIBPORT_VERSION_PATCH() macros.
	Clean up revision log a little.

   3.07b  Tue Nov 27 2007 08:46:26  tmm
	3528:  clean compile for Microsoft Visual C V6.0 with /W4.
	To simplify life, we export __TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI,
	which can be used by applicatoins to control whether the usual
	set of MSC-specific pragmas are turned on.

   3.07b  Wed Dec 05 2007 15:26:18  lin
	Added UINT/INT64, UINT128/INT128 INIT macros.

   3.09a  Wed Dec 26 2007 23:57:36  chwon
	Change version to 3.09a

   3.09a  Fri Feb 27 2009 12:36:10  chwon
	3527: add __TMS_LIBPORT_API_PARAMETER() macro.
	Change version to 3.09b

   3.09b  Mon May 11 2009 18:19:00  tmm
	7991:  somehow we end up on some ports with sizeof UINT_PTR
	less than sizeof ADDRBITS.  Add a check.

   3.09b  Mon May 11 2009 21:49:45  chwon
	7991: fixed __TMS_LIBPORT_C_ASSERT definition -- declare void function

   3.09b  Sat Dec 19 2009 17:39:51  chwon
	Change version to 3.09c.

   3.09c  Tue Dec 21 2010 10:50:46  chwon
	Change version to 3.09d.

   3.09d  Thu Apr 21 2011 11:26:36  chwon
	Change version to 3.09e.

   3.09e  Thu Apr 21 2011 12:28:43  chwon
	13278: if not defined __TMS_USE_STDDEF_FOR_SIZE_T, define with 0 and
	fixed scope of "#if __TMS_USE_STDDEF_FOR_SIZE_T".

   3.09e  Thu May 12 2011 15:29:38  chwon
	13380: add default __TMS_LIBPORT_UNREFERENCED_PARAMETER and
	__TMS_LIBPORT_API_PARAMETER definition for gcc compiler if these are
	not defined.

   3.01f  Tue Jul 24 2012 14:11:09  chwon
	Change version to 3.11a.

   3.11a  Thu Sep 13 2012 11:41:55  chwon
	Change version to 3.11b.

   3.11b  Thu Jan 17 2013 02:46:12  tmm
	16492: add __TMS_STRINGVAL(), __TMS_CONCATVAL(), __TMS_CONCAT3VAL().

   3.11b  Wed Jan 30 2013 12:26:36  chwon
	Change version to 3.11c.

   3.11c  Thu Jun 13 2013 09:42:05  chwon
	Change version to 3.11d.

   3.11d  Thu Jan 02 2014 09:56:32  chwon
	17970: Change version to 3.11e.

   3.11e  Thu Feb 06 2014 16:51:41  chwon
	18043: Change version to 3.13a.

   3.13a  Mon Aug 04 2014 21:56:36  chwon
	Change version to 3.13b.

   3.13b  Sun Aug 17 2014 16:50:12  tmm
	18362: Add __TMS_LIBPORT_VERSION_HAS_CPU_HARVARD(),
	__TMS_LIBPORT_VERSION_HAS_INLINE(), complete implementation of
	__TMS_inline, add __TMS_forceinline, add __TMS_LIBPORT_HAS_INLINE,
	__TMS_LIBPORT_CPU_HARVARD, __TMS_TYPE_DEF_CONST_CODESPACE(),
	__TMS_TYPE_CODESPACE(type).
	(We needed a storage qualifier for the address space for
	constant data.) Set _PATCH() version to 3.13b-1 so that the new
	version macros will return TRUE.  Improve docs.

   3.13b  Sat Aug 23 2014 14:40:23  tmm
	18362: add __TMS_LIBPORT_LENOF(), __TMS_restrict,
	__TMS_LIBPORT_UNREFERENCED_VARIABLE()

   3.13b  Thu Jan 15 2015 13:11:54  chwon
	Change version to 3.15a.

   3.15b  Fri Jul 10 2015 16:05:30  chwon
	19300: Change version to 3.15c.

   3.15c  Tue Sep 01 2015 09:31:06  chwon
	Change version to 3.15d.

   3.15d  Mon Nov 23 2015 18:54:20  skpark
	19666: Add __TMS_VA_LIST_COPY

   3.15d  Tue Jan 19 2016 10:37:01  chwon
	Change version to 3.15e.

   3.15e  Tue Jan 26 2016 12:54:18  chwon
	19852: Change version to 3.17a for next developmenet cycle.

   3.17a  Wed Aug 17 2016 09:51:38  chwon
	Change version to 3.19a.

   3.19a  Fri Jan 06 2017 12:37:50  chwon
	Change version to 3.19b.

*/

#ifndef _DEF_CDEFS_H_		/* prevent multiple includes */
#define _DEF_CDEFS_H_

/*

MODULE DESCRIPTION:	def/cdefs.h

	This module is the central module in the libport scheme for defining
	the standard types for MCCI software products.

	This module is best understood in terms of preconditions, post
	conditions, and invariants.

	Precondition #1:  _DEF_CDEFS_H_ shall either defined or undefined.
	The value is not important; the state of definition is used.

	Postcondition #1:  _DEF_CDEFS_H_ is defined.

	Precondition #2:  if _DEF_CDEFS_H_ is undefined, then all of the
		following post-conditions apply.

		Precondition #2.1:  __TMS_* are reserved words;  
			__TMS_*_DEFINED is defined iff __TMS_* has
			already been defined.

		Precondition #2.2:  if __TMS_SIZE_T_ is defined, it
			shall be the text name of the desired size_t
			type.  Otherwise size_t shall already be
			defined.

		Post condition #2.2a:  if _DEF_ANSI_H_ was undefined,
			or if __TMS_SIZE_T_ was defined at entry, then
			the |typedef| |size_t| has been added to the name
			space.

		Post condition #2.3:  __TMS_SIZE_T_ is not defined.

		Post condition #2.4:  __TMS_SIZE_T_DEFINED is defined
			iff size_t was defined by a libport module.

	Post-condition #3:  
		size_t can be assumed to be defined

	Post-condition #4:
		the following macros are defined

		__TMS_LIBPORT_VERSION
		__TMS_LIBPORT_VERSION_CALC(M,m,L,p)
		__TMS_LIBPORT_VERSION_MAJOR(v)
		__TMS_LIBPORT_VERSION_MINOR(v)
		__TMS_LIBPORT_VERSION_LETTER(v)
		__TMS_LIBPORT_VERSION_PATCH(v)

		__TMS_LIBPORT_VERSION_HAS_CPU_HARVARD(v)
		__TMS_LIBPORT_VERSION_HAS_INLINE(v)

		__TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI

		__TMS_BEGIN_DECLS
		__TMS_CONCAT(x,y)
		__TMS_CONCAT3(x,y,z)
		__TMS_CONST
		__TMS_CONTAINER_OF(p, containerType, fieldInContainer)
		__TMS_ENDDECLS
		__TMS_LIBPORT_HAS_INLINE
		__TMS_LIBPORT_LENOF(a)
		__TMS_LIBPORT_MAX(a, b)
		__TMS_LIBPORT_MIN(a, b)
		__TMS_OFFSET(s, f)
		__TMS_P(args)
		__TMS_ROM
		__TMS_STRING(a)
		__TMS_STRINGVAL(a)
		__TMS_SKIP()
		__TMS_TRYVAR
		__TMS_const
		__TMS_forceinline
		__TMS_inline
		__TMS_restrict
		__TMS_signed
		__TMS_schar
		__TMS_signed
		__TMS_void
		__TMS_volatile
		__TMS_VA_LIST_REF
		__TMS_VA_LIST_COPY

		__TMS_ONEWAY()
		__TMS_ONEWAY_DEF()

		__TMS_LIBPORT_CPU_HARVARD
		__TMS_CONST_CODESPACE_STRING(s)
		__TMS_TYPE_CODESPACE(type)
		__TMS_TYPE_DEF_CONST_CODESPACE(name, type)

		__TMS_LIBPORT_C_ASSERT(e)

		__TMS_LIBPORT_PACKED1(param)
		__TMS_LIBPORT_PACKED2(param)
		__TMS_LIBPORT_PACKED4(param)
		__TMS_LIBPORT_PACKED8(param)

		__TMS_LIBPORT_PACKED1_DEFINITION(s)
		__TMS_LIBPORT_PACKED2_DEFINITION(s)
		__TMS_LIBPORT_PACKED4_DEFINITION(s)
		__TMS_LIBPORT_PACKED8_DEFINITION(s)

		__TMS_S()

		__TMS_PARAStoADDRBITS()
		__TMS_ADDRBITStoPARAS()
		__TMS_ADDRBITStoPARAS_ROUND()

		__TMS_LIBPORT_API_PARAMETER(x)
		__TMS_LIBPORT_UNREFERENCED_PARAMETER(x)
		__TMS_LIBPORT_UNREFERENCED_VARIABLE(x)

		__TMS_XBYTE(lv, n, t)

		__TMS_LIBPORT_INT64_C()	
			like C99 INT64_C()
		__TMS_LIBPORT_UINT64_C()
			like C99 UINT64_C()

		__TMS_LIBPORT_PRI[diouxX]64
		__TMS_LIBPORT_SCN[dioux]64
		
		__TMS_LIBPORT_UINT64_S_INIT(x)
		__TMS_LIBPORT_INT64_S_INIT(x)
		__TMS_LIBPORT_UINT128_S_INIT(x)
		__TMS_LIBPORT_INT128_S_INIT(x)

	Post-condition #5:
		the following constants are defined

		__TMS_LIBPORT_HAS_64BIT	true if the compiler supports
			64 bit and LLONG/ULLONG types.

		__TMS_LIBPORT_HAS_128BIT true if the compiler supports
			128 bit types, but only if __TMS_LIBPORT_HAS_64BIT
			is also true.

		__TMS_USE_ANSI	true if we can use ANSI C89 facilities
		__TMS_USE_LIMITS_H
				true if we are to use the compiler's
				<limits.h>

		__TMS_ENDIAN	endianness of this machine
		__TMS_FPENDIAN	endianness of fp on this machine
		__TMS_QENDIAN	endianness of quads on this machine
		__TMS_BITENDIAN	endianness of bitfields on this machine.
				Values as for __TMS_ENDIAN.

		__TMS_ENDIAN_LITTLE
		__TMS_ENDIAN_BIG
		__TMS_ENDIAN_PDP
		__TMS_QENDIAN_{LITTLE, BIG, ARMMIDDLE, PDP}
		__TMS_QQENDIAN_{LITTLE, BIG}

		__TMS_NULL

		__TMS_FALSE
		__TMS_TRUE

		__TMS_USE_SELECTORS

	Post-condition #6:
		the following types are defined:

		__TMS_{SIZE_T, PSIZE_T, ARG_SIZE_T, PARG_SIZE_T}
		__TMS_{TEXT, PTEXT, ARG_TEXT, PARG_TEXT}
		__TMS_{CHAR, PCHAR, ARG_CHAR, PARG_CHAR}
		__TMS_{SCHAR, PSCHAR, ARG_SCHAR, PARG_SCHAR}
		__TMS_{UCHAR, PUCHAR, ARG_UCHAR, PARG_UCHAR}
		__TMS_{SHORT, PSHORT, ARG_SHORT, PARG_SHORT}
		__TMS_{USHORT, PUSHORT, ARG_USHORT, PARG_USHORT}
		__TMS_INT, __TMS_PINT
		__TMS_UINT, __TMS_PUINT
		__TMS_{UINT_PTR, PUINT_PTR, ARG_UINT_PTR, PARG_UINT_PTR}
		__TMS_{UINT_DPTR, PUINT_DPTR, ARG_UINT_DPTR, PARG_UINT_DPTR}
		__TMS_{UINT_FNPTR, PUINT_FNPTR, ARG_UINT_FNPTR, PARG_UINT_FNPTR}
		__TMS_LONG, __TMS_PLONG
		__TMS_ULONG, __TMS_PULONG
		__TMS_ARGINT, __TMS_PARGINT
		__TMS_{BOOL, PBOOL, ARG_BOOL, PARG_BOOL}
		__TMS_{COUNT, PCOUNT, ARG_COUNT, PARG_COUNT}
		__TMS_{METACH, PMETACH, ARG_METACH, PARG_METACH}
		__TMS_{BITS, PBITS, ARG_BITS, PARG_BITS}
		__TMS_{BYTES, PBYTES, ARG_BYTES, PARG_BYTES}
		__TMS_VOID, __TMS_PVOID

		__TMS_{INT8, PINT8, ARG_INT8, PARG_INT8}
		__TMS_{UINT8, PUINT8, ARG_UINT8, PARG_UINT8}
		__TMS_{INT16, PINT16, ARG_INT16, PARG_INT16}
		__TMS_{UINT16, PUINT16, ARG_UINT16, PARG_UINT16}
		__TMS_{INT32, PINT32, ARG_INT32, PARG_INT32}
		__TMS_{INT64, PINT64, ARG_INT64, PARG_UNT64}
		__TMS_{UINT32, PUINT32, ARG_UINT32, PARG_UINT32}
		__TMS_{UINT64, PUINT64, ARG_UINT64, PARG_UINT64}

		__TMS_{UINT128_S, PUINT128_S}
		__TMS_{INT128_S, PINT128_S}

		__TMS_ARBPTR, __TMS_PARBPTR
		__TMS_PHYSADDR, __TMS_PPHYSADDR
		__TMS_ADDRBITS, __TMS_PADDRBITS (equiv to UINT_PTR)
		__TMS_ADDRBITS_PTR_UNION, __TMS_PADDRBITS_PTR_UNION
		__TMS_ADDRDIFF, __TMS_PADDRDIFF
		__TMS_{SELECTOR, PSELECTOR, ARG_SELECTOR, PARG_SELECTOR}
		__TMS_{RECSIZE, PRECSIZE, ARG_RECSIZE, PARG_RECSIZE}
		__TMS_{CPUFLAGS, PCPUFLAGS, ARG_CPUFLAGS, PARG_CPUFLAGS}
		__TMS_{PARAS, PPARAS, ARG_PARAS, PARG_PARAS}
		__TMS_{IOEDITCH, PIOEDITCH, ARG_IOEDITCH, PARG_IOEDITCH}

		__TMS_{SIG_ATOMIC_INT, PSIG_ATOMIC_INT, ARG_...}
		__TMS_{SIG_ATOMIC_UINT, PSIG_ATOMIC_UINT, ARG_...}
		__TMS_{INTERRUPT_ATOMIC_INT, PINTERRUPT_ATOMIC_INT, ARG_...}
		__TMS_{INTERRUPT_ATOMIC_UINT, PINTERRPT_ATOMIC_UINT, ARG_...}
		__TMS_{CPUBUS_ATOMIC_INT, PCPUBUS_ATOMIC_INT, ARG_...}
		__TMS_{CPUBUS_ATOMIC_UINT, PCPUBUS_ATOMIC_UINT, ARG_...}

		__TMS_{IOPORT, PIOPORT, ARG_IOPORT, PARG_IOPORT}

	Post-condition #7:
		<def/typemacs.h> has been included.
		<def/ansi.h> has been included

	__TMS_LIBPORT_VERSION is a numeric macro that can be used by
	client packages if they need to work around limitations in libport
	that can't otherwise be auto-sensed at compile time.  It is a
	decimal number in the form MMmmLLqq, where MM is the major release,
	mm is the minor release, LL is the letter (A being 1, 0 being no
	letter), and qq being the "patch number" -- normally 0.

	__TMS_LIBPORT_VERSON_CALC(MM, mm, LL, qq) will generate a number
	from MM, mm, LL, qq.

	__TMS_LIBPORT_VERSION_MAJOR(v) will extract the major field;
	__TMS_LIBPORT_VERSION_MINOR(v) will extract the minor field;
	__TMS_LIBPORT_VERSION_LETTER(v) will extract the letter field;
	__TMS_LIBPORT_VERSION_PATCH(v) will extract the patch field.

	__TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI is 1 if we think this 
	compiler will need Microsoft-style pragmas for compiling MCCI code,
	0 otherwise.

*/

/****************************************************************************\
|
|	The version stuff comes first!
|
\****************************************************************************/

#define	__TMS_LIBPORT_VERSION_CALC(MM, mm, LL, qq)	\
	((((MM) * 100ul + (mm)) * 100 + (LL)) * 100 + (qq))

/* the version is 3.19b (patch 0); that's: */
#define	__TMS_LIBPORT_VERSION				\
	__TMS_LIBPORT_VERSION_CALC(3, 19, 2, 0)

/* extract the version fields */
#define	__TMS_LIBPORT_VERSION_MAJOR(v)	((v) / (100ul*100ul*100ul))
#define	__TMS_LIBPORT_VERSION_MINOR(v)	(((v) / (100ul*100ul)) % 100ul)
#define	__TMS_LIBPORT_VERSION_LETTER(v)	(((v) / 100ul) % 100ul)
#define	__TMS_LIBPORT_VERSION_PATCH(v)	((v) % 100ul)

/* we introduced limited Harvard architecture support after base 3.13b */
#define	__TMS_LIBPORT_VERSION_HAS_CPU_HARVARD(v)	\
	((v) > __TMS_LIBPORT_VERSION_CALC(3, 13, 2, 0))

/* we introduced limited inline support after base 3.13b */
#define	__TMS_LIBPORT_VERSION_HAS_INLINE(v)		\
	((v) > __TMS_LIBPORT_VERSION_CALC(3, 13, 2, 0))

/****************************************************************************\
|
|	The following definitions don't pollute the namespace, but do
|	provide a number of useful things for implementation files.
|
\****************************************************************************/

/**** the kinds of endianness ****/
#define	__TMS_ENDIAN_LITTLE	1234
#define	__TMS_ENDIAN_BIG	4321
#define	__TMS_ENDIAN_PDP	3412
#define	__TMS_QENDIAN_LITTLE	12345678L
#define	__TMS_QENDIAN_BIG	87654321L
#define	__TMS_QENDIAN_ARMMIDDLE	56781234L
#define	__TMS_QENDIAN_PDP	76564312L	
#define	__TMS_QQENDIAN_LITTLE	12345678L
#define	__TMS_QQENDIAN_BIG	87654321L

/****************************************************************************\
|
|	Read the per-compiler definitions
|
\****************************************************************************/

/* load the architecture's overrides */
#ifndef _DEF_CCDEFS_H_
# include "def/ccdefs.h"
#endif

/****************************************************************************\
|
|	Now, set up the defaults.
|
\****************************************************************************/

/* assume that MSC pragmas are not needed */
#ifndef __TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI
# define __TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI 0
#endif

/* assume that the compiler includes 64-bit support */
#ifndef __TMS_LIBPORT_HAS_64BIT
# define __TMS_LIBPORT_HAS_64BIT	1
#endif

/* assume that the compiler does NOT include 128-bit support */
#ifndef __TMS_LIBPORT_HAS_128BIT
# define __TMS_LIBPORT_HAS_128BIT	0
#endif

/* idiot check */
#if defined(__TMS_LIBPORT_HAS_INLINE) && !defined(__TMS_inline)
# error "ccdefs.h defined __TMS_LIBPORT_HAS_INLINE but not __TMS_inline"
#endif

/**** handle declaration macros ****/
#ifdef __cplusplus
# ifndef __TMS_BEGIN_DECLS
#   define	__TMS_BEGIN_DECLS	extern "C" {
# endif
# ifndef __TMS_END_DECLS
#   define	__TMS_END_DECLS	};
# endif
# ifndef __TMS_USE_ANSI
#   define	__TMS_USE_ANSI	1
# endif
# ifndef __TMS_inline
#   define	__TMS_inline	inline
#   ifndef __TMS_LIBPORT_HAS_INLINE
#      define __TMS_LIBPORT_HAS_INLINE 1
#   endif
# endif
# ifndef __TMS_forceinline
#   if  (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7))
#      define __TMS_forceinline __inline__ __attribute__((always_inline))
#   else
#      define __TMS_forceinline inline
#   endif
# endif
# ifndef __TMS_NULL
#   define	__TMS_NULL	0
# endif
#else /* ndef __cplusplus */
# ifndef __TMS_BEGIN_DECLS
#   define	__TMS_BEGIN_DECLS
# endif
# ifndef __TMS_END_DECLS
#   define	__TMS_END_DECLS
# endif
# ifdef __STDC__
#   ifndef __TMS_USE_ANSI
#     define	__TMS_USE_ANSI	1
#   endif
#   ifndef __TMS_NULL
#     define	__TMS_NULL	((void *) 0)
#   endif
#   if __STDC__ && defined(__STDC_VERSION__)
#     if __STDC_VERSION__ >= 199901L
#	ifndef __TMS_restrict
#	  define __TMS_restrict restrict
#	endif
#     endif
#   endif
# else /* ndef __STDC__ */
#   ifndef __TMS_USE_ANSI
#     define	__TMS_USE_ANSI	0
#   endif
#   ifndef __TMS_NULL
#     define	__TMS_NULL	((char *) 0)
#   endif
# endif	/* __STDC__ */
# ifndef __GNUC__
#   ifndef  __TMS_inline
#     define	__TMS_inline	/* delete __inline if not c++ and not gcc */
#     ifndef __TMS_LIBPORT_HAS_INLINE
#       define __TMS_LIBPORT_HAS_INLINE 0
#     endif
#   endif
# else /* gcc */
#   ifndef __TMS_inline
#     define	__TMS_inline	__inline__
#     ifndef __TMS_LIBPORT_HAS_INLINE
#       define __TMS_LIBPORT_HAS_INLINE 1
#     endif
#   endif
#   ifndef __TMS_forceinline
#     if  (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7))
#      define __TMS_forceinline __inline__ __attribute__((always_inline))
#     else
#      define __TMS_forceinline __TMS_inline
#     endif
#   endif
#   ifndef __TMS_restrict
#     define __TMS_restrict __restrict__
#   endif
# endif /* gcc */
#endif /* ndef __cplusplus */

/* backup definitions for __TMS_inline and __TMS_forceinline */
#ifndef __TMS_inline
#  define	__TMS_inline
#endif

/*
|| if no __TMS_forceinline yet, then use value of __TMS_inline --
|| which might be empty, but so it goes.
*/
#ifndef __TMS_forceinline
#  define	__TMS_forceinline	__TMS_inline
#endif

/****************************************************************************\
|
| Part 2:  based on value of TMS_ANSI, adapt the canonical macros.
|
|	At end of this section, we'll have:
|
|	__TMS_CONCAT()	string concatenator
|	__TMS_CONCATVAL() expand args, then concatenate
|	__TMS_CONCAT3()	triadic string concatenator
|	__TMS_CONCAT3VAL() expand args, then concatenate.
|	__TMS_P()	arglist specifier.
|	__TMS_SKIP()	the "no-operation" macro.
|	__TMS_const	the const keyword, if available.
|	__TMS_restrict the "restrict" keyword, if avialable.
|	__TMS_schar	the "signed char" keyword, else "char"
|	__TMS_signed	the "signed" keyword.
|	__TMS_void	the void keyword, if available
|	__TMS_volatile	the volatile keyword, if available
|
\****************************************************************************/

/* In the ANSI world, this is pretty easy. */
#if __TMS_USE_ANSI
# ifndef __TMS_CONCAT
#   define	__TMS_CONCAT(A,B)	A ## B
# endif
# ifndef __TMS_CONCATVAL
#   define	__TMS_CONCATVAL(A,B)	__TMS_CONCAT(A,B)
# endif
# ifndef __TMS_CONCAT3
#   define	__TMS_CONCAT3(A,B,C)	A ## B ## C
# endif
# ifndef __TMS_CONCAT3VAL
#   define	__TMS_CONCAT3VAL(A,B,C)	__TMS_CONCAT3(A,B,C)
# endif
# ifndef __TMS_P
#   define	__TMS_P(args)	args	/* can use prototypes */
# endif
# ifndef __TMS_STRING
#   define	__TMS_STRING(A)	#A
# endif
# ifndef __TMS_STRINGVAL
#   define	__TMS_STRINGVAL(A)	__TMS_STRING(A)
# endif
# ifndef __TMS_SKIP
#   define	__TMS_SKIP()	((void) 0)	/* do nothing */
# endif
# ifndef __TMS_const
#   define	__TMS_const	const		/* works on this compiler! */
# endif
# ifndef __TMS_restrict
#   ifdef __STDC_VERSION__
#     if __STDC_VERSION__ >= 199901L
#	define	__TMS_restrict	restrict
#     else
#	define	__TMS_restrict	/* nothing */
#     endif
#   else
#	define	__TMS_restrict	/* nothing */
#   endif
# endif /* ndef __TMS_restrict */
# ifndef __TMS_schar
#   define	__TMS_schar	signed char	/* the ANSI way */
# endif
# ifndef __TMS_signed
#   define	__TMS_signed	signed		/* the ANSI way */
# endif
# ifndef __TMS_void
#   define	__TMS_void	void		/* works on this compiler! */
# endif
# ifndef __TMS_volatile
#   define	__TMS_volatile	volatile	/* works on this compiler! */
# endif

/*
|| Otherwise, we assign default values based on traditional K&R C.
*/
#else /* ! __TMS_USE_ANSI */
# ifndef __TMS_CONCAT
#   define	__TMS_CONCAT(A,B)	A/**/B
# endif
# ifndef __TMS_CONCATVAL
#   define	__TMS_CONCATVAL(A,B)	__TMS_CONCAT(A,B)	/* untested! */
# endif
# ifndef __TMS_CONCAT3
#   define	__TMS_CONCAT(A,B,C)	A/**/B/**/C
# endif
# ifndef __TMS_CONCAT3VAL
#   define	__TMS_CONCAT3VAL(A,B,C)	__TMS_CONCAT3(A,B,C)	/* untested! */
# endif
# ifndef __TMS_P
#   define	__TMS_P(not_used)		/* in case we can't use protos */
# endif
# ifndef __TMS_SKIP
#   define	__TMS_SKIP()			/* do nothing */
# endif
# ifndef __TMS_STRING
#   define	__TMS_STRING(A)	"A"
# endif
# ifndef __TMS_STRINGVAL
#   define	__TMS_STRINGVAL(A)	__TMS_STRING(A)	/* untested! */
# endif

/*
|| We special-case some defaults in case we have GCC; that's a preferred
|| alternative for libport.
*/
# ifdef __GNUC__
#   ifndef __TMS_const
#     define	__TMS_const	__const
#   endif
#   ifndef __TMS_restrict
#     define	__TMS_restrict	__restrict__
#   endif /* __TMS_restrict */
#   ifndef __TMS_schar
#     define	__TMS_schar	__signed char	/* gnu in traditional mode */
#   endif
#   ifndef __TMS_signed
#     define	__TMS_signed	__signed
#   endif
#   ifndef __TMS_void
#     define	__TMS_void	__void
#   endif
#   ifndef __TMS_volatile
#     define	__TMS_volatile	__volatile
#   endif

/*
|| Otherwise we just assume really restrictive defaults.
*/
# else  /* ndef __GNUC__ */
#   ifndef __TMS_const
#     define	__TMS_const
#   endif
#   ifndef __TMS_restrict
#     define	__TMS_restrict
#   endif
#   ifndef __TMS_schar
#     define	__TMS_schar	char
#   endif
#   ifndef __TMS_signed
#     define	__TMS_signed
#   endif
#   ifndef __TMS_void
#     define	__TMS_void	int
#   endif
#   ifndef __TMS_volatile
#     define	__TMS_volatile
#   endif
# endif /* def __GNUCC_ */
#endif /* ! __TMS_USE_ANSI */

/****************************************************************************\
|
|	__TMS_ONEWAY(fndecl) is used to declare a one-way function prototype,
|			by passing the prototopye as an argument.
|
|	__TMS_ONEWAY_DEF(defn) is used to declare a one-way function body.
|			Again the prototype is enclosed as a param.
|
\****************************************************************************/

/*
|| Part 3:  Make sure __TMS_ONEWAY and __TMS_ONEWAY_DEF is defined
|| (ARM SDK C complains about defined(foo) && (foo > 2), if foo is
|| not defined.  So we use a nonce variable.)
*/
#undef __TMS_NONCE
#define __TMS_NONCE 0
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if  (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7))
#   undef __TMS_NONCE
#   define __TMS_NONCE 1
# endif
#endif

#if __TMS_NONCE	/* defined(__GNUC__) && (__GNUC__ version >= 2.7) */
# ifndef __TMS_ONEWAY
#  define __TMS_ONEWAY(defn)		defn __attribute__ ((__noreturn__))
# endif
# ifndef __TMS_ONEWAY_DEF
#  define __TMS_ONEWAY_DEF(defn)	defn
# endif

/* either no GCC, or an early version.  Default to using VOLATILE to mark one-ways */
#else /* !defined(__GNUC__) || (__GNUC__ version < 2.7) */
# ifndef __TMS_ONEWAY
#  define __TMS_ONEWAY(defn)		__TMS_volatile defn	/* for functions that don't return */
# endif
# ifndef __TMS_ONEWAY_DEF
#  define __TMS_ONEWAY_DEF(defn)	__TMS_volatile defn	/* for functions that don't return */
# endif
#endif /* !defined(__GNUC__) || (__GNUC__ version < 2.7) */

/****************************************************************************\
|
| __TMS_LIBPORT_CPU_HARVARD
|	This is TRUE if the underlying CPU is Harvard architecture,
|	FALSE otherwise.
|
| __TMS_TYPE_CODESPACE(type) creates a new type from an old type, for the
|	same thing but living in code space.
|
| __TMS_CONST_CODESPACE_STRING(s) creates a string constant in code-space,
|	and returns a pointer. It can only be used in code; it can't be used
|	as an initializer, due to compiler portability issues.
|
| __TMS_TYPE_DEF_CONST_CODESPACE(name, type) is used to create a type variant
|	for data that resides in code space.  This is important on Harvard
|	architecture processors.
|
|	For example:
|
|	   __TMS_TYPE_DEF_CONST_CODESPACE(
|		CONST_CODESPACE_TEXT, 
|		__TMS_TEXT
|		);
|
|	will create __TMS_CONST_CODESPACE_TEXT (and optionally 
|	CONST_CODESPACE_TEXT) which is the same as TEXT except that it 
|	implicitly lives in code space.
|
|	Then you can declare an object in codespace:
|
|	    static __TMS_CONST_CODESPACE_TEXT a[] = "initializer";
|
|	This is unwieldy, but it's the most portable way of doing this.
|
|	It would be nice if we thought we could simply assume support for
|	the syntax in the ISO Embedded C standard (TR 18037:2006), but I 
|	don't want to wire that into Libport. Maybe in MCCIXDK.
|
|	This could actually be defaulted to use __TMS_TYPE_CODESPACE() but
|	we're presently paranoid about compatibility, and bugs in compilers.
|
\****************************************************************************/

#ifndef __TMS_LIBPORT_CPU_HARVARD
# define __TMS_LIBPORT_CPU_HARVARD 0

# ifdef __TMS_TYPE_CODESPACE
#  error "ccdefs.h didn't define __TMS_LIBPORT_CPU_HARVARD, but did define __TMS_TYPE_CODESPACE"
# endif
# ifdef __TMS_TPYE_DEF_CONST_CODESPACE
#  error "ccdefs.h didn't define __TMS_LIBPORT_CPU_HARVARD, but did define __TMS_TYPE_DEF_CONST_CODESPACE"
# endif
#endif /* ndef __TMS_LIBPORT_CPU_HARVARD */

#ifndef __TMS_TYPE_CODESPACE
# if __TMS_LIBPORT_CPU_HARVARD
#  error "ccdefs.h set __TMS_LIBPORT_CPU_HARVARD=1 but didn't define __TMS_TYPE_CODESPACE()"
# endif
# define __TMS_TYPE_CODESPACE(a_type)	a_type
#endif

#ifndef __TMS_TYPE_DEF_CONST_CODESPACE
# if __TMS_LIBPORT_CPU_HARVARD
#  error "ccdefs.h set __TMS_LIBPORT_CPU_HARVARD=1 but didn't define __TMS_TYPE_DEF_CONST_CODESPACE()"
# endif /* !__TMS_LIBPORT_HAS_CODESPACE */
# define __TMS_TYPE_DEF_CONST_CODESPACE(a_name, a_type)	\
	__TMS_TYPE_DEF(a_name, __TMS_CONST a_type)
#endif /* ndef __TMS_TYPE_DEF_CONST_CODESPACE */

#ifndef __TMS_CONST_CODESPACE_STRING
# if __TMS_LIBPORT_CPU_HARVARD
#  error "ccdefs.h set __TMS_LIBPORT_CPU_HARVARD=1 but didn't define __TMS_CONST_CODESPACE_STRING()"
# endif /* !__TMS_LIBPORT_HAS_CODESPACE */
# define __TMS_CONST_CODESPACE_STRING(s) s
#endif

/****************************************************************************\
|
| __TMS_LIBPORT_C_ASSERT(e)
|
|	A special typedef that will emit a compile error if (e) is FALSE.
|	(e) need not be a pre-processor constant, but must be a compile-
|	time constant.
|
\****************************************************************************/

#ifndef __TMS_LIBPORT_C_ASSERT
# define __TMS_LIBPORT_C_ASSERT(e) \
 void  __TMS_LIBPORT_C_ASSERT__(int __TMS_LIBPORT_C_ASSERT_x[(e) ? 1: -1])
#endif

/****************************************************************************\
|
| Packed structure definitions:
|
|	__TMS_LIBPORT_PACKED1(param-or-var-decl)
|	__TMS_LIBPORT_PACKED2(param-or-var-decl)
|	__TMS_LIBPORT_PACKED4(param-or-var-decl)
|	__TMS_LIBPORT_PACKED8(param-or-var-decl)
|
|	__TMS_LIBPORT_PACKED1_DEFINITION(struct x)
|	__TMS_LIBPORT_PACKED2_DEFINITION(struct x)
|	__TMS_LIBPORT_PACKED4_DEFINITION(struct x)
|	__TMS_LIBPORT_PACKED8_DEFINITION(struct x)
|
|	__TMS_LIBPORT_HAVE_PUSHPACK -- a boolean
|
| We provide default definitions for GCC if these are not defined.
| For other compilers, we #error if they're not pre-defined.  Be aware
| that these may not do what you think!  Read the documentation about what
| they do, as they don't guarantee tight packing.
|
\****************************************************************************/

/*
|| PACKED1_SUFFIX maps more or less onto __attribute__((__packed__)) for GCC;
|| otherwise syntax if used
*/
#ifndef __TMS_LIBPORT_PACKED1
# if !defined(__GNUC__)
#  define __TMS_LIBPORT_PACKED1(x)	__BOGUS_PACKED1__ x
# else
#  define __TMS_LIBPORT_PACKED1(x)	x
# endif
#endif

#ifndef __TMS_LIBPORT_PACKED1_DEFINITION
# if !defined(__GNUC__)
#  define __TMS_LIBPORT_PACKED1_DEFINITION(x)	__BOGUS_PACKED1_DEFINITION__ x
# else
#  define __TMS_LIBPORT_PACKED1_DEFINITION(x)	x
# endif
#endif /* !defined(__TMS_LIBPORT_PACKED1_DEFINITION) */

#ifndef __TMS_LIBPORT_PACKED1_SUFFIX
# if !defined(__GNUC__)
#  if defined(__TMS_LIBPORT_PACKED1_DEFINITION)
#    undef __TMS_LIBPORT_PACKED1_DEFINITION
#    define __TMS_LIBPORT_PACKED1_DEFINITION(x)	__BOGUS_PACKED1_DEFINITION__ x
#  endif
#  define __TMS_LIBPORT_PACKED1_SUFFIX	struct /* an error token! */
# else
#  define __TMS_LIBPORT_PACKED1_SUFFIX  __attribute__((__packed__))
# endif
#endif

/*
|| GCC: packed2, 4, 8 don't have a native equivalent.
*/
#ifndef __TMS_LIBPORT_PACKED2
# if !defined(__GNUC__)
#  define __TMS_LIBPORT_PACKED2(x)	__BOGUS_PACKED2__ x
# else
#  define __TMS_LIBPORT_PACKED2(x)	x
# endif
#endif

#ifndef __TMS_LIBPORT_PACKED2_DEFINITION
# if !defined(__GNUC__)
#  define __TMS_LIBPORT_PACKED2_DEFINITION(x)	__BOGUS_PACKED2_DEFINITION__ x
# else
#  define __TMS_LIBPORT_PACKED2_DEFINITION(x)	x
# endif
#endif /* !defined(__TMS_LIBPORT_PACKED2_DEFINITION) */

#ifndef __TMS_LIBPORT_PACKED2_SUFFIX
# if !defined(__GNUC__)
#  if defined(__TMS_LIBPORT_PACKED2_DEFINITION)
#    undef __TMS_LIBPORT_PACKED2_DEFINITION
#    define __TMS_LIBPORT_PACKED2_DEFINITION(x)	__BOGUS_PACKED2_DEFINITION__ x
#  endif
#  define __TMS_LIBPORT_PACKED2_SUFFIX	struct /* an error token! */
# else
#  define __TMS_LIBPORT_PACKED2_SUFFIX  /* nothing */
# endif
#endif

#ifndef __TMS_LIBPORT_PACKED4
# if !defined(__GNUC__)
#  define __TMS_LIBPORT_PACKED4(x)	__BOGUS_PACKED4__ x
# else
#  define __TMS_LIBPORT_PACKED4(x)	x
# endif
#endif

#ifndef __TMS_LIBPORT_PACKED4_DEFINITION
# if !defined(__GNUC__)
#  define __TMS_LIBPORT_PACKED4_DEFINITION(x)	__BOGUS_PACKED4_DEFINITION__ x
# else
#  define __TMS_LIBPORT_PACKED4_DEFINITION(x)	x
# endif
#endif /* !defined(__TMS_LIBPORT_PACKED4_DEFINITION) */

#ifndef __TMS_LIBPORT_PACKED4_SUFFIX
# if !defined(__GNUC__)
#  if defined(__TMS_LIBPORT_PACKED4_DEFINITION)
#    undef __TMS_LIBPORT_PACKED4_DEFINITION
#    define __TMS_LIBPORT_PACKED4_DEFINITION(x)	__BOGUS_PACKED4_DEFINITION__ x
#  endif
#  define __TMS_LIBPORT_PACKED4_SUFFIX	struct /* an error token! */
# else
#  define __TMS_LIBPORT_PACKED4_SUFFIX  /* nothing */
# endif
#endif

#ifndef __TMS_LIBPORT_PACKED8
# if !defined(__GNUC__)
#  define __TMS_LIBPORT_PACKED8(x)	__BOGUS_PACKED8__ x
# else
#  define __TMS_LIBPORT_PACKED8(x)	x
# endif
#endif

#ifndef __TMS_LIBPORT_PACKED8_DEFINITION
# if !defined(__GNUC__)
#  define __TMS_LIBPORT_PACKED8_DEFINITION(x)	__BOGUS_PACKED8_DEFINITION__ x
# else
#  define __TMS_LIBPORT_PACKED8_DEFINITION(x)	x
# endif
#endif /* !defined(__TMS_LIBPORT_PACKED8_DEFINITION) */

#ifndef __TMS_LIBPORT_PACKED8_SUFFIX
# if !defined(__GNUC__)
#  if defined(__TMS_LIBPORT_PACKED8_DEFINITION)
#    undef __TMS_LIBPORT_PACKED8_DEFINITION
#    define __TMS_LIBPORT_PACKED8_DEFINITION(x)	__BOGUS_PACKED8_DEFINITION__ x
#  endif
#  define __TMS_LIBPORT_PACKED8_SUFFIX	struct /* an error token! */
# else
#  define __TMS_LIBPORT_PACKED8_SUFFIX  /* nothing */
# endif
#endif

/*
|| GCC doesn't have pushpackn.h / poppack.h
*/
#ifndef __TMS_LIBPORT_HAVE_PUSHPACK
# if defined(__GNUC__)
#   define __TMS_LIBPORT_HAVE_PUSHPACK	0
# endif
#endif /* !defined(__TMS_LIBPORT_HAVE_PUSHPACK) */

/****************************************************************************\
|
| Libport portability keywords
|
| __TMS_ROM is the keyword that, applied to a declaration, signals
|	that the associated data is to be placed in read-only memory (i.e.,
|	in the program text segment).  Note, however, that this notation is
|	really just an abbreviation for const. On Harvard architecture
|	processors you may have to do something more.
|
| __TMS_BSS is the initializer for variables that being declared.
|	On most compilers all you need to do is say:
|		int x;
|	and x is instantiated.  But on a few, you must say
|		int x = 0;
|	So we normally say:
|		int x __TMS_BSS;
|	On most compilers, therefore, __TMS_BSS is the null string.
|
| __TMS_TRYVAR is the type tag that must be applied to variables that
|	have to retain their values when an exception is raised (or a
|	longjump() is performed).  Normally this is the volatile keyword.
|
| __TMS_OFFSET() is the offsetof() macro.  It's easier to have our own,
|	because of the nasty variations in compiler implementations.
|
| __TMS_LIBPORT_MAX(x,y) is a shortcut for the C expression (x > y ? x : y)
|
| __TMS_LIBPORT_MIN(x,y) is a shortcut for the C expression (x < y ? x : y)
|
\****************************************************************************/


/*
|| Part 4:  Establish the remaining keywords
*/
#ifndef __TMS_CONST
#  define	__TMS_CONST	__TMS_const
#endif

#ifndef __TMS_ROM
#  define	__TMS_ROM	__TMS_const	/* works for us, too.	   */
#endif

#ifndef __TMS_BSS
#  define	__TMS_BSS			/* nothing */
#endif

#ifndef __TMS_VOLATILE
#  define	__TMS_VOLATILE	__TMS_volatile
#endif

#ifndef __TMS_TRYVAR
#  define	__TMS_TRYVAR	__TMS_volatile	/* ANSI sez -- for TRY blocks */
#endif

#ifndef __TMS_OFFSET
#  define __TMS_OFFSET(s,f)	((__TMS_BYTES) &(((s *)0)->f))
#endif

#ifndef __TMS_LIBPORT_MAX
#  define __TMS_LIBPORT_MAX(a,b)	((a) > (b) ? (a) : (b))
#endif

#ifndef __TMS_LIBPORT_MIN
#  define __TMS_LIBPORT_MIN(a,b)	((a) < (b) ? (a) : (b))
#endif

#ifndef	__TMS_LIBPORT_LENOF
#  define __TMS_LIBPORT_LENOF(a)	(sizeof(a) / sizeof((a)[0]))
#endif
/*
|| find the containing object.
*/
#ifndef __TMS_CONTAINER_OF
# define __TMS_CONTAINER_OF(p, TypeOfContainer, FieldInContainer) \
	((TypeOfContainer *) (((char *) (p)) - __TMS_OFFSET(TypeOfContainer, FieldInContainer)))
#endif

/*
|| __TMS_VA_LIST_REF:  a macro for the type of a va_list copy or result.
|| Must be a macro because we don't want to force va_list to be in scope
|| in every compile; and in most cases this will expand to va_list.
*/
#ifndef __TMS_VA_LIST_REF
# define __TMS_VA_LIST_REF	va_list
#endif

/*
|| __TMS_VA_LIST_COPY:  a macro for the type of a va_list copy.
|| Most cases this will expand to assign va_list.
*/
#ifndef __TMS_VA_LIST_COPY
# define __TMS_VA_LIST_COPY(dest, src)	((dest) = (src))
#endif


/****************************************************************************\
|
| Part 5:  define the standard types.
|
|	size_t	__TMS_SIZE_T, __TMS_ARG_SIZE_T.
|
|	for:
|	TEXT	__TMS_{TEXT, PTEXT, ARG_TEXT, PARG_TEXT}
|	CHAR	__TMS_{CHAR, PCHAR, ARG_CHAR, PARG_CHAR}
|	SCHAR	__TMS_{SCHAR, PSCHAR, ARG_SCHAR, PARG_SCHAR}
|	UCHAR	__TMS_{UCHAR, PUCHAR, ARG_UCHAR, PARG_UCHAR}
|	SHORT		"
|	USHORT		"
|	INT	__TMS_INT, __TMS_PINT
|	UINT	__TMS_UINT, __TMS_PUINT
|	LONG	__TMS_LONG, __TMS_PLONG
|	ULONG		"
|
|	ARGINT	__TMS_ARGINT, __TSM_PARGINT
|	BOOL	__TMS_{BOOL, PBOOL, ARG_BOOL, PARG_BOOL}
|				(also __TMS_TRUE and __TMS_FALSE)
|	COUNT		"
|	METACH		"
|	BITS		"
|	BYTES		"
|	VOID	__TMS_VOID, __TMS_PVOID
|
|	INT8	__TMS_{INT8, PINT8, ARG_INT8, PARG_INT8}
|	UINT8		"
|	INT16		"
|	UINT16		"
|	INT32		"
|	UINT32		"
|
\****************************************************************************/

/**** now that __TMS_CONCAT is defined, it's safe to use the following ****/
#ifndef _DEF_TYPEMACS_H_
# include "def/typemacs.h"
#endif

/**** ansi.h comes from the arch; we get it in order to define size_t and so forth */
#ifndef __TMS_USE_STDDEF_FOR_SIZE_T
# define __TMS_USE_STDDEF_FOR_SIZE_T 0
#endif

#if __TMS_USE_STDDEF_FOR_SIZE_T
# include <stddef.h>
# define __TMS_ARG_SIZE_T_ size_t
#else
# ifndef _DEF_ANSI_H_
#  include "def/ansi.h"
# endif
#endif /* __TMS_USE_STDDEF_FOR_SIZE_T */

/**** the usual dodge ****/
#ifdef __TMS_SIZE_T_
  typedef __TMS_SIZE_T_	size_t;
# undef	  __TMS_SIZE_T_
#endif

#ifdef __TMS_ARG_SIZE_T_
  typedef __TMS_ARG_SIZE_T_ __TMS_ARG_SIZE_T;
# undef	  __TMS_ARG_SIZE_T_
#endif

#ifndef __TMS_SIZE_T_DEFINED
# define __TMS_SIZE_T_DEFINED
__TMS_TYPE_DEF_CLOAK(SIZE_T, size_t);
#endif

#ifndef __TMS_PARG_SIZE_T_DEFINED
# define __TMS_PARG_SIZE_T_DEFINED
typedef	__TMS_ARG_SIZE_T *__TMS_PARG_SIZE_T;
#endif

/**** next, load the target's type overrides ****/
#ifndef _DEF_CCTYPES_H_
# include "def/cctypes.h"
#endif

/**** from here on, we use a more normal means of protecting the definitions ****/
#ifndef __TMS_TEXT_DEFINED
# define __TMS_TEXT_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(TEXT, char, int);		/* the basic character */
#endif

#ifndef __TMS_CHAR_DEFINED
# define __TMS_CHAR_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(CHAR, char, int);		/* the standard char type */
#endif

#ifndef __TMS_SCHAR_DEFINED
# define __TMS_SCHAR_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(SCHAR, __TMS_schar, int);	/* the signed character */
#endif

#ifndef __TMS_UCHAR_DEFINED
# define __TMS_UCHAR_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(UCHAR, unsigned char, unsigned);	/* the standard unsigned char type */
#endif

#ifndef __TMS_SHORT_DEFINED
# define __TMS_SHORT_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(SHORT, short, int);		/* the standard short type */
#endif

#ifndef __TMS_USHORT_DEFINED
# define __TMS_USHORT_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(USHORT, unsigned short, unsigned);	/* the standard unsigned short type */
#endif

#ifndef __TMS_INT_DEFINED
# define __TMS_INT_DEFINED
__TMS_TYPE_DEF_CLOAK(INT, int);
#endif

#ifndef __TMS_UINT_DEFINED
# define __TMS_UINT_DEFINED
__TMS_TYPE_DEF_CLOAK(UINT, unsigned);
#endif

#ifndef __TMS_LONG_DEFINED
# define __TMS_LONG_DEFINED
__TMS_TYPE_DEF_CLOAK(LONG, long);			/* the standard long type */
#endif

#ifndef __TMS_ULONG_DEFINED
# define __TMS_ULONG_DEFINED
__TMS_TYPE_DEF_CLOAK(ULONG, unsigned long);		/* the standard ulong type */
#endif

#ifndef __TMS_UINT_PTR_DEFINED
# define __TMS_UINT_PTR_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(UINT_PTR, unsigned, unsigned); 
			/* can hold an arb ptr (to function or data) */
#endif

#ifndef __TMS_UINT_VOIDPTR_DEFINED
# define __TMS_UINT_VOIDPTR_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(UINT_VOIDPTR, unsigned, unsigned);
			/* can hold any void ptr */
#endif

#ifndef __TMS_UINT_FNPTR_DEFINED
# define __TMS_UINT_FNPTR_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(UINT_FNPTR, unsigned, unsigned);
			/* can hold an arb function ptr */
#endif


/* __TMS_ARGINT is the type of widened args -- it should always be int */
#ifndef __TMS_ARGINT_DEFINED
# define __TMS_ARGINT_DEFINED
__TMS_TYPE_DEF_CLOAK(ARGINT, int);
#endif

/* 
|| BOOL is the type of booleans;  __TMS_FALSE and __TMS_TRUE are the
|| values.
*/
#ifndef __TMS_BOOL_DEFINED
# define __TMS_BOOL_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(BOOL, int, int);
#endif

#ifndef	__TMS_FALSE
# define __TMS_FALSE	0
#endif

#ifndef __TMS_TRUE
# define __TMS_TRUE	(!__TMS_FALSE)
#endif

/* a count is the most convenient int on this arch */
#ifndef __TMS_COUNT_DEFINED
# define __TMS_COUNT_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(COUNT, int, int);
#endif

#ifndef __TMS_METACH_DEFINED
# define __TMS_METACH_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(METACH, int, int);	/* holds a CHAR, or EOF */
#endif

#ifndef __TMS_BITS_DEFINED
# define __TMS_BITS_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(BITS, USHORT);		/* holds a small bitmask */
#endif

/* if size_t is signed, better change the following -- BYTES must be unsigned! */
#ifndef __TMS_BYTES_DEFINED
# define __TMS_BYTES_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(BYTES, size_t, size_t);
#endif

#ifndef __TMS_VOID_DEFINED
# define __TMS_VOID_DEFINED
__TMS_TYPE_DEF_CLOAK(VOID, __TMS_void);		/* the standard "no type* type */
#endif

/* the following types are the smallest types that have exactly n bits */
/* these are ONLY to be used with external representations */
#ifndef __TMS_INT8_DEFINED
# define __TMS_INT8_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(INT8, SCHAR);		/* 8 bit (signed) */
#endif

#ifndef __TMS_SINT8_DEFINED
# define __TMS_SINT8_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(SINT8, SCHAR);		/* 8 bit (definitely signed) */
#endif

#ifndef __TMS_UINT8_DEFINED
# define __TMS_UINT8_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(UINT8, UCHAR);		/* 8 bit (unsigned) */
#endif

#ifndef __TMS_INT16_DEFINED
# define __TMS_INT16_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(INT16, SHORT);
#endif

#ifndef __TMS_UINT16_DEFINED
# define __TMS_UINT16_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(UINT16, USHORT);
#endif

#ifndef	__TMS_INT32_DEFINED
# define __TMS_INT32_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(INT32, int, int);
#endif

#ifndef __TMS_UINT32_DEFINED
# define __TMS_UINT32_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(UINT32, unsigned, unsigned);
#endif

/****************************************************************************\
|
|  64-bit support definitions follow.  The defaults taken follow ISO C99
|  conventions.  In this section, if the compiler has 64-bit support, we
|  define the basic symbols for the exact-width 64-bit types; and we define
|  the PRIx64() and SCNx64() macros as well.  All definitions can be
|  overridden in the def/cctyps.h file, if needed.  For VC6 and VC7, they
|  must be so overridden.
|
\****************************************************************************/

#if __TMS_LIBPORT_HAS_64BIT
# ifndef __TMS_INT64_DEFINED
#  define __TMS_INT64_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(INT64, long long int, long long int);
# endif /* __TMS_INT64_DEFINED */

# ifndef __TMS_LIBPORT_INT64_C
#  define __TMS_LIBPORT_INT64_C(x)	x ## ll
# endif /* __TMS_LIBPORT_INT64_C */

# ifndef __TMS_UINT64_DEFINED
#  define __TMS_UINT64_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(UINT64, long long unsigned, long long unsigned);
# endif /* __TMS_UINT64_DEFINED */

# ifndef __TMS_LIBPORT_UINT64_C
#  define __TMS_LIBPORT_UINT64_C(x)	x ## ull
# endif /* __TMS_LIBPORT_UINT64_C */

# ifndef __TMS_LLONG_DEFINED
#  define __TMS_LLONG_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(LLONG, INT64);
# endif /* __TMS_LLONG_DEFINED */

# ifndef __TMS_ULLONG_DEFINED
#  define __TMS_ULLONG_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(ULLONG, UINT64);
# endif /* __TMS_ULLONG_DEFINED */

/* things from c99 inttypes.h */
# ifndef __TMS_LIBPORT_PRId64
#  define __TMS_LIBPORT_PRId64	"lld"
# endif
# ifndef __TMS_LIBPORT_PRIi64
#  define __TMS_LIBPORT_PRIi64	"lli"
# endif
# ifndef __TMS_LIBPORT_PRIo64
#  define __TMS_LIBPORT_PRIo64	"llo"
# endif
# ifndef __TMS_LIBPORT_PRIu64
#  define __TMS_LIBPORT_PRIu64	"llu"
# endif
# ifndef __TMS_LIBPORT_PRIx64
#  define __TMS_LIBPORT_PRIx64	"llx"
# endif
# ifndef __TMS_LIBPORT_PRIX64
#  define __TMS_LIBPORT_PRIX64	"llX"
# endif
# ifndef __TMS_LIBPORT_SCNd64
#  define __TMS_LIBPORT_SCNd64	"lld"
# endif
# ifndef __TMS_LIBPORT_SCNi64
#  define __TMS_LIBPORT_SCNi64	"lli"
# endif
# ifndef __TMS_LIBPORT_SCNo64
#  define __TMS_LIBPORT_SCNo64	"llo"
# endif
# ifndef __TMS_LIBPORT_SCNu64
#  define __TMS_LIBPORT_SCNu64	"llu"
# endif
# ifndef __TMS_LIBPORT_SCNx64
#  define __TMS_LIBPORT_SCNx64	"llx"
# endif

#endif /* __TMS_LIBPORT_HAS_64BIT */

/*
|| Define the UINT64_S structure.
*/
#ifndef	__TMS_LIBPORT_UINT64_S_DEFINED
# define __TMS_LIBPORT_UINT64_S_DEFINED

  /* define the basic type for UINT64_S */
  __TMS_TYPE_DEF_UNION_CLOAK	(UINT64_S);
  
  /* 
  || The order of the fields are chosen to start from the largest available
  || type to the smallest available type in size.  This is to allow the
  || compiler to initialize the union to the size of the largest element.
  */
  union __TMS_UNIONNAME	(UINT64_S)
	{
# if __TMS_LIBPORT_HAS_64BIT
	__TMS_UINT64 q;
# endif
	__TMS_UINT32 ud[2];
	__TMS_UINT16 uw[4];
	__TMS_UINT8 uc[8];
	};
#endif	/* __TMS_LIBPORT_UINT64_S_DEFINED */

/*
|| Define the INT64_S structure.
*/
#ifndef	__TMS_LIBPORT_INT64_S_DEFINED
# define __TMS_LIBPORT_INT64_S_DEFINED

  /* define the basic type INT64_S */
  __TMS_TYPE_DEF_UNION_CLOAK	(INT64_S);
  
  /* 
  || The order of the fields are chosen to start from the largest available
  || type to the smallest available type in size.  This is to allow the
  || compiler to initialize the union to the size of the largest element.
  */  
  union __TMS_UNIONNAME	(INT64_S)
	{
# if __TMS_LIBPORT_HAS_64BIT
	__TMS_INT64 q;
# endif
	__TMS_INT32 sd[2];
	__TMS_UINT32 ud[2];
	__TMS_UINT16 uw[4];
	__TMS_UINT8 uc[8];
	};
#endif	/* __TMS_LIBPORT_INT64_S_DEFINED */

/****************************************************************************\
|
|  128-bit support definitions follow.  ISO C99 doesn't have any prior art
|  that we can follow, so we don't define the PRIx64 and SCNx64 macros.
|
\****************************************************************************/

/* first of all, 128-bit support requires 64-bit support */
#if __TMS_LIBPORT_HAS_128BIT && !__TMS_LIBPORT_HAS_64BIT
#   error "Compiler has 128-bit support but not 64-bit support!"
#endif

/*
|| Define the UINT128_S structure.
*/
#ifndef	__TMS_LIBPORT_UINT128_S_DEFINED
# define __TMS_LIBPORT_UINT128_S_DEFINED

  /* define the basic type for UINT128_S */
  __TMS_TYPE_DEF_UNION_CLOAK	(UINT128_S);

  /* 
  || The order of the fields are chosen to start from the largest available
  || type to the smallest available type in size.  This is to allow the
  || compiler to initialize the union to the size of the largest element.
  */  
  union __TMS_UNIONNAME	(UINT128_S)
	{
# if __TMS_LIBPORT_HAS_64BIT
#  if __TMS_LIBPORT_HAS_128BIT
        __TMS_UINT128 qq;
#  endif
	__TMS_UINT64 uq[2];
# endif
	__TMS_UINT32 ud[4];
	__TMS_UINT16 uw[8];
	__TMS_UINT8 uc[16];
	};
#endif	/* __TMS_LIBPORT_UINT128_S_DEFINED */

/*
|| Define the INT128_S structure.
*/
#ifndef	__TMS_LIBPORT_INT128_S_DEFINED
# define __TMS_LIBPORT_INT128_S_DEFINED

  /* define the basic type INT128_S */
  __TMS_TYPE_DEF_UNION_CLOAK	(INT128_S);
  
  /* 
  || The order of the fields are chosen to start from the largest available
  || type to the smallest available type in size.  This is to allow the
  || compiler to initialize the union to the size of the largest element.
  */  
  union __TMS_UNIONNAME	(INT128_S)
	{
# if __TMS_LIBPORT_HAS_64BIT
#  if __TMS_LIBPORT_HAS_128BIT
        __TMS_INT128 qq;
#  endif
	__TMS_INT64 sq[2];
	__TMS_UINT64 uq[2];
# endif
        __TMS_UINT32 ud[4];
        __TMS_UINT16 uw[8];
        __TMS_UINT8 uc[16];
	};
#endif	/* __TMS_LIBPORT_INT128_S_DEFINED */

/*
|| Check that the C compiler support package did the right thing and 
|| defined all the aux variables.
*/

#if __TMS_LIBPORT_HAS_128BIT
# if !__TMS_LIBPORT_HAS_64BIT
#   error "Compiler has 128-bit support but not 64-bit support!"
# endif
# ifndef __TMS_INT128_DEFINED
#  error "Compiler has 128-bit support but __TMS_INT128_DEFINED is not set"
# endif /* __TMS_INT128_DEFINED */

# ifndef __TMS_LIBPORT_INT128_C
#  error "Compiler has 128-bit support but __TMS_LIBPORT_INT128_C is not set"
# endif /* __TMS_LIBPORT_INT128_C */

# ifndef __TMS_UINT128_DEFINED
#  error "Compiler has 128-bit support but __TMS_UIN128_DEFINED is not set"
# endif /* __TMS_UINT128_DEFINED */

# ifndef __TMS_LIBPORT_UINT128_C
#  error "Compiler has 128-bit support but __TMS_LIBPORT_UINT128_C is not set"
# endif /* __TMS_LIBPORT_UINT128_C */

# ifndef __TMS_LLLONG_DEFINED
#  define __TMS_LLONG_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(LLLONG, INT128);
# endif /* __TMS_LLLONG_DEFINED */

# ifndef __TMS_ULLLONG_DEFINED
#  define __TMS_ULLLONG_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(ULLLONG, UINT128);
# endif /* __TMS_ULLLONG_DEFINED */

/* things from c99 inttypes.h */
# ifndef __TMS_LIBPORT_PRId128
#  error "Compiler has 128-bit support but __TMS_LIBPORT_PRId128 is not set"
# endif
# ifndef __TMS_LIBPORT_PRIi128
#  error "Compiler has 128-bit support but __TMS_LIBPORT_PRIi128 is not set"
# endif
# ifndef __TMS_LIBPORT_PRIo128
#  error "Compiler has 128-bit support but __TMS_LIBPORT_PRIo128 is not set"
# endif
# ifndef __TMS_LIBPORT_PRIu128
#  error "Compiler has 128-bit support but __TMS_LIBPORT_PRIu128 is not set"
# endif
# ifndef __TMS_LIBPORT_PRIx128
#  error "Compiler has 128-bit support but __TMS_LIBPORT_PRIx128 is not set"
# endif
# ifndef __TMS_LIBPORT_PRIX128
#  error "Compiler has 128-bit support but __TMS_LIBPORT_PRIX128 is not set"
# endif
# ifndef __TMS_LIBPORT_SCNd128
#  error "Compiler has 128-bit support but __TMS_LIBPORT_SCNd128 is not set"
# endif
# ifndef __TMS_LIBPORT_SCNi128
#  error "Compiler has 128-bit support but __TMS_LIBPORT_SCNi128 is not set"
# endif
# ifndef __TMS_LIBPORT_SCNo128
#  error "Compiler has 128-bit support but __TMS_LIBPORT_SCNo128 is not set"
# endif
# ifndef __TMS_LIBPORT_SCNu128
#  error "Compiler has 128-bit support but __TMS_LIBPORT_SCNu128 is not set"
# endif
# ifndef __TMS_LIBPORT_SCNx128
#  error "Compiler has 128-bit support but __TMS_LIBPORT_SCNx128 is not set"
# endif

#endif /* __TMS_LIBPORT_HAS_128BIT */

/****************************************************************************\
|
| Architectural stuff
|	By default, we assume that the underlying machine is pure 32-bit
|	flat.  Also, we assume that it's big-endian, because more of
|	our ports seem to be big-endian.  All of these can be overridden
|	in the __TMS_CCDEFS_H file.
|
|  __TMS_USE_SELECTORS	is 0 if we dont' have selectors.
|  __TMS_S(t)		is t (or the selector type if we have selectors)
|  __TMS_ARBPTR		is the arbitrary pointer type
|  __TMS_PHYSADDR	is the integer type that holds a physical address.
|  __TMS_ADDRBITS	is the integer type that holds a logical address
|			(or CPU pointer -- must be large enough to span
|			entire address space, not just a single segment on x86)
|			ADDRBITS is equiv to UINT_PTR in other type systems.
|  __TMS_RECSIZE	The length of a buffer (also __TMS_ARG_RECSIZE) --
|			the natural type of the difference between two pointers
|			(assuming same segment on segmented arch).
|  __TMS_ADDRDIFF	is the signed integer type that holds the difference
|			between two pointers, possibly with overflow; but at
|			least as wide as __TMS_ADDRBITS.
|  __TMS_CPUFLAGS	holds a PSW.  We assume it's an unsigned int. (ARG)
|			Also __TMS_ARG_CPUFLAGS.
|  __TMS_PARAS		See selectors:  paras are to selectors as RECSIZEs
|			are to normal pointers.  Also __TMS_ARG_PARAS.
|  __TMS_IOEDITCH	Historical name; a signed type wide enough to hold a 
|			UCHAR and also -1.  Also __TMS_ARG_IOEDITCH
|
|  __TMS_SIG_ATOMIC_[U]INT	The type to be used for sig_atomic_t and
|				SIG_ATOMIC_UINT.
|
|  __TMS_INTERRUPT_ATOMIC_[U]INT The type to be used for INTERRUPT-
|				_ATOMIC_[U]INT
|
|  __TMS_CPUBUS_ATOMIC_[U]INT	The type to be used for CPUBUS_ATOMIC_[U]INT
|
|  __TMS_PARAStoADDRIBITS(P)
|			Convert Paras (the size of a buffer in selector units)
|			to addrbits (the number of bytes in the buffer -- on
|			a seg arch this might be bigger than RECSIZE).
|
|  __TMS_ADDRBITStoPARAS(P)
|			Reverse above; no rounding control
|
|  __TMS_ADDRBITStoPARAS_ROUND()
|			Revers above, but round up.
|
|  __TMS_IOPORT		the type (maybe pointer, maybe int) that carries
|			CPU I/O addresses around.  Note that with 
|			contemporary bus architectures, this may not be
|			enough info to get to the device....
|
\****************************************************************************/

#ifndef __TMS_USE_SELECTORS
# define	__TMS_USE_SELECTORS 0	/* selectors AREN'T handles */
#endif 

#ifndef __TMS_S
# if __TMS_USE_SELECTORS
#  define	__TMS_S(t)	__TMS_SELECTOR		/* we have selectors, so use them */
# else
#  define	__TMS_S(t)	t			/* Selectors don't exist on this machine,
							|| so use base type.
							*/
# endif
#endif /* !def(__TMS_S) */

/*
|| __TMS_ARBPTR replaces TEXT* in future usages, and will eventually change to
|| VOID* if we are lucky.
*/
#ifndef __TMS_ARBPTR_DEFINED
# define __TMS_ARBPTR_DEFINED
__TMS_TYPE_DEF_CLOAK(ARBPTR, char *);
#endif

/* a physical address -- as opposed to a program address */
#ifndef __TMS_PHYSADDR_DEFINED
# define __TMS_PHYSADDR_DEFINED
__TMS_TYPE_DEF_CLOAK(PHYSADDR, __TMS_UINT32);
#endif

/* a pointer, considered as a bit pattern */
#ifndef __TMS_ADDRBITS_DEFINED
# define __TMS_ADDRBITS_DEFINED
__TMS_TYPE_DEF_CLOAK(ADDRBITS, __TMS_UINT32);
#endif

/* arithmetic difference between pointers */
#ifndef __TMS_ADDRDIFF_DEFINED
# define __TMS_ADDRDIFF_DEFINED
__TMS_TYPE_DEF_CLOAK(ADDRDIFF, __TMS_INT32);
#endif

/* a union of an ADDRBITS and a VOID* */
#ifndef __TMS_ADDRBITS_PTR_UNION_DEFINED
# define __TMS_ADDRBITS_PTR_UNION_DEFINED
__TMS_TYPE_DEF_UNION_CLOAK(ADDRBITS_PTR_UNION);

union __TMS_UNIONNAME(ADDRBITS_PTR_UNION)
	{
	__TMS_ADDRBITS	Addrbits;
	__TMS_ADDRDIFF	Addrdiff;
	__TMS_VOID	*Ptr;
	};
#endif /* __TMS_ADDRBITS_PTR_UNION_DEFINED */

/* a selector on this architecture is just a cloaked pointer */
#ifndef __TMS_SELECTOR_DEFINED
# define __TMS_SELECTOR_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(SELECTOR, __TMS_ADDRBITS, __TMS_ADDRBITS);
#endif

/**** secondary derived types ****/
/* holds psw */
#ifndef __TMS_CPUFLAGS_DEFINED
# define __TMS_CPUFLAGS_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(CPUFLAGS, unsigned, unsigned);
#endif

/* PARAS holds a 'paragraph count' -- notionally related to selector-to-addrbits mapping */
#ifndef __TMS_PARAS_DEFINED
# define __TMS_PARAS_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(PARAS, BYTES);
#endif

/* RECSIZE holds the size of a buffer */
#ifndef __TMS_RECSIZE_DEFINED
# define __TMS_RECSIZE_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(RECSIZE, BYTES);
#endif

/* IOEDITCH holds UCHAR, or -1, and is efficient */
#ifndef __TMS_IOEDITCH_DEFINED
# define __TMS_IOEDITCH_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(IOEDITCH, SHORT);
#endif

/* 
|| SIG_ATOMIC_INT is ANSI sig_atomic_t; this integral (i.e., signed)
|| type is guaranteed to be atomic across signal deliveries, where
|| atomic means that the C code doesn't need to worry about interlocking
|| to change or sense a SIG_ATOMIC_INT using an '='.  Note, however,
|| that things like '++' are not necessarily safe, as it depends on
|| the compiler and CPU (which might not have an atomic '++' operation.
||
|| For completeness, and just out of paranoia, so that people don't
|| use sig_atomic_t as an 'atomic' type across interrupts, we add
|| SIG_ATOMIC_UINT, INTERRUPT_ATOMIC_[U]INT, CPUBUS_ATOMIC_[U]INT, 
|| each of which indicates a natural [unsigned] type for 'atomic' 
|| updates, across interrupts and across CPU bus cycles.  On a 16-bit 
|| 68K, INTERRUPT_ATOMIC_INT is int, but CPUBUS_ATOMIC_INT is short.
||
|| It must be that the writer's years of programming Z80s, on which 
|| signed-ness was rarely used, has made him tend to use unsigneds in 
|| pref. to ints in most of his code.
||
|| If no value is set by cpudefs.h, each defaults to the corresponding 
|| [U]SIG_ATOMIC_INT; and [U]SIG_ATOMIC_INT defaults to int/unsigned.
*/
#ifndef __TMS_SIG_ATOMIC_INT_DEFINED
# define __TMS_SIG_ATOMIC_INT_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(SIG_ATOMIC_INT, int, int);
#endif

#ifndef __TMS_SIG_ATOMIC_UINT_DEFINED
# define __TMS_SIG_ATOMIC_UINT_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(SIG_ATOMIC_UINT, unsigned, unsigned);
#endif

#ifndef __TMS_INTERRUPT_ATOMIC_INT_DEFINED
# define __TMS_INTERRUPT_ATOMIC_INT_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(INTERRUPT_ATOMIC_INT, SIG_ATOMIC_INT);
#endif

#ifndef __TMS_INTERRUPT_ATOMIC_UINT_DEFINED
# define __TMS_INTERRUPT_ATOMIC_UINT_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(INTERRUPT_ATOMIC_UINT, SIG_ATOMIC_UINT);
#endif

#ifndef __TMS_CPUBUS_ATOMIC_INT_DEFINED
# define __TMS_CPUBUS_ATOMIC_INT_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(CPUBUS_ATOMIC_INT, SIG_ATOMIC_INT);
#endif

#ifndef __TMS_CPUBUS_ATOMIC_UINT_DEFINED
# define __TMS_CPUBUS_ATOMIC_UINT_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK(CPUBUS_ATOMIC_UINT, SIG_ATOMIC_UINT);
#endif

/**** conversions ****/
#ifndef __TMS_PARAStoADDRBITS
# define __TMS_PARAStoADDRBITS(P)	((__TMS_ADDRBITS)(P))
#endif

#ifndef __TMS_ADDRBITStoPARAS
# define __TMS_ADDRBITStoPARAS(A)	((__TMS_PARAS)(A))
#endif 

#ifndef __TMS_ADDRBITStoPARAS_ROUND
# define __TMS_ADDRBITStoPARAS_ROUND(A)	((__TMS_PARAS)(A))
						/* convert to paragraphs,
						|| rounding up.
						*/
#endif

/*
|| LIBPORT_UNREFERENCED_PARAMETER causes a compiler not to emit a warning on 
|| unused sym -- use it when a parameter is intentionally unreferenced (for 
|| example because the implementation is not complete) or because this is a
|| free build rather than a checked build.
*/
#ifndef __TMS_LIBPORT_UNREFERENCED_PARAMETER
# if defined(__GNUC__)
#  define __TMS_LIBPORT_UNREFERENCED_PARAMETER(x) do { (void) (x); } while (0)
# else /* !__GNUC__ */
#  define __TMS_LIBPORT_UNREFERENCED_PARAMETER(x) __TMS_SKIP()  /* do nothing */
# endif /* !__GNUC__ */
#endif /* __TMS_LIBPORT_UNREFERENCED_PARAMETER */

/* 
|| LIBPORT_API_PARAMETER causes a compiler not to emit a warning on unused 
|| parameter -- use it when a parmaeter is passed because of an API contract,
|| but not used by the API implementation.
*/
#ifndef __TMS_LIBPORT_API_PARAMETER
# if defined(__GNUC__)
#  define __TMS_LIBPORT_API_PARAMETER(x) do { (void) (x); } while (0)
# else /* !__GNUC__ */
#  define __TMS_LIBPORT_API_PARAMETER(x) __TMS_SKIP()  /* do nothing */
# endif /* !__GNUC__ */
#endif /* __TMS_LIBPORT_API_PARAMETER */

/* 
|| LIBPORT_UNREFERENCED_VARIABLE causes a compiler not to emit a warning on 
|| unused variable -- use it for variables that are intentionally not
|| referenced (for example for debug variables).
*/
#ifndef __TMS_LIBPORT_UNREFERENCED_VARIABLE
# if defined(__GNUC__)
#  define __TMS_LIBPORT_UNREFERENCED_VARIABLE(x) do { (void) (x); } while (0)
# else /* !__GNUC__ */
#  define __TMS_LIBPORT_UNREFERENCED_VARIABLE(x) __TMS_SKIP()  /* do nothing */
# endif /* !__GNUC__ */
#endif /* __TMS_LIBPORT_API_PARAMETER */


/* I/O port addresses -- on a single address-space machine, these are pointers -- but we need to do arith */
#ifndef __TMS_IOPORT_DEFINED
# define __TMS_IOPORT_DEFINED
__TMS_TYPE_DEF_ARG_CLOAK2(IOPORT, __TMS_volatile __TMS_INT8 *, __TMS_volatile __TMS_INT8 *);
#endif

/****************************************************************************\
|
| Endianness control
|
|	If the platform defines __TMS_ENDIAN and nothing else, that
|	will control the defaults for the rest of the symbols.  If
|	the platform does not define it, we assume big-endian.
|
\****************************************************************************/


#ifndef __TMS_ENDIAN
# define __TMS_ENDIAN	__TMS_ENDIAN_BIG	/* big-endian shorts & longs */
#endif

#ifndef __TMS_FPENDIAN
# if __TMS_ENDIAN != __TMS_ENDIAN_LITTLE
#  define __TMS_FPENDIAN __TMS_QENDIAN_BIG	/* big-endian doubles */
# else
#  define __TMS_FPENDIAN __TMS_QENDIAN_LITTLE
# endif
#endif /* __TMS_FPENDIAN */

#ifndef __TMS_QENDIAN
# if __TMS_ENDIAN != __TMS_ENDIAN_LITTLE
#  define __TMS_QENDIAN	__TMS_QENDIAN_BIG	/* big-endian int64s */
# else
#  define __TMS_QENDIAN __TMS_QENDIAN_LITTLE
# endif
#endif

/* define QQENDIAN (128-bit order) based on ENDIANness for this arch */
#ifndef __TMS_QQENDIAN
# if __TMS_QENDIAN == __TMS_QENDIAN_LITTLE
#  define __TMS_QQENDIAN __TMS_QQENDIAN_LITTLE
# elif __TMS_QENDIAN == __TMS_QENDIAN_BIG
#  define __TMS_QQENDIAN __TMS_QQENDIAN_BIG	/* big-endian int128s */
# else
#    error "Can't auto-define __TMS_QQENDIAN based on value of TMS_QENDIAN"
# endif
#endif

#ifndef __TMS_BITENDIAN
# define __TMS_BITENDIAN  __TMS_ENDIAN
#endif


/**** The XBYTE macro for various architectures ****/
#ifndef __TMS_XBYTE
# if __TMS_ENDIAN == __TMS_ENDIAN_BIG
#    define __TMS_XBYTE(lv, n, t)	((__TMS_UCHAR *)&(lv))[sizeof(t) - (n) - 1]
# elif __TMS_ENDIAN == __TMS_ENDIAN_LITTLE
#    define __TMS_XBYTE(lv, n, t)	((__TMS_UCHAR *)&(lv))[(n)]
# elif __TMS_ENDIAN == __TMS_ENDIAN_PDP
#    define __TMS_XBYTE(lv, n, t)	((__TMS_UCHAR *)&(lv))[(sizeof(t) - (n) - 1) ^ 1]
# else
#    error "Can't auto-define __TMS_XBYTE based on value of TMS_ENDIAN"
# endif
#endif

/**** we must specify, per compiler, whether <limits.h> exists and is sane ***/
#ifndef __TMS_USE_LIMITS_H
# define __TMS_USE_LIMITS_H	(__TMS_USE_ANSI) /* on all ANSI compilers it is */
#endif

/**** UINT64_S, INT64_S, UINT128_S, INT128_S init macros ****/
#if __TMS_LIBPORT_HAS_64BIT
# define __TMS_LIBPORT_UINT64_S_INIT(x)		{ __TMS_LIBPORT_UINT64_C(x) }
# define __TMS_LIBPORT_INT64_S_INIT(x)		{ __TMS_LIBPORT_INT64_C(x) }
#else /* !__TMS_LIBPORT_HAS_64BIT */
# if __TMS_QQENDIAN == __TMS_QENDIAN_LITTLE
#  define __TMS_LIBPORT_UINT64_S_INIT(x)	{{ (x), 0 }}
#  define __TMS_LIBPORT_INT64_S_INIT(x)		{{ (x), (((x)<0)? -1 : 0) }}
# elif __TMS_QQENDIAN == __TMS_QENDIAN_BIG
#  define __TMS_LIBPORT_UINT64_S_INIT(x)	{{ 0, (x) }}
#  define __TMS_LIBPORT_INT64_S_INIT(x)		{{ (((x)<0)? -1 : 0), (x) }}
# else /* __TMS_QQENDIAN neither BIG nor LITTLE */
#  error "__TMS_QQENDIAN neither BIG nor LITTLE"
# endif	/* __TMS_QQENDIAN */
#endif	/* !__TMS_LIBPORT_HAS_64_BIT */

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_LIBPORT_UINT128_S_INIT(x)	{ __TMS_LIBPORT_UINT128_C(x) }
# define __TMS_LIBPORT_INT128_S_INIT(x)		{ __TMS_LIBPORT_INT128_C(x) }
#else /* !_TMS_LIBPORT_HAS_128BIT */
# if __TMS_QQENDIAN == __TMS_QENDIAN_LITTLE
#  if __TMS_LIBPORT_HAS_64BIT
#   define __TMS_LIBPORT_UINT128_S_INIT(x)	{{__TMS_LIBPORT_UINT64_C(x), 0}}
#   define __TMS_LIBPORT_INT128_S_INIT(x)	\
	{{ __TMS_LIBPORT_INT64_C(x), ((x)<0) ? __TMS_LIBPORT_INT64_C(-1) : 0 }}
#  else /* !__TMS_LIBPORT_HAS_64_BIT */
#   define __TMS_LIBPORT_UINT128_S_INIT(x)	{{ (x), 0, 0, 0 }}
#   define __TMS_LIBPORT_INT128_S_INIT(x)	\
	{{ (x), (((x)<0) ? -1 : 0), (((x)<0) ? -1 : 0), (((x)<0) ? -1 : 0) }}
#  endif /* __TMS_LIBPORT_HAS_64_BIT */
# elif __TMS_QQENDIAN == __TMS_QENDIAN_BIG
#  if __TMS_LIBPORT_HAS_64BIT
#   define __TMS_LIBPORT_UINT128_S_INIT(x)	{{0, __TMS_LIBPORT_UINT64_C(x)}}
#   define __TMS_LIBPORT_INT128_S_INIT(x)	\
	{{ ((x)<0) ? __TMS_LIBPORT_INT64_C(-1) : 0, __TMS_LIBPORT_INT64_C(x) }}
#  else /* !__TMS_LIBPORT_HAS_64_BIT */
#   define __TMS_LIBPORT_UINT128_S_INIT(x)	{{ 0, 0, 0, (x) }}
#   define __TMS_LIBPORT_INT128_S_INIT(x)	\
	{{ (((x)<0) ? -1 : 0), (((x)<0) ? -1 : 0), (((x)<0) ? -1 : 0), (x) }}
#  endif /* __TMS_LIBPORT_HAS_64_BIT */
# else /* __TMS_QQENDIAN neither BIG nor LITTLE */
#  error "__TMS_QQENDIAN neither BIG nor LITTLE"
# endif /* __TMS_QQENDIAN */
#endif /* !__TMS_LIBPORT_HAS_128_BIT */

/****************************************************************************\
|
|	Finally, check that everything was set up consistently.  Any errors
|	here mean that you've got a consistency problem.
|
\****************************************************************************/

__TMS_LIBPORT_C_ASSERT(sizeof(__TMS_ADDRBITS) == sizeof(__TMS_VOID *));
__TMS_LIBPORT_C_ASSERT(sizeof(__TMS_ADDRBITS) == sizeof(__TMS_ADDRDIFF));
__TMS_LIBPORT_C_ASSERT(sizeof(__TMS_ADDRBITS) == sizeof(__TMS_UINT_PTR));
__TMS_LIBPORT_C_ASSERT(sizeof(__TMS_PHYSADDR) >= sizeof(__TMS_ADDRBITS));

/**** end of def/cdefs.h ****/
#endif /* _DEF_CDEFS_H_ */
