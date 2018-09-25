/* usbpump_device_xdcd.h	Mon Jan 08 2018 11:43:00 chwon */

/*

Module:  usbpump_device_xdcd.h

Function:
	USBPUMP_DEVICE_XDCD and related types.

Version:
	V3.21e	Mon Jan 08 2018 11:43:00 chwon	Edit level 12

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
   3.13a  Thu Jul 10 2014 16:50:59  tmm
	18309: Module created.

   3.13b  Fri Aug 15 2014 13:11:39  chwon
	18461: Remove udevxdcd_hUhilAux from USBPUMP_DEVICE_XDCD
	and use UDEVICE::udev_hUhilAux instead.

   3.15d  Mon Jan 11 2016 12:10:53  chwon
	19722: Add udevxdcd_fSuspended in the context and change name from
	udevxdcd_bCurSpeed to udevxdcd_CurSpeed.

   3.17a  Wed Feb 10 2016 10:47:05  chwon
	19722: 19843: Remove uFrameNum and add pStopIoctlQe & fDeviceStopping
	in the context.

   3.17a  Mon Feb 15 2016 14:58:05  chwon
	19642: Add uFrameNumber in the context

   3.17a  Thu Mar 17 2016 11:59:10  chwon
	19950: Add udevxdcd_nActiveIsoEndpoint and udevxdcd_fU1Enabled in the
	USBPUMP_DEVICE_XDCD.

   3.17a  Mon Mar 28 2016 12:36:59  chwon
	20024: Add udevxdcd_fU2Enabled in the USBPUMP_DEVICE_XDCD.

   3.17a  Fri Apr 29 2016 12:21:59  chwon
	19950: Add UxTimer and related flags in the USBPUMP_DEVICE_XDCD.

   3.21a  Thu May 04 2017 17:33:34  chwon
	20312: Add Usb2PhyTimer and related valriables in the context.

   3.21b  Thu Jul 20 2017 09:11:31  chwon
	20620: Add udevxdcd_fUsb31Core in the context.

   3.21c  Mon Oct 23 2017 19:28:23  chwon
	20694: Add udevxdcd_fEnableDevInitUx in the context.

   3.21e  Mon Jan 08 2018 11:43:00  chwon
	20704: Add udevxdcd_fIsoInWorkAround in the context.

*/

#ifndef _USBPUMP_DEVICE_XDCD_H_		/* prevent multiple includes */
#define _USBPUMP_DEVICE_XDCD_H_

#ifndef _XDCDKERN_TYPES_H_
# include "xdcdkern_types.h"
#endif

#ifndef _UDEVICE_H_
# include "udevice.h"
#endif

#ifndef _USBPUMP_XDCD_CONFIGURATION_INFO_H_
# include "usbpump_xdcd_configuration_info.h"
#endif

#ifndef _USBPUMP_UHILAUX_H_
# include "usbpump_uhilaux.h"
#endif

#ifndef _UHILINTSYS_H_
# include "uhilintsys.h"
#endif

#ifndef _USBIOCTL_DEVICE_H_
# include "usbioctl_device.h"
#endif

/****************************************************************************\
|
|	Some macros for doing hardware-ish things
|
\****************************************************************************/

#define __TMS_USBPUMP_XDCD_ID			0x55332000
#define __TMS_USBPUMP_XDCD_ID_MASK		0xFFFFF000

struct __TMS_STRUCTNAME	(USBPUMP_XDCD_CORE_STATUS)
	{
	__TMS_UINT32				gsbuscfg0;
	__TMS_UINT32				gsbuscfg1;
	__TMS_UINT32				gtxfifosiz0;
	__TMS_UINT32				gtxfifosiz1;
	__TMS_UINT32				gtxfifosiz2;
	__TMS_UINT32				gtxfifosiz3;
	__TMS_UINT32				grxfifosiz0;
	__TMS_UINT32				dcfg;
	__TMS_UINT32				dctl;
	__TMS_UINT32				devten;
	};

/****************************************************************************\
|
|	If we're debugging hairy problems, we may want to generate a
|	XDCD I/O trace.  The structures in this section are used to
|	compose such a trace.
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XDCD_IOTRACE)
    	{
	__TMS_UINT32	Datum;		/* what we wrote (or read) */
	__TMS_UINT16	Reg;		/* the offset to the register */
	__TMS_UINT16	Flags;		/* the frame number and a read/write
					|| indication.
					*/
	};

/****************************************************************************\
|
|	Definitions of the Event Buffer
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XDCD_EVENT_BUFFER)
	{
	/* Event buffer size in bytes */
	__TMS_BYTES				Size;

	/* Buffer handle of event buffer */
	__TMS_USBPUMP_BUFFER_HANDLE		hBuffer;

	/* Physical address of event buffer */
	__TMS_PHYSADDR				PhyAddr;

	/* Logical address of event buffer */
	__TMS_UINT32 *				pLogical;

	/* Current event address */
	__TMS_UINT32 *				pCurrEvent;

	/* Last event address */
	__TMS_UINT32 *				pLastEvent;
	};

/****************************************************************************\
|
|	Definition of the Message Event
|
\****************************************************************************/

struct __TMS_STRUCTNAME	(USBPUMP_XDCD_MESSAGE_EVENT)
	{
	__TMS_UCALLBACKCOMPLETION		Completion;
	__TMS_VOID *				pEventContext;
	};


/****************************************************************************\
|
|	Next, we define the "USBPUMP_DEVICE_XDCD" structure.  This structure
|	represents an instance of the XDCI USB if in a given system.
|	(It's true that there can be only one; but this architecture
|	allows us to support additional USB interfaces if needed.)
|	It contains all the information that is required to run the chip,
|	and pointers to the information that depends on how the chip is
|	configured.  This is a polymorphic structure; in this header file,
|	we only define the "constant" part.  The additional information
|	is provided by USBRC, after parsing the device descriptors.
|
\****************************************************************************/


/*

Type:	USBPUMP_DEVICE_XDCD

Function:
	Instance object for any chip driver for an XDCI USB device port.

Description:
	USBRC defines a structure that is based on this abstract type;
	this structure is therefore the header for the application-
	specific structure.

Contents:
	See the header file.

Notes:
	For historical reasons, as usual for a UDEVICE-variant, we have a
	number of macros that transform the names. Mostly this was done to
	cater to pre-ANSI compilers that didn't maintain unique name-spaces
	for each structure.

See Also:
	UDEVICE.

*/

struct __TMS_STRUCTNAME(USBPUMP_XDCD_HDR)
	{
	__TMS_UDEVICE_HDR;

	/*
	|| For portability, we have a bus handle (identifying the hardware
	|| 'path' to the device, and an I/O port, identifying the base address
	|| of the device.
	*/
	__TMS_UHIL_BUSHANDLE		udevxdcdhh_Bus;
	__TMS_IOPORT			udevxdcdhh_IoPort;

	/*
	|| On a given system, there may be things we need to know from
	|| the configuration.  That's stored in the config-info entry.
	*/
	__TMS_USBPUMP_XDCD_CONFIGURATION_INFO
					udevxdcdhh_ConfigInfo;

	/*
	|| DCD timer
	*/
	__TMS_USBPUMP_TIMER		udevxdcdhh_Timer;
	__TMS_UINT32			udevxdcdhh_ulEpTimerMask;
	__TMS_USBPUMP_XDCD_ADD_EPTIMER_CLIENT_FN *
					udevxdcdhh_pAddEpTimerClientFn;
	__TMS_USBPUMP_XDCD_DELETE_EPTIMER_CLIENT_FN *
					udevxdcdhh_pDeleteEpTimerClientFn;

	__TMS_USBPUMP_XDCD_COPY_BUFFER_FN *
					udevxdcdhh_pCopyRxBufferFn;
	__TMS_USBPUMP_XDCD_COPY_BUFFER_FN *
					udevxdcdhh_pCopyTxBufferFn;

	/*
	|| IOCTL parent running in OTG mode
	*/
	__TMS_USBPUMP_OBJECT_HEADER *	udevxdcdhh_pUsbPhy;

	/*
	|| A synchronization block is used to communicate from the
	|| primary ISR to the secondary ISR
	*/
	__TMS_UPLATFORM_SYNCHRONIZATION_BLOCK
					udevxdcdhh_UsbIsrSynchBlock;

	/*
	|| Interrupt handle
	*/
	__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE
					udevxdcdhh_hcUsbInt;

	/*
	|| The following is incremented on each interrupt; it allows
	|| the background to detect when an operation is interrupted.
	*/
	__TMS_VOLATILE __TMS_UINT	udevxdcdhh_IntSeq;

	/*
	|| Line-based interrupt event callback is used to communicate
	|| from the primary ISR to the secondary ISR in multi-processor
	*/
	__TMS_UCALLBACKCOMPLETION	udevxdcdhh_LineEvent;

	/*
	|| Message-based interrupt event callback is used to communicate
	|| from the primary ISR to the secondary ISR in multi-processor
	*/
	__TMS_USBPUMP_XDCD_MESSAGE_EVENT *
					udevxdcdhh_pMessageEvents;
	__TMS_BYTES			udevxdcdhh_nMessageEvents;

	/*
	|| UHILAUX
	*/
	__TMS_USBPUMP_UHILAUX_INCALL 	udevxdcdhh_UhilAuxInCall;

	/*
	|| DCD Asynch IOCTL handling from the upper layer
	*/
	__TMS_USBPUMP_IOCTL_QE * 	udevxdcdhh_pIoctlQeHead;
	__TMS_USBPUMP_IOCTL_QE * 	udevxdcdhh_pPendingIoctlQeHead;
	__TMS_UCALLBACKCOMPLETION	udevxdcdhh_IoctlQeEvent;
	__TMS_USBPUMP_IOCTL_QE *	udevxdcdhh_pStopIoctlQe;

	/*
	|| Fifo configuration
	*/
	__TMS_UINT32			udevxdcdhh_TotalFifoSize;
	__TMS_UINT32			udevxdcdhh_RxFifoSize;

	/*
	|| Hardware specific parameters set in XDCD coreConsultant
	*/
	__TMS_UINT32			udevxdcdhh_HwParams0;
	__TMS_UINT32			udevxdcdhh_HwParams1;
	__TMS_UINT32			udevxdcdhh_HwParams2;
	__TMS_UINT32			udevxdcdhh_HwParams3;
	__TMS_UINT32			udevxdcdhh_HwParams4;
	__TMS_UINT32			udevxdcdhh_HwParams5;
	__TMS_UINT32			udevxdcdhh_HwParams6;
	__TMS_UINT32			udevxdcdhh_HwParams7;
	__TMS_UINT32			udevxdcdhh_HwParams8;
	__TMS_UINT32			udevxdcdhh_NumEps;
	__TMS_UINT32			udevxdcdhh_NumInEps;
	__TMS_UINT32			udevxdcdhh_nMaxTRB;
	__TMS_UINT32			udevxdcdhh_MDataBusWidth;
	__TMS_UINT32			udevxdcdhh_TxFifoDepth;
	__TMS_UINT32			udevxdcdhh_fIsoSupport;
	__TMS_UINT32			udevxdcdhh_fBcSupport;
	__TMS_UINT32			udevxdcdhh_fAdpSupport;
	__TMS_UINT32			udevxdcdhh_CurSpeed;
	__TMS_USBPUMP_XDCD_EVENT_BUFFER	udevxdcdhh_EventBuffer;

	/* ScratchPad buffer */
	__TMS_UINT32			udevxdcdhh_nScratchPadBuffer;
	__TMS_USBPUMP_BUFFER_HANDLE	udevxdcdhh_hScratchPadBuffers;
	__TMS_PHYSADDR			udevxdcdhh_ScratchPadBuffersPhysAddr;
	__TMS_UINT32			udevxdcdhh_pScratchPadBuffers;

	__TMS_UINT16			udevxdcdhh_nActiveIsoEndpoint;
	__TMS_UINT16			udevxdcdhh_TimerState;
	__TMS_UINT16			udevxdcdhh_PowerMode;
	__TMS_UINT16			udevxdcdhh_RestoreMode;
	__TMS_USBPUMP_XDCD_CORE_STATUS	udevxdcdhh_CoreRegisters;

	/*
	|| Miscellaneous
	*/
	__TMS_UINT32			udevxdcdhh_Ep0State;
	__TMS_UINT32			udevxdcdhh_LinkState;
	__TMS_UINT32			udevxdcdhh_OldLinkState;

	/*
	|| I/O trace handling
	*/
#if __TMS_WANT_XDCD_IO_TRACE
	__TMS_VOLATILE __TMS_BOOL	udevxdcdhh_fIoTraceLock;
	__TMS_PUSBPUMP_XDCD_IOTRACE	udevxdcdhh_pIoTraceBase;
	__TMS_PUSBPUMP_XDCD_IOTRACE	udevxdcdhh_pIoTraceTop;
	__TMS_PUSBPUMP_XDCD_IOTRACE	udevxdcdhh_pIoTracePtr;
#endif

	/* timer to enable/disable U1 & U2 for ITP */
	__TMS_USBPUMP_TIMER		udevxdcdhh_UxTimer;

	/* timer to restore PHY suspend */
	__TMS_USBPUMP_TIMER		udevxdcdhh_Usb2PhyTimer;
	__TMS_UINT32			udevxdcdhh_Usb2PhySuspend;

	/* DCD configuration flags: RO */
	__TMS_UINT			udevxdcdhh_fUhilAuxCached: 1;

	/* XDCI hardware version */
	__TMS_UINT			udevxdcdhh_fUsb31Core: 1;

	/* DCD internal flags: RW */
	__TMS_UINT			udevxdcdhh_fHwInitialized: 1;
	__TMS_UINT			udevxdcdhh_fHwStarted: 1;
	__TMS_UINT			udevxdcdhh_fTimerRun: 1;
	__TMS_UINT			udevxdcdhh_fHibernated: 1;
	__TMS_UINT			udevxdcdhh_fHibernating: 1;
	__TMS_UINT			udevxdcdhh_fExitFromHiberation: 1;
	__TMS_UINT			udevxdcdhh_fDisconnected: 1;
	__TMS_UINT			udevxdcdhh_fStartConfigIssued: 1;
	__TMS_UINT			udevxdcdhh_fWakeEnabled: 1;
	__TMS_UINT			udevxdcdhh_fHibernationRequested: 1;
	__TMS_UINT			udevxdcdhh_fHibernationWaitConnect: 1;
	__TMS_UINT			udevxdcdhh_fHiberWaitU0: 1;
	__TMS_UINT			udevxdcdhh_fWakeupReady: 1;
	__TMS_UINT			udevxdcdhh_fEpsEnabled: 1;
	__TMS_UINT			udevxdcdhh_fEnumDone: 1;
	__TMS_UINT			udevxdcdhh_fDeviceStopPending: 1;
	__TMS_UINT			udevxdcdhh_fSuspended: 1;
	__TMS_UINT			udevxdcdhh_fDeviceStopping: 1;
	__TMS_UINT			udevxdcdhh_fU1Enabled: 1;
	__TMS_UINT			udevxdcdhh_fU2Enabled: 1;
	__TMS_UINT			udevxdcdhh_fEnableDevInitUx: 1;
	__TMS_UINT			udevxdcdhh_fUxTimerRunning: 1;
	__TMS_UINT			udevxdcdhh_fUxTimerEnable: 1;
	__TMS_UINT			udevxdcdhh_fEp0TwoStage: 1;
	__TMS_UINT			udevxdcdhh_fUsb2PhyTimerRun: 1;
	__TMS_UINT			udevxdcdhh_fWakeUsb2PhySuspend: 1;
	__TMS_UINT			udevxdcdhh_fIsoInWorkAround: 1;

	__TMS_UCALLBACKCOMPLETION	udevxdcdhh_CompleteEvent;
	__TMS_PUBUFQE			udevxdcdhh_pCompleteQe;

	__TMS_USBPUMP_DEVICE_POWER_REQUEST_FN *
					udevxdcdhh_pPowerRequestFn;
	__TMS_VOID *			udevxdcdhh_pPowerRequestCtx;

	__TMS_UCALLBACKCOMPLETION *	udevxdcdhh_pSofCallback;
	__TMS_BOOL			udevxdcdhh_fMicroFrameCallback;

	__TMS_UINT32			udevxdcdhh_uFrameNumber;
	};

/**** the union contents ****/
#define	__TMS_USBPUMP_XDCD_CONTENTS \
	__TMS_UDEVICE_CONTENTS; \
	struct __TMS_USBPUMP_XDCD_HDR udevhdr_udevxdcd

union __TMS_UNIONNAME(USBPUMP_XDCD_UNION)
	{
	__TMS_USBPUMP_XDCD_CONTENTS;
	};

/**** the macro to embed in derived structures ****/
#define	__TMS_USBPUMP_XDCD_HDR	\
	union __TMS_UNIONNAME(USBPUMP_XDCD_UNION) udev_hdr

/**** the generic structure ****/
struct __TMS_STRUCTNAME(USBPUMP_DEVICE_XDCD)
	{
	__TMS_USBPUMP_XDCD_HDR;
	};

/**** sane names ****/
#define	udevxdcd_IoPort			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_IoPort
#define	udevxdcd_Bus			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_Bus
#define	udevxdcd_ConfigInfo			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_ConfigInfo
#define	udevxdcd_Timer			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_Timer
#define udevxdcd_ulEpTimerMask		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_ulEpTimerMask
#define udevxdcd_pAddEpTimerClientFn	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pAddEpTimerClientFn
#define udevxdcd_pDeleteEpTimerClientFn	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pDeleteEpTimerClientFn
#define	udevxdcd_UsbIsrSynchBlock		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UsbIsrSynchBlock
#define	udevxdcd_hcUsbInt 			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_hcUsbInt
#define	udevxdcd_TotalFifoSize		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_TotalFifoSize
#define	udevxdcd_RxFifoSize			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_RxFifoSize
#define	udevxdcd_pUsbPhy			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pUsbPhy
#define	udevxdcd_IntSeq			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_IntSeq
#define	udevxdcd_Ep0State			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_Ep0State
#define	udevxdcd_fEp0TwoStage			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fEp0TwoStage
#define	udevxdcd_LinkState			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_LinkState
#define	udevxdcd_OldLinkState			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_OldLinkState
#define	udevxdcd_LineEvent			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_LineEvent
#define	udevxdcd_nMessageEvents		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_nMessageEvents
#define	udevxdcd_pMessageEvents		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pMessageEvents
#define	udevxdcd_hUhilAux			\
	udev_hUhilAux
#define	udevxdcd_UhilAuxInCall		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall
#define	udevxdcd_pIoctlQeHead		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pIoctlQeHead
#define	udevxdcd_pPendingIoctlQeHead		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pPendingIoctlQeHead
#define	udevxdcd_IoctlQeEvent		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_IoctlQeEvent
#define	udevxdcd_pStopIoctlQe			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pStopIoctlQe
#define	udevxdcd_HwParams0			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_HwParams0
#define	udevxdcd_HwParams1			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_HwParams1
#define	udevxdcd_HwParams2			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_HwParams2
#define	udevxdcd_HwParams3			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_HwParams3
#define	udevxdcd_HwParams4			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_HwParams4
#define	udevxdcd_HwParams5			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_HwParams5
#define	udevxdcd_HwParams6			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_HwParams6
#define	udevxdcd_HwParams7			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_HwParams7
#define	udevxdcd_HwParams8			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_HwParams8
#define	udevxdcd_NumEps			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_NumEps
#define	udevxdcd_NumInEps			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_NumInEps
#define	udevxdcd_nMaxTRB			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_nMaxTRB
#define	udevxdcd_MDataBusWidth		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_MDataBusWidth
#define	udevxdcd_TxFifoDepth		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_TxFifoDepth
#define	udevxdcd_fIsoSupport		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fIsoSupport
#define	udevxdcd_fBcSupport			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fBcSupport
#define	udevxdcd_fAdpSupport		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fAdpSupport
#define	udevxdcd_CurSpeed			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_CurSpeed
#define	udevxdcd_EventBuffer		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_EventBuffer
#define	udevxdcd_nScratchPadBuffer		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_nScratchPadBuffer
#define	udevxdcd_hScratchPadBuffers		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_hScratchPadBuffers
#define	udevxdcd_ScratchPadBuffersPhysAddr	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_ScratchPadBuffersPhysAddr
#define	udevxdcd_pScratchPadBuffers		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pScratchPadBuffers
#define	udevxdcd_nActiveIsoEndpoint		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_nActiveIsoEndpoint
#define	udevxdcd_TimerState			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_TimerState
#define	udevxdcd_PowerMode			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_PowerMode
#define	udevxdcd_UxTimer		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UxTimer
#define	udevxdcd_Usb2PhyTimer		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_Usb2PhyTimer
#define	udevxdcd_Usb2PhySuspend		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_Usb2PhySuspend
#define	udevxdcd_fUsb2PhyTimerRun		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fUsb2PhyTimerRun
#define	udevxdcd_fWakeUsb2PhySuspend		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fWakeUsb2PhySuspend
#define	udevxdcd_fEnableDevInitUx		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fEnableDevInitUx
#define	udevxdcd_fUxTimerRunning		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fUxTimerRunning
#define	udevxdcd_fUxTimerEnable		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fUxTimerEnable
#define	udevxdcd_fHibernated		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fHibernated
#define	udevxdcd_fHibernating		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fHibernating
#define	udevxdcd_fExitFromHiberation		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fExitFromHiberation
#define	udevxdcd_fDisconnected		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fDisconnected
#define	udevxdcd_fStartConfigIssued		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fStartConfigIssued
#define	udevxdcd_fWakeEnabled		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fWakeEnabled
#define	udevxdcd_fHibernationRequested		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fHibernationRequested
#define	udevxdcd_fHibernationWaitConnect		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fHibernationWaitConnect
#define	udevxdcd_fHiberWaitU0		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fHiberWaitU0
#define	udevxdcd_fWakeupReady		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fWakeupReady
#define	udevxdcd_fEpsEnabled		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fEpsEnabled
#define	udevxdcd_fEnumDone		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fEnumDone
#define	udevxdcd_fDeviceStopPending		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fDeviceStopPending
#define	udevxdcd_fSuspended		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fSuspended
#define	udevxdcd_fDeviceStopping		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fDeviceStopping
#define	udevxdcd_fU1Enabled		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fU1Enabled
#define	udevxdcd_fU2Enabled		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fU2Enabled
#define	udevxdcd_RestoreMode		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_RestoreMode
#define	udevxdcd_CoreRegisters		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_CoreRegisters
#define udevxdcd_pCopyRxBufferFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pCopyRxBufferFn
#define udevxdcd_pCopyTxBufferFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pCopyTxBufferFn
#define	udevxdcd_fHwInitialized		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fHwInitialized
#define	udevxdcd_fHwStarted			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fHwStarted
#define	udevxdcd_fTimerRun			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fTimerRun
#define udevxdcd_fUhilAuxCached		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fUhilAuxCached
#define udevxdcd_fUsb31Core		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fUsb31Core
#define udevxdcd_fIsoInWorkAround	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fIsoInWorkAround
#define	udevxdcd_DmaBurstSize		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_ConfigInfo.DmaBurstSize
#define	udevxdcd_nEvents		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_ConfigInfo.nEvents
#define	udevxdcd_Alignment			\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_ConfigInfo.Alignment

#define udevxdcd_CompleteEvent		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_CompleteEvent
#define udevxdcd_pCompleteQe		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pCompleteQe
#define udevxdcd_pPowerRequestFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pPowerRequestFn
#define udevxdcd_pPowerRequestCtx		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pPowerRequestCtx
#define udevxdcd_pSofCallback		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pSofCallback
#define udevxdcd_fMicroFrameCallback		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fMicroFrameCallback
#define udevxdcd_uFrameNumber		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_uFrameNumber

#if __TMS_WANT_XDCD_IO_TRACE
# define udevxdcd_fIoTraceLock		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_fIoTraceLock
# define udevxdcd_pIoTraceBase		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pIoTraceBase
# define udevxdcd_pIoTraceTop		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pIoTraceTop
# define udevxdcd_pIoTracePtr		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_pIoTracePtr
#endif

/* sane names */
#define udevxdcd_pAllocateCommonBufferFn	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pAllocateCommonBufferFn
#define udevxdcd_pAllocateBufferFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pAllocateBufferFn
#define udevxdcd_pFreeBufferFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pFreeBufferFn
#define udevxdcd_pGetPageSizeFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pGetPageSizeFn
#define udevxdcd_pCacheInvalidateBufferFn	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pCacheInvalidateBufferFn
#define udevxdcd_pCacheFlushBufferFn	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pCacheFlushBufferFn
#define udevxdcd_pHbufferMapPioFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferMapPioFn
#define udevxdcd_pHbufferUnMapPioFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferUnMapPioFn
#define udevxdcd_pHbufferGetSegmentCountFn	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferGetSegmentCountFn
#define udevxdcd_pHbufferMapDmaAsyncFn	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferMapDmaAsyncFn
#define udevxdcd_pHbufferUnMapDmaFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferUnMapDmaFn
#define udevxdcd_pHbufferGetMoreSegmentsFn	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferGetMoreSegmentsFn
#define udevxdcd_pHbufferSetRangeFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferSetRangeFn
#define udevxdcd_pHbufferGetMaxTransferFn	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferGetMaxTransferFn
#define udevxdcd_pHbufferLogAddrFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferLogAddrFn
#define udevxdcd_pHbufferPhysAddrFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferPhysAddrFn
#define udevxdcd_pHbufferLengthFn		\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferLengthFn
#define udevxdcd_pHbufferLogToDevRelPhysFn	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferLogToDevRelPhysFn
#define udevxdcd_pHbufferDevRelPhysToLogFn	\
	udev_hdr.udevhdr_udevxdcd.udevxdcdhh_UhilAuxInCall.InCall.pHbufferDevRelPhysToLogFn

/**** end of the declarations related to USBPUMP_DEVICE_XDCD ****/

/****************************************************************************\
|
|	Uncloaked defs
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_XDCD_ID	\
   __TMS_USBPUMP_XDCD_ID
# define USBPUMP_XDCD_ID_MASK	\
   __TMS_USBPUMP_XDCD_ID_MASK
# define USBPUMP_XDCD_CONTENTS	\
   __TMS_USBPUMP_XDCD_CONTENTS
# define USBPUMP_XDCD_HDR	\
   __TMS_USBPUMP_XDCD_HDR
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_device_xdcd.h ****/
#endif /* _USBPUMP_DEVICE_XDCD_H_ */
