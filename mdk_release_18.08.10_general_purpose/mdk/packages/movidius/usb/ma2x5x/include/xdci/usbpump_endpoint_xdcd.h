/* usbpump_endpoint_xdcd.h	Mon Jan 08 2018 11:45:52 chwon */

/*

Module:  usbpump_endpoint_xdcd.h

Function:
	Definition of USBPUMP_ENDPOINT_XDCD

Version:
	V3.21e	Mon Jan 08 2018 11:45:52 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2014, 2016-2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2014

Revision history:
   3.13a  Thu Jul 10 2014 16:48:18  tmm
	18309: Module created.

   3.17a  Wed Feb 10 2016 10:49:26  chwon
	19642: 19722: Remove not used variables in the endpoint context. Add
	bmAttribute, iEndTRB and StartFrame in the context.

   3.21c  Mon Aug 14 2017 11:45:26  chwon
	20643: Add uepxdcd_IntervalAdd in the USBPUMP_ENDPOINT_XDCD. 

   3.21c  Mon Sep 11 2017 16:51:28  chwon
	20633: Add uepxdcd_fWaitingXferNotReady in the USBPUMP_ENDPOINT_XDCD.

   3.21e  Mon Jan 08 2018 11:45:52  chwon
	20704: Add uepxdcd_IsoInState and flags in the USBPUMP_ENDPOINT_XDCD.

*/

#ifndef _USBPUMP_ENDPOINT_XDCD_H_		/* prevent multiple includes */
#define _USBPUMP_ENDPOINT_XDCD_H_

#ifndef _XDCDKERN_TYPES_H_
# include "xdcdkern_types.h"
#endif

#ifndef _UENDPOINT_H_
# include "uendpoint.h"
#endif

/****************************************************************************\
|
|	USBPUMP_ENDPOINT_XDCD is the structure that is used by the USB
|	DataPump to move data.  It contains the additional information that's
|	required to allow the data to get properly queued. There is one 
|	endpoint for each hardware channel; not all endpoints are used in a 
|	given application.
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_ENDPOINT_XDCD)
	{
	__TMS_UENDPOINT_HDR;			/* standard header */

	__TMS_PUBUFQE	uepxdcd_pActiveQe;	/* current active UBUFQE list */
	__TMS_PUBUFQE	uepxdcd_pMappingQe;	/* current mapping UBUFQE list */
	__TMS_PUBUFQE	uepxdcd_pMappedQe;	/* current mapped UBUFQE list */

	__TMS_UCHAR	uepxdcd_Properties;	/* is endpoint RX or TX? */
	__TMS_UCHAR	uepxdcd_EpId;		/* endpoint index */
	__TMS_UCHAR	uepxdcd_EpNo;		/* endpoint address for given pipe */
	__TMS_UCHAR	uepxdcd_bmAttribute;

	__TMS_UINT32	uepxdcd_EpFlags; 	/* working flags */

	__TMS_USBPUMP_XDCD_EVENT
			uepxdcd_Event;		/* endpoint event */
	__TMS_UINT32	uepxdcd_EpState;	/* endpoint state returned from
						   Get Endpoint State command */

	__TMS_UINT16	uepxdcd_nMaxPacket;	/* endpoint max packet size */

	/* last buffer descriptor that the core is updating */
	__TMS_UINT16	uepxdcd_lastTrbIndex;

	/* the index of the paused TRB */
	__TMS_UINT16	uepxdcd_pausedTrbIndex;

	__TMS_UINT16	uepxdcd_nBuffer;	/* endpoint local buffer size */
	__TMS_BYTES	uepxdcd_nPacket;	/* endpoint packet size */
	__TMS_PUINT8	uepxdcd_pBuffer;	/* endpoint local buffer pointer */
	__TMS_PUINT8	uepxdcd_pPacket;	/* endpoint packet pointer */

	__TMS_USBPUMP_XDCD_EVENT
			uepxdcd_EpEvent; 	/* Endpoint Interrupt Reason */

	__TMS_UINT32	uepxdcd_EpCmdReg;
	__TMS_UINT32	uepxdcd_EpCmdParm0Reg;
	__TMS_UINT32	uepxdcd_EpCmdParm1Reg;
	__TMS_UINT32	uepxdcd_EpCmdParm2Reg;

	__TMS_UINT32	uepxdcd_RscIdx;

	__TMS_USBPUMP_DEVICE_XDCD *
			uepxdcd_pDcd;

	/* Timer object */
	__TMS_USBPUMP_TIMER
			uepxdcd_Timer;

	__TMS_UINT	uepxdcd_fTimerRun: 1;
	__TMS_UINT	uepxdcd_fWaitingForCallback: 1;
	__TMS_UINT	uepxdcd_fMapContinue: 1;
	__TMS_UINT	uepxdcd_fPaused: 1;
	__TMS_UINT	uepxdcd_fConfigured: 1;
	/* Is it stopping DMA and hasn't completion interrupt been fired yet? */
	__TMS_UINT	uepxdcd_fDmaStopping: 1;
	__TMS_UINT	uepxdcd_fEnaOnce: 1;
	__TMS_UINT	uepxdcd_fReceivedXferNotReady: 1;
	__TMS_UINT	uepxdcd_fWaitingXferNotReady: 1;

	/*
	|| Issue endpoint event on end transfer command complete event if this
	|| flag is up
	*/
	__TMS_UINT	uepxdcd_fEventAfterEndXfer: 1;

	__TMS_UINT	uepxdcd_IsoInState: 2;
	__TMS_UINT	uepxdcd_IsoInStartXferOk0: 1;
	__TMS_UINT	uepxdcd_IsoInStartXferOk1: 1;

	__TMS_UINT	uepxdcd_nXferRsc;
	__TMS_UINT	uepxdcd_HiberDescIdx;

	/* Total request size in Scatter/Gather DMA */
	__TMS_UINT32	uepxdcd_RequestSize;

	/* Actual transfer size in Scatter/Gather DMA */
	__TMS_UINT32	uepxdcd_ActualSize;

	/* transfer size in local buffer */
	__TMS_UINT32	uepxdcd_nReceivedInLocalBuf;

	/*
	|| DCD transfer descriptor has one more field to save UBUFQE for
	|| each DDMA buffer descriptor when DCD stats DMA with multiple
	|| UBUFQEs.
	*/
	__TMS_USBPUMP_XDCD_TRANSFER_DESCRIPTOR *
			uepxdcd_pTD;

	/* Segment array to map buffer in Scatter/Gather DMA mode */
	__TMS_USBPUMP_UHILAUX_SEGMENT *
			uepxdcd_pSegArray;
	__TMS_UINT32	uepxdcd_MaxNumSegments;
	__TMS_UINT32	uepxdcd_nMappedSegments;

	/* Local bounce buffer */
	__TMS_USBPUMP_BUFFER_HANDLE
			uepxdcd_hBuffer;
	__TMS_PHYSADDR	uepxdcd_BufferPhysAddr;

	/* Isochronous buffer */
	__TMS_UINT16	uepxdcd_Interval;
	__TMS_UINT16	uepxdcd_iCurrTRB;
	__TMS_UINT16	uepxdcd_iAvailTRB;
	__TMS_UINT16	uepxdcd_nAvailTRB;
	__TMS_UINT16	uepxdcd_iEndTRB;
	__TMS_UINT16	uepxdcd_iCurrDesc;
	__TMS_UINT16	uepxdcd_iDescToBeProcessed;
	__TMS_UINT16	uepxdcd_iSegToBeProcessed;
	__TMS_UINT16	uepxdcd_nExtraPkts;
	__TMS_UINT16	uepxdcd_IntervalAdd;
	__TMS_UINT32	uepxdcd_StartFrame;

	/* Maximum transfer size per one TRB */
	__TMS_UINT32	uepxdcd_MaxXferSizePerTRB;

	/* Maximum number of TRBs */
	__TMS_UINT16	uepxdcd_MaxNumTRBs;

	/* number of all penbufqes can be transfered in one DMA transfer */
	__TMS_UINT16	uepxdcd_NumBufqe;

	/* save Endpoint configuration parameters for hibernation */
	__TMS_UINT32	uepxdcd_depcfgparam0;
	__TMS_UINT32	uepxdcd_depcfgparam1;
	};

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_endpoint_xdcd.h ****/
#endif /* _USBPUMP_ENDPOINT_XDCD_H_ */
