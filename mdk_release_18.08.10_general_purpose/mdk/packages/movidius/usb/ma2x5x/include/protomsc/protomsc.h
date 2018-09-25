/* protomsc.h	Fri Dec 28 2012 12:40:53 chwon */

/*

Module:  protomsc.h

Function:
	Mass storage class protocol API.

Version:
	V3.11b	Fri Dec 28 2012 12:40:53 chwon	Edit level 21

Copyright notice:
	This file copyright (C) 2001-2009, 2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Chris Sawran, MCCI Corporation	July 2001

Revision history:
   1.00a  Fri Jul  6 2001 00:00:01  cds
	Module created.

   1.79a  6/5/2002  tmm
	reduce header file bloat.

   1.79b  Fri Oct 18 2002 12:00:00  maw
   	Major overview during update for IOCTL-interface
   	Corrected signature settings according to spec (maw)

   1.85e  Wed Sep 24 2003 13:00:25  chwon
	Added UPROTO_MSCBOT_DISABLE_INTERFACE_FN.

   1.87a  Thu Nov 27 2003 10:25:00 maw
        Enumeration string changed from <subclass>.storage.fn.mcci.com to
        storage.<subclass>.fn.mcci.com

   1.87a  Fri Mar 05 2004 12:14:15  chwon
	Correct type cloaking.

   1.87b  Wed Mar 24 2004 09:56:27 maw
   	Corrected UPROTO_MSCBOTSUBCLASS_NAME macro after rename
   	of object name macro
   	Merge changes from 1.86.1 into main branch
   	- 1.86.1  Sun Nov 30 2003 00:42:34  tmm : Correct the object name macro.

   1.89b  Fri Jun 25 2004 14:59:39  chwon
	Bug 1263: add UPROTO_MSCBOT_BUS_EVENT_FN

   1.89b  Wed Sep 15 2004 14:45:23  maw
   	Added LUN parameter to UPROTO_MSCBOT_PROCESS_CB_FN
   	Added context to UPROTO_MSCBOT_BUFQE
   	Added nLun to UPROTO_MSCBOT

   1.89c  Mon Oct 18 2004 11:21:43 maw
   	Bug 1343: Added additional states and events, reset-ifc
   	function prototype and element in context

   1.91h  Thu Sep  8 2005 13:03:41  tmm
	Get rid of '//' problems.

   1.97e  Sat Sep 02 2006 10:14:43  chwon
	Add data-in-stuff feature

   1.97f  Wed Sep 13 2006 11:33:49  chwon
	Change MSC use data plane and data stream

   1.97f  Wed Nov 22 2006 13:42:32  chwon
	2132: add umsc_nRxMaxPacketSize

   1.97j  Thu Oct 04 2007 16:43:05  chwon
	3253: remove umsc_IfcEvt and umsc_DevEvt

   1.97k  Thu Mar 13 2008 09:27:43  chwon
	3867: add umsc_nTxMaxPacketMask in the UPROTO_MSCBOT

   1.97k  Mon Dec 08 2008 19:41:27  chwon
	4249: change umsc_nTxMaxPacketMask to umsc_nTxMaxPacketSize

   2.01a  Mon Mar 16 2009 15:26:11  chwon
	2829: move out object name from usbpumpobjectname.h to here.

   2.01a  Wed Apr 08 2009 10:07:22 ctpeng
	7119: Add MscBOT_DataFromHost.	

   3.11b  Fri Dec 28 2012 12:40:53  chwon
	16407: Added umsc_pObjectHeader in the UPROTO_MSCBOT and it will be
	used for printing debug message. Change MscBOT_DumpBuffer() first
	input parameter from UDEVICE * to UPROTO_MSCBOT *.

*/

#ifndef _PROTOMSC_H_		/* prevent multiple includes */
#define _PROTOMSC_H_

#ifndef _USBPUMP_H_
# include "usbpump.h"
#endif

#ifndef _USBMSC10_H_
# include "usbmsc10.h"
#endif

#ifndef _UDATAPLANE_H_
# include "udataplane.h"
#endif

#ifndef _UDATASTREAM_H_
# include "udatastream.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

/* forward declarations */
__TMS_TYPE_DEF_STRUCT(UPROTO_MSCBOT_CBW);
__TMS_TYPE_DEF_STRUCT(UPROTO_MSCBOT_CSW);
__TMS_TYPE_DEF_STRUCT(UPROTO_MSCBOT);
__TMS_TYPE_DEF_STRUCT(UPROTO_MSCBOT_CONFIG);
__TMS_TYPE_DEF_STRUCT(UPROTO_MSCBOT_EVENT_CONTEXT);


/* other constants */

/*
|| UPROTO_MSCBOT_MAX_BURST_SIZE sets the maximum transfer size per UBUFQE.
|| We cannot send more than 64K per transfer because of bufqe index
|| integer lengths, so 32K is a more "compatible" max.  Note... a
|| single xfer can send more than 64K in a singe data phase, just
|| not in a single UBUFQE.
*/
#define __TMS_UPROTO_MSCBOT_MAX_BURST_SIZE	(32*1024)


/****************************************************************************\
|
|	UPROTO_MSCBOT_BUFQE and UPROTO_MSCBOT_BUFQE_WRAPPER
|
\****************************************************************************/
__TMS_TYPE_DEF_STRUCT(UPROTO_MSCBOT_BUFQE);
__TMS_TYPE_DEF_STRUCT(UPROTO_MSCBOT_BUFQE_WRAPPER);

__TMS_FNTYPE_DEF(
UPROTO_MSCBOTSUBCLASS_DONE_FN,
	__TMS_VOID, (
		__TMS_VOID *,
		__TMS_BOOL,
		__TMS_BYTES,
		__TMS_VOID *
	));

/*
||	UPROTO_MSCBOT_BUFQE is used to manage buffer elements with
|	upper protocol layers
*/
struct __TMS_STRUCTNAME(UPROTO_MSCBOT_BUFQE)
	{
	__TMS_PUPROTO_MSCBOTSUBCLASS_DONE_FN	umqe_pDoneFn;	/* completion fn */
	__TMS_VOID *				umqe_pDoneCtx;	/* completion function context */
	};

/*
||	UPROTO_MSCBOT_BUFQE_WRAPPER is used to associate a UPROTO_MSCBOT_BUFQE
||	and a UBUFQE
*/
struct __TMS_STRUCTNAME(UPROTO_MSCBOT_BUFQE_WRAPPER)
	{
	__TMS_UBUFQE			umscqew_Ubufqe;
	__TMS_UPROTO_MSCBOT_BUFQE	umscqew_Mbufqe;
	};

/****************************************************************************\
|
|	UPROTO_MSCBOT_CBW
|
\****************************************************************************/


/*
|| the UPROTO_MSCBOT_CBW structure is the Command Block Wrapper packet
|| encapsulating a mass storage command.
*/
struct __TMS_STRUCTNAME(UPROTO_MSCBOT_CBW)
	{
	__TMS_UINT32    dCBWSignature;		/* must be 0x55 0x53 0x42 0x43 ('USBC') */
	__TMS_UINT32    dCBWTag;		/* paired with csw on return */
	__TMS_UINT32    dCBWDataTransferLength;	/* how much data host intends
						|| to move during data phase
						*/
	__TMS_UINT8     bmCBWFlags;		/* bot flags... */
	__TMS_UINT8     bCBWLUN;		/* for devices that attempt
						|| to support multiple luns
						|| on one interface
						*/
	__TMS_UINT8     bCBWCBLength;		/* length of the command
						|| block. (correct values
						|| are subclass specific
						*/
	__TMS_UINT8     CBWCB[16];		/* the command block being
						|| wrapped
						*/
	};

/* Command Block Wrapper constants */
#define __TMS_UPROTO_MSCBOT_CBW_SZ		(31)
#define __TMS_UPROTO_MSCBOT_CBW_SIGNATURE	(0x43425355)	/* Signature in little-endian format */
#define __TMS_UPROTO_MSCBOT_CBW_FLAGS_DATA_IN	(0x80)


/*
|| The UPROTO_MSCBOT_CSW structure is the Command Status Wrapper packet
|| encapsulating a success/failed/phase error response to the
|| command issued.
*/
struct __TMS_STRUCTNAME(UPROTO_MSCBOT_CSW)
	{
	__TMS_UINT32    dCSWSignature;		/* must be 0x55 0x53 0x42 0x53 ('USBS') */
	__TMS_UINT32    dCSWTag;		/* copy of tag passed in */
	__TMS_UINT32    dCSWDataResidue;	/* how much data was NOT
						|| transferred (transferred
						|| + residue = data transfer
						|| length)
						*/
	__TMS_UINT8     bCSWStatus;		/* success/failed/phase
						|| error.  phase error=
						|| guaranteed reset
						*/
	};

/* Command Status Wrapper constants */
#define __TMS_UPROTO_MSCBOT_CSW_SZ			(13)
#define __TMS_UPROTO_MSCBOT_CSW_SIGNATURE		(0x53425355)	/* Signature in little endian format */
#define __TMS_UPROTO_MSCBOT_CSW_STATUS_SUCCESS		(0)
#define __TMS_UPROTO_MSCBOT_CSW_STATUS_FAILED		(1)
#define __TMS_UPROTO_MSCBOT_CSW_STATUS_PHASE_ERROR	(2)


/****************************************************************************\
|
|	Bulk-Only Transport states
|
\****************************************************************************/

#define __TMS_UPROTO_MSCBOT_STATE_UNCONFIGURED	     0	/* we've not been
							|| enumerated
							*/
#define __TMS_UPROTO_MSCBOT_STATE_CBW		     1	/* we've received a CBW */
#define __TMS_UPROTO_MSCBOT_STATE_DATA_IN	     2	/* we are sending data in
							|| to the host
							*/
#define __TMS_UPROTO_MSCBOT_STATE_DATA_OUT	     3	/* we are receiving data from
							|| the host
							*/
#define __TMS_UPROTO_MSCBOT_STATE_NON_DATA	     4	/* this was a non-data
							|| command, but cbw has
							|| already been issued
							*/
#define __TMS_UPROTO_MSCBOT_STATE_DATA_STALL	     5	/* the data phase was
							|| stalled
							*/
#define __TMS_UPROTO_MSCBOT_STATE_CSW		     6	/* we've tx'd a CSW, waiting
							|| for the host to ACK
							*/
#define __TMS_UPROTO_MSCBOT_STATE_DATA_OUT_DRAIN     7	/* we are draining data from
							|| the host
							*/
#define __TMS_UPROTO_MSCBOT_STATE_RESET_REC_RST	     8	/* error detected
							|| wait for reset-recovery MSC BOT Reset
							*/
#define __TMS_UPROTO_MSCBOT_STATE_RESET_REC_CFES     9	/* error detected
							|| wait for reset-recovery clear feature ep stall
							*/
#define __TMS_UPROTO_MSCBOT_STATE_DATA_IN_STUFF     10	/* we are sending data in
							|| to the host
							*/


/****************************************************************************\
|
|	Events in the Bulk-Only-Transport FSM
|
\****************************************************************************/


/* bulk only events */
#define __TMS_UPROTO_MSCBOT_EVENT_RX_BUFFER	(0)	/* we've received a bulk-
							|| packet from the host
							*/
#define __TMS_UPROTO_MSCBOT_EVENT_TX_BUFFER	(1)	/* the host ACK'd a bulk-
							|| packet we transmitted
							*/
#define __TMS_UPROTO_MSCBOT_EVENT_ENABLE	(2)	/* an ifc set or cfg set has
							|| occured
							*/
#define __TMS_UPROTO_MSCBOT_EVENT_DISABLE	(3)	/* an reset,ifc unset or cfg unset
							|| has occured
							*/
#define __TMS_UPROTO_MSCBOT_EVENT_RESET		(4)	/* a RESET (usb or otherwise)
							||  has occured
							*/
#define __TMS_UPROTO_MSCBOT_EVENT_SUSPEND	(5)	/* USB SUSPEND was detected */
#define __TMS_UPROTO_MSCBOT_EVENT_RESUME	(6)	/* USB RESUME was detected */
#define __TMS_UPROTO_MSCBOT_EVENT_DATA_STALL	(7)	/* a STALL request was
							|| detected
							*/
#define __TMS_UPROTO_MSCBOT_EVENT_CFES_OUT	(8)	/* a clear feature out-endpoint
							|| stall detected
							*/
#define __TMS_UPROTO_MSCBOT_EVENT_CFES_IN	(9)	/* a clear feature in-endpoint
							|| stall detected
							*/
#define __TMS_UPROTO_MSCBOT_EVENT_DATA_COMPLETE	 (10)	/* No more data in this data
							|| xfer phase
							*/

/****************************************************************************\
|
|	Function types for client callbacks.
|
\****************************************************************************/


/* client callbacks */

/* process a subclass-specific CB.  the CB has already been
|| validated, and is meaningful by the time this callback is
|| invoked.
*/
__TMS_FNTYPE_DEF(
	UPROTO_MSCBOT_PROCESS_CB_FN,
	__TMS_VOID, (__TMS_UPROTO_MSCBOT *, __TMS_UINT8, __TMS_UINT8, __TMS_UINT8*)
	);

__TMS_FNTYPE_DEF(
	UPROTO_MSCBOT_ENABLE_INTERFACE_FN,
	__TMS_VOID, (__TMS_UPROTO_MSCBOT *)
	);

__TMS_FNTYPE_DEF(
	UPROTO_MSCBOT_DISABLE_INTERFACE_FN,
	__TMS_VOID, (__TMS_UPROTO_MSCBOT *)
	);

__TMS_FNTYPE_DEF(
	UPROTO_MSCBOT_RESET_INTERFACE_FN,
	__TMS_VOID, (__TMS_UPROTO_MSCBOT *)
	);

__TMS_FNTYPE_DEF(
	UPROTO_MSCBOT_BUS_EVENT_FN,
	__TMS_VOID, (__TMS_UPROTO_MSCBOT *, __TMS_UEVENT, __TMS_VOID *)
	);

#define __TMS_UPROTO_MSCBOT_NUMQE	4

/*
|| the UPROTO_MSCBOT structure is the "self" object for each instance of
|| a mass storage class protocol.
*/
struct __TMS_STRUCTNAME(UPROTO_MSCBOT)
	{
	__TMS_PUPROTO_MSCBOT_PROCESS_CB_FN		umsc_pProcessCB;
	__TMS_PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	umsc_pEnableInterface;
	__TMS_PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	umsc_pDisableInterface;
	__TMS_PUPROTO_MSCBOT_RESET_INTERFACE_FN		umsc_pResetInterface;
	__TMS_PUPROTO_MSCBOT_BUS_EVENT_FN		umsc_pBusEvent;

	/* the UDEVICE to which we are attached */
	__TMS_UDEVICE	*				umsc_pDevice;

	/* ptr to the interface in our set to which we are attaching */
	__TMS_UINTERFACE *				umsc_pIfc;

	/*
	|| Data plane object
	*/
	__TMS_UDATAPLANE				umsc_DataPlane;

	/*
	|| The Bulk IN/OUT datastream
	*/
	__TMS_UDATASTREAM				umsc_InStream;
	__TMS_UDATASTREAM				umsc_OutStream;

	/* ptr to bufqes */
	__TMS_UBUFQE *					umsc_pQeHead;

	/***** TEMP FIX ****/
	__TMS_UPROTO_MSCBOT_BUFQE_WRAPPER		umsc_MscQeWArray[__TMS_UPROTO_MSCBOT_NUMQE];

	/* tracks buffers queued to rx ep */
	__TMS_UINT32					umsc_inRxQueue;

	/* tracks buffers queued to tx ep */
	__TMS_UINT32					umsc_inTxQueue;

	/* are we up and running? */
	__TMS_BOOL					umsc_fUp;

	/* current state of the protocol */
	__TMS_UINT32					umsc_State;

	/* current cbw */
	__TMS_UPROTO_MSCBOT_CBW				umsc_Cbw;

	/* current csw */
	__TMS_UPROTO_MSCBOT_CSW				umsc_Csw;

	/* msc-owned buffer for reading/draining data */
	__TMS_BYTES					umsc_nRxDataSize;
	__TMS_UINT8 *					umsc_RxBuf;

	__TMS_USHORT					umsc_nRxMaxPacketSize;
	__TMS_USHORT					umsc_nTxMaxPacketSize;

	/* msc-owned buffer for txing data */
	__TMS_BYTES					umsc_nTxDataSize;
	__TMS_UINT8 *					umsc_TxBuf;

	/* number of LUN's */
	__TMS_BYTES					umsc_nLun;

	/* subclass context */
	__TMS_VOID *					umsc_pSubClass;

	/* subclass provided object header. Generally subclass object header */
	__TMS_USBPUMP_OBJECT_HEADER *			umsc_pObjectHeader;

	/* how many bytes to stuff */
	__TMS_BOOL					umsc_fEnableDataInStuff;
	__TMS_UINT32					umsc_StuffDataResidue;
	};

/* the unique tag */
#define	__TMS_UPROTO_MSCBOTSUBCLASS_TAG(a, b)	__TMS_UHIL_MEMTAG('M', 's', a, b)

/*
|| A macro to generate a string constant for a subclass.
*/
#define	__TMS_UPROTO_MSCBOTSUBCLASS_NAME(Text)	\
	__TMS_USBPUMP_OBJECT_NAME_FN("storage", Text ".bot.msc" )

/* backward compatibility for PR2829 */
#ifndef	__TMS_USBPUMP_OBJECT_NAME_ENUM_STORAGE
# define __TMS_USBPUMP_OBJECT_NAME_ENUM_STORAGE	\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN("storage")
#endif

/* convert from a derived structure to a generic pointer */
#define __TMS_UPROTO_MSCBOTSUBCLASS_from_Derived(pDerived)	\
	(__TMS_UPROTO_MSCBOTSUBCLASS *)(&(pDerived)->MscSubClassHdr)

/****************************************************************************\
|
|	Function prototypes.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/*
|| Subclass API functions
*/
__TMS_BOOL MscBOT_Attach(
	__TMS_UPROTO_MSCBOT *,
	__TMS_UPROTO_MSCBOT_CONFIG *
	);

__TMS_VOID MscBOT_SendStatus(
	__TMS_UPROTO_MSCBOT *			pMsc,
	__TMS_UINT8				status
	);

__TMS_BOOL MscBOT_BufferToHost(
	__TMS_PUPROTO_MSCBOT			pMsc,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				nBuf,
	__TMS_UPROTO_MSCBOTSUBCLASS_DONE_FN *	pDoneFn,
	__TMS_VOID *				pDoneCtx
	);

__TMS_BOOL MscBOT_HostToBuffer(
	__TMS_PUPROTO_MSCBOT			pMsc,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				nBuf,
	__TMS_UPROTO_MSCBOTSUBCLASS_DONE_FN *	pDoneFn,
	__TMS_VOID *				pDoneCtx
	);

__TMS_BOOL MscBOT_DataToHost(
	__TMS_PUPROTO_MSCBOT			pMsc,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				nBuf,
	__TMS_UPROTO_MSCBOTSUBCLASS_DONE_FN *	pDoneFn,
	__TMS_VOID *				pDoneCtx
	);

__TMS_BOOL MscBOT_DataFromHost(
	__TMS_PUPROTO_MSCBOT			pMsc,
	__TMS_BYTES				nBuf,
	__TMS_UPROTO_MSCBOTSUBCLASS_DONE_FN *	pDoneFn,
	__TMS_VOID *				pDoneCtx
	);

/* debug & utility functions */
__TMS_VOID MscBOT_DumpBuffer(
	__TMS_PUPROTO_MSCBOT			pMsc,
	__TMS_BYTES				nBuf,
	__TMS_UINT8 *				pBuf
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_MSCBOT_MAX_BURST_SIZE	\
   __TMS_UPROTO_MSCBOT_MAX_BURST_SIZE
# define UPROTO_MSCBOT_CBW_SZ	\
   __TMS_UPROTO_MSCBOT_CBW_SZ
# define UPROTO_MSCBOT_CBW_SIGNATURE	\
   __TMS_UPROTO_MSCBOT_CBW_SIGNATURE
# define UPROTO_MSCBOT_CBW_FLAGS_DATA_IN	\
   __TMS_UPROTO_MSCBOT_CBW_FLAGS_DATA_IN
# define UPROTO_MSCBOT_CSW_SZ	\
   __TMS_UPROTO_MSCBOT_CSW_SZ
# define UPROTO_MSCBOT_CSW_SIGNATURE	\
   __TMS_UPROTO_MSCBOT_CSW_SIGNATURE
# define UPROTO_MSCBOT_CSW_STATUS_SUCCESS	\
   __TMS_UPROTO_MSCBOT_CSW_STATUS_SUCCESS
# define UPROTO_MSCBOT_CSW_STATUS_FAILED	\
   __TMS_UPROTO_MSCBOT_CSW_STATUS_FAILED
# define UPROTO_MSCBOT_CSW_STATUS_PHASE_ERROR	\
   __TMS_UPROTO_MSCBOT_CSW_STATUS_PHASE_ERROR
# define UPROTO_MSCBOT_STATE_UNCONFIGURED	\
   __TMS_UPROTO_MSCBOT_STATE_UNCONFIGURED
# define UPROTO_MSCBOT_STATE_CBW	\
   __TMS_UPROTO_MSCBOT_STATE_CBW
# define UPROTO_MSCBOT_STATE_DATA_IN	\
   __TMS_UPROTO_MSCBOT_STATE_DATA_IN
# define UPROTO_MSCBOT_STATE_DATA_OUT	\
   __TMS_UPROTO_MSCBOT_STATE_DATA_OUT
# define UPROTO_MSCBOT_STATE_NON_DATA	\
   __TMS_UPROTO_MSCBOT_STATE_NON_DATA
# define UPROTO_MSCBOT_STATE_DATA_STALL	\
   __TMS_UPROTO_MSCBOT_STATE_DATA_STALL
# define UPROTO_MSCBOT_STATE_CSW	\
   __TMS_UPROTO_MSCBOT_STATE_CSW
# define UPROTO_MSCBOT_STATE_DATA_OUT_DRAIN	\
   __TMS_UPROTO_MSCBOT_STATE_DATA_OUT_DRAIN
# define UPROTO_MSCBOT_STATE_RESET_REC_RST	\
   __TMS_UPROTO_MSCBOT_STATE_RESET_REC_RST
# define UPROTO_MSCBOT_STATE_RESET_REC_CFES	\
   __TMS_UPROTO_MSCBOT_STATE_RESET_REC_CFES
# define UPROTO_MSCBOT_STATE_DATA_IN_STUFF	\
   __TMS_UPROTO_MSCBOT_STATE_DATA_IN_STUFF
# define UPROTO_MSCBOT_EVENT_RX_BUFFER	\
   __TMS_UPROTO_MSCBOT_EVENT_RX_BUFFER
# define UPROTO_MSCBOT_EVENT_TX_BUFFER	\
   __TMS_UPROTO_MSCBOT_EVENT_TX_BUFFER
# define UPROTO_MSCBOT_EVENT_ENABLE	\
   __TMS_UPROTO_MSCBOT_EVENT_ENABLE
# define UPROTO_MSCBOT_EVENT_DISABLE	\
   __TMS_UPROTO_MSCBOT_EVENT_DISABLE
# define UPROTO_MSCBOT_EVENT_RESET	\
   __TMS_UPROTO_MSCBOT_EVENT_RESET
# define UPROTO_MSCBOT_EVENT_SUSPEND	\
   __TMS_UPROTO_MSCBOT_EVENT_SUSPEND
# define UPROTO_MSCBOT_EVENT_RESUME	\
   __TMS_UPROTO_MSCBOT_EVENT_RESUME
# define UPROTO_MSCBOT_EVENT_DATA_STALL	\
   __TMS_UPROTO_MSCBOT_EVENT_DATA_STALL
# define UPROTO_MSCBOT_EVENT_CFES_OUT	\
   __TMS_UPROTO_MSCBOT_EVENT_CFES_OUT
# define UPROTO_MSCBOT_EVENT_CFES_IN	\
   __TMS_UPROTO_MSCBOT_EVENT_CFES_IN
# define UPROTO_MSCBOT_EVENT_DATA_COMPLETE	\
   __TMS_UPROTO_MSCBOT_EVENT_DATA_COMPLETE
# define UPROTO_MSCBOT_NUMQE	\
   __TMS_UPROTO_MSCBOT_NUMQE
# define UPROTO_MSCBOTSUBCLASS_TAG(a, b)	\
   __TMS_UPROTO_MSCBOTSUBCLASS_TAG(a, b)
# define UPROTO_MSCBOTSUBCLASS_NAME(Text)	\
   __TMS_UPROTO_MSCBOTSUBCLASS_NAME(Text)
# define USBPUMP_OBJECT_NAME_ENUM_STORAGE	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_STORAGE
# define UPROTO_MSCBOTSUBCLASS_from_Derived(pDerived)	\
   __TMS_UPROTO_MSCBOTSUBCLASS_from_Derived(pDerived)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of protomsc.h ****/
#endif /* _PROTOMSC_H_ */
