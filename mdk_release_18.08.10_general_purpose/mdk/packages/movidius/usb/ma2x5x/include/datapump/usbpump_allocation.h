/* usbpump_allocation.h	Fri May 23 2014 11:57:35 chwon */

/*

Module:  usbpump_allocation.h

Function:
	Definition of allocation tracking facility for DataPump.

Version:
	V3.13a	Fri May 23 2014 11:57:35 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2007, 2013-2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChiaChu Cho, MCCI Corporation	September 2007

Revision history:
   1.97j  Mon Sep 10 2007 10:20:21 cccho
	Module created.

   3.11d  Fri Nov 01 2013 14:03:46  chwon
	17828: Add gk_UsbPumpAllocationTrackingConfig_Default and it is default
	allocation tracking configuration using platform abstract pool.

   3.13a  Tue Apr 08 2014 16:20:40  chwon
	18163: Enable dynamic memory allocation tracking system code for the
	checked build only.

   3.13a  Fri May 23 2014 11:57:35  chwon
	18163: Add USBPUMP_ALLOCATION_TRACKING_CONFIG_DEFAULT definition.

*/

#ifndef _USBPUMP_ALLOCATION_H_		/* prevent multiple includes */
#define _USBPUMP_ALLOCATION_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBPUMP_MEMPOOL_H_
# include "usbpump_mempool.h"
#endif

/****************************************************************************\
|
|	USBPUMP_ALLOCATION_TRACKING
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_ALLOCATION_TRACKING_CONFIG);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_ALLOCATION_SCRATCHPAD);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_ALLOCATION_SCRATCHPAD_METHOD_TABLE);

__TMS_TYPE_DEF_FUNCTION(
	USBPUMP_ALLOCATION_TRACKING_SYSTEM_INIT_FN,
	__TMS_VOID *,
		(
		__TMS_USBPUMP_ALLOCATION_TRACKING 			*pTracking,
		__TMS_CONST __TMS_USBPUMP_ALLOCATION_TRACKING_CONFIG	*pConfig,
		__TMS_UPLATFORM						*pPlatform,
		__TMS_USBPUMP_ALLOCATION_SCRATCHPAD			*pScratchPad
		));

__TMS_TYPE_DEF_FUNCTION(
	USBPUMP_ALLOCATION_TRACKING_SYSTEM_LOG_ALLOCATION_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_ALLOCATION_TRACKING	*pTracking,
		__TMS_CONST __TMS_TEXT			*pType,
		__TMS_CONST __TMS_TEXT			*pWhat,
		__TMS_ADDRDIFF				delta
		));

__TMS_TYPE_DEF_FUNCTION(
	USBPUMP_ALLOCATION_SCRATCHPAD_INIT_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_ALLOCATION_SCRATCHPAD	*pScratchPad,
		__TMS_USBPUMP_ALLOCATION_TRACKING	*pTracking
		));

__TMS_TYPE_DEF_FUNCTION(
	USBPUMP_ALLOCATION_SCRATCHPAD_COMMIT_OBJECT_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_ALLOCATION_SCRATCHPAD	*pScratchPad,
		__TMS_USBPUMP_OBJECT_HEADER		*pObject
		));

__TMS_TYPE_DEF_FUNCTION(
	USBPUMP_ALLOCATION_SCRATCHPAD_CLOSE_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_ALLOCATION_SCRATCHPAD	*pScratchPad
		));

__TMS_TYPE_DEF_FUNCTION(
	USBPUMP_ALLOCATION_SCRATCHPAD_LOG_FOR_OBJECT_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_ALLOCATION_SCRATCHPAD	*pScratchPad,
		__TMS_USBPUMP_OBJECT_HEADER		*pObject
		));

__TMS_TYPE_DEF_FUNCTION(
	USBPUMP_ALLOCATION_SCRATCHPAD_LOG_MISCELLANEOUS_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_ALLOCATION_SCRATCHPAD	*pScratchPad,
		__TMS_CONST __TMS_TEXT			*pType,
		__TMS_CONST __TMS_TEXT			*pWhat
		));

struct __TMS_STRUCTNAME(USBPUMP_ALLOCATION_SCRATCHPAD)
	{
	__TMS_CONST __TMS_USBPUMP_ALLOCATION_SCRATCHPAD_METHOD_TABLE
					*pMethods;
	__TMS_VOID			*pContext;
	__TMS_ADDRBITS_PTR_UNION	info[8];
	};


struct __TMS_STRUCTNAME	(USBPUMP_ALLOCATION_SCRATCHPAD_METHOD_TABLE)
	{
	__TMS_USBPUMP_ALLOCATION_SCRATCHPAD_INIT_FN		*pScratchPadInit;
	__TMS_USBPUMP_ALLOCATION_SCRATCHPAD_COMMIT_OBJECT_FN	*pCommitObject;
	__TMS_USBPUMP_ALLOCATION_SCRATCHPAD_CLOSE_FN		*pClose;
	__TMS_USBPUMP_ALLOCATION_SCRATCHPAD_LOG_FOR_OBJECT_FN	*pLogForObject;
	__TMS_USBPUMP_ALLOCATION_SCRATCHPAD_LOG_MISCELLANEOUS_FN *pLogMiscellaneous;
	};

struct __TMS_STRUCTNAME	(USBPUMP_ALLOCATION_TRACKING)
	{
	__TMS_USBPUMP_OBJECT_HEADER	uAllocTrackHeader;
	__TMS_UPLATFORM			*pPlatform;
	__TMS_VOID			*pContext;
	__TMS_CONST __TMS_USBPUMP_ALLOCATION_SCRATCHPAD_METHOD_TABLE
					*pScratchPadMethods;
	};

struct __TMS_STRUCTNAME	(USBPUMP_ALLOCATION_TRACKING_CONFIG)
	{
	__TMS_USBPUMP_ALLOCATION_TRACKING_SYSTEM_INIT_FN
					*pTrackingSysemInitFn;
	__TMS_USBPUMP_ALLOCATION_TRACKING_SYSTEM_LOG_ALLOCATION_FN
					*pLogAllocationFn;
	__TMS_CONST __TMS_USBPUMP_ALLOCATION_SCRATCHPAD_METHOD_TABLE
					*pScratchPadMethods;
	__TMS_CONST __TMS_TEXT		*pTrackingObjectName;
	__TMS_BYTES			TrackingObjectSize;
	__TMS_CONST __TMS_VOID		*pImplementationDefined;
	};

#define __TMS_USBPUMP_ALLOCATION_SCRATCHPAD_METHOD_TABLE_INIT_V1(	\
		ARG_pScratchPadInit,					\
		ARG_pCommitObject,					\
		ARG_pClose,						\
		ARG_pLogForObject,					\
		ARG_pLogMiscellaneous					\
		)							\
		{							\
		(ARG_pScratchPadInit),					\
		(ARG_pCommitObject),					\
		(ARG_pClose),						\
		(ARG_pLogForObject),					\
		(ARG_pLogMiscellaneous)					\
		}

#define __TMS_USBPUMP_ALLOCATION_TRACKING_CONFIG_INIT_V1(	\
		ARG_pTrackingSystemInitFn,			\
		ARG_pLogAllocationFn,				\
		ARG_pScratchPadMethods,				\
		ARG_pTrackingObjectName,			\
		ARG_TrackingObjectSize,				\
		ARG_pImplementationDefined			\
		)						\
		{						\
		(ARG_pTrackingSystemInitFn),			\
		(ARG_pLogAllocationFn),				\
		(ARG_pScratchPadMethods),			\
		(ARG_pTrackingObjectName),			\
		(ARG_TrackingObjectSize),			\
		(ARG_pImplementationDefined)			\
		}

#define __TMS_USBPUMP_ALLOCATION_TRACKING_CONFIG_SETUP_V1(	\
		ARG_pAllocTrkCfg,				\
		ARG_pTrackingSystemInitFn,			\
		ARG_pLogAllocationFn,				\
		ARG_pScratchPadMethods,				\
		ARG_pTrackingObjectName,			\
		ARG_TrackingObjectSize,				\
		ARG_pImplementationDefined			\
		)						\
	do	{						\
		(ARG_pAllocTrkCfg)->pTrackingSysemInitFn =		\
					(ARG_pTrackingSystemInitFn);	\
		(ARG_pAllocTrkCfg)->pLogAllocationFn =			\
					(ARG_pLogAllocationFn);		\
		(ARG_pAllocTrkCfg)->pScratchPadMethods =		\
					(ARG_pScratchPadMethods);	\
		(ARG_pAllocTrkCfg)->pTrackingObjectName =		\
					(ARG_pTrackingObjectName);	\
		(ARG_pAllocTrkCfg)->TrackingObjectSize =		\
					(ARG_TrackingObjectSize);	\
		(ARG_pAllocTrkCfg)->pImplementationDefined =		\
					(ARG_pImplementationDefined);	\
		} while (0)


#define __TMS_USBPUMP_ALLOCATION_TRACKING_NAME(Name)	\
	Name ".allocation-tracking.mcci.com"

#define __TMS_USBPUMP_ALLOCATION_TRACKING_TAG	\
	__TMS_UHIL_MEMTAG('U', 'T', 'r', 'k')


/****************************************************************************\
|
|	API macros
|
\****************************************************************************/

#ifndef	__TMS_USBPUMP_ALLOCATION_TRACKING_ENABLE
# define __TMS_USBPUMP_ALLOCATION_TRACKING_ENABLE	__TMS_USBPUMP_DEBUG
#endif

#if __TMS_USBPUMP_ALLOCATION_TRACKING_ENABLE

# define UsbPumpAllocation_TrackingSystemInit(				\
		pAllocTrackCfg,						\
		pPlatform,						\
		pScratchPad						\
		)							\
	UsbPumpAllocation_TrackingSystemInit_Default(			\
		pAllocTrackCfg,						\
		pPlatform,						\
		pScratchPad						\
		)

# define UsbPumpPlatform_InitAllocationScratchPad(			\
		pPlatform,						\
		pScratchPad						\
		)							\
	UsbPumpPlatform_InitAllocationScratchPad_Default(		\
		pPlatform,						\
		pScratchPad						\
		)

# define UsbPumpObjectHeader_InitAllocationScratchPad(			\
		pObjectHeader,						\
		pScratchPad						\
		)							\
	UsbPumpObjectHeader_InitAllocationScratchPad_Default(		\
		pObjectHeader,						\
		pScratchPad						\
		)

# define UsbPumpAllocationScratchPad_CommitForObject(			\
		pScratchPad,						\
		pObject							\
		)							\
	UsbPumpAllocationScratchPad_CommitForObject_Default(		\
		pScratchPad,						\
		pObject							\
		)

# define UsbPumpAllocationScratchPad_CommitForObjectHeader(		\
		pScratchPad,						\
		pObjectHeader						\
		)							\
	UsbPumpAllocationScratchPad_CommitForObjectHeader_Default(	\
		pScratchPad,						\
		pObjectHeader						\
		)

# define UsbPumpAllocationScratchPad_Close(				\
		pScratchPad						\
		)							\
	UsbPumpAllocationScratchPad_Close_Default(			\
		pScratchPad						\
		)

# define UsbPumpAllocationScratchPad_LogForObject(			\
		pScratchPad,						\
		pObjectHeader						\
		)							\
	UsbPumpAllocationScratchPad_LogForObject_Default(		\
		pScratchPad,						\
		pObjectHeader						\
		)

# define UsbPumpAllocationScratchPad_LogMiscellaneous(			\
		pScratchPad,						\
		pType,							\
		pWhat							\
		)							\
	UsbPumpAllocationScratchPad_LogMiscellaneous_Default(		\
		pScratchPad,						\
		pType,							\
		pWhat							\
		)

# define __TMS_USBPUMP_ALLOCATION_TRACKING_CONFIG_DEFAULT		\
	&gk_UsbPumpAllocationTrackingConfig_Default

#else /* ! __TMS_USBPUMP_ALLOCATION_TRACKING_ENABLE */

# define UsbPumpAllocation_TrackingSystemInit(				\
		pAllocTrackCfg,						\
		pPlatform,						\
		pScratchPad						\
		)							\
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pScratchPad)

# define UsbPumpPlatform_InitAllocationScratchPad(			\
		pPlatform,						\
		pScratchPad						\
		)							\
    do	{								\
    	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pPlatform);		\
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pScratchPad);		\
	} while (0)

# define UsbPumpObjectHeader_InitAllocationScratchPad(			\
		pObjectHeader,						\
		pScratchPad						\
		)							\
    do	{								\
    	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pObjectHeader);		\
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pScratchPad);		\
	} while (0)

# define UsbPumpAllocationScratchPad_CommitForObject(			\
		pScratchPad,						\
		pObject							\
		)							\
    do	{								\
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pScratchPad);		\
    	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pObject);			\
	} while (0)

# define UsbPumpAllocationScratchPad_CommitForObjectHeader(		\
		pScratchPad,						\
		pObjectHeader						\
		)							\
    do	{								\
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pScratchPad);		\
    	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pObjectHeader);		\
	} while (0)

# define UsbPumpAllocationScratchPad_Close(				\
		pScratchPad						\
		)							\
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pScratchPad)

# define UsbPumpAllocationScratchPad_LogForObject(			\
		pScratchPad,						\
		pObjectHeader						\
		)							\
    do	{								\
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pScratchPad);		\
    	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pObjectHeader);		\
	} while (0)

# define UsbPumpAllocationScratchPad_LogMiscellaneous(			\
		pScratchPad,						\
		pType,							\
		pWhat							\
		)							\
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pScratchPad)

# define __TMS_USBPUMP_ALLOCATION_TRACKING_CONFIG_DEFAULT		\
	__TMS_NULL

#endif /* __TMS_USBPUMP_ALLOCATION_TRACKING_ENABLE */


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_CONST __TMS_USBPUMP_ALLOCATION_TRACKING_CONFIG
gk_UsbPumpAllocationTrackingConfig_Default;

/* Allocation Track System Init */
__TMS_VOID
UsbPumpAllocation_TrackingSystemInit_Default(
	__TMS_CONST __TMS_USBPUMP_ALLOCATION_TRACKING_CONFIG *pAllocTrackCfg,
	__TMS_UPLATFORM				*pPlatform,
	__TMS_USBPUMP_ALLOCATION_SCRATCHPAD	*pScratchPad
	);

/* Begin Allocation Measurement */
__TMS_VOID
UsbPumpPlatform_InitAllocationScratchPad_Default(
	__TMS_UPLATFORM				*pPlatform,
	__TMS_USBPUMP_ALLOCATION_SCRATCHPAD	*pScratchPad
	);

__TMS_VOID
UsbPumpObjectHeader_InitAllocationScratchPad_Default(
	__TMS_USBPUMP_OBJECT_HEADER		*pObjectHeader,
	__TMS_USBPUMP_ALLOCATION_SCRATCHPAD	*pScratchPad
	);

/* Recording Memory Allocations */
__TMS_VOID
UsbPumpAllocationScratchPad_CommitForObject_Default(
	__TMS_USBPUMP_ALLOCATION_SCRATCHPAD	*pScratchPad,
	__TMS_USBPUMP_OBJECT			*pObject
	);

__TMS_VOID
UsbPumpAllocationScratchPad_CommitForObjectHeader_Default(
	__TMS_USBPUMP_ALLOCATION_SCRATCHPAD	*pScratchPad,
	__TMS_USBPUMP_OBJECT_HEADER		*pObjectHeader
	);


__TMS_USBPUMP_ALLOCATION_SCRATCHPAD_CLOSE_FN
UsbPumpAllocationScratchPad_Close_Default;

__TMS_USBPUMP_ALLOCATION_SCRATCHPAD_LOG_FOR_OBJECT_FN
UsbPumpAllocationScratchPad_LogForObject_Default;

__TMS_USBPUMP_ALLOCATION_SCRATCHPAD_LOG_MISCELLANEOUS_FN
UsbPumpAllocationScratchPad_LogMiscellaneous_Default;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_ALLOCATION_SCRATCHPAD_METHOD_TABLE_INIT_V1(	\
		ARG_pScratchPadInit,					\
		ARG_pCommitObject,					\
		ARG_pClose,						\
		ARG_pLogForObject,					\
		ARG_pLogMiscellaneous					\
		)	\
	__TMS_USBPUMP_ALLOCATION_SCRATCHPAD_METHOD_TABLE_INIT_V1(	\
		ARG_pScratchPadInit,					\
		ARG_pCommitObject,					\
		ARG_pClose,						\
		ARG_pLogForObject,					\
		ARG_pLogMiscellaneous					\
		)
# define USBPUMP_ALLOCATION_TRACKING_CONFIG_INIT_V1(	\
		ARG_pTrackingSystemInitFn,			\
		ARG_pLogAllocationFn,				\
		ARG_pScratchPadMethods,				\
		ARG_pTrackingObjectName,			\
		ARG_TrackingObjectSize,				\
		ARG_pImplementationDefined			\
		)	\
	__TMS_USBPUMP_ALLOCATION_TRACKING_CONFIG_INIT_V1(	\
		ARG_pTrackingSystemInitFn,			\
		ARG_pLogAllocationFn,				\
		ARG_pScratchPadMethods,				\
		ARG_pTrackingObjectName,			\
		ARG_TrackingObjectSize,				\
		ARG_pImplementationDefined			\
		)
# define USBPUMP_ALLOCATION_TRACKING_CONFIG_SETUP_V1(	\
		ARG_pAllocTrkCfg,				\
		ARG_pTrackingSystemInitFn,			\
		ARG_pLogAllocationFn,				\
		ARG_pScratchPadMethods,				\
		ARG_pTrackingObjectName,			\
		ARG_TrackingObjectSize,				\
		ARG_pImplementationDefined			\
		)	\
	__TMS_USBPUMP_ALLOCATION_TRACKING_CONFIG_SETUP_V1(	\
		ARG_pAllocTrkCfg,				\
		ARG_pTrackingSystemInitFn,			\
		ARG_pLogAllocationFn,				\
		ARG_pScratchPadMethods,				\
		ARG_pTrackingObjectName,			\
		ARG_TrackingObjectSize,				\
		ARG_pImplementationDefined			\
		)
# define USBPUMP_ALLOCATION_TRACKING_NAME(Name)	\
   __TMS_USBPUMP_ALLOCATION_TRACKING_NAME(Name)
# define USBPUMP_ALLOCATION_TRACKING_TAG	\
   __TMS_USBPUMP_ALLOCATION_TRACKING_TAG
# define USBPUMP_ALLOCATION_TRACKING_ENABLE	\
   __TMS_USBPUMP_ALLOCATION_TRACKING_ENABLE
# define USBPUMP_ALLOCATION_TRACKING_CONFIG_DEFAULT	\
   __TMS_USBPUMP_ALLOCATION_TRACKING_CONFIG_DEFAULT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_allocation.h ****/
#endif /* _USBPUMP_ALLOCATION_H_ */
