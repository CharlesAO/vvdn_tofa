/* utask.h	Mon Jan 14 2013 05:30:21 tmm */

/*

Module:  utask.h

Function:
	The definitions for MCCI Generic Task API (OS independant)

Version:
	V3.11b	Mon Jan 14 2013 05:30:21 tmm	Edit level 24

Copyright notice:
	This file copyright (C) 2005-2007, 2010-2011, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Mats Webjorn, MCCI Corporation	February 2005

Revision history:
   1.91b  Mon Feb 14 2005 14:44:15  maw
	Module created.

   1.91c  Fri May 13 2005 12:15:25  maw
   	Moved DataPump task name to usbtaskmsg_dp.h
   	Added start-task function protoype

   1.91d  Tue May 24 2005 11:23:49 maw
   	Replaced UTASK_MSGLOOP_FN with UTASK_INIT_FN and UTASK_MSGDISPATH_FN

   1.91e  Wed Jun 29 2005 08:48:46 maw
   	Add UTASK_IOCTL_FN to task-config, renamed macro parameters

   1.91f  Fri Jul 01 2005 12:14:56 maw
   	Added UTASK_INVALID_BUF_LENGTH

   1.91f  Thu Jul 07 2005 21:33:25 maw
   	Added UTASK_CONFIG_HDR_SETUP_V1

   1.91g  Wed Jul 13 2005 16:47:21  tmm
	Added UTASK_ROOT_INIT and _SETUP macro

   1.91g  Wed Jul 13 2005 20:05:08  tmm
	Get rid of CONST in the UTASK_ROOT object, and move function
	pointer and other const init data to the switch (where they belong).

   1.91g  Tue Jul 19 2005 10:30:18  chwon
	Moved including mccilims.h after including utask_types.h

   1.91h  2005-08-02  tmm
	1510: Use macros for task linkage in all cases.

   1.91h  Thu Aug 25 2005 12:16:50 maw
   	Added abstract pool pointer to UTASK_HDR, added MemPool and sizeMemPool
   	to UTASK_CONFIG_HDR

   1.91h  Mon Sep 12 2005 14:52:12  maw
   	Updated UTASK_CONFIG_HDR_SETUP_V2

   1.91i  Mon Sep 19 2005 19:26:20 maw
   	Moved UTASK_HDR def to utask_types.h, added macro to convert from hProc
   	to hTask

   1.91i  Sat Sep 24 2005 22:03:35  chwon
	Remove global UPLATFORM pointer (g_pUsbPumpPlatform); remove warning
	for free version build

   1.93a  Tue Oct 11 2005 15:15:27  chwon
	Move DebugFlags from UTASK_HDR to UTASK_PROCESS_HDR

   1.93a  Fri Oct 14 2005 14:59:33 maw
	Corrected def of UTASK_INVALID_TIMEOUT_HANDLE

   1.93a  Tue Oct 18 2005 10:40:47  maw
	Added pOptionalOsInfo to UTASK_CONFIG_HDR

   1.93c  Tue Dec 06 2005 17:06:32  chwon
	Bug 1587: Added CONST definition of pOptionalOsInfo.

   1.93e  Thu Jan 12 2006 13:07:57  chwon
	Added UTASK software check error code.

   1.93e  Mon Jan 16 2006 21:14:37  tmm
	No need for mccilims.h

   1.93e  Thu Jan 26 2006 15:57:53  chwon
	Regenerate uncloaked names

   1.97j  Thu Dec 06 2007 14:32:37  chwon
	3498: Change macros to use arguments (i.e., to look function like).

   3.01b  Thu Mar 18 2010 11:36:26  maw
   	8691: Add pTaskRoot to UTASK_ROOT_CONFIG
   	      Added UTASK_ROOT_CONFIG_INIT_V1() and UTASK_ROOT_CONFIG_SETUP_V2() 
   	      	where pTaskRoot is added
   	      Removed pPlatform from UTASK_ROOT

   3.01f  Tue Sep 06 2011 16:31:53  chwon
	13272: use USBPUMP_DEBUG instead of DEBUG.

   3.11b  Mon Jan 14 2013 05:30:21  tmm
	16459: use USBPUMP_TRACE to control compile-in of trace functions.

*/

#ifndef _UTASK_H_		/* prevent multiple includes */
#define _UTASK_H_

/* MCCI include files */
#ifndef _UTASK_TYPES_H_
# include "utask_types.h"
#endif

/* for the task methods, we need the UPLATFORM */
#ifndef _UPLATFORM_H_
# include "uplatform.h"
#endif

/* for the task, we need the abstract pool pointer */
#ifndef _USBPUMP_MEMPOOL_H_
# include "usbpump_mempool.h"
#endif

/* UHIL error code bases are defined in terms of SWC codes... */
#ifndef _UHILSWC_H_
# include "uhilswc.h"
#endif

/* we need cpu limits */
#include "def/cpulims.h"

/****************************************************************************\
|
|	Macro declarations.
|
\****************************************************************************/

#define __TMS_UTASK_MAX_BUF_LENGTH			((__TMS_BYTES)__TMS_SIZE_T_MAX-1)
#define __TMS_UTASK_INVALID_BUF_LENGTH			((__TMS_BYTES)__TMS_SIZE_T_MAX)
#define __TMS_UTASK_TMO_INFINITE			((__TMS_ULONG)-1)

#define __TMS_UTASK_INVALID_HANDLE			((__TMS_UTASK_HANDLE)__TMS_NULL)
#define __TMS_UTASK_INVALID_PROCESS_HANDLE		((__TMS_UTASK_PROCESS_HANDLE)__TMS_NULL)
#define __TMS_UTASK_INVALID_MAILBOX_HANDLE		((__TMS_UTASK_MAILBOX_HANDLE)__TMS_NULL)
#define __TMS_UTASK_INVALID_MSG_HANDLE			((__TMS_UTASK_MSG_HANDLE)__TMS_NULL)
#define __TMS_UTASK_INVALID_BUFFER_HANDLE		((__TMS_UTASK_BUFFER_HANDLE)__TMS_NULL)
#define __TMS_UTASK_INVALID_TIMEOUT_HANDLE		((__TMS_UTASK_TIMEOUT_HANDLE)-1)
#define __TMS_UTASK_INVALID_COMPLETION_HANDLE		((__TMS_UTASK_COMPLETION_HANDLE)__TMS_NULL)

#define __TMS_UTASK_MSG_ACCESSFLAGS_READ		((__TMS_UTASK_MSG_ACCESSFLAGS)(1 << 0))
#define __TMS_UTASK_MSG_ACCESSFLAGS_WRITE		((__TMS_UTASK_MSG_ACCESSFLAGS)(1 << 1))
#define __TMS_UTASK_MSG_ACCESSFLAGS_READWRITE		(__TMS_UTASK_MSG_ACCESSFLAGS_READ | \
							 __TMS_UTASK_MSG_ACCESSFLAGS_WRITE)
#define __TMS_UTASK_MSG_ACCESSFLAGS_ALLOCED		((__TMS_UTASK_MSG_ACCESSFLAGS)(1 << 2))
#define __TMS_UTASK_MSG_ACCESSFLAGS_LENGTH_VALID	((__TMS_UTASK_MSG_ACCESSFLAGS)(1 << 3))


/****************************************************************************\
|
|	Task and process configuration
|
\****************************************************************************/

struct __TMS_STRUCTNAME(UTASK_PROCESS_CONFIG)
	{
	__TMS_CONST __TMS_TEXT *			pName;
	__TMS_UTASK_PROCESS_INIT_FN *			pInitFn;
	__TMS_UTASK_PROCESS_EXIT_FN *			pExitFn;
	__TMS_UTASK_PROCESS_MSGDISPATCH_FN *		pMsgDispatchFn;

	/*
	|| A slot for extra information, for use by the Message Dispatcher
	|| in doing its work.
	*/
	__TMS_CONST __TMS_VOID *			pOptionalInfo;
	};

struct __TMS_STRUCTNAME(UTASK_CONFIG_HDR)
	{
	__TMS_CONST __TMS_TEXT *			pName;
	__TMS_UTASK_MSGLOOP_FN *			pMsgLoopFn;
	__TMS_BYTES					nProcCfg;
	__TMS_CONST __TMS_UTASK_PROCESS_CONFIG *	pvProcCfg;
	__TMS_UINT8 *					pMemPool;
	__TMS_BYTES					sizeMemPool;
	__TMS_UINT32					DebugFlags;

	/*
	|| A slot for extra information. This is where OS specific
	|| task config info may be provided
	*/
	__TMS_CONST __TMS_VOID *			pOptionalOsInfo;
	};

/*
|| UTASK_PROCESS_CONFIG_INIT_V1 initialization functions.  Don't change the
|| calling sequence for any macro ending in Vn; instead, create a macro
|| ending in Vn+1, and redefine the Vn macro to expand in an upward-compatible
|| way.
*/
#define	__TMS_UTASK_PROCESS_CONFIG_INIT_V1(		\
		ARG_pName,				\
		ARG_pInitFn,				\
		ARG_pExitFn,				\
		ARG_pMsgDispatchFn,			\
		ARG_pOptionalInfo			\
		)					\
	{						\
	(ARG_pName),					\
	(ARG_pInitFn),					\
	(ARG_pExitFn),					\
	(ARG_pMsgDispatchFn),				\
	(ARG_pOptionalInfo)				\
	}

/*
|| UTASK_PROCESS_CONFIG_SETUP_V1 run-time setup functions.  Don't change the
|| calling sequence for any macro ending in Vn; instead, create a macro
|| ending in Vn+1, and redefine the Vn macro to expand in an upward-compatible
|| way.
*/
#define	__TMS_UTASK_PROCESS_CONFIG_SETUP_V1( 			\
		ARG_pProcCfg,					\
		ARG_pName, 					\
		ARG_pInitFn, 					\
		ARG_pExitFn, 					\
		ARG_pMsgDispatchFn, 				\
		ARG_pOptionalInfo 				\
		) 						\
    do 	{ 							\
	(ARG_pProcCfg)->pName = (ARG_pName);			\
	(ARG_pProcCfg)->pInitFn = (ARG_pInitFn);		\
	(ARG_pProcCfg)->pExitFn = (ARG_pExitFn);		\
	(ARG_pProcCfg)->pMsgDispatchFn = (ARG_pMsgDispatchFn);	\
	(ARG_pProcCfg)->pOptionalInfo = (ARG_pOptionalInfo);	\
	} while (0)

/*
|| UTASK_CONFIG_HDR_INIT_Vx initialization functions.  Don't change the
|| calling sequence for any macro ending in Vn; instead, create a macro
|| ending in Vn+1, and redefine the Vn macro to expand in an upward-compatible
|| way.
*/
#define	__TMS_UTASK_CONFIG_HDR_INIT_V4(				\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags,					\
		ARG_pOptionalOsInfo				\
		)						\
	{							\
	(ARG_pName),						\
	(ARG_pMsgLoopFn),					\
	(__TMS_LENOF(ARG_pvProcCfg)),				\
	(ARG_pvProcCfg),					\
	(ARG_pMemPool),						\
	(ARG_sizeMemPool),					\
	(ARG_DebugFlags),					\
	(ARG_pOptionalOsInfo)					\
	}

#define	__TMS_UTASK_CONFIG_HDR_INIT_V3(				\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags					\
		)						\
	__TMS_UTASK_CONFIG_HDR_INIT_V4(				\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags,					\
		__TMS_NULL	/* V4: pOptionalOsInfo */	\
		)						\

#define	__TMS_UTASK_CONFIG_HDR_INIT_V2(				\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool					\
		)						\
	__TMS_UTASK_CONFIG_HDR_INIT_V3(				\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		0		/* V3: DebugFlags */		\
		)						\

#define __TMS_UTASK_CONFIG_HDR_INIT_V1(				\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg					\
		)						\
	__TMS_UTASK_CONFIG_HDR_INIT_V2(				\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		__TMS_NULL,	/* V2: pMemPool */		\
		0		/* V2: sizeMemPool */		\
		)

/*
|| UTASK_CONFIG_HDR_SETUP_Vx runtime initialization functions.  Don't change the
|| calling sequence for any macro ending in Vn; instead, create a macro
|| ending in Vn+1, and redefine the Vn macro to expand in an upward-compatible
|| way.
*/
#define	__TMS_UTASK_CONFIG_HDR_SETUP_V4(			\
		ARG_pConfig,					\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags,					\
		ARG_pOptionalOsInfo				\
		)						\
	do							\
		{						\
		(ARG_pConfig)->pName            = (ARG_pName);			\
		(ARG_pConfig)->pMsgLoopFn       = (ARG_pMsgLoopFn);		\
		(ARG_pConfig)->nProcCfg         = __TMS_LENOF(ARG_pvProcCfg);	\
		(ARG_pConfig)->pvProcCfg        = (ARG_pvProcCfg);		\
		(ARG_pConfig)->pMemPool         = (ARG_pMemPool);		\
		(ARG_pConfig)->sizeMemPool      = (ARG_sizeMemPool);		\
		(ARG_pConfig)->DebugFlags       = (ARG_DebugFlags);		\
		(ARG_pConfig)->pOptionalOsInfo  = (ARG_pOptionalOsInfo);	\
		} 								\
	while (0)

#define	__TMS_UTASK_CONFIG_HDR_SETUP_V3(			\
		ARG_pConfig,					\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags					\
		)						\
	__TMS_UTASK_CONFIG_HDR_SETUP_V4(			\
		ARG_pConfig,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags,					\
		__TMS_NULL	/* V4: pOptionalOsInfo */	\
		)						\

#define	__TMS_UTASK_CONFIG_HDR_SETUP_V2(			\
		ARG_pConfig,					\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool					\
		)						\
	__TMS_UTASK_CONFIG_HDR_SETUP_V3(			\
		ARG_pConfig,					\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		0		/* V3: DebugFlags */		\
		)						\

#define __TMS_UTASK_CONFIG_HDR_SETUP_V1(			\
		ARG_pConfig,					\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_nProcCfg					\
		)						\
	__TMS_UTASK_CONFIG_HDR_SETUP_V2(			\
		ARG_pConfig,					\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		__TMS_NULL,	/* V2: pMemPool */		\
		0		/* V2: sizeMemPool */		\
		)

/****************************************************************************\
|
|	Task context
|
\****************************************************************************/

/* __TMS_TYPE_DEF_STRUCT(UTASK_ROOT); -- in basehiltypes.h */

struct __TMS_STRUCTNAME(UTASK_PROCESS_HDR)
	{
	__TMS_UTASK_HDR	*			pUtask;
	__TMS_UTASK_PROCESS_CONFIG		Config;
	__TMS_VOID *				pCtx_void;
	__TMS_UTASK_PROCESS_INDEX		i;
	__TMS_USHORT				FilterLevel;
	__TMS_UINT32				DebugFlags;
	};

struct __TMS_STRUCTNAME(UTASK_HDR)
	{
	__TMS_TEXT *				pName;
	__TMS_UTASK_MSGLOOP_FN *		pMsgLoopFn;
	__TMS_USBPUMP_ABSTRACT_POOL *		pAbstractPool;
	};

#define __TMS_UTASK_PROC_HANDLE_TO_TASK_HANDLE(ARG_hProc)	\
	((__TMS_UTASK_HANDLE)(((__TMS_UTASK_PROCESS_HDR *)(ARG_hProc))->pUtask))


/*

Type:	UTASK_ROOT_CONFIG

Index:	Type:	UTASK_ROOT_CONFIG_CLIENT_INIT_FN

Function:
	User-prepared initialization control for UTASK_ROOT

Description:
	The platform-specific code for a given operating system must
	provide enough information to initialize the UTASK_ROOT.
	This info is of two kinds:  per-system adjustable parameters,
	and per-implementation constant configuration information
	(static function pointers and so forth).  The UTASK_ROOT_CONFIG
	structure is normally built dyanmically, and contains the per-system
	adjustable parameters, and a pointer to the implementation
	constant configuration information (typically stored in ROM).

Contents:
	UTASK_ROOT * pTaskRoot;
		Optional pointer to already existing task root

	BYTES sizeTaskRoot;
		The size of the task-root pointed to by pTaskRoot, or
		the number of bytes to allocate for the task root.
		This is provided to allow extra bytes to be allocated.
		If zero, the default size is used.

	CONST UTASK_ROOT_IMPLEMENTATION *pRootImplementation;
		Pointer to the TaskRoot implementation structure.  The
		info in this is normally opaque to the platform
		initialization code, which just uses a CONST object
		exported by u<osname>task.c.

	ADDRBITS MessageBase;
		This field is used to initialize the MessageBase field
		in the UTASK_ROOT object.  This is passed in from the
		platform-specific code, because normally the actual
		value of the message base will vary from compile to
		compile.  Without this field, we'd have to recompile
		all the message code each time the message base changed,
		and therefore we'd have to distribute source.
		(This is the overall messgae base, including the internal
		messages needed by the DataPump task, if any.  Of course,
		some operating systems don't need this.  MCCI os/none,
		for example, doesn't.)

	CONST TEXT *pTaskRootName;
		This string defines the name of the UTASK_ROOT object
		in the object system.  It should be created using the
		UTASK_ROOT_NAME() macro.  It must not be NULL.

	UTASK_ROOT_CONFIG_CLIENT_INIT_FN *pClientInitFn;
	VOID *pClientInitInfo;
		This provides a general purpose callback "hook".
		It is called back using the prototype:

		BOOL (*pClientInitFn)(
			UTASK_ROOT *pTaskRoot,
			CONST UTASK_ROOT_CONFIG *pTaskRootConfig,
			VOID *pClientInitInfo
			);

		If this function is provided, the client takes responsibility
		for calling pTaskRootConfig->pRootImplementation->pRootInitFn.
		This gives the client complete flexibility, but requires
		the client be correct for correct system initialization.

		The function returns TRUE for success, FALSE for failure.

Notes:
	Becase we have an implementation-specific call-back, it should
	never be necessary to dynanmically construct the
	UTASK_ROOT_IMPLEMENTATION object.

See Also:
	UTASK_ROOT_IMPLEMENTATION, UTASK_ROOT.

*/

/* __TMS_TYPE_DEF_STRUCT(UTASK_ROOT_CONFIG); -- in basehiltypes.h */

struct __TMS_STRUCTNAME(UTASK_ROOT_CONFIG)
	{
	__TMS_UTASK_ROOT *			pTaskRoot;
	__TMS_BYTES				sizeTaskRoot;
	__TMS_CONST __TMS_UTASK_ROOT_IMPLEMENTATION *
						pRootImplementation;
	__TMS_ADDRBITS				MessageBase;
	__TMS_CONST __TMS_TEXT *		pTaskRootName;

	__TMS_UTASK_ROOT_CONFIG_CLIENT_INIT_FN *pClientInitFn;
	__TMS_VOID *				pClientInitInfo;
	};

/*
|| UTASK_ROOT_CONFIG_INIT_Vx compile-time initialization functions.  Don't
|| change the calling sequence for any macro ending in Vn; instead, create a
|| macro ending in Vn+1, and redefine the Vn macro to expand in an upward-
|| compatible way.
*/
#define	__TMS_UTASK_ROOT_CONFIG_INIT_V2(				\
	a_pTaskRoot,							\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)								\
	{								\
	(a_pTaskRoot),							\
	(a_sizeTaskRoot),						\
	(a_pRootImplementation),					\
	(a_MessageBase),						\
	(a_pTaskRootName),						\
	(a_pClientInitFn),						\
	(a_pClientInitInfo)						\
	}

#define	__TMS_UTASK_ROOT_CONFIG_INIT_V1(				\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)								\
	__TMS_UTASK_ROOT_CONFIG_INIT_V2(				\
	__TMS_NULL, /* pTaskRoot */					\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)								\


/*
|| UTASK_ROOT_CONFIG_SETUP_Vx run-time initialization functions.  Don't change
|| the calling sequence for any macro ending in Vn; instead, create a macro
|| ending in Vn+1, and redefine the Vn macro to expand in an upward-compatible
|| way.
*/
#define	__TMS_UTASK_ROOT_CONFIG_SETUP_V2(				\
	a_pTaskRootConfig,						\
	a_pTaskRoot,							\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)								\
   do	{								\
	__TMS_UTASK_ROOT_CONFIG *__TMS_CONST __pTaskRootConfig =	\
				(a_pTaskRootConfig);			\
	__pTaskRootConfig->pTaskRoot = (a_pTaskRoot);			\
	__pTaskRootConfig->sizeTaskRoot = (a_sizeTaskRoot);		\
	__pTaskRootConfig->pRootImplementation = 			\
			(a_pRootImplementation);			\
	__pTaskRootConfig->MessageBase = (a_MessageBase);		\
	__pTaskRootConfig->pTaskRootName = (a_pTaskRootName);		\
	__pTaskRootConfig->pClientInitFn = (a_pClientInitFn);		\
	__pTaskRootConfig->pClientInitInfo = (a_pClientInitInfo);	\
	} while (0)

#define	__TMS_UTASK_ROOT_CONFIG_SETUP_V1(				\
	a_pTaskRootConfig,						\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)								\
	__TMS_UTASK_ROOT_CONFIG_SETUP_V2(				\
	a_pTaskRootConfig,						\
	__TMS_NULL, /* pTaskRoot */					\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)


/*

Type:	UTASK_ROOT_IMPLEMENTATION

Function:
	Configuration structure, which provides the init-time info needed
	to build the UTASK_ROOT object.

Description:
	Each operating system that supports DataPump inter-task messaging
	must provide an implementation.  This implementation must be
	initialized from within the UPLATFORM initialization.  In order
	to make it convenient to omit this code on non-tasking platforms,
	and in order to make it easy to centralize common initialization
	logic, the initialization information common to a single
	implementation is passed in a UTASK_ROOT_IMPLEMENTATION object.
	This object contains information that is needed at init time,
	and also information (in particular the UTASK_SWITCH) that is
	needed at run time.  This object does not contain parameters
	that must vary at compile time (such as the message base) --
	that info is passed in the UTASK_ROOT_CONFIG object.

Contents:
	BYTES sizeTaskRootMin;
		This provides a minimum size of the UTASK_ROOT object.
		Normally, it will be sizeof(UTASK_ROOT), but it might
		be larger if the implementation requires extra storage.

	UTASK_ROOT_INIT_FN *pRootInitFn;
		Pointer to a function that is called during initialization
		to perform any platform-specific initialization for the
		UTASK_ROOT.  (In C++ we might make this a private method
		of UTASK_ROOT rather than putting it in a separate object.)

	USBPUMP_OBJECT_IOTCTL_FN *pIoctlFn;
		Pointer to the function that provides IOCTL services for
		each UTASK_ROOT object.  (In C++, we might make this a private
		method rather than putting this in a separate object.)

	CONST UTASK_SWITCH *pSwitch;
		The client-accessible functions for the UTASK_ROOT.

Notes:
	If you find that you need to construct this dynamically on
	a given platform, you should instead provide
	UTASK_ROOT_CONFIG_CLIENT_INIT_FN as part of the UTASK_ROOT_CONFIG
	object, and do any necessary modifications dynamically.

	We do not provide dynamic setup macros for this function, because
	we want to reinforce the fact that this object is to be setup at
	compile time.

See Also:
	UTASK_ROOT_CONFIG, UTASK_ROOT

*/

struct __TMS_STRUCTNAME(UTASK_ROOT_IMPLEMENTATION)
	{
	__TMS_BYTES			sizeTaskRootMin;
	__TMS_UTASK_ROOT_INIT_FN	*pRootInitFn;
	__TMS_USBPUMP_OBJECT_IOCTL_FN	*pIoctlFn;
	__TMS_CONST __TMS_UTASK_SWITCH	*pSwitch;
	};

/*
|| UTASK_ROOT_IMPLEMENTATION_INIT_Vx initialization functions.  Don't change
|| the calling sequence for any macro ending in Vn; instead, create a macro
|| ending in Vn+1, and redefine the Vn macro to expand in an upward-compatible
|| way.
*/
#define	__TMS_UTASK_ROOT_IMPLEMENTATION_INIT_V1(			\
	a_sizeTaskRootMin,						\
	a_pRootInitFn,							\
	a_pIoctlFn,							\
	a_pSwitch							\
	)								\
	{								\
	(a_sizeTaskRootMin),						\
	(a_pRootInitFn),						\
	(a_pIoctlFn),							\
	(a_pSwitch)							\
	}

/*

Type:	UTASK_ROOT

Function:
	Central object that represents the DataPump tasking system to
	tasks.  Most primitives are methods of this object.

Description:
	The tasking system requires an abstract interface to the platform's
	implementation methods.  As normal, we implement this abstraction
	via an object that collects a variety of function pointers and
	other information.  This object is created by UsbPumpPlatform_Setup(),
	and is initialized using values passed in a UTASK_ROOT_CONFIG
	structure.

	The fields described under "Contents" are normally accessed by
	macros, and so the documentation here is for reference, not for
	use in client code.

Contents:
	USBPUMP_OBJECT_HEADER ObjectHeader;
		An object header that links the UTASK_ROOT into the
		DataPump object system.  The name of this object is
		normally "<osname>.taskroot.mcci.com".  The parent
		object will be the root object; the class parent
		will be the same as the UPLATFORM's class parent,
		normally the root object.

	CONST UTASK_SWITCH *pSwitch;
		Pointer to the client method function table.

	UTASK_HDR *pTaskHead;
		Pointer to the head of the list of tasks created by the
		tasking system.

	ADDRBITS MessageBase;
		This field is used to as the base message code for
		all messages in the tasking system.  This is passed
		in from the platform-specific code, because normally
		the actual value of the message base will vary from compile to
		compile.  Without this field, we'd have to recompile
		all the message code each time the message base changed,
		and therefore we'd have to distribute source.
		(This is the overall messgae base, including the internal
		messages needed by the DataPump task, if any.  Of course,
		some operating systems don't need this.  MCCI os/none,
		for example, doesn't.)

Notes:
	The size of the UTASK_ROOT may be adjusted at init time, allowing
	platform code to append platform-specific info, and/or space for
	system-specific info.

See Also:
	UTASK_SWITCH, UTASK_ROOT_CONFIG

*/

struct __TMS_STRUCTNAME(UTASK_ROOT)
	{
	__TMS_USBPUMP_OBJECT_HEADER		ObjectHeader;
	__TMS_CONST __TMS_UTASK_SWITCH *	pSwitch;
	__TMS_UTASK_HDR *			pTaskHead;
	__TMS_ADDRBITS				MessageBase;
	};

/*
|| The standard memory tag for UTASK_ROOTs
*/
#define __TMS_UTASK_ROOT_TAG		__TMS_UHIL_MEMTAG('U', 'T', 'k', '/')

/*
|| The macro for generating UTASK_ROOT names
*/
#define	__TMS_UTASK_ROOT_NAME(Text)	Text ".taskroot.mcci.com"

/****************************************************************************\
|
|	Message type declarations.
|
\****************************************************************************/

struct __TMS_STRUCTNAME(UTASK_MSG_HDR)
	{
	__TMS_UTASK_MSGCODE		MsgCode;
	__TMS_BYTES			EstimatedBufSize;

	/*
	|| Include info on who originally created the message
	*/
	__TMS_UTASK_MAILBOX_HANDLE	hSendMailbox;
	__TMS_UTASK_PROCESS_INDEX	iSendProcess;

	__TMS_BYTES			SeqNo;
	};

/****************************************************************************\
|
|	This structure is used to pass message handling functions from
|	an outer task to task-independant routines
|
\****************************************************************************/

struct __TMS_STRUCTNAME(UTASK_SWITCH)
	{
	__TMS_ULONG					MagicBegin;
	__TMS_UTASK_CREATETASK_FN *			pCreateTask;
	__TMS_UTASK_STARTTASK_FN *			pStartTask;
	__TMS_UTASK_LOCATETASK_FN *			pLocateTask;
	__TMS_UTASK_LOCATEPROCESS_FN *			pLocateProcess;
	__TMS_UTASK_ALLOCATEMESSAGE_FN *		pAllocateMessage;
	__TMS_UTASK_MILLITIMEOUT_FN *			pMilliTimeout;
	__TMS_UTASK_MALLOCZERO_FN *			pMallocZero;
	__TMS_UTASK_FREE_FN *				pFree;
	__TMS_UTASK_DBGPRINTF_FN *			pDbgPrintf;
	__TMS_CONST __TMS_UTASK_MSG_SWITCH *		pMsgSwitch;
	__TMS_CONST __TMS_UTASK_BUF_SWITCH *		pBufSwitch;
	__TMS_ULONG					MagicEnd;
	};

#define	__TMS_UTASK_SWITCH_BEGIN			(0x75746300 + '<')
#define	__TMS_UTASK_SWITCH_END				(0x36475700 + '>')

struct __TMS_STRUCTNAME(UTASK_MSG_SWITCH)
	{
	__TMS_ULONG					MagicBegin;
	__TMS_UTASK_MSG_ALLOCATEBUFFER_EX_FN *		pAllocateBufferEx;
	__TMS_UTASK_MSG_ALLOCATEREPLY_FN *		pAllocateReply;
	__TMS_UTASK_MSG_ATTACHBUFFER_EX_FN *		pAttachBufferEx;
	__TMS_UTASK_MSG_CLOSE_FN *			pClose;
	__TMS_UTASK_MSG_GETBODY_FN *			pGetBody;
	__TMS_UTASK_MSG_GETBUFFER_FN *			pGetBuffer;
	__TMS_UTASK_MSG_GETMSGCODE_FN *			pGetMessageCode;
	__TMS_UTASK_MSG_GETSENDER_FN *			pGetSender;
	__TMS_UTASK_MSG_SENDINDICATION_FN *		pSendIndication;
	__TMS_UTASK_MSG_SENDREPLY_FN *			pSendReply;
	__TMS_UTASK_MSG_SENDREQUEST_FN *		pSendRequest;
	__TMS_UTASK_MSG_SENDREQUESTWAITREPLY_FN *	pSendRequestWaitReply;
	__TMS_ULONG					MagicEnd;
	};

#define	__TMS_UTASK_MSG_SWITCH_BEGIN			(0x75746D00 + '<')
#define	__TMS_UTASK_MSG_SWITCH_END			(0xD6475700 + '>')

struct __TMS_STRUCTNAME(UTASK_BUF_SWITCH)
	{
	__TMS_ULONG					MagicBegin;
	__TMS_UTASK_BUFFER_CLOSE_FN *			pClose;
	__TMS_UTASK_BUFFER_GETREFERENCE_FN *		pGetReference;
	__TMS_UTASK_BUFFER_GETREFERENCE_EX_FN *		pGetReferenceEx;
	__TMS_UTASK_BUFFER_SETLENGTH_FN *		pSetLength;
	__TMS_ULONG					MagicEnd;
	};

#define	__TMS_UTASK_BUF_SWITCH_BEGIN			(0x75746200 + '<')
#define	__TMS_UTASK_BUF_SWITCH_END			(0x26475700 + '>')

/*
|| UTASK_SWITCH initialization functions.  Don't change the calling sequence
|| for any macro ending in Vn; instead, create a macro ending in Vn+1, and
|| redefine the Vn macro to expand in an upward-compatible way.
*/
#define	__TMS_UTASK_SWITCH_INIT_V1(		\
		ARG_pCreateTask,		\
		ARG_pStartTask,			\
		ARG_pLocateTask,		\
		ARG_pLocateProcess,		\
		ARG_pAllocateMessage,		\
		ARG_pMilliTimeout,		\
		ARG_pMallocZero,		\
		ARG_pFree,			\
		ARG_pDbgPrintf,			\
		ARG_pMsgSwitch,			\
		ARG_pBufSwitch			\
		)				\
	{					\
	(__TMS_UTASK_SWITCH_BEGIN),		\
	(ARG_pCreateTask),			\
	(ARG_pStartTask),			\
	(ARG_pLocateTask),			\
	(ARG_pLocateProcess),			\
	(ARG_pAllocateMessage),			\
	(ARG_pMilliTimeout),			\
	(ARG_pMallocZero),			\
	(ARG_pFree),				\
	(ARG_pDbgPrintf),			\
	(ARG_pMsgSwitch),			\
	(ARG_pBufSwitch),			\
	(__TMS_UTASK_SWITCH_END)		\
	}

/*
|| UTASK_MSG_SWITCH initialization functions.  Don't change the calling sequence
|| for any macro ending in Vn; instead, create a macro ending in Vn+1, and
|| redefine the Vn macro to expand in an upward-compatible way.
*/
#define	__TMS_UTASK_MSG_SWITCH_INIT_V1(		\
		ARG_pAllocateBufferEx,		\
		ARG_pAllocateReply,		\
		ARG_pAttachBufferEx,		\
		ARG_pClose,			\
		ARG_pGetBody,			\
		ARG_pGetBuffer,			\
		ARG_pGetMessageCode,		\
		ARG_pGetSender,			\
		ARG_pSendIndication,		\
		ARG_pSendReply,			\
		ARG_pSendRequest,		\
		ARG_pSendRequestWaitReply	\
		)				\
	{					\
	(__TMS_UTASK_MSG_SWITCH_BEGIN),		\
	(ARG_pAllocateBufferEx),		\
	(ARG_pAllocateReply),			\
	(ARG_pAttachBufferEx),			\
	(ARG_pClose),				\
	(ARG_pGetBody),				\
	(ARG_pGetBuffer),			\
	(ARG_pGetMessageCode),			\
	(ARG_pGetSender),			\
	(ARG_pSendIndication),			\
	(ARG_pSendReply),			\
	(ARG_pSendRequest),			\
	(ARG_pSendRequestWaitReply),		\
	(__TMS_UTASK_MSG_SWITCH_END)		\
	}

/*
|| UTASK_BUF_SWITCH initialization functions.  Don't change the calling sequence
|| for any macro ending in Vn; instead, create a macro ending in Vn+1, and
|| redefine the Vn macro to expand in an upward-compatible way.
*/
#define	__TMS_UTASK_BUF_SWITCH_INIT_V1(		\
		ARG_pClose,			\
		ARG_pGetReference,		\
		ARG_pGetReferenceEx,		\
		ARG_pSetLength			\
		)				\
	{					\
	(__TMS_UTASK_BUF_SWITCH_BEGIN),		\
	(ARG_pClose),				\
	(ARG_pGetReference),			\
	(ARG_pGetReferenceEx),			\
	(ARG_pSetLength),			\
	(__TMS_UTASK_BUF_SWITCH_END)		\
	}

/****************************************************************************\
|
|	Task primitive macros
|
\****************************************************************************/

/****************************************************************************\
|
|	Use these macros if you can't ensure proper creation order (dp then
|	tasks.
|
\****************************************************************************/

#define __TMS_UTASK_CREATETASK(			\
		pPlatform,			\
		pTaskCfgHdr			\
		)				\
	UPFTASK_CreateTask(			\
		pPlatform,			\
		pTaskCfgHdr			\
		)

#define __TMS_UTASK_STARTTASK(pTaskName)	UPFTASK_StartTask(pTaskName)

#define __TMS_UTASK_LOCATETASK(			\
		pTaskName,			\
		phMailbox			\
		)				\
	UPFTASK_LocateTask(			\
		pTaskName,			\
		phMailbox			\
		)

#define __TMS_UTASK_LOCATEPROCESS(		\
		hProcess,			\
		pTaskName,			\
		pProcName,			\
		phMailbox,			\
		piProc				\
		)				\
	UPFTASK_LocateProcess(			\
		hProcess,			\
		pTaskName,			\
		pProcName,			\
		phMailbox,			\
		piProc				\
		)

#define __TMS_UTASK_ALLOCATEMESSAGE(		\
		hProcess,			\
		MsgCode,			\
		EstimatedBufSize,		\
		phMsg,				\
		ppMsgBody			\
		)				\
	UPFTASK_AllocateMessage(		\
		hProcess,			\
		MsgCode,			\
		EstimatedBufSize,		\
		phMsg,				\
		ppMsgBody			\
		)

#define __TMS_UTASK_MILLITIMEOUT(		\
		hProcess,			\
		MilliSec,			\
		phTmo				\
		)				\
	UPFTASK_MilliTimeout(			\
		hProcess,			\
		MilliSec,			\
		phTmo				\
		)

#define __TMS_UTASK_MALLOCZERO(			\
		hProcess,			\
		nBytes				\
		)				\
	UPFTASK_MallocZero(			\
		hProcess,			\
		nBytes				\
		)

#define __TMS_UTASK_FREE(			\
		hProcess,			\
		pBlock,				\
		nBytes				\
		)				\
	UPFTASK_Free(				\
		hProcess,			\
		pBlock,				\
		nBytes				\
		)

#if __TMS_USBPUMP_TRACE
# define __TMS_UTASK_DBGPRINTF(args)		UPFTASK_DbgPrintf args
#else
# define __TMS_UTASK_DBGPRINTF(args)		__TMS_SKIP()
#endif

#define __TMS_UTASK_MSG_ALLOCATEBUFFER_EX(	\
		hMsg,				\
		iBuf,				\
		nBuf,				\
		sizeBuf,			\
		AccessFlags,			\
		ppBuf				\
		)				\
	UPFTASK_MsgAllocateBufferEx(		\
		hMsg,				\
		iBuf,				\
		nBuf,				\
		sizeBuf,			\
		AccessFlags,			\
		ppBuf				\
		)

#define __TMS_UTASK_MSG_ALLOCATEREPLY(		\
		hRequestMsg,			\
		ReplyMsgCode,			\
		pEstimatedBufSize,		\
		phReplyMsg,			\
		ppReplyMsgBody			\
		)				\
	UPFTASK_MsgAllocateReply(		\
		hRequestMsg,			\
		ReplyMsgCode,			\
		pEstimatedBufSize,		\
		phReplyMsg,			\
		ppReplyMsgBody			\
		)

#define __TMS_UTASK_MSG_ATTACHBUFFER_EX(	\
		hMsg,				\
		iBuf,				\
		AccessFlags,			\
		nBuf,				\
		sizeBuf,			\
		pBuf				\
		)				\
	UPFTASK_MsgAttachBufferEx(		\
		hMsg,				\
		iBuf,				\
		AccessFlags,			\
		nBuf,				\
		sizeBuf,			\
		pBuf				\
		)

#define __TMS_UTASK_MSG_CLOSE(phMsg)		UPFTASK_MsgClose(phMsg)

#define __TMS_UTASK_MSG_GETBODY(		\
		hMsg,				\
		ppMsgBody			\
		)				\
	UPFTASK_MsgGetBody(			\
		hMsg,				\
		ppMsgBody			\
		)

#define __TMS_UTASK_MSG_GETBUFFER(		\
		hMsg,				\
		iBuf,				\
		phBuf				\
		)				\
	UPFTASK_MsgGetBuffer(			\
		hMsg,				\
		iBuf,				\
		phBuf				\
		)

#define __TMS_UTASK_MSG_GETMSGCODE(		\
		hMsg,				\
		pMsgCode			\
		)				\
	UPFTASK_MsgGetMsgCode(			\
		hMsg,				\
		pMsgCode			\
		)

#define __TMS_UTASK_MSG_GETSENDER(		\
		hMsg,				\
		phMailbox,			\
		piProc				\
		)				\
	UPFTASK_MsgGetSender(			\
		hMsg,				\
		phMailbox,			\
		piProc				\
		)

#define __TMS_UTASK_MSG_SENDINDICATION(		\
		hProc,				\
		phIndication,			\
		hDestMailbox,			\
		iDestProcess,			\
		pMsgSeq				\
		)				\
	UPFTASK_MsgSendIndication(		\
		hProc,				\
		phIndication,			\
		hDestMailbox,			\
		iDestProcess,			\
		pMsgSeq				\
		)

#define __TMS_UTASK_MSG_SENDREPLY(		\
		hProc,				\
		phReply,			\
		pMsgSeq				\
		)				\
	UPFTASK_MsgSendReply(			\
		hProc,				\
		phReply,			\
		pMsgSeq				\
		)

#define __TMS_UTASK_MSG_SENDREQUEST(		\
		hProc,				\
		phRequest,			\
		hDestMailbox,			\
		iDestProcess,			\
		pMsgSeq				\
		)				\
	UPFTASK_MsgSendRequest(			\
		hProc,				\
		phRequest,			\
		hDestMailbox,			\
		iDestProcess,			\
		pMsgSeq				\
		)

#define __TMS_UTASK_MSG_SENDREQUESTWAITREPLY(	\
		hProc,				\
		phRequest,			\
		hDestMailbox,			\
		iDestProcess,			\
		phReply,			\
		ReplyMsgCode,			\
		hTmo,				\
		pRequestSeq			\
		)				\
	UPFTASK_MsgSendRequestWaitReply(	\
		hProc,				\
		phRequest,			\
		hDestMailbox,			\
		iDestProcess,			\
		phReply,			\
		ReplyMsgCode,			\
		hTmo,				\
		pRequestSeq			\
		)

#define __TMS_UTASK_BUFFER_CLOSE(phBuf)		UPFTASK_BufferClose(phBuf)

#define __TMS_UTASK_BUFFER_GETREFERENCE(	\
		hBuf,				\
		Offset,				\
		Length,				\
		pnBuf,				\
		psizeBuf,			\
		ppBuf				\
		)				\
	UPFTASK_BufferGetReference(		\
		hBuf,				\
		Offset,				\
		Length,				\
		pnBuf,				\
		psizeBuf,			\
		ppBuf				\
		)

#define __TMS_UTASK_BUFFER_GETREFERENCE_EX(	\
		hBuf,				\
		Offset,				\
		Length,				\
		AccessFlags,			\
		pLocalBuf,			\
		SizeLocalBuf,			\
		pnBuf,				\
		psizeBuf,			\
		ppBuf				\
		)				\
	UPFTASK_BufferGetReferenceEx(		\
		hBuf,				\
		Offset,				\
		Length,				\
		AccessFlags,			\
		pLocalBuf,			\
		SizeLocalBuf,			\
		pnBuf,				\
		psizeBuf,			\
		ppBuf				\
		)

#define __TMS_UTASK_BUFFER_SETLENGTH(		\
		hBuf,				\
		nBuf				\
		)				\
	UPFTASK_BufferSetLength(		\
		hBuf,				\
		nBuf				\
		)

/* get the message base */
#define __TMS_UTASK_GET_MESSAGE_BASE(pRoot)	(pRoot)->MessageBase

/*
|| Define derived functions
*/
#define __TMS_UTASK_MSG_HANDLE_INIT(ARG_hMsg)							\
	do {	ARG_hMsg = __TMS_UTASK_INVALID_MSG_HANDLE; } while (0)

#define __TMS_UTASK_BUFFER_HANDLE_INIT(ARG_hBuf)						\
	do {	ARG_hBuf = __TMS_UTASK_INVALID_BUFFER_HANDLE; } while (0)

#define __TMS_UTASK_MSG_ALLOCATEBUFFER_RO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_ppBuf			\
		)				\
	__TMS_UTASK_MSG_ALLOCATEBUFFER_EX(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_nBuf,			\
		__TMS_UTASK_MSG_ACCESSFLAGS_READ,	\
		ARG_ppBuf			\
		)

#define __TMS_UTASK_MSG_ALLOCATEBUFFER_WO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_sizeBuf			\
		)				\
	__TMS_UTASK_MSG_ALLOCATEBUFFER_EX(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		0,				\
		ARG_sizeBuf,			\
		__TMS_UTASK_MSG_ACCESSFLAGS_WRITE,	\
		__TMS_NULL			\
		)

#define __TMS_UTASK_MSG_ALLOCATEBUFFER_RW(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_sizeBuf,			\
		ARG_ppBuf			\
		)				\
	__TMS_UTASK_MSG_ALLOCATEBUFFER_EX(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_sizeBuf,			\
		__TMS_UTASK_MSG_ACCESSFLAGS_READWRITE,	\
		ARG_ppBuf			\
		)

#define __TMS_UTASK_MSG_ATTACHBUFFER_RO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_pBuf			\
		)				\
	__TMS_UTASK_MSG_ATTACHBUFFER_EX(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		__TMS_UTASK_MSG_ACCESSFLAGS_READ,	\
		ARG_nBuf,			\
		ARG_nBuf,			\
		ARG_pBuf			\
		)

#define __TMS_UTASK_MSG_ATTACHBUFFER_WO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_sizeBuf,			\
		ARG_pBuf			\
		)				\
	__TMS_UTASK_MSG_ATTACHBUFFER_EX(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		__TMS_UTASK_MSG_ACCESSFLAGS_WRITE,	\
		0,				\
		ARG_sizeBuf,			\
		ARG_pBuf			\
		)

#define __TMS_UTASK_MSG_ATTACHBUFFER_RW(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_sizeBuf,			\
		ARG_pBuf			\
		)				\
	__TMS_UTASK_MSG_ATTACHBUFFER_EX(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		__TMS_UTASK_MSG_ACCESSFLAGS_READWRITE,	\
		ARG_nBuf,			\
		ARG_sizeBuf,			\
		ARG_pBuf			\
		)

#define __TMS_UTASK_MSG_NUMBUFFERS(ARG_hMsg, ARG_pnBuf)			\
	do	{							\
		__TMS_UTASK_MSGCODE	ARG_MsgCode;			\
		__TMS_UTASK_MSG_GETMSGCODE(ARG_hMsg, &ARG_MsgCode);	\
		*(ARG_pnBuf) = __TMS_UTASK_MSGCODE_NBUF(ARG_MsgCode);	\
		} while (0)


/****************************************************************************\
|
|	UTASK Software check code
|
\****************************************************************************/

#define __TMS_UTASKERR_BAD_MESSAGECODE	(__TMS_UHILSWC_UTASKBASE | 1)


/****************************************************************************\
|
|	Function prototypes and declarations.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UTASK_CREATETASK_FN			UPFTASK_CreateTask;
__TMS_UTASK_STARTTASK_FN			UPFTASK_StartTask;
__TMS_UTASK_LOCATETASK_FN			UPFTASK_LocateTask;
__TMS_UTASK_LOCATEPROCESS_FN			UPFTASK_LocateProcess;
__TMS_UTASK_ALLOCATEMESSAGE_FN			UPFTASK_AllocateMessage;
__TMS_UTASK_MILLITIMEOUT_FN			UPFTASK_MilliTimeout;
__TMS_UTASK_MALLOCZERO_FN			UPFTASK_MallocZero;
__TMS_UTASK_FREE_FN				UPFTASK_Free;
__TMS_UTASK_DBGPRINTF_FN			UPFTASK_DbgPrintf;

__TMS_UTASK_MSG_ALLOCATEBUFFER_EX_FN		UPFTASK_MsgAllocateBufferEx;
__TMS_UTASK_MSG_ALLOCATEREPLY_FN		UPFTASK_MsgAllocateReply;
__TMS_UTASK_MSG_ATTACHBUFFER_EX_FN		UPFTASK_MsgAttachBufferEx;
__TMS_UTASK_MSG_CLOSE_FN			UPFTASK_MsgClose;
__TMS_UTASK_MSG_GETBODY_FN			UPFTASK_MsgGetBody;
__TMS_UTASK_MSG_GETBUFFER_FN			UPFTASK_MsgGetBuffer;
__TMS_UTASK_MSG_GETMSGCODE_FN			UPFTASK_MsgGetMsgCode;
__TMS_UTASK_MSG_GETSENDER_FN			UPFTASK_MsgGetSender;
__TMS_UTASK_MSG_SENDINDICATION_FN		UPFTASK_MsgSendIndication;
__TMS_UTASK_MSG_SENDREPLY_FN			UPFTASK_MsgSendReply;
__TMS_UTASK_MSG_SENDREQUEST_FN			UPFTASK_MsgSendRequest;
__TMS_UTASK_MSG_SENDREQUESTWAITREPLY_FN		UPFTASK_MsgSendRequestWaitReply;

__TMS_UTASK_BUFFER_CLOSE_FN			UPFTASK_BufferClose;
__TMS_UTASK_BUFFER_GETREFERENCE_FN		UPFTASK_BufferGetReference;
__TMS_UTASK_BUFFER_GETREFERENCE_EX_FN		UPFTASK_BufferGetReferenceEx;
__TMS_UTASK_BUFFER_SETLENGTH_FN			UPFTASK_BufferSetLength;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UTASK_MAX_BUF_LENGTH	\
   __TMS_UTASK_MAX_BUF_LENGTH
# define UTASK_INVALID_BUF_LENGTH	\
   __TMS_UTASK_INVALID_BUF_LENGTH
# define UTASK_TMO_INFINITE	\
   __TMS_UTASK_TMO_INFINITE
# define UTASK_INVALID_HANDLE	\
   __TMS_UTASK_INVALID_HANDLE
# define UTASK_INVALID_PROCESS_HANDLE	\
   __TMS_UTASK_INVALID_PROCESS_HANDLE
# define UTASK_INVALID_MAILBOX_HANDLE	\
   __TMS_UTASK_INVALID_MAILBOX_HANDLE
# define UTASK_INVALID_MSG_HANDLE	\
   __TMS_UTASK_INVALID_MSG_HANDLE
# define UTASK_INVALID_BUFFER_HANDLE	\
   __TMS_UTASK_INVALID_BUFFER_HANDLE
# define UTASK_INVALID_TIMEOUT_HANDLE	\
   __TMS_UTASK_INVALID_TIMEOUT_HANDLE
# define UTASK_INVALID_COMPLETION_HANDLE	\
   __TMS_UTASK_INVALID_COMPLETION_HANDLE
# define UTASK_MSG_ACCESSFLAGS_READ	\
   __TMS_UTASK_MSG_ACCESSFLAGS_READ
# define UTASK_MSG_ACCESSFLAGS_WRITE	\
   __TMS_UTASK_MSG_ACCESSFLAGS_WRITE
# define UTASK_MSG_ACCESSFLAGS_READWRITE	\
   __TMS_UTASK_MSG_ACCESSFLAGS_READWRITE
# define UTASK_MSG_ACCESSFLAGS_ALLOCED	\
   __TMS_UTASK_MSG_ACCESSFLAGS_ALLOCED
# define UTASK_MSG_ACCESSFLAGS_LENGTH_VALID	\
   __TMS_UTASK_MSG_ACCESSFLAGS_LENGTH_VALID
# define UTASK_PROCESS_CONFIG_INIT_V1(		\
		ARG_pName,				\
		ARG_pInitFn,				\
		ARG_pExitFn,				\
		ARG_pMsgDispatchFn,			\
		ARG_pOptionalInfo			\
		)	\
	__TMS_UTASK_PROCESS_CONFIG_INIT_V1(		\
		ARG_pName,				\
		ARG_pInitFn,				\
		ARG_pExitFn,				\
		ARG_pMsgDispatchFn,			\
		ARG_pOptionalInfo			\
		)
# define UTASK_PROCESS_CONFIG_SETUP_V1( 			\
		ARG_pProcCfg,					\
		ARG_pName, 					\
		ARG_pInitFn, 					\
		ARG_pExitFn, 					\
		ARG_pMsgDispatchFn, 				\
		ARG_pOptionalInfo 				\
		)	\
	__TMS_UTASK_PROCESS_CONFIG_SETUP_V1( 			\
		ARG_pProcCfg,					\
		ARG_pName, 					\
		ARG_pInitFn, 					\
		ARG_pExitFn, 					\
		ARG_pMsgDispatchFn, 				\
		ARG_pOptionalInfo 				\
		)
# define UTASK_CONFIG_HDR_INIT_V4(				\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags,					\
		ARG_pOptionalOsInfo				\
		)	\
	__TMS_UTASK_CONFIG_HDR_INIT_V4(				\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags,					\
		ARG_pOptionalOsInfo				\
		)
# define UTASK_CONFIG_HDR_INIT_V3(				\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags					\
		)	\
	__TMS_UTASK_CONFIG_HDR_INIT_V3(				\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags					\
		)
# define UTASK_CONFIG_HDR_INIT_V2(				\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool					\
		)	\
	__TMS_UTASK_CONFIG_HDR_INIT_V2(				\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool					\
		)
# define UTASK_CONFIG_HDR_INIT_V1(				\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg					\
		)	\
	__TMS_UTASK_CONFIG_HDR_INIT_V1(				\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg					\
		)
# define UTASK_CONFIG_HDR_SETUP_V4(			\
		ARG_pConfig,					\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags,					\
		ARG_pOptionalOsInfo				\
		)	\
	__TMS_UTASK_CONFIG_HDR_SETUP_V4(			\
		ARG_pConfig,					\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags,					\
		ARG_pOptionalOsInfo				\
		)
# define UTASK_CONFIG_HDR_SETUP_V3(			\
		ARG_pConfig,					\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags					\
		)	\
	__TMS_UTASK_CONFIG_HDR_SETUP_V3(			\
		ARG_pConfig,					\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn, /* NULL to get default */	\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool,				\
		ARG_DebugFlags					\
		)
# define UTASK_CONFIG_HDR_SETUP_V2(			\
		ARG_pConfig,					\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool					\
		)	\
	__TMS_UTASK_CONFIG_HDR_SETUP_V2(			\
		ARG_pConfig,					\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_pMemPool,					\
		ARG_sizeMemPool					\
		)
# define UTASK_CONFIG_HDR_SETUP_V1(			\
		ARG_pConfig,					\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_nProcCfg					\
		)	\
	__TMS_UTASK_CONFIG_HDR_SETUP_V1(			\
		ARG_pConfig,					\
		ARG_sizeCfg,					\
		ARG_pName,					\
		ARG_pMsgLoopFn,					\
		ARG_pvProcCfg,					\
		ARG_nProcCfg					\
		)
# define UTASK_PROC_HANDLE_TO_TASK_HANDLE(ARG_hProc)	\
   __TMS_UTASK_PROC_HANDLE_TO_TASK_HANDLE(ARG_hProc)
# define UTASK_ROOT_CONFIG_INIT_V2(				\
	a_pTaskRoot,							\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)	\
	__TMS_UTASK_ROOT_CONFIG_INIT_V2(				\
	a_pTaskRoot,							\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)
# define UTASK_ROOT_CONFIG_INIT_V1(				\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)	\
	__TMS_UTASK_ROOT_CONFIG_INIT_V1(				\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)
# define UTASK_ROOT_CONFIG_SETUP_V2(				\
	a_pTaskRootConfig,						\
	a_pTaskRoot,							\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)	\
	__TMS_UTASK_ROOT_CONFIG_SETUP_V2(				\
	a_pTaskRootConfig,						\
	a_pTaskRoot,							\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)
# define UTASK_ROOT_CONFIG_SETUP_V1(				\
	a_pTaskRootConfig,						\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)	\
	__TMS_UTASK_ROOT_CONFIG_SETUP_V1(				\
	a_pTaskRootConfig,						\
	a_sizeTaskRoot,							\
	a_pRootImplementation,						\
	a_MessageBase,							\
	a_pTaskRootName,						\
	a_pClientInitFn,						\
	a_pClientInitInfo						\
	)
# define UTASK_ROOT_IMPLEMENTATION_INIT_V1(			\
	a_sizeTaskRootMin,						\
	a_pRootInitFn,							\
	a_pIoctlFn,							\
	a_pSwitch							\
	)	\
	__TMS_UTASK_ROOT_IMPLEMENTATION_INIT_V1(			\
	a_sizeTaskRootMin,						\
	a_pRootInitFn,							\
	a_pIoctlFn,							\
	a_pSwitch							\
	)
# define UTASK_ROOT_TAG	\
   __TMS_UTASK_ROOT_TAG
# define UTASK_ROOT_NAME(Text)	\
   __TMS_UTASK_ROOT_NAME(Text)
# define UTASK_SWITCH_BEGIN	\
   __TMS_UTASK_SWITCH_BEGIN
# define UTASK_SWITCH_END	\
   __TMS_UTASK_SWITCH_END
# define UTASK_MSG_SWITCH_BEGIN	\
   __TMS_UTASK_MSG_SWITCH_BEGIN
# define UTASK_MSG_SWITCH_END	\
   __TMS_UTASK_MSG_SWITCH_END
# define UTASK_BUF_SWITCH_BEGIN	\
   __TMS_UTASK_BUF_SWITCH_BEGIN
# define UTASK_BUF_SWITCH_END	\
   __TMS_UTASK_BUF_SWITCH_END
# define UTASK_SWITCH_INIT_V1(		\
		ARG_pCreateTask,		\
		ARG_pStartTask,			\
		ARG_pLocateTask,		\
		ARG_pLocateProcess,		\
		ARG_pAllocateMessage,		\
		ARG_pMilliTimeout,		\
		ARG_pMallocZero,		\
		ARG_pFree,			\
		ARG_pDbgPrintf,			\
		ARG_pMsgSwitch,			\
		ARG_pBufSwitch			\
		)	\
	__TMS_UTASK_SWITCH_INIT_V1(		\
		ARG_pCreateTask,		\
		ARG_pStartTask,			\
		ARG_pLocateTask,		\
		ARG_pLocateProcess,		\
		ARG_pAllocateMessage,		\
		ARG_pMilliTimeout,		\
		ARG_pMallocZero,		\
		ARG_pFree,			\
		ARG_pDbgPrintf,			\
		ARG_pMsgSwitch,			\
		ARG_pBufSwitch			\
		)
# define UTASK_MSG_SWITCH_INIT_V1(		\
		ARG_pAllocateBufferEx,		\
		ARG_pAllocateReply,		\
		ARG_pAttachBufferEx,		\
		ARG_pClose,			\
		ARG_pGetBody,			\
		ARG_pGetBuffer,			\
		ARG_pGetMessageCode,		\
		ARG_pGetSender,			\
		ARG_pSendIndication,		\
		ARG_pSendReply,			\
		ARG_pSendRequest,		\
		ARG_pSendRequestWaitReply	\
		)	\
	__TMS_UTASK_MSG_SWITCH_INIT_V1(		\
		ARG_pAllocateBufferEx,		\
		ARG_pAllocateReply,		\
		ARG_pAttachBufferEx,		\
		ARG_pClose,			\
		ARG_pGetBody,			\
		ARG_pGetBuffer,			\
		ARG_pGetMessageCode,		\
		ARG_pGetSender,			\
		ARG_pSendIndication,		\
		ARG_pSendReply,			\
		ARG_pSendRequest,		\
		ARG_pSendRequestWaitReply	\
		)
# define UTASK_BUF_SWITCH_INIT_V1(		\
		ARG_pClose,			\
		ARG_pGetReference,		\
		ARG_pGetReferenceEx,		\
		ARG_pSetLength			\
		)	\
	__TMS_UTASK_BUF_SWITCH_INIT_V1(		\
		ARG_pClose,			\
		ARG_pGetReference,		\
		ARG_pGetReferenceEx,		\
		ARG_pSetLength			\
		)
# define UTASK_CREATETASK(			\
		pPlatform,			\
		pTaskCfgHdr			\
		)	\
	__TMS_UTASK_CREATETASK(			\
		pPlatform,			\
		pTaskCfgHdr			\
		)
# define UTASK_STARTTASK(pTaskName)	\
   __TMS_UTASK_STARTTASK(pTaskName)
# define UTASK_LOCATETASK(			\
		pTaskName,			\
		phMailbox			\
		)	\
	__TMS_UTASK_LOCATETASK(			\
		pTaskName,			\
		phMailbox			\
		)
# define UTASK_LOCATEPROCESS(		\
		hProcess,			\
		pTaskName,			\
		pProcName,			\
		phMailbox,			\
		piProc				\
		)	\
	__TMS_UTASK_LOCATEPROCESS(		\
		hProcess,			\
		pTaskName,			\
		pProcName,			\
		phMailbox,			\
		piProc				\
		)
# define UTASK_ALLOCATEMESSAGE(		\
		hProcess,			\
		MsgCode,			\
		EstimatedBufSize,		\
		phMsg,				\
		ppMsgBody			\
		)	\
	__TMS_UTASK_ALLOCATEMESSAGE(		\
		hProcess,			\
		MsgCode,			\
		EstimatedBufSize,		\
		phMsg,				\
		ppMsgBody			\
		)
# define UTASK_MILLITIMEOUT(		\
		hProcess,			\
		MilliSec,			\
		phTmo				\
		)	\
	__TMS_UTASK_MILLITIMEOUT(		\
		hProcess,			\
		MilliSec,			\
		phTmo				\
		)
# define UTASK_MALLOCZERO(			\
		hProcess,			\
		nBytes				\
		)	\
	__TMS_UTASK_MALLOCZERO(			\
		hProcess,			\
		nBytes				\
		)
# define UTASK_FREE(			\
		hProcess,			\
		pBlock,				\
		nBytes				\
		)	\
	__TMS_UTASK_FREE(			\
		hProcess,			\
		pBlock,				\
		nBytes				\
		)
# define UTASK_DBGPRINTF(args)	\
   __TMS_UTASK_DBGPRINTF(args)
# define UTASK_DBGPRINTF(args)	\
   __TMS_UTASK_DBGPRINTF(args)
# define UTASK_MSG_ALLOCATEBUFFER_EX(	\
		hMsg,				\
		iBuf,				\
		nBuf,				\
		sizeBuf,			\
		AccessFlags,			\
		ppBuf				\
		)	\
	__TMS_UTASK_MSG_ALLOCATEBUFFER_EX(	\
		hMsg,				\
		iBuf,				\
		nBuf,				\
		sizeBuf,			\
		AccessFlags,			\
		ppBuf				\
		)
# define UTASK_MSG_ALLOCATEREPLY(		\
		hRequestMsg,			\
		ReplyMsgCode,			\
		pEstimatedBufSize,		\
		phReplyMsg,			\
		ppReplyMsgBody			\
		)	\
	__TMS_UTASK_MSG_ALLOCATEREPLY(		\
		hRequestMsg,			\
		ReplyMsgCode,			\
		pEstimatedBufSize,		\
		phReplyMsg,			\
		ppReplyMsgBody			\
		)
# define UTASK_MSG_ATTACHBUFFER_EX(	\
		hMsg,				\
		iBuf,				\
		AccessFlags,			\
		nBuf,				\
		sizeBuf,			\
		pBuf				\
		)	\
	__TMS_UTASK_MSG_ATTACHBUFFER_EX(	\
		hMsg,				\
		iBuf,				\
		AccessFlags,			\
		nBuf,				\
		sizeBuf,			\
		pBuf				\
		)
# define UTASK_MSG_CLOSE(phMsg)	\
   __TMS_UTASK_MSG_CLOSE(phMsg)
# define UTASK_MSG_GETBODY(		\
		hMsg,				\
		ppMsgBody			\
		)	\
	__TMS_UTASK_MSG_GETBODY(		\
		hMsg,				\
		ppMsgBody			\
		)
# define UTASK_MSG_GETBUFFER(		\
		hMsg,				\
		iBuf,				\
		phBuf				\
		)	\
	__TMS_UTASK_MSG_GETBUFFER(		\
		hMsg,				\
		iBuf,				\
		phBuf				\
		)
# define UTASK_MSG_GETMSGCODE(		\
		hMsg,				\
		pMsgCode			\
		)	\
	__TMS_UTASK_MSG_GETMSGCODE(		\
		hMsg,				\
		pMsgCode			\
		)
# define UTASK_MSG_GETSENDER(		\
		hMsg,				\
		phMailbox,			\
		piProc				\
		)	\
	__TMS_UTASK_MSG_GETSENDER(		\
		hMsg,				\
		phMailbox,			\
		piProc				\
		)
# define UTASK_MSG_SENDINDICATION(		\
		hProc,				\
		phIndication,			\
		hDestMailbox,			\
		iDestProcess,			\
		pMsgSeq				\
		)	\
	__TMS_UTASK_MSG_SENDINDICATION(		\
		hProc,				\
		phIndication,			\
		hDestMailbox,			\
		iDestProcess,			\
		pMsgSeq				\
		)
# define UTASK_MSG_SENDREPLY(		\
		hProc,				\
		phReply,			\
		pMsgSeq				\
		)	\
	__TMS_UTASK_MSG_SENDREPLY(		\
		hProc,				\
		phReply,			\
		pMsgSeq				\
		)
# define UTASK_MSG_SENDREQUEST(		\
		hProc,				\
		phRequest,			\
		hDestMailbox,			\
		iDestProcess,			\
		pMsgSeq				\
		)	\
	__TMS_UTASK_MSG_SENDREQUEST(		\
		hProc,				\
		phRequest,			\
		hDestMailbox,			\
		iDestProcess,			\
		pMsgSeq				\
		)
# define UTASK_MSG_SENDREQUESTWAITREPLY(	\
		hProc,				\
		phRequest,			\
		hDestMailbox,			\
		iDestProcess,			\
		phReply,			\
		ReplyMsgCode,			\
		hTmo,				\
		pRequestSeq			\
		)	\
	__TMS_UTASK_MSG_SENDREQUESTWAITREPLY(	\
		hProc,				\
		phRequest,			\
		hDestMailbox,			\
		iDestProcess,			\
		phReply,			\
		ReplyMsgCode,			\
		hTmo,				\
		pRequestSeq			\
		)
# define UTASK_BUFFER_CLOSE(phBuf)	\
   __TMS_UTASK_BUFFER_CLOSE(phBuf)
# define UTASK_BUFFER_GETREFERENCE(	\
		hBuf,				\
		Offset,				\
		Length,				\
		pnBuf,				\
		psizeBuf,			\
		ppBuf				\
		)	\
	__TMS_UTASK_BUFFER_GETREFERENCE(	\
		hBuf,				\
		Offset,				\
		Length,				\
		pnBuf,				\
		psizeBuf,			\
		ppBuf				\
		)
# define UTASK_BUFFER_GETREFERENCE_EX(	\
		hBuf,				\
		Offset,				\
		Length,				\
		AccessFlags,			\
		pLocalBuf,			\
		SizeLocalBuf,			\
		pnBuf,				\
		psizeBuf,			\
		ppBuf				\
		)	\
	__TMS_UTASK_BUFFER_GETREFERENCE_EX(	\
		hBuf,				\
		Offset,				\
		Length,				\
		AccessFlags,			\
		pLocalBuf,			\
		SizeLocalBuf,			\
		pnBuf,				\
		psizeBuf,			\
		ppBuf				\
		)
# define UTASK_BUFFER_SETLENGTH(		\
		hBuf,				\
		nBuf				\
		)	\
	__TMS_UTASK_BUFFER_SETLENGTH(		\
		hBuf,				\
		nBuf				\
		)
# define UTASK_GET_MESSAGE_BASE(pRoot)	\
   __TMS_UTASK_GET_MESSAGE_BASE(pRoot)
# define UTASK_MSG_HANDLE_INIT(ARG_hMsg)	\
   __TMS_UTASK_MSG_HANDLE_INIT(ARG_hMsg)
# define UTASK_BUFFER_HANDLE_INIT(ARG_hBuf)	\
   __TMS_UTASK_BUFFER_HANDLE_INIT(ARG_hBuf)
# define UTASK_MSG_ALLOCATEBUFFER_RO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_ppBuf			\
		)	\
	__TMS_UTASK_MSG_ALLOCATEBUFFER_RO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_ppBuf			\
		)
# define UTASK_MSG_ALLOCATEBUFFER_WO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_sizeBuf			\
		)	\
	__TMS_UTASK_MSG_ALLOCATEBUFFER_WO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_sizeBuf			\
		)
# define UTASK_MSG_ALLOCATEBUFFER_RW(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_sizeBuf,			\
		ARG_ppBuf			\
		)	\
	__TMS_UTASK_MSG_ALLOCATEBUFFER_RW(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_sizeBuf,			\
		ARG_ppBuf			\
		)
# define UTASK_MSG_ATTACHBUFFER_RO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_pBuf			\
		)	\
	__TMS_UTASK_MSG_ATTACHBUFFER_RO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_pBuf			\
		)
# define UTASK_MSG_ATTACHBUFFER_WO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_sizeBuf,			\
		ARG_pBuf			\
		)	\
	__TMS_UTASK_MSG_ATTACHBUFFER_WO(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_sizeBuf,			\
		ARG_pBuf			\
		)
# define UTASK_MSG_ATTACHBUFFER_RW(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_sizeBuf,			\
		ARG_pBuf			\
		)	\
	__TMS_UTASK_MSG_ATTACHBUFFER_RW(	\
		ARG_hMsg,			\
		ARG_iBuf,			\
		ARG_nBuf,			\
		ARG_sizeBuf,			\
		ARG_pBuf			\
		)
# define UTASK_MSG_NUMBUFFERS(ARG_hMsg, ARG_pnBuf)	\
   __TMS_UTASK_MSG_NUMBUFFERS(ARG_hMsg, ARG_pnBuf)
# define UTASKERR_BAD_MESSAGECODE	\
   __TMS_UTASKERR_BAD_MESSAGECODE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of utask.h ****/
#endif /* _UTASK_H_ */
