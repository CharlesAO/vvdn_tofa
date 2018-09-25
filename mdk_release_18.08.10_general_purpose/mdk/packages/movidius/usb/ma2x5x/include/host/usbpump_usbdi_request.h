/* usbpump_usbdi_request.h Wed Feb 15 2017 16:31:18 chwon */

/*

Module:  usbpump_usbdi_request.h

Function:
	Home for DataPump USBDI request packet

Version:
	V3.19b	Wed Feb 15 2017 16:31:18 chwon	Edit level 32

Copyright notice:
	This file copyright (C) 2005-2012, 2015, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Wed Jun 15 2005 00:49:16  tmm
	Module created.

   1.91h  2005-08-17  tmm
	Fix problem with MSVC (extra semicolon in
	__TMS_USBPUMP_URB_DEVICE__HDR macro).

   1.91h  Sun Sep  4 2005 03:52:17  tmm
	Make USBPUMP_USBDI_RQ_SUGGEST_CONFIG able to accept a list
	of raw configurations.

   1.91h  Sun Sep  4 2005 22:22:48 tmm
	Re-run uncloak-defs.

   1.97f  Fri Jan 12 2007 13:18:44  chwon
	2208: change USBPUMP_URB_GET_FRAME to match with HCD request

   1.97h  Thu Mar 29 2007 13:47:05  djt
 	Removed A_Code argument from UsbPumpUrb_PrepareHeader() call in
 	UsbPumpUrb_PrepareGetFrame. #2600

   1.97j  Thu Aug 16 2007 17:01:41  djt
	2008:added URB_INTERNAL_FLAG_ABORTING

   1.97j  Thu Nov 08 2007 09:36:34  djt
	3348: Added  bInterfaceClass, bInterfaceSubClass, bInterfaceProtocol 
	and Reserved1 to ALTSET_NODE.

   1.97k  Fri Jan 11 2008 14:53:52  djt
	3049: Added iInterface to ALTSET_NODE.

   1.97k  Mon Feb  4 2008 17:03:03  tmm
	3747:  move USBPUMP_BUFFER_POINTER to i/usbpump_types.h, move
	USBPUMP_USBDI_CFG_NODE, USBPUMP_USBDI_IFC_NODE,
	USBPUMP_USBDI_ALTSET_NODE and USBPUMP_USBDI_PIPE_NODE to 
	host/i/usbpump_usbdi_cfgnodes.h.  With USBPUMP_ISOCH_PACKET_DESCR now
	in i/usbpump_types.h, we no longer need to include
	host/i/usbpump_usbdi_isoch.h.

   1.97k  Fri Jul 18 2008 09:31:56  chwon
	4312: add USBPUMP_BUFFER_HANDLE in the URB

   1.97k  Wed Oct 01 2008 08:43:52  chwon
	3774: Regenerated uncloaked name

   2.01a  Mon Feb 16 2009 10:26:23  chwon
	4312: add USBPUMP_BUFFER_HANDLE in the isoch transfer header

   2.01a  Thu Mar 05 2009 09:33:38  djt
	7416: Added USBPUMP_URB_RQ_BULKINT_STREAM_IN and 
	USBPUMP_URB_RQ_BULKINT_STREAM_OUT

   2.01a  Wed Oct 28 2009 16:00:25  chwon
	9174: Add UrbBulkIntInOut and UrbBulkIntStreamInOut in the USBPUMP_URB
	7416: Fixed typo in PrepareBulkIntStreamIn / Out macro

   2.01a  Wed Nov 04 2009 13:54:00  chwon
	9246: add USBPUMP_USBDI_TIMEOUT and USBPUMP_USBDI_HCD_INDEX type
	definition with compile time option.  Use this types instead of UINT16.
	9247: add USBPUMP_URB_RESET_PIPE structure and add reset pipe control
	flags (USBPUMP_URB_RESET_PIPE_FLAG_xxx).

   3.01a  Sat Jan 23 2010 15:34:45  tmm
	9792:  add USBPUMP_URB_INTERNAL_FLAG_FRAME_UPDATED, so we know for
	sure when we have set IsochStartFrame on completion.

   3.01c  Thu Apr 22 2010 09:33:38  chwon
	10076: add USBPUMP_USBDI_PORT_STATUS_xxx code definition.

   3.01c  Tue Aug 10 2010 18:47:23  chwon
	10950: add USBPUMP_URB_DEFINE_CONFIG_FLAG_DYNAMIC flag for define
	config USB request. Add UsbPumpUrb_PrepareDefineConfig_V2() macro.

   3.01c  Mon Aug 30 2010 14:50:30  lin
	10987: added USBPUMP_URB_INTERNAL_FLAG_LPM_INCREMENTED for
	URB queued when auto LPM is running.

   3.01d  Sat Nov 06 2010 07:09:48  tmm
	11815: need to initialize nActual for URBs that have nActual,
	before they can be completed.  Add USBPUMP_URB_RQ_HAS_nACTUAL()
	so that abstract code can figure out whether the request has
	such a field.

   3.01d  Fri Feb 18 2011 13:23:20  chwon
	12592: add USBPUMP_URB_INTERNAL_FLAG_CLEAN_UP_CONFIG internal flag.

   3.01d  Mon Mar 14 2011 11:10:23  chwon
	12967: add USBPUMP_URB_INTERNAL_FLAG_LPM_QUEUED for URB queued in LPM
	pending queue.

   3.01f  Thu May 12 2011 12:53:43  chwon
	13379: add USBPUMP_URB_ABORT_PIPE structure and add abort pipe control
	flags (USBPUMP_URB_ABORT_PIPE_FLAG_WAIT_COMPLETION).

   3.01f  Wed Aug 03 2011 11:13:09  chwon
	13853: add USBPUMP_URB_RQ_NON_CONTROL_TRANSFER() macro definition.

   3.11b  Fri Oct 12 2012 10:30:21  chwon
	16028: add USBPUMP_URB_TRANSFER_FLAG_FILTER_EXTRA_DESC transfer flag.

   3.15b  Mon Mar 16 2015 13:45:39  chwon
	18673: Add USBPUMP_URB_SUGGEST_CONFIG_RX_CONFIG_BUNDLE_FN in the
	USBPUMP_URB_SUGGEST_CONFIG.

   3.19b  Wed Feb 15 2017 16:31:18  chwon
	20449: Define USBPUMP_USBDI_TIMEOUT and USBPUMP_USBDI_HCD_INDEX
	using USBPUMP_TIMER_TIMEOUT.

*/

#ifndef _USBPUMP_USBDI_REQUEST_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_REQUEST_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

#ifndef _USETUP_H_
# include "usetup.h"
#endif

#ifndef _USBPUMP_USBDI_CFGNODES_H_
# include "usbpump_usbdi_cfgnodes.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

/****************************************************************************\
|
|	Forward definitions
|
\****************************************************************************/

__TMS_TYPE_DEF_ARG(USBPUMP_URB_LENGTH, UINT16);
__TMS_TYPE_DEF_ARG(USBPUMP_URB_CODE, UINT8);

/* __TMS_TYPE_DEF_UNION(USBPUMP_URB); */

/** __TMS_FNTYPE_DEF(
* USBPUMP_URB_DONE_FN,
* 	__TMS_VOID,
* 		(
* 		__TMS_USBPUMP_USBDI_PORT *,
* 		__TMS_USBPUMP_URB *,
* 		__TMS_VOID *,
* 		__TMS_ARG_USTAT
* 		));
*/

__TMS_FNTYPE_DEF(
USBPUMP_URB_CANCEL_FN,
	__TMS_USTAT,
		(
		__TMS_USBPUMP_URB *,
		__TMS_VOID * /*pCancelInfo*/
		));

__TMS_TYPE_DEF_ARG(USBPUMP_USBDI_TIMEOUT, USBPUMP_TIMER_TIMEOUT);
__TMS_TYPE_DEF_ARG(USBPUMP_USBDI_HCD_INDEX, USBPUMP_TIMER_TIMEOUT);


/****************************************************************************\
|
|	The URB Request codes
|
\****************************************************************************/

#define	__TMS_USBPUMP_URB_RQ_GET_DEVICE_INFO		0
#define	__TMS_USBPUMP_URB_RQ_CONTROL_IN			1
#define	__TMS_USBPUMP_URB_RQ_CONTROL_OUT		2
#define	__TMS_USBPUMP_URB_RQ_BULKINT_IN			3
#define	__TMS_USBPUMP_URB_RQ_BULKINT_OUT		4
#define	__TMS_USBPUMP_URB_RQ_ISOCH_IN			5
#define	__TMS_USBPUMP_URB_RQ_ISOCH_OUT			6
#define	__TMS_USBPUMP_URB_RQ_DEFINE_CONFIG		7
#define	__TMS_USBPUMP_URB_RQ_ABORT_PIPE			8
#define	__TMS_USBPUMP_URB_RQ_RESET_PIPE			9
#define	__TMS_USBPUMP_URB_RQ_REENUMERATE_PORT		10
#define	__TMS_USBPUMP_URB_RQ_RESET_PORT			11
#define	__TMS_USBPUMP_URB_RQ_GET_PORT_STATUS		12
#define	__TMS_USBPUMP_URB_RQ_GET_FRAME			13
#define	__TMS_USBPUMP_URB_RQ_SUGGEST_CONFIG		14
#define	__TMS_USBPUMP_URB_RQ_BULKINT_STREAM_IN		15
#define	__TMS_USBPUMP_URB_RQ_BULKINT_STREAM_OUT		16

#define	__TMS_USBPUMP_URB_RQ__MAX			17

#define	__TMS_USBPUMP_URB_RQ_NAME__INIT				\
	"USBPUMP_URB_RQ_GET_DEVICE_INFO",			\
	"USBPUMP_URB_RQ_CONTROL_IN",				\
	"USBPUMP_URB_RQ_CONTROL_OUT",				\
	"USBPUMP_URB_RQ_BULKINT_IN",				\
	"USBPUMP_URB_RQ_BULKINT_OUT",				\
	"USBPUMP_URB_RQ_ISOCH_IN",				\
	"USBPUMP_URB_RQ_ISOCH_OUT",				\
	"USBPUMP_URB_RQ_DEFINE_CONFIG",				\
	"USBPUMP_URB_RQ_ABORT_PIPE",				\
	"USBPUMP_URB_RQ_RESET_PIPE",				\
	"USBPUMP_URB_RQ_REENUMERATE_PORT",			\
	"USBPUMP_URB_RQ_RESET_PORT",				\
	"USBPUMP_URB_RQ_GET_PORT_STATUS",			\
	"USBPUMP_URB_RQ_GET_FRAME",				\
	"USBPUMP_URB_RQ_SUGGEST_CONFIG",			\
	"USBPUMP_URB_RQ_BULKINT_STREAM_IN",			\
	"USBPUMP_URB_RQ_BULKINT_STREAM_OUT"

#define	__TMS_USBPUMP_URB_RQ_SIZE__INIT				\
	/* USBPUMP_URB_RQ_GET_DEVICE_INFO */	sizeof(__TMS_USBPUMP_URB_GET_DEVICE_INFO),	\
	/* USBPUMP_URB_RQ_CONTROL_IN */		sizeof(__TMS_USBPUMP_URB_CONTROL_IN),		\
	/* USBPUMP_URB_RQ_CONTROL_OUT */	sizeof(__TMS_USBPUMP_URB_CONTROL_OUT),		\
	/* USBPUMP_URB_RQ_BULKINT_IN */		sizeof(__TMS_USBPUMP_URB_BULKINT_IN),		\
	/* USBPUMP_URB_RQ_BULKINT_OUT */	sizeof(__TMS_USBPUMP_URB_BULKINT_OUT),		\
	/* USBPUMP_URB_RQ_ISOCH_IN */		sizeof(__TMS_USBPUMP_URB_ISOCH_IN),		\
	/* USBPUMP_URB_RQ_ISOCH_OUT */		sizeof(__TMS_USBPUMP_URB_ISOCH_OUT),		\
	/* USBPUMP_URB_RQ_DEFINE_CONFIG */	sizeof(__TMS_USBPUMP_URB_DEFINE_CONFIG),	\
	/* USBPUMP_URB_RQ_ABORT_PIPE */		sizeof(__TMS_USBPUMP_URB_PIPE_CONTROL),		\
	/* USBPUMP_URB_RQ_RESET_PIPE */		sizeof(__TMS_USBPUMP_URB_PIPE_CONTROL),		\
	/* USBPUMP_URB_RQ_REENUMERATE_PORT */	sizeof(__TMS_USBPUMP_URB_DEVICE_CONTROL),	\
	/* USBPUMP_URB_RQ_RESET_PORT */		sizeof(__TMS_USBPUMP_URB_DEVICE_CONTROL),	\
	/* USBPUMP_URB_RQ_GET_PORT_STATUS */	sizeof(__TMS_USBPUMP_URB_GET_PORT_STATUS),	\
	/* USBPUMP_URB_RQ_GET_FRAME */		sizeof(__TMS_USBPUMP_URB_GET_FRAME),		\
	/* USBPUMP_URB_RQ_SUGGEST_CONFIG */	sizeof(__TMS_USBPUMP_URB_SUGGEST_CONFIG),	\
	/* USBPUMP_URB_RQ_BULKINT_STREAM_IN */	sizeof(__TMS_USBPUMP_URB_BULKINT_STREAM_IN),	\
	/* USBPUMP_URB_RQ_BULKINT_STREAM_OUT */	sizeof(__TMS_USBPUMP_URB_BULKINT_STREAM_OUT)

#define	__TMS_USBPUMP_URB_RQ_HAS_hPIPE_MASK			\
	(							\
	(0u << __TMS_USBPUMP_URB_RQ_GET_DEVICE_INFO) |		\
	(1u << __TMS_USBPUMP_URB_RQ_CONTROL_IN) |		\
	(1u << __TMS_USBPUMP_URB_RQ_CONTROL_OUT) |		\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_IN) |		\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_OUT) |		\
	(1u << __TMS_USBPUMP_URB_RQ_ISOCH_IN) |			\
	(1u << __TMS_USBPUMP_URB_RQ_ISOCH_OUT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_DEFINE_CONFIG) |		\
	(1u << __TMS_USBPUMP_URB_RQ_ABORT_PIPE) |		\
	(1u << __TMS_USBPUMP_URB_RQ_RESET_PIPE) |		\
	(0u << __TMS_USBPUMP_URB_RQ_REENUMERATE_PORT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_RESET_PORT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_GET_PORT_STATUS) |		\
	(0u << __TMS_USBPUMP_URB_RQ_GET_FRAME) |		\
	(0u << __TMS_USBPUMP_URB_RQ_SUGGEST_CONFIG) |		\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_IN) |	\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_OUT) 	\
	)

#define	__TMS_USBPUMP_URB_RQ_HAS_hPIPE(request)			\
	(((1u << (request)) & __TMS_USBPUMP_URB_RQ_HAS_hPIPE_MASK) != 0)

#define	__TMS_USBPUMP_URB_RQ_NULL_hPIPE_OK_MASK			\
	(							\
	(0u << __TMS_USBPUMP_URB_RQ_GET_DEVICE_INFO) |		\
	(1u << __TMS_USBPUMP_URB_RQ_CONTROL_IN) |		\
	(1u << __TMS_USBPUMP_URB_RQ_CONTROL_OUT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_BULKINT_IN) |		\
	(0u << __TMS_USBPUMP_URB_RQ_BULKINT_OUT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_ISOCH_IN) |			\
	(0u << __TMS_USBPUMP_URB_RQ_ISOCH_OUT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_DEFINE_CONFIG) |		\
	(1u << __TMS_USBPUMP_URB_RQ_ABORT_PIPE) |		\
	(1u << __TMS_USBPUMP_URB_RQ_RESET_PIPE) |		\
	(0u << __TMS_USBPUMP_URB_RQ_REENUMERATE_PORT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_RESET_PORT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_GET_PORT_STATUS) |		\
	(0u << __TMS_USBPUMP_URB_RQ_GET_FRAME) |		\
	(0u << __TMS_USBPUMP_URB_RQ_SUGGEST_CONFIG) |		\
	(0u << __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_IN) |	\
	(0u << __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_OUT) 	\
	)

#define	__TMS_USBPUMP_URB_RQ_NULL_hPIPE_OK(request)		\
	(((1u << (request)) & __TMS_USBPUMP_URB_RQ_NULL_hPIPE_OK_MASK) != 0)

#define	__TMS_USBPUMP_URB_RQ_HAS_nACTUAL_MASK			\
	(							\
	(0u << __TMS_USBPUMP_URB_RQ_GET_DEVICE_INFO) |		\
	(1u << __TMS_USBPUMP_URB_RQ_CONTROL_IN) |		\
	(1u << __TMS_USBPUMP_URB_RQ_CONTROL_OUT) |		\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_IN) |		\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_OUT) |		\
	(1u << __TMS_USBPUMP_URB_RQ_ISOCH_IN) |			\
	(1u << __TMS_USBPUMP_URB_RQ_ISOCH_OUT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_DEFINE_CONFIG) |		\
	(0u << __TMS_USBPUMP_URB_RQ_ABORT_PIPE) |		\
	(0u << __TMS_USBPUMP_URB_RQ_RESET_PIPE) |		\
	(0u << __TMS_USBPUMP_URB_RQ_REENUMERATE_PORT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_RESET_PORT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_GET_PORT_STATUS) |		\
	(0u << __TMS_USBPUMP_URB_RQ_GET_FRAME) |		\
	(0u << __TMS_USBPUMP_URB_RQ_SUGGEST_CONFIG) |		\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_IN) |	\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_OUT) 	\
	)

#define	__TMS_USBPUMP_URB_RQ_HAS_nACTUAL(request)		\
	(((1u << (request)) & __TMS_USBPUMP_URB_RQ_HAS_nACTUAL_MASK) != 0)

#define	__TMS_USBPUMP_URB_RQ_NON_CONTROL_TRANSFER_MASK		\
	(							\
	(0u << __TMS_USBPUMP_URB_RQ_GET_DEVICE_INFO) |		\
	(0u << __TMS_USBPUMP_URB_RQ_CONTROL_IN) |		\
	(0u << __TMS_USBPUMP_URB_RQ_CONTROL_OUT) |		\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_IN) |		\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_OUT) |		\
	(1u << __TMS_USBPUMP_URB_RQ_ISOCH_IN) |			\
	(1u << __TMS_USBPUMP_URB_RQ_ISOCH_OUT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_DEFINE_CONFIG) |		\
	(0u << __TMS_USBPUMP_URB_RQ_ABORT_PIPE) |		\
	(0u << __TMS_USBPUMP_URB_RQ_RESET_PIPE) |		\
	(0u << __TMS_USBPUMP_URB_RQ_REENUMERATE_PORT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_RESET_PORT) |		\
	(0u << __TMS_USBPUMP_URB_RQ_GET_PORT_STATUS) |		\
	(0u << __TMS_USBPUMP_URB_RQ_GET_FRAME) |		\
	(0u << __TMS_USBPUMP_URB_RQ_SUGGEST_CONFIG) |		\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_IN) |	\
	(1u << __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_OUT) 	\
	)

#define	__TMS_USBPUMP_URB_RQ_NON_CONTROL_TRANSFER(request)	\
	(((1u << (request)) & __TMS_USBPUMP_URB_RQ_NON_CONTROL_TRANSFER_MASK) != 0)

/****************************************************************************\
|
|	Transfer control flags
|
\****************************************************************************/

#define	__TMS_USBPUMP_URB_TRANSFER_FLAG_ASAP		(1u << 0)
#define	__TMS_USBPUMP_URB_TRANSFER_FLAG_SHORT_OK	(1u << 1)
#define	__TMS_USBPUMP_URB_TRANSFER_FLAG_LINKED		(1u << 2)
#define	__TMS_USBPUMP_URB_TRANSFER_FLAG_POST_BREAK	(1u << 3)
#define	__TMS_USBPUMP_URB_TRANSFER_FLAG_FILTER_EXTRA_DESC (1u << 4)

/****************************************************************************\
|
|	Abort pipe control flags
|
\****************************************************************************/

#define	__TMS_USBPUMP_URB_ABORT_PIPE_FLAG_WAIT_COMPLETION	(1u << 0)

/****************************************************************************\
|
|	Reset pipe control flags
|
\****************************************************************************/

#define	__TMS_USBPUMP_URB_RESET_PIPE_FLAG_DO_CLEAR_FEATURE	(1u << 0)
#define	__TMS_USBPUMP_URB_RESET_PIPE_FLAG_CLEAR_HOST_TOGGLE	(1u << 1)

/****************************************************************************\
|
|	Define config control flags
|
\****************************************************************************/

#define	__TMS_USBPUMP_URB_DEFINE_CONFIG_FLAG_DYNAMIC	(1u << 0)

/****************************************************************************\
|
|	Internal URB management flags
|
\****************************************************************************/

/*
|| Within USBDI, we monitor this flag to see whether an URB has been
|| cancelled, and we avoid trying to do anything more.
*/
#define	__TMS_USBPUMP_URB_INTERNAL_FLAG_CANCELLED	(1u << 0)

/*
|| The passthru flag is used to send an URB through the default pipe FSM
|| without examining it.
*/
#define	__TMS_USBPUMP_URB_INTERNAL_FLAG_PASSTHRU	(1u << 1)

/*
|| The USBPUMP_URB_INTERNAL_FLAG_ABORTING flag is set during port resets.
*/
#define	__TMS_USBPUMP_URB_INTERNAL_FLAG_ABORTING	(1u << 2)

/*
|| For isoch URBs, we set this flag once we've captured the frame number
|| from below.
*/
#define	__TMS_USBPUMP_URB_INTERNAL_FLAG_FRAME_UPDATED	(1u << 3)

/*
|| For URBs that prevent L1 entry, we set this flag once we've received
|| this type of URB
*/
#define __TMS_USBPUMP_URB_INTERNAL_FLAG_LPM_INCREMENTED	(1u << 4)

/*
|| For control URBs, we set this flag when clean up configuration.
*/
#define __TMS_USBPUMP_URB_INTERNAL_FLAG_CLEAN_UP_CONFIG	(1u << 5)

/*
|| For URBs that's queued due to LPM state, we set this flag
*/
#define __TMS_USBPUMP_URB_INTERNAL_FLAG_LPM_QUEUED	(1u << 6)

/****************************************************************************\
|
|	The structure at the beginning of any URB.  This is very
|	similar to an HCD request.  But it's not identical, and we assume
|	that clients will learn the USBD request size via the IOCTL, or that
|	they'll provide USBDI with a means to allocate this.
|
\****************************************************************************/

/*
|| XXX	instead of using variable length objects in the URB, we should
||	allow for one, two, or three buffer, and encode the number of
||	buffers (the meta format) into the URB request (as with TrueTask
||	I/O layers).  This will allow sane marshalling using the MCCI
||	messaging API (without knowing the layout of the URB).  Just put
||	up to four PVOIDs, and a byte of none/r/w/rw bit pairs?
*/

#define	__TMS_USBPUMP_URB__HDR					\
	__TMS_USBPUMP_URB_LENGTH	Length;			\
	__TMS_USBPUMP_URB_CODE		Request;		\
	__TMS_USTAT			Status;			\
	__TMS_UINT8			Refcount;		\
	__TMS_UINT8			Reserved;		\
	__TMS_UINT8			InternalFlags;		\
	__TMS_UINT8			InternalStatus;		\
	__TMS_USBPUMP_URB		*pNext;			\
	__TMS_USBPUMP_URB		*pLast;			\
	__TMS_USBPUMP_URB_DONE_FN	*pDoneFn;		\
	__TMS_VOID			*pDoneInfo;		\
	__TMS_USBPUMP_URB_CANCEL_FN	*pCancelFn;		\
	__TMS_VOID			*pCancelInfo;		\
	__TMS_USBPUMP_USBDI_PORT	*pUsbdPort;		\
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;		\
	__TMS_USBPUMP_USBDI_HCD_INDEX	HcdRequestIndex;	\
	__TMS_USBPUMP_USBDI_TIMEOUT	HcdTimeout

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_HDR);
struct __TMS_STRUCTNAME(USBPUMP_URB_HDR)
	{
	__TMS_USBPUMP_URB__HDR;
	};

/**** for get device info, etc ****/
#define	__TMS_USBPUMP_URB_DEVICE__HDR				\
	/* same as... */					\
	__TMS_USBPUMP_URB__HDR					\
	/* plus... */						\
	/* __TMS_USBPUMP_USBDI_DEVICE_HANDLE	hDevice */

/**** for reenumerate port, reset port ****/
__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_DEVICE_CONTROL);
struct __TMS_STRUCTNAME(USBPUMP_URB_DEVICE_CONTROL)
	{
	/* same as... */
	__TMS_USBPUMP_URB_DEVICE__HDR;
	};

/**** for get device info ****/
__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_GET_DEVICE_INFO);
struct __TMS_STRUCTNAME(USBPUMP_URB_GET_DEVICE_INFO)
	{
	/* same as... */
	__TMS_USBPUMP_URB_DEVICE__HDR;
	/* plus... */
	/* output */
	__TMS_USBPUMP_USBDI_PIPE_HANDLE		hDefaultPipe;
	__TMS_USBPUMP_DEVICE_SPEED		DeviceSpeed;
	__TMS_UINT32				DeviceBandwidth;
	__TMS_UINT32				BusBandwidth;
	__TMS_USBPUMP_USBDI_DEVICE_HANDLE	hHub;
	__TMS_UINT32				iPort;
	__TMS_UINT32				RequestLatency;
	};

/**** for get frame ****/
__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_GET_FRAME);
struct __TMS_STRUCTNAME(USBPUMP_URB_GET_FRAME)
	{
	/* same as... */
	__TMS_USBPUMP_URB_DEVICE__HDR;
	/* plus... */
	/* output */
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

/**** for abort pipe, reset pipe ****/
#define __TMS_USBPUMP_URB_PIPE_CONTROL__HDR			\
	/* ...same as ... */					\
	__TMS_USBPUMP_URB__HDR;					\
	/* ...plus...*/						\
	__TMS_USBPUMP_USBDI_PIPE_HANDLE	hPipe

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_PIPE_CONTROL);
struct __TMS_STRUCTNAME(USBPUMP_URB_PIPE_CONTROL)
	{
	__TMS_USBPUMP_URB_PIPE_CONTROL__HDR;
	};

/**** for all pipe-based transfer ****/
#define __TMS_USBPUMP_URB_PIPE_TRANSFER__HDR(BufferType)	\
	/* same as... */					\
	__TMS_USBPUMP_URB_PIPE_CONTROL__HDR;			\
	/* plus.. */						\
	BufferType		pBuffer;			\
	__TMS_USBPUMP_BUFFER_HANDLE hBuffer;	/* 4312 */	\
	__TMS_BYTES		nBuffer;			\
	__TMS_BYTES		nActual;			\
	__TMS_ARG_UINT32	TransferFlags

__TMS_TYPE_DEF_STRUCT	(USBPUMP_URB_PIPE_INOUT);
struct __TMS_STRUCTNAME	(USBPUMP_URB_PIPE_INOUT)
	{
	/* this is simply the PIPE_TRANSFER header */
	__TMS_USBPUMP_URB_PIPE_TRANSFER__HDR(__TMS_USBPUMP_BUFFER_POINTER);
	};

/*
|| At USBDI level, we allow bulk or interrupt pipes to be substitued
|| for each other without changing code, since the semantics are
|| basically the same.
*/
__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_BULKINT_IN);
struct __TMS_STRUCTNAME(USBPUMP_URB_BULKINT_IN)
	{
	__TMS_USBPUMP_URB_PIPE_TRANSFER__HDR(__TMS_VOID *);
	};

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_BULKINT_OUT);
struct __TMS_STRUCTNAME(USBPUMP_URB_BULKINT_OUT)
	{
	__TMS_USBPUMP_URB_PIPE_TRANSFER__HDR(__TMS_CONST __TMS_VOID *);
	};

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_BULKINT_INOUT);
struct __TMS_STRUCTNAME(USBPUMP_URB_BULKINT_INOUT)
	{
	__TMS_USBPUMP_URB_PIPE_TRANSFER__HDR(__TMS_USBPUMP_BUFFER_POINTER);
	};

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_BULKINT_STREAM_IN);
struct __TMS_STRUCTNAME(USBPUMP_URB_BULKINT_STREAM_IN)
	{
	__TMS_USBPUMP_URB_PIPE_TRANSFER__HDR(__TMS_VOID *);
	/* plus... */
	__TMS_UINT16	StreamID;
	};

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_BULKINT_STREAM_OUT);
struct __TMS_STRUCTNAME(USBPUMP_URB_BULKINT_STREAM_OUT)
	{
	__TMS_USBPUMP_URB_PIPE_TRANSFER__HDR(__TMS_CONST __TMS_VOID *);
	/* plus... */
	__TMS_UINT16	StreamID;
	};

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_BULKINT_STREAM_INOUT);
struct __TMS_STRUCTNAME(USBPUMP_URB_BULKINT_STREAM_INOUT)
	{
	__TMS_USBPUMP_URB_PIPE_TRANSFER__HDR(__TMS_USBPUMP_BUFFER_POINTER);
	/* plus... */
	__TMS_UINT16	StreamID;
	};

/**** for a control-pipe transfer ****/
#define __TMS_USBPUMP_URB_CONTROL_TRANSFER__HDR(BufferType)	\
	/* same as... */					\
	__TMS_USBPUMP_URB_PIPE_TRANSFER__HDR(BufferType);	\
	/* plus... */						\
	__TMS_USETUP_WIRE			Setup

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_CONTROL_IN);
struct __TMS_STRUCTNAME(USBPUMP_URB_CONTROL_IN)
	{
	__TMS_USBPUMP_URB_CONTROL_TRANSFER__HDR(__TMS_VOID *);
	};

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_CONTROL_OUT);
struct __TMS_STRUCTNAME(USBPUMP_URB_CONTROL_OUT)
	{
	__TMS_USBPUMP_URB_CONTROL_TRANSFER__HDR(__TMS_CONST __TMS_VOID *);
	};

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_CONTROL_INOUT);
struct __TMS_STRUCTNAME(USBPUMP_URB_CONTROL_INOUT)
	{
	__TMS_USBPUMP_URB_CONTROL_TRANSFER__HDR(__TMS_USBPUMP_BUFFER_POINTER);
	};

/**** for an isoch transfer ****/
#define __TMS_USBPUMP_URB_ISOCH_TRANSFER__HDR(BufferType)	\
	/* ...same as... */					\
	__TMS_USBPUMP_URB_PIPE_TRANSFER__HDR(BufferType);	\
	/* plus... */						\
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *pIsochDescr;		\
	__TMS_USBPUMP_BUFFER_HANDLE	hIsochDescr;		\
	__TMS_BYTES	IsochDescrSize;				\
	__TMS_UINT32	IsochStartFrame;			\
	__TMS_BYTES	nIsochErrs

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_ISOCH_IN);
struct __TMS_STRUCTNAME(USBPUMP_URB_ISOCH_IN)
	{
	__TMS_USBPUMP_URB_ISOCH_TRANSFER__HDR(__TMS_VOID *);
	};

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_ISOCH_OUT);
struct __TMS_STRUCTNAME(USBPUMP_URB_ISOCH_OUT)
	{
	__TMS_USBPUMP_URB_ISOCH_TRANSFER__HDR(__TMS_CONST __TMS_VOID *);
	};

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_ISOCH_INOUT);
struct __TMS_STRUCTNAME(USBPUMP_URB_ISOCH_INOUT)
	{
	__TMS_USBPUMP_URB_ISOCH_TRANSFER__HDR(__TMS_USBPUMP_BUFFER_POINTER);
	};

/*
|| for an embedded system it makes more sense to establish a devtree
|| than it does to "set config" and "set interface" -- the driver
|| first calls a library routine to change a config descriptor into
|| a "configuration tree", which includes branches for different pipes
|| based on alternate settings -- basically this looks alot like the
|| traditional DataPump UCONFIG/UINTERFACESET/UINTERFACE/UPIPE
|| hierarchy.  Then it pushes that into USBDI, and then does the setconfig
|| that selects it.  USBDI's representation is very much like the
|| device tree structure.  Use driver-local heap to do the work.
*/

/**** for define_config config ****/
__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_DEFINE_CONFIG);
struct __TMS_STRUCTNAME(USBPUMP_URB_DEFINE_CONFIG)
	{
	/* same as... */
	__TMS_USBPUMP_URB_DEVICE__HDR;
	/* plus... */

	__TMS_UINT32			ConfigFlags;
	__TMS_USBPUMP_USBDI_CFG_NODE	*pRootConfigNode;
	__TMS_BYTES			sizeConfigBuffer;
	};

__TMS_FNTYPE_DEF(
USBPUMP_URB_SUGGEST_CONFIG_RX_DEVICE_DESC_FN,
	__TMS_USTAT,
		(
		__TMS_USBPUMP_URB *	/* pUrb */,
		__TMS_VOID *		/* pDeviceDescBuffer */
		));

__TMS_FNTYPE_DEF(
USBPUMP_URB_SUGGEST_CONFIG_RX_CONFIG_BUNDLE_FN,
	__TMS_USTAT,
		(
		__TMS_USBPUMP_URB *	/* pUrb */,
		__TMS_VOID *		/* pConfigBundlesBuffer */,
		__TMS_BYTES		/* sizeConfigBundlesBuffer */,
		__TMS_UINT		/* numConfigBundles */
		));

__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_SUGGEST_CONFIG);
struct __TMS_STRUCTNAME(USBPUMP_URB_SUGGEST_CONFIG)
	{
	/* same as... */
	__TMS_USBPUMP_URB_DEVICE__HDR;
	/* plus... */

	/*
	|| the output from this is a configuration tree, built in the
	|| specified buffer, with the root at the front.
	*/
	__TMS_VOID			*pConfigBuffer;
	__TMS_BYTES			sizeConfigBuffer;
	__TMS_BYTES			nActualConfigBuffer;

	/*
	|| The optional input to this is one or more config bundles.  Note
	|| that this is optional, and USBDI's idea of the config
	|| descriptor for this function will be used if the pointer
	|| is NULL or length is zero.  If multiple configuration bundles
	|| are to be suggested, they should be contatenated in the buffer;
	|| the first byte of the second configuration bundle follows the
	|| last byte of the first configuration bundle.  In the common case
	|| of a single configuruation bundle, sizeConfigBundles must be
	|| exactly equal to wTotalLength in the initial config descriptor.
	*/
	__TMS_CONST __TMS_VOID		*pOptionalConfigBundles;
	__TMS_BYTES			sizeConfigBundles;

	/*
	|| The optional function pointers to allow the caller copy received
	|| device descriptor or configuration bundles into local buffer.
	*/
	__TMS_USBPUMP_URB_SUGGEST_CONFIG_RX_DEVICE_DESC_FN *
					pRxDeviceDescFn;
	__TMS_USBPUMP_URB_SUGGEST_CONFIG_RX_CONFIG_BUNDLE_FN *
					pRxConfigBundleFn;
	};

/*
|| After you send down define-config, USBD will accept set-interface
|| and set-config operations, and will allocate bandwidth.
||
|| The following travel in the configuration
||	USBPUMP_USBDI_CFG_NODE
||	USBPUMP_USBDI_IFC_NODE
||	USBPUMP_USBDI_ALTSET_NODE
||	USBPUMP_USBDI_PIPE_NODE
||
|| Once this has been set, any of the proffered configurations/settings
|| may be selected via a SETUP sent to the default pipe.
*/

/**** for the port status ****/
__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_GET_PORT_STATUS);
struct __TMS_STRUCTNAME(USBPUMP_URB_GET_PORT_STATUS)
	{
	/* same as... */
	__TMS_USBPUMP_URB_DEVICE__HDR;
	/* plus... */

	__TMS_UINT32	PortStatus;
	};

/* PortStatus defineition */
#define	__TMS_USBPUMP_USBDI_PORT_STATUS_ENABLED		(1u << 0)
#define	__TMS_USBPUMP_USBDI_PORT_STATUS_CONNECTED	(1u << 1)

/**** for the reset pipe ****/
__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_RESET_PIPE);
struct __TMS_STRUCTNAME(USBPUMP_URB_RESET_PIPE)
	{
	/* same as... */
	__TMS_USBPUMP_URB_PIPE_CONTROL__HDR;
	/* plus... */
	__TMS_UINT32	ResetPipeFlags;
	};

/**** for the abort pipe ****/
__TMS_TYPE_DEF_STRUCT(USBPUMP_URB_ABORT_PIPE);
struct __TMS_STRUCTNAME(USBPUMP_URB_ABORT_PIPE)
	{
	/* same as... */
	__TMS_USBPUMP_URB_PIPE_CONTROL__HDR;
	/* plus... */
	__TMS_UINT32	AbortPipeFlags;
	};

/****************************************************************************\
|
|	The union of requests
|
\****************************************************************************/

union __TMS_UNIONNAME(USBPUMP_URB)
	{
	__TMS_USBPUMP_URB_HDR			UrbHdr;
	__TMS_USBPUMP_URB_DEVICE_CONTROL	UrbDeviceControl;
	__TMS_USBPUMP_URB_GET_DEVICE_INFO	UrbGetDeviceInfo;
	__TMS_USBPUMP_URB_GET_FRAME		UrbGetFrame;
	__TMS_USBPUMP_URB_PIPE_CONTROL		UrbPipeControl;
	__TMS_USBPUMP_URB_PIPE_INOUT		UrbPipeInOut;
	__TMS_USBPUMP_URB_BULKINT_IN		UrbBulkIntIn;
	__TMS_USBPUMP_URB_BULKINT_OUT		UrbBulkIntOut;
	__TMS_USBPUMP_URB_BULKINT_INOUT		UrbBulkIntInOut;
	__TMS_USBPUMP_URB_CONTROL_IN		UrbControlIn;
	__TMS_USBPUMP_URB_CONTROL_OUT		UrbControlOut;
	__TMS_USBPUMP_URB_CONTROL_INOUT		UrbControlInOut;
	__TMS_USBPUMP_URB_ISOCH_IN		UrbIsochIn;
	__TMS_USBPUMP_URB_ISOCH_OUT		UrbIsochOut;
	__TMS_USBPUMP_URB_ISOCH_INOUT		UrbIsochInOut;
	__TMS_USBPUMP_URB_DEFINE_CONFIG		UrbDefineConfig;
	__TMS_USBPUMP_URB_SUGGEST_CONFIG	UrbSuggestConfig;
	__TMS_USBPUMP_URB_GET_PORT_STATUS	UrbGetPortStatus;
	__TMS_USBPUMP_URB_BULKINT_STREAM_IN	UrbBulkIntStreamIn;
	__TMS_USBPUMP_URB_BULKINT_STREAM_OUT	UrbBulkIntStreamOut;
	__TMS_USBPUMP_URB_BULKINT_STREAM_INOUT	UrbBulkIntStreamInOut;
	__TMS_USBPUMP_URB_RESET_PIPE		UrbResetPipe;
	__TMS_USBPUMP_URB_ABORT_PIPE		UrbAbortPipe;
	};

/****************************************************************************\
|
|	Some useful APIs (mostly for debugging)
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_CONST __TMS_TEXT *
UsbPumpUrb_RequestName(
	__TMS_ARG_USBPUMP_URB_CODE
	);

__TMS_END_DECLS

/****************************************************************************\
|
|	URB initialization macros
|
\****************************************************************************/

/* basic function for preparing the header */
#define	UsbPumpUrb_PrepareHeader(					\
	A_pRequest, A_Length, A_Code, A_PortKey, A_HcdTimeout		\
	)								\
   do	{								\
	__TMS_USBPUMP_URB_LENGTH __TMS_CONST a__Length = (A_Length);	\
	__TMS_USBPUMP_URB_CODE __TMS_CONST a__Code = (A_Code);		\
	__TMS_USBPUMP_USBDI_PORT_KEY __TMS_CONST a__PortKey = (A_PortKey); \
	__TMS_UINT16 __TMS_CONST a__HcdTimeout = (A_HcdTimeout);	\
									\
	(A_pRequest)->UrbHdr.Length = a__Length;			\
	(A_pRequest)->UrbHdr.Request = a__Code;				\
	(A_pRequest)->UrbHdr.PortKey = a__PortKey;			\
	(A_pRequest)->UrbHdr.HcdTimeout = a__HcdTimeout;		\
	} while (0)

/* prepare a DeviceControl URB */
#define	UsbPumpUrb_PrepareDeviceControl(				\
	A_pRequest, A_Length, A_Code, A_PortKey, A_HcdTimeout		\
	)								\
   do	{								\
	UsbPumpUrb_PrepareHeader(					\
		(A_pRequest), (A_Length), (A_Code), (A_PortKey),	\
		(A_HcdTimeout)						\
		);							\
	} while (0)

/* prepare an URB for Reenumerate port */
#define	UsbPumpUrb_PrepareReenumeratePort(				\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout			\
	)								\
   do	{								\
	UsbPumpUrb_PrepareHeader(					\
		(A_pRequest), (A_Length),				\
		__TMS_USBPUMP_URB_RQ_REENUMERATE_PORT, (A_PortKey),	\
		(A_HcdTimeout)						\
		);							\
	} while (0)

/* prepare an URB for Reset Port */
#define	UsbPumpUrb_PrepareResetPort(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout			\
	)								\
   do	{								\
	UsbPumpUrb_PrepareHeader(					\
		(A_pRequest), (A_Length),				\
		__TMS_USBPUMP_URB_RQ_RESET_PORT, (A_PortKey),		\
		(A_HcdTimeout)						\
		);							\
	} while (0)

/* prepare an URB for Get Port Status */
#define	UsbPumpUrb_PrepareGetPortStatus(				\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout			\
	)								\
   do	{								\
	UsbPumpUrb_PrepareHeader(					\
		(A_pRequest), (A_Length),				\
		__TMS_USBPUMP_URB_RQ_GET_PORT_STATUS, (A_PortKey),	\
		(A_HcdTimeout)						\
		);							\
	} while (0)

/* prepare a GetDeviceInfo URB */
#define	UsbPumpUrb_PrepareGetDeviceInfo(				\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout			\
	)								\
   do	{								\
	UsbPumpUrb_PrepareHeader(					\
		(A_pRequest), (A_Length),				\
		__TMS_USBPUMP_URB_RQ_GET_DEVICE_INFO, (A_PortKey),	\
		(A_HcdTimeout)						\
		);							\
	} while (0)

/* prepare a GetFrame URB */
#define	UsbPumpUrb_PrepareGetFrame(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout			\
	)								\
   do	{								\
	UsbPumpUrb_PrepareHeader(					\
		(A_pRequest), (A_Length),				\
		__TMS_USBPUMP_URB_RQ_GET_FRAME, (A_PortKey),		\
		(A_HcdTimeout)						\
		);							\
	} while (0)

/* prepare a PipeControl URB */
#define	UsbPumpUrb_PreparePipeControl(					\
	A_pRequest, A_Length, A_Code, A_PortKey, A_HcdTimeout,		\
	A_hPipe								\
	)								\
   do	{								\
	__TMS_USBPUMP_USBDI_PIPE_HANDLE CONST a__hPipe = (A_hPipe);	\
									\
	UsbPumpUrb_PrepareHeader(					\
		(A_pRequest), (A_Length), (A_Code), (A_PortKey),	\
		(A_HcdTimeout)						\
		);							\
									\
	(A_pRequest)->UrbPipeControl.hPipe = a__hPipe;			\
	} while (0)

/* prepare AbortPipe Urb */
#define	UsbPumpUrb_PrepareAbortPipe_V2(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_AbortPipeFlags					\
	)								\
    do	{								\
    	UsbPumpUrb_PreparePipeControl(					\
		A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_ABORT_PIPE,	\
		A_PortKey, A_HcdTimeout,				\
		A_hPipe							\
		);							\
	(A_pRequest)->UrbAbortPipe.AbortPipeFlags = (A_AbortPipeFlags);	\
	} while (0)

#define	UsbPumpUrb_PrepareAbortPipe(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe								\
	)								\
    UsbPumpUrb_PrepareAbortPipe_V2(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, /* V2: AbortPipeFlags */ 0				\
	)

/* prepare ResetPipe URB */
#define	UsbPumpUrb_PrepareResetPipe_V2(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_ResetPipeFlags					\
	)								\
    do	{								\
	UsbPumpUrb_PreparePipeControl(					\
		A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_RESET_PIPE,	\
		A_PortKey, A_HcdTimeout,				\
		A_hPipe							\
		);							\
	(A_pRequest)->UrbResetPipe.ResetPipeFlags = (A_ResetPipeFlags);	\
	} while (0)

#define	UsbPumpUrb_PrepareResetPipe(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe								\
	)								\
    UsbPumpUrb_PrepareResetPipe_V2(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, /* V2: ResetPipeFlags */ 0				\
	)

/* INTERNAL USE ONLY:  prepare a BulkIntIn or Out URB */
#define	UsbPumpUrbI_PrepareBulkIntInOut(				\
	A_FieldName,							\
	A_pRequest, A_Length, A_Code, A_PortKey, A_HcdTimeout,		\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	A_hBuffer, A_TransferFlags					\
	)								\
   do	{								\
	UsbPumpUrb_PreparePipeControl(					\
		(A_pRequest), (A_Length), (A_Code), (A_PortKey),	\
		(A_HcdTimeout), (A_hPipe)				\
		);							\
									\
	(A_pRequest)->A_FieldName.pBuffer = (A_pBuffer);		\
	(A_pRequest)->A_FieldName.nBuffer = (A_nBuffer);		\
	(A_pRequest)->A_FieldName.hBuffer = (A_hBuffer);		\
	(A_pRequest)->A_FieldName.TransferFlags = (A_TransferFlags);	\
	} while (0)

/* INTERNAL USE ONLY:  prepare a BulkIntStreamIn or Out URB */
#define	UsbPumpUrbI_PrepareBulkIntStreamInOut(				\
	A_FieldName,							\
	A_pRequest, A_Length, A_Code, A_PortKey, A_HcdTimeout,		\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	A_hBuffer, A_TransferFlags, A_StreamID				\
	)								\
   do	{								\
	UsbPumpUrb_PreparePipeControl(					\
		(A_pRequest), (A_Length), (A_Code), (A_PortKey),	\
		(A_HcdTimeout), (A_hPipe)				\
		);							\
									\
	(A_pRequest)->A_FieldName.pBuffer = (A_pBuffer);		\
	(A_pRequest)->A_FieldName.nBuffer = (A_nBuffer);		\
	(A_pRequest)->A_FieldName.hBuffer = (A_hBuffer);		\
	(A_pRequest)->A_FieldName.TransferFlags = (A_TransferFlags);	\
	(A_pRequest)->A_FieldName.StreamID = (A_StreamID);		\
	} while (0)

/* prepare a BulkIntIn URB */
#define	UsbPumpUrb_PrepareBulkIntIn(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer, A_TransferFlags			\
	)								\
   UsbPumpUrbI_PrepareBulkIntInOut(					\
	UrbBulkIntIn,							\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_BULKINT_IN,		\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	/* hBuffer */ __TMS_NULL, A_TransferFlags			\
	)

#define	UsbPumpUrb_PrepareBulkIntIn_V2(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	A_hBuffer, A_TransferFlags					\
	)								\
   UsbPumpUrbI_PrepareBulkIntInOut(					\
	UrbBulkIntIn,							\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_BULKINT_IN,		\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	A_hBuffer, A_TransferFlags					\
	)

/* prepare a BulkIntStreamIn URB */
#define	UsbPumpUrb_PrepareBulkIntStreamIn(				\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	A_hBuffer, A_TransferFlags, A_StreamID				\
	)								\
   UsbPumpUrbI_PrepareBulkIntStreamInOut(				\
	UrbBulkIntStreamIn,						\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_IN,	\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	A_hBuffer, A_TransferFlags, A_StreamID				\
	)

/* prepare a BulkIntOutUrb */
#define	UsbPumpUrb_PrepareBulkIntOut(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer, A_TransferFlags			\
	)								\
   UsbPumpUrbI_PrepareBulkIntInOut(					\
	UrbBulkIntOut,							\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_BULKINT_OUT,		\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	/* hBuffer */ __TMS_NULL, A_TransferFlags			\
	)

#define	UsbPumpUrb_PrepareBulkIntOut_V2(				\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	A_hBuffer, A_TransferFlags					\
	)								\
   UsbPumpUrbI_PrepareBulkIntInOut(					\
	UrbBulkIntOut,							\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_BULKINT_OUT,		\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	A_hBuffer, A_TransferFlags					\
	)

/* prepare a BulkIntStreamOutUrb */
#define	UsbPumpUrb_PrepareBulkIntStreamOut(				\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	A_hBuffer, A_TransferFlags, A_StreamID				\
	)								\
   UsbPumpUrbI_PrepareBulkIntStreamInOut(				\
	UrbBulkIntStreamOut,						\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_OUT,	\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	A_hBuffer, A_TransferFlags, A_StreamID				\
	)

/* prepare a control in or out URB (INTERNAL only) */
#define	UsbPumpUrbI_PrepareControlIO(					\
	A_FieldName,							\
	A_pRequest, A_Length, A_Code, A_PortKey, A_HcdTimeout,		\
	A_hPipe, A_pBuffer, A_nBuffer, A_hBuffer,			\
	A_TransferFlags, A_pSetup					\
	)								\
   do	{								\
	__TMS_CONST __TMS_USETUP_WIRE * __TMS_CONST a__pSetup =		\
			(A_pSetup);					\
	UsbPumpUrb_PreparePipeControl(					\
		(A_pRequest), (A_Length), (A_Code), (A_PortKey),	\
		(A_HcdTimeout), (A_hPipe)				\
		);							\
									\
	(A_pRequest)->A_FieldName.pBuffer = (A_pBuffer);		\
	(A_pRequest)->A_FieldName.nBuffer = (A_nBuffer);		\
	(A_pRequest)->A_FieldName.hBuffer = (A_hBuffer);		\
	(A_pRequest)->A_FieldName.TransferFlags = (A_TransferFlags);	\
	if (a__pSetup)							\
		(A_pRequest)->A_FieldName.Setup = *a__pSetup;		\
	} while (0)

/* prepare a ControlIn URB */
#define	UsbPumpUrb_PrepareControlIn(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer, A_TransferFlags,			\
	A_pSetup							\
	)								\
   UsbPumpUrbI_PrepareControlIO(					\
	UrbControlIn,							\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_CONTROL_IN,		\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer, /* hBuffer */ __TMS_NULL,	\
	A_TransferFlags, A_pSetup					\
	)

#define	UsbPumpUrb_PrepareControlIn_V2(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer, A_hBuffer,			\
	A_TransferFlags, A_pSetup					\
	)								\
   UsbPumpUrbI_PrepareControlIO(					\
	UrbControlIn,							\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_CONTROL_IN,		\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer, A_hBuffer,			\
	A_TransferFlags, A_pSetup					\
	)

/* prepare a ControlOutUrb */
#define	UsbPumpUrb_PrepareControlOut(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer, A_TransferFlags,			\
	A_pSetup							\
	)								\
   UsbPumpUrbI_PrepareControlIO(					\
	UrbControlOut,							\
	A_pRequest, A_Length,						\
	__TMS_USBPUMP_URB_RQ_CONTROL_OUT, A_PortKey, A_HcdTimeout,	\
	A_hPipe, A_pBuffer, A_nBuffer, /* hBuffer */ __TMS_NULL,	\
	A_TransferFlags, A_pSetup					\
	)

#define	UsbPumpUrb_PrepareControlOut_V2(				\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer, A_hBuffer,			\
	A_TransferFlags, A_pSetup					\
	)								\
   UsbPumpUrbI_PrepareControlIO(					\
	UrbControlOut,							\
	A_pRequest, A_Length,						\
	__TMS_USBPUMP_URB_RQ_CONTROL_OUT, A_PortKey, A_HcdTimeout,	\
	A_hPipe, A_pBuffer, A_nBuffer, A_hBuffer,			\
	A_TransferFlags, A_pSetup					\
	)

/* prepare the header for a Isoch I/O URB (internal only!) */
#define	UsbPumpUrbI_PrepareIsochIO(					\
	A_FieldName,							\
	A_pRequest, A_Length, A_Code, A_PortKey, A_HcdTimeout,		\
	A_hPipe, A_pBuffer, A_nBuffer, A_hBuffer, A_TransferFlags,	\
	A_pIsochDescr, A_IsochDescrSize, A_hIsochDescr,			\
	A_IsochStartFrame						\
	)								\
   do	{								\
	UsbPumpUrb_PreparePipeControl(					\
		(A_pRequest), (A_Length), (A_Code), (A_PortKey),	\
		(A_HcdTimeout), (A_hPipe)				\
		);							\
									\
	(A_pRequest)->A_FieldName.pBuffer = (A_pBuffer);		\
	(A_pRequest)->A_FieldName.nBuffer = (A_nBuffer);		\
	(A_pRequest)->A_FieldName.hBuffer = (A_hBuffer);		\
	(A_pRequest)->A_FieldName.TransferFlags = (A_TransferFlags);	\
	(A_pRequest)->A_FieldName.pIsochDescr = (A_pIsochDescr);	\
	(A_pRequest)->A_FieldName.IsochDescrSize = (A_IsochDescrSize);	\
	(A_pRequest)->A_FieldName.hIsochDescr = (A_hIsochDescr);	\
	(A_pRequest)->A_FieldName.IsochStartFrame = (A_IsochStartFrame); \
	} while (0)

/* prepare the header for a Isoch In URB */
#define	UsbPumpUrb_PrepareIsochIn(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer, A_TransferFlags,			\
	A_pIsochDescr, A_IsochDescrSize, A_IsochStartFrame		\
	)								\
   UsbPumpUrbI_PrepareIsochIO(						\
	UrbIsochIn,							\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_ISOCH_IN,		\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	/* hBuffer */ __TMS_NULL, A_TransferFlags,			\
	A_pIsochDescr, A_IsochDescrSize,				\
	/* hIsochDescr */ __TMS_NULL, A_IsochStartFrame			\
	)

#define	UsbPumpUrb_PrepareIsochIn_V2(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer, A_hBuffer, A_TransferFlags,	\
	A_pIsochDescr, A_IsochDescrSize, A_hIsochDescr,			\
	A_IsochStartFrame						\
	)								\
   UsbPumpUrbI_PrepareIsochIO(						\
	UrbIsochIn,							\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_ISOCH_IN,		\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer, A_hBuffer, A_TransferFlags,	\
	A_pIsochDescr, A_IsochDescrSize, A_hIsochDescr,			\
	A_IsochStartFrame						\
	)

/* prepare the header for a Isoch Out URB */
#define	UsbPumpUrb_PrepareIsochOut(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer, A_TransferFlags,			\
	A_pIsochDescr, A_IsochDescrSize, A_IsochStartFrame		\
	)								\
   UsbPumpUrbI_PrepareIsochIO(						\
	UrbIsochOut,							\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_ISOCH_OUT,		\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer,					\
	/* hBuffer */ __TMS_NULL, A_TransferFlags,			\
	A_pIsochDescr, A_IsochDescrSize,				\
	/* hIsochDescr */ __TMS_NULL, A_IsochStartFrame			\
	)

#define	UsbPumpUrb_PrepareIsochOut_V2(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_hPipe, A_pBuffer, A_nBuffer, A_hBuffer, A_TransferFlags,	\
	A_pIsochDescr, A_IsochDescrSize, A_hIsochDescr,			\
	A_IsochStartFrame						\
	)								\
   UsbPumpUrbI_PrepareIsochIO(						\
	UrbIsochOut,							\
	A_pRequest, A_Length, __TMS_USBPUMP_URB_RQ_ISOCH_OUT,		\
	A_PortKey, A_HcdTimeout,					\
	A_hPipe, A_pBuffer, A_nBuffer, A_hBuffer, A_TransferFlags,	\
	A_pIsochDescr, A_IsochDescrSize, A_hIsochDescr,			\
	A_IsochStartFrame						\
	)

/* prepare header for define config */
#define UsbPumpUrb_PrepareDefineConfig_V2(				\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_ConfigFlags,							\
	A_pRootConfigNode, A_sizeConfigBuffer				\
	)								\
   do	{								\
	UsbPumpUrb_PrepareHeader(					\
		(A_pRequest), (A_Length),				\
		__TMS_USBPUMP_URB_RQ_DEFINE_CONFIG, (A_PortKey),	\
		(A_HcdTimeout)						\
		);							\
	(A_pRequest)->UrbDefineConfig.ConfigFlags =			\
		(A_ConfigFlags);					\
	(A_pRequest)->UrbDefineConfig.pRootConfigNode =			\
		(A_pRootConfigNode);					\
	(A_pRequest)->UrbDefineConfig.sizeConfigBuffer =		\
		(A_sizeConfigBuffer);					\
	} while (0)

#define UsbPumpUrb_PrepareDefineConfig(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_pRootConfigNode, A_sizeConfigBuffer				\
	)								\
	UsbPumpUrb_PrepareDefineConfig_V2(				\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	/* V2: ConfigFlags */ 0,					\
	A_pRootConfigNode, A_sizeConfigBuffer				\
	)

/* prepare header for define config */
#define UsbPumpUrb_PrepareSuggestConfig_V2(				\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_pRootConfigNode, A_sizeConfigBuffer,				\
	A_pOptionalConfigBundles, A_sizeConfigBundles,			\
	A_pRxDeviceDescFn, A_pRxConfigBundleFn				\
	)								\
   do	{								\
	UsbPumpUrb_PrepareHeader(					\
		(A_pRequest), (A_Length),				\
		__TMS_USBPUMP_URB_RQ_SUGGEST_CONFIG, (A_PortKey),	\
		(A_HcdTimeout)						\
		);							\
	(A_pRequest)->UrbSuggestConfig.pConfigBuffer =			\
		(A_pRootConfigNode);					\
	(A_pRequest)->UrbSuggestConfig.sizeConfigBuffer =		\
		(A_sizeConfigBuffer);					\
	(A_pRequest)->UrbSuggestConfig.pOptionalConfigBundles =		\
		(A_pOptionalConfigBundles);				\
	(A_pRequest)->UrbSuggestConfig.sizeConfigBundles =		\
		(A_sizeConfigBundles);					\
	(A_pRequest)->UrbSuggestConfig.pRxDeviceDescFn =		\
		(A_pRxDeviceDescFn);					\
	(A_pRequest)->UrbSuggestConfig.pRxConfigBundleFn =		\
		(A_pRxConfigBundleFn);					\
	} while (0)

#define UsbPumpUrb_PrepareSuggestConfig(				\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_pRootConfigNode, A_sizeConfigBuffer,				\
	A_pOptionalConfigBundles, A_sizeConfigBundles			\
	)								\
    UsbPumpUrb_PrepareSuggestConfig_V2(					\
	A_pRequest, A_Length, A_PortKey, A_HcdTimeout,			\
	A_pRootConfigNode, A_sizeConfigBuffer,				\
	A_pOptionalConfigBundles, A_sizeConfigBundles,			\
	/* V2: pRxDeviceDescFn */ __TMS_NULL,				\
	/* V2: pRxConfigBundleFn */ __TMS_NULL				\
	)


/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USE_USBPUMP_TIMER_32BIT	\
   __TMS_USE_USBPUMP_TIMER_32BIT
# define USBPUMP_URB_RQ_GET_DEVICE_INFO	\
   __TMS_USBPUMP_URB_RQ_GET_DEVICE_INFO
# define USBPUMP_URB_RQ_CONTROL_IN	\
   __TMS_USBPUMP_URB_RQ_CONTROL_IN
# define USBPUMP_URB_RQ_CONTROL_OUT	\
   __TMS_USBPUMP_URB_RQ_CONTROL_OUT
# define USBPUMP_URB_RQ_BULKINT_IN	\
   __TMS_USBPUMP_URB_RQ_BULKINT_IN
# define USBPUMP_URB_RQ_BULKINT_OUT	\
   __TMS_USBPUMP_URB_RQ_BULKINT_OUT
# define USBPUMP_URB_RQ_ISOCH_IN	\
   __TMS_USBPUMP_URB_RQ_ISOCH_IN
# define USBPUMP_URB_RQ_ISOCH_OUT	\
   __TMS_USBPUMP_URB_RQ_ISOCH_OUT
# define USBPUMP_URB_RQ_DEFINE_CONFIG	\
   __TMS_USBPUMP_URB_RQ_DEFINE_CONFIG
# define USBPUMP_URB_RQ_ABORT_PIPE	\
   __TMS_USBPUMP_URB_RQ_ABORT_PIPE
# define USBPUMP_URB_RQ_RESET_PIPE	\
   __TMS_USBPUMP_URB_RQ_RESET_PIPE
# define USBPUMP_URB_RQ_REENUMERATE_PORT	\
   __TMS_USBPUMP_URB_RQ_REENUMERATE_PORT
# define USBPUMP_URB_RQ_RESET_PORT	\
   __TMS_USBPUMP_URB_RQ_RESET_PORT
# define USBPUMP_URB_RQ_GET_PORT_STATUS	\
   __TMS_USBPUMP_URB_RQ_GET_PORT_STATUS
# define USBPUMP_URB_RQ_GET_FRAME	\
   __TMS_USBPUMP_URB_RQ_GET_FRAME
# define USBPUMP_URB_RQ_SUGGEST_CONFIG	\
   __TMS_USBPUMP_URB_RQ_SUGGEST_CONFIG
# define USBPUMP_URB_RQ_BULKINT_STREAM_IN	\
   __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_IN
# define USBPUMP_URB_RQ_BULKINT_STREAM_OUT	\
   __TMS_USBPUMP_URB_RQ_BULKINT_STREAM_OUT
# define USBPUMP_URB_RQ__MAX	\
   __TMS_USBPUMP_URB_RQ__MAX
# define USBPUMP_URB_RQ_NAME__INIT	\
   __TMS_USBPUMP_URB_RQ_NAME__INIT
# define USBPUMP_URB_RQ_SIZE__INIT	\
   __TMS_USBPUMP_URB_RQ_SIZE__INIT
# define USBPUMP_URB_RQ_HAS_hPIPE_MASK	\
   __TMS_USBPUMP_URB_RQ_HAS_hPIPE_MASK
# define USBPUMP_URB_RQ_HAS_hPIPE(request)	\
   __TMS_USBPUMP_URB_RQ_HAS_hPIPE(request)
# define USBPUMP_URB_RQ_NULL_hPIPE_OK_MASK	\
   __TMS_USBPUMP_URB_RQ_NULL_hPIPE_OK_MASK
# define USBPUMP_URB_RQ_NULL_hPIPE_OK(request)	\
   __TMS_USBPUMP_URB_RQ_NULL_hPIPE_OK(request)
# define USBPUMP_URB_RQ_HAS_nACTUAL_MASK	\
   __TMS_USBPUMP_URB_RQ_HAS_nACTUAL_MASK
# define USBPUMP_URB_RQ_HAS_nACTUAL(request)	\
   __TMS_USBPUMP_URB_RQ_HAS_nACTUAL(request)
# define USBPUMP_URB_RQ_NON_CONTROL_TRANSFER_MASK	\
   __TMS_USBPUMP_URB_RQ_NON_CONTROL_TRANSFER_MASK
# define USBPUMP_URB_RQ_NON_CONTROL_TRANSFER(request)	\
   __TMS_USBPUMP_URB_RQ_NON_CONTROL_TRANSFER(request)
# define USBPUMP_URB_TRANSFER_FLAG_ASAP	\
   __TMS_USBPUMP_URB_TRANSFER_FLAG_ASAP
# define USBPUMP_URB_TRANSFER_FLAG_SHORT_OK	\
   __TMS_USBPUMP_URB_TRANSFER_FLAG_SHORT_OK
# define USBPUMP_URB_TRANSFER_FLAG_LINKED	\
   __TMS_USBPUMP_URB_TRANSFER_FLAG_LINKED
# define USBPUMP_URB_TRANSFER_FLAG_POST_BREAK	\
   __TMS_USBPUMP_URB_TRANSFER_FLAG_POST_BREAK
# define USBPUMP_URB_TRANSFER_FLAG_FILTER_EXTRA_DESC	\
   __TMS_USBPUMP_URB_TRANSFER_FLAG_FILTER_EXTRA_DESC
# define USBPUMP_URB_ABORT_PIPE_FLAG_WAIT_COMPLETION	\
   __TMS_USBPUMP_URB_ABORT_PIPE_FLAG_WAIT_COMPLETION
# define USBPUMP_URB_RESET_PIPE_FLAG_DO_CLEAR_FEATURE	\
   __TMS_USBPUMP_URB_RESET_PIPE_FLAG_DO_CLEAR_FEATURE
# define USBPUMP_URB_RESET_PIPE_FLAG_CLEAR_HOST_TOGGLE	\
   __TMS_USBPUMP_URB_RESET_PIPE_FLAG_CLEAR_HOST_TOGGLE
# define USBPUMP_URB_DEFINE_CONFIG_FLAG_DYNAMIC	\
   __TMS_USBPUMP_URB_DEFINE_CONFIG_FLAG_DYNAMIC
# define USBPUMP_URB_INTERNAL_FLAG_CANCELLED	\
   __TMS_USBPUMP_URB_INTERNAL_FLAG_CANCELLED
# define USBPUMP_URB_INTERNAL_FLAG_PASSTHRU	\
   __TMS_USBPUMP_URB_INTERNAL_FLAG_PASSTHRU
# define USBPUMP_URB_INTERNAL_FLAG_ABORTING	\
   __TMS_USBPUMP_URB_INTERNAL_FLAG_ABORTING
# define USBPUMP_URB_INTERNAL_FLAG_FRAME_UPDATED	\
   __TMS_USBPUMP_URB_INTERNAL_FLAG_FRAME_UPDATED
# define USBPUMP_URB_INTERNAL_FLAG_LPM_INCREMENTED	\
   __TMS_USBPUMP_URB_INTERNAL_FLAG_LPM_INCREMENTED
# define USBPUMP_URB_INTERNAL_FLAG_CLEAN_UP_CONFIG	\
   __TMS_USBPUMP_URB_INTERNAL_FLAG_CLEAN_UP_CONFIG
# define USBPUMP_URB_INTERNAL_FLAG_LPM_QUEUED	\
   __TMS_USBPUMP_URB_INTERNAL_FLAG_LPM_QUEUED
# define USBPUMP_URB__HDR	\
   __TMS_USBPUMP_URB__HDR
# define USBPUMP_URB_DEVICE__HDR	\
   __TMS_USBPUMP_URB_DEVICE__HDR
# define USBPUMP_URB_PIPE_CONTROL__HDR	\
   __TMS_USBPUMP_URB_PIPE_CONTROL__HDR
# define USBPUMP_URB_PIPE_TRANSFER__HDR(BufferType)	\
   __TMS_USBPUMP_URB_PIPE_TRANSFER__HDR(BufferType)
# define USBPUMP_URB_CONTROL_TRANSFER__HDR(BufferType)	\
   __TMS_USBPUMP_URB_CONTROL_TRANSFER__HDR(BufferType)
# define USBPUMP_URB_ISOCH_TRANSFER__HDR(BufferType)	\
   __TMS_USBPUMP_URB_ISOCH_TRANSFER__HDR(BufferType)
# define USBPUMP_USBDI_PORT_STATUS_ENABLED	\
   __TMS_USBPUMP_USBDI_PORT_STATUS_ENABLED
# define USBPUMP_USBDI_PORT_STATUS_CONNECTED	\
   __TMS_USBPUMP_USBDI_PORT_STATUS_CONNECTED
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_request.h ****/
#endif /* _USBPUMP_USBDI_REQUEST_H_ */
