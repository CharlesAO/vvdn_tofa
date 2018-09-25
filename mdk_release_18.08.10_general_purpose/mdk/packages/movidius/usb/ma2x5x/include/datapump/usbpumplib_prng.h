/* usbpumplib_prng.h	Fri Oct 24 2014 11:21:30 chwon */

/*

Module:  usbpumplib_prng.h

Function:
	Common definitions for Pseudo random number generator.

Version:
	V3.13b	Fri Oct 24 2014 11:21:30 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2012, 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Subbarayalu, MCCI Corporation	February 2012

Revision history:
   3.01f  Fri Feb 03 2012 12:10:12  subbarayalu
	14753: Module created.

   3.13b  Fri Oct 24 2014 11:21:30  chwon
	18600: Fix doc-xml error.

*/

#ifndef _USBPUMPLIB_PRNG_H_		/* prevent multiple includes */
#define _USBPUMPLIB_PRNG_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

/****************************************************************************\
|
|	Context structure for an instance of the pseudo-random number generator
|       (PRNG)
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_PRNG_CONTEXT);
struct __TMS_STRUCTNAME(USBPUMP_PRNG_CONTEXT)
	{
	__TMS_UINT32 x;
	__TMS_UINT32 y;
	__TMS_UINT32 z;
	__TMS_UINT32 c;
	};

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpLib_PRNG_Initialize(
	__TMS_USBPUMP_PRNG_CONTEXT *,	/* pPrngCtx*/
	__TMS_UINT32,			/* init_x */
	__TMS_UINT32, 			/* init_y */
	__TMS_UINT32,			/* init_z */
        __TMS_UINT32			/* init_c */
	);

__TMS_UINT32
UsbPumpLib_PRNG_NextValue(
	__TMS_USBPUMP_PRNG_CONTEXT *	/* pPrngCtx */
	);

__TMS_END_DECLS

/**** end of usbpumplib_prng.h ****/
#endif /* _USBPUMPLIB_PRNG_H_ */
