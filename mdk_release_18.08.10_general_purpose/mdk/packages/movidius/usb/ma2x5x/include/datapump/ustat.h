/* ustat.h	Fri Mar 05 2010 15:53:27 chwon */

/*

Module:  ustat.h

Function:
	Definitions of USTAT_... codes.

Version:
	V3.01b	Fri Mar 05 2010 15:53:27 chwon	Edit level 11

Copyright notice:
	This file copyright (C) 2002, 2005-2006, 2009-2010 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.79a  Mon May 27 2002 16:50:49  tmm
	Module created.

   1.79a  7/24/2002  tmm
	600032/1.1.44:  add USTAT_INVALID_PARAM
	Fix typo.

   1.91d  2005-06-03  tmm
	USTAT_DEFINITE_LENGTH_UNDERRUN was missing from the __INIT table.
	Add host error codes.  Add type cloaking.

   1.91h  Sun Sep  4 2005 07:23:16  tmm
	Add USTAT_NOHW to indicate that HCD or DCD has been removed.

   1.93c  Wed Dec  7 2005 23:14:48  tmm
	Add USTAT_IN_USE to indicate that an operation can't be performed
	because a resource is in use.

   1.93e  Sun Jan 29 2006 22:05:20  tmm
	Add USTAT_NO_MEMORY, USTAT_NO_BANDWIDTH and USTAT_NO_BUS_POWER.

   1.93e  Mon Feb  6 2006 22:06:34  tmm
	Add USTAT_INTERNAL_ERROR.

   2.01a  Wed Jul 15 2009 13:23:51  chwon
	8460: Add USTAT_NOT_SUPPORTED.

   2.01a  Mon Dec 07 2009 15:09:56  chwon
	9468: Add USTAT_ERROR_SHORT_TRANSFER.
	9467: Add USTAT_BABBLE_DETECTED, USTAT_BAD_START_FRAME,
	USTAT_NOT_ACCESSED, USTAT_MISSED_MICRO_FRAME, USTAT_ERROR_HANDSHAKE,
	USTAT_TRANSACTION_ERROR, USTAT_FIFO_ERROR,
	USTAT_ISO_NOT_ACCESSED_BY_HW and USTAT_ISO_NOT_ACCESSED_LATE.

   3.01b  Fri Mar 05 2010 15:53:27  chwon
	9467: Add xHCI related error codes.

*/

#ifndef _USTAT_H_		/* prevent multiple includes */
#define _USTAT_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	The I/O Status Codes.
|
\****************************************************************************/

/*

Type:	USTAT

Function:
	Error codes for the USB DataPump device and host stacks.

Definition:
	typedef UCHAR		USTAT, *PUSTAT;
	typedef ARG_UCHAR	ARG_USTAT, *PARG_USTAT;

Description:
	The |USTAT| type is used to represent error codes returned by
	components within the USB DataPump device and host stacks.

	As might be expected, not all the error codes apply to every
	situation.  A rough indication of the applicability of the error
	codes is given below, along with a description.  The applicability
	is given via the notation "(host)" for host stack operations, 
	"(device)" for device stack operation, or "(device, host)" for 
	both host and device.

	|USTAT_BUSY| (device, host)
		Not an error code -- indicates that the operation is
		now in process.

	|USTAT_OK|  (device, host)
		Indicates that the operation has completed successfully.

	|USTAT_KILL| (device, host)
		Indicates that the operation was cancelled.

	|USTAT_NOTCFG| (device)
		In the device stack, this indicates that an operation
		failed because the underlying pipe has not been configured
		by the host into an operating state.

		Host: not used.

	|USTAT_IOERR| (device, host)
		Indicates that some kind of unrecoverable device
		error occurred.  Used only if no more specific error code
		or error information is availalbe.

		Host:  a packet transaction failed, and there's no better
		idea of what went wrong.

	|USTAT_REPLACED| (device)
		Device, DEPRECATED: Indicates that the UBUFQE was completed 
		because it was replaced by a different packet.  This is
		only used by the interrupt-register module, which is
		no longer supported.

		Host: not used.

	|USTAT_STALL| (device, host)
		Device:  pipe is halted (i.e., is returning STALL pids to
		the host when the host accesses it).

		Host:  a STALL pid was received, or the specified pipe is
		stalled.

	|USTAT_ISOBUF| (device)
		Device:  for an isochronous transfer, the transfer buffer
		is malformed in some way.

	|USTAT_LENGTH_OVERRUN| (device, host)
		Device:  a request was submitted with "definite length",
		and too much data was received in the transfer.

		Host:  the amount of data returned by the device exceeded
		the maximum packet size or the remaining buffer size,
		as applicable.

	|USTAT_DEFINITE_LENGTH_OVERRUN| (device, host)
		This is a synonym for |USTAT_LENGTH_OVERRUN|

	|USTAT_LENGTH_UNDERRUN| (device, host)
		Device: a request was submitted with "definite length",
		and too little data was received in the transfer.

		Host:  short packet was received, and short packets
		were not indicated to be OK on the transfer.
		New HCD should use USTAT_ERROR_SHORT_TRANSFER error code
		instead of USTAT_LENGTH_UNDERRUN.

	|USTAT_DEFINITE_LENGTH_UNDERRUN| (device, host)
		This is a synonym for |USTAT_LENGTH_UNDERRUN|.

	|USTAT_ERROR_SHORT_TRANSFER| (host)
		This is a synonym for |USTAT_LENGTH_UNDERRUN|.

	|USTAT_INVALID_PARAM| (device, host)
		An invalid parameter was received.

	|USTAT_HCD_CRC| (host)
		Device:  not used

		Host:  a packet transaction failed, and the last failure was
		a CRC error.

	|USTAT_HCD_BITSTUFF| (host)
		Host:  a packet transaction failed, and the last failure was
		a bit-stuff error.

	|USTAT_HCD_NO_RESPONSE| (host)
		Host:  a packet transaction failed, and the last failure was
		that the device did not respond.

	|USTAT_HCD_PIDCHECK| (host)
		Host:  a packet transaction failed, and the last failure was
		that the PID check bits didn't match.

	|USTAT_HCD_INVALID_PID| (host)
		Host: a packet transaction failed, and the last failure was
		that the PID was invalid.

	|USTAT_DMA_OVERRUN| (device, host)
		The hardware received data from the bus faster than it
		could stuff the data into memory, and so bytes were lost.

		This error normally only occurs on a host, as a device will
		retry.  But in certain DCDs it might transiently appear
		as an internal error.

	|USTAT_DMA_UNDERRUN| (device, host)
		The hardware could not retrieve data from system memory
		quickly enough to keep up with the USB data rate.

	|USTAT_NOHW| (device, host)
		The device controller or host controller hardware has been
		removed from the system, powered down or made inaccessible
		in some way.

	|USTAT_IN_USE| (host)
		The operation could not be performed because some other
		operation is referencing the element.  For example, this
		will be returned if the client attempts to do a SET_CONFIG
		while I/O is in progress.

	|USTAT_NO_MEMORY| (host)
		The operation could not be performed because an attempt to
		allocate from a memory pool failed.

	|USTAT_NO_BANDWIDTH| (host)
		The operation could not be performed because there's not
		enough bus bandwidth availalble.

	|USTAT_NO_BUS_POWER| (host)
		The operation could not be performed because there's not
		enough bus power available.

	|USTAT_INTERNAL_ERROR| (host, device)
		The operation could not be performed due to an internal
		consistency-check failure of some kind.

	|USTAT_NOT_SUPPORTED| (host, device)
		The operation could not be performed because not supported
		operation.

	|USTAT_BABBLE_DETECTED| (host)
		Host: a packet transaction failed, and the last failure was
		that the babble detected.

	|USTAT_BAD_START_FRAME| (host)
		The operation could not be performed because the requested
		start frame is not within a range of the current USB frame.

	|USTAT_NOT_ACCESSED| (host)
		This code indicates that a transfer descriptor for OHCI or
		similar host controllers was completed, and yet the host
		controller didn't update the status field. This error indicates
		a harware or system software problem.

	|USTAT_MISSED_MICRO_FRAME| (host)
		The operation could not be performed because host induced
		holdoff caused the host controller to miss a required
		complete-split transaction.

	|USTAT_ERROR_HANDSHAKE| (host)
		The operation could not be performed because the host
		controller received an error handshake from the hub for a
		periodic endpoint split transactoin.

	|USTAT_TRANSACTION_ERROR| (host)
		The operation failed because of a transaction error reported
		by the host controller.

	|USTAT_FIFO_ERROR| (host)
		The operation failed because of a FIFO error in the host
		controller.

	|USTAT_ISO_NOT_ACCESSED_BY_HW| (host)
		The operation was not performed because the host completed
		the transfer without updating the status. This might indicate
		a system hardware or software problem.

	|USTAT_ISO_NOT_ACCESSED_LATE| (host)
		The operation was not performed because the frame time had
		already passed.

	|USTAT_TRB_ERROR| (host)
		The operation was not performed because a TRB parameter error
		condition (e.g., out of range or invalid parameter) is detected
		in a TRB.

	|USTAT_NO_RESOURCE| (host)
		The operation was not performed because there are not adequate
		xHC resources available to enable the requested set of endpoints.

	|USTAT_NO_SLOT| (host)
		The operation was not performed because the host controller has
		no available device slot for this implementation.

	|USTAT_INVALID_STREAM_TYPE| (host)
		The operation was not performed because the host controller
		detect a invalid Stream Context Type (SCT) value.

	|USTAT_SLOT_NOT_ENABLED| (host)
		The operation was not performed because a command is issued to
		a device slot that is in the disabled state.

	|USTAT_ENDPOINT_NOT_ENABLED| (host)
		The operation was not performed because a command is issued to
		an endpoint that is in the disabled state.

	|USTAT_BANDWIDTH_OVERRUN| (host)
		This code indicates the TD exceeds the bandwidth allocated to
		the endpoint during an isoch transfer.

	|USTAT_CONTEXT_STATE_ERROR| (host)
		The operation was not performed because a command is issued to
		transition from an illegal context state.

	|USTAT_NO_PING_RESPONSE| (host)
		The operation was not performed because the xHC was unable to
		complete a periodic data transfer associated within the ESIT,
		because it did not receive a PING_RESPONSE in time.

	|USTAT_MISSED_SERVICE| (host)
		This code indicates the xHC was unable to service a isochronous
		endpoint within the interval time.

	|USTAT_ISO_BUFFER_OVERRUN| (host)
		This code indicates an isoch TD on an IN endpoint is less than
		the max ESIT payload in size and the device attempts to send
		more data than it can hold.

	|USTAT_INVALID_STREAM_ID| (host)
		The operation was not performed because a invalid Stream ID
		is received.

	|USTAT_SPLIT_TRANSACTION| (host)
		The operation was not performed because an error is detected
		on a USB2 protocol endpoint for a split transaction.

	|USTAT_NO_STREAM_SUPPORT| (host)
		The operation was not performed because device does not support
		stream.

*/

/* __TMS_TYPE_DEF_ARG(USTAT, UCHAR); */

#define	__TMS_USTAT_BUSY		0	/* in use */
#define	__TMS_USTAT_OK			1	/* success */
#define	__TMS_USTAT_KILL		2	/* cancelled */
#define	__TMS_USTAT_NOTCFG		3	/* not configured */
#define	__TMS_USTAT_IOERR		4	/* read/write error (ISO) */
#define	__TMS_USTAT_REPLACED		5	/* packet superceded */
#define	__TMS_USTAT_STALL		6	/* endpoint is stalled */
#define	__TMS_USTAT_ISOBUF		7	/* malformed ISO buffer */
#define	__TMS_USTAT_LENGTH_OVERRUN	8	/* too much data sent to us! */
#define	__TMS_USTAT_LENGTH_UNDERRUN	9	/* not enough data sent! */
#define	__TMS_USTAT_INVALID_PARAM	10	/* invalid parameter */
#define	__TMS_USTAT_HCD_CRC		11	/* invalid crc */
#define	__TMS_USTAT_HCD_BITSTUFF	12	/* bitstuff err */
#define	__TMS_USTAT_HCD_NO_RESPONSE	13	/* no response */
#define	__TMS_USTAT_HCD_PIDCHECK	14	/* bad PID check code */
#define	__TMS_USTAT_HCD_INVALID_PID	15	/* invalid or unknown PID */
#define	__TMS_USTAT_DMA_OVERRUN		16	/* RX DMA too slow */
#define	__TMS_USTAT_DMA_UNDERRUN	17	/* TX DMA too slow */
#define	__TMS_USTAT_NOHW		18	/* no HC or DC hardware */
#define	__TMS_USTAT_IN_USE		19	/* port has pending URBs */
#define	__TMS_USTAT_NO_MEMORY		20	/* can't malloc */
#define	__TMS_USTAT_NO_BANDWIDTH	21	/* no bus bandwidth */
#define	__TMS_USTAT_NO_BUS_POWER	22	/* not enough bus power */
#define	__TMS_USTAT_INTERNAL_ERROR	23	/* internal error */
#define	__TMS_USTAT_NOT_SUPPORTED	24	/* not supported error */
#define	__TMS_USTAT_BABBLE_DETECTED	25	/* babble detected */
#define	__TMS_USTAT_BAD_START_FRAME	26	/* bad start frame */
#define	__TMS_USTAT_NOT_ACCESSED	27	/* not accessed error */
#define	__TMS_USTAT_MISSED_MICRO_FRAME	28	/* missed micro-frame */
#define	__TMS_USTAT_ERROR_HANDSHAKE	29	/* receive ERR handshake */
#define	__TMS_USTAT_TRANSACTION_ERROR	30	/* transaction error */
#define	__TMS_USTAT_FIFO_ERROR		31	/* FIFO error */
#define	__TMS_USTAT_ISO_NOT_ACCESSED_BY_HW 32	/* HC did not access TD */
#define	__TMS_USTAT_ISO_NOT_ACCESSED_LATE  33	/* did not submit packet on time */
/* xHCI error code */
#define	__TMS_USTAT_TRB_ERROR		34	/* TRB error */
#define	__TMS_USTAT_NO_RESOURCE		35	/* no resource available */
#define	__TMS_USTAT_NO_SLOT		36	/* no slot available */
#define	__TMS_USTAT_INVALID_STREAM_TYPE	37	/* invalid stream type */
#define	__TMS_USTAT_SLOT_NOT_ENABLED	38	/* slot is in disabled state */
#define	__TMS_USTAT_ENDPOINT_NOT_ENABLED 39	/* endpoint is in disabled state */
#define	__TMS_USTAT_BANDWIDTH_OVERRUN	40	/* isoch bandwidth overrun */
#define	__TMS_USTAT_CONTEXT_STATE_ERROR	41	/* illegal context state */
#define	__TMS_USTAT_NO_PING_RESPONSE	42	/* not receive PING_RESPONSE in time */
#define	__TMS_USTAT_MISSED_SERVICE	43	/* missed service for isoch */
#define	__TMS_USTAT_ISO_BUFFER_OVERRUN	44	/* isoch buffer overrun */
#define	__TMS_USTAT_INVALID_STREAM_ID	45	/* invalid stream id */
#define	__TMS_USTAT_SPLIT_TRANSACTION	46	/* split transaction error */
#define	__TMS_USTAT_NO_STREAM_SUPPORT	47	/* device not support stream */

#define	__TMS_USTAT_DEFINITE_LENGTH_OVERRUN	__TMS_USTAT_LENGTH_OVERRUN
#define	__TMS_USTAT_DEFINITE_LENGTH_UNDERRUN	__TMS_USTAT_LENGTH_UNDERRUN
#define	__TMS_USTAT_ERROR_SHORT_TRANSFER	__TMS_USTAT_LENGTH_UNDERRUN


/*
|| Once again, as names are added to the above list, please add them
|| to the following macro.
*/
#define	__TMS_USTAT_NAMES__INIT \
	{ \
	"USTAT_BUSY", \
	"USTAT_OK", \
	"USTAT_KILL", \
	"USTAT_NOTCFG", \
	"USTAT_IOERR", \
	"USTAT_REPLACED", \
	"USTAT_STALL", \
	"USTAT_ISOBUF", \
	"USTAT_LENGTH_OVERRUN", \
	"USTAT_LENGTH_UNDERRUN", \
	"USTAT_INVALID_PARAM", \
	"USTAT_HCD_CRC", \
	"USTAT_HCD_BITSTUFF", \
	"USTAT_HCD_NO_RESPONSE", \
	"USTAT_HCD_PIDCHECK", \
	"USTAT_HCD_INVALID_PID", \
	"USTAT_HCD_DMA_OVERRUN", \
	"USTAT_HCD_DMA_UNDERRUN", \
	"USTAT_NOHW",		\
	"USTAT_IN_USE",	\
	"USTAT_NO_MEMORY",	\
	"USTAT_NO_BANDWIDTH",	\
	"USTAT_NO_BUS_POWER",	\
	"USTAT_INTERNAL_ERROR",	\
	"USTAT_NOT_SUPPORTED",	\
	"USTAT_BABBLE_DETECTED", \
	"USTAT_BAD_START_FRAME", \
	"USTAT_NOT_ACCESSED",	\
	"USTAT_MISSED_MICRO_FRAME", \
	"USTAT_ERROR_HANDSHAKE", \
	"USTAT_TRANSACTION_ERROR", \
	"USTAT_FIFO_ERROR", \
	"USTAT_ISO_NOT_ACCESSED_BY_HW", \
	"USTAT_ISO_NOT_ACCESSED_LATE", \
	"USTAT_TRB_ERROR", \
	"USTAT_NO_RESOURCE", \
	"USTAT_NO_SLOT", \
	"USTAT_INVALID_STREAM_TYPE", \
	"USTAT_SLOT_NOT_ENABLED", \
	"USTAT_ENDPOINT_NOT_ENABLED", \
	"USTAT_BANDWIDTH_OVERRUN", \
	"USTAT_CONTEXT_STATE_ERROR", \
	"USTAT_NO_PING_RESPONSE", \
	"USTAT_MISSED_SERVICE", \
	"USTAT_ISO_BUFFER_OVERRUN", \
	"USTAT_INVALID_STREAM_ID", \
	"USTAT_SPLIT_TRANSACTION", \
	"USTAT_NO_STREAM_SUPPORT" \
	}


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USTAT_BUSY	\
   __TMS_USTAT_BUSY
# define USTAT_OK	\
   __TMS_USTAT_OK
# define USTAT_KILL	\
   __TMS_USTAT_KILL
# define USTAT_NOTCFG	\
   __TMS_USTAT_NOTCFG
# define USTAT_IOERR	\
   __TMS_USTAT_IOERR
# define USTAT_REPLACED	\
   __TMS_USTAT_REPLACED
# define USTAT_STALL	\
   __TMS_USTAT_STALL
# define USTAT_ISOBUF	\
   __TMS_USTAT_ISOBUF
# define USTAT_LENGTH_OVERRUN	\
   __TMS_USTAT_LENGTH_OVERRUN
# define USTAT_LENGTH_UNDERRUN	\
   __TMS_USTAT_LENGTH_UNDERRUN
# define USTAT_INVALID_PARAM	\
   __TMS_USTAT_INVALID_PARAM
# define USTAT_HCD_CRC	\
   __TMS_USTAT_HCD_CRC
# define USTAT_HCD_BITSTUFF	\
   __TMS_USTAT_HCD_BITSTUFF
# define USTAT_HCD_NO_RESPONSE	\
   __TMS_USTAT_HCD_NO_RESPONSE
# define USTAT_HCD_PIDCHECK	\
   __TMS_USTAT_HCD_PIDCHECK
# define USTAT_HCD_INVALID_PID	\
   __TMS_USTAT_HCD_INVALID_PID
# define USTAT_DMA_OVERRUN	\
   __TMS_USTAT_DMA_OVERRUN
# define USTAT_DMA_UNDERRUN	\
   __TMS_USTAT_DMA_UNDERRUN
# define USTAT_NOHW	\
   __TMS_USTAT_NOHW
# define USTAT_IN_USE	\
   __TMS_USTAT_IN_USE
# define USTAT_NO_MEMORY	\
   __TMS_USTAT_NO_MEMORY
# define USTAT_NO_BANDWIDTH	\
   __TMS_USTAT_NO_BANDWIDTH
# define USTAT_NO_BUS_POWER	\
   __TMS_USTAT_NO_BUS_POWER
# define USTAT_INTERNAL_ERROR	\
   __TMS_USTAT_INTERNAL_ERROR
# define USTAT_NOT_SUPPORTED	\
   __TMS_USTAT_NOT_SUPPORTED
# define USTAT_BABBLE_DETECTED	\
   __TMS_USTAT_BABBLE_DETECTED
# define USTAT_BAD_START_FRAME	\
   __TMS_USTAT_BAD_START_FRAME
# define USTAT_NOT_ACCESSED	\
   __TMS_USTAT_NOT_ACCESSED
# define USTAT_MISSED_MICRO_FRAME	\
   __TMS_USTAT_MISSED_MICRO_FRAME
# define USTAT_ERROR_HANDSHAKE	\
   __TMS_USTAT_ERROR_HANDSHAKE
# define USTAT_TRANSACTION_ERROR	\
   __TMS_USTAT_TRANSACTION_ERROR
# define USTAT_FIFO_ERROR	\
   __TMS_USTAT_FIFO_ERROR
# define USTAT_ISO_NOT_ACCESSED_BY_HW	\
   __TMS_USTAT_ISO_NOT_ACCESSED_BY_HW
# define USTAT_ISO_NOT_ACCESSED_LATE	\
   __TMS_USTAT_ISO_NOT_ACCESSED_LATE
# define USTAT_TRB_ERROR	\
   __TMS_USTAT_TRB_ERROR
# define USTAT_NO_RESOURCE	\
   __TMS_USTAT_NO_RESOURCE
# define USTAT_NO_SLOT	\
   __TMS_USTAT_NO_SLOT
# define USTAT_INVALID_STREAM_TYPE	\
   __TMS_USTAT_INVALID_STREAM_TYPE
# define USTAT_SLOT_NOT_ENABLED	\
   __TMS_USTAT_SLOT_NOT_ENABLED
# define USTAT_ENDPOINT_NOT_ENABLED	\
   __TMS_USTAT_ENDPOINT_NOT_ENABLED
# define USTAT_BANDWIDTH_OVERRUN	\
   __TMS_USTAT_BANDWIDTH_OVERRUN
# define USTAT_CONTEXT_STATE_ERROR	\
   __TMS_USTAT_CONTEXT_STATE_ERROR
# define USTAT_NO_PING_RESPONSE	\
   __TMS_USTAT_NO_PING_RESPONSE
# define USTAT_MISSED_SERVICE	\
   __TMS_USTAT_MISSED_SERVICE
# define USTAT_ISO_BUFFER_OVERRUN	\
   __TMS_USTAT_ISO_BUFFER_OVERRUN
# define USTAT_INVALID_STREAM_ID	\
   __TMS_USTAT_INVALID_STREAM_ID
# define USTAT_SPLIT_TRANSACTION	\
   __TMS_USTAT_SPLIT_TRANSACTION
# define USTAT_NO_STREAM_SUPPORT	\
   __TMS_USTAT_NO_STREAM_SUPPORT
# define USTAT_DEFINITE_LENGTH_OVERRUN	\
   __TMS_USTAT_DEFINITE_LENGTH_OVERRUN
# define USTAT_DEFINITE_LENGTH_UNDERRUN	\
   __TMS_USTAT_DEFINITE_LENGTH_UNDERRUN
# define USTAT_ERROR_SHORT_TRANSFER	\
   __TMS_USTAT_ERROR_SHORT_TRANSFER
# define USTAT_NAMES__INIT	\
   __TMS_USTAT_NAMES__INIT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of ustat.h ****/
#endif /* _USTAT_H_ */
