/* mscimpl.h	Thu Oct 04 2007 16:48:22 chwon */

/*

Module:  mscimpl.h

Function:
	MSC implementation-level functions and interfaces.

Version:
	V1.97j	Thu Oct 04 2007 16:48:22 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2002-2007 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Mats Webjörn, MCCI Corporation	Oct 2002

Revision history:
   1.79b  Fri Oct 18 2002 12:00:00  maw
	Module created.

   1.85e  Wed Sep 24 2003 13:00:25  chwon
	Added MscBOT_DisableInterface() prototype.

   1.89c  Wed Oct 13 2004 11:59:48  maw
	Bug 1343: Added MscBOT_ResetInterface() and MscBOT_QueueCBW()
	prototype.

   1.97f  Wed Sep 13 2006 11:32:43  chwon
	Change MSC use data plane and data stream

   1.97j  Thu Oct 04 2007 16:48:22  chwon
	3253: Remove MscBOT_InterfaceEvent() and MscBOT_DeviceEvent() prototype.

*/

#ifndef _MSCIMPL_H_		/* prevent multiple includes */
#define _MSCIMPL_H_

#ifndef _PROTOMSC_H_
# include "protomsc.h"
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

#ifndef _PROTOMSC_H_
# include "protomsc.h"
#endif

/****************************************************************************\
|
|	Internal functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_CONST __TMS_UDATAPLANE_OUTSWITCH
		gk_UsbPumpMscBOT_DataPlaneOutSwitch;

__TMS_BOOL MscBOT_Init(
		__TMS_UPROTO_MSCBOT *,
		__TMS_UDEVICE *
		);

__TMS_VOID MscBOT_EnableInterface(
		__TMS_UPROTO_MSCBOT *
		);

__TMS_VOID MscBOT_DisableInterface(
		__TMS_UPROTO_MSCBOT *
		);

__TMS_VOID MscBOT_ResetInterface(
		__TMS_UPROTO_MSCBOT *
		);

__TMS_UBUFIODONEFN MscBOT_RxCompleteHandler;

__TMS_UBUFIODONEFN MscBOT_TxCompleteHandler;

/* internal state machine */
__TMS_VOID MscBOT_ProcessEvent(
		__TMS_UPROTO_MSCBOT *			pMsc,
		__TMS_UINT32 				event,
		__TMS_UPROTO_MSCBOT_BUFQE_WRAPPER *	pMscQeW
		);

/* Queue buffer to out-ep for receiving a CBW */
__TMS_VOID MscBOT_QueueCBW(
		__TMS_UPROTO_MSCBOT *			pMsc
		);

__TMS_END_DECLS

/**** end of mscimpl.h ****/

#endif /* _MSCIMPL_H_ */
