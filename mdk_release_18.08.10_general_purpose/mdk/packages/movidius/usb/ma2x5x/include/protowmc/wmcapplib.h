/* wmcapplib.h	Fri Aug 07 2009 12:18:20 chwon */

/*

Module:  wmcapplib.h

Function:
	Header file that puts together the necessary things for this
	application.

Version:
	V2.01a	Fri Aug 07 2009 12:18:20 chwon	Edit level 12

Copyright notice:
	This file copyright (C) 2002-2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Sun Jul 28 2002 13:46:21  tmm
	Module created.

   1.79b  Tue Oct 22 2002 11:30:00 maw
   	Updated with separate buffersize macros for modem, obex and devmgmt

   1.79b  Sun Nov 24 2002 15:30:00 maw
   	Corrected typos in macros

   1.83e  Mon Apr 07 2003 10:33:16  chwon
   	Module created from app/wmcdemo/i/wmcdemo_osnone.h.

   1.87b  Wed May 05 2004 13:11:49 maw
   	Added variable for synchronized data-rx-tx-done handling

   1.91b  Thu Mar 03 2005 09:54:54  chwon
   	Added DIAG subclass

   1.97f  Thu Sep 07 2006 15:13:40  chwon
	1943: Change UBUFQE buffer size from 256 to 512.
	Add Rx and Tx UBUFQE list head pointer.

   1.97f  Mon Sep 18 2006 11:54:08  chwon
	Add CtrlQeBufferSize & DataQeBufferSize for high speed. We will allocate
	large buffer for UBUFQE if device support high speed.

   1.97i  Thu Jun 07 2007 15:54:01  chwon
	Add Rx and Tx completion event for obex, devmgmt and diag

   1.97k  Sun May 04 2008 09:53:06  chwon
	3735: reorganize UBUFQE to allow 32-bit sizes

   2.01a  Wed Apr 01 2009 13:52:32  maw
   	2910: Added command parser create fn

   2.01a  Fri Aug 07 2009 12:18:20  chwon
	8672: Added WmcDemo_CreateAtCmd() prototype

*/

#ifndef _WMCAPPLIB_H_		/* prevent multiple includes */
#define _WMCAPPLIB_H_

#include "usbpumpenv.h"
#include "usbpumpobject.h"
#include "ucallback.h"
#include "cdclinecoding.h"

/****************************************************************************\
|
|	parameters
|
\****************************************************************************/

#ifndef WMCDEMO_CTRL_RX_NUM_UBUFQES	/*PARAM*/
# define WMCDEMO_CTRL_RX_NUM_UBUFQES	2
#endif
#ifndef WMCDEMO_CTRL_TX_NUM_UBUFQES	/*PARAM*/
# define WMCDEMO_CTRL_TX_NUM_UBUFQES	2
#endif
#ifndef WMCDEMO_DATA_RX_NUM_UBUFQES	/*PARAM*/
# define WMCDEMO_DATA_RX_NUM_UBUFQES	6
#endif
#ifndef WMCDEMO_DATA_TX_NUM_UBUFQES	/*PARAM*/
# define WMCDEMO_DATA_TX_NUM_UBUFQES	6
#endif

#define WMCDEMO_MODEM_CTRL_RX_NUM_UBUFQES	WMCDEMO_CTRL_RX_NUM_UBUFQES
#define WMCDEMO_MODEM_CTRL_TX_NUM_UBUFQES	WMCDEMO_CTRL_TX_NUM_UBUFQES
#define WMCDEMO_MODEM_DATA_RX_NUM_UBUFQES	WMCDEMO_DATA_RX_NUM_UBUFQES
#define WMCDEMO_MODEM_DATA_TX_NUM_UBUFQES	WMCDEMO_DATA_TX_NUM_UBUFQES

#define WMCDEMO_OBEX_CTRL_RX_NUM_UBUFQES	WMCDEMO_CTRL_RX_NUM_UBUFQES
#define WMCDEMO_OBEX_CTRL_TX_NUM_UBUFQES	WMCDEMO_CTRL_TX_NUM_UBUFQES
#define WMCDEMO_OBEX_DATA_RX_NUM_UBUFQES	WMCDEMO_DATA_RX_NUM_UBUFQES
#define WMCDEMO_OBEX_DATA_TX_NUM_UBUFQES	WMCDEMO_DATA_TX_NUM_UBUFQES

#define WMCDEMO_DEVMGMT_CTRL_RX_NUM_UBUFQES	WMCDEMO_CTRL_RX_NUM_UBUFQES
#define WMCDEMO_DEVMGMT_CTRL_TX_NUM_UBUFQES	WMCDEMO_CTRL_TX_NUM_UBUFQES
#define WMCDEMO_DEVMGMT_DATA_RX_NUM_UBUFQES	WMCDEMO_DATA_RX_NUM_UBUFQES
#define WMCDEMO_DEVMGMT_DATA_TX_NUM_UBUFQES	WMCDEMO_DATA_TX_NUM_UBUFQES

#define WMCDEMO_DIAG_CTRL_RX_NUM_UBUFQES	WMCDEMO_CTRL_RX_NUM_UBUFQES
#define WMCDEMO_DIAG_CTRL_TX_NUM_UBUFQES	WMCDEMO_CTRL_TX_NUM_UBUFQES
#define WMCDEMO_DIAG_DATA_RX_NUM_UBUFQES	WMCDEMO_DATA_RX_NUM_UBUFQES
#define WMCDEMO_DIAG_DATA_TX_NUM_UBUFQES	WMCDEMO_DATA_TX_NUM_UBUFQES

/**** for each function ****/
#define WMCDEMO_MODEM_NUM_UBUFQES	(WMCDEMO_MODEM_CTRL_RX_NUM_UBUFQES + \
					 WMCDEMO_MODEM_CTRL_TX_NUM_UBUFQES + \
					 WMCDEMO_MODEM_DATA_RX_NUM_UBUFQES + \
					 WMCDEMO_MODEM_DATA_TX_NUM_UBUFQES)

#define WMCDEMO_OBEX_NUM_UBUFQES	(WMCDEMO_OBEX_DATA_RX_NUM_UBUFQES + \
					 WMCDEMO_OBEX_DATA_TX_NUM_UBUFQES)

#define WMCDEMO_DEVMGMT_NUM_UBUFQES	(WMCDEMO_DEVMGMT_CTRL_RX_NUM_UBUFQES + \
					 WMCDEMO_DEVMGMT_CTRL_TX_NUM_UBUFQES)

#define WMCDEMO_DIAG_NUM_UBUFQES	(WMCDEMO_DIAG_DATA_RX_NUM_UBUFQES + \
					 WMCDEMO_DIAG_DATA_TX_NUM_UBUFQES)

/*
|| Buffer sizes
*/
#ifndef WMCDEMO_QEBUFFER_SIZE		/*PARAM*/
# define WMCDEMO_QEBUFFER_SIZE		512
#endif

#define	WMCDEMO_MODEM_QEBUFFER_SIZE	WMCDEMO_QEBUFFER_SIZE
#define	WMCDEMO_OBEX_QEBUFFER_SIZE	WMCDEMO_QEBUFFER_SIZE
#define	WMCDEMO_DEVMGMT_QEBUFFER_SIZE	WMCDEMO_QEBUFFER_SIZE
#define	WMCDEMO_DIAG_QEBUFFER_SIZE	WMCDEMO_QEBUFFER_SIZE

/****************************************************************************\
|
|	The main instance data
|
\****************************************************************************/

/*
|| Conveniently, we can use one primary data structure for handling
|| the interface to the lower level.  Because of the way things work, we
|| can just record the configuration of the lower layers, and forward things
|| to the appropriate application engine:  either an AT interpreter or
|| an OBEX engine.
||
|| When running in a real OS, this structure will probably be used without
|| typecloaking, so we do it simply.
*/
__TMS_TYPE_DEF_STRUCT(WMCDEMO_DEVOBJ);
struct __TMS_STRUCTNAME(WMCDEMO_DEVOBJ)
	{
	/*
	|| For convenience, and so we can recieve IOCTLs, we need
	|| an object header.
	*/
	__TMS_USBPUMP_OBJECT_HEADER	Header;

	/*
	|| We need someplace to store the "object handle"
	*/
	__TMS_PUSBPUMP_OBJECT_HEADER	pIoObject;

	/*
	|| For allocations, we really need a pDevice and a pPlatform
	*/
	__TMS_PUDEVICE			pDevice;
	__TMS_PUPLATFORM		pPlatform;
	};

#define	WMCDEMO_DEVOBJ_TAG_MODEM	__TMS_UHIL_MEMTAG('M','o','d','m')
#define	WMCDEMO_DEVOBJ_TAG_OBEX		__TMS_UHIL_MEMTAG('O','b','e','x')
#define	WMCDEMO_DEVOBJ_TAG_DEVMGMT	__TMS_UHIL_MEMTAG('M','g','m','t')
#define	WMCDEMO_DEVOBJ_TAG_DIAG		__TMS_UHIL_MEMTAG('D','i','a','g')

#define	WMCDEMO_DEVOBJ_NAME_MODEM	"modem.osdriver.mcci.com"
#define	WMCDEMO_DEVOBJ_NAME_OBEX	"obex.osdriver.mcci.com"
#define	WMCDEMO_DEVOBJ_NAME_DEVMGMT	"device-management.osdriver.mcci.com"
#define	WMCDEMO_DEVOBJ_NAME_DIAG	"diag.osdriver.mcci.com"

/****************************************************************************\
|
|	The MODEM structure
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(WMCDEMO_DEVOBJ_MODEM);

__TMS_FNTYPE_DEF(
	WMCDEMODATARXQFN,
	__TMS_VOID,
		(
		__TMS_WMCDEMO_DEVOBJ_MODEM *,
		__TMS_UBUFQE *
		)
	);

struct __TMS_STRUCTNAME(WMCDEMO_DEVOBJ_MODEM)
	{
	__TMS_WMCDEMO_DEVOBJ		DevObj;

	/* info for the modem implementation */

	/* UBUFQE buffer size */
	__TMS_BYTES			CtrlQeBufferSize;	/* 3735 */
	__TMS_BYTES			DataQeBufferSize;	/* 3735 */

	/* some parking queues */
	__TMS_PUBUFQE			pCtrlParkQueue;
	__TMS_PUBUFQE			pDataParkQueue;

	/* is break on? */
	__TMS_BOOL			fBreak;

	/* current control state */
	__TMS_UINT			ControlLines;

	/* the current line coding */
	__TMS_USB_Comm_LINE_CODING	LineCoding;

	/* the current abstract multiplexing state */
	__TMS_BOOL			fCmdsInData;

	/* the curent idle state */
	__TMS_BOOL			fIdle;

	/* the current country code */
	__TMS_UINT			CountryCode;

	/* Have we been activated? */
	__TMS_BOOL			fUp;

	/* pointer of port context */
	__TMS_PVOID			pPortCtx;

	/* WMCDEMO interface functions */
	__TMS_PUBUFIODONEFN		pCtrlRxDone;
	__TMS_PUBUFIODONEFN		pDataRxDone;
	__TMS_PUBUFIODONEFN		pCtrlTxDone;
	__TMS_PUBUFIODONEFN		pDataTxDone;
	__TMS_PWMCDEMODATARXQFN		pDataRxQueue;

	__TMS_UCALLBACKCOMPLETION	pDataRxDoneCompletion;
	__TMS_UCALLBACKCOMPLETION	pDataTxDoneCompletion;

	/* Rx and Tx completion UBUFQE list */
	__TMS_PUBUFQE			pDataRxCompleteQe;
	__TMS_PUBUFQE			pDataTxCompleteQe;
	};


/****************************************************************************\
|
|	The WMC Device Management object
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(WMCDEMO_DEVOBJ_DEVMGMT);
struct __TMS_STRUCTNAME(WMCDEMO_DEVOBJ_DEVMGMT)
	{
	__TMS_WMCDEMO_DEVOBJ		DevObj;

	/* info for the modem implementation */

	/* UBUFQE buffer size */
	__TMS_BYTES			CtrlQeBufferSize;	/* 3735 */
	__TMS_BYTES			DataQeBufferSize;	/* 3735 */

	/* some parking queues */
	__TMS_PUBUFQE			pCtrlParkQueue;
	__TMS_PUBUFQE			pDataParkQueue;

	/* Rx and Tx completion event */
	__TMS_UCALLBACKCOMPLETION	DataRxDoneCompletion;
	__TMS_UCALLBACKCOMPLETION	DataTxDoneCompletion;

	/* Rx and Tx completion UBUFQE list */
	__TMS_PUBUFQE			pDataRxCompleteQe;
	__TMS_PUBUFQE			pDataTxCompleteQe;
	};

/****************************************************************************\
|
|	The WMC OBEX object
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(WMCDEMO_DEVOBJ_OBEX);
struct __TMS_STRUCTNAME(WMCDEMO_DEVOBJ_OBEX)
	{
	__TMS_WMCDEMO_DEVOBJ		DevObj;

	/* info for the modem implementation */

	/* UBUFQE buffer size */
	__TMS_BYTES			CtrlQeBufferSize;	/* 3735 */
	__TMS_BYTES			DataQeBufferSize;	/* 3735 */

	/* some parking queues */
	__TMS_PUBUFQE			pDataParkQueue;

	/* Rx and Tx completion event */
	__TMS_UCALLBACKCOMPLETION	DataRxDoneCompletion;
	__TMS_UCALLBACKCOMPLETION	DataTxDoneCompletion;

	/* Rx and Tx completion UBUFQE list */
	__TMS_PUBUFQE			pDataRxCompleteQe;
	__TMS_PUBUFQE			pDataTxCompleteQe;
	};

/****************************************************************************\
|
|	The WMC DIAG object
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(WMCDEMO_DEVOBJ_DIAG);
struct __TMS_STRUCTNAME(WMCDEMO_DEVOBJ_DIAG)
	{
	__TMS_WMCDEMO_DEVOBJ		DevObj;

	/* info for the modem implementation */

	/* UBUFQE buffer size */
	__TMS_BYTES			CtrlQeBufferSize;	/* 3735 */
	__TMS_BYTES			DataQeBufferSize;	/* 3735 */

	/* some parking queues */
	__TMS_PUBUFQE			pDataParkQueue;

	/* Rx and Tx completion event */
	__TMS_UCALLBACKCOMPLETION	DataRxDoneCompletion;
	__TMS_UCALLBACKCOMPLETION	DataTxDoneCompletion;

	/* Rx and Tx completion UBUFQE list */
	__TMS_PUBUFQE			pDataRxCompleteQe;
	__TMS_PUBUFQE			pDataTxCompleteQe;
	};


/****************************************************************************\
|
|	The functions.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL WmcDemo_CreateModem(
		__TMS_PUPLATFORM		pPlatform,
		__TMS_PUSBPUMP_OBJECT_HEADER	pFunctionObject,
		__TMS_PWMCDEMO_DEVOBJ_MODEM *	ppModem
		);

__TMS_BOOL WmcDemo_CreateModem_2p(
		__TMS_PUPLATFORM		pPlatform,
		__TMS_PUSBPUMP_OBJECT_HEADER	pFunctionObject,
		__TMS_PWMCDEMO_DEVOBJ_MODEM *	ppModem
		);

__TMS_BOOL WmcDemo_CreateObex(
		__TMS_PUPLATFORM		pPlatform,
		__TMS_PUSBPUMP_OBJECT_HEADER	pFunctionObject,
		__TMS_PWMCDEMO_DEVOBJ_OBEX *	ppObex
		);

__TMS_BOOL WmcDemo_CreateDevMgmt(
		__TMS_PUPLATFORM		pPlatform,
		__TMS_PUSBPUMP_OBJECT_HEADER	pFunctionObject,
		__TMS_PWMCDEMO_DEVOBJ_DEVMGMT *	ppDevMgmt
		);

__TMS_BOOL WmcDemo_CreateDiag(
		__TMS_PUPLATFORM		pPlatform,
		__TMS_PUSBPUMP_OBJECT_HEADER	pFunctionObject,
		__TMS_PWMCDEMO_DEVOBJ_DIAG *	ppDiag
		);

__TMS_BOOL WmcDemoCmd_Create(
		__TMS_PUPLATFORM		pPlatform,
		__TMS_PUSBPUMP_OBJECT_HEADER	pObject
		);

__TMS_BOOL WmcDemo_CreateAtCmd(
		__TMS_UPLATFORM *		pPlatform,
		__TMS_USBPUMP_OBJECT_HEADER *	pFunctionObject,
		__TMS_PWMCDEMO_DEVOBJ *		ppAtCmdObject
		);

__TMS_END_DECLS

/**** end of wmcapplib.h ****/
#endif /* _WMCAPPLIB_H_ */
