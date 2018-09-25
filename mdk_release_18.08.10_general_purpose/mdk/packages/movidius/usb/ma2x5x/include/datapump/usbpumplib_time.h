/* usbpumplib_time.h	Sun Feb 11 2007 20:30:30 tmm */

/*

Module:  usbpumplib_time.h

Function:
	Standard time manipulation routines and structures.

Version:
	V1.97g	Sun Feb 11 2007 20:30:30 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2007 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	February 2007

Revision history:
   1.97g  Sun Feb 11 2007 20:30:30  tmm
	Module created.

*/

#ifndef _USBPUMPLIB_TIME_H_		/* prevent multiple includes */
#define _USBPUMPLIB_TIME_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

/****************************************************************************\
|
|	Structures
|
\****************************************************************************/

/* the ISO8601 time, in computable form */
__TMS_TYPE_DEF_STRUCT	(USBPUMP_ISO8601_TIME);
struct __TMS_STRUCTNAME	(USBPUMP_ISO8601_TIME)
	{
	__TMS_UINT32	Year;			/* 1601..9999 */
	__TMS_UINT32	Month;			/* 1..12 */
	__TMS_UINT32	Day;			/* 1..28, 1..29, 1..30, 1..31 
						|| depending on Year and Month
						*/
	__TMS_UINT32	Hour;			/* 0..23 */
	__TMS_UINT32	Minute;			/* 0..59 */
	__TMS_UINT32	Second;			/* 0..59 or 0..60 for leap-sec */
	__TMS_UINT32	Nanosecond;		/* 0..999,999,999 */
	__TMS_UINT8	nFractionSpecified;	/* 0..9 -- number of fraction
						|| digits in text form.
						*/
	__TMS_UINT8	fTzSpecified;		/* TRUE if a TZ was given,
						|| FALSE otherwise
						*/

	/* if TzHour and TzMinute both non-zero, signs will be the same */
	__TMS_INT8	TzHour;			/* -23..23 */
	__TMS_INT8	TzMinute;		/* -59..59 */
	};


/****************************************************************************\
|
|	Function prototypes.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL
UsbPumpLib_ValidateIso8601Time(
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *	/*pTime*/,
	__TMS_BYTES 				/*nTime*/,
	__TMS_USBPUMP_ISO8601_TIME *		/*pTimeOut OPTIONAL*/,
	__TMS_BOOL				/*fLeapSecOk*/
	);

__TMS_END_DECLS


/**** end of usbpumplib_time.h ****/
#endif /* _USBPUMPLIB_TIME_H_ */
