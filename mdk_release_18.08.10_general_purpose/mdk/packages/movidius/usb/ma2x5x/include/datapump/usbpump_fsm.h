/* usbpump_fsm.h	Mon Jan 21 2013 05:36:18 tmm */

/*

Module:  usbpump_fsm.h

Function:
	Provides a framework for the generic DataPump finite-state-machine.

Version:
	V3.11b	Mon Jan 21 2013 05:36:18 tmm	Edit level 7

Copyright notice:
	This file copyright (C) 2005-2008, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	July 2005

Revision history:
   1.91d  Sat Jul  2 2005 11:10:17  tmm
	Module created.

   1.91h  Wed Aug 17 2005 19:11:11  chwon
	Fixed compile errors and added function prototype.

   1.91h  2005-08-31  tmm
	Fix problem on Win32 with the ToString functions; make parameters
	ARG_... so that a reasonable variation in width can be accomodated.

   1.93c  Sun Dec  4 2005 18:07:09  tmm
	Initial state is now part of the static info structure.
	Tables are now compressed -- action codes are limited to 5 bits,
	making room for a 6-bit state, a 6-bit event, and 4 5-bit
	action codes.  

   1.97k  Fri Feb 22 2008 09:28:02  chwon
	3831: fix type cloaking problem

   3.11b  Mon Jan 21 2013 05:36:18  tmm
	16459: use TTUSB_TRACE/NTRACE to control trace strings.

*/

#ifndef _USBPUMP_FSM_H_		/* prevent multiple includes */
#define _USBPUMP_FSM_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBPUMPDEBUG_H_
# include "usbpumpdebug.h"
#endif

/****************************************************************************\
|
|	Scalar types and forward references.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_FSM);

__TMS_TYPE_DEF_ARG	(USBPUMP_FSM_ACTION, UINT8);
__TMS_TYPE_DEF_ARG	(USBPUMP_FSM_ACTION_RESULT, UINT32);
__TMS_TYPE_DEF_ARG	(USBPUMP_FSM_EVENT, UINT8);
__TMS_TYPE_DEF_ARG	(USBPUMP_FSM_STATE, UINT8);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_FSM_STATIC_INFO);


#define	__TMS_USBPUMP_FSM_ACTION_RESULT_CONTINUE	0u
#define	__TMS_USBPUMP_FSM_ACTION_RESULT_STOP		1u

/* 
|| This state means 'no change' -- it's not a valid state number.  Actual
|| FSM states must always start with 1.  Normally the initial state is
|| 1, but designers can choose.
*/
#define	__TMS_USBPUMP_FSM_stNoChange	0

/*
|| this action means 'no-op' -- it's not a valid action number.  Actual
|| FSM action codes must always start with 1
*/
#define	__TMS_USBPUMP_FSM_acNop		0

/*
|| For consistency, we also define an event that means "no event".  This
|| can be used, for example, in the "initial event" to prevent an event
|| from being delivered to the FSM.
*/
#define	__TMS_USBPUMP_FSM_evNop		0

/****************************************************************************\
|
|	The FSM wrapper
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
USBPUMP_FSM_DISPATCH_ACTION_FN,
	__TMS_USBPUMP_FSM_ACTION_RESULT,
		(
		__TMS_USBPUMP_FSM *		/* pFsm */,
		__TMS_ARG_USBPUMP_FSM_ACTION	/* what to do */
		));

__TMS_FNTYPE_DEF(
USBPUMP_FSM_PROCESS_EVENT_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_FSM *,
		__TMS_ARG_USBPUMP_FSM_EVENT	/* the event */
		));

__TMS_TYPE_DEF_ARG	(USBPUMP_FSM_DISPATCH, UINT32);

#define	__TMS_USBPUMP_FSM_STATE_SHIFT		26u
#define	__TMS_USBPUMP_FSM_ACTION_SHIFT(i)	((i) * 5u + 6u)
#define	__TMS_USBPUMP_FSM_EVENT_SHIFT		0u

#define	__TMS_USBPUMP_FSM_STATE_MASK		0x3Fu
#define	__TMS_USBPUMP_FSM_ACTION_MASK		0x1Fu
#define __TMS_USBPUMP_FSM_EVENT_MASK		0x3Fu

/*
|| The casts to USBPUMP_FSM_DISPATCH are ugly, but are needed to
|| keep IAR C quiet, as these normally are enums of different
|| types.  Since the result has to be a USBPUMP_FSM_DISPATCH
|| anyway, I think it's innocuous.
*/
#define	__TMS_USBPUMP_FSM_DISPATCH_VALUE(e, s, a0, a1, a2, a3)	\
     ((__TMS_USBPUMP_FSM_DISPATCH)				\
	(((__TMS_USBPUMP_FSM_DISPATCH)(e) << __TMS_USBPUMP_FSM_EVENT_SHIFT) |		\
	 ((__TMS_USBPUMP_FSM_DISPATCH)(s) << __TMS_USBPUMP_FSM_STATE_SHIFT) |		\
	 ((__TMS_USBPUMP_FSM_DISPATCH)(a0) << __TMS_USBPUMP_FSM_ACTION_SHIFT(0)) |	\
	 ((__TMS_USBPUMP_FSM_DISPATCH)(a1) << __TMS_USBPUMP_FSM_ACTION_SHIFT(1)) |	\
	 ((__TMS_USBPUMP_FSM_DISPATCH)(a2) << __TMS_USBPUMP_FSM_ACTION_SHIFT(2)) |	\
	 ((__TMS_USBPUMP_FSM_DISPATCH)(a3) << __TMS_USBPUMP_FSM_ACTION_SHIFT(3))))

#define	__TMS_USBPUMP_FSM_DISPATCH_EVENT(x)			\
	(((x) >> __TMS_USBPUMP_FSM_EVENT_SHIFT) &		\
			__TMS_USBPUMP_FSM_EVENT_MASK)

#define	__TMS_USBPUMP_FSM_DISPATCH_STATE(x)			\
	(((x) >> __TMS_USBPUMP_FSM_STATE_SHIFT) &		\
			__TMS_USBPUMP_FSM_STATE_MASK)

#define	__TMS_USBPUMP_FSM_DISPATCH_ACTION(x, i)			\
	(((x) >> __TMS_USBPUMP_FSM_ACTION_SHIFT(i)) &		\
			__TMS_USBPUMP_FSM_ACTION_MASK)

struct __TMS_STRUCTNAME	(USBPUMP_FSM_STATIC_INFO)
	{
	__TMS_USBPUMP_FSM_DISPATCH_ACTION_FN	*pActionFn;
	__TMS_USBPUMP_FSM_PROCESS_EVENT_FN	*pProcessEventFn;
	__TMS_CONST __TMS_USBPUMP_FSM_DISPATCH * __TMS_CONST *pStateTable;

	/*
	|| The following fields are provided for debugging
	|| purposes.  They're in a separate structure so that the
	|| normal structure isn't bloated by debug info but has
	|| constant size between normal and debug builds.  This allows
	|| some components to be built for debug and others for normal,
	|| and interoperate
	*/
	__TMS_CONST __TMS_TEXT *		pFsmName;
	__TMS_CONST __TMS_TEXT * __TMS_CONST *	pStateStrings;
	__TMS_CONST __TMS_TEXT * __TMS_CONST *	pEventStrings;
	__TMS_CONST __TMS_TEXT * __TMS_CONST *	pActionStrings;

	/*
	|| The initial state is needed to allow for remote anonymous
	|| initialization.
	*/
	__TMS_USBPUMP_FSM_STATE			InitialState;
	__TMS_USBPUMP_FSM_EVENT			InitialEvent;

	/*
	|| These are needed for consistency checks on the input
	|| data.
	*/
	__TMS_USBPUMP_FSM_STATE			MaxState;
	__TMS_USBPUMP_FSM_EVENT			MaxEvent;
	__TMS_USBPUMP_FSM_ACTION		MaxAction;
	};


#define	__TMS_USBPUMP_FSM_STATIC_INFO_INIT_V1(				\
	a_pActionFn,							\
	a_pProcessEventFn,						\
	a_pStateTable,							\
	a_pFsmName,							\
	a_pEventStrings,						\
	a_pStateStrings,						\
	a_pActionStrings,						\
	a_InitialState,							\
	a_InitialEvent,							\
	a_MaxEvent,							\
	a_MaxState,							\
	a_MaxAction							\
	)								\
	{								\
	(a_pActionFn),							\
	(a_pProcessEventFn),						\
	(a_pStateTable),						\
	__TMS_TTUSB_TRACE(a_pFsmName) __TMS_TTUSB_NTRACE(__TMS_NULL),	\
	__TMS_TTUSB_TRACE(a_pStateStrings)				\
					__TMS_TTUSB_NTRACE(__TMS_NULL),	\
	__TMS_TTUSB_TRACE(a_pEventStrings)				\
					__TMS_TTUSB_NTRACE(__TMS_NULL),	\
	__TMS_TTUSB_TRACE(a_pActionStrings)				\
					__TMS_TTUSB_NTRACE(__TMS_NULL),	\
	(a_InitialState),						\
	(a_InitialEvent),						\
	(a_MaxState),							\
	(a_MaxEvent),							\
	(a_MaxAction)							\
	}

struct __TMS_STRUCTNAME	(USBPUMP_FSM)
	{
	/*
	|| The FSM mechanism has an interlock, so that self-reported
	|| events are processed sequentially, rather than recursively.
	|| We put it first for alignment.
	*/
	__TMS_UINT64				DeferredEventMask;

	/*
	|| An object header is required by the FSM framework for
	|| debug printing purposes.  This may be used for any other
	|| convenient purpose by the FSM code.
	*/
	__TMS_USBPUMP_OBJECT_HEADER		*pObject;

	/*
	|| All the static info about the FSM itself is separated into a 
	|| different table so that it can be exported by the implementation
	|| code.
	*/
	__TMS_CONST __TMS_USBPUMP_FSM_STATIC_INFO *pFsmInfo;

	/*
	|| We need some statistics
	*/
	__TMS_UINT32				DeferredEventCount;
	__TMS_UINT32				DeferredEventDispatchCount;

	/*
	|| The "active" flag is set to prevent re-entry.
	*/
	__TMS_UINT8				fActive;

	/*
	|| The current state of the FSM -- this is typically a small
	|| value, so we put it last for algnment.
	*/
	__TMS_USBPUMP_FSM_STATE			CurrentState;
	};

#define __TMS_USBPUMP_FSM_SETUP_V1(					\
	a_pFsm,								\
	a_pObject,							\
	a_pFsmInfo							\
	)								\
   do	{								\
	(a_pFsm)->DeferredEventMask = 0;				\
	(a_pFsm)->pObject = (a_pObject);				\
	(a_pFsm)->pFsmInfo = (a_pFsmInfo);				\
	(a_pFsm)->DeferredEventCount = 0;				\
	(a_pFsm)->DeferredEventDispatchCount = 0;			\
	(a_pFsm)->fActive = __TMS_FALSE;				\
	(a_pFsm)->CurrentState = (a_pFsmInfo)->InitialState;		\
	} while(0)


/****************************************************************************\
|
|	Functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpFsm_Init_V1(
	__TMS_USBPUMP_FSM *  /*pFsm*/,
	__TMS_USBPUMP_OBJECT_HEADER * /*pObjectHeader*/,
	__TMS_CONST __TMS_USBPUMP_FSM_STATIC_INFO * /*pFsmInfo*/
	);

__TMS_USBPUMP_FSM_PROCESS_EVENT_FN
UsbPumpFsm_ProcessEvent;

__TMS_CONST __TMS_TEXT *
UsbPumpFsm_ActionToString(
	__TMS_USBPUMP_FSM *		pFsm,
	__TMS_ARG_USBPUMP_FSM_ACTION	Action
	);

__TMS_CONST __TMS_TEXT *
UsbPumpFsm_EventToString(
	__TMS_USBPUMP_FSM *		pFsm,
	__TMS_ARG_USBPUMP_FSM_EVENT	Event
	);

__TMS_CONST __TMS_TEXT *
UsbPumpFsm_StateToString(
	__TMS_USBPUMP_FSM *		pFsm,
	__TMS_ARG_USBPUMP_FSM_STATE	State
	);

__TMS_END_DECLS

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_FSM_ACTION_RESULT_CONTINUE	\
   __TMS_USBPUMP_FSM_ACTION_RESULT_CONTINUE
# define USBPUMP_FSM_ACTION_RESULT_STOP	\
   __TMS_USBPUMP_FSM_ACTION_RESULT_STOP
# define USBPUMP_FSM_stNoChange	\
   __TMS_USBPUMP_FSM_stNoChange
# define USBPUMP_FSM_acNop	\
   __TMS_USBPUMP_FSM_acNop
# define USBPUMP_FSM_evNop	\
   __TMS_USBPUMP_FSM_evNop
# define USBPUMP_FSM_STATE_SHIFT	\
   __TMS_USBPUMP_FSM_STATE_SHIFT
# define USBPUMP_FSM_ACTION_SHIFT(i)	\
   __TMS_USBPUMP_FSM_ACTION_SHIFT(i)
# define USBPUMP_FSM_EVENT_SHIFT	\
   __TMS_USBPUMP_FSM_EVENT_SHIFT
# define USBPUMP_FSM_STATE_MASK	\
   __TMS_USBPUMP_FSM_STATE_MASK
# define USBPUMP_FSM_ACTION_MASK	\
   __TMS_USBPUMP_FSM_ACTION_MASK
# define USBPUMP_FSM_EVENT_MASK	\
   __TMS_USBPUMP_FSM_EVENT_MASK
# define USBPUMP_FSM_DISPATCH_VALUE(e, s, a0, a1, a2, a3)	\
   __TMS_USBPUMP_FSM_DISPATCH_VALUE(e, s, a0, a1, a2, a3)
# define USBPUMP_FSM_DISPATCH_EVENT(x)	\
   __TMS_USBPUMP_FSM_DISPATCH_EVENT(x)
# define USBPUMP_FSM_DISPATCH_STATE(x)	\
   __TMS_USBPUMP_FSM_DISPATCH_STATE(x)
# define USBPUMP_FSM_DISPATCH_ACTION(x, i)	\
   __TMS_USBPUMP_FSM_DISPATCH_ACTION(x, i)
# define USBPUMP_FSM_STATIC_INFO_INIT_V1(				\
	a_pActionFn,							\
	a_pProcessEventFn,						\
	a_pStateTable,							\
	a_pFsmName,							\
	a_pEventStrings,						\
	a_pStateStrings,						\
	a_pActionStrings,						\
	a_InitialState,							\
	a_InitialEvent,							\
	a_MaxEvent,							\
	a_MaxState,							\
	a_MaxAction							\
	)	\
	__TMS_USBPUMP_FSM_STATIC_INFO_INIT_V1(				\
	a_pActionFn,							\
	a_pProcessEventFn,						\
	a_pStateTable,							\
	a_pFsmName,							\
	a_pEventStrings,						\
	a_pStateStrings,						\
	a_pActionStrings,						\
	a_InitialState,							\
	a_InitialEvent,							\
	a_MaxEvent,							\
	a_MaxState,							\
	a_MaxAction							\
	)
# define USBPUMP_FSM_SETUP_V1(					\
	a_pFsm,								\
	a_pObject,							\
	a_pFsmInfo							\
	)	\
	__TMS_USBPUMP_FSM_SETUP_V1(					\
	a_pFsm,								\
	a_pObject,							\
	a_pFsmInfo							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_fsm.h ****/
#endif /* _USBPUMP_FSM_H_ */
