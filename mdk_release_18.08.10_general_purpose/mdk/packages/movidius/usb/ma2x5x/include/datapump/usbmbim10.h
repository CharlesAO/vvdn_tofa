/* usbmbim10.h	Mon Jun  1 2015 00:20:22 tmm */

/*

Module:  usbmbim10.h

Function:
	USB Communication Device Class (CDC) Mobile Broadband Interface
	Model (MBIM) V1.0 constants

Version:
	V3.15b	Mon Jun  1 2015 00:20:22 tmm	Edit level 9

Copyright notice:
	This file copyright (C) 2011-2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Mats Webjorn, MCCI Corporation	August 2011

Revision history:
   3.01f  Mon Aug 22 2011 16:54:21  maw
	13925: Module created.

   3.01f  Fri Dec 09 2011 16:07:36  chwon
	14550: Fixed RVCT compiler warning -- make unsigned constant.

   3.01f  Tue Dec 27 2011 15:57:11  ssyang
	13926: Changed ifc subclass to 0x0E and added bMaxFilterSize into
	functional descriptor to be compliant to MBIM spec

   3.01f  Thu Jan 05 2012 20:15:29  ssyang
	13926: Added command message constant and structure definitions

   3.01f  Wed Apr 04 2012 16:21:20  subbarayalu
	15152: Added __SIZE definitions for various structures

   3.01f  Wed Apr 25 2012 11:28:32  subbarayalu
	15152: Removed offset() and used DataPump internals __TMS_OFFSET()

   3.01f  Wed May 02 2012 14:53:21  subbarayalu
	15152: Removed __TMS_OFFSET() dependencies
	Update __SIZE definitions and various structures

   3.11c  Sun Mar 31 2013 19:05:48  ssyang
	16956: Add constant USB_Comm_MBIM_wMaxSegmentSize_Minimum.

   3.13a  Wed Nov 05 2014 19:09:21  rakesh
	Renamed member PinNode to PinMode in struct USB_Comm_MBIM_PIN_DESC.

   3.15b  Mon Jun  1 2015 00:20:22  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _USBMBIM10_H_		/* prevent multiple includes */
#define _USBMBIM10_H_

#ifndef _USBNCM10_H_
# include "usbncm10.h"
#endif


/****************************************************************************\
|
|	Subclass, and protocol codes: Section 4, Section 6.5
|
\****************************************************************************/

/* Table 4-1: MBIM Communications Interface Subclasses Code */
#define	USB_bInterfaceSubClass_CommMBIM				0x0E

/* Table 4-2: MBIM Communications Interface Protocol codes */
#define	USB_bInterfaceProtocol_CommMBIM				0x00

/* Table 6-4: Data interface Protocol codes */
#define	USB_bInterfaceProtocol_DataMBIM				0x02

/****************************************************************************\
|
|	Functional Detail Descriptor Codes: Section 4, Table 4-3
|
\****************************************************************************/

/*
|| this byte appears in bGuidDescriptorType to identify an MBIM
|| functional descriptor.
*/
#define	USB_bDescriptorSubtype_Comm_MBIM			0x1B

/****************************************************************************\
|
|	Network Control Model Requests Codes:  Section 4, Table 4-4
|
\****************************************************************************/

/****************************************************************************\
|
|	Requests Codes:  One more request than those defined
|	by CDC NCM are used
|
\****************************************************************************/

#define	USB_bRequest_Mbim_RESET					0x05

/****************************************************************************\
|
|	Notification Elements:  No other notifications than those defined
|	by CDC ECM are used
|
\****************************************************************************/

/****************************************************************************\
|
|	MBIM Functional Descriptor: Section 6.4
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_MBIM
	{
	unsigned char	bFunctionLength;	/* varies */
	unsigned char	bDescriptorType;	/* CS_INTERFACE */
	unsigned char	bDescriptorSubtype;	/* USB_bDescriptorSubtype_Comm_MBIM */
	unsigned char	bcdMBIMVersion[2];	/* 0x0100 */
	unsigned char	wMaxControlMessage[2];
	unsigned char	bNumberFilters;
	unsigned char	bMaxFilterSize;
	unsigned char	wMaxSegmentSize [2];
	unsigned char	bmNetworkCapabilities;
	};

/* size of the descriptor, in bytes */
#define USB_Comm_FunctionalDescriptor_MBIM__SIZE			(12)

/* minimal value for wMaxSegmentSize */
#define USB_Comm_MBIM_wMaxSegmentSize_Minimum				(2048)

/* the bits in bmNetworkCapabilities */
#define	USB_Comm_MBIM_bmNetworkCapabilities_NOT_USED_D0			(1u << 0)
#define	USB_Comm_MBIM_bmNetworkCapabilities_NOT_USED_D1			(1u << 1)
#define	USB_Comm_MBIM_bmNetworkCapabilities_NOT_USED_D2			(1u << 2)
#define	USB_Comm_MBIM_bmNetworkCapabilities_HAS_DATAGRAM_CMD		(1u << 3)
#define	USB_Comm_MBIM_bmNetworkCapabilities_NOT_USED_D4			(1u << 4)
#define	USB_Comm_MBIM_bmNetworkCapabilities_HAS_NTB_EXTENDED_INPUT_SIZE	(1u << 5)

#define	USB_Comm_MBIM_bmNetworkCapabilities__INIT			\
	{ USB_Comm_MBIM_bmNetworkCapabilities_NOT_USED_D0,		\
		"NOT_USED_D0" },					\
	{ USB_Comm_MBIM_bmNetworkCapabilities_NOT_USED_D1,		\
		"NOT_USED_D1" },					\
	{ USB_Comm_MBIM_bmNetworkCapabilities_NOT_USED_D2,		\
		"NOT_USED_D2" },					\
	{ USB_Comm_MBIM_bmNetworkCapabilities_HAS_DATAGRAM_CMD,		\
		"HAS_DATAGRAM_CMD" },					\
	{ USB_Comm_MBIM_bmNetworkCapabilities_NOT_USED_D4,		\
		"NOT_USED_D4" },					\
	{ USB_Comm_MBIM_bmNetworkCapabilities_HAS_NTB_EXTENDED_INPUT_SIZE, \
		"HAS_NTB_EXTENDED_INPUT_SIZE" }

/****************************************************************************\
|
|	The layout of GET_NTB_PARAMETERS data:
|	See NCM section 6.2.1
|
\****************************************************************************/

/****************************************************************************\
|
|	The layout of GET/SET_NTB_FORMAT data:
|	See NCM section 6.2.4/6.2.5
|
\****************************************************************************/

/****************************************************************************\
|
|	The layout of GET/SET_NTB_INPUT_SIZE data:
|	See NCM section 6.2.6/6.2.7
|
\****************************************************************************/

/****************************************************************************\
|
|	The layout of GET/SET_MAX_DATAGRAM_SIZE data:
|	See NCM section 6.2.8/6.2.9
|
\****************************************************************************/

/****************************************************************************\
|
|	The layout of an NCM 16-bit Transfer Header (NTH16):
|	See NCM section 3.2.1
|
\****************************************************************************/

/****************************************************************************\
|
|	The layout of an NCM 32-bit Transfer Header (NTH32):
|	See NCM section 3.2.2
|
\****************************************************************************/

/****************************************************************************\
|
|	The layout of an NCM 16-bit Datagram Pointers (NDP16):
|	See NCM Section 7, Table 7-1
|
\****************************************************************************/

#define USB_Comm_MBIM_NDP16_SIGNATURE_IPS			0x00535049ul /* IPSx */
#define USB_Comm_MBIM_NDP16_SIGNATURE_DSS			0x00535344ul /* DSSx */

#define	USB_Comm_MBIM_NDP16_SIGNATURE_SESSIONID_SHIFT		24
#define	USB_Comm_MBIM_NDP16_SIGNATURE_SESSIONID_MASK	\
	(0xFFul << USB_Comm_MBIM_NDP16_SIGNATURE_SESSIONID_SHIFT)
#define	USB_Comm_MBIM_NDP16_SIGNATURE_SESSIONID(a_signature)\
	(((a_signature) >> USB_Comm_MBIM_NDP16_SIGNATURE_SESSIONID_SHIFT) & 0xFF)

/****************************************************************************\
|
|	The layout of an NCM 32-bit Datagram Pointers (NDP32):
|	See NCM Section 7, Table 7-1
|
\****************************************************************************/

#define USB_Comm_MBIM_NDP32_SIGNATURE_IPS			0x00737069ul /* ipsx */
#define USB_Comm_MBIM_NDP32_SIGNATURE_DSS			0x00737364ul /* dssx */

#define	USB_Comm_MBIM_NDP32_SIGNATURE_SESSIONID_SHIFT		24
#define	USB_Comm_MBIM_NDP32_SIGNATURE_SESSIONID_MASK	\
	(0xFFul << USB_Comm_MBIM_NDP32_SIGNATURE_SESSIONID_SHIFT)
#define	USB_Comm_MBIM_NDP32_SIGNATURE_SESSIONID(a_signature)\
	(((a_signature) >> USB_Comm_MBIM_NDP32_SIGNATURE_SESSIONID_SHIFT) & 0xFF)

/****************************************************************************\
|
|	MBIM Message Header: section 9.1
|
\****************************************************************************/

#define USB_Comm_MBIM_MESSAGE_CONTENTS				\
	unsigned char	MessageType[4];				\
	unsigned char	MessageLength[4];			\
	unsigned char	TransactionId[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_MESSAGE_HEADER
	{
	USB_Comm_MBIM_MESSAGE_CONTENTS;
	};

struct USB_Comm_MBIM_MESSAGE_GENERIC
	{
	USB_Comm_MBIM_MESSAGE_CONTENTS;
	unsigned char	MessageBuffer[1];
	};

/*
|| Do not include MessageBuffer in size. It's only there as a convinient
|| reference to next layer structure
*/
#define USB_Comm_MBIM_MESSAGE_HEADER__SIZE			12

/****************************************************************************\
|
|	MBIM Fragment Header: section 9.2
|	Note: The MBIM Fragment Header includes the MBIM Message Header
|	for ease of use
|
\****************************************************************************/

#define USB_Comm_MBIM_FRAGMENT_CONTENTS				\
	USB_Comm_MBIM_MESSAGE_CONTENTS;				\
	unsigned char	TotalFragments[4];	/* the number of fragments in this message */ \
	unsigned char	CurrentFragment[4]	/* the current fragment */

struct USB_Comm_MBIM_FRAGMENT_HEADER
	{
	USB_Comm_MBIM_FRAGMENT_CONTENTS;
	};

struct USB_Comm_MBIM_FRAGMENT
	{
	USB_Comm_MBIM_FRAGMENT_CONTENTS;
	unsigned char	FragmentBuffer[1];
	};

/*
|| Do not include FragmentBuffer in size, It's only there as a convenient
|| reference to next layer structure
*/
#define USB_Comm_MBIM_FRAGMENT_HEADER__SIZE	\
		(USB_Comm_MBIM_MESSAGE_HEADER__SIZE + 8)

/****************************************************************************\
|
|	MBIM Control Messages : section 9.3 & 9.4
|
\****************************************************************************/

#define USB_Comm_MBIM_MSGTYPE_OPEN				0x1
#define USB_Comm_MBIM_MSGTYPE_CLOSE				0x2
#define USB_Comm_MBIM_MSGTYPE_COMMAND				0x3
#define USB_Comm_MBIM_MSGTYPE_HOST_ERROR			0x4

#define USB_Comm_MBIM_MSGTYPE_OPEN_DONE				0x80000001ul
#define USB_Comm_MBIM_MSGTYPE_CLOSE_DONE			0x80000002ul
#define USB_Comm_MBIM_MSGTYPE_COMMAND_DONE			0x80000003ul
#define USB_Comm_MBIM_MSGTYPE_FUNCTION_ERROR			0x80000004ul
#define USB_Comm_MBIM_MSGTYPE_INDICATE_STATUS			0x80000007ul

/****************************************************************************\
|
|	MBIM Open Message : section 9.3.1 & 9.4.1
|
\****************************************************************************/

struct USB_Comm_MBIM_OPEN_MSG
	{
	unsigned char	MaxControlTransfer[4];
	};
/*
|| Define length of structure
*/
#define USB_Comm_MBIM_OPEN_MSG__SIZE				(4)

struct USB_Comm_MBIM_OPEN_DONE
	{
	unsigned char	Status[4];
	};

#define USB_Comm_MBIM_OPEN_DONE__SIZE				(4)

/****************************************************************************\
|
|	MBIM Close Message : section 9.3.2 & 9.4.2
|
\****************************************************************************/

struct USB_Comm_MBIM_CLOSE_MSG
	{
	unsigned char	Dummy;
	};

#define USB_Comm_MBIM_CLOSE_MSG__SIZE				(0)

struct USB_Comm_MBIM_CLOSE_DONE
	{
	unsigned char	Status[4];
	};

#define USB_Comm_MBIM_CLOSE_DONE__SIZE				(4)

/****************************************************************************\
|
|	MBIM Command Message : section 9.3.3 & 9.4.3
|
\****************************************************************************/

#define USB_Comm_MBIM_COMMAND_MSG_CONTENTS			\
	unsigned char	DeviceServiceId[16];			\
	unsigned char	CID[4];					\
	unsigned char	CommandType[4];				\
	unsigned char	InformationBufferLength[4]	/* no trailing semicolon */

struct USB_Comm_MBIM_COMMAND_HEADER
	{
	USB_Comm_MBIM_COMMAND_MSG_CONTENTS;
	};

struct USB_Comm_MBIM_COMMAND_MSG
	{
	USB_Comm_MBIM_COMMAND_MSG_CONTENTS;
	unsigned char	InformationBuffer[1];
	};

/*
|| Do not include InformationBuffer in size. It's only there as a convenient
|| reference to next layer structure
*/
#define USB_Comm_MBIM_COMMAND_HEADER__SIZE			28


#define USB_Comm_MBIM_CommandTypeQuery				0
#define USB_Comm_MBIM_CommandTypeSet	 			1

#define USB_Comm_MBIM_COMMAND_DONE_CONTENTS			\
	unsigned char	DeviceServiceId[16];			\
	unsigned char	CID[4];					\
	unsigned char	Status[4];				\
	unsigned char	InformationBufferLength[4]	/* no trailing semicolon */

struct USB_Comm_MBIM_COMMAND_DONE_HEADER
	{
	USB_Comm_MBIM_COMMAND_DONE_CONTENTS;
	};

struct USB_Comm_MBIM_COMMAND_DONE
	{
	USB_Comm_MBIM_COMMAND_DONE_CONTENTS;
	unsigned char	InformationBuffer[1];
	};

/*
|| Do not include InformationBuffer in size. It's only there as a convenient
|| reference to next layer structure
*/
#define USB_Comm_MBIM_COMMAND_DONE__SIZE			28

/****************************************************************************\
|
|	MBIM Specific data types : section 10.2
|
\****************************************************************************/

struct USB_Comm_OL_PAIR_LIST
	{
	unsigned char Offset[4];
	unsigned char Length[4];
	};

#define	USB_Comm_OL_PAIR_LIST__SIZE				(2 * 4)
/*
|| Mapping to service UUIDs defined in MBIM v0.81c 10.1
|| MBIM SERVICES AND THEIR CID VALUES
*/
#define USB_Comm_MBIM_ServiceTypeBasicConnect			0
#define USB_Comm_MBIM_ServiceTypeSms	 			1
#define USB_Comm_MBIM_ServiceTypeUssd				2
#define USB_Comm_MBIM_ServiceTypePhoneBook			3
#define USB_Comm_MBIM_ServiceTypeStk	 			4
#define USB_Comm_MBIM_ServiceTypeAuthentication			5
#define USB_Comm_MBIM_ServiceTypeDss				6

/****************************************************************************\
|
|	MBIM Function Error Message : section 9.3.4
|
\****************************************************************************/

struct USB_Comm_MBIM_ERROR_MSG
	{
	unsigned char	ErrorStatusCode[4];
	};

#define USB_Comm_MBIM_ERROR_MSG__SIZE				(4)

/****************************************************************************\
|
|	MBIM Error Status Codes : section 9.3.4
|
\****************************************************************************/

#define USB_Comm_MBIM_ERROR_TIMEOUT_FRAGMENT			1
#define USB_Comm_MBIM_ERROR_FRAGMENT_OUT_OF_SEQUENCE		2
#define USB_Comm_MBIM_ERROR_LENGTH_MISMATCH			3
#define USB_Comm_MBIM_ERROR_DUPLICATED_TID			4
#define USB_Comm_MBIM_ERROR_NOT_OPENED				5
#define USB_Comm_MBIM_ERROR_UNKNOWN				6
#define USB_Comm_MBIM_ERROR_CANCEL				7
#define USB_Comm_MBIM_ERROR_MAX_TRANSFER			8

/****************************************************************************\
|
|	MBIM Status Codes : section 9.4.5
|
\****************************************************************************/

#define USB_Comm_MBIM_STATUS_SUCCESS				0
#define USB_Comm_MBIM_STATUS_BUSY				1
#define USB_Comm_MBIM_STATUS_FAILURE				2
#define USB_Comm_MBIM_STATUS_SIM_NOT_INSERTED			3
#define USB_Comm_MBIM_STATUS_BAD_SIM				4
#define USB_Comm_MBIM_STATUS_PIN_REQUIRED			5
#define USB_Comm_MBIM_STATUS_PIN_DISABLED			6
#define USB_Comm_MBIM_STATUS_NOT_REGISTERED			7
#define USB_Comm_MBIM_STATUS_PROVIDERS_NOT_FOUND		8
#define USB_Comm_MBIM_STATUS_NO_DEVICE_SUPPORT			9
#define USB_Comm_MBIM_STATUS_PROVIDER_NOT_VISIBLE		10
#define USB_Comm_MBIM_STATUS_DATA_CLASS_NOT_AVAILABLE		11
#define USB_Comm_MBIM_STATUS_PACKET_SERVICE_DETACHED		12
#define USB_Comm_MBIM_STATUS_MAX_ACTIVATED_CONTEXTS		13
#define USB_Comm_MBIM_STATUS_NOT_INITIALIZED			14
#define USB_Comm_MBIM_STATUS_VOICE_CALL_IN_PROGRESS		15
#define USB_Comm_MBIM_STATUS_CONTEXT_NOT_ACTIVATED		16
#define USB_Comm_MBIM_STATUS_SERVICE_NOT_ACTIVATED		17
#define USB_Comm_MBIM_STATUS_INVALID_ACCESS_STRING		18
#define USB_Comm_MBIM_STATUS_INVALID_USER_NAME_PWD		19
#define USB_Comm_MBIM_STATUS_RADIO_POWER_OFF			20
#define USB_Comm_MBIM_STATUS_INVALID_PARAMETERS			21
#define USB_Comm_MBIM_STATUS_READ_FAILURE			22
#define USB_Comm_MBIM_STATUS_WRITE_FAILURE			23
/* reserved 24 */
#define USB_Comm_MBIM_STATUS_NO_PHONEBOOK			25
#define USB_Comm_MBIM_STATUS_PARAMETER_TOO_LONG			26
#define USB_Comm_MBIM_STATUS_STK_BUSY				27
#define USB_Comm_MBIM_STATUS_OPERATION_NOT_ALLOWED		28
#define USB_Comm_MBIM_STATUS_MEMORY_FAILURE			29
#define USB_Comm_MBIM_STATUS_INVALID_MEMORY_INDEX		30
#define USB_Comm_MBIM_STATUS_MEMORY_FULL			31
#define USB_Comm_MBIM_STATUS_FILTER_NOT_SUPPORTED		32
#define USB_Comm_MBIM_STATUS_DSS_INSTANCE_LIMIT			33
#define USB_Comm_MBIM_STATUS_INVALID_DEVICE_SERVICE_OPERATION	34
#define USB_Comm_MBIM_STATUS_AUTH_INCORRECT_AUTH		35
#define USB_Comm_MBIM_STATUS_AUTH_SYNC_FAILURE			36
#define USB_Comm_MBIM_STATUS_AUTH_AMF_NOT_SET			37
/* reserved 38 ~ 99 */
#define USB_Comm_MBIM_STATUS_SMS_UNKNOWN_SMSC_ADDRESS		100
#define USB_Comm_MBIM_STATUS_SMS_NETWORK_TIMEOUT		101
#define USB_Comm_MBIM_STATUS_SMS_LANG_NOT_SUPPORTED		102
#define USB_Comm_MBIM_STATUS_SMS_ENCODING_NOT_SUPPORTED		103
#define USB_Comm_MBIM_STATUS_SMS_FORMAT_NOT_SUPPORTED		104
/* device service specific status: 0x80000000 ~ 0xFFFFFFFF */

/****************************************************************************\
|
|	MBIM Indicate Status Message : section 9.5.4
|
\****************************************************************************/

#define USB_Comm_MBIM_INDICATE_STATUS_MSG_CONTENTS		\
	unsigned char	DeviceServiceId[16];			\
	unsigned char	CID[4];					\
	unsigned char	InformationBufferLength[4]	/* no trailing semicolon */

struct USB_Comm_MBIM_INDICATE_STATUS_MSG_HEADER
	{
	USB_Comm_MBIM_INDICATE_STATUS_MSG_CONTENTS;
	};

struct USB_Comm_MBIM_INDICATE_STATUS_MSG
	{
	USB_Comm_MBIM_INDICATE_STATUS_MSG_CONTENTS;
	unsigned char	InformationBuffer[1];
	};

/*
|| Do not include InformationBuffer in size. It's only there as a convenient
|| reference to next layer structure
*/
#define USB_Comm_MBIM_INDICATE_STATUS_MSG_HEADER__SIZE		24

/****************************************************************************\
|
|	MBIM CID Codes: section 10.1 table 10-4
|
\****************************************************************************/

/* UUID_BASIC_CONNECT */
#define USB_Comm_MBIM_CID_DEVICE_CAPS				1
#define USB_Comm_MBIM_CID_SUBSCRIBER_READY_STATUS		2
#define USB_Comm_MBIM_CID_RADIO_STATE				3
#define USB_Comm_MBIM_CID_PIN					4
#define USB_Comm_MBIM_CID_PIN_LIST				5
#define USB_Comm_MBIM_CID_HOME_PROVIDER				6
#define USB_Comm_MBIM_CID_PREFERRED_PROVIDERS			7
#define USB_Comm_MBIM_CID_VISIBLE_PROVIDERS			8
#define USB_Comm_MBIM_CID_REGISTER_STATE			9
#define USB_Comm_MBIM_CID_PACKET_SERVICE			10
#define USB_Comm_MBIM_CID_SIGNAL_STATE				11
#define USB_Comm_MBIM_CID_CONNECT				12
#define USB_Comm_MBIM_CID_PROVISIONED_CONTEXTS			13
#define USB_Comm_MBIM_CID_SERVICE_ACTIVATION			14
#define USB_Comm_MBIM_CID_IP_CONFIGURATION			15
#define USB_Comm_MBIM_CID_DEVICE_SERVICES			16
/* reserved 17 ~ 18 */
#define USB_Comm_MBIM_CID_SERVICE_SUBSCRIBE_LIST		19
#define USB_Comm_MBIM_CID_PACKET_STATISTICS			20
#define USB_Comm_MBIM_CID_NETWORK_IDLE_HINT			21
#define USB_Comm_MBIM_CID_EMERGENCY_MODE			22
#define USB_Comm_MBIM_CID_IP_PACKET_FILTERS			23
#define USB_Comm_MBIM_CID_MULTICARRIER_PROVIDERS		24

/* UUID_SMS */
#define USB_Comm_MBIM_CID_SMS_CONFIGURATION			1
#define USB_Comm_MBIM_CID_SMS_READ				2
#define USB_Comm_MBIM_CID_SMS_SEND				3
#define USB_Comm_MBIM_CID_SMS_DELETE				4
#define USB_Comm_MBIM_CID_SMS_MESSAGE_STORE_STATUS		5

/* UUID_USSD */
#define USB_Comm_MBIM_CID_USSD					1

/* UUID_PHONEBOOK */
#define USB_Comm_MBIM_CID_PHONEBOOK_CONFIGURATION		1
#define USB_Comm_MBIM_CID_PHONEBOOK_READ			2
#define USB_Comm_MBIM_CID_PHONEBOOK_DELETE			3
#define USB_Comm_MBIM_CID_PHONEBOOK_WRITE			4

/* UUID_STK */
#define USB_Comm_MBIM_CID_STK_PAC				1
#define USB_Comm_MBIM_CID_STK_TERMINAL_RESPONSE			2
#define USB_Comm_MBIM_CID_STK_ENVELOPE				3

/* UUID_AUTH */
#define USB_Comm_MBIM_CID_AKA_AUTH				1
#define USB_Comm_MBIM_CID_AKAP_AUTH				2
#define USB_Comm_MBIM_CID_SIM_AUTH				3

/* UUID_DSS */
#define USB_Comm_MBIM_CID_DSS_CONNECT				1


/****************************************************************************\
|
|	MBIM_CID_DEVICE_CAPS : section 10.5.1
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_DEVICE_TYPE	Table 10-7
\****************************************************************************/
#define USB_Comm_MBIM_DeviceTypeUnknown				0
#define USB_Comm_MBIM_DeviceTypeEmbedded			1
#define USB_Comm_MBIM_DeviceTypeRemovable			2
#define USB_Comm_MBIM_DeviceTypeRemote				3

/****************************************************************************\
|	MBIM_CELLULAR_CLASS	Table 10-8
\****************************************************************************/
#define USB_Comm_MBIM_CellularClassGsm				0x01
#define USB_Comm_MBIM_CellularClassCdma				0x02

/****************************************************************************\
|	MBIM_VOICE_CLASS	Table 10-9
\****************************************************************************/
#define USB_Comm_MBIM_VoiceClassUnknown				0
#define USB_Comm_MBIM_VoiceClassNoVoice				1
#define USB_Comm_MBIM_VoiceClassSeperateVoiceData		2
#define USB_Comm_MBIM_VoiceClassSimultaneousVoiceData		3

/****************************************************************************\
|	MBIM_SIM_CLASS	Table 10-10
\****************************************************************************/
#define USB_Comm_MBIM_SimClassSimLogical			0x01
#define USB_Comm_MBIM_SimClassSimRemovable			0x02

/****************************************************************************\
|	MBIM_DATA_CLASS	Table 10-11
\****************************************************************************/
#define USB_Comm_MBIM_DataClassNone				0x00
#define USB_Comm_MBIM_DataClassGPRS				0x01
#define USB_Comm_MBIM_DataClassEDGE				0x02
#define USB_Comm_MBIM_DataClassUMTS				0x04
#define USB_Comm_MBIM_DataClassHSDPA				0x08
#define USB_Comm_MBIM_DataClassHSUPA				0x10
#define USB_Comm_MBIM_DataClassLTE				0x20
/* reserved 40 ~ 8000 [For future GSM classes] */
#define USB_Comm_MBIM_DataClass1XRTT				0x10000
#define USB_Comm_MBIM_DataClass1XEVDO				0x20000
#define USB_Comm_MBIM_DataClass1XEVDORevA			0x40000
#define USB_Comm_MBIM_DataClass1XEVDV				0x80000
#define USB_Comm_MBIM_DataClass3XRTT				0x100000
#define USB_Comm_MBIM_DataClass1XEVDORevB			0x200000
#define USB_Comm_MBIM_DataClassUMB				0x400000
/* resrved 800000 ~ 40000000 [For future CDMA classes] */
#define USB_Comm_MBIM_DataClassCustom				0x80000000

/****************************************************************************\
|	MBIM_SMS_CAPS	Table 10-12
\****************************************************************************/
#define USB_Comm_MBIM_SmsCapsNone				0x00
#define USB_Comm_MBIM_SmsCapsPduReceive				0x01
#define USB_Comm_MBIM_SmsCapsPduSend				0x02
#define USB_Comm_MBIM_SmsCapsTextReceive			0x04
#define USB_Comm_MBIM_SmsCapsTextSend				0x08

/****************************************************************************\
|	MBIM_CTRL_CAPS	Table 10-13
\****************************************************************************/
#define USB_Comm_MBIM_CtrlCapsNone				0x00
#define USB_Comm_MBIM_CtrlCapsRegManual				0x01
#define USB_Comm_MBIM_CtrlCapsHwRadioSwitch			0x02
#define USB_Comm_MBIM_CtrlCapsCdmaMobileIp			0x04
#define USB_Comm_MBIM_CtrlCapsCdmaSimpleIp			0x08
#define USB_Comm_MBIM_CtrlCapsMultiCarrier			0x10

#define USB_Comm_MBIM_DEVICE_CAPS_INFO_CONTENTS			\
	unsigned char	DeviceType[4];				\
	unsigned char	CellularClass[4];			\
	unsigned char	VoiceClass[4];				\
	unsigned char	SimClass[4];				\
	unsigned char	DataClass[4];				\
	unsigned char	SmsCaps[4];				\
	unsigned char	ControlCaps[4];				\
	unsigned char	MaxSessions[4];				\
	unsigned char	CustomDataClassOffset[4];		\
	unsigned char	CustomDataClassSize[4];			\
	unsigned char	DeviceIdOffset[4];			\
	unsigned char	DeviceIdSize[4];			\
	unsigned char	FirmwareInfoOffset[4];			\
	unsigned char	FirmwareInfoSize[4];			\
	unsigned char	HardwareInfoOffset[4];			\
	unsigned char	HardwareInfoSize[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_DEVICE_CAPS_INFO_HEADER
	{
	USB_Comm_MBIM_DEVICE_CAPS_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_DEVICE_CAPS_INFO
	{
	USB_Comm_MBIM_DEVICE_CAPS_INFO_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| Define length of structure, but do not include DataBuffer
*/
#define USB_Comm_MBIM_DEVICE_CAPS_INFO__SIZE			64

/****************************************************************************\
|
|	MBIM_CID_SUBSCRIBER_READY_STATUS : section 10.5.2
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_UNIQUE_ID_FLAGS	Table 10-16
\****************************************************************************/
#define USB_Comm_MBIM_SubscriberReadyStateNotInitialized	0
#define USB_Comm_MBIM_SubscriberReadyStateInitialized		1
#define USB_Comm_MBIM_SubscriberReadyStateSimNotInserted	2
#define USB_Comm_MBIM_SubscriberReadyStateBadSim		3
#define USB_Comm_MBIM_SubscriberReadyStateFailure		4
#define USB_Comm_MBIM_SubscriberReadyStateNotActivated		5
#define USB_Comm_MBIM_SubscriberReadyStateDeviceLocked		6

/****************************************************************************\
|	MBIM_UNIQUE_ID_FLAGS	Table 10-17
\****************************************************************************/
#define USB_Comm_MBIM_ReadyInfoFlagsNone			0
#define USB_Comm_MBIM_ReadyInfoFlagsProtectUniqueID		1

#define USB_Comm_MBIM_SUBSCRIBER_READY_INFO_CONTENTS		\
	unsigned char			ReadyState[4];		\
	unsigned char			SubscriberIdOffset[4];	\
	unsigned char			SubscriberIdSize[4];	\
	unsigned char			SimIccldOffset[4];	\
	unsigned char			SimIccldSize[4];	\
	unsigned char			ReadyInfo[4];		\
	unsigned char			ElementCount[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_SUBSCRIBER_READY_INFO_HEADER
	{
	USB_Comm_MBIM_SUBSCRIBER_READY_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_SUBSCRIBER_READY_INFO
	{
	USB_Comm_MBIM_SUBSCRIBER_READY_INFO_CONTENTS;
	struct USB_Comm_OL_PAIR_LIST 	TelephoneNumbersRefList[1];
	};

#define USB_Comm_MBIM_SUBSCRIBER_READY_INFO_HEADER__SIZE	28

/*
|| Define length of structure,
|| but do not include DataBuffer
*/
#define USB_Comm_MBIM_SUBSCRIBER_READY_INFO__SIZE(ARG_ElementCount)	\
	((ARG_ElementCount) * USB_Comm_OL_PAIR_LIST__SIZE + 		\
	USB_Comm_MBIM_SUBSCRIBER_READY_INFO_HEADER__SIZE)

/*
|| Get DataBuffer from USB_Comm_MBIM_SUBSCRIBER_READY_INFO
*/
#define USB_Comm_MBIM_SUBSCRIBER_READY_INFO_DATABUFFER(			\
	ARG_pSubscriberReadyInfo, 					\
	ARG_ElementCount)						\
	((ARG_pSubscriberReadyInfo)->ReadyState +			\
	USB_Comm_MBIM_SUBSCRIBER_READY_INFO__SIZE(ARG_ElementCount))

/****************************************************************************\
|
|	MBIM_CID_RADIO_STATE : section 10.5.3
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_RADIO_SWITCH_STATE	Table 10-20
\****************************************************************************/
#define USB_Comm_MBIM_RadioOff					0
#define USB_Comm_MBIM_RadioOn					1

struct USB_Comm_MBIM_SET_RADIO_STATE
	{
	unsigned char	RadioState[4];
	};

#define USB_Comm_MBIM_SET_RADIO_STATE__SIZE			(4)

struct USB_Comm_MBIM_RADIO_STATE_INFO
	{
	unsigned char	HwRadioState[4];
	unsigned char	SwRadioState[4];
	};

#define USB_Comm_MBIM_RADIO_STATE_INFO__SIZE			(2 * 4)

/****************************************************************************\
|
|	MBIM_CID_PIN : section 10.5.4
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_PIN_TYPE	Table 10-24
\****************************************************************************/
#define USB_Comm_MBIM_PinTypeNone				0
#define USB_Comm_MBIM_PinTypeCustom				1
#define USB_Comm_MBIM_PinTypePin1				2
#define USB_Comm_MBIM_PinTypePin2				3
#define USB_Comm_MBIM_PinTypeDeviceSimPin			4
#define USB_Comm_MBIM_PinTypeDeviceFirstSimPin			5
#define USB_Comm_MBIM_PinTypeNetworkPin				6
#define USB_Comm_MBIM_PinTypeNetworkSubsetPin			7
#define USB_Comm_MBIM_PinTypeServiceProviderPin			8
#define USB_Comm_MBIM_PinTypeCorporatePin			9
#define USB_Comm_MBIM_PinTypeSubsidyLock			10
#define USB_Comm_MBIM_PinTypePuk1				11
#define USB_Comm_MBIM_PinTypePuk2				12
#define USB_Comm_MBIM_PinTypeDeviceFirstSimPuk			13
#define USB_Comm_MBIM_PinTypeNetworkPuk				14
#define USB_Comm_MBIM_PinTypeNetworkSubsetPuk			15
#define USB_Comm_MBIM_PinTypeServiceProviderPuk			16
#define USB_Comm_MBIM_PinTypeCorporatePuk			17

/****************************************************************************\
|	MBIM_PIN_STATE	Table 10-25
\****************************************************************************/
#define USB_Comm_MBIM_PinStateUnlocked				0
#define USB_Comm_MBIM_PinStatelocked				1

/****************************************************************************\
|	MBIM_PIN_OPERATION	Table 10-26
\****************************************************************************/
#define USB_Comm_MBIM_PinOperationEnter				0
#define USB_Comm_MBIM_PinOperationEnable			1
#define USB_Comm_MBIM_PinOperationDisable			2
#define USB_Comm_MBIM_PinOperationChange			3

#define USB_Comm_MBIM_SET_PIN_CONTENTS				\
	unsigned char	PinType[4];				\
	unsigned char	PinOperation[4];			\
	unsigned char	PinOffset[4];				\
	unsigned char	PinSize[4];				\
	unsigned char	NewPinOffset[4];			\
	unsigned char	NewPinSize[4]			/* no trailing semicolon */

struct USB_Comm_MBIM_SET_PIN_HEADER
	{
	USB_Comm_MBIM_SET_PIN_CONTENTS;
	};

struct USB_Comm_MBIM_SET_PIN
	{
	USB_Comm_MBIM_SET_PIN_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| Define length of structure, but do not include DataBuffer
*/
#define USB_Comm_MBIM_SET_PIN__SIZE				24

#define USB_Comm_MBIM_PIN_INFO_CONTENTS				\
	unsigned char	PinType[4];				\
	unsigned char	PinState[4]			/* no trailing semicolon */


struct USB_Comm_MBIM_PIN_INFO_HEADER
	{
	USB_Comm_MBIM_PIN_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_PIN_INFO
	{
	USB_Comm_MBIM_PIN_INFO_CONTENTS;
	unsigned char	RemainingAttempts[1];
	};
/*
|| RemainingAttempts is not included
*/
#define USB_Comm_MBIM_PIN_INFO__SIZE				8

/****************************************************************************\
|
|	MBIM_CID_PIN_LIST : section 10.5.5
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_PIN_MODE	Table 10-31
\****************************************************************************/
#define USB_Comm_MBIM_PinModeNotSupported			0
#define USB_Comm_MBIM_PinModeEnabled				1
#define USB_Comm_MBIM_PinModeDisabled				2

/****************************************************************************\
|	MBIM_PIN_FORMAT	Table 10-32
\****************************************************************************/
#define USB_Comm_MBIM_PinFormatUnknown				0
#define USB_Comm_MBIM_PinFormatNumeric				1
#define USB_Comm_MBIM_PinFormatAlphaNumeric			2

struct USB_Comm_MBIM_PIN_DESC
	{
	unsigned char	PinMode[4];
	unsigned char	PinFormat[4];
	unsigned char	PinLengthMin[4];
	unsigned char	PinLengthMax[4];
	};

#define USB_Comm_MBIM_PIN_DESC__SIZE				(4 * 4)

struct USB_Comm_MBIM_PIN_LIST_INFO
	{
	struct USB_Comm_MBIM_PIN_DESC	PinDescPin1;
	struct USB_Comm_MBIM_PIN_DESC	PinDescPin2;
	struct USB_Comm_MBIM_PIN_DESC	PinDescDeviceSimPin;
	struct USB_Comm_MBIM_PIN_DESC	PinDescDeviceFirstSimPin;
	struct USB_Comm_MBIM_PIN_DESC	PinDescNetworkPin;
	struct USB_Comm_MBIM_PIN_DESC	PinDescNetworkSubsetPin;
	struct USB_Comm_MBIM_PIN_DESC	PinDescServiceProviderPin;
	struct USB_Comm_MBIM_PIN_DESC	PinDescCorporatePin;
	struct USB_Comm_MBIM_PIN_DESC	PinDescSubsidyLock;
	struct USB_Comm_MBIM_PIN_DESC	PinDescCustom;
	};

/****************************************************************************\
|
|	MBIM_CID_HOME_PROVIDER : section 10.5.6
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_PROVIDER_STATE	Table 10-36
\****************************************************************************/
#define USB_Comm_MBIM_ProviderStateUnknown			0x00
#define USB_Comm_MBIM_ProviderStateHome				0x01
#define USB_Comm_MBIM_ProviderStateForbidden			0x02
#define USB_Comm_MBIM_ProviderStatePreffered			0x04
#define USB_Comm_MBIM_ProviderStateVisible			0x08
#define USB_Comm_MBIM_ProviderStateRegistered			0x10
#define USB_Comm_MBIM_ProviderStatePrefferredMulticarrier	0x20

#define USB_Comm_MBIM_PROVIDER_CONTENTS				\
	unsigned char	ProviderIdOffset[4];			\
	unsigned char	ProviderIdSize[4];			\
	unsigned char	ProviderState[4];			\
	unsigned char	ProviderNameOffset[4];			\
	unsigned char	ProviderNameSize[4];			\
	unsigned char	CellularClass[4];			\
	unsigned char	Rssi[4];				\
	unsigned char	ErrorRate[4]			/* no trailing semicolon */


struct USB_Comm_MBIM_PROVIDER_HEADER
	{
	USB_Comm_MBIM_PROVIDER_CONTENTS;
	};

struct USB_Comm_MBIM_PROVIDER
	{
	USB_Comm_MBIM_PROVIDER_CONTENTS;
	unsigned char	DataBuffer[1];
	};
/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_PROVIDER__SIZE				32


/****************************************************************************\
|
|	MBIM_CID_PREFERRED_PROVIDERS : section 10.5.7
|
\****************************************************************************/

#define USB_Comm_MBIM_PROVIDERS_CONTENTS			\
	unsigned char			ElementCount[4]

struct USB_Comm_MBIM_PROVIDERS_HEADER
	{
	USB_Comm_MBIM_PROVIDERS_CONTENTS;
	};

struct USB_Comm_MBIM_PROVIDERS
	{
	USB_Comm_MBIM_PROVIDERS_CONTENTS;
	struct USB_Comm_OL_PAIR_LIST 	ProvidersRefList[1];
	};

#define USB_Comm_MBIM_PROVIDERS_HEADER__SIZE			4

/*
|| Define length of structure,
|| but do not include DataBuffer
*/
#define USB_Comm_MBIM_PROVIDERS__SIZE(ARG_ElementCount)		\
	((ARG_ElementCount) * USB_Comm_OL_PAIR_LIST__SIZE + 	\
	USB_Comm_MBIM_PROVIDERS_HEADER__SIZE)

/*
|| Get DataBuffer from USB_Comm_MBIM_PROVIDERS
*/
#define USB_Comm_MBIM_PROVIDERS_DATABUFFER(			\
	ARG_pProviders, 					\
	ARG_ElementCount)					\
	((ARG_pProviders)->ElementCount +			\
	USB_Comm_MBIM_SUBSCRIBER_READY_INFO__SIZE(ARG_ElementCount))

/****************************************************************************\
|
|	MBIM_CID_VISIBLE_PROVIDERS : section 10.5.8
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_VISIBLE_PROVIDERS_ACTION	Table 10-41
\****************************************************************************/

#define USB_Comm_MBIM_VisibleProvidersActionFullScan		0
#define USB_Comm_MBIM_VisibleProvidersActionRestrictedScan	1

struct USB_Comm_MBIM_VISIBLE_PROVIDERS_REQ
	{
	unsigned char	Action[4];
	};

#define USB_Comm_MBIM_VISIBLE_PROVIDERS_REQ__SIZE		(4)

/****************************************************************************\
|
|	MBIM_CID_REGISTER_STATE : section 10.5.9
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_REGISTER_ACTION	Table 10-45
\****************************************************************************/
#define USB_Comm_MBIM_RegisterActionAutomatic			0
#define USB_Comm_MBIM_RegisterActionManual			1

/****************************************************************************\
|	MBIM_REGISTER_STATE	Table 10-46
\****************************************************************************/
#define USB_Comm_MBIM_RegisterStateUnknown			0
#define USB_Comm_MBIM_RegisterStateDeregistered			1
#define USB_Comm_MBIM_RegisterStateSearching			2
#define USB_Comm_MBIM_RegisterStateHome				3
#define USB_Comm_MBIM_RegisterStateRoaming			4
#define USB_Comm_MBIM_RegisterStatePartner			5
#define USB_Comm_MBIM_RegisterStateDenied			6

/****************************************************************************\
|	MBIM_REGISTER_MODE	Table 10-47
\****************************************************************************/
#define USB_Comm_MBIM_RegisterModeUnknown			0
#define USB_Comm_MBIM_RegisterModeAutomatic			1
#define USB_Comm_MBIM_RegisterModeManual			2

/****************************************************************************\
|	MBIM_REGISTRATION_FLAGS	Table 10-48
\****************************************************************************/
#define USB_Comm_MBIM_RegistrationNone				0x00
#define USB_Comm_MBIM_RegistrationManualSelectionNotAvailable	0x01
#define USB_Comm_MBIM_RegistrationPacketSeviceAutomaticAttach	0x02

#define USB_Comm_MBIM_SET_REGISTRATION_STATE_CONTENTS		\
	unsigned char	ProviderIdOffset[4];			\
	unsigned char	ProviderIdSize[4];			\
	unsigned char	RegisterAction[4];			\
	unsigned char	DataClass[4]			/* no trailing semicolon */

struct USB_Comm_MBIM_SET_REGISTRATION_STATE_HEADER
	{
	USB_Comm_MBIM_SET_REGISTRATION_STATE_CONTENTS;
	};

struct USB_Comm_MBIM_SET_REGISTRATION_STATE
	{
	USB_Comm_MBIM_SET_REGISTRATION_STATE_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SET_REGISTRATION_STATE__SIZE		16

#define USB_Comm_MBIM_REGISTRATION_STATE_INFO_CONTENTS		\
	unsigned char	NwError[4];				\
	unsigned char	RegisterState[4];			\
	unsigned char	RegisterMode[4];			\
	unsigned char	AvailableDataClasses[4];		\
	unsigned char	CurrentCellularClass[4];		\
	unsigned char	ProviderIdOffset[4];			\
	unsigned char	ProviderIdSize[4];			\
	unsigned char	ProviderNameOffset[4];			\
	unsigned char	ProviderNameSize[4];			\
	unsigned char	RoamingTextOffset[4];			\
	unsigned char	RoamingTextSize[4];			\
	unsigned char	RegistrationFlag[4]		/* no trailing semicolon */


struct USB_Comm_MBIM_REGISTRATION_STATE_INFO_HEADER
	{
	USB_Comm_MBIM_REGISTRATION_STATE_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_REGISTRATION_STATE_INFO
	{
	USB_Comm_MBIM_REGISTRATION_STATE_INFO_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_REGISTRATION_STATE_INFO__SIZE		48

/****************************************************************************\
|
|	MBIM_CID_PACKET_SERVICE : section 10.5.10
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_PACKET_SERVICE_ACTION	Table 10-52
\****************************************************************************/
#define USB_Comm_MBIM_PacketServiceActionAttach			0
#define USB_Comm_MBIM_PacketServiceActionDetach			1

/****************************************************************************\
|	MBIM_PACKET_SERVICE_STATE	Table 10-53
\****************************************************************************/
#define USB_Comm_MBIM_PacketServiceStateUnknown			0
#define USB_Comm_MBIM_PacketServiceStateAttaching		1
#define USB_Comm_MBIM_PacketServiceStateAttached		2
#define USB_Comm_MBIM_PacketServiceStateDetaching		3
#define USB_Comm_MBIM_PacketServiceStateDetached		4

struct USB_Comm_MBIM_SET_PACKET_SERVICE
	{
	unsigned char	PacketServiceAction[4];
	};

#define USB_Comm_MBIM_SET_PACKET_SERVICE__SIZE			(4)

struct USB_Comm_MBIM_PACKET_SERVICE_INFO
	{
	unsigned char	NwError[4];
	unsigned char	PacketServiceState[4];
	unsigned char	HighestAvailableDataClass[4];
	unsigned char	UpLinkSpeed[8];
	unsigned char	DownLinkSpeed[8];
	};

#define USB_Comm_MBIM_PACKET_SERVICE_INFO__SIZE			((3 * 4) + (2 * 8))

/****************************************************************************\
|
|	MBIM_CID_SIGNAL_STATE : section 10.5.11
|
\****************************************************************************/

struct USB_Comm_MBIM_SET_SIGNAL_STATE
	{
	unsigned char	SignalStrengthInterval[4];
	unsigned char	RssiThreshold[4];
	unsigned char	ErrorRateThreshold[4];
	};

#define USB_Comm_MBIM_SET_SIGNAL_STATE__SIZE			(3 * 4)

struct USB_Comm_MBIM_SIGNAL_STATE_INFO
	{
	unsigned char	Rssi[4];
	unsigned char	ErrorRate[4];
	unsigned char	SignalStrengthInterval[4];
	unsigned char	RssiThreshold[4];
	unsigned char	ErrorRateThreshold[4];
	};

#define USB_Comm_MBIM_SIGNAL_STATE_INFO__SIZE			(5 * 4)

/****************************************************************************\
|
|	MBIM_CID_CONNECT : section 10.5.12
|
\****************************************************************************/

#define USB_Comm_MBIM_ActivationCommandDeactivate		0
#define USB_Comm_MBIM_ActivationCommandActivate			1

#define USB_Comm_MBIM_CompressionNone				0
#define USB_Comm_MBIM_CompressionEnable				1

#define USB_Comm_MBIM_AuthProtocolNone				0
#define USB_Comm_MBIM_AuthProtocolPap				1
#define USB_Comm_MBIM_AuthProtocolChap				2
#define USB_Comm_MBIM_AuthProtocolMsChapV2			3

#define USB_Comm_MBIM_ContextIPTypeDefault 			0
#define USB_Comm_MBIM_ContextIPTypeIPv4				1
#define USB_Comm_MBIM_ContextIPTypeIPv6				2
#define USB_Comm_MBIM_ContextIPTypeIPv4v6			3
#define USB_Comm_MBIM_ContextIPTypeIPv4AndIPv6			4

#define USB_Comm_MBIM_ActivationStateUnknown 			0
#define USB_Comm_MBIM_ActivationStateActivated  		1
#define USB_Comm_MBIM_ActivationStateActivating 		2
#define USB_Comm_MBIM_ActivationStateDeactivated 		3
#define USB_Comm_MBIM_ActivationStateDeactivating 		4

#define USB_Comm_MBIM_VoiceCallStateNone 			0
#define USB_Comm_MBIM_VoiceCallStateInProgress 			1
#define USB_Comm_MBIM_VoiceCallStateHangUp 			2

#define USB_Comm_MBIM_MaxAccessStringSize			200
#define USB_Comm_MBIM_MaxUserNameSize				510
#define USB_Comm_MBIM_MaxPasswordSize				510

/*
|| Mapping to UUIDs defined in MBIM 0.9 Table 10-63 MBIM_CONTEXT_TYPES
*/
#define USB_Comm_MBIM_ContextTypeNone 				0
#define USB_Comm_MBIM_ContextTypeInternet 			1
#define USB_Comm_MBIM_ContextTypeVpn 				2
#define USB_Comm_MBIM_ContextTypeVoice 				3
#define USB_Comm_MBIM_ContextTypeVideoShare 			4
#define USB_Comm_MBIM_ContextTypePurchase 			5
#define USB_Comm_MBIM_ContextTypeIMS 				6
#define USB_Comm_MBIM_ContextTypeMMS 				7
#define USB_Comm_MBIM_ContextTypeLocal	 			8

#define USB_Comm_MBIM_SET_CONNECT_CONTENTS			\
	unsigned char	SessionId[4];				\
	unsigned char	ActivationCommand[4];			\
	unsigned char	AccessStringOffset[4];			\
	unsigned char	AccessStringSize[4];			\
	unsigned char	UserNameOffset[4];			\
	unsigned char	UserNameSize[4];			\
	unsigned char	PasswordOffset[4];			\
	unsigned char	PasswordSize[4];			\
	unsigned char	Compression[4];				\
	unsigned char	AuthProtocol[4];			\
	unsigned char	IPType[4];				\
	unsigned char	ContextType[16]			/* no trailing semicolon */

struct USB_Comm_MBIM_SET_CONNECT_HEADER
	{
	USB_Comm_MBIM_SET_CONNECT_CONTENTS;
	};

struct USB_Comm_MBIM_SET_CONNECT
	{
	USB_Comm_MBIM_SET_CONNECT_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| Define length of structure, but do not include DataBuffer
*/
#define USB_Comm_MBIM_SET_CONNECT__SIZE				60

struct USB_Comm_MBIM_CONNECT_INFO /* Called MBIM_CONTEXT_STATE */
	{
	unsigned char	SessionId[4];
	unsigned char	ActivationState[4];
	unsigned char	VoiceCallState[4];
	unsigned char	IPType[4];
	unsigned char	ContextType[16];
	unsigned char	uNwError[4];
	};

#define USB_Comm_MBIM_CONNECT_INFO__SIZE			(5 * 4 + 16)

#define USB_Comm_MBIM_ActivationCommandDeactivate		0
#define USB_Comm_MBIM_ActivationCommandActivate			1

/****************************************************************************\
|
|	MBIM_CID_PROVISIONED_CONTEXTS : section 10.5.13
|
\****************************************************************************/

#define USB_Comm_MBIM_CONTEXT_CONTENTS				\
	unsigned char	ContexId[4];				\
	unsigned char	ContextType[16];			\
	unsigned char	AccessStringOffset[4];			\
	unsigned char	AccessStringSize[4];			\
	unsigned char	UserNameOffset[4];			\
	unsigned char	UserNameSize[4];			\
	unsigned char	PasswordOffset[4];			\
	unsigned char	PasswordSize[4];			\
	unsigned char	Compression[4];				\
	unsigned char	AuthProtocol[4]			/* no trailing semicolon */

struct USB_Comm_MBIM_CONTEXT_HEADER
	{
	USB_Comm_MBIM_CONTEXT_CONTENTS;
	};

struct USB_Comm_MBIM_CONTEXT
	{
	USB_Comm_MBIM_CONTEXT_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_CONTEXT__SIZE		52

#define USB_Comm_MBIM_SET_PROVISIONED_CONTEXT_CONTENTS		\
	unsigned char	ContexId[4];				\
	unsigned char	ContextType[16];			\
	unsigned char	AccessStringOffset[4];			\
	unsigned char	AccessStringSize[4];			\
	unsigned char	UserNameOffset[4];			\
	unsigned char	UserNameSize[4];			\
	unsigned char	PasswordOffset[4];			\
	unsigned char	PasswordSize[4];			\
	unsigned char	Compression[4];				\
	unsigned char	AuthProtocol[4];			\
	unsigned char	ProvideridOffset[4];			\
	unsigned char	ProvideridSize[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_SET_PROVISIONED_CONTEXT_HEADER
	{
	USB_Comm_MBIM_SET_PROVISIONED_CONTEXT_CONTENTS;
	};

struct USB_Comm_MBIM_SET_PROVISIONED_CONTEXT
	{
	USB_Comm_MBIM_SET_PROVISIONED_CONTEXT_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SET_PROVISIONED_CONTEXT__SIZE	60

#define USB_Comm_MBIM_PROVISIONED_CONTEXTS_INFO_CONTENTS	\
	unsigned char			ElementCount[4]

struct USB_Comm_MBIM_PROVISIONED_CONTEXTS_INFO_HEADER
	{
	USB_Comm_MBIM_PROVISIONED_CONTEXTS_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_PROVISIONED_CONTEXTS_INFO
	{
	USB_Comm_MBIM_PROVISIONED_CONTEXTS_INFO_CONTENTS;
	struct USB_Comm_OL_PAIR_LIST	ProvisionedContextRefList[1];
	};

#define USB_Comm_MBIM_PROVISIONED_CONTEXTS_INFO_HEADER__SIZE	4

/*
|| Define length of structure,
|| but do not include DataBuffer
*/
#define USB_Comm_MBIM_PROVISIONED_CONTEXTS_INFO__SIZE(ARG_ElementCount)	\
	((ARG_ElementCount) * USB_Comm_OL_PAIR_LIST__SIZE + 		\
	USB_Comm_MBIM_PROVISIONED_CONTEXTS_INFO_HEADER__SIZE)

/*
|| Get DataBuffer from USB_Comm_MBIM_PROVISIONED_CONTEXTS_INFO
*/
#define USB_Comm_MBIM_PROVISIONED_CONTEXTS_INFO_DATABUFFER(		\
	ARG_pProvisionContextInfo, 					\
	ARG_ElementCount)						\
	((ARG_pProvisionContextInfo)->ElementCount +			\
	USB_Comm_MBIM_SUBSCRIBER_READY_INFO__SIZE(ARG_ElementCount))

/****************************************************************************\
|
|	MBIM_CID_SERVICE_ACTIVATION : section 10.5.14
|
\****************************************************************************/

/****************************************************************************\
|
|	MBIM_CID_SMS_CONFIGURATION : section 10.5.15
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_SMS_STORAGE_STATE	Table 10-77
\****************************************************************************/
#define USB_Comm_MBIM_SmsStorageNotInitialized			0
#define USB_Comm_MBIM_SmsStorageInitialized			1

/****************************************************************************\
|	MBIM_SMS_FORMAT	Table 10-78
\****************************************************************************/
#define USB_Comm_MBIM_SmsFormatPdu				0
#define USB_Comm_MBIM_SmsFormatCdma				1

#define USB_Comm_MBIM_SET_SMS_CONFIGURATION_CONTENTS		\
	unsigned char	Format[4];				\
	unsigned char	ScAddressOffset[4];			\
	unsigned char	ScAddressSize[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_SET_SMS_CONFIGURATION_HEADER
	{
	USB_Comm_MBIM_SET_SMS_CONFIGURATION_CONTENTS;
	};

struct USB_Comm_MBIM_SET_SMS_CONFIGURATION
	{
	USB_Comm_MBIM_SET_SMS_CONFIGURATION_CONTENTS;
	unsigned char	DataBuffer[1];
	};
/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SET_SMS_CONFIGURATION__SIZE		12

#define USB_Comm_MBIM_SMS_CONFIGURATION_INFO_CONTENTS		\
	unsigned char	SmsStorageState[4];			\
	unsigned char	Format[4];				\
	unsigned char	MaxMessages[4];				\
	unsigned char	CdmaShortMessageSize[4];		\
	unsigned char	ScAddressOffset[4];			\
	unsigned char	ScAddressSize[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_SMS_CONFIGURATION_INFO_HEADER
	{
	USB_Comm_MBIM_SMS_CONFIGURATION_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_SMS_CONFIGURATION_INFO
	{
	USB_Comm_MBIM_SMS_CONFIGURATION_INFO_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SMS_CONFIGURATION_INFO__SIZE		24

/****************************************************************************\
|
|	MBIM_CID_SMS_READ : section 10.5.16
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_SMS_FLAG	Table 10-82
\****************************************************************************/
#define USB_Comm_MBIM_SmsFlagAll				0
#define USB_Comm_MBIM_SmsFlagIndex				1
#define USB_Comm_MBIM_SmsFlagNew				2
#define USB_Comm_MBIM_SmsFlagOld				3
#define USB_Comm_MBIM_SmsFlagSent				4
#define USB_Comm_MBIM_SmsFlagDraft				5

/****************************************************************************\
|	MBIM_SMS_CDMA_LANG	Table 10-83
\****************************************************************************/
#define USB_Comm_MBIM_SmsCdmaLangUnknown			0
#define USB_Comm_MBIM_SmsCdmaLangEnglish			1
#define USB_Comm_MBIM_SmsCdmaLangFrench				2
#define USB_Comm_MBIM_SmsCdmaLangSpanish			3
#define USB_Comm_MBIM_SmsCdmaLangJapanese			4
#define USB_Comm_MBIM_SmsCdmaLangKorean				5
#define USB_Comm_MBIM_SmsCdmaLangChinese			6
#define USB_Comm_MBIM_SmsCdmaLangHebrew				7

/****************************************************************************\
|	MBIM_SMS_CDMA_ENCODING	Table 10-84
\****************************************************************************/
#define USB_Comm_MBIM_SmsCdmaEncodingOctet			0
#define USB_Comm_MBIM_SmsCdmaEncodingEpm			1
#define USB_Comm_MBIM_SmsCdmaEncoding7BitAscii			2
#define USB_Comm_MBIM_SmsCdmaEncodingla5			3
#define USB_Comm_MBIM_SmsCdmaEncodingUnicode			4
#define USB_Comm_MBIM_SmsCdmaEncodingShiftJis			5
#define USB_Comm_MBIM_SmsCdmaEncodingKorean			6
#define USB_Comm_MBIM_SmsCdmaEncodingLatinHebrew		7
#define USB_Comm_MBIM_SmsCdmaEncodingLatin			8
#define USB_Comm_MBIM_SmsCdmaEncodingGsm7Bit			9

/****************************************************************************\
|	MBIM_SMS_MESSAGE_STATUS	Table 10-85
\****************************************************************************/
#define USB_Comm_MBIM_SmsStatusNew				0
#define USB_Comm_MBIM_SmsStatusOld				1
#define USB_Comm_MBIM_SmsStatusDraft				2
#define USB_Comm_MBIM_SmsStatusSent				3

#define USB_Comm_MBIM_SMS_PDU_RECORD_CONTENTS			\
	unsigned char	MessageIndex[4];			\
	unsigned char	MessageStatus[4];			\
	unsigned char	PduDataOffset[4];			\
	unsigned char	PduDataSize[4]			/* no trailing semicolon */

struct USB_Comm_MBIM_SMS_PDU_RECORD_HEADER
	{
	USB_Comm_MBIM_SMS_PDU_RECORD_CONTENTS;
	};

struct USB_Comm_MBIM_SMS_PDU_RECORD
	{
	USB_Comm_MBIM_SMS_PDU_RECORD_CONTENTS;
	unsigned char	DataBuffer[1];
	};
/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SMS_PDU_RECORD__SIZE			16

#define USB_Comm_MBIM_SMS_CDMA_RECORD_CONTENTS			\
	unsigned char	MessageIndex[4];			\
	unsigned char	MessageStatus[4];			\
	unsigned char	AddressOffset[4];			\
	unsigned char	AddressSize[4];				\
	unsigned char	TimeStampOffset[4];			\
	unsigned char	TimeStampSize[4];			\
	unsigned char	EncodingId[4];				\
	unsigned char	LanguageId[4];				\
	unsigned char	EncodedMessageOffset[4];		\
	unsigned char	SizeInBytes[4];				\
	unsigned char	SizeInCharacters[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_SMS_CDMA_RECORD_HEADER
	{
	USB_Comm_MBIM_SMS_CDMA_RECORD_CONTENTS;
	};

struct USB_Comm_MBIM_SMS_CDMA_RECORD
	{
	USB_Comm_MBIM_SMS_CDMA_RECORD_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SMS_CDMA_RECORD__SIZE			44

struct USB_Comm_MBIM_SMS_READ_REQ
	{
	unsigned char	SmsFormat[4];
	unsigned char	Flag[4];
	unsigned char	MessageIndex[4];
	};

#define USB_Comm_MBIM_SMS_READ_REQ__SIZE			(3 * 4)

#define USB_Comm_MBIM_SMS_READ_INFO_CONTENTS			\
	unsigned char			Format[4];		\
	unsigned char			ElementCount[4]

struct USB_Comm_MBIM_SMS_READ_INFO_HEADER
	{
	USB_Comm_MBIM_SMS_READ_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_SMS_READ_INFO
	{
	USB_Comm_MBIM_SMS_READ_INFO_CONTENTS;
	struct USB_Comm_OL_PAIR_LIST	SmsRefList[1];
	};

#define USB_Comm_MBIM_SMS_READ_INFO_HEADER__SIZE	8

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SMS_READ_INFO__SIZE(ARG_ElementCount)	\
	((ARG_ElementCount) * USB_Comm_OL_PAIR_LIST__SIZE + 	\
	USB_Comm_MBIM_SMS_READ_INFO_HEADER__SIZE)


/*
|| Get DataBuffer from USB_Comm_MBIM_SMS_READ_INFO
*/
#define USB_Comm_MBIM_SMS_READ_INFO_DATABUFFER(			\
	ARG_pSMSReadInfo,					\
	ARG_ElementCount)					\
	((ARG_pSMSReadInfo)->Format +				\
	USB_Comm_MBIM_SMS_READ_INFO__SIZE(ARG_ElementCount))

/****************************************************************************\
|
|	MBIM_CID_SMS_SEND : section 10.5.17
|
\****************************************************************************/

#define USB_Comm_MBIM_SMS_SEND_PDU_CONTENTS			\
	unsigned char	PduDataOffset[4];			\
	unsigned char	PduDataSize[4]			/* no trailing semicolon */

struct USB_Comm_MBIM_SMS_SEND_PDU_HEADER
	{
	USB_Comm_MBIM_SMS_SEND_PDU_CONTENTS;
	};

struct USB_Comm_MBIM_SMS_SEND_PDU
	{
	USB_Comm_MBIM_SMS_SEND_PDU_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SMS_SEND_PDU__SIZE			8

#define USB_Comm_MBIM_SMS_SEND_CDMA_CONTENTS			\
	unsigned char	EncodingId[4];				\
	unsigned char	LanguageId[4];				\
	unsigned char	AddressOffset[4];			\
	unsigned char	AddressSize[4];				\
	unsigned char	EncodedMessageOffset[4];		\
	unsigned char	SizeInBytes[4];				\
	unsigned char	SizeInCharacters[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_SMS_SEND_CDMA_HEADER
	{
	USB_Comm_MBIM_SMS_SEND_CDMA_CONTENTS;
	};

struct USB_Comm_MBIM_SMS_SEND_CDMA
	{
	USB_Comm_MBIM_SMS_SEND_CDMA_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SMS_SEND_CDMA__SIZE			28

#define USB_Comm_MBIM_SET_SMS_SEND_CONTENTS			\
	unsigned char	SmsFormat[4]

struct USB_Comm_MBIM_SET_SMS_SEND_HEADER
	{
	USB_Comm_MBIM_SET_SMS_SEND_CONTENTS;
	};

struct USB_Comm_MBIM_SET_SMS_SEND
	{
	USB_Comm_MBIM_SET_SMS_SEND_CONTENTS;
	unsigned char	DataBuffer[1];
	};
/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SET_SMS_SEND__SIZE			4

struct USB_Comm_MBIM_SMS_SEND_INFO
	{
	unsigned char	MessageReference[4];
	};

#define USB_Comm_MBIM_SMS_SEND_INFO__SIZE			4

/****************************************************************************\
|
|	MBIM_CID_SMS_DELETE : section 10.5.18
|
\****************************************************************************/

struct USB_Comm_MBIM_SET_SMS_DELETE
	{
	unsigned char	Flags[4];
	unsigned char	MessageIndex[4];
	};

#define USB_Comm_MBIM_SET_SMS_DELETE__SIZE			(2 * 4)

/****************************************************************************\
|
|	MBIM_CID_SMS_MESSAGE_STORE_STATUS : section 10.5.19
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_SMS_STATUS_FLAGS	Table 10-98
\****************************************************************************/
#define USB_Comm_MBIM_FlagNone					0
#define USB_Comm_MBIM_FlagMessageStoreFull			1
#define USB_Comm_MBIM_FlagNewMessage				2

struct USB_Comm_MBIM_SMS_STATUS_INFO
	{
	unsigned char	Flag[4];
	unsigned char	MessageIndex[4];
	};

#define USB_Comm_MBIM_SMS_STATUS_INFO__SIZE			(2 * 4)

/****************************************************************************\
|
|	MBIM_CID_IP_CONFIGURATION : section 10.5.20
|
\****************************************************************************/

struct USB_Comm_MBIM_IPV4_ADDRESS
	{
	unsigned char	IPv4Address[4];
	};

#define USB_Comm_MBIM_IPV4_ADDRESS__SIZE			(4)

struct USB_Comm_MBIM_IPV4_ELEMENT
	{
	unsigned char	OnLinkPrefixLength[4];
	unsigned char	IPv4Address[4];
	};

#define USB_Comm_MBIM_IPV4_ELEMENT__SIZE			(2 * 4)

struct USB_Comm_MBIM_IPV6_ADDRESS
	{
	unsigned char	IPv6Address[16];
	};

#define USB_Comm_MBIM_IPV6_ADDRESS__SIZE			(16)

struct USB_Comm_MBIM_IPV6_ELEMENT
	{
	unsigned char	OnLinkPrefixLength[4];
	unsigned char	IPv6Address[16];
	};

#define USB_Comm_MBIM_IPV6_ELEMENT__SIZE			(4 + 16)

#define USB_Comm_MBIM_IP_CONFIGURATION_INFO_CONTENTS		\
	unsigned char	SessionId[4];				\
	unsigned char	IPv4ConfigurationAvailable[4];		\
	unsigned char	IPv6ConfigurationAvailable[4];		\
	unsigned char	IPv4AddressCount[4];			\
	unsigned char	IPv4AddressOffset[4];			\
	unsigned char	IPv6AddressCount[4];			\
	unsigned char	IPv6AddressOffset[4];			\
	unsigned char	IPv4GatewayOffset[4];			\
	unsigned char	IPv6GatewayOffset[4];			\
	unsigned char	IPv4DnsServerCount[4];			\
	unsigned char	IPv4DnsServerOffset[4];			\
	unsigned char	IPv6DnsServerCount[4];			\
	unsigned char	IPv6DnsServerOffset[4];			\
	unsigned char	IPv4Mtu[4];				\
	unsigned char	IPv6Mtu[4]			/* no trailing semicolon */

struct USB_Comm_MBIM_IP_CONFIGURATION_INFO_HEADER
	{
	USB_Comm_MBIM_IP_CONFIGURATION_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_IP_CONFIGURATION_INFO
	{
	USB_Comm_MBIM_IP_CONFIGURATION_INFO_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_IP_CONFIGURATION_INFO__SIZE		60

/****************************************************************************\
|
|	MBIM_CID_USSD : section 10.5.21
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_USSD_ACTION	Table 10-107
\****************************************************************************/
#define USB_Comm_MBIM_USSDInitiate				0
#define USB_Comm_MBIM_USSDContinue				1
#define USB_Comm_MBIM_USSDCancel				2

/****************************************************************************\
|	MBIM_USSD_RESPONSE	Table 10-108
\****************************************************************************/
#define USB_Comm_MBIM_USSDNoActionRequired			0
#define USB_Comm_MBIM_USSDActionRequired			1
#define USB_Comm_MBIM_USSDTerminatedByNW			2
#define USB_Comm_MBIM_USSDOtherLocalClient			3
#define USB_Comm_MBIM_USSDOperationNotSupported			4
#define USB_Comm_MBIM_USSDNetworkTimeOut			5

/****************************************************************************\
|	MBIM_USSD_SESSION_STATE	Table 10-109
\****************************************************************************/
#define USB_Comm_MBIM_USSDNewSession				0
#define USB_Comm_MBIM_USSDExistingSession			1

#define USB_Comm_MBIM_SET_USSD_CONTENTS				\
	unsigned char	USSDAction[4];				\
	unsigned char	USSDDataCodingScheme[4];		\
	unsigned char	USSDPayloadOffset[4];			\
	unsigned char	USSDPayloadLength[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_SET_USSD_HEADER
	{
	USB_Comm_MBIM_SET_USSD_CONTENTS;
	};

struct USB_Comm_MBIM_SET_USSD
	{
	USB_Comm_MBIM_SET_USSD_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SET_USSD__SIZE				16

#define USB_Comm_MBIM_USSD_INFO_CONTENTS			\
	unsigned char	USSDResponse[4];			\
	unsigned char	USSDSessionState[4];			\
	unsigned char	USSDDataCodingScheme[4];		\
	unsigned char	USSDPayloadOffset[4];			\
	unsigned char	USSDPayloadLength[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_USSD_INFO_HEADER
	{
	USB_Comm_MBIM_USSD_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_USSD_INFO
	{
	USB_Comm_MBIM_USSD_INFO_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_USSD_INFO__SIZE				20

/****************************************************************************\
|
|	MBIM_CID_PHONEBOOK_CONFIGURATION : section 10.5.22
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_PHONEBOOK_STATE	Table 10-113
\****************************************************************************/
#define USB_Comm_MBIM_PhonebookNotInitialized			0
#define USB_Comm_MBIM_PhonebookInitialized			1

struct USB_Comm_MBIM_PHONEBOOK_CONFIGURATION_INFO
	{
	unsigned char	PhonebookState[4];
	unsigned char	TotalNbrOfEntries[4];
	unsigned char	UsedEntries[4];
	unsigned char	MaxNumberLength[4];
	unsigned char	MaxNameLength[4];
	};

#define USB_Comm_MBIM_PHONEBOOK_CONFIGURATION_INFO__SIZE	(5 * 4)

/****************************************************************************\
|
|	MBIM_CID_PHONEBOOK_READ : section 10.5.23
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_PHONEBOOK_FLAG	Table 10-116
\****************************************************************************/
#define USB_Comm_MBIM_PhonebookFlagAll				0
#define USB_Comm_MBIM_PhonebookFlagIndex			1

#define USB_Comm_MBIM_PHONEBOOK_ENTRY_CONTENTS			\
	unsigned char	EntryIndex[4];				\
	unsigned char	NumberOffset[4];			\
	unsigned char	NumberLength[4];			\
	unsigned char	NameOffset[4];				\
	unsigned char	NameLength[4]			/* no trailing semicolon */

struct USB_Comm_MBIM_PHONEBOOK_ENTRY_HEADER
	{
	USB_Comm_MBIM_PHONEBOOK_ENTRY_CONTENTS;
	};

struct USB_Comm_MBIM_PHONEBOOK_ENTRY
	{
	USB_Comm_MBIM_PHONEBOOK_ENTRY_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_PHONEBOOK_ENTRY__SIZE			20

struct USB_Comm_MBIM_PHONEBOOK_READ_REQ
	{
	unsigned char	FilterFlag[4];
	unsigned char	FilterMessageIndex[4];
	};

#define USB_Comm_MBIM_PHONEBOOK_READ_REQ__SIZE			(2 * 4)

#define USB_Comm_MBIM_PHONEBOOK_READ_INFO_CONTENTS		\
	unsigned char			ElementCount[4]

struct USB_Comm_MBIM_PHONEBOOK_READ_INFO_HEADER
	{
	USB_Comm_MBIM_PHONEBOOK_READ_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_PHONEBOOK_READ_INFO
	{
	USB_Comm_MBIM_PHONEBOOK_READ_INFO_CONTENTS;
	struct USB_Comm_OL_PAIR_LIST	PhoneBookRefList[1];
	};

#define USB_Comm_MBIM_PHONEBOOK_READ_INFO_HEADER__SIZE		4

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_PHONEBOOK_READ_INFO__SIZE(ARG_ElementCount)	\
	((ARG_ElementCount) * USB_Comm_OL_PAIR_LIST__SIZE + 		\
	USB_Comm_MBIM_PHONEBOOK_READ_INFO_HEADER__SIZE)

/*
|| Get DataBuffer from USB_Comm_MBIM_PHONEBOOK_READ_INFO
*/
#define USB_Comm_MBIM_PHONEBOOK_READ_INFO_DATABUFFER(			\
	ARG_pPhonebookReadInfo,						\
	ARG_ElementCount) 						\
	((ARG_pPhonebookReadInfo)->ElementCount +			\
	USB_Comm_MBIM_PHONEBOOK_READ_INFO__SIZE(ARG_ElementCount))

/****************************************************************************\
|
|	MBIM_CID_PHONEBOOK_DELETE : section 10.5.24
|
\****************************************************************************/

struct USB_Comm_MBIM_SET_PHONEBOOK_DELETE
	{
	unsigned char	FilterFlag[4];
	unsigned char	FilterMessageIndex[4];
	};

#define USB_Comm_MBIM_SET_PHONEBOOK_DELETE__SIZE		(2 * 4)

/****************************************************************************\
|
|	MBIM_CID_PHONEBOOK_WRITE : section 10.5.25
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_PHONEBOOK_WRITE_FLAG	Table 10-123
\****************************************************************************/
#define USB_Comm_MBIM_PhonebookFlagSaveUnused			0
#define USB_Comm_MBIM_PhonebookFlagSaveIndex			1

#define USB_Comm_MBIM_SET_PHONEBOOK_WRITE_CONTENTS		\
	unsigned char	SaveFlag[4];				\
	unsigned char	SaveIndex[4];				\
	unsigned char	NumberOffset[4];			\
	unsigned char	NumberLength[4];			\
	unsigned char	NameOffset[4];				\
	unsigned char	NameLength[4]			/* no trailing semicolon */

struct USB_Comm_MBIM_SET_PHONEBOOK_WRITE_HEADER
	{
	USB_Comm_MBIM_SET_PHONEBOOK_WRITE_CONTENTS;
	};

struct USB_Comm_MBIM_SET_PHONEBOOK_WRITE
	{
	USB_Comm_MBIM_SET_PHONEBOOK_WRITE_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SET_PHONEBOOK_WRITE__SIZE			24

/****************************************************************************\
|
|	MBIM_CID_STK_PAC : section 10.5.26
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_STK_PAC_TYPE	Table 10-127
\****************************************************************************/
#define USB_Comm_MBIM_STKProactiveCommand			0
#define USB_Comm_MBIM_STKNotification				1

/****************************************************************************\
|	MBIM_STK_PAC_PROFILE	Table 10-126
\****************************************************************************/
#define USB_Comm_MBIM_STKNotHandledByFunctionCannotBeHandledByHost	0
#define USB_Comm_MBIM_STKNotHandledByFunctionMayBeHandledByHost		1
#define USB_Comm_MBIM_STKHandledByFunctionOnlyTransparentToHost		2
#define USB_Comm_MBIM_STKHandledByFunctionNotificationToHostPossible	3
#define USB_Comm_MBIM_STKHandledByFunctionNotificationToHostEnabled	4
#define USB_Comm_MBIM_STKHandledByFunctionCanBeOverriddenByHost		5
#define USB_Comm_MBIM_STKHandledByHostFunctionNotAbleToHandle		6
#define USB_Comm_MBIM_STKHandledByHostFunctionAbleToHandle		7

struct USB_Comm_MBIM_SET_STK_PAC
	{
	unsigned char	PacHostControl[32];
	};

#define USB_Comm_MBIM_SET_STK_PAC__SIZE				(32)

struct USB_Comm_MBIM_STK_PAC_INFO
	{
	unsigned char	PacSupport[256];
	};

#define USB_Comm_MBIM_STK_PAC_INFO__SIZE			(256)

#define USB_Comm_MBIM_STK_PAC_CONTENTS 				\
	unsigned char	PacType[4]

struct USB_Comm_MBIM_STK_PAC_HEADER
	{
	USB_Comm_MBIM_STK_PAC_CONTENTS;
	};

struct USB_Comm_MBIM_STK_PAC
	{
	USB_Comm_MBIM_STK_PAC_CONTENTS;
	unsigned char	DataBuffer[1];
	};
/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_STK_PAC__SIZE				4

/****************************************************************************\
|
|	MBIM_CID_STK_TERMINAL_RESPONSE : section 10.5.27
|
\****************************************************************************/

#define USB_Comm_MBIM_SET_STK_TERMINAL_RESPONSE_CONTENTS	\
	unsigned char	ResponseLength[4]

struct USB_Comm_MBIM_SET_STK_TERMINAL_RESPONSE_HEADER
	{
	USB_Comm_MBIM_SET_STK_TERMINAL_RESPONSE_CONTENTS;
	};

struct USB_Comm_MBIM_SET_STK_TERMINAL_RESPONSE
	{
	USB_Comm_MBIM_SET_STK_TERMINAL_RESPONSE_CONTENTS;
	unsigned char	DataBuffer[1];
	};
/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SET_STK_TERMINAL_RESPONSE__SIZE		4

#define USB_Comm_MBIM_STK_TERMINAL_RESPONSE_INFO_CONTENTS	\
	unsigned char	ResultDataStringOffset[4];		\
	unsigned char	ResultLength[4];			\
	unsigned char	StatusWords[4]

struct USB_Comm_MBIM_STK_TERMINAL_RESPONSE_INFO_HEADER
	{
	USB_Comm_MBIM_STK_TERMINAL_RESPONSE_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_STK_TERMINAL_RESPONSE_INFO
	{
	USB_Comm_MBIM_STK_TERMINAL_RESPONSE_INFO_CONTENTS;
	unsigned char	DataBuffer[1];
	};

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_STK_TERMINAL_RESPONSE_INFO__SIZE		12

/****************************************************************************\
|
|	MBIM_CID_STK_ENVELOPE : section 10.5.28
|
\****************************************************************************/

struct USB_Comm_MBIM_SET_STK_ENVELOPE
	{
	unsigned char	DataBuffer[1];
	};

struct USB_Comm_MBIM_STK_ENVELOPE_INFO
	{
	unsigned char	EnvelopeSupport[32];
	};

#define USB_Comm_MBIM_STK_ENVELOPE_INFO__SIZE			(32)

/****************************************************************************\
|
|	MBIM_CID_DEVICE_SERVICES : section 10.5.29
|
\****************************************************************************/

#define USB_Comm_MBIM_DEVICE_SERVICE_ELEMENT_CONTENTS		\
	unsigned char	DeviceServiceId[16];			\
	unsigned char	DSSPayload[4];				\
	unsigned char	MaxDSSInstances[4];			\
	unsigned char	CidCount[4]			/* no trailing semicolon */

struct USB_Comm_MBIM_DEVICE_SERVICE_ELEMENT_HEADER
	{
	USB_Comm_MBIM_DEVICE_SERVICE_ELEMENT_CONTENTS;
	};

struct USB_Comm_MBIM_DEVICE_SERVICE_ELEMENT
	{
	USB_Comm_MBIM_DEVICE_SERVICE_ELEMENT_CONTENTS;
	unsigned char	DataBuffer[1];
	};
/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_DEVICE_SERVICE_ELEMENT__SIZE		28

#define USB_Comm_MBIM_DEVICE_SERVICES_INFO_CONTENTS		\
	unsigned char			DeviceServiceCount[4];	\
	unsigned char			MaxDSSSessions[4]

struct USB_Comm_MBIM_DEVICE_SERVICES_INFO_HEADER
	{
	USB_Comm_MBIM_DEVICE_SERVICES_INFO_CONTENTS;
	};

struct USB_Comm_MBIM_DEVICE_SERVICES_INFO
	{
	USB_Comm_MBIM_DEVICE_SERVICES_INFO_CONTENTS;
	struct USB_Comm_OL_PAIR_LIST	DeviceServicesRefList[1];
	};

#define USB_Comm_MBIM_DEVICE_SERVICES_INFO_HEADER__SIZE		8

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_DEVICE_SERVICES_INFO__SIZE(ARG_DeviceServiceCount)	\
	((ARG_DeviceServiceCount) * USB_Comm_OL_PAIR_LIST__SIZE + 		\
	USB_Comm_MBIM_DEVICE_SERVICES_INFO_HEADER__SIZE)

/*
|| Get DataBuffer from USB_Comm_MBIM_DEVICE_SERVICES_INFO
*/
#define USB_Comm_MBIM_DEVICE_SERVICES_INFO_DATABUFFER(				\
	ARG_pDeviceServiceInfo,							\
	ARG_DeviceServiceCount) 						\
	((ARG_pDeviceServiceInfo)->DeviceServiceCount +				\
	USB_Comm_MBIM_DEVICE_SERVICES_INFO__SIZE(ARG_DeviceServiceCount)

/****************************************************************************\
|
|	MBIM_CID_DEVICE_SERVICE_SUBSCRIBE_LIST : section 10.5.30
|
\****************************************************************************/

#define USB_Comm_MBIM_EVENT_ENTRY_CONTENTS			\
	unsigned char	DeviceServiceId[16];			\
	unsigned char	CidCount[4]			/* no trailing semicolon */

struct USB_Comm_MBIM_EVENT_ENTRY_HEADER
	{
	USB_Comm_MBIM_EVENT_ENTRY_CONTENTS;
	};

struct USB_Comm_MBIM_EVENT_ENTRY
	{
	USB_Comm_MBIM_EVENT_ENTRY_CONTENTS;
	unsigned char	DataBuffer[1];
	};
/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_EVENT_ENTRY__SIZE				20

#define USB_Comm_MBIM_DEVICE_SERVICE_SUBSCRIBE_LIST_CONTENTS	\
	unsigned char			ElementCount[4]

struct USB_Comm_MBIM_DEVICE_SERVICE_SUBSCRIBE_LIST_HEADER
	{
	USB_Comm_MBIM_DEVICE_SERVICE_SUBSCRIBE_LIST_CONTENTS;
	};

struct USB_Comm_MBIM_DEVICE_SERVICE_SUBSCRIBE_LIST
	{
	USB_Comm_MBIM_DEVICE_SERVICE_SUBSCRIBE_LIST_CONTENTS;
	struct USB_Comm_OL_PAIR_LIST	DeviceServicesRefList[1];
	};

#define USB_Comm_MBIM_DEVICE_SERVICE_SUBSCRIBE_LIST_HEADER__SIZE	4

/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_DEVICE_SERVICE_SUBSCRIBE_LIST__SIZE(ARG_ElementCount)	\
	((ARG_ElementCount) * USB_Comm_OL_PAIR_LIST__SIZE + 			\
	USB_Comm_MBIM_DEVICE_SERVICE_SUBSCRIBE_LIST_HEADER__SIZE)

/*
|| Get DataBuffer from USB_Comm_MBIM_DEVICE_SERVICE_SUBSCRIBE_LIST
*/
#define USB_Comm_MBIM_DEVICE_SERVICE_SUBSCRIBE_LIST_DATABUFFER(			\
	ARG_pDeviceServiceSubscribeList,					\
	ARG_ElementCount) 							\
	((ARG_pDeviceServiceSubscribeList)->ElementCount +			\
	USB_Comm_MBIM_DEVICE_SERVICE_SUBSCRIBE_LIST__SIZE(ARG_ElementCount))

/****************************************************************************\
|
|	MBIM_CID_AKA_AUTH : section 10.5.31
|
\****************************************************************************/

struct USB_Comm_MBIM_AKA_AUTH_REQ
	{
	unsigned char	Rand[16];
	unsigned char	Autn[16];
	};

#define USB_Comm_MBIM_AKA_AUTH_REQ__SIZE			(2 * 16)

struct USB_Comm_MBIM_AKA_AUTH_INFO
	{
	unsigned char	Res[16];
	unsigned char	ResLen[4];
	unsigned char	IK[16];
	unsigned char	CK[16];
	unsigned char	Auts[14];
	};

#define USB_Comm_MBIM_AKA_AUTH_INFO__SIZE			(3 * 16 + 4 + 14)

/****************************************************************************\
|
|	MBIM_CID_AKAP_AUTH : section 10.5.32
|
\****************************************************************************/

#define USB_Comm_MBIM_AKAP_AUTH_REQ_CONTENTS			\
	unsigned char	Rand[16];				\
	unsigned char	Autn[16];				\
	unsigned char	NetworkNameOffset[4];			\
	unsigned char	NetworkNameLength[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_AKAP_AUTH_REQ_HEADER
	{
	USB_Comm_MBIM_AKAP_AUTH_REQ_CONTENTS;
	};

struct USB_Comm_MBIM_AKAP_AUTH_REQ
	{
	USB_Comm_MBIM_AKAP_AUTH_REQ_CONTENTS;
	unsigned char	DataBuffer[1];
	};
/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_AKAP_AUTH_REQ__SIZE			40

struct USB_Comm_MBIM_AKAP_AUTH_INFO
	{
	unsigned char	Res[16];
	unsigned char	ResLen[4];
	unsigned char	IK[16];
	unsigned char	CK[16];
	unsigned char	Auts[14];
	};

#define USB_Comm_MBIM_AKAP_AUTH_INFO__SIZE			(3 * 16 + 4 + 14)

/****************************************************************************\
|
|	MBIM_CID_SIM_AUTH : section 10.5.33
|
\****************************************************************************/

struct USB_Comm_MBIM_SIM_AUTH_REQ
	{
	unsigned char	Rand1[16];
	unsigned char	Rand2[16];
	unsigned char	Rand3[16];
	unsigned char	n[4];
	};

#define USB_Comm_MBIM_SIM_AUTH_REQ__SIZE			(3 * 16 + 4)

struct USB_Comm_MBIM_SIM_AUTH_INFO
	{
	unsigned char	Sres1[4];
	unsigned char	Kc1[8];
	unsigned char	Sres2[4];
	unsigned char	Kc2[8];
	unsigned char	Sres3[4];
	unsigned char	Kc3[8];
	unsigned char	n[4];
	};

#define USB_Comm_MBIM_SIM_AUTH_INFO__SIZE			(4 * 4 + 3 * 8)

/****************************************************************************\
|
|	MBIM_CID_PACKET_STATISTICS : section 10.5.34
|
\****************************************************************************/

struct USB_Comm_MBIM_PACKET_STATISTICS_INFO
	{
	unsigned char	InDiscards[4];
	unsigned char	InErrors[4];
	unsigned char	InOctets[8];
	unsigned char	InPackets[8];
	unsigned char	OutOctets[8];
	unsigned char	OutPackets[8];
	unsigned char	OutErrors[4];
	unsigned char	OutDiscards[4];
	};

#define USB_Comm_MBIM_PACKET_STATISTICS_INFO__SIZE		(4 * 4 + 4 * 8)

/****************************************************************************\
|
|	MBIM_CID_NETWORK_IDLE_HINT : section 10.5.35
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_NETWORK_IDLE_HINT_STATES	Table 10-155
\****************************************************************************/
#define USB_Comm_MBIM_NetworkIdleHintDisabled			0
#define USB_Comm_MBIM_NetworkIdleHintEnabled			1

struct USB_Comm_MBIM_NETWORK_IDLE_HINT
	{
	unsigned char	NetworkIdleHintState[4];
	};

#define USB_Comm_MBIM_NETWORK_IDLE_HINT__SIZE			(4)

/****************************************************************************\
|
|	MBIM_CID_EMERGENCY_MODE : section 10.5.36
|
\****************************************************************************/

/****************************************************************************\
|	MBIM_EMERGENCY_MODE_STATES	Table 10-158
\****************************************************************************/
#define USB_Comm_MBIM_EmergencyModeOff				0
#define USB_Comm_MBIM_EmergencyModeON				1

struct USB_Comm_MBIM_EMERGENCY_MODE_INFO
	{
	unsigned char	EmergencyMode[4];
	};

#define USB_Comm_MBIM_EMERGENCY_MODE_INFO__SIZE			(4)

/****************************************************************************\
|
|	MBIM_CID_IP_PACKET_FILTERS : section 10.5.37
|
\****************************************************************************/

#define USB_Comm_MBIM_SINGLE_PACKET_FILTER_CONTENTS		\
	unsigned char	FilterSize[4];				\
	unsigned char	PacketFilterOffset[4];			\
	unsigned char	PacketMaskOffset[4]		/* no trailing semicolon */

struct USB_Comm_MBIM_SINGLE_PACKET_FILTER_HEADER
	{
	USB_Comm_MBIM_SINGLE_PACKET_FILTER_CONTENTS;
	};

struct USB_Comm_MBIM_SINGLE_PACKET_FILTER
	{
	USB_Comm_MBIM_SINGLE_PACKET_FILTER_CONTENTS;
	unsigned char	DataBuffer[1];
	};
/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_SINGLE_PACKET_FILTER__SIZE		12

#define USB_Comm_MBIM_PACKET_FILTER_CONTENTS			\
	unsigned char			SessionId[4];		\
	unsigned char			PacketFiltersCount[4]

struct USB_Comm_MBIM_PACKET_FILTER_HEADER
	{
	USB_Comm_MBIM_PACKET_FILTER_CONTENTS;
	};

struct USB_Comm_MBIM_PACKET_FILTER
	{
	USB_Comm_MBIM_PACKET_FILTER_CONTENTS;
	struct USB_Comm_OL_PAIR_LIST	PacketFilterRefList[1];
	};


#define USB_Comm_MBIM_PACKET_FILTER_HEADER__SIZE		8
/*
|| DataBuffer is not included
*/
#define USB_Comm_MBIM_PACKET_FILTER__SIZE(ARG_PacketFiltersCount)	\
	((ARG_PacketFiltersCount) * USB_Comm_OL_PAIR_LIST__SIZE + 	\
	USB_Comm_MBIM_PACKET_FILTER_HEADER__SIZE)

/*
|| Get DataBuffer from USB_Comm_MBIM_PACKET_FILTER
*/
#define USB_Comm_MBIM_PACKET_FILTER_DATABUFFER(				\
	ARG_pPacketFilter,						\
	ARG_PacketFiltersCount)						\
	((ARG_pPacketFilter)->SessionId +				\
	USB_Comm_MBIM_PACKET_FILTER__SIZE(ARG_PacketFiltersCount))

/****************************************************************************\
|
|	MBIM_CID_DSS_CONNECT : section 10.5.38
|
\****************************************************************************/

#define USB_MBIM_DSSLinkDeActivate				0
#define USB_MBIM_DSSLinkActivate				1

struct USB_Comm_MBIM_SET_DSS_CONNECT
	{
	unsigned char	DeviceServiceId[16];
	unsigned char	DssSessionId[4];
	unsigned char	DssLinkState[4];
	struct USB_Comm_MBIM_SET_DSS_CONNECT *pNext;
	};

#define USB_Comm_MBIM_SET_DSSCONNECT__SIZE			(4 * 4 + 16)

/****************************************************************************\
|
|	MBIM_CID_MULTICARRIER_PROVIDERS : section 10.5.39
|
\****************************************************************************/


/**** end of usbmbim10.h ****/
#endif /* _USBMBIM10_H_ */
