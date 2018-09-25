/* usbpump_hcd_pipe.h	Wed Apr 01 2015 15:22:04 tmm */

/*

Module:  usbpump_hcd_pipe.h

Function:
	Home for USBPUMP_HCD_PIPE definition.

Version:
	V3.15b	Wed Apr 01 2015 15:22:04 tmm	Edit level 17

Copyright notice:
	This file copyright (C) 2005-2010, 2012, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	May 2005

Revision history:
   1.91d  Mon May 30 2005 17:45:06  tmm
	Module created.

   1.93c  Tue Dec  6 2005 22:13:42  tmm
	Add a macro to reset a pipe's state.

   1.93e  Fri Jan 27 2006 17:19:14  tmm
	Add tracking fields for periodic schedule insertion points.

   1.97h  Thu Mar 29 2007 13:23:55  djt
	Fixed documentation for USBPUMP_HCD_REQUEST.

   1.97h  Tue Apr 24 2007 14:22:14  chwon
	Fixed revision history format

   1.97i  Wed Jul 11 2007 08:19:18  djt
	Added StartSplitMask and CompleteSplitMask.

   1.97i  Thu Jul 12 2007 11:31:17  djt
	Added missing end comments.

   1.97j  Mon Sep 10 2007 19:09:54  djt
	3265: Added fClearTT

   1.97k  Thu Jan 31 2008 12:59:28  tmm
	Correct errors in documentation.

   1.97k  Fri Sep 26 2008 09:37:38  djt
	6900: Added DataMask

   2.01a  Mon Feb 23 2009 09:54:22  djt
	7416: Added USB 3.0 fields

   3.01a  Fri Jan 22 2010 10:22:40  chwon
	9783: Valid bMaxBurstSize value is [1..16]. Change default value
	from 0 to 1.

   3.01a  Mon Feb  8 2010 19:52:44  tmm
	9893:  rename bMaxPStreams to wMaxStreamID and resize.

   3.01f  Wed May 30 2012 12:01:20  chwon
	15440: Added bInterfaceNumber in USBPUMP_HCD_PIPE and _V3() macro.

   3.15b  Mon Mar 30 2015 12:36:33  tmm
	19019: Update internal documentation.

   3.15b  Wed Apr 01 2015 15:22:04  tmm
	19019: Update internal documentation.

*/

#ifndef _USBPUMP_HCD_PIPE_H_		/* prevent multiple includes */
#define _USBPUMP_HCD_PIPE_H_

#ifndef _USBPUMP_HCD_TYPES_H_
# include "usbpump_hcd_types.h"
#endif

#ifndef _USBPUMP_DEVSPEED_H_
# include "usbpump_devspeed.h"
#endif

/*

Type:	USBPUMP_HCD_PIPE

Function:
	A simple structure that represents a data flow to an HCD.

Description:
	HCDs use USBPUMP_HCD_PIPE instances to model logical pipes to
	their managed HC.  These objects are allocated by USBD as part
	of the schedule creation process.  The main purpose is to serve
	as a queue-head for the USBPUMP_HCD_REQUEST operations that target
	pipes.  It also provides a convenient point of abstraction for
	USBD to use, so that complex common operations can be implemented
	by USBD.

Contents:
	The following fields are wirtten by the client, and must never be
	modified by the HCD.

	UINT32 Tag;
		The tag, 'UHpi' arranged in byte order so it dumps as
		"UHpi" in the debugger.

	USBPUMP_HCD_DEVICE *pHcdDevice;
		This points to the object that models the
		containing USB device to the HCD.  (This allows for
		common storage of speed, address, default pipe, etc.)
		The USBPUMP_HCD_DEVICE in turn points to the host
		controller object USBPUMP_HCD.

	BYTES MaxTransferSize;
		The configured maximum transfer size for this pipe.  This size 
		is advisory to the HCD, and may be used by the HCD to optimize 
		resource allocation.

	UINT16 wInterval;
		The interval for this pipe in (micro)frames. If the pipe is 
		for a full-speed or low-speed device, then this is in frames.  
		If the pipe is for a high-speed device, then this is in 
		microframes. 

	UINT16 wMaxPacketSize;
		The maximum packet size for this endpoint, in a form
		that should be fast for comparison and computation.  For
		high-bandwidth periodic endpoints, this is the base max-
		packet size (bits 10..0 of the endpoint descriptor).
		Bits 15..11 will always be zero.  For other kinds of endpoints,
		this is the max-packet size.

	UINT8 bMaxBurstSize;
		USB 3.0. The maximum consecutive USB transactions that should be 
		executed per scheduling opportunity.  Valid values are from 1 to
		16.  SuperSpeed Endpoint Companion Descriptor:bMaxBurst + 1.

	UINT16 wMaxStreamID;
		USB 3.0: maximum stream ID supported.
		Only applicable to bulk endpoints.
		Values based on the endpoint companion descriptor are  0, 2, 
		4, 8, 16, .. 32,768, 65,533 (sic); but note that some HCs
		erroneously may not support (for example) a max StreamID of 
		32, but only a max ID of 31.  So we store the max stream ID,
		which allows the HC to reduce the stream ID to something it
		can support.

		A value of 0 indicates that the endpoint does not support 
		streams. 

	UINT16 wBytesPerInterval;
		USB 3.0. The total number of bytes this endpoint will transfer 
		every service interval. 

	UINT8 bMult;
		USB 3.0. A zero based value that determines the maximum number 
		of packets within a service interval that this endpoint supports.
		From SuperSpeed Endpoint Companion Descriptor:bmAttributes Mult 
		field. Set for periodic endpoints only. Always 0 for interrupt
		endpoint.

	UINT8 bEndpointAddress;
		The endpoint address for this pipe (includes IN/OUT
		bit in bit 7).

	UINT8 bEndpointType;
		The endpoint type (bulk, interrupt, isoch, control)

	UINT8 bInterfaceNumber;
		The interface number for this pipe.

	UINT8 StartSplitMask;
		This tells the HCD in which microframe to schedule the Start 
		Split transactions. 

	UINT8 CompleteSplitMask;
		This tells the HCD in which microframe to schedule the Complete 
		Split transactions. 

	UINT8 PeriodicLayer;
		For periodic endpoints only, this field gives the layer
		within the abstract binary schedule that this endpoint
		has been assigned to.  Architecturally, this is required
		to be a number in the range from 0 to 7, corresponding
		to polling intervals of 1, 2, 4, ..., 128 microframes.
		This value will always be less than or equal to the
		binary tree depth reported by the HCD at init time.

	UINT8 PeriodicBucket;
		For periodic endpoints only, the field gives the offset
		within the abstract binary schedule tree for this endpoint.
		If many endpoints share the same periodic polling interval,
		USBDI will try to arrange to smooth out the traffic profile
		by starting in different frames/microframes.

	UINT8 bExtraPackets;
		For high-speed periodic endpoints only, this field gives
		the number of extra wMaxPacketSize packets to be transferred
		per frame.  This will only be a value other than 0 if this
		is a high-bandwidth periodic endpoint. In that case, it
		may be 1 or 2.

	The following entries are used for bi-directional communication 
	between the client and the HCD. The sharing model is copy-based, not
	pointer-pased. It is assumed, in other words, that the HCD keeps
	the actual values of both of the following in internal storage, 
	and uses these as input only during processing of 
	USBPUMP_HCD_RQ_UPDATE_PIPE. The HCD must only update these fields
	from within the DataPump thread.

	UINT8 fToggle;
		The data toggle to be used on the next transfer.  Only
		valid when no transfer is pending.  Initialized by client,
		updated by the HCD whenever last transfer completes.  For isoch
		pipes, toggle is not used.   

		If the client changes the data toggle independently of the
		HCD, the client must notify the HCD using 
		USBPUMP_HCD_RQ_UPDATE_PIPE.

	UINT8 fHalted;
		If true, the pipe is halted.  No transfers can be advanced
		from the transfer queue to the hardware.  The HCD sets this
		flag while processing requests; the client can set or reset this, 
		but must notify the HCD using USBPUMP_HCD_RQ_UPDATE_PIPE.

	The following field is an output from the HCD to the client. The HCD
	sets this field as needed. The client is responsible for clearing the 
	field. The HCD must update this field only from within the DataPump 
	thread.

	UINT8 fClearTT;
		If true, the TT is in a state where it may not be able to pass 
		traffic due to HCD cancellation, timeout or error. The HCD
		sets this flag upon request completion, and the client clears 
		it after it clears the TT buffer in the hub.

Private Contents:
	The following fields are for the benefit of the HCD. They are 
	reserved for use by the HCD.  MCCI strongly recommends that these
	fields also only be updated in DataPump context; if the HCD has
	fields that must be updated in some other context, one of the
	HcdReserved[] pointers should be set to point to a separate
	structure allocated for that purpose. 

	USBPUMP_HCD_REQUEST *pRequestQueue;
		The pointer to the list of transfers pending for this
		pipe.  Queued transfers are ultimately placed here.
		Items in this queue are not shared with the hardware,
		so aborts are easy.

	USBPUMP_HCD_PIPE *pNext, *pLast;
		Queue threads for managing pipes on a particular service
		list.

	ADDRBITS_PTR_UNION HcdReserved[4];
		Four entries which can be used as needed by the HCD for
		managing traffic on this pipe.

Notes:
	This object is not necessarily sufficient for managing a full
	USBD, but it should have enough info to get a transfer over to
	the hardware.

See Also:
	USBPUMP_HCD_DEVICE

*/

struct __TMS_STRUCTNAME(USBPUMP_HCD_PIPE)
	{
	__TMS_UINT32		Tag;

	/*
	|| from client:
	*/

	/* pointer to object representing state of owning device */
	__TMS_USBPUMP_HCD_DEVICE *pHcdDevice;

	/* maximum transfer size */
	__TMS_BYTES		MaxTransferSize;

	/* digested interval info */
	__TMS_UINT16		wInterval;

	/* digested max packet size (no extra bits) */
	__TMS_UINT16		wMaxPacketSize;

	/* 
	|| the maximum stream ID supported:  0, or the max value.  HC should
	|| reduce this to the maximum supported value or report an error
	|| if streams are not supported.
	*/
	__TMS_UINT16		wMaxStreamID;

	/* 
	|| total number of bytes this ep will transfer every service interval
	|| (USB 3.0)
	*/
	__TMS_UINT16		wBytesPerInterval;

	/* 
	|| maximum consecutive USB transactions that should be executed per
	|| scheduling opportunity (USB 3.0)
	*/
	__TMS_UINT8		bMaxBurstSize;

	/* 
	|| zero based value that determines the maximum number of packets
	|| within a service interval that this ep supports (USB 3.0)
	*/
	__TMS_UINT8		bMult;

	/* endpoint address 0..0xF, ored with 0x80 for IN */
	__TMS_UINT8		bEndpointAddress;

	/*
	|| endpoint type: 2 bits would suffice, but bitfields can create
	|| subtle headaches
	*/
	__TMS_UINT8		bEndpointType;

	/* scheduling info */
	__TMS_UINT8		PeriodicBucket;
	__TMS_UINT8		PeriodicLevel;

	/*
	|| high-speed high-bandwith periodic endpoints need to worry about
	|| how many extra packets are scheduled per frame.
	*/
	__TMS_UINT8		bExtraPackets;

	/* interface number */
	__TMS_UINT8		bInterfaceNumber;

	/*
	|| shared by client and HCD layer:
	*/

	/* the next data toggle */
	__TMS_UINT8		fToggle;

	/* 0 if running, non-zero if halted */
	__TMS_UINT8		fHalted;

	/* TRUE if TT needs clearing */
	__TMS_UINT8		fClearTT;

	/* TT scheduling info */
	__TMS_UINT8		StartSplitMask;
	__TMS_UINT8		CompleteSplitMask;
	__TMS_UINT8		DataMask;

	/*
	|| private but architectural [common HCD layer]
	*/

	/* queue of waiting transfers */
	__TMS_USBPUMP_HCD_REQUEST	*pRequestQueue;

	/* forward and back links to the other pipes in this service list */
	/* could be asynch queue, or periodic queue */
	__TMS_USBPUMP_HCD_PIPE		*pNext;
	__TMS_USBPUMP_HCD_PIPE		*pLast;

	/* private [but defined by specific HCD]: */
	__TMS_ADDRBITS_PTR_UNION	HcdReserved[4];
	};

/*
|| Tags and so forth
*/
#define	__TMS_USBPUMP_HCD_PIPE_TAG	__TMS_UHIL_MEMTAG('U', 'H', 'p', 'i')

#define	__TMS_USBPUMP_HCD_PIPE_SETUP_V3(				\
	a_pPipe,							\
	a_pHcdDevice,							\
	a_MaxTransferSize,						\
	a_wInterval,							\
	a_wMaxPacketSize,						\
	a_bEndpointAddress,						\
	a_bEndpointType,						\
	a_fToggle,							\
	a_bExtraPackets,						\
	a_bMaxBurstSize,						\
	a_wMaxStreamID,							\
	a_wBytesPerInterval,						\
	a_bMult,							\
	a_bInterfaceNumber						\
	)								\
    do	{								\
	__TMS_USBPUMP_HCD_PIPE * __TMS_CONST a__pPipe = (a_pPipe);	\
									\
	UHIL_fill(a__pPipe, sizeof(*a__pPipe), 0);			\
									\
	a__pPipe->pHcdDevice = (a_pHcdDevice);				\
	a__pPipe->MaxTransferSize = (a_MaxTransferSize);		\
	a__pPipe->wInterval = (a_wInterval);				\
	a__pPipe->wMaxPacketSize = (a_wMaxPacketSize);			\
	a__pPipe->bMaxBurstSize = (a_bMaxBurstSize);			\
	a__pPipe->wMaxStreamID = (a_wMaxStreamID);			\
	a__pPipe->wBytesPerInterval = (a_wBytesPerInterval);		\
	a__pPipe->bMult = (a_bMult);					\
	a__pPipe->bEndpointAddress = (a_bEndpointAddress);		\
	a__pPipe->bEndpointType = (a_bEndpointType);			\
	a__pPipe->fToggle = (a_fToggle);				\
	a__pPipe->bExtraPackets = (a_bExtraPackets);			\
	a__pPipe->bInterfaceNumber = (a_bInterfaceNumber);		\
	a__pPipe->pNext = a__pPipe->pLast = a__pPipe;			\
									\
	a__pPipe->Tag = __TMS_USBPUMP_HCD_PIPE_TAG;			\
	} while (0)

#define	__TMS_USBPUMP_HCD_PIPE_SETUP_V2(				\
	a_pPipe,							\
	a_pHcdDevice,							\
	a_MaxTransferSize,						\
	a_wInterval,							\
	a_wMaxPacketSize,						\
	a_bEndpointAddress,						\
	a_bEndpointType,						\
	a_fToggle,							\
	a_bExtraPackets,						\
	a_bMaxBurstSize,						\
	a_wMaxStreamID,							\
	a_wBytesPerInterval,						\
	a_bMult								\
	)								\
	__TMS_USBPUMP_HCD_PIPE_SETUP_V3(				\
	a_pPipe,							\
	a_pHcdDevice,							\
	a_MaxTransferSize,						\
	a_wInterval,							\
	a_wMaxPacketSize,						\
	a_bEndpointAddress,						\
	a_bEndpointType,						\
	a_fToggle,							\
	a_bExtraPackets,						\
	a_bMaxBurstSize,						\
	a_wMaxStreamID,							\
	a_wBytesPerInterval,						\
	a_bMult,							\
	/* V3: bInterfaceNumber */ 0xFF					\
	)

#define	__TMS_USBPUMP_HCD_PIPE_SETUP_V1(				\
	a_pPipe,							\
	a_pHcdDevice,							\
	a_MaxTransferSize,						\
	a_wInterval,							\
	a_wMaxPacketSize,						\
	a_bEndpointAddress,						\
	a_bEndpointType,						\
	a_fToggle,							\
	a_bExtraPackets							\
	)								\
	__TMS_USBPUMP_HCD_PIPE_SETUP_V2(				\
	a_pPipe,							\
	a_pHcdDevice,							\
	a_MaxTransferSize,						\
	a_wInterval,							\
	a_wMaxPacketSize,						\
	a_bEndpointAddress,						\
	a_bEndpointType,						\
	a_fToggle,							\
	a_bExtraPackets,						\
	/* V2: bMaxBurstSize */ 1,					\
	/* V2: wMaxStreamID */ 0,					\
	/* V2: wBytesPerInterval */ 0,					\
	/* V2: bMult */ 0						\
	)		
	
/****************************************************************************\
|
|	Reset a pipe.
|
\****************************************************************************/

#define	__TMS_USBPUMP_HCD_PIPE_RESET(pPipe)				\
    do	{								\
	(pPipe)->fToggle = __TMS_FALSE;					\
	(pPipe)->fHalted = __TMS_FALSE;					\
	} while (0)

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_HCD_PIPE_TAG	\
   __TMS_USBPUMP_HCD_PIPE_TAG
# define USBPUMP_HCD_PIPE_SETUP_V3(				\
	a_pPipe,							\
	a_pHcdDevice,							\
	a_MaxTransferSize,						\
	a_wInterval,							\
	a_wMaxPacketSize,						\
	a_bEndpointAddress,						\
	a_bEndpointType,						\
	a_fToggle,							\
	a_bExtraPackets,						\
	a_bMaxBurstSize,						\
	a_wMaxStreamID,							\
	a_wBytesPerInterval,						\
	a_bMult,							\
	a_bInterfaceNumber						\
	)	\
	__TMS_USBPUMP_HCD_PIPE_SETUP_V3(				\
	a_pPipe,							\
	a_pHcdDevice,							\
	a_MaxTransferSize,						\
	a_wInterval,							\
	a_wMaxPacketSize,						\
	a_bEndpointAddress,						\
	a_bEndpointType,						\
	a_fToggle,							\
	a_bExtraPackets,						\
	a_bMaxBurstSize,						\
	a_wMaxStreamID,							\
	a_wBytesPerInterval,						\
	a_bMult,							\
	a_bInterfaceNumber						\
	)
# define USBPUMP_HCD_PIPE_SETUP_V2(				\
	a_pPipe,							\
	a_pHcdDevice,							\
	a_MaxTransferSize,						\
	a_wInterval,							\
	a_wMaxPacketSize,						\
	a_bEndpointAddress,						\
	a_bEndpointType,						\
	a_fToggle,							\
	a_bExtraPackets,						\
	a_bMaxBurstSize,						\
	a_wMaxStreamID,							\
	a_wBytesPerInterval,						\
	a_bMult								\
	)	\
	__TMS_USBPUMP_HCD_PIPE_SETUP_V2(				\
	a_pPipe,							\
	a_pHcdDevice,							\
	a_MaxTransferSize,						\
	a_wInterval,							\
	a_wMaxPacketSize,						\
	a_bEndpointAddress,						\
	a_bEndpointType,						\
	a_fToggle,							\
	a_bExtraPackets,						\
	a_bMaxBurstSize,						\
	a_wMaxStreamID,							\
	a_wBytesPerInterval,						\
	a_bMult								\
	)
# define USBPUMP_HCD_PIPE_SETUP_V1(				\
	a_pPipe,							\
	a_pHcdDevice,							\
	a_MaxTransferSize,						\
	a_wInterval,							\
	a_wMaxPacketSize,						\
	a_bEndpointAddress,						\
	a_bEndpointType,						\
	a_fToggle,							\
	a_bExtraPackets							\
	)	\
	__TMS_USBPUMP_HCD_PIPE_SETUP_V1(				\
	a_pPipe,							\
	a_pHcdDevice,							\
	a_MaxTransferSize,						\
	a_wInterval,							\
	a_wMaxPacketSize,						\
	a_bEndpointAddress,						\
	a_bEndpointType,						\
	a_fToggle,							\
	a_bExtraPackets							\
	)
# define USBPUMP_HCD_PIPE_RESET(pPipe)	\
   __TMS_USBPUMP_HCD_PIPE_RESET(pPipe)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_hcd_pipe.h ****/
#endif /* _USBPUMP_HCD_PIPE_H_ */
