/* uplatformmsg.h	Wed Apr 02 2007 09:17:30 yssaroha */

/*

Module:  uplatformmsg.h

Function:
	Definition of Platform Message Code and Structures

Version:
	V1.97k	Wed Apr 02 2007 09:17:30 yssaroha	Edit level 1

Copyright notice:
	This file copyright (C) 2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Yogender Saroha, MCCI Corporation	April 2008

Revision history:
   1.97k  Wed Apr 02 2007 09:17:30 yssaroha
	Module created.

*/

#ifndef _UPLATFORMMSG_H_		/* prevent multiple includes */
#define _UPLATFORMMSG_H_

#ifndef _UTASK_TYPES_H_	
# include "utask_types.h"
#endif

#ifndef _UTASKMSG_DP_H_
# include "utaskmsg_dp.h"
#endif

#ifndef _UTASKMSG_CODES_H_
# include "utaskmsg_codes.h"
#endif

#ifndef _UTASKMSG_SERVICES_H_
# include "utaskmsg_services.h"
#endif

/****************************************************************************\
|
|	Service Index
|
\****************************************************************************/

#define	__TMS_UTASK_MC_SERVICE_PLATFORM			12


/****************************************************************************\
|
|	Request / Reply / Notification Index
|
\****************************************************************************/

/* Request and Reply message index */
#define	__TMS_UTASK_MSGIDX_PLATFORM_EXECUTE_CMD		0

/* Notification message index */
#define	__TMS_UTASK_MSGIDX_PLATFORM_NOTIFY_MSG		0


/*******************************************************************
*
* Platform Message Code generation Macros
*
******************************************************************/
/*nBuf: # of buffers attached in the message */
#define __TMS_UTASK_MSGCODE_PLATFORM_REQUEST_SYNC(Idx, nBuf)		\
	__TMS_UTASK_MC_REQ(						\
		__TMS_UTASK_MC_SERVICE_PLATFORM,			\
		__TMS_CONCAT(__TMS_UTASK_MSGIDX_PLATFORM_, Idx),	\
		sizeof(__TMS_CONCAT3(__TMS_UTASK_MSG_PLATFORM_, Idx, _REQUEST)), \
		(nBuf)							\
		)


#define __TMS_UTASK_MSGCODE_PLATFORM_REQUEST_ASYNC(Idx, nBuf)		\
	__TMS_UTASK_MC_REQ_ASYNC(					\
		__TMS_UTASK_MC_SERVICE_PLATFORM,			\
		__TMS_CONCAT(__TMS_UTASK_MSGIDX_PLATFORM_, Idx),	\
		sizeof(__TMS_CONCAT3(__TMS_UTASK_MSG_PLATFORM_, Idx, _REQUEST)), \
		(nBuf)							\
		)

#define __TMS_UTASK_MSGCODE_PLATFORM_REPLY_SYNC(Idx, nBuf)		\
	__TMS_UTASK_MC_REP(						\
		__TMS_UTASK_MC_SERVICE_PLATFORM,			\
		__TMS_CONCAT(__TMS_UTASK_MSGIDX_PLATFORM_, Idx),	\
		sizeof(__TMS_CONCAT3(__TMS_UTASK_MSG_PLATFORM_, Idx, _REPLY)), \
		(nBuf)							\
		)

#define __TMS_UTASK_MSGCODE_PLATFORM_REPLY_ASYNC(Idx, nBuf)		\
	__TMS_UTASK_MC_REP_ASYNC(					\
		__TMS_UTASK_MC_SERVICE_PLATFORM,			\
		__TMS_CONCAT(__TMS_UTASK_MSGIDX_PLATFORM_, Idx),	\
		sizeof(__TMS_CONCAT3(__TMS_UTASK_MSG_PLATFORM_, Idx, _REPLY)), \
		(nBuf)							\
		)

#define __TMS_UTASK_MSGCODE_PLATFORM_INDICATION(Idx)			\
	__TMS_UTASK_MC_IND(						\
		__TMS_UTASK_MC_SERVICE_PLATFORM,			\
		__TMS_CONCAT(__TMS_UTASK_MSGIDX_PLATFORM_, Idx),	\
		sizeof(__TMS_CONCAT3(__TMS_UTASK_MSG_PLATFORM_, Idx, _INDICATION)) \
		)


/****************************************************************************\
|
|	Platform message structures
|
\****************************************************************************/

#define __TMS_UTASK_MSG_PLATFORM_HEADER		\
	__TMS_UTASK_DPMSG_HDR

/* Request and Reply for Execute Command */
__TMS_TYPE_DEF_STRUCT	(UTASK_MSG_PLATFORM_EXECUTE_CMD_REQUEST);
struct __TMS_STRUCTNAME	(UTASK_MSG_PLATFORM_EXECUTE_CMD_REQUEST)
	{
	__TMS_UTASK_MSG_PLATFORM_HEADER;
	__TMS_UINT32	CommandId;
	__TMS_UINT8 *	pParam;
	__TMS_BYTES	SizeParam;
	};

/* Request and Reply for Execute Command */
__TMS_TYPE_DEF_STRUCT	(UTASK_MSG_PLATFORM_EXECUTE_CMD_REPLY);
struct __TMS_STRUCTNAME	(UTASK_MSG_PLATFORM_EXECUTE_CMD_REPLY)
	{
	__TMS_UINT32	CommandId;
	__TMS_UINT32	Status;
	};


/* Notify Message Indication  */
__TMS_TYPE_DEF_STRUCT	(UTASK_MSG_PLATFORM_NOTIFY_MSG_INDICATION);
struct __TMS_STRUCTNAME	(UTASK_MSG_PLATFORM_NOTIFY_MSG_INDICATION)
	{
	__TMS_UINT32	MessageId;
	__TMS_TEXT	MsgData[128];
	};


/****************************************************************************\
|
|	Platform message code interface
|
\****************************************************************************/
/* Request/Reply message code */

#define __TMS_UTASK_MSGCODE_PLATFORM_EXECUTE_CMD_REQUEST		\
	__TMS_UTASK_MSGCODE_PLATFORM_REQUEST_ASYNC(EXECUTE_CMD, 1)

#define __TMS_UTASK_MSGCODE_PLATFORM_EXECUTE_CMD_REPLY		\
	__TMS_UTASK_MSGCODE_PLATFORM_REPLY_ASYNC(EXECUTE_CMD, 1)

#define __TMS_UTASK_MSGCODE_PLATFORM_NOTIFY_MSG_INDICATION		\
	__TMS_UTASK_MSGCODE_PLATFORM_INDICATION(NOTIFY_MSG)


/****************************************************************************\
|
|	Platform Command Request Status Macro
|
\****************************************************************************/

#define __TMS_UTASK_MSG_PLATFORM_CMD_SUCCESS		0
#define __TMS_UTASK_MSG_PLATFORM_CMD_FAIL		1
#define __TMS_UTASK_MSG_PLATFORM_CMD_NOT_SUPPORTED	2


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS


__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UTASK_MC_SERVICE_PLATFORM	\
   __TMS_UTASK_MC_SERVICE_PLATFORM
# define UTASK_MSGIDX_PLATFORM_EXECUTE_CMD	\
   __TMS_UTASK_MSGIDX_PLATFORM_EXECUTE_CMD
# define UTASK_MSGIDX_PLATFORM_NOTIFY_MSG	\
   __TMS_UTASK_MSGIDX_PLATFORM_NOTIFY_MSG
# define UTASK_MSGCODE_PLATFORM_REQUEST_SYNC(Idx, nBuf)	\
   __TMS_UTASK_MSGCODE_PLATFORM_REQUEST_SYNC(Idx, nBuf)
# define UTASK_MSGCODE_PLATFORM_REQUEST_ASYNC(Idx, nBuf)	\
   __TMS_UTASK_MSGCODE_PLATFORM_REQUEST_ASYNC(Idx, nBuf)
# define UTASK_MSGCODE_PLATFORM_REPLY_SYNC(Idx, nBuf)	\
   __TMS_UTASK_MSGCODE_PLATFORM_REPLY_SYNC(Idx, nBuf)
# define UTASK_MSGCODE_PLATFORM_REPLY_ASYNC(Idx, nBuf)	\
   __TMS_UTASK_MSGCODE_PLATFORM_REPLY_ASYNC(Idx, nBuf)
# define UTASK_MSGCODE_PLATFORM_INDICATION(Idx)	\
   __TMS_UTASK_MSGCODE_PLATFORM_INDICATION(Idx)
# define UTASK_MSG_PLATFORM_HEADER	\
   __TMS_UTASK_MSG_PLATFORM_HEADER
# define UTASK_MSGCODE_PLATFORM_EXECUTE_CMD_REQUEST	\
   __TMS_UTASK_MSGCODE_PLATFORM_EXECUTE_CMD_REQUEST
# define UTASK_MSGCODE_PLATFORM_EXECUTE_CMD_REPLY	\
   __TMS_UTASK_MSGCODE_PLATFORM_EXECUTE_CMD_REPLY
# define UTASK_MSGCODE_PLATFORM_NOTIFY_MSG_INDICATION	\
   __TMS_UTASK_MSGCODE_PLATFORM_NOTIFY_MSG_INDICATION
# define UTASK_MSG_PLATFORM_CMD_SUCCESS	\
   __TMS_UTASK_MSG_PLATFORM_CMD_SUCCESS
# define UTASK_MSG_PLATFORM_CMD_FAIL	\
   __TMS_UTASK_MSG_PLATFORM_CMD_FAIL
# define UTASK_MSG_PLATFORM_CMD_NOT_SUPPORTED	\
   __TMS_UTASK_MSG_PLATFORM_CMD_NOT_SUPPORTED
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of  uplatformmsg.h****/
#endif /* _UPLATFORMMSG_H_ */
