/* usbpump_hcd_request.h	Mon Jan 14 2013 10:50:45 chwon */

/*

Module:  usbpump_hcd_request.h

Function:
	Low-level HCD request block.

Version:
	V3.11b	Mon Jan 14 2013 10:50:45 chwon	Edit level 28

Copyright notice:
	This file copyright (C) 2005-2011, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Thu Jun  2 2005 01:50:11  tmm
	Module created.

   1.91h  2005-08-14  tmm
	Move USETUP_WIRE to core "i/usetup.h"

   1.91h  Thu Sep  1 2005 05:40:02  tmm
	Fix typos in documentation.

   1.91h  Fri Sep  2 2005 00:17:22  tmm
	Move docs for USBPUMP_HCD_REQUEST_DONE_FN and
	USBPUMP_HCD_REQUEST_CANCEL_FN to usbpump_hcd_types.h (where they
	are defined).

   1.91h  Mon Sep  5 2005 00:12:34  tmm
	Add typedefs for INOUT forms of requests, add more characterization
	macros.

   1.93b  Mon Nov  22 2005 12:54:34  ssyang
	Fix typo in macro and macro bug

   1.93c  Sat Dec  3 2005 18:27:57  tmm
	Fix problems identified by Soonseop in writing OTG test program.

   1.97f  Fri Oct 27 2006 16:29:42  chwon
	2208: Add USBPUMP_HCD_RQ_GET_FRAME request and related structure
	2226: Add USBPUMP_HCD_RQ_ABORT_PIPE request and related structure

   1.97h  Tue Apr 03 2007 02:08:23  tmm
	2226: revert out USBPUMP_HCD_RQ_ABORT_PIPE. It is not architecturally
	correct. USBD must track requests at its level, and issue the existing
	request cancel operation.  The HCD does not need the overhead of
	the tracking needed for doing this operation.

	Correct definition of UsbPumpHcdI_PrepareGetFrameRequestHeader();
	it's now UsbPumpHcd_PrepareGetFrameRequest(), an external API.  Add
	the flags and timemout arguments, for consistency with all other
	requests.

	2623: Correct UsbPumpHcd_PrepareIsochOutHeader() to
	UsbPumpHcd_PrepareIsochOutRequest().  Remove A_Code arguments from
	both IsochOut and In.

   1.97i  Mon Jun 04 2007 09:31:59  djt
   	2939: Added USBPUMP_HCD_REQUEST_FLAG_TEST_IMMEDIATE_COMPLETE.
   	Fixed comments for Flags and InternalFlags.

   1.97j  Fri Oct 19 2007 13:41:06  chwon
	2939: added USBPUMP_HCD_REQUEST_FLAG_TEST_SKIP_SETUP_STAGE flag.

   1.97k  Tue Jan 29 2008 14:19:57  skpark
	3644: added sizeIsochDescr in the ISO HCD request

   1.97k  Fri Aug 08 2008 13:26:54  chwon
	UDP-4312: Add USBPUMP_HCD_REQUEST_ALIGNMENT definition

   2.01a  Thu Feb 12 2009 17:52:52  chwon
	4312: add USBPUMP_BUFFER_HANDLE in the USBPUMP_HCD_REQUEST

   2.01a  Mon Mar 02 2009 09:55:21  djt
	7416: Added requests required for XHCI.
	Added HCD_RQ_BULK_STREAM_IN, HCD_RQ_BULK_STREAM_OUT.

   2.01a  Fri Apr 03 2009 09:48:36  djt
	7416: Fixed compiler errors.

   2.01a  Fri Apr 24 2009 11:54:15  djt
	7879: Added USBPUMP_HCD_REQUEST_FLAG_POST_BREAK.

   2.01a  Fri May 15 2009 23:35:26  chwon
	7416: Add USBPUMP_HCD_PIPE in the SET_ADDRESS and RESET_DEVICE request

   2.01a  Fri May 29 2009 00:25:36  tmm
	8144: Add USBPUMP_HCD_REQUEST_FLAG_NOT_FIRST,
	USBPUMP_HCD_REQUEST_FLAG_NOT_LAST, USBPUMP_HCD_REQUEST_FLAGS type,
	USBPUMP_HCD_REQUEST_INTERNAL_FLAGS type; improved documentation.

   2.01a  Mon Jul 13 2009 15:59:01  chwon
	8441: Add fSendSetAddressRequest and bSelectedDeviceAddress field
	in the USBPUMP_HCD_REQUEST_SET_ADDRESS.

   2.01a  Wed Jul 29 2009 12:03:41  chwon
	7416: RESET_DEVICE and SET_ADDRESS HCD request should take a HCD_DEVICE
	as the parameter instead of using a PIPE or slot id.

   2.01a  Wed Nov 11 2009 12:03:17  chwon
	3987: Add USBPUMP_HCD_RQ_VALIDATE_PIPE() macro

   3.01b  Fri Mar 05 2010 11:09:35  chwon
	9989: Add USBPUMP_HCD_RQ_IS_STREAM() macro.

   3.01c  Tue May 25 2010 14:21:04  chwon
	10416: Add USBPUMP_HCD_RQ_DEVICE_PAUSE HCD request.

   3.01d  Mon Oct 11 2010 16:06:27  ssyang
	11606: Add USBPUMP_HCD_REQUEST_INTERNAL_FLAG_BUFFER_ENTERSCOPE
	and USBPUMP_HCD_REQUEST_INTERNAL_FLAG_ISOCHDESC_ENTERSCOPE

   3.01d  Thu Oct 14 2010 16:39:00  chwon
	11630: add fForL1Suspend in the DEVICE_PAUSE HCD request.

   3.01d  Thu Dec 16 2010 15:15:19  chwon
	12183: fix IsochFrame setting in the ..._PrepareIsochIORequestHeader.

   3.01f  Thu May 26 2011 15:21:44  chwon
	13490: add USBPUMP_HCD_RQ_FUNCTION_WAKE HCD request and setup macro.

   3.11b  Mon Jan 14 2013 10:50:45  chwon
	16347: add USBPUMP_HCD_RQ_CONTROL_PIPE HCD request and setup macro.

*/

#ifndef _USBPUMP_HCD_REQUEST_H_		/* prevent multiple includes */
#define _USBPUMP_HCD_REQUEST_H_

#ifndef _USBPUMP_HCD_TYPES_H_
# include "usbpump_hcd_types.h"
#endif

#ifndef _USBPUMP_USBDI_ISOCH_H_
# include "usbpump_usbdi_isoch.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

#ifndef _USETUP_H_
# include "usetup.h"
#endif

/*

Type:	USBPUMP_HCD_REQUEST

Index:	Type:	USBPUMP_HCD_REQUEST_LENGTH
	Type:	ARG_USBPUMP_HCD_REQUEST_LENGTH
	Type:	USBPUMP_HCD_REQUEST_CODE
	Type:	ARG_USBPUMP_HCD_REQUEST_CODE
	Type:	USBPUMP_HCD_REQUEST_FLAGS
	Type:	ARG_USBPUMP_HCD_REQUEST_FLAGS
	Type:	USBPUMP_HCD_REQUEST_INTERNAL_FLAGS
	Type:	ARG_USBPUMP_HCD_REQUEST_INTERNAL_FLAGS

Function:
	Encapsulates a data or control request to an HCD.

Description:
	The portable HCD API uses special request packets to convey
	requests to the HCD layer.  The USBPUMP_HCD_REQUEST object defines
	the layout for all the defined requests, with a common header
	followed by variant forms for the specific commands.

Definition:
	typedef union __TMS_UNIONNAME(USBPUMP_HCD_REQUEST)
		USBPUMP_HCD_REQUEST, *PUSBPUMP_HCD_REQUEST;

	typedef UINTxx USBPUMP_HCD_REQUEST_LENGTH, *PUSBPUMP_REQUEST_LENGTH;
	typedef ARG_UINTxx
		ARG_USBPUMP_HCD_REQUEST_LENGTH, *PARG_USBPUMP_REQUEST_LENGTH;
	typedef UINTxx USBPUMP_HCD_REQUEST_LENGTH, *PUSBPUMP_REQUEST_LENGTH;
	typedef	ARG_UINTxx
		ARG_USBPUMP_HCD_REQUEST_CODE, *PARG_USBPUMP_REQUEST_CODE;
	typedef UINT8 USBPUMP_HCD_REQUEST_FLAGS, *PUSBPUMP_HCD_REQUEST_FLAGS;
	typedef ARG_UINT8
		ARG_USBPUMP_HCD_REQUEST_FLAGS, *PARG_USBPUMP_HCD_REQUEST_FLAGS;
	typedef UINT8 USBPUMP_HCD_REQUEST_INTERNAL_FLAGS, 
		*PUSBPUMP_HCD_REQUEST_INTERNAL_FLAGS;
	typedef ARG_UINT8
		ARG_USBPUMP_HCD_REQUEST_INTERNAL_FLAGS, 
		*PARG_USBPUMP_HCD_REQUEST_INTERNAL_FLAGS;

Contents:
	USBPUMP_HCD_REQUEST_LENGTH Hdr.Length;
		length of the request in bytes

	USBPUMP_HCD_REQUEST_CODE Hdr.Request;
		The request code, chosen from symbols named
		USBPUMP_HCD_RQ_....

	USBPUMP_HCD_REQUEST_DONE_FN Hdr.pDoneFn;
		The function to be called when the request is completed.
		The function is of type:

			VOID (*)(
				USBPUMP_HCD *pHcd,
				USBPUMP_HCD_REQUEST *pRequest,
				VOID *pDoneInfo,
				ARG_USTAT Status
				);

		The parameters are provided for convenience; on typical
		RISC processors, the parameters will be passed in
		registers, saving the code that would otherwise be
		required to fetch the result.

	VOID *Hdr.pDoneInfo;
		Completion information.

	USBPUMP_HCD_REQUEST *Hdr.pNext, *Hdr.pLast;
		Fields reserved for use by the transient owner of the
		request packet.  This is the client prior to queuing
		and as soon as the packet is completed; and the
		HCD or its delegates from the time the packet is queued
		until the moment the completion routine is called.

	USBPUMP_HCD *Hdr.pHcd;
		Pointer to the owning HCD.

	USBPUMP_ADDRBIT_PTR_UNION Hdr.HcdReserved[4];
		Four pointer-size fields reserved for use by the
		current owner of the request packet, as defined above.
		As the name implies, these fields are intended for
		use by the HCD.

	USBPUMP_HCD_REQUEST_TIMEOUT Hdr.Timeout;
		The timeout, expressed in milliseconds.  A timeout value
		of 0 means no timeout.  The constant
		|USBPUMP_HCD_REQUEST_TIMEOUT_MAX| is defined as the maximum
		value of Hdr.Timeout, and |USBPUMP_HCD_REQUEST_TIMEOUT_NONE|
		is defined to be zero.

	USTAT Hdr.Status;
		On completion, set to the completion code for this
		request.  The common HCD entry logic sets this to
		USTAT_BUSY when the request is accepted, and to
		USTAT_COMPLETED when the request is done.

	USBPUMP_HCD_REQUEST_FLAGS Hdr.Flags;
		These flags are set by layers above the HCD.
		On submission, set to the bitwise combination of any of
		the following.  Note that the flag bits change meaning
		based on the command being issued; bits are reused as
		convenient.  Only 8 bits are allocated, so we need
		to reuse where possible.

		USBPUMP_HCD_REQUEST_FLAG_ASAP
			For isochronous requests only, schedule as soon
			as possible; otherwise wait for the frame.
			See USBPUMP_HCD_REQUEST_ISOCH_IN for details.

		USBPUMP_HCD_REQUEST_FLAG_SHORT_OK
			Indicates that a short packet is OK, as defined
			by the USB 2.0 spec.  Only for pipe _IN requests.

		USBPUMP_HCD_REQUEST_FLAG_TEST_IMMEDIATE_COMPLETE
			Used in the test mode only. This will tell the HCD
			to immediately complete after the SETUP stage.
			Only use for CONTROL_IN/_OUT requests.

		USBPUMP_HCD_REQUEST_FLAG_TEST_SKIP_SETUP_STAGE
			Used in the test mode only. This will tell the HCD
			to sktp the SETUP stage. If this flag set, HCD should
			start DATA stage.  Only use for CONTROL_IN/_OUT
			requests.

		USBPUMP_HCD_REQUEST_FLAG_POST_BREAK
			For control/bulk/int OUT requests only, send a ZLP
			if this transfer is exactly a multiple of 
			wMaxTransferSize.
			
		USBPUMP_HCD_REQUEST_FLAG_NOT_FIRST_OF_SET
			For DEINIT_PIPE, INIT_PIPE, UPDATE_PIPE: indicates
			that this request is not the first of a collection
			of related changes.
			
		USBPUMP_HCD_REQUEST_FLAG_NOT_LAST_OF_SET
			For DEINIT_PIPE, INIT_PIPE, UPDATE_PIPE: indicates
			that this request is not the last of a collection
			of related changes.
			
			HCDs are allowed to collect all changes between
			the first and the last of a set, and only apply
			the change when the last is received.

		USBPUMP_HCD_REQUEST_FLAG_INACTIVATE_DURING_SUSPEND
			For CONTROL_PIPE, sub-request REQ_INACTIVATE: indicates
			pipe inactivate operation happens during device
			selective suspend or function suspend.

		USBPUMP_HCD_REQUEST_FLAG_RESTART_KEEP_TRANSFER_STATE
			For CONTROL_PIPE, sub-request REQ_RESTART: indicates
			HCD need to keep data toggle state during restart pipe
			operation.  If not set this flag, HCD need to reset
			data toggle state.

	USBPUMB_HCD_REQUEST_INTERNAL_FLAGS Hdr.InternalFlags;
		These flags are set by the HCD.  On submission, set to zero.
		Clients must not manipulate or inspect these flags directly,
		or depend on them in any way.

		During processing, will be set to the bitwise combination of any of
		the following:

		USBPUMP_HCD_REQUEST_INTERNAL_FLAG_CANCELLED
			Used during processing to indicate that the
			request has been cancelled (or timed out).
			If this is set, any normal processing routines
			should expeditiously retire the request.

		USBPUMP_HCD_REQUEST_INTERNAL_FLAG_TIMING
			Flag to indicate a Timer is set.

		USBPUMP_HCD_REQUEST_INTERNAL_FLAG_TIMED_OUT
			Flag to indicate the request has timed out.

		USBPUMP_HCD_REQUEST_INTERNAL_FLAG_BUFFER_ENTERSCOPE
			Flag to indicate buffer enter scope is called.

		USBPUMP_HCD_REQUEST_INTERNAL_FLAG_ISOCHDESC_ENTERSCOPE
			Flag to indicate isoch packet descriptor buffer enter
			scope is called.

	The following entries are separately indexed, but given here for
	completeness.

	USBPUMP_HCD_REQUEST_PIPE_CONTROL PipeControl;
		Fields to be used for pipe control operations.

	USBPUMP_HCD_REQUEST_CONTROL_IN ControlIn;
		Fields to be used for control-in transfers.

	USBPUMP_HCD_REQUEST_CONTROL_OUT ControlOut;
		Fields to be used for control-out transfers.

	USBPUMP_HCD_REQUEST_BULKINT_IN BulkIntIn;
		Fields to be used for bulk- or interrupt-IN transfers.

	USBPUMP_HCD_REQUEST_BULKINT_OUT BulkIntOut;
		Fields to be used for bulk- or interrupt-OUT transfers.

	USBPUMP_HCD_REQUEST_ISOCH_IN IsochIn;
		Fields to be used for isochronous-IN transfers.

	USBPUMP_HCD_REQUEST_ISOCH_OUT IsochOut;
		Fields to be used for isochronous-OUT transfers.

	USBPUMP_HCD_REQUEST_BULKINT_STREAM_IN BulkIntStreamIn;
		Fields to be used for bulk- or interrupt-IN Stream transfers.

	USBPUMP_HCD_REQUEST_BULKINT_STREAM_OUT BulkIntStreamOut;
		Fields to be used for bulk- or interrupt-OUT Stream transfers.

	USBPUMP_HCD_REQUEST_GET_FRAME GetFrame;
		Fields to be used for get frame request.

	USBPUMP_HCD_REQUEST_SET_ADDRESS SetAddress;
		Fields to be used for set address request.

	USBPUMP_HCD_REQUEST_RESET_DEVICE ResetDevice;
		Fields to be used for reset device request.

	USBPUMP_HCD_REQUEST_DEVICE_PAUSE DevicePause;
		Fields to be used for pause device request.

	USBPUMP_HCD_REQUEST_FUNCTION_WAKE FunctionWake;
		Fields to be used for function wake request.

	USBPUMP_HCD_REQUEST_CONTROL_PIPE ControlPipe;
		Fields to be used for control pipe request.

	Note that the number of bytes actually used in a given request
	can vary based on the actual request code.

Private Contents:
	USBPUMP_HCD_REQUEST_CANCEL_FN Hdr.pCancelFn;
		If non-NULL, this function will be called to report the
		cancellation of the request.  This field will be
		reset to NULL prior to calling the cancel function.

	VOID *Hdr.pCancelInfo;
		Context pointer for use by the cancel function.

Notes:
	The top-level union and each of the different request
	packets all are set up so that Hdr.* will work, and means the
	same thing.

	API functions are provided to allow easier creation of the request
	packets.

See Also:
	USBPUMP_HCD_REQUEST_PIPE_CONTROL
	USBPUMP_HCD_REQUEST_BULKINT_IN
	USBPUMP_HCD_REQUEST_BULKINT_OUT
	USBPUMP_HCD_REQUEST_BULKINT_STREAM_IN
	USBPUMP_HCD_REQUEST_BULKINT_STREAM_OUT
	USBPUMP_HCD_REQUEST_CONTROL_IN
	USBPUMP_HCD_REQUEST_CONTROL_OUT
	USBPUMP_HCD_REQUEST_ISOCH_IN
	USBPUMP_HCD_REQUEST_ISOCH_OUT
	USBPUMP_HCD_REQUEST_CANCEL_FN
	USBPUMP_HCD_REQUEST_DONE_FN
	USBPUMP_HCD_REQUEST_CODE

*/

/****************************************************************************\
|
|	The scalar types, forward structures, and function types.
|
\****************************************************************************/

/* __TMS_TYPE_DEF_UNION(USBPUMP_HCD_REQUEST); */
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_HDR);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_GENERIC);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_PIPE_CONTROL);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_CONTROL_IN);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_CONTROL_OUT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_CONTROL_INOUT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_BULKINT_IN);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_BULKINT_OUT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_BULKINT_INOUT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_BULKINT_STREAM_IN);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_BULKINT_STREAM_OUT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_BULKINT_STREAM_INOUT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_ISOCH_IN);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_ISOCH_OUT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_ISOCH_INOUT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_PIPE_INOUT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_GET_FRAME);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_SET_ADDRESS);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_RESET_DEVICE);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_DEVICE_PAUSE);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_FUNCTION_WAKE);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_CONTROL_PIPE);

__TMS_TYPE_DEF_UNION	(USBPUMP_HCD_REQUEST_BUFPTR);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_ISO_DESCR);

__TMS_TYPE_DEF_ARG	(USBPUMP_HCD_REQUEST_LENGTH, UINT16);
__TMS_TYPE_DEF_ARG	(USBPUMP_HCD_REQUEST_CODE, UINT16);
__TMS_TYPE_DEF_ARG	(USBPUMP_HCD_REQUEST_FLAGS, UINT8);
__TMS_TYPE_DEF_ARG	(USBPUMP_HCD_REQUEST_INTERNAL_FLAGS, UINT8);
__TMS_TYPE_DEF_ARG	(USBPUMP_HCD_REQUEST_TIMEOUT, UINT16);
__TMS_TYPE_DEF_ARG	(USBPUMP_HCD_REQUEST_ID, UINT32);


/****************************************************************************\
|
|	USBPUMP_HCD_BUFPTR is an internal type used in the "generic"
|	(in-or-out) flavors of the setup packets.
|
\****************************************************************************/

union __TMS_UNIONNAME(USBPUMP_HCD_REQUEST_BUFPTR)
	{
	__TMS_VOID *pReadBuffer;
	__TMS_CONST __TMS_VOID *pcWriteBuffer;
	};

/****************************************************************************\
|
|	The asynchronous request codes
|
\****************************************************************************/

#define	__TMS_USBPUMP_HCD_RQ_BULK_IN		0
#define	__TMS_USBPUMP_HCD_RQ_BULK_OUT		1
#define	__TMS_USBPUMP_HCD_RQ_INT_IN		2
#define	__TMS_USBPUMP_HCD_RQ_INT_OUT		3
#define	__TMS_USBPUMP_HCD_RQ_CTL_IN		4
#define	__TMS_USBPUMP_HCD_RQ_CTL_OUT		5
#define	__TMS_USBPUMP_HCD_RQ_ISO_IN		6
#define	__TMS_USBPUMP_HCD_RQ_ISO_OUT		7
#define	__TMS_USBPUMP_HCD_RQ_INIT_PIPE		8
#define	__TMS_USBPUMP_HCD_RQ_DEINIT_PIPE	9
#define	__TMS_USBPUMP_HCD_RQ_UPDATE_PIPE	10
#define	__TMS_USBPUMP_HCD_RQ_GET_FRAME		11
#define	__TMS_USBPUMP_HCD_RQ_BULK_STREAM_IN	12
#define	__TMS_USBPUMP_HCD_RQ_BULK_STREAM_OUT	13
#define	__TMS_USBPUMP_HCD_RQ_SET_ADDRESS	14
#define	__TMS_USBPUMP_HCD_RQ_RESET_DEVICE	15
#define	__TMS_USBPUMP_HCD_RQ_DEVICE_PAUSE	16
#define	__TMS_USBPUMP_HCD_RQ_FUNCTION_WAKE	17
#define	__TMS_USBPUMP_HCD_RQ_CONTROL_PIPE	18

#define	__TMS_USBPUMP_HCD_RQ__MAX		19

#define	__TMS_USBPUMP_HCD_RQ__INIT		\
	"USBPUMP_HCD_RQ_BULK_IN",		\
	"USBPUMP_HCD_RQ_BULK_OUT",		\
	"USBPUMP_HCD_RQ_INT_IN",		\
	"USBPUMP_HCD_RQ_INT_OUT",		\
	"USBPUMP_HCD_RQ_CTL_IN",		\
	"USBPUMP_HCD_RQ_CTL_OUT",		\
	"USBPUMP_HCD_RQ_ISO_IN",		\
	"USBPUMP_HCD_RQ_ISO_OUT",		\
	"USBPUMP_HCD_RQ_INIT_PIPE",		\
	"USBPUMP_HCD_RQ_DEINIT_PIPE",		\
	"USBPUMP_HCD_RQ_UPDATE_PIPE",		\
	"USBPUMP_HCD_RQ_GET_FRAME",		\
	"USBPUMP_HCD_RQ_BULK_STREAM_IN",	\
	"USBPUMP_HCD_RQ_BULK_STREAM_OUT",	\
	"USBPUMP_HCD_RQ_SET_ADDRESS",		\
	"USBPUMP_HCD_RQ_RESET_DEVICE",		\
	"USBPUMP_HCD_RQ_DEVICE_PAUSE",		\
	"USBPUMP_HCD_RQ_FUNCTION_WAKE",		\
	"USBPUMP_HCD_RQ_CONTROL_PIPE"


/****************************************************************************\
|
|	This structure is always at the front of any HCD request.
|
\****************************************************************************/

#define	__TMS_USBPUMP_HCD_REQUEST__HDR				\
	__TMS_USBPUMP_HCD_REQUEST_LENGTH	Length;		\
	__TMS_USBPUMP_HCD_REQUEST_CODE		Request;	\
	__TMS_USBPUMP_HCD_REQUEST		*pNext;		\
	__TMS_USBPUMP_HCD_REQUEST		*pLast;		\
	__TMS_USBPUMP_HCD_REQUEST_DONE_FN	*pDoneFn;	\
	__TMS_VOID				*pDoneInfo;	\
	__TMS_USBPUMP_HCD_REQUEST_CANCEL_FN	*pCancelFn;	\
	__TMS_VOID				*pCancelInfo;	\
	__TMS_USBPUMP_HCD			*pHcd;		\
	__TMS_ADDRBITS_PTR_UNION		HcdReserved[4];	\
	/* the results show up here */				\
	/* a USTAT is inconveniently aligned, so put timeout first */	\
	__TMS_USBPUMP_TIMER			TimeoutTimer;	\
	__TMS_UINT8				Refcount;	\
	__TMS_UINT8				Status;		\
	__TMS_USBPUMP_HCD_REQUEST_FLAGS		Flags;		\
	__TMS_USBPUMP_HCD_REQUEST_INTERNAL_FLAGS InternalFlags

#define	__TMS_USBPUMP_HCD_REQUEST_PIPE_CONTROL__HDR		\
	__TMS_USBPUMP_HCD_REQUEST__HDR;				\
	__TMS_USBPUMP_HCD_PIPE			*pPipe

struct __TMS_STRUCTNAME	(USBPUMP_HCD_REQUEST_HDR)
	{
	__TMS_USBPUMP_HCD_REQUEST__HDR;
	};

/****************************************************************************\
|
|	This macro is used to define a pointer to CONST or non-CONST
|	user buffer.
|
\****************************************************************************/

#define	__TMS_USBPUMP_HCD_REQUEST_PIPE_IO__HDR(BufType)		\
	__TMS_USBPUMP_HCD_REQUEST_PIPE_CONTROL__HDR;		\
	BufType					pBuffer;	\
	__TMS_USBPUMP_BUFFER_HANDLE		hBuffer;	\
	__TMS_BYTES				nBuffer;	\
	__TMS_BYTES				nActual


struct __TMS_STRUCTNAME	(USBPUMP_HCD_REQUEST_PIPE_INOUT)
	{
	__TMS_USBPUMP_HCD_REQUEST_PIPE_IO__HDR(__TMS_USBPUMP_HCD_REQUEST_BUFPTR);
	};

/**** the flag values ****/
/*
|| For isochronous commands
*/
#define	__TMS_USBPUMP_HCD_REQUEST_FLAG_ASAP			(1u << 0)
/*
|| For IN transfers.  Must be different from FLAG_ASAP, because both
|| apply to isoch-in transfers.
*/
#define	__TMS_USBPUMP_HCD_REQUEST_FLAG_SHORT_OK			(1u << 1)
/*
|| For Control-IN/OUT.  Must be different from SHORT_OK, because
|| both apply to Control-IN transfers.  
*/
#define	__TMS_USBPUMP_HCD_REQUEST_FLAG_TEST_IMMEDIATE_COMPLETE	(1u << 2)
/*
|| For Control-IN/OUT.  Must be different from SHORT_OK, because
|| both apply to Control-IN transfers.  
*/
#define	__TMS_USBPUMP_HCD_REQUEST_FLAG_TEST_SKIP_SETUP_STAGE	(1u << 3)
/*
|| For Int/Bulk OUT.  No conflict with any of the above bits, but is
|| separate for historical reasons. 
*/
#define	__TMS_USBPUMP_HCD_REQUEST_FLAG_POST_BREAK		(1u << 4)
/*
|| The flags for INIT_PIPE, UPDATE_PIPE, DEINIT_PIPE can overlap
|| any of the above, because none of the above flags apply to pipe
|| control commands; and because none of the pipe control flags
|| apply to general transfer commands
*/
#define	__TMS_USBPUMP_HCD_REQUEST_FLAG_NOT_FIRST_OF_SET		(1u << 0)
#define	__TMS_USBPUMP_HCD_REQUEST_FLAG_NOT_LAST_OF_SET		(1u << 1)
/*
|| The flags for CONTROL_PIPE.
*/
/*
|| For USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_INACTIVATE, this flag indicates
|| pipe inactivate operation happends during device selective suspend or
|| function suspend.
*/
#define	__TMS_USBPUMP_HCD_REQUEST_FLAG_INACTIVATE_DURING_SUSPEND (1u << 0)
/*
|| For USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_RESTART, this flag indicates
|| HCD need to keep data toggle state during restart pipe operation.
|| 
*/
#define	__TMS_USBPUMP_HCD_REQUEST_FLAG_RESTART_KEEP_TRANSFER_STATE (1u << 1)

/* bits 5..7 reserved for future use */

/**** internal flag values ****/
#define	__TMS_USBPUMP_HCD_REQUEST_INTERNAL_FLAG_CANCELLED	(1u << 0)
#define	__TMS_USBPUMP_HCD_REQUEST_INTERNAL_FLAG_TIMING		(1u << 1)
#define	__TMS_USBPUMP_HCD_REQUEST_INTERNAL_FLAG_TIMED_OUT	(1u << 2)
#define	__TMS_USBPUMP_HCD_REQUEST_INTERNAL_FLAG_BUFFER_ENTERSCOPE    (1u << 3)
#define	__TMS_USBPUMP_HCD_REQUEST_INTERNAL_FLAG_ISOCHDESC_ENTERSCOPE (1u << 4)

/**** the timeout values ****/
#define	__TMS_USBPUMP_HCD_TIMEOUT_NONE		0
#define	__TMS_USBPUMP_HCD_TIMEOUT_DEFAULT	1
#define	__TMS_USBPUMP_HCD_TIMEOUT_MIN		2
#define	__TMS_USBPUMP_HCD_TIMEOUT_MAX		__TMS_UINT16_MAX


/****************************************************************************\
|
|	The requests for controlling a pipe
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_PIPE_CONTROL)
	{
	__TMS_USBPUMP_HCD_REQUEST_PIPE_CONTROL__HDR;
	};

/****************************************************************************\
|
|	The request used for control-pipe operations.  This includes a
|	small nonce buffer field for use by the HCD for status INs (so
|	the HCD can specify a one-byte buffer instead of a zero-byte
|	buffer).  Without this, we might suffer if the device actually
|	sends data instead of a ZLP.  Of course, not all HCDs will need this,
|	but it adds little cost.
|
\****************************************************************************/

#define	__TMS_USBPUMP_HCD_REQUEST_CONTROL_IO__HDR(BufType)	\
	__TMS_USBPUMP_HCD_REQUEST_PIPE_IO__HDR(BufType);	\
								\
	/* the setup packet */					\
	__TMS_USETUP_WIRE		Setup;			\
								\
	/* a buffer for the status phase */			\
	__TMS_UINT8			StatusBuffer[1]


struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_CONTROL_IN)
	{
	__TMS_USBPUMP_HCD_REQUEST_CONTROL_IO__HDR(__TMS_VOID *);
	};

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_CONTROL_OUT)
	{
	__TMS_USBPUMP_HCD_REQUEST_CONTROL_IO__HDR(__TMS_CONST __TMS_VOID *);
	};

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_CONTROL_INOUT)
	{
	__TMS_USBPUMP_HCD_REQUEST_CONTROL_IO__HDR(__TMS_USBPUMP_HCD_REQUEST_BUFPTR);
	};

/****************************************************************************\
|
|	The request used for bulk or interrupt operations
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_BULKINT_IN)
	{
	__TMS_USBPUMP_HCD_REQUEST_PIPE_IO__HDR(__TMS_VOID *);
	};

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_BULKINT_OUT)
	{
	__TMS_USBPUMP_HCD_REQUEST_PIPE_IO__HDR(__TMS_CONST __TMS_VOID *);
	};

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_BULKINT_INOUT)
	{
	__TMS_USBPUMP_HCD_REQUEST_PIPE_IO__HDR(__TMS_USBPUMP_HCD_REQUEST_BUFPTR);
	};

/****************************************************************************\
|
|	The request used for bulk or interrupt stream operations
|
\****************************************************************************/
#define	__TMS_USBPUMP_HCD_BULKINT_STREAM_REQUEST__HDR(BufType)	\
	__TMS_USBPUMP_HCD_REQUEST_PIPE_IO__HDR(BufType);	\
	__TMS_UINT16	StreamID
/* end */

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_BULKINT_STREAM_IN)
	{
	__TMS_USBPUMP_HCD_BULKINT_STREAM_REQUEST__HDR(__TMS_VOID *);
	};

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_BULKINT_STREAM_OUT)
	{
	__TMS_USBPUMP_HCD_BULKINT_STREAM_REQUEST__HDR(__TMS_CONST __TMS_VOID *);
	};

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_BULKINT_STREAM_INOUT)
	{
	__TMS_USBPUMP_HCD_BULKINT_STREAM_REQUEST__HDR(
		__TMS_USBPUMP_HCD_REQUEST_BUFPTR
		);
	};

/****************************************************************************\
|
|	The request used for isochronous operations
|
\****************************************************************************/

#define	__TMS_USBPUMP_HCD_ISOCH_REQUEST__HDR(BufType)		\
	__TMS_USBPUMP_HCD_REQUEST_PIPE_IO__HDR(BufType);	\
								\
	__TMS_USBPUMP_ISOCH_PACKET_DESCR	*pIsochDescr;	\
	__TMS_USBPUMP_BUFFER_HANDLE		hIsochDescr;	\
	__TMS_UINT32			IsochFrame;		\
	__TMS_BYTES			sizeIsochDescr;		\
	__TMS_BYTES			nIsochErrs
/* end */

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_ISOCH_IN)
	{
	__TMS_USBPUMP_HCD_ISOCH_REQUEST__HDR(__TMS_VOID *);
	};

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_ISOCH_OUT)
	{
	__TMS_USBPUMP_HCD_ISOCH_REQUEST__HDR(__TMS_CONST __TMS_VOID *);
	};

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_ISOCH_INOUT)
	{
	__TMS_USBPUMP_HCD_ISOCH_REQUEST__HDR(__TMS_USBPUMP_HCD_REQUEST_BUFPTR);
	};

/****************************************************************************\
|
|	The request used for get frame operation
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_GET_FRAME)
	{
	__TMS_USBPUMP_HCD_REQUEST__HDR;

	/*
	|| the "standard" frame count, in milliseconds (always compatible
	|| with full speed), as a 32-bit number.
	*/
	__TMS_UINT32				StandardFrame;

	/*
	|| the "native" frame count, (microframes for high speed, frames for
	|| full speed) -- as a 64-bit number
	*/
	__TMS_UINT64				NativeFrame;

	/*
	|| numerator and denominator for converting native frame count to
	|| standard frame count:  1/1 for full speed, 1/8 for high speed, and
	|| other values for Wireless USB or interchip USB.  (Native Wireless
	|| USB is 1000/1024, simplified to 125/128; and for interchip USB there
	|| are a number of possible values.)
	*/
	__TMS_UINT32				Numerator;
	__TMS_UINT32				Denominator;
	};

/****************************************************************************\
|
|	The request used for set address operation
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_SET_ADDRESS)
	{
	__TMS_USBPUMP_HCD_REQUEST__HDR;

	__TMS_USBPUMP_HCD_DEVICE *		pHcdDevice;

	/*
	|| Send SET_ADDRESS request command to device flag:
	|| If this flag is TRUE, HCD should schedule SetAddress command
	|| to the device. Otherwise, HCD should not send SetAddress
	|| USB command to the device. Instead it just needs to do some
	|| other necessary jobs. For example, if HCD is HCD_XHCI, when
	|| this flags is TRUE, HCD_XHCI clears the Block Set Address Request
	|| (BSR) flag in the Address Device Command TRB and let the xHc
	|| send to SetAddress command. If this flag is FALSE, HCD_XHCI
	|| should set the BSR flag to block the hardware to set SetAddress
	|| request. It just allocates device context and asks xHci for the SlotID.
	*/
	__TMS_BOOL				fSendSetAddressRequest;

	/*
	|| Slot ID and selected device address returned by HCD
	*/
	__TMS_UINT8				bSlotId;
	__TMS_UINT8				bSelectedDeviceAddress;
	};

/****************************************************************************\
|
|	The request used for reset device operation
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_RESET_DEVICE)
	{
	__TMS_USBPUMP_HCD_REQUEST__HDR;

	__TMS_USBPUMP_HCD_DEVICE *		pHcdDevice;
	};

/****************************************************************************\
|
|	The request used for pause device operation
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_DEVICE_PAUSE)
	{
	__TMS_USBPUMP_HCD_REQUEST__HDR;

	__TMS_USBPUMP_HCD_DEVICE *		pHcdDevice;
	__TMS_BOOL				fDevicePause;
	__TMS_BOOL				fForL1Suspend;
	};

/****************************************************************************\
|
|	The request used for function wake operation
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_FUNCTION_WAKE)
	{
	__TMS_USBPUMP_HCD_REQUEST__HDR;

	__TMS_USBPUMP_HCD_DEVICE *		pHcdDevice;
	__TMS_UINT8				bSlotId;
	};

/****************************************************************************\
|
|	The request used for control pipe operation
|
\****************************************************************************/

/*
|| This is for supporting WiGig WSE protocol implementation; we define a
|| separate sub-request code to minimize code footprint in normal USBD/HCD
|| integrations.  We use multiple subcodes because otherwise we'd have to
|| have multiple NULL pointers in the function tables that HCDKIT uses for
|| dispatching requests.
*/
struct __TMS_STRUCTNAME(USBPUMP_HCD_REQUEST_CONTROL_PIPE)
	{
	__TMS_USBPUMP_HCD_REQUEST_PIPE_CONTROL__HDR;

	__TMS_UINT				SubRequest;
	};

/* Pipe control sub-request code definition */
#define	__TMS_USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_ACTIVATE		0
#define	__TMS_USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_INACTIVATE		1
#define	__TMS_USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_CLEAR_TRANSFER	2
#define	__TMS_USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_RESTART		3


/****************************************************************************\
|
|	The union of all of the above.
|
\****************************************************************************/

union __TMS_UNIONNAME(USBPUMP_HCD_REQUEST)
	{
	__TMS_USBPUMP_HCD_REQUEST_HDR			Hdr;
	__TMS_USBPUMP_HCD_REQUEST_PIPE_CONTROL		PipeControl;
	__TMS_USBPUMP_HCD_REQUEST_CONTROL_IN		ControlIn;
	__TMS_USBPUMP_HCD_REQUEST_CONTROL_OUT		ControlOut;
	__TMS_USBPUMP_HCD_REQUEST_BULKINT_IN		BulkIntIn;
	__TMS_USBPUMP_HCD_REQUEST_BULKINT_OUT		BulkIntOut;
	__TMS_USBPUMP_HCD_REQUEST_BULKINT_STREAM_IN	BulkIntStreamIn;
	__TMS_USBPUMP_HCD_REQUEST_BULKINT_STREAM_OUT	BulkIntStreamOut;
	__TMS_USBPUMP_HCD_REQUEST_ISOCH_IN		IsochIn;
	__TMS_USBPUMP_HCD_REQUEST_ISOCH_OUT		IsochOut;
	__TMS_USBPUMP_HCD_REQUEST_GET_FRAME		GetFrame;
	__TMS_USBPUMP_HCD_REQUEST_SET_ADDRESS		SetAddress;
	__TMS_USBPUMP_HCD_REQUEST_RESET_DEVICE		ResetDevice;
	__TMS_USBPUMP_HCD_REQUEST_DEVICE_PAUSE		DevicePause;
	__TMS_USBPUMP_HCD_REQUEST_FUNCTION_WAKE		FunctionWake;
	__TMS_USBPUMP_HCD_REQUEST_CONTROL_PIPE		ControlPipe;

	/*
	|| the following are for taking generic views, not for a specific
	|| request
	*/
	__TMS_USBPUMP_HCD_REQUEST_PIPE_INOUT	PipeInOut;
	__TMS_USBPUMP_HCD_REQUEST_CONTROL_INOUT	ControlInOut;
	__TMS_USBPUMP_HCD_REQUEST_BULKINT_INOUT	BulkIntInOut;
	__TMS_USBPUMP_HCD_REQUEST_BULKINT_STREAM_INOUT	BulkIntStreamInOut;
	__TMS_USBPUMP_HCD_REQUEST_ISOCH_INOUT	IsochInOut;
	};


/****************************************************************************\
|
|	USBPUMP_HCD_REQUEST_ALIGNMENT definition (UDP-4312)
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCD_REQUEST_ALIGNMENT_STRUCT);
struct __TMS_STRUCTNAME	(USBPUMP_HCD_REQUEST_ALIGNMENT_STRUCT)
	{
	char				ThisGetsUsOnAnOddAddress;
	__TMS_USBPUMP_HCD_REQUEST	HcdRequest;
	};

#define __TMS_USBPUMP_HCD_REQUEST_ALIGNMENT	\
	__TMS_OFFSET(__TMS_USBPUMP_HCD_REQUEST_ALIGNMENT_STRUCT, HcdRequest)


/****************************************************************************\
|
|	Functions/macros
|
\****************************************************************************/

/* check to see if the request has a pipe control header */
#define	__TMS_USBPUMP_HCD_RQ_HAS_PIPE(Req)				\
	(((1 << (Req)) & ((1 << __TMS_USBPUMP_HCD_RQ_BULK_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_BULK_OUT) |	\
                          (1 << __TMS_USBPUMP_HCD_RQ_BULK_STREAM_IN) |	\
                          (1 << __TMS_USBPUMP_HCD_RQ_BULK_STREAM_OUT) |	\
			  (1 << __TMS_USBPUMP_HCD_RQ_INT_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_INT_OUT) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_CTL_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_CTL_OUT) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_ISO_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_ISO_OUT) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_INIT_PIPE) |	\
			  (1 << __TMS_USBPUMP_HCD_RQ_DEINIT_PIPE) |	\
			  (1 << __TMS_USBPUMP_HCD_RQ_UPDATE_PIPE) |	\
			  (1 << __TMS_USBPUMP_HCD_RQ_CONTROL_PIPE))) != 0)

/* check to see if the request has a buffer */
#define	__TMS_USBPUMP_HCD_RQ_HAS_BUFFER(Req)				\
	(((1 << (Req)) & ((1 << __TMS_USBPUMP_HCD_RQ_BULK_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_BULK_OUT) |	\
                          (1 << __TMS_USBPUMP_HCD_RQ_BULK_STREAM_IN) |	\
                          (1 << __TMS_USBPUMP_HCD_RQ_BULK_STREAM_OUT) |	\
			  (1 << __TMS_USBPUMP_HCD_RQ_INT_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_INT_OUT) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_CTL_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_CTL_OUT) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_ISO_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_ISO_OUT))) != 0)

/* check to see if the request is isoch */
#define	__TMS_USBPUMP_HCD_RQ_IS_ISO(Req)				\
	(((1 << (Req)) & ((1 << __TMS_USBPUMP_HCD_RQ_ISO_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_ISO_OUT))) != 0)

/* check to see if the request is control */
#define __TMS_USBPUMP_HCD_RQ_IS_CTL(Req)				\
	(((1 << (Req)) & ((1 << __TMS_USBPUMP_HCD_RQ_CTL_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_CTL_OUT))) != 0)

/* check to see if the request bulk or int */
#define	__TMS_USBPUMP_HCD_RQ_IS_BULKINT(Req)				\
	(((1 << (Req)) & ((1 << __TMS_USBPUMP_HCD_RQ_BULK_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_BULK_OUT) |	\
                          (1 << __TMS_USBPUMP_HCD_RQ_BULK_STREAM_IN) |	\
                          (1 << __TMS_USBPUMP_HCD_RQ_BULK_STREAM_OUT) |	\
			  (1 << __TMS_USBPUMP_HCD_RQ_INT_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_INT_OUT))) != 0)

/* check to see if the request is stream */
#define	__TMS_USBPUMP_HCD_RQ_IS_STREAM(Req)				\
	(((1 << (Req)) & ((1 << __TMS_USBPUMP_HCD_RQ_BULK_STREAM_IN) |	\
			  (1 << __TMS_USBPUMP_HCD_RQ_BULK_STREAM_OUT))) != 0)

/* check to see if the pipe is stalled */
#define	__TMS_USBPUMP_HCD_RQ_VALIDATE_PIPE(Req)				\
	(((1 << (Req)) & ((1 << __TMS_USBPUMP_HCD_RQ_BULK_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_BULK_OUT) |	\
                          (1 << __TMS_USBPUMP_HCD_RQ_BULK_STREAM_IN) |	\
                          (1 << __TMS_USBPUMP_HCD_RQ_BULK_STREAM_OUT) |	\
			  (1 << __TMS_USBPUMP_HCD_RQ_INT_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_INT_OUT) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_ISO_IN) |		\
			  (1 << __TMS_USBPUMP_HCD_RQ_ISO_OUT))) != 0)

/* basic function for preparing a request header */
#define	UsbPumpHcdI_PrepareRequestHeader(				\
	A_pRequest, A_Length, A_Code, A_Timeout, A_Flags		\
	)  do	{							\
		(A_pRequest)->Hdr.Length =				\
			(__TMS_USBPUMP_HCD_REQUEST_LENGTH)(A_Length);	\
		(A_pRequest)->Hdr.Request =				\
			(__TMS_USBPUMP_HCD_REQUEST_CODE)(A_Code);	\
		(A_pRequest)->Hdr.pNext = (A_pRequest);			\
		(A_pRequest)->Hdr.pLast = (A_pRequest);			\
		/* submit request inits pDoneFn */			\
		/* submit request inits pDoneInfo */			\
		/* submit request inits pCancelFn */			\
		/* submit request inits pCancelInfo */			\
		/* submit request inits pHcd */				\
		/* submit request inits Timer, but needs Ticks */	\
		(A_pRequest)->Hdr.TimeoutTimer.Ticks = (A_Timeout);	\
		/* submit request inits status */			\
		(A_pRequest)->Hdr.Flags = (A_Flags);			\
		/* (A_pRequest)->Hdr.Refcount = 0; */			\
		} while (0)

/****************************************************************************\
|
|	Functions to prepare Pipe Control Requests
|
\****************************************************************************/

#define	UsbPumpHcdI_PreparePipeControlRequestHeader(			\
	A_pRequest, A_Code, A_pHcdPipe, A_Timeout, A_Flags		\
	)  do	{							\
		UsbPumpHcdI_PrepareRequestHeader(			\
			(A_pRequest),					\
			sizeof(*(A_pRequest)),				\
			(A_Code),					\
			(A_Timeout),					\
			(A_Flags)					\
			);						\
		(A_pRequest)->PipeControl.pPipe = (A_pHcdPipe);		\
		} while (0)

#define	UsbPumpHcd_PreparePipeInitRequest(				\
	A_pRequest, A_pHcdPipe, A_Flags					\
	)  do	{							\
		UsbPumpHcdI_PreparePipeControlRequestHeader(		\
			(A_pRequest),					\
			__TMS_USBPUMP_HCD_RQ_INIT_PIPE,			\
			(A_pHcdPipe),					\
			__TMS_USBPUMP_HCD_TIMEOUT_DEFAULT,		\
			(A_Flags)					\
			);						\
		} while (0)

#define	UsbPumpHcd_PreparePipeDeInitRequest(				\
	A_pRequest, A_pHcdPipe, A_Flags					\
	)  do	{							\
		UsbPumpHcdI_PreparePipeControlRequestHeader(		\
			(A_pRequest),					\
			__TMS_USBPUMP_HCD_RQ_DEINIT_PIPE,		\
			(A_pHcdPipe),					\
			__TMS_USBPUMP_HCD_TIMEOUT_DEFAULT,		\
			A_Flags						\
			);						\
		} while (0)

#define	UsbPumpHcd_PreparePipeUpdateRequest(				\
	A_pRequest, A_pHcdPipe, A_Flags					\
	)  do	{							\
		UsbPumpHcdI_PreparePipeControlRequestHeader(		\
			(A_pRequest),					\
			__TMS_USBPUMP_HCD_RQ_UPDATE_PIPE,		\
			(A_pHcdPipe),					\
			__TMS_USBPUMP_HCD_TIMEOUT_DEFAULT,		\
			(A_Flags)					\
			);						\
		} while (0)

#define	UsbPumpHcd_PreparePipeControlRequest(				\
	A_pRequest, A_pHcdPipe, A_SubRequest, A_Flags			\
	)  do	{							\
		UsbPumpHcdI_PreparePipeControlRequestHeader(		\
			(A_pRequest),					\
			__TMS_USBPUMP_HCD_RQ_CONTROL_PIPE,		\
			(A_pHcdPipe),					\
			__TMS_USBPUMP_HCD_TIMEOUT_DEFAULT,		\
			(A_Flags)					\
			);						\
		(A_pRequest)->ControlPipe.SubRequest = (A_SubRequest);	\
		} while (0)


/****************************************************************************\
|
|	Functions to prepare Control I/O requests
|
\****************************************************************************/

#define	UsbPumpHcdI_PrepareControlIORequestHeader(			\
	A_pRequest, A_Code, A_Struct, A_pHcdPipe,			\
	A_Timeout, A_Flags,						\
	A_pSetup, A_pBuffer, A_nBuffer,					\
	A_hBuffer							\
	)  do	{							\
		UsbPumpHcdI_PrepareRequestHeader(			\
			(A_pRequest),					\
			sizeof(*(A_pRequest)),				\
			(A_Code),					\
			(A_Timeout),					\
			(A_Flags)					\
			);						\
		(A_pRequest)->A_Struct.pPipe = (A_pHcdPipe);		\
		(A_pRequest)->A_Struct.pBuffer = (A_pBuffer);		\
		(A_pRequest)->A_Struct.nBuffer = (A_nBuffer);		\
		(A_pRequest)->A_Struct.hBuffer = (A_hBuffer);		\
		(A_pRequest)->A_Struct.Setup = *(A_pSetup);		\
		} while (0)

#define	UsbPumpHcd_PrepareControlInRequest(				\
	A_pRequest, A_pHcdPipe, A_Timeout, A_Flags,			\
	A_pSetup, A_pBuffer, A_nBuffer					\
	)  do	{							\
		UsbPumpHcdI_PrepareControlIORequestHeader(		\
			(A_pRequest),					\
			__TMS_USBPUMP_HCD_RQ_CTL_IN,			\
			ControlIn,					\
			(A_pHcdPipe),					\
			(A_Timeout),					\
			(A_Flags),					\
			(A_pSetup), (A_pBuffer), (A_nBuffer),		\
			/* hBuffer */ __TMS_NULL			\
			);						\
		} while (0)

#define	UsbPumpHcd_PrepareControlInRequest_V2(				\
	A_pRequest, A_pHcdPipe, A_Timeout, A_Flags,			\
	A_pSetup, A_pBuffer, A_nBuffer,					\
	A_hBuffer							\
	)  do	{							\
		UsbPumpHcdI_PrepareControlIORequestHeader(		\
			(A_pRequest),					\
			__TMS_USBPUMP_HCD_RQ_CTL_IN,			\
			ControlIn,					\
			(A_pHcdPipe),					\
			(A_Timeout),					\
			(A_Flags),					\
			(A_pSetup), (A_pBuffer), (A_nBuffer),		\
			(A_hBuffer)					\
			);						\
		} while (0)

#define	UsbPumpHcd_PrepareControlOutRequest(				\
	A_pRequest, A_pHcdPipe, A_Timeout, A_Flags,			\
	A_pSetup, A_pBuffer, A_nBuffer					\
	)  do	{							\
		UsbPumpHcdI_PrepareControlIORequestHeader(		\
			(A_pRequest),					\
			__TMS_USBPUMP_HCD_RQ_CTL_OUT,			\
			ControlOut,					\
			(A_pHcdPipe),					\
			(A_Timeout),					\
			(A_Flags),					\
			(A_pSetup),					\
			(A_pBuffer), (A_nBuffer),			\
			/* hBuffer */ __TMS_NULL			\
			);						\
		} while (0)

#define	UsbPumpHcd_PrepareControlOutRequest_V2(				\
	A_pRequest, A_pHcdPipe, A_Timeout, A_Flags,			\
	A_pSetup, A_pBuffer, A_nBuffer,					\
	A_hBuffer							\
	)  do	{							\
		UsbPumpHcdI_PrepareControlIORequestHeader(		\
			(A_pRequest),					\
			__TMS_USBPUMP_HCD_RQ_CTL_OUT,			\
			ControlOut,					\
			(A_pHcdPipe),					\
			(A_Timeout),					\
			(A_Flags),					\
			(A_pSetup),					\
			(A_pBuffer), (A_nBuffer),			\
			(A_hBuffer)					\
			);						\
		} while (0)

#define	UsbPumpHcdI_PrepareAsyncIORequestHeader(			\
	A_pRequest, A_Code, A_Struct, A_pHcdPipe,			\
	A_Timeout, A_Flags,						\
	A_pBuffer, A_nBuffer,						\
	A_hBuffer							\
	)  do	{							\
		UsbPumpHcdI_PrepareRequestHeader(			\
			(A_pRequest),					\
			sizeof(*(A_pRequest)),				\
			(A_Code),					\
			(A_Timeout),					\
			(A_Flags)					\
			);						\
		(A_pRequest)->A_Struct.pPipe = (A_pHcdPipe);		\
		(A_pRequest)->A_Struct.pBuffer = (A_pBuffer);		\
		(A_pRequest)->A_Struct.nBuffer = (A_nBuffer);		\
		(A_pRequest)->A_Struct.hBuffer = (A_hBuffer);		\
		} while (0)

#define	UsbPumpHcd_PrepareBulkIntInRequest(				\
	A_pRequest, A_Code, A_pHcdPipe,					\
	A_Timeout, A_Flags,						\
	A_pBuffer, A_nBuffer						\
	)								\
	UsbPumpHcdI_PrepareAsyncIORequestHeader(			\
		A_pRequest, A_Code,					\
		BulkIntIn,						\
		A_pHcdPipe,						\
		A_Timeout, A_Flags,					\
		A_pBuffer, A_nBuffer,					\
		/* hBuffer */ __TMS_NULL				\
	)

#define	UsbPumpHcd_PrepareBulkIntInRequest_V2(				\
	A_pRequest, A_Code, A_pHcdPipe,					\
	A_Timeout, A_Flags,						\
	A_pBuffer, A_nBuffer,						\
	A_hBuffer							\
	)								\
	UsbPumpHcdI_PrepareAsyncIORequestHeader(			\
		A_pRequest, A_Code,					\
		BulkIntIn,						\
		A_pHcdPipe,						\
		A_Timeout, A_Flags,					\
		A_pBuffer, A_nBuffer,					\
		A_hBuffer						\
	)

#define	UsbPumpHcdI_PrepareStreamIORequestHeader(			\
	A_pRequest, A_Code, A_Struct, A_pHcdPipe,			\
	A_Timeout, A_Flags,						\
	A_pBuffer, A_nBuffer,						\
	A_hBuffer, A_StreamID						\
	)  do	{							\
		UsbPumpHcdI_PrepareRequestHeader(			\
			(A_pRequest),					\
			sizeof(*(A_pRequest)),				\
			(A_Code),					\
			(A_Timeout),					\
			(A_Flags)					\
			);						\
		(A_pRequest)->A_Struct.pPipe = (A_pHcdPipe);		\
		(A_pRequest)->A_Struct.pBuffer = (A_pBuffer);		\
		(A_pRequest)->A_Struct.nBuffer = (A_nBuffer);		\
		(A_pRequest)->A_Struct.hBuffer = (A_hBuffer);		\
		(A_pRequest)->A_Struct.StreamID = (A_StreamID);		\
		} while (0)

#define	UsbPumpHcd_PrepareBulkIntStreamInRequest(			\
	A_pRequest, A_Code, A_pHcdPipe,					\
	A_Timeout, A_Flags,						\
	A_pBuffer, A_nBuffer,						\
	A_hBuffer, A_StreamID						\
	)								\
	UsbPumpHcdI_PrepareStreamIORequestHeader(			\
		A_pRequest, A_Code,					\
		BulkIntStreamIn,					\
		A_pHcdPipe,						\
		A_Timeout, A_Flags,					\
		A_pBuffer, A_nBuffer,					\
		A_hBuffer, A_StreamID					\
	)

#define	UsbPumpHcd_PrepareBulkIntOutRequest(				\
	A_pRequest, A_Code, A_pHcdPipe,					\
	A_Timeout, A_Flags,						\
	A_pBuffer, A_nBuffer						\
	)								\
	UsbPumpHcdI_PrepareAsyncIORequestHeader(			\
		A_pRequest, A_Code,					\
		BulkIntOut,						\
		A_pHcdPipe,						\
		A_Timeout, A_Flags,					\
		A_pBuffer, A_nBuffer,					\
		/* hBuffer */ __TMS_NULL				\
	)

#define	UsbPumpHcd_PrepareBulkIntOutRequest_V2(				\
	A_pRequest, A_Code, A_pHcdPipe,					\
	A_Timeout, A_Flags,						\
	A_pBuffer, A_nBuffer,						\
	A_hBuffer							\
	)								\
	UsbPumpHcdI_PrepareAsyncIORequestHeader(			\
		A_pRequest, A_Code,					\
		BulkIntOut,						\
		A_pHcdPipe,						\
		A_Timeout, A_Flags,					\
		A_pBuffer, A_nBuffer,					\
		A_hBuffer						\
	)

#define	UsbPumpHcd_PrepareBulkIntStreamOutRequest(			\
	A_pRequest, A_Code, A_pHcdPipe,					\
	A_Timeout, A_Flags,						\
	A_pBuffer, A_nBuffer,						\
	A_hBuffer, A_StreamID						\
	)								\
	UsbPumpHcdI_PrepareStreamIORequestHeader(			\
		A_pRequest, A_Code,					\
		BulkIntStreamOut,					\
		A_pHcdPipe,						\
		A_Timeout, A_Flags,					\
		A_pBuffer, A_nBuffer,					\
		A_hBuffer, A_StreamID					\
	)

#define	UsbPumpHcdI_PrepareIsochIORequestHeader(			\
	A_pRequest, A_Code, A_Struct, A_pHcdPipe,			\
	A_Timeout, A_Flags,						\
	A_IsochStartFrame, A_pIsochDescr, A_sizeIsochDescrs,		\
	A_hIsochDescr,							\
	A_pBuffer, A_nBuffer,						\
	A_hBuffer							\
	)  do	{							\
		UsbPumpHcdI_PrepareRequestHeader(			\
			(A_pRequest),					\
			sizeof(*(A_pRequest)),				\
			(A_Code),					\
			(A_Timeout),					\
			(A_Flags)					\
			);						\
		(A_pRequest)->A_Struct.pPipe = (A_pHcdPipe);		\
		(A_pRequest)->A_Struct.pIsochDescr = (A_pIsochDescr);	\
		(A_pRequest)->A_Struct.hIsochDescr = (A_hIsochDescr);	\
		(A_pRequest)->A_Struct.IsochFrame = (A_IsochStartFrame);\
		(A_pRequest)->A_Struct.pPipe = (A_pHcdPipe);		\
		(A_pRequest)->A_Struct.pPipe = (A_pHcdPipe);		\
		(A_pRequest)->A_Struct.pBuffer = (A_pBuffer);		\
		(A_pRequest)->A_Struct.nBuffer = (A_nBuffer);		\
		(A_pRequest)->A_Struct.hBuffer = (A_hBuffer);		\
		(A_pRequest)->A_Struct.sizeIsochDescr = (A_sizeIsochDescrs); \
		} while (0)

#define	UsbPumpHcd_PrepareIsochInRequest(				\
		A_pRequest, A_pHcdPipe,					\
		A_Timeout, A_Flags,					\
		A_IsochStartFrame, A_pIsochDescr, A_sizeIsochDescrs,	\
		A_pBuffer, A_nBuffer					\
		)							\
	UsbPumpHcdI_PrepareIsochIORequestHeader(			\
		A_pRequest, __TMS_USBPUMP_HCD_RQ_ISO_IN,		\
		IsochIn,						\
		A_pHcdPipe,						\
		A_Timeout, A_Flags,					\
		A_IsochStartFrame, A_pIsochDescr, A_sizeIsochDescrs,	\
		/* hIsochDescr */ __TMS_NULL,				\
		A_pBuffer, A_nBuffer,					\
		/* hBuffer */ __TMS_NULL				\
	)

#define	UsbPumpHcd_PrepareIsochInRequest_V2(				\
		A_pRequest, A_pHcdPipe,					\
		A_Timeout, A_Flags,					\
		A_IsochStartFrame, A_pIsochDescr, A_sizeIsochDescrs,	\
		A_hIsochDescr,						\
		A_pBuffer, A_nBuffer,					\
		A_hBuffer						\
		)							\
	UsbPumpHcdI_PrepareIsochIORequestHeader(			\
		A_pRequest, __TMS_USBPUMP_HCD_RQ_ISO_IN,		\
		IsochIn,						\
		A_pHcdPipe,						\
		A_Timeout, A_Flags,					\
		A_IsochStartFrame, A_pIsochDescr, A_sizeIsochDescrs,	\
		A_hIsochDescr,						\
		A_pBuffer, A_nBuffer,					\
		A_hBuffer						\
	)

#define	UsbPumpHcd_PrepareIsochOutRequest(				\
		A_pRequest, A_pHcdPipe,					\
		A_Timeout, A_Flags,					\
		A_IsochStartFrame, A_pIsochDescr, A_sizeIsochDescrs,	\
		A_pBuffer, A_nBuffer					\
		)							\
	UsbPumpHcdI_PrepareIsochIORequestHeader(			\
		A_pRequest, __TMS_USBPUMP_HCD_RQ_ISO_OUT,		\
		IsochOut,						\
		A_pHcdPipe,						\
		A_Timeout, A_Flags,					\
		A_IsochStartFrame, A_pIsochDescr, A_sizeIsochDescrs,	\
		/* hIsochDescr */ __TMS_NULL,				\
		A_pBuffer, A_nBuffer,					\
		/* hBuffer */ __TMS_NULL				\
	)

#define	UsbPumpHcd_PrepareIsochOutRequest_V2(				\
		A_pRequest, A_pHcdPipe,					\
		A_Timeout, A_Flags,					\
		A_IsochStartFrame, A_pIsochDescr, A_sizeIsochDescrs,	\
		A_hIsochDescr,						\
		A_pBuffer, A_nBuffer,					\
		A_hBuffer						\
		)							\
	UsbPumpHcdI_PrepareIsochIORequestHeader(			\
		A_pRequest, __TMS_USBPUMP_HCD_RQ_ISO_OUT,		\
		IsochOut,						\
		A_pHcdPipe,						\
		A_Timeout, A_Flags,					\
		A_IsochStartFrame, A_pIsochDescr, A_sizeIsochDescrs,	\
		A_hIsochDescr,						\
		A_pBuffer, A_nBuffer,					\
		A_hBuffer						\
	)

#define	UsbPumpHcd_PrepareGetFrameRequest(				\
	A_pRequest,							\
	A_Timeout, A_Flags						\
	)								\
	UsbPumpHcdI_PrepareRequestHeader(				\
		(A_pRequest),						\
		sizeof(*(A_pRequest)),					\
		__TMS_USBPUMP_HCD_RQ_GET_FRAME,				\
		(A_Timeout),						\
		(A_Flags)						\
		)

#define	UsbPumpHcd_PrepareSetAddressRequest(				\
	A_pRequest, A_pHcdDevice,					\
	A_Timeout, A_Flags, A_fSendSetAddressRequest			\
	) do	{							\
		UsbPumpHcdI_PrepareRequestHeader(			\
			(A_pRequest),					\
			sizeof(*(A_pRequest)),				\
			__TMS_USBPUMP_HCD_RQ_SET_ADDRESS,		\
			(A_Timeout),					\
			(A_Flags)					\
			);						\
		(A_pRequest)->SetAddress.pHcdDevice = (A_pHcdDevice);	\
		(A_pRequest)->SetAddress.fSendSetAddressRequest =	\
			(A_fSendSetAddressRequest);			\
		(A_pRequest)->SetAddress.bSlotId = 0;			\
		(A_pRequest)->SetAddress.bSelectedDeviceAddress = 0xff;	\
		} while (0)

#define	UsbPumpHcd_PrepareResetDeviceRequest(				\
	A_pRequest, A_pHcdDevice,					\
	A_Timeout, A_Flags						\
	) do	{							\
		UsbPumpHcdI_PrepareRequestHeader(			\
			(A_pRequest),					\
			sizeof(*(A_pRequest)),				\
			__TMS_USBPUMP_HCD_RQ_RESET_DEVICE,		\
			(A_Timeout),					\
			(A_Flags)					\
			);						\
		(A_pRequest)->ResetDevice.pHcdDevice = (A_pHcdDevice);	\
		} while (0)

#define	UsbPumpHcd_PrepareDevicePauseRequest_V2(			\
	A_pRequest, A_pHcdDevice, A_fDevicePause, A_fForL1Suspend,	\
	A_Timeout, A_Flags						\
	) do	{							\
		UsbPumpHcdI_PrepareRequestHeader(			\
			(A_pRequest),					\
			sizeof(*(A_pRequest)),				\
			__TMS_USBPUMP_HCD_RQ_DEVICE_PAUSE,		\
			(A_Timeout),					\
			(A_Flags)					\
			);						\
		(A_pRequest)->DevicePause.pHcdDevice = (A_pHcdDevice);	\
		(A_pRequest)->DevicePause.fDevicePause = (A_fDevicePause); \
		(A_pRequest)->DevicePause.fForL1Suspend = (A_fForL1Suspend); \
		} while (0)

#define	UsbPumpHcd_PrepareDevicePauseRequest(				\
	A_pRequest, A_pHcdDevice, A_fDevicePause,			\
	A_Timeout, A_Flags						\
	)								\
	UsbPumpHcd_PrepareDevicePauseRequest_V2(			\
	A_pRequest, A_pHcdDevice, A_fDevicePause,			\
	/* V2: fForL1Suspend */ __TMS_FALSE,				\
	A_Timeout, A_Flags						\
	)

#define	UsbPumpHcd_PrepareFunctionWakeRequest(				\
	A_pRequest, A_pHcdDevice, A_bSlotId,				\
	A_Timeout, A_Flags						\
	) do	{							\
		UsbPumpHcdI_PrepareRequestHeader(			\
			(A_pRequest),					\
			sizeof(*(A_pRequest)),				\
			__TMS_USBPUMP_HCD_RQ_FUNCTION_WAKE,		\
			(A_Timeout),					\
			(A_Flags)					\
			);						\
		(A_pRequest)->FunctionWake.pHcdDevice = (A_pHcdDevice);	\
		(A_pRequest)->FunctionWake.bSlotId = (A_bSlotId);	\
		} while (0)


/****************************************************************************\
|
|	Real function prototypes.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_CONST __TMS_TEXT *
UsbPumpHcdRequest_RequestName(
	__TMS_USBPUMP_HCD_REQUEST_CODE Request
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_HCD_RQ_BULK_IN	\
   __TMS_USBPUMP_HCD_RQ_BULK_IN
# define USBPUMP_HCD_RQ_BULK_OUT	\
   __TMS_USBPUMP_HCD_RQ_BULK_OUT
# define USBPUMP_HCD_RQ_INT_IN	\
   __TMS_USBPUMP_HCD_RQ_INT_IN
# define USBPUMP_HCD_RQ_INT_OUT	\
   __TMS_USBPUMP_HCD_RQ_INT_OUT
# define USBPUMP_HCD_RQ_CTL_IN	\
   __TMS_USBPUMP_HCD_RQ_CTL_IN
# define USBPUMP_HCD_RQ_CTL_OUT	\
   __TMS_USBPUMP_HCD_RQ_CTL_OUT
# define USBPUMP_HCD_RQ_ISO_IN	\
   __TMS_USBPUMP_HCD_RQ_ISO_IN
# define USBPUMP_HCD_RQ_ISO_OUT	\
   __TMS_USBPUMP_HCD_RQ_ISO_OUT
# define USBPUMP_HCD_RQ_INIT_PIPE	\
   __TMS_USBPUMP_HCD_RQ_INIT_PIPE
# define USBPUMP_HCD_RQ_DEINIT_PIPE	\
   __TMS_USBPUMP_HCD_RQ_DEINIT_PIPE
# define USBPUMP_HCD_RQ_UPDATE_PIPE	\
   __TMS_USBPUMP_HCD_RQ_UPDATE_PIPE
# define USBPUMP_HCD_RQ_GET_FRAME	\
   __TMS_USBPUMP_HCD_RQ_GET_FRAME
# define USBPUMP_HCD_RQ_BULK_STREAM_IN	\
   __TMS_USBPUMP_HCD_RQ_BULK_STREAM_IN
# define USBPUMP_HCD_RQ_BULK_STREAM_OUT	\
   __TMS_USBPUMP_HCD_RQ_BULK_STREAM_OUT
# define USBPUMP_HCD_RQ_SET_ADDRESS	\
   __TMS_USBPUMP_HCD_RQ_SET_ADDRESS
# define USBPUMP_HCD_RQ_RESET_DEVICE	\
   __TMS_USBPUMP_HCD_RQ_RESET_DEVICE
# define USBPUMP_HCD_RQ_DEVICE_PAUSE	\
   __TMS_USBPUMP_HCD_RQ_DEVICE_PAUSE
# define USBPUMP_HCD_RQ_FUNCTION_WAKE	\
   __TMS_USBPUMP_HCD_RQ_FUNCTION_WAKE
# define USBPUMP_HCD_RQ_CONTROL_PIPE	\
   __TMS_USBPUMP_HCD_RQ_CONTROL_PIPE
# define USBPUMP_HCD_RQ__MAX	\
   __TMS_USBPUMP_HCD_RQ__MAX
# define USBPUMP_HCD_RQ__INIT	\
   __TMS_USBPUMP_HCD_RQ__INIT
# define USBPUMP_HCD_REQUEST__HDR	\
   __TMS_USBPUMP_HCD_REQUEST__HDR
# define USBPUMP_HCD_REQUEST_PIPE_CONTROL__HDR	\
   __TMS_USBPUMP_HCD_REQUEST_PIPE_CONTROL__HDR
# define USBPUMP_HCD_REQUEST_PIPE_IO__HDR(BufType)	\
   __TMS_USBPUMP_HCD_REQUEST_PIPE_IO__HDR(BufType)
# define USBPUMP_HCD_REQUEST_FLAG_ASAP	\
   __TMS_USBPUMP_HCD_REQUEST_FLAG_ASAP
# define USBPUMP_HCD_REQUEST_FLAG_SHORT_OK	\
   __TMS_USBPUMP_HCD_REQUEST_FLAG_SHORT_OK
# define USBPUMP_HCD_REQUEST_FLAG_TEST_IMMEDIATE_COMPLETE	\
   __TMS_USBPUMP_HCD_REQUEST_FLAG_TEST_IMMEDIATE_COMPLETE
# define USBPUMP_HCD_REQUEST_FLAG_TEST_SKIP_SETUP_STAGE	\
   __TMS_USBPUMP_HCD_REQUEST_FLAG_TEST_SKIP_SETUP_STAGE
# define USBPUMP_HCD_REQUEST_FLAG_POST_BREAK	\
   __TMS_USBPUMP_HCD_REQUEST_FLAG_POST_BREAK
# define USBPUMP_HCD_REQUEST_FLAG_NOT_FIRST_OF_SET	\
   __TMS_USBPUMP_HCD_REQUEST_FLAG_NOT_FIRST_OF_SET
# define USBPUMP_HCD_REQUEST_FLAG_NOT_LAST_OF_SET	\
   __TMS_USBPUMP_HCD_REQUEST_FLAG_NOT_LAST_OF_SET
# define USBPUMP_HCD_REQUEST_FLAG_INACTIVATE_DURING_SUSPEND	\
   __TMS_USBPUMP_HCD_REQUEST_FLAG_INACTIVATE_DURING_SUSPEND
# define USBPUMP_HCD_REQUEST_FLAG_RESTART_KEEP_TRANSFER_STATE	\
   __TMS_USBPUMP_HCD_REQUEST_FLAG_RESTART_KEEP_TRANSFER_STATE
# define USBPUMP_HCD_REQUEST_INTERNAL_FLAG_CANCELLED	\
   __TMS_USBPUMP_HCD_REQUEST_INTERNAL_FLAG_CANCELLED
# define USBPUMP_HCD_REQUEST_INTERNAL_FLAG_TIMING	\
   __TMS_USBPUMP_HCD_REQUEST_INTERNAL_FLAG_TIMING
# define USBPUMP_HCD_REQUEST_INTERNAL_FLAG_TIMED_OUT	\
   __TMS_USBPUMP_HCD_REQUEST_INTERNAL_FLAG_TIMED_OUT
# define USBPUMP_HCD_REQUEST_INTERNAL_FLAG_BUFFER_ENTERSCOPE	\
   __TMS_USBPUMP_HCD_REQUEST_INTERNAL_FLAG_BUFFER_ENTERSCOPE
# define USBPUMP_HCD_REQUEST_INTERNAL_FLAG_ISOCHDESC_ENTERSCOPE	\
   __TMS_USBPUMP_HCD_REQUEST_INTERNAL_FLAG_ISOCHDESC_ENTERSCOPE
# define USBPUMP_HCD_TIMEOUT_NONE	\
   __TMS_USBPUMP_HCD_TIMEOUT_NONE
# define USBPUMP_HCD_TIMEOUT_DEFAULT	\
   __TMS_USBPUMP_HCD_TIMEOUT_DEFAULT
# define USBPUMP_HCD_TIMEOUT_MIN	\
   __TMS_USBPUMP_HCD_TIMEOUT_MIN
# define USBPUMP_HCD_TIMEOUT_MAX	\
   __TMS_USBPUMP_HCD_TIMEOUT_MAX
# define USBPUMP_HCD_REQUEST_CONTROL_IO__HDR(BufType)	\
   __TMS_USBPUMP_HCD_REQUEST_CONTROL_IO__HDR(BufType)
# define USBPUMP_HCD_BULKINT_STREAM_REQUEST__HDR(BufType)	\
   __TMS_USBPUMP_HCD_BULKINT_STREAM_REQUEST__HDR(BufType)
# define USBPUMP_HCD_ISOCH_REQUEST__HDR(BufType)	\
   __TMS_USBPUMP_HCD_ISOCH_REQUEST__HDR(BufType)
# define USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_ACTIVATE	\
   __TMS_USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_ACTIVATE
# define USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_INACTIVATE	\
   __TMS_USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_INACTIVATE
# define USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_CLEAR_TRANSFER	\
   __TMS_USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_CLEAR_TRANSFER
# define USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_RESTART	\
   __TMS_USBPUMP_HCD_REQUEST_CONTROL_PIPE_REQ_RESTART
# define USBPUMP_HCD_REQUEST_ALIGNMENT	\
   __TMS_USBPUMP_HCD_REQUEST_ALIGNMENT
# define USBPUMP_HCD_RQ_HAS_PIPE(Req)	\
   __TMS_USBPUMP_HCD_RQ_HAS_PIPE(Req)
# define USBPUMP_HCD_RQ_HAS_BUFFER(Req)	\
   __TMS_USBPUMP_HCD_RQ_HAS_BUFFER(Req)
# define USBPUMP_HCD_RQ_IS_ISO(Req)	\
   __TMS_USBPUMP_HCD_RQ_IS_ISO(Req)
# define USBPUMP_HCD_RQ_IS_CTL(Req)	\
   __TMS_USBPUMP_HCD_RQ_IS_CTL(Req)
# define USBPUMP_HCD_RQ_IS_BULKINT(Req)	\
   __TMS_USBPUMP_HCD_RQ_IS_BULKINT(Req)
# define USBPUMP_HCD_RQ_IS_STREAM(Req)	\
   __TMS_USBPUMP_HCD_RQ_IS_STREAM(Req)
# define USBPUMP_HCD_RQ_VALIDATE_PIPE(Req)	\
   __TMS_USBPUMP_HCD_RQ_VALIDATE_PIPE(Req)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

#endif /* _USBPUMP_HCD_REQUEST_H_ */
