/* usbpump_hcdkit_switch.h	Wed Jan 14 2015 16:06:56 luxw */

/*

Module:  usbpump_hcdkit_switch.h

Function:
	Linkage from HCDKIT to the HCD's hardware-specific routines

Version:
	V3.13b	Wed Jan 14 2015 16:06:56 luxw	Edit level 7

Copyright notice:
	This file copyright (C) 2005-2006, 2009, 2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Wed Jun  8 2005 17:55:53  tmm
	Module created.

   1.93e  Fri Jan 06 2006 11:09:40  chwon
	Change USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_PORT_FEATURE_FN intput
	parameter type to match with others.

   1.93e  Fri Jan 06 2006 11:09:40  chwon
	Change USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_PORT_FEATURE_FN intput
	parameter type to match with others.

   1.93e  Thu Mar 02 2006 14:51:25 ssyang
	Add UsbPumpHcdKitI_vRootNotifyStatusChange into USBPUMP_HCDKIT_SWITCH

   2.01a  Fri May 15 2009 15:49:38  chwon
	7701: Add USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_ERROR_COUNT_FN into
	USBPUMP_HCDKIT_SWITCH.

   2.01a  Mon Nov 16 2009 12:21:20  chwon
	9172: Added pRootHub input parameter in the HCDKIT switch function

   2.01a  Mon Dec 14 2009 14:20:51  chwon
	9569: Add USBPUMP_HCDKIT_SWITCH_ROOT_LPM_REQUEST_FN

   3.11c  Thu Mar 14 2013 13:37:27  chwon
	16861: Added USBPUMP_HCDKIT_SWITCH_OTG structure and init macro.

   3.13b  Wed Jan 14 2015 16:06:56  luxw
	18571: add USBPUMP_HCDKIT_PROCESS_HOLDING_FRAME_SPINLOCK_FN.

*/

#ifndef _USBPUMP_HCDKIT_SWITCH_H_		/* prevent multiple includes */
#define _USBPUMP_HCDKIT_SWITCH_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBPUMP_HCDKIT_TYPES_H_
# include "usbpump_hcdkit_types.h"
#endif

#ifndef _USBPUMP_HCD_TYPES_H_
# include "usbpump_hcd_types.h"
#endif

/*

Type:	USBPUMP_HCDKIT_SWITCH

Index:	Type:	USBPUMP_HCDKIT_SWITCH_IOCTL_FN
	Type:	USBPUMP_HCDKIT_SWITCH_VALIDATE_REQUEST_FN
	Type:	USBPUMP_HCDKIT_SWITCH_SUBMIT_REQUEST_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_GET_DESC_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_SET_HUB_FEATURE_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_HUB_FEATURE_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_SET_PORT_FEATURE_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_PORT_FEATURE_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_GET_HUB_STATUS_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_STATUS_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_SUBMIT_STATUS_READ_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_TT_REQUEST_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_NOTIFY_STATUS_CHANGE_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_ERROR_COUNT_FN
	Type:	USBPUMP_HCDKIT_SWITCH_ROOT_LPM_REQUEST_FN

Function:
	A structure that collects the hardware-specific methods for a
	given HCDKIT implementation into a central place

Description:
	The HCDKIT code interfaces to the device-specific code by
	calling standardized methods out of this table.  Most of the methods
	serve specific purposes; but one, the IOCTL method, can be used
	as a general purpose expansion for low-frequency control paths.

Contents:
	typedef USBPUMP_IOCTL_RESULT 
		USBPUMP_HCDKIT_SWITCH_IOCTL_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_IOCTL_CODE RequestCode,
			CONST VOID *pInParam,
			VOID *pOutParam
			);

	USBPUMP_HCDKIT_SWITCH_IOCTL_FN *pIoctlFn;
		This function is called to allow the hw-specific half of the
		HCD driver to process IOCTLs.  Since it's called from the
		upper half of the HCD driver, the type of the parameter is
		the upper HCD object.

	typedef USTAT
		USBPUMP_HCDKIT_SWITCH_VALIDATE_REQUEST_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCD_REQUEST *pHcdRequest
			);

	USBPUMP_HCDKIT_SWITCH_VALIDATE_REQUEST_FN *pValidateRequestFn;
		This function is called to validate USBPUMP_HCD_REQUEST.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_SUBMIT_REQUEST_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCD_REQUEST *pHcdRequest
			);

	USBPUMP_HCDKIT_SWITCH_SUBMIT_REQUEST_FN *pSubmitRequestFn;
		This function is called to submit USBPUMP_HCD_REQUEST.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_GET_DESC_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub,
			USBPUMP_HCD_REQUEST *pHcdRequest,
			VOID *pBuffer,
			BYTES nByffer
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_GET_DESC_FN *pRootGetDescriptorFn;
		This function is called to get descriptor from root hub device.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_SET_HUB_FEATURE_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub,
			USBPUMP_HCD_REQUEST *pHcdRequest,
			ARG_UINT16 uFeature
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_SET_HUB_FEATURE_FN *pRootSetHubFeatureFn;
		This function is called to set hub feature for root hub device.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_HUB_FEATURE_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub,
			USBPUMP_HCD_REQUEST *pHcdRequest,
			ARG_UINT16 uFeature
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_HUB_FEATURE_FN *pRootClearHubFeatureFn;
		This function is called to clear hub feature for root hub device.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_SET_PORT_FEATURE_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub,
			USBPUMP_HCD_REQUEST *pHcdRequest,
			ARG_UINT16 uFeature,
			ARG_UINT8 uPort,
			ARG_UINT8 uSelector
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_SET_PORT_FEATURE_FN *pRootSetPortFeatureFn;
		This function is called to set port feature for root hub device.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_PORT_FEATURE_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub,
			USBPUMP_HCD_REQUEST *pHcdRequest,
			ARG_UINT16 uFeature,
			ARG_UINT8 uPort,
			ARG_UINT8 uSelector
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_PORT_FEATURE_FN *pRootClearPortFeatureFn;
		This function is called to clear port feature for root hub device.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_GET_HUB_STATUS_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub,
			USBPUMP_HCD_REQUEST *pHcdRequest,
			VOID * pBuffer
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_GET_HUB_STATUS_FN *pRootGetHubStatusFn;
		This function is called to get hub status from root hub device.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_STATUS_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub,
			USBPUMP_HCD_REQUEST *pHcdRequest,
			VOID * pBuffer,
			ARG_UINT8 uPort
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_STATUS_FN *pRootGetPortStatusFn;
		This function is called to get port status from root hub device.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_SUBMIT_STATUS_READ_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub,
			USBPUMP_HCD_REQUEST *pHcdRequest,
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_SUBMIT_STATUS_READ_FN *pRootSubmitStatusReadFn;
		This function is called to read status change from root hub
		device.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_TT_REQUEST_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub,
			USBPUMP_HCD_REQUEST *pHcdRequest,
			ARG_UINT8 uPort
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_TT_REQUEST_FN *pRootTtRequestFn;
		This function is called to send transaction translation request
		to the root hub device.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_NOTIFY_STATUS_CHANGE_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_NOTIFY_STATUS_CHANGE_FN *pRootNotifyStatusChangeFn;
		This function is called by HCD to notify status change.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_ERROR_COUNT_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub,
			USBPUMP_HCD_REQUEST *pHcdRequest,
			VOID * pBuffer,
			ARG_UINT8 uPort
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_ERROR_COUNT_FN *pRootGetPortErrorCountFn;
		This function is called to get port error count from root hub
		device.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_ROOT_LPM_REQUEST_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_HCDKIT_ROOTHUB *pRootHub,
			USBPUMP_HCD_REQUEST *pHcdRequest,
			BOOL fSetAndTestPortFeature,
			ARG_UINT8 uPort
			);

	USBPUMP_HCDKIT_SWITCH_ROOT_LPM_REQUEST_FN *pRootLpmRequestFn;
		This function is called to set and clear PORT_L1 state from root
		hub device.  If fSetAndTestPortFeature is TRUE, this request is
		"Set and Test port feature" request.  If fSetAndTestPortFeature
		is FALSE, it is "clear port feature PORT_L1".

Notes:
	None.

See Also:
	USBPUMP_HCDKIT_SWITCH_INIT_V3
	USBPUMP_HCDKIT_SWITCH_INIT_V2
	USBPUMP_HCDKIT_SWITCH_INIT_V1

*/

__TMS_USBPUMP_DECLARE_IOCTL_FNTYPE(
USBPUMP_HCDKIT_SWITCH_IOCTL_FN,
	__TMS_USBPUMP_HCDKIT_HCD *
	);

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_VALIDATE_REQUEST_FN,
__TMS_USTAT,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_SUBMIT_REQUEST_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_ROOT_GET_DESC_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */,
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */,
	__TMS_VOID *			/* pBuffer */,
	__TMS_BYTES			/* nBuffer */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_ROOT_SET_HUB_FEATURE_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */,
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */,
	__TMS_ARG_UINT16		/* uFeature */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_HUB_FEATURE_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */,
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */,
	__TMS_ARG_UINT16		/* uFeature */
	));


__TMS_FNTYPE_DEF(						\
USBPUMP_HCDKIT_SWITCH_ROOT_SET_PORT_FEATURE_FN,			\
__TMS_VOID,							\
	(							\
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,		\
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */,		\
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */,		\
	__TMS_ARG_UINT16		/* uFeature */,		\
	__TMS_ARG_UINT8			/* uPort */,		\
	__TMS_ARG_UINT8			/* uSelector */		\
	));

__TMS_FNTYPE_DEF(						\
USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_PORT_FEATURE_FN,		\
__TMS_VOID,							\
	(							\
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,		\
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */,		\
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */,		\
	__TMS_ARG_UINT16		/* uFeature */,		\
	__TMS_ARG_UINT8			/* uPort */,		\
	__TMS_ARG_UINT8			/* uSelector */		\
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_ROOT_GET_HUB_STATUS_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */,
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */,
	__TMS_VOID *			/* pBuffer[4] */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_STATUS_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */,
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */,
	__TMS_VOID *			/* pBuffer[4] */,
	__TMS_ARG_UINT8			/* uPort */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_ROOT_SUBMIT_STATUS_READ_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */,
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_ROOT_NOTIFY_STATUS_CHANGE_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_ROOT_TT_REQUEST_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */,
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */,
	__TMS_ARG_UINT8			/* uPort */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_ERROR_COUNT_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */,
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */,
	__TMS_VOID *			/* pBuffer[2] */,
	__TMS_ARG_UINT8			/* uPort */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_ROOT_LPM_REQUEST_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *	/* pRootHub */,
	__TMS_USBPUMP_HCD_REQUEST *	/* pRequest */,
	__TMS_BOOL			/* fSetAndTestPortFeature */,
	__TMS_ARG_UINT8			/* uPort */
	));
	
__TMS_TYPE_DEF_STRUCT(USBPUMP_HCD_CALCULATE_FRAME_CONTEXT);	
struct __TMS_STRUCTNAME	(USBPUMP_HCD_CALCULATE_FRAME_CONTEXT)
	{
	__TMS_UINT32	FrameNumber;
	__TMS_UINT32	MicroFrame;
	};
	
__TMS_FNTYPE_DEF(
USBPUMP_HCD_GET_FRAME_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCD *				/* pHcd */,
	__TMS_USBPUMP_HCD_CALCULATE_FRAME_CONTEXT *	/* pFrameCtx */
	));

__TMS_TYPE_DEF_FUNCTION(
USBPUMP_HCDKIT_PROCESS_HOLDING_FRAME_SPINLOCK_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCD *,
	__TMS_USBPUMP_HCD_GET_FRAME_FN *,
	__TMS_VOID * /*pFrameContext*/
	));

struct __TMS_STRUCTNAME	(USBPUMP_HCDKIT_SWITCH)
	{
	__TMS_USBPUMP_HCDKIT_SWITCH_IOCTL_FN *		pIoctlFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_VALIDATE_REQUEST_FN	*pValidateRequestFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_SUBMIT_REQUEST_FN *	pSubmitRequestFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_GET_DESC_FN *	pRootGetDescriptorFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_SET_HUB_FEATURE_FN *
							pRootSetHubFeatureFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_HUB_FEATURE_FN *
							pRootClearHubFeatureFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_SET_PORT_FEATURE_FN *
							pRootSetPortFeatureFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_PORT_FEATURE_FN *
							pRootClearPortFeatureFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_GET_HUB_STATUS_FN *
							pRootGetHubStatusFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_STATUS_FN *
							pRootGetPortStatusFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_SUBMIT_STATUS_READ_FN *
							pRootSubmitStatusReadFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_TT_REQUEST_FN *pRootTtRequestFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_NOTIFY_STATUS_CHANGE_FN *
							pRootNotifyStatusChangeFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_ERROR_COUNT_FN *
							pRootGetPortErrorCountFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_LPM_REQUEST_FN *
							pRootLpmRequestFn;
	__TMS_USBPUMP_HCDKIT_PROCESS_HOLDING_FRAME_SPINLOCK_FN *
							pProcessHoldingFrameSpinlockFn;	
	};
	
#define	__TMS_USBPUMP_HCDKIT_SWITCH_INIT_V4(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootNotifyStatusChangeFn,				\
	ARG_pRootTtRequestFn,					\
	ARG_pRootGetPortErrorCountFn,	/* optional */		\
	ARG_pRootLpmRequestFn,		/* optional */		\
	ARG_pProcessHoldingFrameSpinlockFn			\
	)							\
	{							\
	(ARG_pIoctlFn),						\
	(ARG_pValidateRequestFn),				\
	(ARG_pSubmitRequestFn),					\
	(ARG_pRootGetDescriptorFn),				\
	(ARG_pRootSetHubFeatureFn),				\
	(ARG_pRootClearHubFeatureFn),				\
	(ARG_pRootSetPortFeatureFn),				\
	(ARG_pRootClearPortFeatureFn),				\
	(ARG_pRootGetHubStatusFn),				\
	(ARG_pRootGetPortStatusFn),				\
	(ARG_pRootSubmitStatusReadFn),				\
	(ARG_pRootTtRequestFn),					\
	(ARG_pRootNotifyStatusChangeFn),			\
	(ARG_pRootGetPortErrorCountFn),				\
	(ARG_pRootLpmRequestFn),				\
	(ARG_pProcessHoldingFrameSpinlockFn)			\
	}

#define	__TMS_USBPUMP_HCDKIT_SWITCH_INIT_V3(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootNotifyStatusChangeFn,				\
	ARG_pRootTtRequestFn,					\
	ARG_pRootGetPortErrorCountFn,	/* optional */		\
	ARG_pRootLpmRequestFn		/* optional */		\
	)							\
	__TMS_USBPUMP_HCDKIT_SWITCH_INIT_V4(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootNotifyStatusChangeFn,				\
	ARG_pRootTtRequestFn,					\
	ARG_pRootGetPortErrorCountFn,	/* optional */		\
	ARG_pRootLpmRequestFn,		/* optional */		\
	/* V4: pProcessHoldingFrameSpinlockFn */ 		\
	UsbPumpHcdKitI_DefaultProcessHoldingFrameSpinLockFn	\
	)

#define	__TMS_USBPUMP_HCDKIT_SWITCH_INIT_V2(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootNotifyStatusChangeFn,				\
	ARG_pRootTtRequestFn					\
	)							\
	__TMS_USBPUMP_HCDKIT_SWITCH_INIT_V3(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootNotifyStatusChangeFn,				\
	ARG_pRootTtRequestFn,					\
	/* V3: pRootGetPortErrorCountFn */ __TMS_NULL,		\
	/* V3: pRootLpmRequestFn */ __TMS_NULL			\
	)

#define	__TMS_USBPUMP_HCDKIT_SWITCH_INIT_V1(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootTtRequestFn					\
	)							\
	__TMS_USBPUMP_HCDKIT_SWITCH_INIT_V2(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	/* V2: pRootNotifyStatusChangeFn */ __TMS_NULL,		\
	ARG_pRootTtRequestFn					\
	)

/*

Type:	USBPUMP_HCDKIT_SWITCH_OTG

Index:	Type:	USBPUMP_HCDKIT_SWITCH_IOCTL_FN
	Type:	USBPUMP_HCDKIT_SWITCH_GET_ID_STATE_FN
	Type:	USBPUMP_HCDKIT_SWITCH_START_HCD_FN
	Type:	USBPUMP_HCDKIT_SWITCH_STOP_HCD_FN

Function:
	A structure that collects the hardware-specific methods to support
	OTG for a given HCDKIT implementation into a central place

Description:
	The HCDKIT code interfaces to the device-specific code by
	calling standardized methods out of this table.  Most of the methods
	serve specific purposes; but one, the IOCTL method, can be used
	as a general purpose expansion for low-frequency control paths.

Contents:
	typedef USBPUMP_IOCTL_RESULT 
		USBPUMP_HCDKIT_SWITCH_IOCTL_FN(
			USBPUMP_HCDKIT_HCD *pSelf,
			USBPUMP_IOCTL_CODE RequestCode,
			CONST VOID *pInParam,
			VOID *pOutParam
			);

	USBPUMP_HCDKIT_SWITCH_IOCTL_FN *pIoctlFn;
		This function is called to allow the hw-specific half of the
		HCD driver to process OTG related IOCTLs.  Since it's called
		from the upper half of the HCD driver, the type of the
		parameter is the upper HCD object.

	typedef BOOL
		USBPUMP_HCDKIT_SWITCH_GET_ID_STATE_FN(
			USBPUMP_HCDKIT_HCD *	pHcd,
			UINT			PortNum
			);

	USBPUMP_HCDKIT_SWITCH_GET_ID_STATE_FN *pGetIdStateFn;
		This function is called to get ID pin state. This function
		returns TRUE if ID pin state is high and returns FALSE if ID
		pin state is low.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_START_HCD_FN(
			USBPUMP_HCDKIT_HCD *	pHcd
			);

	USBPUMP_HCDKIT_SWITCH_START_HCD_FN *pStartHcdFn;
		This function is called to start HCD operation.

	typedef VOID
		USBPUMP_HCDKIT_SWITCH_STOP_HCD_FN(
			USBPUMP_HCDKIT_HCD *	pHcd
			);

	USBPUMP_HCDKIT_SWITCH_STOP_HCD_FN *pStopHcdFn;
		This function is called to stop HCD operation.

Notes:
	None.

See Also:
	USBPUMP_HCDKIT_SWITCH_OTG_INIT_V1

*/

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_GET_ID_STATE_FN,
__TMS_BOOL,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_UINT			/* PortNum */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_START_HCD_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */
	));

__TMS_FNTYPE_DEF(
USBPUMP_HCDKIT_SWITCH_STOP_HCD_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */
	));

struct __TMS_STRUCTNAME	(USBPUMP_HCDKIT_SWITCH_OTG)
	{
	__TMS_USBPUMP_HCDKIT_SWITCH_IOCTL_FN *		pIoctlFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_GET_ID_STATE_FN *	pGetIdStateFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_START_HCD_FN *	pStartHcdFn;
	__TMS_USBPUMP_HCDKIT_SWITCH_STOP_HCD_FN *	pStopHcdFn;
	};


#define	__TMS_USBPUMP_HCDKIT_SWITCH_OTG_INIT_V1(	\
	ARG_pIoctlFn,					\
	ARG_pGetIdStateFn,				\
	ARG_pStartHcdFn,				\
	ARG_pStopHcdFn					\
	)						\
	{						\
	(ARG_pIoctlFn),					\
	(ARG_pGetIdStateFn),				\
	(ARG_pStartHcdFn),				\
	(ARG_pStopHcdFn)				\
	}

__TMS_BEGIN_DECLS
__TMS_USBPUMP_HCDKIT_PROCESS_HOLDING_FRAME_SPINLOCK_FN 
			UsbPumpHcdKitI_DefaultProcessHoldingFrameSpinLockFn;
__TMS_END_DECLS			

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_HCDKIT_SWITCH_INIT_V4(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootNotifyStatusChangeFn,				\
	ARG_pRootTtRequestFn,					\
	ARG_pRootGetPortErrorCountFn,	/* optional */		\
	ARG_pRootLpmRequestFn,		/* optional */		\
	ARG_pProcessHoldingFrameSpinlockFn			\
	)	\
	__TMS_USBPUMP_HCDKIT_SWITCH_INIT_V4(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootNotifyStatusChangeFn,				\
	ARG_pRootTtRequestFn,					\
	ARG_pRootGetPortErrorCountFn,	/* optional */		\
	ARG_pRootLpmRequestFn,		/* optional */		\
	ARG_pProcessHoldingFrameSpinlockFn			\
	)
# define USBPUMP_HCDKIT_SWITCH_INIT_V3(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootNotifyStatusChangeFn,				\
	ARG_pRootTtRequestFn,					\
	ARG_pRootGetPortErrorCountFn,	/* optional */		\
	ARG_pRootLpmRequestFn		/* optional */		\
	)	\
	__TMS_USBPUMP_HCDKIT_SWITCH_INIT_V3(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootNotifyStatusChangeFn,				\
	ARG_pRootTtRequestFn,					\
	ARG_pRootGetPortErrorCountFn,	/* optional */		\
	ARG_pRootLpmRequestFn		/* optional */		\
	)
# define USBPUMP_HCDKIT_SWITCH_INIT_V2(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootNotifyStatusChangeFn,				\
	ARG_pRootTtRequestFn					\
	)	\
	__TMS_USBPUMP_HCDKIT_SWITCH_INIT_V2(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootNotifyStatusChangeFn,				\
	ARG_pRootTtRequestFn					\
	)
# define USBPUMP_HCDKIT_SWITCH_INIT_V1(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootTtRequestFn					\
	)	\
	__TMS_USBPUMP_HCDKIT_SWITCH_INIT_V1(			\
	ARG_pIoctlFn,						\
	ARG_pValidateRequestFn /* optional */,			\
	ARG_pSubmitRequestFn,					\
	ARG_pRootGetDescriptorFn,				\
	ARG_pRootSetHubFeatureFn,				\
	ARG_pRootClearHubFeatureFn,				\
	ARG_pRootSetPortFeatureFn,				\
	ARG_pRootClearPortFeatureFn,				\
	ARG_pRootGetHubStatusFn,				\
	ARG_pRootGetPortStatusFn,				\
	ARG_pRootSubmitStatusReadFn,				\
	ARG_pRootTtRequestFn					\
	)
# define USBPUMP_HCDKIT_SWITCH_OTG_INIT_V1(	\
	ARG_pIoctlFn,					\
	ARG_pGetIdStateFn,				\
	ARG_pStartHcdFn,				\
	ARG_pStopHcdFn					\
	)	\
	__TMS_USBPUMP_HCDKIT_SWITCH_OTG_INIT_V1(	\
	ARG_pIoctlFn,					\
	ARG_pGetIdStateFn,				\
	ARG_pStartHcdFn,				\
	ARG_pStopHcdFn					\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_hcdkit_switch.h ****/
#endif /* _USBPUMP_HCDKIT_SWITCH_H_ */
