/* usbpump_debugio_defines.h	Mon Jan 14 2013 07:31:24 tmm */

/*

Module:  usbpump_debugio_defines.h

Function:
	Define constants and macros used in Debug-IO API

Version:
	V3.11b	Mon Jan 14 2013 07:31:24 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2009, 2013 by	

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Mats Webjorn, MCCI Corporation	April 2009

Revision history:
   2.01a  Tue Apr 21 2009 09:24:09 maw
	7563: Module created from usbpump_abstract_sio_defines.h EL 2

   3.11b  Mon Jan 14 2013 07:31:24  tmm
	16472: UDMASK_... should be unsigned.

*/

#ifndef _USBPUMP_DEBUGIO_DEFINES_H_	/* prevent multiple includes */
#define _USBPUMP_DEBUGIO_DEFINES_H_

#ifndef _USBPUMPLIST_H_
#include "usbpumplist.h"
#endif

#ifndef _USBPUMPOBJECTNAME_H_
#include "usbpumpobjectname.h"
#endif

/****************************************************************************\
|
|	Class matching string
|
\****************************************************************************/
#define	__TMS_USBPUMP_OBJECT_NAME_DEBUGIO	"debug-io"

#define	__TMS_USBPUMP_OBJECT_NAME_ENUM_DEBUGIO		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_DEBUGIO)

/* the object tag */
#define	__TMS_USBPUMP_DEBUGIO_TAG(ARG_a, ARG_b)		\
	__TMS_UHIL_MEMTAG('A', 'S', (ARG_a), (ARG_b))

/****************************************************************************\
|
|	Debug constants
|
\****************************************************************************/

#define	__TMS_UDMASK_DBGIO_ENTRY	(1u << (__TMS_UDMASK_DEVBASE_N))
#define	__TMS_UDMASK_DBGIO_TOHOSTFLOW	(1u << (__TMS_UDMASK_DEVBASE_N + 1))
#define	__TMS_UDMASK_DBGIO_FROMHOSTFLOW	(1u << (__TMS_UDMASK_DEVBASE_N + 2))

#define	__TMS_UDMASK_DBGIO_FLOW		(__TMS_UDMASK_DBGIO_TOHOSTFLOW | \
					 __TMS_UDMASK_DBGIO_FROMHOSTFLOW)

/****************************************************************************\
|
|	Session linked list management
|
\****************************************************************************/

/*
|| Free Session list management macros
*/
#define __TMS_USBPUMP_DEBUGIO_GET_FREE_SESSION(			\
		ARG_pSelf, ARG_pSession)			\
	__TMS_USBPUMPLIB_LIST_REMOVE_NODE(			\
		__TMS_USBPUMP_DEBUGIO_SESSION,			\
		&(ARG_pSelf)->DebugIo.pFreeSessionHead,		\
		ARG_pSession,					\
		Session.pNext,					\
		Session.pLast					\
		)

#define __TMS_USBPUMP_DEBUGIO_GET_FIRST_FREE_SESSION(		\
		ARG_pSelf, ARG_pSession)			\
	ARG_pSession = (ARG_pSelf)->DebugIo.pFreeSessionHead;	\
	if (ARG_pSession)					\
		{						\
		__TMS_USBPUMP_DEBUGIO_GET_FREE_SESSION(		\
			ARG_pSelf,				\
			ARG_pSession				\
			);					\
		}

#define __TMS_USBPUMP_DEBUGIO_PUT_FREE_SESSION(			\
		ARG_pSelf, ARG_pSession)			\
	__TMS_USBPUMPLIB_LIST_INSERT_NODE_TAIL(			\
		__TMS_USBPUMP_DEBUGIO_SESSION,			\
		&(ARG_pSelf)->DebugIo.pFreeSessionHead,		\
		ARG_pSession,					\
		Session.pNext,					\
		Session.pLast					\
		)

/*
|| Open Session list management macros
*/
#define __TMS_USBPUMP_DEBUGIO_GET_OPEN_SESSION(			\
		ARG_pSelf, ARG_pSession)			\
	__TMS_USBPUMPLIB_LIST_REMOVE_NODE(			\
		__TMS_USBPUMP_DEBUGIO_SESSION,			\
		&(ARG_pSelf)->DebugIo.pSessionHead,		\
		ARG_pSession,					\
		Session.pNext,					\
		Session.pLast					\
		)

#define __TMS_USBPUMP_DEBUGIO_GET_FIRST_OPEN_SESSION(		\
		ARG_pSelf, ARG_pSession)			\
	ARG_pSession = (ARG_pSelf)->DebugIo.pSessionHead;	\
	if (ARG_pSession)					\
		{						\
		__TMS_USBPUMP_DEBUGIO_GET_OPEN_SESSION(		\
			ARG_pSelf,				\
			ARG_pSession				\
			);					\
		}

#define __TMS_USBPUMP_DEBUGIO_PUT_OPEN_SESSION(			\
		ARG_pSelf, ARG_pSession)			\
	__TMS_USBPUMPLIB_LIST_INSERT_NODE_TAIL(			\
		__TMS_USBPUMP_DEBUGIO_SESSION,			\
		&(ARG_pSelf)->DebugIo.pSessionHead,		\
		ARG_pSession,					\
		Session.pNext,					\
		Session.pLast					\
		)

/*
|| Pending SendQe list management macros
*/
#define __TMS_USBPUMP_DEBUGIO_GET_PEND_SENDQE(			\
		ARG_pSession, ARG_pSendQe)			\
	__TMS_USBPUMPLIB_LIST_REMOVE_NODE(			\
		__TMS_USBPUMP_DEBUGIO_SEND_QE,			\
		&(ARG_pSession)->Session.pPendSendQeHead,	\
		ARG_pSendQe,					\
		pNext,						\
		pLast						\
		)

#define __TMS_USBPUMP_DEBUGIO_GET_FIRST_PEND_SENDQE(		\
		ARG_pSession, ARG_pSendQe)			\
	ARG_pSendQe = (ARG_pSession)->Session.pPendSendQeHead;	\
	if (ARG_pSendQe)					\
		{						\
		__TMS_USBPUMP_DEBUGIO_GET_PEND_SENDQE(		\
			ARG_pSession,				\
			ARG_pSendQe				\
			);					\
		}

#define __TMS_USBPUMP_DEBUGIO_PUT_PEND_SENDQE(			\
		ARG_pSession, ARG_pSendQe)			\
	__TMS_USBPUMPLIB_LIST_INSERT_NODE_TAIL(			\
		__TMS_USBPUMP_DEBUGIO_SEND_QE,			\
		&(ARG_pSession)->Session.pPendSendQeHead,	\
		ARG_pSendQe,					\
		pNext,						\
		pLast						\
		)

/*
|| Pending RecvQe list management macros
*/
#define __TMS_USBPUMP_DEBUGIO_GET_PEND_RECVQE(			\
		ARG_pSession, ARG_pRecvQe)			\
	__TMS_USBPUMPLIB_LIST_REMOVE_NODE(			\
		__TMS_USBPUMP_DEBUGIO_RECV_QE,			\
		&(ARG_pSession)->Session.pPendRecvQeHead,	\
		ARG_pRecvQe,					\
		pNext,						\
		pLast						\
		)

#define __TMS_USBPUMP_DEBUGIO_GET_FIRST_PEND_RECVQE(		\
		ARG_pSession, ARG_pRecvQe)			\
	ARG_pRecvQe = (ARG_pSession)->Session.pPendRecvQeHead;	\
	if (ARG_pRecvQe)					\
		{						\
		__TMS_USBPUMP_DEBUGIO_GET_PEND_RECVQE(		\
			ARG_pSession,				\
			ARG_pRecvQe				\
			);					\
		}

#define __TMS_USBPUMP_DEBUGIO_PUT_PEND_RECVQE(			\
		ARG_pSession, ARG_pRecvQe)			\
	__TMS_USBPUMPLIB_LIST_INSERT_NODE_TAIL(			\
		__TMS_USBPUMP_DEBUGIO_RECV_QE,			\
		&(ARG_pSession)->Session.pPendRecvQeHead,	\
		ARG_pRecvQe,					\
		pNext,						\
		pLast						\
		)

/****************************************************************************\
|
|	Other defs
|
\****************************************************************************/
/*
|| Define max number of sessions that may be opened to 
|| a Debug-IO instance
||
|| There is currently no mechanism where data from different sessions
|| can be separated while being send/received over the same link
*/
#define	__TMS_USBPUMP_DEBUGIO_MAX_SESSIONS			(1u)

/*
|| Defines for SIO status
*/
#define	__TMS_USBPUMP_DEBUGIO_STATUS_SUCCESS			(0u)
#define	__TMS_USBPUMP_DEBUGIO_STATUS_FAILED			(1u)
#define	__TMS_USBPUMP_DEBUGIO_STATUS_ALREADY_OPEN		(2u)
#define	__TMS_USBPUMP_DEBUGIO_STATUS_INVALID_PARAMETER		(3u)
#define	__TMS_USBPUMP_DEBUGIO_STATUS_INVALID_SESSION_HANDLE	(4u)
#define	__TMS_USBPUMP_DEBUGIO_STATUS_TRANSPORT_INACTIVE		(5u)
#define	__TMS_USBPUMP_DEBUGIO_STATUS_BUFFER_TOO_SMALL		(6u)

/*
|| Once again, as names are added to the above list, please add them
|| to the following macro.
*/
#define	__TMS_USBPUMP_DEBUGIO_STATUS_NAMES__INIT \
	{ \
	"USBPUMP_DEBUGIO_STATUS_SUCCESS", \
	"USBPUMP_DEBUGIO_STATUS_FAILED", \
	"USBPUMP_DEBUGIO_STATUS_ALREADY_OPEN", \
	"USBPUMP_DEBUGIO_STATUS_INVALID_PARAMETER", \
	"USBPUMP_DEBUGIO_STATUS_INVALID_SESSION_HANDLE", \
	"USBPUMP_DEBUGIO_STATUS_TRANSPORT_INACTIVE", \
	"USBPUMP_DEBUGIO_STATUS_BUFFER_TOO_SMALL" \
	}

/*
|| Helper macros
*/
#define __TMS_USBPUMP_DEBUGIO_SUCCESS(ARG_status) \
	((ARG_status) == __TMS_USBPUMP_DEBUGIO_STATUS_SUCCESS)

#define	__TMS_USBPUMP_DEBUGIO_FAILED(ARG_status) \
	((ARG_status) > __TMS_USBPUMP_DEBUGIO_STATUS_SUCCESS)

/*
|| Defines for Debug-IO events
*/
#define	__TMS_USBPUMP_DEBUGIO_EVENT_TRANSPORT_ACTIVE		(0u)
#define	__TMS_USBPUMP_DEBUGIO_EVENT_TRANSPORT_INACTIVE		(1u)
#define	__TMS_USBPUMP_DEBUGIO_EVENT_USB_SUSPEND_WITHOUT_WAKEUP	(2u)
#define	__TMS_USBPUMP_DEBUGIO_EVENT_USB_SUSPEND_WITH_WAKEUP	(3u)
#define	__TMS_USBPUMP_DEBUGIO_EVENT_USB_RESUME			(4u)


/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_OBJECT_NAME_DEBUGIO	\
   __TMS_USBPUMP_OBJECT_NAME_DEBUGIO
# define USBPUMP_OBJECT_NAME_ENUM_DEBUGIO	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_DEBUGIO
#define	USBPUMP_DEBUGIO_TAG(ARG_a, ARG_b)	\
  __TMS_USBPUMP_DEBUGIO_TAG(ARG_a, ARG_b)
# define UDMASK_DBGIO_ENTRY	__TMS_UDMASK_DBGIO_ENTRY
# define UDMASK_DBGIO_TOHOSTFLOW	__TMS_UDMASK_DBGIO_TOHOSTFLOW
# define UDMASK_DBGIO_FROMHOSTFLOW	__TMS_UDMASK_DBGIO_FROMHOSTFLOW
# define UDMASK_DBGIO_FLOW	__TMS_UDMASK_DBGIO_FLOW
# define USBPUMP_DEBUGIO_GET_FREE_SESSION(			\
		ARG_pSelf, ARG_pSession)	\
	__TMS_USBPUMP_DEBUGIO_GET_FREE_SESSION(			\
		ARG_pSelf, ARG_pSession)
# define USBPUMP_DEBUGIO_GET_FIRST_FREE_SESSION(		\
		ARG_pSelf, ARG_pSession)	\
	__TMS_USBPUMP_DEBUGIO_GET_FIRST_FREE_SESSION(		\
		ARG_pSelf, ARG_pSession)
# define USBPUMP_DEBUGIO_PUT_FREE_SESSION(			\
		ARG_pSelf, ARG_pSession)	\
	__TMS_USBPUMP_DEBUGIO_PUT_FREE_SESSION(			\
		ARG_pSelf, ARG_pSession)
# define USBPUMP_DEBUGIO_GET_OPEN_SESSION(			\
		ARG_pSelf, ARG_pSession)	\
	__TMS_USBPUMP_DEBUGIO_GET_OPEN_SESSION(			\
		ARG_pSelf, ARG_pSession)
# define USBPUMP_DEBUGIO_GET_FIRST_OPEN_SESSION(		\
		ARG_pSelf, ARG_pSession)	\
	__TMS_USBPUMP_DEBUGIO_GET_FIRST_OPEN_SESSION(		\
		ARG_pSelf, ARG_pSession)
# define USBPUMP_DEBUGIO_PUT_OPEN_SESSION(			\
		ARG_pSelf, ARG_pSession)	\
	__TMS_USBPUMP_DEBUGIO_PUT_OPEN_SESSION(			\
		ARG_pSelf, ARG_pSession)
# define USBPUMP_DEBUGIO_GET_PEND_SENDQE(			\
		ARG_pSession, ARG_pSendQe)	\
	__TMS_USBPUMP_DEBUGIO_GET_PEND_SENDQE(			\
		ARG_pSession, ARG_pSendQe)
# define USBPUMP_DEBUGIO_GET_FIRST_PEND_SENDQE(		\
		ARG_pSession, ARG_pSendQe)	\
	__TMS_USBPUMP_DEBUGIO_GET_FIRST_PEND_SENDQE(		\
		ARG_pSession, ARG_pSendQe)
# define USBPUMP_DEBUGIO_PUT_PEND_SENDQE(			\
		ARG_pSession, ARG_pSendQe)	\
	__TMS_USBPUMP_DEBUGIO_PUT_PEND_SENDQE(			\
		ARG_pSession, ARG_pSendQe)
# define USBPUMP_DEBUGIO_GET_PEND_RECVQE(			\
		ARG_pSession, ARG_pRecvQe)	\
	__TMS_USBPUMP_DEBUGIO_GET_PEND_RECVQE(			\
		ARG_pSession, ARG_pRecvQe)
# define USBPUMP_DEBUGIO_GET_FIRST_PEND_RECVQE(		\
		ARG_pSession, ARG_pRecvQe)	\
	__TMS_USBPUMP_DEBUGIO_GET_FIRST_PEND_RECVQE(		\
		ARG_pSession, ARG_pRecvQe)
# define USBPUMP_DEBUGIO_PUT_PEND_RECVQE(			\
		ARG_pSession, ARG_pRecvQe)	\
	__TMS_USBPUMP_DEBUGIO_PUT_PEND_RECVQE(			\
		ARG_pSession, ARG_pRecvQe)
# define USBPUMP_DEBUGIO_MAX_SESSIONS	\
   __TMS_USBPUMP_DEBUGIO_MAX_SESSIONS
# define USBPUMP_DEBUGIO_STATUS_SUCCESS	\
   __TMS_USBPUMP_DEBUGIO_STATUS_SUCCESS
# define USBPUMP_DEBUGIO_STATUS_FAILED	\
   __TMS_USBPUMP_DEBUGIO_STATUS_FAILED
# define USBPUMP_DEBUGIO_STATUS_ALREADY_OPEN	\
   __TMS_USBPUMP_DEBUGIO_STATUS_ALREADY_OPEN
# define USBPUMP_DEBUGIO_STATUS_INVALID_PARAMETER	\
   __TMS_USBPUMP_DEBUGIO_STATUS_INVALID_PARAMETER
# define USBPUMP_DEBUGIO_STATUS_INVALID_SESSION_HANDLE	\
   __TMS_USBPUMP_DEBUGIO_STATUS_INVALID_SESSION_HANDLE
# define USBPUMP_DEBUGIO_STATUS_TRANSPORT_INACTIVE	\
   __TMS_USBPUMP_DEBUGIO_STATUS_TRANSPORT_INACTIVE
# define USBPUMP_DEBUGIO_STATUS_BUFFER_TOO_SMALL	\
   __TMS_USBPUMP_DEBUGIO_STATUS_BUFFER_TOO_SMALL
# define USBPUMP_DEBUGIO_STATUS_NAMES__INIT	\
   __TMS_USBPUMP_DEBUGIO_STATUS_NAMES__INIT
# define USBPUMP_DEBUGIO_SUCCESS(ARG_status)	\
   __TMS_USBPUMP_DEBUGIO_SUCCESS(ARG_status)
# define USBPUMP_DEBUGIO_FAILED(ARG_status)	\
   __TMS_USBPUMP_DEBUGIO_FAILED(ARG_status)
# define USBPUMP_DEBUGIO_EVENT_TRANSPORT_ACTIVE	\
   __TMS_USBPUMP_DEBUGIO_EVENT_TRANSPORT_ACTIVE
# define USBPUMP_DEBUGIO_EVENT_TRANSPORT_INACTIVE	\
   __TMS_USBPUMP_DEBUGIO_EVENT_TRANSPORT_INACTIVE
# define USBPUMP_DEBUGIO_EVENT_USB_SUSPEND_WITHOUT_WAKEUP	\
   __TMS_USBPUMP_DEBUGIO_EVENT_USB_SUSPEND_WITHOUT_WAKEUP
# define USBPUMP_DEBUGIO_EVENT_USB_SUSPEND_WITH_WAKEUP	\
   __TMS_USBPUMP_DEBUGIO_EVENT_USB_SUSPEND_WITH_WAKEUP
# define USBPUMP_DEBUGIO_EVENT_USB_RESUME	\
   __TMS_USBPUMP_DEBUGIO_EVENT_USB_RESUME
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_debugio_defines.h ****/
#endif /* _USBPUMP_DEBUGIO_DEFINES_H_ */
