/* usbpump_mdk_uplat_uhilaux.h	Wed Aug 23 2017 16:13:07 chwon */

/*

Module:  usbpump_mdk_uplat_uhilaux.h

Function:
	Definition of UHILAUX internals

Version:
	V3.21c	Wed Aug 23 2017 16:13:07 chwon	Edit level 10

Copyright notice:
	This file copyright (C) 2014-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	October 2014

Revision history:
   3.13b  Thu Oct 09 2014 15:56:28  chwon
	18574: Module created.

   3.13b  Wed Dec 24 2014 11:18:12  chwon
	18574: Create from os/none/i/usbpump_mdk_uplat_uhilaux.h

   3.13b  Wed Jan 07 2015 14:52:45  chwon
	18534: Add abstract pool input parameter.

   3.15b  Tue Mar 10 2015 10:58:34  chwon
	18966: 18967: Add UsbPumpMdk_Uplatform_UhilAuxCreate_V2() prototype.

   3.17a  Wed Feb 03 2016 09:04:28  chwon
	19642: Add UsbPumpMdkUhilAux_HbufferCreate() and
	UsbPumpMdkUhilAux_HbufferDelete() prototype.

   3.17a  Wed Feb 17 2016 12:44:55  chwon
	19642: Remove UsbPumpMdkUhilAux_HbufferCreate() and
	UsbPumpMdkUhilAux_HbufferDelete() prototype.

   3.17a  Wed Mar 09 2016 15:36:14  chwon
	19642: Re-add UsbPumpMdkUhilAux_HbufferCreate() and
	UsbPumpMdkUhilAux_HbufferDelete() prototype for backward compatibility.

   3.19b  Wed Mar 08 2017 17:06:07  chwon
	20480: Add UsbPumpMdkUhilAux_SegmentHbufferCreate() and
	UsbPumpMdkUhilAux_SegmentHbufferAddBuffer() prototype.

   3.21a  Wed Jun 14 2017 15:52:28  chwon
	20591: Add UsbPumpMdk_Uplatform_UhilAuxCreate_V3() prototype.

   3.21c  Wed Aug 23 2017 16:13:07  chwon
	20644: Add UsbPumpMdkUhilAux_SegmentHbufferReset() prototype.

*/

#ifndef _USBPUMP_MDK_UPLAT_UHILAUX_H_	/* prevent multiple includes */
#define _USBPUMP_MDK_UPLAT_UHILAUX_H_

#ifndef _USBPUMP_UHILAUX_H_
# include "usbpump_uhilaux.h"
#endif

#ifndef _USBPUMP_MEMPOOL_H_
# include "usbpump_mempool.h"
#endif

/****************************************************************************\
|
|	Cache management function prototypes
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
	USBPUMP_MDK_CACHE_FLUSH_FN,
	__TMS_VOID,
	(
	__TMS_VOID *	/* pBuffer */,
	__TMS_BYTES	/* nBuffer */
	));

__TMS_FNTYPE_DEF(
	USBPUMP_MDK_CACHE_INVALIDATE_FN,
	__TMS_VOID,
	(
	__TMS_VOID *	/* pBuffer */,
	__TMS_BYTES	/* nBuffer */
	));


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_UHILAUX *
UsbPumpMdk_Uplatform_UhilAuxCreate(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_USBPUMP_ABSTRACT_POOL *	/* pAbstractPool */
	);

__TMS_USBPUMP_UHILAUX *
UsbPumpMdk_Uplatform_UhilAuxCreate_V2(
	__TMS_UPLATFORM *			/* pPlatform */,
	__TMS_USBPUMP_ABSTRACT_POOL *		/* pAbstractPool */,
	__TMS_BOOL				/* fCacheEnabled */,
	__TMS_USBPUMP_MDK_CACHE_FLUSH_FN *	/* pCacheFlushFn */,
	__TMS_USBPUMP_MDK_CACHE_INVALIDATE_FN *	/* pCacheInvalidateFn */
	);

__TMS_USBPUMP_UHILAUX *
UsbPumpMdk_Uplatform_UhilAuxCreate_V3(
	__TMS_UPLATFORM *			/* pPlatform */,
	__TMS_USBPUMP_ABSTRACT_POOL *		/* pAbstractPool */,
	__TMS_BOOL				/* fCacheEnabled */,
	__TMS_USBPUMP_MDK_CACHE_FLUSH_FN *	/* pCacheFlushFn */,
	__TMS_USBPUMP_MDK_CACHE_INVALIDATE_FN *	/* pCacheInvalidateFn */,
	__TMS_UINT				/* CacheDataLineSize */
	);

/* Below is for backward compatibility. Please do not use these function */
__TMS_USBPUMP_UHILAUX_STATUS
UsbPumpMdkUhilAux_HbufferCreate(
	__TMS_USBPUMP_SESSION_HANDLE			/* hSession */,
	__TMS_USBPUMP_BUFFER_HANDLE *			/* phBuffer */,
	__TMS_UINT					/* numSegments */,
	__TMS_ADDRBITS					/* vaBuffer */,
	__TMS_BYTES					/* nSegBuffer */,
	__TMS_ADDRBITS					/* vaBuffer2 */,
	__TMS_BYTES					/* nSegBuffer2 */,
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE	/* requestedAccess */
	);

__TMS_USBPUMP_UHILAUX_STATUS
UsbPumpMdkUhilAux_HbufferDelete(
	__TMS_USBPUMP_SESSION_HANDLE			/* hSession */,
	__TMS_USBPUMP_BUFFER_HANDLE *			/* phBuffer */
	);

__TMS_USBPUMP_UHILAUX_STATUS
UsbPumpMdkUhilAux_SegmentHbufferCreate(
	__TMS_USBPUMP_SESSION_HANDLE			/* hSession */,
	__TMS_USBPUMP_BUFFER_HANDLE *			/* phBuffer */,
	__TMS_UINT					/* numSegments */,
	__TMS_ADDRBITS					/* vaBuffer */,
	__TMS_BYTES					/* nBuffer */,
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE 	/* requestedAccess */
	);

__TMS_USBPUMP_UHILAUX_STATUS
UsbPumpMdkUhilAux_SegmentHbufferAddBuffer(
	__TMS_USBPUMP_SESSION_HANDLE	/* hSession */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* hBuffer */,
	__TMS_ADDRBITS			/* vaBuffer */,
	__TMS_BYTES			/* nBuffer */
	);

__TMS_USBPUMP_UHILAUX_STATUS
UsbPumpMdkUhilAux_SegmentHbufferReset(
	__TMS_USBPUMP_SESSION_HANDLE	/* hSession */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* hBuffer */
	);

__TMS_END_DECLS

/**** end of usbpump_mdk_uplat_uhilaux.h ****/
#endif /* _USBPUMP_MDK_UPLAT_UHILAUX_H_ */
