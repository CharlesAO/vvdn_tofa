/* usbpump_uhilaux_types.h	Wed Aug 23 2017 12:05:58 chwon */

/*

Module:  usbpump_uhilaux_types.h

Function:
	Scalar types and handles exported by usbpump_uhilaux.

Version:
	V3.21c	Wed Aug 23 2017 12:05:58 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2014-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2014

Revision history:
   3.13b  Sun Oct 19 2014 19:27:06  tmm
	18362: Module created.

   3.15b  Mon Apr 06 2015 19:11:53  tmm
	19019: improve documentation.

   3.17a  Wed Feb 17 2016 11:51:22  chwon
	19642: Add USBPUMP_UHILAUX_CREATE_HBUFFER_WITH_SEGMENT_FN.

   3.21c  Wed Aug 23 2017 12:05:58  chwon
	20644: Add USBPUMP_UHILAUX_CREATE_SEGMENT_HBUFFER_FN,
	USBPUMP_UHILAUX_SEGMENT_HBUFFER_ADD_FN and
	USBPUMP_UHILAUX_SEGMENT_HBUFFER_RESET_FN type.

*/

#ifndef _USBPUMP_UHILAUX_TYPES_H_		/* prevent multiple includes */
#define _USBPUMP_UHILAUX_TYPES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION	(USBPUMP_UHILAUX);
__TMS_TYPE_DEF_ARG	(USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE, UINT32);
/* __TMS_TYPE_DEF_UNION	(USBPUMP_UHILAUX_INCALL); -- in basehiltypes.h */
__TMS_TYPE_DEF_STRUCT	(USBPUMP_UHILAUX_SEGMENT);
__TMS_TYPE_DEF_HANDLE	(USBPUMP_UHILAUX_SEGMENT_HANDLE);
__TMS_TYPE_DEF_ARG	(USBPUMP_UHILAUX_SEGMENT_COUNT, UINT32);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_UHILAUX_SESSION);
__TMS_TYPE_DEF_ARG	(USBPUMP_UHILAUX_STATUS, UINT32);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_UHILAUX_INCALL_CONTENTS);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_UHILAUX_CONTENTS);

__TMS_TYPE_DEF		(USBPUMP_PHYSADDR_DEVICE_RELATIVE, __TMS_PHYSADDR);

enum __TMS_ENUMNAME(USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE_ENUM)
	{
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_READ = 0,
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_WRITE = 1,
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_MODIFY = 2
	};

/* Manual cloaking for enum type */
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_UHILAUX_BUFFER_ACCESS_READ	\
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_READ
# define USBPUMP_UHILAUX_BUFFER_ACCESS_WRITE	\
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_WRITE
# define USBPUMP_UHILAUX_BUFFER_ACCESS_MODIFY	\
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_MODIFY
#endif


/****************************************************************************\
|
|	USBPUMP_UHILAUX in-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_UHILAUX_ALLOCATE_COMMON_BUFFER_FN

Function:
	Allocate memory elements accessible by the software and hardware

Definition:
	typedef USBPUMP_BUFFER_HANDLE
	(*USBPUMP_UHILAUX_ALLOCATE_COMMON_BUFFER_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		BYTES			SizeInBytes,
		BYTES			AlignmentInBytes
		);

Description:
	Allocate memory elements accessible by the software and hardware from
	common memory.  If SizeInBytes <= PAGESIZE then returned memory is
	guaranteed not to cross a page boundary. Otherwise, the memory will
	cross a page boundary, but the returned memory is guaranteed to be
	contiguous (unless an error occurs, indicated by a return value of NULL)

Returns:
	A USBPUMP_BUFFER_HANDLE, or NULL if the requested memory cannot be
	allocated.

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_ALLOCATE_COMMON_BUFFER_FN,
__TMS_USBPUMP_BUFFER_HANDLE,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_BYTES			/* SizeInBytes */,
	__TMS_BYTES			/* AlignmentInBytes */
	));

/*

Type:  USBPUMP_UHILAUX_ALLOCATE_BUFFER_FN

Function:
	Allocate buffer used for a data transfer

Definition:
	typedef USBPUMP_BUFFER_HANDLE
	(*USBPUMP_UHILAUX_ALLOCATE_BUFFER_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		BYTES					SizeInBytes,
		USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE	AccessType
		);

Description:
	Allocate any kernel menory used for a data transfer fabricated by
	the software.

Returns:
	A USBPUMP_BUFFER_HANDLE, or NULL if the requested memory cannot be
	allocated.

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_ALLOCATE_BUFFER_FN,
__TMS_USBPUMP_BUFFER_HANDLE,
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* SessionHandle */,
	__TMS_BYTES					/* SizeInBytes */,
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE	/* AccessType */
	));

/*

Type:  USBPUMP_UHILAUX_FREE_BUFFER_FN

Function:
	Free buffer memory with buffer handle

Definition:
	typedef VOID
	(*USBPUMP_UHILAUX_FREE_BUFFER_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle
		);

Description:
	Free memory allocated via USBPUMP_UHILAUX_ALLOCATE_COMMON_BUFFER_FN
	and USBPUMP_UHILAUX_ALLOCATE_BUFFER_FN, and release handle.

Returns:
	Nothing

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_FREE_BUFFER_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */
	));

/*

Type:  USBPUMP_UHILAUX_GET_PAGE_SIZE_FN

Function:
	Get the physical memory page size

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_GET_PAGE_SIZE_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		BYTES *			pPageSizeInBytes	// OUT
		);

Description:
	This function returns the physical memory page size from the
	underlying OS.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_GET_PAGE_SIZE_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_BYTES *			/* OUT pPageSizeInBytes */
	));

/*

Type:  USBPUMP_UHILAUX_CACHE_FLUSH_BUFFER_FN

Function:
	Flush the contents of the cache to memory

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_CACHE_FLUSH_BUFFER_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle
		);

Description:
	This function flushes the contents of the cache to the physical memory.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_CACHE_FLUSH_BUFFER_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */
	));

/*

Type:  USBPUMP_UHILAUX_CACHE_INVALIDATE_BUFFER_FN

Function:
	Invalidate the contents of the cache memory

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_CACHE_INVALIDATE_BUFFER_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle
		);

Description:
	This function invalidates the contents of the cache memory to restore
	the contents of the cache memory from the physical memory.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_CACHE_INVALIDATE_BUFFER_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_SET_RANGE_FN

Function:
	Set range of give buffer handle

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_SET_RANGE_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle,
		BYTES			OffsetInBytes,
		BYTES			sizeInBytes
		);

Description:
	The range of a buffer is initialized to have an offset of zero, and
	a count of the entire size, when the buffer is created.  Therefore it
	is not necessary to call HbufferSetRange unless circumstances require
	incremental mapping of the buffer.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_SET_RANGE_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */,
	__TMS_BYTES			/* OffsetInBytes */,
	__TMS_BYTES			/* sizeInBytes */
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_GET_MAX_TRANSFER_FN

Function:
	Get maximum transfer size of give buffer handle

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_GET_MAX_TRANSFER_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle,
		BYTES *			pMaxTransferInBytes	// OUT
		);

Description:
	If an attempt to map a buffer fails (e.g., there are not enough PCI
	mapping registers available), then this function returns the number
	of segments that can actually be mapped.  This enables the HCD to
	"back off" and map a smaller portion, iteratively moving sub-ranges
	of the the buffer.  (See also HBUFFER_SET_RANGE_FN).  The HCD must
	round *pMaxTransferInBytes downwards to account for packet size
	boundaries.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_GET_MAX_TRANSFER_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */,
	__TMS_BYTES *			/* OUT pMaxTransferInBytes */
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_MAP_PIO_FN

Function:
	Map the buffer for PIO operation

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_MAP_PIO_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle,
		USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE	AccessType,
		VOID **			pRangeLogicalAddress,	// OUT
		BYTES *			pRangeLengthInBytes	// OUT
		);

Description:
	If the buffer is common memory from _AllocateCommonBuffer() then it is
	already page locked (level 1), so this function simply hands back the
	logical kernel address.

	If the buffer is user memory from _AllocateBuffer() then this function
	page locks (level 1) the buffer, and hands back the logical kernel
	address.

	If the buffer comes from an HCD request then it is already page locked
	(level 1), so this function simply hands back the logical kernel
	address.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(							\
USBPUMP_UHILAUX_HBUFFER_MAP_PIO_FN,					\
__TMS_USBPUMP_UHILAUX_STATUS,						\
	(								\
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,		\
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */,		\
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE /* AccessType */,	\
	__TMS_VOID **			/* OUT pRangeLogicalAddress */,	\
	__TMS_BYTES *			/* OUT pRangeLengthInBytes */	\
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_UNMAP_PIO_FN

Function:
	Unmap PIO buffer with buffer handle

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_UNMAP_PIO_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle
		);

Description:
	If the buffer is common memory from _AllocateCommonBuffer() then this
	function is essentially a NOP.

	If the buffer is user memory from _AllocateBuffer() then this function
	removes the page locks (level 1).

	If the buffer is from an  HCD request then this function is
	essentially a NOP.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_UNMAP_PIO_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_MAP_DMA_ASYNC_FN

Index:	Type:	USBPUMP_UHILAUX_HBUFFER_MAP_DMA_ASYNC_CB_FN

Function:
	Map the buffer for DMA operation (asynchronously)

Definition:
	typedef VOID (USBPUMP_UHILAUX_HBUFFER_MAP_DMA_ASYNC_CB_FN)(
		VOID *pClientContext,
		USBPUMP_UHILAUX_STATUS Status,
		USBPUMP_BUFFER_HANDLE hBuffer,
		USBPUMP_UHILAUX_SEGMENT *pSegmentArray,
		USBPUMP_UHILAUX_SEGMENT_COUNT nSegmentArray,
		USBPUMP_UHILAUX_SEGMENT_COUNT nSegmentsDelivered,
		USBPUMP_UHILAUX_SEGMENT_COUNT nSegmentsThisRange
		);

	typedef VOID (USBPUMP_UHILAUX_HBUFFER_MAP_DMA_ASYNC_FN)(
		USBPUMP_SESSION_HANDLE		SessionHandle,
		USBPUMP_UHILAUX_HBUFFER_MAP_DMA_ASYNC_CB_FN *pCallBackFn,
		VOID *				pCallBackContext,
		USBPUMP_BUFFER_HANDLE		hBuffer,
		USBPUMP_UHILAUX_SEGMENT *	pSegmentArray,
		USBPUMP_UHILAUX_SEGMENT_COUNT	nSegmentArray,
		USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE AccessType
		);

Description:
	This routine maps the physical memory underlying hBuffer, and
	builds a OS-specific scatter-gather list. It then copies as much of 
	the scatter-gather list into pSegmentArray, and calls the supplied
	pCallBackFn (in DataPump context, therefore from the event loop).

	In the callback, pSegmentArray and nSegmentArray are set to the
	same values passed by the caller.

	If any errors occur, Status is set to the appropriate failure code,
	and both nSegmentsDelivered and nSegmentsThisRange will be zero.

	Otherwise, pSegmentArray[0..nSegmentsDelivered-1] is set to the
	scatter-gather entries, with device-relative physical addresses.
	Sometimes the OS will prepare more entries than are available in
	pSegmentArray; in this case, nSegmentsThisRange will be greater than
	nSegmentsDelivered, and the client must call
	USBPUMP_UHILAUX_HBUFFER_GET_MORE_SEGMENTS_FN to collect the extra
	entries.

Returns:
	No explicit result.

Notes:
	The callback function is guaranteed to be called. However it
	might be called before the initial call returns, especially in 
	case of errors.

*/

__TMS_FNTYPE_DEF(							\
USBPUMP_UHILAUX_HBUFFER_MAP_DMA_ASYNC_CB_FN,				\
__TMS_VOID,								\
	(								\
	__TMS_VOID *				/* pClientContext */,	\
	__TMS_USBPUMP_UHILAUX_STATUS		/* Status */,		\
	__TMS_USBPUMP_BUFFER_HANDLE		/* BufferHandle */,	\
	__TMS_USBPUMP_UHILAUX_SEGMENT *		/* pSegmentArray */,	\
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT	/* nSegmentArray */,	\
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT	/* nSegThisArray */,	\
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT	/* nSegThisRange */	\
	));

__TMS_FNTYPE_DEF(							\
USBPUMP_UHILAUX_HBUFFER_MAP_DMA_ASYNC_FN,				\
__TMS_VOID,								\
	(								\
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,	\
	__TMS_USBPUMP_UHILAUX_HBUFFER_MAP_DMA_ASYNC_CB_FN * /* pCallBack */, \
	__TMS_VOID *				/* pCallBackContext */,	\
	__TMS_USBPUMP_BUFFER_HANDLE		/* BufferHandle */,	\
	__TMS_USBPUMP_UHILAUX_SEGMENT *		/* pSegmentArray */,	\
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT	/* nSegmentArray */,	\
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE /* AccessType */	\
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_GET_MORE_SEGMENTS_FN

Function:
	Get mapped segment array for DMA operation

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_GET_MORE_SEGMENTS_FN)(
		USBPUMP_SESSION_HANDLE		SessionHandle,
		USBPUMP_BUFFER_HANDLE		BufferHandle,
		USBPUMP_UHILAUX_SEGMENT *	pSegmentArray,
		USBPUMP_UHILAUX_SEGMENT_COUNT	nSegmentArray,
		USBPUMP_UHILAUX_SEGMENT_COUNT	StartingSegment,
		USBPUMP_UHILAUX_SEGMENT_COUNT *	pReturnedSegmentCount	// OUT
		);

Description:
	This is only used this if the segment count returned in the callback
	is less that the requested amount.  It simply allows more entries
	to be filled into a supplied scatter/gather list from the OS's
	prepared tranlastion.  It is synchronous.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(							\
USBPUMP_UHILAUX_HBUFFER_GET_MORE_SEGMENTS_FN,				\
__TMS_USBPUMP_UHILAUX_STATUS,						\
	(								\
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,	\
	__TMS_USBPUMP_BUFFER_HANDLE		/* BufferHandle */,	\
	__TMS_USBPUMP_UHILAUX_SEGMENT *		/* pSegmentArray */,	\
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT	/* nSegmentArray */,	\
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT	/* StartingSegment */,	\
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT *	/* pReturnedSegmentCount */ \
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_UNMAP_DMA_FN

Function:
	Unmap DMA buffer with buffer handle

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_UNMAP_DMA_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle
		);

Description:
	If the buffer is common memory from _AllocateCommonBuffer(), and it
	has been scatter/gather (level 2) locked then this function removes
	the scatter/gather lock (level 2).

	If the buffer is user memory from _AllocateBuffer(), and it has been
	scatter/gather level 2) locked, and page locked (level 1), then this
	function removes the scatter/gather lock (level 2), and the page lock
	(level 1).

	If the buffer is user memory passed in an HCD request, and it has been
	scatter/gather (level 2) locked, then this function removes the
	scatter/gather lock (level 2).

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_UNMAP_DMA_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_GET_SEGMENT_COUNT_FN

Function:
	Get the number of segments required to map the buffer

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_GET_SEGMENT_COUNT_FN)(
		USBPUMP_SESSION_HANDLE		SessionHandle,
		USBPUMP_BUFFER_HANDLE		BufferHandle,
		USBPUMP_UHILAUX_SEGMENT_COUNT *	pSegmentCount
		);

Description:
	This function is used by the HCD to ask the driver how many segments
	are required to map the buffer.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_GET_SEGMENT_COUNT_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE		/* BufferHandle */,
	__TMS_USBPUMP_UHILAUX_SEGMENT_COUNT *	/* pSegmentCount */
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_LOGADDR_FN

Function:
	Returns the logical (virtual kernel) address

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_LOGADDR_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle,
		VOID **			pLogicalAddress
		);

Description:
	This function provides the logical (virtual kernel) address of the
	start of a buffer for the given BufferHandle.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_LOGADDR_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */,
	__TMS_VOID **			/* pLogicalAddress */
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_DEV_REL_PHYSADDR_FN

Function:
	Returns the physical address of the start of a buffer

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_DEV_REL_PHYSADDR_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle,
		PHYSADDR *		pPhysicalAddress
		);

Description:
	This function provides the physical address of the start of a buffer
	for the given BufferHandle.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_DEV_REL_PHYSADDR_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */,
	__TMS_PHYSADDR *		/* pPhysicalAddress */
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_LENGTH_FN

Function:
	Returns the length of a buffer, in bytes

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_LENGTH_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle,
		BYTES *			pLength		// OUT
		);

Description:
	This function provides the length of a buffer, in bytes.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_LENGTH_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */,
	__TMS_BYTES *			/* OUT pLength */
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_LOG_TO_DEV_REL_PHYS_FN

Function:
	Converts a logical address to a physical address

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_LOG_TO_DEV_REL_PHYS_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle,
		VOID *			LogicalAddress,
		PHYSADDR *		pPhysicalAddress	// OUT
		);

Description:
	This function converts a logical (virtual kernel) address to a physical
	address.  It fails if called on a buffer obtained via
	_AllocateUserBuffer() and that buffer has not been page locked
	(level 1).

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_LOG_TO_DEV_REL_PHYS_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */,
	__TMS_VOID *			/* LogicalAddress */,
	__TMS_PHYSADDR *		/* OUT pPhysicalAddress */
	));

/*

Type:  USBPUMP_UHILAUX_HBUFFER_DEV_REL_PHYS_TO_LOG_FN

Function:
	Converts a physical address to a logical address

Definition:
	typedef USBPUMP_UHILAUX_STATUS
	(*USBPUMP_UHILAUX_HBUFFER_DEV_REL_PHYS_TO_LOG_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle,
		USBPUMP_BUFFER_HANDLE	BufferHandle,
		PHYSADDR		physicalAddress,
		VOID **			pLogicalAddressInBuffer	// OUT
		);

Description:
	This function converts a physical address to a logical (virtual
	kernel) address.

Returns:
	USBPUMP_UHILAUX_STATUS

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_DEV_REL_PHYS_TO_LOG_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */,
	__TMS_PHYSADDR			/* physicalAddress */,
	__TMS_VOID **			/* OUT pLogicalAddress */
	));

/*

Name:	USBPUMP_UHILAUX_HBUFFER_ADD_REFERENCE_FN

Function:
	Add a reference to an HBUFFER.

Definition:
	typedef USBPUMP_UHILAUX_STATUS
		USBPUMP_UHILAUX_HBUFFER_ADD_REFERENCE_FN(
			USBPUMP_SESSION_HANDLE hSession,
			USBPUMP_BUFFER_HANDLE hBuffer
			);

Description:
	This API increments the reference count in the HBuffer.  The
	hBuffer is not freed until the reference count is reduced to zero
	by a call to USBPUMP_UHILAUX_FREE_BUFFER_FN

Returns:
	Status.  If handle is valid, this always succeeds.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_ADD_REFERENCE_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE	/* BufferHandle */
	));

/*

Name:	USBPUMP_UHILAUX_HBUFFER_ENTER_SCOPE_FN

Function:
	Convenient utility for adding HBUFFER wrappers to internal buffers

Definition:
	typedef USBPUMP_UHILAUX_STATUS
		USBPUMP_UHILAUX_HBUFFER_ENTER_SCOPE_FN(
			USBPUMP_SESSION_HANDLE hSession,
			USBPUMP_BUFFER_HANDLE *phBuffer,
			ADDRBITS vaBuffer,
			BYTES nBuffer,
			USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE access
			);

Description:
	This API is a general purpose utility for connecting non-HBUFFER-
	aware upper clients to HBUFFER-aware services.  Non-HBUFFER-aware
	clients will always provide an "always valid" pointer to a buffer,
	and a NULL buffer handle.  HBUFFER-aware services don't like
	NULL buffer handles; they explicitly manage access to the buffer
	by mapping and unmapping.  At the boudary, therefore, we need to
	wrap client buffers in buffer handles (on the way in) and unwrap
	them on the way out.

	The mechanism is simple.  At an appropriate place (HCD submit request
	or USBD submit request), where it's easy to know the desired access
	from the request structure, and where the HBUFFER is in the user's
	request block, just call ENTER_SCOPE, passing the user's buffer
	pointer, size, and a pointer to the HBUFFER in the request block.

	If the client is not HBUFFER-aware, then the referenced HBUFFER cell
	will be NULL; this routine then allocates an HBUFFER to wrap the
	specified buffer, and puts the pointer in the refernenced cell.  On
	the other hand, if the client is HBUFFER-aware, then *phBuffer will
	already have a buffer handle.  This routine increaments that
	hBuffer's ref count and returns.

	In the exit path from the API (USBD complete request or HCD complete
	request), you must also have a matching call to
	USBPUMP_UHILAUX_HBUFFER_EXIT_SCOPE_FN.  If the HBUFFER was generated
	by the matching call to ENTER_SCOPE, (i.e., if the reference count
	is now zero) then it will be freed.  So that the HBUFFER doesn't 
	leak back to the client, the EXIT_SCOPE API NULLs it out if it 
	closes it.  Otherwise *phBuffer is left unchanged.

	Because of CONST/non-CONST casting issues, we pass pBuffer as
	a ADDRBITS value rather than as a VOID*.

Returns:
	Status.

*/


__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_ENTER_SCOPE_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE *	/* pointer to BufferHandle */,
	__TMS_ADDRBITS			/* vaBuffer */,
	__TMS_BYTES			/* nBuffer */,
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE
	));

/*

Name:	USBPUMP_UHILAUX_HBUFFER_EXIT_SCOPE_FN

Function:
	Release a reference to a buffer handle, and close it if ref count
	drops to zero

Definition:
	typedef USBPUMP_UHILAUX_STATUS
		USBPUMP_UHILAUX_HBUFFER_EXIT_SCOPE_FN(
			USBPUMP_SESSION_HANDLE hSession,
			USBPUMP_BUFFER_HANDLE *phBuffer
			);

Description:
	This API is always used in conjunction with
	USBPUMP_UHILAUX_HBUFFER_ENTER_SCOPE_FN.

	This API decrements the reference count in the hBuffer at *phBuffer.
	If the reference count drops to zero, the cell at *phBuffer will
	be cleared, otherwise it's left unchanged.

Returns:
	Normal status.  If the buffer handle at *phBuffer is valid, then
	this always succeeds.

See also:
	USBPUMP_UHILAUX_HBUFFER_ENTER_SCOPE_FN.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_HBUFFER_EXIT_SCOPE_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE *	/* pointer to BufferHandle */
	));


/*

Name:	USBPUMP_UHILAUX_CREATE_HBUFFER_WITH_SEGMENT_FN

Function:
	Convenient utility for creating HBUFFER wrappers to internal buffers

Definition:
	typedef USBPUMP_UHILAUX_STATUS
		USBPUMP_UHILAUX_CREATE_HBUFFER_WITH_SEGMENT_FN(
			USBPUMP_SESSION_HANDLE			hSession,
			USBPUMP_BUFFER_HANDLE *			phBuffer,
			UINT					numSegments,
			ADDRBITS				vaBuffer1,
			BYTES					nSegBuffer1,
			ADDRBITS				vaBuffer2,
			BYTES					nSegBuffer2,
			USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE 	Access
			);

Description:
	This API is similar with USBPUMP_UHILAUX_HBUFFER_ENTER_SCOPE_FN but
	difference is caller provide a collection of buffer segments (which
	you'd describe as an array of pointer/length pairs).

	The mechanism is simple.  At an appropriate place (HCD submit request
	or USBD submit request), where it's easy to know the desired access
	from the request structure, and where the HBUFFER is in the user's
	request block, just call CREATE_HBUFFER_WITH_SEGMENT, passing the
	user's two buffer pointer, size, number of segment, and a pointer to
	the HBUFFER in the request block.

	The numSegments input parameter is number of segment of the two buffer.
	The vaBuffer1 and nSegBuffer1 is describeing first buffer address and
	segmented buffer size. The vaBuffer2 and nSegBuffer2 is describeing
	second buffer address and segmented buffer size.

	The referenced HBUFFER cell should be NULL; this routine then allocates
	an HBUFFER to wrap the specified segmented buffer, and puts the pointer
	in the refernenced cell.

	In the exit path from the API (USBD complete request or HCD complete
	request), you must also have a matching call to
	USBPUMP_UHILAUX_HBUFFER_EXIT_SCOPE_FN.  If the HBUFFER was generated
	by the matching call to ENTER_SCOPE, (i.e., if the reference count
	is now zero) then it will be freed.  So that the HBUFFER doesn't 
	leak back to the client, the EXIT_SCOPE API NULLs it out if it 
	closes it.  Otherwise *phBuffer is left unchanged.

	Because of CONST/non-CONST casting issues, we pass pBuffer as
	a ADDRBITS value rather than as a VOID*.

Returns:
	Status.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_CREATE_HBUFFER_WITH_SEGMENT_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE *		/* pointer to BufferHandle */,
	__TMS_UINT				/* numSegments */,
	__TMS_ADDRBITS				/* vaBuffer1 */,
	__TMS_BYTES				/* nSegBuffer1 */,
	__TMS_ADDRBITS				/* vaBuffer2 */,
	__TMS_BYTES				/* nSegBuffer2 */,
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE /* Access */
	));

/*

Name:	USBPUMP_UHILAUX_CREATE_SEGMENT_HBUFFER_FN

Function:
	Create HBUFFER with given buffer and number of segments

Definition:
	typedef USBPUMP_UHILAUX_STATUS
		USBPUMP_UHILAUX_CREATE_SEGMENT_HBUFFER_FN(
			USBPUMP_SESSION_HANDLE			hSession,
			USBPUMP_BUFFER_HANDLE *			phBuffer,
			UINT					numSegments,
			ADDRBITS				vaBuffer,
			BYTES					nBuffer,
			USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE 	Access
			);

Description:
	This API is similar with USBPUMP_UHILAUX_HBUFFER_ENTER_SCOPE_FN but
	difference is caller provide a buffer segment and numer of total
	segments to add in this buffer handle. This HBUFFER can be used to
	create scatter-gather buffer list.

	The mechanism is simple.  At an appropriate place (HCD submit request
	or USBD submit request), where it's easy to know the desired access
	from the request structure, and where the HBUFFER is in the user's
	request block, just call USBPUMP_UHILAUX_CREATE_SEGMENT_HBUFFER_FN,
	passing the user's buffer pointer, size, number of segment, and a
	pointer to the HBUFFER in the request block.

	The numSegments input parameter is number of total segment of buffer.
	User can add more buffers using USBPUMP_UHILAUX_SEGMENT_HBUFFER_ADD_FN
	call. User can add (numSegments - 1) more buffers.

	The referenced HBUFFER cell should be NULL; this routine then allocates
	an HBUFFER to wrap the specified segmented buffer, and puts the pointer
	in the refernenced cell.

	In the exit path from the API (USBD complete request or HCD complete
	request), you must also have a matching call to
	USBPUMP_UHILAUX_HBUFFER_EXIT_SCOPE_FN.  If the HBUFFER was generated
	by the matching call to ENTER_SCOPE, (i.e., if the reference count
	is now zero) then it will be freed.  So that the HBUFFER doesn't 
	leak back to the client, the EXIT_SCOPE API NULLs it out if it 
	closes it.  Otherwise *phBuffer is left unchanged.

	Because of CONST/non-CONST casting issues, we pass pBuffer as
	a ADDRBITS value rather than as a VOID*.

Returns:
	Status.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_CREATE_SEGMENT_HBUFFER_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE *		/* pointer to BufferHandle */,
	__TMS_UINT				/* numSegments */,
	__TMS_ADDRBITS				/* vaBuffer */,
	__TMS_BYTES				/* nBuffer */,
	__TMS_USBPUMP_UHILAUX_BUFFER_ACCESS_TYPE /* Access */
	));

/*

Name:	USBPUMP_UHILAUX_SEGMENT_HBUFFER_ADD_FN

Function:
	Add a buffer to given segment HBUFFER.

Definition:
	typedef USBPUMP_UHILAUX_STATUS
		USBPUMP_UHILAUX_SEGMENT_HBUFFER_ADD_FN(
			USBPUMP_SESSION_HANDLE			hSession,
			USBPUMP_BUFFER_HANDLE			hBuffer,
			ADDRBITS				vaBuffer,
			BYTES					nBuffer
			);

Description:
	This API adds a buffer to given segment HBUFFER. User can add
	(numSegments - 1) more buffers.  The referenced HBUFFER cell should
	not be NULL.

	Because of CONST/non-CONST casting issues, we pass pBuffer as
	a ADDRBITS value rather than as a VOID*.

Returns:
	Status.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_SEGMENT_HBUFFER_ADD_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE		/* BufferHandle */,
	__TMS_ADDRBITS				/* vaBuffer */,
	__TMS_BYTES				/* nBuffer */
	));

/*

Name:	USBPUMP_UHILAUX_SEGMENT_HBUFFER_RESET_FN

Function:
	Reset segment HBUFFER.

Definition:
	typedef USBPUMP_UHILAUX_STATUS
		USBPUMP_UHILAUX_SEGMENT_HBUFFER_RESET_FN(
			USBPUMP_SESSION_HANDLE			hSession,
			USBPUMP_BUFFER_HANDLE			hBuffer
			);

Description:
	This API resets segment HBUFFER and it removes all added buffer
	segments using USBPUMP_UHILAUX_SEGMENT_HBUFFER_ADD_FN. So HBUFFER has
	only one buffer which is same buffer when create segment HBUFFER.

Returns:
	Status.

*/

__TMS_FNTYPE_DEF(
USBPUMP_UHILAUX_SEGMENT_HBUFFER_RESET_FN,
__TMS_USBPUMP_UHILAUX_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,
	__TMS_USBPUMP_BUFFER_HANDLE		/* BufferHandle */
	));

/**** end of usbpump_uhilaux_types.h ****/
#endif /* _USBPUMP_UHILAUX_TYPES_H_ */
