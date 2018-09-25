/* usbpump_hcdkit_mapbuf.h	Wed Sep 09 2009 15:10:00 chwon */

/*

Module:  usbpump_hcdkit_mapbuf.h

Function:
	Definition of mapping buffer function

Version:
	V2.01a	Wed Sep 09 2009 15:10:00 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	June 2009

Revision history:
   2.01a  Wed Jun 03 2009 10:39:06 chwon
	8184: Module created.

   2.01a  Thu Jun 04 2009 11:55:51  chwon
	8184: Added USBPUMP_HCDKIT_HCD input parameter in the
	USBPUMP_HCDKIT_COPY_PIO_BUFFER_FN.

   2.01a  Wed Jun 24 2009 16:49:06  chwon
	8184: Added UsbPumpHcdKitI_MapPioIsochDescr() and
	UsbPumpHcdKitI_UnMapPioIsochDescr() prototype.

   2.01a  Wed Sep 09 2009 15:10:00  chwon
	8184: Change input parameter type from CONST VOID * to VOID * in the
	USBPUMP_HCDKIT_COPY_PIO_BUFFER_FN.

*/

#ifndef _USBPUMP_HCDKIT_MAPBUF_H_		/* prevent multiple includes */
#define _USBPUMP_HCDKIT_MAPBUF_H_

#ifndef _USBPUMP_HCDKIT_TYPES_H_
# include "usbpump_hcdkit_types.h"
#endif

#ifndef _USBPUMP_HCDKIT_HCD_H_
# include "usbpump_hcdkit_hcd.h"
#endif

#ifndef _USBPUMP_HCD_REQUEST_H_
# include "usbpump_hcd_request.h"
#endif

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_COPY_PIO_BUFFER_FN,
__TMS_BYTES,
	(
	__TMS_USBPUMP_HCDKIT_HCD *,
	__TMS_VOID *,
	__TMS_VOID *,
	__TMS_ARG_BYTES
	));


__TMS_BEGIN_DECLS

__TMS_USTAT
UsbPumpHcdKitI_MapCopyFromPioBuffer(
	__TMS_USBPUMP_HCDKIT_HCD *	pHcd,
	__TMS_USBPUMP_HCD_REQUEST *	pRequest,
	__TMS_BYTES			StartOffsetInBytes,
	__TMS_VOID *			pBuffer,
	__TMS_BYTES			nBuffer,
	__TMS_USBPUMP_HCDKIT_COPY_PIO_BUFFER_FN *pCopyBufFn
	);

__TMS_USTAT
UsbPumpHcdKitI_MapCopyToPioBuffer(
	__TMS_USBPUMP_HCDKIT_HCD *	pHcd,
	__TMS_USBPUMP_HCD_REQUEST *	pRequest,
	__TMS_BYTES			StartOffsetInBytes,
	__TMS_VOID *			pBuffer,
	__TMS_BYTES			nBuffer,
	__TMS_USBPUMP_HCDKIT_COPY_PIO_BUFFER_FN *pCopyBufFn
	);

__TMS_USTAT
UsbPumpHcdKitI_MapPioBuffer(
	__TMS_USBPUMP_HCDKIT_HCD *	pHcd,
	__TMS_USBPUMP_HCD_REQUEST *	pRequest,
	__TMS_BYTES			StartOffsetInBytes,
	__TMS_BYTES			SizeInBytes,
	__TMS_VOID **			pRangeLogicalAddress,
	__TMS_BYTES *			pRangeLengthInBytes
	);

__TMS_USTAT
UsbPumpHcdKitI_UnMapPioBuffer(
	__TMS_USBPUMP_HCDKIT_HCD *	pHcd,
	__TMS_USBPUMP_HCD_REQUEST *	pRequest
	);

__TMS_USTAT
UsbPumpHcdKitI_MapPioIsochDescr(
	__TMS_USBPUMP_HCDKIT_HCD *	pHcd,
	__TMS_USBPUMP_HCD_REQUEST *	pRequest,
	__TMS_BYTES			StartOffsetInBytes,
	__TMS_BYTES			SizeInBytes,
	__TMS_USBPUMP_ISOCH_PACKET_DESCR **pIsochLogicalAddress,
	__TMS_BYTES *			pRangeLengthInBytes
	);

__TMS_USTAT
UsbPumpHcdKitI_UnMapPioIsochDescr(
	__TMS_USBPUMP_HCDKIT_HCD *	pHcd,
	__TMS_USBPUMP_HCD_REQUEST *	pRequest
	);

__TMS_END_DECLS

/**** end of usbpump_hcdkit_mapbuf.h ****/
#endif /* _USBPUMP_HCDKIT_MAPBUF_H_ */
