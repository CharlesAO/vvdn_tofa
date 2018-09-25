/* protohid.h	Mon Jun  1 2015 00:48:42 tmm */

/*

Module:  protohid.h

Function:
	The definitions for the standard USB HID protocol on top of the
	MCCI DataPump.

Version:
	V3.15b	Mon Jun  1 2015 00:48:42 tmm	Edit level 9

Copyright notice:
	This file copyright (C) 1998-2000, 2006, 2009, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	May 1998

Revision history:
   1.42a  Sun May 24 1998 17:18:17  tmm
	Module created.

   1.60n  10/1/2000  tmm
	Bring into line with current standards for macros, etc.

   1.97e  Wed Jul 26 2006 17:18:00  tmm
	Convert to current standards.  The only API function now is
	UsbHid10_ProcotolCreate(); and we need a USBPUMP_USBHID_CONFIG
	object.

   1.97e  Wed Aug 02 2006 12:48:01  chwon
	Fix GCC compiler warning

   1.97j  Wed Oct 10 2007 13:52:13  ssyang
	Change "uhid_IntQe" to "uhid_IntInQe" and add uhid_IntOutQe
	into UPROTO_USBHID structure.

   1.97j  Tue Nov 13 2007 11:58:29  ssyang
	Added new QueueForOutReportV2() API support

   2.01a  Thu Sep 24 2009 17:03:15  ssyang
	7250: Changed __TMS_UPROTO_USBHID_CONFIG_INTOUT_REPORTBUFFER_SIZE_DEFAULT
	and __TMS_USBHID_HOST_DATA_SIZE_DEFAULT to 64

   2.01a  Sun Nov 15 2009 13:55:29  ssyang
	9347: Add sizeHostDataBuffer field to UPROTO_USBHID_CONFIG.
	
   3.15b  Mon Jun  1 2015 00:48:42  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _PROTOHID_H_		/* prevent multiple includes */
#define _PROTOHID_H_

#ifndef _USBPUMP_H_
# include "usbpump.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBHID10_H_
# include "usbhid10.h"
#endif

#ifndef _USBPROTOINIT_H_
# include "usbprotoinit.h"
#endif

/*
|| Declare the types which we'll use below.
*/
__TMS_TYPE_DEF_STRUCT	(UPROTO_USBHID_CLIENT_METHODS);
__TMS_TYPE_DEF_STRUCT	(UPROTO_USBHID_PUBLIC_METHODS);
__TMS_TYPE_DEF_STRUCT	(UPROTO_USBHID);
__TMS_TYPE_DEF_STRUCT	(UPROTO_USBHID_CONFIG);
__TMS_TYPE_DEF_STRUCT	(UPROTO_USBHID_OUT_REPORT_QE);

/*
|| The UPROTO_USBHID_OUT_REPORT_QE object
*/

/*
|| Done function type for UPROTO_USBHID_OUT_REPORT_QE
|| this should return TRUE for success, FALSE for failure
*/
__TMS_FNTYPE_DEF(
UPROTO_USBHID_OUT_REPORT_QE_DONE_FN,
__TMS_BOOL,	(
	__TMS_UPROTO_USBHID_OUT_REPORT_QE *	pHidOutReportQe,
	__TMS_VOID *				pDoneInfo,
	__TMS_USTAT				Status
	));
	
struct __TMS_STRUCTNAME	(UPROTO_USBHID_OUT_REPORT_QE)
	{
	/* linked list fields */
	__TMS_UPROTO_USBHID_OUT_REPORT_QE *	pNext;
	__TMS_UPROTO_USBHID_OUT_REPORT_QE *	pLast;
	
	__TMS_UINT8 *		pBuffer;	/* IN: pointer to buffer */
	__TMS_USHORT		nBuffer;	/* IN: size of buffer */
	
	__TMS_USHORT		wReportTag;	/* IN: report tag */
	__TMS_BYTES		nReceived;	/* OUT: number of bytes received */

	__TMS_USTAT		Status;		/* OUT: status of transfer */
	
	/* IN: pointer to done-function and done-info*/
	__TMS_UPROTO_USBHID_OUT_REPORT_QE_DONE_FN *	pDoneFn;
	__TMS_VOID *					pDoneInfo;
	};

#define	__TMS_UPROTO_USBHID_OUT_REPORT_QE_INIT_V1(			\
	A_pBuffer,							\
	A_nBuffer,							\
	A_wReportTag,							\
	A_pDoneFn,							\
	A_pDoneInfo							\
	)								\
	{								\
	/* pNext */ __TMS_NULL,						\
	/* pLast */ __TMS_NULL,						\
	(A_pBuffer),							\
	(A_nBuffer),							\
	(A_wReportTag)							\
	/* nReceived */ 0,						\
	/* Status */ __TMS_USTAT_OK,					\
	(A_pDoneFn)							\
	(A_pDoneInfo)							\
	}

#define	__TMS_UPROTO_USBHID_OUT_REPORT_QE_SETUP_V1(			\
	A_pHidOutReportQe,						\
	A_pBuffer,							\
	A_nBuffer,							\
	A_wReportTag,							\
	A_pDoneFn,							\
	A_pDoneInfo							\
	)								\
   do	{								\
	(A_pHidOutReportQe)->pNext = __TMS_NULL;			\
	(A_pHidOutReportQe)->pLast = __TMS_NULL;			\
	(A_pHidOutReportQe)->pBuffer = (A_pBuffer);			\
	(A_pHidOutReportQe)->nBuffer = (A_nBuffer);			\
	(A_pHidOutReportQe)->wReportTag = (A_wReportTag);		\
	(A_pHidOutReportQe)->nReceived = 0;				\
	(A_pHidOutReportQe)->Status = __TMS_USTAT_OK;			\
	(A_pHidOutReportQe)->pDoneFn = (A_pDoneFn);			\
	(A_pHidOutReportQe)->pDoneInfo = (A_pDoneInfo);			\
	} while (0)

/*
|| The UPROTO_USBHID_CONFIG object
*/
#ifndef __TMS_UPROTO_USBHID_CONFIG_INTOUT_REPORTBUFFER_SIZE_DEFAULT	/*PARAM*/ 
# define __TMS_UPROTO_USBHID_CONFIG_INTOUT_REPORTBUFFER_SIZE_DEFAULT	64
#endif

struct __TMS_STRUCTNAME	(UPROTO_USBHID_CONFIG)
	{
	/* if NULL, defaults are used */
	__TMS_CONST __TMS_TEXT			*pNameOverride;
	/* if zero, client must OPEN */
	__TMS_BYTES				sizeClientObject;
	/* if NULL, client must OPEN & supply at open time */
	__TMS_CONST __TMS_UPROTO_USBHID_CLIENT_METHODS	*pClientObjectMethodTable;
	
	/*
	|| buffer size to be allocated at protocol init time and used to
	|| receive out-report via interrupt-out pipe. but buffer is allocated
	|| only if HID interface has interrupt-out endpoint declared.
	*/
	__TMS_USHORT		sizeIntOutReportBuffer;
	__TMS_USHORT		sizeHostDataBuffer;
	};

#define	__TMS_UPROTO_USBHID_CONFIG_INIT_V3(				\
	A_pNameOverride,						\
	A_sizeClientObject,						\
	A_pClientObjectMethodTable,					\
	A_sizeIntOutReportBuffer,					\
	A_sizeHostDataBuffer						\
	)								\
	{								\
	(A_pNameOverride),						\
	(A_sizeClientObject),						\
	(A_pClientObjectMethodTable),					\
	(A_sizeIntOutReportBuffer),					\
	(A_sizeHostDataBuffer)						\
	}

#define	__TMS_UPROTO_USBHID_CONFIG_SETUP_V3(				\
	A_pConfig,							\
	A_pNameOverride,						\
	A_sizeClientObject,						\
	A_pClientObjectMethodTable,					\
	A_sizeIntOutReportBuffer,					\
	A_sizeHostDataBuffer						\
	)								\
   do	{								\
	(A_pConfig)->pNameOverride = (A_pNameOverride);			\
	(A_pConfig)->sizeClientObject = (A_sizeClientObject);		\
	(A_pConfig)->pClientObjectMethodTable =	(A_pClientObjectMethodTable);\
	(A_pConfig)->sizeIntOutReportBuffer = (A_sizeIntOutReportBuffer);\
	(A_pConfig)->sizeHostDataBuffer = (A_sizeHostDataBuffer);	\
	} while (0)
		
#define	__TMS_UPROTO_USBHID_CONFIG_INIT_V2(				\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable,				\
		sizeIntOutReportBuffer					\
		)							\
	__TMS_UPROTO_USBHID_CONFIG_INIT_V3(				\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable,				\
		sizeIntOutReportBuffer,					\
		__TMS_USBHID_HOST_DATA_SIZE_DEFAULT			\
		)

#define	__TMS_UPROTO_USBHID_CONFIG_SETUP_V2(				\
		pConfig,						\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable,				\
		sizeIntOutReportBuffer					\
		)							\
	__TMS_UPROTO_USBHID_CONFIG_SETUP_V3(				\
		pConfig,						\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable,				\
		sizeIntOutReportBuffer,					\
		__TMS_USBHID_HOST_DATA_SIZE_DEFAULT			\
		)
	
#define	__TMS_UPROTO_USBHID_CONFIG_INIT_V1(				\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable				\
		)							\
	__TMS_UPROTO_USBHID_CONFIG_INIT_V2(				\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable,				\
		__TMS_UPROTO_USBHID_CONFIG_INTOUT_REPORTBUFFER_SIZE_DEFAULT\
		)
			
#define	__TMS_UPROTO_USBHID_CONFIG_SETUP_V1(				\
		pConfig,						\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable				\
		)							\
	__TMS_UPROTO_USBHID_CONFIG_SETUP_V2(				\
		pConfig,						\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable				\
		__TMS_UPROTO_USBHID_CONFIG_INTOUT_REPORTBUFFER_SIZE_DEFAULT\
		)

/*
|| The UPROTO_USBHID structure is the "self" object for each instance
|| of a HID object.
*/
struct __TMS_STRUCTNAME	(UPROTO_USBHID)
	{
	__TMS_USBPUMP_OBJECT_HEADER uhid_ObjectHeader;	/* the object header */
	__TMS_UDEVICE		*uhid_pDevice;		/* the device */
	__TMS_UINTERFACE	*uhid_pInterface;	/* the controlling ifc */
	__TMS_ULONG		uhid_Flags;		/* miscellaneous state */

	__TMS_CONST __TMS_UPROTO_USBHID_PUBLIC_METHODS
				*uhid_pPublicMethods;	/* the switch we export */

	__TMS_CONST __TMS_UPROTO_USBHID_CLIENT_METHODS
				*uhid_pClientMethods;	/* the method table exported to us by the client */

	__TMS_VOID		*uhid_pClientObject;	/* context data for client */
	__TMS_UCHAR		*uhid_pHostDataBuf;	/* pointer to the host data
							|| buffer
							*/
	__TMS_UBUFQE		*uhid_pOutReportQeHead;	/* list of pending ubufqes
							|| that are waiting for
							|| reports from host.
							*/						
	__TMS_UBUFQE		*uhid_pThisOutReport;	/* pointer to outreport being
							|| accepted from host.
							*/
	/*
	|| List of pending UPROTO_USBHID_OUT_REPORT_QEs and QE in processing
	|| These are for new QueueForOutReportV2 API which support out-report
	|| via both default pipe and interrupt-out pipe
	*/
	__TMS_UPROTO_USBHID_OUT_REPORT_QE	*uhid_pOutReportQeHeadV2;
	__TMS_UPROTO_USBHID_OUT_REPORT_QE	*uhid_pThisOutReportV2;
	__TMS_BOOL				uhid_fOutputReportUseId;

	/*
	|| Internal buffer to read out HID out-report from interrupt-out pipe
	*/
	__TMS_USHORT 				uhid_nIntOutReportBuffer;
	__TMS_VOID 				*uhid_pIntOutReportBuffer;
								
	__TMS_UBUFQE		*uhid_pInReportQeHead;	/* the head of the list of
							|| inreports that are
							|| waiting for the host
							*/
	__TMS_UEVENTNODE	uhid_DeviceEvent;	/* the device ev node */
	__TMS_UEVENTNODE	uhid_InterfaceEvent;	/* the controlling ifc ev node */
	__TMS_USETUP		uhid_LastSetup;		/* the last setup packet
							|| we received; useful for
							|| deferred processing
							|| of SET commands.
							*/
	__TMS_USHORT		uhid_usLastReportFrame;	/* the frame# of the last
							|| time we completed a
							|| report.
							*/
	__TMS_USHORT		uhid_usHostDataSize;	/* size of the host data
							|| buffer
							*/
	__TMS_UBUFQE		uhid_IntInQe;		/* the interrupt IN pipe ubufqe */
	__TMS_UBUFQE		uhid_IntOutQe;		/* the interrupt OUT pipe ubufqe */
	
	__TMS_CALLBACKCOMPLETION uhid_NotifyEventCompletion;
	};


#define	__TMS_UPROTO_USBHID_TAG		__TMS_UHIL_MEMTAG('U', 'h', 'i', 'd')

/* all TAs have the same base name */
#define	__TMS_UPROTO_USBHID_DERIVED_NAME(name)	name ".hid.proto.mcci.com"
#define	__TMS_UPROTO_USBHID_NAME	__TMS_UPROTO_USBHID_DERIVED_NAME("generic")

/*
|| The flags for this protocol.
*/
#define	__TMS_UHIDFLAG_UP		(1ul << 0)	/* it's up */
#define	__TMS_UHIDFLAG_BOOTPROTO	(1ul << 1)	/* we're using boot proto */
#define	__TMS_UHIDFLAG_FRAMEVALID	(1ul << 2)	/* last report framenumber
							|| is valid
							*/
#define	__TMS_UHIDFLAG_SUPPORTBOOT	(1ul << 3)	/* we support the boot proto */
#define	__TMS_UHIDFLAG_TX_PENDING	(1ul << 4)	/* interrupt I/O is pending */

#ifndef __TMS_USBHID_HOST_DATA_SIZE_DEFAULT	/*PARAM*/
# define	__TMS_USBHID_HOST_DATA_SIZE_DEFAULT	64	/* should be enough in many cases. */
#endif
/****************************************************************************\
|
| The client method table is used for sending messages from the protocol
| up to the client (it's used by the protocol)
|
\****************************************************************************/


__TMS_FNTYPE_DEF(
UPROTO_USBHID_CLIENT_METHOD_ATTACH_FN,
__TMS_BOOL, 	(
	__TMS_VOID *		/* pClientObject */,
	__TMS_PUPROTO_USBHID	/* self */
	));

__TMS_FNTYPE_DEF(
UPROTO_USBHID_CLIENT_METHOD_DETACH_FN,
__TMS_VOID, 	(
	__TMS_VOID *		/* pClientObject */,
	__TMS_PUPROTO_USBHID	/* self */
	));

__TMS_FNTYPE_DEF(
UPROTO_USBHID_CLIENT_METHOD_CONFIGURE_FN,
__TMS_VOID, 	(
	__TMS_VOID *		/* pClientObject */,
	__TMS_PUPROTO_USBHID	/* self */,
	__TMS_UEVENT		/* why */
	));

__TMS_FNTYPE_DEF(
UPROTO_USBHID_CLIENT_METHOD_UNCONFIGURE_FN,
__TMS_VOID, 	(
	__TMS_VOID *		/* pClientObject */,
	__TMS_PUPROTO_USBHID	/* self */,
	__TMS_UEVENT		/* why */
	));

__TMS_FNTYPE_DEF(
UPROTO_USBHID_CLIENT_METHOD_SUSPEND_FN,
__TMS_VOID, 	(
	__TMS_VOID *		/* pClientObject */,
	__TMS_PUPROTO_USBHID	/* self */
	));

__TMS_FNTYPE_DEF(
UPROTO_USBHID_CLIENT_METHOD_RESUME_FN,
__TMS_VOID, 	(
	__TMS_VOID *		/* pClientObject */,
	__TMS_PUPROTO_USBHID	/* self */
	));

__TMS_FNTYPE_DEF(
UPROTO_USBHID_CLIENT_METHOD_GET_NEXT_REPORT_FN,
__TMS_VOID, 	(
	__TMS_VOID *		/* pClientObject */,
	__TMS_PUPROTO_USBHID	/* self */
	));

__TMS_FNTYPE_DEF(
UPROTO_USBHID_CLIENT_METHOD_SETUP_FN,
__TMS_BOOL,	(
	__TMS_VOID *		/* pClientObject */,
	__TMS_PUPROTO_USBHID	/* self */,
	__TMS_PUEVENTSETUP	/* the setup packet */
	));

__TMS_FNTYPE_DEF(
UPROTO_USBHID_CLIENT_METHOD_EVENT_RESPONSE_FN,
__TMS_BOOL, 	(
	__TMS_VOID *		/* pClientObject */,
	__TMS_PUPROTO_USBHID	/* self */
	));

/*
|| The switch layout.
*/
struct __TMS_STRUCTNAME(UPROTO_USBHID_CLIENT_METHODS)
	{
	__TMS_ULONG					MagicBegin;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_ATTACH_FN		Attach;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_DETACH_FN		Detach;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_CONFIGURE_FN		Configure;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_UNCONFIGURE_FN	Unconfigure;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SUSPEND_FN		Suspend;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_RESUME_FN		Resume;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_GET_NEXT_REPORT_FN	GetNextReport;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SETUP_FN		GetReport;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetReport;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SETUP_FN		GetIdle;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetIdle;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetProtocol;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SETUP_FN		GetReportDescriptor;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetReportDescriptor;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SETUP_FN		GetPhysicalDescriptor;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetPhysicalDescriptor;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SETUP_FN		GetMiscellaneous;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetMiscellaneous;
	__TMS_PUPROTO_USBHID_CLIENT_METHOD_EVENT_RESPONSE_FN	EventResponse;
	__TMS_ULONG					MagicEnd;
	};

#define	__TMS_UPROTO_USBHID_CLIENT_METHODS_BEGIN	(0xCC21C700 + '<')
#define	__TMS_UPROTO_USBHID_CLIENT_METHODS_END		(0x7C12CC00 + '>')

/* never add an argument to this macro; instead, create a V2 or higher */
#define __TMS_UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS_V1(		\
		Attach,							\
		Detach,							\
		Configure,						\
		Unconfigure,						\
		Suspend,						\
		Resume,							\
		GetNextReport,						\
		GetReport,						\
		SetReport,						\
		GetIdle,						\
		SetIdle,						\
		SetProtocol,						\
		GetReportDescriptor,					\
		SetReportDescriptor,					\
		GetPhysicalDescriptor,					\
		SetPhysicalDescriptor,					\
		GetMiscellaneous,					\
		SetMiscellaneous,					\
		EventResponse						\
		)							\
__TMS_UPROTO_USBHID_CLIENT_METHOD_ATTACH_FN		Attach;		\
__TMS_UPROTO_USBHID_CLIENT_METHOD_DETACH_FN		Detach;		\
__TMS_UPROTO_USBHID_CLIENT_METHOD_CONFIGURE_FN		Configure;	\
__TMS_UPROTO_USBHID_CLIENT_METHOD_UNCONFIGURE_FN	Unconfigure;	\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SUSPEND_FN		Suspend;	\
__TMS_UPROTO_USBHID_CLIENT_METHOD_RESUME_FN		Resume;		\
__TMS_UPROTO_USBHID_CLIENT_METHOD_GET_NEXT_REPORT_FN	GetNextReport;	\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SETUP_FN		GetReport;	\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetReport;	\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SETUP_FN		GetIdle;	\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetIdle;	\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetProtocol;	\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SETUP_FN		GetReportDescriptor;\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetReportDescriptor;\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SETUP_FN		GetPhysicalDescriptor;\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetPhysicalDescriptor;\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SETUP_FN		GetMiscellaneous;\
__TMS_UPROTO_USBHID_CLIENT_METHOD_SETUP_FN		SetMiscellaneous;\
__TMS_UPROTO_USBHID_CLIENT_METHOD_EVENT_RESPONSE_FN	EventResponse

/*
|| Use UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS() to declare all of
|| the individual functions.  It's OK to declare functions that you
|| never actually use, so normally, one just declares them all and
|| NULLs the unused entries in the PROTOSWITCH.
||
|| For backward compatibility, this must be the same as
|| UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS_STD(Prefix)
*/
#define	__TMS_UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS(Prefix)	\
	__TMS_UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS_STD(Prefix)


/*
|| Use UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS() to declare all of
|| the individual functions.  It's OK to declare functions that you
|| never actually use, so normally, one just declares them all and
|| NULLs the unused entries in the PROTOSWITCH.
||
|| Declare all the functions in a standard way.  You can change the
|| use of the embedded macro to use V2, or whatever, as needed.
*/
#define	__TMS_UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS_STD(Prefix)	\
	__TMS_UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS_V1(		\
		__TMS_CONCAT(Prefix, _Attach),				\
		__TMS_CONCAT(Prefix, _Detach),				\
		__TMS_CONCAT(Prefix, _Configure),			\
		__TMS_CONCAT(Prefix, _Unconfigure),			\
		__TMS_CONCAT(Prefix, _Suspend),				\
		__TMS_CONCAT(Prefix, _Resume),				\
		__TMS_CONCAT(Prefix, _GetNextReport),			\
		__TMS_CONCAT(Prefix, _GetReport),			\
		__TMS_CONCAT(Prefix, _SetReport),			\
		__TMS_CONCAT(Prefix, _GetIdle),				\
		__TMS_CONCAT(Prefix, _SetIdle),				\
		__TMS_CONCAT(Prefix, _SetProtocol),			\
		__TMS_CONCAT(Prefix, _GetReportDescriptor),		\
		__TMS_CONCAT(Prefix, _SetReportDescriptor),		\
		__TMS_CONCAT(Prefix, _GetPhysicalDescriptor),		\
		__TMS_CONCAT(Prefix, _SetPhysicalDescriptor),		\
		__TMS_CONCAT(Prefix, _GetMiscellaneous),		\
		__TMS_CONCAT(Prefix, _SetMiscellaneous),		\
		__TMS_CONCAT(Prefix, _EventResponse)			\
	)

/*
|| Declare the initialization explicitly for a callout (proto) switch.
|| Never add an argument to this macro; instead create a V2 or higher;
|| but as higher versions are added, make sure to update this instance
|| to provide the appropriate initializer.
*/
#define	__TMS_UPROTO_USBHID_CLIENT_METHODS_INIT_V1(	\
		Attach,					\
		Detach,					\
		Configure,				\
		Unconfigure,				\
		Suspend,				\
		Resume,					\
		GetNextReport,				\
		GetReport,				\
		SetReport,				\
		GetIdle,				\
		SetIdle,				\
		SetProtocol,				\
		GetReportDescriptor,			\
		SetReportDescriptor,			\
		GetPhysicalDescriptor,			\
		SetPhysicalDescriptor,			\
		GetMiscellaneous,			\
		SetMiscellaneous,			\
		EventResponse				\
	)						\
	{						\
	__TMS_UPROTO_USBHID_CLIENT_METHODS_BEGIN,	\
		Attach,					\
		Detach,					\
		Configure,				\
		Unconfigure,				\
		Suspend,				\
		Resume,					\
		GetNextReport,				\
		GetReport,				\
		SetReport,				\
		GetIdle,				\
		SetIdle,				\
		SetProtocol,				\
		GetReportDescriptor,			\
		SetReportDescriptor,			\
		GetPhysicalDescriptor,			\
		SetPhysicalDescriptor,			\
		GetMiscellaneous,			\
		SetMiscellaneous,			\
		EventResponse,				\
	__TMS_UPROTO_USBHID_CLIENT_METHODS_END		\
	}

/****************************************************************************\
|
|	The client switch is used by clients to call into the hid class
|	object.  We use this in order to keep the non-init code as
|	flexible as possible.
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
UPROTO_USBHID_PUBLIC_METHOD_QUEUE_FOR_OUT_REPORT_FN,
__TMS_VOID,	(
	__TMS_PUPROTO_USBHID	/* self */,
	__TMS_PUBUFQE		/* the buffer to queue */,
	__TMS_USHORT		/* the report wValue tag */
	));

__TMS_FNTYPE_DEF(
UPROTO_USBHID_PUBLIC_METHOD_QUEUE_FOR_OUT_REPORT_V2_FN,
__TMS_VOID,	(
	__TMS_PUPROTO_USBHID		/* self */,
	__TMS_PUPROTO_USBHID_OUT_REPORT_QE /* the out-report request queue element */
	));

__TMS_FNTYPE_DEF(
UPROTO_USBHID_PUBLIC_METHOD_QUEUE_IN_REPORT_FN,
__TMS_VOID,	(
	__TMS_PUPROTO_USBHID	/* self */,
	__TMS_PUBUFQE		/* the buffer to queue */
	));

__TMS_FNTYPE_DEF(
UPROTO_USBHID_PUBLIC_METHOD_NOTIFY_EVENT_FN,
__TMS_VOID,	(
	__TMS_PUPROTO_USBHID	/* self */
	));

struct __TMS_STRUCTNAME	(UPROTO_USBHID_PUBLIC_METHODS)
	{
	__TMS_ULONG						MagicBegin;
	__TMS_PUPROTO_USBHID_PUBLIC_METHOD_QUEUE_FOR_OUT_REPORT_FN	QueueForOutReport;
	__TMS_PUPROTO_USBHID_PUBLIC_METHOD_QUEUE_FOR_OUT_REPORT_V2_FN	QueueForOutReportV2;
	__TMS_PUPROTO_USBHID_PUBLIC_METHOD_QUEUE_IN_REPORT_FN		QueueInReport;
	__TMS_PUPROTO_USBHID_PUBLIC_METHOD_NOTIFY_EVENT_FN		NotifyEvent;
	__TMS_ULONG						MagicEnd;
	};

#define	__TMS_UPROTO_USBHID_PUBLIC_METHODS_BEGIN	(0x21C72900 + '<')
#define	__TMS_UPROTO_USBHID_PUBLIC_METHODS_END		(0x927C1200 + '>')


/****************************************************************************\
|
|	Initialization for client switches.
|
\****************************************************************************/

/* never add an argument to this macro; instead, create a V2 or higher */
#define	__TMS_UPROTO_USBHID_PUBLIC_METHODS_DECLARE_FNS_V1(		\
		QueueForOutReport,					\
		QueueInReport,						\
		NotifyEvent						\
		)							\
	__TMS_UPROTO_USBHID_PUBLIC_METHOD_QUEUE_FOR_OUT_REPORT_FN	\
						QueueForOutReport;	\
	__TMS_UPROTO_USBHID_PUBLIC_METHOD_QUEUE_IN_REPORT_FN		\
						QueueInReport;		\
	__TMS_UPROTO_USBHID_PUBLIC_METHOD_NOTIFY_EVENT_FN		\
						NotifyEvent

/*
|| Protocols can use UPROTO_USBHID_PUBLIC_METHODS() to declare all
|| of the individual functions which the protocol wants to provide.
|| It's OK to declare functions that are not actually ever defined,
|| so normally, one just declares them all and NULLs the unused entries
||
|| You can change the embedded macro to use V2, or whatever, as needed.
*/
#define	__TMS_UPROTO_USBHID_PUBLIC_METHODS_DECLARE_FNS_STD(Prefix)	\
	__TMS_UPROTO_USBHID_PUBLIC_METHODS_DECLARE_FNS_V1(		\
		__TMS_CONCAT(Prefix, _QueueForOutReport),		\
		__TMS_CONCAT(Prefix, _QueueInReport),			\
		__TMS_CONCAT(Prefix, _NotifyEvent)			\
		)

/*
|| Declare the initialization explicitly for a callin (client) switch.
|| Never add an argument to this macro; instead create a V2 or higher;
|| but as higher versions are added, make sure to update this instance
|| to provide the appropriate initializer.
*/
#define	__TMS_UPROTO_USBHID_PUBLIC_METHODS_INIT_V2(		\
		QueueForOutReport,				\
		QueueForOutReportV2,				\
		QueueInReport,					\
		NotifyEvent					\
		)						\
	{							\
	__TMS_UPROTO_USBHID_PUBLIC_METHODS_BEGIN,		\
		QueueForOutReport,				\
		QueueForOutReportV2,				\
		QueueInReport,					\
		NotifyEvent,					\
	__TMS_UPROTO_USBHID_PUBLIC_METHODS_END			\
	}
	
#define	__TMS_UPROTO_USBHID_PUBLIC_METHODS_INIT_V1(		\
		QueueForOutReport,				\
		QueueInReport,					\
		NotifyEvent					\
		)						\
	__TMS_UPROTO_USBHID_PUBLIC_METHODS_INIT_V2(		\
		QueueForOutReport,				\
		/* V3: QueueForOutReportV2 */ __TMS_NULL,	\
		QueueInReport,					\
		NotifyEvent					\
		)

/*
|| Init the switch in a standard way -- you can change this definition to
|| use V2, or whatever, as needed.
*/
#define	__TMS_UPROTO_USBHID_PUBLIC_METHODS_INIT_STD(Prefix)	\
	__TMS_UPROTO_USBHID_PUBLIC_METHODS_INIT_V1(		\
		__TMS_CONCAT(Prefix, _QueueForOutReport),	\
		__TMS_CONCAT(Prefix, _QueueInReport),		\
		__TMS_CONCAT(Prefix, _NotifyEvent)		\
		)

/*
|| Declare and init the switch in a standard way, naming it
|| as "..._kCallinSwitch", to match current practice.
*/
#define	__TMS_UPROTO_USBHID_PUBLIC_METHODS_DECLARE_STD(Prefix)		\
	__TMS_UPROTO_USBHID_PUBLIC_METHODS_DECLARE_FNS_STD(Prefix);	\
	__TMS_CONST __TMS_UPROTO_USBHID_PUBLIC_METHODS			\
		__TMS_CONCAT(Prefix, _kCallinSwitch) =			\
			__TMS_UPROTO_USBHID_PUBLIC_METHODS_INIT_STD(Prefix)


/****************************************************************************\
|
|	Here are the functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_PROTOCOL_CREATE_FN
UsbHid10_ProtocolCreate;

__TMS_END_DECLS

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_USBHID_OUT_REPORT_QE_INIT_V1(			\
	A_pBuffer,							\
	A_nBuffer,							\
	A_wReportTag,							\
	A_pDoneFn,							\
	A_pDoneInfo							\
	)	\
	__TMS_UPROTO_USBHID_OUT_REPORT_QE_INIT_V1(			\
	A_pBuffer,							\
	A_nBuffer,							\
	A_wReportTag,							\
	A_pDoneFn,							\
	A_pDoneInfo							\
	)
# define UPROTO_USBHID_OUT_REPORT_QE_SETUP_V1(			\
	A_pHidOutReportQe,						\
	A_pBuffer,							\
	A_nBuffer,							\
	A_wReportTag,							\
	A_pDoneFn,							\
	A_pDoneInfo							\
	)	\
	__TMS_UPROTO_USBHID_OUT_REPORT_QE_SETUP_V1(			\
	A_pHidOutReportQe,						\
	A_pBuffer,							\
	A_nBuffer,							\
	A_wReportTag,							\
	A_pDoneFn,							\
	A_pDoneInfo							\
	)
# define UPROTO_USBHID_CONFIG_INTOUT_REPORTBUFFER_SIZE_DEFAULT	\
   __TMS_UPROTO_USBHID_CONFIG_INTOUT_REPORTBUFFER_SIZE_DEFAULT
# define UPROTO_USBHID_CONFIG_INIT_V3(				\
	A_pNameOverride,						\
	A_sizeClientObject,						\
	A_pClientObjectMethodTable,					\
	A_sizeIntOutReportBuffer,					\
	A_sizeHostDataBuffer						\
	)	\
	__TMS_UPROTO_USBHID_CONFIG_INIT_V3(				\
	A_pNameOverride,						\
	A_sizeClientObject,						\
	A_pClientObjectMethodTable,					\
	A_sizeIntOutReportBuffer,					\
	A_sizeHostDataBuffer						\
	)
# define UPROTO_USBHID_CONFIG_SETUP_V3(				\
	A_pConfig,							\
	A_pNameOverride,						\
	A_sizeClientObject,						\
	A_pClientObjectMethodTable,					\
	A_sizeIntOutReportBuffer,					\
	A_sizeHostDataBuffer						\
	)	\
	__TMS_UPROTO_USBHID_CONFIG_SETUP_V3(				\
	A_pConfig,							\
	A_pNameOverride,						\
	A_sizeClientObject,						\
	A_pClientObjectMethodTable,					\
	A_sizeIntOutReportBuffer,					\
	A_sizeHostDataBuffer						\
	)
# define UPROTO_USBHID_CONFIG_INIT_V2(				\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable,				\
		sizeIntOutReportBuffer					\
		)	\
	__TMS_UPROTO_USBHID_CONFIG_INIT_V2(				\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable,				\
		sizeIntOutReportBuffer					\
		)
# define UPROTO_USBHID_CONFIG_SETUP_V2(				\
		pConfig,						\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable,				\
		sizeIntOutReportBuffer					\
		)	\
	__TMS_UPROTO_USBHID_CONFIG_SETUP_V2(				\
		pConfig,						\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable,				\
		sizeIntOutReportBuffer					\
		)
# define UPROTO_USBHID_CONFIG_INIT_V1(				\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable				\
		)	\
	__TMS_UPROTO_USBHID_CONFIG_INIT_V1(				\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable				\
		)
# define UPROTO_USBHID_CONFIG_SETUP_V1(				\
		pConfig,						\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable				\
		)	\
	__TMS_UPROTO_USBHID_CONFIG_SETUP_V1(				\
		pConfig,						\
		pNameOverride,						\
		sizeClientObject,					\
		pClientObjectMethodTable				\
		)
# define UPROTO_USBHID_TAG	\
   __TMS_UPROTO_USBHID_TAG
# define UPROTO_USBHID_DERIVED_NAME(name)	\
   __TMS_UPROTO_USBHID_DERIVED_NAME(name)
# define UPROTO_USBHID_NAME	\
   __TMS_UPROTO_USBHID_NAME
# define UHIDFLAG_UP	\
   __TMS_UHIDFLAG_UP
# define UHIDFLAG_BOOTPROTO	\
   __TMS_UHIDFLAG_BOOTPROTO
# define UHIDFLAG_FRAMEVALID	\
   __TMS_UHIDFLAG_FRAMEVALID
# define UHIDFLAG_SUPPORTBOOT	\
   __TMS_UHIDFLAG_SUPPORTBOOT
# define UHIDFLAG_TX_PENDING	\
   __TMS_UHIDFLAG_TX_PENDING
# define USBHID_HOST_DATA_SIZE_DEFAULT	\
   __TMS_USBHID_HOST_DATA_SIZE_DEFAULT
# define UPROTO_USBHID_CLIENT_METHODS_BEGIN	\
   __TMS_UPROTO_USBHID_CLIENT_METHODS_BEGIN
# define UPROTO_USBHID_CLIENT_METHODS_END	\
   __TMS_UPROTO_USBHID_CLIENT_METHODS_END
# define UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS_V1(		\
		Attach,							\
		Detach,							\
		Configure,						\
		Unconfigure,						\
		Suspend,						\
		Resume,							\
		GetNextReport,						\
		GetReport,						\
		SetReport,						\
		GetIdle,						\
		SetIdle,						\
		SetProtocol,						\
		GetReportDescriptor,					\
		SetReportDescriptor,					\
		GetPhysicalDescriptor,					\
		SetPhysicalDescriptor,					\
		GetMiscellaneous,					\
		SetMiscellaneous,					\
		EventResponse						\
		)	\
	__TMS_UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS_V1(		\
		Attach,							\
		Detach,							\
		Configure,						\
		Unconfigure,						\
		Suspend,						\
		Resume,							\
		GetNextReport,						\
		GetReport,						\
		SetReport,						\
		GetIdle,						\
		SetIdle,						\
		SetProtocol,						\
		GetReportDescriptor,					\
		SetReportDescriptor,					\
		GetPhysicalDescriptor,					\
		SetPhysicalDescriptor,					\
		GetMiscellaneous,					\
		SetMiscellaneous,					\
		EventResponse						\
		)
# define UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS(Prefix)	\
   __TMS_UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS(Prefix)
# define UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS_STD(Prefix)	\
   __TMS_UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS_STD(Prefix)
# define UPROTO_USBHID_CLIENT_METHODS_INIT_V1(	\
		Attach,					\
		Detach,					\
		Configure,				\
		Unconfigure,				\
		Suspend,				\
		Resume,					\
		GetNextReport,				\
		GetReport,				\
		SetReport,				\
		GetIdle,				\
		SetIdle,				\
		SetProtocol,				\
		GetReportDescriptor,			\
		SetReportDescriptor,			\
		GetPhysicalDescriptor,			\
		SetPhysicalDescriptor,			\
		GetMiscellaneous,			\
		SetMiscellaneous,			\
		EventResponse				\
	)	\
	__TMS_UPROTO_USBHID_CLIENT_METHODS_INIT_V1(	\
		Attach,					\
		Detach,					\
		Configure,				\
		Unconfigure,				\
		Suspend,				\
		Resume,					\
		GetNextReport,				\
		GetReport,				\
		SetReport,				\
		GetIdle,				\
		SetIdle,				\
		SetProtocol,				\
		GetReportDescriptor,			\
		SetReportDescriptor,			\
		GetPhysicalDescriptor,			\
		SetPhysicalDescriptor,			\
		GetMiscellaneous,			\
		SetMiscellaneous,			\
		EventResponse				\
	)
# define UPROTO_USBHID_PUBLIC_METHODS_BEGIN	\
   __TMS_UPROTO_USBHID_PUBLIC_METHODS_BEGIN
# define UPROTO_USBHID_PUBLIC_METHODS_END	\
   __TMS_UPROTO_USBHID_PUBLIC_METHODS_END
# define UPROTO_USBHID_PUBLIC_METHODS_DECLARE_FNS_V1(		\
		QueueForOutReport,					\
		QueueInReport,						\
		NotifyEvent						\
		)	\
	__TMS_UPROTO_USBHID_PUBLIC_METHODS_DECLARE_FNS_V1(		\
		QueueForOutReport,					\
		QueueInReport,						\
		NotifyEvent						\
		)
# define UPROTO_USBHID_PUBLIC_METHODS_DECLARE_FNS_STD(Prefix)	\
   __TMS_UPROTO_USBHID_PUBLIC_METHODS_DECLARE_FNS_STD(Prefix)
# define UPROTO_USBHID_PUBLIC_METHODS_INIT_V2(		\
		QueueForOutReport,				\
		QueueForOutReportV2,				\
		QueueInReport,					\
		NotifyEvent					\
		)	\
	__TMS_UPROTO_USBHID_PUBLIC_METHODS_INIT_V2(		\
		QueueForOutReport,				\
		QueueForOutReportV2,				\
		QueueInReport,					\
		NotifyEvent					\
		)
# define UPROTO_USBHID_PUBLIC_METHODS_INIT_V1(		\
		QueueForOutReport,				\
		QueueInReport,					\
		NotifyEvent					\
		)	\
	__TMS_UPROTO_USBHID_PUBLIC_METHODS_INIT_V1(		\
		QueueForOutReport,				\
		QueueInReport,					\
		NotifyEvent					\
		)
# define UPROTO_USBHID_PUBLIC_METHODS_INIT_STD(Prefix)	\
   __TMS_UPROTO_USBHID_PUBLIC_METHODS_INIT_STD(Prefix)
# define UPROTO_USBHID_PUBLIC_METHODS_DECLARE_STD(Prefix)	\
   __TMS_UPROTO_USBHID_PUBLIC_METHODS_DECLARE_STD(Prefix)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of protohid.h ****/
#endif /* _PROTOHID_H_ */
