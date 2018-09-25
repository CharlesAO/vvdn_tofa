/* usbpumpapi.h		Wed Feb 20 2013 04:32:29 tmm */

/*

Module:  usbpumpapi.h

Function:
	API functions for getting to the DataPump

Version:
	V3.11b	Wed Feb 20 2013 04:32:29 tmm	Edit level 17

Copyright notice:
	This file copyright (C) 2002-2009, 2012-2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	June 2002

Revision history:
   1.79a  Thu Jun 20 2002 00:29:54  tmm
	Module created.

   1.79a  7/24/2002  tmm
	Add UsbCompleteQEList().

   1.91c  Thu Apr 28 2005 10:52:35 maw
	Add UsbDeviceStop() && UsbDeviceStart

   1.91d  2005-06-02  tmm
	Correct the inline versions of marking completions busy and not-busy
	to use do-while(0).  Also put the definition of not busy after the
	function prototype so if we ever use the macro version it will work.

   1.91d  2005-08-10  tmm
	1515:  Add UsbPumpInterface_ScanUnionDescriptor().

   1.91i  Sun Sep 25 2005 21:59:36  tmm
	For some reason, UsbPumpQueueBufferWithTimeout(), though in the
	tree, was never actually plumbed into the system.

   1.91i  Mon Sep 26 2005 15:41:36 lin
	Added two spaces in UsbPipeQueueBuffer() before VOID's

   1.97e  Thu Jul 27 2006 21:15:28  tmm
	Add UsbFreeDeviceBuffer()

   1.97f  Wed Dec 13 2006 18:37:16  chwon
	2164: add UsbDeviceRemoteWakeup()

   1.97j  Thu Oct 04 2007 23:25:33  chwon
	3253: add UsbPumpInterface_MatchConfig() prototype

   1.97k  Thu Mar 20 2008 15:09:39  chwon
	3735: reorganize UBUFQE to allow 32-bit sizes

   1.97k  Tue Apr 22 2008 11:00:03  chwon
	4040: use __TMS_NULL instead of NULL

   2.01a  Thu Jun 04 2009 15:34:01  chwon
	8367: Change UsbPumpPlatform_MarkCompletionBusy() prototype and
	use interlocked API.

   3.01f  Fri Jan 20 2012 13:00:36  chwon
	14623: Added UsbDeviceRemoteWakeup_V2().

   3.01f  Thu Feb 09 2012 16:11:50  chwon
	14804: Added UsbDeviceGetFrameNumber().

   3.11b  Wed Feb 20 2013 04:32:29  tmm
	16761: add support for replacing the event routines with platform-
	specific functions.

*/

#ifndef _USBPUMPAPI_H_		/* prevent multiple includes */
#define _USBPUMPAPI_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBPUMPIOCTLAPI_H_
# include "usbpumpioctlapi.h"
#endif

#ifndef _UHIL_INTERLOCKED_H_
# include "uhil_interlocked.h"
#endif

#ifndef _UDEVICE_H_
# include "udevice.h"
#endif

#if defined(__TMS_USBPUMP_DEVICE_EVENT_LOCAL_INCLUDE__)
# include __TMS_STRINGVAL(__TMS_USBPUMP_DEVICE_EVENT_LOCAL_INCLUDE__)
#endif

/****************************************************************************\
|
|	DEBUGGING MACROS
|
\****************************************************************************/

#ifndef _USBPUMPDEBUG_H_
# include "usbpumpdebug.h"
#endif

/****************************************************************************\
|
|	The event handling primitives. These are designed to be
|	overridden by the local platform, if needed, using definitions in
|	the include file named by the value of 
|		__TMS_USBPUMP_DEVICE_EVENT_LOCAL_INCLUDE__
|
\****************************************************************************/

#ifndef UsbMarkCompletionBusy_inline
# define UsbMarkCompletionBusy_inline(pDevice, pCompletion, Context)			\
	(UHIL_InterlockedCompareExchangePointer(				\
		(pDevice)->udev_pPlatform,					\
		(__TMS_VOLATILE __TMS_PVOID *) &(pCompletion)->callback_ctx,	\
		(Context),							\
		__TMS_NULL							\
		) == __TMS_NULL)
#endif

#ifndef UsbMarkCompletionNotBusy_inline
# define UsbMarkCompletionNotBusy_inline(pDevice, pCompletion)			\
   	((__TMS_VOID) UHIL_InterlockedExchangePointer(				\
		(pDevice)->udev_pPlatform,					\
		(__TMS_VOLATILE __TMS_PVOID *) &(pCompletion)->callback_ctx,	\
		 __TMS_NULL							\
		))
#endif

#define UsbMarkCompletionBusy(pDevice, pCompletion, pContext) \
	UsbMarkCompletionBusy_inline(pDevice, pCompletion, pContext)
#define UsbMarkCompletionNotBusy(pDevice, pCompletion) \
	UsbMarkCompletionNotBusy_inline(pDevice, pCompletion)


__TMS_BEGIN_DECLS
/*
|| Mark a given completion not busy.  Udevice argument given for symmetry
*/
__TMS_BOOL	(UsbMarkCompletionBusy)(
			__TMS_UDEVICE *,
			__TMS_CALLBACKCOMPLETION *
			);


__TMS_VOID (UsbMarkCompletionNotBusy)(
	__TMS_UDEVICE *, 
	__TMS_CALLBACKCOMPLETION *
	);

#ifndef __TMS_USBPUMP_USE_LOCAL_UsbPostIfNotBusy
# define __TMS_USBPUMP_USE_LOCAL_UsbPostIfNotBusy 0
#endif
__TMS_BOOL UsbPostIfNotBusy(
	__TMS_PUDEVICE, 
	__TMS_PCALLBACKCOMPLETION,
	__TMS_VOID * /* ctx */
	);

__TMS_END_DECLS

/****************************************************************************\
|
|	The device independent routines
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID	*UsbAllocateDeviceBuffer __TMS_P((__TMS_UDEVICE *, __TMS_BYTES));

__TMS_BOOL	UsbCompleteQE __TMS_P((
			__TMS_PUDEVICE,
			__TMS_UBUFQE **, __TMS_UBUFQE *,
			__TMS_USTAT));

__TMS_VOID	UsbCompleteQEList(
			__TMS_PUDEVICE,
			__TMS_UBUFQE *,
			__TMS_USTAT
			);

__TMS_USHORT	UsbCopyAndReply __TMS_P((
			__TMS_UDEVICE *,
			__TMS_VOID *, __TMS_USHORT,
			__TMS_CONST __TMS_VOID *,
			__TMS_USHORT, __TMS_USHORT));

__TMS_VOID	UsbDeviceGetFrameNumber __TMS_P((
			__TMS_UDEVICE *,
			__TMS_USBPUMP_FRAME_NUMBER *
			));

__TMS_USBPUMP_IOCTL_RESULT UsbDeviceIoctl __TMS_P((
			__TMS_UDEVICE *,
			__TMS_USBPUMP_IOCTL_CODE,
			__TMS_CONST __TMS_VOID *pInParam,
			__TMS_VOID *pOutParam
			));

__TMS_USHORT	UsbDeviceReply __TMS_P((
			__TMS_PUDEVICE, __TMS_CONST __TMS_VOID *,
			__TMS_USHORT, __TMS_USHORT
			));
__TMS_VOID	UsbDeviceStart(
			__TMS_UDEVICE *
			);
__TMS_VOID	UsbDeviceStop(
			__TMS_UDEVICE *
			);
__TMS_VOID	UsbDeviceRemoteWakeup(
			__TMS_UDEVICE *
			);
__TMS_VOID	UsbDeviceRemoteWakeup_V2(
			__TMS_UDEVICE *,
			__TMS_UINTERFACE *
			);
__TMS_VOID	UsbEndpointCancelIo __TMS_P((
			__TMS_UDEVICE *, __TMS_UENDPOINT *,
			__TMS_USTAT /*why*/
			));
__TMS_UENDPOINT *UsbFindEndpointByAddr __TMS_P((
			__TMS_CONST __TMS_UDEVICE *,
			unsigned epid
			));
__TMS_BOOL	UsbFindIndexForIfcset __TMS_P((
			__TMS_CONST __TMS_UDEVICE *,
			__TMS_UINTERFACESET *, __TMS_USHORT *
			));
__TMS_INT	UsbFindIndexForConfiguration __TMS_P((
			__TMS_CONST __TMS_UCONFIG *
			));
__TMS_INT	UsbFindIndexForInterface __TMS_P((
			__TMS_CONST __TMS_UINTERFACE *
			));
__TMS_INT	UsbFindIndexForInterfaceSet __TMS_P((
			__TMS_CONST __TMS_UINTERFACESET *
			));

__TMS_UINTERFACE *UsbFindInterfaceByAddr __TMS_P((
			__TMS_CONST __TMS_UDEVICE *,
			unsigned ifcid
			));

__TMS_VOID	UsbFreeDeviceBuffer __TMS_P((
			__TMS_UDEVICE *,
			__TMS_VOID *,
			__TMS_BYTES
			));

__TMS_PUBUFQE	UsbGetQe __TMS_P((__TMS_PUBUFQE *listhead));

__TMS_VOID	UsbPumpPipe_CancelIo(__TMS_PUPIPE, __TMS_USTAT);
__TMS_VOID	UsbPipeQueue __TMS_P((__TMS_PUDEVICE, __TMS_PUBUFQE));
__TMS_VOID	UsbPipeQueueBuffer __TMS_P((
			__TMS_PUDEVICE, __TMS_PUPIPE, __TMS_PUBUFQE,
			__TMS_VOID * /*buf*/, __TMS_BYTES /*size*/,
			__TMS_PUBUFIODONEFN, __TMS_VOID * /*ctx*/,
			__TMS_UBUFQE_FLAGS
			));

__TMS_VOID	UsbPipeQueueBufferWithTimeout(
			__TMS_PUDEVICE, __TMS_PUPIPE, __TMS_PUBUFQE,
			__TMS_VOID * /*buf*/, __TMS_BYTES /*size*/,
			__TMS_PUBUFIODONEFN, __TMS_VOID * /*ctx*/,
			__TMS_UBUFQE_FLAGS,
			__TMS_UBUFQE_TIMEOUT /*timeout*/
			);

/* from pipe_enum.c */
__TMS_UPIPE *	UsbPumpInterface_EnumeratePipes(
			__TMS_UINTERFACE *,
			__TMS_UINT /* Binding Flags */,
			__TMS_UINT /* EpAddrMask */,
			__TMS_UINT /* PipeOrdinal */
			);

__TMS_BOOL
UsbPumpInterface_MatchConfig(
	__TMS_UINTERFACE *	/* pInterface */
	);

__TMS_UINTERFACESET *
UsbPumpInterface_ScanUnionDescriptor(
	__TMS_UINTERFACE *
	);

__TMS_VOID
UsbPumpPipe_QueueList(
	__TMS_UDEVICE *,
	__TMS_UBUFQE *,
	__TMS_BOOL /* fInitIfTrue */
	);

/* from stat_name.c */
__TMS_CONST __TMS_TEXT *
UsbPumpStatus_Name(
	__TMS_USTAT
	);

__TMS_VOID	UsbPutQe __TMS_P((
			__TMS_PUBUFQE *listhead, __TMS_PUBUFQE pqe
			));

__TMS_VOID	UsbReportDeviceEvent __TMS_P((
			__TMS_UDEVICE *, __TMS_UEVENT, __TMS_VOID *
			));
__TMS_VOID	UsbReportEndpointEvent __TMS_P((
			__TMS_UDEVICE *,
			__TMS_UENDPOINT *,
			__TMS_UEVENT,
			__TMS_VOID *
			));
__TMS_VOID	UsbReportEvent __TMS_P((
			__TMS_UDEVICE *, __TMS_UEVENTNODE *,
			__TMS_UEVENT, __TMS_VOID *
			));
__TMS_VOID	UsbReportInterfaceEvent __TMS_P((
			__TMS_UDEVICE *,
			__TMS_UINTERFACE *,
			__TMS_UEVENT,
			__TMS_VOID *
			));

/**** the version strings ****/
extern __TMS_ROM __TMS_TEXT Usb_txt_copyright[];
extern __TMS_ROM __TMS_TEXT Usb_txt_romdate[];
extern __TMS_ROM __TMS_TEXT Usb_txt_romid[];
extern __TMS_ROM __TMS_TEXT Usb_txt_version[];

__TMS_END_DECLS


/**** end of usbpumpapi.h ****/
#endif /* _USBPUMPAPI_H_ */
