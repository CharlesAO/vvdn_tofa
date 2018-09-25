/* usbpump_mempool.h	Fri Apr 24 2015 10:39:49 chwon */

/*

Module:  usbpump_mempool.h

Function:
	Definitions for the abstract MALLOC facility for the DataPump

Version:
	V3.15b	Fri Apr 24 2015 10:39:49 chwon	Edit level 11

Copyright notice:
	This file copyright (C) 2005-2008, 2012, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Fri Jun 24 2005 13:28:28  tmm
	Module created.

   1.91h  2005-08-14  tmm
	Move some definitions to central types file.

   1.93c  Thu Dec 08 2005 22:25:48  chwon
	Added UsbPumpPool_AllocZero prototype

   1.93e  Wed Feb 01 2006 13:16:01  chwon
	Add USBPUMP_POOL_INFO_FN method

   1.93e  Thu Feb 23 2006 00:45:44  tmm
	Add UsbPumpPool_Close() and UsbPumpPool_Reset().

   1.97i  Mon Jun 04 2007 16:21:37  chwon
	2930: Add UsbPumpPool_AllocCacheAligned() support

   1.97j  Thu Nov 08 2007 13:33:11  chwon
	3454: UsbPumpPool_AllocAligned() support

   1.97k  Tue Oct 14 2008 13:50:18  chwon
	6860: Add abstract pool validation support

   3.11a  Mon Aug 20 2012 15:35:41  chwon
	15828: use __TMS_USBPUMP_MAKE_ALIGN_POINTER().

   3.15b  Fri Apr 24 2015 10:39:49  chwon
	19143: Add USBPUMP_MEMORY_BLOCK_CACHE_ALIGNMENT definition.

*/

#ifndef _USBPUMP_MEMPOOL_H_		/* prevent multiple includes */
#define _USBPUMP_MEMPOOL_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _UHILALIGN_H_
# include "uhilalign.h"
#endif

/****************************************************************************\
|
|	The abstract memory pool structure
|
\****************************************************************************/

/* in usbpump_types.h": __TMS_TYPE_DEF_UNION(USBPUMP_ABSTRACT_POOL); */
__TMS_TYPE_DEF_STRUCT(USBPUMP_ABSTRACT_POOL_CONTENTS);
__TMS_TYPE_DEF_STRUCT(USBPUMP_ABSTRACT_POOL_INFO);

__TMS_FNTYPE_DEF(USBPUMP_POOL_ALLOC_FN,
__TMS_VOID *,
	(
	__TMS_USBPUMP_ABSTRACT_POOL *,
	__TMS_ADDRBITS
	));

__TMS_FNTYPE_DEF(USBPUMP_POOL_REALLOC_FN,
__TMS_VOID *,
	(
	__TMS_USBPUMP_ABSTRACT_POOL *,
	__TMS_VOID *,
	__TMS_ADDRBITS
	));

__TMS_FNTYPE_DEF(USBPUMP_POOL_FREE_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_ABSTRACT_POOL *,
	__TMS_VOID *
	));

__TMS_FNTYPE_DEF(USBPUMP_POOL_RESET_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_ABSTRACT_POOL *
	));

__TMS_FNTYPE_DEF(USBPUMP_POOL_CLOSE_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_ABSTRACT_POOL *
	));

__TMS_FNTYPE_DEF(USBPUMP_POOL_INFO_FN,
__TMS_BOOL,
	(
	__TMS_USBPUMP_ABSTRACT_POOL *,
	__TMS_USBPUMP_ABSTRACT_POOL_INFO *
	));

/* a macros that gets us to the abstract pool header from the block */
#define	__TMS_USBPUMP_MEMORY_BLOCK_GET_ABSTRACT_POOL(pv)	\
	((__TMS_USBPUMP_ABSTRACT_POOL *)(((__TMS_ADDRBITS *)(pv))[-1]))

/* a macros that validate memory block pointer */
#define __TMS_USBPUMP_MEMORY_BLOCK_IS_VALID(pv)			\
	(((pv) != __TMS_NULL) &&				\
	 (((__TMS_ADDRBITS)(pv) & (sizeof(__TMS_ADDRBITS) - 1)) == 0))


#define __TMS_USBPUMP_MEMORY_BLOCK_MAKE_ALIGNMENT(p, alignment)	\
	__TMS_USBPUMP_MAKE_ALIGN_POINTER(p, __TMS_ADDRBITS, alignment)

#define	__TMS_USBPUMP_MEMORY_BLOCK_ALIGN_MAGIC_MASK	0x0000FFFF
#define __TMS_USBPUMP_MEMORY_BLOCK_ALIGN_MAGIC_CODE	0x0000cabf

#define	__TMS_USBPUMP_MEMORY_BLOCK_IS_ALIGNED_MEMBLOCK(pv)		\
	((((__TMS_ADDRBITS *)(pv))[-1] & __TMS_USBPUMP_MEMORY_BLOCK_ALIGN_MAGIC_MASK) \
	 == __TMS_USBPUMP_MEMORY_BLOCK_ALIGN_MAGIC_CODE)

#define	__TMS_USBPUMP_MEMORY_BLOCK_SET_ALIGNMENT_OFFSET(pv, offset)	\
	((__TMS_ADDRBITS *)(pv))[-1] =					\
		(__TMS_ADDRBITS)(offset) << 16 |			\
		__TMS_USBPUMP_MEMORY_BLOCK_ALIGN_MAGIC_CODE

#define	__TMS_USBPUMP_MEMORY_BLOCK_GET_ALIGNMENT_OFFSET(pv)		\
	(((__TMS_ADDRBITS *)(pv))[-1] >> 16)

#define	__TMS_USBPUMP_MEMORY_BLOCK_GET_ORIGINAL_MEMBLOCK(pv)		\
	((__TMS_VOID *)(((__TMS_UINT8 *)(pv)) -				\
	                __TMS_USBPUMP_MEMORY_BLOCK_GET_ALIGNMENT_OFFSET(pv)))

#define	__TMS_USBPUMP_ABSTRACT_POOL_MAGIC_BEGIN		(0x55415000u + '<')
#define	__TMS_USBPUMP_ABSTRACT_POOL_MAGIC_END		(0x05145500u + '>')

#define	__TMS_USBPUMP_ABSTRACT_POOL_IS_VALID(pPool)			\
	((pPool) != __TMS_NULL &&					\
	 (pPool)->AbstractPool.MagicBegin == __TMS_USBPUMP_ABSTRACT_POOL_MAGIC_BEGIN && \
	 (pPool)->AbstractPool.MagicEnd == __TMS_USBPUMP_ABSTRACT_POOL_MAGIC_END)

#ifndef	__TMS_USBPUMP_MEMORY_BLOCK_CACHE_ALIGNMENT	/*PARAM*/
# define __TMS_USBPUMP_MEMORY_BLOCK_CACHE_ALIGNMENT	32
#endif


struct __TMS_STRUCTNAME(USBPUMP_ABSTRACT_POOL_CONTENTS)
	{
	__TMS_UINT32				MagicBegin;

	__TMS_VOID *				pContext;

	/*
	|| Allocate has to be here, because objects are issued from here.
	*/
	__TMS_USBPUMP_POOL_ALLOC_FN *		pAllocFn;

	/*
	|| the following are methods of an individual block -- you
	|| normally get here through the block header, not directly.
	*/
	__TMS_USBPUMP_POOL_REALLOC_FN *		pReallocFn;
	__TMS_USBPUMP_POOL_FREE_FN *		pFreeFn;

	/*
	|| the following method resets an absract pool -- this is handy
	|| for subpools created for device drivers.  Basically all blocks
	|| allocated from this pool are freed at once.
	*/
	__TMS_USBPUMP_POOL_RESET_FN *		pResetFn;

	/*
	|| the following method is used for closing this object, when the
	|| pool object is no longer to be used (as on task exit)
	*/
	__TMS_USBPUMP_POOL_CLOSE_FN *		pCloseFn;

	/*
	|| the following method is used for getting the pool object information
	*/
	__TMS_USBPUMP_POOL_INFO_FN *		pInfoFn;

	__TMS_UINT32				MagicEnd;
	};

/**** the usual mechanisms ****/
union __TMS_UNIONNAME(USBPUMP_ABSTRACT_POOL)
	{
	__TMS_USBPUMP_ABSTRACT_POOL_CONTENTS	AbstractPool;
	};

#define __TMS_USBPUMP_ABSTRACT_POOL_CONTENTS__STRUCT		\
	__TMS_USBPUMP_ABSTRACT_POOL_CONTENTS	AbstractPool

#define __TMS_USBPUMP_ABSTRACT_POOL_CONTENTS__UNION		\
	__TMS_USBPUMP_ABSTRACT_POOL_CONTENTS	AbstractPool;	\
	__TMS_USBPUMP_ABSTRACT_POOL		AbstractPoolCast

#define	__TMS_USBPUMP_ABSTRACT_POOL_INIT_V1(			\
	a_pContext,						\
	a_pAllocFn,						\
	a_pReallocFn,						\
	a_pFreeFn,						\
	a_pResetFn,						\
	a_pCloseFn,						\
	a_pInfoFn						\
	)							\
	{{							\
	__TMS_USBPUMP_ABSTRACT_POOL_MAGIC_BEGIN,		\
	(a_pContext),						\
	(a_pAllocFn),						\
	(a_pReallocFn),						\
	(a_pFreeFn),						\
	(a_pResetFn),						\
	(a_pCloseFn),						\
	(a_pInfoFn),						\
	__TMS_USBPUMP_ABSTRACT_POOL_MAGIC_END			\
	}}

#define	__TMS_USBPUMP_ABSTRACT_POOL_SETUP_V1(			\
	a_pPool,						\
	a_pContext,						\
	a_pAllocFn,						\
	a_pReallocFn,						\
	a_pFreeFn,						\
	a_pResetFn,						\
	a_pCloseFn,						\
	a_pInfoFn						\
	)							\
    do	{							\
	(a_pPool)->AbstractPool.MagicBegin =			\
		__TMS_USBPUMP_ABSTRACT_POOL_MAGIC_BEGIN;	\
	(a_pPool)->AbstractPool.pContext = (a_pContext);	\
	(a_pPool)->AbstractPool.pAllocFn = (a_pAllocFn);	\
	(a_pPool)->AbstractPool.pReallocFn = (a_pReallocFn);	\
	(a_pPool)->AbstractPool.pFreeFn = (a_pFreeFn);		\
	(a_pPool)->AbstractPool.pResetFn = (a_pResetFn);	\
	(a_pPool)->AbstractPool.pCloseFn = (a_pCloseFn);	\
	(a_pPool)->AbstractPool.pInfoFn = (a_pInfoFn);	       	\
	(a_pPool)->AbstractPool.MagicEnd =			\
		__TMS_USBPUMP_ABSTRACT_POOL_MAGIC_END;		\
	} while (0)


struct __TMS_STRUCTNAME(USBPUMP_ABSTRACT_POOL_INFO)
	{
	/* the total amount of memory pool, in bytes */
	__TMS_BYTES		TotalBytes;

	/* the total amount of free memory, in bytes */
	__TMS_BYTES		FreeBytes;

	/* the number of free blocks */
	__TMS_BYTES		nFreeBlock;

	/* the size of the larges free block in bytes */
	__TMS_BYTES		LargestBlock;
	};

/****************************************************************************\
|
|	Functions, etc.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/* DataPump memory block API */
__TMS_VOID	UsbPumpMemoryBlock_Free(
			__TMS_VOID *			pMemoryBlock
			);

__TMS_USBPUMP_ABSTRACT_POOL *
		UsbPumpMemoryBlock_GetAbstractPool(
			__TMS_VOID *			pMemoryBlock
			);

__TMS_VOID *	UsbPumpMemoryBlock_Realloc(
			__TMS_VOID *			pMemoryBlock,
			__TMS_ADDRBITS			NewSize
			);

/* DataPump abstract memory pool API */
__TMS_VOID *	UsbPumpPool_Alloc(
			__TMS_USBPUMP_ABSTRACT_POOL *	pAbstractPool,
			__TMS_ADDRBITS			Size
			);

__TMS_VOID *	UsbPumpPool_AllocAligned(
			__TMS_USBPUMP_ABSTRACT_POOL *	pAbstractPool,
			__TMS_ADDRBITS			Size,
			__TMS_ADDRBITS			Alignment
			);

__TMS_VOID *	UsbPumpPool_AllocCacheAligned(
			__TMS_USBPUMP_ABSTRACT_POOL *	pAbstractPool,
			__TMS_ADDRBITS			Size
			);

__TMS_VOID *	UsbPumpPool_AllocZero(
			__TMS_USBPUMP_ABSTRACT_POOL *	pAbstractPool,
			__TMS_ADDRBITS			Size
			);

__TMS_VOID	UsbPumpPool_Close(
			__TMS_USBPUMP_ABSTRACT_POOL *	pAbstractPool
			);

__TMS_VOID	UsbPumpPool_Free(
			__TMS_USBPUMP_ABSTRACT_POOL *	pAbstractPool,
			__TMS_VOID *			pMemoryBlock
			);

__TMS_BOOL	UsbPumpPool_Info(
			__TMS_USBPUMP_ABSTRACT_POOL *	pAbstractPool,
			__TMS_USBPUMP_ABSTRACT_POOL_INFO *pPoolInfo
			);

__TMS_VOID *	UsbPumpPool_Realloc(
			__TMS_USBPUMP_ABSTRACT_POOL *	pAbstractPool,
			__TMS_VOID *			pMemoryBlock,
			__TMS_ADDRBITS			NewSize
			);

__TMS_VOID	UsbPumpPool_Reset(
			__TMS_USBPUMP_ABSTRACT_POOL *	pAbstractPool
			);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_MEMORY_BLOCK_GET_ABSTRACT_POOL(pv)	\
   __TMS_USBPUMP_MEMORY_BLOCK_GET_ABSTRACT_POOL(pv)
# define USBPUMP_MEMORY_BLOCK_IS_VALID(pv)	\
   __TMS_USBPUMP_MEMORY_BLOCK_IS_VALID(pv)
# define USBPUMP_MEMORY_BLOCK_MAKE_ALIGNMENT(p, alignment)	\
   __TMS_USBPUMP_MEMORY_BLOCK_MAKE_ALIGNMENT(p, alignment)
# define USBPUMP_MEMORY_BLOCK_ALIGN_MAGIC_MASK	\
   __TMS_USBPUMP_MEMORY_BLOCK_ALIGN_MAGIC_MASK
# define USBPUMP_MEMORY_BLOCK_ALIGN_MAGIC_CODE	\
   __TMS_USBPUMP_MEMORY_BLOCK_ALIGN_MAGIC_CODE
# define USBPUMP_MEMORY_BLOCK_IS_ALIGNED_MEMBLOCK(pv)	\
   __TMS_USBPUMP_MEMORY_BLOCK_IS_ALIGNED_MEMBLOCK(pv)
# define USBPUMP_MEMORY_BLOCK_SET_ALIGNMENT_OFFSET(pv, offset)	\
   __TMS_USBPUMP_MEMORY_BLOCK_SET_ALIGNMENT_OFFSET(pv, offset)
# define USBPUMP_MEMORY_BLOCK_GET_ALIGNMENT_OFFSET(pv)	\
   __TMS_USBPUMP_MEMORY_BLOCK_GET_ALIGNMENT_OFFSET(pv)
# define USBPUMP_MEMORY_BLOCK_GET_ORIGINAL_MEMBLOCK(pv)	\
   __TMS_USBPUMP_MEMORY_BLOCK_GET_ORIGINAL_MEMBLOCK(pv)
# define USBPUMP_ABSTRACT_POOL_MAGIC_BEGIN	\
   __TMS_USBPUMP_ABSTRACT_POOL_MAGIC_BEGIN
# define USBPUMP_ABSTRACT_POOL_MAGIC_END	\
   __TMS_USBPUMP_ABSTRACT_POOL_MAGIC_END
# define USBPUMP_ABSTRACT_POOL_IS_VALID(pPool)	\
   __TMS_USBPUMP_ABSTRACT_POOL_IS_VALID(pPool)
# define USBPUMP_MEMORY_BLOCK_CACHE_ALIGNMENT	\
   __TMS_USBPUMP_MEMORY_BLOCK_CACHE_ALIGNMENT
# define USBPUMP_ABSTRACT_POOL_CONTENTS__STRUCT	\
   __TMS_USBPUMP_ABSTRACT_POOL_CONTENTS__STRUCT
# define USBPUMP_ABSTRACT_POOL_CONTENTS__UNION	\
   __TMS_USBPUMP_ABSTRACT_POOL_CONTENTS__UNION
# define USBPUMP_ABSTRACT_POOL_INIT_V1(			\
	a_pContext,						\
	a_pAllocFn,						\
	a_pReallocFn,						\
	a_pFreeFn,						\
	a_pResetFn,						\
	a_pCloseFn,						\
	a_pInfoFn						\
	)	\
	__TMS_USBPUMP_ABSTRACT_POOL_INIT_V1(			\
	a_pContext,						\
	a_pAllocFn,						\
	a_pReallocFn,						\
	a_pFreeFn,						\
	a_pResetFn,						\
	a_pCloseFn,						\
	a_pInfoFn						\
	)
# define USBPUMP_ABSTRACT_POOL_SETUP_V1(			\
	a_pPool,						\
	a_pContext,						\
	a_pAllocFn,						\
	a_pReallocFn,						\
	a_pFreeFn,						\
	a_pResetFn,						\
	a_pCloseFn,						\
	a_pInfoFn						\
	)	\
	__TMS_USBPUMP_ABSTRACT_POOL_SETUP_V1(			\
	a_pPool,						\
	a_pContext,						\
	a_pAllocFn,						\
	a_pReallocFn,						\
	a_pFreeFn,						\
	a_pResetFn,						\
	a_pCloseFn,						\
	a_pInfoFn						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_mempool.h ****/
#endif /* _USBPUMP_MEMPOOL_H_ */
