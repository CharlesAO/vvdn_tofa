/* usbpump_usbd_class_uvc_request.h	Thu Mar 16 2017 16:55:51 chwon */

/*

Module:  usbpump_usbd_class_uvc_request.h

Function:
	Definition of the USBD Uvc driver configuration

Version:
	V3.19b	Thu Mar 16 2017 16:55:51 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2017

Revision history:
   3.19b  Thu Mar 16 2017 16:55:51  chwon
	20517: Module created.

*/

#ifndef _USBPUMP_USBD_CLASS_UVC_REQUEST_H_	/* prevent multiple includes */
#define _USBPUMP_USBD_CLASS_UVC_REQUEST_H_

#ifndef _USBPUMP_USBD_CLASS_UVC_TYPES_H_
# include "usbpump_usbd_class_uvc_types.h"
#endif

#ifndef _USBPUMP_USBDI_FUNCTION_H_
# include "usbpump_usbdi_function.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

#ifndef _USBIF_H_
# include "usbif.h"
#endif

#ifndef _USBVIDEO15_H_
# include "usbvideo15.h"
#endif

/****************************************************************************\
|
|	The USBD UVC Class Driver Request
|
\****************************************************************************/

/*

Type:	USBPUMP_USBDI_CLASS_UVC_REQUEST

Index:	Type:	USBPUMP_USBDI_CLASS_UVC_REQUEST_FLAG
	Type:	ARG_USBPUMP_USBDI_CLASS_UVC_REQUEST_FLAG
	Type:	USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT
	Type:	ARG_USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT

Function:
	Encapsulates a request to a UVC class driver.

Description:
	The portable UVC class API uses special request packets to convey
	requests to the UVC class driver.

Definition:
	typedef struct __TMS_STRUCTNAME(USBPUMP_USBDI_CLASS_UVC_REQUEST)
		USBPUMP_USBDI_CLASS_UVC_REQUEST,
		*PUSBPUMP_USBDI_CLASS_UVC_REQUEST;

	typedef UINT8
		USBPUMP_USBDI_CLASS_UVC_REQUEST_CODE,
		*PUSBPUMP_USBDI_CLASS_UVC_REQUEST_CODE;
	typedef ARG_UINT8
		ARG_USBPUMP_USBDI_CLASS_UVC_REQUEST_CODE,
		*PARG_USBPUMP_USBDI_CLASS_UVC_REQUEST_CODE;

	typedef UINT8
		USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT,
		*PUSBPUMP_USBDI_CLASS_UVC_REQUEST_STAT;
	typedef	ARG_UINT8
		ARG_USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT,
		*PARG_USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT;

Contents:
	USBPUMP_USBDI_CLASS_UVC_REQUEST_CODE	Request;
		Request code of UVC class request. Defined request codes are

		USBPUMP_USBDI_CLASS_UVC_RQ_VC_REQUEST
			The Video Control request allows the host to send an
			UVC specific request command to video control interface
			via the control pipe.

		USBPUMP_USBDI_CLASS_UVC_RQ_VS_REQUEST
			The Video Stream request allows the host to send an
			UVC specific request command to video streaming
			interface via the control pipe.

		USBPUMP_USBDI_CLASS_UVC_RQ_VS_OPEN
			The Open Stream request allows the host to prepare
			starting video streaming.

		USBPUMP_USBDI_CLASS_UVC_RQ_VS_CLOSE
			The Close Stream request allows the host to finish
			video streaming.

		USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ
			The Read request allows the host to receive a video
			frame data via the bulk or isochronous pipe.

		USBPUMP_USBDI_CLASS_UVC_RQ_VS_WRITE
			The Write request allows the host to send a video frame
			data via the bulk or isochronous pipe.

		USBPUMP_USBDI_CLASS_UVC_RQ_VS_SET_STILL
			The Set Still request allows the host to set up still
			image transfer.

		USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ_STILL
			The Read Still request allows the host to receive a
			video still image data from bulk pipe. This request is
			valid only if UVC device's bStillCaptureMethod is 3.

		USBPUMP_USBDI_CLASS_UVC_RQ_VS_CANCEL
			The cancel request allows the host to cancel pending
			video control and video stream request.

	USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT	Status;
		On completion, set to the completion code for this request.
		The common UVC entry logic sets this to USBPUMP_USBDI_CLASS_
		UVC_REQUEST_STATUS_BUSY when the request is accepted, and to
		USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT_XXX when the request
		is done.

	USBPUMP_USBDI_CLASS_UVC_REQUEST_TIMEOUT Timeout;
		Timeout value of request. If Timeout is zero, class driver sets
		default timeout value.

	UINT8	bRequestCode;
		The UVC defined request code like USB_bRequest_Video_SET_CUR.
		Request code is defined in the usbvideo11.h file.

	UINT8	bControlSelector;
		The UVC defined control selector value. The bControlSelector
		value is defined in the usbvideo11.h file.

	UINT8	bUnitTerminalId;
		The unit or terminal id value of the VC_REQUEST.

	UINT8	iVideoStream;
		The video stream interface index value and it is 0-based index.

	UINT8 *			pBuffer;
	USBPUMP_BUFFER_HANDLE	hBuffer;
	BYTES			nBuffer;
	BYTES			nActual;
		Transfer data buffer information -- pointer of buffer, buffer
		handle, and size of the buffer.  The nActual is actual data
		transfer size in bytes and UVC driver will update this value
		when complete UVC request.

	USBPUMP_ISOCH_PACKET_DESCR *	pIsochDesc;
	USBPUMP_BUFFER_HANDLE		hIsochDesc;
	UINT16				nIsochDesc;
	UINT16				nActualDesc;
		Isoch packet descriptor information -- pointer of the isoch
		packet descriptor, number of the isoch packet descriptor. The
		nActualDesc is actual number of isoch packet descriptor and
		UVC driver will set this value. 

	USBPUMP_USBDI_CLASS_UVC_REQUEST_DONE_FN pDoneFn;
		The function to be called when the request is completed.
		The function is of type:

			VOID (pDoneFn)(
				USBPUMP_USBDI_CLASS_UVC_REQUEST *pRequest,
				USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT Status
				);

		The parameters are provided for convenience; on typical
		RISC processors, the parameters will be passed in
		registers, saving the code that would otherwise be
		required to fetch the result.

	VOID *pDoneInfo;
		Completion information.

	USBPUMP_USBDI_CLASS_UVC_REQUEST *pNext, *pLast;
		Fields reserved for use by the transient owner of the
		request packet.  This is the client prior to queuing
		and as soon as the packet is completed; and the
		UVC or its delegates from the time the packet is queued
		until the moment the completion routine is called.

Private Contents:
	UCALLBACKCOMPLETION	Event;
		DataPump callback event.

	VOID *	pFunction;
		Save function pointer.

Notes:
	None.

See Also:
	USBPUMP_USBDI_CLASS_UVC_REQUEST_DONE_FN
	USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT

*/

struct __TMS_STRUCTNAME(USBPUMP_USBDI_CLASS_UVC_REQUEST)
	{
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST_CODE	Request;
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT	Status;
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST_TIMEOUT	Timeout;

	__TMS_UINT8					bRequestCode;
	__TMS_UINT8					bControlSelector;
	__TMS_UINT8					bUnitTerminalId;
	__TMS_UINT8					iVideoStream;

	__TMS_VOID *					pBuffer;
	__TMS_USBPUMP_BUFFER_HANDLE			hBuffer;
	__TMS_BYTES					nBuffer;
	__TMS_BYTES					nActual;

	__TMS_USBPUMP_ISOCH_PACKET_DESCR *		pIsochDesc;
	__TMS_USBPUMP_BUFFER_HANDLE			hIsochDesc;
	__TMS_UINT16					nIsochDesc;
	__TMS_UINT16					nActualDesc;

	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST_DONE_FN *	pDoneFn;
	__TMS_VOID *					pDoneInfo;

	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST *		pNext;
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST *		pLast;

	/* Internal usage */
	__TMS_UCALLBACKCOMPLETION			Event;
	__TMS_VOID *					pFunction;
	};

/* Request code */
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VC_REQUEST	0
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_REQUEST	1
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_OPEN	2
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_CLOSE	3
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ	4
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_WRITE	5
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_SET_STILL	6
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ_STILL	7
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_CANCEL		8
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ__MAX		9

/*
|| Cancel request flags.
|| If client wants to cancel read and write request, client should use
|| USBPUMP_USBDI_CLASS_UVC_RQ_VS_CLOSE request.
*/
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_CANCEL_VC_REQUEST	(1u << 0)
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_CANCEL_VS_REQUEST	(1u << 1)
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_CANCEL_ALL	(3u << 0)

/* Status */
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_OK		0
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_BUSY		1
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_ERROR	2
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_NO_DEVICE	3
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_NO_STREAM	4
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_INVALID	5
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_NOT_SUPPORT	6
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_ALREADY_OPEN	7
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_NOT_OPEN	8
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_KILL		9
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_NO_MEMORY	10
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_STAT_STALL	11


/****************************************************************************\
|
|	Functions/macros
|
\****************************************************************************/

/* check to see if the request has a iVideoStream */
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_HAS_STREAM(Req)		\
	(((1 << (Req)) &						\
	  ((1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_REQUEST) |		\
	   (1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_OPEN) |		\
	   (1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_CLOSE) |		\
	   (1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ) |		\
	   (1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_WRITE) |		\
	   (1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_SET_STILL) |	\
	   (1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ_STILL))) != 0)

#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_CHECK_STREAM_OPEN(Req)		\
	(((1 << (Req)) &						\
	  ((1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_CLOSE) |		\
	   (1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ) |		\
	   (1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_WRITE) |		\
	   (1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_SET_STILL) |	\
	   (1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ_STILL))) != 0)

#define	__TMS_USBPUMP_USBDI_CLASS_UVC_RQ_CHECK_ISOCH_DESC(Req)		\
	(((1 << (Req)) &						\
	  ((1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ) |		\
	   (1 << __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_WRITE))) != 0)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

#define	UsbPumpUsbdUvc_PrepareOpenRequest(					\
	a_pRequest,								\
	a_iVideoStream,								\
	/* USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT * */ a_pVideoStreamProbe,	\
	a_pDoneFn,								\
	a_pDoneInfo								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_OPEN;	\
	(a_pRequest)->iVideoStream = (a_iVideoStream);				\
	(a_pRequest)->pBuffer = (a_pVideoStreamProbe);				\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareCloseRequest(					\
	a_pRequest,								\
	a_iVideoStream,								\
	a_pDoneFn,								\
	a_pDoneInfo								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_CLOSE;	\
	(a_pRequest)->iVideoStream = (a_iVideoStream);				\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareCancelRequest(					\
	a_pRequest,								\
	a_CancelFlags,								\
	a_pDoneFn,								\
	a_pDoneInfo								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_CANCEL;	\
	(a_pRequest)->bRequestCode = (a_CancelFlags);				\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareReadRequest(					\
	a_pRequest,								\
	a_iVideoStream,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ;		\
	(a_pRequest)->iVideoStream = (a_iVideoStream);				\
	(a_pRequest)->pBuffer = (a_pBuffer);					\
	(a_pRequest)->hBuffer = (a_hBuffer);					\
	(a_pRequest)->nBuffer = (a_nBuffer);					\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	(a_pRequest)->Timeout = (a_Timeout);					\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareWriteRequest(					\
	a_pRequest,								\
	a_iVideoStream,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_WRITE;		\
	(a_pRequest)->iVideoStream = (a_iVideoStream);				\
	(a_pRequest)->pBuffer = (a_pBuffer);					\
	(a_pRequest)->hBuffer = (a_hBuffer);					\
	(a_pRequest)->nBuffer = (a_nBuffer);					\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	(a_pRequest)->Timeout = (a_Timeout);					\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareReadIsochRequest(					\
	a_pRequest,								\
	a_iVideoStream,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pIsochDesc,								\
	a_hIsochDesc,								\
	a_nIsochDesc,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ;		\
	(a_pRequest)->iVideoStream = (a_iVideoStream);				\
	(a_pRequest)->pBuffer = (a_pBuffer);					\
	(a_pRequest)->hBuffer = (a_hBuffer);					\
	(a_pRequest)->nBuffer = (a_nBuffer);					\
	(a_pRequest)->pIsochDesc = (a_pIsochDesc);				\
	(a_pRequest)->hIsochDesc = (a_hIsochDesc);				\
	(a_pRequest)->nIsochDesc = (a_nIsochDesc);				\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	(a_pRequest)->Timeout = (a_Timeout);					\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareWriteIsochRequest(				\
	a_pRequest,								\
	a_iVideoStream,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pIsochDesc,								\
	a_hIsochDesc,								\
	a_nIsochDesc,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_WRITE;		\
	(a_pRequest)->iVideoStream = (a_iVideoStream);				\
	(a_pRequest)->pBuffer = (a_pBuffer);					\
	(a_pRequest)->hBuffer = (a_hBuffer);					\
	(a_pRequest)->nBuffer = (a_nBuffer);					\
	(a_pRequest)->pIsochDesc = (a_pIsochDesc);				\
	(a_pRequest)->hIsochDesc = (a_hIsochDesc);				\
	(a_pRequest)->nIsochDesc = (a_nIsochDesc);				\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	(a_pRequest)->Timeout = (a_Timeout);					\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareSetStillRequest(					\
	a_pRequest,								\
	a_iVideoStream,								\
	/* USBPUMP_USBDI_UVC_STREAM_STILL_PROBE_COMMIT * */ a_pVideoStillProbe,	\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_SET_STILL;	\
	(a_pRequest)->iVideoStream = (a_iVideoStream);				\
	(a_pRequest)->pBuffer = (a_pVideoStillProbe);				\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	(a_pRequest)->Timeout = (a_Timeout);					\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareReadStillRequest(					\
	a_pRequest,								\
	a_iVideoStream,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ_STILL;	\
	(a_pRequest)->iVideoStream = (a_iVideoStream);				\
	(a_pRequest)->pBuffer = (a_pBuffer);					\
	(a_pRequest)->hBuffer = (a_hBuffer);					\
	(a_pRequest)->nBuffer = (a_nBuffer);					\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	(a_pRequest)->Timeout = (a_Timeout);					\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareInterfaceControlRequest(				\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VC_REQUEST;	\
	(a_pRequest)->bRequestCode = (a_bRequestCode); 				\
	(a_pRequest)->bControlSelector = (a_bControlSelector);			\
	(a_pRequest)->bUnitTerminalId = 0;					\
	(a_pRequest)->pBuffer = (a_pBuffer);					\
	(a_pRequest)->hBuffer = (a_hBuffer);					\
	(a_pRequest)->nBuffer = (a_nBuffer);					\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	(a_pRequest)->Timeout = (a_Timeout);					\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VC_REQUEST;	\
	(a_pRequest)->bRequestCode = (a_bRequestCode); 				\
	(a_pRequest)->bControlSelector = (a_bControlSelector);			\
	(a_pRequest)->bUnitTerminalId = (a_bUnitTerminalId);			\
	(a_pRequest)->pBuffer = (a_pBuffer);					\
	(a_pRequest)->hBuffer = (a_hBuffer);					\
	(a_pRequest)->nBuffer = (a_nBuffer);					\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	(a_pRequest)->Timeout = (a_Timeout);					\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareCameraTerminalControlRequest(			\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_bTerminalId,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_bTerminalId,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareSelectorUnitControlRequest(			\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_bUnitId,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_bUnitId,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareProcessingUnitControlRequest(			\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_bUnitId,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_bUnitId,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareEncodingUnitControlRequest(			\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_bUnitId,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_bUnitId,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareExtensionUnitControlRequest(			\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_bUnitId,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_bUnitId,								\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareControlRequest_SET_CUR(				\
	a_pRequest,								\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	/* bRequest */ USB_bRequest_Video_SET_CUR,				\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareControlRequest_GET_CUR(				\
	a_pRequest,								\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	/* bRequest */ USB_bRequest_Video_GET_CUR,				\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareControlRequest_GET_MIN(				\
	a_pRequest,								\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	/* bRequest */ USB_bRequest_Video_GET_MIN,				\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareControlRequest_GET_MAX(				\
	a_pRequest,								\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	/* bRequest */ USB_bRequest_Video_GET_MAX,				\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareControlRequest_GET_RES(				\
	a_pRequest,								\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	/* bRequest */ USB_bRequest_Video_GET_RES,				\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareControlRequest_GET_LEN(				\
	a_pRequest,								\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	/* bRequest */ USB_bRequest_Video_GET_LEN,				\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareControlRequest_GET_INFO(				\
	a_pRequest,								\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	/* bRequest */ USB_bRequest_Video_GET_INFO,				\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareControlRequest_GET_DEF(				\
	a_pRequest,								\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
	UsbPumpUsbdUvc_PrepareUnitTerminalControlRequest(			\
	a_pRequest,								\
	/* bRequest */ USB_bRequest_Video_GET_DEF,				\
	a_bControlSelector,							\
	a_bUnitTerminalId,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)

#define	UsbPumpUsbdUvc_PrepareVideoStreamingRequest(				\
	a_pRequest,								\
	a_iVideoStream,								\
	a_bRequestCode,								\
	a_bControlSelector,							\
	a_pBuffer,								\
	a_hBuffer,								\
	a_nBuffer,								\
	a_pDoneFn,								\
	a_pDoneInfo,								\
	a_Timeout								\
	)									\
    do	{									\
	(a_pRequest)->Request = __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_REQUEST;	\
	(a_pRequest)->iVideoStream = (a_iVideoStream);				\
	(a_pRequest)->bRequestCode = (a_bRequestCode); 				\
	(a_pRequest)->bControlSelector = (a_bControlSelector);			\
	(a_pRequest)->pBuffer = (a_pBuffer);					\
	(a_pRequest)->hBuffer = (a_hBuffer);					\
	(a_pRequest)->nBuffer = (a_nBuffer);					\
	(a_pRequest)->pDoneFn = (a_pDoneFn);					\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);				\
	(a_pRequest)->Timeout = (a_Timeout);					\
	} while (0)


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_CLASS_UVC_FRAME_LENGTH_MASK	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_FRAME_LENGTH_MASK
# define USBPUMP_USBDI_CLASS_UVC_FRAME_START_FRAME	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_FRAME_START_FRAME
# define USBPUMP_USBDI_CLASS_UVC_FRAME_END_FRAME	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_FRAME_END_FRAME
# define USBPUMP_USBDI_CLASS_UVC_FRAME_GET_LENGTH(nPlayload)	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_FRAME_GET_LENGTH(nPlayload)
# define USBPUMP_USBDI_CLASS_UVC_FRAME_IS_START_FRAME(nPlayload)	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_FRAME_IS_START_FRAME(nPlayload)
# define USBPUMP_USBDI_CLASS_UVC_FRAME_IS_END_FRAME(nPlayload)	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_FRAME_IS_END_FRAME(nPlayload)
# define USBPUMP_USBDI_CLASS_UVC_RQ_VC_REQUEST	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VC_REQUEST
# define USBPUMP_USBDI_CLASS_UVC_RQ_VS_REQUEST	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_REQUEST
# define USBPUMP_USBDI_CLASS_UVC_RQ_VS_OPEN	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_OPEN
# define USBPUMP_USBDI_CLASS_UVC_RQ_VS_CLOSE	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_CLOSE
# define USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ
# define USBPUMP_USBDI_CLASS_UVC_RQ_VS_WRITE	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_WRITE
# define USBPUMP_USBDI_CLASS_UVC_RQ_VS_SET_STILL	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_SET_STILL
# define USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ_STILL	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_VS_READ_STILL
# define USBPUMP_USBDI_CLASS_UVC_RQ_CANCEL	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_CANCEL
# define USBPUMP_USBDI_CLASS_UVC_RQ__MAX	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ__MAX
# define USBPUMP_USBDI_CLASS_UVC_CANCEL_VC_REQUEST	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_CANCEL_VC_REQUEST
# define USBPUMP_USBDI_CLASS_UVC_CANCEL_VS_REQUEST	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_CANCEL_VS_REQUEST
# define USBPUMP_USBDI_CLASS_UVC_CANCEL_ALL	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_CANCEL_ALL
# define USBPUMP_USBDI_CLASS_UVC_STAT_OK	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_OK
# define USBPUMP_USBDI_CLASS_UVC_STAT_BUSY	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_BUSY
# define USBPUMP_USBDI_CLASS_UVC_STAT_ERROR	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_ERROR
# define USBPUMP_USBDI_CLASS_UVC_STAT_NO_DEVICE	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_NO_DEVICE
# define USBPUMP_USBDI_CLASS_UVC_STAT_NO_STREAM	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_NO_STREAM
# define USBPUMP_USBDI_CLASS_UVC_STAT_INVALID	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_INVALID
# define USBPUMP_USBDI_CLASS_UVC_STAT_NOT_SUPPORT	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_NOT_SUPPORT
# define USBPUMP_USBDI_CLASS_UVC_STAT_ALREADY_OPEN	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_ALREADY_OPEN
# define USBPUMP_USBDI_CLASS_UVC_STAT_NOT_OPEN	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_NOT_OPEN
# define USBPUMP_USBDI_CLASS_UVC_STAT_KILL	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_KILL
# define USBPUMP_USBDI_CLASS_UVC_STAT_NO_MEMORY	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_NO_MEMORY
# define USBPUMP_USBDI_CLASS_UVC_STAT_STALL	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_STAT_STALL
# define USBPUMP_USBDI_CLASS_UVC_RQ_HAS_STREAM(Req)	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_HAS_STREAM(Req)
# define USBPUMP_USBDI_CLASS_UVC_RQ_CHECK_STREAM_OPEN(Req)	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_CHECK_STREAM_OPEN(Req)
# define USBPUMP_USBDI_CLASS_UVC_RQ_CHECK_ISOCH_DESC(Req)	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_RQ_CHECK_ISOCH_DESC(Req)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbd_class_uvc_request.h ****/
#endif /* _USBPUMP_USBD_CLASS_UVC_REQUEST_H_ */
