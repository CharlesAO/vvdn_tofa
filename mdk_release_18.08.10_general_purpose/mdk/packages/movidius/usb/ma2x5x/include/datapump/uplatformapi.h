/* uplatformapi.h	Tue Mar 07 2017 16:50:08 chwon */

/*

Module:  uplatformapi.h

Function:
	UPLATFORM API functions.

Version:
	V3.19b	Tue Mar 07 2017 16:50:08 chwon	Edit level 12

Copyright notice:
	This file copyright (C) 2002-2005, 2009, 2013-2015, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Sun Jul 28 2002 23:12:11  tmm
	Module created.

   1.91d  Fri Jun 24 2005 13:31:42  chwon
	Added platform event handling API.

   2.01a  Mon May 11 2009 15:32:42  chwon
	7989: Change UsbPumpPlatform_MarkCompletionBusy() prototype and
	use interlocked API. 

   2.01a  Sun May 17 2009 14:43:05  chwon
	7992: Correct misspelled name -- UsbPumpPlatform_RelaseEventLock.

   2.01a  Wed Jul 01 2009 14:32:33  chwon
	8367: Added UsbPumpPlatform_InterlockedPutQe() and
	UsbPumpPlatform_InterlockedPutQeList() prototype.

   3.11b  Wed Feb 20 2013 04:23:59  tmm
	16761: make it possible to override the event handling definitions
	from the core library.

   3.11d  Fri Oct 25 2013 14:47:41  chwon
	17809: Add UsbPumpPlatform_CreateAbstractPool() prototype.

   3.13b  Wed Sep 17 2014 12:32:26  chwon
	18538: Add UsbPumpPlatform_RegisterOsDescProcess() prototype.

   3.15a  Sun Feb  8 2015 00:39:39  tmm
	Add UsbPumpPlatform_PollEvents() prototype.

   3.19b  Tue Mar 07 2017 16:50:08  chwon
	20480: Add UsbPumpPlatform_AllocateCommonBuffer() and
	UsbPumpPlatform_FreeCommonBuffer() prototype.

*/

#ifndef _UPLATFORMAPI_H_		/* prevent multiple includes */
#define _UPLATFORMAPI_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _UHILEVENT_H_
# include "uhilevent.h"
#endif

#ifndef _UHIL_INTERLOCKED_H_
# include "uhil_interlocked.h"
#endif

#if defined(__TMS_UPLATFORMAPI_LOCAL_INCLUDE__)
# include __TMS_STRINGVAL(__TMS_UPLATFORMAPI_LOCAL_INCLUDE__)
#endif

/****************************************************************************\
|
|	Synchronizing to the DataPump -- a set of primitives that allows the 
|	DataPump to run without a thread, if desired.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(UPLATFORM_SYNCHRONIZATION_BLOCK);

__TMS_FNTYPE_DEF(
UPLATFORM_SYNCHRONIZED_FN,
__TMS_VOID,
	(
	__TMS_UPLATFORM *	pPlatform,
	__TMS_VOID *		pContext
	));

/*
|| You may need to define this yourself; if so, #define
|| __TTUSB_UPLATFORM_SYNCHRONIZATION_BLOCK_DEFINED, and the rest of this
|| section becomes an abstract type specification.
*/
#ifndef __TTUSB_UPLATFORM_SYNCHRONIZATION_BLOCK_DEFINED
# define __TTUSB_UPLATFORM_SYNCHRONIZATION_BLOCK_DEFINED
struct __TMS_STRUCTNAME(UPLATFORM_SYNCHRONIZATION_BLOCK)
	{
	__TMS_CALLBACKCOMPLETION		upsb_Event;
	__TMS_UPLATFORM *			upsb_pPlatform;
	__TMS_UPLATFORM_SYNCHRONIZED_FN *	upsb_pSynchFn;
	__TMS_VOID *				upsb_pContext;
	};
#endif /* __TTUSB_UPLATFORM_SYNCHRONIZATION_BLOCK_DEFINED */


/****************************************************************************\
|
|	DataPump Event handling API functions for dealing with the UPLATFORM.
|
|	These are designed to be overridden by the local platform, if needed, 
|	using definitions in the include file named by the value of 
|		__TMS_USBPUMP_UPLATFORMAPI_LOCAL_INCLUDE__
|
\****************************************************************************/

/* some primitives */
#ifndef UsbPumpPlatform_CheckEvent_inline
#define UsbPumpPlatform_CheckEvent_inline(pPlatform)			\
	(*(pPlatform)->upf_pCheckEvent)((pPlatform)->upf_pEventctx)
#endif

#ifndef UsbPumpPlatform_GetEvent_inline
#define	UsbPumpPlatform_GetEvent_inline(pPlatform)			\
	(*(pPlatform)->upf_pGetEvent)((pPlatform)->upf_pEventctx, __TMS_FALSE)
#endif

#ifndef UsbPumpPlatform_GetEventEx_inline
#define	UsbPumpPlatform_GetEventEx_inline(pPlatform, fWait)		\
	(*(pPlatform)->upf_pGetEvent)((pPlatform)->upf_pEventctx, fWait)
#endif

#ifndef UsbPumpPlatform_IsCompletionBusy_inline
#define UsbPumpPlatform_IsCompletionBusy_inline(pPlatform, pCompletion)	\
	((pCompletion)->callback_ctx != __TMS_NULL)
#endif

#ifndef UsbPumpPlatform_MarkCompletionBusy_inline
#define UsbPumpPlatform_MarkCompletionBusy_inline(pPlatform, pCompletion, pCtx) \
	(UHIL_InterlockedCompareExchangePointer(					\
		(pPlatform),							\
		(__TMS_VOLATILE __TMS_PVOID *) &(pCompletion)->callback_ctx,	\
		(pCtx),								\
		__TMS_NULL							\
		) == __TMS_NULL)
#endif

#ifndef UsbPumpPlatform_MarkCompletionNotBusy_inline
#define UsbPumpPlatform_MarkCompletionNotBusy_inline(pPlatform, pCompletion)	\
   	((__TMS_VOID) UHIL_InterlockedExchangePointer(				\
		(pPlatform),							\
		(__TMS_VOLATILE __TMS_PVOID *) &(pCompletion)->callback_ctx,	\
		 __TMS_NULL							\
		))
#endif

#ifndef UsbPumpPlatform_PostEvent_inline
#define UsbPumpPlatform_PostEvent_inline(pPlatform, pCompletion)	\
	(*(pPlatform)->upf_pPostEvent)((pPlatform)->upf_pEventctx, pCompletion)
#endif

/*
|| Normally, we want to expand the functions in-line, so macroize them
|| The library functions require that these be defined, and there's no
|| point to overriding them -- change the inline definitions instead.
*/
#define	UsbPumpPlatform_CheckEvent(pPlatform)				\
	UsbPumpPlatform_CheckEvent_inline(pPlatform)

#define	UsbPumpPlatform_GetEventEx(pPlatform, fWait)			\
	UsbPumpPlatform_GetEventEx_inline(pPlatform, fWait)

#define UsbPumpPlatform_IsCompletionBusy(pPlatform, pCompletion)	\
	UsbPumpPlatform_IsCompletionBusy_inline(pPlatform, pCompletion)

#define UsbPumpPlatform_MarkCompletionBusy(pPlatform, pCompletion, pCtx) \
	UsbPumpPlatform_MarkCompletionBusy_inline(pPlatform, pCompletion, pCtx)

#define UsbPumpPlatform_MarkCompletionNotBusy(pPlatform, pCompletion)	\
	UsbPumpPlatform_MarkCompletionNotBusy_inline(pPlatform, pCompletion)

#define	UsbPumpPlatform_PostEvent(pPlatform, pCompletion)		\
	UsbPumpPlatform_PostEvent_inline(pPlatform, pCompletion)



/*
|| Declare the prototypes -- the parenthesized functions below are potentially
|| overridded by inline declarations, and the parentheses prevent macro
|| expansion
*/
__TMS_BEGIN_DECLS

__TMS_UCALLBACKCOMPLETION *
		(UsbPumpPlatform_CheckEvent)(
			__TMS_UPLATFORM *		pPlatform
			);

#ifndef	__TMS_USBPUMP_USE_LOCAL_UsbPumpPlatform_DispatchEvent
# define __TMS_USBPUMP_USE_LOCAL_UsbPumpPlatform_DispatchEvent 0
#endif

__TMS_BOOL	UsbPumpPlatform_DispatchEvent(
			__TMS_UPLATFORM *		pPlatform,
			__TMS_UCALLBACKCOMPLETION *	pCompletion
			);

#ifndef __TMS_USBPUMP_USE_LOCAL_UsbPumpPlatform_DoEvents
# define __TMS_USBPUMP_USE_LOCAL_UsbPumpPlatform_DoEvents 0
#endif

__TMS_VOID	UsbPumpPlatform_DoEvents(
			__TMS_UPLATFORM *		pPlatform,
			__TMS_ULONG			nMaxEvents
			);

__TMS_UCALLBACKCOMPLETION *
		(UsbPumpPlatform_GetEvent)(
			__TMS_UPLATFORM *		pPlatform
			);

__TMS_UCALLBACKCOMPLETION *
		(UsbPumpPlatform_GetEventEx)(
			__TMS_UPLATFORM *		pPlatform,
			__TMS_BOOL			fWait
			);

__TMS_BOOL	(UsbPumpPlatform_MarkCompletionBusy)(
			__TMS_UPLATFORM *		pPlatform,
			__TMS_UCALLBACKCOMPLETION *	pCompletion,
			__TMS_VOID *			pCtx
			);

__TMS_VOID	(UsbPumpPlatform_MarkCompletionNotBusy)(
			__TMS_UPLATFORM *		pPlatform,
			__TMS_UCALLBACKCOMPLETION *	pCompletion
			);

__TMS_VOID	(UsbPumpPlatform_PostEvent)(
			__TMS_UPLATFORM *		pPlatform,
			__TMS_UCALLBACKCOMPLETION *	pCompletion
			);

__TMS_BOOL	(UsbPumpPlatform_PostIfNotBusy)(
			__TMS_UPLATFORM *		pPlatform,
			__TMS_UCALLBACKCOMPLETION *	pCompletion,
			__TMS_VOID *			pContext
			);

#ifndef __TMS_USBPUMP_USE_LOCAL_UsbPumpPlatform_ReleaseEventLock
# define __TMS_USBPUMP_USE_LOCAL_UsbPumpPlatform_ReleaseEventLock 0
#endif

__TMS_VOID	UsbPumpPlatform_ReleaseEventLock(
			__TMS_UPLATFORM *		pPlatform
			);

#ifndef __TMS_USBPUMP_USE_LOCAL_UsbPumpPlatform_SynchronizeToDataPump
# define __TMS_USBPUMP_USE_LOCAL_UsbPumpPlatform_SynchronizeToDataPump 0
#endif

__TMS_VOID	UsbPumpPlatform_SynchronizeToDataPump(
			__TMS_UPLATFORM *			pPlatform,
			__TMS_UPLATFORM_SYNCHRONIZATION_BLOCK *	pSyncBlock,
			__TMS_UPLATFORM_SYNCHRONIZED_FN *	pSyncFn,
			__TMS_VOID *				pContext
			);

__TMS_END_DECLS


/****************************************************************************\
|
|	DataPump Memory handling API functions for dealing with the UPLATFORM.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_ABSTRACT_POOL *
UsbPumpPlatform_CreateAbstractPool(
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_BYTES		/* SizeMemoryPool */
	);

__TMS_VOID
UsbPumpPlatform_InterlockedPutQe(
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_UBUFQE **		/* ppHead */,
	__TMS_UBUFQE *		/* pQe */
	);

__TMS_VOID
UsbPumpPlatform_InterlockedPutQeList(
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_UBUFQE **		/* ppHead */,
	__TMS_UBUFQE *		/* pQeList */
	);

__TMS_VOID
UsbPumpPlatform_Free(
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_VOID *		/* pBuffer */,
	__TMS_BYTES		/* nBuffer */
	);

__TMS_VOID *
UsbPumpPlatform_Malloc(
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_BYTES		/* Size */
	);

__TMS_VOID *
UsbPumpPlatform_MallocZero(
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_BYTES		/* Size */
	);

__TMS_USBPUMP_BUFFER_HANDLE
UsbPumpPlatform_AllocateCommonBuffer(
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_BYTES		/* SizeInBytes */,
	__TMS_BYTES		/* AlignmentInBytes */,
	__TMS_VOID **		/* ppBuffer */
	);

__TMS_VOID
UsbPumpPlatform_FreeCommonBuffer(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* hBuffer */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	DataPump UPLATFORM functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpPlatform_RegisterOsDescProcess(
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_UINT		/* VendorCode */
	);

__TMS_VOID 
UsbPumpPlatform_PollEvents(
	__TMS_UPLATFORM *	/* pPlatform */
	);

__TMS_END_DECLS

/**** end of uplatformapi.h ****/
#endif /* _UPLATFORMAPI_H_ */
