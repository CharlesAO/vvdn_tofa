/* usbpump_usbd_class_uvc_api.h	Wed Aug 02 2017 11:34:21 chwon */

/*

Module:  usbpump_usbd_class_uvc_api.h

Function:
	Definition of API Uvc Device functions for USBDI clients.

Version:
	V3.21b	Wed Aug 02 2017 11:34:21 chwon	Edit level 2

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

   3.21b  Wed Aug 02 2017 11:34:21  chwon
	20517: Add USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT_V10.

*/

#ifndef _USBPUMP_USBD_CLASS_UVC_API_H_	/* prevent multiple includes */
#define _USBPUMP_USBD_CLASS_UVC_API_H_

#ifndef _USBPUMP_USBD_CLASS_UVC_TYPES_H_
# include "usbpump_usbd_class_uvc_types.h"
#endif

#ifndef _USBPUMP_USBDI_CLASS_H_
# include "usbpump_usbdi_class.h"
#endif

#ifndef _USBPUMP_USBDI_FUNCTION_H_
# include "usbpump_usbdi_function.h"
#endif

#ifndef _USBPUMP_USBDI_INIT_H_
# include "usbpump_usbdi_init.h"
#endif

#ifndef _USBPUMP_CLASSKIT_API_H_
# include "usbpump_classkit_api.h"
#endif

#ifndef _USBVIDEO15_H_
# include "usbvideo15.h"
#endif

/****************************************************************************\
|
|	Uvc driver names
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_CLASS_UVC_NAME				\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_NAME("uvc")

#define	__TMS_USBPUMP_USBDI_FUNCTION_UVC_NAME				\
	__TMS_USBPUMP_USBDI_FUNCTION_NAME("uvc")

/* UVC Class GUID: 8aef8626-63a3-49da-8d7c-9345b5759be4 */
#define	__TMS_USBPUMP_USBDI_CLASS_UVC_GUID_INIT()			\
	__TMS_USBUUID_INIT(0x8aef8626, 0x63a3, 0x49da,			\
		0x8d, 0x7c, 0x93, 0x45, 0xb5, 0x75, 0x9b, 0xe4)


/****************************************************************************\
|
|	Uvc driver class in-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_USBDI_UVC_CLASS_GET_DRIVER_FEATURE_FN

Function:
	Learn capabilities of UVC driver

Definition:
	typedef BOOL
	(USBPUMP_USBDI_UVC_CLASS_GET_DRIVER_FEATURE_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		CONST TEXT *				pFeatureName,
		SIZE_T					sizeFeatureName
		);

Description:
	This is UVC driver class in-call API. It will be called by
	client to determine a feature which the UVC driver supports.

Returns:
	TRUE if feature is present, otherwise FALSE.

Notes:
	Useful in order for clients to learn capabilities of UVC driver.
	Would be better (instead of version) to define "feature masks"
	- "I support Isoch"
	- "I support high-speed"
	- "I support wireless"
	- "I have bug-fix XXX"
	Could use getter methods feature-by-feature
	Could use getter methods based on string feature IDs

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_UVC_CLASS_GET_DRIVER_FEATURE_FN,
__TMS_BOOL,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_CONST __TMS_TEXT *	/* pFeatureName */,
	__TMS_SIZE_T			/* sizeFeatureName */
	));


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_CLASS_UVC_INCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_CLASS_UVC_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER;

	__TMS_USBPUMP_USBDI_UVC_CLASS_GET_DRIVER_FEATURE_FN *
						pGetDriverFeatureFn;

	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER;
	};

union __TMS_UNIONNAME	(USBPUMP_USBDI_CLASS_UVC_INCALL)
	{
	__TMS_USBPUMP_USBDI_CLASS_UVC_INCALL_CONTENTS	Uvc;
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS__UNION;
	};

#define	__TMS_USBPUMP_USBDI_CLASS_UVC_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)								\
	{{								\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_INIT_V1(		\
		sizeof(__TMS_USBPUMP_USBDI_CLASS_UVC_INCALL),		\
		a_pCloseFn,						\
		a_pOpenFunctionFn,					\
		a_pGetNumDevicesFn,					\
		a_pGetBoundDevicesFn					\
		),							\
	(a_pGetDriverFeatureFn),					\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_INIT_V1(		\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_USBDI_CLASS_UVC_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)								\
    do	{								\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_SETUP_V1(		\
		a_pInCall,						\
		sizeof(*(a_pInCall)),					\
		a_pCloseFn,						\
		a_pOpenFunctionFn,					\
		a_pGetNumDevicesFn,					\
		a_pGetBoundDevicesFn					\
		);							\
	(a_pInCall)->Uvc.pGetDriverFeatureFn = (a_pGetDriverFeatureFn);	\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_SETUP_V1(		\
		a_pInCall,						\
		Uvc,							\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		);							\
	} while (0)


/****************************************************************************\
|
|	Uvc driver function in-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_USBDI_UVC_FUNCTION_GET_DEVICE_INFO_FN

Function:
	Get device information of the UVC device.

Definition:
	typedef USBPUMP_CLASSKIT_STATUS
	(USBPUMP_USBDI_UVC_FUNCTION_GET_DEVICE_INFO_FN)(
		USBPUMP_SESSION_HANDLE		FunctionHandle,
		USBPUMP_USBDI_UVC_DEVICE_INFO *	pDeviceInfo
		);

Description:
	This API gets the device information of the UVC device.

Returns:
	USBPUMP_CLASSKIT_STATUS_OK
	USBPUMP_CLASSKIT_STATUS_INVALID_FUNCTION_HANDLE
	USBPUMP_CLASSKIT_STATUS_INVALID_PARAMETER

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_UVC_FUNCTION_GET_DEVICE_INFO_FN,
__TMS_USBPUMP_CLASSKIT_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* FunctionHandle */,
	__TMS_USBPUMP_USBDI_UVC_DEVICE_INFO *	/* pDeviceInfo */
	));

struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_DEVICE_INFO)
	{
	__TMS_UINT16			idVendor;
	__TMS_UINT16			idProduct;

	__TMS_UINT8			bControlInterfaceClass;
	__TMS_UINT8			bControlInterfaceSubClass;
	__TMS_UINT8			bControlInterfaceProtocol;

	__TMS_UINT8			bNumStreamingInterfaces;

	__TMS_UINT16			bcdUVC;
	__TMS_UINT16			nVideoControlDescriptors;
	__TMS_CONST __TMS_UINT8 *	pVideoControlDescriptors;

	__TMS_CONST __TMS_CHAR *	pVideoString;
	};

/*

Type:  USBPUMP_USBDI_UVC_FUNCTION_GET_VIDEO_STREAM_INFO_FN

Function:
	Get video streaming information of the UVC device.

Definition:
	typedef USBPUMP_CLASSKIT_STATUS
	(USBPUMP_USBDI_UVC_FUNCTION_GET_DEVICE_INFO_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		UINT					iVideoStream,
		USBPUMP_USBDI_UVC_VIDEO_STREAM_INFO *	pVideoStreamInfo
		);

Description:
	This API gets the video streamming interface information of the
	UVC device.

Returns:
	USBPUMP_CLASSKIT_STATUS_OK
	USBPUMP_CLASSKIT_STATUS_INVALID_FUNCTION_HANDLE
	USBPUMP_CLASSKIT_STATUS_INVALID_PARAMETER

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_UVC_FUNCTION_GET_VIDEO_STREAM_INFO_FN,
__TMS_USBPUMP_CLASSKIT_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,
	__TMS_UINT					/* iVideoStream */,
	__TMS_USBPUMP_USBDI_UVC_VIDEO_STREAM_INFO *	/* pVideoStreamInfo */
	));

struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT_V10)
	{
	__TMS_UINT16			bmHint;
	__TMS_UINT8			bFormatIndex;
	__TMS_UINT8			bFrameIndex;
	__TMS_UINT32			dwFrameInterval;
	__TMS_UINT16			wKeyFrameRate;
	__TMS_UINT16			wPFrameRate;
	__TMS_UINT16			wCompQuality;
	__TMS_UINT16			wCompWindowSize;
	__TMS_UINT16			wDelay;
	__TMS_UINT32			dwMaxVideoFrameSize;
	__TMS_UINT32			dwMaxPayloadTransferSize;
	};

struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT_V11)
	{
	__TMS_UINT16			bmHint;
	__TMS_UINT8			bFormatIndex;
	__TMS_UINT8			bFrameIndex;
	__TMS_UINT32			dwFrameInterval;
	__TMS_UINT16			wKeyFrameRate;
	__TMS_UINT16			wPFrameRate;
	__TMS_UINT16			wCompQuality;
	__TMS_UINT16			wCompWindowSize;
	__TMS_UINT16			wDelay;
	__TMS_UINT32			dwMaxVideoFrameSize;
	__TMS_UINT32			dwMaxPayloadTransferSize;
	__TMS_UINT32			dwClockFrequency;
	__TMS_UINT8			bmFramingInfo;
	__TMS_UINT8			bPreferedVersion;
	__TMS_UINT8			bMinVersion;
	__TMS_UINT8			bMaxVersion;
	};

struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT_V15)
	{
	__TMS_UINT16			bmHint;
	__TMS_UINT8			bFormatIndex;
	__TMS_UINT8			bFrameIndex;
	__TMS_UINT32			dwFrameInterval;
	__TMS_UINT16			wKeyFrameRate;
	__TMS_UINT16			wPFrameRate;
	__TMS_UINT16			wCompQuality;
	__TMS_UINT16			wCompWindowSize;
	__TMS_UINT16			wDelay;
	__TMS_UINT32			dwMaxVideoFrameSize;
	__TMS_UINT32			dwMaxPayloadTransferSize;
	__TMS_UINT32			dwClockFrequency;
	__TMS_UINT8			bmFramingInfo;
	__TMS_UINT8			bPreferedVersion;
	__TMS_UINT8			bMinVersion;
	__TMS_UINT8			bMaxVersion;
	__TMS_UINT8			bUsage;
	__TMS_UINT8			bBitDepthLuma;
	__TMS_UINT8			bmSettings;
	__TMS_UINT8			bMaxNumberOfRefFramesPlus1;
	__TMS_UINT16			bmRateControlModes;
	__TMS_UINT16			bmLayoutPerStream[4];
	};

union __TMS_UNIONNAME	(USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT)
	{
	__TMS_USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT_V10	V10;
	__TMS_USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT_V11	V11;
	__TMS_USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT_V15	V15;
	};

struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_STREAM_STILL_PROBE_COMMIT)
	{
	__TMS_UINT8			bFormatIndex;
	__TMS_UINT8			bFrameIndex;
	__TMS_UINT8			bCompressionIndex;
	__TMS_UINT32			dwMaxVideoFrameSize;
	__TMS_UINT32			dwMaxPayloadTransferSize;
	};

struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_VIDEO_STREAM_INFO)
	{
	__TMS_UINT8			bInterfaceClass;
	__TMS_UINT8			bInterfaceSubClass;
	__TMS_UINT8			bInterfaceProtocol;
	__TMS_UINT8			bStillCaptureMethod;
	__TMS_UINT8			fInputStream;
	__TMS_UINT8			fIsochEndpoint;
	__TMS_UINT16			wMaxPacketSize;
	__TMS_UINT16			wStillMaxPacketSize;
	__TMS_UINT16			nVideoStreamDescriptors;
	__TMS_CONST __TMS_UINT8 *	pVideoStreamDescriptors;

	__TMS_USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT *
					pVideoProbe;
	__TMS_USBPUMP_USBDI_UVC_STREAM_STILL_PROBE_COMMIT *
					pStillProbe;
	};


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_FUNCTION_UVC_INCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_FUNCTION_UVC_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER;

	/* UVC driver instance function in-call functions here */
	__TMS_USBPUMP_USBDI_UVC_FUNCTION_GET_DEVICE_INFO_FN *
							pGetDeviceInfoFn;
	__TMS_USBPUMP_USBDI_UVC_FUNCTION_GET_VIDEO_STREAM_INFO_FN *
							pGetVideoStreamInfoFn;
	__TMS_USBPUMP_USBDI_UVC_FUNCTION_SUBMIT_REQUEST_FN *
							pSubmitRequestFn;

	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER;
	};

union __TMS_UNIONNAME	(USBPUMP_USBDI_FUNCTION_UVC_INCALL)
	{
	__TMS_USBPUMP_USBDI_FUNCTION_UVC_INCALL_CONTENTS	Uvc;
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS__UNION;
	};


#define	__TMS_USBPUMP_USBDI_FUNCTION_UVC_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetVideoStreamInfoFn,					\
	a_pSubmitRequestFn						\
	)								\
	{{								\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_INIT_V1(		\
		sizeof(__TMS_USBPUMP_USBDI_FUNCTION_UVC_INCALL),	\
		a_pCloseFn						\
		),							\
	(a_pGetDeviceInfoFn),						\
	(a_pGetVideoStreamInfoFn),					\
	(a_pSubmitRequestFn),						\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_INIT_V1(		\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_USBDI_FUNCTION_UVC_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetVideoStreamInfoFn,					\
	a_pSubmitRequestFn						\
	)								\
    do	{								\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_SETUP_V1(		\
		a_pInCall,						\
		sizeof(*(a_pInCall)),					\
		a_pCloseFn						\
		);							\
	(a_pInCall)->Uvc.pGetDeviceInfoFn = (a_pGetDeviceInfoFn);	\
	(a_pInCall)->Uvc.pGetVideoStreamInfoFn = (a_pGetVideoStreamInfoFn); \
	(a_pInCall)->Uvc.pSubmitRequestFn = (a_pSubmitRequestFn);	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_SETUP_V1(	\
		a_pInCall,						\
		Uvc,							\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		);							\
	} while (0)


/****************************************************************************\
|
|	Uvc Class Driver Status Codes
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_UVC_STATUS_BASE		\
	__TMS_USBPUMP_CLASSKIT_STATUS__MAX

#define	__TMS_USBPUMP_USBDI_UVC_STATUS_INTERNAL_ERROR	\
	(__TMS_USBPUMP_USBDI_UVC_STATUS_BASE + 0)
#define	__TMS_USBPUMP_USBDI_UVC_STATUS_INVALID_STREAM	\
	(__TMS_USBPUMP_USBDI_UVC_STATUS_BASE + 1)


/****************************************************************************\
|
|	Uvc Class Driver Notification Definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_UVC_EVENT_BASE		\
	__TMS_USBPUMP_CLASSKIT_EVENT__MAX

#define	__TMS_USBPUMP_USBDI_UVC_EVENT_STATUS_INTERRUPT	\
	(__TMS_USBPUMP_USBDI_UVC_EVENT_BASE + 0)

#define	__TMS_USBPUMP_USBDI_UVC_EVENT_NAMES__INIT	\
	__TMS_USBPUMP_CLASSKIT_EVENT_NAMES__INIT	\
	"UVC_EVENT_STATUS_INTERRUPT"


__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_UVC_EVENT_STATUS_INTERRUPT_INFO);
union __TMS_UNIONNAME	(USBPUMP_USBDI_UVC_EVENT_STATUS_INTERRUPT_INFO)
	{
	USB_Video_StatusPacketFormatHeader		Hdr;
	USB_Video_StatusPacketFormatVideoControl	Control;
	USB_Video_StatusPacketFormatVideoStreaming	Stream;
	};


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_CONST __TMS_USBGUID	gk_UsbPumpUsbdiClassUvc_Guid;

extern __TMS_CONST __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG
					gk_UsbPumpUsbdiUvc_ClassConfig;

__TMS_USBPUMP_USBDI_DRIVER_CLASS_INIT_FN
UsbPumpUsbdiClassUvc_Initialize;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_CLASS_UVC_NAME	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_NAME
# define USBPUMP_USBDI_FUNCTION_UVC_NAME	\
   __TMS_USBPUMP_USBDI_FUNCTION_UVC_NAME
# define USBPUMP_USBDI_CLASS_UVC_GUID_INIT()	\
   __TMS_USBPUMP_USBDI_CLASS_UVC_GUID_INIT()
# define USBPUMP_USBDI_CLASS_UVC_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)	\
	__TMS_USBPUMP_USBDI_CLASS_UVC_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)
# define USBPUMP_USBDI_CLASS_UVC_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)	\
	__TMS_USBPUMP_USBDI_CLASS_UVC_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)
# define USBPUMP_USBDI_FUNCTION_UVC_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetVideoStreamInfoFn,					\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_USBDI_FUNCTION_UVC_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetVideoStreamInfoFn,					\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_USBDI_FUNCTION_UVC_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetVideoStreamInfoFn,					\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_USBDI_FUNCTION_UVC_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetVideoStreamInfoFn,					\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_USBDI_UVC_STATUS_BASE	\
   __TMS_USBPUMP_USBDI_UVC_STATUS_BASE
# define USBPUMP_USBDI_UVC_STATUS_INTERNAL_ERROR	\
   __TMS_USBPUMP_USBDI_UVC_STATUS_INTERNAL_ERROR
# define USBPUMP_USBDI_UVC_STATUS_INVALID_STREAM	\
   __TMS_USBPUMP_USBDI_UVC_STATUS_INVALID_STREAM
# define USBPUMP_USBDI_UVC_EVENT_BASE	\
   __TMS_USBPUMP_USBDI_UVC_EVENT_BASE
# define USBPUMP_USBDI_UVC_EVENT_STATUS_INTERRUPT	\
   __TMS_USBPUMP_USBDI_UVC_EVENT_STATUS_INTERRUPT
# define USBPUMP_USBDI_UVC_EVENT_NAMES__INIT	\
   __TMS_USBPUMP_USBDI_UVC_EVENT_NAMES__INIT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbd_class_uvc_api.h ****/
#endif /* _USBPUMP_USBD_CLASS_UVC_API_H_ */
