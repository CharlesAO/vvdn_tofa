/* usbaudio20.h	Mon Jul 01 2013 11:27:18 ssyang */

/*

Module:  usbaudio20.h

Function:
	Definition of USB Audio Class V2.0

Version:
	V3.11d	Mon Jul 01 2013 11:27:18 ssyang	Edit level 2

Copyright notice:
	This file copyright (C) 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2013

Revision history:
   3.11c  Thu Mar 07 2013 12:30:19 chwon
	Module created.

   3.11d  Mon Jul 01 2013 11:27:18 ssyang
	17466: added more macros and constants necessary

*/

#ifndef _USBAUDIO20_H_	/* prevent multiple includes */
#define _USBAUDIO20_H_

/****************************************************************************\
|
|	Audio class, subclass, and protocol codes. (A.4, A.5, A.6)
|
\****************************************************************************/

/* Audio interface class codes */
#define	USB_bInterfaceClass_AUDIO				0x01

/* Audio interface subclass codes */
#define	USB_bInterfaceSubClass_INTERFACE_SUBCLASS_UNDEFINED	0x00
#define	USB_bInterfaceSubClass_AUDIOCONTROL			0x01
#define	USB_bInterfaceSubClass_AUDIOSTREAMING			0x02
#define	USB_bInterfaceSubClass_MIDISTREAMING			0x03

/* Audio interface protocol codes */
#define	USB_bInterfaceProtocol_INTERFACE_PROTOCOL_UNDEFINED	0x00
#define	USB_bInterfaceProtocol_IP_VERSION_02_00			0x20


/****************************************************************************\
|
|	Class-Specific Generic Descriptor
|
\****************************************************************************/

struct	USB_Audio_Generic_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	};

/* Audio Class-Specific Descriptor Types. (A.8) */
#define	USB_Audio_bDescriptorType_CS_UNDEFINED			0x20
#define	USB_Audio_bDescriptorType_CS_DEVICE			0x21
#define	USB_Audio_bDescriptorType_CS_CONFIGURATION		0x22
#define	USB_Audio_bDescriptorType_CS_STRING			0x23
#define	USB_Audio_bDescriptorType_CS_INTERFACE			0x24
#define	USB_Audio_bDescriptorType_CS_ENDPOINT			0x25

/* Audio Class-Specific AC Interface Descriptor Subtypes (A.9) */
#define USB_Audio_bDescriptorSubtype_AC_DESCRIPTOR_UNDEFINED	0x00
#define USB_Audio_bDescriptorSubtype_HEADER			0x01
#define USB_Audio_bDescriptorSubtype_INPUT_TERMINAL		0x02
#define USB_Audio_bDescriptorSubtype_OUTPUT_TERMINAL		0x03
#define USB_Audio_bDescriptorSubtype_MIXER_UNIT			0x04
#define USB_Audio_bDescriptorSubtype_SELECTOR_UNIT		0x05
#define USB_Audio_bDescriptorSubtype_FEATURE_UNIT		0x06
#define USB_Audio_bDescriptorSubtype_EFFECT_UNIT		0x07
#define USB_Audio_bDescriptorSubtype_PROCESSING_UNIT		0x08
#define USB_Audio_bDescriptorSubtype_EXTENSION_UNIT		0x09
#define USB_Audio_bDescriptorSubtype_CLOCK_SOURCE		0x0A
#define USB_Audio_bDescriptorSubtype_CLOCK_SELECTOR		0x0B
#define USB_Audio_bDescriptorSubtype_CLOCK_MULTIPLIER		0x0C
#define USB_Audio_bDescriptorSubtype_SAMPLE_RATE_CONVERTER	0x0D

/* Audio Class-Specific AS Interface Descriptor Subtypes (A.10) */
#define USB_Audio_bDescriptorSubtype_AS_DESCRIPTOR_UNDEFINED	0x00
#define	USB_Audio_bDescriptorSubtype_AS_GENERAL			0x01
#define	USB_Audio_bDescriptorSubtype_FORMAT_TYPE		0x02
#define	USB_Audio_bDescriptorSubtype_ENCODER			0x03
#define	USB_Audio_bDescriptorSubtype_DECODER			0x04


/****************************************************************************\
|
|	General Structure and Constant Definition
|
\****************************************************************************/

struct	USB_Audio_AC_Generic_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bEntityId;
	};

#define	USB_Audio_bmControls_Control_Disabled			0
#define	USB_Audio_bmControls_Control_Readable			1
#define	USB_Audio_bmControls_Control_Programmable		3


/****************************************************************************\
|
|	Class-Specific AC Interface Header Descriptor (Table 4-5)
|
\****************************************************************************/

struct	USB_Audio_AC_Interface_Header_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bcdADC[2];
	unsigned char	bCategory;
	unsigned char	wTotalLength[2];
	unsigned char	bmControls;
	};

#define	USB_Audio_AC_Interface_Header_Descriptor__SIZE		9

/* Audio Function Category Codes. (A.7) */
#define	USB_Audio_bCategory_FUNCTION_SUBCLASS_UNDEFINED		0x00
#define	USB_Audio_bCategory_DESKTOP_SPEAKER			0x01
#define	USB_Audio_bCategory_HOME_THEATER			0x02
#define	USB_Audio_bCategory_MICROPHONE				0x03
#define	USB_Audio_bCategory_HEADSET				0x04
#define	USB_Audio_bCategory_TELEPHONE				0x05
#define	USB_Audio_bCategory_CONVERTER				0x06
#define	USB_Audio_bCategory_VOICE_SOUND_RECORDER		0x07
#define	USB_Audio_bCategory_IO_BOX				0x08
#define	USB_Audio_bCategory_MUSICAL_INSTRUMENT			0x09
#define	USB_Audio_bCategory_PRO_AUDIO				0x0A
#define	USB_Audio_bCategory_AUDIO_VIDEO				0x0B
#define	USB_Audio_bCategory_CONTROL_PANEL			0x0C
#define	USB_Audio_bCategory_OTHER				0xFF

#define	USB_Audio_bmControls_LatencyControl_Mask		0x03


/****************************************************************************\
|
|	Clock Source Descriptor (Table 4-6)
|
\****************************************************************************/

struct	USB_Audio_Clock_Source_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bClockID;
	unsigned char	bmAttributes;
	unsigned char	bmControls;
	unsigned char	bAssocTerminal;
	unsigned char	iClockSource;
	};

#define	USB_Audio_Clock_Source_Descriptor__SIZE			8

#define	USB_Audio_bmAttributes_ClockType_Mask			0x03
#define	USB_Audio_bmAttributes_ClockType_External		0x00
#define	USB_Audio_bmAttributes_ClockType_InternalFixed		0x01
#define	USB_Audio_bmAttributes_ClockType_InternalVariable	0x02
#define	USB_Audio_bmAttributes_ClockType_InternalProgrammable	0x03
#define	USB_Audio_bmAttributes_ClockSync2SOF			0x04

#define	USB_Audio_bmControls_ClockFrequencyControl_Mask		0x03
#define	USB_Audio_bmControls_ClockValidityControl_Mask		0x0C


/****************************************************************************\
|
|	Clock Selector Descriptor (Table 4-7)
|
\****************************************************************************/

struct	USB_Audio_Clock_Selector_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bClockID;
	unsigned char	bNrInPins;
	unsigned char	baCSourceID[1];		/* variable size: bNrInPins */
/*	unsigned char	bmControls;		*/
/*	unsigned char	iClockSelector;		*/
	};

#define	USB_Audio_Clock_Selector_Descriptor__SIZE(bNrInPins)	\
	(7 + (bNrInPins))

#define	USB_Audio_Clock_Selector_Descriptor_bmControls(pDesc)	\
	(pDesc)->baCSourceID[(pDesc)->bNrInPins]

#define	USB_Audio_Clock_Selector_Descriptor_iClockSelector(pDesc) \
	(pDesc)->baCSourceID[(pDesc)->bNrInPins + 1]

#define	USB_Audio_bmControls_ClockSelectorControl_Mask		0x03


/****************************************************************************\
|
|	Clock Multiplier Descriptor (Table 4-8)
|
\****************************************************************************/

struct	USB_Audio_Clock_Multiplier_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bClockID;
	unsigned char	bCSourceID;
	unsigned char	bmControls;
	unsigned char	iClockMultiplier;
	};

#define	USB_Audio_Clock_Multiplier_Descriptor__SIZE		7

#define	USB_Audio_bmControls_ClockNumeratorControl_Mask		0x03
#define	USB_Audio_bmControls_ClockDenominatorControl_Mask	0x0C


/****************************************************************************\
|
|	Input Terminal Descriptor (Table 4-9)
|
\****************************************************************************/

struct	USB_Audio_Input_Terminal_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bTerminalID;
	unsigned char	wTerminalType[2];
	unsigned char	bAssocTerminal;
	unsigned char	bCSourceID;
	unsigned char	bNrChannels;
	unsigned char	bmChannelConfig[4];
	unsigned char	iChannelNames;
	unsigned char	bmControls[2];
	unsigned char	iTerminal;
	};

#define	USB_Audio_Input_Terminal_Descriptor__SIZE		17

#define	USB_Audio_bmControls_CopyProtectControl_Mask		0x0003
#define	USB_Audio_bmControls_ConnectorControl_Mask		0x000C
#define	USB_Audio_bmControls_OverloadControl_Mask		0x0030
#define	USB_Audio_bmControls_ClusterControl_Mask		0x00C0
#define	USB_Audio_bmControls_InputUnderflowControl_Mask		0x0300
#define	USB_Audio_bmControls_InputOverflowControl_Mask		0x0C00


/****************************************************************************\
|
|	Output Terminal Descriptor (Table 4-10)
|
\****************************************************************************/

struct	USB_Audio_Output_Terminal_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bTerminalID;
	unsigned char	wTerminalType[2];
	unsigned char	bAssocTerminal;
	unsigned char	bSourceID;
	unsigned char	bCSourceID;
	unsigned char	bmControls[2];
	unsigned char	iTerminal;
	};

#define	USB_Audio_Output_Terminal_Descriptor__SIZE		12

/*
|| #define USB_Audio_bmControls_CopyProtectControl_Mask		0x0003
|| #define USB_Audio_bmControls_ConnectorControl_Mask		0x000C
|| #define USB_Audio_bmControls_OverloadControl_Mask		0x0030
*/
#define	USB_Audio_bmControls_OutputUnderflowControl_Mask	0x00C0
#define	USB_Audio_bmControls_OutputOverflowControl_Mask		0x0300


/****************************************************************************\
|
|	Mixer Unit Descriptor (Table 4-11)
|
\****************************************************************************/

struct	USB_Audio_Mixer_Unit_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	bNrInPins;
	unsigned char	baSourceID[1];		/* variable size = bNrInPins */
/*	unsigned char	bNrChannels;		*/
/*	unsigned char	bmChannelConfig[4];	*/
/*	unsigned char	iChannelNames;		*/
/*	unsigned char	bmMixerControls[1];	-- variable size = N */
/*	unsigned char	bmControls;		*/
/*	unsigned char	iMixer;			*/
	};

#define	USB_Audio_Mixer_Unit_Descriptor__SIZE(bNrInPins, N)	\
	(13 + (bNrInPins) + (N))


/****************************************************************************\
|
|	Selector Unit Descriptor (Table 4-12)
|
\****************************************************************************/

struct	USB_Audio_Selector_Unit_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	bNrInPins;
	unsigned char	baSourceID[1];		/* variable size = bNrInPins */
/*	unsigned char	bmControls;		*/
/*	unsigned char	iSelector;		*/
	};

#define	USB_Audio_Selector_Unit_Descriptor__SIZE(bNrInPins)	\
	(7 + (bNrInPins))

#define	USB_Audio_Selector_Unit_Descriptor_bmControls(pDesc)	\
	(pDesc)->baCSourceID[(pDesc)->bNrInPins]

#define	USB_Audio_Selector_Unit_Descriptor_iSelector(pDesc)	\
	(pDesc)->baCSourceID[(pDesc)->bNrInPins + 1]

#define	USB_Audio_bmControls_SelectorControl_Mask		0x03


/****************************************************************************\
|
|	Feature Unit Descriptor (Table 4-13)
|
\****************************************************************************/

struct	USB_Audio_Feature_Unit_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	bSourceID;
	unsigned char	bmaControls[4];		/* variable size */
/*	unsigned char	iFeature;		*/
	};

#define	USB_Audio_Feature_Unit_Descriptor__SIZE(Ch)	(6 + (ch) * 4)

#define	USB_Audio_Feature_Unit_Descriptor_iFeature(pDesc)	\
	(pDesc)->bmaControls[(pDesc)->bLength - 5]

#define	USB_Audio_bmaControls_MuteControl_Mask			0x00000003
#define	USB_Audio_bmaControls_VolumeControl_Mask		0x0000000C
#define	USB_Audio_bmaControls_BassControl_Mask			0x00000030
#define	USB_Audio_bmaControls_MidControl_Mask			0x000000C0
#define	USB_Audio_bmaControls_TrebleControl_Mask		0x00000300
#define	USB_Audio_bmaControls_GraphicEqualizerControl_Mask	0x00000C00
#define	USB_Audio_bmaControls_AutomaticGainControl_Mask		0x00003000
#define	USB_Audio_bmaControls_DelayControl_Mask			0x0000C000
#define	USB_Audio_bmaControls_BassBoostControl_Mask		0x00030000
#define	USB_Audio_bmaControls_LoudnessControl_Mask		0x000C0000
#define	USB_Audio_bmaControls_InputGainControl_Mask		0x00300000
#define	USB_Audio_bmaControls_InputGainPadControl_Mask		0x00C00000
#define	USB_Audio_bmaControls_PhaseInverterControl_Mask		0x03000000
#define	USB_Audio_bmaControls_UnderflowControl_Mask		0x0C000000
#define	USB_Audio_bmaControls_OverflowControl_Mask		0x30000000


/****************************************************************************\
|
|	Sampling Rate Converter Descriptor (Table 4-14)
|
\****************************************************************************/

struct	USB_Audio_Sampling_Rate_Converter_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	bSourceID;
	unsigned char	bCSourceInID;
	unsigned char	bCSourceOutID;
	unsigned char	iSRC;
	};

#define	USB_Audio_Sampling_Rate_Converter_Descriptor__SIZE	8


/****************************************************************************\
|
|	Effect Unit Descriptor (Table 4-15, 4-16, 4-17, 4-18, 4-19)
|
\****************************************************************************/

struct	USB_Audio_Effect_Unit_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	wEffectType[2];
	unsigned char	bSourceID;
	unsigned char	bmaControls[4];		/* variable size = ch */
/*	unsigned char	iEffects;	*/
	};

#define	USB_Audio_Effect_Unit_Descriptor__SIZE(ch)	(16 + (ch) * 4)

#define	USB_Audio_Effect_Unit_Descriptor_iEffects(pDesc)	\
	(pDesc)->bmaControls[(pDesc)->bLength - 7]

/* Effect Unit Effect Types. (A.11) */
#define	USB_Audio_wEffectType_EFFECT_UNDEFINED			0x00
#define	USB_Audio_wEffectType_PARAM_EQ_SECTION_EFFECT		0x01
#define	USB_Audio_wEffectType_REVERBERATION_EFFECT		0x02
#define	USB_Audio_wEffectType_MOD_DELAY_EFFECT			0x03
#define	USB_Audio_wEffectType_DYN_RANGE_COMP_EFFECT		0x04

/* PARAM_EQ_SECTION_EFFECT */
#define	USB_Audio_bmaControls_EQ_EnableControl_Mask		0x00000003
#define	USB_Audio_bmaControls_EQ_CenterFrequencyControl_Mask	0x0000000C
#define	USB_Audio_bmaControls_EQ_QFactorControl_Mask		0x00000030
#define	USB_Audio_bmaControls_EQ_GainControl_Mask		0x000000C0
#define	USB_Audio_bmaControls_EQ_UnderflowControl_Mask		0x00000300
#define	USB_Audio_bmaControls_EQ_OverflowControl_Mask		0x00000C00

/* REVERBERATION_EFFECT */
#define	USB_Audio_bmaControls_REV_EnableControl_Mask		0x00000003
#define	USB_Audio_bmaControls_REV_TypeControl_Mask		0x0000000C
#define	USB_Audio_bmaControls_REV_LevelControl_Mask		0x00000030
#define	USB_Audio_bmaControls_REV_TimeControl_Mask		0x000000C0
#define	USB_Audio_bmaControls_REV_DelayFeedbackControl_Mask	0x00000300
#define	USB_Audio_bmaControls_REV_PreDelayControl_Mask		0x00000C00
#define	USB_Audio_bmaControls_REV_DensityControl_Mask		0x00003000
#define	USB_Audio_bmaControls_REV_HiFreqRollOffControl_Mask	0x0000C000
#define	USB_Audio_bmaControls_REV_UnderflowControl_Mask		0x00030000
#define	USB_Audio_bmaControls_REV_OverflowControl_Mask		0x000C0000

/* MOD_DELAY_EFFECT */
#define	USB_Audio_bmaControls_MOD_EnableControl_Mask		0x00000003
#define	USB_Audio_bmaControls_MOD_BalanceControl_Mask		0x0000000C
#define	USB_Audio_bmaControls_MOD_RateControl_Mask		0x00000030
#define	USB_Audio_bmaControls_MOD_DepthControl_Mask		0x000000C0
#define	USB_Audio_bmaControls_MOD_TimeControl_Mask		0x00000300
#define	USB_Audio_bmaControls_MOD_FeedbackLevelControl_Mask	0x00000C00
#define	USB_Audio_bmaControls_MOD_UnderflowControl_Mask		0x00003000
#define	USB_Audio_bmaControls_MOD_OverflowControl_Mask		0x0000C000

/* DYN_RANGE_COMP_EFFECT */
#define	USB_Audio_bmaControls_DYN_EnableControl_Mask		0x00000003
#define	USB_Audio_bmaControls_DYN_CompressRatioControl_Mask	0x0000000C
#define	USB_Audio_bmaControls_DYN_MaxAmplControl_Mask		0x00000030
#define	USB_Audio_bmaControls_DYN_ThresholdControl_Mask		0x000000C0
#define	USB_Audio_bmaControls_DYN_AttackTimeControl_Mask	0x00000300
#define	USB_Audio_bmaControls_DYN_ReleaseTimeControl_Mask	0x00000C00
#define	USB_Audio_bmaControls_DYN_UnderflowControl_Mask		0x00003000
#define	USB_Audio_bmaControls_DYN_OverflowControl_Mask		0x0000C000


/****************************************************************************\
|
|	Processing Unit Descriptor (Table 4-20, 4-21, 4-22, 4-23)
|
\****************************************************************************/

struct	USB_Audio_Processing_Unit_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	wProcessType[2];
	unsigned char	bNrInPins;
	unsigned char	baSourceID[1];		/* variable size = bNrInPins */
/*	unsigned char	bNrChannels;		*/
/*	unsigned char	bmChannelConfig[4];	*/
/*	unsigned char	iChannelNames;		*/
/*	unsigned char	bmControls[2];		*/
/*	unsigned char	iProcessing;		*/
/*	unsigned char	ProcessSpecific[1];	-- variable size = x */
	};

#define	USB_Audio_Processing_Unit_Descriptor__SIZE(bNrInPins, x)	\
	(17 + (bNrInPins) + (x))

#define	USB_Audio_Processing_Unit_Descriptor_bNrChannels(pDesc)		\
	(pDesc)->baSourceID[(pDesc)->bNrInPins]

#define	USB_Audio_Processing_Unit_Descriptor_bmChannelConfig(pDesc)	\
	(pDesc)->baSourceID[(pDesc)->bNrInPins + 1]

#define	USB_Audio_Processing_Unit_Descriptor_iChannelNames(pDesc)	\
	(pDesc)->baSourceID[(pDesc)->bNrInPins + 5]

#define	USB_Audio_Processing_Unit_Descriptor_bmControls(pDesc)		\
	(pDesc)->baSourceID[(pDesc)->bNrInPins + 6]

#define	USB_Audio_Processing_Unit_Descriptor_iProcessing(pDesc)		\
	(pDesc)->baSourceID[(pDesc)->bNrInPins + 8]

#define	USB_Audio_Processing_Unit_Descriptor_ProcessSpecific(pDesc)	\
	(pDesc)->baSourceID[(pDesc)->bNrInPins + 9]

/* Processing Unit Process Types. (A.12) */
#define	USB_Audio_wProcessType_PROCESS_UNDEFINED		0x00
#define	USB_Audio_wProcessType_UP_DOWNMIX_PROCESS		0x01
#define	USB_Audio_wProcessType_DOLBY_PROLOGIC_PROCESS		0x02
#define	USB_Audio_wProcessType_STEREO_EXTENDER_PROCESS		0x03


struct	USB_Audio_UpDownMix_Processing_Unit_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	wProcessType[2];
	unsigned char	bNrInPins;
	unsigned char	bSourceID;
	unsigned char	bNrChannels;
	unsigned char	bmChannelConfig[4];
	unsigned char	iChannelNames;
	unsigned char	bmControls[2];
	unsigned char	iProcessing;
	unsigned char	bNrModes;
	unsigned char	daModes[4];		/* variable size = bNrModes */
	};

#define	USB_Audio_UpDownMix_Processing_Unit_Descriptor__SIZE(bNrModes)	\
	(18 + (bNrModes) * 4)


struct	USB_Audio_DolbyPrologic_Processing_Unit_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	wProcessType[2];
	unsigned char	bNrInPins;
	unsigned char	bSourceID;
	unsigned char	bNrChannels;
	unsigned char	bmChannelConfig[4];
	unsigned char	iChannelNames;
	unsigned char	bmControls[2];
	unsigned char	iProcessing;
	unsigned char	bNrModes;
	unsigned char	daModes[4];		/* variable size = bNrModes */
	};

#define	USB_Audio_DolbyPrologic_Processing_Unit_Descriptor__SIZE(bNrModes) \
	(18 + (bNrModes) * 4)

struct	USB_Audio_StereoExtender_Processing_Unit_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	wProcessType[2];
	unsigned char	bNrInPins;
	unsigned char	bSourceID;
	unsigned char	bNrChannels;
	unsigned char	bmChannelConfig[4];
	unsigned char	iChannelNames;
	unsigned char	bmControls[2];
	unsigned char	iProcessing;
	};

#define	USB_Audio_StereoExtender_Processing_Unit_Descriptor__SIZE	17

#define	USB_Audio_bmControls_PU_EnableControl_Mask		0x00000003
#define	USB_Audio_bmControls_PU_ModeSelectControl_Mask		0x0000000C
#define	USB_Audio_bmControls_PU_WidthControl_Mask		0x0000000C
#define	USB_Audio_bmControls_PU_ClusterControl_Mask		0x00000030
#define	USB_Audio_bmControls_PU_UnderflowControl_Mask		0x000000C0
#define	USB_Audio_bmControls_PU_OverflowControl_Mask		0x00000300


/****************************************************************************\
|
|	Extension Unit Descriptor (Table 4-24)
|
\****************************************************************************/

struct	USB_Audio_Extension_Unit_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bUnitID;
	unsigned char	wExtensionCode[2];
	unsigned char	bNrInPins;
	unsigned char	baSourceID[1];		/* variable size = bNrInPins */
	unsigned char	bNrChannels;
	unsigned char	bmChannelConfig[4];
	unsigned char	iChannelNames;
	unsigned char	bmControls[2];
	unsigned char	iExtension;
	};

#define	USB_Audio_Extension_Unit_Descriptor__SIZE(bNrInPins)		\
	(16 + (bNrInPins))

#define	USB_Audio_Extension_Unit_Descriptor_bNrChannels(pDesc)		\
	(pDesc)->baSourceID[(pDesc)->bNrInPins]

#define	USB_Audio_Extension_Unit_Descriptor_bmChannelConfig(pDesc)	\
	(pDesc)->baSourceID[(pDesc)->bNrInPins + 1]

#define	USB_Audio_Extension_Unit_Descriptor_iChannelNames(pDesc)	\
	(pDesc)->baSourceID[(pDesc)->bNrInPins + 5]

#define	USB_Audio_Extension_Unit_Descriptor_bmControls(pDesc)		\
	(pDesc)->baSourceID[(pDesc)->bNrInPins + 6]

#define	USB_Audio_Extension_Unit_Descriptor_iExtension(pDesc)		\
	(pDesc)->baSourceID[(pDesc)->bNrInPins + 8]

#define	USB_Audio_bmControls_EU_EnableControl_Mask		0x00000003
#define	USB_Audio_bmControls_EU_ClusterControl_Mask		0x0000000C
#define	USB_Audio_bmControls_EU_UnderflowControl_Mask		0x00000030
#define	USB_Audio_bmControls_EU_OverflowControl_Mask		0x000000C0


/****************************************************************************\
|
|	Class-Specific AS Interface Header Descriptor (Table 4-27)
|
\****************************************************************************/

struct	USB_Audio_AS_Interface_Header_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bTerminalLink;
	unsigned char	bmControls;
	unsigned char	bFormatType;
	unsigned char	bmFormats[4];
	unsigned char	bNrChannels;
	unsigned char	bmChannelConfig[4];
	unsigned char	iChannelNames;
	};

#define	USB_Audio_AS_Interface_Header_Descriptor__SIZE		16

#define	USB_Audio_bmControls_ActiveAlternateSettingControl_Mask	0x00000003
#define	USB_Audio_bmControls_ValidAlternateSettingsControl_Mask	0x0000000C

/* Format Type Codes (Fmts20 A.1)*/
#define	USB_Audio_AS_Format_Type_Undefined	0x00
#define	USB_Audio_AS_Format_Type1		0x01
#define	USB_Audio_AS_Format_Type2		0x02
#define	USB_Audio_AS_Format_Type3		0x03
#define	USB_Audio_AS_Format_Type4		0x04
#define	USB_Audio_AS_Ext_Format_Type1		0x81
#define	USB_Audio_AS_Ext_Format_Type2		0x82
#define	USB_Audio_AS_Ext_Format_Type3		0x83

/* Format Type 1 Bit Allocation (Fmts20 A.2.1) */
#define	USB_Audio_AS_Format_Type1_PCM		(1 << 0)
#define	USB_Audio_AS_Format_Type1_PCM8		(1 << 1)
#define	USB_Audio_AS_Format_Type1_IEEE_FLOAT	(1 << 2)
#define	USB_Audio_AS_Format_Type1_ALAW		(1 << 3)
#define	USB_Audio_AS_Format_Type1_MULAW		(1 << 4)
#define	USB_Audio_AS_Format_Type1_RAW_DATA	(1 << 31)

/* Format Type 2 Bit Allocation (Fmts20 A.2.2) */
#define	USB_Audio_AS_Format_Type2_MPEG		(1 << 0)
#define	USB_Audio_AS_Format_Type2_AC3		(1 << 1)
#define	USB_Audio_AS_Format_Type2_WMA		(1 << 2)
#define	USB_Audio_AS_Format_Type2_DTS		(1 << 3)
#define	USB_Audio_AS_Format_Type2_RAW_DATA	(1 << 31)

/* Format Type 3 Bit Allocation (Fmts20 A.2.3) */
#define	USB_Audio_AS_Format_Type3_IEC61937_AC3			(1 << 0)
#define	USB_Audio_AS_Format_Type3_IEC61937_MPEG1_Layer1		(1 << 1)
#define	USB_Audio_AS_Format_Type3_IEC61937_MPEG2_NOEXT		(1 << 2)
#define	USB_Audio_AS_Format_Type3_IEC61937_MPEG2_EXT		(1 << 3)
#define	USB_Audio_AS_Format_Type3_IEC61937_MPEG2_AAC_ADTS	(1 << 4)
#define	USB_Audio_AS_Format_Type3_IEC61937_MPEG2_Layer1_LS	(1 << 5)
#define	USB_Audio_AS_Format_Type3_IEC61937_MPEG2_Layer23_LS	(1 << 6)
#define	USB_Audio_AS_Format_Type3_IEC61937_DTS1			(1 << 7)
#define	USB_Audio_AS_Format_Type3_IEC61937_DTS2			(1 << 8)
#define	USB_Audio_AS_Format_Type3_IEC61937_DTS3			(1 << 9)
#define	USB_Audio_AS_Format_Type3_IEC61937_ATRAC		(1 << 10)
#define	USB_Audio_AS_Format_Type3_IEC61937_ATRAC23		(1 << 11)
#define	USB_Audio_AS_Format_Type3_TYPE3_WMA			(1 << 12)

/* Format Type 4 Bit Allocation (Fmts20 A.2.4) */
#define	USB_Audio_AS_Format_Type4_PCM				(1 << 0)
#define	USB_Audio_AS_Format_Type4_PCM8				(1 << 1)
#define	USB_Audio_AS_Format_Type4_IEEE_FLOAT			(1 << 2)
#define	USB_Audio_AS_Format_Type4_ALAW				(1 << 3)
#define	USB_Audio_AS_Format_Type4_MULAW				(1 << 4)
#define	USB_Audio_AS_Format_Type4_MPEG				(1 << 5)
#define	USB_Audio_AS_Format_Type4_AC3				(1 << 6)
#define	USB_Audio_AS_Format_Type4_WMA				(1 << 7)
#define	USB_Audio_AS_Format_Type4_IEC61937_AC3			(1 << 8)
#define	USB_Audio_AS_Format_Type4_IEC61937_MPEG1_Layer1		(1 << 9)
#define	USB_Audio_AS_Format_Type4_IEC61937_MPEG2_NOEXT		(1 << 10)
#define	USB_Audio_AS_Format_Type4_IEC61937_MPEG2_EXT		(1 << 11)
#define	USB_Audio_AS_Format_Type4_IEC61937_MPEG2_AAC_ADTS	(1 << 12)
#define	USB_Audio_AS_Format_Type4_IEC61937_MPEG2_Layer1_LS	(1 << 13)
#define	USB_Audio_AS_Format_Type4_IEC61937_MPEG2_Layer23_LS	(1 << 14)
#define	USB_Audio_AS_Format_Type4_IEC61937_DTS1			(1 << 15)
#define	USB_Audio_AS_Format_Type4_IEC61937_DTS2			(1 << 16)
#define	USB_Audio_AS_Format_Type4_IEC61937_DTS3			(1 << 17)
#define	USB_Audio_AS_Format_Type4_IEC61937_ATRAC		(1 << 18)
#define	USB_Audio_AS_Format_Type4_IEC61937_ATRAC23		(1 << 19)
#define	USB_Audio_AS_Format_Type4_TYPE3_WMA			(1 << 20)
#define	USB_Audio_AS_Format_Type4_IEC60958_PCM			(1 << 21)


/****************************************************************************\
|
|	Encoder Descriptor (Table 4-28)
|
\****************************************************************************/

struct	USB_Audio_Encoder_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bEncoderID;
	unsigned char	bEncoder;
	unsigned char	bmControls[4];
	unsigned char	iParam1;
	unsigned char	iParam2;
	unsigned char	iParam3;
	unsigned char	iParam4;
	unsigned char	iParam5;
	unsigned char	iParam6;
	unsigned char	iParam7;
	unsigned char	iParam8;
	unsigned char	iEncoder;
	};

#define	USB_Audio_Encoder_Descriptor__SIZE			16

/* Encoder Type Codes. (A.15) */
#define	USB_Audio_bEncoder_ENCODER_UNDEFINED			0x00
#define	USB_Audio_bEncoder_OTHER_ENCODER			0x01
#define	USB_Audio_bEncoder_MPEG_ENCODER				0x02
#define	USB_Audio_bEncoder_AC3_ENCODER				0x03
#define	USB_Audio_bEncoder_WMA_ENCODER				0x04
#define	USB_Audio_bEncoder_DTS_ENCODER				0x05


#define	USB_Audio_bmControls_BitRateControl_Mask		0x00000003
#define	USB_Audio_bmControls_QualityControl_Mask		0x0000000C
#define	USB_Audio_bmControls_VBRControl_Mask			0x00000030
#define	USB_Audio_bmControls_TypeControl_Mask			0x000000C0
#define	USB_Audio_bmControls_UnderflowControl_Mask		0x00000300
#define	USB_Audio_bmControls_OverflowControl_Mask		0x00000C00
#define	USB_Audio_bmControls_EncoderErrorControl_Mask		0x00003000
#define	USB_Audio_bmControls_Param1Control_Mask			0x0000C000
#define	USB_Audio_bmControls_Param2Control_Mask			0x00030000
#define	USB_Audio_bmControls_Param3Control_Mask			0x000C0000
#define	USB_Audio_bmControls_Param4Control_Mask			0x00300000
#define	USB_Audio_bmControls_Param5Control_Mask			0x00C00000
#define	USB_Audio_bmControls_Param6Control_Mask			0x03000000
#define	USB_Audio_bmControls_Param7Control_Mask			0x0C000000
#define	USB_Audio_bmControls_Param8Control_Mask			0x30000000


/****************************************************************************\
|
|	Decoder Descriptor (Table 4-29, 4-30, 4-31, 4-32)
|
\****************************************************************************/

struct	USB_Audio_MPEG_Decoder_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bDecoderID;
	unsigned char	bDecoder;
	unsigned char	bmMPEGCapabilities[2];
	unsigned char	bmMPEGFeatures;
	unsigned char	bmControls;
	unsigned char	iDecoder;
	};

#define	USB_Audio_MPEG_Decoder_Descriptor__SIZE			10

/* Decoder Type Codes. (A.16) */
#define	USB_Audio_bDecoder_DECODER_UNDEFINED			0x00
#define	USB_Audio_bDecoder_OTHER_DECODER			0x01
#define	USB_Audio_bDecoder_MPEG_DECODER				0x02
#define	USB_Audio_bDecoder_AC3_DECODER				0x03
#define	USB_Audio_bDecoder_WMA_DECODER				0x04
#define	USB_Audio_bDecoder_DTS_DECODER				0x05


#define	USB_Audio_bmMPEGCapabilities_Layer_Mask			0x0007
#define	USB_Audio_bmMPEGCapabilities_Layer_1			0x0001
#define	USB_Audio_bmMPEGCapabilities_Layer_2			0x0002
#define	USB_Audio_bmMPEGCapabilities_Layer_3			0x0004
#define	USB_Audio_bmMPEGCapabilities_MPEG_1			0x0008
#define	USB_Audio_bmMPEGCapabilities_MPEG_1_DualChannel		0x0010
#define	USB_Audio_bmMPEGCapabilities_MPEG_2_SecondStereo	0x0020
#define	USB_Audio_bmMPEGCapabilities_MPEG_2_7_1Channel		0x0040
#define	USB_Audio_bmMPEGCapabilities_AdaptiveMultiChannel	0x0080
#define	USB_Audio_bmMPEGCapabilities_MPEG_2_Multilingual_Mask	0x0300
#define	USB_Audio_bmMPEGCapabilities_Support_HalfFs		0x0400

#define	USB_Audio_bmMPEGFeatures_RangeControl_Mask		0x30
#define	USB_Audio_bmMPEGFeatures_RangeControl_NotSupported	0x00
#define	USB_Audio_bmMPEGFeatures_RangeControl_NotScalable	0x10
#define	USB_Audio_bmMPEGFeatures_RangeControl_ScaleCommon	0x20
#define	USB_Audio_bmMPEGFeatures_RangeControl_ScaleSeparate	0x30

#define	USB_Audio_bmControls_DecUnderflowControl_Mask		0x03
#define	USB_Audio_bmControls_DecOverflowControl_Mask		0x0C
#define	USB_Audio_bmControls_DecoderErrorControl_Mask		0x30


/* AC3 Decoder Descriptor (Table 4-30) */
struct	USB_Audio_AC3_Decoder_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bDecoderID;
	unsigned char	bDecoder;
	unsigned char	bmBSID[4];
	unsigned char	bmAC3Features;
	unsigned char	bmControls;
	unsigned char	iDecoder;
	};

#define	USB_Audio_AC3_Decoder_Descriptor__SIZE			12

#define	USB_Audio_bmAC3Features_RF_Mode				0x01
#define	USB_Audio_bmAC3Features_Line_Mode			0x02
#define	USB_Audio_bmAC3Features_Custom0_Mode			0x03
#define	USB_Audio_bmAC3Features_Custom1_Mode			0x08
#define	USB_Audio_bmAC3Features_RangeControl_Mask		0x30
#define	USB_Audio_bmAC3Features_RangeControl_NotSupport		0x00
#define	USB_Audio_bmAC3Features_RangeControl_NotScalable	0x10
#define	USB_Audio_bmAC3Features_RangeControl_ScaleCommon	0x20
#define	USB_Audio_bmAC3Features_RangeControl_ScaleSeparate	0x30


/* WMA Decoder Descriptor (Table 4-31) */
struct	USB_Audio_WMA_Decoder_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bDecoderID;
	unsigned char	bDecoder;
	unsigned char	bmWMAProfile[2];
	unsigned char	bmControls;
	unsigned char	iDecoder;
	};

#define	USB_Audio_WMA_Decoder_Descriptor__SIZE			9

#define	USB_Audio_bmWMAProfile_L1				0x0001
#define	USB_Audio_bmWMAProfile_L2				0x0002
#define	USB_Audio_bmWMAProfile_L3				0x0004
#define	USB_Audio_bmWMAProfile_L				0x0008
#define	USB_Audio_bmWMAProfile_S1				0x0010
#define	USB_Audio_bmWMAProfile_S2				0x0020
#define	USB_Audio_bmWMAProfile_M1				0x0040
#define	USB_Audio_bmWMAProfile_M2				0x0080
#define	USB_Audio_bmWMAProfile_M3				0x0100
#define	USB_Audio_bmWMAProfile_M				0x0200
#define	USB_Audio_bmWMAProfile_Lossless				0x0400


/* DTS Decoder Descriptor (Table 4-31) */
struct	USB_Audio_DTS_Decoder_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bDecoderID;
	unsigned char	bDecoder;
	unsigned char	bmCapabilities;
	unsigned char	bmControls;
	unsigned char	iDecoder;
	};

#define	USB_Audio_DTS_Decoder_Descriptor__SIZE			8

#define	USB_Audio_bmCapabilities_Core				0x01
#define	USB_Audio_bmCapabilities_Lossless			0x02
#define	USB_Audio_bmCapabilities_LBR				0x04
#define	USB_Audio_bmCapabilities_MultipleStreamMixing		0x08
#define	USB_Audio_bmCapabilities_DualDecode			0x10


/****************************************************************************\
|
|	Class-Specific AS Isochronous Audio Data Endpoint Descriptor (Table 4-34)
|
\****************************************************************************/

struct	USB_Audio_AS_Data_Endpoint_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bmAttributes;
	unsigned char	bmControls;
	unsigned char	bLockDelayUnits;
	unsigned char	wLockDelay[2];
	};

#define	USB_Audio_AS_Data_Endpoint_Descriptor__SIZE		8

#define	USB_Audio_bmAttributes_MaxPacketsOnly			0x80

#define	USB_Audio_bmControls_PitchControl_Mask			0x03
#define	USB_Audio_bmControls_DataOverrunControl_Mask		0x0C
#define	USB_Audio_bmControls_DataUnderrunControl_Mask		0x30

#define	USB_Audio_bLockDelayUnits_Undefined			0x00
#define	USB_Audio_bLockDelayUnits_Milliseconds			0x01
#define	USB_Audio_bLockDelayUnits_DecodedPCMsamples		0x02


/****************************************************************************\
|
|	Audio Class-Specific Endpoint Descriptor Subtypes (A.13)
|
\****************************************************************************/

#define USB_Audio_bDescriptorSubtype_DESCRIPTOR_UNDEFINED	0x00
#define USB_Audio_bDescriptorSubtype_EP_GENERAL			0x01


/****************************************************************************\
|
|	Standard AS Isochronous Feedback Endpoint Descriptor (Table 4-35)
|
\****************************************************************************/

struct	USB_Audio_AS_Feedback_Endpoint_Descriptor
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bEndpointAddress;
	unsigned char	bmAttributes;
	unsigned char	wMaxPacketSize[2];
	unsigned char	bInterval;
	};

#define	USB_Audio_AS_Feedback_Endpoint_Descriptor__SIZE		7


/****************************************************************************\
|
|	Audio Class-Specific Request Codes (A.14)
|
\****************************************************************************/

#define	USB_Audio_bRequest_REQUEST_CODE_UNDEFINED		0x00
#define	USB_Audio_bRequest_CUR					0x01
#define	USB_Audio_bRequest_RANGE				0x02
#define	USB_Audio_bRequest_MEM					0x03

#define	USB_Audio_bRequest__INIT	\
	"REQUEST_CODE_UNDEFINED",	\
	"CUR",				\
	"RANGE",			\
	"MEM"


/****************************************************************************\
|
|	Audio Class-Specific Request Parameter Block
|
\****************************************************************************/

/*
|| Audio2.0 spec defines USB_Audio_RANGE_LayoutX_Parameter_Block structure
|| so that it could have multiple entires of Min/Max/Res pair.
|| But practically there's no case that multiple entries are necessary.
|| So, for clarity and simplicity, below structures were defined to have
|| only on Min/Max/Res pair entry.
||
||	__TMS_LIBPORT_PACKED1_DEFINITION()
||	#include "usbpump_poppack.h"
*/

/* Layout 1 Parameter Block */
struct	USB_Audio_CUR_Layout1_Parameter_Block
	{
	unsigned char	bCUR;
	};

struct	USB_Audio_RANGE_Layout1_Parameter_Block
	{
	unsigned char	wNumSubRanges[2];
	unsigned char	bMIN;
	unsigned char	bMAX;
	unsigned char	bRES;
	};

/* Layout 2 Parameter Block */
struct	USB_Audio_CUR_Layout2_Parameter_Block
	{
	unsigned char	wCUR[2];
	};

struct	USB_Audio_RANGE_Layout2_Parameter_Block
	{
	unsigned char	wNumSubRanges[2];
	unsigned char	wMIN[2];
	unsigned char	wMAX[2];
	unsigned char	wRES[2];
	};

/* Layout 3 Parameter Block */
struct	USB_Audio_CUR_Layout3_Parameter_Block
	{
	unsigned char	dCUR[4];
	};

struct	USB_Audio_RANGE_Layout3_Parameter_Block
	{
	unsigned char	wNumSubRanges[2];
	unsigned char	dMIN[4];
	unsigned char	dMAX[4];
	unsigned char	dRES[4];
	};


/****************************************************************************\
|
|	Control Selector Codes (A.17)
|
\****************************************************************************/

/* Clock Source Control Selectors (A.17.1) */
#define	USB_Audio_CS_CONTROL_UNDEFINED				0x00
#define	USB_Audio_CS_SAM_FREQ_CONTROL				0x01
#define	USB_Audio_CS_CLOCK_VALID_CONTROL			0x02

/* Clock Selector Control Selectors (A.17.2) */
#define	USB_Audio_CX_CONTROL_UNDEFINED				0x00
#define	USB_Audio_CX_CLOCK_SELECTOR_CONTROL			0x01

/* Clock Multiplier Control Selectors (A.17.3) */
#define	USB_Audio_CM_CONTROL_UNDEFINED				0x00
#define	USB_Audio_CM_NUMERATOR_CONTROL				0x01
#define	USB_Audio_CM_DENOMINATOR_CONTROL			0x02

/* Terminal Control Selectors (A.17.4) */
#define	USB_Audio_TE_CONTROL_UNDEFINED				0x00
#define	USB_Audio_TE_COPY_PROTECT_CONTROL			0x01
#define	USB_Audio_TE_CONNECTOR_CONTROL				0x02
#define	USB_Audio_TE_OVERLOAD_CONTROL				0x03
#define	USB_Audio_TE_CLUSTER_CONTROL				0x04
#define	USB_Audio_TE_UNDERFLOW_CONTROL				0x05
#define	USB_Audio_TE_OVERFLOW_CONTROL				0x06
#define	USB_Audio_TE_LATENCY_CONTROL				0x07

/* Mixer Control Selectors (A.17.5) */
#define	USB_Audio_MU_CONTROL_UNDEFINED				0x00
#define	USB_Audio_MU_MIXER_CONTROL				0x01
#define	USB_Audio_MU_CLUSTER_CONTROL				0x02
#define	USB_Audio_MU_UNDERFLOW_CONTROL				0x03
#define	USB_Audio_MU_OVERFLOW_CONTROL				0x04
#define	USB_Audio_MU_LATENCY_CONTROL				0x05

/* Selector Control Selectors (A.17.6) */
#define	USB_Audio_SU_CONTROL_UNDEFINED				0x00
#define	USB_Audio_SU_SELECTOR_CONTROL				0x01
#define	USB_Audio_SU_LATENCY_CONTROL				0x02

/* Feature Unit Control Selectors (A.17.7) */
#define	USB_Audio_FU_CONTROL_UNDEFINED				0x00
#define	USB_Audio_FU_MUTE_CONTROL				0x01
#define	USB_Audio_FU_VOLUME_CONTROL				0x02
#define	USB_Audio_FU_BASS_CONTROL				0x03
#define	USB_Audio_FU_MID_CONTROL				0x04
#define	USB_Audio_FU_TREBLE_CONTROL				0x05
#define	USB_Audio_FU_GRAPHIC_EQUALIZER_CONTROL			0x06
#define	USB_Audio_FU_AUTOMATIC_GAIN_CONTROL			0x07
#define	USB_Audio_FU_DELAY_CONTROL				0x08
#define	USB_Audio_FU_BASS_BOOST_CONTROL				0x09
#define	USB_Audio_FU_LOUDNESS_CONTROL				0x0A
#define	USB_Audio_FU_INPUT_GAIN_CONTROL				0x0B
#define	USB_Audio_FU_INPUT_GAIN_PAD_CONTROL			0x0C
#define	USB_Audio_FU_PHASE_INVERTER_CONTROL			0x0D
#define	USB_Audio_FU_UNDERFLOW_CONTROL				0x0E
#define	USB_Audio_FU_OVERFLOW_CONTROL				0x0F
#define	USB_Audio_FU_LATENCY_CONTROL				0x10

/* Parametric Equalizer Section Effect Unit Control Selectors (A.17.8.1) */
#define	USB_Audio_PE_CONTROL_UNDEFINED				0x00
#define	USB_Audio_PE_ENABLE_CONTROL				0x01
#define	USB_Audio_PE_CENTERFREQ_CONTROL				0x02
#define	USB_Audio_PE_QFACTOR_CONTROL				0x03
#define	USB_Audio_PE_GAIN_CONTROL				0x04
#define	USB_Audio_PE_UNDERFLOW_CONTROL				0x05
#define	USB_Audio_PE_OVERFLOW_CONTROL				0x06
#define	USB_Audio_PE_LATENCY_CONTROL				0x07

/* Reverberation Effect Unit Control Selectors (A.17.8.2) */
#define	USB_Audio_RV_CONTROL_UNDEFINED				0x00
#define	USB_Audio_RV_ENABLE_CONTROL				0x01
#define	USB_Audio_RV_TYPE_CONTROL				0x02
#define	USB_Audio_RV_LEVEL_CONTROL				0x03
#define	USB_Audio_RV_TIME_CONTROL				0x04
#define	USB_Audio_RV_FEEDBACK_CONTROL				0x05
#define	USB_Audio_RV_PREDELAY_CONTROL				0x06
#define	USB_Audio_RV_DENSITY_CONTROL				0x07
#define	USB_Audio_RV_HIFREQ_ROLLOFF_CONTROLL			0x08
#define	USB_Audio_RV_UNDERFLOW_CONTROL				0x09
#define	USB_Audio_RV_OVERFLOW_CONTROL				0x0A
#define	USB_Audio_RV_LATENCY_CONTROL				0x0B

/* Modulation Delay Effect Unit Control Selectors (A.17.8.3) */
#define	USB_Audio_MD_CONTROL_UNDEFINED				0x00
#define	USB_Audio_MD_ENABLE_CONTROL				0x01
#define	USB_Audio_MD_BALANCE_CONTROL				0x02
#define	USB_Audio_MD_RATE_CONTROL				0x03
#define	USB_Audio_MD_DEPTH_CONTROL				0x04
#define	USB_Audio_MD_TIME_CONTROL				0x05
#define	USB_Audio_MD_FEEDBACK_CONTROL				0x06
#define	USB_Audio_MD_UNDERFLOW_CONTROL				0x07
#define	USB_Audio_MD_OVERFLOW_CONTROL				0x08
#define	USB_Audio_MD_LATENCY_CONTROL				0x09

/* Dynamic Range Compressor Effect Unit Control Selectors (A.17.8.4) */
#define	USB_Audio_DR_CONTROL_UNDEFINED				0x00
#define	USB_Audio_DR_ENABLE_CONTROL				0x01
#define	USB_Audio_DR_COMPRESSION_RATE_CONTROL			0x02
#define	USB_Audio_DR_MAXAMPL_CONTROL				0x03
#define	USB_Audio_DR_THRESHOLD_CONTROL				0x04
#define	USB_Audio_DR_ATTACK_TIME_CONTROL			0x05
#define	USB_Audio_DR_RELEASE_TIME_CONTROL			0x06
#define	USB_Audio_DR_UNDERFLOW_CONTROL				0x07
#define	USB_Audio_DR_OVERFLOW_CONTROL				0x08
#define	USB_Audio_DR_LATENCY_CONTROL				0x09

/* Up/Down-mix Processing Unit Control Selectors (A.17.9.1) */
#define	USB_Audio_UD_CONTROL_UNDEFINED				0x00
#define	USB_Audio_UD_ENABLE_CONTROL				0x01
#define	USB_Audio_UD_MODE_SELECT_CONTROL			0x02
#define	USB_Audio_UD_CLUSTER_CONTROL				0x03
#define	USB_Audio_UD_UNDERFLOW_CONTROL				0x04
#define	USB_Audio_UD_OVERFLOW_CONTROL				0x05
#define	USB_Audio_UD_LATENCY_CONTROL				0x06

/* Dolby Prologic Processing Unit Control Selectors (A.17.9.2) */
#define	USB_Audio_DP_CONTROL_UNDEFINED				0x00
#define	USB_Audio_DP_ENABLE_CONTROL				0x01
#define	USB_Audio_DP_MODE_SELECT_CONTROL			0x02
#define	USB_Audio_DP_CLUSTER_CONTROL				0x03
#define	USB_Audio_DP_UNDERFLOW_CONTROL				0x04
#define	USB_Audio_DP_OVERFLOW_CONTROL				0x05
#define	USB_Audio_DP_LATENCY_CONTROL				0x06

/* Stereo Extender Processing Unit Control Selectors (A.17.9.3) */
#define	USB_Audio_ST_EXT_CONTROL_UNDEFINED			0x00
#define	USB_Audio_ST_EXT_ENABLE_CONTROL				0x01
#define	USB_Audio_ST_EXT_WIDTH_CONTROL				0x02
#define	USB_Audio_ST_EXT_UNDERFLOW_CONTROL			0x03
#define	USB_Audio_ST_EXT_OVERFLOW_CONTROL			0x04
#define	USB_Audio_ST_EXT_LATENCY_CONTROL			0x05

/* Extension Unit Control Selectors (A.17.10) */
#define	USB_Audio_XU_CONTROL_UNDEFINED				0x00
#define	USB_Audio_XU_ENABLE_CONTROL				0x01
#define	USB_Audio_XU_CLUSTER_CONTROL				0x02
#define	USB_Audio_XU_UNDERFLOW_CONTROL				0x03
#define	USB_Audio_XU_OVERFLOW_CONTROL				0x04
#define	USB_Audio_XU_LATENCY_CONTROL				0x05

/* AudiStreaming Interface Control Selectors (A.17.11) */
#define	USB_Audio_AS_CONTROL_UNDEFINED				0x00
#define	USB_Audio_AS_ACT_ALT_SETTING_CONTROL			0x01
#define	USB_Audio_AS_VAL_ALT_SETTING_CONTROL			0x02
#define	USB_Audio_AS_AUDIO_DATA_FORMAT_CONTROL			0x03

/* Encoder Control Selectors (A.17.12) */
#define	USB_Audio_EN_CONTROL_UNDEFINED				0x00
#define	USB_Audio_EN_BIT_RATE_CONTROL				0x01
#define	USB_Audio_EN_QUALITY_CONTROL				0x02
#define	USB_Audio_EN_VBR_CONTROL				0x03
#define	USB_Audio_EN_TYPE_CONTROL				0x04
#define	USB_Audio_EN_UNDERFLOW_CONTROL				0x05
#define	USB_Audio_EN_OVERFLOW_CONTROL				0x06
#define	USB_Audio_EN_ENCODER_ERROR_CONTROL			0x07
#define	USB_Audio_EN_PARAM1_CONTROL				0x08
#define	USB_Audio_EN_PARAM2_CONTROL				0x09
#define	USB_Audio_EN_PARAM3_CONTROL				0x0A
#define	USB_Audio_EN_PARAM4_CONTROL				0x0B
#define	USB_Audio_EN_PARAM5_CONTROL				0x0C
#define	USB_Audio_EN_PARAM6_CONTROL				0x0D
#define	USB_Audio_EN_PARAM7_CONTROL				0x0E
#define	USB_Audio_EN_PARAM8_CONTROL				0x0F

/* MPEG Decoder Control Selectors (A.17.13.1) */
#define	USB_Audio_MD_CONTROL_UNDEFINED				0x00
#define	USB_Audio_MD_DUAL_CHANNEL_CONTROL			0x01
#define	USB_Audio_MD_SECOND_STEREO_CONTROL			0x02
#define	USB_Audio_MD_MULTILINGUAL_CONTROL			0x03
#define	USB_Audio_MD_DYN_RANGE_CONTROL				0x04
#define	USB_Audio_MD_SCALING_CONTROL				0x05
#define	USB_Audio_MD_HILO_SCALING_CONTROL			0x06
#define	USB_Audio_MD_UNDERFLOW_CONTROL				0x07
#define	USB_Audio_MD_OVERFLOW_CONTROL				0x08
#define	USB_Audio_MD_DECODER_ERROR_CONTROL			0x09

/* AC-3 Decoder Control Selectors (A.17.13.2) */
#define	USB_Audio_AD_CONTROL_UNDEFINED				0x00
#define	USB_Audio_AD_MODE_CONTROL				0x01
#define	USB_Audio_AD_DYN_RANGE_CONTROL				0x02
#define	USB_Audio_AD_SCALING_CONTROL				0x03
#define	USB_Audio_AD_HILO_SCALING_CONTROL			0x04
#define	USB_Audio_AD_UNDERFLOW_CONTROL				0x05
#define	USB_Audio_AD_OVERFLOW_CONTROL				0x06
#define	USB_Audio_AD_DECODER_ERROR_CONTROL			0x07

/* WMA Decoder Control Selectors (A.17.13.3) */
#define	USB_Audio_WD_CONTROL_UNDEFINED				0x00
#define	USB_Audio_WD_UNDERFLOW_CONTROL				0x01
#define	USB_Audio_WD_OVERFLOW_CONTROL				0x02
#define	USB_Audio_WD_DECODER_ERROR_CONTROL			0x03

/* DTS Decoder Control Selectors (A.17.13.4) */
#define	USB_Audio_DD_CONTROL_UNDEFINED				0x00
#define	USB_Audio_DD_UNDERFLOW_CONTROL				0x01
#define	USB_Audio_DD_OVERFLOW_CONTROL				0x02
#define	USB_Audio_DD_DECODER_ERROR_CONTROL			0x03

/* DTS Decoder Control Selectors (A.17.13.4) */
#define	USB_Audio_DD_CONTROL_UNDEFINED				0x00
#define	USB_Audio_DD_UNDERFLOW_CONTROL				0x01
#define	USB_Audio_DD_OVERFLOW_CONTROL				0x02
#define	USB_Audio_DD_DECODER_ERROR_CONTROL			0x03

/* Endpoint Control Selectors (A.17.14) */
#define	USB_Audio_EP_CONTROL_UNDEFINED				0x00
#define	USB_Audio_EP_PICTCH_CONTROL				0x01
#define	USB_Audio_EP_DATA_OVERFLOW_CONTROL			0x02
#define	USB_Audio_EP_DATA_UNDERFLOW_CONTROL			0x03


/****************************************************************************\
|
|	Interrupt Data Message Format
|
\****************************************************************************/

struct	USB_Audio_Interrupt_Data_Message
	{
	unsigned char	bInfo;
	unsigned char	bAttribute;
	unsigned char	wValue[2];
	unsigned char	wIndex[2];
	};

#define	USB_Audio_Interrupt_Data_Message__SIZE		6

#define	USB_Audio_bInfo_VendorSpecific_Mask		0x01
#define	USB_Audio_bInfo_InterfaceOrEndpoint_Mask	0x02


/**** end of usbaudio20.h ****/
#endif /* _USBAUDIO20_H_ */
