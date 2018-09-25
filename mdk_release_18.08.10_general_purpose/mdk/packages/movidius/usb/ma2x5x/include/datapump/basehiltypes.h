/* basehiltypes.h	Mon Jun  1 2015 00:18:35 tmm */

/*

Module:  basehiltypes.h

Function:
	The basic types for the USB hardware interface layer

Version:
	V3.15b	Mon Jun  1 2015 00:18:35 tmm	Edit level 9

Copyright notice:
	This file copyright (C) 2001-2002, 2005, 2007, 2009, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	November 2001

Revision history:
   1.63d  Thu Nov  8 2001 08:07:33  tmm
	Module created.

   1.63f  24/3/2002  maw
	Moved UHIL_INTERRUPT_RESOURCE_HANDLE to basehiltypes.h

   1.79a  6/6/2002  tmm
	Major header file reorganization.  Add UCALLBACKFN etc. here.

   1.91g  2005-07-21  tmm
	Add UTASK_ROOT_CONFIG typedef here, so it can be used in the
	platform APIs.
	
   1.93c  Fri Dec 09 2005 16:46:57  tmm
   	Force __USBRCTAB_WANT_V3 to be 1 during any compile, to work around
   	usbrctab.h backward/forward compatibility issues.

   1.97j  Tue Nov 27 2007 09:21:25  tmm
	3528:  clean compile for Microsoft Visual C V6.0 with /W4.
	In this file's case, this means we need to have the MSC pragmas
	in-scope.

   2.01a  Sun May 17 2009 10:59:45  tmm
	Add USBPUMP_SESSION_HANDLE and USBPUMP_UHILAUX_INCALL to the 
	global types, because we need them in UPLATFORM for UHILAUX.

   3.15b  Thu Apr  2 2015 15:09:10  tmm
	19107: Add UHIL_ISR_INTERLOCK_STATE.

   3.15b  Mon Jun  1 2015 00:18:35  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _BASEHILTYPES_H_		/* prevent multiple includes */
#define _BASEHILTYPES_H_

/* we need the mangled definitions */
#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif

/* we need the MSC pragmas, maybe */
#ifndef _USBPUMP_MSC_PRAGMAS_H_
# include "usbpump_msc_pragmas.h"
#endif

/****************************************************************************\
|
|	Fundamental forward types.  For historical reasons, these don't
|	all follow MCCI/libport naming conventions for structure tags,
|	so we do some unusual things.
|
\****************************************************************************/

/* 
|| function types are effectively scalar (can't be anonymous); so we have
|| to list them here.
*/

/* the generic call-back function.  For new code, use UCALLBACKFN */
__TMS_FNTYPE_DEF(
	UCALLBACKFN,
	__TMS_VOID, (__TMS_VOID *)
	);

/* but old code uses CALLBACKFN */
__TMS_TYPE_DEF(CALLBACKFN, __TMS_UCALLBACKFN);

__TMS_TYPE_DEF(UPLATFORM, struct TTUSB_PLATFORM);
__TMS_TYPE_DEF(UEVENTCONTEXT, struct TTUSB_EVENTCONTEXT);
__TMS_TYPE_DEF(UPOLLCONTEXT, struct TTUSB_POLLCONTEXT);
__TMS_TYPE_DEF(UCALLBACKCOMPLETION, struct TTUSB_CALLBACKCOMPLETION);
__TMS_TYPE_DEF(CALLBACKCOMPLETION, __TMS_UCALLBACKCOMPLETION);

__TMS_TYPE_DEF_STRUCT(UHIL_INTERRUPT_SYSTEM_INTERFACE);

__TMS_TYPE_DEF_STRUCT(UPLATFORM_DEBUG_PRINT_CONTROL);
__TMS_TYPE_DEF_UNION(USBPUMP_UHILAUX_INCALL);
__TMS_TYPE_DEF_HANDLE(USBPUMP_SESSION_HANDLE);

__TMS_TYPE_DEF_STRUCT(UTASK_ROOT);
__TMS_TYPE_DEF_STRUCT(UTASK_ROOT_CONFIG);

/*
|| The "hilbusio.h" include file lives in the arch/.../port/.../i directory.
|| Normally, it simply includes the appropriate
|| compiler/arch/os-specific header file.  Those live at the
|| arch layer -- e.g., arch/real86/i/msvc16io.h.  In this example,
|| arch/real86/i/msvc16io.h would include the appropriate MSVC++ 16-bit
|| header file (conio.h), and then define the standard UHIL_inb/outb/inw/outw
|| macros.
||
|| Architecturally, we require the use of bus handles.  Unfortunately,
|| we don't have a libport standard type for bus handles.  So we'll define
|| one here, which you can override if necessary.
*/

#ifndef __TTUSB_HIL_BUSHANDLE_DEFINED
typedef __TMS_VOID	*__TTUSB_HIL_BUSHANDLE;
#define __TTUSB_BUSHANDLE_DEFINED
#endif

__TMS_TYPE_DEF(UHIL_BUSHANDLE, __TTUSB_HIL_BUSHANDLE);

/*
|| an interrupt resource handle identifies (in a platform-specific way) the
|| specific interrupt to be used.
*/
#ifndef __TTUSB_HIL_INTERRUPT_RESOURCE_HANDLE_DEFINED
# define __TTUSB_INTERRUPT_RESOURCE_HANDLE_DEFINED
typedef __TMS_VOID	*__TTUSB_HIL_INTERRUPT_RESOURCE_HANDLE;
#endif /* __TTUSB_HIL_INTERRUPT_RESOURCE_HANDLE_DEFINED */

__TMS_TYPE_DEF(UHIL_INTERRUPT_RESOURCE_HANDLE, __TTUSB_HIL_INTERRUPT_RESOURCE_HANDLE);
__TMS_TYPE_DEF(UHIL_ISR_INTERLOCK_STATE, __TMS_UINT_PTR);

__TMS_TYPE_DEF(UHIL_SWCCODE, unsigned int);
__TMS_TYPE_DEF(UHILSTATUS, __TMS_UHIL_SWCCODE);

/****************************************************************************\
|
|	Tags are used for marking data structures.  Since the desired
|	value is almost never easy to generate portably, we use a macro.
|
\****************************************************************************/

#ifndef __TMS_ENDIAN
# error "Need __TMS_ENDIAN to figure out what to do for UHIL_MEMTAG."
#endif

#if __TMS_ENDIAN == __TMS_ENDIAN_LITTLE
# define __TMS_UHIL_MEMTAG(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | (a))
#elif __TMS_ENDIAN == __TMS_ENDIAN_PDP
# define __TMS_UHIL_MEMTAG(a, b, c, d) (((b) << 24) | ((a) << 16) | ((d) << 8) | (c))
#else /* assume it's big-endian */
# define __TMS_UHIL_MEMTAG(a, b, c, d) (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))
#endif

/****************************************************************************\
|
|	Because of ugly backward/forward compatibility problems with
|	usbrctab.h, we have to #define __USBRCTAB_WANT_V3 here.  This won't
|	help if the calling file doesn't 
|
\****************************************************************************/

/* 
|| look for pathological situations -- usually this means usbrctab.h was
|| included before any other DataPump file.
*/
#ifdef __USBRCTAB_WANT_V3
# if !__USBRCTAB_WANT_V3
#   error "This version of the DataPump requires that __USBRCTAB_WANT_V3 == 1"
# endif
#endif

/* define it here to force a compile error on compilers that are careful */
#define	__USBRCTAB_WANT_V3	1	/* see comment above */


/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UHIL_MEMTAG(a, b, c, d) __TMS_UHIL_MEMTAG(a, b, c, d)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of basehiltypes.h ****/
#endif /* _BASEHILTYPES_H_ */
