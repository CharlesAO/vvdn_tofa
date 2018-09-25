/* usbpump_proto_vsc2_request.h	Fri Jun 29 2018 16:33:53 chwon */

/*

Module:  usbpump_proto_vsc2_request.h

Function:
	Definition of the VSC protocol request

Version:
	V3.21f	Fri Jun 29 2018 16:33:53 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2013, 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2013

Revision history:
   3.11c  Fri Mar 22 2013 14:04:28 chwon
	16968: Module created.

   3.11c  Thu Apr 11 2013 16:01:05  chwon
	16968: Added UsbPumpProtoVsc2Request_Init().

   3.21f  Fri Jun 29 2018 16:33:53  chwon
	20840: Added UsbPumpProtoVsc2Request_PrepareSuperSpeedStreamIn(),
	UsbPumpProtoVsc2Request_PrepareSuperSpeedStreamOut() and
	UsbPumpProtoVsc2Request_SetTimeout().

*/

#ifndef _USBPUMP_PROTO_VSC2_REQUEST_H_	/* prevent multiple includes */
#define _USBPUMP_PROTO_VSC2_REQUEST_H_

#ifndef _USBPUMP_PROTO_VSC2_TYPES_H_
# include "usbpump_proto_vsc2_types.h"
#endif

#ifndef _UBUFQE_H_
# include "ubufqe.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

#ifndef _USTAT_H_
# include "ustat.h"
#endif

/****************************************************************************\
|
|	USBPUMP_PROTO_VSC2_REQUEST structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME	(USBPUMP_PROTO_VSC2_REQUEST)
	{
	__TMS_USBPUMP_PROTO_VSC2_STREAM_HANDLE	hStream;
	__TMS_UBUFQE_GENERIC			Qe;

	/* VSC protocol internal usage */
	__TMS_USBPUMP_PROTO_VSC2 *		pSelf;
	__TMS_UCALLBACKCOMPLETION		Callback;
	};


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

#define	UsbPumpProtoVsc2Request_Init(					\
	a_pRequest							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeLegacy.uqe_next =			\
	    (a_pRequest)->Qe.UbufqeLegacy.uqe_last =			\
		&(a_pRequest)->Qe.UbufqeLegacy;				\
	} while (0)

#define	UsbPumpProtoVsc2Request_PrepareCommon(				\
	a_pRequest,							\
	a_QeType,							\
	a_hStream,							\
	a_pBuffer,							\
	a_hBuffer,							\
	a_nBuffer,							\
	a_TransferFlags,						\
	a_pDoneFn,							\
	a_pDoneCtx							\
	)								\
    do	{								\
	(a_pRequest)->hStream = (a_hStream);				\
	(a_pRequest)->Qe.a_QeType.uqe_buf = (a_pBuffer);		\
	(a_pRequest)->Qe.a_QeType.uqe_hBuffer = (a_hBuffer);		\
	(a_pRequest)->Qe.a_QeType.uqe_bufsize = (a_nBuffer);		\
	(a_pRequest)->Qe.a_QeType.uqe_flags = (a_TransferFlags);	\
	(a_pRequest)->Qe.a_QeType.uqe_donefn = (a_pDoneFn);		\
	(a_pRequest)->Qe.a_QeType.uqe_doneinfo = (a_pDoneCtx);		\
	} while (0)

#define	UsbPumpProtoVsc2Request_PrepareLegacy(				\
	a_pRequest,							\
	a_hStream,							\
	a_pBuffer,							\
	a_hBuffer,							\
	a_nBuffer,							\
	a_TransferFlags,						\
	a_pDoneFn,							\
	a_pDoneCtx							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeLegacy.uqe_length =			\
		sizeof((a_pRequest)->Qe);				\
	(a_pRequest)->Qe.UbufqeLegacy.uqe_request =			\
		__TMS_UBUFQE_RQ_LEGACY;					\
	UsbPumpProtoVsc2Request_PrepareCommon(				\
		a_pRequest,						\
		UbufqeLegacy,						\
		a_hStream,						\
		a_pBuffer,						\
		a_hBuffer,						\
		a_nBuffer,						\
		a_TransferFlags,					\
		a_pDoneFn,						\
		a_pDoneCtx						\
		);							\
	} while (0)

#define	UsbPumpProtoVsc2Request_PrepareImplicit(			\
	a_pRequest,							\
	a_hStream,							\
	a_pBuffer,							\
	a_hBuffer,							\
	a_nBuffer,							\
	a_TransferFlags,						\
	a_pDoneFn,							\
	a_pDoneCtx							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeImplicit.uqe_length =			\
		sizeof((a_pRequest)->Qe);				\
	(a_pRequest)->Qe.UbufqeImplicit.uqe_request =			\
		__TMS_UBUFQE_RQ_IMPLICIT;				\
	UsbPumpProtoVsc2Request_PrepareCommon(				\
		a_pRequest,						\
		UbufqeImplicit,						\
		a_hStream,						\
		a_pBuffer,						\
		a_hBuffer,						\
		a_nBuffer,						\
		a_TransferFlags,					\
		a_pDoneFn,						\
		a_pDoneCtx						\
		);							\
	} while (0)

#define	UsbPumpProtoVsc2Request_PrepareStreamIn(			\
	a_pRequest,							\
	a_hStream,							\
	a_pBuffer,							\
	a_hBuffer,							\
	a_nBuffer,							\
	a_TransferFlags,						\
	a_pDoneFn,							\
	a_pDoneCtx							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeStreamIn.uqe_length =			\
		sizeof((a_pRequest)->Qe);				\
	(a_pRequest)->Qe.UbufqeStreamIn.uqe_request =			\
		__TMS_UBUFQE_RQ_STREAM_IN;				\
	UsbPumpProtoVsc2Request_PrepareCommon(				\
		a_pRequest,						\
		UbufqeStreamIn,						\
		a_hStream,						\
		a_pBuffer,						\
		a_hBuffer,						\
		a_nBuffer,						\
		a_TransferFlags,					\
		a_pDoneFn,						\
		a_pDoneCtx						\
		);							\
	} while (0)

#define	UsbPumpProtoVsc2Request_PrepareStreamOut(			\
	a_pRequest,							\
	a_hStream,							\
	a_pBuffer,							\
	a_hBuffer,							\
	a_nBuffer,							\
	a_TransferFlags,						\
	a_pDoneFn,							\
	a_pDoneCtx							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeStreamOut.uqe_length =			\
		sizeof((a_pRequest)->Qe);				\
	(a_pRequest)->Qe.UbufqeStreamOut.uqe_request =			\
		__TMS_UBUFQE_RQ_STREAM_OUT;				\
	UsbPumpProtoVsc2Request_PrepareCommon(				\
		a_pRequest,						\
		UbufqeStreamOut,					\
		a_hStream,						\
		a_pBuffer,						\
		a_hBuffer,						\
		a_nBuffer,						\
		a_TransferFlags,					\
		a_pDoneFn,						\
		a_pDoneCtx						\
		);							\
	} while (0)

#define	UsbPumpProtoVsc2Request_PrepareIsochIn(				\
	a_pRequest,							\
	a_hStream,							\
	a_pBuffer,							\
	a_hBuffer,							\
	a_nBuffer,							\
	a_TransferFlags,						\
	a_pIsochDescr,							\
	a_hIsochDescr,							\
	a_nIsochDescr,							\
	a_IsochStartFrame,						\
	a_pDoneFn,							\
	a_pDoneCtx							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeIsochIn.uqe_length =			\
		sizeof((a_pRequest)->Qe);				\
	(a_pRequest)->Qe.UbufqeIsochIn.uqe_request =			\
		__TMS_UBUFQE_RQ_ISOCH_IN;				\
	UsbPumpProtoVsc2Request_PrepareCommon(				\
		a_pRequest,						\
		UbufqeIsochIn,						\
		a_hStream,						\
		a_pBuffer,						\
		a_hBuffer,						\
		a_nBuffer,						\
		a_TransferFlags,					\
		a_pDoneFn,						\
		a_pDoneCtx						\
		);							\
	(a_pRequest)->Qe.UbufqeIsochIn.pIsochDescr = (a_pIsochDescr);	\
	(a_pRequest)->Qe.UbufqeIsochIn.hIsochDescr = (a_hIsochDescr);	\
	(a_pRequest)->Qe.UbufqeIsochIn.IsochDescrSize = (a_nIsochDescr);\
	(a_pRequest)->Qe.UbufqeIsochIn.IsochStartFrame =		\
		(a_IsochStartFrame);					\
	} while (0)

#define	UsbPumpProtoVsc2Request_PrepareIsochOut(			\
	a_pRequest,							\
	a_hStream,							\
	a_pBuffer,							\
	a_hBuffer,							\
	a_nBuffer,							\
	a_TransferFlags,						\
	a_pIsochDescr,							\
	a_hIsochDescr,							\
	a_nIsochDescr,							\
	a_IsochStartFrame,						\
	a_pDoneFn,							\
	a_pDoneCtx							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeIsochOut.uqe_length =			\
		sizeof((a_pRequest)->Qe);				\
	(a_pRequest)->Qe.UbufqeIsochOut.uqe_request =			\
		__TMS_UBUFQE_RQ_ISOCH_OUT;				\
	UsbPumpProtoVsc2Request_PrepareCommon(				\
		a_pRequest,						\
		UbufqeIsochOut,						\
		a_hStream,						\
		a_pBuffer,						\
		a_hBuffer,						\
		a_nBuffer,						\
		a_TransferFlags,					\
		a_pDoneFn,						\
		a_pDoneCtx						\
		);							\
	(a_pRequest)->Qe.UbufqeIsochOut.pIsochDescr = (a_pIsochDescr);	\
	(a_pRequest)->Qe.UbufqeIsochOut.hIsochDescr = (a_hIsochDescr);	\
	(a_pRequest)->Qe.UbufqeIsochOut.IsochDescrSize =		\
		(a_nIsochDescr);					\
	(a_pRequest)->Qe.UbufqeIsochOut.IsochStartFrame =		\
		(a_IsochStartFrame);					\
	} while (0)

#define	UsbPumpProtoVsc2Request_PrepareSuperSpeedStreamIn(		\
	a_pRequest,							\
	a_hStream,							\
	a_pBuffer,							\
	a_hBuffer,							\
	a_nBuffer,							\
	a_StreamId,							\
	a_TransferFlags,						\
	a_pDoneFn,							\
	a_pDoneCtx							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeSsStreamIn.uqe_length =			\
		sizeof((a_pRequest)->Qe);				\
	(a_pRequest)->Qe.UbufqeSsStreamIn.uqe_request =			\
		__TMS_UBUFQE_RQ_SS_STREAM_IN;				\
	(a_pRequest)->Qe.UbufqeSsStreamIn.StreamID = a_StreamId;	\
	UsbPumpProtoVsc2Request_PrepareCommon(				\
		a_pRequest,						\
		UbufqeSsStreamIn,					\
		a_hStream,						\
		a_pBuffer,						\
		a_hBuffer,						\
		a_nBuffer,						\
		a_TransferFlags,					\
		a_pDoneFn,						\
		a_pDoneCtx						\
		);							\
	} while (0)

#define	UsbPumpProtoVsc2Request_PrepareSuperSpeedStreamOut(		\
	a_pRequest,							\
	a_hStream,							\
	a_pBuffer,							\
	a_hBuffer,							\
	a_nBuffer,							\
	a_StreamId,							\
	a_TransferFlags,						\
	a_pDoneFn,							\
	a_pDoneCtx							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeSsStreamOut.uqe_length =			\
		sizeof((a_pRequest)->Qe);				\
	(a_pRequest)->Qe.UbufqeSsStreamOut.uqe_request =		\
		__TMS_UBUFQE_RQ_SS_STREAM_OUT;				\
	(a_pRequest)->Qe.UbufqeSsStreamOut.StreamID = a_StreamId;	\
	UsbPumpProtoVsc2Request_PrepareCommon(				\
		a_pRequest,						\
		UbufqeSsStreamOut,					\
		a_hStream,						\
		a_pBuffer,						\
		a_hBuffer,						\
		a_nBuffer,						\
		a_TransferFlags,					\
		a_pDoneFn,						\
		a_pDoneCtx						\
		);							\
	} while (0)

#define	UsbPumpProtoVsc2Request_SetStreamHandle(			\
	a_pRequest,							\
	a_hStream							\
	)								\
    do	{								\
	(a_pRequest)->hStream = (a_hStream);				\
	} while (0)

#define	UsbPumpProtoVsc2Request_SetBuffer(				\
	a_pRequest,							\
	a_pBuffer,							\
	a_hBuffer,							\
	a_nBuffer							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeLegacy.uqe_buf = (a_pBuffer);		\
	(a_pRequest)->Qe.UbufqeLegacy.uqe_hBuffer = (a_hBuffer);	\
	(a_pRequest)->Qe.UbufqeLegacy.uqe_bufsize = (a_nBuffer);	\
	} while (0)

#define	UsbPumpProtoVsc2Request_SetTransferFlags(			\
	a_pRequest,							\
	a_TransferFlags							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeLegacy.uqe_flags = (a_TransferFlags);	\
	} while (0)

#define	UsbPumpProtoVsc2Request_SetTimeout(				\
	a_pRequest,							\
	a_Timeout							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeLegacy.uqe_timeout = (a_Timeout);	\
	} while (0)

#define	UsbPumpProtoVsc2Request_SetCallback(				\
	a_pRequest,							\
	a_pDoneFn,							\
	a_pDoneCtx							\
	)								\
    do	{								\
	(a_pRequest)->Qe.UbufqeLegacy.uqe_donefn = (a_pDoneFn);		\
	(a_pRequest)->Qe.UbufqeLegacy.uqe_doneinfo = (a_pDoneCtx);	\
	} while (0)

#define	UsbPumpProtoVsc2Request_GetStatus(a_pRequest)			\
	((a_pRequest)->Qe.UbufqeLegacy.uqe_status)

#define	UsbPumpProtoVsc2Request_StatusOk(a_pRequest)			\
	((a_pRequest)->Qe.UbufqeLegacy.uqe_status == __TMS_USTAT_OK)

#define	UsbPumpProtoVsc2Request_GetActual(a_pRequest)			\
	((a_pRequest)->Qe.UbufqeLegacy.uqe_bufars)

/**** end of usbpump_proto_vsc2_request.h ****/
#endif /* _USBPUMP_PROTO_VSC2_REQUEST_H_ */
