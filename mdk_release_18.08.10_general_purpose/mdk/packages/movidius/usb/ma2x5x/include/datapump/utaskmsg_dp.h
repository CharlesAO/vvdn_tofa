/* utaskmsg_dp.h	Fri Apr 04 2008 10:02:45 chwon */

/*

Module:  utaskmsg_dp.h

Function:
	Message-code definitions for core functions in DataPump

Version:
	V1.97k	Fri Apr 04 2008 10:02:45 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2005-2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Mats Webjorn, MCCI Corporation	March 2005

Revision history:
   1.91b  Tue Mar 22 2005 08:11:11 maw
	Module created.

   1.91c  Tue Apr 26 2005 10:13:52 maw
   	Added Status Phase defs from usbtaskmsg_sic.h

   1.91i  Tue Sep 27 2005 09:56:24 maw
   	 Added message to get DataPump serial number
	
   1.93a  Mon Oct 17 2005 23:35:51 maw
   	 Added async request/reply message-codes to get 
   	 DataPump serial number

   1.97k  Fri Apr 04 2008 10:02:45  chwon
	3981: add UPROCESS method function prototype. Generate uncloaked name.

*/

#ifndef _UTASKMSG_DP_H_		/* prevent multiple includes */
#define _UTASKMSG_DP_H_

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _UTASKMSG_CODES_H_
# include "utaskmsg_codes.h"
#endif

#ifndef _UTASKMSG_SERVICES_H_
# include "utaskmsg_services.h"
#endif

#ifndef _UTASK_H_
# include "utask.h"
#endif

/****************************************************************************\
|
|	DataPump core message code definitions
|
\****************************************************************************/

#define	__TMS_UTASK_MC_INDEX_DPCORE_OPEN			0
#define	__TMS_UTASK_MC_INDEX_DPCORE_CLOSE			1
#define	__TMS_UTASK_MC_INDEX_DPCORE_STATUS_PHASE		2
#define	__TMS_UTASK_MC_INDEX_DPCORE_GET_SERIALNO_LENGTH		3
#define	__TMS_UTASK_MC_INDEX_DPCORE_GET_SERIALNO		4

#define __TMS_UTASK_MSGCODE_DPCORE_REQUEST(Index,nBuf)					\
	__TMS_CONCAT(__TMS_UTASK_MC_, REQ)(						\
		__TMS_UTASK_MC_SERVICE_DPCORE,						\
		__TMS_CONCAT(__TMS_UTASK_MC_INDEX_DPCORE_, Index),			\
		(sizeof(__TMS_CONCAT3(__TMS_UTASK_MSG_DPCORE_, Index, _REQUEST))),	\
		(nBuf)									\
		)

#define __TMS_UTASK_MSGCODE_DPCORE_REQUEST_ASYNC(Index,nBuf)				\
	__TMS_CONCAT(__TMS_UTASK_MC_, REQ_ASYNC)(					\
		__TMS_UTASK_MC_SERVICE_DPCORE,						\
		__TMS_CONCAT(__TMS_UTASK_MC_INDEX_DPCORE_, Index),			\
		(sizeof(__TMS_CONCAT3(__TMS_UTASK_MSG_DPCORE_, Index, _REQUEST))),	\
		(nBuf)									\
		)

#define __TMS_UTASK_MSGCODE_DPCORE_REPLY(Index,nBuf)					\
	__TMS_CONCAT(__TMS_UTASK_MC_, REP)(						\
		__TMS_UTASK_MC_SERVICE_DPCORE,						\
		__TMS_CONCAT(__TMS_UTASK_MC_INDEX_DPCORE_, Index),			\
		(sizeof(__TMS_CONCAT3(__TMS_UTASK_MSG_DPCORE_, Index, _REPLY))),	\
		(nBuf)									\
		)

#define __TMS_UTASK_MSGCODE_DPCORE_REPLY_ASYNC(Index,nBuf)				\
	__TMS_CONCAT(__TMS_UTASK_MC_, REP_ASYNC)(					\
		__TMS_UTASK_MC_SERVICE_DPCORE,						\
		__TMS_CONCAT(__TMS_UTASK_MC_INDEX_DPCORE_, Index),			\
		(sizeof(__TMS_CONCAT3(__TMS_UTASK_MSG_DPCORE_, Index, _REPLY))),	\
		(nBuf)									\
		)

#define __TMS_UTASK_MSGCODE_DPCORE_INDICATION(Index)					\
	__TMS_CONCAT(__TMS_UTASK_MC_, IND)(						\
		__TMS_UTASK_MC_SERVICE_DPCORE,						\
		__TMS_CONCAT(__TMS_UTASK_MC_INDEX_DPCORE_, Index),			\
		(sizeof(__TMS_CONCAT3(__TMS_UTASK_MSG_DPCORE_, Index, _INDICATION)))	\
		)

#define	__TMS_UTASK_MSGCODE_DPCORE_OPEN_REQUEST		\
	__TMS_UTASK_MSGCODE_DPCORE_REQUEST(OPEN, 1)
		
#define	__TMS_UTASK_MSGCODE_DPCORE_OPEN_REPLY		\
	__TMS_UTASK_MSGCODE_DPCORE_REPLY(OPEN, 1)

#define	__TMS_UTASK_MSGCODE_DPCORE_CLOSE_REQUEST	\
	__TMS_UTASK_MSGCODE_DPCORE_REQUEST(CLOSE, 0)
		
#define	__TMS_UTASK_MSGCODE_DPCORE_CLOSE_REPLY	\
	__TMS_UTASK_MSGCODE_DPCORE_REPLY(CLOSE, 0)

#define	__TMS_UTASK_MSGCODE_DPCORE_STATUS_PHASE_INDICATION	\
	__TMS_UTASK_MSGCODE_DPCORE_INDICATION(STATUS_PHASE)

#define	__TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REQUEST	\
	__TMS_UTASK_MSGCODE_DPCORE_REQUEST(GET_SERIALNO_LENGTH, 0)
		
#define	__TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REPLY	\
	__TMS_UTASK_MSGCODE_DPCORE_REPLY(GET_SERIALNO_LENGTH, 0)

#define	__TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REQUEST_ASYNC	\
	__TMS_UTASK_MSGCODE_DPCORE_REQUEST_ASYNC(GET_SERIALNO_LENGTH, 0)
		
#define	__TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REPLY_ASYNC	\
	__TMS_UTASK_MSGCODE_DPCORE_REPLY_ASYNC(GET_SERIALNO_LENGTH, 0)

#define	__TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_REQUEST	\
	__TMS_UTASK_MSGCODE_DPCORE_REQUEST(GET_SERIALNO, 1)
		
#define	__TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_REPLY	\
	__TMS_UTASK_MSGCODE_DPCORE_REPLY(GET_SERIALNO, 1)

#define	__TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_REQUEST_ASYNC	\
	__TMS_UTASK_MSGCODE_DPCORE_REQUEST_ASYNC(GET_SERIALNO, 1)
		
#define	__TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_REPLY_ASYNC	\
	__TMS_UTASK_MSGCODE_DPCORE_REPLY_ASYNC(GET_SERIALNO, 1)


/****************************************************************************\
|
|	DataPump message header definitions
|
\****************************************************************************/

/*
|| Define a generic header for all messages send to DataPump objects according
|| to description in usbpump.h
*/
__TMS_TYPE_DEF_STRUCT(UTASK_DPMSG);
__TMS_TYPE_DEF_STRUCT(UTASK_DPMSG_HDR_STRUCT);

struct __TMS_STRUCTNAME(UTASK_DPMSG_HDR_STRUCT)
	{
	__TMS_USBPUMP_OBJECT_HANDLE	hObject;
	};

#define __TMS_UTASK_DPMSG_HDR	__TMS_UTASK_DPMSG_HDR_STRUCT DpHdr

struct __TMS_STRUCTNAME(UTASK_DPMSG)
	{
	__TMS_UTASK_DPMSG_HDR;			/* the normal header */
	__TMS_UINT8			ClientData[1];	/* the data payload */
	};

/**** sane names ****/
#define	dpm_hObject			DpHdr.hObject

/****************************************************************************\
|
|	DataPump core message structure definitions
|
\****************************************************************************/

/*
|| Open object message
||
|| NOTE: Object name is suppled as an attached buffer
*/
__TMS_TYPE_DEF_STRUCT(  UTASK_MSG_DPCORE_OPEN_REQUEST);
struct __TMS_STRUCTNAME(UTASK_MSG_DPCORE_OPEN_REQUEST)
	{
	__TMS_ULONG			InstanceNumber;
	};

__TMS_TYPE_DEF_STRUCT(  UTASK_MSG_DPCORE_OPEN_REPLY);
struct __TMS_STRUCTNAME(UTASK_MSG_DPCORE_OPEN_REPLY)
	{
	__TMS_USBPUMP_OBJECT_HANDLE	hObject;
	__TMS_UTASK_STAT		Status;
	};

__TMS_TYPE_DEF_STRUCT(  UTASK_MSG_DPCORE_CLOSE_REQUEST);
struct __TMS_STRUCTNAME(UTASK_MSG_DPCORE_CLOSE_REQUEST)
	{
	__TMS_USBPUMP_OBJECT_HANDLE	hObject;
	};

__TMS_TYPE_DEF_STRUCT(  UTASK_MSG_DPCORE_CLOSE_REPLY);
struct __TMS_STRUCTNAME(UTASK_MSG_DPCORE_CLOSE_REPLY)
	{
	__TMS_UTASK_STAT		Status;
	};

__TMS_TYPE_DEF_STRUCT(  UTASK_MSG_DPCORE_STATUS_PHASE_INDICATION);
struct __TMS_STRUCTNAME(UTASK_MSG_DPCORE_STATUS_PHASE_INDICATION)
	{
	__TMS_UTASK_DPMSG_HDR;
	__TMS_BOOL			Success;
	__TMS_USETUP_HANDLE		hSetup;
	};

__TMS_TYPE_DEF_STRUCT(  UTASK_MSG_DPCORE_GET_SERIALNO_LENGTH_REQUEST);
struct __TMS_STRUCTNAME(UTASK_MSG_DPCORE_GET_SERIALNO_LENGTH_REQUEST)
	{
	__TMS_UINT			NotUsed;
	};

__TMS_TYPE_DEF_STRUCT(  UTASK_MSG_DPCORE_GET_SERIALNO_LENGTH_REPLY);
struct __TMS_STRUCTNAME(UTASK_MSG_DPCORE_GET_SERIALNO_LENGTH_REPLY)
	{
	__TMS_BYTES			Length;
	__TMS_UTASK_STAT		Status;
	};

__TMS_TYPE_DEF_STRUCT(  UTASK_MSG_DPCORE_GET_SERIALNO_REQUEST);
struct __TMS_STRUCTNAME(UTASK_MSG_DPCORE_GET_SERIALNO_REQUEST)
	{
	__TMS_UINT			NotUsed;
	};

__TMS_TYPE_DEF_STRUCT(  UTASK_MSG_DPCORE_GET_SERIALNO_REPLY);
struct __TMS_STRUCTNAME(UTASK_MSG_DPCORE_GET_SERIALNO_REPLY)
	{
	__TMS_UTASK_STAT		Status;
	};


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BOOL
UsbPumpProcess_CloseObject(
	__TMS_UTASK_PROCESS_HANDLE	hSelf,
	__TMS_UTASK_MAILBOX_HANDLE	hTaskMailbox,
	__TMS_UTASK_PROCESS_INDEX	iTaskProcess,
	__TMS_USBPUMP_OBJECT_HANDLE	hTaskObject
	);

__TMS_BOOL
UsbPumpProcess_OpenObject(
	__TMS_UTASK_PROCESS_HANDLE	hSelf,
	__TMS_UTASK_MAILBOX_HANDLE	hTaskMailbox,
	__TMS_UTASK_PROCESS_INDEX	iTaskProcess,
	__TMS_CONST __TMS_TEXT *	TaskObjectName,
	__TMS_BYTES			nTaskObjectName,
	__TMS_ULONG			InstanceNumber,
	__TMS_USBPUMP_OBJECT_HANDLE *	phTaskObject
	);

__TMS_BOOL
UsbPumpProcess_GetSerialNo(
	__TMS_UTASK_PROCESS_HANDLE	hSelf,
	__TMS_UTASK_MAILBOX_HANDLE	hDpMailbox,
	__TMS_UTASK_PROCESS_INDEX	iDpProcess,
	__TMS_TEXT *			pSerialNoBuf,
	__TMS_BYTES			sizeSerialNoBuf,
	__TMS_BYTES *			pnSerialNo
	);

__TMS_VOID
UsbPumpProcess_ProcessCloseObjectRequest(
	__TMS_UTASK_PROCESS_HANDLE	hProcess,
	__TMS_USBPUMP_OBJECT_HEADER *	pFunctionObject,
	__TMS_UTASK_MSG_HANDLE		hRequest
	);

__TMS_BOOL
UsbPumpProcess_ProcessGetSerialNoLengthReply(
	__TMS_UTASK_PROCESS_HANDLE	hProcess,
	__TMS_UTASK_MSG_HANDLE		hReply,
	__TMS_BYTES *			pnSerialNo
	);

__TMS_BOOL
UsbPumpProcess_ProcessGetSerialNoReply(
	__TMS_UTASK_PROCESS_HANDLE	hProcess,
	__TMS_UTASK_MSG_HANDLE		hReply,
	__TMS_TEXT *			pSerialNoBuf,
	__TMS_BYTES			sizeSerialNoBuf,
	__TMS_BYTES *			pnSerialNo
	);

__TMS_VOID
UsbPumpProcess_ProcessOpenObjectRequest(
	__TMS_UTASK_PROCESS_HANDLE	hProcess,
	__TMS_USBPUMP_OBJECT_HEADER *	pFunctionObject,
	__TMS_UTASK_MSG_HANDLE		hRequest
	);

__TMS_BOOL
UsbPumpProcess_SendGetSerialNoLengthRequest(
	__TMS_UTASK_PROCESS_HANDLE	hSelf,
	__TMS_UTASK_MAILBOX_HANDLE	hDpMailbox,
	__TMS_UTASK_PROCESS_INDEX	iDpProcess
	);

__TMS_BOOL
UsbPumpProcess_SendGetSerialNoRequest(
	__TMS_UTASK_PROCESS_HANDLE	hSelf,
	__TMS_UTASK_MAILBOX_HANDLE	hDpMailbox,
	__TMS_UTASK_PROCESS_INDEX	iDpProcess,
	__TMS_BYTES			sizeSerialNoBuf
	);


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UTASK_MC_INDEX_DPCORE_OPEN	\
   __TMS_UTASK_MC_INDEX_DPCORE_OPEN
# define UTASK_MC_INDEX_DPCORE_CLOSE	\
   __TMS_UTASK_MC_INDEX_DPCORE_CLOSE
# define UTASK_MC_INDEX_DPCORE_STATUS_PHASE	\
   __TMS_UTASK_MC_INDEX_DPCORE_STATUS_PHASE
# define UTASK_MC_INDEX_DPCORE_GET_SERIALNO_LENGTH	\
   __TMS_UTASK_MC_INDEX_DPCORE_GET_SERIALNO_LENGTH
# define UTASK_MC_INDEX_DPCORE_GET_SERIALNO	\
   __TMS_UTASK_MC_INDEX_DPCORE_GET_SERIALNO
# define UTASK_MSGCODE_DPCORE_REQUEST(Index,nBuf)	\
   __TMS_UTASK_MSGCODE_DPCORE_REQUEST(Index,nBuf)
# define UTASK_MSGCODE_DPCORE_REQUEST_ASYNC(Index,nBuf)	\
   __TMS_UTASK_MSGCODE_DPCORE_REQUEST_ASYNC(Index,nBuf)
# define UTASK_MSGCODE_DPCORE_REPLY(Index,nBuf)	\
   __TMS_UTASK_MSGCODE_DPCORE_REPLY(Index,nBuf)
# define UTASK_MSGCODE_DPCORE_REPLY_ASYNC(Index,nBuf)	\
   __TMS_UTASK_MSGCODE_DPCORE_REPLY_ASYNC(Index,nBuf)
# define UTASK_MSGCODE_DPCORE_INDICATION(Index)	\
   __TMS_UTASK_MSGCODE_DPCORE_INDICATION(Index)
# define UTASK_MSGCODE_DPCORE_OPEN_REQUEST	\
   __TMS_UTASK_MSGCODE_DPCORE_OPEN_REQUEST
# define UTASK_MSGCODE_DPCORE_OPEN_REPLY	\
   __TMS_UTASK_MSGCODE_DPCORE_OPEN_REPLY
# define UTASK_MSGCODE_DPCORE_CLOSE_REQUEST	\
   __TMS_UTASK_MSGCODE_DPCORE_CLOSE_REQUEST
# define UTASK_MSGCODE_DPCORE_CLOSE_REPLY	\
   __TMS_UTASK_MSGCODE_DPCORE_CLOSE_REPLY
# define UTASK_MSGCODE_DPCORE_STATUS_PHASE_INDICATION	\
   __TMS_UTASK_MSGCODE_DPCORE_STATUS_PHASE_INDICATION
# define UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REQUEST	\
   __TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REQUEST
# define UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REPLY	\
   __TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REPLY
# define UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REQUEST_ASYNC	\
   __TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REQUEST_ASYNC
# define UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REPLY_ASYNC	\
   __TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_LENGTH_REPLY_ASYNC
# define UTASK_MSGCODE_DPCORE_GET_SERIALNO_REQUEST	\
   __TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_REQUEST
# define UTASK_MSGCODE_DPCORE_GET_SERIALNO_REPLY	\
   __TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_REPLY
# define UTASK_MSGCODE_DPCORE_GET_SERIALNO_REQUEST_ASYNC	\
   __TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_REQUEST_ASYNC
# define UTASK_MSGCODE_DPCORE_GET_SERIALNO_REPLY_ASYNC	\
   __TMS_UTASK_MSGCODE_DPCORE_GET_SERIALNO_REPLY_ASYNC
# define UTASK_DPMSG_HDR	\
   __TMS_UTASK_DPMSG_HDR
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of utaskmsg_dp.h ****/
#endif /* _UTASKMSG_DP_H_ */
