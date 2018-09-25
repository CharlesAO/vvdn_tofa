/* udevice_mappiobuf.h	Wed Sep 09 2009 15:12:48 chwon */

/*

Module:  udevice_mappiobuf.h

Function:
	Definition of map PIO buffer function

Version:
	V2.01a	Wed Sep 09 2009 15:12:48 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	August 2009

Revision history:
   2.01a  Thu Aug 20 2009 11:44:13 chwon
	8748: Module created.

   2.01a  Tue Sep 08 2009 14:13:38  chwon
	8851: Added UsbPumpDevice_HbufferEnterScope(),
	UsbPumpDevice_HbufferExitScope() and UsbPumpDevice_MapPioBufferCopy()

   2.01a  Wed Sep 09 2009 15:12:48  chwon
	8851: Change input parameter type from CONST VOID * to VOID * in the
	UDEVICE_COPY_PIO_BUFFER_FN.

*/

#ifndef _UDEVICE_MAPPIOBUF_H_		/* prevent multiple includes */
#define _UDEVICE_MAPPIOBUF_H_

#ifndef _UDEVICE_H_
# include "udevice.h"
#endif

#ifndef _UBUFQE_H_
# include "ubufqe.h"
#endif

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
UDEVICE_COPY_PIO_BUFFER_FN,
__TMS_BYTES,
	(
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_VOID *			/* pDestBuffer */,
	__TMS_VOID *			/* pSrcBuffer */,
	__TMS_ARG_BYTES			/* nSrcBuffer */
	));


__TMS_BEGIN_DECLS

__TMS_BOOL
UsbPumpDevice_HbufferEnterScope(
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_VOID *			/* pBuffer */,
	__TMS_BYTES			/* nBuffer */,
	__TMS_UINT32			/* AccessType */,
	__TMS_USBPUMP_BUFFER_HANDLE *	/* phBuffer */
	);

__TMS_VOID
UsbPumpDevice_HbufferExitScope(
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_USBPUMP_BUFFER_HANDLE *	/* phBuffer */
	);

__TMS_USTAT
UsbPumpDevice_MapCopyFromPioBuffer(
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_UBUFQE *			/* pQe */,
	__TMS_BYTES			/* StartOffsetInBytes */,
	__TMS_VOID *			/* pBuffer */,
	__TMS_BYTES			/* nBuffer */,
	__TMS_UDEVICE_COPY_PIO_BUFFER_FN * /* pCopyBufFn */
	);

__TMS_USTAT
UsbPumpDevice_MapCopyToPioBuffer(
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_UBUFQE *			/* pQe */,
	__TMS_BYTES			/* StartOffsetInBytes */,
	__TMS_VOID *			/* pBuffer */,
	__TMS_BYTES			/* nBuffer */,
	__TMS_UDEVICE_COPY_PIO_BUFFER_FN * /* pCopyBufFn */
	);

__TMS_USTAT
UsbPumpDevice_MapPioBuffer(
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_UBUFQE *			/* pQe */,
	__TMS_BYTES			/* StartOffsetInBytes */,
	__TMS_BYTES			/* SizeInBytes */,
	__TMS_VOID **			/* pRangeLogicalAddress */,
	__TMS_BYTES *			/* pRangeLengthInBytes */
	);

__TMS_USTAT
UsbPumpDevice_UnMapPioBuffer(
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_UBUFQE *			/* pQe */
	);

__TMS_USTAT
UsbPumpDevice_MapPioIsochDescr(
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_UBUFQE *			/* pQe */,
	__TMS_BYTES			/* StartOffsetInBytes */,
	__TMS_BYTES			/* SizeInBytes */,
	__TMS_USBPUMP_ISOCH_PACKET_DESCR ** /* pIsochLogicalAddress */,
	__TMS_BYTES *			/* pRangeLengthInBytes */
	);

__TMS_USTAT
UsbPumpDevice_UnMapPioIsochDescr(
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_UBUFQE *			/* pQe */
	);

__TMS_BOOL
UsbPumpDevice_MapPioBufferCopy(
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* hBuffer */,
	__TMS_VOID *			/* pHandleBuffer */,
	__TMS_BYTES			/* StartOffsetInBytes */,
	__TMS_BYTES			/* CopyInBytes */,
	__TMS_VOID *			/* pBuffer */,
	__TMS_BOOL			/* fCopyFromHandleToBuffer */
	);

__TMS_END_DECLS

/**** end of udevice_mappiobuf.h ****/
#endif /* _UDEVICE_MAPPIOBUF_H_ */
