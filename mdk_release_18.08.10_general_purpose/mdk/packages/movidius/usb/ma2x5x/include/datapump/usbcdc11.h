/* usbcdc11.h	Wed Aug 10 2005 01:08:46 tmm */

/*

Module:  usbcdc11.h

Function:
	USB Communication Device Class (CDC) V1.1 constants, augmented by
	WMC 1.0

Version:
	V1.91d	Wed Aug 10 2005 01:08:46 tmm	Edit level 12

Copyright notice:
	This file copyright (C) 1998-2001, 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	Distribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistribution of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. MCCI's name may not be used to endorse or promote products
	   derived from this software without specific prior written
	   permission.

Disclaimer:
	THIS FILE IS PROVIDED BY MCCI "AS IS" AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED.  IN NO EVENT SHALL MCCI BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
	GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS FILE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
Author:
	Terry Moore, MCCI Corporation	September 1998

Review Status:
   1.45e	Check compiled, tested with some MCCI products.
   1.60e	Check compiled

Revision history:
   1.45d  Sat Sep 26 1998 17:26:02  tmm
	Module created.

   1.45d  10/16/1998  tmm
	Revised for 1.0.9RC spec.  RR63 renumbered a lot of descriptor codes.
	RR64 removed a bit in the multi-channel management descriptor.  RR66
	renamed some bits.

   1.45d  10/27/1998 johnk
	Minor problems  detected and fixed after use with usbwin32/cdc code.

   1.45e  09/01/1999 sgs
	Added USB CDC definitions for MCM, CAPI, ETH, and ATM (0x04 - 0x07).
	Left in the MCCI vendor specific version definitions.

   1.60e  7/23/2000  tmm
	Clean up prior to release.  Change to open-source copyright/
	disclaimer.

   1.60m  9/19/2000  tmm
	Merge in the changes from MPC850.

   1.61h  3/10/2001  tmm
	Add CDC-Vendor-Interface functional descriptor codes.

   1.62b  5/22/2001  tmm
	Add an explicit length for the LINE_CODING structure.

   1.63f  12/5/2001  tmm
	Eliminate duplicate request declarations, add WMC 1.0 support, and
	change the name of USB_bInterfaceSubClass_Comm_INIT to
	USB_bInterfaceSubClass_Comm__INIT for consistency with other
	declarations in this file.  (The old name was retained for backwards
	compatibility.)

   1.91f  2005-08-10  tmm
	1516:  correct error in USB_Comm_FunctionalDescriptor_NETTERMINAL
	field bPhysicalInterface (was bPhysicalINterface, note extra 'N').

*/

#ifndef _USBCDC11_H_		/* prevent multiple includes */
#define _USBCDC11_H_

/****************************************************************************\
|
|	Class, subclass, and protocol codes.
|
\****************************************************************************/

/* class codes */
#define	USB_bDeviceClass_Comm		0x02	/* communication class
						|| device 
						*/
#define	USB_bInterfaceClass_Comm	0x02	/* the communication class */
#define	USB_bInterfaceClass_Data	0x0A	/* the data class */

/* Interface subclasses */
#define	USB_bInterfaceSubClass_CommDLC	0x01	/* Direct Line Control */
#define	USB_bInterfaceSubClass_CommACM	0x02	/* Abstract Control Model */
#define	USB_bInterfaceSubClass_CommTCM	0x03	/* Telehpone Control Model */
#define	USB_bInterfaceSubClass_CommMCM	0x04	/* multi-channel model */
#define	USB_bInterfaceSubClass_CommCAPI	0x05	/* CAPI */
#define	USB_bInterfaceSubClass_CommETH	0x06	/* Ethernet */
#define	USB_bInterfaceSubClass_CommATM	0x07	/* ATM */
#define	USB_bInterfaceSubClass_CommWHCM	0x08	/* Wireless Handset Control Model  */
#define	USB_bInterfaceSubClass_CommDEVMGMT 0x09	/* Device Management */
#define	USB_bInterfaceSubClass_CommMDLM	0x0A	/* Mobile Direct Line */
#define	USB_bInterfaceSubClass_CommOBEX	0x0B	/* Object Exchange */

/* 
|| Pre 1.1 stuff -- this was defined when the specification was still at 
|| 1.09rc, so we used 'mcci' values.  CAUTION:  these should only be used
|| in devices that are marked bDeviceClass == VENDOR_SPECIFIC
*/
#define	MCCI_bInterfaceSubClass_CommETH		0x86	/* Ethernet */

/* A table initializer */
#define	USB_bInterfaceSubClass_Comm__INIT					\
	{ USB_bInterfaceSubClass_CommDLC, "Direct Line Control" },		\
	{ USB_bInterfaceSubClass_CommACM, "Abstract Control Model" },		\
	{ USB_bInterfaceSubClass_CommTCM, "Telephone Control Model" },		\
	{ USB_bInterfaceSubClass_CommMCM, "Multi-Channel Model" },		\
	{ USB_bInterfaceSubClass_CommCAPI, "CAPI" },				\
	{ USB_bInterfaceSubClass_CommETH, "Ethernet Control Model"},		\
	{ USB_bInterfaceSubClass_CommATM, "ATM Control Model" },		\
	{ USB_bInterfaceSubClass_CommWHCM, "Wireless Handset Control Model" },	\
	{ USB_bInterfaceSubClass_CommDEVMGMT, "Device Management" },		\
	{ USB_bInterfaceSubClass_CommMDLM, "Mobile Direct Line" },		\
	{ USB_bInterfaceSubClass_CommOBEX, "Object Exchange" },			\
	{ MCCI_bInterfaceSubClass_CommETH, "MCCI Ethernet Control Model"}

/* an obsolete name */
#define	USB_bInterfaceSubClass_Comm_INIT	\
	USB_bInterfaceSubClass_Comm__INIT

/* Comm class Protocol codes */
#define	USB_bInterfaceProtocol_CommV25ter	0x01	/* "AT" commands */
#define	USB_bInterfaceProtocol_CommPCCA101	0x02	/* PCCA STD-101 */
#define	USB_bInterfaceProtocol_CommPCCA101AO	0x03	/* STD101 + Annex O */
#define	USB_bInterfaceProtocol_CommGSM0707	0x04	/* GSM 07.07 */
#define	USB_bInterfaceProtocol_Comm3GPP27007	0x05	/* 3GPP 27.007 */
#define	USB_bInterfaceProtocol_CommCS0017_0	0x06	/* CDMA C-S0017-0 */
#define	USB_bInterfaceProtocol_CommEXTERNAL	0xFE	/* identified by GUID */

#define	USB_bInterfaceProtocol_Comm_INIT \
	{ USB_bInterfaceProtocol_CommV25ter, "V.25ter" },			\
	{ USB_bInterfaceProtocol_CommPCCA101, "PCCA STD-101" },			\
	{ USB_bInterfaceProtocol_CommPCCA101AO, "PCCA STD-101+Annex O" },	\
	{ USB_bInterfaceProtocol_CommGSM0707, "GSM 07.07" },			\
	{ USB_bInterfaceProtocol_Comm3GPP27007, "3GPP 27.007" },		\
	{ USB_bInterfaceProtocol_CommCS0017_0, "CDMA C-S0017-0" },		\
	{ USB_bInterfaceProtocol_CommEXTERNAL, "identified by GUID" }		\

/* Data class protocol codes from Table 19 */
#define	USB_bInterfaceProtocol_DataI430	  0x30	/* I.430 */
#define	USB_bInterfaceProtocol_DataHDLC	  0x31	/* HDLC (ISO 3309-1993) */
#define	USB_bInterfaceProtocol_DataTRANS  0x32	/* Transparent */
#define	USB_bInterfaceProtocol_DataQ921M  0x50	/* Q.921M management */
#define	USB_bInterfaceProtocol_DataQ921   0x51	/* Q.921 data */
#define	USB_bInterfaceProtocol_DataQ921TM 0x52	/* Q.921 TEI multiplexor */
#define	USB_bInterfaceProtocol_DataV42BIS 0x90	/* V.42bis data compression */
#define	USB_bInterfaceProtocol_DataQ931   0x91	/* Q.931/Euro ISDN */
#define	USB_bInterfaceProtocol_DataV120   0x92	/* V.120 rate adaptation */
#define	USB_bInterfaceProtocol_DataCAPI2  0x93	/* CAPI 2.0 */
#define	USB_bInterfaceProtocol_DataHOST	  0xFD	/* Host driver */
#define	USB_bInterfaceProtocol_DataPUF    0xFE	/* Protocol from Protocol 
						|| Unit Functional Desciptor
						*/

/* a table initializer */
#define	USB_bInterfaceProtocol_Data_INIT \
	{ USB_bInterfaceProtocol_DataI430, "I.430" }, \
	{ USB_bInterfaceProtocol_DataHDLC, "HDLC" }, \
	{ USB_bInterfaceProtocol_DataTRANS, "Transparent" }, \
	{ USB_bInterfaceProtocol_DataQ921M, "Q.921 Management" }, \
	{ USB_bInterfaceProtocol_DataQ921,  "Q.921 Data" }, \
	{ USB_bInterfaceProtocol_DataQ921TM, "Q.921 TEI-multiplexor" }, \
	{ USB_bInterfaceProtocol_DataV42BIS, "V.42bis compression" }, \
	{ USB_bInterfaceProtocol_DataQ931,  "Q.931/Euro-ISDN" }, \
	{ USB_bInterfaceProtocol_DataV120,  "V.120" }, \
	{ USB_bInterfaceProtocol_DataCAPI2, "CAPI 2.0" }, \
	{ USB_bInterfaceProtocol_DataHOST,  "Host Driver" }, \
	{ USB_bInterfaceProtocol_DataPUF,   "Protocol from Protocol Unit Descriptor" }

/****************************************************************************\
|
|	Management Element Requests (6.2, table 45 and [cdcwmc10], section 5
|	(These are the request codes in SETUP packets)
|
\****************************************************************************/

#define	USB_bRequest_Comm_SEND_ENCAPSULATED_COMMAND		0x00
#define	USB_bRequest_Comm_GET_ENCAPSULATED_RESPONSE		0x01
#define	USB_bRequest_Comm_SET_COMM_FEATURE			0x02
#define	USB_bRequest_Comm_GET_COMM_FEATURE			0x03
#define	USB_bRequest_Comm_CLEAR_COMM_FEATURE			0x04
#define	USB_bRequest_Comm_SET_AUX_LINE_STATE			0x10
#define	USB_bRequest_Comm_SET_HOOK_STATE			0x11
#define	USB_bRequest_Comm_PULSE_SETUP				0x12
#define	USB_bRequest_Comm_SEND_PULSE				0x13
#define	USB_bRequest_Comm_SET_PULSE_TIME			0x14
#define	USB_bRequest_Comm_RING_AUX_JACK				0x15
#define	USB_bRequest_Comm_SET_LINE_CODING			0x20
#define	USB_bRequest_Comm_GET_LINE_CODING			0x21
#define	USB_bRequest_Comm_SET_CONTROL_LINE_STATE		0x22
#define	USB_bRequest_Comm_SEND_BREAK				0x23
#define	USB_bRequest_Comm_SET_RINGER_PARMS			0x30
#define	USB_bRequest_Comm_GET_RINGER_PARMS			0x31
#define	USB_bRequest_Comm_SET_OPERATION_PARMS			0x32
#define	USB_bRequest_Comm_GET_OPERATION_PARMS			0x33
#define	USB_bRequest_Comm_SET_LINE_PARMS			0x34
#define	USB_bRequest_Comm_GET_LINE_PARMS			0x35
#define	USB_bRequest_Comm_DIAL_DIGITS				0x36
#define	USB_bRequest_Comm_SET_UNIT_PARAMETER			0x37
#define	USB_bRequest_Comm_GET_UNIT_PARAMETER			0x38
#define	USB_bRequest_Comm_CLEAR_UNIT_PARAMETER			0x39
#define	USB_bRequest_Comm_GET_PROFILE				0x3A
#define	USB_bRequest_Comm_SET_ETHERNET_MULTICAST_FILTERS	0x40
#define	USB_bRequest_Comm_SET_ETHERNET_POWER_MGMT_FILTER	0x41
#define	USB_bRequest_Comm_GET_ETHERNET_POWER_MGMT_FILTER	0x42
#define	USB_bRequest_Comm_SET_ETHERNET_PACKET_FILTER		0x43
#define	USB_bRequest_Comm_GET_ETHERNET_STATISTIC		0x44
#define	USB_bRequest_Comm_SET_ATM_DATA_FORMAT			0x50
#define	USB_bRequest_Comm_GET_ATM_DEVICE_STATISTICS		0x51
#define	USB_bRequest_Comm_SET_ATM_DEFAULT_VC			0x52
#define	USB_bRequest_Comm_GET_ATM_VC_STATISTICS			0x53
#define	USB_bRequest_Comm_MDLM_60				0x60
#define	USB_bRequest_Comm_MDLM_61				0x61
#define	USB_bRequest_Comm_MDLM_62				0x62
#define	USB_bRequest_Comm_MDLM_63				0x63
#define	USB_bRequest_Comm_MDLM_64				0x64
#define	USB_bRequest_Comm_MDLM_65				0x65
#define	USB_bRequest_Comm_MDLM_66				0x66
#define	USB_bRequest_Comm_MDLM_67				0x67
#define	USB_bRequest_Comm_MDLM_68				0x68
#define	USB_bRequest_Comm_MDLM_69				0x68
#define	USB_bRequest_Comm_MDLM_6A				0x6A
#define	USB_bRequest_Comm_MDLM_6B				0x6B
#define	USB_bRequest_Comm_MDLM_6C				0x6C
#define	USB_bRequest_Comm_MDLM_6D				0x6D
#define	USB_bRequest_Comm_MDLM_6E				0x6E
#define	USB_bRequest_Comm_MDLM_6F				0x6F
#define	USB_bRequest_Comm_MDLM_70				0x70
#define	USB_bRequest_Comm_MDLM_71				0x71
#define	USB_bRequest_Comm_MDLM_72				0x72
#define	USB_bRequest_Comm_MDLM_73				0x73
#define	USB_bRequest_Comm_MDLM_74				0x74
#define	USB_bRequest_Comm_MDLM_75				0x75
#define	USB_bRequest_Comm_MDLM_76				0x76
#define	USB_bRequest_Comm_MDLM_77				0x77
#define	USB_bRequest_Comm_MDLM_78				0x78
#define	USB_bRequest_Comm_MDLM_79				0x79
#define	USB_bRequest_Comm_MDLM_7A				0x7A
#define	USB_bRequest_Comm_MDLM_7B				0x7B
#define	USB_bRequest_Comm_MDLM_7C				0x7C
#define	USB_bRequest_Comm_MDLM_7D				0x7D
#define	USB_bRequest_Comm_MDLM_7E				0x7E
#define	USB_bRequest_Comm_MDLM_7F				0x7F

#define	USB_bRequest_Comm__INIT \
	{ USB_bRequest_Comm_SEND_ENCAPSULATED_COMMAND, "SEND_ENCAPSULATED_COMMAND" }, \
	{ USB_bRequest_Comm_GET_ENCAPSULATED_RESPONSE, "GET_ENCAPSULATED_RESPONSE" }, \
	{ USB_bRequest_Comm_SET_COMM_FEATURE, "SET_COMM_FEATURE" }, \
	{ USB_bRequest_Comm_GET_COMM_FEATURE, "GET_COMM_FEATURE" }, \
	{ USB_bRequest_Comm_CLEAR_COMM_FEATURE, "CLEAR_COMM_FEATURE" }, \
	{ USB_bRequest_Comm_SET_AUX_LINE_STATE, "SET_AUX_LINE_STATE" }, \
	{ USB_bRequest_Comm_SET_HOOK_STATE, "SET_HOOK_STATE" }, \
	{ USB_bRequest_Comm_PULSE_SETUP, "PULSE_SETUP" }, \
	{ USB_bRequest_Comm_SEND_PULSE, "SEND_PULSE" }, \
	{ USB_bRequest_Comm_SET_PULSE_TIME, "SET_PULSE_TIME" }, \
	{ USB_bRequest_Comm_RING_AUX_JACK, "RING_AUX_JACK" }, \
	{ USB_bRequest_Comm_SET_LINE_CODING, "SET_LINE_CODING" }, \
	{ USB_bRequest_Comm_GET_LINE_CODING, "GET_LINE_CODING" }, \
	{ USB_bRequest_Comm_SET_CONTROL_LINE_STATE, "SET_CONTROL_LINE_STATE" }, \
	{ USB_bRequest_Comm_SEND_BREAK, "SEND_BREAK" }, \
	{ USB_bRequest_Comm_SET_RINGER_PARMS, "SET_RINGER_PARMS" }, \
	{ USB_bRequest_Comm_GET_RINGER_PARMS, "GET_RINGER_PARMS" }, \
	{ USB_bRequest_Comm_SET_OPERATION_PARMS, "SET_OPERATION_PARMS" }, \
	{ USB_bRequest_Comm_GET_OPERATION_PARMS, "GET_OPERATION_PARMS" }, \
	{ USB_bRequest_Comm_SET_LINE_PARMS, "SET_LINE_PARMS" }, \
	{ USB_bRequest_Comm_GET_LINE_PARMS, "GET_LINE_PARMS" }, \
	{ USB_bRequest_Comm_DIAL_DIGITS, "DIAL_DIGITS" }, \
	{ USB_bRequest_Comm_SET_UNIT_PARAMETER, "SET_UNIT_PARAMETER" }, \
	{ USB_bRequest_Comm_GET_UNIT_PARAMETER, "GET_UNIT_PARAMETER" }, \
	{ USB_bRequest_Comm_CLEAR_UNIT_PARAMETER, "CLEAR_UNIT_PARAMETER" }, \
	{ USB_bRequest_Comm_GET_PROFILE, "GET_PROFILE" }, \
	{ USB_bRequest_Comm_SET_ETHERNET_MULTICAST_FILTERS, "SET_ETHERNET_MULTICAST_FILTERS" }, \
	{ USB_bRequest_Comm_SET_ETHERNET_POWER_MGMT_FILTER, "SET_ETHERNET_POWER_MGMT_FILTER" }, \
	{ USB_bRequest_Comm_GET_ETHERNET_POWER_MGMT_FILTER, "GET_ETHERNET_POWER_MGMT_FILTER" }, \
	{ USB_bRequest_Comm_SET_ETHERNET_PACKET_FILTER, "SET_ETHERNET_PACKET_FILTER" }, \
	{ USB_bRequest_Comm_GET_ETHERNET_STATISTIC, "GET_ETHERNET_STATISTIC" }, \
	{ USB_bRequest_Comm_SET_ATM_DATA_FORMAT, "SET_ATM_DATA_FORMAT" }, \
	{ USB_bRequest_Comm_GET_ATM_DEVICE_STATISTICS, "GET_ATM_DEVICE_STATISTICS" }, \
	{ USB_bRequest_Comm_SET_ATM_DEFAULT_VC, "SET_ATM_DEFAULT_VC" }, \
	{ USB_bRequest_Comm_GET_ATM_VC_STATISTICS, "GET_ATM_VC_STATISTICS" }, \
	{ USB_bRequest_Comm_MDLM_60, "MDLM_60" }, \
	{ USB_bRequest_Comm_MDLM_61, "MDLM_61" }, \
	{ USB_bRequest_Comm_MDLM_62, "MDLM_62" }, \
	{ USB_bRequest_Comm_MDLM_63, "MDLM_63" }, \
	{ USB_bRequest_Comm_MDLM_64, "MDLM_64" }, \
	{ USB_bRequest_Comm_MDLM_65, "MDLM_65" }, \
	{ USB_bRequest_Comm_MDLM_66, "MDLM_66" }, \
	{ USB_bRequest_Comm_MDLM_67, "MDLM_67" }, \
	{ USB_bRequest_Comm_MDLM_68, "MDLM_68" }, \
	{ USB_bRequest_Comm_MDLM_69, "MDLM_69" }, \
	{ USB_bRequest_Comm_MDLM_6A, "MDLM_6A" }, \
	{ USB_bRequest_Comm_MDLM_6B, "MDLM_6B" }, \
	{ USB_bRequest_Comm_MDLM_6C, "MDLM_6C" }, \
	{ USB_bRequest_Comm_MDLM_6D, "MDLM_6D" }, \
	{ USB_bRequest_Comm_MDLM_6E, "MDLM_6E" }, \
	{ USB_bRequest_Comm_MDLM_6F, "MDLM_6F" }, \
	{ USB_bRequest_Comm_MDLM_70, "MDLM_70" }, \
	{ USB_bRequest_Comm_MDLM_71, "MDLM_71" }, \
	{ USB_bRequest_Comm_MDLM_72, "MDLM_72" }, \
	{ USB_bRequest_Comm_MDLM_73, "MDLM_73" }, \
	{ USB_bRequest_Comm_MDLM_74, "MDLM_74" }, \
	{ USB_bRequest_Comm_MDLM_75, "MDLM_75" }, \
	{ USB_bRequest_Comm_MDLM_76, "MDLM_76" }, \
	{ USB_bRequest_Comm_MDLM_77, "MDLM_77" }, \
	{ USB_bRequest_Comm_MDLM_78, "MDLM_78" }, \
	{ USB_bRequest_Comm_MDLM_79, "MDLM_79" }, \
	{ USB_bRequest_Comm_MDLM_7A, "MDLM_7A" }, \
	{ USB_bRequest_Comm_MDLM_7B, "MDLM_7B" }, \
	{ USB_bRequest_Comm_MDLM_7C, "MDLM_7C" }, \
	{ USB_bRequest_Comm_MDLM_7D, "MDLM_7D" }, \
	{ USB_bRequest_Comm_MDLM_7E, "MDLM_7E" }, \
	{ USB_bRequest_Comm_MDLM_7F, "MDLM_7F" }

/****************************************************************************\
|
|	Notifications
|
\****************************************************************************/

/* the numerical constant */
#define	USB_bNotification_Comm_NETWORK_CONNECTION	0x00
#define	USB_bNotification_Comm_RESPONSE_AVAILABLE	0x01
#define	USB_bNotification_Comm_AUX_JACK_HOOK_STATE	0x08
#define	USB_bNotification_Comm_RING_DETECT		0x09
#define	USB_bNotification_Comm_SERIAL_STATE		0x20
#define	USB_bNotification_Comm_CALL_STATE_CHANGE	0x28
#define	USB_bNotification_Comm_LINE_STATE_CHANGE	0x29
#define	USB_bNotification_Comm_CONNECTION_SPEED_CHANGE	0x2A
#define	USB_bNotification_Comm_MDLM_40			0x40
#define	USB_bNotification_Comm_MDLM_41			0x41
#define	USB_bNotification_Comm_MDLM_42			0x42
#define	USB_bNotification_Comm_MDLM_43			0x43
#define	USB_bNotification_Comm_MDLM_44			0x44
#define	USB_bNotification_Comm_MDLM_45			0x45
#define	USB_bNotification_Comm_MDLM_46			0x46
#define	USB_bNotification_Comm_MDLM_47			0x47
#define	USB_bNotification_Comm_MDLM_48			0x48
#define	USB_bNotification_Comm_MDLM_49			0x48
#define	USB_bNotification_Comm_MDLM_4A			0x4A
#define	USB_bNotification_Comm_MDLM_4B			0x4B
#define	USB_bNotification_Comm_MDLM_4C			0x4C
#define	USB_bNotification_Comm_MDLM_4D			0x4D
#define	USB_bNotification_Comm_MDLM_4E			0x4E
#define	USB_bNotification_Comm_MDLM_4F			0x4F
#define	USB_bNotification_Comm_MDLM_50			0x50
#define	USB_bNotification_Comm_MDLM_51			0x51
#define	USB_bNotification_Comm_MDLM_52			0x52
#define	USB_bNotification_Comm_MDLM_53			0x53
#define	USB_bNotification_Comm_MDLM_54			0x54
#define	USB_bNotification_Comm_MDLM_55			0x55
#define	USB_bNotification_Comm_MDLM_56			0x56
#define	USB_bNotification_Comm_MDLM_57			0x57
#define	USB_bNotification_Comm_MDLM_58			0x58
#define	USB_bNotification_Comm_MDLM_59			0x59
#define	USB_bNotification_Comm_MDLM_5A			0x5A
#define	USB_bNotification_Comm_MDLM_5B			0x5B
#define	USB_bNotification_Comm_MDLM_5C			0x5C
#define	USB_bNotification_Comm_MDLM_5D			0x5D
#define	USB_bNotification_Comm_MDLM_5E			0x5E
#define	USB_bNotification_Comm_MDLM_5F			0x5F

/* a table initializer, for debugging, etc. */
#define	USB_bNotification_Comm__INIT \
	{ USB_bNotification_Comm_NETWORK_CONNECTION, "NETWORK_CONNECTION" }, \
	{ USB_bNotification_Comm_RESPONSE_AVAILABLE, "RESPONSE_AVAILABLE" }, \
	{ USB_bNotification_Comm_AUX_JACK_HOOK_STATE, "AUX_JACK_HOOK_STATE" }, \
	{ USB_bNotification_Comm_RING_DETECT, "RING_DETECT" }, \
	{ USB_bNotification_Comm_SERIAL_STATE, "SERIAL_STATE" }, \
	{ USB_bNotification_Comm_CALL_STATE_CHANGE, "CALL_STATE_CHANGE" }, \
	{ USB_bNotification_Comm_LINE_STATE_CHANGE, "LINE_STATE_CHANGE" }, \
	{ USB_bNotification_Comm_CONNECTION_SPEED_CHANGE, "CONNECTION_SPEED_CHANGE" }, \
	{ USB_bNotification_Comm_MDLM_40, "MDLM_40" }, \
	{ USB_bNotification_Comm_MDLM_41, "MDLM_41" }, \
	{ USB_bNotification_Comm_MDLM_42, "MDLM_42" }, \
	{ USB_bNotification_Comm_MDLM_43, "MDLM_43" }, \
	{ USB_bNotification_Comm_MDLM_44, "MDLM_44" }, \
	{ USB_bNotification_Comm_MDLM_45, "MDLM_45" }, \
	{ USB_bNotification_Comm_MDLM_46, "MDLM_46" }, \
	{ USB_bNotification_Comm_MDLM_47, "MDLM_47" }, \
	{ USB_bNotification_Comm_MDLM_48, "MDLM_48" }, \
	{ USB_bNotification_Comm_MDLM_49, "MDLM_49" }, \
	{ USB_bNotification_Comm_MDLM_4A, "MDLM_4A" }, \
	{ USB_bNotification_Comm_MDLM_4B, "MDLM_4B" }, \
	{ USB_bNotification_Comm_MDLM_4C, "MDLM_4C" }, \
	{ USB_bNotification_Comm_MDLM_4D, "MDLM_4D" }, \
	{ USB_bNotification_Comm_MDLM_4E, "MDLM_4E" }, \
	{ USB_bNotification_Comm_MDLM_4F, "MDLM_4F" }, \
	{ USB_bNotification_Comm_MDLM_50, "MDLM_50" }, \
	{ USB_bNotification_Comm_MDLM_51, "MDLM_51" }, \
	{ USB_bNotification_Comm_MDLM_52, "MDLM_52" }, \
	{ USB_bNotification_Comm_MDLM_53, "MDLM_53" }, \
	{ USB_bNotification_Comm_MDLM_54, "MDLM_54" }, \
	{ USB_bNotification_Comm_MDLM_55, "MDLM_55" }, \
	{ USB_bNotification_Comm_MDLM_56, "MDLM_56" }, \
	{ USB_bNotification_Comm_MDLM_57, "MDLM_57" }, \
	{ USB_bNotification_Comm_MDLM_58, "MDLM_58" }, \
	{ USB_bNotification_Comm_MDLM_59, "MDLM_59" }, \
	{ USB_bNotification_Comm_MDLM_5A, "MDLM_5A" }, \
	{ USB_bNotification_Comm_MDLM_5B, "MDLM_5B" }, \
	{ USB_bNotification_Comm_MDLM_5C, "MDLM_5C" }, \
	{ USB_bNotification_Comm_MDLM_5D, "MDLM_5D" }, \
	{ USB_bNotification_Comm_MDLM_5E, "MDLM_5E" }, \
	{ USB_bNotification_Comm_MDLM_5F, "MDLM_5F" }

/****************************************************************************\
|
|	Functional Descriptors
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_Generic
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFunctionSpecifcData[1];
	};

/* the introductory byte */
#define	USB_bDescriptorType_Comm_INTERFACE		0x24	/* Class-specific descriptor, INTERACE */
#define	USB_bDescriptorType_Comm_VENDOR_INTERFACE	0x44	/* Vendor-specific descriptor, INTERACE */
#define	USB_bDescriptorType_Data_INTERFACE		0x24	/* Class-specific descriptor, INTERACE */
#define	USB_bDescriptorType_Data_VENDOR_INTERFACE	0x44	/* Vendor-specific descriptor, INTERACE */

/* 
|| the comm-class descriptors.  Note that telephone operation mode was apparently
|| omitted from the 1.0 spec.
*/
#define USB_bDescriptorSubtype_Comm_HEADER	0x00	/* Header */
#define USB_bDescriptorSubtype_Comm_CALL_MANAGEMENT	0x01	/* Call management */
#define USB_bDescriptorSubtype_Comm_ACM		0x02	/* Abstract Control Model */
#define USB_bDescriptorSubtype_Comm_DLM		0x03	/* Direct Line Management */
#define USB_bDescriptorSubtype_Comm_RINGER	0x04	/* Telephone Ringer */
#define USB_bDescriptorSubtype_Comm_CALL_LINE_CAPS	0x05	/* Telehpone Call and Line Report Capabilities */
#define USB_bDescriptorSubtype_Comm_UNION	0x06	/* Union */
#define USB_bDescriptorSubtype_Comm_COUNTRY	0x07	/* Country Selection */
#define USB_bDescriptorSubtype_Comm_TOM		0x08	/* Telephone Operating Mode */
#define USB_bDescriptorSubtype_Comm_USBTERMINAL	0x09	/* USB Terminal */
#define USB_bDescriptorSubtype_Comm_NETTERMINAL	0x0A	/* Network Channel terminal */
#define USB_bDescriptorSubtype_Comm_PROTOUNIT	0x0B	/* Protocol Unit */
#define USB_bDescriptorSubtype_Comm_EXTUNIT	0x0C	/* Extension Unit */
#define USB_bDescriptorSubtype_Comm_MCM		0x0D	/* Multi-Channel Management */
#define USB_bDescriptorSubtype_Comm_CAPI	0x0E	/* CAPI */
#define USB_bDescriptorSubtype_Comm_ETHERNET	0x0F	/* Ethernet Networking */
#define USB_bDescriptorSubtype_Comm_ATM		0x10	/* ATM Networking */
#define USB_bDescriptorSubtype_Comm_WHCM	0x11	/* WHCM */
#define USB_bDescriptorSubtype_Comm_MDLM	0x12	/* MDLM */
#define USB_bDescriptorSubtype_Comm_MDLMDETAIL	0x13	/* MDLM Detail */
#define USB_bDescriptorSubtype_Comm_DEVMGMT	0x14	/* Device Management */
#define USB_bDescriptorSubtype_Comm_OBEX	0x15	/* OBEX */
#define USB_bDescriptorSubtype_Comm_CMDSET	0x16	/* Command Set */
#define USB_bDescriptorSubtype_Comm_CMDSETDETAIL 0x17	/* Command Set Detail */
#define USB_bDescriptorSubtype_Comm_TCM		0x18	/* Telehpone Control Model */

#define	USB_bDescriptorSubtype_Comm__INIT \
	{ USB_bDescriptorSubtype_Comm_HEADER, "Header" }, \
	{ USB_bDescriptorSubtype_Comm_CALL_MANAGEMENT, "Call management" }, \
	{ USB_bDescriptorSubtype_Comm_ACM, "Abstract Control Model" }, \
	{ USB_bDescriptorSubtype_Comm_DLM, "Direct Line Management" }, \
	{ USB_bDescriptorSubtype_Comm_RINGER, "Telephone Ringer" }, \
	{ USB_bDescriptorSubtype_Comm_CALL_LINE_CAPS, "Telehpone Call and Line Report Capabilities" }, \
	{ USB_bDescriptorSubtype_Comm_UNION, "Union" }, \
	{ USB_bDescriptorSubtype_Comm_COUNTRY, "Country Selection" }, \
	{ USB_bDescriptorSubtype_Comm_TOM, "Telephone Operating Mode" }, \
	{ USB_bDescriptorSubtype_Comm_USBTERMINAL, "USB Terminal" }, \
	{ USB_bDescriptorSubtype_Comm_NETTERMINAL, "Network Channel terminal" }, \
	{ USB_bDescriptorSubtype_Comm_PROTOUNIT, "Protocol Unit" }, \
	{ USB_bDescriptorSubtype_Comm_EXTUNIT, "Extension Unit" }, \
	{ USB_bDescriptorSubtype_Comm_MCM, "Multi-Channel Management" }, \
	{ USB_bDescriptorSubtype_Comm_CAPI, "CAPI" }, \
	{ USB_bDescriptorSubtype_Comm_ETHERNET, "Ethernet Networking" }, \
	{ USB_bDescriptorSubtype_Comm_ATM, "ATM Networking" }, \
	{ USB_bDescriptorSubtype_Comm_WHCM, "WHCM" }, \
	{ USB_bDescriptorSubtype_Comm_MDLM, "MDLM" }, \
	{ USB_bDescriptorSubtype_Comm_MDLMDETAIL, "MDLM Detail" }, \
	{ USB_bDescriptorSubtype_Comm_DEVMGMT, "Device Management" }, \
	{ USB_bDescriptorSubtype_Comm_OBEX, "OBEX" }, \
	{ USB_bDescriptorSubtype_Comm_CMDSET, "Command Set" }, \
	{ USB_bDescriptorSubtype_Comm_CMDSETDETAIL, "Command Set Detail" }, \
	{ USB_bDescriptorSubtype_Comm_TCM, "Telehpone Control Model" }


/* class-specific descriptors for DATA class */
#define USB_bDescriptorSubtype_Data_HEADER	0x00	/* Header */

#define	USB_bDescriptorSubtype_Data__INIT \
	{ USB_bDescriptorSubtype_Data_HEADER, "Header" }

/****************************************************************************\
|
|	The header functional descriptor (5.2.3.1)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_HEADER
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bcdCDC[2];		/* the version of the spec.  1.1 == 0x0110 */
	};

/****************************************************************************\
|
|	The Call Managment functional descriptor (5.2.3.2)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_CALL_MANAGEMENT
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bmCapabilities[1];
	unsigned char	bDataInterface;
	};

#define	USB_Comm_CALL_MANAGEMENT_bmCapabilities_AVAIL	0x01	/* call management is availalbe */
#define	USB_Comm_CALL_MANAGEMENT_bmCapabilities_MUX	0x02	/* can be muxed over data class i/f */

#define USB_Comm_CALL_MANAGEMENT_bmCapabilities__INIT \
	{ USB_Comm_CALL_MANAGEMENT_bmCapabilities_AVAIL, "AVAIL" }, \
	{ USB_Comm_CALL_MANAGEMENT_bmCapabilities_MUX, "MUX" }

/****************************************************************************\
|
|	The Abstract Control Model functional descriptor (5.2.3.3)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_ACM
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bmCapabilities;
	};

#define	USB_Comm_ACM_bmCapabilities_COMMSGC	0x01	/* device supports set/get/clear comm feature */
#define	USB_Comm_ACM_bmCapabilities_LINE_CSGN	0x02	/* device supports line coding/state features */
#define	USB_Comm_ACM_bmCapabilities_BREAK	0x04	/* device supports SEND_BREAK */
#define	USB_Comm_ACM_bmCapabilities_NETCONN	0x08	/* device supports NETWORK_CONNECTION notification */

#define USB_Comm_ACM_bmCapabilities__INIT \
	{ USB_Comm_ACM_bmCapabilities_COMMSGC,		"COMMSGC" }, \
	{ USB_Comm_ACM_bmCapabilities_LINE_CSGN,	"LINE_CSGN" }, \
	{ USB_Comm_ACM_bmCapabilities_BREAK,		"BREAK" }, \
	{ USB_Comm_ACM_bmCapabilities_NETCONN,		"NETCONN" }

/****************************************************************************\
|
|	The Direct Line Model functional descriptor (5.2.3.4)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_DLM
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bmCapabilities;
	};

#define	USB_Comm_DLM_bmCapabilities_PULSE	0x01	/* supports PULSE SETUP/SEND PULSE/SET PULSE */
#define	USB_Comm_DLM_bmCapabilities_AUX		0x02	/* supports AUX command set */
#define	USB_Comm_DLM_bmCapabilities_HOLD	0x04	/* device needs extra pulses for hold (section 6.2.8) */

#define USB_Comm_DLM_bmCapabilities__INIT \
	{ USB_Comm_DLM_bmCapabilities_PULSE,	"PULSE" }, \
	{ USB_Comm_DLM_bmCapabilities_AUX,	"AUX" }, \
	{ USB_Comm_DLM_bmCapabilities_HOLD,	"HOLD" }

/****************************************************************************\
|
|	Telephone Ringer Functional Descriptor (5.2.3.5)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_RINGER
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bRingerVolSteps;
	unsigned char	bNumRingerPatterns;
	};

/****************************************************************************\
|
|	Telephone Operating Modes Functional Descriptor (5.2.3.6)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_TOM
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bmCapabilities;
	};

#define	USB_Comm_TOM_bmCapabilities_SIMPLE	0x01	/* supports simple mode */
#define	USB_Comm_TOM_bmCapabilities_STANDALONE	0x02	/* supports standalone mode */
#define	USB_Comm_TOM_bmCapabilities_COMPUTER	0x04	/* supports computer mode */

#define USB_Comm_TOM_bmCapabilities__INIT \
	{ USB_Comm_TOM_bmCapabilities_SIMPLE,		"SIMPLE" }, \
	{ USB_Comm_TOM_bmCapabilities_STANDALONE,	"STANDALONE" }, \
	{ USB_Comm_TOM_bmCapabilities_PULSE,		"PULSE" }

/****************************************************************************\
|
|	Telephone Call and Line State Reporting Capabilities Descriptor
|	(5.2.3.7)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_CALL_LINE_CAPS
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bmCapabilities;
	};

#define	USB_Comm_CALL_LINE_CAPS_bmCapabilities_IDIAL	0x01	/* reports interrupted dialtone */
#define	USB_Comm_CALL_LINE_CAPS_bmCapabilities_BUSY	0x02	/* reports ringback/busy/fast busy */
#define	USB_Comm_CALL_LINE_CAPS_bmCapabilities_CALLID	0x04	/* reports caller ID */
#define	USB_Comm_CALL_LINE_CAPS_bmCapabilities_DRING	0x08	/* reports distinctive ring */
#define	USB_Comm_CALL_LINE_CAPS_bmCapabilities_DTMF	0x10	/* reports DTMF input */
#define	USB_Comm_CALL_LINE_CAPS_bmCapabilities_LSTNOTE	0x20	/* supports line state change notification */

#define USB_Comm_CALL_LINE_CAPS_bmCapabilities__INIT \
	{ USB_Comm_CALL_LINE_CAPS_bmCapabilities_IDIAL,		"IDIAL" }, \
	{ USB_Comm_CALL_LINE_CAPS_bmCapabilities_BUSY,		"BUSY" }, \
	{ USB_Comm_CALL_LINE_CAPS_bmCapabilities_CALLID,	"CALLID" }, \
	{ USB_Comm_CALL_LINE_CAPS_bmCapabilities_DRING,		"DRING" }, \
	{ USB_Comm_CALL_LINE_CAPS_bmCapabilities_DTMF,		"DTMF" }, \
	{ USB_Comm_CALL_LINE_CAPS_bmCapabilities_LSTNOTE,	"LSTNOTE" }

/****************************************************************************\
|
|	UNION functional descriptor (5.2.3.8)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_UNION
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bMasterInterface;
	unsigned char	bSlaveInterface[1];
	};

/****************************************************************************\
|
|	Country Selection Functional Descriptor (5.2.3.9)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_COUNTRY
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	iCountryCodeRelDate;	/* string index: release date of ISO 3166 spec: ddmmyyyy */
	unsigned char	wCountryCode[2];	/* ARRAY of country codes as words, from ISO 3166 */
	};

#define	USB_Comm_CLC_bmCapabilities_IDIAL	0x01	/* reports interrupted dialtone */
#define	USB_Comm_CLC_bmCapabilities_BUSY	0x02	/* reports ringback/busy/fast busy */
#define	USB_Comm_CLC_bmCapabilities_CALLID	0x04	/* reports caller ID */
#define	USB_Comm_CLC_bmCapabilities_DRING	0x08	/* reports distinctive ring */
#define	USB_Comm_CLC_bmCapabilities_DTMF	0x10	/* reports DTMF input */
#define	USB_Comm_CLC_bmCapabilities_LSTNOTE	0x20	/* supports line state change notification */

/****************************************************************************\
|
|	USB TERMINAL Functional Descriptor (5.2.3.10)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_USBTERMINAL
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bEntryId;		/* constant uniquely identifying the terminal */
	unsigned char	bInInterfaceNo;		/* input interface number */
	unsigned char	bOutInterfaceNo;	/* output interface number */
	unsigned char	bmOptions;		/* options bitmap */
	unsigned char	bChildId0[1];		/* array of IDs of lower terminals or units */
	};

#define	USB_Comm_USBTERMINAL_bmOptions_WRAPPER	0x01	/* protocol wrapper is used */

#define USB_Comm_USBTERMINAL_bmOptions__INIT \
	{ USB_Comm_USBTERMINAL_bmOptions_WRAPPER,	"WRAPPER" }

/****************************************************************************\
|
|	Network Channel Terminal Functional Descriptor (5.2.3.11)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_NETTERMINAL
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bEntityId;
	unsigned char	iName;
	unsigned char	bChannelIndex;
	unsigned char	bPhysicalInterface;
	};

/* Network Channel Terminal type */
#define	USB_Comm_NETTERMINAL_bPhysicalInterace_NONE 0	/* no physical interface */
#define	USB_Comm_NETTERMINAL_bPhysicalInterace_ISDN 1	/* physical interface is ISDN */
#define	USB_Comm_NETTERMINAL_bPhysicalInterace_VS_MIN 201	/* physical interface is Vendor Specific */
#define	USB_Comm_NETTERMINAL_bPhysicalInterace_VS_MAX 255	/* physical interface is Vendor Specific */

#define USB_Comm_NETTERMINAL_bPhysicalInterace___INIT \
	{ USB_Comm_NETTERMINAL_bPhysicalInterace_NONE,		"NONE" }, \
	{ USB_Comm_NETTERMINAL_bPhysicalInterace_ISDN,		"ISDN" }, \
	{ USB_Comm_NETTERMINAL_bPhysicalInterace_VS_MIN,	"VS_MIN" }, \
	{ USB_Comm_NETTERMINAL_bPhysicalInterace_VS_MAX,	"VS_MAX" }

/****************************************************************************\
|
|	Protocol Unit Functional Descriptor (5.2.3.12)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_PROTOUNIT
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bEntityId;		/* constant uniquely identifying the unit */
	unsigned char	bProtocol;		/* the protocol code: USB_bInterfaceProtocol_Data... */
	unsigned char	bChildId[1];		/* array of IDs of lower terminals/units */
	};

/****************************************************************************\
|
|	Extension Unit Functional Descriptor (5.2.3.13)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_EXTUNIT
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bEntityId;		/* constant uniquely identifying the unit */
	unsigned char	bExtensionCode;		/* vendor-specific code identifying the Extension Unit */
	unsigned char	iName;			/* index of string descriptor naming the EXT unit */
	unsigned char	bChildId[1];		/* array of IDs of lower terminals/units */
	};

/****************************************************************************\
|
|	Multi-Channel Model function descriptor (5.2.3.14)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_MCM
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bmCapabilities;
	};

#define	USB_Comm_MCM_bmCapabilities_NV		0x01	/* paramters are stored in non-volatile memory */
#define	USB_Comm_MCM_bmCapabilities_CLEAR	0x02	/* supports CLEAR_UNIT_PARAMETER */
#define	USB_Comm_MCM_bmCapabilities_SET		0x04	/* supports SET_UNIT_PARAMETER */

#define USB_Comm_MCM_bmCapabilities__INIT \
	{ USB_Comm_MCM_bmCapabilities_NV,	"NV" }, \
	{ USB_Comm_MCM_bmCapabilities_CLEAR,	"CLEAR" }, \
	{ USB_Comm_MCM_bmCapabilities_SET,	"SET" }

/****************************************************************************\
|
|	CAPI control model function descriptor fields (5.2.3.15)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_CAPI
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bmCapabilities;
	};

#define	USB_Comm_CAPI_bmCapabilities_INTELLIGENT 0x01	/* device is an intelligent CAPI */

#define USB_Comm_CAPI_bmCapabilities__INIT \
	{ USB_Comm_CAPI_bmCapabilities_INTELLIGENT,	"INTELLIGENT" }

/****************************************************************************\
|
|	ETHERNET control model functional descriptor bit-values (5.2.3.16)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_ETHERNET
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	iMACAddress;
	unsigned char	bmEthernetStatistics[4];	/* see capabilities
							|| bits below.
							*/
	unsigned char	wMaxSegmentSize[2];
	unsigned char	wNumberMCFilters[2];		/* bitmask: 
							|| see below 
							*/
	unsigned char	bNumberPowerFilters;
	};

/*
|| Define the selector codes for the various Statistics -- these correspond
|| to the capability bits.
*/
#define	USB_Comm_ETHERNET_STAT_XMIT_OK			1
#define	USB_Comm_ETHERNET_STAT_RCV_OK			2
#define	USB_Comm_ETHERNET_STAT_XMIT_ERROR		3
#define	USB_Comm_ETHERNET_STAT_RCV_ERROR		4
#define	USB_Comm_ETHERNET_STAT_RCV_NO_BUFFER		5
#define	USB_Comm_ETHERNET_STAT_DIRECTED_BYTES_XMIT	6
#define	USB_Comm_ETHERNET_STAT_DIRECTED_FRAMES_XMIT	7
#define	USB_Comm_ETHERNET_STAT_MULTICAST_BYTES_XMIT	8
#define	USB_Comm_ETHERNET_STAT_MULTICAST_FRAMES_XMIT	9
#define	USB_Comm_ETHERNET_STAT_BROADCAST_BYTES_XMIT	10
#define	USB_Comm_ETHERNET_STAT_BROADCAST_FRAMES_XMIT	11
#define	USB_Comm_ETHERNET_STAT_DIRECTED_BYTES_RCV	12
#define	USB_Comm_ETHERNET_STAT_DIRECTED_FRAMES_RCV	13
#define	USB_Comm_ETHERNET_STAT_MULTICAST_BYTES_RCV	14
#define	USB_Comm_ETHERNET_STAT_MULTICAST_FRAMES_RCV	15
#define	USB_Comm_ETHERNET_STAT_BROADCAST_BYTES_RCV	16
#define	USB_Comm_ETHERNET_STAT_BROADCAST_FRAMES_RCV	17
#define	USB_Comm_ETHERNET_STAT_RCV_CRC_ERROR		18
#define	USB_Comm_ETHERNET_STAT_TRANSMIT_QUEUE_LENGTH	19
#define	USB_Comm_ETHERNET_STAT_RCV_ERROR_ALIGNMENT	20
#define	USB_Comm_ETHERNET_STAT_XMIT_ONE_COLLISION	21
#define	USB_Comm_ETHERNET_STAT_XMIT_MORE_COLLISIONS	22
#define	USB_Comm_ETHERNET_STAT_XMIT_DEFERRED		23
#define	USB_Comm_ETHERNET_STAT_XMIT_MAX_COLLISIONS	24
#define	USB_Comm_ETHERNET_STAT_RCV_OVERRUN		25
#define	USB_Comm_ETHERNET_STAT_XMIT_UNDERRUN		26
#define	USB_Comm_ETHERNET_STAT_XMIT_HEARTBEAT_FAILURE	27
#define	USB_Comm_ETHERNET_STAT_XMIT_TIME_CRS_LOST	28
#define	USB_Comm_ETHERNET_STAT_XMIT_LATE_COLLISION	29

/* a table initializizer, for debugging */
#define USB_Comm_ETHERNET_STAT__INIT \
	{ USB_Comm_ETHERNET_STAT_XMIT_OK, "XMIT_OK" }, \
	{ USB_Comm_ETHERNET_STAT_RCV_OK, "RCV_OK" }, \
	{ USB_Comm_ETHERNET_STAT_XMIT_ERROR, "XMIT_ERROR" }, \
	{ USB_Comm_ETHERNET_STAT_RCV_ERROR, "RCV_ERROR" }, \
	{ USB_Comm_ETHERNET_STAT_RCV_NO_BUFFER, "RCV_NO_BUFFER" }, \
	{ USB_Comm_ETHERNET_STAT_DIRECTED_BYTES_XMIT, "DIRECTED_BYTES_XMIT" }, \
	{ USB_Comm_ETHERNET_STAT_DIRECTED_FRAMES_XMIT, "DIRECTED_FRAMES_XMIT" }, \
	{ USB_Comm_ETHERNET_STAT_MULTICAST_BYTES_XMIT, "MULTICAST_BYTES_XMIT" }, \
	{ USB_Comm_ETHERNET_STAT_MULTICAST_FRAMES_XMIT, "MULTICAST_FRAMES_XMIT" }, \
	{ USB_Comm_ETHERNET_STAT_BROADCAST_BYTES_XMIT, "BROADCAST_BYTES_XMIT" }, \
	{ USB_Comm_ETHERNET_STAT_BROADCAST_FRAMES_XMIT, "BROADCAST_FRAMES_XMIT" }, \
	{ USB_Comm_ETHERNET_STAT_DIRECTED_BYTES_RCV, "DIRECTED_BYTES_RCV" }, \
	{ USB_Comm_ETHERNET_STAT_DIRECTED_FRAMES_RCV, "DIRECTED_FRAMES_RCV" }, \
	{ USB_Comm_ETHERNET_STAT_MULTICAST_BYTES_RCV, "MULTICAST_BYTES_RCV" }, \
	{ USB_Comm_ETHERNET_STAT_MULTICAST_FRAMES_RCV, "MULTICAST_FRAMES_RCV" }, \
	{ USB_Comm_ETHERNET_STAT_BROADCAST_BYTES_RCV, "BROADCAST_BYTES_RCV" }, \
	{ USB_Comm_ETHERNET_STAT_BROADCAST_FRAMES_RCV, "BROADCAST_FRAMES_RCV" }, \
	{ USB_Comm_ETHERNET_STAT_RCV_CRC_ERROR, "RCV_CRC_ERROR" }, \
	{ USB_Comm_ETHERNET_STAT_TRANSMIT_QUEUE_LENGTH, "TRANSMIT_QUEUE_LENGTH" }, \
	{ USB_Comm_ETHERNET_STAT_RCV_ERROR_ALIGNMENT, "RCV_ERROR_ALIGNMENT" }, \
	{ USB_Comm_ETHERNET_STAT_XMIT_ONE_COLLISION, "XMIT_ONE_COLLISION" }, \
	{ USB_Comm_ETHERNET_STAT_XMIT_MORE_COLLISIONS, "XMIT_MORE_COLLISIONS" }, \
	{ USB_Comm_ETHERNET_STAT_XMIT_DEFERRED, "XMIT_DEFERRED" }, \
	{ USB_Comm_ETHERNET_STAT_XMIT_MAX_COLLISIONS, "XMIT_MAX_COLLISIONS" }, \
	{ USB_Comm_ETHERNET_STAT_RCV_OVERRUN, "RCV_OVERRUN" }, \
	{ USB_Comm_ETHERNET_STAT_XMIT_UNDERRUN, "XMIT_UNDERRUN" }, \
	{ USB_Comm_ETHERNET_STAT_XMIT_HEARTBEAT_FAILURE, "XMIT_HEARTBEAT_FAILURE" }, \
	{ USB_Comm_ETHERNET_STAT_XMIT_TIME_CRS_LOST, "XMIT_TIME_CRS_LOST" }, \
	{ USB_Comm_ETHERNET_STAT_XMIT_LATE_COLLISION, "XMIT_LATE_COLLISION" }

/*

Macro:	USB_Comm_ETHERNET_STAT_to_MASK()

Function:
	Convert an Ethernet Statistics index to the corresponding
	Ethernet functional descriptor statistic capability
	mask pattern.

Synopsis:
	unsigned long 
	USB_Comm_ETHERNET_STAT_to_MASK(
		(generic) unsigned uStatisticIndex
		);

Description:
	This macro converts the statistic index to the corresponding
	unsigned long mask.  It's a true macro, so if the
	input argument is a constant, the result will also be
	a compile-time constant.

Returns:
	unsigned long bitmask (unless the input parameter is something
	like a unsigned quad.

*/

#define	USB_Comm_ETHERNET_STAT_to_MASK(s)	(1ul << ((s) - 1))

/* capability mask bits */
#define USB_Comm_ETHERNET_bmCapabilities_XMIT_OK		USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_XMIT_OK)
#define USB_Comm_ETHERNET_bmCapabilities_RCV_OK			USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_RCV_OK)
#define USB_Comm_ETHERNET_bmCapabilities_XMIT_ERROR		USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_XMIT_ERROR)
#define USB_Comm_ETHERNET_bmCapabilities_RCV_ERROR		USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_RCV_ERROR)
#define USB_Comm_ETHERNET_bmCapabilities_RCV_NO_BUFFER		USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_RCV_NO_BUFFER)
#define USB_Comm_ETHERNET_bmCapabilities_DIRECTED_BYTES_XMIT	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_DIRECTED_BYTES_XMIT)
#define USB_Comm_ETHERNET_bmCapabilities_DIRECTED_FRAMES_XMIT	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_DIRECTED_FRAMES_XMIT)
#define USB_Comm_ETHERNET_bmCapabilities_MULTICAST_BYTES_XMIT	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_MULTICAST_BYTES_XMIT)
#define USB_Comm_ETHERNET_bmCapabilities_MULTICAST_FRAMES_XMIT	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_MULTICAST_FRAMES_XMIT)
#define USB_Comm_ETHERNET_bmCapabilities_BROADCAST_BYTES_XMIT	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_BROADCAST_BYTES_XMIT)
#define USB_Comm_ETHERNET_bmCapabilities_BROADCAST_FRAMES_XMIT	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_BROADCAST_FRAMES_XMIT)
#define USB_Comm_ETHERNET_bmCapabilities_DIRECTED_BYTES_RCV	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_DIRECTED_BYTES_RCV)
#define USB_Comm_ETHERNET_bmCapabilities_DIRECTED_FRAMES_RCV	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_DIRECTED_FRAMES_RCV)
#define USB_Comm_ETHERNET_bmCapabilities_MULTICAST_BYTES_RCV	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_MULTICAST_BYTES_RCV)
#define USB_Comm_ETHERNET_bmCapabilities_MULTICAST_FRAMES_RCV	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_MULTICAST_FRAMES_RCV)
#define USB_Comm_ETHERNET_bmCapabilities_BROADCAST_BYTES_RCV	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_BROADCAST_BYTES_RCV)
#define USB_Comm_ETHERNET_bmCapabilities_BROADCAST_FRAMES_RCV	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_BROADCAST_FRAMES_RCV)
#define USB_Comm_ETHERNET_bmCapabilities_RCV_CRC_ERROR		USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_RCV_CRC_ERROR)
#define USB_Comm_ETHERNET_bmCapabilities_TRANSMIT_QUEUE_LENGTH	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_TRANSMIT_QUEUE_LENGTH)
#define USB_Comm_ETHERNET_bmCapabilities_RCV_ERROR_ALIGNMENT	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_RCV_ERROR_ALIGNMENT)
#define USB_Comm_ETHERNET_bmCapabilities_XMIT_ONE_COLLISION	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_XMIT_ONE_COLLISION)
#define USB_Comm_ETHERNET_bmCapabilities_XMIT_MORE_COLLISIONS	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_XMIT_MORE_COLLISIONS)
#define USB_Comm_ETHERNET_bmCapabilities_XMIT_DEFERRED		USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_XMIT_DEFERRED)
#define USB_Comm_ETHERNET_bmCapabilities_XMIT_MAX_COLLISIONS	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_XMIT_MAX_COLLISIONS)
#define USB_Comm_ETHERNET_bmCapabilities_RCV_OVERRUN		USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_RCV_OVERRUN)
#define USB_Comm_ETHERNET_bmCapabilities_XMIT_UNDERRUN		USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_XMIT_UNDERRUN)
#define USB_Comm_ETHERNET_bmCapabilities_XMIT_HEARTBEAT_FAILURE	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_XMIT_HEARTBEAT_FAILURE)
#define USB_Comm_ETHERNET_bmCapabilities_XMIT_TIME_CRS_LOST	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_XMIT_TIME_CRS_LOST)
#define USB_Comm_ETHERNET_bmCapabilities_XMIT_LATE_COLLISION	USB_Comm_ETHERNET_STAT_to_MASK(USB_Comm_ETHERNET_STAT_XMIT_LATE_COLLISION)

/* a table initializizer, for debugging */
#define USB_Comm_ETHERNET_bmCapabilities__INIT \
	{ USB_Comm_ETHERNET_bmCapabilities_XMIT_OK, "XMIT_OK" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_RCV_OK, "RCV_OK" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_XMIT_ERROR, "XMIT_ERROR" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_RCV_ERROR, "RCV_ERROR" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_RCV_NO_BUFFER, "RCV_NO_BUFFER" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_DIRECTED_BYTES_XMIT, "DIRECTED_BYTES_XMIT" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_DIRECTED_FRAMES_XMIT, "DIRECTED_FRAMES_XMIT" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_MULTICAST_BYTES_XMIT, "MULTICAST_BYTES_XMIT" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_MULTICAST_FRAMES_XMIT, "MULTICAST_FRAMES_XMIT" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_BROADCAST_BYTES_XMIT, "BROADCAST_BYTES_XMIT" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_BROADCAST_FRAMES_XMIT, "BROADCAST_FRAMES_XMIT" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_DIRECTED_BYTES_RCV, "DIRECTED_BYTES_RCV" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_DIRECTED_FRAMES_RCV, "DIRECTED_FRAMES_RCV" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_MULTICAST_BYTES_RCV, "MULTICAST_BYTES_RCV" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_MULTICAST_FRAMES_RCV, "MULTICAST_FRAMES_RCV" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_BROADCAST_BYTES_RCV, "BROADCAST_BYTES_RCV" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_BROADCAST_FRAMES_RCV, "BROADCAST_FRAMES_RCV" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_RCV_CRC_ERROR, "RCV_CRC_ERROR" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_TRANSMIT_QUEUE_LENGTH, "TRANSMIT_QUEUE_LENGTH" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_RCV_ERROR_ALIGNMENT, "RCV_ERROR_ALIGNMENT" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_XMIT_ONE_COLLISION, "XMIT_ONE_COLLISION" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_XMIT_MORE_COLLISIONS, "XMIT_MORE_COLLISIONS" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_XMIT_DEFERRED, "XMIT_DEFERRED" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_XMIT_MAX_COLLISIONS, "XMIT_MAX_COLLISIONS" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_RCV_OVERRUN, "RCV_OVERRUN" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_XMIT_UNDERRUN, "XMIT_UNDERRUN" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_XMIT_HEARTBEAT_FAILURE, "XMIT_HEARTBEAT_FAILURE" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_XMIT_TIME_CRS_LOST, "XMIT_TIME_CRS_LOST" }, \
	{ USB_Comm_ETHERNET_bmCapabilities_XMIT_LATE_COLLISION, "XMIT_LATE_COLLISION" }

/* the mask for wNumberMCFilters, after conversion to local endianness */
#define	USB_Comm_ETHERNET_wNumberMCFilters_EXACT		(1u << 15)
#define	USB_Comm_ETHERNET_wNumberMCFilters_SUPPORTED_MASK	(0x7FFF)

/****************************************************************************\
|
|	ATM functional descriptor bits (5.2.3.17)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_ATM
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	iEndSystemIdentifier;
	unsigned char	bmDataCapabilities;
	unsigned char	bmATMDeviceStatistics;
	unsigned char	wType2MaxSegmentSize[2];
	unsigned char	wType3MaxSegmentSize[2];
	};

/**** ATM ****/
#define	USB_Comm_ATM_bmDataCapabilities_TYPE1	0x02	/* supports Type1 concatenated cells */
#define	USB_Comm_ATM_bmDataCapabilities_TYPE2	0x04	/* supports Type2 concatenated payloads */
#define	USB_Comm_ATM_bmDataCapabilities_TYPE3	0x08	/* supports Type3 AAL5 SDUs */

#define USB_Comm_ATM_bmDataCapabilities__INIT \
	{ USB_Comm_ATM_bmDataCapabilities_TYPE1, "TYPE1" }, \
	{ USB_Comm_ATM_bmDataCapabilities_TYPE2, "TYPE2" }, \
	{ USB_Comm_ATM_bmDataCapabilities_TYPE3, "TYPE3" }

#define	USB_COMM_ATM_bmATMDeviceStatistics_VC_US_CELLS_SENT		0x01
#define	USB_COMM_ATM_bmATMDeviceStatistics_VC_DS_CELLS_RECEIVED		0x02
#define	USB_COMM_ATM_bmATMDeviceStatistics_DS_CELLS_HEC_ERROR_CORRECTED	0x04
#define	USB_COMM_ATM_bmATMDeviceStatistics_US_CELLS_SENT		0x08

#define USB_Comm_ATM_bmATMDeviceStatistics__INIT \
	{ USB_Comm_ATM_bmATMDeviceStatistics_VC_US_CELLS_SENT,			"VC_US_CELLS_SENT" }, \
	{ USB_Comm_ATM_bmATMDeviceStatistics_VC_DS_CELLS_SENT,			"VC_DS_CELLS_SENT" }, \
	{ USB_Comm_ATM_bmATMDeviceStatistics_DS_CELLS_HEC_ERROR_CORRECTED,	"DS_CELLS_HEC_ERROR_CORRECTED" }, \
	{ USB_Comm_ATM_bmATMDeviceStatistics_US_CELLS_SENT,			"US_CELLS_SENT" }

/****************************************************************************\
|
|	WHCM Functional Descriptor ([cdcwmc10] 6.1.2.3)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_WHCM
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bcdWmcVersion[2];	/* the version of the WMC spec.  1.1 == 0x0110 */
	};

/****************************************************************************\
|
|	MDLM Functional Descriptor ([cdcwmc10] 6.7.2.3)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_MDLM
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bcdVersion[2];		/* the version of the WMC
						|| spec.  1.1 == 0x0110 
						*/
	unsigned char	bGUID[16];		/* the GUID that identifies
						|| the MDLM transport spec 
						*/
	};

/****************************************************************************\
|
|	MDLM Detail Descriptor ([cdcwmc10] 6.7.2.4)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_MDLMDETAIL
	{
	unsigned char	bFunctionLength;	/* varies */
	unsigned char	bDescriptorType;	/* CS_INTERFACE */
	unsigned char	bDescriptorSubtype;	/* USB_bDescriptorSubtype_Comm_MDLMDETAIL */
	unsigned char	bGuidDescriptorType;	/* the discriminator */
	unsigned char	bDetailData[1];		/* the payload */
	};

/****************************************************************************\
|
|	Device Management Functional Descriptor ([cdcwmc10] 6.6.2.3)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_DEVMGMT
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;	/* CS_INTERFACE */
	unsigned char	bDescriptorSubtype;	/* USB_bDescriptorSubtype_Comm_DEVMGMT */
	unsigned char	bcdVersion[2];		/* the version of the spec.  1.1 == 0x0110 */
	unsigned char	wWaxCommand[2];		/* The buffer size allocated in the device 
						|| for data sent from the host using
						|| SEND_ENCAPSULATED_COMMAND. If the
						|| host attempts to send more than this 
						|| in a single command, the device shall
						|| fail the request. This shall be at 
						|| least 256 decimal (0x100).
						*/
	};

#define USB_Comm_DEVMGMT_wMaxCommmand_MIN	0x100


/****************************************************************************\
|
|	OBEX Functional Descriptor ([cdcwmc10] 6.5.2.3)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_OBEX
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bcdVersion[2];	/* the version of the WMC spec.  1.1 == 0x0110 */
	};

/****************************************************************************\
|
|	Command Set Functional Descriptor ([cdcwmc10] 8.1.2.2)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_CMDSET
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bcdVersion[2];		/* the version of the WMC spec.  1.1 == 0x0110 */
	unsigned char	iCommandSet;
	unsigned char	bGUID[16];		/* the GUID that identifies
						|| the command set spec 
						*/
	};

/****************************************************************************\
|
|	Command Set Detail Functional Descriptor ([cdcwmc10] 8.1.2.3)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_CMDSETDETAIL
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;	/* USB_bDescriptorSubtype_Comm_CMDSETDETAIL */
	unsigned char	bGuidDescriptorType;	/* the discriminator */
	unsigned char	bDetailData[1];		/* the payload */
	};

/****************************************************************************\
|
|	Telephone Control Model Functional Descriptor ([cdcwmc10] 6.3.2.3)
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_TCM
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;	/* USB_bDescriptorSubtype_Comm_TCM */
	unsigned char	bcdVersion[2];		/* the version of the spec.  1.1 == 0x0110 */
	unsigned char	wWaxCommand[2];		/* The buffer size allocated in the device 
						|| for data sent from the host using
						|| SEND_ENCAPSULATED_COMMAND. If the
						|| host attempts to send more than this 
						|| in a single command, the device shall
						|| fail the request. This shall be at 
						|| least 256 decimal (0x100).
						*/
	};


/****************************************************************************\
|
|	Bits in various SETUP commands
|
\****************************************************************************/

/*
|| SET_COMM_FEATURE, CLEAR_COMM_FEATURE, GET_COMM_FEATURE.
*/
#define	USB_Comm_COMM_FEATURE_wValue_ABSTRACT_STATE		1
#define	USB_Comm_COMM_FEATURE_wValue_COUNTRY_SETTING		2

#define	USB_Comm_COMM_FEATURE_wValue__INIT \
	{ USB_Comm_COMM_FEATURE_wValue_ABSTRACT_STATE,	"ABSTRACT_STATE" }, \
	{ USB_Comm_COMM_FEATURE_wValue_COUNTRY_SETTING, "COUNTRY_SETTING" }

/* bits in abstract-state data payload (table 48) */
#define	USB_Comm_COMM_FEATURE_ABSTRACT_STATE_Idle		0x1
#define	USB_Comm_COMM_FEATURE_ABSTRACT_STATE_CMMultiplexed	0x2

#define	USB_Comm_COMM_FEATURE_ABSTRACT_STATE__INIT \
	{ USB_Comm_COMM_FEATURE_ABSTRACT_STATE_Idle,		"Idle" }, \
	{ USB_Comm_COMM_FEATURE_ABSTRACT_STATE_CMMultiplexed,	"CMMultiplexed" }


/*
|| SET_AUX_LINE_STATE (6.2.6)
*/
#define	USB_Comm_SET_AUX_LINE_STATE_wValue_DISCONNECT	0
#define	USB_Comm_SET_AUX_LINE_STATE_wValue_CONNECT	1

#define	USB_Comm_SET_AUX_LINE_STATE_wValue__INIT \
	{ USB_Comm_SET_AUX_LINE_STATE_wValue_DISCONNECT,	"DISCONNECT" }, \
	{ USB_Comm_SET_AUX_LINE_STATE_wValue_CONNECT,		"CONNECT" }

/*
|| SET_HOOK_STATE (6.2.7)
*/
#define	USB_Comm_SET_HOOK_STATE_wValue_ON_HOOK	0
#define	USB_Comm_SET_HOOK_STATE_wValue_OFF_HOOK	1
#define	USB_Comm_SET_HOOK_STATE_wValue_SNOOPING	2	/* for receiving caller ID */

#define	USB_Comm_SET_HOOK_STATE_wValue__INIT \
	{ USB_Comm_SET_HOOK_STATE_wValue_ON_HOOK,	"ON_HOOK" }, \
	{ USB_Comm_SET_HOOK_STATE_wValue_OFF_HOOK,	"OFF_HOOK" }, \
	{ USB_Comm_SET_HOOK_STATE_wValue_SNOOPING,	"SNOOPING" }

/*
|| PULSE_SETUP (6.2.8)
*/
#define	USB_Comm_PULSE_SETUP_wValue_DISABLE	0xFFFF
#define	USB_Comm_PULSE_SETUP_wValue_ENABLE	0	/* really, anything but DISABLE */

#define	USB_Comm_PULSE_SETUP_wValue__INIT \
	{ USB_Comm_PULSE_SETUP_wValue_DISABLE,	"DISABLE" }, \
	{ USB_Comm_PULSE_SETUP_wValue_ENABLE,	"ENABLE" }

/*
|| SET_LINE_CODING (6.2.12)
|| GET_LINE_CODING (6.2.13)
*/

/* the payload */
struct USB_Comm_LINE_CODING
	{
	unsigned char	dwDTERate[4];
	unsigned char	bCharFormat;
	unsigned char	bParityType;
	unsigned char	bDataBits;
	};

/* declare size of structure to avoid packing problems */
#define	USB_Comm_LINE_CODING__SIZE	7

#define	USB_Comm_LINE_CODING_bCharFormat_1STOP	 0
#define	USB_Comm_LINE_CODING_bCharFormat_1_5STOP 1
#define	USB_Comm_LINE_CODING_bCharFormat_2STOP	2

#define	USB_Comm_LINE_CODING_bCharFormat__INIT \
	{ USB_Comm_LINE_CODING_bCharFormat_1STOP,	"1STOP" }, \
	{ USB_Comm_LINE_CODING_bCharFormat_1_5STOP,	"1.5STOP" }, \
	{ USB_Comm_LINE_CODING_bCharFormat_2STOP,	"2STOP" }

#define	USB_Comm_LINE_CODING_bParityType_NONE	0
#define	USB_Comm_LINE_CODING_bParityType_ODD	1
#define	USB_Comm_LINE_CODING_bParityType_EVEN	2
#define	USB_Comm_LINE_CODING_bParityType_MARK	3
#define	USB_Comm_LINE_CODING_bParityType_SPACE	4

#define	USB_Comm_LINE_CODING_bParityType__INIT \
	{ USB_Comm_LINE_CODING_bParityType_NONE,	"NONE" }, \
	{ USB_Comm_LINE_CODING_bParityType_ODD,		"ODD" }, \
	{ USB_Comm_LINE_CODING_bParityType_EVEN,	"EVEN" }, \
	{ USB_Comm_LINE_CODING_bParityType_MARK,	"MARK" }, \
	{ USB_Comm_LINE_CODING_bParityType_SPACE,	"SPACE" }

/*
|| SET_CONTROL_LINE_STATE (6.2.14)
*/
#define	USB_Comm_SET_CONTROL_LINE_STATE_wValue_DTR	0x01
#define	USB_Comm_SET_CONTROL_LINE_STATE_wValue_RTS	0x02

#define	USB_Comm_SET_CONTROL_LINE_STATE_wValue__INIT \
	{ USB_Comm_SET_CONTROL_LINE_STATE_wValue_DTR,	"DTR" }, \
	{ USB_Comm_SET_CONTROL_LINE_STATE_wValue_RTS,	"RTS" }

/*
|| SET_RINGER_PARMS (6.2.16)
|| GET_RINGER_PARMS (6.2.17)
*/
struct USB_Comm_RINGER_CONFIG
	{
	unsigned char dwBitmap[4];
	};

struct USB_Comm_RINGER_CONFIG_MCCI
	{
	unsigned char	bPattern;
	unsigned char	bVolume;
	unsigned char	bReserved;
	unsigned char	bFlags;
	};

#define	USB_Comm_RINGER_CONFIG_dwBitmap_EXISTS		0x80000000ul
#define	USB_Comm_RINGER_CONFIG_dwBitmap_VOLUME_MASK	0x0000FF00ul
#define USB_Comm_RINGER_CONFIG_dwBitmap_VOLUME_SHIFT	8
#define	USB_Comm_RINGER_CONFIG_dwBitmap_PATTERN_MASK	0x000000FFul
#define	USB_Comm_RINGER_CONFIG_dwBitmap_PATTERN_SHIFT	0

/*
|| SET_OPERATION_PARMS (6.2.18)
|| GET_OPERATION_PARMS (6.2.19)
*/
#define	USB_Comm_SET_OPERATION_PARMS_wValue_SIMPLE	0
#define	USB_Comm_SET_OPERATION_PARMS_wValue_STANDALONE	1
#define	USB_Comm_SET_OPERATION_PARMS_wValue_HOST	2

#define	USB_Comm_SET_OPERATION_PARMS_wValue__INIT \
	{ USB_Comm_SET_OPERATION_PARMS_wValue_SIMPLE,		"SIMPLE" }, \
	{ USB_Comm_SET_OPERATION_PARMS_wValue_STANDALONE,	"STANDALONE" }, \
	{ USB_Comm_SET_OPERATION_PARMS_wValue_HOST,		"HOST" }

/*
|| SET_LINE_PARMS (6.2.20)
*/
#define	USB_Comm_SET_LINE_PARMS_wValue_DROP	0
#define	USB_Comm_SET_LINE_PARMS_wValue_START	1
#define	USB_Comm_SET_LINE_PARMS_wValue_RING	2
#define	USB_Comm_SET_LINE_PARMS_wValue_UNRING	3
#define	USB_Comm_SET_LINE_PARMS_wValue_SWITCH	4	/* data phase includes 1-byte call index */

#define	USB_Comm_SET_LINE_PARMS_wValue__INIT \
	{ USB_Comm_SET_LINE_PARMS_wValue_DROP,		"DROP" }, \
	{ USB_Comm_SET_LINE_PARMS_wValue_START,		"START" }, \
	{ USB_Comm_SET_LINE_PARMS_wValue_RING,		"RING" }, \
	{ USB_Comm_SET_LINE_PARMS_wValue_UNRING,	"UNRING" }, \
	{ USB_Comm_SET_LINE_PARMS_wValue_SWITCH,	"SWITCH" }

/*
|| GET_LINE_PARMS (6.2.21)
*/
struct USB_Comm_LINE_STATUS
	{
	unsigned char	wLength[2];
	unsigned char	dwRingerBitmap[4];	/* as given in table 52; use
						|| USB_Comm_RINGER_CONFIG_dwBitmap_...
						*/
	unsigned char	dwLineState[4];		/* line status */
	unsigned char	dwCallState0[4];
	};

#define	USB_Comm_LINE_STATUS_dwLineState_CALL_INDEX_MASK	(0x000000FFul)
#define	USB_Comm_LINE_STATUS_dwLineState_CALL_INDEX_SHIFT	0
#define	USB_Comm_LINE_STATUS_dwLineState_ACTIVE			(0x80000000ul)

#define	USB_Comm_LINE_STATUS_dwCallState_ACTIVE			(0x80000000ul)
#define	USB_Comm_LINE_STATUS_dwCallState_CHANGE_MASK		(0x0000FF00ul)
#define	USB_Comm_LINE_STATUS_dwCallState_CHANGE_SHIFT		8
#define	USB_Comm_LINE_STATUS_dwCallState_VALUE_MASK		(0x000000FFul)
#define	USB_Comm_LINE_STATUS_dwCallState_VALUE_SHIFT		0

#define	USB_Comm_LINE_STATUS_dwCallState_VALUE_IDLE		(0 << USB_Comm_LINE_STATUS_dwCallState_VALUE_SHIFT)
#define	USB_Comm_LINE_STATUS_dwCallState_VALUE_DIALTONE_TYP	(1 << USB_Comm_LINE_STATUS_dwCallState_VALUE_SHIFT)
#define	USB_Comm_LINE_STATUS_dwCallState_VALUE_DIALTONE_INT	(2 << USB_Comm_LINE_STATUS_dwCallState_VALUE_SHIFT)
#define	USB_Comm_LINE_STATUS_dwCallState_VALUE_DIALING		(3 << USB_Comm_LINE_STATUS_dwCallState_VALUE_SHIFT)
#define	USB_Comm_LINE_STATUS_dwCallState_VALUE_RINGBACK		(4 << USB_Comm_LINE_STATUS_dwCallState_VALUE_SHIFT)
#define	USB_Comm_LINE_STATUS_dwCallState_VALUE_CONNECTED	(5 << USB_Comm_LINE_STATUS_dwCallState_VALUE_SHIFT)
#define	USB_Comm_LINE_STATUS_dwCallState_VALUE_INCOMING		(6 << USB_Comm_LINE_STATUS_dwCallState_VALUE_SHIFT)

#define	USB_Comm_LINE_STATUS_dwCallState_VALUE__INIT \
	{ USB_Comm_LINE_STATUS_dwCallState_VALUE_IDLE,		"IDLE" }, \
	{ USB_Comm_LINE_STATUS_dwCallState_VALUE_DIALTONE_TYP,	"DIALTONE_TYP" }, \
	{ USB_Comm_LINE_STATUS_dwCallState_VALUE_DIALTONE_INT,	"DIALTONE_INT" }, \
	{ USB_Comm_LINE_STATUS_dwCallState_VALUE_DIALING,	"DIALING" }, \
	{ USB_Comm_LINE_STATUS_dwCallState_VALUE_RINGING,	"RINGING" }, \
	{ USB_Comm_LINE_STATUS_dwCallState_VALUE_RINGBACK,	"RINGBACK" }, \
	{ USB_Comm_LINE_STATUS_dwCallState_VALUE_CONNECTED,	"CONNECTED" }, \
	{ USB_Comm_LINE_STATUS_dwCallState_VALUE_INCOMING,	"INCOMING" }

#define	USB_Comm_LINE_STATUS_dwCallState_CHANGE_IDLE		(1 << USB_Comm_LINE_STATUS_dwCallState_CHANGE_SHIFT)
#define	USB_Comm_LINE_STATUS_dwCallState_CHANGE_DIALING		(2 << USB_Comm_LINE_STATUS_dwCallState_CHANGE_SHIFT)
#define	USB_Comm_LINE_STATUS_dwCallState_CHANGE_RINGBACK	(3 << USB_Comm_LINE_STATUS_dwCallState_CHANGE_SHIFT)
#define	USB_Comm_LINE_STATUS_dwCallState_CHANGE_CONNECTED	(4 << USB_Comm_LINE_STATUS_dwCallState_CHANGE_SHIFT)
#define	USB_Comm_LINE_STATUS_dwCallState_CHANGE_INCOMING	(5 << USB_Comm_LINE_STATUS_dwCallState_CHANGE_SHIFT)

#define	USB_Comm_LINE_STATUS_dwCallState_CHANGE__INIT \
	{ USB_Comm_LINE_STATUS_dwCallState_CHANGE_IDLE,		"IDLE" }, \
	{ USB_Comm_LINE_STATUS_dwCallState_CHANGE_DIALING,	"DIALING" }, \
	{ USB_Comm_LINE_STATUS_dwCallState_CHANGE_RINGBACK,	"RINGBACK" }, \
	{ USB_Comm_LINE_STATUS_dwCallState_CHANGE_CONNECTED,	"CONNECTED" }, \
	{ USB_Comm_LINE_STATUS_dwCallState_CHANGE_INCOMING,	"INCOMING" }

/*
|| SET_UNIT_PARAMETER (6.2.23)
|| GET_UNIT_PARAMETER (6.2.24)
|| CLEAR_UNIT_PARAMETER (6.2.25)
*/
#define	USB_Comm_SET_UNIT_PARAMETER_wValue_bEntityId_SHIFT		0
#define	USB_Comm_SET_UNIT_PARAMETER_wValue_bEntityId_MASK		0x00FF

#define	USB_Comm_SET_UNIT_PARAMETER_wValue_bParameterIndex_SHIFT	8
#define	USB_Comm_SET_UNIT_PARAMETER_wValue_bParameterIndex_MASK		0xFF00

/*
|| set Ethernet Packet Filter (6.2.30)
*/
#define	USB_Comm_SET_ETHERNET_PACKET_FILTER_wValue_PROMISCUOUS	 0x01
#define	USB_Comm_SET_ETHERNET_PACKET_FILTER_wValue_ALL_MULTICAST 0x02
#define	USB_Comm_SET_ETHERNET_PACKET_FILTER_wValue_DIRECTED	 0x04
#define	USB_Comm_SET_ETHERNET_PACKET_FILTER_wValue_BROADCAST	 0x08
#define	USB_Comm_SET_ETHERNET_PACKET_FILTER_wValue_MULTICAST	 0x10

#define USB_Comm_SET_ETHERNET_PACKET_FILTER_wValue__INIT \
	{ USB_Comm_SET_ETHERNET_PACKET_FILTER_wValue_PROMISCUOUS,	"PROMISCUOUS" }, \
	{ USB_Comm_SET_ETHERNET_PACKET_FILTER_wValue_ALL_MULTICAST,	"ALL_MULTICAST" }, \
	{ USB_Comm_SET_ETHERNET_PACKET_FILTER_wValue_DIRECTED,		"DIRECTED" }, \
	{ USB_Comm_SET_ETHERNET_PACKET_FILTER_wValue_BROADCAST,		"BROADCAST" }, \
	{ USB_Comm_SET_ETHERNET_PACKET_FILTER_wValue_MULTICAST,		"MULTICAST" }

/*
|| GET_ETHERNET_STATISTIC (6.2.31)
||	See above, under bmCapabilities for the ETHERNET Functional Descriptor
||	as the feature selectors are defined there.
*/

/*
|| SET_ATM_DATA_FORMAT (6.2.32)
*/
#define	USB_Comm_SET_ATM_DATA_FORMAT_wValue_TYPE1	1	/* concatenated ATM cells */
#define	USB_Comm_SET_ATM_DATA_FORMAT_wValue_TYPE2	2	/* ATM header + concatenated payloads */
#define	USB_Comm_SET_ATM_DATA_FORMAT_wValue_TYPE3	3	/* AAL5 SDU */

#define USB_Comm_SET_ATM_DATA_FORMAT_wValue__INIT \
	{ USB_Comm_SET_ATM_DATA_FORMAT_wValue_TYPE1,	"TYPE1" }, \
	{ USB_Comm_SET_ATM_DATA_FORMAT_wValue_TYPE2,	"TYPE2" }, \
	{ USB_Comm_SET_ATM_DATA_FORMAT_wValue_TYPE3,	"TYPE3" }

/*
|| GET_ATM_DEVICE_STATISTICS  (6.2.33)
*/
#define	USB_Comm_ATM_DEVICE_STATISTICS_wValue_US_CELLS_SENT		1
#define	USB_Comm_ATM_DEVICE_STATISTICS_wValue_DS_CELLS_RECEIVED		2
#define	USB_Comm_ATM_DEVICE_STATISTICS_wValue_DS_CELLS_USB_CONGESTION	3
#define	USB_Comm_ATM_DEVICE_STATISTICS_wValue_DS_CELLS_AAL5_CRC		4
#define	USB_Comm_ATM_DEVICE_STATISTICS_wValue_DS_CELLS_HEC_ERROR	5
#define	USB_Comm_ATM_DEVICE_STATISTICS_wValue_DS_CELLS_HEC_CORRECTED	6

#define	USB_Comm_ATM_DEVICE_STATISTICS_wValue__INIT \
	{ USB_Comm_ATM_DEVICE_STATISTICS_wValue_US_CELLS_SENT,			"US_CELLS_SENT" }, \
	{ USB_Comm_ATM_DEVICE_STATISTICS_wValue_DS_CELLS_RECEIVED,		"DS_CELLS_RECEIVED" }, \
	{ USB_Comm_ATM_DEVICE_STATISTICS_wValue_DS_CELLS_USB_CONGESTION,	"DS_CELLS_USB_CONGESTION" }, \
	{ USB_Comm_ATM_DEVICE_STATISTICS_wValue_DS_CELLS_AAL5_CRC,		"DS_CELLS_AAL5_CRC" }, \
	{ USB_Comm_ATM_DEVICE_STATISTICS_wValue_DS_CELLS_HEC_ERROR,		"DS_CELLS_HEC_ERROR" }, \
	{ USB_Comm_ATM_DEVICE_STATISTICS_wValue_DS_CELLS_HEC_CORRECTED,		"DS_CELLS_HEC_CORRECTED" }

/*
|| SET_ATM_DEFAULT_VC (6.2.34)
*/
struct USB_Comm_ATM_VPI_VCI
	{
	unsigned char	bVPI;
	unsigned char	bVCI[2];
	};

/*
|| GET_ATM_VC_STATISTICS (6.2.35)
*/
#define	USB_Comm_ATM_VC_STATISTICS_wValue_VC_US_CELLS_SENT	1
#define	USB_Comm_ATM_VC_STATISTICS_wValue_VC_DS_CELLS_RECEIVED	2

#define	USB_Comm_ATM_VC_STATISTICS_wValue__INIT \
	{ USB_Comm_ATM_VC_STATISTICS_wValue_VC_US_CELLS_SENT,		"VC_US_CELLS_SENT" }, \
	{ USB_Comm_ATM_VC_STATISTICS_wValue_VC_DS_CELLS_RECEIVED,	"VC_DS_CELLS_RECEIVED" }

/****************************************************************************\
|
|	Messages (Notifications) contents
|
\****************************************************************************/

struct USB_Comm_NOTIFICATION_GENERIC
	{
	unsigned char	bmRequestType;
	unsigned char	bNotification;
	unsigned char	wValue[2];
	unsigned char	wIndex[2];
	unsigned char	wLength[2];
	unsigned char	bData[1];
	};

/*
|| NETWORK_CONNECTION (6.3.1)
*/
#define	USB_Comm_NETWORK_CONNECTION_wValue_DISCONNECT	0
#define	USB_Comm_NETWORK_CONNECTION_wValue_CONNECT	1

#define	USB_Comm_NETWORK_CONNECTION_wValue__INIT \
	{ USB_Comm_NETWORK_CONNECTION_wValue_DISCONNECT,	"DISCONNECT" }, \
	{ USB_Comm_NETWORK_CONNECTION_wValue_CONNECT,		"CONNECT" }

/*
|| SERIAL_STATE (6.3.5)
*/
struct USB_Comm_NOTIFICATION_SERIAL_STATE
	{
	unsigned char	bmRequestType;
	unsigned char	bNotification;
	unsigned char	wValue[2];
	unsigned char	wIndex[2];
	unsigned char	wLength[2];
	unsigned char	bmUartState[2];			
	};

/* need the size of the message to avoid packing problems */
#define	USB_Comm_NOTIFICATION_SERIAL_STATE__SIZE	10

#define	USB_Comm_SERIAL_STATE_bRxCarrier	0x01
#define	USB_Comm_SERIAL_STATE_bTxCarrier	0x02
#define	USB_Comm_SERIAL_STATE_bBreak		0x04
#define	USB_Comm_SERIAL_STATE_bRingSignal	0x08
#define	USB_Comm_SERIAL_STATE_bFraming		0x10
#define	USB_Comm_SERIAL_STATE_bParity		0x20
#define	USB_Comm_SERIAL_STATE_bOverRun		0x40

#define	USB_Comm_SERIAL_STATE__INIT \
	{ USB_Comm_SERIAL_STATE_bRxCarrier,	"RxCarrier(DCD)" }, \
	{ USB_Comm_SERIAL_STATE_bTxCarrier,	"TxCarrier(DSR)" }, \
	{ USB_Comm_SERIAL_STATE_bBreak,		"Break" }, \
	{ USB_Comm_SERIAL_STATE_bRingSignal,	"RingSignal" }, \
	{ USB_Comm_SERIAL_STATE_bFraming,	"Framing" }, \
	{ USB_Comm_SERIAL_STATE_bParity,	"Parity" }, \
	{ USB_Comm_SERIAL_STATE_bOverRun,	"OverRun" }

/*
|| CALL_STATE_CHANGE (6.3.6)
*/
#define	USB_Comm_CALL_STATE_CHANGE_wValue_INDEX_MASK	0xFF00
#define	USB_Comm_CALL_STATE_CHANGE_wValue_INDEX_SHIFT	8

#define	USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_MASK	0x00FF
#define	USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_SHIFT	0

#define	USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_IDLE		(1 << USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_SHIFT)
#define	USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_DIALING	(2 << USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_SHIFT)
#define	USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_RINGBACK	(3 << USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_SHIFT)
#define	USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_CONNECTED	(4 << USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_SHIFT)
#define	USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_INCOMING	(5 << USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_SHIFT)

#define	USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE__INIT \
	{ USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_IDLE,	"IDLE" }, \
	{ USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_DIALING,	"DIALING" }, \
	{ USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_RINGBACK,	"RINGBACK" }, \
	{ USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_CONNECTED,	"CONNECTED" }, \
	{ USB_Comm_CALL_STATE_CHANGE_wValue_CHANGE_INCOMING,	"INCOMING" }

struct USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_RINGBACK
	{
	unsigned char	bRingbackType;
	};

#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_RINGBACK_bRingbackType_NORMAL	0
#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_RINGBACK_bRingbackType_BUSY	1
#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_RINGBACK_bRingbackType_FASTBUSY	2
#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_RINGBACK_bRingbackType_UNKNOWN	255

struct USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_CONNECTED
	{
	unsigned char	bConnectionType;
	};

#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_RINGBACK_bConnectionType_VOICE	 0
#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_RINGBACK_bConnectionType_TAD	 1	/* telephone answering device */
#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_RINGBACK_bConnectionType_FAX	 2
#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_RINGBACK_bConnectionType_MODEM	 3
#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_RINGBACK_bConnectionType_UNKNOWN 4

struct USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_INCOMING
	{
	unsigned char	bRingPattern;
	unsigned char	bCallerIdData[1];
	};

/* CallerID string layout */
struct USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_INCOMING_ID_STRING
	{
	unsigned char	ucLength;
	unsigned char	bText[1];
	};

/* CallerID string sequences */
#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_INCOMING_INDEX_TIME	0
#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_INCOMING_INDEX_NUMBER	1
#define	USB_Comm_NOTIFICATION_CALL_STATE_CHANGE_INCOMING_INDEX_NAME	2

/*
|| LINE_STATE_CHANGE (6.3.7)
*/
#define	USB_Comm_LINE_STATE_CHANGE_wValue_IDLE		0
#define	USB_Comm_LINE_STATE_CHANGE_wValue_HOLD		1
#define	USB_Comm_LINE_STATE_CHANGE_wValue_OFFHOOK	2
#define	USB_Comm_LINE_STATE_CHANGE_wValue_ONHOOK	3

/*
|| CONNECTION_SPEED_CHANGE (6.3.8)
*/
struct USB_Comm_CONNECTION_SPEED_CHANGE_Struct
	{
	unsigned char	dwUSBitRate[4];
	unsigned char	dwDSBitRate[4];
	};

/**** end of usbcdc11.h ****/
#endif /* _USBCDC11_H_ */
