/* usbpump_xhcd.h	Thu Mar 30 2017 10:36:08 chwon */

/*

Module:  usbpump_xhcd.h

Function:
	Definition of XHCD structure, constants and functions.

Version:
	V3.19b	Thu Mar 30 2017 10:36:08 chwon	Edit level 71

Copyright notice:
	This file copyright (C) 2009-2012, 2014-2015, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Xiaowen Lu, MCCI Corporation	January 2009

Revision history:
   2.01a  Mon Jan 12 2009 10:27:07 luxw
	7678: Module created.

   2.01a  Sat May  9 2009 21:07:54  tmm
	7667:  adapt to new interrupt connection system for uni/multi
	processors.

   2.01a  Tue May 12 2009 09:23:16  luxw
	7667: Added message and line isr related structure in HCD-XHCI.

   2.01a  Tue May 12 2009 20:09:37  luxw
	7678: Added xHCI version field in HCD structure.

   2.01a  Wed May 13 2009 10:38:37  luxw
	Added debug flag in XHCD configuration.

   2.01a  Thu May 14 2009 01:34:58  luxw
	Added fSet_PORTSC_CSC because there is mass storage device internally
	connects to host in Fresco board.

   2.01a  Wed May 20 2009 20:51:28  luxw
	Added pLinkTRB in command ring and transfer ring.
	Added EpID and Slot ID in transfer ring.

   2.01a  Sun May 24 2009 11:40:11  luxw
	Remove xhcd_ImanIpSafeMask and xhcd_ImanIpClearMask because they
	are typo actually.

   2.01a  Wed Jun 03 2009 13:41:30  luxw
	7669: Change type of PageSize from SIZE_T to BYTES.

   2.01a  Wed Jun 03 2009 14:11:17  luxw
	8160: Added input context in device context structure.
	Added MaxEpID in input context.

   2.01a  Thu Jul 02 2009 10:30:03  luxw
	8306: Added transfer ring state field.
	Added UsbPumpXhcd_CancelActiveRequest() and
	UsbPumpXhcd_CancelPendingRequest() declaration.

   2.01a  Thu Jul 16 2009 14:58:08  luxw
	Added pSsHcd in USBPUMP_HCD_XHCI_CONTENTS structure.

   2.01a  Thu Jul 23 2009 19:17:42  luxw
	Added fHcdStarted and fHwInitialized flags to support
	all HCD stop/deinit.

   2.01a  Fri Aug 07 2009 11:51:56  luxw
	8664: Added IMOD in the XHCD configuration
	8666: Added timer in USBPUMP_HCD_XHCI_HDR.

   2.01a  Wed Aug 26 2009 11:49:19  luxw
	8788: Added RecoverSlotID in the XHCD head strucrure.

   2.01a  Fri Sep 18 2009 23:33:32  luxw
	8892: Added pSegment in the transfer ring structure.

   2.01a  Tue Nov 24 2009 19:30:39  luxw
	9272: Define Stream context array structure.

   2.01a  Wed Dec 02 2009 13:46:13  luxw
	9191: Single HCD object in XHCD.
	8435: XHCI HCD creates object name from local constant string.

   3.01a  Wed Dec 30 2009 19:33:50  luxw
	9660: Define USBPUMP_XHCD_ISO_PKT_DESC structure and put it
	in transfer ring structure.

   3.01a  Sat Feb 06 2010 00:10:24  luxw
	9660: Updated buffer segment and packet descriptor structure.

   3.01b  Thu Feb 18 2010 16:57:50  luxw
	9828: Add stream information and other capability info in HCD structure.

   3.01b  Thu Feb 18 2010 17:24:34  luxw
	9902: Added fSkip and pPrevTRB in sw-defind Isoch packet descriptor.

   3.01c  Wed Mar 17 2010 19:37:31  chwon
	10020: Add xHC extended capability information in the HCD structure.

   3.01c  Wed Mar 24 2010 00:11:27  luxw
	Added Async Ioctl fields in HCD structure.

   3.01c  Wed Apr 14 2010 22:30:59  luxw
	10107: Added pending HcdReq queue in the transfer ring.

   3.01c  Wed May 12 2010 17:17:05  chwon
	10324: Add USBPUMP_XHCD_COMMAND_RING::fRunning flag.
	Add fNoHardware, pStartHcdIoctlQe and HC register variable in the
	USBPUMP_HCD_XHCI_CONTENTS.

   3.01c  Wed May 19 2010 14:53:42  luxw
	Updated the USBPUMP_XHCD_ISO_PKT_DESC structure since Hcd doesn't
	call MapPio for packet descriptor anymore.

   3.01c  Thu Jun 03 2010 14:59:54  luxw
	Added ResidualSeg field in USBPUMP_XHCD_BUFFER_SEGMENT structure.

   3.01c  Wed Jun 30 2010 23:10:00  luxw
	10107: Added wiring flag in xhcd configuration structure.

   3.01c  Mon Jul 19 2010 13:47:44  luxw
	10107: Added wiring flag for TI hub workaround.

   3.01c  Wed Jul 21 2010 22:51:19  luxw
	8779: Added scratchpad buffer structure.

   3.01c  Thu Aug 12 2010 11:12:03  luxw
	10886: Added the following fields in the Xhcd structure:
	pSuspendIoctlQe, pResumeIoctlQe and all operational resgisters
	and runtime resgisters.

   3.01c  Thu Aug 26 2010 18:10:46  luxw
	10886: Added pPortCapIoctlQe in HCD structure.

   3.01c  Thu Sep 02 2010 17:15:28  luxw
	10886: Added fRestoreFailed in xhcd structure.

   3.01d  Sun Sep 26 2010 23:52:14  luxw
	11421: Added USBPUMP_XHCD_SET_ALT_PORT wiring flag.

   3.01d  Tue Sep 28 2010 10:02:50  luxw
	11421: Change wiring flag name.

   3.01d  Thu Oct 07 2010 09:33:50  chwon
	11183: added fHcdStarting and pStopHcdIoctlQe in the XHCD.

   3.01d  Fri Oct 08 2010 01:06:17  luxw
	11525: Added pIsoFreeSeg, pIsoMappingSeg, pIsoMappedSeg in
	transfering structure for supporting multiple HcdRequests.

   3.01d  Tue Oct 12 2010 13:20:26  luxw
	11569: Add TxThreshold for TI xHc in configuration structure.
	11455: Add system resume recovery time in configuration structure.

   3.01d  Wed Nov 03 2010 15:50:08  chwon
	11780: Add TimerRetryCount and remove fRunTimer in the XHCD.

   3.01d  Tue Nov 09 2010 14:58:44  cccho
	11221, 11520: Add addrUsbLegSupCap, fValidUsbLegSupCap to
	support USB Legacy Support Capability access.

   3.01d  Wed Nov 10 2010 14:29:05  luxw
	11775: Added event ring segment index.
	Added mapping HcdRequests queue in the transfer ring.
	Added the number of buffer segment in the Hcd configuration.

   3.01d  Thu Nov 11 2010 11:12:27  luxw
	11775: Set default buffer segment to 1.

   3.01d  Fri Nov 12 2010 16:41:10  luxw
	11852: Added fCommandRingAbortTimer in Hcd structure.

   3.01d  Mon Nov 29 2010 13:51:31  chwon
	11213: add RootPortSlotID in the HCD structure.
	11825: change ulSystemResumeRecoveryTime default value from 200 to 10.

   3.01d  Thu Dec 09 2010 17:02:18  luxw
	Added transfer ring post proc function in the transfer ring structure.

   3.01d  Tue Jan 04 2011 10:32:33  luxw
	12205, 12143: Added intervalInMicroFrame in transfer ring.
	Added uIST in Hcd contents.

   3.01d  Tue Jan 18 2011 14:49:33  chwon
	12405: add Usb2PortNumber, Usb2PortNumber and pvRootHubPorts in the
	XHCD contents.

   3.01d  Mon Jan 24 2011 17:59:49  luxw
	12371: Added USBPUMP_XHCD_WIRING_DISABLE_USB2_SUSPEND.

   3.01d  Tue Jan 25 2011 12:42:13  luxw
	11357: Add ulMapDmaRangeTime and ulMapDmaRange in Xhcd configuration
	structure.

   3.01d  Fri Jan 28 2011 00:40:05  luxw
	11575: Added USBPUMP_XHCD_WIRING_SET_ALT_PORT_WAKE_MASK flag.

   3.01d  Mon Jan 31 2011 18:07:22  luxw
	12321: Added fSetupTrbCycle and pSetupTRB in the transfer ring.

   3.01d  Fri Apr 01 2011 10:12:28  luxw
	12702: Added fSavexHcFailed and fHcPowerDown in Hcd structure.
	13065: Added MaxStreamID in the Xhcd configuration structure.

   3.01e  Fri Apr 08 2011 17:18:06  luxw
	13142: Added USBPUMP_XHCD_WIRING_SET_P3 in Xhcd wiring flags.

   3.01e  Mon Apr 11 2011 18:21:35  luxw
	13177: Added HwResetTime in the xHcd configuration structure.
	13168: Add USB2/USB3 port map information in xhcd configuration
	informtation. Also add a new wiring to support non-default xHC port
	mapping.

   3.01f  Mon May 02 2011 14:26:15  luxw
	13331: Define pMessageEvents in XHCD structure.

   3.01f  Fri May 06 2011 16:53:33  luxw
	13237: Add CommandAbort time and USBD cleanup time in XHCD configuration.

   3.01f  Fri May 27 2011 11:27:30  chwon
	13490: Remove RootPortSlotID and add pFunctionWakeRequest in the XHCD.

   3.01f  Fri Jul 08 2011 14:17:52  chwon
	13716: Add wMaxPacketSize and remove pPipe in the transfer ring.

   3.01f  Thu Jul 28 2011 17:05:57  luxw
	13716: Added pipe the transfer ring which only saves default pipe.

   3.01f  Wed Aug 03 2011 13:58:14  luxw
	13878: Added CmdRingTimer in xHcd structure.

   3.01f  Tue Oct 18 2011 17:14:45  luxw
	14305: Added USBPUMP_XHCD_WIRING_PORT_OFF_WHEN_SHUT_DOWN wiring flag.
	13565: Added USBPUMP_XHCD_WIRING_DISABLE_WAKE_MASK.
	13542: Added USBPUMP_XHCD_WIRING_LIMIT_BINTERNAL.
	13866: Added USBPUMP_XHCD_WIRING_DISABLE_SAVE_RESTORE.

   3.01f  Wed Nov 09 2011 14:13:34  luxw
	14391: Added IntSts in Xhcd structure.

   3.01f  Wed Nov 09 2011 15:00:49  luxw
	12868: Added FrameNum in Xhcd structure.

   3.01f  Thu Mar 29 2012 12:36:32  luxw
	15046: 15628: Added USBPUMP_XHCD_WIRING_REPORT_COMPATIBLE_PORT.

   3.01f  Fri Jul 06 2012 10:38:34  luxw
	15607: Added ulUsbdFlagMask and ulUsbdFlagBits in HCD structure.

   3.01f  Tue Nov 29 2011 20:18:54  luxw
	10915: Support xHci V1.0.
	13450: Added fIgnoreEvent and PostProc in the Transfer Ring structure.
	13182: Added Usb3PSIC and Usb2PSIC in Hcd structure.

   3.11a  Fri Aug 24 2012 12:05:17  luxw
	15860: Change fSkip to fPktLoss in USBPUMP_XHCD_ISO_PKT_DESC.

   3.11a  Tue Sep 04 2012 14:43:45  luxw
	15858: Added bMaxBurstSize in the structure of Transfer Ring.

   3.11b  Fri Oct 12 2012 17:39:19  luxw
	15972: Added pGlobalBufSeg in XHCD structure and nGlobalBufSeg
	in XHCD configuration.

   3.11b  Mon Oct 29 2012 15:38:16  luxw
	16108: Add USBPUMP_XHCD_EVENT_TRB.

   3.11c  Wed Jul 17 2013 16:17:53  luxw
	Merge XHCD wiring flags from Andover branch.

   3.13b  Fri Aug 15 2014 14:28:56  chwon
	18461: Remove hUhilAux from USBPUMP_HCD_XHCI
	and use USBPUMP_HCDKIT_HCD::hUhilAux instead.

   3.13b  Wed Jan 14 2015 16:28:33  luxw
	18571: add LastFrameNumberReturned in the structure of xHCD.
	add HcdKitSwitch in USBPUMP_XHCD_CONFIG_INFO and V10 for
	Init/Setup for XHCD configuration.

   3.19b  Thu Mar 30 2017 10:36:09  chwon
	20505: Add fNeedDelayAfterReset flag in the context.

*/

#ifndef _USBPUMP_XHCD_H_	/* prevent multiple includes */
#define _USBPUMP_XHCD_H_

#ifndef _BASEHIL_H_
# include "basehil.h"
#endif

#ifndef _USBPUMP_HCDKIT_H_
# include "usbpump_hcdkit.h"
#endif

#ifndef _USBPUMP_HCDKIT_HCD_H_
# include "usbpump_hcdkit_hcd.h"
#endif

#ifndef _USBPUMP_HCDKIT_SWITCH_H_
# include "usbpump_hcdkit_switch.h"
#endif

#ifndef _USBPUMP_HCD_REQUEST_H_
# include "usbpump_hcd_request.h"
#endif

#ifndef _UPLATFORMAPI_H_
# include "uplatformapi.h"
#endif

#ifndef _USBPUMP_XHCD_TYPES_H_
# include "usbpump_xhcd_types.h"
#endif

#ifndef _USBPUMP_UHILAUX_H_
# include "usbpump_uhilaux.h"
#endif

/****************************************************************************\
|
|	Forward type references
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION(USBPUMP_HCD_XHCI);
__TMS_TYPE_DEF_STRUCT(USBPUMP_HCD_XHCI_CONTENTS);
__TMS_TYPE_DEF_STRUCT(USBPUMP_HCD_XHCI_HDR);
__TMS_TYPE_DEF_STRUCT(USBPUMP_XHCD_EVENT_TRB);

/****************************************************************************\
|
|	Definition of XHCD_INPUT_CONTEXT
|
\****************************************************************************/

struct __TMS_STRUCTNAME (USBPUMP_XHCD_INPUT_CONTEXT)
	{
	/* 32 bytes or 64 bytes */
	__TMS_UINT			CSZ;

	__TMS_UINT			MaxEpID;

	/* Size of input context */
	__TMS_BYTES			InputSize;

	/* Buffer handle */
	__TMS_USBPUMP_BUFFER_HANDLE	hInput;

	/* Physical address of Input context */
	__TMS_PHYSADDR			InputPhyAddr;

	/* Logical address of input context */
	__TMS_VOID *			pInputContext;
	};
/*
|| Slot or Endpoint Contexts are found is referred to as an Input Slot
|| or Endpoint Context. SW can write information and xHC read it.
*/
#define __TMS_USBPUMP_XHCD_GET_INPUT_CONTROL_CONTEXT(pInput)	\
	((__TMS_USBPUMP_XHCI_INPUT_CONTROL_CONTEXT *)((pInput)->pInputContext))

#define __TMS_USBPUMP_XHCD_GET_SLOT_CONTEXT_FROM_INPUT(pInput)	\
	((__TMS_USBPUMP_XHCI_SLOT_CONTEXT *)((__TMS_UINT8 *)	\
			((pInput)->pInputContext) + (pInput)->CSZ))

/*
|| EpID = Device Context Index(DCI), it is from 1 to 31.
|| See UsbPumpXhcdI_GetEpId_Type()
*/
#define __TMS_USBPUMP_XHCD_GET_ENDPOINT_CONTEXT_FROM_INPUT(pInput, EpID)\
	((__TMS_USBPUMP_XHCI_ENDPOINT_CONTEXT *)((__TMS_UINT8 *)	\
		((pInput)->pInputContext) + (pInput)->CSZ*((EpID) + 1)))

/****************************************************************************\
|
|	Definition of XHCD_DEVICE_CONTEXT
|
\****************************************************************************/

struct __TMS_STRUCTNAME (USBPUMP_XHCD_DEVICE_CONTEXT)
	{
	/* 32 bytes or 64 bytes */
	__TMS_UINT			CSZ;

	/*
	|| Doorbell register for a device. The Doorbell Array is organized as
	|| an array of up to 256 Doorbell Registers. One 32-bit Doorbell
	|| Register is defined in the array for each Device Slot.
	*/
	__TMS_UINT32			DeviceDoorbellReg;

	__TMS_USBPUMP_XHCD_INPUT_CONTEXT DevInputCtx;

	/* Buffer handle */
	__TMS_USBPUMP_BUFFER_HANDLE	hDevCtx;

	/* Physical address of Device context */
	__TMS_PHYSADDR			DevCtxPhyAddr;

	/* Logical address of Device context. It is read only for HCD */
	__TMS_VOID *			pXhciDeviceContext;
	};

/*
|| Slot or Endpoint Contexts are found in Device is referred to as an
|| Output Slot or Endpoint Context. xHC update the contents and SW can
|| only read it.
*/
#define __TMS_USBPUMP_XHCD_GET_SLOT_CONTEXT(pDevCtx)	\
	((__TMS_USBPUMP_XHCI_SLOT_CONTEXT *)((pDevCtx)->pXhciDeviceContext))

/*
|| EpID = Device Context Index(DCI), it is from 1 to 31.
|| See UsbPumpXhcdI_GetEpId_Type()
*/
#define __TMS_USBPUMP_XHCD_GET_ENDPOINT_CONTEXT(pDevCtx, EpID)	\
	((__TMS_USBPUMP_XHCI_ENDPOINT_CONTEXT *)		\
	 (((__TMS_UINT8*)((pDevCtx)->pXhciDeviceContext)) + ((pDevCtx)->CSZ * (EpID))))

/****************************************************************************\
|
|	Definition of XHCD Device Context Base Address Array
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XHCD_DCBAA)
	{
	/* Number of device base address */
	__TMS_UINT			  NumDCBAAEntry;

	/* Buffer handle */
	__TMS_USBPUMP_BUFFER_HANDLE	  hDCBAA;

	/* Physical address */
	__TMS_PHYSADDR			  DcbaaPhyAddr;

	/* Logical address */
	__TMS_USBPUMP_XHCI_DCBAA_ENTRY *  pDCBAA;
	};

/****************************************************************************\
|
|	Definition of Stream Context
|
\****************************************************************************/

/*

Type:	USBPUMP_XHCD_STREAM_CONTEXT_ARRAY

Function:
	The structure represents xHCD Stream Context(Array).

Description:
	The stream context array will be coreesponding to one pipe
	which supports stream. The 0th stream context is not used
	for transfer data. Other stream context have one transfer
	ring each. The physical address of stream context array is
	saved in the endpoint context. HW will find it during configuration
	endpoint command.

Contents:
	USBPUMP_XHCD_STREAM_ID NumOfStreamCtx;
		The number of stream context in the stream context array.

	USBPUMP_XHCD_STREAM_IDT nActualSupportedStreamID;
		The number of stream ID host and device can use.

	USBPUMP_HCD_PIPE * pPipe;
		The Hcd pipe for this stream context.

	USBPUMP_XHCD_TRANSFER_RING **	ppStreamTransferRings;
		The transfer rings in this pipe.

	USBPUMP_BUFFER_HANDLE	hStreamCtxArray;
		The buffer handle of stream context array.

	PHYSADDR StreamCtxArrayPhyAddr;
		The physical address of stream context array.

	USBPUMP_XHCI_STREAM_CONTEXT *	pStreamCtxArray;
		The logical address of stream context array.

See Also:
	UsbPumpXhcdI_CreateStreamCtxArray()

*/

struct __TMS_STRUCTNAME(USBPUMP_XHCD_STREAM_CONTEXT_ARRAY)
	{
	__TMS_USBPUMP_XHCD_STREAM_ID		NumOfStreamCtx;

	__TMS_USBPUMP_XHCD_STREAM_ID		nActualStreamID;

	__TMS_USBPUMP_HCD_PIPE *		pPipe;

	__TMS_USBPUMP_XHCD_TRANSFER_RING **	pvStreamTransferRings;

	/* Buffer handle */
	__TMS_USBPUMP_BUFFER_HANDLE	  	hStreamCtxArray;

	/* Physical address */
	__TMS_PHYSADDR			  	StreamCtxArrayPhyAddr;

	/* Logical address */
	__TMS_USBPUMP_XHCI_STREAM_CONTEXT *	pStreamCtx;
	};

/****************************************************************************\
|
|	Definition of segment buffer structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XHCD_MAPPED_BUFFER_RANGE)
	{
	/*
	|| the sum of mapped segment data size which is going to be written into
	|| TRBs. That is, the total transfer size in the Transfer Descriptor(TD).
	|| This value should be equal to GetTransferSize if transfer is successful.
	|| Due to lack of TRBs, HCD may ask mapper for additional segment in
	|| the transfer event. and build next TD for the same buffer(HcdRequest).
	|| the valude of this filed must be clear before calling GetMore...Fn or
	|| MapDma...Fn()
	*/
	__TMS_BYTES				TransferSizeInTD;

	/*
	|| Record the size in byte for one time mapping. HCD will use it
	|| to estimate the average size of one segment to decide whether
	|| need to use multiple TRBs.
	*/
	__TMS_BYTES				MappingRangeSize;

	/*
	|| The field is set in two cases:
	|| 1. Callback of MapDmaAsync, set with acutal mapped segment cout.
	|| 2. Due to lack of TRBs, HCD can't fill totalMappedSegmentCountInRange
	|| at one time for a piece of range. HCD needs to call GetMore...Fn() to
	|| retrieve the rest in the transfer event. This field is passed by
	|| address and assigned by mapper then HCD knows how many segments are
	|| writted in the array.
	|| The value of this field is less than or equal to pSegment->SegmentCount.
	|| HCD also uses this field to calculate the size of data actually
	|| received during "ShortPacket" happens.
	|| the value of this filed must be clear before calling GetMore...Fn or
	|| MapDma...Fn()
	*/
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT	SegmentCountInTD;

	/*
	|| Due to lack of TRBs, HCD can't fill totalMappedSegmentCountInRange
	|| at one time for a piece of range. HCD has to build several TDs to
	|| transfer this range one by one in the transfer event. Since HCD calls
	|| GEtMore...Fn() to retrieve the additional segments from the mapper,
	|| this fields is used as an index to tell the mapper where it starts
	|| from last time.
	|| the value of this fields is increased by TransferSizeInTD and must
	|| be clear before calling MapDma...Fn().
	*/
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT	SegmentCountIndexInRange;

	/*
	|| Total number of segment count for a mapped buffer. It may NOT be
	|| equal to the total number of segment for the WHOLE buffer.
	*/
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT	totalMappedSegmentCountInRange;

	/*
	|| Indicate which segment is used by ISO currently. This field should
	|| be less than or equal to SegmentCountIndexInRange because TRBs may
	|| be run out. For Isoch transfer, a segment WILL NOT correspond to
	|| one TRB.
	*/
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT	SegmentCountIndexUsedByIso;
	};

struct __TMS_STRUCTNAME(USBPUMP_XHCD_BUFFER_SEGMENT)
	{
	/*
	|| The number of tried mapping process due to failure.
	|| The counter is increase in UsbPumpXhcdI_MapDmaBufferWithRange()
	|| and it is cleared when map is successful.
	*/
	__TMS_UINT8					MapDmaCounter;

	__TMS_BOOL					fMapEntireBuf;

	__TMS_USBPUMP_XHCD_BUFFER_SEGMENT *		pNext;
	__TMS_USBPUMP_XHCD_BUFFER_SEGMENT *		pPrev;

	/* sw defined Isoch packet descriptor */
	__TMS_USBPUMP_XHCD_ISO_PKT_DESC *		pIsochDesc;

	/*
	|| This field is initialized as return value of pGetSegmentCountFn()
	|| It decrease when segment data is writtent into TRBs.
	*/
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT		RemainingSegmentCount;

	/* Information of mapped buffer in a range */
	__TMS_USBPUMP_XHCD_MAPPED_BUFFER_RANGE 		MapBufRange;

	/*
	|| The maximum transfer size that the segment array can carry.
	|| In worst case, it is SegmentCount * pagesize.
	*/
	__TMS_BYTES					MaxTransferSize;

	/*
	|| Only used for Isoch transfer: indicate where to start set range
	|| because HCD cannot use nActual to set the start offset of the
	|| buffer. offsetRange is the sum of previous MappingRangeSize.
	*/
	__TMS_BYTES					offsetRange;

	/* Residual segment */
	__TMS_USBPUMP_UHILAUX_SEGMENT			ResidualSeg;

	/*
	|| Only used for Isoch transfer. The size of data in all previouly-
	|| processed segments. HCD use this information and uOffset in
	|| packet descriptor to find the start address of next Isoch packet.
	*/
	__TMS_BYTES					BufSizeInConsumedSegment;

	/* Number of USBPUMP_UHILAUX_SEGMENT allocated*/
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT 		SegmentCount;

	/* The actual array to get scatter/gather list element */
	__TMS_USBPUMP_UHILAUX_SEGMENT *			pSeg;
	};

#define __TMS_USBPUMP_XHCD_BUFFER_SEGMENT_REQUIRED_SIZE(pO, n)		\
	(sizeof(*(pO)) + (n) * sizeof(*(pO)->pSeg))

#define __TMS_USBPUMP_XHCD_BUFFER_SEGMENT_SIZEOF(pO)			\
	__TMS_USBPUMP_XHCD_BUFFER_SEGMENT_REQUIRED_SIZE(pO, (pO)->SegmentCount)

/****************************************************************************\
|
|	Definition of XHCD defined Isoch packet descriptor
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XHCD_ISO_PKT_DESC)
	{
	/* Number of packets are mapped */
	__TMS_UINT32				totalPacket;
	/* Index of packet which is being processed */
	__TMS_UINT32				curPacketIndex;
	/* Number of packets in a transfer */
	__TMS_UINT32				NumOfPktsInOneTransfer;
	/* The number of packets received previously for one HcdRequest */
	__TMS_UINT32				prevNumPkts;
	/* Previous TRB before error happens */
	__TMS_USBPUMP_XHCI_TRB *		pPrevTRB;
	/* Indicate whether Hcd needs to skip Isoch packets */
	__TMS_BOOL				fPktLoss;
	};


/****************************************************************************\
|
|	Definition of Command Ring Structure
|
\****************************************************************************/

/*

Type:	USBPUMP_XHCD_COMMAND_RING

Function:
	The structure represents xHCD command ring.

Description:
	A Command Ring is for xHC and xHC has only one Command Ring. In this
	design, the size of Command Ring is one page (256 Command TRBs). Each
	HC command will use different type of Command TRB.

	The doorbell target for HC command will be XHCI_DOORBELL_DB_TARGET_HC.
	Command Completion event will be generated in primary interrupter.

Contents:
	UINT32 PCS;
		Producer Cycle State. Initialized to 1. Toggle when Enqueue Ptr
		points to Link TRB.

	USBPUMP_HCD_REQUEST *pPendingHcdReq;
		Point to the next HcdRequest which will be processed in the
		Command Ring. Because multiple HcdRequests can be inserted
		into the Command Ring at the same time. When previous command
		is finished during command completion event, HCD will check
		pPendingHcdReq.

	USBPUMP_HCD_REQUEST *pActiveHcdReq;
		Point to the HcdRequests which are inseted into command ring.

	USBPUMP_XHCI_TRB * pEnqueue;
		Enquque pointer address. Initialized with the beginning address
		of first TRB of Ring. HCD will increase it by adding sizeof(TRB).

	USBPUMP_XHCI_TRB * pDequeue;
		HCD updates it based on Command Completion Event. HCD uses it
		to check whether a ring is Full.

	PHYSADDR CmdRingPhyAddr;
		64-bit alignment address of Command Ring. HCD allocates it
		by calling USBPUMP_UHILAUX_ALLOCATE_COMMON_BUFFER_FN() at the
		initialization time.

	USBPUMP_BUFFER_HANDLE	hCmdRing;
		Buffer handle of Command Ring. HCD gets it from
		USBPUMP_UHILAUX_ALLOCATE_COMMON_BUFFER_FN().

	UINT32  SizeOfCmdRing;
		The number of TRBs in the command ring.

	USBPUMP_XHCI_TRB * pBaseCmdRing;
		Head(logical address) of command ring.

See Also:
	UsbPumpXhcdI_CreateCommandRing()
	UsbPumpXhcdI_InitCommandRing()
	UsbPumpXhcdI_FreeCommandRing().
	UsbPumpXhcdI_GetNumOfAvailableTrb()

*/

struct __TMS_STRUCTNAME(USBPUMP_XHCD_COMMAND_RING)
	{
	/* Producer Cycle State */
	__TMS_UINT			PCS: 1;
	__TMS_UINT			fRunning: 1;

	/* Waiting HcdRequests  */
	__TMS_USBPUMP_HCD_REQUEST *	pPendingHcdReq;

	/* Active HcdRequest */
	__TMS_USBPUMP_HCD_REQUEST *	pActiveHcdReq;

	/* Enqueue pointer of command ring */
	__TMS_USBPUMP_XHCI_TRB *	pEnqueue;

	/* Dequeue pointer of comand ring */
	__TMS_USBPUMP_XHCI_TRB *	pDequeue;

	__TMS_USBPUMP_XHCI_TRB *	pLinkTrb;

	__TMS_UINT			SizeOfCmdRing;

	/* Buffer handle */
	__TMS_USBPUMP_BUFFER_HANDLE	hCmdRing;

	/* Physical address of command ring */
	__TMS_PHYSADDR			CmdRingPhyAddr;

	/* Logical address of command ring */
	__TMS_USBPUMP_XHCI_TRB * 	pBaseCmdRing;
	};

/****************************************************************************\
|
|	Definition of Event Ring structure
|
\****************************************************************************/

/*

Type:	USBPUMP_XHCD_EVENT_RING

Function:
	The structure represents xHCD event ring.

Description:
	An Event Ring is for an interrupter and xHC can have multiple Event Rings.
	In this design, HCD implements only one Event Ring. All kinds of event,
	Transfer Event, Command Completion Event, PortStatus Event, HC Event
	are inserted into this Event Ring.

	The Event Ring has multiple Event Ring Segments. HCD writes the Event
	Ring segment table with every Event Ring Segment's address and Size.

		Event Ring Segment Table
	+--------------------------------------+
	| Ring Segment Base Address | Size     |   ERST(0)
	|---------------------------+----------|
	| Ring Segment Base Address | Size     |   ERST(1)
	|---------------------------+----------|
	| Ring Segment Base Address | Size     |   ERST(2)
	+--------------------------------------+

Contents:
	UINT32 CCS;
		Consumer Cycle State. Initialized to 1. Toggle when Dequeue Ptr
		reaches the end of Event Ring.

	USBPUMP_XHCI_TRB * pDequeue;
		HCD updates it based on Command Completion Event. HCD updates it
		by adding size of TRB. Dequeue Pointer is set to EventRingAddr
		when Event TRB reaches the end of Event Ring.

	PHYSADDR PhyDequeuePtr;
		16-bit alignment address of Event Ring. HCD allocates it
		by calling USBPUMP_UHILAUX_ALLOCATE_COMMON_BUFFER_FN() at the
		initialization time.

	USBPUMP_XHCD_EVENT_RING_SEGMENT * pERSHead;
		Head of link list of Event Ring segment.

	USBPUMP_XHCD_EVENT_RING_SEGMENT *	pERSCurr;
		The event ring segment which dequeue pointer is located.

	USBPUMP_XHCD_EVENT_RING_SEGMENT_TABLE *pERST;
		Logical address of event ring segment table.

See Also:
	UsbPumpXhcdI_CreateEventRing()
	UsbPumpXhcdI_InitEventRing()
	UsbPumpXhcdI_FreeEventRing().

*/

struct __TMS_STRUCTNAME(USBPUMP_XHCD_EVENT_RING_SEGMENT_TABLE)
	{
	/* The number of event ring segments in the table */
	__TMS_UINT			NumberOfEntres;

	/* Buffer handle of event ring segment table handle */
	__TMS_USBPUMP_BUFFER_HANDLE	hERST;

	/* Physical address of event ring segment table */
	__TMS_PHYSADDR			physicalAddr;

	/* Logical address of event ring segment table */
	__TMS_USBPUMP_XHCI_EVENT_RING_SEGMENT_TABLE *pTable;
	};

struct __TMS_STRUCTNAME(USBPUMP_XHCD_EVENT_RING_SEGMENT)
	{
	/* The number of TRBs in the event ring segment */
	__TMS_UINT				Size;

	/* Event Ring segment index */
	__TMS_UINT				iEventSeg;

	/* Last event trb address */
	__TMS_USBPUMP_XHCI_TRB *		pLastEventTRB;

	/* Buffer handle of event ring segment handle */
	__TMS_USBPUMP_BUFFER_HANDLE		hEventSegment;

	/* Physical address of event ring segment */
	__TMS_PHYSADDR				ERSPhyAddr;

	/* Logical address of event ring segment */
	__TMS_USBPUMP_XHCI_TRB *		pERS;

	/* Make it be circular link list */
	__TMS_USBPUMP_XHCD_EVENT_RING_SEGMENT * pNext;
	__TMS_USBPUMP_XHCD_EVENT_RING_SEGMENT * pLast;
	};

struct __TMS_STRUCTNAME(USBPUMP_XHCD_EVENT_RING)
	{
	/* Consumer Cycle State */
	__TMS_UINT32				CCS;

	/* Dequeue pointer of event ring */
	__TMS_USBPUMP_XHCI_TRB *		pDequeue;

	/* Physical address of Dequeue pointer because HCD has to let HC know */
	__TMS_PHYSADDR				PhyDequeuePtr;

	/* Current Event Ring Segment which dequeue pointer is located */
	__TMS_USBPUMP_XHCD_EVENT_RING_SEGMENT *	pERSCurr;

	/* First Event Ring Segment */
	__TMS_USBPUMP_XHCD_EVENT_RING_SEGMENT *	pERSHead;

	__TMS_USBPUMP_XHCD_EVENT_RING_SEGMENT_TABLE *pERST;
	};

/****************************************************************************\
|
|	Definition of Transfer Ring structure
|
\****************************************************************************/

/*

Type:	USBPUMP_XHCD_TRANSFER_RING

Function:
	The structure represents xHCD Transfer ring.

Description:
	A transfer Ring is for each pipe. In this design, the size of Transfer
	Ring is one page (256 Command TRBs) for Bulk/ISO, 4 for other kinds of
	pipes.

	The interrupt target for Transfer Ring is defined in Interrupter Target
	in the TRB data structure. In this design, all transfer events are
	inserted into primary Event Ring as well..

Contents:
	UINT PCS:1;
		Producer Cycle State. Initialized to 1. Toggle when Enqueue Ptr
		points to Link TRB.

	UINT fSupportStream:1;
		Whether this transfer ring belongs to the pipe which supports
		stream transfer.

	UINT fIgnoreEvent: 1;
		xHci V1.0 only. Two transfer events are reported when short
		transfer happens.

	UINT intervalInMicroFrame;
		Used by Isoch only. If device is FS or HS behind FS-hub, the
		wInterval should be multiply by 8. If a device is HS, the
		intervalInMicroFrame is the same value as wInterval

	UINT intervalInFrame;
		Used by Isoch only. It can be calculated with intervalInMicroFrame.
		If intervalInMicroFrame is less than 8, then intervalInFrame = 1.

	USBPUMP_XHCD_TR_STATE		State;
		The state indicates the transfer ring(endpoint context) status.
		XHCD_TR_STATE_IDLE: when transfer ring is created. It is working
		   status.
		XHCD_TR_STATE_STOP_ENDPOINT:
		   Stop endpoint is sent and wait for command event.
		XHCD_TR_STATE_SET_TR_DEQUEUE:
		   Set TR Dequeue is send and wait for command event.
		XHCD_TR_STATE_RESET_EP:
		   Reset endpoint is sent and wait for command event.
		XHCD_TR_STATE_HALTED:
		   Completion code is STALL or transaction error.
		XHCD_TR_STATE_ERROR:
		   Completion code is Error.

	USBPUMP_XHCD_SLOT_ID SlotId;
		Slot ID of the device.

	USBPUMP_XHCD_ENDPOINT_ID EndpointId;
		Endpoint ID for this pipe.

	USBPUMP_XHCD_STREAM_ID	StreamId;
		Stream ID for this transfer ring. Remember, StreamID = 0
		does NOT mean this pipe doesn't support transfer ring. It
		is either the 0th transfer ring of stream context or the
		transfer ring of normal transfer ring.

	USBPUMP_XHCD_STREAM_CONTEXT_ARRAY *	pStreamCtxArray;
		Pointer of stream context array.

	BYTES	nAvailableTRB;
		The number of TRBs can be written by HCD.

	USBPUMP_XHCD_START_NEXT_IO_FN * pStartNextIoFn;
		Post proc function for the transfer ring.

	USBPUMP_XHCD_TRANSFER_RING_POST_PROCESS PostProc;
		Post Proc for Isoch transfer and Bulk-In transfer

	USBPUMP_HCD_REQUEST *pActiveReq;
		The HcdRequests which are served in a Transfer Ring.

	USBPUMP_HCD_REQUEST *pMappingReq;
		The HcdRequests which are called MapDmaAsync in a Transfer Ring.

	USBPUMP_HCD_REQUEST *pPendingReq;
		The HcdRequests which have not been put into a Transfer Ring.

	USBPUMP_HCD_REQUEST InternalHcdReq;
		This field is to save some HcdRequest which are using command
		ring. But these HcdRequest is not from upper layer but from HCD
		itself under some special cases. For example, CancelRequest,
		HCD has to send "Stop Endpoint". Another sample is HCD wants to
		change TR pointer by sending "Set TR Deuqueue Pointer" when
		transfer event with error.

	USBPUMP_XHCI_TRB * pEnqueue;
		Enquque pointer address. Initialized with the beginning address
		of first TRB of Ring. HCD will increase it by adding sizeof(TRB).

	USBPUMP_XHCI_TRB * pDequeue;
		HCD updates it based on Transfer Event. HCD uses it
		to check whether a ring is Full.

	USBPUMP_HCD_PIPE * pPipe;
		Point to pipe.

	USBPUMP_XHCD_BUFFER_SEGMENT *	pSegment;
		Sw defined segment structure which also contain segment array
		for DMA mapping. It is created/freed with transfer ring.

	USBPUMP_XHCD_ISO_PKT_DESC *	pIsochDesc;
		Sw defined Isoch packet descriptor structure which contains
		the current packet descriptor index, total number of packet
		descriptor, etc.

	PHYSADDR TransferRingPhyAddr;
		16-bit alignment address of Transfer Ring. HCD allocates it
		by calling USBPUMP_UHILAUX_ALLOCATE_COMMON_BUFFER_FN() at the
		initialization time.

	USBPUMP_BUFFER_HANDLE	hTransferRing;
		Buffer handle of Transfer Ring. HCD gets it from
		USBPUMP_UHILAUX_ALLOCATE_COMMON_BUFFER_FN().

	UINT32	SizeOfTransferRing;
		The number of TRBs in the transfer ring.

	USBPUMP_XHCI_TRB * pBaseTransferRing;
		The head(logical address) of transfer ring.

See Also:
	UsbPumpXhcdI_CreateTransferRing()
	UsbPumpXhcdI_FreeTransferRing()
	UsbPumpXhcdI_CfgEpInitPipe()
	UsbPumpXhcdI_GetNumOfAvailableTrb().

*/

struct __TMS_STRUCTNAME (USBPUMP_XHCD_EVENT_TRB)
        {
        __TMS_UINT32	DW0;
	__TMS_UINT32	DW1;
	__TMS_UINT32	DW2;
	__TMS_UINT32	DW3;
        };

__TMS_FNTYPE_DEF(
USBPUMP_XHCD_START_NEXT_IO_FN,
__TMS_VOID,
        (
        __TMS_USBPUMP_HCD_XHCI *,       /* pHcd */
        __TMS_USBPUMP_XHCD_TRANSFER_RING * /* pTR */
        ));

__TMS_FNTYPE_DEF(                                               \
USBPUMP_XHCD_ISOCH_POST_PROCESS_FN,                             \
__TMS_VOID,                                                     \
        (                                                       \
        __TMS_USBPUMP_HCD_XHCI *,       /* pHcd */              \
        __TMS_USBPUMP_HCD_REQUEST *,    /* pHcdReq */           \
        __TMS_USBPUMP_XHCD_TRANSFER_RING *, /* pTR */           \
        __TMS_USBPUMP_XHCD_COMPLETION_CODE, /* CompletionCode*/ \
        __TMS_USBPUMP_XHCI_TRB *,        /* pLastTrb */          \
        __TMS_USBPUMP_XHCD_ISO_PKT_DESC *, /* pXhcdIsochDescr */ \
	__TMS_BYTES			/* uEventDataLen */	\
        ));

__TMS_FNTYPE_DEF(                                               \
USBPUMP_XHCD_POST_PROCESS_FN,                           	\
__TMS_VOID,                                                     \
        (                                                       \
        __TMS_USBPUMP_HCD_XHCI *,       /* pHcd */              \
        __TMS_USBPUMP_HCD_REQUEST *,    /* pHcdReq */           \
        __TMS_USBPUMP_XHCD_TRANSFER_RING *, /* pTR */           \
        __TMS_USBPUMP_XHCD_COMPLETION_CODE, /* CompletionCode*/ \
        __TMS_USBPUMP_XHCD_EVENT_TRB *,       /* pEventTRB */   \
        __TMS_USBPUMP_XHCI_TRB *        /* pLastTrb */          \
        ));

__TMS_TYPE_DEF_UNION(USBPUMP_XHCD_TRANSFER_RING_POST_PROCESS);
union __TMS_UNIONNAME(USBPUMP_XHCD_TRANSFER_RING_POST_PROCESS)
	{
	/* Post Proc for Isoch transfer */
	__TMS_USBPUMP_XHCD_ISOCH_POST_PROCESS_FN *	pIsochFn;
	/* Post Proc for Bulk-In transfer */
	__TMS_USBPUMP_XHCD_POST_PROCESS_FN *	pBulkIntFn;
	};

struct __TMS_STRUCTNAME(USBPUMP_XHCD_TRANSFER_RING)
	{
	/* Producer Cycle State */
	__TMS_UINT				PCS: 1;

	__TMS_UINT				fSupportStream: 1;

	__TMS_UINT				fIgnoreEvent:1;
	/* Cycle bit saved for Setup TRB */
	__TMS_UINT				fSetupTrbCycle: 1;

	/* Address for Setup TRB */
	__TMS_USBPUMP_XHCI_TRB *		pSetupTRB;

	/* wInterval for pipe in microframe */
	__TMS_UINT				intervalInMicroFrame;

	/* wInterval for pipe in frame */
	__TMS_UINT				intervalInFrame;

	/* wMaxPacketSize for pipe */
	__TMS_UINT16				wMaxPacketSize;
	__TMS_UINT8				bMaxBurstSize;
	__TMS_UINT8				bSpeed;
	__TMS_UINT8				bRootHubPortNo;

	/* Transfer Ring state */
	__TMS_USBPUMP_XHCD_TR_STATE		State;

	__TMS_USBPUMP_XHCD_SLOT_ID		SlotId;
	__TMS_USBPUMP_XHCD_ENDPOINT_ID		EndpointId;
	__TMS_USBPUMP_XHCD_STREAM_ID		StreamId;

	/* Stream context array pointer */
	__TMS_USBPUMP_XHCD_STREAM_CONTEXT_ARRAY *	pStreamCtxArray;

	__TMS_USBPUMP_XHCD_TRANSFER_RING **	ppTransferRingVector;

	/* Number of available TRB */
	__TMS_UINT				nAvailableTRB;

	/* Post Proc in SetTRDeqEvent */
	__TMS_USBPUMP_XHCD_START_NEXT_IO_FN * 	pStartNextIoFn;

	__TMS_USBPUMP_XHCD_TRANSFER_RING_POST_PROCESS	PostProc;

	/* Internal HcdRequest: Command HcdReq*/
	__TMS_USBPUMP_HCD_REQUEST 		InternalHcdReq;

	/* HcdRequests in the service */
	__TMS_USBPUMP_HCD_REQUEST *		pActiveReq;

	/* HcdRequests which is waiting for callback of MapDamAsync */
	__TMS_USBPUMP_HCD_REQUEST *		pMappingReq;

	/* pending HcdReq */
	__TMS_USBPUMP_HCD_REQUEST *		pPendingReq;

	/* Pipe informaiton*/
	__TMS_USBPUMP_HCD_PIPE *		pPipe;

	/* sw defined segment structure */
	__TMS_USBPUMP_XHCD_BUFFER_SEGMENT *	pSegment;

	/* Segment queue for Isoch */
	__TMS_USBPUMP_XHCD_BUFFER_SEGMENT *	pIsoFreeSeg;
	__TMS_USBPUMP_XHCD_BUFFER_SEGMENT *	pIsoMappingSeg;
	__TMS_USBPUMP_XHCD_BUFFER_SEGMENT *	pIsoMappedSeg;

	/* Enqueue pointer of transfer ring */
	__TMS_USBPUMP_XHCI_TRB *		pEnqueue;

	/* Dequeue pointer of transfer ring */
	__TMS_USBPUMP_XHCI_TRB *		pDequeue;

	__TMS_USBPUMP_XHCI_TRB *		pLinkTrb;

	/* Physical address of Transfer Ring  */
	__TMS_PHYSADDR				LinkTrbPhyAddr;

	/* Number of TRBs in transfer ring */
	__TMS_UINT				SizeOfTransferRing;

	/* Buffer handle */
	__TMS_USBPUMP_BUFFER_HANDLE		hTransferRing;

	/* Physical address of Transfer Ring  */
	__TMS_PHYSADDR				TransferRingPhyAddr;

	/* Logical address of transfer ring */
	__TMS_USBPUMP_XHCI_TRB *		pBaseTransferRing;
	};

/****************************************************************************\
|
|	Definition of Scratchpad buffer structure
|
\****************************************************************************/

/*

Type:	USBPUMP_XHCD_SCRATCHPAD_BUFFER_ARRAY

Function:
	The structure represents xHCD Scrachpad buffer array.

Description:
	The Scrachpad buffer array is used to define the locations of
	statically allocated memory pages that are available for the
	private use of the xHC.

	The location of the Scrachpad buffer array is defined by the
	Max Scrachpad Buffers in the HCSPARAMS2 register.

Contents:
	UINT 	NumberOfEntres;
		The number of Scrachpad buffer element in the array. It
		should be same as pHcd->xhcd_MaxScratchpadBuf.

	BYTES	SizeInBytesArray
		The size in bytes for scratchpad buffer array.

	USBPUMP_XHCD_SCRATCHPAD_BUFFER_ELEMENT * pHead
		Head of Scratchpad buffer element link list.

	USBPUMP_BUFFER_HANDLE	hScrachArray;
		Buffer handle of Scrachpad buffer array.

	PHYSADDR ScrachBufArrayPhyAddr;
		Physical address of Scrachpad buffer array.

	USBPUMP_XHCI_SCRATCHPAD_BUFFER_ARRAY_ELEMENT *	pArray;
		Logical address of Scrachpad buffer array.

See Also:
	UsbPumpXhcdI_CreateScratchpadBuffer()

*/
struct __TMS_STRUCTNAME(USBPUMP_XHCD_SCRATCHPAD_BUFFER_ARRAY)
	{
	/* The number of scratchpad buffer in the array */
	__TMS_UINT			NumberOfElements;

	/* The size in bytes for scratchpad buffer array */
	__TMS_BYTES			SizeInBytesArray;

	/* head of Scratchpad buffer */
	__TMS_USBPUMP_XHCD_SCRATCHPAD_BUFFER_ELEMENT * pHead;

	/* Buffer handle of scrachpad buffer array */
	__TMS_USBPUMP_BUFFER_HANDLE	hScrachArray;

	/* Physical address of scrachpad buffer array */
	__TMS_PHYSADDR			ScrachBufArrayPhyAddr;

	/* Logical address of scrachpad buffer array */
	__TMS_USBPUMP_XHCI_SCRATCHPAD_BUFFER_ELEMENT *	pArray;
	};

struct __TMS_STRUCTNAME(USBPUMP_XHCD_SCRATCHPAD_BUFFER_ELEMENT)
	{
	/* The size in byte of each Scrachpad buffer entry */
	__TMS_UINT				SizeInBytesElement;

	/* Buffer handle of scrachpad element */
	__TMS_USBPUMP_BUFFER_HANDLE		hElement;

	/* Physical address of scrachpad buffer element */
	__TMS_PHYSADDR				ScrachBufElePhyAddr;

	__TMS_VOID *				pScrachBufEleLogicalAddr;

	__TMS_USBPUMP_XHCD_SCRATCHPAD_BUFFER_ELEMENT * pNext;
	__TMS_USBPUMP_XHCD_SCRATCHPAD_BUFFER_ELEMENT * pPrev;
	};

/****************************************************************************\
|
|	Definition of XHCD configuration
|
\****************************************************************************/

/*

Type:  USBPUMP_XHCD_ALLOCATE_TRB_POLICY_FN

Function:
	Allocate the number of TRB for each pipe

Definition:
	typedef UINT
	(*USBPUMP_XHCD_ALLOCATE_TRB_POLICY_FN)(
		USBPUMP_DEVICE_SPEED	bSpeed,
		UINT8			bEndpointType
		);

Description:
	Allocate the number of TRB for different types of pipes.

Returns:
	The number of TRBs.

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_XHCD_ALLOCATE_TRB_POLICY_FN,
__TMS_UINT,
	(
	__TMS_USBPUMP_DEVICE_SPEED,	/* bSpeed */
	__TMS_UINT8			/* bEndpointType */
	));

/*

Type:	USBPUMP_XHCD_CONFIG_INFO

Function:
	The structure XHCD configuration structure.

Description:
	This structure is used to pass configuration information from
|	platform initialization to the XHCD init code.

Contents:
	UINT32	ulWiring;
	Bitmap of wiring value.

	UHIL_BUSHANDLE hBus;
		Bus handle.

	IOPORT IoPort
		Io base address.

	UHIL_INTERRUPT_RESOURCE_HANDLE hUsbInt
		Interrupt handle

	UINT	MaxStreamId;
		Max stream ID supported by xHcd.

	UINT MaxSlots
		Maximum number of slots.

	UINT	nBufSeg;
		The number of buffer segment.

	UINT SizeOfEventRing
		The number of TRBs in the event ring.

	UINT SizeOfEventRingSeg;
		The number of TRBs in an Event Ring segment.

	UINT IMod
		The Interrupt Moderaton Interval.

	UINT HwResetTime;
		Hardware reset time.

	UINT CmdAbortTime;
		Time to check CommandAbort operation

	UINT CleanUpTime;
		Time for USBD to clean up after command abort timeout.

	USBPUMP_XHCD_ALLOCATE_TRB_POLICY_FN * 	pTrbAllocPolicyFn;
		TRB allocation policy function set by client.

	UINT32	ulRxThresholdRegOffset;
	UINT32	ulRxThresholdRegValue;
	UINT32	ulTxThresholdRegOffset;
	UINT32	ulTxThresholdRegValue;
		Rx/Tx threshold register set for Truro.

	UINT32 ulSystemResumeRecoveryTime;
		System recovery time from the resume.

	UINT ulMapDmaRange;
	UINT32 ulMapDmaRangeTime;
		Workaround for Andover driver.

	UINT32 ulDebugFlags
		Debug level for XHCD.

See also:
	USBPUMP_XHCD_CONFIG_INFO_INIT_V1

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_XHCD_CONFIG_INFO);
struct __TMS_STRUCTNAME(USBPUMP_XHCD_CONFIG_INFO)
	{
	__TMS_UINT32					ulWiring;

	__TMS_UHIL_BUSHANDLE				hBus;

	/*Base address */
	__TMS_IOPORT					IoPort;

	/* interrupt handle */
	__TMS_UHIL_INTERRUPT_RESOURCE_HANDLE		hUsbInt;

	/* Max Stream IDs */
	__TMS_UINT					MaxStreamId;

	/* Number of slots are supported by xHCI */
	__TMS_UINT					MaxSlots;

	/* The number of buffer segment */
	__TMS_UINT					nBufSeg;

	/* Number of Event TRBs */
	__TMS_UINT					SizeOfEventRing;

	/* Number of TRB in a Event ring segment */
	__TMS_UINT					SizeOfEventRingSeg;

	/* Interrupt Modieration Interval */
	__TMS_UINT					IMod;

	/* Hardware reset time */
	__TMS_UINT					HwResetTime;

	/* Command Abort time */
	__TMS_UINT					CmdAbortTime;

	/* CleanUp time */
	__TMS_UINT					CleanUpTime;

	/* TRB allocation policy function */
	__TMS_USBPUMP_XHCD_ALLOCATE_TRB_POLICY_FN * 	pTrbAllocPolicyFn;
	
	/* HCDKIT Switch */
	__TMS_CONST __TMS_USBPUMP_HCDKIT_SWITCH *	pHcdKitSwitch;

	__TMS_UINT32					ulRxThresholdRegOffset;
	__TMS_UINT32					ulRxThresholdRegValue;

	__TMS_UINT32					ulTxThresholdRegOffset;
	__TMS_UINT32					ulTxThresholdRegValue;

	__TMS_UINT32					ulSystemResumeRecoveryTime;
	/* MapDmaRange */
	__TMS_UINT					ulMapDmaRange;
	__TMS_UINT32					ulMapDmaRangeTime;

/* the number of TRBs in global buffer segment */
	__TMS_UINT32					nGlobalBufSeg;
	/* the debug flags */
	__TMS_UINT32					ulDebugFlags;
	};
	
#define	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V10(	\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_pHcdKitSwitch,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_nGlobalBufSeg,			\
		a_ulDebugFlags				\
		)					\
	{						\
	(a_ulWiring),					\
	(a_hBus),					\
	(a_IoPort),					\
	(a_hUsbInt),					\
	(a_MaxStreamId),				\
	(a_MaxSlots),					\
	(a_nBufSeg),					\
	(a_SizeOfEventRing),				\
	(a_SizeOfEventRingSeg),				\
	(a_IMod),					\
	(a_HwResetTime),				\
	(a_CmdAbortTime),				\
	(a_CleanUpTime),				\
	(a_pTrbAllocPolicyFn),				\
	(a_pHcdKitSwitch),				\
	(a_ulRxThresholdRegOffset),			\
	(a_ulRxThresholdRegValue),			\
	(a_ulTxThresholdRegOffset),			\
	(a_ulTxThresholdRegValue),			\
	(a_ulSystemResumeRecoveryTime),			\
	(a_ulMapDmaRange),				\
	(a_ulMapDmaRangeTime),				\
	(a_nGlobalBufSeg),				\
	(a_ulDebugFlags)				\
	}

#define	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V9(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_nGlobalBufSeg,			\
		a_ulDebugFlags				\
		)					\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V10(	\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		/* V10: pHcdKitSwitch */&gk_hub_xhcd_switch, \
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_nGlobalBufSeg,			\
		a_ulDebugFlags				\
		)

#define	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V8(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_ulDebugFlags				\
		)					\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V9(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		/* V9 nGlobalBufSeg */ 0,		\
		a_ulDebugFlags				\
		)

#define	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V7(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_pUsb2HcPortMap,			\
		a_ulUsb2HcPortMap,			\
		a_pUsb3HcPortMap,			\
		a_ulUsb3HcPortMap,			\
		a_ulDebugFlags				\
		)					\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V8(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		/* V8: removed a_pUsb2HcPortMap */	\
		/* V8: removed a_ulUsb2HcPortMap */	\
		/* V8: removed a_pUsb3HcPortMap */	\
		/* V8: removed a_ulUsb3HcPortMap */	\
		a_ulDebugFlags				\
		)

#define	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V6(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_pUsb2HcPortMap,			\
		a_ulUsb2HcPortMap,			\
		a_pUsb3HcPortMap,			\
		a_ulUsb3HcPortMap,			\
		a_ulDebugFlags				\
		)					\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V7(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		/*V7: CmdAbortTime */ 15000,		\
		/*V7: CleanUpTime */ 6000,		\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_pUsb2HcPortMap,			\
		a_ulUsb2HcPortMap,			\
		a_pUsb3HcPortMap,			\
		a_ulUsb3HcPortMap,			\
		a_ulDebugFlags				\
		)					\

#define	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V5(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_ulDebugFlags				\
		)					\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V6(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		/* V6: HwResetTime */ 20,		\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		/* V6: pUsb2HcPortMap */ __TMS_NULL,	\
		/* V6: ulUsb2HcPortMap */ 0,		\
		/* V6: pUsb3HcPortMap */ __TMS_NULL,	\
		/* V6: ulUsb3HcPortMap */ 0,		\
		a_ulDebugFlags				\
		)

#define	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V4(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_ulDebugFlags				\
		)					\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V5(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		65534,					\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_ulDebugFlags				\
		)

#define	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V3(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulDebugFlags				\
		)					\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V4(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		/* MapDmaRange, 4M */ 0x400000,		\
		/* ulMapDmaRangeTime, 3s */ 3000,	\
		a_ulDebugFlags				\
		)

#define	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V2(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulDebugFlags				\
		)					\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V3(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		/* V3: nBufSeg */ 2,			\
		a_SizeOfEventRing,			\
		/* V3: SizeOfEventRingSeg */ 512,	\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulDebugFlags				\
		)

#define	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V1(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_SizeOfEventRing,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulDebugFlags				\
		)					\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V2(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_SizeOfEventRing,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		/* V2: ulTxThresholdRegOffset */ 0,	\
		/* V2: ulTxThresholdRegValue */ 0,	\
		/* V2: ulSystemResumeRecoveryTime */ 10,\
		a_ulDebugFlags				\
		)
		
#define __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V10(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_pHcdKitSwitch,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_nGlobalBufSeg,				\
		a_ulDebugFlags					\
                )                                               \
    do	{                                                       \
	(a_pConfig)->ulWiring		= (a_ulWiring);		\
	(a_pConfig)->hBus               = (a_hBus);             \
	(a_pConfig)->IoPort             = (a_IoPort);           \
	(a_pConfig)->hUsbInt            = (a_hUsbInt);          \
	(a_pConfig)->MaxStreamId	= (a_MaxStreamId);	\
	(a_pConfig)->MaxSlots           = (a_MaxSlots);         \
	(a_pConfig)->nBufSeg		= (a_nBufSeg);		\
	(a_pConfig)->SizeOfEventRing    = (a_SizeOfEventRing);  \
	(a_pConfig)->SizeOfEventRingSeg = (a_SizeOfEventRingSeg); \
	(a_pConfig)->IMod		= (a_IMod);		\
	(a_pConfig)->HwResetTime	= (a_HwResetTime);	\
	(a_pConfig)->CmdAbortTime	= (a_CmdAbortTime);	\
	(a_pConfig)->CleanUpTime	= (a_CleanUpTime);	\
	(a_pConfig)->pTrbAllocPolicyFn  = (a_pTrbAllocPolicyFn);\
	(a_pConfig)->pHcdKitSwitch	= (a_pHcdKitSwitch);	\
	(a_pConfig)->ulRxThresholdRegOffset = (a_ulRxThresholdRegOffset); \
	(a_pConfig)->ulRxThresholdRegValue  = (a_ulRxThresholdRegValue);  \
	(a_pConfig)->ulTxThresholdRegOffset = (a_ulTxThresholdRegOffset); \
	(a_pConfig)->ulTxThresholdRegValue  = (a_ulTxThresholdRegValue);  \
	(a_pConfig)->ulSystemResumeRecoveryTime = (a_ulSystemResumeRecoveryTime); \
        (a_pConfig)->ulMapDmaRange 	= (a_ulMapDmaRange);	\
        (a_pConfig)->ulMapDmaRangeTime  = (a_ulMapDmaRangeTime); \
        (a_pConfig)->nGlobalBufSeg 	= (a_nGlobalBufSeg);	\
	(a_pConfig)->ulDebugFlags       = (a_ulDebugFlags);	\
	} while (0)

#define __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V9(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_nGlobalBufSeg,				\
		a_ulDebugFlags					\
                )                                               \
    __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V10(                	\
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		/* V10: pHcdKitSwitch */&gk_hub_xhcd_switch,	\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_nGlobalBufSeg,				\
		a_ulDebugFlags					\
                )

#define __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V8(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_ulDebugFlags					\
                )                                               \
    __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V9(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		/* V9: nGlobalBufSeg */ 0,			\
		a_ulDebugFlags					\
                )

#define __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V7(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_pUsb2HcPortMap,				\
		a_ulUsb2HcPortMap,				\
		a_pUsb3HcPortMap,				\
		a_ulUsb3HcPortMap,				\
		a_ulDebugFlags					\
                )                                               \
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V8(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		/* V8: removed a_pUsb2HcPortMap */		\
		/* V8: removed a_ulUsb2HcPortMap */		\
		/* V8: removed a_pUsb3HcPortMap */		\
		/* V8: removed a_ulUsb3HcPortMap */		\
		a_ulDebugFlags					\
                )

#define __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V6(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_pUsb2HcPortMap,				\
		a_ulUsb2HcPortMap,				\
		a_pUsb3HcPortMap,				\
		a_ulUsb3HcPortMap,				\
		a_ulDebugFlags					\
                )                                               \
    __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V7(                	\
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                /* V7: CmdAbortTime */ 15000,			\
		/* V7: CleanUpTime */ 6000,			\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_pUsb2HcPortMap,				\
		a_ulUsb2HcPortMap,				\
		a_pUsb3HcPortMap,				\
		a_ulUsb3HcPortMap,				\
		a_ulDebugFlags					\
                )

#define __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V5(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_ulDebugFlags					\
                )                                               \
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V6(        	\
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                /* V6: HwResetTime */ 0,			\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		/* pUsb2HcPortMap */ __TMS_NULL,		\
		/* ulUsb2HcPortMap */ 0,			\
		/* pUsb3HcPortMap */ __TMS_NULL,		\
		/* ulUsb3HcPortMap */ 0,			\
		a_ulDebugFlags					\
                )

#define __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V4(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_ulDebugFlags					\
                )                                               \
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V5(        	\
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                65534,						\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_ulDebugFlags					\
                )

#define	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V3(	\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulDebugFlags				\
		)					\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V4(	\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		/* ulMapDmaRange, 4M */ 0x400000,	\
		/* ulMapDmaRangeTime, 3s */ 3000,	\
		a_ulDebugFlags				\
		)

#define __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V2(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulDebugFlags					\
                )                                               \
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V3(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxSlots,                                     \
                /* V3: nBufSeg */ 2,				\
                a_SizeOfEventRing,                              \
                /* V3: SizeOfEventRingSeg */ 512,		\
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulDebugFlags					\
                )

#define __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V1(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxSlots,                                     \
                a_SizeOfEventRing,                              \
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulDebugFlags					\
                )                                               \
      __TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V2(               	\
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxSlots,                                     \
                a_SizeOfEventRing,                              \
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		/* V2: ulTxThresholdRegOffset */ 0,		\
		/* V2: ulTxThresholdRegValue */ 0,		\
		/* V2: ulSystemResumeRecoveryTime */ 10,	\
		a_ulDebugFlags					\
                )
/*
|| This bit gives the client an option to diable IE.
*/
#define __TMS_USBPUMP_XHCD_WIRING_NOT_DISABLE_IE		(1 << 0)
#define __TMS_USBPUMP_XHCD_WIRING_SET_RX_REGISTER		(1 << 1)
#define __TMS_USBPUMP_XHCD_WIRING_HUB_WORKAROUND_ENABLE		(1 << 2)
#define __TMS_USBPUMP_XHCD_WIRING_SET_ALT_PORT_POWER		(1 << 3)
#define __TMS_USBPUMP_XHCD_WIRING_CLEAR_P3			(1 << 4)
#define __TMS_USBPUMP_XHCD_WIRING_SET_TX_REGISTER		(1 << 5)
/*
|| Workaround for TI xHc since port speed is detected incorrectly
|| by hw during system wait/wake
*/
#define __TMS_USBPUMP_XHCD_WIRING_PORT_SPEED_WORKAROUND		(1 << 6)
/* workaround to disable usb2 port suspend for TI xHc */
#define __TMS_USBPUMP_XHCD_WIRING_DISABLE_USB2_SUSPEND		(1 << 7)
/* Mapping range flag for Andover branch */
#define __TMS_USBPUMP_XHCD_WIRING_SET_MAPDMA_RANGE		(1 << 8)
/* Set wakemask for Altport */
#define __TMS_USBPUMP_XHCD_WIRING_SET_ALT_PORT_WAKE_MASK	(1 << 9)
/* Set PHY for TI xHc */
#define __TMS_USBPUMP_XHCD_WIRING_SET_P3			(1 << 10)
/* Non-default port connector mapping */
#define __TMS_USBPUMP_XHCD_WIRING_USE_PORT_MAPMPING_INFO	(1 << 11)
/* Turn off WakeOn mask during system suspend */
#define __TMS_USBPUMP_XHCD_WIRING_DISABLE_WAKE_MASK		(1 << 12)
/* Limit bInterval */
#define __TMS_USBPUMP_XHCD_WIRING_LIMIT_BINTERNAL		(1 << 13)
/* Disable save/restore */
#define __TMS_USBPUMP_XHCD_WIRING_DISABLE_SAVE_RESTORE		(1 << 14)
/* Turn off port power during shutdown */
#define __TMS_USBPUMP_XHCD_WIRING_PORT_OFF_WHEN_SHUT_DOWN	(1 << 15)
/* Don't ignore short event for Andover */
#define __TMS_USBPUMP_XHCD_WIRING_SET_MAX_BURST			(1 << 16)
/* dummy short transfer for Andover */
#define __TMS_USBPUMP_XHCD_WIRING_DUMMY_SHORT_TRANSFER		(1 << 17)
/* report compatible ports instead of normalized RootHub Ports */
#define __TMS_USBPUMP_XHCD_WIRING_REPORT_COMPATIBLE_PORT	(1 << 18)

/****************************************************************************\
|
|	Definition of XHCI HCD
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_XHCD_MESSAGE_EVENT);
struct __TMS_STRUCTNAME	(USBPUMP_XHCD_MESSAGE_EVENT)
	{
	__TMS_UCALLBACKCOMPLETION		Completion;
	__TMS_USBPUMP_HCD_XHCI *		pHcd;
	};

struct __TMS_STRUCTNAME	(USBPUMP_HCD_XHCI_CONTENTS)
	{
	__TMS_USBPUMP_HCDKIT_HCD_CONTENTS__STRUCT;

	/* Command Ring */
	__TMS_USBPUMP_XHCD_COMMAND_RING  	CmdRing;

	/* Event Ring */
	__TMS_USBPUMP_XHCD_EVENT_RING 		EventRing;

	/*
	|| The size of pvTransferRing is MaxSlot * 31 items.
	|| Each item is pointer of transfer ring and filled
	|| during pipe's initialization. This table is used
	|| to find the matched HcdRequests from Slot ID and
	|| Endpoint ID from transfer event.
	*/
	__TMS_USBPUMP_XHCD_TRANSFER_RING ** 	pvTransferRing;

	/* Scratchpad buffer array */
	__TMS_USBPUMP_XHCD_SCRATCHPAD_BUFFER_ARRAY ScratchPadBufferArray;

	__TMS_UPLATFORM_SYNCHRONIZATION_BLOCK 	SyncBlock;
	__TMS_VOLATILE __TMS_UINT		IntSeq;

	/* Line-based interrupt event callback */
	__TMS_UCALLBACKCOMPLETION		LineEvent;

	/* message-based interrupt event callback */
	__TMS_USBPUMP_XHCD_MESSAGE_EVENT *	pMessageEvents;
	__TMS_UHIL_INTERRUPT_MESSAGE		nMessageEvents;

	/* Unknown message interrupt event callback */
	__TMS_UCALLBACKCOMPLETION		UnknownMessageEvent;
	__TMS_UINT_PTR __TMS_VOLATILE		UnknownMessageCode;

	__TMS_USBPUMP_XHCD_CONFIG_INFO 		ConfigInfo;

	/* Interrupt Handle */
	__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE	hcUsbInt;

	__TMS_VOLATILE __TMS_UINT_PTR		IntSts;

	/* Device Context Base Address Array: (MaxSlot + 1) * 8 bytes */
	__TMS_USBPUMP_XHCD_DCBAA		DCBAA;

	/*
	|| Device Context Array: It has (MaxSlot + 1) items.
	|| Each item is the pointer of DeviceContext and is
	|| filled during "AddressDevice" command.
	*/
	__TMS_USBPUMP_XHCD_DEVICE_CONTEXT **   	pvDeviceContext;

	/* Operaional register offset */
	__TMS_UINT32				OperationRegOffset;

	/* Run time register offset */
	__TMS_UINT32				RuntimeRegOffset;

	/* HC command Doorbell register */
	__TMS_UINT32				HcCmdDoorbellReg;

	/* xHC version */
	__TMS_UINT16				xHcVersion;

	/* xHC capability paramters */
	__TMS_UINT32				Cap;
	__TMS_UINT32				Cap1;
	__TMS_UINT32				Cap2;
	__TMS_UINT32				Cap3;

	/* xHC Extended capabilities */
	__TMS_UINT32				Usb3Cap;
	__TMS_UINT32				Usb2Cap;
	__TMS_ADDRBITS				addrUsbLegSupCap;

	/* xHc supported protocol cap */
	__TMS_ADDRBITS				Usb3CapOffset;
	__TMS_ADDRBITS				Usb2CapOffset;

	__TMS_UINT8				Usb3PSIC;
	__TMS_UINT8				Usb2PSIC;

	/* U1 Device Exit Latency */
	__TMS_UINT8				U1Latency;
	/* U1 Device Exit Latency */
	__TMS_UINT16				U2Latency;

	/*
	|| This field will be set when reading HCCPARAMS register. It includes
	|| AC64, BNC, PPC, CSZ information which indicate the xHC capability.
	|| HCD uses this information in some operations.
	*/
	__TMS_UINT				fAC64:1;
	__TMS_UINT				fBNC:1;
	__TMS_UINT				fCSZ:1;
	__TMS_UINT				fPIND:1;
	__TMS_UINT				fLHRC:1;
	__TMS_UINT				fLTC:1;
	__TMS_UINT				fNSS:1;
	__TMS_UINT				fPAE:1;

	__TMS_UINT				fSet_PORTSC_CSC:1;

	__TMS_UINT				fValidUsbLegSupCap: 1;

	/*
	|| Maximum Primary Stream Array Size(MaxPSASize). This field indentifies
	|| the maximum size Primary Stream Array that the xHC supports. The
	|| Primary Stream Array size = 2 ^ (MaxPSASize + 1). Valid MaxPSASize
	|| is from 1 to 15
	*/
	__TMS_UINT				MaxPSASize;
	__TMS_UINT				MaxNumStreamIDHostSupport;

	/* Isoch Scheduleing Threshold (IST) */
	__TMS_UINT				uIST;

	/*
	|| Valid values are 0-15. This field determines the maximum value
	|| supported the Event Ring Segment Table Base Size registers.
	|| The maximum number of Event Ring Segment Table entries = 2^MaxERST.
	*/
	__TMS_UINT32				MaxERST;

	/* Maximum interrupters the xHc supports */
	__TMS_UINT32				MaxIntrs;

	/* Page Size Boundary */
	__TMS_UINT32				PSZ;

	/* Max Scratchpad number */
	__TMS_UINT32				MaxScratchpadBuf;

	/* UHILAUX_INCALL */
	__TMS_USBPUMP_UHILAUX_INCALL		UhilAuxInCall;

	/* page size of OS, might not be 4k, depends on OS */
	__TMS_BYTES				OsPageSize;
	/* page size of OS, might not be 4k, depends on xHc */
	__TMS_BYTES				HcPageSize;

	__TMS_UINT32				PortChangeMask;

	/* Port control in xHC */
	__TMS_UINT				fPortPowerCtl: 1;

	__TMS_UINT				fHcdStarting: 1;
	__TMS_UINT				fHcdStarted: 1;
	__TMS_UINT				fHwInitialized: 1;
	__TMS_UINT				fNoHardware: 1;

	__TMS_UINT				fSuspend:1;
	__TMS_UINT				fResume:1;
	__TMS_UINT				fSavexHcFailed: 1;
	__TMS_UINT				fRestoreFailed: 1;
	__TMS_UINT				fCommandRingAbortTimer: 1;
	/* flag for MapDmaWithRange */
	__TMS_UINT				fMapDmaRange: 1;
	/* flag to indicate power state of xHc */
	__TMS_UINT				fHcPowerDown: 1;

	__TMS_UINT				fNeedDelayAfterReset: 1;

	/* WorkAround for NEC */
	__TMS_USBPUMP_XHCD_SLOT_ID		RecoverSlotID;

	/* The number of root port is obtained at run time */
	__TMS_UINT8				NumRootPort;

	/* starting port number for USB2 and USB3 */
	__TMS_UINT8				Usb2PortNumber;
	__TMS_UINT8				Usb3PortNumber;

	/* root hub port vector */
	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT *	pvRootHubPorts[16];

	/* Operational registers */
	__TMS_UINT32				ulCmd;
	__TMS_UINT32				ulStatus;
	__TMS_UINT32				ulDNCTRL;
	__TMS_UINT32				ulCONFIG;
	__TMS_UINT64				ullDCBAAP;

	/* Runtime registers */
	__TMS_UINT32				ulIMAN;
	__TMS_UINT32				ulIMOD;
	__TMS_UINT32				ulERSTSZ;
	__TMS_UINT64				ullERSTBA;
	__TMS_UINT64				ullERDP;

	/* Timer */
	__TMS_USBPUMP_TIMER			Timer;
	__TMS_UINT32				TimerRetryCount;

	/* command ring timer: For command abort use */
	__TMS_USBPUMP_TIMER			CmdRingTimer;

	/* ASYNC IOCTL QE handling */
	__TMS_USBPUMP_IOCTL_QE *		pIoctlQeHead;
	__TMS_UCALLBACKCOMPLETION		IoctlQeEvent;

	__TMS_USBPUMP_IOCTL_QE *		pStartHcdIoctlQe;
	__TMS_USBPUMP_IOCTL_QE *		pStopHcdIoctlQe;
	__TMS_USBPUMP_IOCTL_QE *		pSuspendIoctlQe;
	__TMS_USBPUMP_IOCTL_QE *		pResumeIoctlQe;
	__TMS_USBPUMP_IOCTL_QE *		pPortCapIoctlQe;

	/* Gloabl buffer segment */
	__TMS_USBPUMP_UHILAUX_SEGMENT *		pGlobalBufSeg;

	/*
	|| LPM request handling for a roothub port
	*/
	__TMS_USBPUMP_HCD_REQUEST * 		pRootPortReq;

	/* function wake notification handling */
	__TMS_USBPUMP_HCD_REQUEST * 		pFunctionWakeRequest;

	__TMS_UINT32				ThresholdRegOffset;
	__TMS_UINT32				ThresholdRegValue;
	
	/*
	|| Frame number handling
	*/
	__TMS_UINT32				FrameNum;
	__TMS_UINT32				LastFrameNumberReturned;

	__TMS_UINT32				ulUsbdFlagMask;
	__TMS_UINT32				ulUsbdFlagBits;

	/* Information for map ports */
	__TMS_UINT8				Usb2HcPortMap[16];
	__TMS_UINT32				ulUsb2HcPortMap;
	__TMS_UINT8				Usb3HcPortMap[16];
	__TMS_UINT32				ulUsb3HcPortMap;
	};

#define __TMS_USBPUMP_HCD_XHCI_CONTENTS__UNION		\
	__TMS_USBPUMP_HCDKIT_HCD_CONTENTS__UNION;	\
	__TMS_USBPUMP_HCD_XHCI_CONTENTS	Xhci

union __TMS_UNIONNAME(USBPUMP_HCD_XHCI)
	{
	__TMS_USBPUMP_HCD_XHCI_CONTENTS__UNION;
	};

#define __TMS_USBPUMP_HCD_XHCI_NAME	__TMS_USBPUMP_HCD_NAME("xhcd")

/* sane names */
#define xhcd_CommandRing		Xhci.CmdRing
#define xhcd_EventRing			Xhci.EventRing
#define xhcd_pvTransferRing		Xhci.pvTransferRing
#define xhcd_ScratchPadBufferArray	Xhci.ScratchPadBufferArray
#define xhcd_DCBAA			Xhci.DCBAA
#define xhcd_pvDeviceContext		Xhci.pvDeviceContext
#define xhcd_Cap			Xhci.Cap
#define xhcd_Cap1			Xhci.Cap1
#define xhcd_Cap2			Xhci.Cap2
#define xhcd_Cap3			Xhci.Cap3
#define xhcd_U1Latency			Xhci.U1Latency
#define xhcd_U2Latency			Xhci.U2Latency
#define xhcd_fAC64			Xhci.fAC64
#define xhcd_fBNC			Xhci.fBNC
#define xhcd_fCSZ			Xhci.fCSZ
#define xhcd_fPIND			Xhci.fPIND
#define xhcd_fLHRC			Xhci.fLHRC
#define xhcd_fLTC			Xhci.fLTC
#define xhcd_fNSS			Xhci.fNSS
#define xhcd_fPAE			Xhci.fPAE
#define xhcd_fSet_PORTSC_CSC		Xhci.fSet_PORTSC_CSC
#define xhcd_MaxPSASize			Xhci.MaxPSASize
#define xhcd_MaxNumStreamIDHostSupport	Xhci.MaxNumStreamIDHostSupport
#define xhcd_uIST			Xhci.uIST
#define xhcd_MaxERST			Xhci.MaxERST
#define xhcd_MaxIntrs			Xhci.MaxIntrs
#define xhcd_PSZ			Xhci.PSZ
#define xhcd_MaxScratchpadBuf		Xhci.MaxScratchpadBuf
#define xhcd_pvDeviceNotification	Xhci.pvDeviceNotification
#define xhcd_SyncBlock			Xhci.SyncBlock
#define xhcd_IntSeq			Xhci.IntSeq
#define xhcd_LineEvent			Xhci.LineEvent
#define xhcd_pMessageEvents		Xhci.pMessageEvents
#define xhcd_nMessageEvents		Xhci.nMessageEvents
#define xhcd_UnknownMessageEvent	Xhci.UnknownMessageEvent
#define xhcd_UnknownMessageCode		Xhci.UnknownMessageCode
#define xhcd_ConfigInfo			Xhci.ConfigInfo
#define xhcd_hcUsbInt			Xhci.hcUsbInt
#define xhcd_IntSts			Xhci.IntSts
#define xhcd_OperationRegOffset		Xhci.OperationRegOffset
#define xhcd_RuntimeRegOffset		Xhci.RuntimeRegOffset
#define xhcd_HcCmdDoorbellReg		Xhci.HcCmdDoorbellReg
#define xhcd_xHcVersion			Xhci.xHcVersion
#define xhcd_ulWiring			Xhci.ConfigInfo.ulWiring
#define	xhcd_hBus			Xhci.ConfigInfo.hBus
#define	xhcd_IoPort			Xhci.ConfigInfo.IoPort
#define xhcd_hcUsbIntConfig		Xhci.ConfigInfo.hUsbInt
#define xhcd_MaxStreamId		Xhci.ConfigInfo.MaxStreamId
#define xhcd_MaxSlots			Xhci.ConfigInfo.MaxSlots
#define xhcd_SizeOfEventRing		Xhci.ConfigInfo.SizeOfEventRing
#define xhcd_SizeOfEventRingSeg		Xhci.ConfigInfo.SizeOfEventRingSeg
#define xhcd_IMod			Xhci.ConfigInfo.IMod
#define xhcd_HwResetTime		Xhci.ConfigInfo.HwResetTime
#define xhcd_CmdAbortTime		Xhci.ConfigInfo.CmdAbortTime
#define xhcd_CleanUpTime		Xhci.ConfigInfo.CleanUpTime
#define xhcd_pTrbAllocPolicyFn		Xhci.ConfigInfo.pTrbAllocPolicyFn
#define xhcd_RxThresholdRegOffset	Xhci.ConfigInfo.ulRxThresholdRegOffset
#define xhcd_RxThresholdRegValue	Xhci.ConfigInfo.ulRxThresholdRegValue
#define xhcd_TxThresholdRegOffset	Xhci.ConfigInfo.ulTxThresholdRegOffset
#define xhcd_TxThresholdRegValue	Xhci.ConfigInfo.ulTxThresholdRegValue
#define xhcd_SystemResumeRecoveryTime	Xhci.ConfigInfo.ulSystemResumeRecoveryTime
#define xhcd_ulMapDmaRange		Xhci.ConfigInfo.ulMapDmaRange
#define xhcd_ulMapDmaRangeTime		Xhci.ConfigInfo.ulMapDmaRangeTime
#define xhcd_ulDebugFlags		Xhci.ConfigInfo.ulDebugFlags
#define xhcd_nGlobalBufSeg		Xhci.ConfigInfo.nGlobalBufSeg
#define xhcd_OsPageSize			Xhci.OsPageSize
#define xhcd_HcPageSize			Xhci.HcPageSize
#define xhcd_fPortPowerCtl		Xhci.fPortPowerCtl
#define xhcd_PortChangeMask		Xhci.PortChangeMask
#define xhcd_fHcdStarting		Xhci.fHcdStarting
#define xhcd_fHcdStarted		Xhci.fHcdStarted
#define xhcd_fHwInitialized		Xhci.fHwInitialized
#define xhcd_fNoHardware		Xhci.fNoHardware
#define xhcd_fSuspend			Xhci.fSuspend
#define xhcd_fResume			Xhci.fResume
#define xhcd_fSavexHcFailed		Xhci.fSavexHcFailed
#define xhcd_fRestoreFailed		Xhci.fRestoreFailed
#define xhcd_fCommandRingAbortTimer	Xhci.fCommandRingAbortTimer
#define xhcd_fHcPowerDown		Xhci.fHcPowerDown
#define xhcd_RecoverSlotID		Xhci.RecoverSlotID
#define xhcd_NumRootPort		Xhci.NumRootPort
#define xhcd_Usb2PortNumber		Xhci.Usb2PortNumber
#define xhcd_Usb3PortNumber		Xhci.Usb3PortNumber
#define xhcd_pvRootHubPorts		Xhci.pvRootHubPorts
#define xhcd_ulCmd			Xhci.ulCmd
#define xhcd_ulStatus			Xhci.ulStatus
#define xhcd_ulDNCTRL			Xhci.ulDNCTRL
#define xhcd_ullDCBAAP			Xhci.ullDCBAAP
#define xhcd_ulCONFIG			Xhci.ulCONFIG
#define xhcd_ulIMAN			Xhci.ulIMAN
#define xhcd_ulIMOD			Xhci.ulIMOD
#define xhcd_ulERSTSZ			Xhci.ulERSTSZ
#define xhcd_ullERSTBA			Xhci.ullERSTBA
#define xhcd_ullERDP			Xhci.ullERDP
#define xhcd_Timer			Xhci.Timer
#define xhcd_TimerRetryCount		Xhci.TimerRetryCount
#define xhcd_pGlobalBufSeg		Xhci.pGlobalBufSeg
#define xhcd_FrameNum			Xhci.FrameNum
#define xhcd_LastFrameNumberReturned	Xhci.LastFrameNumberReturned
#define xhcd_UsbdFlagMask		Xhci.ulUsbdFlagMask
#define xhcd_UsbdFlagBits		Xhci.ulUsbdFlagBits
#define xhcd_SessionHandle		HcdKit.hUhilAux
#define xhcd_UhilAuxInCall		Xhci.UhilAuxInCall
#define xhcd_pAllocateCommonBufferFn	xhcd_UhilAuxInCall.InCall.pAllocateCommonBufferFn
#define xhcd_pAllocateBufferFn		xhcd_UhilAuxInCall.InCall.pAllocateBufferFn
#define xhcd_pFreeBufferFn		xhcd_UhilAuxInCall.InCall.pFreeBufferFn
#define xhcd_pGetPageSizeFn		xhcd_UhilAuxInCall.InCall.pGetPageSizeFn
#define xhcd_pCacheInvalidateBufferFn	xhcd_UhilAuxInCall.InCall.pCacheInvalidateBufferFn
#define xhcd_pCacheFlushBufferFn	xhcd_UhilAuxInCall.InCall.pCacheFlushBufferFn
#define xhcd_pHbufferMapPioFn		xhcd_UhilAuxInCall.InCall.pHbufferMapPioFn
#define xhcd_pHbufferUnMapPioFn		xhcd_UhilAuxInCall.InCall.pHbufferUnMapPioFn
#define xhcd_pHbufferGetSegmentCountFn	xhcd_UhilAuxInCall.InCall.pHbufferGetSegmentCountFn
#define xhcd_pHbufferMapDmaAsyncFn	xhcd_UhilAuxInCall.InCall.pHbufferMapDmaAsyncFn
#define xhcd_pHbufferUnMapDmaFn		xhcd_UhilAuxInCall.InCall.pHbufferUnMapDmaFn
#define xhcd_pHbufferGetMoreSegmentsFn	xhcd_UhilAuxInCall.InCall.pHbufferGetMoreSegmentsFn
#define xhcd_pHbufferSetRangeFn		xhcd_UhilAuxInCall.InCall.pHbufferSetRangeFn
#define xhcd_pHbufferGetMaxTransfer	xhcd_UhilAuxInCall.InCall.pHbufferGetMaxTransferFn
#define xhcd_pHbufferLogAddrFn		xhcd_UhilAuxInCall.InCall.pHbufferLogAddrFn
#define xhcd_pHbufferPhysAddrFn		xhcd_UhilAuxInCall.InCall.pHbufferPhysAddrFn
#define xhcd_pHbufferLengthFn		xhcd_UhilAuxInCall.InCall.pHbufferLengthFn
#define xhcd_pHbufferLogToDevRelPhysFn	xhcd_UhilAuxInCall.InCall.pHbufferLogToDevRelPhysFn
#define xhcd_pHbufferDevRelPhysToLogFn	xhcd_UhilAuxInCall.InCall.pHbufferDevRelPhysToLogFn

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/


/****************************************************************************\
|
|	Global Variables Reference
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_ROM __TMS_USBPUMP_HCDKIT_SWITCH gk_hub_xhcd_switch;

__TMS_END_DECLS

/****************************************************************************\
|
|	external function declaration
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_HCD_INIT_FN			   UsbPumpXhcd_Init;
__TMS_USBPUMP_HCDKIT_SWITCH_IOCTL_FN		   UsbPumpXhcd_Ioctl;
__TMS_USBPUMP_HCDKIT_SWITCH_SUBMIT_REQUEST_FN	   UsbPumpXhcd_SubmitRequest;
__TMS_UHIL_MP_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN UsbPumpXhcd_Mp_PrimaryIsr_Imsr;
__TMS_UHIL_MP_LINE_INTERRUPT_SERVICE_ROUTINE_FN	   UsbPumpXhcd_Mp_PrimaryIsr_Line;
__TMS_USBPUMP_HCD_REQUEST_CANCEL_FN		   UsbPumpXhcd_CancelActiveRequest;
__TMS_USBPUMP_HCD_REQUEST_CANCEL_FN		   UsbPumpXhcd_CancelPendingRequest;
__TMS_USBPUMP_HCD_REQUEST_CANCEL_FN		   UsbPumpXhcd_CancelMappingRequest;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_LPM_REQUEST_FN	   UsbPumpXhcdI_RootHubLpmRequest;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_STATUS_FN UsbPumpXhcdI_vRootGetPortStatus;

__TMS_END_DECLS

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_XHCD_GET_INPUT_CONTROL_CONTEXT(pInput)	\
   __TMS_USBPUMP_XHCD_GET_INPUT_CONTROL_CONTEXT(pInput)
# define USBPUMP_XHCD_GET_SLOT_CONTEXT_FROM_INPUT(pInput)	\
   __TMS_USBPUMP_XHCD_GET_SLOT_CONTEXT_FROM_INPUT(pInput)
# define USBPUMP_XHCD_GET_ENDPOINT_CONTEXT_FROM_INPUT(pInput, EpID)	\
   __TMS_USBPUMP_XHCD_GET_ENDPOINT_CONTEXT_FROM_INPUT(pInput, EpID)
# define USBPUMP_XHCD_GET_SLOT_CONTEXT(pDevCtx)	\
   __TMS_USBPUMP_XHCD_GET_SLOT_CONTEXT(pDevCtx)
# define USBPUMP_XHCD_GET_ENDPOINT_CONTEXT(pDevCtx, EpID)	\
   __TMS_USBPUMP_XHCD_GET_ENDPOINT_CONTEXT(pDevCtx, EpID)
# define USBPUMP_XHCD_BUFFER_SEGMENT_REQUIRED_SIZE(pO, n)	\
   __TMS_USBPUMP_XHCD_BUFFER_SEGMENT_REQUIRED_SIZE(pO, n)
# define USBPUMP_XHCD_BUFFER_SEGMENT_SIZEOF(pO)	\
   __TMS_USBPUMP_XHCD_BUFFER_SEGMENT_SIZEOF(pO)
# define USBPUMP_XHCD_CONFIG_INFO_INIT_V10(	\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_pHcdKitSwitch,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_nGlobalBufSeg,			\
		a_ulDebugFlags				\
		)	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V10(	\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_pHcdKitSwitch,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_nGlobalBufSeg,			\
		a_ulDebugFlags				\
		)
# define USBPUMP_XHCD_CONFIG_INFO_INIT_V9(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_nGlobalBufSeg,			\
		a_ulDebugFlags				\
		)	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V9(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_nGlobalBufSeg,			\
		a_ulDebugFlags				\
		)
# define USBPUMP_XHCD_CONFIG_INFO_INIT_V8(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_ulDebugFlags				\
		)	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V8(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_ulDebugFlags				\
		)
# define USBPUMP_XHCD_CONFIG_INFO_INIT_V7(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_pUsb2HcPortMap,			\
		a_ulUsb2HcPortMap,			\
		a_pUsb3HcPortMap,			\
		a_ulUsb3HcPortMap,			\
		a_ulDebugFlags				\
		)	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V7(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_CmdAbortTime,				\
		a_CleanUpTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_pUsb2HcPortMap,			\
		a_ulUsb2HcPortMap,			\
		a_pUsb3HcPortMap,			\
		a_ulUsb3HcPortMap,			\
		a_ulDebugFlags				\
		)
# define USBPUMP_XHCD_CONFIG_INFO_INIT_V6(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_pUsb2HcPortMap,			\
		a_ulUsb2HcPortMap,			\
		a_pUsb3HcPortMap,			\
		a_ulUsb3HcPortMap,			\
		a_ulDebugFlags				\
		)	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V6(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_HwResetTime,				\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_pUsb2HcPortMap,			\
		a_ulUsb2HcPortMap,			\
		a_pUsb3HcPortMap,			\
		a_ulUsb3HcPortMap,			\
		a_ulDebugFlags				\
		)
# define USBPUMP_XHCD_CONFIG_INFO_INIT_V5(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_ulDebugFlags				\
		)	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V5(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxStreamId,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_ulDebugFlags				\
		)
# define USBPUMP_XHCD_CONFIG_INFO_INIT_V4(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_ulDebugFlags				\
		)	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V4(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulMapDmaRange,			\
		a_ulMapDmaRangeTime,			\
		a_ulDebugFlags				\
		)
# define USBPUMP_XHCD_CONFIG_INFO_INIT_V3(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulDebugFlags				\
		)	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V3(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulDebugFlags				\
		)
# define USBPUMP_XHCD_CONFIG_INFO_INIT_V2(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulDebugFlags				\
		)	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V2(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulDebugFlags				\
		)
# define USBPUMP_XHCD_CONFIG_INFO_INIT_V1(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_SizeOfEventRing,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulDebugFlags				\
		)	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_INIT_V1(		\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_SizeOfEventRing,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulDebugFlags				\
		)
# define USBPUMP_XHCD_CONFIG_INFO_SETUP_V10(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_pHcdKitSwitch,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_nGlobalBufSeg,				\
		a_ulDebugFlags					\
                )	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V10(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_pHcdKitSwitch,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_nGlobalBufSeg,				\
		a_ulDebugFlags					\
                )
# define USBPUMP_XHCD_CONFIG_INFO_SETUP_V9(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_nGlobalBufSeg,				\
		a_ulDebugFlags					\
                )	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V9(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_nGlobalBufSeg,				\
		a_ulDebugFlags					\
                )
# define USBPUMP_XHCD_CONFIG_INFO_SETUP_V8(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_ulDebugFlags					\
                )	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V8(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_ulDebugFlags					\
                )
# define USBPUMP_XHCD_CONFIG_INFO_SETUP_V7(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_pUsb2HcPortMap,				\
		a_ulUsb2HcPortMap,				\
		a_pUsb3HcPortMap,				\
		a_ulUsb3HcPortMap,				\
		a_ulDebugFlags					\
                )	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V7(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
                a_CmdAbortTime,					\
		a_CleanUpTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_pUsb2HcPortMap,				\
		a_ulUsb2HcPortMap,				\
		a_pUsb3HcPortMap,				\
		a_ulUsb3HcPortMap,				\
		a_ulDebugFlags					\
                )
# define USBPUMP_XHCD_CONFIG_INFO_SETUP_V6(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_pUsb2HcPortMap,				\
		a_ulUsb2HcPortMap,				\
		a_pUsb3HcPortMap,				\
		a_ulUsb3HcPortMap,				\
		a_ulDebugFlags					\
                )	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V6(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
                a_HwResetTime,					\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_pUsb2HcPortMap,				\
		a_ulUsb2HcPortMap,				\
		a_pUsb3HcPortMap,				\
		a_ulUsb3HcPortMap,				\
		a_ulDebugFlags					\
                )
# define USBPUMP_XHCD_CONFIG_INFO_SETUP_V5(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_ulDebugFlags					\
                )	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V5(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxStreamId,					\
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_ulDebugFlags					\
                )
# define USBPUMP_XHCD_CONFIG_INFO_SETUP_V4(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_ulDebugFlags					\
                )	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V4(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulMapDmaRange,				\
		a_ulMapDmaRangeTime,				\
		a_ulDebugFlags					\
                )
# define USBPUMP_XHCD_CONFIG_INFO_SETUP_V3(	\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulDebugFlags				\
		)	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V3(	\
		a_ulWiring,				\
		a_hBus,					\
		a_IoPort,				\
		a_hUsbInt,				\
		a_MaxSlots,				\
		a_nBufSeg,				\
		a_SizeOfEventRing,			\
		a_SizeOfEventRingSeg,			\
		a_IMod,					\
		a_pTrbAllocPolicyFn,			\
		a_ulRxThresholdRegOffset,		\
		a_ulRxThresholdRegValue,		\
		a_ulTxThresholdRegOffset,		\
		a_ulTxThresholdRegValue,		\
		a_ulSystemResumeRecoveryTime,		\
		a_ulDebugFlags				\
		)
# define USBPUMP_XHCD_CONFIG_INFO_SETUP_V2(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulDebugFlags					\
                )	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V2(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxSlots,                                     \
                a_nBufSeg,					\
                a_SizeOfEventRing,                              \
                a_SizeOfEventRingSeg,				\
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulTxThresholdRegOffset,			\
		a_ulTxThresholdRegValue,			\
		a_ulSystemResumeRecoveryTime,			\
		a_ulDebugFlags					\
                )
# define USBPUMP_XHCD_CONFIG_INFO_SETUP_V1(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxSlots,                                     \
                a_SizeOfEventRing,                              \
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulDebugFlags					\
                )	\
	__TMS_USBPUMP_XHCD_CONFIG_INFO_SETUP_V1(                \
                a_pConfig,                                      \
                a_ulWiring,					\
                a_hBus,                                         \
                a_IoPort,                                       \
                a_hUsbInt,                                      \
                a_MaxSlots,                                     \
                a_SizeOfEventRing,                              \
                a_IMod,						\
		a_pTrbAllocPolicyFn,				\
		a_ulRxThresholdRegOffset,			\
		a_ulRxThresholdRegValue,			\
		a_ulDebugFlags					\
                )
# define USBPUMP_XHCD_WIRING_NOT_DISABLE_IE	\
   __TMS_USBPUMP_XHCD_WIRING_NOT_DISABLE_IE
# define USBPUMP_XHCD_WIRING_SET_RX_REGISTER	\
   __TMS_USBPUMP_XHCD_WIRING_SET_RX_REGISTER
# define USBPUMP_XHCD_WIRING_HUB_WORKAROUND_ENABLE	\
   __TMS_USBPUMP_XHCD_WIRING_HUB_WORKAROUND_ENABLE
# define USBPUMP_XHCD_WIRING_SET_ALT_PORT_POWER	\
   __TMS_USBPUMP_XHCD_WIRING_SET_ALT_PORT_POWER
# define USBPUMP_XHCD_WIRING_CLEAR_P3	\
   __TMS_USBPUMP_XHCD_WIRING_CLEAR_P3
# define USBPUMP_XHCD_WIRING_SET_TX_REGISTER	\
   __TMS_USBPUMP_XHCD_WIRING_SET_TX_REGISTER
# define USBPUMP_XHCD_WIRING_PORT_SPEED_WORKAROUND	\
   __TMS_USBPUMP_XHCD_WIRING_PORT_SPEED_WORKAROUND
# define USBPUMP_XHCD_WIRING_DISABLE_USB2_SUSPEND	\
   __TMS_USBPUMP_XHCD_WIRING_DISABLE_USB2_SUSPEND
# define USBPUMP_XHCD_WIRING_SET_MAPDMA_RANGE	\
   __TMS_USBPUMP_XHCD_WIRING_SET_MAPDMA_RANGE
# define USBPUMP_XHCD_WIRING_SET_ALT_PORT_WAKE_MASK	\
   __TMS_USBPUMP_XHCD_WIRING_SET_ALT_PORT_WAKE_MASK
# define USBPUMP_XHCD_WIRING_SET_P3	\
   __TMS_USBPUMP_XHCD_WIRING_SET_P3
# define USBPUMP_XHCD_WIRING_USE_PORT_MAPMPING_INFO	\
   __TMS_USBPUMP_XHCD_WIRING_USE_PORT_MAPMPING_INFO
# define USBPUMP_XHCD_WIRING_DISABLE_WAKE_MASK	\
   __TMS_USBPUMP_XHCD_WIRING_DISABLE_WAKE_MASK
# define USBPUMP_XHCD_WIRING_LIMIT_BINTERNAL	\
   __TMS_USBPUMP_XHCD_WIRING_LIMIT_BINTERNAL
# define USBPUMP_XHCD_WIRING_DISABLE_SAVE_RESTORE	\
   __TMS_USBPUMP_XHCD_WIRING_DISABLE_SAVE_RESTORE
# define USBPUMP_XHCD_WIRING_PORT_OFF_WHEN_SHUT_DOWN	\
   __TMS_USBPUMP_XHCD_WIRING_PORT_OFF_WHEN_SHUT_DOWN
# define USBPUMP_XHCD_WIRING_SET_MAX_BURST	\
   __TMS_USBPUMP_XHCD_WIRING_SET_MAX_BURST
# define USBPUMP_XHCD_WIRING_DUMMY_SHORT_TRANSFER	\
   __TMS_USBPUMP_XHCD_WIRING_DUMMY_SHORT_TRANSFER
# define USBPUMP_XHCD_WIRING_REPORT_COMPATIBLE_PORT	\
   __TMS_USBPUMP_XHCD_WIRING_REPORT_COMPATIBLE_PORT
# define USBPUMP_HCD_XHCI_CONTENTS__UNION	\
   __TMS_USBPUMP_HCD_XHCI_CONTENTS__UNION
# define USBPUMP_HCD_XHCI_NAME	\
   __TMS_USBPUMP_HCD_XHCI_NAME
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

#endif /* _USBPUMP_XHCD_H_ */
