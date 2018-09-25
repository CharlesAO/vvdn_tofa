/* protovideo.h	Thu May 12 2016 09:58:15 chwon */

/*

Module:  protovideo.h

Function:
	video class protocol API.

Version:
	V3.17a	Thu May 12 2016 09:58:15 chwon	Edit level 10

Copyright notice:
	This file copyright (C) 2007-2010, 2015-2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaoTsung Perng, MCCI Corporation	September 2007

Revision history:
   1.97j  Fri Sep 14 2007 09:59:12  ctpeng
	Module created.

   1.97j  Mon Mar 24 2008 11:28:33  ctpeng
	Fix compiler warnings.	

   1.97k  Wed Apr 16 2008 11:11:04  chwon
	3735: reorganize UBUFQE to allow 32-bit sizes and change buffer size
	from UINT16 to BYTES.

   2.01a  Tue Mar 31 2009 15:09:54  chwon
	7730: redefine UPROTO_VIDEO_STREAM_HANDLE instead of UINT32

   2.01a  Wed Dec 02 2009 10:45:16  skpark
	9357: Fixed doc-xml error in revision history

   3.01d  Mon Dec 27 2010 14:12:55  ctpeng
	12250: Add UsbPumpVideo_ProtocolAddInterface

   3.15b  Thu Apr 02 2015 12:20:44  chwon
	19117: Add isoch endpoint stream read/write switch function. Add get
	video status and error code name API.

   3.17a  Thu Jan 28 2016 15:13:43  chwon
	19642: Add buffer handle in read and write switch functions.

   3.17a  Fri Apr 29 2016 15:20:07  chwon
	20075: Increase Data buffer size of UPROTO_VIDEO_XU_GENERIC.

   3.17a  Thu May 12 2016 09:58:15  chwon
	20075: Change default Data buffer size of UPROTO_VIDEO_XU_GENERIC.

*/

#ifndef _PROTOVIDEO_H_		/* prevent multiple includes */
#define _PROTOVIDEO_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPROTOINIT_H_
# include "usbprotoinit.h"
#endif

#ifndef _UDATAPLANE_H_
# include "udataplane.h"
#endif

#ifndef _UDATASTREAM_H_
# include "udatastream.h"
#endif

#ifndef _USBPUMPOBJECTNAME_H_
# include "usbpumpobjectname.h"
#endif

/****************************************************************************\
|
|	Forward Declaration
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CONFIG);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_INSWITCH);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_OUTSWITCH);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_STREAM);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_ENTITY);
__TMS_TYPE_DEF_ARG(UPROTO_VIDEO_STATUS, UINT8);
__TMS_TYPE_DEF_HANDLE(UPROTO_VIDEO_STREAM_HANDLE);


/****************************************************************************\
|
|	Class matching strings
|
\****************************************************************************/

#define	__TMS_USBPUMP_OBJECT_NAME_VIDEO		"video"

/*
|| Define strings to be used when enumerating functions
*/
#define	__TMS_USBPUMP_OBJECT_NAME_ENUM_VIDEO		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_VIDEO)


/****************************************************************************\
|
|	Video Protocol Constants And Types
|
\****************************************************************************/

#define __TMS_UPROTO_VIDEO_STREAM_INVALID_HANDLE	\
	((__TMS_UPROTO_VIDEO_STREAM_HANDLE) __TMS_NULL)


#define	__TMS_UPROTO_VIDEO_STATUS_START				0
#define	__TMS_UPROTO_VIDEO_STATUS_SUSPEND_WITH_WAKEUP		1
#define	__TMS_UPROTO_VIDEO_STATUS_SUSPEND_WITHOUT_WAKEUP	2
#define	__TMS_UPROTO_VIDEO_STATUS_RESUME			3
#define	__TMS_UPROTO_VIDEO_STATUS_RESET				4
#define	__TMS_UPROTO_VIDEO_STATUS_ATTACH			5
#define	__TMS_UPROTO_VIDEO_STATUS_DETACH			6
#define	__TMS_UPROTO_VIDEO_STATUS_ACTIVATE			7
#define	__TMS_UPROTO_VIDEO_STATUS_DEACTIVATE			8

#define	__TMS_UPROTO_VIDEO_STATUS__INIT				\
	{							\
	/* UPROTO_VIDEO_STATUS_START		*/ "START",	\
	/* UPROTO_VIDEO_STATUS_SUSPEND_WITH_WAKEUP    */ "SUSPEND_WITH_WAKEUP", \
	/* UPROTO_VIDEO_STATUS_SUSPEND_WITHOUT_WAKEUP */ "SUSPEND_WITHOUT_WAKEUP", \
	/* UPROTO_VIDEO_STATUS_RESUME 		*/ "RESUME",	\
	/* UPROTO_VIDEO_STATUS_RESET		*/ "RESET",	\
	/* UPROTO_VIDEO_STATUS_ATTACH		*/ "ATTACH",	\
	/* UPROTO_VIDEO_STATUS_DETACH		*/ "DETACH",	\
	/* UPROTO_VIDEO_STATUS_ACTIVATE		*/ "ACTIVATE",	\
	/* UPROTO_VIDEO_STATUS_DEACTIVATE	*/ "DEACTIVATE"	\
	}


/****************************************************************************\
|
|	Video Control / Streaming Request Structure
|
\****************************************************************************/

/* XXX: we should define video control/streaming request structure */
__TMS_TYPE_DEF_UNION(UPROTO_VIDEO_CONTROL_REQUEST);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CONTROL_REQUEST_HDR);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CONTROL_GET_INFO_REQUEST);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_VC_REQUEST_POWER_MODE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_VC_REQUEST_ERROR_CODE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_SCANNING_MODE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_AE_MODE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_AE_PRIORITY_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_EXPOSURE_TIME_RELATIVE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_FOCUS_ABSOLUTE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_FOCUS_RELATIVE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_FOCUS_AUTO_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_IRIS_ABSOLUTE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_IRIS_RELATIVE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_ZOOM_ABSOLUTE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_ZOOM_RELATIVE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_PANTILT_ABSOLUTE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_PANTILT_RELATIVE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_ROLL_ABSOLUTE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_ROLL_RELATIVE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_CT_PRIVACY_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_SU_INPUT_SELECT_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_BACKLIGHT_COMPENSATION_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_BRIGHTNESS_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_CONTRAST_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_GAIN_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_POWER_LINE_FREQUENCY_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_HUE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_HUE_AUTO_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_SATURATION_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_SHARPNESS_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_GAMMA_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_WHITE_BALANCE_TEMPERATURE_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_WHITE_BALANCE_COMPONENT_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_DIGITAL_MULTIPLIER_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_ANALOG_VIDEO_STANDARD_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_PU_ANALOG_LOCK_STATUS_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_XU_GENERIC);


#define	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR		\
	__TMS_UINT8		bRequestType;		\
	__TMS_UINT8		bControlSelector;	\
	__TMS_UINT8		bRequest;		\
	__TMS_UINT8		bAcceptRequest;		\
	__TMS_UINT16		wLength

/* definition of bRequestType */
#define	__TMS_UPROTO_VIDEO_CONTROL_REQUEST_VC	0	/* Interface control */
#define	__TMS_UPROTO_VIDEO_CONTROL_REQUEST_CT	1	/* Camera Terminal */
#define	__TMS_UPROTO_VIDEO_CONTROL_REQUEST_SU	2	/* Selector Unit */
#define	__TMS_UPROTO_VIDEO_CONTROL_REQUEST_PU	3	/* Processing Unit */
#define	__TMS_UPROTO_VIDEO_CONTROL_REQUEST_XU	4	/* Extension Unit */


struct __TMS_STRUCTNAME(UPROTO_VIDEO_CONTROL_REQUEST_HDR)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	};

/* strucure for GET_INFO request */
struct __TMS_STRUCTNAME(UPROTO_VIDEO_CONTROL_GET_INFO_REQUEST)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bmCapabilities;
	};

/* Interface Control Requests */
struct __TMS_STRUCTNAME(UPROTO_VIDEO_VC_REQUEST_POWER_MODE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bDevicePowerMode;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_VC_REQUEST_ERROR_CODE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bRequestErrorCode;
	};

/* Uint and Terminal Control Requests */
struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_SCANNING_MODE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bScanningMode;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_AE_MODE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bAutoExposureMode;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_AE_PRIORITY_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bAutoExposurePriority;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT32		dwExposureTimeAbsolute;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_EXPOSURE_TIME_RELATIVE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bExposureTimeRelative;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_FOCUS_ABSOLUTE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wFocusAbsolute;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_FOCUS_RELATIVE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bFocusRelative;
	__TMS_UINT8		bSpeed;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_FOCUS_AUTO_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bFocusAuto;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_IRIS_ABSOLUTE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wIrisAbsolute;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_IRIS_RELATIVE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bIrisRelative;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_ZOOM_ABSOLUTE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wObjectiveFocalLength;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_ZOOM_RELATIVE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bZoom;
	__TMS_UINT8		bDigitalZoom;
	__TMS_UINT8		bSpeed;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_PANTILT_ABSOLUTE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT32		dwPanAbsolute;
	__TMS_UINT32		dwTiltAbsolute;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_PANTILT_RELATIVE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bPanRelative;
	__TMS_UINT8		bPanSpeed;
	__TMS_UINT8		bTiltRelative;
	__TMS_UINT8		bTiltSpeed;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_ROLL_ABSOLUTE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wRollAbsolute;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_ROLL_RELATIVE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bRollRelative;
	__TMS_UINT8		bSpeed;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CT_PRIVACY_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bPrivacy;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_SU_INPUT_SELECT_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bSelector;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_BACKLIGHT_COMPENSATION_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wBacklightCompensation;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_BRIGHTNESS_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wBrightness;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_CONTRAST_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wContrast;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_GAIN_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wGain;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_POWER_LINE_FREQUENCY_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bPowerLineFrequency;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_HUE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wHue;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_HUE_AUTO_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bHueAuto;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_SATURATION_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wSaturation;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_SHARPNESS_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wSharpness;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_GAMMA_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wGamma;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_WHITE_BALANCE_TEMPERATURE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wWhiteBalanceTemperature;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bWhiteBalanceTemperatureAuto;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_WHITE_BALANCE_COMPONENT_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wWhiteBalanceBlue;
	__TMS_UINT16		wWhiteBalanceRed;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bWhiteBalanceComponentAuto;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_DIGITAL_MULTIPLIER_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wMultiplierStep;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT16		wMultiplierLimit;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_ANALOG_VIDEO_STANDARD_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bVideoStandard;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_PU_ANALOG_LOCK_STATUS_CONTROL)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bStatus;
	};

#ifndef	__TMS_UPROTO_VIDEO_XU_DATA_SIZE		/* PARAM */
# define __TMS_UPROTO_VIDEO_XU_DATA_SIZE	64
#endif

struct __TMS_STRUCTNAME(UPROTO_VIDEO_XU_GENERIC)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		Data[__TMS_UPROTO_VIDEO_XU_DATA_SIZE];
	};


union __TMS_UNIONNAME(UPROTO_VIDEO_CONTROL_REQUEST)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST_HDR			Hdr;
	__TMS_UPROTO_VIDEO_CONTROL_GET_INFO_REQUEST		GetInfo;
	__TMS_UPROTO_VIDEO_VC_REQUEST_POWER_MODE_CONTROL	PowerMode;
	__TMS_UPROTO_VIDEO_VC_REQUEST_ERROR_CODE_CONTROL	ErrorCode;
	__TMS_UPROTO_VIDEO_CT_SCANNING_MODE_CONTROL		Scanning;
	__TMS_UPROTO_VIDEO_CT_AE_MODE_CONTROL			AeMode;
	__TMS_UPROTO_VIDEO_CT_AE_PRIORITY_CONTROL		AePriority;
	__TMS_UPROTO_VIDEO_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL	ExpTimeAbs;
	__TMS_UPROTO_VIDEO_CT_EXPOSURE_TIME_RELATIVE_CONTROL	ExpTimeRel;
	__TMS_UPROTO_VIDEO_CT_FOCUS_ABSOLUTE_CONTROL		FocusAbs;
	__TMS_UPROTO_VIDEO_CT_FOCUS_RELATIVE_CONTROL		FocusRel;
	__TMS_UPROTO_VIDEO_CT_FOCUS_AUTO_CONTROL		FocusAuto;
	__TMS_UPROTO_VIDEO_CT_IRIS_ABSOLUTE_CONTROL		IrisAbs;
	__TMS_UPROTO_VIDEO_CT_IRIS_RELATIVE_CONTROL		IrisRel;
	__TMS_UPROTO_VIDEO_CT_ZOOM_ABSOLUTE_CONTROL		ZoomAbs;
	__TMS_UPROTO_VIDEO_CT_ZOOM_RELATIVE_CONTROL		ZoomRel;
	__TMS_UPROTO_VIDEO_CT_PANTILT_ABSOLUTE_CONTROL		PantiltAbs;
	__TMS_UPROTO_VIDEO_CT_PANTILT_RELATIVE_CONTROL		PantiltRel;
	__TMS_UPROTO_VIDEO_CT_ROLL_ABSOLUTE_CONTROL		RollAbs;
	__TMS_UPROTO_VIDEO_CT_ROLL_RELATIVE_CONTROL		RollRel;
	__TMS_UPROTO_VIDEO_CT_PRIVACY_CONTROL			Privacy;
	__TMS_UPROTO_VIDEO_SU_INPUT_SELECT_CONTROL		InputSelect;
	__TMS_UPROTO_VIDEO_PU_BACKLIGHT_COMPENSATION_CONTROL	Backlight;
	__TMS_UPROTO_VIDEO_PU_BRIGHTNESS_CONTROL		Brightness;
	__TMS_UPROTO_VIDEO_PU_CONTRAST_CONTROL			Contrast;
	__TMS_UPROTO_VIDEO_PU_GAIN_CONTROL			Gain;
	__TMS_UPROTO_VIDEO_PU_POWER_LINE_FREQUENCY_CONTROL	PowerLine;
	__TMS_UPROTO_VIDEO_PU_HUE_CONTROL			Hue;
	__TMS_UPROTO_VIDEO_PU_HUE_AUTO_CONTROL			HueAuto;
	__TMS_UPROTO_VIDEO_PU_SATURATION_CONTROL		Saturation;
	__TMS_UPROTO_VIDEO_PU_SHARPNESS_CONTROL			Sharpness;
	__TMS_UPROTO_VIDEO_PU_GAMMA_CONTROL			Gamma;
	__TMS_UPROTO_VIDEO_PU_WHITE_BALANCE_TEMPERATURE_CONTROL	WhiteTemp;
	__TMS_UPROTO_VIDEO_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL WhiteTempAuto;
	__TMS_UPROTO_VIDEO_PU_WHITE_BALANCE_COMPONENT_CONTROL	WhiteComp;
	__TMS_UPROTO_VIDEO_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL WhiteCompAuto;
	__TMS_UPROTO_VIDEO_PU_DIGITAL_MULTIPLIER_CONTROL	Digital;
	__TMS_UPROTO_VIDEO_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL	DigitalLimit;
	__TMS_UPROTO_VIDEO_PU_ANALOG_VIDEO_STANDARD_CONTROL	AnalogVideo;
	__TMS_UPROTO_VIDEO_PU_ANALOG_LOCK_STATUS_CONTROL	AnalogLock;
	__TMS_UPROTO_VIDEO_XU_GENERIC				Extension;
	};


__TMS_TYPE_DEF_UNION(UPROTO_VIDEO_STREAM_REQUEST);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_STREAM_REQUEST_HDR);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_STREAM_GET_INFO_REQUEST);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_VS_PROBE_COMMIT_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_VS_STILL_PROBE_COMMIT_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_VS_SYNCH_DELAY_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_VS_STILL_IMAGE_TRIGGER_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_VS_GENERATE_KEY_FRAME_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_VS_UPDATE_FRAME_SEGMENT_CONTROL);
__TMS_TYPE_DEF_STRUCT(UPROTO_VIDEO_VS_STREAM_ERROR_CODE_CONTROL);

#define	__TMS_UPROTO_VIDEO_STREAM_REQUEST__HDR		\
	__TMS_UINT8		bControlSelector;	\
	__TMS_UINT8		bRequest;		\
	__TMS_UINT16		wLength;		\
	__TMS_UINT8		bAcceptRequest

struct __TMS_STRUCTNAME(UPROTO_VIDEO_STREAM_REQUEST_HDR)
	{
	__TMS_UPROTO_VIDEO_STREAM_REQUEST__HDR;
	};

/* strucure for GET_INFO request */
struct __TMS_STRUCTNAME(UPROTO_VIDEO_STREAM_GET_INFO_REQUEST)
	{
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR;
	__TMS_UINT8		bmCapabilities;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_VS_PROBE_COMMIT_CONTROL)
	{
	__TMS_UPROTO_VIDEO_STREAM_REQUEST__HDR;
	__TMS_UINT16		bmHint;
	__TMS_UINT8		bFormatIndex;
	__TMS_UINT8		bFrameIndex;
	__TMS_UINT32		dwFrameInterval;
	__TMS_UINT16		wKeyFrameRate;
	__TMS_UINT16		wPFrameRate;
	__TMS_UINT16		wCompQuality;
	__TMS_UINT16		wCompWindowSize;
	__TMS_UINT16		wDelay;
	__TMS_UINT32		dwMaxVideoFrameSize;
	__TMS_UINT32		dwMaxPayloadTransferSize;
	__TMS_UINT32		dwClockFrequency;
	__TMS_UINT8		bmFramingInfo;
	__TMS_UINT8		bPreferedVersion;
	__TMS_UINT8		bMinVersion;
	__TMS_UINT8		bMaxVersion;
	};

__TMS_TYPE_DEF_STRUCT  (UPROTO_VIDEO_VS_PROBE_COMMIT_CONTROL_WIRE);
struct __TMS_STRUCTNAME(UPROTO_VIDEO_VS_PROBE_COMMIT_CONTROL_WIRE)
	{
	__TMS_UINT8		bmHint[2];
	__TMS_UINT8		bFormatIndex;
	__TMS_UINT8		bFrameIndex;
	__TMS_UINT8		dwFrameInterval[4];
	__TMS_UINT8		wKeyFrameRate[2];
	__TMS_UINT8		wPFrameRate[2];
	__TMS_UINT8		wCompQuality[2];
	__TMS_UINT8		wCompWindowSize[2];
	__TMS_UINT8		wDelay[2];
	__TMS_UINT8		dwMaxVideoFrameSize[4];
	__TMS_UINT8		dwMaxPayloadTransferSize[4];
	__TMS_UINT8		dwClockFrequency[4];
	__TMS_UINT8		bmFramingInfo;
	__TMS_UINT8		bPreferedVersion;
	__TMS_UINT8		bMinVersion;
	__TMS_UINT8		bMaxVersion;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_VS_STILL_PROBE_COMMIT_CONTROL)
	{
	__TMS_UPROTO_VIDEO_STREAM_REQUEST__HDR;
	__TMS_UINT8		bFormatIndex;
	__TMS_UINT8		bFrameIndex;
	__TMS_UINT8		bCompressionIndex;
	__TMS_UINT32		dwMaxVideoFrameSize;
	__TMS_UINT32		dwMaxPayloadTransferSize;
	};

__TMS_TYPE_DEF_STRUCT  (UPROTO_VIDEO_VS_STILL_PROBE_COMMIT_CONTROL_WIRE);
struct __TMS_STRUCTNAME(UPROTO_VIDEO_VS_STILL_PROBE_COMMIT_CONTROL_WIRE)
	{
	__TMS_UINT8		bFormatIndex;
	__TMS_UINT8		bFrameIndex;
	__TMS_UINT8		bCompressionIndex;
	__TMS_UINT8		dwMaxVideoFrameSize[4];
	__TMS_UINT8		dwMaxPayloadTransferSize[4];
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_VS_SYNCH_DELAY_CONTROL)
	{
	__TMS_UPROTO_VIDEO_STREAM_REQUEST__HDR;
	__TMS_UINT16		wDelay;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_VS_STILL_IMAGE_TRIGGER_CONTROL)
	{
	__TMS_UPROTO_VIDEO_STREAM_REQUEST__HDR;
	__TMS_UINT8		bTrigger;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_VS_GENERATE_KEY_FRAME_CONTROL)
	{
	__TMS_UPROTO_VIDEO_STREAM_REQUEST__HDR;
	__TMS_UINT8		bGenerateKeyFrame;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_VS_UPDATE_FRAME_SEGMENT_CONTROL)
	{
	__TMS_UPROTO_VIDEO_STREAM_REQUEST__HDR;
	__TMS_UINT8		bStartFrameSegment;
	__TMS_UINT8		bEndFrameSegment;
	};

struct __TMS_STRUCTNAME(UPROTO_VIDEO_VS_STREAM_ERROR_CODE_CONTROL)
	{
	__TMS_UPROTO_VIDEO_STREAM_REQUEST__HDR;
	__TMS_UINT8		bStreamErrorCode;
	};

union __TMS_UNIONNAME(UPROTO_VIDEO_STREAM_REQUEST)
	{
	__TMS_UPROTO_VIDEO_STREAM_REQUEST_HDR			Hdr;
	__TMS_UPROTO_VIDEO_STREAM_GET_INFO_REQUEST		GetInfo;
	__TMS_UPROTO_VIDEO_VS_PROBE_COMMIT_CONTROL		Video;
	__TMS_UPROTO_VIDEO_VS_STILL_PROBE_COMMIT_CONTROL	Still;
	__TMS_UPROTO_VIDEO_VS_SYNCH_DELAY_CONTROL		SyncDelay;
	__TMS_UPROTO_VIDEO_VS_STILL_IMAGE_TRIGGER_CONTROL	StillImage;
	__TMS_UPROTO_VIDEO_VS_GENERATE_KEY_FRAME_CONTROL	GenerateKey;
	__TMS_UPROTO_VIDEO_VS_UPDATE_FRAME_SEGMENT_CONTROL	UpdateFrame;
	__TMS_UPROTO_VIDEO_VS_STREAM_ERROR_CODE_CONTROL		StreamError;
	};


/****************************************************************************\
|
|	Video Protocol Object Definition
|
\****************************************************************************/

struct __TMS_STRUCTNAME(UPROTO_VIDEO)
	{
	/*
	|| Each object begins with a header, including a tag and a size.
	*/
	__TMS_USBPUMP_OBJECT_HEADER		ObjectHeader;

	/*
	|| In & Out bound switch function table
	*/
	__TMS_CONST __TMS_UPROTO_VIDEO_INSWITCH *	pInSwitch;
	__TMS_CONST __TMS_UPROTO_VIDEO_OUTSWITCH *	pOutSwitch;

	/*
	|| Client object and client context
	*/
	__TMS_USBPUMP_OBJECT_HEADER *		pClientObject;
	__TMS_VOID *				pClientContext;

	/*
	|| Device and video control interface
	*/
	__TMS_UDEVICE	*			pDevice;
	__TMS_UINTERFACE *			pVideoCtlIfc;

	/* UBUFQE related stuff */
	__TMS_UBUFQE *				pVideoQeList;
	__TMS_UINT				VideoQeSize;
	__TMS_UINT8				tmoVideoStreamRead;
	__TMS_UINT8				tmoVideoStreamWrite;

	/*
	|| Video device status
	*/
	__TMS_UPROTO_VIDEO_STATUS		VideoStatus;

	/*
	|| Control endpoint handling stuff
	*/
	__TMS_UINT8				fAcceptSetup;
	__TMS_USETUP_HANDLE			hSetup;
	__TMS_USETUP				Setup;
	__TMS_UINT				nControlBuffer;
	__TMS_UINT8 *				pControlBuffer;

	/*
	|| Video stream list head
	*/
	__TMS_UPROTO_VIDEO_STREAM *		pVideoStreamHead;

	/*
	|| Enitry list head
	*/
	__TMS_UPROTO_VIDEO_ENTITY *		pEntityHead;

	/*
	|| pointer to communications video control interface for this function.
	*/
	__TMS_UDATAPLANE			VideoControlPlane;
	__TMS_UDATASTREAM *			pVideoControlStream;

	/*
	|| We want (possibly) a collection of data planes, though normally
	|| there's just one.
	*/
	__TMS_UDATAPLANE *			pVideoStreamDataPlane;

	__TMS_BOOL				VideoUp;

	__TMS_UPROTO_VIDEO_CONTROL_REQUEST	ControlRequest;
	__TMS_UPROTO_VIDEO_STREAM_REQUEST	StreamRequest;

	__TMS_UPROTO_VIDEO_STREAM *		pVideoStream;
	__TMS_UINT				nVideoStream;
	};

/* Video protocol instance tag */
#define	__TMS_UPROTO_VIDEO_TAG	__TMS_UHIL_MEMTAG('V', 'i', 'D', 'o')

/*
|| This expands to something like video.proto-instance.fn.mcci.com
|| you'll normally match on "video.*.fn.mcci.com", or even "video.*.fn.*"
*/
#define	__TMS_UPROTO_VIDEO_NAME				\
	__TMS_USBPUMP_OBJECT_NAME_FN(			\
		__TMS_USBPUMP_OBJECT_NAME_VIDEO, 	\
		"proto-instance"			\
		)


/****************************************************************************\
|
|	Video Protocol Configuration
|
\****************************************************************************/

struct __TMS_STRUCTNAME(UPROTO_VIDEO_CONFIG)
	{
	__TMS_CONST __TMS_UPROTO_VIDEO_INSWITCH *	pInSwitch;
	__TMS_UINT					nControlBufferSize;
	__TMS_UINT					nMaxQueuedBuffers;
	__TMS_UINT8					tmoVideoStreamRead;
	/* XXX: don't need tmoVideoStreamWrite */
	__TMS_UINT8					tmoVideoStreamWrite;
	};

#define	__TMS_UPROTO_VIDEO_CONFIG_INIT_V1(	\
	a_pInSwitch,				\
	a_nControlBufferSize,			\
	a_nMaxQueuedBuffers,			\
	a_tmoVideoStreamRead,			\
	a_tmoVideoStreamWrite			\
	)					\
	{					\
	(a_pInSwitch),				\
	(a_nControlBufferSize),			\
	(a_nMaxQueuedBuffers),			\
	(a_tmoVideoStreamRead),			\
	(a_tmoVideoStreamWrite)			\
	}


/****************************************************************************\
|
|	Video Protocol API Error Code
|
\****************************************************************************/

#define	__TMS_UPROTO_VIDEO_ERROR_OK			(0)
#define	__TMS_UPROTO_VIDEO_ERROR_INVALID_PARAMETER	(1)
#define	__TMS_UPROTO_VIDEO_ERROR_ALREADY_OPEN		(2)
#define	__TMS_UPROTO_VIDEO_ERROR_NOT_OPEN		(3)
#define __TMS_UPROTO_VIDEO_ERROR_INVALID_REQUEST	(4)
#define __TMS_UPROTO_VIDEO_ERROR_NO_BUFQE		(5)
#define __TMS_UPROTO_VIDEO_ERROR_INTERNAL_ERROR		(6)
#define __TMS_UPROTO_VIDEO_ERROR_NO_SUCH_STREAM		(7)
#define __TMS_UPROTO_VIDEO_ERROR_IO_BUSY		(8)
#define __TMS_UPROTO_VIDEO_ERROR_IO_ERROR		(9)
#define __TMS_UPROTO_VIDEO_ERROR_IO_KILL		(10)
#define __TMS_UPROTO_VIDEO_ERROR_IO_STALL		(11)
#define __TMS_UPROTO_VIDEO_ERROR_IO_NOTCFG		(12)

#define	__TMS_UPROTO_VIDEO_ERROR__INIT					\
	{								\
	/* UPROTO_VIDEO_ERROR_OK		*/ "OK",		\
	/* UPROTO_VIDEO_ERROR_INVALID_PARAMETER */ "INVALID_PARAMETER",	\
	/* UPROTO_VIDEO_ERROR_ALREADY_OPEN	*/ "ALREADY_OPEN",	\
	/* UPROTO_VIDEO_ERROR_NOT_OPEN		*/ "NOT_OPEN",		\
	/* UPROTO_VIDEO_ERROR_INVALID_REQUEST	*/ "INVALID_REQUEST",	\
	/* UPROTO_VIDEO_ERROR_NO_BUFQE		*/ "NO_BUFQE",		\
	/* UPROTO_VIDEO_ERROR_INTERNAL_ERROR	*/ "INTERNAL_ERROR",	\
	/* UPROTO_VIDEO_ERROR_NO_SUCH_STREAM	*/ "NO_SUCH_STREAM",	\
	/* UPROTO_VIDEO_ERROR_IO_BUSY		*/ "IO_BUSY",		\
	/* UPROTO_VIDEO_ERROR_IO_ERROR		*/ "IO_ERROR",		\
	/* UPROTO_VIDEO_ERROR_IO_KILL		*/ "IO_KILL",		\
	/* UPROTO_VIDEO_ERROR_IO_STALL		*/ "IO_STALL",		\
	/* UPROTO_VIDEO_ERROR_IO_NOTCFG		*/ "IO_NOTCFG"		\
	}


/****************************************************************************\
|
|	Video protocol inbound switch definition
|
\****************************************************************************/


__TMS_FNTYPE_DEF(
UPROTO_VIDEO_CONTROL_OPEN_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UINT32					/* ErrorCode */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_CONTROL_OPEN_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_CONTROL_OPEN_CB_FN *		/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_VOID *					/* pClientContext */,
	__TMS_CONST __TMS_UPROTO_VIDEO_OUTSWITCH * 	/* pOutSwitch */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_CONTROL_CLOSE_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UINT32					/* ErrorCode */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_CONTROL_CLOSE_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_CONTROL_CLOSE_CB_FN *	/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_CONTROL_REQUEST_REPLY_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UINT32					/* ErrorCode */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_CONTROL_REQUEST_REPLY_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST_REPLY_CB_FN * /* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST *		/* pReply */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_REQUEST_REPLY_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UINT32					/* ErrorCode */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_REQUEST_REPLY_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_STREAM_REQUEST_REPLY_CB_FN *	/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_STREAM_REQUEST *		/* pReply */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_OPEN_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UINT32					/* ErrorCode */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE		/* hVideoStream */
	));

__TMS_FNTYPE_DEF(								\
UPROTO_VIDEO_STREAM_OPEN_FN,							\
__TMS_VOID,									\
	(									\
	__TMS_UPROTO_VIDEO *				/* pVideo */,		\
	__TMS_UPROTO_VIDEO_STREAM_OPEN_CB_FN *		/* pCallback */,	\
	__TMS_VOID *					/* pCallbackCtx */,	\
	__TMS_INT8					/* bInterfaceNumber */,	\
	__TMS_INT8					/* bAlternateSetting */,\
	__TMS_INT8					/* bTerminalLink */,	\
	__TMS_BOOL					/* fTrueIfVideoInput */	\
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_CLOSE_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UINT32					/* ErrorCode */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_CLOSE_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_STREAM_CLOSE_CB_FN *		/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE		/* hVideoStream */
	));

__TMS_FNTYPE_DEF(								\
UPROTO_VIDEO_STREAM_GET_INFO_CB_FN,						\
__TMS_VOID,									\
	(									\
	__TMS_VOID *				/* pCallbackCtx */,		\
	__TMS_UINT32				/* ErrorCode */,		\
	__TMS_UINT8				/* bInterfaceNumber */,		\
	__TMS_UINT8				/* bAlternateSetting */,	\
	__TMS_UINT8				/* bTerminalLink */,		\
	__TMS_UINT16				/* wMaxPacketSize */,		\
	__TMS_UINT16				/* wTransportHeaderSize */	\
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_GET_INFO_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_STREAM_GET_INFO_CB_FN *	/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE		/* hVideoStream */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_READ_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UINT32					/* ErrorCode */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */,
	__TMS_BYTES					/* nActualRead */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_READ_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_STREAM_READ_CB_FN *		/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE		/* hVideoStream */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_READ_V2_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_STREAM_READ_CB_FN *		/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE		/* hVideoStream */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */,
	__TMS_USBPUMP_BUFFER_HANDLE			/* hBuffer */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_WRITE_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UINT32					/* ErrorCode */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_WRITE_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_STREAM_WRITE_CB_FN *		/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE		/* hVideoStream */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */,
	__TMS_BOOL					/* fEndOfData */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_WRITE_V2_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_STREAM_WRITE_CB_FN *		/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE		/* hVideoStream */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */,
	__TMS_USBPUMP_BUFFER_HANDLE			/* hBuffer */,
	__TMS_BOOL					/* fEndOfData */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_READ_ISOCH_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UINT32					/* ErrorCode */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */,
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *		/* pIsochDescr */,
	__TMS_BYTES					/* IsochDescrSize */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_READ_ISOCH_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_STREAM_READ_ISOCH_CB_FN *	/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE		/* hVideoStream */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */,
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *		/* pIsochDescr */,
	__TMS_BYTES					/* IsochDescrSize */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_READ_ISOCH_V2_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_STREAM_READ_ISOCH_CB_FN *	/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE		/* hVideoStream */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */,
	__TMS_USBPUMP_BUFFER_HANDLE			/* hBuffer */,
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *		/* pIsochDescr */,
	__TMS_BYTES					/* IsochDescrSize */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_WRITE_ISOCH_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UINT32					/* ErrorCode */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */,
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *		/* pIsochDescr */,
	__TMS_BYTES					/* IsochDescrSize */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_WRITE_ISOCH_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_STREAM_WRITE_ISOCH_CB_FN *	/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE		/* hVideoStream */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */,
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *		/* pIsochDescr */,
	__TMS_BYTES					/* IsochDescrSize */
	));

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_WRITE_ISOCH_V2_FN,
__TMS_VOID,
	(
	__TMS_UPROTO_VIDEO *				/* pVideo */,
	__TMS_UPROTO_VIDEO_STREAM_WRITE_ISOCH_CB_FN *	/* pCallback */,
	__TMS_VOID *					/* pCallbackCtx */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE		/* hVideoStream */,
	__TMS_UINT8 *					/* pBuffer */,
	__TMS_BYTES					/* nBuffer */,
	__TMS_USBPUMP_BUFFER_HANDLE			/* hBuffer */,
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *		/* pIsochDescr */,
	__TMS_BYTES					/* IsochDescrSize */
	));

struct __TMS_STRUCTNAME(UPROTO_VIDEO_INSWITCH)
	{
	__TMS_UPROTO_VIDEO_CONTROL_OPEN_FN *		pControlOpenFn;
	__TMS_UPROTO_VIDEO_CONTROL_CLOSE_FN *		pControlCloseFn;
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST_REPLY_FN *	pControlRequestReplyFn;
	__TMS_UPROTO_VIDEO_STREAM_OPEN_FN *		pStreamOpenFn;
	__TMS_UPROTO_VIDEO_STREAM_CLOSE_FN *		pStreamCloseFn;
	__TMS_UPROTO_VIDEO_STREAM_GET_INFO_FN * 	pStreamGetInfoFn;
	__TMS_UPROTO_VIDEO_STREAM_READ_V2_FN *		pStreamReadFn;
	__TMS_UPROTO_VIDEO_STREAM_WRITE_V2_FN *		pStreamWriteFn;
	__TMS_UPROTO_VIDEO_STREAM_REQUEST_REPLY_FN *	pStreamRequestReplyFn;
	__TMS_UPROTO_VIDEO_STREAM_READ_ISOCH_V2_FN *	pStreamReadIsochFn;
	__TMS_UPROTO_VIDEO_STREAM_WRITE_ISOCH_V2_FN *	pStreamWriteIsochFn;
	};

#define	__TMS_UPROTO_VIDEO_INSWITCH_INIT_V2(			\
	a_pControlOpenFn,					\
	a_pControlCloseFn,					\
	a_pControlRequestReplyFn,				\
	a_pStreamOpenFn,					\
	a_pStreamCloseFn,					\
	a_pStreamGetInfoFn,					\
	a_pStreamReadFn,					\
	a_pStreamWriteFn,					\
	a_pStreamRequestReplyFn,				\
	a_pStreamReadIsochFn,					\
	a_pStreamWriteIsochFn					\
	)							\
	{							\
	(a_pControlOpenFn),					\
	(a_pControlCloseFn),					\
	(a_pControlRequestReplyFn),				\
	(a_pStreamOpenFn),					\
	(a_pStreamCloseFn),					\
	(a_pStreamGetInfoFn),					\
	(a_pStreamReadFn),					\
	(a_pStreamWriteFn),					\
	(a_pStreamRequestReplyFn),				\
	(a_pStreamReadIsochFn),					\
	(a_pStreamWriteIsochFn)					\
	}

#define	__TMS_UPROTO_VIDEO_INSWITCH_INIT_V1(			\
	a_pControlOpenFn,					\
	a_pControlCloseFn,					\
	a_pControlRequestReplyFn,				\
	a_pStreamOpenFn,					\
	a_pStreamCloseFn,					\
	a_pStreamGetInfoFn,					\
	a_pStreamReadFn,					\
	a_pStreamWriteFn,					\
	a_pStreamRequestReplyFn					\
	)							\
	__TMS_UPROTO_VIDEO_INSWITCH_INIT_V2(			\
		a_pControlOpenFn,				\
		a_pControlCloseFn,				\
		a_pControlRequestReplyFn,			\
		a_pStreamOpenFn,				\
		a_pStreamCloseFn,				\
		a_pStreamGetInfoFn,				\
		a_pStreamReadFn,				\
		a_pStreamWriteFn,				\
		a_pStreamRequestReplyFn,			\
		/* V2: pStreamReadIsochFn */ __TMS_NULL,	\
		/* V2: pStreamWriteIsochFn */ __TMS_NULL	\
		)


/****************************************************************************\
|
|	Video protocol outbound switch definition
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_CONTROL_STATUS_EVENT_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *			/* pClientContext */,
	__TMS_UPROTO_VIDEO_STATUS	/* VideoStatusCode */
	));


__TMS_FNTYPE_DEF(
UPROTO_VIDEO_CONTROL_REQUEST_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *				/* pClientContext */,
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST *	/* pRequest */
	));
/*
|| When client get this callback, client should provide control endpoint reply
|| using UPROTO_VIDEO_CONTROL_REQUEST_REPLY_FN
*/

__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_STATUS_EVENT_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *				/* pClientContext */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE	/* hVideoStream */,
	__TMS_BOOL				/* fActivate */
	));


__TMS_FNTYPE_DEF(
UPROTO_VIDEO_STREAM_REQUEST_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *				/* pClientContext */,
	__TMS_UPROTO_VIDEO_STREAM_HANDLE	/* hVideoStream */,
	__TMS_UPROTO_VIDEO_STREAM_REQUEST *	/* pRequest */
	));
/*
|| When client get this callback, client should provide control endpoint reply
|| using UPROTO_VIDEO_STREAM_REQUEST_REPLY_FN
*/

struct __TMS_STRUCTNAME(UPROTO_VIDEO_OUTSWITCH)
	{

	__TMS_UPROTO_VIDEO_CONTROL_STATUS_EVENT_CB_FN *	pControlStatusEventFn;
	__TMS_UPROTO_VIDEO_CONTROL_REQUEST_CB_FN *	pControlRequestFn;
	__TMS_UPROTO_VIDEO_STREAM_STATUS_EVENT_CB_FN *	pStreamStatusEventFn;
	__TMS_UPROTO_VIDEO_STREAM_REQUEST_CB_FN *	pStreamRequestFn;
	};

#define	__TMS_UPROTO_VIDEO_OUTSWITCH_INIT_V1(		\
	a_pControlStatusEventFn,			\
	a_pControlRequestFn,				\
	a_pStreamStatusEventFn,				\
	a_pStreamRequestFn				\
	)						\
	{						\
	(a_pControlStatusEventFn),			\
	(a_pControlRequestFn),				\
	(a_pStreamStatusEventFn),			\
	(a_pStreamRequestFn)				\
	}


/****************************************************************************\
|
|	Default video class protocol inswitch
|
\****************************************************************************/

extern __TMS_CONST __TMS_UPROTO_VIDEO_INSWITCH	gk_UsbPumpVideo_InSwitch;


/****************************************************************************\
|
|	API Function
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_PROTOCOL_PROBE_FN		UsbPumpVideo_ProtocolProbe;
__TMS_USBPUMP_PROTOCOL_CREATE_FN	UsbPumpVideo_ProtocolCreate;
__TMS_USBPUMP_PROTOCOL_ADD_INTERFACE_FN	UsbPumpVideo_ProtocolAddInterface;

__TMS_CONST __TMS_TEXT *
UsbPumpVideo_GetStatusName(
	__TMS_UPROTO_VIDEO_STATUS	/* VideoStatusCode */
	);

__TMS_CONST __TMS_TEXT *
UsbPumpVideo_GetErrorName(
	__TMS_UINT32			/* ErrorCode */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_OBJECT_NAME_VIDEO	\
   __TMS_USBPUMP_OBJECT_NAME_VIDEO
# define USBPUMP_OBJECT_NAME_ENUM_VIDEO	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_VIDEO
# define UPROTO_VIDEO_STREAM_INVALID_HANDLE	\
   __TMS_UPROTO_VIDEO_STREAM_INVALID_HANDLE
# define UPROTO_VIDEO_STATUS_START	\
   __TMS_UPROTO_VIDEO_STATUS_START
# define UPROTO_VIDEO_STATUS_SUSPEND_WITH_WAKEUP	\
   __TMS_UPROTO_VIDEO_STATUS_SUSPEND_WITH_WAKEUP
# define UPROTO_VIDEO_STATUS_SUSPEND_WITHOUT_WAKEUP	\
   __TMS_UPROTO_VIDEO_STATUS_SUSPEND_WITHOUT_WAKEUP
# define UPROTO_VIDEO_STATUS_RESUME	\
   __TMS_UPROTO_VIDEO_STATUS_RESUME
# define UPROTO_VIDEO_STATUS_RESET	\
   __TMS_UPROTO_VIDEO_STATUS_RESET
# define UPROTO_VIDEO_STATUS_ATTACH	\
   __TMS_UPROTO_VIDEO_STATUS_ATTACH
# define UPROTO_VIDEO_STATUS_DETACH	\
   __TMS_UPROTO_VIDEO_STATUS_DETACH
# define UPROTO_VIDEO_STATUS_ACTIVATE	\
   __TMS_UPROTO_VIDEO_STATUS_ACTIVATE
# define UPROTO_VIDEO_STATUS_DEACTIVATE	\
   __TMS_UPROTO_VIDEO_STATUS_DEACTIVATE
# define UPROTO_VIDEO_STATUS__INIT	\
   __TMS_UPROTO_VIDEO_STATUS__INIT
# define UPROTO_VIDEO_CONTROL_REQUEST__HDR	\
   __TMS_UPROTO_VIDEO_CONTROL_REQUEST__HDR
# define UPROTO_VIDEO_CONTROL_REQUEST_VC	\
   __TMS_UPROTO_VIDEO_CONTROL_REQUEST_VC
# define UPROTO_VIDEO_CONTROL_REQUEST_CT	\
   __TMS_UPROTO_VIDEO_CONTROL_REQUEST_CT
# define UPROTO_VIDEO_CONTROL_REQUEST_SU	\
   __TMS_UPROTO_VIDEO_CONTROL_REQUEST_SU
# define UPROTO_VIDEO_CONTROL_REQUEST_PU	\
   __TMS_UPROTO_VIDEO_CONTROL_REQUEST_PU
# define UPROTO_VIDEO_CONTROL_REQUEST_XU	\
   __TMS_UPROTO_VIDEO_CONTROL_REQUEST_XU
# define UPROTO_VIDEO_XU_DATA_SIZE	\
   __TMS_UPROTO_VIDEO_XU_DATA_SIZE
# define UPROTO_VIDEO_STREAM_REQUEST__HDR	\
   __TMS_UPROTO_VIDEO_STREAM_REQUEST__HDR
# define UPROTO_VIDEO_TAG	\
   __TMS_UPROTO_VIDEO_TAG
# define UPROTO_VIDEO_NAME	\
   __TMS_UPROTO_VIDEO_NAME
# define UPROTO_VIDEO_CONFIG_INIT_V1(	\
	a_pInSwitch,				\
	a_nControlBufferSize,			\
	a_nMaxQueuedBuffers,			\
	a_tmoVideoStreamRead,			\
	a_tmoVideoStreamWrite			\
	)	\
	__TMS_UPROTO_VIDEO_CONFIG_INIT_V1(	\
	a_pInSwitch,				\
	a_nControlBufferSize,			\
	a_nMaxQueuedBuffers,			\
	a_tmoVideoStreamRead,			\
	a_tmoVideoStreamWrite			\
	)
# define UPROTO_VIDEO_ERROR_OK	\
   __TMS_UPROTO_VIDEO_ERROR_OK
# define UPROTO_VIDEO_ERROR_INVALID_PARAMETER	\
   __TMS_UPROTO_VIDEO_ERROR_INVALID_PARAMETER
# define UPROTO_VIDEO_ERROR_ALREADY_OPEN	\
   __TMS_UPROTO_VIDEO_ERROR_ALREADY_OPEN
# define UPROTO_VIDEO_ERROR_NOT_OPEN	\
   __TMS_UPROTO_VIDEO_ERROR_NOT_OPEN
# define UPROTO_VIDEO_ERROR_INVALID_REQUEST	\
   __TMS_UPROTO_VIDEO_ERROR_INVALID_REQUEST
# define UPROTO_VIDEO_ERROR_NO_BUFQE	\
   __TMS_UPROTO_VIDEO_ERROR_NO_BUFQE
# define UPROTO_VIDEO_ERROR_INTERNAL_ERROR	\
   __TMS_UPROTO_VIDEO_ERROR_INTERNAL_ERROR
# define UPROTO_VIDEO_ERROR_NO_SUCH_STREAM	\
   __TMS_UPROTO_VIDEO_ERROR_NO_SUCH_STREAM
# define UPROTO_VIDEO_ERROR_IO_BUSY	\
   __TMS_UPROTO_VIDEO_ERROR_IO_BUSY
# define UPROTO_VIDEO_ERROR_IO_ERROR	\
   __TMS_UPROTO_VIDEO_ERROR_IO_ERROR
# define UPROTO_VIDEO_ERROR_IO_KILL	\
   __TMS_UPROTO_VIDEO_ERROR_IO_KILL
# define UPROTO_VIDEO_ERROR_IO_STALL	\
   __TMS_UPROTO_VIDEO_ERROR_IO_STALL
# define UPROTO_VIDEO_ERROR_IO_NOTCFG	\
   __TMS_UPROTO_VIDEO_ERROR_IO_NOTCFG
# define UPROTO_VIDEO_ERROR__INIT	\
   __TMS_UPROTO_VIDEO_ERROR__INIT
# define UPROTO_VIDEO_INSWITCH_INIT_V2(			\
	a_pControlOpenFn,					\
	a_pControlCloseFn,					\
	a_pControlRequestReplyFn,				\
	a_pStreamOpenFn,					\
	a_pStreamCloseFn,					\
	a_pStreamGetInfoFn,					\
	a_pStreamReadFn,					\
	a_pStreamWriteFn,					\
	a_pStreamRequestReplyFn,				\
	a_pStreamReadIsochFn,					\
	a_pStreamWriteIsochFn					\
	)	\
	__TMS_UPROTO_VIDEO_INSWITCH_INIT_V2(			\
	a_pControlOpenFn,					\
	a_pControlCloseFn,					\
	a_pControlRequestReplyFn,				\
	a_pStreamOpenFn,					\
	a_pStreamCloseFn,					\
	a_pStreamGetInfoFn,					\
	a_pStreamReadFn,					\
	a_pStreamWriteFn,					\
	a_pStreamRequestReplyFn,				\
	a_pStreamReadIsochFn,					\
	a_pStreamWriteIsochFn					\
	)
# define UPROTO_VIDEO_INSWITCH_INIT_V1(			\
	a_pControlOpenFn,					\
	a_pControlCloseFn,					\
	a_pControlRequestReplyFn,				\
	a_pStreamOpenFn,					\
	a_pStreamCloseFn,					\
	a_pStreamGetInfoFn,					\
	a_pStreamReadFn,					\
	a_pStreamWriteFn,					\
	a_pStreamRequestReplyFn					\
	)	\
	__TMS_UPROTO_VIDEO_INSWITCH_INIT_V1(			\
	a_pControlOpenFn,					\
	a_pControlCloseFn,					\
	a_pControlRequestReplyFn,				\
	a_pStreamOpenFn,					\
	a_pStreamCloseFn,					\
	a_pStreamGetInfoFn,					\
	a_pStreamReadFn,					\
	a_pStreamWriteFn,					\
	a_pStreamRequestReplyFn					\
	)
# define UPROTO_VIDEO_OUTSWITCH_INIT_V1(		\
	a_pControlStatusEventFn,			\
	a_pControlRequestFn,				\
	a_pStreamStatusEventFn,				\
	a_pStreamRequestFn				\
	)	\
	__TMS_UPROTO_VIDEO_OUTSWITCH_INIT_V1(		\
	a_pControlStatusEventFn,			\
	a_pControlRequestFn,				\
	a_pStreamStatusEventFn,				\
	a_pStreamRequestFn				\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of protovideo.h ****/
#endif /* _PROTOVIDEO_H_ */
