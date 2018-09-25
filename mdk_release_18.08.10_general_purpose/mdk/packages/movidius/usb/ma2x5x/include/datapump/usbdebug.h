/* usbdebug.h	Wed Arpil 3 2002 13:00:00 maw */

/*

Module:  usbdebug.h

Function:
	Internal definitions for the UsbDebug printing functions.

Version:
	V1.63f	Wed April 3 2002 13:00:00 maw	Edit level 1

Copyright notice:
	This file copyright (C) 1998, 2000-2001 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	April 1998

Revision history:
   1.00a  Sun Apr 26 1998 19:48:58  tmm
	Module created.

   1.60f  6/6/2000  tmm
	Add place to get an additional definition in case we dont have
	va_list in our <stdarg.h>

   1.61a  1/2/2001  tmm
	Need the standard debug print include file.
	
  1.63f  Wed April 3 2002 13:00:00 maw
  	Changed __BEGIN/END_DECLS to __TMS_BEGIN/END_DECLS

*/

#ifndef _USBDEBUG_H_		/* prevent multiple includes */
#define _USBDEBUG_H_

#include "usbpump.h"
#include <stdarg.h>
#include "usbvalst.h"
#include "uhildbg.h"

__TMS_FNTYPE_DEF(
	TTUSB_VPRINTF_FN,
	__TMS_BYTES,
		(
		__TMS_VOID *ctx,
		__TMS_CONST __TMS_TEXT *buf,
		__TMS_BYTES nc
		)
	);

__TMS_TYPE_DEF_STRUCT(TTUSB_PRINTF_COLLECTBUF_CONTEXT);
struct __TMS_STRUCTNAME(TTUSB_PRINTF_COLLECTBUF_CONTEXT)
	{
	__TMS_TEXT	*buf;
	__TMS_BYTES	nc;
	};

__TMS_BEGIN_DECLS
int UsbDebugVprintf(
	__TMS_TTUSB_VPRINTF_FN *pfn,
	__TMS_VOID *fn_ctx,
	__TMS_CONST __TMS_TEXT *fmt,
	va_list ap
	);

int UsbDebugVprintf_p(
	__TMS_TTUSB_VPRINTF_FN *pfn,
	__TMS_VOID *fn_ctx,
	__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_TEXT) *fmt,
	va_list ap
	);

int UsbDebugSnprintf(
	__TMS_TEXT *outbuf,
	__TMS_BYTES outbuf_size,
	__TMS_CONST __TMS_TEXT *fmt,
	...
	);

int UsbDebugSnprintf_p(
	__TMS_TEXT *outbuf,
	__TMS_BYTES outbuf_size,
	__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_TEXT) *fmt,
	...
	);

int UsbDebugVsnprintf(
	__TMS_TEXT *outbuf,
	__TMS_BYTES outbuf_size,
	__TMS_CONST __TMS_TEXT *fmt,
	va_list ap
	);

int UsbDebugVsnprintf_p(
	__TMS_TEXT *outbuf,
	__TMS_BYTES outbuf_size,
	__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_TEXT) *fmt,
	va_list ap
	);

/* 
|| UsbDebugPrintf_putbuf is the glue to the HIL print routines.  With the
|| new virtual debug methods, this should never have to change, but we 
|| leave it broken out just in case.
*/
__TMS_TTUSB_VPRINTF_FN UsbDebugPrintf_putbuf;

/*
|| UsbDebugPrintf_collectbuf collects debug output and puts it into
|| a buffer.  It's shared by UsbDebugSnprintf and UsbDebugVsprintf, so we 
|| it out into a separate routine.
*/
__TMS_TTUSB_VPRINTF_FN UsbDebugPrintf_collectbuf;

__TMS_END_DECLS


/**** end of usbdebug.h ****/
#endif /* _USBDEBUG_H_ */
