/* usbaudio10.h	Mon Jul 10 2006 16:42:29 ssyang */

/*

Module:  usbaudio10.h

Function:
	USB Audio Class constants

Version:
	V1.97b	Mon Jul 10 2006 16:42:29  ssyang	Edit level 4

Copyright notice:
	This file copyright (C) 2001 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	January 2001

Revision history:
   1.61g  Wed Jan 17 2001 18:16:14  tmm
	Module created.

   1.61h  3/10/2001  tmm
	Add more definitions.

   1.97b  Mon Jul 10 2006 16:42:29  ssyang
	Add more types and constants.
	
*/

#ifndef _USBAUDIO10_H_		/* prevent multiple includes */
#define _USBAUDIO10_H_

/****************************************************************************\
|
|	Audio class, subclass, and protocol codes. (A.1, A.2, A.3)
|
\****************************************************************************/

/* class codes */
#define	USB_bInterfaceClass_Audio	0x01	/* the audio class */

/* Interface subclasses */
#define	USB_bInterfaceSubClass_AudioControl	0x01	/* control */
#define	USB_bInterfaceSubClass_AudioStreaming	0x02	/* streaming subclass */
#define	USB_bInterfaceSubClass_AudioMidi	0x03	/* Midi subclass */

/* A table initializer */
#define	USB_bInterfaceSubClass_Audio_INIT \
	{ USB_bInterfaceSubClass_AudioControl, "Audio Control" }, \
	{ USB_bInterfaceSubClass_AudioStreaming, "Audio Streaming" }, \
	{ USB_bInterfaceSubClass_AudioMidi, "Audio Midi Streaming" }


/****************************************************************************\
|
|	Audio Class-Specific Functional Descriptors (A.4, A.5, A.6)
|
\****************************************************************************/

/* XXX incomplete */
struct USB_Audio_FunctionalDescriptor_Generic
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFunctionSpecifcData[1];
	};

/*
|| Audio class specific descriptor Types
*/
#define	USB_bDescriptorType_Audio_DEVICE	0x21	/* Class-specific descriptor, INTERACE */
#define	USB_bDescriptorType_Audio_CONFIGURATION	0x22	/* Class-specific descriptor, INTERACE */
#define	USB_bDescriptorType_Audio_STRING	0x23	/* Class-specific descriptor, INTERACE */
#define	USB_bDescriptorType_Audio_INTERFACE	0x24	/* Class-specific descriptor, INTERACE */
#define	USB_bDescriptorType_Audio_ENDPOINT	0x25	/* Class-specific descriptor, INTERACE */

/*
|| Audio control interfce descriptor subtypes
*/
#define USB_bDescriptorSubtype_AudioControl_HEADER		0x01
#define USB_bDescriptorSubtype_AudioControl_INPUT_TERMINAL	0x02	
#define USB_bDescriptorSubtype_AudioControl_OUTPUT_TERMINAL	0x03	
#define USB_bDescriptorSubtype_AudioControl_MIXER_UNIT		0x04
#define USB_bDescriptorSubtype_AudioControl_SELECTOR_UNIT	0x05
#define USB_bDescriptorSubtype_AudioControl_FEATURE_UNIT	0x06
#define USB_bDescriptorSubtype_AudioControl_PROCESSING_UNIT	0x07
#define USB_bDescriptorSubtype_AudioControl_EXTENSION_UNIT	0x08

#define	USB_bDescriptorSubtype_AudioControl__INIT \
	{ USB_bDescriptorSubtype_AudioControl_HEADER, "Header" }, \
	{ USB_bDescriptorSubtype_AudioControl_INPUT_TERMINAL, "InputTerminal" }, \
	{ USB_bDescriptorSubtype_AudioControl_OUTPUT_TERMINAL, "OutputTerminal" }, \
	{ USB_bDescriptorSubtype_AudioControl_MIXER_UNIT, "MixerUnit" }, \
	{ USB_bDescriptorSubtype_AudioControl_SELECTOR_UNIT, "SelectorUnit" }, \
	{ USB_bDescriptorSubtype_AudioControl_FEATURE_UNIT, "FeatureUnit" }, \
	{ USB_bDescriptorSubtype_AudioControl_PROCESSING_UNIT, "ProcessingUnit" }, \
	{ USB_bDescriptorSubtype_AudioControl_EXTENSION_UNIT, "ExtensionUnit" }

/*
|| Audio streamming interfce descriptor subtypes
*/
#define USB_bDescriptorSubtype_AudioStreaming_GENERAL		0x01
#define USB_bDescriptorSubtype_AudioStreaming_FORMAT_TYPE	0x02
#define USB_bDescriptorSubtype_AudioStreaming_FORMAT_SPECIFIC	0x03

#define	USB_bDescriptorSubtype_AudioStreaming__INIT \
	{ USB_bDescriptorSubtype_AudioStreaming_GENERAL, "General" }, \
	{ USB_bDescriptorSubtype_AudioStreaming_FORMAT_TYPE, "FormatType" }, \
	{ USB_bDescriptorSubtype_AudioStreaming_FORMAT_SPECIFIC, "FormatSpecific" }


/****************************************************************************\
|
|	Process Types (A.7)
|
\****************************************************************************/

/* XXX incomplete */


/****************************************************************************\
|
|	Audio Class-Specific Endpoint Descriptor Subtypes (A.8)
|
\****************************************************************************/

#define USB_bDescriptorSubtype_AudioEP_GENERAL		0x01

#define	USB_bDescriptorSubtype_AudioEP__INIT \
	{ USB_bDescriptorSubtype_AudioEP_GENERAL, "General" }


/****************************************************************************\
|
|	Audio Class-Specific Request Codes (A.9)
|
\****************************************************************************/

#define	USB_bRequest_Audio_UNDEFINED	0x00	/* undefined */
#define	USB_bRequest_Audio_SET_CUR	0x01	/* set current */
#define	USB_bRequest_Audio_GET_CUR	0x81	/* get current */
#define	USB_bRequest_Audio_SET_MIN	0x02	/* set minimum */
#define	USB_bRequest_Audio_GET_MIN	0x82	/* get minimum */
#define	USB_bRequest_Audio_SET_MAX	0x03	/* set maximum */
#define	USB_bRequest_Audio_GET_MAX	0x83	/* get maximum */
#define	USB_bRequest_Audio_SET_RES	0x04	/* set resolution */
#define	USB_bRequest_Audio_GET_RES	0x84	/* get resolution */
#define	USB_bRequest_Audio_SET_MEM	0x05	/* set memory space */
#define	USB_bRequest_Audio_GET_MEM	0x85	/* get memory space */
#define	USB_bRequest_Audio_GET_STAT	0xFF	/* get status */

#define	USB_bRequest_Audio__INIT \
	{ USB_bRequest_Audio_UNDEFINED, "UNDEFINED" },	\
	{ USB_bRequest_Audio_SET_CUR, "SET_CUR" },	\
	{ USB_bRequest_Audio_GET_CUR, "GET_CUR"	},	\
	{ USB_bRequest_Audio_SET_MIN, "SET_MIN" },	\
	{ USB_bRequest_Audio_GET_MIN, "GET_MIN"	},	\
	{ USB_bRequest_Audio_SET_MAX, "SET_MAX" },	\
	{ USB_bRequest_Audio_GET_MAX, "GET_MAX"	},	\
	{ USB_bRequest_Audio_SET_RES, "SET_RES" },	\
	{ USB_bRequest_Audio_GET_RES, "GET_RES"	},	\
	{ USB_bRequest_Audio_SET_MEM, "SET_MEM" },	\
	{ USB_bRequest_Audio_GET_MEM, "GET_MEM"	},	\
	{ USB_bRequest_Audio_GET_STAT, "GET_STAT"}
	

/****************************************************************************\
|
|	Audio Control Request Parameter Block
|
\****************************************************************************/

/*
|| Terminal Control Request Parameter Block
*/
struct USB_AudioControlRequest_ParamBlock_TM_COPY_PROTECT
	{
	unsigned char	bCopyProtect;
	};

/*
|| Selector Unit Control Request Parameter Block
*/
struct USB_AudioControlRequest_ParamBlock_SELECTOR
	{
	unsigned char	bSelector;
	};

/*
|| Feature Unit Control Request Parameter Block
*/
struct USB_AudioControlRequest_ParamBlock_FU_MUTE
	{
	unsigned char	bMute;
	};

struct USB_AudioControlRequest_ParamBlock_FU_MUTE_ALL
	{
	unsigned char	bMute[1];
	};

struct USB_AudioControlRequest_ParamBlock_FU_VOLUME
	{
	unsigned char	wVolume[2];
	};

struct USB_AudioControlRequest_ParamBlock_FU_VOLUME_ALL
	{
	unsigned char	wVolume[1][2];
	};

/* XXX */	

/****************************************************************************\
|
|	Control Selector Codes (A.10)
|
\****************************************************************************/

/* Terminal Control Selectors */
#define	USBAUDIO_bControlSelector_TM_UNDEFINED			0x00
#define	USBAUDIO_bControlSelector_TM_COPY_PROTECT		0x01

/* Feature Unit Control Selectors */
#define	USBAUDIO_bControlSelector_FU_UNDEFINED			0x00
#define	USBAUDIO_bControlSelector_FU_MUTE			0x01
#define	USBAUDIO_bControlSelector_FU_VOLUME			0x02
#define	USBAUDIO_bControlSelector_FU_BASS			0x03
#define	USBAUDIO_bControlSelector_FU_MID			0x04
#define	USBAUDIO_bControlSelector_FU_TREBLE			0x05
#define	USBAUDIO_bControlSelector_FU_GRAPHIC_EQUALIZER		0x06
#define	USBAUDIO_bControlSelector_FU_AUTOMATIC_GAIN		0x07
#define	USBAUDIO_bControlSelector_FU_DELAY			0x08
#define	USBAUDIO_bControlSelector_FU_BASS_BOOST			0x09
#define	USBAUDIO_bControlSelector_FU_LOUDNESS			0x0A

/* Up/Down-Mix Processing Unit Control Selectors */
#define	USBAUDIO_bControlSelector_PU_UD_UNDEFINED		0x00
#define	USBAUDIO_bControlSelector_PU_UD_ENABLE			0x01
#define	USBAUDIO_bControlSelector_PU_UD_MODE_SELECT		0x02

/* Dolby Prologic Processing Unit Control Selectors */
#define	USBAUDIO_bControlSelector_PU_DP_UNDEFINED		0x00
#define	USBAUDIO_bControlSelector_PU_UP_ENABLE			0x01
#define	USBAUDIO_bControlSelector_PU_UP_MODE_SELECT		0x02

/* 3D Stereo Extender Processing Unit Control Selectors */
#define	USBAUDIO_bControlSelector_PU_3D_UNDEFINED		0x00
#define	USBAUDIO_bControlSelector_PU_3D_ENABLE			0x01
#define	USBAUDIO_bControlSelector_PU_3D_SPACIOUSNESS		0x03 /* there's no 2 in spec */

/* Reverberation Processing Unit Control Selectors */
#define	USBAUDIO_bControlSelector_PU_RV_UNDEFINED		0x00
#define	USBAUDIO_bControlSelector_PU_RV_ENABLE			0x01
#define	USBAUDIO_bControlSelector_PU_RV_LEVEL			0x02
#define	USBAUDIO_bControlSelector_PU_RV_TIME			0x03
#define	USBAUDIO_bControlSelector_PU_RV_FEEDBACK		0x04

/* Chorus Processing Unit Control Selectors */
#define	USBAUDIO_bControlSelector_PU_CH_UNDEFINED		0x00
#define	USBAUDIO_bControlSelector_PU_CH_ENABLE			0x01
#define	USBAUDIO_bControlSelector_PU_CH_LEVEL			0x02
#define	USBAUDIO_bControlSelector_PU_CH_RATE			0x03
#define	USBAUDIO_bControlSelector_PU_CH_DEPTH			0x04

/* Dynamic Range Compressor Processing Unit Control Selectors */
#define	USBAUDIO_bControlSelector_PU_DR_UNDEFINED		0x00
#define	USBAUDIO_bControlSelector_PU_DR_ENABLE			0x01
#define	USBAUDIO_bControlSelector_PU_DR_COMPRESSION_RATE	0x02
#define	USBAUDIO_bControlSelector_PU_DR_MAXAMPL			0x03
#define	USBAUDIO_bControlSelector_PU_DR_THRESHOLD		0x04
#define	USBAUDIO_bControlSelector_PU_DR_ATTACH_TIME		0x05
#define	USBAUDIO_bControlSelector_PU_DR_RELEASE_TIME		0x06

/* Extension Unit Control Selectors */
#define	USBAUDIO_bControlSelector_XU_UNDEFINED			0x00
#define	USBAUDIO_bControlSelector_XU_ENABLE			0x01

/* Endpoint Control Selectors */
#define	USBAUDIO_bControlSelector_EP_UNDEFINED			0x00
#define	USBAUDIO_bControlSelector_EP_SAMPLING_FREQ		0x01
#define	USBAUDIO_bControlSelector_EP_PITCH			0x02


/****************************************************************************\
|
|	Terminal Types [Audio Terminal Types 1.0]
|
\****************************************************************************/

#define	USBAUDIO_wTerminalType_USB_UNDEFINED		0x0100
#define	USBAUDIO_wTerminalType_USB_STREAMING		0x0101
#define	USBAUDIO_wTerminalType_USB_VENDOR		0x01FF

#define	USBAUDIO_wTerminalType_INPUT_UNDEFINED		0x0200
#define	USBAUDIO_wTerminalType_INPUT_MIC_GEN		0x0201
#define	USBAUDIO_wTerminalType_INPUT_MIC_DESKTOP	0x0202
#define	USBAUDIO_wTerminalType_INPUT_MIC_PERSONAL	0x0203
#define	USBAUDIO_wTerminalType_INPUT_MIC_OMNI		0x0204
#define	USBAUDIO_wTerminalType_INPUT_MIC_ARRAY		0x0205
#define	USBAUDIO_wTerminalType_INPUT_MIC_PROCESSING	0x0206

#define	USBAUDIO_wTerminalType_OUTPUT_UNDEFINED		0x0300
#define	USBAUDIO_wTerminalType_OUTPUT_SPEAKER		0x0301
#define	USBAUDIO_wTerminalType_OUTPUT_HEADPHONES	0x0302
#define	USBAUDIO_wTerminalType_OUTPUT_HEAD_MOUNTED_DISPLAY 0x0303
#define	USBAUDIO_wTerminalType_OUTPUT_SPEAKER_DESKTOP	0x0304
#define	USBAUDIO_wTerminalType_OUTPUT_SPEAKER_ROOM	0x0305
#define	USBAUDIO_wTerminalType_OUTPUT_SPEAKER_COMMS	0x0306
#define	USBAUDIO_wTerminalType_OUTPUT_SPEAKER_SUBWOOFER	0x0307

#define	USBAUDIO_wTerminalType_BIDI_UNDEFINED		0x0400
#define	USBAUDIO_wTerminalType_BIDI_HANDSET		0x0401
#define	USBAUDIO_wTerminalType_BIDI_HEADSET		0x0402
#define	USBAUDIO_wTerminalType_BIDI_SPKRPHONE_HOSTEC	0x0403
#define	USBAUDIO_wTerminalType_BIDI_SPKRPHONE_HDX	0x0404
#define	USBAUDIO_wTerminalType_BIDI_SPKRPHONE_FDX	0x0405

#define	USBAUDIO_wTerminalType_TEL_UNDEFINED		0x0500
#define	USBAUDIO_wTerminalType_TEL_PHONELINE		0x0501
#define	USBAUDIO_wTerminalType_TEL_TELEPHONE		0x0502
#define	USBAUDIO_wTerminalType_TEL_DOWNLINE_PHONE	0x0503

#define	USBAUDIO_wTerminalType_EXT_UNDEFINED		0x0600
#define	USBAUDIO_wTerminalType_EXT_ANALOG		0x0601
#define	USBAUDIO_wTerminalType_EXT_DIGNIAL		0x0602
#define	USBAUDIO_wTerminalType_EXT_LINE			0x0603
#define	USBAUDIO_wTerminalType_EXT_LEGACY		0x0604
#define	USBAUDIO_wTerminalType_EXT_SPDIF		0x0605
#define	USBAUDIO_wTerminalType_EXT_1394DA		0x0606
#define	USBAUDIO_wTerminalType_EXT_1394DV		0x0607

#define	USBAUDIO_wTerminalType_EMB_UNDEFINED		0x0700
#define	USBAUDIO_wTerminalType_EMB_LEVELCAL		0x0701
#define	USBAUDIO_wTerminalType_EMB_EQNOISE		0x0702
#define	USBAUDIO_wTerminalType_EMB_CD			0x0703
#define	USBAUDIO_wTerminalType_EMB_DAT			0x0704
#define	USBAUDIO_wTerminalType_EMB_DCC			0x0705
#define	USBAUDIO_wTerminalType_EMB_MINIDISK		0x0706
#define	USBAUDIO_wTerminalType_EMB_ANALOGTAPE		0x0707
#define	USBAUDIO_wTerminalType_EMB_VINYL		0x0708
#define	USBAUDIO_wTerminalType_EMB_VCR			0x0709
#define	USBAUDIO_wTerminalType_EMB_VD			0x070A
#define	USBAUDIO_wTerminalType_EMB_DVD			0x070B
#define	USBAUDIO_wTerminalType_EMB_TV			0x070C
#define	USBAUDIO_wTerminalType_EMB_SATTV		0x070D
#define	USBAUDIO_wTerminalType_EMB_CATV			0x070E
#define	USBAUDIO_wTerminalType_EMB_DSS			0x070F
#define	USBAUDIO_wTerminalType_EMB_AMFM_RX		0x0710
#define	USBAUDIO_wTerminalType_EMB_AMFM_TX		0x0711
#define	USBAUDIO_wTerminalType_EMB_MULTITRACK		0x0712
#define	USBAUDIO_wTerminalType_EMB_SYNTH		0x0713

/* symbolic names */
#define USBAUDIO_wTerminalType__INIT \
	{ USBAUDIO_wTerminalType_USB_UNDEFINED, "USBAUDIO_wTerminalType_USB_UNDEFINED" }, \
	{ USBAUDIO_wTerminalType_USB_STREAMING, "USBAUDIO_wTerminalType_USB_STREAMING" }, \
	{ USBAUDIO_wTerminalType_USB_VENDOR, "USBAUDIO_wTerminalType_USB_VENDOR" }, \
	{ USBAUDIO_wTerminalType_INPUT_UNDEFINED, "USBAUDIO_wTerminalType_INPUT_UNDEFINED" }, \
	{ USBAUDIO_wTerminalType_INPUT_MIC_GEN, "USBAUDIO_wTerminalType_INPUT_MIC_GEN" }, \
	{ USBAUDIO_wTerminalType_INPUT_MIC_DESKTOP, "USBAUDIO_wTerminalType_INPUT_MIC_DESKTOP" }, \
	{ USBAUDIO_wTerminalType_INPUT_MIC_PERSONAL, "USBAUDIO_wTerminalType_INPUT_MIC_PERSONAL" }, \
	{ USBAUDIO_wTerminalType_INPUT_MIC_OMNI, "USBAUDIO_wTerminalType_INPUT_MIC_OMNI" }, \
	{ USBAUDIO_wTerminalType_INPUT_MIC_ARRAY, "USBAUDIO_wTerminalType_INPUT_MIC_ARRAY" }, \
	{ USBAUDIO_wTerminalType_INPUT_MIC_PROCESSING, "USBAUDIO_wTerminalType_INPUT_MIC_PROCESSING" }, \
	{ USBAUDIO_wTerminalType_OUTPUT_UNDEFINED, "USBAUDIO_wTerminalType_OUTPUT_UNDEFINED" }, \
	{ USBAUDIO_wTerminalType_OUTPUT_SPEAKER, "USBAUDIO_wTerminalType_OUTPUT_SPEAKER" }, \
	{ USBAUDIO_wTerminalType_OUTPUT_HEADPHONES, "USBAUDIO_wTerminalType_OUTPUT_HEADPHONES" }, \
	{ USBAUDIO_wTerminalType_OUTPUT_HEAD_MOUNTED_DISPLAY, "USBAUDIO_wTerminalType_OUTPUT_HEAD_MOUNTED_DISPLAY" }, \
	{ USBAUDIO_wTerminalType_OUTPUT_SPEAKER_DESKTOP, "USBAUDIO_wTerminalType_OUTPUT_SPEAKER_DESKTOP" }, \
	{ USBAUDIO_wTerminalType_OUTPUT_SPEAKER_ROOM, "USBAUDIO_wTerminalType_OUTPUT_SPEAKER_ROOM" }, \
	{ USBAUDIO_wTerminalType_OUTPUT_SPEAKER_COMMS, "USBAUDIO_wTerminalType_OUTPUT_SPEAKER_COMMS" }, \
	{ USBAUDIO_wTerminalType_OUTPUT_SPEAKER_SUBWOOFER, "USBAUDIO_wTerminalType_OUTPUT_SPEAKER_SUBWOOFER" }, \
	{ USBAUDIO_wTerminalType_BIDI_UNDEFINED, "USBAUDIO_wTerminalType_BIDI_UNDEFINED" }, \
	{ USBAUDIO_wTerminalType_BIDI_HANDSET, "USBAUDIO_wTerminalType_BIDI_HANDSET" }, \
	{ USBAUDIO_wTerminalType_BIDI_HEADSET, "USBAUDIO_wTerminalType_BIDI_HEADSET" }, \
	{ USBAUDIO_wTerminalType_BIDI_SPKRPHONE_HOSTEC, "USBAUDIO_wTerminalType_BIDI_SPKRPHONE_HOSTEC" }, \
	{ USBAUDIO_wTerminalType_BIDI_SPKRPHONE_HDX, "USBAUDIO_wTerminalType_BIDI_SPKRPHONE_HDX" }, \
	{ USBAUDIO_wTerminalType_BIDI_SPKRPHONE_FDX, "USBAUDIO_wTerminalType_BIDI_SPKRPHONE_FDX" }, \
	{ USBAUDIO_wTerminalType_TEL_UNDEFINED, "USBAUDIO_wTerminalType_TEL_UNDEFINED" }, \
	{ USBAUDIO_wTerminalType_TEL_PHONELINE, "USBAUDIO_wTerminalType_TEL_PHONELINE" }, \
	{ USBAUDIO_wTerminalType_TEL_TELEPHONE, "USBAUDIO_wTerminalType_TEL_TELEPHONE" }, \
	{ USBAUDIO_wTerminalType_TEL_DOWNLINE_PHONE, "USBAUDIO_wTerminalType_TEL_DOWNLINE_PHONE" }, \
	{ USBAUDIO_wTerminalType_EXT_UNDEFINED, "USBAUDIO_wTerminalType_EXT_UNDEFINED" }, \
	{ USBAUDIO_wTerminalType_EXT_ANALOG, "USBAUDIO_wTerminalType_EXT_ANALOG" }, \
	{ USBAUDIO_wTerminalType_EXT_DIGNIAL, "USBAUDIO_wTerminalType_EXT_DIGNIAL" }, \
	{ USBAUDIO_wTerminalType_EXT_LINE, "USBAUDIO_wTerminalType_EXT_LINE" }, \
	{ USBAUDIO_wTerminalType_EXT_LEGACY, "USBAUDIO_wTerminalType_EXT_LEGACY" }, \
	{ USBAUDIO_wTerminalType_EXT_SPDIF, "USBAUDIO_wTerminalType_EXT_SPDIF" }, \
	{ USBAUDIO_wTerminalType_EXT_1394DA, "USBAUDIO_wTerminalType_EXT_1394DA" }, \
	{ USBAUDIO_wTerminalType_EXT_1394DV, "USBAUDIO_wTerminalType_EXT_1394DV" }, \
	{ USBAUDIO_wTerminalType_EMB_UNDEFINED, "USBAUDIO_wTerminalType_EMB_UNDEFINED" }, \
	{ USBAUDIO_wTerminalType_EMB_LEVELCAL, "USBAUDIO_wTerminalType_EMB_LEVELCAL" }, \
	{ USBAUDIO_wTerminalType_EMB_EQNOISE, "USBAUDIO_wTerminalType_EMB_EQNOISE" }, \
	{ USBAUDIO_wTerminalType_EMB_CD, "USBAUDIO_wTerminalType_EMB_CD" }, \
	{ USBAUDIO_wTerminalType_EMB_DAT, "USBAUDIO_wTerminalType_EMB_DAT" }, \
	{ USBAUDIO_wTerminalType_EMB_DCC, "USBAUDIO_wTerminalType_EMB_DCC" }, \
	{ USBAUDIO_wTerminalType_EMB_MINIDISK, "USBAUDIO_wTerminalType_EMB_MINIDISK" }, \
	{ USBAUDIO_wTerminalType_EMB_ANALOGTAPE, "USBAUDIO_wTerminalType_EMB_ANALOGTAPE" }, \
	{ USBAUDIO_wTerminalType_EMB_VINYL, "USBAUDIO_wTerminalType_EMB_VINYL" }, \
	{ USBAUDIO_wTerminalType_EMB_VCR, "USBAUDIO_wTerminalType_EMB_VCR" }, \
	{ USBAUDIO_wTerminalType_EMB_VD, "USBAUDIO_wTerminalType_EMB_VD" }, \
	{ USBAUDIO_wTerminalType_EMB_DVD, "USBAUDIO_wTerminalType_EMB_DVD" }, \
	{ USBAUDIO_wTerminalType_EMB_TV, "USBAUDIO_wTerminalType_EMB_TV" }, \
	{ USBAUDIO_wTerminalType_EMB_SATTV, "USBAUDIO_wTerminalType_EMB_SATTV" }, \
	{ USBAUDIO_wTerminalType_EMB_CATV, "USBAUDIO_wTerminalType_EMB_CATV" }, \
	{ USBAUDIO_wTerminalType_EMB_DSS, "USBAUDIO_wTerminalType_EMB_DSS" }, \
	{ USBAUDIO_wTerminalType_EMB_AMFM_RX, "USBAUDIO_wTerminalType_EMB_AMFM_RX" }, \
	{ USBAUDIO_wTerminalType_EMB_AMFM_TX, "USBAUDIO_wTerminalType_EMB_AMFM_TX" }, \
	{ USBAUDIO_wTerminalType_EMB_MULTITRACK, "USBAUDIO_wTerminalType_EMB_MULTITRACK" }, \
	{ USBAUDIO_wTerminalType_EMB_SYNTH, "USBAUDIO_wTerminalType_EMB_SYNTH" }

/****************************************************************************\
|
|	Audio Format Tags
|
\****************************************************************************/

/* codes for type I */
#define	USBAUDIO_wFormatTag_TYPE_I_UNDEFINED	0x0000
#define	USBAUDIO_wFormatTag_PCM			0x0001
#define	USBAUDIO_wFormatTag_PCM8		0x0002
#define	USBAUDIO_wFormatTag_IEEE_FLOAT		0x0003
#define	USBAUDIO_wFormatTag_ALAW		0x0004
#define	USBAUDIO_wFormatTag_MULAW		0x0005

#define	USBAUDIO_wFormatTag_TYPE_II_UNDEFINED	0x1000
#define	USBAUDIO_wFormatTag_MPEG		0x1001
#define	USBAUDIO_wFormatTag_AC_3		0x1002

#define	USBAUDIO_wFormatTag_TYPE_III_UNDEFINED	0x2000
#define	USBAUDIO_wFormatTag_IEC1937_AC_3	0x2001
#define	USBAUDIO_wFormatTag_IEC1937_MPEG_1_Layer1	0x2002
#define	USBAUDIO_wFormatTag_IEC1937_MPEG_2_NOEXT	0x2003
#define	USBAUDIO_wFormatTag_IEC1937_MPEG_2_EXT		0x2004
#define	USBAUDIO_wFormatTag_IEC1937_MPEG_2_Layer1_LS	0x2005
#define	USBAUDIO_wFormatTag_IEC1937_MPEG_2_Layer23_LS	0x2006

/* XXX need MPEG control selectors and AC-3 control selectors */

/****************************************************************************\
|
|	The header functional descriptor (4.3.2)
|
\****************************************************************************/

struct USB_AudioControl_FunctionalDescriptor_HEADER
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bcdADC[2];		/* the version of the spec.  1.1 == 0x0110 */
	unsigned char	wTotalLength[2];	/* total length including
						|| other descriptors
						*/
	unsigned char	bInCollection;		/* # of interfaces in
						|| collection 
						*/
	unsigned char	baInterfaceNr[1];	/* vector of associated ifcs */
	};

/****************************************************************************\
|
|	The generic entity descriptor
|
\****************************************************************************/

struct USB_AudioControl_FunctionalDescriptor_ENTITY_GENERAL
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bEntityId;
	};

/****************************************************************************\
|
|	The input terminal functional descriptor (4.3.2.1)
|
\****************************************************************************/

struct USB_AudioControl_FunctionalDescriptor_INPUT_TERMINAL
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| A constant uniqely identifyingthe terminal within the audio
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
	|| A bit mask of the spatial location of the logical channels
	*/
	unsigned char	wChannelConfig[2];

	/*
	|| string index of the name of the first logical channel --
	|| subsequent names use ascending indices.
	*/
	unsigned char	iChannelNames;

	/*
	|| string index of the name of the input terminal
	*/
	unsigned char	iTerminal;
	};

/**** bits in wChannelConfig (3.7.2.3) ****/
#define USBAUDIO_wChannelConfig_LeftFront	0x0001
#define USBAUDIO_wChannelConfig_RightFront	0x0002
#define USBAUDIO_wChannelConfig_CenterFront	0x0004
#define USBAUDIO_wChannelConfig_SubWoofer	0x0008
#define USBAUDIO_wChannelConfig_LeftSurround	0x0010
#define USBAUDIO_wChannelConfig_RightSurroud	0x0020
#define USBAUDIO_wChannelConfig_LeftOfCenter	0x0040
#define USBAUDIO_wChannelConfig_RightOfCenter	0x0080
#define USBAUDIO_wChannelConfig_Surround	0x0100
#define USBAUDIO_wChannelConfig_SideLeft	0x0200
#define USBAUDIO_wChannelConfig_SideRight	0x0400
#define USBAUDIO_wChannelConfig_Top		0x0800

#define USBAUDIO_wChannelConfig__INIT \
	{ USBAUDIO_wChannelConfig_LeftFront,	"LeftFront" }, \
	{ USBAUDIO_wChannelConfig_RightFront,	"RightFront" }, \
	{ USBAUDIO_wChannelConfig_CenterFront,	"CenterFront" }, \
	{ USBAUDIO_wChannelConfig_SubWoofer,	"SubWoofer" }, \
	{ USBAUDIO_wChannelConfig_LeftSurround,	"LeftSurround" }, \
	{ USBAUDIO_wChannelConfig_RightSurroud,	"RightSurroud" }, \
	{ USBAUDIO_wChannelConfig_LeftOfCenter,	"LeftOfCenter" }, \
	{ USBAUDIO_wChannelConfig_RightOfCenter, "RightOfCenter" }, \
	{ USBAUDIO_wChannelConfig_Surround,	"Surround" }, \
	{ USBAUDIO_wChannelConfig_SideLeft,	"SideLeft" }, \
	{ USBAUDIO_wChannelConfig_SideRight,	"SideRight" }, \
	{ USBAUDIO_wChannelConfig_Top,		"Top" }


/****************************************************************************\
|
|	The output terminal functional descriptor (4.3.2.2)
|
\****************************************************************************/

struct USB_AudioControl_FunctionalDescriptor_OUTPUT_TERMINAL
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| A constant uniqely identifying the terminal within the audio
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

/****************************************************************************\
|
|	The mixer unit functional descriptor (4.3.2.3)
|
\****************************************************************************/

struct USB_AudioControl_FunctionalDescriptor_MIXER_UNIT
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| A constant uniqely identifying the unit within the audio
	|| function.  This value is used in all requests to address this
	|| Unit
	*/
	unsigned char	bUnitID;

	/* Number of input pins of this Unit:  [p] */
	unsigned char	bNrInPins;
	unsigned char	bMixerData[1];
	};

/*
|| The audio descriptor data structures were evidently not designed by
|| C programmers, so the access methods are a bit clunky.
*/
#define	USBAUDIO_MIXER_UNIT_beSourceID(pDesc, iPin) \
	((pDesc)->bMixerData[(iPin)])
#define	USBAUDIO_MIXER_UNIT_bNrChannels(pDesc) \
	((pDesc)->bMixerData[(pDesc)->bNrInPins]
#define	USBAUDIO_MIXER_UNIT_wChannelConfig(pDesc) \
	(&USBAUDIO_MIXER_UNIX_bNrChannels(pDesc) + 1)
#define	USBAUDIO_MIXER_UNIT_iChannelNames(pDesc) \
	(USBAUDIO_MIXER_UNIT_wChannelConfig(pDesc)[2])
#define	USBAUDIO_MIXER_UNIT_bmControls(pDesc, iByte) \
	((&USBAUDIO_MIXER_UNIT_iChannelNames(pDesc))[(iByte) + 1])
#define	USBAUDIO_MIXER_UNIT_iMixer(pDesc) \
	(((unsigned char *)(pDesc))[(pDesc)->bFunctionLength - 1])

/****************************************************************************\
|
|	The selector unit functional descriptor (4.3.2.4)
|
\****************************************************************************/

struct USB_AudioControl_FunctionalDescriptor_SELECTOR_UNIT
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| A constant uniqely identifying the unit within the audio
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
	};

/*
|| The audio descriptor data structures were evidently not designed by
|| C programmers, so the access methods are a bit clunky.
*/
#define	USBAUDIO_SELECTOR_UNIT_iSelector(pDesc) \
	(((unsigned char *)(pDesc))[(pDesc)->bFunctionLength - 1])

/****************************************************************************\
|
|	The feature unit functional descriptor (4.3.2.5)
|
\****************************************************************************/

struct USB_AudioControl_FunctionalDescriptor_FEATURE_UNIT
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| A constant uniqely identifying the unit within the audio
	|| function.  This value is used in all requests to address this
	|| Unit
	*/
	unsigned char	bUnitID;

	/*
	|| ID of the Unit or Terminal to which this feature unit is connected
	*/
	unsigned char	bSourceID;

	/*
	|| size of each element of the control array, in bytes
	*/
	unsigned char	bControlSize;

	/* 
	|| Array of control feature masks
	*/
	unsigned char	bmaControls[1];
	};

/*
|| The audio descriptor data structures were evidently not designed by
|| C programmers, so the access methods are a bit clunky.
*/
#define	USBAUDIO_FEATURE_UNIT_bmaControls(pDesc, iChan) \
	((pDesc)->bmaControls + (pDesc)->bControlSize * (iChan))

#define	USBAUDIO_FEATURE_UNIT_iFeature(pDesc) \
	(((unsigned char *)(pDesc))[(pDesc)->bFunctionLength - 1])

/*
|| The audio control bitmask definitions.  It's a word for now, so we'll
|| define directly.
*/
#define	USBAUDIO_FEATURE_CONTROLS_Mute		0x0001
#define	USBAUDIO_FEATURE_CONTROLS_Volume	0x0002
#define	USBAUDIO_FEATURE_CONTROLS_Bass		0x0004
#define	USBAUDIO_FEATURE_CONTROLS_Mid		0x0008
#define	USBAUDIO_FEATURE_CONTROLS_Treble	0x0010
#define	USBAUDIO_FEATURE_CONTROLS_GraphicEQ	0x0020
#define	USBAUDIO_FEATURE_CONTROLS_AGC		0x0040
#define	USBAUDIO_FEATURE_CONTROLS_Delay		0x0080
#define	USBAUDIO_FEATURE_CONTROLS_BassBoost	0x0100
#define	USBAUDIO_FEATURE_CONTROLS_Loudness	0x0200

/****************************************************************************\
|
|	The Processing Unit functional descriptor (4.3.2.6)
|
\****************************************************************************/

/* XXX */

/****************************************************************************\
|
|	The Up/Down-mix Processing Unit functional descriptor (4.3.2.6.1)
|
\****************************************************************************/

/* XXX */

/****************************************************************************\
|
|	The Dolby Prologic Processing Unit functional descriptor (4.3.2.6.2)
|
\****************************************************************************/

/* XXX */

/****************************************************************************\
|
|	The 3D-Stereo Extender Processing Unit functional descriptor (4.3.2.6.3)
|
\****************************************************************************/

/* XXX */

/****************************************************************************\
|
|	The Reverb Processing Unit functional descriptor (4.3.2.6.4)
|
\****************************************************************************/

/* XXX */

/****************************************************************************\
|
|	The Chorus Processing Unit functional descriptor (4.3.2.6.5)
|
\****************************************************************************/

/* XXX */

/****************************************************************************\
|
|	The Dynamic Range Compressor Processing Unit functional descriptor (4.3.2.6.6)
|
\****************************************************************************/

/* XXX */

/****************************************************************************\
|
|	The Extension Unit functional descriptor (4.3.2.7)
|
\****************************************************************************/

/* XXX */

/****************************************************************************\
|
|	The Associated Interface functional descriptor (4.3.2.8)
|
\****************************************************************************/

/* XXX */


/****************************************************************************\
|
|	The Audio Streaming General functional descriptor (4.5.2)
|
\****************************************************************************/

struct USB_AudioStreaming_FunctionalDescriptor_GENERAL
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	/*
	|| The terminal ID of the terminal to which the endpoint of this
	|| interface is connected.
	*/
	unsigned char	bTerminalLink;

	/*
	|| The delay introduced by the data path, in frames.
	*/
	unsigned char	bDelay;

	/*
	|| The format tag for the audio data format that has
	|| to be used to communicate with this interface.
	*/
	unsigned char	wFormatTag[2];
	};


/****************************************************************************\
|
|	The Audio Streaming Format Type functional descriptor (4.5.3)
|	[AUDIOFMTS1.0] 2.2.5
|
\****************************************************************************/

struct USB_AudioStreaming_FunctionalDescriptor_FORMAT_TYPE_I
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	unsigned char	bFormatType;
	unsigned char	bNrChannels;
	unsigned char	bSubframeSize;
	unsigned char	bBitResolution;
	unsigned char	bSamFreqType;
	unsigned char	bSamFreqTables[1];
	};

struct USB_AudioStreaming_FunctionalDescriptor_FORMAT_TYPE_I_Continuous
	{
	unsigned char	tLowerSamFreq[3];
	unsigned char	tUpperSamFreq[3];
	};

struct USB_AudioStreaming_FunctionalDescriptor_FORMAT_TYPE_I_Discrete
	{
	unsigned char	tSamFreq[3];
	};

/****************************************************************************\
|
|	The Audio Streaming Format Type functional descriptor (4.5.4)
|
\****************************************************************************/

/* XXX */

/****************************************************************************\
|
|	The Audio Streaming Endpoint functional descriptor (4.6.1.2)
|
\****************************************************************************/

struct USB_AudioStreaming_EndpointDescriptor_GENERAL
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;

	unsigned char	bmAttribute;
	unsigned char	bLockDelayUnits;
	unsigned char	wLockDelayUnits[2];
	};

/* XXX */


/****************************************************************************\
|
|	XXX other audio-class things -- not completed.
|
\****************************************************************************/

/**** end of usbaudio10.h ****/
#endif /* _USBAUDIO10_H_ */
