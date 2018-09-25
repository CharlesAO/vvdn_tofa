/* pndfuimpl.h	Wed Jun 09 2004 18:26:14 maw */

/*

Module:  pndfuimpl.h

Function:
	NDFU protocol implementation definitions.

Version:
	V1.89a	Wed Jun 09 2004 18:26:14 maw	Edit level 1

Copyright notice:
	This file copyright (C) 2004 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Mats Webjörn, MCCI Corporation	June 2004

Revision history:
   1.89a  Wed Jun 09 2004 18:26:14 maw
	Module created from pdfuimpl.c V1.63b

*/

#ifndef _PNDFUIMPL_H_		/* prevent multiple includes */
#define _PNDFUIMPL_H_

#include "protondfu.h"

/**** the internal functions ****/

__TMS_USBPUMP_OBJECT_IOCTL_FN	NDfuI_Ioctl;

__TMS_BOOL NDfuI_EdgeActivate(
	__TMS_PUPROTO_NDFU	pSelf
	);
	
__TMS_BOOL NDfuI_EdgeDeactivate(
	__TMS_PUPROTO_NDFU	pSelf
	);
	
__TMS_BOOL NDfuI_EdgeBusEvent(
	__TMS_PUPROTO_NDFU	pSelf,
	__TMS_UEVENT		EventCode,
	__TMS_VOID *		pEventSpecificInfo	
	);
		
__TMS_BOOL NDfuI_EdgeStartTimer(
	__TMS_PUPROTO_NDFU	pSelf,
	__TMS_ULONG		TmoInMs,
	__TMS_ULONG		Id
	);
	
__TMS_BOOL NDfuI_EdgeOpenWrite(
	__TMS_PUPROTO_NDFU	pSelf
	);
	
__TMS_BOOL NDfuI_EdgeWrite(
	__TMS_PUPROTO_NDFU	pSelf,
	__TMS_USHORT 		nBytes,
	__TMS_VOID * 		pBuf
	);
	
__TMS_BOOL NDfuI_EdgeCloseWrite(
	__TMS_PUPROTO_NDFU	pSelf
	);
	
__TMS_BOOL NDfuI_EdgeManifest(
	__TMS_PUPROTO_NDFU	pSelf
	);
	
__TMS_BOOL NDfuI_EdgeOpenRead(
	__TMS_PUPROTO_NDFU	pSelf
	);
	
__TMS_BOOL NDfuI_EdgeRead(
	__TMS_PUPROTO_NDFU	pSelf
	);

__TMS_BOOL NDfuI_EdgeCloseRead(
	__TMS_PUPROTO_NDFU	pSelf
	);
	
__TMS_BOOL NDfuI_EdgeStatus(
	__TMS_PUPROTO_NDFU	pSelf
	);
	
__TMS_BOOL NDfuI_EdgeReset(
	__TMS_PUPROTO_NDFU	pSelf
	);

__TMS_BOOL NDfuI_EdgeRestart(
	__TMS_PUPROTO_NDFU			pSelf,
	__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE	DeviceMode
	);

__TMS_BOOL NDfuI_Attach(
	__TMS_PUPROTO_NDFU			pSelf,
	__TMS_UDEVICE *				pDevice,
	__TMS_UINTERFACE *			pInterface,
	__TMS_CONST __TMS_UPROTO_NDFU_CONFIG *	pConfig
	);

__TMS_UINT32 NDfuI_Crc(
	__TMS_CONST __TMS_UCHAR *	pBuf,
	__TMS_BYTES			Length,
	__TMS_UINT32			CrcAcc
	);

__TMS_BOOL NDfuI_Error(
	__TMS_PUPROTO_NDFU	pSelf,
	__TMS_UCHAR		Status
	);

__TMS_VOID 
NDfuI_GetStatusReply(
	__TMS_PUPROTO_NDFU	pSelf,
	__TMS_ULONG		PollTmo
	);

__TMS_VOID 
NDfuI_GetStateReply(
	__TMS_PUPROTO_NDFU	pSelf
	);
	
__TMS_VOID NDfuI_GetStatusReplyWithCallback(
	__TMS_PUPROTO_NDFU	pSelf,
	__TMS_ULONG		PollTmo,
	__TMS_PUBUFIODONEFN	pDoneFn, 
	__TMS_PVOID		pDoneCtx,
	__TMS_UBUFQE_FLAGS	Flags
	);

__TMS_UEVENTFN	NDfuI_InterfaceEvent;

__TMS_VOID NDfuI_GotoState(
	__TMS_PUPROTO_NDFU	pSelf, 
	__TMS_UCHAR		NewState
	);

__TMS_VOID NDfuI_SetState(
	__TMS_PUPROTO_NDFU	pSelf, 
	__TMS_UCHAR 		NewState
	);

__TMS_BOOL NDfuI_ValidateRequest(
	__TMS_PUPROTO_NDFU	pSelf
	);

/**** end of pndfuimpl.h ****/
#endif /* _PNDFUIMPL_H_ */
