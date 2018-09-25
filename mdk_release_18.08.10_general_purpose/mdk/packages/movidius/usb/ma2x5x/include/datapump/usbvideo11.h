/* usbvideo11.h	Wed Aug 09 2017 14:01:35 chwon */

/*

Module:  usbvideo11.h

Function:
	USB Video Class constants

Version:
	V3.21c	Wed Aug 09 2017 14:01:35 chwon	Edit level 6

Copyright notice:
	This file copyright (C) 2007-2008, 2012, 2017 by

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
	Module Created

   1.97k  Tue Sep 16 2008 12:36:19  chwon
	Fix doc-xml error

   3.01f  Tue Jul 24 2012 09:53:57  chwon
	Fixed documentation.

   3.19b  Thu Jan 12 2017 15:26:49  chwon
	20422: Add USB_VideoControl_FunctionalDescriptor_OUTPUT_TERMINAL
	structure.

   3.21a  Wed Apr 05 2017 10:12:23  chwon
	20517: Add missing code definition and add more descriptors.

   3.21c  Wed Aug 09 2017 14:01:35  chwon
	20634: Fix bcdUVC value definition.

*/

#ifndef _USBVIDEO11_H_		/* prevent multiple includes */
#define _USBVIDEO11_H_

/****************************************************************************\
|
|	Video class, subclass, and protocol codes. (A.1, A.2, A.3)
|
\****************************************************************************/

/* class codes */
#define	USB_bInterfaceClass_Video		0x0E	/* the video class */

/* Interface subclasses */
#define	USB_bInterfaceSubClass_VideoUndefined	0x00	/* Undefined */
#define	USB_bInterfaceSubClass_VideoControl	0x01	/* control */
#define	USB_bInterfaceSubClass_VideoStreaming	0x02	/* streaming subclass */
#define	USB_bInterfaceSubClass_VideoInterfaceCollection	0x03	/* Midi subclass */

/* A table initializer */
#define	USB_bInterfaceSubClass_Video_INIT \
	{ USB_bInterfaceSubClass_VideoUndefined, "Undefined" }, \
	{ USB_bInterfaceSubClass_VideoControl, "Video Control" }, \
	{ USB_bInterfaceSubClass_VideoStreaming, "Video Streaming" }, \
	{ USB_bInterfaceSubClass_VideoInterfaceCollection, "Video Interface Collection" }

/* Interface protocol */
#define	USB_bInterfaceProtocol_VideoUndefined	0x00


/****************************************************************************\
|
|	Video Class-Specific Functional Descriptors (A.4, A.5, A.6)
|
\****************************************************************************/

struct USB_Video_FunctionalDescriptor_Generic
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFunctionSpecifcData[1];
	};

typedef struct USB_Video_FunctionalDescriptor_Generic
	USB_Video_GenericDescriptor;

/*
|| Video class specific descriptor Types
*/
#define	USB_bDescriptorType_Video_UNDEFINED	0x20	/* Class-specific descriptor, Undefined */
#define	USB_bDescriptorType_Video_DEVICE	0x21	/* Class-specific descriptor, Device */
#define	USB_bDescriptorType_Video_CONFIGURATION	0x22	/* Class-specific descriptor, Configuration */
#define	USB_bDescriptorType_Video_STRING	0x23	/* Class-specific descriptor, String */
#define	USB_bDescriptorType_Video_INTERFACE	0x24	/* Class-specific descriptor, Interace */
#define	USB_bDescriptorType_Video_ENDPOINT	0x25	/* Class-specific descriptor, Endpoint */

/*
|| Video control interfce descriptor subtypes
*/
#define USB_bDescriptorSubtype_VideoControl_UNDEFINED		0x00
#define USB_bDescriptorSubtype_VideoControl_HEADER		0x01
#define USB_bDescriptorSubtype_VideoControl_INPUT_TERMINAL	0x02
#define USB_bDescriptorSubtype_VideoControl_OUTPUT_TERMINAL	0x03
#define USB_bDescriptorSubtype_VideoControl_SELECTOR_UNIT	0x04
#define USB_bDescriptorSubtype_VideoControl_PROCESSING_UNIT	0x05
#define USB_bDescriptorSubtype_VideoControl_EXTENSION_UNIT	0x06

#define	USB_bDescriptorSubtype_VideoControl__INIT \
	{ USB_bDescriptorSubtype_VideoControl_UNDEFINED, "Undefined" }, \
	{ USB_bDescriptorSubtype_VideoControl_HEADER, "Header" }, \
	{ USB_bDescriptorSubtype_VideoControl_INPUT_TERMINAL, "InputTerminal" }, \
	{ USB_bDescriptorSubtype_VideoControl_OUTPUT_TERMINAL, "OutputTerminal" }, \
	{ USB_bDescriptorSubtype_VideoControl_SELECTOR_UNIT, "SelectorUnit" }, \
	{ USB_bDescriptorSubtype_VideoControl_PROCESSING_UNIT, "ProcessingUnit" }, \
	{ USB_bDescriptorSubtype_VideoControl_EXTENSION_UNIT, "ExtensionUnit" }

/*
|| Video streamming interfce descriptor subtypes
*/
#define USB_bDescriptorSubtype_VideoStreaming_UNDEFINED			0x00
#define USB_bDescriptorSubtype_VideoStreaming_INPUT_HEADER		0x01
#define USB_bDescriptorSubtype_VideoStreaming_OUTPUT_HEADER		0x02
#define USB_bDescriptorSubtype_VideoStreaming_STILL_IMAGE_FRAME		0x03
#define USB_bDescriptorSubtype_VideoStreaming_FORMAT_UNCOMPRESSED	0x04
#define USB_bDescriptorSubtype_VideoStreaming_FRAME_UNCOMPRESSED	0x05
#define USB_bDescriptorSubtype_VideoStreaming_FORMAT_MJPEQ		0x06
#define USB_bDescriptorSubtype_VideoStreaming_FRAME_MJPEQ		0x07
#define USB_bDescriptorSubtype_VideoStreaming_Reserve8			0x08
#define USB_bDescriptorSubtype_VideoStreaming_Reserve9			0x09
#define USB_bDescriptorSubtype_VideoStreaming_FORMAT_MPEG2TS		0x0A
#define USB_bDescriptorSubtype_VideoStreaming_ReserveB			0x0B
#define USB_bDescriptorSubtype_VideoStreaming_FORMAT_DV			0x0C
#define USB_bDescriptorSubtype_VideoStreaming_COLORFORMAT		0x0D
#define USB_bDescriptorSubtype_VideoStreaming_ReserveE			0x0E
#define USB_bDescriptorSubtype_VideoStreaming_ReserveF			0x0F
#define USB_bDescriptorSubtype_VideoStreaming_FORMAT_FRAME_BASED	0x10
#define USB_bDescriptorSubtype_VideoStreaming_FRAME_FRAME_BASED		0x11
#define USB_bDescriptorSubtype_VideoStreaming_FORMAT_STREAM_BASED	0x12

#define	USB_bDescriptorSubtype_VideoStreaming__INIT \
	{ USB_bDescriptorSubtype_VideoStreaming_UNDEFINED, "Undefined" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_INPUT_HEADER, "InputHeader" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_OUTPUT_HEADER, "OutputHeader" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_STILL_IMAGE_FRAME, "StillImageFrame" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FORMAT_UNCOMPRESSED, "FormatUncompressed" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FRAME_UNCOMPRESSED, "FrameUncompressed" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FORMAT_MJPEQ, "FormatMJPEG" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FRAME_MJPEQ, "FrameMJPEG" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_Reserve8, "Reserve8" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_Reserve9, "Reserve9" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FORMAT_MPEG2TS, "FormatMPEG2TS" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_ReserveB, "ReserveB" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FORMAT_DV, "FormatDV" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_COLORFORMAT, "ColorFormat" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_ReserveE, "ReserveE" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_ReserveF, "ReserveE" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FORMAT_FRAME_BASED, "FormatFrameBased" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FRAME_FRAME_BASED, "FrametFrameBased" }, \
	{ USB_bDescriptorSubtype_VideoStreaming_FORMAT_STREAM_BASED, "FormatStreamBased" }


/****************************************************************************\
|
|	Video Class-Specific Endpoint Descriptor Subtypes (A.7)
|
\****************************************************************************/

#define USB_bDescriptorSubtype_VideoEP_UNDEFINED	0x00
#define USB_bDescriptorSubtype_VideoEP_GENERAL		0x01
#define USB_bDescriptorSubtype_VideoEP_ENDPOINT		0x02
#define USB_bDescriptorSubtype_VideoEP_INTERRUPT	0x03

#define	USB_bDescriptorSubtype_VideoEP__INIT \
	{ USB_bDescriptorSubtype_VideoEP_UNDEFINED, "Undefined" }, \
	{ USB_bDescriptorSubtype_VideoEP_GENERAL, "General" }, \
	{ USB_bDescriptorSubtype_VideoEP_ENDPOINT, "Endpoint" }, \
	{ USB_bDescriptorSubtype_VideoEP_INTERRUPT, "Interrupt" }


/****************************************************************************\
|
|	Video Class-Specific Request Codes (A.8)
|
\****************************************************************************/

#define	USB_bRequest_Video_UNDEFINED	0x00	/* undefined */
#define	USB_bRequest_Video_SET_CUR	0x01	/* set current */
#define	USB_bRequest_Video_GET_CUR	0x81	/* get current */
#define	USB_bRequest_Video_GET_MIN	0x82	/* get minimum */
#define	USB_bRequest_Video_GET_MAX	0x83	/* get maximum */
#define	USB_bRequest_Video_GET_RES	0x84	/* get resolution */
#define	USB_bRequest_Video_GET_LEN	0x85	/* get length space */
#define	USB_bRequest_Video_GET_INFO	0x86	/* get information space */
#define	USB_bRequest_Video_GET_DEF	0x87	/* get default space */

#define	USB_bRequest_Video__INIT \
	{ USB_bRequest_Video_UNDEFINED, "UNDEFINED" },	\
	{ USB_bRequest_Video_SET_CUR, "SET_CUR" },	\
	{ USB_bRequest_Video_GET_CUR, "GET_CUR"	},	\
	{ USB_bRequest_Video_GET_MIN, "GET_MIN"	},	\
	{ USB_bRequest_Video_GET_MAX, "GET_MAX"	},	\
	{ USB_bRequest_Video_GET_RES, "GET_RES"	},	\
	{ USB_bRequest_Video_GET_LEN, "SET_LEN" },	\
	{ USB_bRequest_Video_GET_INFO, "GET_INFO" },	\
	{ USB_bRequest_Video_GET_DEF, "GET_DEF"}


/****************************************************************************\
|
|	Control Selector Codes (A.9)
|
\****************************************************************************/

/* A.9.1 VideoControl Interface Control Selectors */
#define	USBVIDEO_bControlSelector_VC_CONTROL_UNDEFINED		0x00
#define	USBVIDEO_bControlSelector_VC_VIDEO_POWER_MODE_CONTROL	0x01
#define	USBVIDEO_bControlSelector_VC_REQUEST_ERROR_CODE_CONTROL	0x02

/* A.9.2 Terminal Control Selectors */
#define	USBVIDEO_bControlSelector_TE_CONTROL_UNDEFINED		0x00

/* A.9.3 Selector Unit Control Selectors */
#define	USBVIDEO_bControlSelector_SU_CONTROL_UNDEFINED		0x00
#define	USBVIDEO_bControlSelector_SU_INPUT_SELECT_CONTROL	0x01

/* A.9.4 Camera Terminal Control Selectors */
#define	USBVIDEO_bControlSelector_CT_CONTROL_UNDEFINED		0x00
#define	USBVIDEO_bControlSelector_CT_SCANNING_MODE_CONTROL	0x01
#define	USBVIDEO_bControlSelector_CT_AE_MODE_CONTROL		0x02
#define	USBVIDEO_bControlSelector_CT_AE_PRIORITY_CONTROL	0x03
#define	USBVIDEO_bControlSelector_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL	0x04
#define	USBVIDEO_bControlSelector_CT_EXPOSURE_TIME_RELATIVE_CONTROL	0x05
#define	USBVIDEO_bControlSelector_CT_FOCUS_ABSOLUTE_CONTROL	0x06
#define	USBVIDEO_bControlSelector_CT_FOCUS_RELATIVE_CONTROL	0x07
#define	USBVIDEO_bControlSelector_CT_FOCUS_AUTO_CONTROL		0x08
#define	USBVIDEO_bControlSelector_CT_IRIS_ABSOLUTE_CONTROL	0x09
#define	USBVIDEO_bControlSelector_CT_IRIS_RELATIVE_CONTROL	0x0A
#define	USBVIDEO_bControlSelector_CT_ZOOM_ABSOLUTE_CONTROL	0x0B
#define	USBVIDEO_bControlSelector_CT_ZOOM_RELATIVE_CONTROL	0x0C
#define	USBVIDEO_bControlSelector_CT_PANTILT_ABSOLUTE_CONTROL	0x0D
#define	USBVIDEO_bControlSelector_CT_PANTILT_RELATIVE_CONTROL	0x0E
#define	USBVIDEO_bControlSelector_CT_ROLL_ABSOLUTE_CONTROL	0x0F
#define	USBVIDEO_bControlSelector_CT_ROLL_RELATIVE_CONTROL	0x10
#define	USBVIDEO_bControlSelector_CT_PRIVACY_CONTROL		0x11

/* A.9.5 Processing Unit Control Selectors*/
#define	USBVIDEO_bControlSelector_PU_CONTROL_UNDEFINED		0x00
#define	USBVIDEO_bControlSelector_PU_BACKLIGHT_COMPENSATION_CONTROL	0x01
#define	USBVIDEO_bControlSelector_PU_BRIGHTNESS_CONTROL		0x02
#define	USBVIDEO_bControlSelector_PU_CONTRAST_CONTROL		0x03
#define	USBVIDEO_bControlSelector_PU_GAIN_CONTROL		0x04
#define	USBVIDEO_bControlSelector_PU_POWER_LINE_FREQUENCY_CONTROL	0x05
#define	USBVIDEO_bControlSelector_PU_HUE_CONTROL		0x06
#define	USBVIDEO_bControlSelector_PU_SATURATION_CONTROL		0x07
#define	USBVIDEO_bControlSelector_PU_SHARPNESS_CONTROL		0x08
#define	USBVIDEO_bControlSelector_PU_GAMMA_CONTROL		0x09
#define	USBVIDEO_bControlSelector_PU_WHITE_BALANCE_TEMPERATURE_CONTROL	0x0A
#define	USBVIDEO_bControlSelector_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL 0x0B
#define	USBVIDEO_bControlSelector_PU_WHITE_BALANCE_COMPONENT_CONTROL	0x0C
#define	USBVIDEO_bControlSelector_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL 0x0D
#define	USBVIDEO_bControlSelector_PU_DIGITAL_MULTIPLIER_CONTROL	0x0E
#define	USBVIDEO_bControlSelector_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL	0x0F
#define	USBVIDEO_bControlSelector_PU_HUE_AUTO_CONTROL		0x10
#define	USBVIDEO_bControlSelector_PU_ANALOG_VIDEO_STANDARD_CONTROL	0x11
#define	USBVIDEO_bControlSelector_PU_ANALOG_LOCK_STATUS_CONTROL	0x12

/* A.9.6 Extension Unit Control Selectors*/
#define	USBVIDEO_bControlSelector_XU_CONTROL_UNDEFINED		0x00

/* A.9.7 VideoStream Interface Control Selectors*/
#define	USBVIDEO_bControlSelector_VS_CONTROL_UNDEFINED		0x00
#define	USBVIDEO_bControlSelector_VS_PROBE_CONTROL		0x01
#define	USBVIDEO_bControlSelector_VS_COMMIT_CONTROL		0x02
#define	USBVIDEO_bControlSelector_VS_STILL_PROBE_CONTROL	0x03
#define	USBVIDEO_bControlSelector_VS_STILL_COMMIT_CONTROL	0x04
#define	USBVIDEO_bControlSelector_VS_STILL_IMAGE_TRIGGER_CONTROL	0x05
#define	USBVIDEO_bControlSelector_VS_STREAM_ERROR_CODE_CONTROL 	0x06
#define	USBVIDEO_bControlSelector_VS_GENERATE_KEY_FRAME_CONTROL 0x07
#define	USBVIDEO_bControlSelector_VS_UPDATE_FRAME_SEGMENT_CONTROL	0x08
#define	USBVIDEO_bControlSelector_VS_SYNCH_DELAY_CONTROL 	0x09

/****************************************************************************\
|
|	Terminal Types [Video Terminal Types] (B)
|
\****************************************************************************/

/* B.1  USB Terminal Types */
#define	USBVIDEO_wTerminalType_TT_VENDOR_SPECIFIC		0x0100
#define	USBVIDEO_wTerminalType_TT_STREAMING			0x0101

/* B.2  Input Terminal Types */
#define	USBVIDEO_wTerminalType_ITT_VENDOR_SPECIFIC		0x0200
#define	USBVIDEO_wTerminalType_ITT_CAMERA			0x0201
#define	USBVIDEO_wTerminalType_ITT_MEDIA_TRANPORT_INPUT		0x0202

/* B.3  Input Terminal Types */
#define	USBVIDEO_wTerminalType_OTT_VENDOR_SPECIFIC		0x0300
#define	USBVIDEO_wTerminalType_OTT_DISPLAY			0x0301
#define	USBVIDEO_wTerminalType_OTT_MEDIA_TRANPORT_OUTPUT	0x0302

/* B.4  External Terminal Types */
#define	USBVIDEO_wTerminalType_EXTERNAL_VENDOR_SPECIFIC		0x0400
#define	USBVIDEO_wTerminalType_COMPOSITE_CONNECTOR		0x0401
#define	USBVIDEO_wTerminalType_SVIDEO_CONNECTOR			0x0402
#define	USBVIDEO_wTerminalType_COMPONENT_CONNECTOR		0x0403

/* symbolic names */
#define USBVIDEO_wTerminalType__INIT \
	{ USBVIDEO_wTerminalType_TT_VENDOR_SPECIFIC, "USBVIDEO_wTerminalType_VENDOR_SPECIFIC" }, \
	{ USBVIDEO_wTerminalType_TT_STREAMING, "USBVIDEO_wTerminalType_TT_STREAMING" }, \
	{ USBVIDEO_wTerminalType_ITT_VENDOR_SPECIFIC, "USBVIDEO_wTerminalType_INPUT_ITT_VENDOR_SPECIFIC" }, \
	{ USBVIDEO_wTerminalType_ITT_CAMERA, "USBVIDEO_wTerminalType_INPUT_ITT_CAMERA" }, \
	{ USBVIDEO_wTerminalType_ITT_MEDIA_TRANPORT_INPUT, "USBVIDEO_wTerminalType_INPUT_ITT_MEDIA_TRANPORT_INPUT" }, \
	{ USBVIDEO_wTerminalType_OTT_VENDOR_SPECIFIC, "USBVIDEO_wTerminalType_OUTPUT_OTT_VENDOR_SPECIFIC" }, \
	{ USBVIDEO_wTerminalType_OTT_DISPLAY, "USBVIDEO_wTerminalType_OUTPUT_OTT_DISPLAY" }, \
	{ USBVIDEO_wTerminalType_OTT_MEDIA_TRANPORT_OUTPUT, "USBVIDEO_wTerminalType_OUTPUT_OTT_MEDIA_TRANPORT_OUTPUT" }, \
	{ USBVIDEO_wTerminalType_EXTERNAL_VENDOR_SPECIFIC, "USBVIDEO_wTerminalType_EXTERNAL_VENDOR_SPECIFIC" }, \
	{ USBVIDEO_wTerminalType_COMPOSITE_CONNECTOR, "USBVIDEO_wTerminalType_EXTERNAL_COMPOSITE_CONNECTOR" }, \
	{ USBVIDEO_wTerminalType_SVIDEO_CONNECTOR, "USBVIDEO_wTerminalType_EXTERNAL_SVIDEO_CONNECTOR" }, \
	{ USBVIDEO_wTerminalType_COMPONENT_CONNECTOR, "USBVIDEO_wTerminalType_EXTERNAL_COMPONENT_CONNECTOR" }


/****************************************************************************\
|
|	Video Format Tags
|
\****************************************************************************/


/* XXX need MPEG control selectors and AC-3 control selectors */

/****************************************************************************\
|
|	The status packet format (2.4.2.2)
|
\****************************************************************************/

struct USB_Video_StatusPacketFormat_HEADER
	{
	unsigned char	bStatusType;
	unsigned char	bOriginator;
	};

typedef struct USB_Video_StatusPacketFormat_HEADER
	USB_Video_StatusPacketFormatHeader;

#define	USBVIDEO_bStatusType_Reserved			0
#define	USBVIDEO_bStatusType_VideoControl		1
#define	USBVIDEO_bStatusType_VideoStreaming		2


struct USB_Video_StatusPacketFormat_VIDEO_CONTROL
	{
	unsigned char	bStatusType;
	unsigned char	bOriginator;
	unsigned char	bEvent;
	unsigned char	bSelector;
	unsigned char	bAttribute;
	unsigned char	bValue;
	};

typedef struct USB_Video_StatusPacketFormat_VIDEO_CONTROL
	USB_Video_StatusPacketFormatVideoControl;

#define	USBVIDEO_bEvent_ControlChange			0

#define	USBVIDEO_bAttribute_ControlValueChange		0
#define	USBVIDEO_bAttribute_ControlInfoChange		1
#define	USBVIDEO_bAttribute_ControlFailureChange	2


struct USB_Video_StatusPacketFormat_VIDEO_STREAMING
	{
	unsigned char	bStatusType;
	unsigned char	bOriginator;
	unsigned char	bEvent;
	unsigned char	bValue;
	};

typedef struct USB_Video_StatusPacketFormat_VIDEO_STREAMING
	USB_Video_StatusPacketFormatVideoStreaming;

#define	USBVIDEO_bEvent_ButtonPress			0

#define	USBVIDEO_bValue_ButtonReleased			0
#define	USBVIDEO_bValue_ButtonPressed			1


/****************************************************************************\
|
|	The Video and Still Image Payload Headers (2.4.3.3)
|
\****************************************************************************/

struct USB_Video_Payload_HEADER
	{
	unsigned char	bHeaderLength;
	unsigned char	bmHeaderInfo;
	unsigned char	dwPresentationTime[4];
/*	unsigned char	scrSourceClock[6];	*/
	unsigned char	scrSourceClockSTC[4];
	unsigned char	scrSourceClockSOF[2];
	};

typedef struct USB_Video_Payload_HEADER
	USB_Video_PayloadHeader;

/* bmHeaderInfo */
#define	USBVIDEO_bmHeaderInfo_FID	(1u << 0)	/* Frame ID */
#define	USBVIDEO_bmHeaderInfo_EOF	(1u << 1)	/* End of Frame */
#define	USBVIDEO_bmHeaderInfo_PST	(1u << 2)	/* Presentation Time */
#define	USBVIDEO_bmHeaderInfo_SCR	(1u << 3)	/* Source Clock Reference */
#define	USBVIDEO_bmHeaderInfo_RES	(1u << 4)	/* Reserved */
#define	USBVIDEO_bmHeaderInfo_STI	(1u << 5)	/* Still image */
#define	USBVIDEO_bmHeaderInfo_ERR	(1u << 6)	/* Error */
#define	USBVIDEO_bmHeaderInfo_EOH	(1u << 7)	/* End of header */


/****************************************************************************\
|
|	The header functional descriptor (3.7.2)
|
\****************************************************************************/

struct USB_VideoControl_FunctionalDescriptor_HEADER
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bcdUVC[2];		/*
						|| the version of the spec.
						||  1.0 == 0x0100
						||  1.1 == 0x0110
						||  1.5 == 0x0150
						*/
	unsigned char	wTotalLength[2];	/* total length including
						|| other descriptors
						*/
	unsigned char	dwClockFrequency[4];
	unsigned char	bInCollection;		/* # of interfaces in
						|| collection
						*/
	unsigned char	baInterfaceNr[1];	/* vector of associated ifcs */
	};

typedef struct USB_VideoControl_FunctionalDescriptor_HEADER
	USB_VideoControl_HeaderDescriptor;

#define	USBVIDEO_bcdUVC_V1_0	0x0100
#define	USBVIDEO_bcdUVC_V1_1	0x0110


/****************************************************************************\
|
|	The generic entity descriptor
|
\****************************************************************************/

struct USB_VideoControl_FunctionalDescriptor_ENTITY_GENERAL
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bEntityId;
	};

typedef struct USB_VideoControl_FunctionalDescriptor_ENTITY_GENERAL
	USB_VideoControl_GeneralEntityDescriptor;


/****************************************************************************\
|
|	The input terminal functional descriptor (3.7.2.1)
|
\****************************************************************************/

struct USB_VideoControl_FunctionalDescriptor_INPUT_TERMINAL
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| A constant uniqely identifyingthe terminal within the video
	|| function.  This value is used in all requests to address this
	|| Terminal.
	*/
	unsigned char	bTerminalID;

	/*
	|| The type of the input terminal.  See [USBAUDTERM1.0].
	*/
	unsigned char	wTerminalType[2];

	/*
	|| the ID of the output terminal to which the input terminal is
	|| associated.
	*/
	unsigned char	bAssocTerminal;

	/*
	|| string index of the name of the input terminal
	*/
	unsigned char	iTerminal;

	/*
	|| Depending on the Terminal type, certain Input Terminal descriptors
	|| have additional fields.
	*/
	unsigned char	Data[1];
	};

typedef struct USB_VideoControl_FunctionalDescriptor_INPUT_TERMINAL
	USB_VideoControl_InputTerminalDescriptor;


/****************************************************************************\
|
|	The output terminal functional descriptor (3.7.2.2)
|
\****************************************************************************/

struct USB_VideoControl_FunctionalDescriptor_OUTPUT_TERMINAL
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| A constant uniqely identifying the terminal within the video
	|| function.  This value is used in all requests to address this
	|| Terminal.
	*/
	unsigned char	bTerminalID;

	/*
	|| The type of the output terminal.  See [USBAUDTERM1.0].
	*/
	unsigned char	wTerminalType[2];

	/*
	|| the ID of the input terminal to which the output terminal is
	|| associated.
	*/
	unsigned char	bAssocTerminal;

	/*
	|| the ID of the Unit or Terminal to which this Terminal is
	|| connected
	*/
	unsigned char	bSourceID;

	/*
	|| string index for name of this output terminal
	*/
	unsigned char	iTerminal;
	};

typedef struct USB_VideoControl_FunctionalDescriptor_OUTPUT_TERMINAL
	USB_VideoControl_OutputTerminalDescriptor;


/****************************************************************************\
|
|	The camera terminal descriptor (3.7.2.3)
|
\****************************************************************************/

struct USB_VideoControl_FunctionalDescriptor_CAMERA_TERMINAL
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| A constant uniqely identifyingthe terminal within the video
	|| function.  This value is used in all requests to address this
	|| Terminal.
	*/
	unsigned char	bTerminalID;

	/*
	|| The type of the input terminal.  See [USBAUDTERM1.0].
	*/
	unsigned char	wTerminalType[2];

	/*
	|| the ID of the output terminal to which the input terminal is
	|| associated.
	*/
	unsigned char	bAssocTerminal;

	/*
	|| string index of the name of the input terminal
	*/
	unsigned char	iTerminal;

	/*
	|| Min Focal Length
	*/
	unsigned char	wObjectiveFocalLengthMin[2];

	/*
	|| Max Focal Length
	*/
	unsigned char	wObjectiveFocalLengthMax[2];

	/*
	|| Focal Length Step
	*/
	unsigned char	wObjectiveFocalLength[2];

	/*
	|| The size of control bitmap
	*/
	unsigned char	bControlSize;

	/*
	|| Bitmap of related attributes
	*/
	unsigned char	bmControls[3];
	};

typedef struct USB_VideoControl_FunctionalDescriptor_CAMERA_TERMINAL
	USB_VideoControl_CameraTerminalDescriptor;


/****************************************************************************\
|
|	The selector unit functional descriptor (3.7.2.4)
|
\****************************************************************************/

struct USB_VideoControl_FunctionalDescriptor_SELECTOR_UNIT
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| A constant uniqely identifying the unit within the video
	|| function.  This value is used in all requests to address this
	|| Unit
	*/
	unsigned char	bUnitID;

	/* Number of input pins of this Unit:  [p] */
	unsigned char	bNrInPins;

	/*
	|| ID of each Unit or terminal to which the input pins of this
	|| Selector unit are connected.
	*/
	unsigned char	baSourceID[1];

/*	unsigned char	baSourceID[bNrInPins];	*/
/*	unsigned char	iSelector;		*/
	};

typedef struct USB_VideoControl_FunctionalDescriptor_SELECTOR_UNIT
	USB_VideoControl_SelectorUnitDescriptor;

/*
|| The video descriptor data structures were evidently not designed by
|| C programmers, so the access methods are a bit clunky.
*/
#define	USBVIDEO_SELECTOR_UNIT_iSelector(pDesc) \
	(((unsigned char *)(pDesc))[(pDesc)->bLength - 1])


/****************************************************************************\
|
|	The processing unit descriptor (3.7.2.5)
|
\****************************************************************************/

struct USB_VideoControl_FunctionalDescriptor_PROCESSING_UNIT
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	bSourceID;
	unsigned char	wMaxMultiplier[2];
	unsigned char	bControlSize;
	unsigned char	bmControls[1];
/*	unsigned char	bmControls[bControlSize];	*/
/*	unsigned char	iProcessing;			*/
/*	unsigned char	bmVideoStandards;		*/
	};

typedef struct USB_VideoControl_FunctionalDescriptor_PROCESSING_UNIT
	USB_VideoControl_ProcessingUnitDescriptor;

/*
|| The video descriptor data structures were evidently not designed by
|| C programmers, so the access methods are a bit clunky.
*/
#define	USBVIDEO_PROCESSING_UNIT_iProcessing(pDesc) \
	(((unsigned char *)(pDesc))[(pDesc)->bLength - 2])

#define	USBVIDEO_PROCESSING_UNIT_bmVideoStandards(pDesc) \
	(((unsigned char *)(pDesc))[(pDesc)->bLength - 1])


/****************************************************************************\
|
|	The extension unit descriptor (3.7.2.6)
|
\****************************************************************************/

struct USB_VideoControl_FunctionalDescriptor_EXTENSION_UNIT
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	guidExtensionCode[16];
	unsigned char	bNumControls;
	unsigned char	bNrInPins;
	unsigned char	baSourceID[1];
/*	unsigned char	baSourceID[bNrInPins];		*/
/*	unsigned char	bControlSize;			*/
/*	unsigned char	bmControls[bControlSize];	*/
/*	unsigned char	iExtension;			*/
	};

typedef struct USB_VideoControl_FunctionalDescriptor_EXTENSION_UNIT
	USB_VideoControl_ExtensionUnitDescriptor;

/*
|| The video descriptor data structures were evidently not designed by
|| C programmers, so the access methods are a bit clunky.
*/
#define	USBVIDEO_EXTENSION_UNIT_bControlSize(pDesc) \
	((pDesc)->baSourceID[(pDesc)->bNrInPins])

#define	USBVIDEO_EXTENSION_UNIT_bmControls(pDesc, i) \
	((pDesc)->baSourceID[(pDesc)->bNrInPins + 1 + (i)])

#define	USBVIDEO_EXTENSION_UNIT_iExtension(pDesc) \
	(((unsigned char *)(pDesc))[(pDesc)->bLength - 1])


/****************************************************************************\
|
|	The Video Streaming Endpoint functional descriptor (3.8.2.2)
|
\****************************************************************************/

struct USB_VideoControl_EndpointDescriptor_GENERAL
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	wMaxTransferSize[2];
	};

typedef struct USB_VideoControl_EndpointDescriptor_GENERAL
	USB_VideoControl_EndpointDescriptor;


/****************************************************************************\
|
|	The Video Streaming Input Header Descriptor (3.9.2.1)
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_INPUT_HEADER
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| The number of video payload format descriptors following
	|| for this interface.
	*/
	unsigned char	bNumFormats;

	/*
	|| The total length of bytes returned for this class-specific
	|| VideoStreaming interface descriptors including this header
	|| descriptor.
	*/
	unsigned char	wTotalLength[2];

	/*
	|| The address of the iso/bulk endpoint used for video data.
	*/
	unsigned char	bEndpointAddress;

	/*
	|| Bitmap indicating the capabilities of this VideoStreaming
	|| interface.
	*/
	unsigned char	bmInfo;

	/*
	|| The terminal ID of the Output terminal to which the endpoint
	|| of this interface is connected.
	*/
	unsigned char	bTerminalLink;

	/*
	|| Method of still image capture supported.
	*/
	unsigned char	bStillCaptureMethod;

	/*
	|| Specify if hardware triggering is supported through this interface.
	*/
	unsigned char	bTriggerSupport;

	/*
	|| Specify how the host software shall respond to a hardware trigger
	|| interrupt event from this interface.
	*/
	unsigned char	bTriggerUsage;

	/*
	|| Size of each bmaControls field, in bytes.
	*/
	unsigned char	bControlSize;

	/*
	|| Indicate that named field is supported by Video Probe and Commit
	|| Control when bFormatIndex is 1.
	*/
	unsigned char	bmaControls[1];
/*	unsigned char	bmaControls[bControlSize];	*/
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_INPUT_HEADER
	USB_VideoStreaming_InputHeaderDescriptor;


/****************************************************************************\
|
|	The Video Streaming Output Header Descriptor (3.9.2.2)
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_OUTPUT_HEADER
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| The number of video payload format descriptors following
	|| for this interface.
	*/
	unsigned char	bNumFormats;

	/*
	|| The total length of bytes returned for this class-specific
	|| VideoStreaming interface descriptors including this header
	|| descriptor.
	*/
	unsigned char	wTotalLength[2];

	/*
	|| The address of the iso/bulk endpoint used for video data.
	*/
	unsigned char	bEndpointAddress;

	/*
	|| The terminal ID of the Output terminal to which the endpoint
	|| of this interface is connected.
	*/
	unsigned char	bTerminalLink;

	/*
	|| Size of each bmaControls field, in bytes.
	*/
	unsigned char	bControlSize;

	/*
	|| Indicate that named field is supported by Video Probe and Commit
	|| Control when bFormatIndex is 1.
	*/
	unsigned char	bmaControls[1];
/*	unsigned char	bmaControls[bControlSize];	*/
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_OUTPUT_HEADER
	USB_VideoStreaming_OutputHeaderDescriptor;


/****************************************************************************\
|
|	The still image frame descriptor (3.9.2.5)
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_STILL_IMAGE_FRAME
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bEndpointAddress;
	unsigned char	bNumImageSizePatterns;
	unsigned char	wWidth[2];
	unsigned char	wHeight[2];
/*	unsigned char	wWidth[bNumImageSizePatterns][2];	*/
/*	unsigned char	wHeight[bNumImageSizePatterns][2];	*/
/*	unsigned char	bNumCompressionPattern;			*/
/*	unsigned char	bCompression[bNumCompressionPattern];	*/
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_STILL_IMAGE_FRAME
	USB_VideoStreaming_StillImageFrameDescriptor;


/****************************************************************************\
|
|	The color matching descriptor (3.9.2.6)
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_COLOR_MATCHING
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bColorPrimaries;
	unsigned char	bTransferCharacteristics;
	unsigned char	bMatrixCoefficients;
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_COLOR_MATCHING
	USB_VideoStreaming_ColorMatchingDescriptor;


/****************************************************************************\
|
|	USB Device Class Definition for Video Devices: Uncompressed Payload
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_UNCOMPRESSED
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFormatIndex;
	unsigned char	bNumFrameDescriptors;
	unsigned char	guidFormat[16];
	unsigned char	bBitsPerPixel;
	unsigned char	bDefaultFrameIndex;
	unsigned char	bAspectRatioX;
	unsigned char	bAspectRatioY;
	unsigned char	bmInterlaceFlags;
	unsigned char	bCopyProtect;
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_UNCOMPRESSED
	USB_VideoStreaming_FORMAT_UNCOMPRESSED;


struct USB_VideoStreaming_FunctionalDescriptor_FRAME_UNCOMPRESSED
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFrameIndex;
	unsigned char	bmCapabilities;
	unsigned char	wWidth[2];
	unsigned char	wHeight[2];
	unsigned char	dwMinBitRate[4];
	unsigned char	dwMaxBitRate[4];
	unsigned char	dwMaxVideoFrameBufferSize[4];
	unsigned char	dwDefaultFrameInterval[4];
	unsigned char	bFrameIntervalType;
	unsigned char	dwFrameInterval[4];  /* bFrameIntervalType!=0 */
/*	unsigned char	dwMinFrameInterval[4];	bFrameIntervalType==0 */
/*	unsigned char	dwMaxFrameInterval[4];	bFrameIntervalType==0 */
/*	unsigned char	dwFrameIntervalStep[4];	bFrameIntervalType==0 */
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FRAME_UNCOMPRESSED
	USB_VideoStreaming_FRAME_UNCOMPRESSED;


/****************************************************************************\
|
|	USB Device Class Definition for Video Devices: MJPEG Payload
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_MJPEG
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFormatIndex;
	unsigned char	bNumFrameDescriptors;
	unsigned char	bmFlags;
	unsigned char	bDefaultFrameIndex;
	unsigned char	bAspectRatioX;
	unsigned char	bAspectRatioY;
	unsigned char	bmInterlaceFlags;
	unsigned char	bCopyProtect;
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_MJPEG
	USB_VideoStreaming_FORMAT_MJPEG;


struct USB_VideoStreaming_FunctionalDescriptor_FRAME_MJPEG
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFrameIndex;
	unsigned char	bmCapabilities;
	unsigned char	wWidth[2];
	unsigned char	wHeight[2];
	unsigned char	dwMinBitRate[4];
	unsigned char	dwMaxBitRate[4];
	unsigned char	dwMaxVideoFrameBufferSize[4];
	unsigned char	dwDefaultFrameInterval[4];
	unsigned char	bFrameIntervalType;
	unsigned char	dwFrameInterval[4];  /* bFrameIntervalType!=0 */
/*	unsigned char	dwMinFrameInterval[4];	bFrameIntervalType==0 */
/*	unsigned char	dwMaxFrameInterval[4];	bFrameIntervalType==0 */
/*	unsigned char	dwFrameIntervalStep[4];	bFrameIntervalType==0 */
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FRAME_MJPEG
	USB_VideoStreaming_FRAME_MJPEG;


/****************************************************************************\
|
|	USB Device Class Definition for Video Devices: MJPEG-2 TS Payload
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_MJPEG2TS
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFormatIndex;
	unsigned char	bDataOffset;
	unsigned char	bPacketLength;
	unsigned char	bStrideLength;
	unsigned char	guidStrideFormat[16];
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_MJPEG2TS
	USB_VideoStreaming_FORMAT_MJPEG2TS;


/****************************************************************************\
|
|	USB Device Class Definition for Video Devices: DV Payload
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_DV
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFormatIndex;
	unsigned char	dwMaxVideoFrameBufferSize[4];
	unsigned char	bFormatType;
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_DV
	USB_VideoStreaming_FORMAT_DV;


/****************************************************************************\
|
|	USB Device Class Definition for Video Devices: Frame Based Payload
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_FRAME_BASED
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFormatIndex;
	unsigned char	bNumFrameDescriptors;
	unsigned char	guidFormat[16];
	unsigned char	bBitsPerPixel;
	unsigned char	bDefaultFrameIndex;
	unsigned char	bAspectRatioX;
	unsigned char	bAspectRatioY;
	unsigned char	bmInterlaceFlags;
	unsigned char	bCopyProtect;
	unsigned char	bVariableSize;
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_FRAME_BASED
	USB_VideoStreaming_FORMAT_FRAME_BASED;


struct USB_VideoStreaming_FunctionalDescriptor_FRAME_FRAME_BASED
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFrameIndex;
	unsigned char	bmCapabilities;
	unsigned char	wWidth[2];
	unsigned char	wHeight[2];
	unsigned char	dwMinBitRate[4];
	unsigned char	dwMaxBitRate[4];
	unsigned char	dwDefaultFrameInterval[4];
	unsigned char	bFrameIntervalType;
	unsigned char	dwBytesPerLine[4];
	unsigned char	dwFrameInterval[4];  /* bFrameIntervalType!=0 */
/*	unsigned char	dwMinFrameInterval[4];	bFrameIntervalType==0 */
/*	unsigned char	dwMaxFrameInterval[4];	bFrameIntervalType==0 */
/*	unsigned char	dwFrameIntervalStep[4];	bFrameIntervalType==0 */
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FRAME_FRAME_BASED
	USB_VideoStreaming_FRAME_FRAME_BASED;


/****************************************************************************\
|
|	USB Device Class Definition for Video Devices: Stream Based Payload
|
\****************************************************************************/

struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_STREAM_BASED
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFormatIndex;
	unsigned char	guidFormat[16];
	unsigned char	dwPacketLength[4];
	};

typedef struct USB_VideoStreaming_FunctionalDescriptor_FORMAT_STREAM_BASED
	USB_VideoStreaming_FORMAT_STREAM_BASED;


/****************************************************************************\
|
|	Video Control Requests (4.2)
|
\****************************************************************************/

/* 4.1.2  Get_INFO request */
struct USB_VideoRequest_GetInfo_s
	{
	unsigned char	bmCapabilities;
	};

typedef struct USB_VideoRequest_GetInfo_s
	USB_VideoRequest_GetInfo;

/* bmCapabilities */
#define	USBVIDEO_bmCapabilities_SUPPORT_GET	(1u << 0)
#define	USBVIDEO_bmCapabilities_SUPPORT_SET	(1u << 1)
#define	USBVIDEO_bmCapabilities_DISABLED_AUTO	(1u << 2)
#define	USBVIDEO_bmCapabilities_AUTOUPDATE	(1u << 3)
#define	USBVIDEO_bmCapabilities_ASYNCHRONOUS	(1u << 4)


/* 4.2.1 Interface Control Requests */
/* 4.2.1.1 Power Mode Control */
struct USB_VideoControlRequest_PowerModeControl_s
	{
	unsigned char	bDevicePowerMode;
	};

typedef struct USB_VideoControlRequest_PowerModeControl_s
	USB_VideoControlRequest_PowerModeControl;

/* 4.2.1.2 Request Error Code Control */
struct USB_VideoControlRequest_RequestErrorCodeControl_s
	{
	unsigned char	bRequestErrorCode;
	};

typedef struct USB_VideoControlRequest_RequestErrorCodeControl_s
	USB_VideoControlRequest_RequestErrorCodeControl;

/* 4.2.2 Unit and Terminal Control Requests */
/* 4.2.2.1 Camera Terminal Control Requests */
/* 4.2.2.1.1 Scanning Mode Control */
struct USB_VideoControlRequest_ScanningModeControl_s
	{
	unsigned char	bScanningMode;
	};

typedef struct USB_VideoControlRequest_ScanningModeControl_s
	USB_VideoControlRequest_ScanningModeControl;

/* 4.2.2.1.2 Auto-Exposure Mode Control */
struct USB_VideoControlRequest_AutoExposureModeControl_s
	{
	unsigned char	bAutoExposureMode;
	};

typedef struct USB_VideoControlRequest_AutoExposureModeControl_s
	USB_VideoControlRequest_AutoExposureModeControl;

/* 4.2.2.1.3 Auto-Exposure Priority Control */
struct USB_VideoControlRequest_AutoExposurePriorityControl_s
	{
	unsigned char	bAutoExposurePriority;
	};

typedef struct USB_VideoControlRequest_AutoExposurePriorityControl_s
	USB_VideoControlRequest_AutoExposurePriorityControl;

/* 4.2.2.1.4 Exposure Time (Absolute) Control */
struct USB_VideoControlRequest_ExposureTimeAbsoluteControl_s
	{
	unsigned char	dwExposureTimeAbsolute[4];
	};

typedef struct USB_VideoControlRequest_ExposureTimeAbsoluteControl_s
	USB_VideoControlRequest_ExposureTimeAbsoluteControl;

/* 4.2.2.1.5 Exposure Time (Relative) Control */
struct USB_VideoControlRequest_ExposureTimeRelativeControl_s
	{
	unsigned char	bExposureTimeRelative;
	};

typedef struct USB_VideoControlRequest_ExposureTimeRelativeControl_s
	USB_VideoControlRequest_ExposureTimeRelativeControl;

/* 4.2.2.1.6 Focus (Absolute) Control */
struct USB_VideoControlRequest_FocusAbsoluteControl_s
	{
	unsigned char	wFocusAbsolute[2];
	};

typedef struct USB_VideoControlRequest_FocusAbsoluteControl_s
	USB_VideoControlRequest_FocusAbsoluteControl;

/* 4.2.2.1.7 Focus (Relative) Control */
struct USB_VideoControlRequest_FocusRelativeControl_s
	{
	unsigned char	bFocusRelative;
	unsigned char	bSpeed;
	};

typedef struct USB_VideoControlRequest_FocusRelativeControl_s
	USB_VideoControlRequest_FocusRelativeControl;

/* 4.2.2.1.8 Focus, Auto Control */
struct USB_VideoControlRequest_FocusAutoControl_s
	{
	unsigned char	bFocusAuto;
	};

typedef struct USB_VideoControlRequest_FocusAutoControl_s
	USB_VideoControlRequest_FocusAutoControl;

/* 4.2.2.1.9 Iris (Absolute) Control */
struct USB_VideoControlRequest_IrisAbsoluteControl_s
	{
	unsigned char	wIrisAbsolute[2];
	};

typedef struct USB_VideoControlRequest_IrisAbsoluteControl_s
	USB_VideoControlRequest_IrisAbsoluteControl;

/* 4.2.2.1.10 Iris (Relative) Control */
struct USB_VideoControlRequest_IrisRelativeControl_s
	{
	unsigned char	bIrisRelative;
	};

typedef struct USB_VideoControlRequest_IrisRelativeControl_s
	USB_VideoControlRequest_IrisRelativeControl;

/* 4.2.2.1.11 Zoom (Absolute) Control */
struct USB_VideoControlRequest_ZoomAbsoluteControl_s
	{
	unsigned char	wObjectiveFocalLength[2];
	};

typedef struct USB_VideoControlRequest_ZoomAbsoluteControl_s
	USB_VideoControlRequest_ZoomAbsoluteControl;

/* 4.2.2.1.12 Zoom (Relative) Control */
struct USB_VideoControlRequest_ZoomRelativeControl_s
	{
	unsigned char	bZoom;
	unsigned char	bDigitalZoom;
	unsigned char	bSpeed;
	};

typedef struct USB_VideoControlRequest_ZoomRelativeControl_s
	USB_VideoControlRequest_ZoomRelativeControl;

/* 4.2.2.1.13 PanTilt (Absolute) Control */
struct USB_VideoControlRequest_PanTiltAbsoluteControl_s
	{
	unsigned char	dwPanAbsolute[4];
	unsigned char	dwTiltAbsolute[4];
	};

typedef struct USB_VideoControlRequest_PanTiltAbsoluteControl_s
	USB_VideoControlRequest_PanTiltAbsoluteControl;

/* 4.2.2.1.14 PanTilt (Relative) Control */
struct USB_VideoControlRequest_PanTiltRelativeControl_s
	{
	unsigned char	bPanRelative;
	unsigned char	bPanSpeed;
	unsigned char	bTiltRelative;
	unsigned char	bTiltSpeed;
	};

typedef struct USB_VideoControlRequest_PanTiltRelativeControl_s
	USB_VideoControlRequest_PanTiltRelativeControl;

/* 4.2.2.1.15 Roll (Absolute) Control */
struct USB_VideoControlRequest_RollAbsoluteControl_s
	{
	unsigned char	wRollAbsolute[2];
	};

typedef struct USB_VideoControlRequest_RollAbsoluteControl_s
	USB_VideoControlRequest_RollAbsoluteControl;

/* 4.2.2.1.16 Roll (Relative) Control */
struct USB_VideoControlRequest_RollRelativeControl_s
	{
	unsigned char	bRollRelative;
	unsigned char	bSpeed;
	};

typedef struct USB_VideoControlRequest_RollRelativeControl_s
	USB_VideoControlRequest_RollRelativeControl;

/* 4.2.2.1.17 Privacy Control */
struct USB_VideoControlRequest_PrivacyControl_s
	{
	unsigned char	bPrivacy;
	};

typedef struct USB_VideoControlRequest_PrivacyControl_s
	USB_VideoControlRequest_PrivacyControl;

/* 4.2.2.2 Selector Unit Control Requests */
struct USB_VideoControlRequest_SelectorUnitControl_s
	{
	unsigned char	bSelector;
	};

typedef struct USB_VideoControlRequest_SelectorUnitControl_s
	USB_VideoControlRequest_SelectorUnitControl;

/* 4.2.2.3 Processing Unit Control Requests */
/* 4.2.2.3.1 Backlight Compensation Control */
struct USB_VideoControlRequest_BacklightCompensationControl_s
	{
	unsigned char	wBacklightCompensation[2];
	};

typedef struct USB_VideoControlRequest_BacklightCompensationControl_s
	USB_VideoControlRequest_BacklightCompensationControl;

/* 4.2.2.3.2 Brightness Control */
struct USB_VideoControlRequest_BrightnessControl_s
	{
	unsigned char	wBrightness[2];
	};

typedef struct USB_VideoControlRequest_BrightnessControl_s
	USB_VideoControlRequest_BrightnessControl;

/* 4.2.2.3.3 Contrast Control */
struct USB_VideoControlRequest_ContrastControl_s
	{
	unsigned char	wContrast[2];
	};

typedef struct USB_VideoControlRequest_ContrastControl_s
	USB_VideoControlRequest_ContrastControl;

/* 4.2.2.3.4 Gain Control */
struct USB_VideoControlRequest_GainControl_s
	{
	unsigned char	wGain[2];
	};

typedef struct USB_VideoControlRequest_GainControl_s
	USB_VideoControlRequest_GainControl;

/* 4.2.2.3.5 Power Line Frequency Control */
struct USB_VideoControlRequest_PowerLineFrequencyControl_s
	{
	unsigned char	bPowerLineFrequency;
	};

typedef struct USB_VideoControlRequest_PowerLineFrequencyControl_s
	USB_VideoControlRequest_PowerLineFrequencyControl;

/* 4.2.2.3.6 Hue Control */
struct USB_VideoControlRequest_HueControl_s
	{
	unsigned char	wHue[2];
	};

typedef struct USB_VideoControlRequest_HueControl_s
	USB_VideoControlRequest_HueControl;

/* 4.2.2.3.7 HueAuto Control */
struct USB_VideoControlRequest_HueAutoControl_s
	{
	unsigned char	bHueAuto;
	};

typedef struct USB_VideoControlRequest_HueAutoControl_s
	USB_VideoControlRequest_HueAutoControl;

/* 4.2.2.3.8 Saturation Control */
struct USB_VideoControlRequest_SaturationControl_s
	{
	unsigned char	wSaturation[2];
	};

typedef struct USB_VideoControlRequest_SaturationControl_s
	USB_VideoControlRequest_SaturationControl;

/* 4.2.2.3.9 Sharpness Control */
struct USB_VideoControlRequest_SharpnessControl_s
	{
	unsigned char	wSharpness[2];
	};

typedef struct USB_VideoControlRequest_SharpnessControl_s
	USB_VideoControlRequest_SharpnessControl;

/* 4.2.2.3.10 Gamma Control */
struct USB_VideoControlRequest_GammaControl_s
	{
	unsigned char	wGamma[2];
	};

typedef struct USB_VideoControlRequest_GammaControl_s
	USB_VideoControlRequest_GammaControl;

/* 4.2.2.3.11 White Balance Temperature Control */
struct USB_VideoControlRequest_WhiteBalanceTemperatureControl_s
	{
	unsigned char	wWhiteBalanceTemperature[2];
	};

typedef struct USB_VideoControlRequest_WhiteBalanceTemperatureControl_s
	USB_VideoControlRequest_WhiteBalanceTemperatureControl;

/* 4.2.2.3.12 White Balance Temperature, Auto Control */
struct USB_VideoControlRequest_WhiteBalanceTemperatureAutoControl_s
	{
	unsigned char	bWhiteBalanceTemperatureAuto;
	};

typedef struct USB_VideoControlRequest_WhiteBalanceTemperatureAutoControl_s
	USB_VideoControlRequest_WhiteBalanceTemperatureAutoControl;

/* 4.2.2.3.13 White Balance Component Control */
struct USB_VideoControlRequest_WhiteBalanceComponentControl_s
	{
	unsigned char	wWhiteBalanceBlue[2];
	unsigned char	wWhiteBalanceRed[2];
	};

typedef struct USB_VideoControlRequest_WhiteBalanceComponentControl_s
	USB_VideoControlRequest_WhiteBalanceComponentControl;

/* 4.2.2.3.14 White Balance Component, Auto Control */
struct USB_VideoControlRequest_WhiteBalanceComponentAutoControl_s
	{
	unsigned char	bWhiteBalanceComponentAuto;
	};

typedef struct USB_VideoControlRequest_WhiteBalanceComponentAutoControl_s
	USB_VideoControlRequest_WhiteBalanceComponentAutoControl;

/* 4.2.2.3.15 Digital Multiplier Control */
struct USB_VideoControlRequest_DigitalMultiplierControl_s
	{
	unsigned char	wMultiplierStep[2];
	};

typedef struct USB_VideoControlRequest_DigitalMultiplierControl_s
	USB_VideoControlRequest_DigitalMultiplierControl;

/* 4.2.2.3.16 Digital Multiplier Limit Control */
struct USB_VideoControlRequest_DigitalMultiplierLimitControl_s
	{
	unsigned char	wMultiplierLimit[2];
	};

typedef struct USB_VideoControlRequest_DigitalMultiplierLimitControl_s
	USB_VideoControlRequest_DigitalMultiplierLimitControl;

/* 4.2.2.3.17 Analog Video Standard Control */
struct USB_VideoControlRequest_AnalogVideoStandardControl_s
	{
	unsigned char	bVideoStandard;
	};

typedef struct USB_VideoControlRequest_AnalogVideoStandardControl_s
	USB_VideoControlRequest_AnalogVideoStandardControl;

/* 4.2.2.3.18 Analog Video Lock Status Control */
struct USB_VideoControlRequest_AnalogVideoLockStatusControl_s
	{
	unsigned char	bStatus;
	};

typedef struct USB_VideoControlRequest_AnalogVideoLockStatusControl_s
	USB_VideoControlRequest_AnalogVideoLockStatusControl;


/****************************************************************************\
|
|	Video Streaming Requests (4.3)
|
\****************************************************************************/

/* 4.3.1 Interface Control Requests */
/* 4.3.1.1 Video Probe and Commit Controls */
struct USB_VideoStreamRequest_ProbeCommitControl_s
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
	};

typedef struct USB_VideoStreamRequest_ProbeCommitControl_s
	USB_VideoStreamRequest_ProbeCommitControl;

#define	USBVIDEO_bmHint_dwFrameInterval		(1u << 0)
#define	USBVIDEO_bmHint_wKeyFrameRate		(1u << 1)
#define	USBVIDEO_bmHint_wPFrameRate		(1u << 2)
#define	USBVIDEO_bmHint_wCompQuality		(1u << 3)
#define	USBVIDEO_bmHint_wCompWindowSize		(1u << 4)

/* 4.3.1.2 Video Still Probe Control and Still Commit Control */
struct USB_VideoStreamRequest_StillProbeCommitControl_s
	{
	unsigned char	bFormatIndex;
	unsigned char	bFrameIndex;
	unsigned char	bCompressionIndex;
	unsigned char	dwMaxVideoFrameSize[4];
	unsigned char	dwMaxPayloadTransferSize[4];
	};

typedef struct USB_VideoStreamRequest_StillProbeCommitControl_s
	USB_VideoStreamRequest_StillProbeCommitControl;

/* 4.3.1.3 Synch Delay Control */
struct USB_VideoStreamRequest_SynchDelayControl_s
	{
	unsigned char	wDelay[2];
	};

typedef struct USB_VideoStreamRequest_SynchDelayControl_s
	USB_VideoStreamRequest_SynchDelayControl;

/* 4.3.1.4 Still Image Trigger Control */
struct USB_VideoStreamRequest_StillImageTriggerControl_s
	{
	unsigned char	bTrigger;
	};

typedef struct USB_VideoStreamRequest_StillImageTriggerControl_s
	USB_VideoStreamRequest_StillImageTriggerControl;

/* 4.3.1.5 Generate Key Frame Control */
struct USB_VideoStreamRequest_GenerateKeyFrameControl_s
	{
	unsigned char	bGenerateKeyFrame;
	};

typedef struct USB_VideoStreamRequest_GenerateKeyFrameControl_s
	USB_VideoStreamRequest_GenerateKeyFrameControl;

/* 4.3.1.6 Update Frame Segment Control */
struct USB_VideoStreamRequest_UpdateFrameSegmentControl_s
	{
	unsigned char	bStartFrameSegment;
	unsigned char	bEndFrameSegment;
	};

typedef struct USB_VideoStreamRequest_UpdateFrameSegmentControl_s
	USB_VideoStreamRequest_UpdateFrameSegmentControl;

/* 4.3.1.7 Stream Error Code Control */
struct USB_VideoStreamRequest_StreamErrorCodeControl_s
	{
	unsigned char	bStreamErrorCode;
	};

typedef struct USB_VideoStreamRequest_StreamErrorCodeControl_s
	USB_VideoStreamRequest_StreamErrorCodeControl;

/**** end of usbvideo11.h ****/
#endif /* _USBVIDEO11_H_ */
