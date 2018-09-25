/* usbpump_usbd_class_uvc_lib.h	Thu Mar 16 2017 16:55:51 chwon */

/*

Module:  usbpump_usbd_class_uvc_lib.h

Function:
	Library of the USBD Uvc driver

Version:
	V3.19b	Thu Mar 16 2017 16:55:51 chwon	Edit level 1

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

*/

#ifndef _USBPUMP_USBD_CLASS_UVC_LIB_H_	/* prevent multiple includes */
#define _USBPUMP_USBD_CLASS_UVC_LIB_H_

#ifndef _USBPUMP_USBD_CLASS_UVC_REQUEST_H_
# include "usbpump_usbd_class_uvc_request.h"
#endif

/****************************************************************************\
|
|	Video Control Request Info Structure
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CONTROL_INFO_HDR);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO_HDR)
	{
	__TMS_UINT8			bInfo;
	__TMS_UINT8			bmValid;
	};

#define	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_MIN	(1u << 0)
#define	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_MAX	(1u << 1)
#define	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_RES	(1u << 2)
#define	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_DEF	(1u << 3)
#define	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_CUR	(1u << 4)

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE)
	{
	__TMS_UINT8			bInfo;
	__TMS_UINT8			bmValid;
	__TMS_UINT8			bMin;
	__TMS_UINT8			bMax;
	__TMS_UINT8			bRes;
	__TMS_UINT8			bDef;
	__TMS_UINT8			bCur;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE2);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE2)
	{
	__TMS_UINT8			bInfo;
	__TMS_UINT8			bmValid;
	__TMS_UINT8			bMin[2];
	__TMS_UINT8			bMax[2];
	__TMS_UINT8			bRes[2];
	__TMS_UINT8			bDef[2];
	__TMS_UINT8			bCur[2];
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE3);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE3)
	{
	__TMS_UINT8			bInfo;
	__TMS_UINT8			bmValid;
	__TMS_UINT8			bMin[3];
	__TMS_UINT8			bMax[3];
	__TMS_UINT8			bRes[3];
	__TMS_UINT8			bDef[3];
	__TMS_UINT8			bCur[3];
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE4);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE4)
	{
	__TMS_UINT8			bInfo;
	__TMS_UINT8			bmValid;
	__TMS_UINT8			bMin[4];
	__TMS_UINT8			bMax[4];
	__TMS_UINT8			bRes[4];
	__TMS_UINT8			bDef[4];
	__TMS_UINT8			bCur[4];
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CONTROL_INFO_WORD);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO_WORD)
	{
	__TMS_UINT8			bInfo;
	__TMS_UINT8			bmValid;
	__TMS_UINT16			wMin;
	__TMS_UINT16			wMax;
	__TMS_UINT16			wRes;
	__TMS_UINT16			wDef;
	__TMS_UINT16			wCur;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CONTROL_INFO_WORD2);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO_WORD2)
	{
	__TMS_UINT8			bInfo;
	__TMS_UINT8			bmValid;
	__TMS_UINT16			wMin[2];
	__TMS_UINT16			wMax[2];
	__TMS_UINT16			wRes[2];
	__TMS_UINT16			wDef[2];
	__TMS_UINT16			wCur[2];
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CONTROL_INFO_WORD5);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO_WORD5)
	{
	__TMS_UINT8			bInfo;
	__TMS_UINT8			bmValid;
	__TMS_UINT16			wMin[5];
	__TMS_UINT16			wMax[5];
	__TMS_UINT16			wRes[5];
	__TMS_UINT16			wDef[5];
	__TMS_UINT16			wCur[5];
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CONTROL_INFO_WORD6);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO_WORD6)
	{
	__TMS_UINT8			bInfo;
	__TMS_UINT8			bmValid;
	__TMS_UINT16			wMin[6];
	__TMS_UINT16			wMax[6];
	__TMS_UINT16			wRes[6];
	__TMS_UINT16			wDef[6];
	__TMS_UINT16			wCur[6];
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CONTROL_INFO_DWORD);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO_DWORD)
	{
	__TMS_UINT8			bInfo;
	__TMS_UINT8			bmValid;
	__TMS_UINT32			dwMin;
	__TMS_UINT32			dwMax;
	__TMS_UINT32			dwRes;
	__TMS_UINT32			dwDef;
	__TMS_UINT32			dwCur;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CONTROL_INFO_DWORD2);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO_DWORD2)
	{
	__TMS_UINT8			bInfo;
	__TMS_UINT8			bmValid;
	__TMS_UINT32			dwMin[2];
	__TMS_UINT32			dwMax[2];
	__TMS_UINT32			dwRes[2];
	__TMS_UINT32			dwDef[2];
	__TMS_UINT32			dwCur[2];
	};

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_UVC_CONTROL_INFO);
union __TMS_UNIONNAME	(USBPUMP_USBDI_UVC_CONTROL_INFO)
	{
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_HDR	Hdr;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	B;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE2	B2;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE3	B3;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE4	B4;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	W;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD2	W2;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD5	W5;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD6	W6;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_DWORD	D;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_DWORD2	D2;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_CAMERA_TERMINAL_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_CAMERA_TERMINAL_INFO)
	{
	__TMS_CONST USB_VideoControl_CameraTerminalDescriptor *pCTDesc;
	__TMS_UINT					bmControls;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	SM;	/* ScanningMode */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	AEM;	/* Auto-Exposure Mode */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	AEP;	/* Auto-Exposure Priority */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_DWORD	ETA;	/* Exposure Time Absolute */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	ETR;	/* Exposure Time Relative */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	FA;	/* Focus Absolute */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE2	FR;	/* Focus Relative */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	IA;	/* Iris Absolute */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	IR;	/* Iris Relative */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	ZA;	/* Zoom Absolute */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE3	ZR;	/* Zoom Relative */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_DWORD2	PTA;	/* PanTilt Absolute */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE4	PTR;	/* PanTilt Relative */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	RA;	/* Roll Absolute */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE2	RR;	/* Roll Relative */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	FAuto;	/* Focus Auto */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	P;	/* Privacy */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	FS;	/* Focus Simple */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD6	W;	/* Window */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD5	ROI;	/* Region of Interest */
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_PROCESSING_UNIT_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_PROCESSING_UNIT_INFO)
	{
	__TMS_CONST USB_VideoControl_ProcessingUnitDescriptor *pPUDesc;
	__TMS_UINT					bmControls;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	Backlight;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	Brightness;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	Contrast;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	Gain;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	PowerLineFreq;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	Hue;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	Saturation;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	Sharpness;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	Gamma;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	WBT;	/* WhiteBalanceTemperature */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	WBTA;	/* WhiteBalanceTemperatureAuto */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD2	WBC;	/* WhiteBalanceComponent */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	WBCA;	/* WhiteBalanceComponentAuto */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	DM;	/* DigitalMultiplier */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_WORD	DML;	/* DigitalMultiplierLimit */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	HueAuto;
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	AVS;	/* AnalogVideoStandard */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	AVLS;	/* AnalogVideoLockStatus */
	__TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_BYTE	ContrastAuto;
	};


/****************************************************************************\
|
|	Video Frame Structure
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_VIDEO_FRAME_BULK);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_VIDEO_FRAME_BULK)
	{
	__TMS_UINT32			dwPresentationTime;
	__TMS_UINT32			scrSourceClockSTC;
	__TMS_UINT16			scrSourceClockSOF;
	__TMS_UINT8			bHeaderLength;
	__TMS_UINT8			bmHeaderInfo;

	__TMS_UINT8 *			pVideoFrameBuffer;
	__TMS_BYTES			nVideoFrameBuffer;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_UVC_VIDEO_FRAME_ISOCH);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_UVC_VIDEO_FRAME_ISOCH)
	{
	__TMS_UINT32			dwPresentationTime;
	__TMS_UINT32			scrSourceClockSTC;
	__TMS_UINT16			scrSourceClockSOF;
	__TMS_UINT8			bHeaderLength;
	__TMS_UINT8			bmHeaderInfo;

	__TMS_UINT8 *			pVideoFrameBuffer;
	__TMS_BYTES			nVideoFrameBuffer;

	__TMS_UINT8			bStatus;
	__TMS_UINT8			nNoPayload;
	__TMS_UINT16			nIsochPayloadError;
	__TMS_BYTES			nCopiedFrameBuffer;
	};

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_UVC_VIDEO_FRAME);
union __TMS_UNIONNAME	(USBPUMP_USBDI_UVC_VIDEO_FRAME)
	{
	__TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_BULK	Bulk;
	__TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_ISOCH	Isoch;
	};

#define	__TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_BUSY		0
#define	__TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_OK		1
#define	__TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_BUF_FULL	2
#define	__TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_NO_DATA		3


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

#define	UsbPumpUsbdUvc_PrepareGetCameraTerminalInfoRequest(		\
	a_pRequest,							\
	a_pCtInfo,	/* USBPUMP_USBDI_UVC_CAMERA_TERMINAL_INFO * */	\
	a_pDoneFn,							\
	a_pDoneInfo,							\
	a_Timeout							\
	)								\
    do	{								\
	(a_pRequest)->pBuffer = (a_pCtInfo);				\
	(a_pRequest)->nBuffer = sizeof(*(a_pCtInfo));			\
	(a_pRequest)->pDoneFn = (a_pDoneFn);				\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);			\
	(a_pRequest)->Timeout = (a_Timeout);				\
	} while (0)

#define	UsbPumpUsbdUvc_PrepareGetProcessingUnitInfoRequest(		\
	a_pRequest,							\
	a_pPuInfo,	/* USBPUMP_USBDI_UVC_PROCESSING_UNIT_INFO * */	\
	a_pDoneFn,							\
	a_pDoneInfo,							\
	a_Timeout							\
	)								\
    do	{								\
	(a_pRequest)->pBuffer = (a_pPuInfo);				\
	(a_pRequest)->nBuffer = sizeof(*(a_pPuInfo));			\
	(a_pRequest)->pDoneFn = (a_pDoneFn);				\
	(a_pRequest)->pDoneInfo = (a_pDoneInfo);			\
	(a_pRequest)->Timeout = (a_Timeout);				\
	} while (0)

__TMS_BEGIN_DECLS

__TMS_CONST __TMS_VOID *
UsbPumpUsbdUvcLib_GetControlDescriptor(
	__TMS_USBPUMP_SESSION_HANDLE	FunctionHandle,
	__TMS_UINT8			bDescriptorSubtype
	);

__TMS_CONST __TMS_VOID *
UsbPumpUsbdUvcLib_GetStreamDescriptor(
	__TMS_USBPUMP_SESSION_HANDLE	FunctionHandle,
	__TMS_UINT			iVideoStream,
	__TMS_UINT8			bDescriptorSubtype,
	__TMS_CONST __TMS_VOID *	pPreviousDescriptor	/* OPTIONAL */
	);

__TMS_VOID
UsbPumpUsbdUvcLib_GetCameraTerminalInfo(
	__TMS_USBPUMP_SESSION_HANDLE		FunctionHandle,
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST *	pRequest
	);

__TMS_VOID
UsbPumpUsbdUvcLib_GetProcessingUnitInfo(
	__TMS_USBPUMP_SESSION_HANDLE		FunctionHandle,
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST *	pRequest
	);

__TMS_BOOL
UsbPumpUsbdUvcLib_GetBulkVideoFrame(
	__TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_BULK *	pVideoFrame,
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST *		pRequest
	);

__TMS_BOOL
UsbPumpUsbdUvcLib_ReadVideoFrameFirst(
	__TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_ISOCH *	pVideoFrame,
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST *		pRequest
	);

__TMS_BOOL
UsbPumpUsbdUvcLib_ReadVideoFrameNext(
	__TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_ISOCH *	pVideoFrame,
	__TMS_USBPUMP_USBDI_CLASS_UVC_REQUEST *		pRequest
	);

__TMS_BYTES
UsbPumpUsbdUvcLib_GetVideoFrameBufferSize(
	__TMS_USBPUMP_USBDI_UVC_VIDEO_STREAM_INFO *	pStreamInfo,
	__TMS_USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT *	pStreamProbe
	);

__TMS_BYTES
UsbPumpUsbdUvcLib_GetVideoFramePayloadSize(
	__TMS_USBPUMP_USBDI_UVC_VIDEO_STREAM_INFO *	pStreamInfo,
	__TMS_USBPUMP_USBDI_UVC_STREAM_PROBE_COMMIT *	pStreamProbe
	);

__TMS_BYTES
UsbPumpUsbdUvcLib_GetStillImageBufferSize(
	__TMS_USBPUMP_USBDI_UVC_VIDEO_STREAM_INFO *		pStreamInfo,
	__TMS_USBPUMP_USBDI_UVC_STREAM_STILL_PROBE_COMMIT *	pStillProbe
	);

__TMS_BYTES
UsbPumpUsbdUvcLib_GetStillImagePayloadSize(
	__TMS_USBPUMP_USBDI_UVC_VIDEO_STREAM_INFO *		pStreamInfo,
	__TMS_USBPUMP_USBDI_UVC_STREAM_STILL_PROBE_COMMIT *	pStillProbe
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_MIN	\
   __TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_MIN
# define USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_MAX	\
   __TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_MAX
# define USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_RES	\
   __TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_RES
# define USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_DEF	\
   __TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_DEF
# define USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_CUR	\
   __TMS_USBPUMP_USBDI_UVC_CONTROL_INFO_VALID_CUR
# define USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_BUSY	\
   __TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_BUSY
# define USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_OK	\
   __TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_OK
# define USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_BUF_FULL	\
   __TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_BUF_FULL
# define USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_NO_DATA	\
   __TMS_USBPUMP_USBDI_UVC_VIDEO_FRAME_STS_NO_DATA
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbd_class_uvc_lib.h ****/
#endif /* _USBPUMP_USBD_CLASS_UVC_LIB_H_ */
