/* usbpump_usbseri.h	Mon Jan 16 2017 12:56:48 chwon */

/*

Module:  usbpump_usbseri.h

Function:
	Include file for USBSERI -- API functions and types for calling
	from inside the DataPump.

Version:
	V3.19b	Mon Jan 16 2017 12:56:48 chwon	Edit level 9

Copyright notice:
	This file copyright (C) 2006, 2010, 2013, 2015-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	May 2006

Revision history:
   1.00a  Tue May 23 2006 06:23:19  tmm
	Module created.

   1.97d  Thu Jul 13 2006 23:41:47  chwon
	Move common place from client tree and remove client specific info

   1.97f  Mon Sep 18 2006 12:13:32  chwon
	Add CtrlQeBufferSize & DataQeBufferSize for high speed. We will allcate
	large buffer for UBUFQE if device support high speed.

   3.01d  Wed Nov 10 2010 14:29:02  chwon
	11849: add USBSERI_WMC_OBJECT_MODEM_CONFIG type and init macros and add
	Usbseri_CreateModem_V2() prototype.

   3.11b  Tue Jan 22 2013 08:43:28  tmm
	16507: change buffer sizes to size_t / BYTESs, at least for write
	path.

   3.11b  Wed Jan 30 2013 20:53:32  chwon
	16386: add fSupportNoDataCopy in the configuration and USBSERI_UART.

   3.15b  Sun May 31 2015 23:51:20  tmm
	19211: correct usages of "DataPump".

   3.17a  Thu Jul 07 2016 17:20:17  chwon
	20190: Add fActivated flag in the USBSERI_WMC_OBJECT_MODEM and add
	RxDataSize in the USBSERI_UART. Remove not used variables.

   3.17a  Fri Aug 05 2016 09:16:46  chwon
	20232: Add RxDone event and RxDone UBUFQE list head in USBSERI_UART.

   3.19b  Mon Jan 16 2017 12:56:49  chwon
	20440: 20190: Remove RxDataSize in the USBSERI_UART.

*/

#ifndef _USBPUMP_USBSERI_H_		/* prevent multiple includes */
#define _USBPUMP_USBSERI_H_

#include "usbpumpenv.h"
#include "usbpumpobject.h"
#include "usbioctl_modem.h"
#include "cdclinecoding.h"

#include "usbserifc.h"
#include "usbpump_usbseri_params.h"
#include "usbpump_usbseri_types.h"
#include "ubufqe.h"

/****************************************************************************\
|
|	The main instance data
|
\****************************************************************************/

/*
|| Conveniently, we can use one primary data structure for handling
|| the interface to the lower level.  Because of the way things work, we
|| can just record the configuration of the lower layers, and forward things
|| to the appropriate application engine:  either an AT interpreter or
|| an OBEX engine.
||
|| When running in a real OS, this structure will probably be used without
|| typecloaking, so we do it simply.
*/

/****************************************************************************\
|
|	The MODEM structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBSERI_WMC_OBJECT_MODEM)
	{
	/*
	|| For convenience, and so we can recieve IOCTLs, we need
	|| an object header.
	*/
	__TMS_USBPUMP_OBJECT_HEADER	Header;

	/*
	|| We need someplace to store the "object handle"
	*/
	__TMS_PUSBPUMP_OBJECT_HEADER	pIoObject;

	/*
	|| For allocations, we really need a pDevice and a pPlatform
	*/
	__TMS_PUDEVICE			pDevice;
	__TMS_PUPLATFORM		pPlatform;

	/* current control state */
	__TMS_UINT			ControlLines;

	/* the current country code */
	__TMS_UINT			CountryCode;

	/* the current line coding */
	__TMS_USB_Comm_LINE_CODING	LineCoding;

	/* is break on? */
	__TMS_UINT			fBreak: 1;

	/* the current abstract multiplexing state */
	__TMS_UINT			fCmdsInData: 1;

	/* the curent idle state */
	__TMS_UINT			fIdle: 1;

	/* Have we been activated? */
	__TMS_UINT			fUp: 1;
	__TMS_UINT			fActivated: 1;

	/* UBUFQE count and buffer size */
	__TMS_UINT16			CtrlRxQesCount;
	__TMS_UINT16			CtrlRxQeBufferSize;
	__TMS_UINT16			CtrlTxQesCount;
	__TMS_UINT16			CtrlTxQeBufferSize;
	__TMS_UINT16			DataRxQesCount;
	__TMS_UINT16			DataTxQesCount;
	__TMS_BYTES			DataRxQeBufferSize;
	__TMS_BYTES			DataTxQeBufferSize;

	/* per-uart context */
	__TMS_UINT			nUsbUarts;
	__TMS_PUSBSERI_UART		vpUsbUarts[USBSERI_MAX_UARTS_PER_MODEM];
	};

#define	USBSERI_WMCMODEM_TAG		__TMS_UHIL_MEMTAG('M','o','d','m')
#define	USBSERI_WMCMODEM_NAME		"modem.usbseri.mcci.com"

/****************************************************************************\
|
|	The data structure that models a UART to the USBSERI.
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBSERI_UART)
	{
	/*
	|| For convenience, and so we can recieve IOCTLs, we need
	|| an object header.
	*/
	__TMS_USBPUMP_OBJECT_HEADER	Header;

	/* the parent object pointer */
	__TMS_PUSBSERI_WMC_OBJECT_MODEM	pModem;

	/*
	|| In order to bounce into the DataPump task, we need a few
	|| __TMS_UCALLBACKCOMPLETION objects.  We use an 'e' prefix
	|| to help us remember that it is accessed with the DataPump
	|| Event API.
	||
	|| evPushIn is used to tell the DataPump to push
	|| transmit buffers towards the host.
	||
	|| evPullOut is used to tell the DataPump to
	|| queue receive buffers.
	||
	|| evModem is used to push status towards the
	|| DataPump.
	*/
	__TMS_UCALLBACKCOMPLETION	evPushIn;
	__TMS_UCALLBACKCOMPLETION	evPullOut;
	__TMS_UCALLBACKCOMPLETION	evModem;

	__TMS_UCALLBACKCOMPLETION	evRxDone;

/** #if USBSERI_USE_SW_INTERRUPT
* 	__TMS_UINT			ulInterruptEvCount;
*
* 	//
* 	// We need some link fields to communicate with the SW
* 	// interrupt for actually generating the call-backs.
* 	// These are NULL when not enqueued for interrupt
* 	// dispatch by the sw interrupt, and non-NULL when
* 	// already enqueued.  Interrupt altered, therefore....
* 	// but since we manage interlocks we don't use the
* 	// "volatile" keyword.
* 	//
* 	__TMS_USBSERI_UART		*pNextUartInInterruptQueue;
* 	__TMS_USBSERI_UART		*pLastUartInInterruptQueue;
* #endif
*/

	/*
	|| We use a couple of bit-masks to communicate modem changes
	|| to the DataPump.
	*/
	__TMS_UINT32			uModemStatusMask;
	__TMS_UINT32			uModemStatusValues;

	/*
	|| We need to know which plane this UART uses.
	*/
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR
					iPlane;
	__TMS_UBUFQE_FLAGS		uqeRxFlags;
	__TMS_UCHAR			uqeRxTimeout;

	__TMS_UINT			iUart;

	/*
	|| When a UBUFQE is availble to be filled, we put it
	|| in the TxIdleQueue.  Note that data is put into the
	|| first UBUFQE on this queue until either (a) it
	|| gets full or (b) the DataPump decides to grab it away
	|| and ship it to the Host.
	*/
	__TMS_PUBUFQE			pTxIdleQueue;

	/*
	|| When a UBUFQE has been filled, but not yet queued,
	||it gets put on this queue.
	*/
	__TMS_PUBUFQE			pTxReadyQueue;

	__TMS_UINT32			uTxPendingCount;
	__TMS_BYTES			TxBufferSize;

	/*
	|| When a UBUFQE has data to go to the USBSERI, we put it
	|| in the RxAvailQueue until the USBSERI collects it or until
	|| we get bored.  Otherwise it will sit in the
	|| UDATASTREAM waitig for data to arrive.
	*/
	__TMS_BYTES			RxBufferSize;
	__TMS_PUBUFQE			pRxAvailQueue;

	__TMS_PUBUFQE			pRxIdleQueue;
	__TMS_PUBUFQE			pRxDoneQueue;

	/* some parking queues */
	__TMS_PUBUFQE			pRxParkQueue;
	__TMS_PUBUFQE			pTxParkQueue;

	/* and some info in case we need to free the buffers */
	__TMS_UINT32			fIsOpen: 1,
					fIsDevBuf: 1,
					fIsPrimary: 1,
					fSupportCtsControl: 1,
					fSupportNoDataCopy: 1,
					fEnableRxFlow: 1,
					fStopRx: 1,
					fPostRxEvent: 1;

	__TMS_INT			SuggestedUart;

	/* client notification functions */
	USB_SerialEventHandler_t	*pEventNotifyFn;
	__TMS_VOID			*pEventNotifyContext;

	/* we need an event map */
	USBSERI_EVENT_CODE_MAP		EventCodeMap;
	};

#define	USBSERI_UART_TAG		__TMS_UHIL_MEMTAG('U','a','r','t')
#define	USBSERI_UART_NAME		"uart.usbseri.mcci.com"


/****************************************************************************\
|
|	The UBUFQE wrapper structure
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT  (USBSERI_QE);
struct __TMS_STRUCTNAME(USBSERI_QE)
	{
	__TMS_UBUFQE			Qe;

	__TMS_USBSERI_UART *		pUsbUart;
	__TMS_UCALLBACKCOMPLETION	Event;
	UsbSeri_SubmitDone_t *		pDoneFn;
	void *				pDoneCtx;
	};


/****************************************************************************\
|
|	The MODEM configuration structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBSERI_WMC_OBJECT_MODEM_CONFIG)
	{
	__TMS_UINT16			NumCtrlRxQes;
	__TMS_UINT16			SizeCtrlRxQeBuffer;

	__TMS_UINT16			NumCtrlTxQes;
	__TMS_UINT16			SizeCtrlTxQeBuffer;

	__TMS_UINT16			NumDataRxQes;
	__TMS_UINT16			NumDataTxQes;

	__TMS_BYTES			SizeDataRxQeBuffer;
	__TMS_BYTES			SizeDataTxQeBuffer;

	__TMS_BOOL			fSupportCtsControl;
	__TMS_BOOL			fAdjustDataBufferSize;
	__TMS_BOOL			fSetSpecialRxQeFlag;
	__TMS_BOOL			fSupportNoDataCopy;
	};

#define	USBSERI_WMC_OBJECT_MODEM_CONFIG_INIT_V2(			\
	a_NumCtrlRxQes,							\
	a_SizeCtrlRxQeBuffer,						\
	a_NumCtrlTxQes,							\
	a_SizeCtrlTxQeBuffer,						\
	a_NumDataRxQes,							\
	a_SizeDataRxQeBuffer,						\
	a_NumDataTxQes,							\
	a_SizeDataTxQeBuffer,						\
	a_fSupportCtsControl,						\
	a_fAdjustDataBufferSize,					\
	a_fSetSpecialRxQeFlag,						\
	a_fSupportNoDataCopy						\
	)								\
	{								\
	(a_NumCtrlRxQes),						\
	(a_SizeCtrlRxQeBuffer),						\
	(a_NumCtrlTxQes),						\
	(a_SizeCtrlTxQeBuffer),						\
	(a_NumDataRxQes),						\
	(a_SizeDataRxQeBuffer),						\
	(a_NumDataTxQes),						\
	(a_SizeDataTxQeBuffer),						\
	(a_fSupportCtsControl),						\
	(a_fAdjustDataBufferSize),					\
	(a_fSetSpecialRxQeFlag),					\
	(a_fSupportNoDataCopy)						\
    	}

#define	USBSERI_WMC_OBJECT_MODEM_CONFIG_INIT_V1(			\
	a_NumCtrlRxQes,							\
	a_SizeCtrlRxQeBuffer,						\
	a_NumCtrlTxQes,							\
	a_SizeCtrlTxQeBuffer,						\
	a_NumDataRxQes,							\
	a_SizeDataRxQeBuffer,						\
	a_NumDataTxQes,							\
	a_SizeDataTxQeBuffer,						\
	a_fSupportCtsControl,						\
	a_fAdjustDataBufferSize,					\
	a_fSetSpecialRxQeFlag						\
	)								\
	USBSERI_WMC_OBJECT_MODEM_CONFIG_INIT_V2(			\
	a_NumCtrlRxQes,							\
	a_SizeCtrlRxQeBuffer,						\
	a_NumCtrlTxQes,							\
	a_SizeCtrlTxQeBuffer,						\
	a_NumDataRxQes,							\
	a_SizeDataRxQeBuffer,						\
	a_NumDataTxQes,							\
	a_SizeDataTxQeBuffer,						\
	a_fSupportCtsControl,						\
	a_fAdjustDataBufferSize,					\
	a_fSetSpecialRxQeFlag,						\
	/* V2: fSupportNoDataCopy */ __TMS_FALSE			\
	)

#define	USBSERI_WMC_OBJECT_MODEM_CONFIG_SETUP_V2(			\
	a_pConfig,							\
	a_NumCtrlRxQes,							\
	a_SizeCtrlRxQeBuffer,						\
	a_NumCtrlTxQes,							\
	a_SizeCtrlTxQeBuffer,						\
	a_NumDataRxQes,							\
	a_SizeDataRxQeBuffer,						\
	a_NumDataTxQes,							\
	a_SizeDataTxQeBuffer,						\
	a_fSupportCtsControl,						\
	a_fAdjustDataBufferSize,					\
	a_fSetSpecialRxQeFlag,						\
	a_fSupportNoDataCopy						\
	)								\
    do	{								\
	(a_pConfig)->NumCtrlRxQes = (a_NumCtrlRxQes);			\
	(a_pConfig)->SizeCtrlRxQeBuffer = (a_SizeCtrlRxQeBuffer);	\
	(a_pConfig)->NumCtrlTxQes = (a_NumCtrlTxQes);			\
	(a_pConfig)->SizeCtrlTxQeBuffer = (a_SizeCtrlTxQeBuffer);	\
	(a_pConfig)->NumDataRxQes = (a_NumDataRxQes);			\
	(a_pConfig)->SizeDataRxQeBuffer = (a_SizeDataRxQeBuffer);	\
	(a_pConfig)->NumDataTxQes = (a_NumDataTxQes);			\
	(a_pConfig)->SizeDataTxQeBuffer = (a_SizeDataTxQeBuffer);	\
	(a_pConfig)->fSupportCtsControl = (a_fSupportCtsControl);	\
	(a_pConfig)->fAdjustDataBufferSize = (a_fAdjustDataBufferSize);	\
	(a_pConfig)->fSetSpecialRxQeFlag = (a_fSetSpecialRxQeFlag);	\
	(a_pConfig)->fSupportNoDataCopy = (a_fSupportNoDataCopy);	\
    	} while (0)

#define	USBSERI_WMC_OBJECT_MODEM_CONFIG_SETUP_V1(			\
	a_pConfig,							\
	a_NumCtrlRxQes,							\
	a_SizeCtrlRxQeBuffer,						\
	a_NumCtrlTxQes,							\
	a_SizeCtrlTxQeBuffer,						\
	a_NumDataRxQes,							\
	a_SizeDataRxQeBuffer,						\
	a_NumDataTxQes,							\
	a_SizeDataTxQeBuffer,						\
	a_fSupportCtsControl,						\
	a_fAdjustDataBufferSize,					\
	a_fSetSpecialRxQeFlag						\
	)								\
	USBSERI_WMC_OBJECT_MODEM_CONFIG_SETUP_V2(			\
	a_pConfig,							\
	a_NumCtrlRxQes,							\
	a_SizeCtrlRxQeBuffer,						\
	a_NumCtrlTxQes,							\
	a_SizeCtrlTxQeBuffer,						\
	a_NumDataRxQes,							\
	a_SizeDataRxQeBuffer,						\
	a_NumDataTxQes,							\
	a_SizeDataTxQeBuffer,						\
	a_fSupportCtsControl,						\
	a_fAdjustDataBufferSize,					\
	a_fSetSpecialRxQeFlag,						\
	/* V2: fSupportNoDataCopy */ __TMS_FALSE			\
	)


/****************************************************************************\
|
|	The functions.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/* call this multiple times, once for each modem */
__TMS_BOOL
Usbseri_CreateModem(
	__TMS_UPLATFORM *			/* pPlatform */,
	__TMS_USBPUMP_OBJECT_HEADER *		/* pFunctionObject */,
	__TMS_USBSERI_WMC_OBJECT_MODEM **	/* ppModem -- OUT */,
	__TMS_BOOL				/* fSupportCtsControl */
	);

/* call this multiple times, once for each modem */
__TMS_BOOL
Usbseri_CreateModem_V2(
	__TMS_UPLATFORM *			/* pPlatform */,
	__TMS_USBPUMP_OBJECT_HEADER *		/* pFunctionObject */,
	__TMS_USBSERI_WMC_OBJECT_MODEM **	/* ppModem -- OUT */,
	__TMS_CONST __TMS_USBSERI_WMC_OBJECT_MODEM_CONFIG * /* pModemConfig */
	);

/* call this once to set things up */
__TMS_VOID
Usbseri_Initialize(
	__TMS_UPLATFORM *			/* pPlatform */
	);

__TMS_END_DECLS

/**** end of usbpump_usbseri.h ****/
#endif /* _USBPUMP_USBSERI_H_ */
