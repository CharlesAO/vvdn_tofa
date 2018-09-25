/* uhilevent.h	Wed Feb 20 2013 04:49:28 tmm */

/*

Module:  uhilevent.h

Function:
	Home for UEVENTCONTEXT structure and related APIs

Version:
	V3.11b	Wed Feb 20 2013 04:49:28 tmm	Edit level 6

Copyright notice:
	This file copyright (C) 2002-2007, 2009, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 2002

Revision history:
   1.79a  Wed Jun  5 2002 10:34:49  tmm
	Module created.  Change preferrend name to UCALLBACKFN and
	UCALLBACKCOMPLETION from CALLBACKFN and CALLBACKCOMPLETION,
	which are now deprecated.

   1.79b  8/28/2002  chwon
	Add __TMS_VOLATILE to uevent_active.

   1.91e  Fri Jun 17 2005 12:38:03  maw
   	Bug 1486: Added missing __TMS_END_DECLS for libarary routines

   1.97j  Mon Dec 17 2007 12:58:06  chwon
	3594: add UHIL_PostEvent_GetEventStatus() prototype

   2.01a  Mon May 11 2009 17:37:06  chwon
	7989: change uevent_active type from ULONG to UINT_PTR

   3.11b  Wed Feb 20 2013 04:49:28  tmm
	16761: make it possible to override UHIL_dispatchevent().

*/

#ifndef _UHILEVENT_H_		/* prevent multiple includes */
#define _UHILEVENT_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

#ifndef _UPLATFORM_H_
# include "uplatform.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

#if defined(__TMS_USBPUMP_UHILEVENT_LOCAL_INCLUDE__)
# include __TMS_STRINGVAL(__TMS_USBPUMP_UHILEVENT_LOCAL_INCLUDE__)
#endif


struct	TTUSB_EVENTCONTEXT
	{
	__TMS_UCALLBACKCOMPLETION	**uevent_base;
	__TMS_UCALLBACKCOMPLETION	**uevent_head;
	__TMS_UCALLBACKCOMPLETION	**uevent_tail;
	__TMS_UCALLBACKCOMPLETION	**uevent_top;
	__TMS_ULONG			uevent_overrun;
	__TMS_VOLATILE __TMS_UINT_PTR	uevent_active;	/* nonzero if
							|| processing an 
							|| event.
							*/
	__TMS_UPLATFORM			*uevent_pPlatform;
	};

/*
|| Libarary routines
*/
__TMS_BEGIN_DECLS

__TMS_UPLATFORM_POST_EVENT_FN	UHIL_PostEvent_Default;
__TMS_UPLATFORM_CHECK_EVENT_FN	UHIL_CheckEvent_Default;
__TMS_UPLATFORM_GET_EVENT_FN	UHIL_GetEvent_Default;

__TMS_VOID	UHIL_DoEvents __TMS_P((__TMS_PUEVENTCONTEXT pevq, 
				    __TMS_ULONG max_events));

#ifndef __TMS_USBPUMP_USE_LOCAL_UHIL_dispatchevent
# define __TMS_USBPUMP_USE_LOCAL_UHIL_dispatchevent 0
#endif

__TMS_BOOL	UHIL_dispatchevent __TMS_P((__TMS_PUEVENTCONTEXT, 
					 __TMS_UCALLBACKCOMPLETION *pev));

/* we provide subroutines just for consistency -- normally macros are used */
__TMS_VOID	UHIL_PostEvent __TMS_P((__TMS_UEVENTCONTEXT *,
				     __TMS_UCALLBACKCOMPLETION *));
__TMS_UCALLBACKCOMPLETION *
		UHIL_CheckEvent __TMS_P((__TMS_UEVENTCONTEXT *));

/*
|| GetEvent, because of backward compatibility, doesn't take a block/nonblock
|| argument.  If you need a nonblock version, use GetEventEx.
*/
__TMS_UCALLBACKCOMPLETION *
		UHIL_GetEvent __TMS_P((__TMS_UEVENTCONTEXT *));

__TMS_UCALLBACKCOMPLETION *
		UHIL_GetEventEx __TMS_P((__TMS_UEVENTCONTEXT *, __TMS_BOOL));

__TMS_BOOL	UHIL_PostEvent_GetEventStatus __TMS_P((
			__TMS_PUEVENTCONTEXT		/* pEventCtx */,
			__TMS_PCALLBACKCOMPLETION	/* pCallBack */
			));

__TMS_END_DECLS

/* some primitives */
#define UHIL_PostEvent_inline(pevq, pCompletion) \
	(*(pevq)->uevent_pPlatform->upf_pPostEvent)(pevq, pCompletion)

#define	UHIL_CheckEvent_inline(pevq) \
	(*(pevq)->uevent_pPlatform->upf_pCheckEvent)(pevq)

#define	UHIL_GetEvent_inline(pevq) \
	(*(pevq)->uevent_pPlatform->upf_pGetEvent)(pevq, __TMS_FALSE)

#define	UHIL_GetEventEx_inline(pevq, bBlock) \
	(*(pevq)->uevent_pPlatform->upf_pGetEvent)(pevq, bBlock)

/* Normally, we want to expand the functions in-line, so macroize them */
#define	UHIL_PostEvent(pevq, pCompletion) \
	UHIL_PostEvent_inline(pevq, pCompletion)

#define	UHIL_CheckEvent(pevq) \
	UHIL_CheckEvent_inline(pevq)

#define	UHIL_GetEvent(pevq) \
	UHIL_GetEvent_inline(pevq)

#define	UHIL_GetEventEx(pevq, bBlock) \
	UHIL_GetEventEx_inline(pevq, bBlock)

/* Finally, some deprecated names */
#define	UHIL_checkevent(pevq) \
	(*(pevq)->uevent_pPlatform->upf_pCheckEvent)(pevq)

#define	UHIL_getevent(pevq) \
	(*(pevq)->uevent_pPlatform->upf_pGetEvent)(pevq, __TMS_FALSE)


/****************************************************************************\
|
|	Set up the appropriate bus-based I/O macros and function calls
|
\****************************************************************************/

__TMS_BEGIN_DECLS
__TMS_PUEVENTCONTEXT	UPF_GetEventContext __TMS_P((__TMS_UPLATFORM *));
__TMS_END_DECLS

#if !__TMS_CLOAKED_NAMES_ONLY
# define UHIL_GetEventContext_inline(pPlatform) ((pPlatform)->upf_pEventctx)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/**** end of uhilevent.h ****/
#endif /* _UHILEVENT_H_ */
