/* usbpump_usbd_class_uvc_types.h	Wed Aug 02 2017 11:34:00 chwon */

/*

Module:  usbpump_usbd_class_uvc_types.h

Function:
	Definition of the USBD Uvc driver configuration

Version:
	V3.21b	Wed Aug 02 2017 11:34:00 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2017

Revision history:
   3.19b  Thu Mar 16 2017 16:55:51  chwon
	20517: Module created.

   3.21b  Wed Aug 02 2017 11:34:00  chwon
	20517: Add USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT_V10.

*/

#ifndef _USBPUMP_USBD_CLASS_UVC_TYPES_H_	/* prevent multiple includes */
#define _USBPUMP_USBD_CLASS_UVC_TYPES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMP_USBD_CLASS_UVC_PARAMS_H_
# include "usbpump_usbd_class_uvc_params.h"
#endif

/****************************************************************************\
|
|	The USBD Uvc Class Driver Types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_CLASS_UVC_CONFIG);
__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_CLASS_UVC_INCALL);
__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_FUNCTION_UVC_INCALL);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_DEVICE_INFO);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_VIDEO_STREAM_INFO);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT_V10);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT_V11);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT_V15);
__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_STREAM_STILL_PROBE_COMMIT);

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_CLASS_UVC_REQUEST);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBDI_CLASS_UVC_REQUEST_CODE, UINT8);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT, UINT8);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBDI_CLASS_UVC_REQUEST_TIMEOUT, UINT16);

/*

Type:	USBPUMP_USBDI_CLASS_UVC_REQUEST_DONE_FN

Index:	Type:	PUSBPUMP_USBDI_CLASS_UVC_REQUEST_DONE_FN

Function:
	Called to report that a UVC request has completed

Definition:
	typedef VOID USBPUMP_USBDI_CLASS_UVC_REQUEST_DONE_FN(
			USBPUMP_USBDI_CLASS_UVC_REQUEST *	pRequest,
			USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT	Status
			);

	typedef USBPUMP_HCD_REQUEST_DONE_FN
		*PUSBPUMP_HCD_REQUEST_DONE_FN;

Description:
	Every request passed to an HCD for processing must have a non-NULL
	pointer to the completion function in its Hdr.pDoneFn.  This
	function is called after the HCD has completed all transfer-related
	processing for the UVC request.

Returns:
	No explicit result.

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_CLASS_UVC_REQUEST_DONE_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST * 	/* pRequest */,
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST_STAT	/* Status */
	));

/*

Type:  USBPUMP_USBDI_UVC_FUNCTION_SUBMIT_REQUEST_FN

Function:
	Submit a UVC request

Definition:
	typedef VOID
	(USBPUMP_USBDI_UVC_FUNCTION_SUBMIT_REQUEST_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_CLASS_UVC_REQUEST *	pRequest
		);

Description:
	This API submits a UVC request to the UVC function driver.
	The USBPUMP_USBDI_CLASS_UVC_REQUEST::pDoneFn function will be called
	when this request is completed.

Returns:
	No explicit result.

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_UVC_FUNCTION_SUBMIT_REQUEST_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* FunctionHandle */,
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST *	/* pRequest */
	));

/**** end of usbpump_usbd_class_uvc_types.h ****/
#endif /* _USBPUMP_USBD_CLASS_UVC_TYPES_H_ */
