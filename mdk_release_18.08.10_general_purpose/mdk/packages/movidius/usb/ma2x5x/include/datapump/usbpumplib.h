/* usbpumplib.h	Thu Jan 15 2015 00:04:57 tmm */

/*

Module:  usbpumplib.h

Function:
	Header file for the various USB DataPump library functions.

Version:
	V3.13b	Thu Jan 15 2015 00:04:57 tmm	Edit level 30

Copyright notice:
	This file copyright (C) 2002, 2005-2009, 2011-2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.63i  Thu May 16 2002 18:18:39  tmm
	Module created.

   1.79b  Tue Sep 24 2002 10:39:00 maw
   	Added macros for big endian handling

   1.91d  Thu Jun 16 2005 10:27:16  cccho
	Added UHIL_cpynstr() function prototype

   1.91e  2005-06-26  tmm
	Add UHIL_btol and UHIL_btoul.
	Added UHIL_le_getuint64(), UHIL_le_putuint64(), UHIL_be_getuint64(),
	UHIL_be_putuint64(), UHIL_LE_GETUINT64(), UHIL_LE_PUTUINT64(),
	UHIL_BE_GETUINT64(), UHIL_BE_PUTUINT64()

   1.91h  2005-08-29  tmm
	Add UsbPumpLib_BufferFieldIndex, UsbPumpLib_BufferFieldLength,
	UsbPumpLib_ScanBuffer().

   1.91h  Mon Sep  5 2005 03:51:07  tmm
	Add UsbPumpLib_HubFeatureName(), and normalize layout of some of the
	other function prototypes.

   1.91i  Sat Sep 17 2005 16:52:34  tmm
	Add wire size macros for the integer types, and cloak the
	UHIL_LE_PUT.., etc, macros.

   1.93a  Thu Oct 06 2005 00:40:23  chwon
	Add UHIL_cmpstr.

   1.93c  Thu Dec  1 2005 23:54:40  tmm
	Add UsbPumpLib_BufferCompareString()

   1.93e  Sun Jan 29 2006 12:38:03  tmm
	Add UsbPumpLib_ReverseSmall(), UsbPumpLib_IntervalKind() and
	USBPUMP_INTERVAL_KIND_... symbols.

   1.93e  Wed Feb 08 2006 12:55:56  chwon
	Add UsbPumpLib_ExpandBuffer(), UsbPumpLib_MemoryCompare() and
	UsbPumpLib_ScanString()

   1.93e  Thu Feb 23 2006 22:18:44  tmm
	Add __TMS_LIBPORT_PACKED1() on the get/put routines to make RealView
	happier.

   1.97b  Thu Apr 13 2006 07:15:46  tmm
	Add 128-bit definitions and recloak.

   1.96.1  Wed Apr 26 2006 11:44:34 maw
   	Added UsbPumpLib_ProcessEscSeq & UsbPumpLib_ParseCommandLine
   	UsbPumpLib_TokenizeCommandLine

   1.97d  Tue Jun 13 2006 13:48:11  maw
    	Changed return value from BYTES to INT on UsbPumpLib_ProcessEscSeq

   1.97h  Wed Apr 11 2007 20:52:37  chwon
	Add UsbPumpLib_BufferCompareStringCaseInsensitive() and
	UsbPumpLib_ToLower()

   1.97k  Fri Oct 31 2008 10:59:01  cccho
	6683: Add UsbPumpLib_HirdToMicroSecond().

   2.01a  Tue May 26 2009 11:36:42  tmm
	8039: Add new kinds of intervals for wireless USB.

   2.01a  Wed Nov 25 2009 11:59:08  chwon
	9425: include __TMS_UHIL_GETPUT_OVERRIDE_FILE file if it is defined.

   3.01d  Fri Jan 14 2011 12:44:37  chwon
	12396: Add UHIL_udiv64() and UHIL_urem64().

   3.01f  Thu May 31 2012 10:40:52  chwon
	14821: Added UsbPumpLib_BeslToMicroSecond() prototype.

   3.01f  Wed Jun 13 2012 16:01:23  chwon
	15535: Added fHubFeatureCode parameter in the 
	UsbPumpLib_HubFeatureName().

   3.11a  Sat Sep 08 2012 21:10:31  chwon
	15846: include uhil_getput_macros.h if 
	__TMS_INCLUDE_UHIL_GETPUT_MACROS is defined.

   3.11c  Thu Apr 18 2013 14:27:56  chwon
	16706: include local UHIL file if __TMS_USBPUMP_LOCAL_UHIL_INCLUDE__
	is defined.

   3.13b  Thu Nov  6 2014 14:10:01  tmm
	18362: add UHIL_cpybuf_p(). Make it (by default) a synonym for
	UHIL_cpybuf.

   3.13b  Thu Jan 15 2015 00:04:57  tmm
	18845: add UsbPumpLib_HubTestSelectorName()

*/

#ifndef _USBPUMPLIB_H_		/* prevent multiple includes */
#define _USBPUMPLIB_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _UHILFNS_H_
# include "uhilfns.h"
#endif

/****************************************************************************\
|
|	Function prototypes -- for historical reasons, the real names of
|	these functions are UHIL_..., but they should be UsbPumpLib_...
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL
UsbPumpLib_BufferCompareString(
	__TMS_CONST __TMS_TEXT *,
	__TMS_BYTES,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BOOL
UsbPumpLib_BufferCompareStringCaseInsensitive(
	__TMS_CONST __TMS_TEXT *,
	__TMS_BYTES,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UsbPumpLib_BufferFieldIndex(
	__TMS_CONST __TMS_TEXT *,
	__TMS_BYTES,
	__TMS_BYTES,
	__TMS_BYTES,
	__TMS_INT
	);

__TMS_BYTES
UsbPumpLib_BufferFieldLength(
	__TMS_CONST __TMS_TEXT *,
	__TMS_BYTES,
	__TMS_BYTES,
	__TMS_INT
	);

__TMS_BYTES
UsbPumpLib_BufferToLong(
	__TMS_CONST __TMS_TEXT *,
	__TMS_BYTES,
	__TMS_UINT,
	__TMS_LONG *,
	__TMS_BOOL *
	);

__TMS_BYTES
UsbPumpLib_BufferToUlong(
	__TMS_CONST __TMS_TEXT *,
	__TMS_BYTES,
	__TMS_UINT,
	__TMS_ULONG *,
	__TMS_BOOL *
	);

__TMS_INT32
UsbPumpLib_CalculateMaxPacketSize(
	__TMS_USBPUMP_OBJECT_HEADER *	/*pObjHdr*/,
	__TMS_USBPUMP_DEVICE_SPEED	/*devSpeed*/,
	__TMS_UINT8			/*bmAttributes*/,
	__TMS_ARG_UINT16		/*wMaxPacketSize*/,
	__TMS_BOOL			/*fStrict*/
	);

__TMS_INT32
UsbPumpLib_CalculateNormalizedInterval(
	__TMS_USBPUMP_OBJECT_HEADER *	/*pObjHdr*/,
	__TMS_USBPUMP_DEVICE_SPEED	/*devSpeed*/,
	__TMS_UINT8			/*bInterval*/,
	__TMS_UINT8			/*bmAttributes*/,
	__TMS_BOOL			/*fDirectionIn*/,
	__TMS_BOOL			/*fStrict*/
	);

__TMS_COUNT
UsbPumpLib_ExpandBuffer(
	__TMS_TEXT *	/*pBuf*/,
	__TMS_BYTES	/*len*/,
	__TMS_COUNT	/*index*/,
	__TMS_COUNT	/*inlen*/
	);

__TMS_UINT
UsbPumpLib_IntervalKind(
	__TMS_USBPUMP_DEVICE_SPEED	/*devSpeed*/,
	__TMS_UINT8			/*bmAttributes*/,
	__TMS_BOOL			/*fDirectionIn*/
	);

/* the return values */
#define __TMS_USBPUMP_INTERVAL_KIND_ZERO	0	/* bInterval is to be zero */
#define __TMS_USBPUMP_INTERVAL_KIND_LINEAR0	1	/* bInterval is linear, from 0..255 */
#define __TMS_USBPUMP_INTERVAL_KIND_LINEAR1	2	/* bInterval is to be linear from 1..255 */
#define __TMS_USBPUMP_INTERVAL_KIND_LOG1	3	/* bInterval is a log, [1..16], return 2^(x-1) */
#define __TMS_USBPUMP_INTERVAL_KIND_LOG2	4	/* bInterval is a log, [6..16], return 2^(x-1) */
#define __TMS_USBPUMP_INTERVAL_KIND_LOG3	5	/* bInterval is a log, [0..16], return 2^(x-1) (zero is a wild card) */


__TMS_CONST __TMS_TEXT *
UsbPumpLib_HubFeatureName(
	__TMS_ARG_UINT16	/* FeatureCode */,
	__TMS_BOOL		/* fHubFeatureCode */
	);

__TMS_CONST __TMS_TEXT *
UsbPumpLib_HubTestSelectorName(
	__TMS_ARG_UINT8		/* TestSelector */
	);

__TMS_BOOL
UsbPumpLib_MatchPattern(
	__TMS_CONST __TMS_TEXT *	/*pPattern*/ ,
	__TMS_BYTES			/*nPattern*/ ,
	__TMS_CONST __TMS_TEXT *	/*pValue*/ ,
	__TMS_BYTES			/*nValue*/
	);

__TMS_INT
UsbPumpLib_MemoryCompare(
	__TMS_CONST __TMS_VOID *	/*pMemory1*/,
	__TMS_CONST __TMS_VOID *	/*pMemory2*/,
	__TMS_BYTES			/*SizeOfMemory*/
	);

__TMS_CONST __TMS_TYPE_CODESPACE(__TMS_TEXT) *
UsbPumpLib_MultiSzIndex_p(
	__TMS_CONST __TMS_TYPE_CODESPACE(__TMS_TEXT) *,
	__TMS_UINT
	);

__TMS_INT
UsbPumpLib_ParseCommandLine(
	__TMS_TEXT *			/* ref to commandline */
	);

__TMS_INT
UsbPumpLib_ProcessEscSeq(
	__TMS_TEXT **			/* ref to pointer to string */,
	__TMS_TEXT *			/* list of ext. to normal conventions */
	);

__TMS_UINT
UsbPumpLib_ReverseSmall(
	__TMS_UINT /* number of bits */,
	__TMS_UINT /* value to revers */
	);

__TMS_BYTES
UsbPumpLib_SafeCopyBuffer(
	__TMS_PVOID,			/* base of output buffer */
	__TMS_BYTES,			/* size of output buffer */
	__TMS_BYTES,			/* where to start copying w/in buffer */
	__TMS_CONST __TMS_VOID *,	/* input buffer */
	__TMS_BYTES			/* size of data to copy */
	);

__TMS_BYTES
UsbPumpLib_SafeCopyString(
	__TMS_TEXT *			/*pBuffer*/,
	__TMS_BYTES			/*nBuffer*/,
	__TMS_BYTES			/*iBuffer*/,
	__TMS_CONST __TMS_TEXT *	/*pString*/
	);

__TMS_BYTES
UsbPumpLib_ScanBuffer(
	__TMS_CONST __TMS_TEXT *	/*pBuffer*/,
	__TMS_BYTES			/*nBuffer*/,
	__TMS_TEXT			/*charToMatch*/
	);

__TMS_BYTES
UsbPumpLib_ScanString(
	__TMS_CONST __TMS_TEXT *	/*pString*/,
	__TMS_TEXT			/*charToMatch*/
	);

__TMS_BYTES
UsbPumpLib_TokenizeCommandLine(
	__TMS_TEXT *			/* pCmdBuf */,
	__TMS_INT			/* nArgs */,
	__TMS_TEXT **			/* av */
	);

__TMS_TEXT
UsbPumpLib_ToLower(
	__TMS_TEXT	/* InputChar */
	);

__TMS_BYTES
UsbPumpLib_UlongToBuffer(
	__TMS_TEXT *	/*pBuf*/,
	__TMS_BYTES	/*nBuf*/,
	__TMS_ULONG	/*ulnum*/,
	__TMS_INT	/*radix*/
	);

__TMS_BYTES
UsbPumpLib_UlongToBufferHex(
	__TMS_TEXT *	/*pBuf*/,
	__TMS_BYTES	/*nBuf*/,
	__TMS_ULONG	/*ulnum*/,
	__TMS_BYTES	/*nDigits*/
	);

__TMS_UINT
UsbPumpLib_BeslToMicroSecond(
	__TMS_UINT8	/* ucBesl -- encoded BESL */
	);

__TMS_UINT
UsbPumpLib_HirdToMicroSecond(
	__TMS_UINT8	/* encoded hird */
	);

__TMS_END_DECLS

/****************************************************************************\
|
|	uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_INTERVAL_KIND_ZERO	\
   __TMS_USBPUMP_INTERVAL_KIND_ZERO
# define USBPUMP_INTERVAL_KIND_LINEAR0	\
   __TMS_USBPUMP_INTERVAL_KIND_LINEAR0
# define USBPUMP_INTERVAL_KIND_LINEAR1	\
   __TMS_USBPUMP_INTERVAL_KIND_LINEAR1
# define USBPUMP_INTERVAL_KIND_LOG1	\
   __TMS_USBPUMP_INTERVAL_KIND_LOG1
# define USBPUMP_INTERVAL_KIND_LOG2	\
   __TMS_USBPUMP_INTERVAL_KIND_LOG2
# define USBPUMP_INTERVAL_KIND_LOG3	\
   __TMS_USBPUMP_INTERVAL_KIND_LOG3
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpumplib.h ****/
#endif /* _USBPUMPLIB_H_ */
