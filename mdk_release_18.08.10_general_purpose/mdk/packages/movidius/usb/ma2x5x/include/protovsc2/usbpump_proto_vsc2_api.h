/* usbpump_proto_vsc2_api.h	Fri Jun 29 2018 16:33:31 chwon */

/*

Module:  usbpump_proto_vsc2_api.h

Function:
	Definition of the VSC protocol IN/OUT calls.

Version:
	V3.21f	Fri Jun 29 2018 16:33:31 chwon	Edit level 8

Copyright notice:
	This file copyright (C) 2013-2015, 2017-2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2013

Revision history:
   3.11c  Tue Mar 19 2013 17:22:10 chwon
	16968: Module created.

   3.13b  Wed Aug 06 2014 11:21:53  chwon
	18431: Add USBPUMP_PROTO_VSC2_CONTROL_REPLY_ASYNC_FN() API.

   3.13b  Fri Aug 08 2014 09:25:09  chwon
	18431: Add QeFlags parameter in ControlReplyAsync API.

   3.13b  Sat Nov 29 2014 14:12:40  chwon
	18659: Add USBPUMP_PROTO_VSC2_CANCEL_STREAM_FN() API.

   3.15c  Tue Aug 11 2015 16:21:40  chwon
	19385: Add pIoctlFn in the OUTCALL and create V2() macros.

   3.21c  Wed Aug 02 2017 15:34:34  chwon
	20629: Add USBPUMP_PROTO_VSC2_GET_STREAM_INFO_FN() API. Add incall
	tables and API function prototypes.

   3.21f  Fri Feb 02 2018 15:04:37  chwon
	20754: Change USBPUMP_PROTO_VSC2_STREAM_INFO::wMaxPacketSize to 18 bits

   3.21f  Fri Jun 29 2018 16:33:32  chwon
	20840: Fix type cloaking.

*/

#ifndef _USBPUMP_PROTO_VSC2_API_H_	/* prevent multiple includes */
#define _USBPUMP_PROTO_VSC2_API_H_

#ifndef _USBPUMP_PROTO_VSC2_TYPES_H_
# include "usbpump_proto_vsc2_types.h"
#endif

#ifndef _USBIOCTL_API_H_
# include "usbioctl_api.h"
#endif

#ifndef _UBUFQE_H_
# include "ubufqe.h"
#endif

#ifndef _USTAT_H_
# include "ustat.h"
#endif

/****************************************************************************\
|
|	VSC protocol driver in-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_PROTO_VSC2_OPEN_STREAM_FN

Function:
	Open data stream associated with a single pipe.

Definition:
	typedef USBPUMP_PROTO_VSC2_STATUS
	(USBPUMP_PROTO_VSC2_OPEN_STREAM_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		UINT					BindingFlags,
		UINT					EpAddrMask,
		UINT					PipeOrdinal,
		USBPUMP_PROTO_VSC2_STREAM_HANDLE *	pStreamHandle
		);

Description:
	This is the VSC protocol driver in-call function.  The API opens
	data stream associated with a single pipe.  This API allows callers
	to discover the pipes that are from a specified UINTERFACE.
	|BindingFlags|, |EpAddrMask|, and |PipeOrdinal| are used to select a
	subset of the pipes, based on caller's preferences.

	Binding Flags specifies the type (or types) of pipes that are
	acceptable.  The available types are:

		UPIPE_SETTING_MASK_ISO_OUT
		UPIPE_SETTING_MASK_ISO_IN
		UPIPE_SETTING_MASK_BULK_OUT
		UPIPE_SETTING_MASK_BULK_IN
		UPIPE_SETTING_MASK_INT_OUT
		UPIPE_SETTING_MASK_INT_IN

	speicifying the obvious pipe types.  These bit values may be OR'ed
	together.  The API also allows:

		UPIPE_SETTING_MASK_CONTROL_OUT
		UPIPE_SETTING_MASK_CONTROL_IN

	but these are not fully supported because it is default pipe.  However,
	CONTROL_IN means the to-host half of a control endpoint; CONTROL_OUT
	means the from-host half.

	Because flags may be or'ed together, it's possible to accept multiple
	kinds of pipes with a single binding.  The following abbreviations
	are defined:

	UPIPE_SETTING_MASK_ANY matches any possible pipe.
	UPIPE_SETTING_MASK_DATA matches any data pipe (iso,
		bulk, int) x (in, out)
	UPIPE_SETTING_MASK_DATA_IN matches for any IN data pipe
		(iso, bulk, int) x in
	UPIPE_SETTING_MASK_DATA_OUT matches any OUT data pipe
		(iso, bulk, int) x out
	UPIPE_SETTING_MASK_BULKINT_IN matches any bulk or int IN
		pipe:  (bulk, int) x in
	UPIPE_SETTING_MASK_BULKINT_OUT stands for bulk or int OUT pipe
		(bulk, int) x out

	After considering the endpoint binding flags, the endpoint address
	is checked.  If EpAddrMask is zero, then any endpoint can match.
	Otherwise, EpAddrMask is a bit mask, where bit[0] corresponds to
	endpoint address 0, bit[1] corresponds to endpoint address 1, and
	so forth; endpoints are matched by setting the bit corresponding
	to the *desired* endpoint(s).

	|PipeOrdinal| is used to select sequential matching pipes.  This is
	a zero-based index.

Returns:
	USBPUMP_PROTO_VSC2_STATUS_OK if stream opened successfully;
	Possible error codes are
		USBPUMP_PROTO_VSC2_STATUS_INVALID_SESSION_HANDLE
		USBPUMP_PROTO_VSC2_STATUS_INVALID_PARAMETER
		USBPUMP_PROTO_VSC2_STATUS_NO_STREAM_TO_OPEN

Notes:
	This API is synchronous call. This API should be called in the
	DataPump context.

*/

__TMS_FNTYPE_DEF(
USBPUMP_PROTO_VSC2_OPEN_STREAM_FN,
__TMS_USBPUMP_PROTO_VSC2_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* SessionHandle */,
	__TMS_UINT					/* BindingFlags */,
	__TMS_UINT					/* EpAddrMask */,
	__TMS_UINT					/* PipeOrdinal */,
	__TMS_USBPUMP_PROTO_VSC2_STREAM_HANDLE *	/* pStreamHandle */
	));


/*

Type:  USBPUMP_PROTO_VSC2_CLOSE_STREAM_FN

Function:
	Close data stream.

Definition:
	typedef USBPUMP_PROTO_VSC2_STATUS
	(USBPUMP_PROTO_VSC2_CLOSE_STREAM_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		USBPUMP_PROTO_VSC2_STREAM_HANDLE	StreamHandle
		);

Description:
	This is the VSC protocol driver in-call function.  The API closes
	opened data stream.

Returns:
	USBPUMP_PROTO_VSC2_STATUS_OK if stream closed successfully;
	Possible error codes are
		USBPUMP_PROTO_VSC2_STATUS_INVALID_SESSION_HANDLE
		USBPUMP_PROTO_VSC2_STATUS_INVALID_STREAM_HANDLE

Notes:
	This API is synchronous call. This API should be called in the
	DataPump context.

*/

__TMS_FNTYPE_DEF(
USBPUMP_PROTO_VSC2_CLOSE_STREAM_FN,
__TMS_USBPUMP_PROTO_VSC2_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,
	__TMS_USBPUMP_PROTO_VSC2_STREAM_HANDLE	/* StreamHandle */
	));


/*

Type:  USBPUMP_PROTO_VSC2_CANCEL_STREAM_FN

Function:
	Cancel all data transfer requests of the stream.

Definition:
	typedef USBPUMP_PROTO_VSC2_STATUS
	(USBPUMP_PROTO_VSC2_CANCEL_STREAM_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		USBPUMP_PROTO_VSC2_STREAM_HANDLE	StreamHandle
		);

Description:
	This is the VSC protocol driver in-call function.  It will be called by
	the client to cancel all data transfer requests of the given stream.

	If there are pending data transfer requests, it will be completed
	with USTAT_KILL status code. The related endpoint FIFO will be flushed.

Returns:
	USBPUMP_PROTO_VSC2_STATUS_OK if submit request successfully.
	Possible error codes are
		USBPUMP_PROTO_VSC2_STATUS_INVALID_SESSION_HANDLE
		USBPUMP_PROTO_VSC2_STATUS_INVALID_STREAM_HANDLE
		USBPUMP_PROTO_VSC2_STATUS_NOT_CONFIGURED

Notes:
	This API can be called on the arbitration context.

*/

__TMS_FNTYPE_DEF(
USBPUMP_PROTO_VSC2_CANCEL_STREAM_FN,
__TMS_USBPUMP_PROTO_VSC2_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,
	__TMS_USBPUMP_PROTO_VSC2_STREAM_HANDLE	/* StreamHandle */
	));


/*

Type:  USBPUMP_PROTO_VSC2_GET_STREAM_INFO_FN

Function:
	Get current information of the stream.

Definition:
	typedef USBPUMP_PROTO_VSC2_STATUS
	(USBPUMP_PROTO_VSC2_GET_STREAM_INFO_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		USBPUMP_PROTO_VSC2_STREAM_HANDLE	StreamHandle,
		USBPUMP_PROTO_VSC2_STREAM_INFO *	pStreamInfo
		);

Description:
	This is the VSC protocol driver in-call function.  It will be called by
	the client to get current stream information of the given stream.

	Client have to provide pStreamInfo and VSC protocol driver will fill
	in stream information.

Returns:
	USBPUMP_PROTO_VSC2_STATUS_OK if submit request successfully.
	Possible error codes are
		USBPUMP_PROTO_VSC2_STATUS_INVALID_SESSION_HANDLE
		USBPUMP_PROTO_VSC2_STATUS_INVALID_STREAM_HANDLE
		USBPUMP_PROTO_VSC2_STATUS_INVALID_PARAMETER

Notes:
	This API can be called on the arbitration context.

*/

__TMS_FNTYPE_DEF(
USBPUMP_PROTO_VSC2_GET_STREAM_INFO_FN,
__TMS_USBPUMP_PROTO_VSC2_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,
	__TMS_USBPUMP_PROTO_VSC2_STREAM_HANDLE	/* StreamHandle */,
	__TMS_USBPUMP_PROTO_VSC2_STREAM_INFO *	/* pStreamInfo */
	));

struct __TMS_STRUCTNAME	(USBPUMP_PROTO_VSC2_STREAM_INFO)
	{
	__TMS_UINT	wMaxPacketSize: 18;	/* the packet size */
	__TMS_UINT	bEndpointAddress: 8;	/* the endpoint address */
	__TMS_UINT	bmAttributes: 2;	/* the endpoint type */
	__TMS_UINT	fStreamActivated: 1;	/* stream is activated */
	__TMS_UINT	bCurrentSpeed: 3;	/* current device speed */
	};


/*

Type:  USBPUMP_PROTO_VSC2_CONTROL_REPLY_FN

Function:
	Send the reply of a vendor specific class control request.

Definition:
	typedef USBPUMP_PROTO_VSC2_STATUS
	(USBPUMP_PROTO_VSC2_CONTROL_REPLY_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		VOID *			pReplyBuffer,
		UINT16			nReplyBuffer
		);

Description:
	This is the VSC protocol driver in-call function.  It will be called by
	the client to send the reply of a vendor specific class control request
	to the VSC protocol driver.  This API will be used to send control
	request data or status to the host.

	To send control request status OK (or to send a zero length packet /
	ZLP), the client should set pReplyBuffer to not NULL and set
	nReplyBuffer to zero.  To send STALL, the client should set pReplyBuffer
	to NULL and nReplyBuffer to zero.

Returns:
	USBPUMP_PROTO_VSC2_STATUS_OK if sent control reply successfully;
	Possible error codes are
		USBPUMP_PROTO_VSC2_STATUS_INVALID_SESSION_HANDLE
		USBPUMP_PROTO_VSC2_STATUS_INVALID_REQUEST

Notes:
	This API can be called on the arbitration context. The caller should
	ensure that pReplyBuffer is same same data buffer as the
	USBPUMP_PROTO_VSC2_SETUP_PROCESS_FN() callback.

*/

__TMS_FNTYPE_DEF(
USBPUMP_PROTO_VSC2_CONTROL_REPLY_FN,
__TMS_USBPUMP_PROTO_VSC2_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_VOID *			/* pReplyBuffer */,
	__TMS_UINT16			/* nReplyBuffer */
	));


/*

Type:  USBPUMP_PROTO_VSC2_CONTROL_REPLY_ASYNC_FN

Function:
	Send the reply of a vendor specific class control request with callback.

Definition:
	typedef USBPUMP_PROTO_VSC2_STATUS
	(USBPUMP_PROTO_VSC2_CONTROL_REPLY_ASYNC_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		VOID *			pReplyBuffer,
		UINT16			nReplyBuffer,
		UBUFQE_FLAGS		QeFlags,
		USBPUMP_PROTO_VSC2_CONTROL_REPLY_CALLBACK_FN *pCallbackFn,
		VOID *			pCallbackCtx
		);

Description:
	This is the VSC protocol driver in-call function.  It will be called by
	the client to send the reply of a vendor specific class control request
	to the VSC protocol driver.  This API will be used to send control
	request data or status to the host.

	To send control request status OK (or to send a zero length packet /
	ZLP), the client should set pReplyBuffer to not NULL and set
	nReplyBuffer to zero.  To send STALL, the client should set pReplyBuffer
	to NULL and nReplyBuffer to zero.

	This API is asynchronous call and pCallback function will be called
	when complete actual IO operation. This pCallback function will be
	called from DataPump context. So don't block inside of this pCallback
	function.  If this function returns not USBPUMP_PROTO_VSC2_STATUS_OK,
	the pCallback function will not be called.

	The QeFlags is same as UBUFQE flags and client can provides extra
	semantic information about the information in the buffer.  You can find
	detail information regarding UBUFQE flags in UBUFQE documentation.
	FOr example, you can set UBUFQEFLAG_SYNC flag if you want to receive
	callback after all data has been moved from the hardware to the host.

Returns:
	USBPUMP_PROTO_VSC2_STATUS_OK if sent control reply successfully;
	Possible error codes are
		USBPUMP_PROTO_VSC2_STATUS_INVALID_SESSION_HANDLE
		USBPUMP_PROTO_VSC2_STATUS_INVALID_REQUEST

Notes:
	This API can be called on the arbitration context. The caller should
	ensure that pReplyBuffer is same same data buffer as the
	USBPUMP_PROTO_VSC2_SETUP_PROCESS_FN() callback.

*/

__TMS_FNTYPE_DEF(
USBPUMP_PROTO_VSC2_CONTROL_REPLY_CALLBACK_FN,
__TMS_VOID,
	(
	__TMS_VOID *			/* pCallbackCtx */,
	__TMS_USTAT			/* Status */
	));

__TMS_FNTYPE_DEF(							\
USBPUMP_PROTO_VSC2_CONTROL_REPLY_ASYNC_FN,				\
__TMS_USBPUMP_PROTO_VSC2_STATUS,					\
	(								\
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,		\
	__TMS_VOID *			/* pReplyBuffer */,		\
	__TMS_UINT16			/* nReplyBuffer */,		\
	__TMS_UBUFQE_FLAGS		/* QeFlags */,			\
	__TMS_USBPUMP_PROTO_VSC2_CONTROL_REPLY_CALLBACK_FN * /* pCallbackFn */,\
	__TMS_VOID *			/* pCallbackCtx */		\
	));


/*

Type:  USBPUMP_PROTO_VSC2_SUBMIT_REQUEST_FN

Function:
	Submit data transfer request.

Definition:
	typedef USBPUMP_PROTO_VSC2_STATUS
	(USBPUMP_PROTO_VSC2_SUBMIT_REQUEST_FN)(
		USBPUMP_SESSION_HANDLE		SessionHandle,
		USBPUMP_PROTO_VSC2_REQUEST *	pRequest
		);

Description:
	This is the VSC protocol driver in-call function.  It will be called by
	the client to submit data transfer request to the VSC protocol driver.

	The client should provide pRequest->Qe.uqe_donefn. This API always calls
	pRequest->Qe.uqe_donefn function when either the request complete or an
	error happens.  If this API returns USBPUMP_PROTO_VSC2_STATUS_OK, then
	client should wait pRequest->Qe.uqe_donefn to be called. If this API
	returns not USBPUMP_PROTO_VSC2_STATUS_OK, pRequest->Qe.uqe_donefn will
	not be called.

Returns:
	USBPUMP_PROTO_VSC2_STATUS_OK if submit request successfully.
	Possible error codes are
		USBPUMP_PROTO_VSC2_STATUS_INVALID_SESSION_HANDLE
		USBPUMP_PROTO_VSC2_STATUS_INVALID_STREAM_HANDLE
		USBPUMP_PROTO_VSC2_STATUS_INVALID_PARAMETER
		USBPUMP_PROTO_VSC2_STATUS_NOT_CONFIGURED

Notes:
	This API can be called on the arbitration context. In general, if there
	is no input parameter error, pRequest->Qe.uqe_donefn will be called
	from the DataPump context.

*/

__TMS_FNTYPE_DEF(
USBPUMP_PROTO_VSC2_SUBMIT_REQUEST_FN,
__TMS_USBPUMP_PROTO_VSC2_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,
	__TMS_USBPUMP_PROTO_VSC2_REQUEST *	/* prequest */
	));


/****************************************************************************\
|
|	USBPUMP_PROTO_VSC2_INCALL structure
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_PROTO_VSC2_INCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_PROTO_VSC2_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_API_INCALL_HEADER;

	/* VSC protocol in-calls */
	__TMS_USBPUMP_PROTO_VSC2_OPEN_STREAM_FN *	pOpenStreamFn;
	__TMS_USBPUMP_PROTO_VSC2_CLOSE_STREAM_FN *	pCloseStreamFn;
	__TMS_USBPUMP_PROTO_VSC2_CANCEL_STREAM_FN *	pCancelStreamFn;
	__TMS_USBPUMP_PROTO_VSC2_GET_STREAM_INFO_FN *	pGetStreamInfoFn;
	__TMS_USBPUMP_PROTO_VSC2_CONTROL_REPLY_FN *	pControlReplyFn;
	__TMS_USBPUMP_PROTO_VSC2_CONTROL_REPLY_ASYNC_FN *pControlReplyAsyncFn;
	__TMS_USBPUMP_PROTO_VSC2_SUBMIT_REQUEST_FN *	pSubmitRequestFn;

	__TMS_USBPUMP_API_INCALL_TRAILER;
	};

/* __TMS_TYPE_DEF_UNION	(USBPUMP_PROTO_VSC2_INCALL); */
union __TMS_UNIONNAME	(USBPUMP_PROTO_VSC2_INCALL)
	{
	__TMS_USBPUMP_PROTO_VSC2_INCALL_CONTENTS	Vsc;
	__TMS_USBPUMP_API_INCALL_CONTENTS__UNION;
	};


#define	__TMS_USBPUMP_PROTO_VSC2_INCALL_INIT_V4(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pGetStreamInfoFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)								\
	{{								\
	__TMS_USBPUMP_API_INCALL_HEADER_INIT_V1(			\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,			\
		sizeof(__TMS_USBPUMP_PROTO_VSC2_INCALL),		\
		a_pCloseFn						\
		),							\
	(a_pOpenStreamFn),						\
	(a_pCloseStreamFn),						\
	(a_pCancelStreamFn),						\
	(a_pGetStreamInfoFn),						\
	(a_pControlReplyFn),						\
	(a_pControlReplyAsyncFn),					\
	(a_pSubmitRequestFn),						\
	__TMS_USBPUMP_API_INCALL_TRAILER_INIT_V1(			\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_PROTO_VSC2_INCALL_INIT_V3(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)								\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_INIT_V4(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	/* V4: pGetStreamInfoFn */ __TMS_NULL,				\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_INCALL_INIT_V2(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)								\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_INIT_V3(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	/* V3: pCancelStreamFn */ __TMS_NULL,				\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pSubmitRequestFn						\
	)								\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_INIT_V2(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	/* V2: pControlReplyAsyncFn */ __TMS_NULL,			\
	a_pSubmitRequestFn						\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_INCALL_SETUP_V4(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pGetStreamInfoFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)								\
    do	{								\
	__TMS_USBPUMP_API_INCALL_HEADER_SETUP_V1(			\
		a_pInCall,						\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,			\
		sizeof(*(a_pInCall)),					\
		a_pCloseFn						\
		);							\
	(a_pInCall)->Vsc.pOpenStreamFn = (a_pOpenStreamFn);		\
	(a_pInCall)->Vsc.pCloseStreamFn = (a_pCloseStreamFn);		\
	(a_pInCall)->Vsc.pCancelStreamFn = (a_pCancelStreamFn);		\
	(a_pInCall)->Vsc.pGetStreamInfoFn = (a_pGetStreamInfoFn);	\
	(a_pInCall)->Vsc.pControlReplyFn = (a_pControlReplyFn);		\
	(a_pInCall)->Vsc.pControlReplyAsyncFn = (a_pControlReplyAsyncFn); \
	(a_pInCall)->Vsc.pSubmitRequestFn = (a_pSubmitRequestFn);	\
	__TMS_USBPUMP_API_INCALL_TRAILER_SETUP_V1(			\
		a_pInCall,						\
		Vsc,							\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		);							\
	} while (0)

#define	__TMS_USBPUMP_PROTO_VSC2_INCALL_SETUP_V3(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)								\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_SETUP_V4(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	/* V4: pGetStreamInfoFn */ __TMS_NULL,				\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_INCALL_SETUP_V2(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)								\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_SETUP_V3(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	/* V3: pCancelStreamFn */ __TMS_NULL,				\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pSubmitRequestFn						\
	)								\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_SETUP_V2(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	/* V2: pControlReplyAsyncFn */ __TMS_NULL,			\
	a_pSubmitRequestFn						\
	)


/****************************************************************************\
|
|	VSC protocol driver out-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_PROTO_VSC2_EVENT_FN

Function:
	Deliver a VSC protocol event to the registered client

Definition:
	typedef VOID
	(*USBPUMP_PROTO_VSC2_EVENT_FN)(
		VOID *				ClientHandle,
		USBPUMP_PROTO_VSC2_EVENT	Event,
		CONST VOID *			pEventInfo
		);

Description:
	This is the VSC protocol driver out-call function.  It will be called
	by the VSC protocol driver to deliver a VSC protocol interface event
	to the registered client.

Returns:
	No explicit result.

Notes:
	This functoin will be called from the DataPump context.

*/

__TMS_FNTYPE_DEF(
USBPUMP_PROTO_VSC2_EVENT_FN,
__TMS_VOID,
	(
	__TMS_VOID *			/* ClientHandle */,
	__TMS_USBPUMP_PROTO_VSC2_EVENT	/* Event */,
	__TMS_CONST __TMS_VOID *	/* pEventInfo */
	));

#define	__TMS_USBPUMP_PROTO_VSC2_EVENT_INTERFACE_UP	0
#define	__TMS_USBPUMP_PROTO_VSC2_EVENT_INTERFACE_DOWN	1
#define	__TMS_USBPUMP_PROTO_VSC2_EVENT_SUSPEND		2
#define	__TMS_USBPUMP_PROTO_VSC2_EVENT_RESUME		3
#define	__TMS_USBPUMP_PROTO_VSC2_EVENT_RESET		4
#define	__TMS_USBPUMP_PROTO_VSC2_EVENT_ATTACH		5
#define	__TMS_USBPUMP_PROTO_VSC2_EVENT_DETACH		6

#define	__TMS_USBPUMP_PROTO_VSC2_EVENT__INIT		\
	{						\
	"VSC2_EVENT_INTERFACE_UP",			\
	"VSC2_EVENT_INTERFACE_DOWN",			\
	"VSC2_EVENT_SUSPEND",				\
	"VSC2_EVENT_RESUME",				\
	"VSC2_EVENT_RESET",				\
	"VSC2_EVENT_ATTACH",				\
	"VSC2_EVENT_DETACH"				\
	}

__TMS_TYPE_DEF_STRUCT	(USBPUMP_PROTO_VSC2_EVENT_INTERFACE_UP_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_PROTO_VSC2_EVENT_INTERFACE_UP_INFO)
	{
	__TMS_UINT		CurrentAltSetting;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_PROTO_VSC2_EVENT_SUSPEND_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_PROTO_VSC2_EVENT_SUSPEND_INFO)
	{
	__TMS_BOOL		fRemoteWakeEnabled;
	};


/*

Type:  USBPUMP_PROTO_VSC2_SETUP_VALIDATE_FN

Function:
	Validate a vendor specific class control request.

Definition:
	typedef USBPUMP_PROTO_VSC2_SETUP_STATUS
	(*USBPUMP_PROTO_VSC2_SETUP_VALIDATE_FN)(
		VOID *		ClientHandle,
		CONST USETUP *	pSetup
		);

Description:
	This is the VSC protocol driver out-call function. It will be called by
	the VSC protocol driver when it receive vendor specific class request
	from host.  The client providing USBPUMP_PROTO_VSC2_SETUP_VALIDATE_FN()
	should validate the vendor specific class control request.

	If the client can accept this control request, the client should return
	USBPUMP_PROTO_VSC2_SETUP_STATUS_ACCEPTED.  If this control request is
	unknown (the client can't accept this control request), the client
	should return USBPUMP_PROTO_VSC2_SETUP_STATUS_NOT_CLAIMED. If the client
	recognizes this request but wants to reject this request for some
	reason, the client should return USBPUMP_PROTO_VSC2_SETUP_STATUS_
	REJECTED.

	If the client accepted this vendor specific control request, then the
	client will subsequently receive a USBPUMP_PROTO_VSC2_SETUP_PROCESS_FN()
	callback to process the accepted control request.

	If the client returns USBPUMP_PROTO_VSC2_SETUP_STATUS_REJECTED, the VSC
	protocol will send STALL.

	If the client returns USBPUMP_PROTO_VSC2_SETUP_STATUS_NOT_CLAIMED, the
	VSC protocol do nothing for this vendor specific command and DataPump
	core will handle this command.

Returns:
	USBPUMP_PROTO_VSC2_SETUP_STATUS

Notes:
	This functoin will be called on the DataPump context.

*/

/* Definition of USBPUMP_PROTO_VSC2_SETUP_STATUS */
#define	__TMS_USBPUMP_PROTO_VSC2_SETUP_STATUS_ACCEPTED		0
#define	__TMS_USBPUMP_PROTO_VSC2_SETUP_STATUS_REJECTED		1
#define	__TMS_USBPUMP_PROTO_VSC2_SETUP_STATUS_NOT_CLAIMED	2

__TMS_FNTYPE_DEF(
USBPUMP_PROTO_VSC2_SETUP_VALIDATE_FN,
__TMS_USBPUMP_PROTO_VSC2_SETUP_STATUS,
	(
	__TMS_VOID *			/* ClientHandle */,
	__TMS_CONST __TMS_USETUP *	/* pSetup */
	));


/*

Type:  USBPUMP_PROTO_VSC2_SETUP_PROCESS_FN

Function:
	Process a vendor specific class control request.

Definition:
	typedef BOOL
	(*USBPUMP_PROTO_VSC2_SETUP_PROCESS_FN)(
		VOID *			ClientHandle,
		CONST USETUP *		pSetup,
		VOID *			pBuffer,
		UINT16			nBuffer
		);

Description:
	This is the VSC protocol driver out-call function. It will be called by
	the VSC protocol driver to let client process vendor specific control
	request.

	If direction of vendor request is from host to device, the VSC protocol
	receives the data from the host, passing the received data in pBuffer,
	with the count of valid data indicated by nBuffer.  The client should
	sends its reply using USBPUMP_PROTO_VSC2_CONTROL_REPLY_FN().

	If direction of vendor request is from device to host, the VSC protocol
	provides a data buffer pBuffer of size nBuffer.  The client processes
	the control request and copies the resulting data to the pBuffer,
	setting nBuffer to the actual count of valid response data bytes.  Then
	the client sends the reply using USBPUMP_PROTO_VSC2_CONTROL_REPLY_FN().

	If this process function returns FALSE, the VSC protocol driver will
	send STALL for this setup packet.  If it returns TRUE, this indicates
	the client has handled this setup packet.

Returns:
	TRUE if the client has handled this setup packet.  FALSE if the client
	doesn't want to handle this setup packet and want to send STALL.

Notes:
	This functoin will be called from the DataPump context.

*/

__TMS_FNTYPE_DEF(							\
USBPUMP_PROTO_VSC2_SETUP_PROCESS_FN,					\
__TMS_BOOL,								\
	(								\
	__TMS_VOID *			/* ClientHandle */,		\
	__TMS_CONST __TMS_USETUP *	/* pSetup */,			\
	__TMS_VOID *			/* pBuffer */,			\
	__TMS_UINT16			/* nBuffer */			\
	));


/****************************************************************************\
|
|	USBPUMP_PROTO_VSC2_OUTCALL structure
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_PROTO_VSC2_OUTCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_PROTO_VSC2_OUTCALL_CONTENTS)
	{
	__TMS_USBPUMP_API_OUTCALL_HEADER;

	__TMS_USBPUMP_PROTO_VSC2_EVENT_FN *		pEventFn;
	__TMS_USBPUMP_PROTO_VSC2_SETUP_VALIDATE_FN *	pSetupValidateFn;
	__TMS_USBPUMP_PROTO_VSC2_SETUP_PROCESS_FN *	pSetupProcessFn;
	__TMS_USBPUMP_OBJECT_IOCTL_FN *			pIoctlFn;

	__TMS_USBPUMP_API_OUTCALL_TRAILER;
	};

/* __TMS_TYPE_DEF_UNION	(USBPUMP_PROTO_VSC2_OUTCALL); */
union __TMS_UNIONNAME	(USBPUMP_PROTO_VSC2_OUTCALL)
	{
	__TMS_USBPUMP_PROTO_VSC2_OUTCALL_CONTENTS	Vsc;
	__TMS_USBPUMP_API_OUTCALL_CONTENTS__UNION;
	};


#define	__TMS_USBPUMP_PROTO_VSC2_OUTCALL_INIT_V2(			\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn,						\
	a_pIoctlFn							\
	)								\
	{{								\
	__TMS_USBPUMP_API_OUTCALL_HEADER_INIT_V1(			\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN,			\
		sizeof(__TMS_USBPUMP_PROTO_VSC2_OUTCALL)		\
		),							\
	(a_pEventFn),							\
	(a_pSetupValidateFn),						\
	(a_pSetupProcessFn),						\
	(a_pIoctlFn),							\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_INIT_V1(			\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_PROTO_VSC2_OUTCALL_INIT_V1(			\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn						\
	)								\
	__TMS_USBPUMP_PROTO_VSC2_OUTCALL_INIT_V2(			\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn,						\
	/* V2: pIoctlFn */ __TMS_NULL					\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_OUTCALL_SETUP_V2(			\
	a_pOutCall,							\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn,						\
	a_pIoctlFn							\
	)								\
    do	{								\
	__TMS_USBPUMP_API_OUTCALL_HEADER_SETUP_V1(			\
		a_pOutCall,						\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN,			\
		sizeof(*(a_pOutCall))					\
		);							\
	(a_pOutCall)->Vsc.pEventFn = (a_pEventFn);			\
	(a_pOutCall)->Vsc.pSetupValidateFn = (a_pSetupValidateFn);	\
	(a_pOutCall)->Vsc.pSetupProcessFn = (a_pSetupProcessFn);	\
	(a_pOutCall)->Vsc.pIoctlFn = (a_pIoctlFn);			\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_SETUP_V1(			\
		a_pOutCall,						\
		Vsc,							\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_END			\
		);							\
	} while (0)

#define	__TMS_USBPUMP_PROTO_VSC2_OUTCALL_SETUP_V1(			\
	a_pOutCall,							\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn						\
	)								\
	__TMS_USBPUMP_PROTO_VSC2_OUTCALL_SETUP_V2(			\
	a_pOutCall,							\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn,						\
	/* V2: pIoctlFn */ __TMS_NULL					\
	)


/****************************************************************************\
|
|	USBPUMP_PROTO_VSC2_STATUS Codes
|
\****************************************************************************/

#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_OK			\
	__TMS_USBPUMP_API_STATUS_OK
#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_INVALID_PARAMETER	\
	__TMS_USBPUMP_API_STATUS_INVALID_PARAMETER
#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_ARG_AREA_TOO_SMALL	\
	__TMS_USBPUMP_API_STATUS_ARG_AREA_TOO_SMALL
#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_BUFFER_TOO_SMALL	\
	__TMS_USBPUMP_API_STATUS_BUFFER_TOO_SMALL
#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_NOT_SUPPORTED		\
	__TMS_USBPUMP_API_STATUS_NOT_SUPPORTED
#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_NO_MORE_SESSIONS	\
	__TMS_USBPUMP_API_STATUS_NO_MORE_SESSIONS
#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_INVALID_SESSION_HANDLE	\
	__TMS_USBPUMP_API_STATUS_INVALID_SESSION_HANDLE
#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_UNSUCCESSFUL		\
	__TMS_USBPUMP_API_STATUS_UNSUCCESSFUL

#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_INVALID_STREAM_HANDLE	\
	(__TMS_USBPUMP_API_STATUS__MAX + 0)
#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_ALREADY_OPENED		\
	(__TMS_USBPUMP_API_STATUS__MAX + 1)
#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_NO_STREAM_TO_OPEN	\
	(__TMS_USBPUMP_API_STATUS__MAX + 2)
#define __TMS_USBPUMP_PROTO_VSC2_STATUS_INVALID_REQUEST		\
	(__TMS_USBPUMP_API_STATUS__MAX + 3)
#define __TMS_USBPUMP_PROTO_VSC2_STATUS_NOT_CONFIGURED		\
	(__TMS_USBPUMP_API_STATUS__MAX + 4)


#define	__TMS_USBPUMP_PROTO_VSC2_STATUS__INIT		\
	{						\
	"VSC2_STATUS_OK",				\
	"VSC2_STATUS_INVALID_PARAMETER",		\
	"VSC2_STATUS_ARG_AREA_TOO_SMALL",		\
	"VSC2_STATUS_BUFFER_TOO_SMAL",			\
	"VSC2_STATUS_NOT_SUPPORTED",			\
	"VSC2_STATUS_NO_MORE_SESSIONS",			\
	"VSC2_STATUS_INVALID_SESSION_HANDLE",		\
	"VSC2_STATUS_UNSUCCESSFUL"			\
	}

#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_ERROR__INIT	\
	{						\
	"VSC2_STATUS_INVALID_STREAM_HANDLE",		\
	"VSC2_STATUS_ALREADY_OPENED",			\
	"VSC2_STATUS_NO_STREAM_TO_OPEN",		\
	"VSC2_STATUS_INVALID_REQUEST",			\
	"VSC2_STATUS_NOT_CONFIGURED"			\
	}


#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_SUCCESS(Status)	\
	((Status) == __TMS_USBPUMP_PROTO_VSC2_STATUS_OK)

#define	__TMS_USBPUMP_PROTO_VSC2_STATUS_FAILED(Status)	\
	((Status) != __TMS_USBPUMP_PROTO_VSC2_STATUS_OK)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_CONST __TMS_USBPUMP_PROTO_VSC2_INCALL
gk_UsbPumpVsc2_InCallAsyncDelay;

extern __TMS_CONST __TMS_USBPUMP_PROTO_VSC2_INCALL
gk_UsbPumpVsc2_InCallBasic;

extern __TMS_CONST __TMS_USBPUMP_PROTO_VSC2_INCALL
gk_UsbPumpVsc2_InCallDefault;

extern __TMS_CONST __TMS_USBPUMP_PROTO_VSC2_INCALL
gk_UsbPumpVsc2_InCallFull;

extern __TMS_CONST __TMS_USBPUMP_PROTO_VSC2_INCALL
gk_UsbPumpVsc2_InCallGetStreamInfo;

__TMS_USBPUMP_API_CLOSE_FN
UsbPumpVsc2_Close;

__TMS_USBPUMP_PROTO_VSC2_OPEN_STREAM_FN
UsbPumpVsc2_OpenStream;

__TMS_USBPUMP_PROTO_VSC2_CLOSE_STREAM_FN
UsbPumpVsc2_CloseStream;

__TMS_USBPUMP_PROTO_VSC2_CANCEL_STREAM_FN
UsbPumpVsc2_CancelStream;

__TMS_USBPUMP_PROTO_VSC2_GET_STREAM_INFO_FN
UsbPumpVsc2_GetStreamInfo;

__TMS_USBPUMP_PROTO_VSC2_GET_STREAM_INFO_FN
UsbPumpVsc2_GetStreamInfoDummy;

__TMS_USBPUMP_PROTO_VSC2_CONTROL_REPLY_FN
UsbPumpVsc2_ControlReply;

__TMS_USBPUMP_PROTO_VSC2_CONTROL_REPLY_ASYNC_FN
UsbPumpVsc2_ControlReplyAsync;

__TMS_USBPUMP_PROTO_VSC2_CONTROL_REPLY_ASYNC_FN
UsbPumpVsc2_ControlReplyAsyncDelay;

__TMS_USBPUMP_PROTO_VSC2_CONTROL_REPLY_ASYNC_FN
UsbPumpVsc2_ControlReplyAsyncDummy;

__TMS_USBPUMP_PROTO_VSC2_SUBMIT_REQUEST_FN
UsbPumpVsc2_SubmitRequest;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_PROTO_VSC2_INCALL_INIT_V4(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pGetStreamInfoFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_INIT_V4(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pGetStreamInfoFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_PROTO_VSC2_INCALL_INIT_V3(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_INIT_V3(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_PROTO_VSC2_INCALL_INIT_V2(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_INIT_V2(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_PROTO_VSC2_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_PROTO_VSC2_INCALL_SETUP_V4(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pGetStreamInfoFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_SETUP_V4(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pGetStreamInfoFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_PROTO_VSC2_INCALL_SETUP_V3(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_SETUP_V3(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pCancelStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_PROTO_VSC2_INCALL_SETUP_V2(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_SETUP_V2(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pControlReplyAsyncFn,						\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_PROTO_VSC2_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenStreamFn,						\
	a_pCloseStreamFn,						\
	a_pControlReplyFn,						\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_PROTO_VSC2_EVENT_INTERFACE_UP	\
   __TMS_USBPUMP_PROTO_VSC2_EVENT_INTERFACE_UP
# define USBPUMP_PROTO_VSC2_EVENT_INTERFACE_DOWN	\
   __TMS_USBPUMP_PROTO_VSC2_EVENT_INTERFACE_DOWN
# define USBPUMP_PROTO_VSC2_EVENT_SUSPEND	\
   __TMS_USBPUMP_PROTO_VSC2_EVENT_SUSPEND
# define USBPUMP_PROTO_VSC2_EVENT_RESUME	\
   __TMS_USBPUMP_PROTO_VSC2_EVENT_RESUME
# define USBPUMP_PROTO_VSC2_EVENT_RESET	\
   __TMS_USBPUMP_PROTO_VSC2_EVENT_RESET
# define USBPUMP_PROTO_VSC2_EVENT_ATTACH	\
   __TMS_USBPUMP_PROTO_VSC2_EVENT_ATTACH
# define USBPUMP_PROTO_VSC2_EVENT_DETACH	\
   __TMS_USBPUMP_PROTO_VSC2_EVENT_DETACH
# define USBPUMP_PROTO_VSC2_EVENT__INIT	\
   __TMS_USBPUMP_PROTO_VSC2_EVENT__INIT
# define USBPUMP_PROTO_VSC2_SETUP_STATUS_ACCEPTED	\
   __TMS_USBPUMP_PROTO_VSC2_SETUP_STATUS_ACCEPTED
# define USBPUMP_PROTO_VSC2_SETUP_STATUS_REJECTED	\
   __TMS_USBPUMP_PROTO_VSC2_SETUP_STATUS_REJECTED
# define USBPUMP_PROTO_VSC2_SETUP_STATUS_NOT_CLAIMED	\
   __TMS_USBPUMP_PROTO_VSC2_SETUP_STATUS_NOT_CLAIMED
# define USBPUMP_PROTO_VSC2_OUTCALL_INIT_V2(			\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn,						\
	a_pIoctlFn							\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_OUTCALL_INIT_V2(			\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn,						\
	a_pIoctlFn							\
	)
# define USBPUMP_PROTO_VSC2_OUTCALL_INIT_V1(			\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_OUTCALL_INIT_V1(			\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn						\
	)
# define USBPUMP_PROTO_VSC2_OUTCALL_SETUP_V2(			\
	a_pOutCall,							\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn,						\
	a_pIoctlFn							\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_OUTCALL_SETUP_V2(			\
	a_pOutCall,							\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn,						\
	a_pIoctlFn							\
	)
# define USBPUMP_PROTO_VSC2_OUTCALL_SETUP_V1(			\
	a_pOutCall,							\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_OUTCALL_SETUP_V1(			\
	a_pOutCall,							\
	a_pEventFn,							\
	a_pSetupValidateFn,						\
	a_pSetupProcessFn						\
	)
# define USBPUMP_PROTO_VSC2_STATUS_OK	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_OK
# define USBPUMP_PROTO_VSC2_STATUS_INVALID_PARAMETER	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_INVALID_PARAMETER
# define USBPUMP_PROTO_VSC2_STATUS_ARG_AREA_TOO_SMALL	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_ARG_AREA_TOO_SMALL
# define USBPUMP_PROTO_VSC2_STATUS_BUFFER_TOO_SMALL	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_BUFFER_TOO_SMALL
# define USBPUMP_PROTO_VSC2_STATUS_NOT_SUPPORTED	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_NOT_SUPPORTED
# define USBPUMP_PROTO_VSC2_STATUS_NO_MORE_SESSIONS	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_NO_MORE_SESSIONS
# define USBPUMP_PROTO_VSC2_STATUS_INVALID_SESSION_HANDLE	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_INVALID_SESSION_HANDLE
# define USBPUMP_PROTO_VSC2_STATUS_UNSUCCESSFUL	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_UNSUCCESSFUL
# define USBPUMP_PROTO_VSC2_STATUS_INVALID_STREAM_HANDLE	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_INVALID_STREAM_HANDLE
# define USBPUMP_PROTO_VSC2_STATUS_ALREADY_OPENED	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_ALREADY_OPENED
# define USBPUMP_PROTO_VSC2_STATUS_NO_STREAM_TO_OPEN	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_NO_STREAM_TO_OPEN
# define USBPUMP_PROTO_VSC2_STATUS_INVALID_REQUEST	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_INVALID_REQUEST
# define USBPUMP_PROTO_VSC2_STATUS_NOT_CONFIGURED	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_NOT_CONFIGURED
# define USBPUMP_PROTO_VSC2_STATUS__INIT	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS__INIT
# define USBPUMP_PROTO_VSC2_STATUS_ERROR__INIT	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_ERROR__INIT
# define USBPUMP_PROTO_VSC2_STATUS_SUCCESS(Status)	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_SUCCESS(Status)
# define USBPUMP_PROTO_VSC2_STATUS_FAILED(Status)	\
   __TMS_USBPUMP_PROTO_VSC2_STATUS_FAILED(Status)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_proto_vsc2_api.h ****/
#endif /* _USBPUMP_PROTO_VSC2_API_H_ */
