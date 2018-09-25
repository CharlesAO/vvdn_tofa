/* wmcimpl.h	Fri Jun 04 2004 14:18:20 tmm */

/*

Module:  wmcimpl.h

Function:
	WMC implementation-level functions and interfaces.

Version:
	V1.89a	Fri Jun 04 2004 14:18:20 tmm	Edit level 3

Copyright notice:
	This file copyright (C) 2002-2004 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Sat Jul  6 2002 11:06:45  tmm
	Module created.

   1.89a  Fri Jun 04 2004 14:18:20  tmm
	Add WmcTAI_ReNotifyEncapsulatedQueues() prototype.

*/

#ifndef _WMCIMPL_H_		/* prevent multiple includes */
#define _WMCIMPL_H_

#ifndef _PROTOWMC_H_
# include "protowmc.h"
#endif

#ifndef _USBPUMPDEBUG_H_
# include "usbpumpdebug.h"
#endif

#ifndef _USBPUMPAPI_H_
# include "usbpumpapi.h"
#endif

#ifndef _USBPUMPLIB_H_
# include "usbpumplib.h"
#endif

#ifndef _UDATAPLANE_H_
# include "udataplane.h"
#endif


__TMS_BEGIN_DECLS

BOOL 
WmcTA_Attach(
	UPROTO_WMCTA *pWmc,
	UDEVICE *pDevice,
	UINTERFACE *pIfc,
	CONST UPROTO_WMCTA_CONFIG *pConfig,
	CONST USBPUMP_PROTOCOL_INIT_NODE *,
	USBPUMP_OBJECT_HEADER * /* pProtoInitContext */
	);

extern CONST UDATAPLANE_OUTSWITCH gk_WmcTA_ControlPlaneOutSwitch;

BOOL 
WmcTA_CreateSubClass(
	UPROTO_WMCTA *pWmc,
	CONST UPROTO_WMCTA_CONFIG *pConfig
	);

extern CONST UDATAPLANE_OUTSWITCH gk_WmcTA_DataPlaneOutSwitch;

BOOL
WmcTA_DeactivateSubClass(
	UPROTO_WMCTA *pWmc
	);

VOID
WmcTA_EncapsQueueCancelIn(
	UPROTO_WMCTA *pWmc,
	USTAT Why
	);

VOID
WmcTA_EncapsQueueCancelOut(
	UPROTO_WMCTA *pWmc,
	USTAT Why
	);

VOID
WmcTA_EncapsQueueIn(
	UPROTO_WMCTA *pWmc,
	UBUFQE *pQe
	);

VOID
WmcTA_EncapsQueueOut(
	UPROTO_WMCTA *pWmc,
	UBUFQE *pQe
	);

BOOL
WmcTA_ResponseAvailable(
	UPROTO_WMCTA *pWmc,
        UINT16  wValue
        );

/*
|| Functions for use only by WMC implementation functions.
*/
VOID 
WmcTAI_EncapsQueueCancel(
	UPROTO_WMCTA *pWmc,
	UBUFQE **ppQueueHead,
	USB_RING_BUFFER *pRingBuffer,
	USTAT Status
	);

VOID
WmcTAI_PushEncapsulatedQueues(
	UPROTO_WMCTA *pWmc
	);

VOID
WmcTAI_ReNotifyEncapsulatedQueues(
	UPROTO_WMCTA *pWmc
	);

__TMS_END_DECLS

/**** end of wmcimpl.h ****/
#endif /* _WMCIMPL_H_ */
