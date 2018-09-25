/* usbvideo15.h	Wed Aug 09 2017 14:02:16 chwon */

/*

Module:  usbvideo15.h

Function:
	Definition of USB Video Class V1.5 constants

Version:
	V3.21c	Wed Aug 09 2017 14:02:16 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	April 2017

Revision history:
   3.21a  Wed Apr 05 2017 10:12:23 chwon
	20517: Module created.

   3.21c  Wed Aug 09 2017 14:02:16  chwon
	20634: Fix bcdUVC value definition and change bmLayoutPerStream type.

*/

#ifndef _USBVIDEO15_H_	/* prevent multiple includes */
#define _USBVIDEO15_H_

#ifndef _USBVIDEO11_H_
# include "usbvideo11.h"
#endif

/****************************************************************************\
|
|	Video class, subclass, and protocol codes. (A.1, A.2, A.3)
|
\****************************************************************************/

/* Interface protocol */
#define	USB_bInterfaceProtocol_Video15		0x01	/* Protocol 1.5 */


/****************************************************************************\
|
|	Video Class-Specific Functional Descriptors (A.4, A.5, A.6)
|
\****************************************************************************/

/*
|| Video control interfce descriptor subtypes
*/
#define USB_bDescriptorSubtype_VideoControl_ENCODING_UNIT	0x07

#define	USB_bDescriptorSubtype_VideoControl_V1_5__INIT	\
	USB_bDescriptorSubtype_VideoControl__INIT,	\
	{ USB_bDescriptorSubtype_VideoControl_ENCODING_UNIT, "EncodingUnit" }


/*
|| Video streamming interfce descriptor subtypes
*/
#define USB_bDescriptorSubtype_VideoStreaming_FORMAT_H264		0x13
#define USB_bDescriptorSubtype_VideoStreaming_FRAME_H264		0x14
#define USB_bDescriptorSubtype_VideoStreaming_FORMAT_H264_SIMULCAST	0x15
#define USB_bDescriptorSubtype_VideoStreaming_FORMAT_VP8		0x16
#define USB_bDescriptorSubtype_VideoStreaming_FRAME_VP8			0x17
#define USB_bDescriptorSubtype_VideoStreaming_FORMAT_VP8_SIMULCAST	0x18

#define	USB_bDescriptorSubtype_VideoStreaming_V1_5__INIT	\
	USB_bDescriptorSubtype_VideoStreaming__INIT,		\
	{ USB_bDescriptorSubtype_VideoStreaming_FORMAT_H264, "FormatH264" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FRAME_H264, "FrameH264" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FORMAT_H264_SIMULCAST, "FormatH264Simulcast" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FORMAT_VP8, "FormatVp8" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FRAME_VP8, "FrameVp8" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FORMAT_VP8_SIMULCAST, "FormatVp8Simulcast" }


/****************************************************************************\
|
|	Video Class-Specific Request Codes (A.8)
|
\****************************************************************************/

#define	USB_bRequest_Video_GET_CUR_ALL	0x91	/* get current all */
#define	USB_bRequest_Video_GET_MIN_ALL	0x92	/* get minimum all */
#define	USB_bRequest_Video_GET_MAX_ALL	0x93	/* get maximum all */
#define	USB_bRequest_Video_GET_RES_ALL	0x94	/* get resolution all */
#define	USB_bRequest_Video_GET_DEF_ALL	0x97	/* get default space all */

#define	USB_bRequest_Video_V1_5__INIT	\
	USB_bRequest_Video__INIT,	\
	{ USB_bRequest_Video_GET_CUR_ALL, "GET_CUR_ALL"	},	\
	{ USB_bRequest_Video_GET_MIN_ALL, "GET_MIN_ALL"	},	\
	{ USB_bRequest_Video_GET_MAX_ALL, "GET_MAX_ALL"	},	\
	{ USB_bRequest_Video_GET_RES_ALL, "GET_RES_ALL"	},	\
	{ USB_bRequest_Video_GET_DEF_ALL, "GET_DEF_ALL"}


/****************************************************************************\
|
|	Control Selector Codes (A.9)
|
\****************************************************************************/

/* A.9.4 Camera Terminal Control Selectors */
#define	USBVIDEO_bControlSelector_CT_FOCUS_SIMPLE_CONTROL	0x12
#define	USBVIDEO_bControlSelector_CT_WINDOW_CONTROL		0x13
#define	USBVIDEO_bControlSelector_CT_REGION_OF_INTEREST_CONTROL	0x14

/* A.9.5 Processing Unit Control Selectors */
#define	USBVIDEO_bControlSelector_PU_CONTRAST_AUTO_CONTROL	0x13

/* A.9.6 Encoding Unit Control Selectors */
#define	USBVIDEO_bControlSelector_EU_CONTROL_UNDEFINED		0x00
#define	USBVIDEO_bControlSelector_EU_SELECT_LAYER_CONTROL	0x01
#define	USBVIDEO_bControlSelector_EU_PROFILE_TOOLSET_CONTROL	0x02
#define	USBVIDEO_bControlSelector_EU_VIDEO_RESOLUTION_CONTROL	0x03
#define	USBVIDEO_bControlSelector_EU_MIN_FRAME_INTERVAL_CONTROL	0x04
#define	USBVIDEO_bControlSelector_EU_SLICE_MODE_CONTROL		0x05
#define	USBVIDEO_bControlSelector_EU_RATE_CONTROL_MODE_CONTROL	0x06
#define	USBVIDEO_bControlSelector_EU_AVERAGE_BITRATE_CONTROL	0x07
#define	USBVIDEO_bControlSelector_EU_CPB_SIZE_CONTROL		0x08
#define	USBVIDEO_bControlSelector_EU_PEAK_BIT_RATE_CONTROL	0x09
#define	USBVIDEO_bControlSelector_EU_QUANTIZATION_PARAMS_CONTROL 0x0A
#define	USBVIDEO_bControlSelector_EU_SYNC_REF_FRAME_CONTROL	0x0B
#define	USBVIDEO_bControlSelector_EU_LTR_BUFFER_CONTROL		0x0C
#define	USBVIDEO_bControlSelector_EU_LTR_PICTURE_CONTROL	0x0D
#define	USBVIDEO_bControlSelector_EU_LTR_VALIDATION_CONTROL	0x0E
#define	USBVIDEO_bControlSelector_EU_LEVEL_IDC_LIMIT_CONTROL	0x0F
#define	USBVIDEO_bControlSelector_EU_SEI_PAYLOADTYPE_CONTROL	0x10
#define	USBVIDEO_bControlSelector_EU_QP_RANGE_CONTROL		0x11
#define	USBVIDEO_bControlSelector_EU_PRIORITY_CONTROL		0x12
#define	USBVIDEO_bControlSelector_EU_START_OR_STOP_LAYER_CONTROL 0x13
#define	USBVIDEO_bControlSelector_EU_ERROR_RESILIENCY_CONTROL	0x14


/****************************************************************************\
|
|	The status packet format (2.4.2.2)
|
\****************************************************************************/

#define	USBVIDEO_bAttribute_ControlMinChange	3
#define	USBVIDEO_bAttribute_ControlMaxChange	4


/****************************************************************************\
|
|	The header functional descriptor (3.7.2)
|
\****************************************************************************/

#define	USBVIDEO_bcdUVC_V1_5	0x0150


/****************************************************************************\
|
|	The encoding unit descriptor (3.7.2.6)
|
\****************************************************************************/

struct USB_VideoControl_FunctionalDescriptor_ENCODING_UNIT
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	bSourceID;
	unsigned char	iEncoding;
	unsigned char	bControlSize;	/* Must be 3 */
	unsigned char	bmControls[3];
	unsigned char	bmControlsRuntime[3];
	};

typedef struct USB_VideoControl_FunctionalDescriptor_ENCODING_UNIT
	USB_VideoControl_EncodingUnitDescriptor;


/****************************************************************************\
|
|	USB Device Class Definition for Video Devices: H.264 Payload
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_H264
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFormatIndex;
	unsigned char	bNumFrameDescriptors;
	unsigned char	bDefaultFrameIndex;
	unsigned char	bMaxCodecConfigDelay;
	unsigned char	bmSupportedSliceModes;
	unsigned char	bmSupportedSyncFrameTypes;
	unsigned char	bResolutionScaling;
	unsigned char	Reserved1;
	unsigned char	bmSupportedRateControlModes;
	unsigned char	wMaxMBperSecOneResolutionNoScalability[2];
	unsigned char	wMaxMBperSecThreeResolutionsNoScalability[2];
	unsigned char	wMaxMBperSecFourResolutionsNoScalability[2];
	unsigned char	wMaxMBperSecOneResolutionTemporalScalability[2];
	unsigned char	wMaxMBperSecTwoResolutionsTemporalScalablility[2];
	unsigned char	wMaxMBperSecThreeResolutionsTemporalScalability[2];
	unsigned char	wMaxMBperSecFourResolutionsTemporalScalability[2];
	unsigned char	wMaxMBperSecOneResolutionTemporalQualityScalability[2];
	unsigned char	wMaxMBperSecTwoResolutionsTemporalQualityScalability[2];
	unsigned char	wMaxMBperSecThreeResolutionsTemporalQualityScalablity[2];
	unsigned char	wMaxMBperSecFourResolutionsTemporalQualityScalability[2];
	unsigned char	wMaxMBperSecOneResolutionsTemporalSpatialScalability[2];
	unsigned char	wMaxMBperSecTwoResolutionsTemporalSpatialScalability[2];
	unsigned char	wMaxMBperSecThreeResolutionsTemporalSpatialScalability[2];
	unsigned char	wMaxMBperSecFourResolutionsTemporalSpatialScalability[2];
	unsigned char	wMaxMBperSecOneResolutionFullScalability[2];
	unsigned char	wMaxMBperSecTwoResolutionsFullScalability[2];
	unsigned char	wMaxMBperSecThreeResolutionsFullScalability[2];
	unsigned char	wMaxMBperSecFourResolutionsFullScalability[2];
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_H264
	USB_VideoStreaming_FORMAT_H264;


struct USB_VideoStreaming_FunctionalDescriptor_FRAME_H264
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFrameIndex;
	unsigned char	wWidth[2];
	unsigned char	wHeight[2];
	unsigned char	wSARwidth[2];
	unsigned char	wSARheight[2];
	unsigned char	wProfile[2];
	unsigned char	bLevelIDC;
	unsigned char	wConstrainedToolset[2];
	unsigned char	bmSupportedUsages[4];
	unsigned char	bmCapabilities[2];
	unsigned char	bmSVCCapabilities[4];
	unsigned char	bmMVCCapabilities[4];
	unsigned char	dwMinBitRate[4];
	unsigned char	dwMaxBitRate[4];
	unsigned char	dwDefaultFrameInterval[4];
	unsigned char	bNumFrameIntervals;
	unsigned char	dwFrameInterval[4];
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FRAME_H264
	USB_VideoStreaming_FRAME_H264;


/****************************************************************************\
|
|	USB Device Class Definition for Video Devices: VP8 Payload
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_VP8
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFormatIndex;
	unsigned char	bNumFrameDescriptors;
	unsigned char	bDefaultFrameIndex;
	unsigned char	bMaxCodecConfigDelay;
	unsigned char	bSupportedPartitionCount;
	unsigned char	bmSupportedSyncFrameTypes;
	unsigned char	bResolutionScaling;
	unsigned char	bmSupportedRateControlModes;
	unsigned char	wMaxMBperSec[2];
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_VP8
	USB_VideoStreaming_FORMAT_VP8;


struct USB_VideoStreaming_FunctionalDescriptor_FRAME_VP8
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFrameIndex;
	unsigned char	wWidth[2];
	unsigned char	wHeight[2];
	unsigned char	bmSupportedUsages[4];
	unsigned char	bmCapabilities[2];
	unsigned char	bmScalabilityCapabilities[4];
	unsigned char	dwMinBitRate[4];
	unsigned char	dwMaxBitRate[4];
	unsigned char	dwDefaultFrameInterval[4];
	unsigned char	bNumFrameIntervals;
	unsigned char	dwFrameInterval[4];
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FRAME_VP8
	USB_VideoStreaming_FRAME_VP8;


/****************************************************************************\
|
|	Video Control Requests (4.2)
|
\****************************************************************************/

/* 4.1.2  Get_INFO request */
/* bmCapabilities */
#define	USBVIDEO_bmCapabilities_DISABLED_INCOMP	(1u << 5)

/* 4.2.2.1.8 Focus, Simple Control */
struct USB_VideoControlRequest_FocusSimpleControl_s
	{
	unsigned char	bFocus;
	};

typedef struct USB_VideoControlRequest_FocusSimpleControl_s
	USB_VideoControlRequest_FocusSimpleControl;

/* 4.2.2.1.19 Digital Window Control */
struct USB_VideoControlRequest_DigitalWindowControl_s
	{
	unsigned char	wWindow_Top[2];
	unsigned char	wWindow_Left[2];
	unsigned char	wWindow_Bottom[2];
	unsigned char	wWindow_Right[2];
	unsigned char	wNumSteps[2];
	unsigned char	bmNumStepsUnits[2];
	};

typedef struct USB_VideoControlRequest_DigitalWindowControl_s
	USB_VideoControlRequest_DigitalWindowControl;

/* 4.2.2.1.20 Digital Region of Interest (ROI) Control */
struct USB_VideoControlRequest_DigitalRegionInterest_s
	{
	unsigned char	wROI_Top[2];
	unsigned char	wROI_Left[2];
	unsigned char	wROI_Bottom[2];
	unsigned char	wROI_Right[2];
	unsigned char	bmAutoControls[2];
	};

typedef struct USB_VideoControlRequest_DigitalRegionInterest_s
	USB_VideoControlRequest_DigitalRegionInterest;

/* 4.2.2.3.4 Contrast, Auto Control */
struct USB_VideoControlRequest_ContrastAutoControl_s
	{
	unsigned char	bContrastAuto;
	};

typedef struct USB_VideoControlRequest_ContrastAutoControl_s
	USB_VideoControlRequest_ContrastAutoControl;

/* 4.2.2.4.2 Select Layer Control */
struct USB_VideoControlRequest_SelectLayerControl_s
	{
	unsigned char	wLayerOrViewID[2];
	};

typedef struct USB_VideoControlRequest_SelectLayerControl_s
	USB_VideoControlRequest_SelectLayerControl;

/* 4.2.2.4.3 Video Resolution Control */
struct USB_VideoControlRequest_VideoResolutionControl_s
	{
	unsigned char	wWidth[2];
	unsigned char	wHeight[2];
	};

typedef struct USB_VideoControlRequest_VideoResolutionControl_s
	USB_VideoControlRequest_VideoResolutionControl;

/* 4.2.2.4.4 Profile and Toolset Control */
struct USB_VideoControlRequest_ProfileToolsetControl_s
	{
	unsigned char	wProfile[2];
	unsigned char	wConstrainedToolset[2];
	unsigned char	bmSettings[4];
	};

typedef struct USB_VideoControlRequest_ProfileToolsetControl_s
	USB_VideoControlRequest_ProfileToolsetControl;

/* 4.2.2.4.5 Minimum Frame Interval Control */
struct USB_VideoControlRequest_MinimumFrameIntervalControl_s
	{
	unsigned char	dwFrameInterval[4];
	};

typedef struct USB_VideoControlRequest_MinimumFrameIntervalControl_s
	USB_VideoControlRequest_MinimumFrameIntervalControl;

/* 4.2.2.4.6 Slice Mode Control */
struct USB_VideoControlRequest_SliceModeControl_s
	{
	unsigned char	wSliceMode[2];
	unsigned char	wSliceConfigSetting[2];
	};

typedef struct USB_VideoControlRequest_SliceModeControl_s
	USB_VideoControlRequest_SliceModeControl;

/* 4.2.2.4.7 Rate Control Mode Control */
struct USB_VideoControlRequest_RateControlModeControl_s
	{
	unsigned char	bRateControlMode;
	};

typedef struct USB_VideoControlRequest_RateControlModeControl_s
	USB_VideoControlRequest_RateControlModeControl;

/* 4.2.2.4.8 Average Bit Rate Control */
struct USB_VideoControlRequest_AverageBitRateControl_s
	{
	unsigned char	dwAverageBitRate[4];
	};

typedef struct USB_VideoControlRequest_AverageBitRateControl_s
	USB_VideoControlRequest_AverageBitRateControl;

/* 4.2.2.4.9 CPB Size Control */
struct USB_VideoControlRequest_CPBSizeControl_s
	{
	unsigned char	dwCPBsize[4];
	};

typedef struct USB_VideoControlRequest_CPBSizeControl_s
	USB_VideoControlRequest_CPBSizeControl;

/* 4.2.2.4.10 Peak Bit Rate Control */
struct USB_VideoControlRequest_PeakBitRateControl_s
	{
	unsigned char	dwPeakBitRate[4];
	};

typedef struct USB_VideoControlRequest_PeakBitRateControl_s
	USB_VideoControlRequest_PeakBitRateControl;

/* 4.2.2.4.11 Quantization Parameter Control */
struct USB_VideoControlRequest_QuantizationParameterControl_s
	{
	unsigned char	wQpPrime_I[2];
	unsigned char	wQpPrime_P[2];
	unsigned char	wQpPrime_B[2];
	};

typedef struct USB_VideoControlRequest_QuantizationParameterControl_s
	USB_VideoControlRequest_QuantizationParameterControl;

/* 4.2.2.4.12 Quantization Parameter Range Control */
struct USB_VideoControlRequest_QuantizationParameterRangeControl_s
	{
	unsigned char	bMinQp;
	unsigned char	bMaxQp;
	};

typedef struct USB_VideoControlRequest_QuantizationParameterRangeControl_s
	USB_VideoControlRequest_QuantizationParameterRangeControl;

/* 4.2.2.4.13 Synchronization and Long Term Reference Frame Control */
struct USB_VideoControlRequest_SynchronizationLongTermReferenceFrameControl_s
	{
	unsigned char	bSyncFrameType;
	unsigned char	wSyncFrameInterval[2];
	unsigned char	bGradualDecoderRefresh;
	};

typedef struct USB_VideoControlRequest_SynchronizationLongTermReferenceFrameControl_s
	USB_VideoControlRequest_SynchronizationLongTermReferenceFrameControl;

/* 4.2.2.4.14 Long-Term Buffer Control */
struct USB_VideoControlRequest_LongTermBufferControl_s
	{
	unsigned char	bNumHostControlLTRBuffers;
	unsigned char	bTrustMode;
	};

typedef struct USB_VideoControlRequest_LongTermBufferControl_s
	USB_VideoControlRequest_LongTermBufferControl;

/* 4.2.2.4.15 Long-Term Reference Picture Control */
struct USB_VideoControlRequest_LongTermReferencePictureControl_s
	{
	unsigned char	bPutAtPositionInLTRBuffer;
	unsigned char	bLTRMode;
	};

typedef struct USB_VideoControlRequest_LongTermReferencePictureControl_s
	USB_VideoControlRequest_LongTermReferencePictureControl;

/* 4.2.2.4.16 Long-Term Reference Validation Control */
struct USB_VideoControlRequest_LongTermReferenceValidationControl_s
	{
	unsigned char	bmValidLTRs[2];
	};

typedef struct USB_VideoControlRequest_LongTermReferenceValidationControl_s
	USB_VideoControlRequest_LongTermReferenceValidationControl;

/* 4.2.2.4.17 SEI Messages Control */
struct USB_VideoControlRequest_SEIMessagesControl_s
	{
	unsigned char	bmSEIMessages[8];
	};

typedef struct USB_VideoControlRequest_SEIMessagesControl_s
	USB_VideoControlRequest_SEIMessagesControl;

/* 4.2.2.4.18 Priority Control */
struct USB_VideoControlRequest_PriorityControl_s
	{
	unsigned char	bPriority;
	};

typedef struct USB_VideoControlRequest_PriorityControl_s
	USB_VideoControlRequest_PriorityControl;

/* 4.2.2.4.19 Start or Stop Layer Control */
struct USB_VideoControlRequest_StartStopLayerControl_s
	{
	unsigned char	bUpdate;
	};

typedef struct USB_VideoControlRequest_StartStopLayerControl_s
	USB_VideoControlRequest_StartStopLayerControl;

/* 4.2.2.4.20 Level IDC Control */
struct USB_VideoControlRequest_LevelIDCControl_s
	{
	unsigned char	bLevelIDC;
	};

typedef struct USB_VideoControlRequest_LevelIDCControl_s
	USB_VideoControlRequest_LevelIDCControl;

/* 4.2.2.4.21 Error Resiliency Control */
struct USB_VideoControlRequest_ErrorResiliencyControl_s
	{
	unsigned char	bmErrorResiliencyFeatures;
	};

typedef struct USB_VideoControlRequest_ErrorResiliencyControl_s
	USB_VideoControlRequest_ErrorResiliencyControl;


/****************************************************************************\
|
|	Video Streaming Requests (4.3)
|
\****************************************************************************/

/* 4.3.1 Interface Control Requests */
/* 4.3.1.1 Video Probe and Commit Controls */
struct USB_VideoStreamRequest_ProbeCommitControlV15_s
	{
	unsigned char	bmHint[2];
	unsigned char	bFormatIndex;
	unsigned char	bFrameIndex;
	unsigned char	dwFrameInterval[4];
	unsigned char	wKeyFrameRate[2];
	unsigned char	wPFrameRate[2];
	unsigned char	wCompQuality[2];
	unsigned char	wCompWindowSize[2];
	unsigned char	wDelay[2];
	unsigned char	dwMaxVideoFrameSize[4];
	unsigned char	dwMaxPayloadTransferSize[4];
	unsigned char	dwClockFrequency[4];
	unsigned char	bmFramingInfo;
	unsigned char	bPreferedVersion;
	unsigned char	bMinVersion;
	unsigned char	bMaxVersion;
	unsigned char	bUsage;
	unsigned char	bBitDepthLuma;
	unsigned char	bmSettings;
	unsigned char	bMaxNumberOfRefFramesPlus1;
	unsigned char	bmRateControlModes[2];
	unsigned char	bmLayoutPerStream[4][2];
	};

typedef struct USB_VideoStreamRequest_ProbeCommitControlV15_s
	USB_VideoStreamRequest_ProbeCommitControlV15;


/**** end of usbvideo15.h ****/
#endif /* _USBVIDEO15_H_ */
