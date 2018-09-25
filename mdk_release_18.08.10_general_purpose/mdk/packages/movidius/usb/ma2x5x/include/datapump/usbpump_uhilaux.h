/* usbpump_uhilaux.h	Wed Aug 23 2017 12:19:29 chwon */

/*

Module:  usbpump_uhilaux.h

Function:
	Definition of UHILAUX object

Version:
	V3.21c	Wed Aug 23 2017 12:19:29 chwon	Edit level 10

Copyright notice:
	This file copyright (C) 2009, 2014, 2016-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2009

Revision history:
   2.01a  Mon Mar 23 2009 10:58:50 chwon
	7669: Module created.

   2.01a  Thu Apr 23 2009 13:00:24  chwon
	7669: Fixed typo in the comment

   2.01a  Wed May 06 2009 14:15:05  chwon
	7669: Added GetPageSize, GetMoreSegments, SetRange, GetMaxTransfer
	function and changed buffer handle method function name to
	USBPUMP_UHILAUX_HBUFFER_xxx.

   2.01a  Sun May 17 2009 09:40:01  tmm
	Add USBPUMP_UHILAUX_HBUFFER_ADD_REFERENCE_FN, 
	USBPUMP_UHILAUX_HBUFFER_ENTER_SCOPE_FN, 
	USBPUMP_UHILAUX_HBUFFER_EXIT_SCOPE_FN.  Move a couple of types
	to basehiltypes.h because they're needed in the UPLATFORM.

   2.01a  Sun May 17 2009 15:31:28  chwon
	8040: Change macro name USBPUMP_UHILAUX_SUCCESS/FAILED to
	USBPUMP_UHILAUX_STATUS_SUCCESS/FAILED

   2.01a  Wed Jun 03 2009 10:14:07  chwon
	7669: Use BYTES type instead of SIZE_T

   3.13b  Sat Sep 13 2014 18:32:51  tmm
	18362: the embedded in-switch should be of TYPE_CODESPACE to
	save RAM.

   3.13b  Sun Oct 19 2014 19:43:32  tmm
	18362: extract the common types, so that this entire header file
	doesn't have to be pulled into scope in order to declare functions.

   3.17a  Wed Feb 17 2016 12:17:54  chwon
	19642: Add USBPUMP_UHILAUX_CREATE_HBUFFER_WITH_SEGMENT_FN in the
	in-call and add _V2() macro.

   3.21c  Wed Aug 23 2017 12:19:29  chwon
	20644: Add USBPUMP_UHILAUX_CREATE_SEGMENT_HBUFFER_FN,
	USBPUMP_UHILAUX_SEGMENT_HBUFFER_ADD_FN and
	USBPUMP_UHILAUX_SEGMENT_HBUFFER_RESET_FN in the in-call and add _V3().

*/

#ifndef _USBPUMP_UHILAUX_H_		/* prevent multiple includes */
#define _USBPUMP_UHILAUX_H_

#ifndef _USBPUMP_UHILAUX_TYPES_H_
# include "usbpump_uhilaux_types.h"
#endif

#ifndef _USBIOCTL_API_H_
# include "usbioctl_api.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

/* __TMS_TYPE_DEF_UNION	(USBPUMP_UHILAUX); -- in usbpump_uhilaux_types.h */
/* __TMS_TYPE_DEF_ARG	(USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE, UINT32); -- in usbpump_uhilaux_types.h */
/* __TMS_TYPE_DEF_UNION	(USBPUMP_UHILAUX_INCALL); -- in basehiltypes.h */
/* __TMS_TYPE_DEF_STRUCT(USBPUMP_UHILAUX_SEGMENT);-- in usbpump_uhilaux_types.h */
/*__TMS_TYPE_DEF_HANDLE	(USBPUMP_UHILAUX_SEGMENT_HANDLE); -- in usbpump_uhilaux_types.h */
/* __TMS_TYPE_DEF_ARG	(USBPUMP_UHILAUX_SEGMENT_COUNT, UINT32); -- in usbpump_uhilaux_types.h */
/* __TMS_TYPE_DEF_STRUCT(USBPUMP_UHILAUX_SESSION); -- in usbpump_uhilaux_types.h */
/* __TMS_TYPE_DEF_ARG	(USBPUMP_UHILAUX_STATUS, UINT32); -- in usbpump_uhilaux_types.h */

/* __TMS_TYPE_DEF	(USBPUMP_PHYSADDR_DEVICE_RELATIVE, __TMS_PHYSADDR); -- in usbpump_uhilaux_types.h */


/****************************************************************************\
|
|	Definition of USBPUMP_UHILAUX_INCALL
|
\****************************************************************************/

/* __TMS_TYPE_DEF_STRUCT	(USBPUMP_UHILAUX_INCALL_CONTENTS); */
struct __TMS_STRUCTNAME	(USBPUMP_UHILAUX_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_API_INCALL_HEADER;

	/* memory management interface */
	__TMS_USBPUMP_UHILAUX_ALLOCATE_COMMON_BUFFER_FN	*pAllocateCommonBufferFn;
	__TMS_USBPUMP_UHILAUX_ALLOCATE_BUFFER_FN	*pAllocateBufferFn;
	__TMS_USBPUMP_UHILAUX_FREE_BUFFER_FN		*pFreeBufferFn;
	__TMS_USBPUMP_UHILAUX_GET_PAGE_SIZE_FN		*pGetPageSizeFn;

	/* cache management interface */
	__TMS_USBPUMP_UHILAUX_CACHE_FLUSH_BUFFER_FN	*pCacheFlushBufferFn;
	__TMS_USBPUMP_UHILAUX_CACHE_INVALIDATE_BUFFER_FN*pCacheInvalidateBufferFn;

	/* buffer mapping interface */
	__TMS_USBPUMP_UHILAUX_HBUFFER_MAP_PIO_FN	*pHbufferMapPioFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_UNMAP_PIO_FN	*pHbufferUnMapPioFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_MAP_DMA_ASYNC_FN	*pHbufferMapDmaAsyncFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_UNMAP_DMA_FN	*pHbufferUnMapDmaFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_GET_MORE_SEGMENTS_FN *pHbufferGetMoreSegmentsFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_GET_SEGMENT_COUNT_FN *pHbufferGetSegmentCountFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_SET_RANGE_FN	*pHbufferSetRangeFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_GET_MAX_TRANSFER_FN *pHbufferGetMaxTransferFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_LOGADDR_FN	*pHbufferLogAddrFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_DEV_REL_PHYSADDR_FN *pHbufferPhysAddrFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_LENGTH_FN		*pHbufferLengthFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_LOG_TO_DEV_REL_PHYS_FN *pHbufferLogToDevRelPhysFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_DEV_REL_PHYS_TO_LOG_FN *pHbufferDevRelPhysToLogFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_ADD_REFERENCE_FN	*pHbufferAddReferenceFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_ENTER_SCOPE_FN	*pHbufferEnterScopeFn;
	__TMS_USBPUMP_UHILAUX_HBUFFER_EXIT_SCOPE_FN	*pHbufferExitScopeFn;

	/* buffer handle creating interface */
	__TMS_USBPUMP_UHILAUX_CREATE_HBUFFER_WITH_SEGMENT_FN *
						pCreateHbufferWithSegmentFn;
	__TMS_USBPUMP_UHILAUX_CREATE_SEGMENT_HBUFFER_FN *pCreateSegmentHbufferFn;
	__TMS_USBPUMP_UHILAUX_SEGMENT_HBUFFER_ADD_FN *	pSegmentHbufferAddFn;
	__TMS_USBPUMP_UHILAUX_SEGMENT_HBUFFER_RESET_FN *pSegmentHbufferResetFn;

	__TMS_USBPUMP_API_INCALL_TRAILER;
	};


union __TMS_UNIONNAME(USBPUMP_UHILAUX_INCALL)
	{
	__TMS_USBPUMP_UHILAUX_INCALL_CONTENTS	InCall;
	__TMS_USBPUMP_API_INCALL_CONTENTS__UNION;
	};

#define	__TMS_USBPUMP_UHILAUX_INCALL_INIT_V3(			\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn,				\
	a_pCreateSegmentHbufferFn,				\
	a_pSegmentHbufferAddFn,					\
	a_pSegmentHbufferResetFn				\
	)							\
	{{							\
	__TMS_USBPUMP_API_INCALL_HEADER_INIT_V1(		\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,		\
		sizeof(__TMS_USBPUMP_UHILAUX_INCALL),		\
		a_pCloseFn					\
		),						\
	(a_pAllocateCommonBufferFn),				\
	(a_pAllocateBufferFn),					\
	(a_pFreeBufferFn),					\
	(a_pGetPageSizeFn),					\
	(a_pCacheFlushBufferFn),				\
	(a_pCacheInvalidateBufferFn),				\
	(a_pHbufferMapPioFn),					\
	(a_pHbufferUnMapPioFn),					\
	(a_pHbufferMapDmaAsyncFn),				\
	(a_pHbufferUnMapDmaFn),					\
	(a_pHbufferGetMoreSegmentsFn),				\
	(a_pHbufferGetSegmentCountFn),				\
	(a_pHbufferSetRangeFn),					\
	(a_pHbufferGetMaxTransferFn),				\
	(a_pHbufferLogAddrFn),					\
	(a_pHbufferPhysAddrFn),					\
	(a_pHbufferLengthFn),					\
	(a_pHbufferLogToDevRelPhysFn),				\
	(a_pHbufferDevRelPhysToLogFn),				\
	(a_pHbufferAddReferenceFn),				\
	(a_pHbufferEnterScopeFn),				\
	(a_pHbufferExitScopeFn),				\
	(a_pCreateHbufferWithSegmentFn),			\
	(a_pCreateSegmentHbufferFn),				\
	(a_pSegmentHbufferAddFn),				\
	(a_pSegmentHbufferResetFn),				\
	__TMS_USBPUMP_API_INCALL_TRAILER_INIT_V1(		\
		__TMS_USBPUMP_API_INCALL_MAGIC_END		\
		)						\
	}}

#define	__TMS_USBPUMP_UHILAUX_INCALL_INIT_V2(			\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn				\
	)							\
	__TMS_USBPUMP_UHILAUX_INCALL_INIT_V3(			\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn,				\
	/* V3: pCreateSegmentHbufferFn */			\
		UsbPumpUhilAux_CreateSegmentHbufferDummy,	\
	/* V3: pSegmentHbufferAddFn */				\
		UsbPumpUhilAux_SegmentHbufferAddDummy,		\
	/* V3: pSegmentHbufferResetFn */			\
		UsbPumpUhilAux_SegmentHbufferResetDummy		\
	)

#define	__TMS_USBPUMP_UHILAUX_INCALL_INIT_V1(			\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn					\
	)							\
	__TMS_USBPUMP_UHILAUX_INCALL_INIT_V2(			\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	/* V2: pCreateHbufferWithSegmentFn */			\
		UsbPumpUhilAux_CreateHbufferWithSegmentDummy	\
	)

#define	__TMS_USBPUMP_UHILAUX_INCALL_SETUP_V3(			\
	a_pInCall,						\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn,				\
	a_pCreateSegmentHbufferFn,				\
	a_pSegmentHbufferAddFn,					\
	a_pSegmentHbufferResetFn				\
	)							\
    do  {							\
	__TMS_USBPUMP_API_INCALL_HEADER_SETUP_V1(		\
		a_pInCall,					\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,		\
		sizeof(*(a_pInCall)),				\
		a_pCloseFn					\
		);						\
	(a_pInCall)->InCall.pAllocateCommonBufferFn =		\
		(a_pAllocateCommonBufferFn);			\
	(a_pInCall)->InCall.pAllocateBufferFn =			\
		(a_pAllocateBufferFn);				\
	(a_pInCall)->InCall.pFreeBufferFn =			\
		(a_pFreeBufferFn);				\
	(a_pInCall)->InCall.pGetPageSizeFn =			\
		(a_pGetPageSizeFn);				\
	(a_pInCall)->InCall.pCacheFlushBufferFn =		\
		(a_pCacheFlushBufferFn);			\
	(a_pInCall)->InCall.pCacheInvalidateBufferFn =		\
		(a_pCacheInvalidateBufferFn);			\
	(a_pInCall)->InCall.pHbufferMapPioFn =			\
		(a_pHbufferMapPioFn);				\
	(a_pInCall)->InCall.pHbufferUnMapPioFn =		\
		(a_pHbufferUnMapPioFn);				\
	(a_pInCall)->InCall.pHbufferMapDmaAsyncFn =		\
		(a_pHbufferMapDmaAsyncFn);			\
	(a_pInCall)->InCall.pHbufferUnMapDmaFn =		\
		(a_pHbufferUnMapDmaFn);				\
	(a_pInCall)->InCall.pHbufferGetMoreSegmentsFn =		\
		(a_pHbufferGetMoreSegmentsFn);			\
	(a_pInCall)->InCall.pHbufferGetSegmentCountFn =		\
		(a_pHbufferGetSegmentCountFn);			\
	(a_pInCall)->InCall.pHbufferSetRangeFn =		\
		(a_pHbufferSetRangeFn);				\
	(a_pInCall)->InCall.pHbufferGetMaxTransferFn =		\
		(a_pHbufferGetMaxTransferFn);			\
	(a_pInCall)->InCall.pHbufferLogAddrFn =			\
		(a_pHbufferLogAddrFn);				\
	(a_pInCall)->InCall.pHbufferPhysAddrFn =		\
		(a_pHbufferPhysAddrFn);				\
	(a_pInCall)->InCall.pHbufferLengthFn =			\
		(a_pHbufferLengthFn);				\
	(a_pInCall)->InCall.pHbufferLogToDevRelPhysFn =		\
		(a_pHbufferLogToDevRelPhysFn);			\
	(a_pInCall)->InCall.pHbufferDevRelPhysToLogFn =		\
		(a_pHbufferDevRelPhysToLogFn);			\
	(a_pInCall)->InCall.pHbufferAddReferenceFn =		\
		(a_pHbufferAddReferenceFn);			\
	(a_pInCall)->InCall.pHbufferEnterScopeFn =		\
		(a_pHbufferEnterScopeFn);			\
	(a_pInCall)->InCall.pHbufferExitScopeFn =		\
		(a_pHbufferExitScopeFn);			\
	(a_pInCall)->InCall.pCreateHbufferWithSegmentFn =	\
		(a_pCreateHbufferWithSegmentFn);		\
	(a_pInCall)->InCall.pCreateSegmentHbufferFn =		\
		(a_pCreateSegmentHbufferFn);			\
	(a_pInCall)->InCall.pSegmentHbufferAddFn =		\
		(a_pSegmentHbufferAddFn);			\
	(a_pInCall)->InCall.pSegmentHbufferResetFn =		\
		(a_pSegmentHbufferResetFn);			\
	__TMS_USBPUMP_API_INCALL_TRAILER_SETUP_V1(		\
		a_pInCall,					\
		InCall,						\
		__TMS_USBPUMP_API_INCALL_MAGIC_END		\
		);						\
	} while (0)

#define	__TMS_USBPUMP_UHILAUX_INCALL_SETUP_V2(			\
	a_pInCall,						\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn				\
	)							\
	__TMS_USBPUMP_UHILAUX_INCALL_SETUP_V3(			\
	a_pInCall,						\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn,				\
	/* V3: pCreateSegmentHbufferFn */			\
		UsbPumpUhilAux_CreateSegmentHbufferDummy,	\
	/* V3: pSegmentHbufferAddFn */				\
		UsbPumpUhilAux_SegmentHbufferAddDummy,		\
	/* V3: pSegmentHbufferResetFn */			\
		UsbPumpUhilAux_SegmentHbufferResetDummy		\
	)

#define	__TMS_USBPUMP_UHILAUX_INCALL_SETUP_V1(			\
	a_pInCall,						\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn					\
	)							\
	__TMS_USBPUMP_UHILAUX_INCALL_SETUP_V2(			\
	a_pInCall,						\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	/* V2: pCreateHbufferWithSegmentFn */			\
		UsbPumpUhilAux_CreateHbufferWithSegmentDummy	\
	)


/****************************************************************************\
|
|	Definition of USBPUMP_UHILAUX_SESSION
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_UHILAUX_SESSION)
	{
	__TMS_USBPUMP_UHILAUX *			pUhilAux;

	__TMS_USBPUMP_UHILAUX_SESSION *		pNext;
	__TMS_USBPUMP_UHILAUX_SESSION *		pLast;
	};


struct __TMS_STRUCTNAME(USBPUMP_UHILAUX_SEGMENT)
	{
	__TMS_PHYSADDR				segmentStartAddress;
	__TMS_BYTES				segmentSizeInBytes;
	};


/****************************************************************************\
|
|	Definition of USBPUMP_UHILAUX
|
\****************************************************************************/

/*

Type:	USBPUMP_UHILAUX

Function:
	The DataPump object that represents UHIL++.

Description:
	A USBPUMP_UHILAUX object provides the following UHIL++ interfaces
	Memory management interface, which has in-calls, but no out-calls
	Cache Management Interface, which has in-calls, but no out-calls
	Buffer mapping interface, which has in-calls, one with a callback,
	but no out-calls

Contents:
	USBPUMP_OBJECT_HEADER ObjectHeader;
		The standard object header.  The tag is |USBPUMP_UHILAUX_TAG|.
		The IOCTL parent is the pointer to the next object closer
		to the |UPLATFORM| -- depending on the design of the UHILAUX,
		there may be no objects, or several objects, in series between
		this UHILAUX and the UPLATFORM.

	UPLATFORM *pPlatform;
		Pointer to the platform object, for convenience.

	CONST USBGUID *pUhilAuxGuid;
		The USBPUMP_UHILAUX's GUID.  For claiming the
		USBPUMP_IOCTL_API_OPEN IOCTL, UHILAUX check pInArg->pGuid to
		make sure it's the right GUID for the UHILAUX API.  If not
		match, reject the IOCTL as unclaimed.

	CONST USBPUMP_UHILAUX_INCALL *	pInCall;
		Platform specific UHILAUX in-call switch table.

	UINT MaxSession;
		A configuration parameter, set the maximum number of session

	USBPUMP_UHILAUX_SESSION * pvSession;
		UHILAUX open session handling memory blocks.

	USBPUMP_UHILAUX_SESSION *pFreeSession;
		Head pointer of free session list.

	USBPUMP_UHILAUX_SESSION *pOpenSession;
		Head pointer of opened session list.

See Also:
	UsbPumpUhilAux_Create()
	UsbPumpUhilAux_Delete()
	UsbPumpUhilAux_Init()

*/

/* __TMS_TYPE_DEF_STRUCT	(USBPUMP_UHILAUX_CONTENTS); */
struct __TMS_STRUCTNAME	(USBPUMP_UHILAUX_CONTENTS)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__STRUCT;

	/*
	|| The USBPUMP_UHILAUX implementation may need to use platform routines;
	|| so it stashes a UPLATFORM pointer.
	*/
	__TMS_UPLATFORM *				pPlatform;

	__TMS_CONST __TMS_USBGUID *			pUhilAuxGuid;

	__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_USBPUMP_UHILAUX_INCALL) *	pInCall;

	__TMS_UINT					MaxSession;
	__TMS_USBPUMP_UHILAUX_SESSION *			pvSession;

	__TMS_USBPUMP_UHILAUX_SESSION *			pFreeSession;
	__TMS_USBPUMP_UHILAUX_SESSION *			pOpenSession;
	};

union __TMS_UNIONNAME(USBPUMP_UHILAUX)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;

	__TMS_USBPUMP_UHILAUX_CONTENTS		UhilAux;
	};

#define	__TMS_USBPUMP_UHILAUX_CONTENTS__STRUCT			\
	__TMS_USBPUMP_UHILAUX_CONTENTS		UhilAux

#define __TMS_USBPUMP_UHILAUX_CONTENTS__UNION			\
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;			\
	__TMS_USBPUMP_UHILAUX_CONTENTS		UhilAux;	\
	__TMS_USBPUMP_UHILAUX			UhilAuxCast

/*
|| Object related things
*/

/* the DataPump UHILAUX object tag */
#define	__TMS_USBPUMP_UHILAUX_TAG	__TMS_UHIL_MEMTAG('U', 'A', 'u', 'x')

/* the UhilAux object name */
#define	__TMS_USBPUMP_UHILAUX_OBJECT_NAME(Text)		\
	Text ".uhilaux.mcci.com"

#define	__TMS_USBPUMP_UHILAUX_OBJECT_ENUM_NAME		\
	__TMS_USBPUMP_UHILAUX_OBJECT_NAME("*")

#define __TMS_USBPUMP_UHILAUX_GUID_INIT()		\
	__TMS_USBUUID_INIT(0xbb40be6e, 0x1197, 0x46cc,	\
			   0x9d, 0x91, 0xd3, 0x73, 0xf9, 0x3a, 0xcf, 0x16)


/****************************************************************************\
|
|	Definition of USBPUMP_UHILAUX_STATUS
|
\****************************************************************************/

#define	__TMS_USBPUMP_UHILAUX_STATUS_OK			\
	__TMS_USBPUMP_API_STATUS_OK
#define	__TMS_USBPUMP_UHILAUX_STATUS_INVALID_PARAMETER	\
	__TMS_USBPUMP_API_STATUS_INVALID_PARAMETER
#define	__TMS_USBPUMP_UHILAUX_STATUS_ARG_AREA_TOO_SMALL	\
	__TMS_USBPUMP_API_STATUS_ARG_AREA_TOO_SMALL
#define	__TMS_USBPUMP_UHILAUX_STATUS_BUFFER_TOO_SMALL	\
	__TMS_USBPUMP_API_STATUS_BUFFER_TOO_SMALL
#define	__TMS_USBPUMP_UHILAUX_STATUS_NOT_SUPPORTED	\
	__TMS_USBPUMP_API_STATUS_NOT_SUPPORTED
#define	__TMS_USBPUMP_UHILAUX_STATUS_NO_MORE_SESSIONS	\
	__TMS_USBPUMP_API_STATUS_NO_MORE_SESSIONS
#define	__TMS_USBPUMP_UHILAUX_STATUS_INVALID_SESSION_HANDLE	\
	__TMS_USBPUMP_API_STATUS_INVALID_SESSION_HANDLE
#define	__TMS_USBPUMP_UHILAUX_STATUS_UNSUCCESSFUL	\
	__TMS_USBPUMP_API_STATUS_UNSUCCESSFUL

#define	__TMS_USBPUMP_UHILAUX_STATUS_INVALID_BUFFER_HANDLE	\
	(__TMS_USBPUMP_API_STATUS__MAX + 0)
#define	__TMS_USBPUMP_UHILAUX_STATUS_INSUFFICIENT_RESOURCES	\
	(__TMS_USBPUMP_API_STATUS__MAX + 1)
#define	__TMS_USBPUMP_UHILAUX_STATUS_ACCESS_VIOLATION	\
	(__TMS_USBPUMP_API_STATUS__MAX + 2)
#define	__TMS_USBPUMP_UHILAUX_STATUS_INTERNAL_ERROR	\
	(__TMS_USBPUMP_API_STATUS__MAX + 3)
#define	__TMS_USBPUMP_UHILAUX_STATUS_GENERAL_ERROR	\
	(__TMS_USBPUMP_API_STATUS__MAX + 4)


#define	__TMS_USBPUMP_UHILAUX_STATUS_SUCCESS(Status)	\
	((Status) == __TMS_USBPUMP_UHILAUX_STATUS_OK)

#define	__TMS_USBPUMP_UHILAUX_STATUS_FAILED(Status)	\
	((Status) != __TMS_USBPUMP_UHILAUX_STATUS_OK)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/* Default linkage for UHILAUX object GUID */
extern __TMS_CONST __TMS_USBGUID	gk_UsbPumpUhilAux_Guid;

/* Default linkage for UHILAUX object name */
extern __TMS_CONST __TMS_TEXT		gk_UsbPumpUhilAux_ObjectName[];

__TMS_USBPUMP_UHILAUX *
UsbPumpUhilAux_Create(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_CONST __TMS_TEXT *	/* pObjectName */,
	__TMS_BYTES			/* sizeUhilAuxObject */,
	__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_USBPUMP_UHILAUX_INCALL) * /* pInCall */,
	__TMS_UINT			/* MaxSession */,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pIoctlParent	-- OPTIONAL */
	);

__TMS_VOID
UsbPumpUhilAux_Delete(
	__TMS_USBPUMP_UHILAUX *		/* pUhilAux */
	);

__TMS_VOID
UsbPumpUhilAux_Init(
	__TMS_USBPUMP_UHILAUX *		/* pUhilAux */,
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_CONST __TMS_TEXT *	/* pObjectName */,
	__TMS_BYTES			/* sizeUhilAuxObject */,
	__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_USBPUMP_UHILAUX_INCALL) * /* pInCall */,
	__TMS_UINT			/* MaxSession */,
	__TMS_USBPUMP_UHILAUX_SESSION *	/* pvSession */,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pIoctlParent	-- OPTIONAL */
	);

__TMS_USBPUMP_API_CLOSE_FN	UsbPumpUhilAux_SessionClose;

__TMS_USBPUMP_UHILAUX *
UsbPumpUhilAux_SessionValidate(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */
	);

__TMS_USBPUMP_UHILAUX_CREATE_HBUFFER_WITH_SEGMENT_FN
UsbPumpUhilAux_CreateHbufferWithSegmentDummy;

__TMS_USBPUMP_UHILAUX_CREATE_SEGMENT_HBUFFER_FN
UsbPumpUhilAux_CreateSegmentHbufferDummy;

__TMS_USBPUMP_UHILAUX_SEGMENT_HBUFFER_ADD_FN
UsbPumpUhilAux_SegmentHbufferAddDummy;

__TMS_USBPUMP_UHILAUX_SEGMENT_HBUFFER_RESET_FN
UsbPumpUhilAux_SegmentHbufferResetDummy;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_UHILAUX_INCALL_INIT_V3(			\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn,				\
	a_pCreateSegmentHbufferFn,				\
	a_pSegmentHbufferAddFn,					\
	a_pSegmentHbufferResetFn				\
	)	\
	__TMS_USBPUMP_UHILAUX_INCALL_INIT_V3(			\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn,				\
	a_pCreateSegmentHbufferFn,				\
	a_pSegmentHbufferAddFn,					\
	a_pSegmentHbufferResetFn				\
	)
# define USBPUMP_UHILAUX_INCALL_INIT_V2(			\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn				\
	)	\
	__TMS_USBPUMP_UHILAUX_INCALL_INIT_V2(			\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn				\
	)
# define USBPUMP_UHILAUX_INCALL_INIT_V1(			\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn					\
	)	\
	__TMS_USBPUMP_UHILAUX_INCALL_INIT_V1(			\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn					\
	)
# define USBPUMP_UHILAUX_INCALL_SETUP_V3(			\
	a_pInCall,						\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn,				\
	a_pCreateSegmentHbufferFn,				\
	a_pSegmentHbufferAddFn,					\
	a_pSegmentHbufferResetFn				\
	)	\
	__TMS_USBPUMP_UHILAUX_INCALL_SETUP_V3(			\
	a_pInCall,						\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn,				\
	a_pCreateSegmentHbufferFn,				\
	a_pSegmentHbufferAddFn,					\
	a_pSegmentHbufferResetFn				\
	)
# define USBPUMP_UHILAUX_INCALL_SETUP_V2(			\
	a_pInCall,						\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn				\
	)	\
	__TMS_USBPUMP_UHILAUX_INCALL_SETUP_V2(			\
	a_pInCall,						\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn,					\
	a_pCreateHbufferWithSegmentFn				\
	)
# define USBPUMP_UHILAUX_INCALL_SETUP_V1(			\
	a_pInCall,						\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn					\
	)	\
	__TMS_USBPUMP_UHILAUX_INCALL_SETUP_V1(			\
	a_pInCall,						\
	a_pCloseFn,						\
	a_pAllocateCommonBufferFn,				\
	a_pAllocateBufferFn,					\
	a_pFreeBufferFn,					\
	a_pGetPageSizeFn,					\
	a_pCacheFlushBufferFn,					\
	a_pCacheInvalidateBufferFn,				\
	a_pHbufferMapPioFn,					\
	a_pHbufferUnMapPioFn,					\
	a_pHbufferMapDmaAsyncFn,				\
	a_pHbufferUnMapDmaFn,					\
	a_pHbufferGetMoreSegmentsFn,				\
	a_pHbufferGetSegmentCountFn,				\
	a_pHbufferSetRangeFn,					\
	a_pHbufferGetMaxTransferFn,				\
	a_pHbufferLogAddrFn,					\
	a_pHbufferPhysAddrFn,					\
	a_pHbufferLengthFn,					\
	a_pHbufferLogToDevRelPhysFn,				\
	a_pHbufferDevRelPhysToLogFn,				\
	a_pHbufferAddReferenceFn,				\
	a_pHbufferEnterScopeFn,					\
	a_pHbufferExitScopeFn					\
	)
# define USBPUMP_UHILAUX_CONTENTS__STRUCT	\
   __TMS_USBPUMP_UHILAUX_CONTENTS__STRUCT
# define USBPUMP_UHILAUX_CONTENTS__UNION	\
   __TMS_USBPUMP_UHILAUX_CONTENTS__UNION
# define USBPUMP_UHILAUX_TAG	\
   __TMS_USBPUMP_UHILAUX_TAG
# define USBPUMP_UHILAUX_OBJECT_NAME(Text)	\
   __TMS_USBPUMP_UHILAUX_OBJECT_NAME(Text)
# define USBPUMP_UHILAUX_OBJECT_ENUM_NAME	\
   __TMS_USBPUMP_UHILAUX_OBJECT_ENUM_NAME
# define USBPUMP_UHILAUX_GUID_INIT()	\
   __TMS_USBPUMP_UHILAUX_GUID_INIT()
# define USBPUMP_UHILAUX_STATUS_OK	\
   __TMS_USBPUMP_UHILAUX_STATUS_OK
# define USBPUMP_UHILAUX_STATUS_INVALID_PARAMETER	\
   __TMS_USBPUMP_UHILAUX_STATUS_INVALID_PARAMETER
# define USBPUMP_UHILAUX_STATUS_ARG_AREA_TOO_SMALL	\
   __TMS_USBPUMP_UHILAUX_STATUS_ARG_AREA_TOO_SMALL
# define USBPUMP_UHILAUX_STATUS_BUFFER_TOO_SMALL	\
   __TMS_USBPUMP_UHILAUX_STATUS_BUFFER_TOO_SMALL
# define USBPUMP_UHILAUX_STATUS_NOT_SUPPORTED	\
   __TMS_USBPUMP_UHILAUX_STATUS_NOT_SUPPORTED
# define USBPUMP_UHILAUX_STATUS_NO_MORE_SESSIONS	\
   __TMS_USBPUMP_UHILAUX_STATUS_NO_MORE_SESSIONS
# define USBPUMP_UHILAUX_STATUS_INVALID_SESSION_HANDLE	\
   __TMS_USBPUMP_UHILAUX_STATUS_INVALID_SESSION_HANDLE
# define USBPUMP_UHILAUX_STATUS_UNSUCCESSFUL	\
   __TMS_USBPUMP_UHILAUX_STATUS_UNSUCCESSFUL
# define USBPUMP_UHILAUX_STATUS_INVALID_BUFFER_HANDLE	\
   __TMS_USBPUMP_UHILAUX_STATUS_INVALID_BUFFER_HANDLE
# define USBPUMP_UHILAUX_STATUS_INSUFFICIENT_RESOURCES	\
   __TMS_USBPUMP_UHILAUX_STATUS_INSUFFICIENT_RESOURCES
# define USBPUMP_UHILAUX_STATUS_ACCESS_VIOLATION	\
   __TMS_USBPUMP_UHILAUX_STATUS_ACCESS_VIOLATION
# define USBPUMP_UHILAUX_STATUS_INTERNAL_ERROR	\
   __TMS_USBPUMP_UHILAUX_STATUS_INTERNAL_ERROR
# define USBPUMP_UHILAUX_STATUS_GENERAL_ERROR	\
   __TMS_USBPUMP_UHILAUX_STATUS_GENERAL_ERROR
# define USBPUMP_UHILAUX_STATUS_SUCCESS(Status)	\
   __TMS_USBPUMP_UHILAUX_STATUS_SUCCESS(Status)
# define USBPUMP_UHILAUX_STATUS_FAILED(Status)	\
   __TMS_USBPUMP_UHILAUX_STATUS_FAILED(Status)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_uhilaux.h ****/
#endif /* _USBPUMP_UHILAUX_H_ */
