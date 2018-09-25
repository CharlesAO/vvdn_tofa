/* utask_types.h	Sat Oct 01 2005 11:06:31maw */

/*

Module:  utask_types.h

Function:
	Define the basic type tags for MCCI Task API -- useful when 
	interfacing to other code subsystems and the full "utask.h" cannot 
	be included.

Version:
	V1.91i	Sat Oct 01 2005 11:06:31 maw	Edit level 12

Copyright notice:
	This file copyright (C) 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Mats Webjorn, MCCI Corporation	February 2005

Revision history:
   1.91b  Mon Feb 14 2005 14:44:15  maw
	Module created.

   1.91c  Tue May 10 2005 08:00:05 maw
   	Changed all buffer pointers from VOID* to UINT8*, changed return 
	parameter of MsgLoop to BOOL. Added start-task function protoype. 
	Updated buffer index type to UTASK_MSG_BUFINDEX for all fn's. Fn's 
	that manipulates msg-handle takes it as reference-parameter
   	Removed return value from UTASK_MSG_GETBUFFER_FN

   1.91d  Wed Jun 08 2005 19:01:09 maw
   	Replaced UTASK_MSGLOOP_FN with UTASK_INIT_FN and UTASK_MSGDISPATH_FN
   	Replaced UTASK_MSG_SENDREQUESTWAITREPLY_FN with UTASK_MSG_WAITREPLY_FN
   	Added UTASK_MAILBOX_HANDLE

   1.91e  Wed Jun 29 2005 08:48:46 maw
   	Add UTASK_IOCTL_FN

   1.91g  Thu Jul 14 2005 00:27:44  tmm
	clean up commentary slightly.

   1.91h  Fri Sep 09 2005 23:27:43  chwon
	Renamed UTASK_UTF8 into USBPUMP_UTF8_TEXT

   1.91i  Mon Sep 19 2005 19:26:20 maw
   	Moved UTASK_HDR def from utask.h

   1.91i  Sat Oct 01 2005 11:06:31  tmm
	Eliminate use of ENUMs and define a minimum and maximum for
	UTASK_MSGCODE_TYPE.
   	
*/

#ifndef _UTASK_TYPES_H_		/* prevent multiple includes */
#define _UTASK_TYPES_H_

#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif

#ifndef _BASEHILTYPES_H_
# include "basehiltypes.h"
#endif

#ifndef _UPLATFORM_H_
# include "uplatform.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	Useful macros
|
\****************************************************************************/


/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(UTASK_PROCESS_CONFIG);
__TMS_TYPE_DEF_STRUCT(UTASK_CONFIG_HDR);

__TMS_TYPE_DEF_STRUCT(UTASK_PROCESS_HDR);
__TMS_TYPE_DEF_STRUCT(UTASK_HDR);

__TMS_TYPE_DEF_STRUCT(UTASK_MSG_HDR);

__TMS_TYPE_DEF_HANDLE(UTASK_HANDLE);
__TMS_TYPE_DEF_HANDLE(UTASK_PROCESS_HANDLE);
__TMS_TYPE_DEF_HANDLE(UTASK_MAILBOX_HANDLE);
__TMS_TYPE_DEF_HANDLE(UTASK_MSG_HANDLE);
__TMS_TYPE_DEF_HANDLE(UTASK_BUFFER_HANDLE);
__TMS_TYPE_DEF_HANDLE(UTASK_TIMEOUT_HANDLE);
__TMS_TYPE_DEF_HANDLE(UTASK_COMPLETION_HANDLE);

__TMS_TYPE_DEF_ARG(UTASK_PROCESS_INDEX, UINT16);
__TMS_TYPE_DEF_ARG(UTASK_MSG_ACCESSFLAGS, BITS);
__TMS_TYPE_DEF_ARG(UTASK_MSG_BUFINDEX, UINT16);

__TMS_TYPE_DEF_ARG(UTASK_MSGCODE, UINT32);
/* renamed USBPUMP_UTF8_TEXT: __TMS_TYPE_DEF_ARG(UTASK_UTF8, TEXT); */

__TMS_TYPE_DEF_STRUCT(UTASK_SWITCH);
__TMS_TYPE_DEF_STRUCT(UTASK_MSG_SWITCH);
__TMS_TYPE_DEF_STRUCT(UTASK_BUF_SWITCH);

__TMS_TYPE_DEF_STRUCT(UTASK_ROOT_IMPLEMENTATION);

/****************************************************************************\
|
|	Enums
|
\****************************************************************************/

/*
|| Message types
*/

__TMS_TYPE_DEF_ARG(UTASK_MSGCODE_TYPE, UINT32);

#define	__TMS_UTASK_MSGCODE_TYPE__MIN	0
#define	__TMS_UTASK_MSGCODE_TYPE_REQ	0
#define	__TMS_UTASK_MSGCODE_TYPE_REP	1
#define	__TMS_UTASK_MSGCODE_TYPE_IND	2
#define	__TMS_UTASK_MSGCODE_TYPE_EVT	3	/* Event: Used to send OS specific events */
#define	__TMS_UTASK_MSGCODE_TYPE__MAX	3

/*
|| Yet another set of UTASK status codes.  
|| XXX tmm: This should be changed to USTATs or just use the UPOSIX errors.
|| XXX tmm: These should be documented as to meaning and usage.  
|| XXX tmm: we ought to have a debugging table for looking up the name at
|| runtime.
*/
__TMS_TYPE_DEF_ARG(UTASK_STAT, UINT32);
#define	__TMS_UTASK_STAT_OK			0
#define	__TMS_UTASK_STAT_ENOTFOUND		1
#define	__TMS_UTASK_STAT_ETIMEOUT		2
#define	__TMS_UTASK_STAT_EQUIT			3
#define	__TMS_UTASK_STAT_EINVALID_PARAMETER	4
#define	__TMS_UTASK_STAT_ENOMEMORY		5
#define	__TMS_UTASK_STAT_EUNKNOWN		6

/****************************************************************************\
|
|	General UTASK abstract function types
|
\****************************************************************************/

/* Function type for optional client UTASK initialization */
__TMS_FNTYPE_DEF(
	UTASK_ROOT_CONFIG_CLIENT_INIT_FN,
	__TMS_BOOL, (
		__TMS_UTASK_ROOT *			/* IN OUT: pTaskRoot */,
		__TMS_CONST __TMS_UTASK_ROOT_CONFIG *	/* IN: pRootConfig */,
		__TMS_VOID *				/* IN: pClientInfo */
	));

/* Function type for initializing the UTASK system */
__TMS_FNTYPE_DEF(
	UTASK_ROOT_INIT_FN,
	__TMS_BOOL, (
		__TMS_UTASK_ROOT *			/* IN OUT: pTaskRoot */,
		__TMS_CONST __TMS_UTASK_ROOT_CONFIG *	/* IN: pRootConfig */
		)
	);

/* Function type for initializing a process */
__TMS_FNTYPE_DEF(
	UTASK_PROCESS_INIT_FN,
	__TMS_VOID *, (
		__TMS_UTASK_PROCESS_HANDLE,		/* IN: hProc */
		__TMS_CONST __TMS_VOID *		/* IN: pOptionalInfo */
		)
	);

/* Function type for de-initializing a process */
__TMS_FNTYPE_DEF(
	UTASK_PROCESS_EXIT_FN,
	__TMS_VOID, (
		__TMS_VOID * __TMS_CONST		/* IN: pCtx_void */
		)
	);

/* Function type for main message loop */
__TMS_FNTYPE_DEF(
	UTASK_MSGLOOP_FN,
	__TMS_UTASK_STAT, (
		__TMS_UTASK_HANDLE,			/* IN:  hTask */
		__TMS_UTASK_PROCESS_INDEX,		/* IN:  iReplyProc */
		__TMS_UTASK_MSG_HANDLE *		/* OUT: phReply */
		)
	);

/* Function type for process message dispatcher called by message loop */
__TMS_FNTYPE_DEF(
	UTASK_PROCESS_MSGDISPATCH_FN,
	__TMS_VOID, (
		__TMS_VOID * __TMS_CONST,		/* IN: pCtx_void */
		__TMS_UTASK_MSG_HANDLE			/* IN: hMsg */
		)
	);

/****************************************************************************\
|
|	Task switch functions
|
\****************************************************************************/

/* 
|| Function type to create a process
||
|| It should register it's name with the OS, copy the configuration parameter
|| and then halt and wait to be started before calling the message loops
*/
__TMS_FNTYPE_DEF(
	UTASK_CREATETASK_FN,
	__TMS_UTASK_STAT, (
		__TMS_UPLATFORM *,			/* IN: pPlatform */
		__TMS_CONST __TMS_UTASK_CONFIG_HDR *	/* IN: pTaskCfgHdr */
		)
	);

/* 
|| Function type to start a task
||
|| The task is released to start receiving messages
*/
__TMS_FNTYPE_DEF(
	UTASK_STARTTASK_FN,
	__TMS_UTASK_STAT, (
		__TMS_CONST __TMS_TEXT *		/* IN: pTaskName */
		)
	);

/* Function type to locate a task */
__TMS_FNTYPE_DEF(
	UTASK_LOCATETASK_FN,
	__TMS_UTASK_STAT, (
		__TMS_CONST __TMS_TEXT *,		/* IN:  pTaskName */
		__TMS_UTASK_MAILBOX_HANDLE *		/* OUT: phMailbox */
		)
	);

/* Function type to get the mailbox and index of process withing a task */
__TMS_FNTYPE_DEF(
	UTASK_LOCATEPROCESS_FN,
	__TMS_UTASK_STAT, (
		__TMS_UTASK_PROCESS_HANDLE,		/* IN:  hSelf */
		__TMS_CONST __TMS_TEXT *,		/* IN:  pTaskName */
		__TMS_CONST __TMS_TEXT *,		/* IN:  pProcName */
		__TMS_UTASK_MAILBOX_HANDLE *,		/* OUT: phMailbox */
		__TMS_UTASK_PROCESS_INDEX *		/* OUT: piProc */
		)
	);

/* Function type to allocate a message */
__TMS_FNTYPE_DEF(
	UTASK_ALLOCATEMESSAGE_FN,
	__TMS_UTASK_STAT, (
		__TMS_UTASK_PROCESS_HANDLE,		/* IN:  hProc */
		__TMS_UTASK_MSGCODE,			/* IN:  MsgCode */
		__TMS_BYTES,				/* IN:  EstimatedBufSize */
		__TMS_UTASK_MSG_HANDLE *,		/* OUT: phMsg */
		__TMS_UINT8 **				/* OUT: ppMsgBody */
		)
	);

/* 
|| Function type to get a timer handle 
||	This function will never fail once debugged - assert-fail if error found
*/
__TMS_FNTYPE_DEF(
	UTASK_MILLITIMEOUT_FN,
	__TMS_VOID, (
		__TMS_UTASK_PROCESS_HANDLE,		/* IN:  hProc */
		__TMS_ULONG,				/* IN:  MilliSec */
		__TMS_UTASK_TIMEOUT_HANDLE *		/* OUT: phTmo */
		)
	);

/* Function type to allocate some memory (zero'ed) for work context */
__TMS_FNTYPE_DEF(
	UTASK_MALLOCZERO_FN,
	__TMS_VOID *, (
		__TMS_UTASK_PROCESS_HANDLE,		/* IN: hProc */
		__TMS_BYTES				/* IN: nBytes */
		)
	);

/* Function type to free some memory for a task */
__TMS_FNTYPE_DEF(
	UTASK_FREE_FN,
	__TMS_VOID, (
		__TMS_UTASK_PROCESS_HANDLE,		/* IN: hProc */
		__TMS_VOID *,				/* IN: pBlock */
		__TMS_BYTES				/* IN: nBytes */
		)
	);

/* 
|| Function type to print a debug message on the console 
*/
__TMS_FNTYPE_DEF(
	UTASK_DBGPRINTF_FN,
	__TMS_VOID, (
		__TMS_UTASK_PROCESS_HANDLE,		/* IN: hProc */
		__TMS_UINT32,				/* IN: DebugMask */
		__TMS_CONST __TMS_TEXT *,		/* IN: fmt */
		...
		)
	);

/****************************************************************************\
|
|	Message functions
|
\****************************************************************************/

/* 
|| Function type to allocate a buffer and attach it to a message
||	This will on some OS'es allocate and attach a buffer to the message, 
||	and on other OS'es just return a pointer into the already allocated message
*/
__TMS_FNTYPE_DEF(									\
	UTASK_MSG_ALLOCATEBUFFER_EX_FN,							\
	__TMS_UTASK_STAT, (								\
		__TMS_UTASK_MSG_HANDLE,			/* IN:  hMsg */			\
		__TMS_UTASK_MSG_BUFINDEX,		/* IN:  iBuf */			\
		__TMS_BYTES,				/* IN:  nBuf */			\
		__TMS_BYTES,				/* IN:  sizeBuf */		\
		__TMS_UTASK_MSG_ACCESSFLAGS,		/* IN:  AccessFlags */		\
		__TMS_UINT8 **				/* OUT: ppBuf (Optional) */	\
		)									\
	);

/* 
|| Function type to allocate a reply-message 
*/
__TMS_FNTYPE_DEF(
	UTASK_MSG_ALLOCATEREPLY_FN,
	__TMS_UTASK_STAT, (
		__TMS_UTASK_MSG_HANDLE,			/* IN:   hRequestMsg */
		__TMS_UTASK_MSGCODE,			/* IN:   ReplyMsgCode */
		__TMS_BYTES *,				/* OUT:  pEstimatedBufSize (Optional) */
		__TMS_UTASK_MSG_HANDLE *,		/* OUT:  phReplyMsg */
		__TMS_UINT8 **				/* OUT:  ppReplyMsgBody */
		)
	);

/* 
|| Function type to attach a buffer to message 
||	This will on some OS'es attach pBuffer to the message,
||	and on other OS'es copy *pBuffer into the message
||	This function will never fail once debugged - assert-fail if error found
*/
__TMS_FNTYPE_DEF(									\
	UTASK_MSG_ATTACHBUFFER_EX_FN,							\
	__TMS_VOID, (									\
		__TMS_UTASK_MSG_HANDLE,			/* IN:  hMsg  */		\
		__TMS_UTASK_MSG_BUFINDEX,		/* IN:  iBuf */			\
		__TMS_UTASK_MSG_ACCESSFLAGS,		/* IN:  AccessFlags */		\
		__TMS_BYTES,				/* IN:  nBuf */			\
		__TMS_BYTES,				/* IN:  sizeBuf */		\
		__TMS_CONST __TMS_UINT8 *		/* IN:  pBuf */			\
		)									\
	);

/* 
|| Function type to free a message 
||	This function will never fail once debugged - assert-fail if error found
||	Message handle wil be invalidated
*/
__TMS_FNTYPE_DEF(
	UTASK_MSG_CLOSE_FN,
	__TMS_VOID, (
		__TMS_UTASK_MSG_HANDLE *		/* IN/OUT: phMsg */
		)
	);

/* 
|| Function type to get a pointer to message body
||	This function will never fail once debugged - assert-fail if error found
*/
__TMS_FNTYPE_DEF(
	UTASK_MSG_GETBODY_FN,
	__TMS_VOID, (
		__TMS_UTASK_MSG_HANDLE,			/* IN:  hMsg */
		__TMS_UINT8 **				/* OUT: ppMsgBody */
		)
	);

/* 
Function type to get a handle to an attached buffer 
||	This function will never fail once debugged - assert-fail if error found
*/
__TMS_FNTYPE_DEF(
	UTASK_MSG_GETBUFFER_FN,
	__TMS_VOID, (
		__TMS_UTASK_MSG_HANDLE,			/* IN:  hMsg */
		__TMS_UTASK_MSG_BUFINDEX,		/* IN:  iBuf */
		__TMS_UTASK_BUFFER_HANDLE *		/* OUT: phBuf */
		)
	);

/* 
|| Function type to get the message code from a message
||	This function will never fail once debugged - assert-fail if error found
*/
__TMS_FNTYPE_DEF(
	UTASK_MSG_GETMSGCODE_FN,
	__TMS_VOID, (
		__TMS_UTASK_MSG_HANDLE,			/* IN:  hMsg */
		__TMS_UTASK_MSGCODE *			/* OUT: pMsgCode */
		)
	);

/* 
|| Function type to get sender of a message 
||	This function will never fail once debugged - assert-fail if error found
*/
__TMS_FNTYPE_DEF(
	UTASK_MSG_GETSENDER_FN,
	__TMS_VOID, (
		__TMS_UTASK_MSG_HANDLE,			/* IN:  hMsg */
		__TMS_UTASK_MAILBOX_HANDLE *,		/* OUT: phMailbox */
		__TMS_UTASK_PROCESS_INDEX *		/* OUT: piProc */
		)
	);

/* 
|| Function type to send an indication message
*/
__TMS_FNTYPE_DEF(
	UTASK_MSG_SENDINDICATION_FN,
	__TMS_UTASK_STAT, (
		__TMS_UTASK_PROCESS_HANDLE,		/* IN:     hProc */
		__TMS_UTASK_MSG_HANDLE *,		/* IN/OUT: phIndication */
		__TMS_UTASK_MAILBOX_HANDLE,		/* IN:     hDestMailbox */
		__TMS_UTASK_PROCESS_INDEX,		/* IN:     iDestProcess */
		__TMS_BYTES *				/* OUT:    pMsgSeq (Optional) */
		)
	);

/* 
|| Function type to send a reply-message 
*/
__TMS_FNTYPE_DEF(
	UTASK_MSG_SENDREPLY_FN,
	__TMS_UTASK_STAT, (
		__TMS_UTASK_PROCESS_HANDLE,		/* IN:     hProc */
		__TMS_UTASK_MSG_HANDLE *,		/* IN/OUT: phReply */
		__TMS_BYTES *				/* OUT:    pMsgSeq (Optional) */
		)
	);

/* 
|| Function type to send a request
*/
__TMS_FNTYPE_DEF(
	UTASK_MSG_SENDREQUEST_FN,
	__TMS_UTASK_STAT, (
		__TMS_UTASK_PROCESS_HANDLE,		/* IN:     hProc */
		__TMS_UTASK_MSG_HANDLE *,		/* IN/OUT: phRequest */
		__TMS_UTASK_MAILBOX_HANDLE,		/* IN:     hDestMailbox */
		__TMS_UTASK_PROCESS_INDEX,		/* IN:     iDestProcess */
		__TMS_BYTES *				/* OUT:    pMsgSeq (Optional) */
		)
	);

/* 
|| Function type to send a request and receive the reply in a RPC-style
*/
__TMS_FNTYPE_DEF(
	UTASK_MSG_SENDREQUESTWAITREPLY_FN,							\
	__TMS_UTASK_STAT, (									\
		__TMS_UTASK_PROCESS_HANDLE,		/* IN:     hProc */			\
		__TMS_UTASK_MSG_HANDLE *,		/* IN/OUT: phRequest */			\
		__TMS_UTASK_MAILBOX_HANDLE,		/* IN:     hDestMailbox */		\
		__TMS_UTASK_PROCESS_INDEX,		/* IN:     iDestProcess */		\
		__TMS_UTASK_MSG_HANDLE *,		/* OUT:    phReply */			\
		__TMS_UTASK_MSGCODE,			/* IN:     ReplyMsgCode */		\
		__TMS_UTASK_TIMEOUT_HANDLE,		/* IN:     hTmo */			\
		__TMS_BYTES *				/* OUT:    pRequestSeq (Optional) */	\
		)										\
	);

/****************************************************************************\
|
|	Buffer functions
|
\****************************************************************************/

/* Function type to free a buffer */
__TMS_FNTYPE_DEF(
	UTASK_BUFFER_CLOSE_FN,
	__TMS_VOID, (
		__TMS_UTASK_BUFFER_HANDLE *		/* IN/OUT:  phBuf */
		)
	);

/* Function type to get a pointer to data in attached buffer */
__TMS_FNTYPE_DEF(								\
	UTASK_BUFFER_GETREFERENCE_FN,						\
	__TMS_VOID, (								\
		__TMS_UTASK_BUFFER_HANDLE,		/* IN:  hBuf */		\
		__TMS_BYTES,				/* IN:  Offset */	\
		__TMS_BYTES,				/* IN:  Length */	\
		__TMS_BYTES *,				/* OUT: pnBuf */	\
		__TMS_BYTES *,				/* OUT: psizeBuf */	\
		__TMS_UINT8 **				/* OUT: ppBuf */	\
		)								\
	);

/* Function type to get a pointer to data in attached buffer */
__TMS_FNTYPE_DEF(								\
	UTASK_BUFFER_GETREFERENCE_EX_FN,					\
	__TMS_VOID, (								\
		__TMS_UTASK_BUFFER_HANDLE,		/* IN:  hBuf */		\
		__TMS_BYTES,				/* IN:  Offset */	\
		__TMS_BYTES,				/* IN:  Length */	\
		__TMS_UTASK_MSG_ACCESSFLAGS,		/* IN:  AccessFlags */	\
		__TMS_UINT8 *,				/* IN:  pLocalBuf */	\
		__TMS_BYTES,				/* IN:  SizeLocalBuf */	\
		__TMS_BYTES *,				/* OUT: pnBuf */	\
		__TMS_BYTES *,				/* OUT: psizeBuf */	\
		__TMS_UINT8 **				/* OUT: ppBuf */	\
		)								\
	);

/* Function type to set the actual number of bytes in the buffer */
__TMS_FNTYPE_DEF(
	UTASK_BUFFER_SETLENGTH_FN,
	__TMS_VOID, (
		__TMS_UTASK_BUFFER_HANDLE,		/* IN:  hBuf */
		__TMS_BYTES				/* IN:  nBuf */
		)
	);

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UTASK_MSGCODE_TYPE__MIN	__TMS_UTASK_MSGCODE_TYPE__MIN
# define UTASK_MSGCODE_TYPE_REQ	__TMS_UTASK_MSGCODE_TYPE_REQ
# define UTASK_MSGCODE_TYPE_REP	__TMS_UTASK_MSGCODE_TYPE_REP
# define UTASK_MSGCODE_TYPE_IND	__TMS_UTASK_MSGCODE_TYPE_IND
# define UTASK_MSGCODE_TYPE_EVT	__TMS_UTASK_MSGCODE_TYPE_EVT
# define UTASK_MSGCODE_TYPE__MAX	__TMS_UTASK_MSGCODE_TYPE__MAX
# define UTASK_STAT_OK	__TMS_UTASK_STAT_OK
# define UTASK_STAT_ENOTFOUND	__TMS_UTASK_STAT_ENOTFOUND
# define UTASK_STAT_ETIMEOUT	__TMS_UTASK_STAT_ETIMEOUT
# define UTASK_STAT_EQUIT	__TMS_UTASK_STAT_EQUIT
# define UTASK_STAT_EINVALID_PARAMETER	__TMS_UTASK_STAT_EINVALID_PARAMETER
# define UTASK_STAT_ENOMEMORY	__TMS_UTASK_STAT_ENOMEMORY
# define UTASK_STAT_EUNKNOWN	__TMS_UTASK_STAT_EUNKNOWN
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of utask_types.h ****/
#endif /* _UTASK_TYPES_H_ */
