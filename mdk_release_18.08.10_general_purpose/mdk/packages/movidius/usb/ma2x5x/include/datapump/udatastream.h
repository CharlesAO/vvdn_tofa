/* udatastream.h	Mon Jun  1 2015 00:19:18 tmm */

/*

Module:  udatastream.h

Function:
	Home of UDATASTREAM

Version:
	V3.15b	Mon Jun  1 2015 00:19:18 tmm	Edit level 9

Copyright notice:
	This file copyright (C) 2002-2007, 2009, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	June 2002

Revision history:
   1.79a  Sun Jun  2 2002 17:01:51  tmm
	Module created.

   1.89a  Wed May 12 2004 08:13:33 maw
   	Added macro UDATASTREAM_NO_CURRENT_PIPE

   1.89a  Wed Jun 02 2004 09:32:07  chwon
	Changed from NULL to __TMS_NULL for type cloaking

   1.97e  Mon Aug 07 2006 18:59:36  tmm
	Documentation changes only. (Removed one macro that was an artifact
	of older code, and that no longer made sense.)

   1.97e  Mon Aug 14 2006 14:43:45  tmm
	Add new API functions UsbPumpDataStream_GetBoundPipe(),
	UsbPumpDataStream_GetCurrentPipe(), UsbPumpDataStream_GetLastPipe().

   1.97e  Tue Aug 15 2006 19:30:08  tmm
	Fix typos.

   1.97i  Tue May 29 2007 15:11:15  chwon
	2520: parameter name changed from pDataPlane to pDataStream

   2.01a  Thu May 21 2009 09:52:04  chwon
	8079: add UsbPumpDataStream_QueueV2() prototype.

   3.15b  Mon Jun  1 2015 00:19:18  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _UDATASTREAM_H_		/* prevent multiple includes */
#define _UDATASTREAM_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _UEVENTNODE_H_
# include "ueventnode.h"
#endif

#ifndef _UDATAPLANE_H_
# include "udataplane.h"
#endif

/****************************************************************************\
|
|	The structures
|
\****************************************************************************/

/*

Type:	UDATASTREAM

Function:
	C structure: represents a single pipe that may range over
	a variety of UINTERFACEs.

Description:
	One of the headaches in the V1.6 and previous DataPump is locating
	the pipe to be used for multi-interface devices.  Most applications
	end up duplicating initialization code that could be centralized,
	and repeatedly looking up UPIPEs afer a configuration or interface
	change.

	The UDATASTREAM object is intended to allow the DataPump to provide
	most of that logic, if the protocol implementer chooses.

Contents:
	UDATASTREAM *pNext, *pLast;
		Pointers to sibling UDATASTREAMs that are collected
		by the parent dataplane.

	UPIPE *pCurrentPipe;
		Pointer to the UPIPE that is currently associated with
		this UDATASTREAM.  Will be NULL until a matching
		pipe can be found.

	UCHAR ucBindingFlags;
		These flags are set up when the UDATASTREAM is bound to
		the UDATAPLANE.  They are specified by the caller, and
		indicate what kinds of pipe are acceptable.  Each bit,
		if set, allows a UPIPE of the specified kind to match.

		The known flags are UPIPE_SETTING_MASK_..., primarily:

		  UPIPE_SETTING_MASK_{CONTROL,BULK,INT,ISO}_{IN,OUT}

		and secondarily

		  UPIPE_SETTING_MASK_{ANY, DATA_{IN,OUT}, BULKINT_{IN,OUT}}

		These flags are described in
		UsbPumpInterface_EnumeratePipes().

	UCHAR ucPipeOrdinal;
		If 0, the first matching pipe is used.  If 1, the second
		matching pipe is used; etc.

	UBUFQE *pHoldQueue;
		A slot for holding UBUFQEs received when no physical
		pipe is bound.  THese UBUFQEs will automatically be queued
		to the physical pipe when the pipe is rebound.

	UDATAPLANE *pDataPlane;
		The containing UDATAPLANE.

See Also:
	UDATAPLANE

	UsbPumpInterface_EnumeratePipes()

*/


/* __TMS_TYPE_DEF_STRUCT(UDATASTREAM); */

struct __TMS_STRUCTNAME(UDATASTREAM)
	{
	/*
	|| We need pointers to the next stream in this set; for consistency,
	|| we have a back pointer, too.
	*/
	__TMS_UDATASTREAM	*pNext;
	__TMS_UDATASTREAM	*pLast;

	/*
	|| When the stream has been plumbed completely, pPipe
	|| will point to the underlying UPIPE structure.  However,
	|| pPipe is only valid until a setting change occurs; at that
	|| point we have to revalidate.  pPipe will be NULL if there
	|| currently is no pipe.
	*/
	__TMS_UPIPE		*pCurrentPipe;

	/*
	|| It is convenient to have a parking queue for UBUFQEs on this
	|| stream, so that UBUFQEs can be submitted even when the
	|| pipe is not bound.
	*/
	__TMS_UBUFQE		*pHoldQueue;

	/*
	|| We need a pointer to the UDATAPLANE that owns this UDATASTREAM
	*/
	__TMS_UDATAPLANE	*pDataPlane;

	/*
	|| We save an indication of the stream's binding so we can replumb
	|| after an interface change.  ucBindingFlags identifies the kind of
	|| pipe; ucPipeOrdinal identifies which pipe of that kind.
	*/
	__TMS_UCHAR		ucBindingFlags;
	__TMS_UCHAR		ucPipeOrdinal;
	__TMS_USHORT		usEpAddrMask;
	};

/*
|| Test if there is a pipe connected to the stream
*/
#define __TMS_UDATASTREAM_NO_CURRENT_PIPE(pDataStream)			\
	((pDataStream)->pCurrentPipe == __TMS_NULL)


/****************************************************************************\
|
|	API functions.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/* from dstream_bindtopipe.c */
__TMS_VOID
UsbPumpDataStream_BindToPipe(
	__TMS_UDATASTREAM *pDataStream,
	__TMS_UINT	BindingFlags,
	__TMS_UINT	EpAddrMask,
	__TMS_UINT	PipeOrdinal
	);

/* from dstream_cancel.c */
__TMS_VOID
UsbPumpDataStream_Cancel(
	__TMS_UDATASTREAM *pDataStream,
	__TMS_USTAT	  Why
	);

/* from dstream_create.c */
__TMS_UDATASTREAM *
UsbPumpDataStream_Create(
	__TMS_UDATAPLANE *pDataPlane
	);

/* from dstream_delete.c */
__TMS_VOID
UsbPumpDataStream_Delete(
	__TMS_UDATASTREAM *pDataStream
	);

/* from dstream_deinit.c */
__TMS_VOID
UsbPumpDataStream_Deinit(
	__TMS_UDATASTREAM *pDataStream
	);

/* from dstream_getboundpipe.c */
__TMS_UPIPE *
UsbPumpDataStream_GetBoundPipe(
	__TMS_UDATASTREAM *pDataStream
	);

/* from dstream_getcurrentpipe.c */
__TMS_UPIPE *
UsbPumpDataStream_GetCurrentPipe(
	__TMS_UDATASTREAM *pDataStream,
	__TMS_BOOL *pfValid
	);

/* from dstream_getlastpipe.c */
__TMS_UPIPE *
UsbPumpDataStream_GetLastPipe(
	__TMS_UDATASTREAM *pDataStream,
	__TMS_BOOL *pfValid
	);

/* from dstream_init.c */
__TMS_VOID
UsbPumpDataStream_Init(
	__TMS_UDATASTREAM *pDataStream,
	__TMS_UDATAPLANE *pDataPlane
	);

__TMS_UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN UsbPumpDataStreamI_DataPlaneUp;

/* from dstream_queue.c */
__TMS_USTAT
UsbPumpDataStream_Queue(
	__TMS_UDEVICE *pDevice,
	__TMS_UDATASTREAM *pDataStream,
	__TMS_UBUFQE *pQeHead,
	__TMS_BOOL bHoldIfDown
	);

__TMS_VOID
UsbPumpDataStream_QueueV2(
	__TMS_UDEVICE *pDevice,
	__TMS_UDATASTREAM *pDataStream,
	__TMS_UBUFQE *pQeHead,
	__TMS_BOOL bHoldIfDown
	);

/* from dstreami_reconnect.c */
__TMS_UPIPE *
UsbPumpDataStreamI_ReconnectPipe(
	__TMS_UDATASTREAM *pDataStream
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UDATASTREAM_NO_CURRENT_PIPE(pDataStream)	\
   __TMS_UDATASTREAM_NO_CURRENT_PIPE(pDataStream)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of udatastream.h ****/
#endif /* _UDATASTREAM_H_ */
