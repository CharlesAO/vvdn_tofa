/* usbpump_usbdi_class_msd_request.h	Thu Dec 04 2014 00:17:52 chwon */

/*

Module:  usbpump_usbdi_class_msd_request.h

Function:
	Definition of USBPUMP_USBDI_CLASS_MSD_REQUEST

Version:
	V3.13b	Thu Dec 04 2014 00:17:52 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2013-2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	August 2013

Revision history:
   3.11d  Thu Aug 15 2013 11:42:07 chwon
	9010: Module created.

   3.13b  Thu Dec 04 2014 00:17:52  chwon
	18701: Add SCSI requests and change private structure.

*/

#ifndef _USBPUMP_USBDI_CLASS_MSD_REQUEST_H_	/* prevent multiple includes */
#define _USBPUMP_USBDI_CLASS_MSD_REQUEST_H_

#ifndef _USBPUMP_USBDI_CLASS_MSD_TYPES_H_
# include "usbpump_usbdi_class_msd_types.h"
#endif

#ifndef _USBPUMP_USBDI_FUNCTION_H_
# include "usbpump_usbdi_function.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

/*

Type:	USBPUMP_USBDI_CLASS_MSD_REQUEST_DONE_FN

Index:	Type:	PUSBPUMP_USBDI_CLASS_MSD_REQUEST_DONE_FN

Function:
	Called to report that an MSD request has completed

Definition:
	typedef VOID USBPUMP_USBDI_CLASS_MSD_REQUEST_DONE_FN(
			USBPUMP_USBDI_FUNCTION *		pFunction,
			USBPUMP_USBDI_CLASS_MSD_REQUEST *	pRequest,
			VOID *					pDoneInfo,
			USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT	Status
			);

	typedef USBPUMP_HCD_REQUEST_DONE_FN
		*PUSBPUMP_HCD_REQUEST_DONE_FN;

Description:
	Every request passed to an HCD for processing must have a non-NULL
	pointer to the completion function in its Hdr.pDoneFn.  This
	function is called after the HCD has completed all transfer-related
	processing for the 

Returns:
	No explicit result.

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_CLASS_MSD_REQUEST_DONE_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_USBDI_FUNCTION *			/* pFunction */,
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST * 	/* pRequest */,
	__TMS_VOID *					/* pDoneInfo */,
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT	/* Status */
	));

/*

Type:	USBPUMP_USBDI_CLASS_MSD_REQUEST

Index:	Type:	USBPUMP_USBDI_CLASS_MSD_REQUEST_FLAG
	Type:	ARG_USBPUMP_USBDI_CLASS_MSD_REQUEST_FLAG
	Type:	USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT
	Type:	ARG_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT

Function:
	Encapsulates a request to a MSC class driver.

Description:
	The portable MSC class API uses special request packets to convey
	requests to the MSC class driver.

Definition:
	typedef struct __TMS_STRUCTNAME(USBPUMP_USBDI_CLASS_MSD_REQUEST)
		USBPUMP_USBDI_CLASS_MSD_REQUEST,
		*PUSBPUMP_USBDI_CLASS_MSD_REQUEST;

	typedef UINT8
		USBPUMP_USBDI_CLASS_MSD_REQUEST_CODE,
		*PUSBPUMP_USBDI_CLASS_MSD_REQUEST_CODE;
	typedef ARG_UINT8
		ARG_USBPUMP_USBDI_CLASS_MSD_REQUEST_CODE,
		*PARG_USBPUMP_USBDI_CLASS_MSD_REQUEST_CODE;

	typedef UINT8
		USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT,
		*PUSBPUMP_USBDI_CLASS_MSD_REQUEST_STAT;
	typedef	ARG_UINT8
		ARG_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT,
		*PARG_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT;

Contents:
	USBPUMP_USBDI_CLASS_MSD_REQUEST_CODE	Request;
		Request code of this MSD request. Defined request codes are

		USBPUMP_USBDI_CLASS_MSD_REQUEST_READ
			Data read request from given LUN.

		USBPUMP_USBDI_CLASS_MSD_REQUEST_WRITE
			Data write request to given LUN.

	USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT	Status;
		On completion, set to the completion code for this request.
		The common MSD entry logic sets this to USBPUMP_USBDI_CLASS_
		MSD_REQUEST_STATUS_BUSY when the request is accepted, and to
		USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_XXX when the request
		is done.

	UINT8	Lun;
		Logical unit number.

	USBPUMP_TIMER_TIMEOUT	Timeout;
		The timeout, expressed in milliseconds.  A timeout value
		of 0 means no timeout.
		|USBPUMP_USBDI_CLASS_MSD_TIMEOUT_MAX| is defined as the maximum
		value of Timeout, and |USBPUMP_USBDI_CLASS_MSD_TIMEOUT_NONE|
		is defined to be zero.

	VOID *			pBuffer;
	USBPUMP_BUFFER_HANDLE	hBuffer;
	BYTES			nBuffer;
	BYTES			nActual;
		Data buffer information. The pBuffer is pointer of data buffer,
		the nBuffer is data buffer size in bytes. The hBuffer is handle
		of the data buffer. The nActual is actual data transfer size
		in bytes and MSD will update this value when complete.


	USBPUMP_USBDI_CLASS_MSD_REQUEST *pNext, *pLast;
		Fields reserved for use by the transient owner of the
		request packet.  This is the client prior to queuing
		and as soon as the packet is completed; and the
		MSD or its delegates from the time the packet is queued
		until the moment the completion routine is called.

	USBPUMP_USBDI_CLASS_MSD_REQUEST_DONE_FN pDoneFn;
		The function to be called when the request is completed.
		The function is of type:

			VOID (*)(
				USBPUMP_USBDI_FUNCTION *pFunction,
				USBPUMP_USBDI_CLASS_MSD_REQUEST *pRequest,
				VOID *pDoneInfo,
				USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT Status
				);

		The parameters are provided for convenience; on typical
		RISC processors, the parameters will be passed in
		registers, saving the code that would otherwise be
		required to fetch the result.

	VOID *pDoneInfo;
		Completion information.

Private Contents:
	UCALLBACKCOMPLETION	Private.Callback.Event;
		DataPump callback event.

	VOID *	Private.Callback.pFunction;
		Save function pointer.

	UINT8	Private.Command.Data;
		The data direction of the request. Defined data directions are

		USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_NONE
			The request has no data phase.

		USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_IN
			The request has data IN phase.

		USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_OUT
			The request has data OUT phase.

	UINT8	Private.Command.bBlockLength;
		Length of the command block in bytes.

	UINT8	Private.Command.Block[16];
		Command block buffer.

Notes:
	None.

See Also:
	USBPUMP_USBDI_CLASS_MSD_REQUEST_DONE_FN
	USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT

*/

#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_PRIVATE_HDR		\
	__TMS_UINT8					Block[16];	\
	__TMS_UINT8					BlockLength;	\
	__TMS_UINT8					DataPhase

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_CLASS_MSD_REQUEST_CALLBACK);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_CLASS_MSD_REQUEST_CALLBACK)
	{
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_PRIVATE_HDR;
	__TMS_UCALLBACKCOMPLETION			Event;
	__TMS_VOID *					pFunction;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_CLASS_MSD_REQUEST_COMMAND);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_CLASS_MSD_REQUEST_COMMAND)
	{
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_PRIVATE_HDR;
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_DONE_FN *	pDoneFn;
	};

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_CLASS_MSD_REQUEST_PRIVATE);
union __TMS_UNIONNAME	(USBPUMP_USBDI_CLASS_MSD_REQUEST_PRIVATE)
	{
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_CALLBACK	Callback;
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_COMMAND	Command;
	};

struct __TMS_STRUCTNAME(USBPUMP_USBDI_CLASS_MSD_REQUEST)
	{
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_CODE	Request;
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT	Status;

	__TMS_UINT8					Lun;

	__TMS_UINT32					StartLBA;
	__TMS_UINT16					NumberOfLBA;

	__TMS_USBPUMP_TIMER_TIMEOUT			Timeout;

	__TMS_VOID *					pBuffer;
	__TMS_USBPUMP_BUFFER_HANDLE			hBuffer;
	__TMS_BYTES					nBuffer;
	__TMS_BYTES					nActual;

	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_DONE_FN *	pDoneFn;
	__TMS_VOID *					pDoneInfo;

	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST *		pNext;
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST *		pLast;

	/* Internal usage */
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_PRIVATE	Private;
	};

/* Request code */
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_READ		0
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_WRITE		1
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST__MAX		2

#define	__TMS_USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST_DATA_NONE	0
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST_DATA_IN	1
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST_DATA_OUT	2
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST__MAX		3

/* Status */
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_OK		0
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_BUSY		1
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_ERROR	2
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_NO_MEDIA	3
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_NO_DEVICE	4
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_PROTECTED	5
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_INVALID	6

/* Data */
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_NONE		0
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_IN		1
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_OUT		2

/**** the timeout values ****/
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_TIMEOUT_NONE		0
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_TIMEOUT_DEFAULT		1
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_TIMEOUT_MIN		2
#define	__TMS_USBPUMP_USBDI_CLASS_MSD_TIMEOUT_MAX		__TMS_UINT16_MAX

/* sane names */
#define	msdrq_CallbackEvent		Private.Callback.Event
#define	msdrq_CallbackFunction		Private.Callback.pFunction
#define	msdrq_CommandBlock		Private.Command.Block
#define	msdrq_CommandBlockLength	Private.Command.BlockLength
#define	msdrq_CommandDataPhase		Private.Command.DataPhase
#define	msdrq_CommandDoneFn		Private.Command.pDoneFn


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

#define	UsbPumpUsbdMsd_PrepareRequest(			\
	a_pRequest,					\
	a_Request,					\
	a_Lun,						\
	a_StartLBA,					\
	a_NumberOfLBA,					\
	a_Timeout,					\
	a_pBuffer,					\
	a_hBuffer,					\
	a_nBuffer,					\
	a_pDoneFn,					\
	a_pDoneInfo					\
	)						\
    do	{						\
	(a_pRequest)->Request = (a_Request);		\
	(a_pRequest)->Lun = (a_Lun);			\
	(a_pRequest)->StartLBA = (a_StartLBA);		\
	(a_pRequest)->NumberOfLBA = (a_NumberOfLBA);	\
	(a_pRequest)->Timeout = (a_Timeout);		\
	(a_pRequest)->pBuffer = (a_pBuffer);		\
	(a_pRequest)->hBuffer = (a_hBuffer);		\
	(a_pRequest)->nBuffer = (a_nBuffer);		\
	(a_pRequest)->pDoneFn = (a_pDoneFn);		\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);	\
	} while (0)

#define	UsbPumpUsbdMsd_PrepareRequestScsi(		\
	a_pRequest,					\
	a_Request,					\
	a_Lun,						\
	a_Timeout,					\
	a_pBuffer,					\
	a_hBuffer,					\
	a_nBuffer,					\
	a_pDoneFn,					\
	a_pDoneInfo,					\
	a_pCommandDataBlock,				\
	a_nCommandDataBlock				\
	)						\
    do	{						\
	(a_pRequest)->Request = (a_Request);		\
	(a_pRequest)->Lun = (a_Lun);			\
	(a_pRequest)->Timeout = (a_Timeout);		\
	(a_pRequest)->pBuffer = (a_pBuffer);		\
	(a_pRequest)->hBuffer = (a_hBuffer);		\
	(a_pRequest)->nBuffer = (a_nBuffer);		\
	(a_pRequest)->pDoneFn = (a_pDoneFn);		\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);	\
	UHIL_cpybuf(					\
		(a_pRequest)->msdrq_CommandBlock,	\
		a_pCommandDataBlock,			\
		a_nCommandDataBlock			\
		);					\
    	(a_pRequest)->msdrq_CommandBlockLength =	\
    		a_nCommandDataBlock;			\
	} while (0)


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_PRIVATE_HDR	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_PRIVATE_HDR
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_READ	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_READ
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_WRITE	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_WRITE
# define USBPUMP_USBDI_CLASS_MSD_REQUEST__MAX	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST__MAX
# define USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST_DATA_NONE	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST_DATA_NONE
# define USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST_DATA_IN	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST_DATA_IN
# define USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST_DATA_OUT	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST_DATA_OUT
# define USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST__MAX	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_SCSI_REQUEST__MAX
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_OK	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_OK
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_BUSY	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_BUSY
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_ERROR	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_ERROR
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_NO_MEDIA	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_NO_MEDIA
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_NO_DEVICE	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_NO_DEVICE
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_PROTECTED	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_PROTECTED
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_INVALID	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_INVALID
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_NONE	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_NONE
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_IN	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_IN
# define USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_OUT	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST_DATA_OUT
# define USBPUMP_USBDI_CLASS_MSD_TIMEOUT_NONE	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_TIMEOUT_NONE
# define USBPUMP_USBDI_CLASS_MSD_TIMEOUT_DEFAULT	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_TIMEOUT_DEFAULT
# define USBPUMP_USBDI_CLASS_MSD_TIMEOUT_MIN	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_TIMEOUT_MIN
# define USBPUMP_USBDI_CLASS_MSD_TIMEOUT_MAX	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_TIMEOUT_MAX
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_class_msd_request.h ****/
#endif /* _USBPUMP_USBDI_CLASS_MSD_REQUEST_H_ */
