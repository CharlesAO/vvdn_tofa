/* uplatform.h	Mon Jun  1 2015 00:19:29 tmm */

/*

Module:  uplatform.h

Function:
	Definitions for the UPLATFORM structure.

Version:
	V3.15b	Mon Jun  1 2015 00:19:29 tmm	Edit level 16

Copyright notice:
	This file copyright (C) 2001-2007, 2009-2010, 2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	November 2001

Revision history:
   1.63d  Thu Nov  8 2001 08:03:33  tmm
	Module created.

   1.79a  6/5/2002  tmm
	Add UHIL_di, UHIL_setpsw; these are exported by the platform
	code, so we think they belong here.

   1.83e  3/6/2003  chwon
	Change struct TTUSB_PLATFORM to __TMS_UPLATFORM.

   1.91b  Mon Apr 04 2005 10:26:15 maw
   	Added pointer for object-handle hash-table to UPLATFORM

   1.91d  Tue Jun 14 2005 13:07:04 maw
   	Renamed USBTASK_.. to UTASK_...

   1.91g  Thu Jul 14 2005 00:22:16  tmm
	Carry a pointer to the UTASK_ROOT in the uplatform, and add
	updated uplatform init var; and change the hitherto unused
	signatures of upf_pDi and ufp_pSetPsw to be proper methods.
	XXX shouldn't we just make UTASK_ROOT into a normal object?

   1.91g  2005-07-21  tmm
	Finish the 07-14 mods -- we now ask the client to provide
	a pointer to a UTASK_ROOT_CONFIG object.  This requires changes
	to macros and to UsbPumpPlatform_Setup().  Since 1.91g was never
	released, I didn't change the version of the init macros.
	Changed __self to __pSelf to make some obscure compilers happy.

   1.91h  Mon Jul 25 2005 16:39:52  chwon
   	Added timer handling field to UPLATFORM.
   	Remove including usbpump_timer.h because moved timer definitions
   	to central types file.

   1.97j  Thu Nov 01 2007 10:50:31  cccho
	4248: Add allocation tracking field to UPLATFORM.
	Generate uncloaked name using uncloak-defs.sh. Change company name.

   2.01a  Sun May 17 2009 10:56:25  tmm
	Add upf_hUhilAux and upf_pUhilAuxIncall to the platform.

   2.01a  Wed Oct 28 2009 10:35:07  chwon
	9173: Added UPLATFORM_INIT_V6() macro -- added pClassParent and
	pPlatformObjectName input parameter.

   2.01a  Fri Oct 30 2009 09:10:54  chwon
	9173: Improve documentation

   3.01a  Sun Jan 10 2010 09:54:15  chwon
	9720: Added UPLATFORM_CREATE_ABSTRACT_POOL_FN in the UPLATFORM and
	create UPLATFORM_INIT_V7() macro

   3.11d  Fri Nov 01 2013 13:10:04  chwon
	17827: Added upf_pAbstractPoolHead in the UPLATFORM.

   3.15b  Mon Jun  1 2015 00:19:29  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _UPLATFORM_H_		/* prevent multiple includes */
#define _UPLATFORM_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

#ifndef _BASEHILTYPES_H_
# include "basehiltypes.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

/****************************************************************************\
|
|	Low level platform interface declarations.
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
UPLATFORM_MALLOC_FN,
__TMS_VOID *,
	(
	__TMS_UPLATFORM *,
	__TMS_BYTES /* nBytes */
	));

__TMS_FNTYPE_DEF(
UPLATFORM_FREE_FN,
__TMS_VOID,
	(
	__TMS_UPLATFORM *,
	__TMS_VOID *, /* original pointer */
	__TMS_BYTES /* nBytes */
	));

__TMS_FNTYPE_DEF(
UPLATFORM_POST_EVENT_FN,
__TMS_VOID,
	(
	__TMS_UEVENTCONTEXT *,
	__TMS_CALLBACKCOMPLETION *
	));

__TMS_FNTYPE_DEF(
UPLATFORM_CHECK_EVENT_FN,
__TMS_CALLBACKCOMPLETION *,
	(
	__TMS_UEVENTCONTEXT *
	));


__TMS_FNTYPE_DEF(
UPLATFORM_GET_EVENT_FN,
__TMS_CALLBACKCOMPLETION *,
	(
	__TMS_UEVENTCONTEXT *,
	__TMS_BOOL	/* blocking (not non-blocking) */
	));

__TMS_FNTYPE_DEF(
UPLATFORM_YIELD_FN,
__TMS_VOID,
	(
	__TMS_UPLATFORM *
	));

__TMS_FNTYPE_DEF(
UPLATFORM_CLOSE_FN,
__TMS_VOID,
	(
	__TMS_UPLATFORM *
	));

/* declare the platform IOCTL function type as a typical IOCTL function */
__TMS_USBPUMP_DECLARE_IOCTL_FNTYPE(
	UPLATFORM_IOCTL_FN,
	__TMS_UPLATFORM *
	);

__TMS_FNTYPE_DEF(
UPLATFORM_DI_FN,
__TMS_CPUFLAGS,
	(
	__TMS_UPLATFORM *
	));

__TMS_FNTYPE_DEF(
UPLATFORM_SETPSW_FN,
__TMS_VOID,
	(
	__TMS_UPLATFORM *,
	__TMS_CPUFLAGS
	));

__TMS_FNTYPE_DEF(
UPLATFORM_CREATE_ABSTRACT_POOL_FN,
__TMS_USBPUMP_ABSTRACT_POOL *,
	(
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_BYTES		/* sizeMemoryPool */
	));

__TMS_TYPE_DEF_UNION(UPLATFORM_ABSTRACT_POOL);


/****************************************************************************\
|
|	The platform structure
|
\****************************************************************************/

struct TTUSB_PLATFORM
	{
	/*
	|| UPLATFORMs are USBPUMP objects, so must begin with a header.
	*/
	__TMS_USBPUMP_OBJECT_HEADER	upf_Header;

	/*
	|| The object header hash table is used to quickly find an object
	|| from an object handle
	*/
	__TMS_USBPUMP_OBJECT_HEADER **	upf_ppHashTbl;
	__TMS_BYTES			upf_nHashTbl;

	/*
	|| The event context block is used for event processing.
	|| It's only dereferenced by the event processing methods,
	|| and is treated as a private handle by the rest of the
	|| DataPump code.
	*/
	__TMS_UEVENTCONTEXT		*upf_pEventctx;

	/*
	|| The polling context block is used, on a somewhat application-
	|| specific basis, to hold context for outcalls for "polling"
	|| other subsystems.  The sample event loop, for example, uses
	|| this.  This should be treated as a private handle by modules
	|| outside of the polling code (UHIL_DoPoll).  In addition, this
	|| is obsolescent, and should be superceded by functionality in
	|| UPLATFORM_YIELD_FN.
	*/
	__TMS_UPOLLCONTEXT		*upf_pPollctx;

	/*
	|| The platform context pointer is a generic, opaque pointer for
	|| use by the platform code.
	*/
	__TMS_VOID			*upf_pContext;

	/*
	|| Method functions for a variety of abstract
	|| operations.
	*/
	__TMS_UPLATFORM_MALLOC_FN	*upf_pMalloc;
	__TMS_UPLATFORM_FREE_FN		*upf_pFree;
	__TMS_CONST __TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE *upf_pInterruptSystem;
	__TMS_UPLATFORM_POST_EVENT_FN	*upf_pPostEvent;
	__TMS_UPLATFORM_GET_EVENT_FN	*upf_pGetEvent;
	__TMS_UPLATFORM_CHECK_EVENT_FN	*upf_pCheckEvent;
	__TMS_UPLATFORM_YIELD_FN	*upf_pYield; /* OPTIONAL */

	__TMS_UPLATFORM_DEBUG_PRINT_CONTROL *upf_pDebugPrintControl;
	__TMS_UPLATFORM_CLOSE_FN	*upf_pPlatformClose;
	__TMS_UPLATFORM_IOCTL_FN	*upf_pIoctl;

	__TMS_UPLATFORM_DI_FN		*upf_pDi;
	__TMS_UPLATFORM_SETPSW_FN	*upf_pSetPsw;
	__TMS_UPLATFORM_CREATE_ABSTRACT_POOL_FN *upf_pCreateAbstractPool;

	/*
	|| A pointer to the task root dispatch object, or NULL if this
	|| implementation doesn't support (or doesn't have) such a concept.
	|| We spend the 4 bytes for this pointer, because this makes it easy
	|| for us to do all the initialization using the current paradigm
	*/
	__TMS_UTASK_ROOT		*upf_pTaskRoot;

	/* Timer support */
	__TMS_CONST __TMS_USBPUMP_TIMER_SWITCH *upf_pTimerSwitch;
	__TMS_VOID			*upf_pTimerContext;

	/* allocation tracking */
	__TMS_USBPUMP_ALLOCATION_TRACKING	*upf_pAllocationTracking;

	/* UHILAUX */
	__TMS_USBPUMP_SESSION_HANDLE	upf_hUhilAux;
	__TMS_USBPUMP_UHILAUX_INCALL	*upf_pUhilAuxIncall;

	/* platform abstract pool information */
	__TMS_USBPUMP_ABSTRACT_POOL	*upf_pAbstractPool;
	__TMS_BYTES			upf_PoolUsed;
	__TMS_UPLATFORM_ABSTRACT_POOL	*upf_pPlatformAbstractPoolHead;
	};

/*
|| The standard memory tag for UPLATFORMs
*/
#define __TMS_UPLATFORM_TAG	__TMS_UHIL_MEMTAG('U', 'P', 'f', 'm')

/*
|| The macro for generating UPLATFORM names
*/
#define	__TMS_UPLATFORM_NAME(Text)	Text ".platform.mcci.com"

/*

Name:  UPLATFORM_INIT_V1()

Function:
	Initialize a UPLATFORM structure (with version 1.60 assumptions).

Definition:
	VOID UPLATFORM_INIT_V1(
		UPLATFORM *pSelf,	// structure to init.
		VOID *pContext,		// arbitrary context pointer.
		UEVENTCONTEXT *pEvctx,	// event context
		UPOLLCONTEXT *pPoll,	// "polling context" -- not used.
		CONST UHIL_INTERRUPT_SYSTEM_INTERFACE
			*pIntIfc,	// interrupt system transfer vector
		UPLATFORM_DEBUG_PRINT_CONTROL *pPrintControl OPTIONAL
		//
		// the method functions
		//
		UPLATFORM_MALLOC_FN *pMalloc,
		UPLATFORM_FREE_FN *pFree,
		UPLATFORM_POST_EVENT_FN *pPostEvent,
		UPLATFORM_GET_EVENT_FN *pGetEvent,
		UPLATFORM_CHECK_EVENT_FN *pCheckEvent,
		UPLATFORM_YIELD_FN *pYield OPTIONAL
		);

Description:
	This macro generates the code to initialize a UPLATFORM according
	to V1 semantics.  This macro should be used instead of initializing
	the UPLATFORM manually, because it will be revised to maintain upward
	compatibility whenever new fields are added to the UPLATFORM.

Returns:
	No explicit result.

Notes:
	This function is obsolete.  Please use UPLATFORM_INIT_V2 in new
	code.

*/

/* don't add parameters to this macro; instead create a V2 */
#define	__TMS_UPLATFORM_INIT_V1(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */					\
		)							\
	__TMS_UPLATFORM_INIT_V2(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		(__TMS_UPLATFORM_DEBUG_PRINT_CONTROL *)__TMS_NULL, /* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		(__TMS_UPLATFORM_IOCTL_FN *)__TMS_NULL /* pIoctl */,	\
		(__TMS_UPLATFORM_CLOSE_FN *)__TMS_NULL /* pPlatformClose */	\
		)

/*

Name:  UPLATFORM_INIT_V2()

Function:
	Initialize a UPLATFORM structure (with version 1.60 assumptions).

Definition:
	VOID UPLATFORM_INIT_V2(
		UPLATFORM *pSelf,	// structure to init.
		VOID *pContext,		// arbitrary context pointer.
		UEVENTCONTEXT *pEvctx,	// event context
		UPOLLCONTEXT *pPoll,	// "polling context" -- not used.
		CONST UHIL_INTERRUPT_SYSTEM_INTERFACE
			*pIntIfc,	// interrupt system transfer vector
		UPLATFORM_DEBUG_PRINT_CONTROL *pPrintControl OPTIONAL
		//
		// the method functions
		//
		UPLATFORM_MALLOC_FN *pMalloc,
		UPLATFORM_FREE_FN *pFree,
		UPLATFORM_POST_EVENT_FN *pPostEvent,
		UPLATFORM_GET_EVENT_FN *pGetEvent,
		UPLATFORM_CHECK_EVENT_FN *pCheckEvent,
		UPLATFORM_YIELD_FN *pYield OPTIONAL,
		UPLATFORM_IOCTL_FN *pIoctl OPTIONAL,
		UPLATFORM_CLOSE_FN *pPlatformClose OPTIONAL
		);

Description:
	This macro generates the code to initialize a UPLATFORM according
	to V1 semantics.  This macro should be used instead of initializing
	the UPLATFORM manually, because it will be revised to maintain upward
	compatibility whenever new fields are added to the UPLATFORM.

	If you use this macro, the uplatform will be created with a default
	name ("default.platform.mcci.com"), and the central class object will
	be automatically created.

Returns:
	No explicit result.

*/

/* don't add parameters to this macro; instead create a V3 */
#define	__TMS_UPLATFORM_INIT_V2(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */				\
		)							\
	__TMS_UPLATFORM_INIT_V3(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		/* pDi */ UsbPumpPlatform_DisableInterrupts_Default,	\
		/* pSetPsw */ UsbPumpPlatform_SetPsw_Default,		\
		/* pTaskRootConfig */ (__TMS_CONST __TMS_UTASK_ROOT_CONFIG *)__TMS_NULL	\
		)

/*

Name:  UPLATFORM_INIT_V3()

Function:
	Initialize a UPLATFORM structure (with version 1.91 assumptions).

Definition:
	VOID UPLATFORM_INIT_V3(
		UPLATFORM *pSelf,	// structure to init.
		VOID *pContext,		// arbitrary context pointer.
		UEVENTCONTEXT *pEvctx,	// event context
		UPOLLCONTEXT *pPoll,	// "polling context" -- not used.
		CONST UHIL_INTERRUPT_SYSTEM_INTERFACE
			*pIntIfc,	// interrupt system transfer vector
		UPLATFORM_DEBUG_PRINT_CONTROL *pPrintControl OPTIONAL
		//
		// the method functions
		//
		UPLATFORM_MALLOC_FN *pMalloc,
		UPLATFORM_FREE_FN *pFree,
		UPLATFORM_POST_EVENT_FN *pPostEvent,
		UPLATFORM_GET_EVENT_FN *pGetEvent,
		UPLATFORM_CHECK_EVENT_FN *pCheckEvent,
		UPLATFORM_YIELD_FN *pYield OPTIONAL,
		UPLATFORM_IOCTL_FN *pIoctl OPTIONAL,
		UPLATFORM_CLOSE_FN *pPlatformClose OPTIONAL,
		UPLATFORM_DI_FN *pDi,
		UPLATFORM_SETPSW_FN *pSetPsw,
		CONST UTASK_ROOT_CONFIG *pTaskRootConfig OPTIONAL
		);

Description:
	This macro generates the code to initialize a UPLATFORM according
	to V1 semantics.  This macro should be used instead of initializing
	the UPLATFORM manually, because it will be revised to maintain upward
	compatibility whenever new fields are added to the UPLATFORM.

	If you use this macro, the uplatform will be created with a default
	name ("default.platform.mcci.com"), and the central class object will
	be automatically created.

Returns:
	No explicit result.

*/

/* don't add parameters to this macro; instead create a V4 */
#define	__TMS_UPLATFORM_INIT_V3(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		/* v3 */ pDi,						\
		/* v3 */ pSetPsw,					\
		/* v3 */ pTaskRootConfig /* OPTIONAL */			\
		)							\
	__TMS_UPLATFORM_INIT_V4(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		/* pTimerSwitch */ (__TMS_CONST __TMS_USBPUMP_TIMER_SWITCH *) __TMS_NULL \
		)

/*

Name:  UPLATFORM_INIT_V4()

Function:
	Initialize a UPLATFORM structure (with version 1.91 assumptions).

Definition:
	VOID UPLATFORM_INIT_V4(
		UPLATFORM *pSelf,	// structure to init.
		VOID *pContext,		// arbitrary context pointer.
		UEVENTCONTEXT *pEvctx,	// event context
		UPOLLCONTEXT *pPoll,	// "polling context" -- not used.
		CONST UHIL_INTERRUPT_SYSTEM_INTERFACE
			*pIntIfc,	// interrupt system transfer vector
		UPLATFORM_DEBUG_PRINT_CONTROL *pPrintControl OPTIONAL
		//
		// the method functions
		//
		UPLATFORM_MALLOC_FN *pMalloc,
		UPLATFORM_FREE_FN *pFree,
		UPLATFORM_POST_EVENT_FN *pPostEvent,
		UPLATFORM_GET_EVENT_FN *pGetEvent,
		UPLATFORM_CHECK_EVENT_FN *pCheckEvent,
		UPLATFORM_YIELD_FN *pYield OPTIONAL,
		UPLATFORM_IOCTL_FN *pIoctl OPTIONAL,
		UPLATFORM_CLOSE_FN *pPlatformClose OPTIONAL,
		UPLATFORM_DI_FN *pDi,
		UPLATFORM_SETPSW_FN *pSetPsw,
		CONST UTASK_ROOT_CONFIG *pTaskRootConfig OPTIONAL,
		CONST USBPUMP_TIMER_SWITCH *pTimerSwitch OPTIONAL
		);

Description:
	This macro generates the code to initialize a UPLATFORM according
	to V1 semantics.  This macro should be used instead of initializing
	the UPLATFORM manually, because it will be revised to maintain upward
	compatibility whenever new fields are added to the UPLATFORM.

	If you use this macro, the uplatform will be created with a default
	name ("default.platform.mcci.com"), and the central class object will
	be automatically created.

Returns:
	No explicit result.

*/

/* don't add parameters to this macro; instead create a V5 */
#define	__TMS_UPLATFORM_INIT_V4(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		/* v4 */ pTimerSwitch /* OPTIONAL */			\
		)							\
	__TMS_UPLATFORM_INIT_V5(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		(__TMS_USBPUMP_ALLOCATION_TRACKING *) __TMS_NULL	\
		)

/*

Name:  UPLATFORM_INIT_V5()

Function:
	Initialize a UPLATFORM structure (with version 1.97 assumptions).

Definition:
	VOID UPLATFORM_INIT_V5(
		UPLATFORM *pSelf,	// structure to init.
		VOID *pContext,		// arbitrary context pointer.
		UEVENTCONTEXT *pEvctx,	// event context
		UPOLLCONTEXT *pPoll,	// "polling context" -- not used.
		CONST UHIL_INTERRUPT_SYSTEM_INTERFACE
			*pIntIfc,	// interrupt system transfer vector
		UPLATFORM_DEBUG_PRINT_CONTROL *pPrintControl OPTIONAL
		//
		// the method functions
		//
		UPLATFORM_MALLOC_FN *pMalloc,
		UPLATFORM_FREE_FN *pFree,
		UPLATFORM_POST_EVENT_FN *pPostEvent,
		UPLATFORM_GET_EVENT_FN *pGetEvent,
		UPLATFORM_CHECK_EVENT_FN *pCheckEvent,
		UPLATFORM_YIELD_FN *pYield OPTIONAL,
		UPLATFORM_IOCTL_FN *pIoctl OPTIONAL,
		UPLATFORM_CLOSE_FN *pPlatformClose OPTIONAL,
		UPLATFORM_DI_FN *pDi,
		UPLATFORM_SETPSW_FN *pSetPsw,
		CONST UTASK_ROOT_CONFIG *pTaskRootConfig OPTIONAL,
		CONST USBPUMP_TIMER_SWITCH *pTimerSwitch OPTIONAL,
		USBPUMP_ALLOCATION_TRACKING *pAllocTracking OPTIONAL
		);

Description:
	This macro generates the code to initialize a UPLATFORM according
	to V1 semantics.  This macro should be used instead of initializing
	the UPLATFORM manually, because it will be revised to maintain upward
	compatibility whenever new fields are added to the UPLATFORM.

	If you use this macro, the uplatform will be created with a default
	name ("default.platform.mcci.com"), and the central class object will
	be automatically created.

Returns:
	No explicit result.

*/

/* don't add parameters to this macro; instead create a V6 */
#define	__TMS_UPLATFORM_INIT_V5(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		/* v5 */ pAllocTracking /* OPTIONAL */			\
		)							\
	__TMS_UPLATFORM_INIT_V6(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		pAllocTracking /* OPTIONAL */,				\
		/* v6: pClassParent OPTIONAL */ __TMS_NULL,		\
		/* v6: pPlatformObjectName OPTIONAL */ __TMS_NULL	\
		)

/*

Name:  UPLATFORM_INIT_V6()

Function:
	Initialize a UPLATFORM structure (with version 2.01 assumptions).

Definition:
	VOID UPLATFORM_INIT_V6(
		UPLATFORM *pSelf,	// structure to init.
		VOID *pContext,		// arbitrary context pointer.
		UEVENTCONTEXT *pEvctx,	// event context
		UPOLLCONTEXT *pPoll,	// "polling context" -- not used.
		CONST UHIL_INTERRUPT_SYSTEM_INTERFACE
			*pIntIfc,	// interrupt system transfer vector
		UPLATFORM_DEBUG_PRINT_CONTROL *pPrintControl OPTIONAL
		//
		// the method functions
		//
		UPLATFORM_MALLOC_FN *pMalloc,
		UPLATFORM_FREE_FN *pFree,
		UPLATFORM_POST_EVENT_FN *pPostEvent,
		UPLATFORM_GET_EVENT_FN *pGetEvent,
		UPLATFORM_CHECK_EVENT_FN *pCheckEvent,
		UPLATFORM_YIELD_FN *pYield OPTIONAL,
		UPLATFORM_IOCTL_FN *pIoctl OPTIONAL,
		UPLATFORM_CLOSE_FN *pPlatformClose OPTIONAL,
		UPLATFORM_DI_FN *pDi,
		UPLATFORM_SETPSW_FN *pSetPsw,
		CONST UTASK_ROOT_CONFIG *pTaskRootConfig OPTIONAL,
		CONST USBPUMP_TIMER_SWITCH *pTimerSwitch OPTIONAL,
		USBPUMP_ALLOCATION_TRACKING *pAllocTracking OPTIONAL,
		USBPUMP_OBJECT_HEADER *pClassParent OPTIONAL,
		CONST TEXT *pPlatformObjectName OPTIONAL
		);

Description:
	This macro generates the code to initialize a UPLATFORM according
	to V1 semantics.  This macro should be used instead of initializing
	the UPLATFORM manually, because it will be revised to maintain upward
	compatibility whenever new fields are added to the UPLATFORM.

	If you use this macro, the uplatform will be created with a
	pPlatformObjectName, and the central class object will be
	automatically created.

Returns:
	No explicit result.

*/

/* don't add parameters to this macro; instead create a V7 */
#define	__TMS_UPLATFORM_INIT_V6(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		pAllocTracking /* OPTIONAL */,				\
		/* v6 */ pClassParent /* OPTIONAL */,			\
		/* v6 */ pPlatformObjectName /* OPTIONAL */		\
		)							\
	__TMS_UPLATFORM_INIT_V7(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		pAllocTracking /* OPTIONAL */,				\
		pClassParent /* OPTIONAL */,				\
		pPlatformObjectName /* OPTIONAL */,			\
		/* V7: pCreateAbstractPool OPTIONAL */ __TMS_NULL	\
		)

/*

Name:  UPLATFORM_INIT_V7()

Function:
	Initialize a UPLATFORM structure (with version 3.01a assumptions).

Definition:
	VOID UPLATFORM_INIT_V7(
		UPLATFORM *pSelf,	// structure to init.
		VOID *pContext,		// arbitrary context pointer.
		UEVENTCONTEXT *pEvctx,	// event context
		UPOLLCONTEXT *pPoll,	// "polling context" -- not used.
		CONST UHIL_INTERRUPT_SYSTEM_INTERFACE
			*pIntIfc,	// interrupt system transfer vector
		UPLATFORM_DEBUG_PRINT_CONTROL *pPrintControl OPTIONAL
		//
		// the method functions
		//
		UPLATFORM_MALLOC_FN *pMalloc,
		UPLATFORM_FREE_FN *pFree,
		UPLATFORM_POST_EVENT_FN *pPostEvent,
		UPLATFORM_GET_EVENT_FN *pGetEvent,
		UPLATFORM_CHECK_EVENT_FN *pCheckEvent,
		UPLATFORM_YIELD_FN *pYield OPTIONAL,
		UPLATFORM_IOCTL_FN *pIoctl OPTIONAL,
		UPLATFORM_CLOSE_FN *pPlatformClose OPTIONAL,
		UPLATFORM_DI_FN *pDi,
		UPLATFORM_SETPSW_FN *pSetPsw,
		CONST UTASK_ROOT_CONFIG *pTaskRootConfig OPTIONAL,
		CONST USBPUMP_TIMER_SWITCH *pTimerSwitch OPTIONAL,
		USBPUMP_ALLOCATION_TRACKING *pAllocTracking OPTIONAL,
		USBPUMP_OBJECT_HEADER *pClassParent OPTIONAL,
		CONST TEXT *pPlatformObjectName OPTIONAL,
		UPLATFORM_CREATE_ABSTRACT_POOL_FN *pCreateAbstractPool OPTIONAL
		);

Description:
	This macro generates the code to initialize a UPLATFORM according
	to V7 semantics.  This macro should be used instead of initializing
	the UPLATFORM manually, because it will be revised to maintain upward
	compatibility whenever new fields are added to the UPLATFORM.

	If you use this macro, the uplatform will be created with a
	pPlatformObjectName, and the central class object will be
	automatically created.

Returns:
	No explicit result.

*/

/* don't add parameters to this macro; instead create a V8 */
#define	__TMS_UPLATFORM_INIT_V7(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		pAllocTracking /* OPTIONAL */,				\
		pClassParent /* OPTIONAL */,				\
		pPlatformObjectName /* OPTIONAL */,			\
		/* V7 */ pCreateAbstractPool /* OPTIONAL */		\
		)							\
	do	{							\
		__TMS_PUPLATFORM __TMS_CONST __pSelf = (pSelf);		\
									\
		__pSelf->upf_pEventctx		= (pEvctx);		\
		__pSelf->upf_pPollctx		= (pPoll);		\
		__pSelf->upf_pContext		= (pContext);		\
		__pSelf->upf_pMalloc		= (pMalloc);		\
		__pSelf->upf_pFree		= (pFree);		\
		__pSelf->upf_pInterruptSystem	= (pIntIfc);		\
		__pSelf->upf_pPostEvent		= (pPostEvent);		\
		__pSelf->upf_pGetEvent		= (pGetEvent);		\
		__pSelf->upf_pCheckEvent	= (pCheckEvent);	\
		__pSelf->upf_pYield		= (pYield);		\
		__pSelf->upf_pDebugPrintControl	= (pPrintControl);	\
		__pSelf->upf_pIoctl		= (pIoctl);		\
		__pSelf->upf_pPlatformClose	= (pPlatformClose);	\
		__pSelf->upf_pTimerSwitch	= (pTimerSwitch);	\
		__pSelf->upf_pAllocationTracking = (pAllocTracking);	\
		__pSelf->upf_hUhilAux		= __TMS_NULL;		\
		__pSelf->upf_pUhilAuxIncall	= __TMS_NULL;		\
									\
		__pSelf->upf_pDi		= (pDi);		\
		__pSelf->upf_pSetPsw		= (pSetPsw);		\
		__pSelf->upf_pCreateAbstractPool= (pCreateAbstractPool);\
									\
		/* now, do the normal setup */				\
		UsbPumpPlatform_Setup(					\
			__pSelf,					\
			/* pClassParent */ (pClassParent),		\
			/* pTaskRootConfig */ (pTaskRootConfig),	\
			/* pName */ (pPlatformObjectName),		\
			/* size */ sizeof(*__pSelf)			\
			);						\
		} while (0)


/****************************************************************************\
|
|	Functions, etc.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_IOCTL_RESULT UsbPlatformIoctl __TMS_P((
			__TMS_UPLATFORM *,
			__TMS_USBPUMP_IOCTL_CODE,
			__TMS_CONST __TMS_VOID *pInParam,
			__TMS_VOID *pOutParam
			));

__TMS_UPLATFORM_DI_FN	UsbPumpPlatform_DisableInterrupts_Default;

#define UsbPlatformIoctl_inline(pPlatform, Ioctl, pInParam, pOutParam)	\
	UsbPumpObject_Ioctl(						\
		&(pPlatform)->upf_Header,				\
		(Ioctl), (pInParam), (pOutParam)			\
		)

__TMS_VOID	UsbPumpPlatform_Setup(
			__TMS_UPLATFORM *pPlatform,
			__TMS_USBPUMP_OBJECT_HEADER *pClassParent,
			__TMS_CONST __TMS_UTASK_ROOT_CONFIG *pRootConfig,
			__TMS_CONST __TMS_TEXT *pName,
			__TMS_BYTES Size
			);


__TMS_UPLATFORM_SETPSW_FN UsbPumpPlatform_SetPsw_Default;

__TMS_CPUFLAGS	UHIL_di(__TMS_VOID);

__TMS_VOID	UHIL_setpsw(
			__TMS_ARG_CPUFLAGS
			);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPLATFORM_TAG	\
   __TMS_UPLATFORM_TAG
# define UPLATFORM_NAME(Text)	\
   __TMS_UPLATFORM_NAME(Text)
# define UPLATFORM_INIT_V1(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */					\
		)	\
	__TMS_UPLATFORM_INIT_V1(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */					\
		)
# define UPLATFORM_INIT_V2(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */				\
		)	\
	__TMS_UPLATFORM_INIT_V2(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */				\
		)
# define UPLATFORM_INIT_V3(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		/* v3 */ pDi,						\
		/* v3 */ pSetPsw,					\
		/* v3 */ pTaskRootConfig /* OPTIONAL */			\
		)	\
	__TMS_UPLATFORM_INIT_V3(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		/* v3 */ pDi,						\
		/* v3 */ pSetPsw,					\
		/* v3 */ pTaskRootConfig /* OPTIONAL */			\
		)
# define UPLATFORM_INIT_V4(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		/* v4 */ pTimerSwitch /* OPTIONAL */			\
		)	\
	__TMS_UPLATFORM_INIT_V4(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		/* v4 */ pTimerSwitch /* OPTIONAL */			\
		)
# define UPLATFORM_INIT_V5(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		/* v5 */ pAllocTracking /* OPTIONAL */			\
		)	\
	__TMS_UPLATFORM_INIT_V5(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		/* v5 */ pAllocTracking /* OPTIONAL */			\
		)
# define UPLATFORM_INIT_V6(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		pAllocTracking /* OPTIONAL */,				\
		/* v6 */ pClassParent /* OPTIONAL */,			\
		/* v6 */ pPlatformObjectName /* OPTIONAL */		\
		)	\
	__TMS_UPLATFORM_INIT_V6(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		pAllocTracking /* OPTIONAL */,				\
		/* v6 */ pClassParent /* OPTIONAL */,			\
		/* v6 */ pPlatformObjectName /* OPTIONAL */		\
		)
# define UPLATFORM_INIT_V7(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		pAllocTracking /* OPTIONAL */,				\
		pClassParent /* OPTIONAL */,				\
		pPlatformObjectName /* OPTIONAL */,			\
		/* V7 */ pCreateAbstractPool /* OPTIONAL */		\
		)	\
	__TMS_UPLATFORM_INIT_V7(					\
		pSelf,		/* structure to init. */		\
		pContext,	/* arbitrary context pointer. */	\
		pEvctx,		/* event context */			\
		pPoll,		/* "polling context" -- not used. */	\
		pIntIfc,	/* interrupt system transfer vector */	\
		pPrintControl,	/* OPTIONAL print control struct */	\
		pMalloc,						\
		pFree,							\
		pPostEvent,						\
		pGetEvent,						\
		pCheckEvent,						\
		pYield /* OPTIONAL */,					\
		pIoctl /* OPTIONAL */,					\
		pPlatformClose /* OPTIONAL */,				\
		pDi,							\
		pSetPsw,						\
		pTaskRootConfig /* OPTIONAL */,				\
		pTimerSwitch /* OPTIONAL */,				\
		pAllocTracking /* OPTIONAL */,				\
		pClassParent /* OPTIONAL */,				\
		pPlatformObjectName /* OPTIONAL */,			\
		/* V7 */ pCreateAbstractPool /* OPTIONAL */		\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of uplatform.h ****/
#endif /* _UPLATFORM_H_ */
