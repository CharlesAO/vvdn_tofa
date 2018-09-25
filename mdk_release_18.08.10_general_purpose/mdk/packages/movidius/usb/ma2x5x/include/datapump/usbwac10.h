/* usbwac10.h	Mon Jul 14 2008 13:29:36 wkkim */

/*

Module:  usbwac10.h

Function:
	WUSB Wire Adapter Class V1.0 constants, augmented by
	WUSB 1.0

Version:
	V1.97k	Mon Jul 14 2008 13:29:36 wkkim	Edit level 3

Copyright notice:
	This file copyright (C) 2007-2008 by

		MCCI Corporation.
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
	Yogender Saroha, MCCI Corporation.	November 2007

Revision history:
   1.97j  Sat Nov 03 2007 12:10:51  yssaroha
	Module created.

   1.97k  Wed Jun 11 2008 17:39:06  wkkim
	PR4185: Add Wire Adapter Class and RPipe Descriptors, and Requests

   1.97k  Mon Jul 14 2008 13:29:36  wkkim
	UDP-4185: Added WUSB Channel Time Types
	UDP-4185: Added key index of group key assigned by host
	UDP-4185: Fix a typo
	(USB_Wa_WA_FEATURE_wValue_DISABLE -> USB_Wa_WA_FEATURE_wValue_RESET)
	UDP-4185: Added RC related types
	UDP-4185: Changed request codes from deximal to hexadecimal
	UDP-4185: Grouped feature selectors by the set feature command

*/

#ifndef _USBWAC10_H_		/* prevent multiple includes */
#define _USBWAC10_H_

/****************************************************************************\
|
|	Class, subclass, interface and protocol codes.
|
\****************************************************************************/

	/* Miscellaneous */
#define	USB_bDeviceClass_Misc				0xEF	
	/* Common Class */
#define	USB_bDeviceSubClass_Common			0x02	
	/* Wire Adapter Multifunction Peripheral */
#define	USB_bDeviceProtocol_WAMP			0x02

	/* Wireless Controller */
#define	USB_bFunctionClass_WirelessController		0xE0
	/* Wireless USB adapter */
#define	USB_bFunctionSubClass_WUSB_Adapter		0x02
	/* Device Wire Adapter Control/Data Streaming interface */
#define	USB_bFunctionProtocol_DWA_CD_StreamInterface	0x02

	/* Wireless Controller */
#define	USB_bInterfaceClass_WirelessController		0xE0	
	/* Wireless USB adapter */
#define	USB_bInterfaceSubClass_WUSB_Adapter		0x02	
	/* Host Wire Adapter Control/Data Streaming interface */
#define	USB_bInterfaceProtocol_HWA_CD_StreamInterface	0x01

	/* Device Wire Adapter Control/Data Streaming interface */
#define	USB_bInterfaceProtocol_DWA_CD_StreamInterface	0x02
	/* Device Wire Adapter Isochronous Streaming interface */
#define	USB_bInterfaceProtocol_DWA_ISO_StreamInterface	0x03

	/* RF Controller */
#define	USB_bInterfaceSubClass_RF_Controller		0x01	
	/* UWB Radio Control Interface */
#define	USB_bInterfaceProtocol_UWB_RC_Interface		0x02


/****************************************************************************\
|
|	Bits in various SETUP commands
|
\****************************************************************************/

/*****  Wire Adapter Class extended Recipient Field  *****/
/*	D...  means dev to host						    */
/*	H...  means host to dev						    */
/*	.S..  means standard						    */
/*	.C..  means class						    */
/*	..PORT means "Port" is target					    */
/*	..PIPE means "RPIPE" is target					    */
#define USB_bmRequestType_DSPORT	0x84
#define USB_bmRequestType_HSPORT	0x04
#define USB_bmRequestType_DSPIPE	0x85
#define USB_bmRequestType_HSPIPE	0x05
#define USB_bmRequestType_DCPIPE	0xA5
#define USB_bmRequestType_HCPIPE	0x25

/*****  Wire Adapter Class request codes  *****/
#define USB_bRequest_WA_ABORT_RPIPE	14
#define USB_bRequest_WA_RESET_RPIPE	15

/*
|| SET_RPIPE_FEATURE, CLEAR_RPIPE_FEATURE
*/
#define	USB_Wa_RPIPE_FEATURE_wValue_PAUSE	1
#define	USB_Wa_RPIPE_FEATURE_wValue_STALL	2
#define	USB_Wa_RPIPE_FEATURE_wValue__INIT \
	{ USB_Wa_RPIPE_FEATURE_wValue_PAUSE,	"RPIPE_PAUSE" }, \
	{ USB_Wa_RPIPE_FEATURE_wValue_STALL, 	"RPIPE_STALL" }

/*
|| SET_WIRE_ADAPTER_FEATURE, CLEAR_WIRE_ADAPTER_FEATURE
*/
#define	USB_Wa_WA_FEATURE_wValue_ENABLE	1
#define	USB_Wa_WA_FEATURE_wValue_RESET	2
#define	USB_Wa_WA_FEATURE_wValue__INIT \
	{ USB_Wa_WA_FEATURE_wValue_ENABLE,	"WA_ENABLE" }, \
	{ USB_Wa_WA_FEATURE_wValue_RESET, 	"WA_RESET" }

/*****  radio control specific request codes  *****/
#define USB_bRequest_HWA_EXEC_RC_CMD		0x28

/*****  HWA specific request codes  *****/
#define USB_bRequest_HWA_ADD_MMC_IE		0x14
#define USB_bRequest_HWA_REMOVE_MMC_IE		0x15
#define USB_bRequest_HWA_SET_NUM_DNTS		0x16
#define USB_bRequest_HWA_SET_CLUSTER_ID		0x17
#define USB_bRequest_HWA_SET_DEVICE_INFO	0x18
#define USB_bRequest_HWA_GET_TIME		0x19
#define USB_bRequest_HWA_SET_STREAM_IDX		0x1A
#define USB_bRequest_HWA_SET_WUSB_MAS		0x1B
#define USB_bRequest_HWA_WUSB_CH_STOP		0x1C

/*****  WUSB Channel Time Types  *****/
#define	USB_wValue_WA_TIME_ADJ			0
#define	USB_wValue_WA_TIME_BPST			1
#define	USB_wValue_WA_TIME_WUSB			2

/*****  WUSB Key index for group key assigned by host  *****/
#define USB_wValue_WUSB_HOST_GROUP_KEY_INDEX	0x20

/*****  DWA specific request codes  *****/
#define USB_bRequest_DWA_SET_EP_ATTRIB		0x1E

/*****  WUSB Device Notification codes  *****/
#define	USB_bNotification_WUSB_DN_CONNECT		0x01
#define	USB_bNotification_WUSB_DN_DISCONNECT		0x02
#define	USB_bNotification_WUSB_DN_EPREADY		0x03
#define	USB_bNotification_WUSB_DN_MAS_AVAIL_CHANGED	0x04
#define	USB_bNotification_WUSB_DN_REMOTE_WAKEUP		0x05
#define	USB_bNotification_WUSB_DN_SLEEP			0x06
#define	USB_bNotification_WUSB_DN_ALIVE			0x07

/*****  Wire Adapter Transfer Status *****/
#define	USB_bTransferStatus_WA_SUCCESS				0			
#define	USB_bTransferStatus_WA_HALTED				1			
#define	USB_bTransferStatus_WA_DATA_BUFFER_ERROR		2			
#define	USB_bTransferStatus_WA_BABBLE				3			
#define	USB_bTransferStatus_WA_NOT_FOUND			4			
#define	USB_bTransferStatus_WA_INSUFFICIENT_RESOURCE		5			
#define	USB_bTransferStatus_WA_TRANSACTION_ERROR		7			
#define	USB_bTransferStatus_WA_ABORTED				8			
#define	USB_bTransferStatus_WA_RPIPE_NOT_READY			9			
#define	USB_bTransferStatus_WA_INVALID_REQUEST_FORMAT		10			
#define	USB_bTransferStatus_WA_UNEXPECTED_SEGMENT_NUMBER	11			
#define	USB_bTransferStatus_WA_RPIPE_TYPE_MISMATCH		12			
#define	USB_bTransferStatus_WA_PACKET_DISCARDED			13			
#define	USB_bTransferStatus_WA_WARNING_BIT			(1 << 6)			
#define	USB_bTransferStatus_WA_ERROR_BIT			(1 << 7)

/***** RCI command/event types *****/
#define WUSB_RC_COMMAND_TYPE_GENERAL		0
#define WUSB_RC_COMMAND_TYPE_RESERVED		1
#define WUSB_RC_COMMAND_TYPE_VENDOR_SPECIFIC	2

/***** RCCB command and RCEB event *****/
#define WUSB_RC_CHANNEL_CHANGE			16
#define WUSB_RC_DEV_ADDR			17
#define WUSB_RC_GET_IE				18
#define WUSB_RC_RESET				19
#define WUSB_RC_SCAN				20
#define WUSB_RC_SET_BEACON_FILTER		21
#define WUSB_RC_SET_DRP_IE			22
#define WUSB_RC_SET_IE				23
#define WUSB_RC_SET_NOTIFICATION_FILTER		24
#define WUSB_RC_TX_POWER			25
#define WUSB_RC_SLEEP				26
#define WUSB_RC_START_BEACONING			27
#define WUSB_RC_STOP_BEACONING			28
#define WUSB_RC_BP_MERGE			29
#define WUSB_RC_SEND_COMMAND_FRAME		30
#define WUSB_RC_SET_ASIE_NOTIFICATION		31

/***** RCEB notification *****/
#define WUSB_RCEB_IE_RECEIVED			0
#define WUSB_RCEB_BEACON_RECEIVED		1
#define WUSB_RCEB_BEACON_SIZE_CHANGE		2
#define WUSB_RCEB_BPOIE_CHANGE			3
#define WUSB_RCEB_BP_SLOT_CHANGE		4
#define WUSB_RCEB_BP_SWITCH_IE_RECEIVED		5
#define WUSB_RCEB_DEV_ADDR_CONFLICT		6
#define WUSB_RCEB_DRP_AVAILABILITY		7
#define WUSB_RCEB_DRP				8
#define WUSB_RCEB_BP_SWICH_STATUS		9
#define WUSB_RCEB_COMMAND_FRAME_RECEIVED	10
#define WUSB_RCEB_CHANNEL_CHANGE_IE_RECEIVED	11

/***** RCI Result code *****/
#define WUSB_RC_RESULT_SUCCESS			0
#define WUSB_RC_RESULT_FAILURE			1
#define WUSB_RC_RESULT_FAILURE_HARDWARE		2
#define WUSB_RC_RESULT_FAILURE_NO_SLOTS		3
#define WUSB_RC_RESULT_FAILURE_BEACON_TOO_LARGE	4
#define WUSB_RC_RESULT_FAILURE_INVALID_PARAMETER	5
#define WUSB_RC_RESULT_FAILURE_UNSUPPORTED_PWR_LEVEL	6
#define WUSB_RC_RESULT_TIME_OUT			7


/****************************************************************************\
|
|	WA Class specific packet formats
|
\****************************************************************************/

/*****  Wire Adapter Control Transfer Request  *****/
#define	USB_bRequestType_WA_CONTROL_TRANSFER		0x80

/* define the type */
typedef struct USB_WA_Control_Transfer_Request_Wire	
	USB_WA_CONTROL_TRANSFER_WIRE, 
	*PUSB_WA_CONTROL_TRANSFER_WIRE;

struct USB_WA_Control_Transfer_Request_Wire
	{
	unsigned char	bLength;
	unsigned char	bRequestType;
	unsigned char	wRPipe[2];
	unsigned char	dwTransferID[4];
	unsigned char	dwTransferLength[4];
	unsigned char	bTransferSegment;
	unsigned char	bmAttribute;
	unsigned char	wReserved[2];
	unsigned char	baSetupData[8];
	};

#define	LENGTHOF_USB_WA_CONTROL_TRANSFER_WIRE		24

/*****  Wire Adapter Bulk or Interrupt Transfer Request  *****/
#define	USB_bRequestType_WA_BULKINT_TRANSFER		0x81

/* define the type */
typedef struct USB_WA_BulkInt_Transfer_Request_Wire	
	USB_WA_BULKINT_TRANSFER_WIRE, 
	*PUSB_WA_BULKINT_TRANSFER_WIRE;

struct USB_WA_BulkInt_Transfer_Request_Wire
	{
	unsigned char	bLength;
	unsigned char	bRequestType;
	unsigned char	wRPipe[2];
	unsigned char	dwTransferID[4];
	unsigned char	dwTransferLength[4];
	unsigned char	bTransferSegment;
	unsigned char	bReserved;
	unsigned char	wReserved[2];
	};

#define	LENGTHOF_USB_WA_BULKINT_TRANSFER_WIRE		16

/*****  Wire Adapter Transfer Completion Notification  *****/
#define	USB_bNotifyType_WA_TRANSFER_NOTIFICATION	0x93

/* define the type */
typedef struct USB_WA_Transfer_Completion_Notification_Wire	
	USB_WA_TRANSFER_NOTIFICATION_WIRE, 
	*PUSB_WA_TRANSFER_NOTIFICATION_WIRE;

struct USB_WA_Transfer_Completion_Notification_Wire
	{
	unsigned char	bLength;
	unsigned char	bNotifyType;
	unsigned char	bEndpoint;
	unsigned char	bReserved;
	};

#define LENGTHOF_USB_WA_TRANSFER_NOTIFICATION_WIRE	4

/*****  Wire Adapter Transfer Result  *****/
#define	USB_bResultType_WA_TRANSFER_RESULT		0x83

/* define the type */
typedef struct USB_WA_Transfer_Result_Wire	
	USB_WA_TRANSFER_RESULT_WIRE, 
	*PUSB_WA_TRANSFER_RESULT_WIRE;

struct USB_WA_Transfer_Result_Wire
	{
	unsigned char	bLength;
	unsigned char	bResultType;
	unsigned char	dwTransferID[4];
	unsigned char	dwTransferLength[4];
	unsigned char	bTransferSegment;
	unsigned char	bTransferStatus;
	unsigned char	dwNumOfPackets[4];
	};

#define	LENGTHOF_USB_WA_TRANSFER_RESULT_WIRE		16

/*****  Wire Adapter Abort Transfer Request  *****/
#define	USB_bRequestType_WA_ABORT_TRANSFER		0x84

/* define the type */
typedef struct USB_WA_Abort_Transfer_Request_Wire	
	USB_WA_ABORT_TRANSFER_WIRE, 
	*PUSB_WA_ABORT_TRANSFER_WIRE;

struct USB_WA_Abort_Transfer_Request_Wire
	{
	unsigned char	bLength;
	unsigned char	bRequestType;
	unsigned char	wRPipe[2];
	unsigned char	dwTransferID[4];
	};

#define	LENGTHOF_USB_WA_ABORT_TRANSFER_WIRE		8

/****************************************************************************\
|
|	WA Class specific descriptors
|
\****************************************************************************/

/*****  Wire Adapter Class Descriptor  *****/
#define	USB_bDescType_WA_CLASS_DESCRIPTOR		0x21

/* define the type */
typedef struct USB_WA_ClassDescriptor_Wire	
	USBIF_WA_CLASSDESC_WIRE, 
	*PUSBIF_WA_CLASSDESC_WIRE;

struct USB_WA_ClassDescriptor_Wire
	{
	/*
	|| Size of this descriptor in bytes, including this field.
	*/
	unsigned char	bLength;
	unsigned char	bDescriptorType;

	/*
	|| WA Class Specification Release Number in Binary- Coded Decimal.
	*/
	unsigned char	bcdWAVersion[2];

	/*
	|| HWA - The maximum number of simultaneous devices that this HWA can 
	|| support.
	|| 
	|| DWA - The number of ports supported by this Wire Adapter.
	*/
	unsigned char	bNumPorts;	

	/* 
	|| HWA - Reserved, must be set to zero. 
	||
	|| DWA - Refer to WUSB Spec. Table 8-21.
	*/
	unsigned char	bmAttributes;	

	/* 
	|| Number of RPipes supported by WA. 
	*/
	unsigned char	wNumRPipes[2];	

	/*
	|| The maximum number of buffer blocks assignable to all RPipes.
	*/
	unsigned char	wRPipeMaxBlock[2];

	/*
	|| The size of an RPipe buffer block, expressed In the form 
	|| 2 ^ (bRpipeBlockSize-1) bytes per block. For example, a value of 
	|| 10 would be 512.
	*/
	unsigned char	bRPipeBlockSize;

	/*
	|| HWA - This field must be set to 0.
	|| 
	|| DWA - Time (in 2 ms intervals) from the time the power-on sequence 
	|| begins on a port until power is good on that port.  The USB System 
	|| Software uses this value to determine how long to wait before 
	|| accessing a powered-on port. 
	*/
	unsigned char	bPwrOn2PwrGood;	

	/*
	|| HWA - This field specifies the number of MMC IE blocks that a HWA can
	|| support at the same time. Each block must have at least 255 bytes of
	|| storage. Valid values are in the range of 1H to FFH. A zero in this
	|| field is undefined for an HWA.
	||
	|| DWA - This field is not used by DWAs and must be set to 0.
	*/
	unsigned char	bNumMMCIEs;	

	/*
	|| HWA - This field is of length 1 and all bits are set to 0.
	|| 
	|| DWA - Indicates if a port has a removable device attached. This field 
	|| is reported on byte-granularity. Within a byte, if no port exists 				
	|| for a given location, the field representing the port characteristics 
	|| returns 0. 
	|| Bit value definition: 
	|| 	0B	Device is removable. 
	|| 	1B	Device is non-removable 
	|| This is a bitmap corresponding to the 
	|| individual ports on the hub: 
	|| 	Bit 0	Reserved. 
	|| 	Bit 1	Port 1 
	|| 	Bit 2	Port 2 
	|| 	.... 
	|| 	Bit n	Port n (implementation-dependent, up to a maximum of 127 
	||		ports). 
	*/
	unsigned char	DeviceRemovable;	
	};
	
#define LENGTHOF_USBIF_HWA_CLASSDESC_WIRE	14
#define	LENGTHOF_USBIF_DWA_CLASSDESC_WIRE(bNumPorts)			\
		(13 + (((bNumPorts) + 8) / 8))

/*****  Wire Adapter RPipe Descriptors (HWA and DWA)  *****/
#define	USB_bDescType_WA_RPIPE_DESCRIPTOR		0x22

/* define the type */
typedef struct USB_HWA_RPipeDescriptor_Wire	
	USBIF_HWA_RPIPEDESC_WIRE, 
	*PUSBIF_HWA_RPIPEDESC_WIRE;

struct USB_HWA_RPipeDescriptor_Wire
	{
	/*
	|| Size of this descriptor in bytes, including this field.
	|| This is a read only field.
	*/
	unsigned char	bLength;

	/*
	|| USB_bDescType_WIRE_ADAPTER_RPIPE. This is a read only field.
	*/
	unsigned char	bDescriptorType;

	/*
	|| Number of this RPipe. Zero-based value identifying the
	|| index in the array of concurrent RPipes supported by this
	|| Wire Adapter. This is a read only field.
	*/
	unsigned char	wRPipeIndex[2];

	/*
	|| The number of concurrent requests that can be assigned to this RPipe.
	|| This is a read only field.
	*/
	unsigned char	wRequests[2];

	/*
	|| The number of buffer blocks assigned to this RPipe.
	|| If the value in this field is zero then the Set RPipe Descriptor 
	|| request can be used to set the number of  blocks to be assigned to 
	|| this RPipe.
	|| If the value in this field is not zero then the number of blocks
	|| assigned to this RPipe cannot be changed by host software.
	|| This field may be modified by host software if this is set to zero.
	*/
	unsigned char	wBlocks[2];
	
	/* 
	|| Maximum packet size of this RPipe
	*/
	unsigned char	wMaxPacketSize[2];

 	/*
	|| Maximum data burst size. Valid values are 1 through 16
	*/
	unsigned char	bMaxBurst;

	/*
	|| This field specifies the device index where the device information
	|| buffer is present.
	*/
	unsigned char	bDeviceInfoIndex;

	/*
	|| The PhyRate at which to communicate with the endpoint targeted by 
	|| RPipe. The value and the associated rate is given in Section 5.6. of 
	|| WUSB Spec 1.0
	*/
	unsigned char	bSpeed;
	unsigned char	bReserved;

	/*
	|| Endpoint Address to be used with this RPipe.
	|| Bit Description
	|| 3:0 	The endpoint number
	|| 6:4 	Reserved; set to zero
	|| 7   	Direction, ignored for control
	||	endpoints
	||	0 = OUT endpoint
	||	1 = IN endpoint
	*/
	unsigned char	bEndpointAddress;

	/*
	|| Current data sequence. This is the next data sequence value to be
	|| used when sending data to the endpoint that this RPipe is targeted 
	|| at.
	*/
	unsigned char	bDataSequence;

	/*
	|| Current Window used for data sequence management and burst transfers.
	*/
	unsigned char	dwCurrentWindow[4];

	/*
	|| Maximum sequence number that the endpoint supports. Valid values are 
	|| 1 through 31.
	*/
	unsigned char	bMaxDataSequence;

	/*
	|| For Interrupt transfers this is the polling interval to be used by 
	|| this RPipe in downstream communications
	|| For Isochronous transfers this is the logical service interval.
	|| See bInterval in Table 7-31 of WUSB Spec 1.0
	*/
	unsigned char	bInterval;

	/*
	|| If the transfer type is Isochronous, then this field is the interval
	|| for polling the downstream endpoint.
	|| This field is Reserved and must be set to zero for all other transfer 
	|| types. See bOverTheAirInterval in Table 7-32 of WUSB Spec 1.0 for the 
	|| encoding of this field.
	*/
	unsigned char	bOverTheAirInterval;

	/*
	|| Bit Description
	|| 1:0 Value Transfer Type
	||	00B Control
	||	01B Isochronous
	||	10B Bulk
	||	11B Interrupt
	|| 4:2 Transmit Power
	||	See Section 5.2.1.2 of WUSB Spec 1.0 for details on the use of 
	|| 	this field.
	|| 7:5 Data Burst Preamble Policy
	||	See Table 5-7 of WUSB Spec 1.0 for the encoding of this field.
	*/
	unsigned char	bmAttribute;

	/*
	|| Transfer types supported on this RPipe
	|| Bit Description
	|| 0	1: Control Transfer supported
	||	0: Control Transfer NOT supported
	|| 1    1: Isochronous Transfer supported
	||	0: Isochronous Transfer NOT supported
	|| 2    1: Bulk Transfer supported
	||	0: Bulk Transfer NOT supported
	|| 3    1: Interrupt Transfer supported
	||	0: Interrupt Transfer NOT supported
	|| 7:4 Reserved
	|| This is a read only field.
	*/
	unsigned char	bmCharacteristics;

	/*
	|| Bit Description
	|| 3:0 Max Retry Count
	|| 	The maximum number of times a transaction must be retried before 
	|| 	the transfer request is failed. 
	|| 	The valid values are 0 through 15. A value of zero in this field 
	||	indicates  that the HWA must not count errors and there is no 
	|| 	limit on the retries.
	||	For Isochronous transfers this field is set to zero.
	|| 6:4 Reserved
	|| 7   Low Power Interrupt. 
	|| 	If this bit is set, then this is a low power interrupt endpoint 
	|| 	and the	MaxRetryCount field is ignored.
	*/
	unsigned char	bmRetryOptions;

	/*
	|| The HWA increments this field when it encounters an error while
	|| performing transactions to the downstream endpoint targeted by
	|| this RPipe.
	|| The host is responsible for resetting this field.
	*/
	unsigned char	wNumTransactionErrors[2];
	};

/* define the type */
typedef struct USB_DWA_RPipeDescriptor_Wire	
	USBIF_DWA_RPIPEDESC_WIRE, 
	*PUSBIF_DWA_RPIPEDESC_WIRE;

struct USB_DWA_RPipeDescriptor_Wire
	{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	wRPipeIndex[2];
	unsigned char	wRequests[2];
	unsigned char	wBlocks[2];
	unsigned char	wMaxPacketSize[2];
	
	/*
	|| This is the address of the attached device’s parent high speed hub. 
	|| This number is used only when a full/low device is connected to the 
	|| DWA through a high speed hub. This field must be set to Zero if the 
	|| full/low speed device is directly connected to the DWA. This field is 
	|| reserved if the bSpeed field is set to High Speed.
	*/
	unsigned char	bHSHubAddress;

	/*
	|| This is the Port number on which the attached device is connected to 
	|| its parent high speed hub. This number is used only when a full/low 
	|| device is connected to the DWA through a high speed hub. This field
	|| must be set to Zero if the full/low speed device is directly 
	|| connected to the DWA. This field is reserved if the bSpeed field
	|| is set to High Speed.
	*/
	unsigned char	bHSHubPort;
	
	/*
	|| The speed of device to be targeted by RPipe.
	|| Value 	Description
	|| 00B 		Full-Speed (12Mbs)
	|| 01B 		Low-Speed (1.5Mbs)
	|| 10B 		High-Speed (480 Mbs)
	|| 11B 		Reserved
	*/
	unsigned char	bSpeed;

	/*
	|| Address to be used with attached device
	*/
	unsigned char	bDeviceAddress;
	unsigned char	bEndpointAddress;
	unsigned char	bDataSequence;

	/*
	|| Reserved and must be set to zero.
	*/
	unsigned char	dwCurrentWindow[4];

	/*
	|| Reserved and must be set to zero.
	*/
	unsigned char	bMaxDataSequence;

	/*
	|| Polling interval to be used by this RPipe in downstream 
	|| communications
	*/
	unsigned char	bInterval;

	/*
	|| If the transfer type is Isochronous, then this is the interval at 
	|| which the upstream wireless endpoint is polled. See 
	|| bOverTheAirInterval in Table 7-32 for the encoding of this field.
	|| This field is Reserved and must be set to zero for all other transfer 
	|| types.
	*/
	unsigned char	bOverTheAirInterval;

	/*
	|| Bit Description
	|| 1:0	Value 	Transfer Type
	||	00B	Control
	||	01B	Isochronous
	||	10B	Bulk
	||	11B	Interrupt
	||
	|| 5:2 	If the transfer type is Isochronous, then this field indicates 
	|| 	the associated upstream Isochronous endpoint on the DWA. This 
	|| 	field is reserved and must be set to zero for all other transfer 
	|| 	types.
	||
	|| 6   Reserved
	||
	|| 7	If this bit is set to one then the endpoint that is the target 	
	|| 	of this RPipe is connected behind a hub that has Multi-TT 	
	|| 	support enabled. This field is only valid if the bHSHubAddress 
	|| 	field is not set to zero and the bSpeed is not set to High-
	|| 	Speed.
	||	Host software must not set this bit to one if the bmAttributes.
	||	Multi-TT Supported field is set to zero in the Wire Adapter 
	||	Class Descriptor.
	*/
	unsigned char	bmAttribute;
	unsigned char	bmCharacteristics;

	/*
	|| Bit Description
	||
	|| 2:0	The maximum number of times a transaction must be retried before
	||	the transfer request is failed.
	||	The valid values are 0 through 3. A value of zero in this field 
	|| 	indicates that the DWA must not count errors and there is no 
	|| 	limit on the retries. 
	||	For Isochronous transfers this field is set to zero.
	|| 7:3  Reserved.
	*/
	unsigned char	bmRetryOptions;
	unsigned char	wNumTransactionErrors[2];
	};

#define LENGTHOF_USBIF_HWA_RPIPEDESC_WIRE	28
#define LENGTHOF_USBIF_DWA_RPIPEDESC_WIRE	28

/**** end of usbwac10.h ****/
#endif /* _USBWAC10_H_ */
