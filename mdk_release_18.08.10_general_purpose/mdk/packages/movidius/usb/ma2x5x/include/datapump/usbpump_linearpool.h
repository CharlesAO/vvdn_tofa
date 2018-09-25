/* usbpump_linearpool.h	Mon Aug 20 2012 15:31:02 chwon */

/*

Module:  usbpump_linearpool.h

Function:
	Definitions for the default DataPump memory allocation package

Version:
	V3.11a	Mon Aug 20 2012 15:31:02 chwon	Edit level 7

Copyright notice:
	This file copyright (C) 2005-2006, 2008, 2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	July 2005

Revision history:
   1.91d  Tue Jul 05 2005 22:49:02  chwon
	Module created.

   1.93a  Fri Oct 07 2005 09:39:28  chwon
	Pull out union definition inside of struct

   1.93e  Wed Feb 01 2006 13:14:57  chwon
	Add USBPUMP_POOL_INFO_FN method

   1.93e  Sat Mar  4 2006 12:19:13  tmm
	Bug 1688: add parentheseis on USBPUMP_LINERAR_POOL_BLOCK_ROUND_SIZE().

   1.97k  Wed Nov 26 2008 15:03:48  chwon
	6954: add compile time override for UsbPumpLinearPool_Create() and
	UsbPumpLinearPool_Initialize() function

   1.97k  Mon Jan 05 2009 15:45:33  chwon
	6954: add function prototype for overriding linear pool creation and
	initialization function.

   3.11a  Mon Aug 20 2012 15:31:02  chwon
	15828: use __TMS_USBPUMP_ALIGN_SIZE().

*/

#ifndef _USBPUMP_LINEARPOOL_H_		/* prevent multiple includes */
#define _USBPUMP_LINEARPOOL_H_

#ifndef _USBPUMP_MEMPOOL_H_
# include "usbpump_mempool.h"
#endif

#ifndef _UHILALIGN_H_
# include "uhilalign.h"
#endif

/****************************************************************************\
|
|	The linear pool memory block structure
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_LINEAR_POOL_BLOCK);
__TMS_TYPE_DEF_UNION(USBPUMP_LINEAR_POOL_BLOCK_INFO);

union __TMS_UNIONNAME(USBPUMP_LINEAR_POOL_BLOCK_INFO)
	{
	/* pointer to next in chain */
	__TMS_USBPUMP_LINEAR_POOL_BLOCK *	pNext;

	/* the memory block information */
	__TMS_TEXT				MemoryBlock[1];
	};

struct __TMS_STRUCTNAME(USBPUMP_LINEAR_POOL_BLOCK)
	{
	/* Size of the block, in bytes, including waste space and the header */
	__TMS_ADDRBITS				lpb_Size;

	/* Base address of the pool-header, as a UINT_PTR. */
	__TMS_ADDRBITS				lpb_PoolHeader;

	__TMS_USBPUMP_LINEAR_POOL_BLOCK_INFO	lpb_Info;
	};

/**** sane names ****/
#define	lpb_pNext				lpb_Info.pNext
#define	lpb_MemoryBlock				lpb_Info.MemoryBlock


#define	__TMS_USBPUMP_LINEAR_POOL_BLOCK_HEADER_SIZE		\
	__TMS_OFFSET(__TMS_USBPUMP_LINEAR_POOL_BLOCK, lpb_MemoryBlock[0])

#define	__TMS_USBPUMP_LINEAR_POOL_BLOCK_MIN_SIZE		\
	(__TMS_USBPUMP_LINEAR_POOL_BLOCK_HEADER_SIZE * 2)

#define	__TMS_USBPUMP_LINEAR_POOL_BLOCK_ALIGN_SIZE(s)		\
	__TMS_USBPUMP_ALIGN_SIZE(s)

#define	__TMS_USBPUMP_LINEAR_POOL_BLOCK_ROUND_SIZE(s)		\
	(((s) < __TMS_USBPUMP_LINEAR_POOL_BLOCK_MIN_SIZE)	\
		? __TMS_USBPUMP_LINEAR_POOL_BLOCK_ALIGN_SIZE(	\
		    __TMS_USBPUMP_LINEAR_POOL_BLOCK_MIN_SIZE)	\
		: __TMS_USBPUMP_LINEAR_POOL_BLOCK_ALIGN_SIZE(s))

#define	__TMS_USBPUMP_LINEAR_POOL_BLOCK_GET_NEXT(p, s)		\
	(__TMS_USBPUMP_LINEAR_POOL_BLOCK *)((__TMS_TEXT *)(p) + (s))


/****************************************************************************\
|
|	The linear pool structure
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION(USBPUMP_LINEAR_POOL);
__TMS_TYPE_DEF_STRUCT(USBPUMP_LINEAR_POOL_CONTENTS);

struct __TMS_STRUCTNAME(USBPUMP_LINEAR_POOL_CONTENTS)
	{
	__TMS_USBPUMP_ABSTRACT_POOL_CONTENTS__STRUCT;

	/* Base address of the buffer */
	__TMS_VOID *				pBase;

	/* Number of bytes in use (raw) */
	__TMS_ADDRBITS				nBytesInUse;

	/* Number of bytes total in the pool */
	__TMS_ADDRBITS				nBytesTotal;

	__TMS_USBPUMP_LINEAR_POOL_BLOCK		FreeList;
	};

union __TMS_UNIONNAME(USBPUMP_LINEAR_POOL)
	{
	__TMS_USBPUMP_ABSTRACT_POOL_CONTENTS__UNION;

	__TMS_USBPUMP_LINEAR_POOL_CONTENTS	LinearPool;
	};


/****************************************************************************\
|
|	Functions, etc.
|
\****************************************************************************/

#ifdef	UsbPumpLinearPool_Create
__TMS_USBPUMP_LINEAR_POOL *UsbPumpLinearPool_Create(
			__TMS_VOID *			pPoolBuffer,
			__TMS_ADDRBITS			SizePoolBuffer
			);
#else
# define UsbPumpLinearPool_Create(pPoolBuffer, SizePoolBuffer)		\
	UsbPumpLinearPool_Create_Default(pPoolBuffer, SizePoolBuffer)
#endif

#ifdef	UsbPumpLinearPool_Initialize
__TMS_VOID	UsbPumpLinearPool_Initialize(
			__TMS_USBPUMP_LINEAR_POOL *	pLinearPool,
			__TMS_VOID *			pPoolBuffer,
			__TMS_ADDRBITS			SizePoolBuffer
			);
#else
# define UsbPumpLinearPool_Initialize(pLinearPool, pPoolBuf, nPoolBuf)	\
	UsbPumpLinearPool_Initialize_Default(pLinearPool, pPoolBuf, nPoolBuf)
#endif


__TMS_BEGIN_DECLS

__TMS_USBPUMP_LINEAR_POOL *UsbPumpLinearPool_Create_Default(
			__TMS_VOID *			pPoolBuffer,
			__TMS_ADDRBITS			SizePoolBuffer
			);

__TMS_VOID	UsbPumpLinearPool_Initialize_Default(
			__TMS_USBPUMP_LINEAR_POOL *	pLinearPool,
			__TMS_VOID *			pPoolBuffer,
			__TMS_ADDRBITS			SizePoolBuffer
			);

__TMS_USBPUMP_POOL_ALLOC_FN	UsbPumpLinearPool_Alloc;
__TMS_USBPUMP_POOL_CLOSE_FN	UsbPumpLinearPool_Close;
__TMS_USBPUMP_POOL_FREE_FN	UsbPumpLinearPool_Free;
__TMS_USBPUMP_POOL_INFO_FN	UsbPumpLinearPool_Info;
__TMS_USBPUMP_POOL_REALLOC_FN	UsbPumpLinearPool_Realloc;
__TMS_USBPUMP_POOL_RESET_FN	UsbPumpLinearPool_Reset;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_LINEAR_POOL_BLOCK_HEADER_SIZE	\
   __TMS_USBPUMP_LINEAR_POOL_BLOCK_HEADER_SIZE
# define USBPUMP_LINEAR_POOL_BLOCK_MIN_SIZE	\
   __TMS_USBPUMP_LINEAR_POOL_BLOCK_MIN_SIZE
# define USBPUMP_LINEAR_POOL_BLOCK_ALIGN_SIZE(s)	\
   __TMS_USBPUMP_LINEAR_POOL_BLOCK_ALIGN_SIZE(s)
# define USBPUMP_LINEAR_POOL_BLOCK_ROUND_SIZE(s)	\
   __TMS_USBPUMP_LINEAR_POOL_BLOCK_ROUND_SIZE(s)
# define USBPUMP_LINEAR_POOL_BLOCK_GET_NEXT(p, s)	\
   __TMS_USBPUMP_LINEAR_POOL_BLOCK_GET_NEXT(p, s)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_linearpool.h ****/
#endif /* _USBPUMP_LINEARPOOL_H_ */
