/* usbpumpdebug.h	Mon Jan 21 2013 08:32:21 tmm */

/*

Module:  usbpumpdebug.h

Function:
	USB DataPump debugging macros.

Version:
	V3.11b	Mon Jan 21 2013 08:32:21 tmm	Edit level 19

Copyright notice:
	This file copyright (C) 2002, 2005-2006, 2008-2009, 2011, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.79a  Mon May 27 2002 23:08:43  tmm
	Module created.

   1.91h  Mon Sep  5 2005 01:42:54   tmm
	Add some host-stack flags.

   1.93e  Tue Jan 24 2006 10:01:33  tmm
	Add USBPUMP_DEBUG_PARAMETER() and USBPUMP_NDEBUG_PARAMETER() for
	parameters that are used only when debugging and not debugging.
	Revise cloaking to use the automatic uncloaking procedures.

   1.93e  Fri Jan 27 2006 03:11:56  tmm
	Fix typo that clobbered __TMS_TTUSB_NDEBUG() in free builds only.

   1.97e  Wed Aug 2 2006 13:23:19  ssyang
	Add UDMASK_HCD debug flag.

   1.97k  Sat Oct 25 2008 18:40:24  maw
	6706: Add UsbPumpDebug_CreateCmdIfc()

   2.01a  Fri Mar 13 2009 15:45:13  chwon
	6706: Changed function name from UsbPumpDebug_CreateCmdIfc() to
	UsbPumpDebug_CreateCommandInterface().
	7031: Update document regarding debug flags.

   3.01f  Fri Sep 02 2011 16:38:12  chwon
	13272: use USBPUMP_DEBUG instead of DEBUG.

   3.11b  Fri Jan 11 2013 06:05:06  tmm
	16452: add support for local trace overrides.

   3.11b  Sat Jan 12 2013 09:39:37  tmm
	16459: control trace macros using __TMS_USBPUMP_TRACE.

   3.11b  Sat Jan 12 2013 10:18:52  tmm
	16462: add UDMASK_FATAL_ERROR.

   3.11b  Mon Jan 14 2013 07:20:06  tmm
	16472: UDMASK_... should be ULONG, not LONG.

   3.11b  Wed Jan 16 2013 06:13:44  tmm
	16463: eliminate TTUSB_LOGF(()).

   3.11b  Thu Jan 17 2013 01:39:36  tmm
	16461: remove UsbDebugLogf().
	16464: remove TTUSB_OBJFLUSH() and UsbPumpDebug_ObjectFlush()
	16466: remove UsbDebugPrintf().
	Cleanup redundant uncloaking.

   3.11b  Thu Jan 17 2013 02:49:36  tmm
	16492: use libport __TMS_STRINGVAL() instead of local definition.

   3.11b  Mon Jan 21 2013 08:32:21  tmm
	16459: we need TTUSB_TRACE() and TTUSB_NTRACE() macros.

*/

#ifndef _USBPUMPDEBUG_H_		/* prevent multiple includes */
#define _USBPUMPDEBUG_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

/*

Macro:	__TMS_USBPUMP_LOCAL_TRACE_INCLUDE__

Function:
	Compile time override of MCCI debug trace macros.

Definition:
	CFLAGS_PORT += -D__TMS_USBPUMP_LOCAL_TRACE_INCLUDE__=my_file.h

Description:
	The DataPump allow the platform build system to inject a header file
	to override the default definitions of the DataPump trace macros. 
	This header file must define (as post condition) all of
	the __TMS_TTUSB_PRINTF(()) macros: __TMS_TTUSB_PRINTF,
	__TMS_TTUSB_OBJPRINTF, __TMS_TTUSB_PLATFORM_PRINTF, and 
	__TMS_TTUSB_FLUSH.

	This header file will be included after #including "usbpumpenv.h"
	(and possibly after including other DataPump header files, depending 
	on the module).  The header file should take into account the
	configured value of __TMS_USBPUMP_TRACE; if 0, the trace macros
	should produce no output, if non-zero, the trace macros should
	produce output.

Notes:
	To avoid quoting issues in the build system the file name is
	given directly (without quotes). "usbpumpdebug.h" supplies
	the leading and trailing quotes. However, this means that you
	can't use an angle-bracketed include file.

*/

#ifdef __TMS_USBPUMP_LOCAL_TRACE_INCLUDE__

# if ! (__TMS_LIBPORT_VERSION > __TMS_LIBPORT_VERSION_CALC(3,11,2,0))
#  error __TMS_USBPUMP_LOCAL_TRACE_INCLUDE__ needs a version of libport that defines __TMS_STRINGVAL.
# endif

/* __TMS_STRINGVAL expands the argument before stringifying */
# include __TMS_STRINGVAL(__TMS_USBPUMP_LOCAL_TRACE_INCLUDE__)
#endif

/* 
|| debugging flags --
||	bits 0..15 are for the usbpump global flags;
||	bits 16..31 are for the device layer (DCDs and HCDs) flags.
||
|| Initialization code always turns on UDMASK_ANY so you can use it as
|| a default for messages to be printed whenever logging is enabled.
||
|| UDMASK_FATAL_ERROR is special, and it's only used in TTUSB_PRINTF()
|| constant masks. Fatal error messages should always be output, so this
|| gives a heads-up to the trace output logic.
*/

#define	__TMS_UDMASK_FATAL_ERROR (0u)		/* pseudo level: fatal error */
#define	__TMS_UDMASK_ERRORS	(1u << 0)	/* trace gross errors */
#define	__TMS_UDMASK_ANY	(1u << 1)	/* catch-all category */
#define	__TMS_UDMASK_FLOW	(1u << 2)	/* flow through the system */
#define	__TMS_UDMASK_CHAP9	(1u << 3)	/* chapter 9 events */
#define	__TMS_UDMASK_PROTO	(1u << 4)	/* protocol events */
#define	__TMS_UDMASK_BUFQE	(1u << 5)	/* bufqe events */
#define	__TMS_UDMASK_HWINT	(1u << 6)	/* trace hw interrupts */
#define __TMS_UDMASK_TXDATA	(1u << 7)	/* trace TX data */
#define	__TMS_UDMASK_RXDATA	(1u << 8)	/* trace RX data */
#define	__TMS_UDMASK_HWDIAG	(1u << 9)	/* trace HW diagnostics */
#define	__TMS_UDMASK_HWEVENT	(1u << 10)	/* device event */
#define __TMS_UDMASK_VSP	(1u << 11)	/* vsp protocol */
#define	__TMS_UDMASK_ENTRY	(1u << 12)	/* procedure entry/exit */
#define	__TMS_UDMASK_ROOTHUB	(1u << 13)	/* root hub flow */
#define	__TMS_UDMASK_USBDI	(1u << 14)	/* USBDI debug */
#define	__TMS_UDMASK_HUB	(1u << 15)	/* hub class flow */

/*			16..32 are for devices (DCDs and HCDs) */
#define	__TMS_UDMASK_DEVBASE_N	16		/* for building masks */

/*			16 is for HCDs. Global flag that any HCD can use */
#define	__TMS_UDMASK_HCD	(1u << 16)	/* hcd flow */

/*
|| if no local trace include file, define the trace macros.
*/
#ifndef __TMS_USBPUMP_LOCAL_TRACE_INCLUDE__
# if __TMS_USBPUMP_TRACE
#  define  __TMS_TTUSB_PRINTF(args)	UsbPumpDebug_DevicePrintf args
#  define  __TMS_TTUSB_OBJPRINTF(args)	UsbPumpDebug_ObjectPrintf args
#  if __TMS_DATAPUMP_COMPAT_V3_10
#   define  __TMS_TTUSB_LOGF(args)	UsbPumpDebug_PlatformLogf args
#  endif
#  define  __TMS_TTUSB_PLATFORM_PRINTF(args)	UsbPumpPlatform_Printf args
#  define  __TMS_TTUSB_FLUSH(pPlatform)	UsbPumpDebug_PlatformFlush(pPlatform)
# else
#  define  __TMS_TTUSB_PRINTF(args)	__TMS_SKIP()
#  define  __TMS_TTUSB_OBJPRINTF(args)	__TMS_SKIP()
#  define  __TMS_TTUSB_PLATFORM_PRINTF(args)	__TMS_SKIP()
#  if __TMS_DATAPUMP_COMPAT_V3_10
#   define  __TMS_TTUSB_LOGF(args)	__TMS_SKIP()
#  endif
#  define  __TMS_TTUSB_FLUSH(pPlatform)	__TMS_SKIP()
# endif
#else /* def __TMS_USBPUMP_LOCAL_TRACE_INCLUDE__ */
# ifndef __TMS_TTUSB_PRINTF
#  error __TMS_USBPUMP_LOCAL_TRACE_INCLUDE__ did not define __TMS_TTUSB_PRINTF
# endif
# ifndef __TMS_TTUSB_OBJPRINTF
#  error __TMS_USBPUMP_LOCAL_TRACE_INCLUDE__ did not define __TMS_TTUSB_OBJPRINTF
# endif
# ifndef __TMS_TTUSB_PLATFORM_PRINTF
#  error __TMS_USBPUMP_LOCAL_TRACE_INCLUDE__ did not define __TMS_TTUSB_PLATFORM_PRINTF
# endif
# ifndef __TMS_TTUSB_FLUSH
#  error __TMS_USBPUMP_LOCAL_TRACE_INCLUDE__ did not define __TMS_TTUSB_FLUSH
# endif

/* check for obsolete APIs */
# if __TMS_DATAPUMP_COMPAT_V3_10 && !defined(__TMS_TTUSB_LOGF)
#  error __TMS_USBPUMP_LOCAL_TRACE_INCLUDE__ did not define __TMS_TTUSB_LOGF
# endif
# if !__TMS_DATAPUMP_COMPAT_V3_10 && defined(__TMS_TTUSB_LOGF)
#  error __TMS_USBPUMP_LOCAL_TRACE_INCLUDE__ defines __TMS_TTUSB_LOGF, which is obsolete.
# endif
#endif /* def __TMS_USBPUMP_LOCAL_TRACE_INCLUDE__ */


/*
|| the local trace include file is not allowed to override the following,
|| which are used for non-tracing debug functions.
*/
#if __TMS_USBPUMP_DEBUG
# define  __TMS_USBPUMP_DEBUG_PARAMETER(x) /* nothing */
# define  __TMS_USBPUMP_NDEBUG_PARAMETER(x) __TMS_USBPUMP_UNREFERENCED_PARAMETER(x)
#else
# define  __TMS_USBPUMP_DEBUG_PARAMETER(x) __TMS_USBPUMP_UNREFERENCED_PARAMETER(x)
# define  __TMS_USBPUMP_NDEBUG_PARAMETER(x) /* nothing */
#endif

#if __TMS_USBPUMP_TRACE
# define  __TMS_USBPUMP_TRACE_PARAMETER(x) /* nothing */
# define  __TMS_USBPUMP_NTRACE_PARAMETER(x) __TMS_USBPUMP_UNREFERENCED_PARAMETER(x)
#else
# define  __TMS_USBPUMP_TRACE_PARAMETER(x) __TMS_USBPUMP_UNREFERENCED_PARAMETER(x)
# define  __TMS_USBPUMP_NTRACE_PARAMETER(x) /* nothing */
#endif

/* +++ begin manual cloaking +++ */
#if __TMS_USBPUMP_DEBUG
# define  __TMS_TTUSB_DEBUG(code)	code
# define  __TMS_TTUSB_NDEBUG(code)	/* nothing */
# if !__TMS_CLOAKED_NAMES_ONLY
#  define  TTUSB_DEBUG(code)	code
#  define  TTUSB_NDEBUG(code)	/* nothing */
# endif /* !__TMS_CLOAKED_NAMES_ONLY */
#else 
# define  __TMS_TTUSB_DEBUG(code)	/* nothing */
# define  __TMS_TTUSB_NDEBUG(code)	code
# if !__TMS_CLOAKED_NAMES_ONLY
#  define  TTUSB_DEBUG(code)	/* nothing */
#  define  TTUSB_NDEBUG(code)	code
# endif
#endif

#if __TMS_USBPUMP_TRACE
# define  __TMS_TTUSB_TRACE(code)	code
# define  __TMS_TTUSB_NTRACE(code)	/* nothing */
# if !__TMS_CLOAKED_NAMES_ONLY
#  define  TTUSB_TRACE(code)	code
#  define  TTUSB_NTRACE(code)	/* nothing */
# endif /* !__TMS_CLOAKED_NAMES_ONLY */
#else 
# define  __TMS_TTUSB_TRACE(code)	/* nothing */
# define  __TMS_TTUSB_NTRACE(code)	code
# if !__TMS_CLOAKED_NAMES_ONLY
#  define  TTUSB_TRACE(code)	/* nothing */
#  define  TTUSB_NTRACE(code)	code
# endif
#endif
/* --- end manual cloaking --- */

/****************************************************************************\
|
|	The functions we need
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/* debug logf is used for logging message unconditionally */
#if __TMS_DATAPUMP_COMPAT_V3_10
__TMS_VOID	UsbDebugLogf(
			__TMS_UPLATFORM *, __TMS_CONST __TMS_TEXT *fmt, ...
			);
#endif /* __TMS_DATAPUMP_COMPAT_V3_10 */

__TMS_VOID	UsbPumpDebug_PlatformLogf(
			__TMS_UPLATFORM *, __TMS_CONST __TMS_TEXT *fmt, ...
			);

/* debug printf is used for logging messages based on device flags */
/* obsolete: */
#if __TMS_DATAPUMP_COMPAT_V3_10
__TMS_VOID	UsbDebugPrintf(
			__TMS_UDEVICE *, __TMS_UINT32 mask, 
			__TMS_CONST __TMS_TEXT *fmt, ...
			);
#endif /* __TMS_DATAPUMP_COMPAT_V3_10 */

__TMS_VOID	UsbPumpDebug_DevicePrintf(
			__TMS_UDEVICE *, __TMS_UINT32 mask, 
			__TMS_CONST __TMS_TEXT *fmt, ...
			);

__TMS_VOID	UsbPumpDebug_DevicePrintf_p(
			__TMS_UDEVICE *, 
			__TMS_UINT32 mask, 
			__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_TEXT) *fmt, 
			...
			);

/* object debug printf is used for issuing the debug IOCTL for printing */
__TMS_VOID	UsbPumpDebug_ObjectPrintf(
			__TMS_USBPUMP_OBJECT_HEADER *,
			__TMS_UINT32 /*mask*/,
			__TMS_CONST __TMS_TEXT *fmt,
			...
			);

__TMS_VOID	UsbPumpDebug_ObjectPrintf_p(
			__TMS_USBPUMP_OBJECT_HEADER *,
			__TMS_UINT32 /*mask*/,
			__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_TEXT) *fmt,
			...
			);

__TMS_VOID	UsbPumpDebug_ObjectFlush(
			__TMS_USBPUMP_OBJECT_HEADER *
			);

/* logging is often write-behind; this tries to wait for log to catch up */
__TMS_VOID	UsbPumpDebug_PlatformFlush(
			__TMS_UPLATFORM *
			);

__TMS_BOOL	UsbPumpDebug_CreateCommandInterface(
			__TMS_UPLATFORM *		/* pPlatform */,
			__TMS_CONST __TMS_TEXT *	/* pObjectNamePattern */
			);

__TMS_VOID
UsbPumpPlatform_Printf(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_UINT32			/* uMessageFlags */,
	__TMS_CONST __TMS_TEXT *	/* pFmt */,
	...
	);

__TMS_VOID
UsbPumpPlatform_Printf_p(
	__TMS_UPLATFORM *				/* pPlatform */,
	__TMS_UINT32					/* uMessageFlags */,
	__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_TEXT) *	/* pFmt */,
	...
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked definitions....
|
\****************************************************************************/

/* this must be manually cloaked */
#if __TMS_DATAPUMP_COMPAT_V3_10
# define TTUSB_LOGF(args)	\
   __TMS_TTUSB_LOGF(args)
#endif

/*
|| you have to edit the output of uncloak-defs.h, because of the conditional
|| definitions above, and some manual uncloaking to avoid double expansion.
*/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
#define	UDMASK_FATAL_ERROR \
  __TMS_UDMASK_FATAL_ERROR
# define UDMASK_ERRORS	\
   __TMS_UDMASK_ERRORS
# define UDMASK_ANY	\
   __TMS_UDMASK_ANY
# define UDMASK_FLOW	\
   __TMS_UDMASK_FLOW
# define UDMASK_CHAP9	\
   __TMS_UDMASK_CHAP9
# define UDMASK_PROTO	\
   __TMS_UDMASK_PROTO
# define UDMASK_BUFQE	\
   __TMS_UDMASK_BUFQE
# define UDMASK_HWINT	\
   __TMS_UDMASK_HWINT
# define UDMASK_TXDATA	\
   __TMS_UDMASK_TXDATA
# define UDMASK_RXDATA	\
   __TMS_UDMASK_RXDATA
# define UDMASK_HWDIAG	\
   __TMS_UDMASK_HWDIAG
# define UDMASK_HWEVENT	\
   __TMS_UDMASK_HWEVENT
# define UDMASK_VSP	\
   __TMS_UDMASK_VSP
# define UDMASK_ENTRY	\
   __TMS_UDMASK_ENTRY
# define UDMASK_ROOTHUB	\
   __TMS_UDMASK_ROOTHUB
# define UDMASK_USBDI	\
   __TMS_UDMASK_USBDI
# define UDMASK_HUB	\
   __TMS_UDMASK_HUB
# define UDMASK_DEVBASE_N	\
   __TMS_UDMASK_DEVBASE_N
# define UDMASK_HCD	\
   __TMS_UDMASK_HCD
# define TTUSB_PRINTF(args)	\
   __TMS_TTUSB_PRINTF(args)
# define TTUSB_OBJPRINTF(args)	\
   __TMS_TTUSB_OBJPRINTF(args)
# define TTUSB_FLUSH(pPlatform)	\
   __TMS_TTUSB_FLUSH(pPlatform)
# define TTUSB_PRINTF(args)	\
   __TMS_TTUSB_PRINTF(args)
# define TTUSB_OBJPRINTF(args)	\
   __TMS_TTUSB_OBJPRINTF(args)
# define TTUSB_PLATFORM_PRINTF(pObj)	\
   __TMS_TTUSB_PLATFORM_PRINTF(pObj)
# define USBPUMP_DEBUG_PARAMETER(x)	\
   __TMS_USBPUMP_DEBUG_PARAMETER(x)
# define USBPUMP_NDEBUG_PARAMETER(x)	\
   __TMS_USBPUMP_NDEBUG_PARAMETER(x)
# define USBPUMP_TRACE_PARAMETER(x)	\
   __TMS_USBPUMP_TRACE_PARAMETER(x)
# define USBPUMP_NTRACE_PARAMETER(x)	\
   __TMS_USBPUMP_NTRACE_PARAMETER(x)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpumpdebug.h ****/
#endif /* _USBPUMPDEBUG_H_ */
