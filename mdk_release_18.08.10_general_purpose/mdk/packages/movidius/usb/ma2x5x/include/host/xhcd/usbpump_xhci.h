/* usbpump_xhci.h	Fri Dec 26 2014 22:31:08 chwon */

/*

Module: usbpump_xhci.h

Function:
	Definition of structure and constants defined in xHCI spec.

Version:
	V3.13b	Fri Dec 26 2014 22:31:08 chwon	Edit level 10

Copyright notice:
	This file copyright (C) 2009-2012, 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Xiaowen Lu, MCCI Corporation	March 2009

Revision history:
   2.01a  Fri Mar 13 2009 11:59:25 luxw
	7678: Module created.

   2.01a  Sun May 17 2009 15:27:22  luxw
	7678: Added port speed contants definition.

   2.01a  Mon Jun 22 2009 19:20:50  luxw
	7678: Added slot context state and endpoint context state definition.

   2.01a  Fri Sep 18 2009 23:34:40  luxw
	8931: Defined USBPUMP_XHCI_TRB_MAX_TRANSFER_SIZE.

   2.01a  Tue Nov 24 2009 19:32:49  luxw
	9272: Add stream context declaration.

   3.01c  Thu Jul 15 2010 11:06:42  luxw
	8779: Added USBPUMP_XHCI_SCRATCHPAD_BUFFER_ELEMENT definition. 

   3.01d  Tue Nov 02 2010 02:20:45  luxw
	11684: Define max frame number for frame index register.

   3.01d  Sat Apr 02 2011 12:48:55  luxw
	8779: Fixed scratchpad buffer element size problem.

   3.01f  Wed Apr 25 2012 14:08:14  luxw
	15113: Change the name fro USBPUMP_XHCI_STOPPED_LEN to
	USBPUMP_XHCI_STOPPED_INVALID_LEN

   3.13b  Fri Dec 26 2014 22:31:08  chwon
	18796: Add USBPUMP_XHCI_EVENT_RING_SEGMENT_TABLE_ALIGN.

*/

#ifndef _USBPUMP_XHCI_H_		/* prevent multiple includes */
#define _USBPUMP_XHCI_H_

#ifndef _USBPUMP_XHCD_TYPES_H_
# include "usbpump_xhcd_types.h"
#endif

/****************************************************************************\
|
|	Definition of TRB structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XHCI_TRB)
	{
	__TMS_VOLATILE __TMS_UINT32	DW0;
	__TMS_VOLATILE __TMS_UINT32	DW1;
	__TMS_VOLATILE __TMS_UINT32	DW2;
	__TMS_VOLATILE __TMS_UINT32	DW3;
	};

/****************************************************************************\
|
|	Definition of SLOT Context structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME (USBPUMP_XHCI_SLOT_CONTEXT)
	{
	__TMS_VOLATILE __TMS_UINT32	DW0;
	__TMS_VOLATILE __TMS_UINT32	DW1;
	__TMS_VOLATILE __TMS_UINT32	DW2;
	__TMS_VOLATILE __TMS_UINT32	DW3;
	__TMS_VOLATILE __TMS_UINT32	DW4;
	__TMS_VOLATILE __TMS_UINT32	DW5;
	__TMS_VOLATILE __TMS_UINT32	DW6;
	__TMS_VOLATILE __TMS_UINT32	DW7;
	};

/****************************************************************************\
|
|	Definition of Endpoint Context structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME (USBPUMP_XHCI_ENDPOINT_CONTEXT)
	{
	__TMS_VOLATILE __TMS_UINT32	DW0;
	__TMS_VOLATILE __TMS_UINT32	DW1;
	__TMS_VOLATILE __TMS_UINT32	DW2;
	__TMS_VOLATILE __TMS_UINT32	DW3;
	__TMS_VOLATILE __TMS_UINT32	DW4;
	__TMS_VOLATILE __TMS_UINT32	DW5;
	__TMS_VOLATILE __TMS_UINT32	DW6;
	__TMS_VOLATILE __TMS_UINT32	DW7;
	};

/****************************************************************************\
|
|	Definition of Input Control Context structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME (USBPUMP_XHCI_INPUT_CONTROL_CONTEXT)
	{
	__TMS_VOLATILE __TMS_UINT32	DW0;
	__TMS_VOLATILE __TMS_UINT32	DW1;
	__TMS_VOLATILE __TMS_UINT32	DW2;
	__TMS_VOLATILE __TMS_UINT32	DW3;
	__TMS_VOLATILE __TMS_UINT32	DW4;
	__TMS_VOLATILE __TMS_UINT32	DW5;
	__TMS_VOLATILE __TMS_UINT32	DW6;
	__TMS_VOLATILE __TMS_UINT32	DW7;
	};

/****************************************************************************\
|
|	Definition of Input Control Context structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME (USBPUMP_XHCI_STREAM_CONTEXT)
	{
	__TMS_VOLATILE __TMS_UINT32	DW0;
	__TMS_VOLATILE __TMS_UINT32	DW1;
	__TMS_VOLATILE __TMS_UINT32	DW2;
	__TMS_VOLATILE __TMS_UINT32	DW3;
	};

/****************************************************************************\
|
|	Definition of XHCI_DCBAA_ENTRY
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XHCI_DCBAA_ENTRY)
	{
	__TMS_VOLATILE __TMS_UINT64	Entry;
	};

/****************************************************************************\
|
|	Definition of XHCI COMMAND RING structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XHCI_COMMAND_RING)
	{
	__TMS_USBPUMP_XHCI_TRB	TRB[1];
	};

/****************************************************************************\
|
|	Definition of Event Ring Segment Table structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XHCI_EVENT_RING_SEGMENT_TABLE_ENTRY)
	{
	__TMS_VOLATILE __TMS_UINT32	DW0;
	__TMS_VOLATILE __TMS_UINT32	DW1;
	__TMS_VOLATILE __TMS_UINT32	DW2;
	__TMS_VOLATILE __TMS_UINT32	DW3;
	};

struct __TMS_STRUCTNAME(USBPUMP_XHCI_EVENT_RING_SEGMENT_TABLE)
	{
	__TMS_USBPUMP_XHCI_EVENT_RING_SEGMENT_TABLE_ENTRY	Entry[1];
	};

struct __TMS_STRUCTNAME(USBPUMP_XHCI_EVENT_RING_SEGMENT)
	{
	__TMS_USBPUMP_XHCI_TRB		TRB[1];
	};

/****************************************************************************\
|
|	Definition of XHCI TRANSFER RING structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XHCI_TRANSFER_RING)
	{
	__TMS_USBPUMP_XHCI_TRB	TRB[1];
	};

/****************************************************************************\
|
|	Definition of XHCI Scratchpad buffer array entry structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XHCI_SCRATCHPAD_BUFFER_ELEMENT)
	{
	__TMS_VOLATILE __TMS_UINT32	DW0;
	__TMS_VOLATILE __TMS_UINT32	DW1;
	};	

/****************************************************************************\
|
|	Internal Constants definition
|
\****************************************************************************/

#define __TMS_USBPUMP_XHCI_TRB_SIZE				16
#define __TMS_USBPUMP_XHCI_TRB_MAX_TRANSFER_SIZE		65536
#define __TMS_USBPUMP_XHCI_SIZE_DEVICE_CTX_32			1024
#define __TMS_USBPUMP_XHCI_SIZE_DEVICE_CTX_64			2048
#define __TMS_USBPUMP_XHCI_SIZE_INPUT_CTX_32			1056
#define __TMS_USBPUMP_XHCI_SIZE_INPUT_CTX_64			2112
#define __TMS_USBPUMP_XHCI_DCBAA_ENTRY_SIZE			8

#define __TMS_USBPUMP_XHCI_CMD_RING_ALIGN			64
#define __TMS_USBPUMP_XHCI_EVENT_RING_ALIGN			16
#define	__TMS_USBPUMP_XHCI_EVENT_RING_SEGMENT_TABLE_ALIGN	64
#define __TMS_USBPUMP_XHCI_TRANSFER_RING_ALIGN			16
#define __TMS_USBPUMP_XHCI_DEVICE_CTX_ALIGN			64
#define __TMS_USBPUMP_XHCI_STREAM_CTX_ALIGN			16
#define __TMS_USBPUMP_XHCI_SCRATCHPAD_BUFFER_ARRAY_ALIGN	64

#define __TMS_USBPUMP_XHCI_MAX_NUMBER_EP			31
#define __TMS_USBPUMP_XHCI_MAX_NUMBER_TRB_IN_TRANSFER_RING	4096  /* TRBs */
#define __TMS_USBPUMP_XHCI_MAX_EVENT_RING_SEGMENT_TABLE		32768 /* TRBs */

#define __TMS_USBPUMP_XHCI_DEFAULT_ENDPOINT_ID			1

#define __TMS_USBPUMP_XHCI_FRAME_NUM_MASK			0x7FF

/****************************************************************************\
|
|	Root port speed definition
|
\****************************************************************************/

/* Low-Speed device */
#define __TMS_USBPUMP_XHCI_PORT_SPEED_LOW			2
/* Full-speed device */
#define __TMS_USBPUMP_XHCI_PORT_SPEED_FULL			1
/* High-speed device */
#define __TMS_USBPUMP_XHCI_PORT_SPEED_HIGH			3
/* Super-speed device */
#define __TMS_USBPUMP_XHCI_PORT_SPEED_SUPER			4

/****************************************************************************\
|
|	Endpoint state definition
|
\****************************************************************************/

#define __TMS_USBPUMP_XHCI_ENDPOINT_STATE_DISABLE		0
#define __TMS_USBPUMP_XHCI_ENDPOINT_STATE_RUNNING		1
#define __TMS_USBPUMP_XHCI_ENDPOINT_STATE_HALTED		2
#define __TMS_USBPUMP_XHCI_ENDPOINT_STATE_STOPPED		3
#define __TMS_USBPUMP_XHCI_ENDPOINT_STATE_ERROR			4

/****************************************************************************\
|
|	Slot state definition
|
\****************************************************************************/

#define __TMS_USBPUMP_XHCI_SLOT_STATE_DISABLE			0
#define __TMS_USBPUMP_XHCI_SLOT_STATE_ENABLE			0
#define __TMS_USBPUMP_XHCI_SLOT_STATE_DEFAULT			1
#define __TMS_USBPUMP_XHCI_SLOT_STATE_ADDRESSED			2
#define __TMS_USBPUMP_XHCI_SLOT_STATE_CONFIGURED		3

/****************************************************************************\
|
|	TRB Type
|
\****************************************************************************/

#define __TMS_USBPUMP_XHCI_TRB_RESERVED			0
#define __TMS_USBPUMP_XHCI_TRB_NORMAL			1
#define __TMS_USBPUMP_XHCI_TRB_SETUP			2
#define __TMS_USBPUMP_XHCI_TRB_DATA			3
#define __TMS_USBPUMP_XHCI_TRB_STATUS			4
#define __TMS_USBPUMP_XHCI_TRB_ISO			5
#define __TMS_USBPUMP_XHCI_TRB_LINK			6
#define __TMS_USBPUMP_XHCI_TRB_EVENT_DATA 		7
#define __TMS_USBPUMP_XHCI_TRB_NOOP			8
#define __TMS_USBPUMP_XHCI_TRB_ENABLE_SLOT		9
#define __TMS_USBPUMP_XHCI_TRB_DISABLE_SLOT		10
#define __TMS_USBPUMP_XHCI_TRB_ADDRESS_DEVICE		11
#define __TMS_USBPUMP_XHCI_TRB_CONFIG_ENDPOINT		12
#define __TMS_USBPUMP_XHCI_TRB_EVALUATE_CONTEXT		13
#define __TMS_USBPUMP_XHCI_TRB_RESET_ENDPOINT		14
#define __TMS_USBPUMP_XHCI_TRB_STOP_ENDPOINT		15
#define __TMS_USBPUMP_XHCI_TRB_SET_TR_DEQEUE		16
#define __TMS_USBPUMP_XHCI_TRB_RESET_DEVICE		17
#define __TMS_USBPUMP_XHCI_TRB_FORCE_EVENT 		18
#define __TMS_USBPUMP_XHCI_TRB_NEGOTIATE_BW		19
#define __TMS_USBPUMP_XHCI_TRB_BELT			20
#define __TMS_USBPUMP_XHCI_TRB_GET_PORT_BW		21
#define __TMS_USBPUMP_XHCI_TRB_FORCE_HEADER 		22
#define __TMS_USBPUMP_XHCI_TRB_NO_OP_COMMAND		23
#define __TMS_USBPUMP_XHCI_TRB_TRANSFER_EVENT 		32
#define __TMS_USBPUMP_XHCI_TRB_COMMAND_COMPLETION	33
#define __TMS_USBPUMP_XHCI_TRB_PTSC			34
#define __TMS_USBPUMP_XHCI_TRB_BW_REQUEST		35
#define __TMS_USBPUMP_XHCI_TRB_DOORBELL			36
#define __TMS_USBPUMP_XHCI_TRB_HOST_CONTROLLER		37
#define __TMS_USBPUMP_XHCI_TRB_DEV_NOTIFICATION		38
#define __TMS_USBPUMP_XHCI_TRB_MFINDEX			39

/****************************************************************************\
|
|	Completion Code
|
\****************************************************************************/

#define __TMS_USBPUMP_XHCI_INVALID			0
#define __TMS_USBPUMP_XHCI_SUCCESS			1
/* transfer event */
#define __TMS_USBPUMP_XHCI_DATA_BUFFER_ERROR		2
/* transfer event */
#define __TMS_USBPUMP_XHCI_BABBLE_ERROR			3
/* transfer event */
#define __TMS_USBPUMP_XHCI_USB_TRANSACTION_ERROR 	4
/* command event, transfer event */
#define __TMS_USBPUMP_XHCI_TRB_ERROR			5
/* transfer event */
#define __TMS_USBPUMP_XHCI_STALL			6
/* command event - configure endpoint */
#define __TMS_USBPUMP_XHCI_RESOURCE_ERROR		7
/* command event - configure endpoint */
#define __TMS_USBPUMP_XHCI_BW_ERROR			8
/* command event - enable slotd */
#define __TMS_USBPUMP_XHCI_NO_SLOTS			9
/* transfer event */
#define __TMS_USBPUMP_XHCI_INVALID_STREAM_TYPE		10
/* command event */
#define __TMS_USBPUMP_XHCI_SLOT_NOT_ENABLE		11
/* command event - related to endpoint operation */
#define __TMS_USBPUMP_XHCI_EP_NOT_ENABLE		12
/* transfer event */
#define __TMS_USBPUMP_XHCI_SHORT_PACKET			13
/* transfer event */
#define __TMS_USBPUMP_XHCI_RING_UNDER_RUN		14
/* transfer event */
#define __TMS_USBPUMP_XHCI_RING_OVER_RUN		15
/* host event */
#define __TMS_USBPUMP_XHCI_VF_EVENT_RING_FULL		16
/* command event */
#define __TMS_USBPUMP_XHCI_PARAMETER_ERROR		17
/* transfer event - ISO */
#define __TMS_USBPUMP_XHCI_BW_OVERRUN			18
/* command event */
#define __TMS_USBPUMP_XHCI_CTX_STATE_ERROR		19
/* host event */
#define __TMS_USBPUMP_XHCI_NO_PING			20
/* host event */
#define __TMS_USBPUMP_XHCI_EVENT_RING_FULL		21
/* transfer event - ISO */
#define __TMS_USBPUMP_XHCI_MISSED_SERVICE		23
/* host event */
#define __TMS_USBPUMP_XHCI_COMMAND_RING_STOPPED 	24
/* host event */
#define __TMS_USBPUMP_XHCI_COMMAND_ABORT		25
/* transfer event - due to stop endpoint */
#define __TMS_USBPUMP_XHCI_STOPPED			26
/* transfer event - due to stop endpoint */
#define __TMS_USBPUMP_XHCI_STOPPED_INVALID_LEN		27
/* transfer event */
#define __TMS_USBPUMP_XHCI_CTRL_ABORT			28
/* port status event */
#define __TMS_USBPUMP_XHCI_L1_ERROR			29
/* command event - reset endpoint or set tr dequeue */
#define __TMS_USBPUMP_XHCI_TRANSFER_RING_BUSY		30
/* transfer event - ISO */
#define __TMS_USBPUMP_XHCI_ISO_OVERRUN			31
/* host event */
#define __TMS_USBPUMP_XHCI_EVENT_LOST			32
#define __TMS_USBPUMP_XHCI_UNDEFINED			33
/* related to stream, not used in phase 1 */
#define __TMS_USBPUMP_XHCI_INVALID_STREAM_ID 		34
/* USB2.0 */
#define __TMS_USBPUMP_XHCI_SPLIT_ERROR			36

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_XHCI_TRB_SIZE	\
   __TMS_USBPUMP_XHCI_TRB_SIZE
# define USBPUMP_XHCI_TRB_MAX_TRANSFER_SIZE	\
   __TMS_USBPUMP_XHCI_TRB_MAX_TRANSFER_SIZE
# define USBPUMP_XHCI_SIZE_DEVICE_CTX_32	\
   __TMS_USBPUMP_XHCI_SIZE_DEVICE_CTX_32
# define USBPUMP_XHCI_SIZE_DEVICE_CTX_64	\
   __TMS_USBPUMP_XHCI_SIZE_DEVICE_CTX_64
# define USBPUMP_XHCI_SIZE_INPUT_CTX_32	\
   __TMS_USBPUMP_XHCI_SIZE_INPUT_CTX_32
# define USBPUMP_XHCI_SIZE_INPUT_CTX_64	\
   __TMS_USBPUMP_XHCI_SIZE_INPUT_CTX_64
# define USBPUMP_XHCI_DCBAA_ENTRY_SIZE	\
   __TMS_USBPUMP_XHCI_DCBAA_ENTRY_SIZE
# define USBPUMP_XHCI_CMD_RING_ALIGN	\
   __TMS_USBPUMP_XHCI_CMD_RING_ALIGN
# define USBPUMP_XHCI_EVENT_RING_ALIGN	\
   __TMS_USBPUMP_XHCI_EVENT_RING_ALIGN
# define USBPUMP_XHCI_EVENT_RING_SEGMENT_TABLE_ALIGN	\
   __TMS_USBPUMP_XHCI_EVENT_RING_SEGMENT_TABLE_ALIGN
# define USBPUMP_XHCI_TRANSFER_RING_ALIGN	\
   __TMS_USBPUMP_XHCI_TRANSFER_RING_ALIGN
# define USBPUMP_XHCI_DEVICE_CTX_ALIGN	\
   __TMS_USBPUMP_XHCI_DEVICE_CTX_ALIGN
# define USBPUMP_XHCI_STREAM_CTX_ALIGN	\
   __TMS_USBPUMP_XHCI_STREAM_CTX_ALIGN
# define USBPUMP_XHCI_SCRATCHPAD_BUFFER_ARRAY_ALIGN	\
   __TMS_USBPUMP_XHCI_SCRATCHPAD_BUFFER_ARRAY_ALIGN
# define USBPUMP_XHCI_MAX_NUMBER_EP	\
   __TMS_USBPUMP_XHCI_MAX_NUMBER_EP
# define USBPUMP_XHCI_MAX_NUMBER_TRB_IN_TRANSFER_RING	\
   __TMS_USBPUMP_XHCI_MAX_NUMBER_TRB_IN_TRANSFER_RING
# define USBPUMP_XHCI_MAX_EVENT_RING_SEGMENT_TABLE	\
   __TMS_USBPUMP_XHCI_MAX_EVENT_RING_SEGMENT_TABLE
# define USBPUMP_XHCI_DEFAULT_ENDPOINT_ID	\
   __TMS_USBPUMP_XHCI_DEFAULT_ENDPOINT_ID
# define USBPUMP_XHCI_FRAME_NUM_MASK	\
   __TMS_USBPUMP_XHCI_FRAME_NUM_MASK
# define USBPUMP_XHCI_PORT_SPEED_LOW	\
   __TMS_USBPUMP_XHCI_PORT_SPEED_LOW
# define USBPUMP_XHCI_PORT_SPEED_FULL	\
   __TMS_USBPUMP_XHCI_PORT_SPEED_FULL
# define USBPUMP_XHCI_PORT_SPEED_HIGH	\
   __TMS_USBPUMP_XHCI_PORT_SPEED_HIGH
# define USBPUMP_XHCI_PORT_SPEED_SUPER	\
   __TMS_USBPUMP_XHCI_PORT_SPEED_SUPER
# define USBPUMP_XHCI_ENDPOINT_STATE_DISABLE	\
   __TMS_USBPUMP_XHCI_ENDPOINT_STATE_DISABLE
# define USBPUMP_XHCI_ENDPOINT_STATE_RUNNING	\
   __TMS_USBPUMP_XHCI_ENDPOINT_STATE_RUNNING
# define USBPUMP_XHCI_ENDPOINT_STATE_HALTED	\
   __TMS_USBPUMP_XHCI_ENDPOINT_STATE_HALTED
# define USBPUMP_XHCI_ENDPOINT_STATE_STOPPED	\
   __TMS_USBPUMP_XHCI_ENDPOINT_STATE_STOPPED
# define USBPUMP_XHCI_ENDPOINT_STATE_ERROR	\
   __TMS_USBPUMP_XHCI_ENDPOINT_STATE_ERROR
# define USBPUMP_XHCI_SLOT_STATE_DISABLE	\
   __TMS_USBPUMP_XHCI_SLOT_STATE_DISABLE
# define USBPUMP_XHCI_SLOT_STATE_ENABLE	\
   __TMS_USBPUMP_XHCI_SLOT_STATE_ENABLE
# define USBPUMP_XHCI_SLOT_STATE_DEFAULT	\
   __TMS_USBPUMP_XHCI_SLOT_STATE_DEFAULT
# define USBPUMP_XHCI_SLOT_STATE_ADDRESSED	\
   __TMS_USBPUMP_XHCI_SLOT_STATE_ADDRESSED
# define USBPUMP_XHCI_SLOT_STATE_CONFIGURED	\
   __TMS_USBPUMP_XHCI_SLOT_STATE_CONFIGURED
# define USBPUMP_XHCI_TRB_RESERVED	\
   __TMS_USBPUMP_XHCI_TRB_RESERVED
# define USBPUMP_XHCI_TRB_NORMAL	\
   __TMS_USBPUMP_XHCI_TRB_NORMAL
# define USBPUMP_XHCI_TRB_SETUP	\
   __TMS_USBPUMP_XHCI_TRB_SETUP
# define USBPUMP_XHCI_TRB_DATA	\
   __TMS_USBPUMP_XHCI_TRB_DATA
# define USBPUMP_XHCI_TRB_STATUS	\
   __TMS_USBPUMP_XHCI_TRB_STATUS
# define USBPUMP_XHCI_TRB_ISO	\
   __TMS_USBPUMP_XHCI_TRB_ISO
# define USBPUMP_XHCI_TRB_LINK	\
   __TMS_USBPUMP_XHCI_TRB_LINK
# define USBPUMP_XHCI_TRB_EVENT_DATA	\
   __TMS_USBPUMP_XHCI_TRB_EVENT_DATA
# define USBPUMP_XHCI_TRB_NOOP	\
   __TMS_USBPUMP_XHCI_TRB_NOOP
# define USBPUMP_XHCI_TRB_ENABLE_SLOT	\
   __TMS_USBPUMP_XHCI_TRB_ENABLE_SLOT
# define USBPUMP_XHCI_TRB_DISABLE_SLOT	\
   __TMS_USBPUMP_XHCI_TRB_DISABLE_SLOT
# define USBPUMP_XHCI_TRB_ADDRESS_DEVICE	\
   __TMS_USBPUMP_XHCI_TRB_ADDRESS_DEVICE
# define USBPUMP_XHCI_TRB_CONFIG_ENDPOINT	\
   __TMS_USBPUMP_XHCI_TRB_CONFIG_ENDPOINT
# define USBPUMP_XHCI_TRB_EVALUATE_CONTEXT	\
   __TMS_USBPUMP_XHCI_TRB_EVALUATE_CONTEXT
# define USBPUMP_XHCI_TRB_RESET_ENDPOINT	\
   __TMS_USBPUMP_XHCI_TRB_RESET_ENDPOINT
# define USBPUMP_XHCI_TRB_STOP_ENDPOINT	\
   __TMS_USBPUMP_XHCI_TRB_STOP_ENDPOINT
# define USBPUMP_XHCI_TRB_SET_TR_DEQEUE	\
   __TMS_USBPUMP_XHCI_TRB_SET_TR_DEQEUE
# define USBPUMP_XHCI_TRB_RESET_DEVICE	\
   __TMS_USBPUMP_XHCI_TRB_RESET_DEVICE
# define USBPUMP_XHCI_TRB_FORCE_EVENT	\
   __TMS_USBPUMP_XHCI_TRB_FORCE_EVENT
# define USBPUMP_XHCI_TRB_NEGOTIATE_BW	\
   __TMS_USBPUMP_XHCI_TRB_NEGOTIATE_BW
# define USBPUMP_XHCI_TRB_BELT	\
   __TMS_USBPUMP_XHCI_TRB_BELT
# define USBPUMP_XHCI_TRB_GET_PORT_BW	\
   __TMS_USBPUMP_XHCI_TRB_GET_PORT_BW
# define USBPUMP_XHCI_TRB_FORCE_HEADER	\
   __TMS_USBPUMP_XHCI_TRB_FORCE_HEADER
# define USBPUMP_XHCI_TRB_NO_OP_COMMAND	\
   __TMS_USBPUMP_XHCI_TRB_NO_OP_COMMAND
# define USBPUMP_XHCI_TRB_TRANSFER_EVENT	\
   __TMS_USBPUMP_XHCI_TRB_TRANSFER_EVENT
# define USBPUMP_XHCI_TRB_COMMAND_COMPLETION	\
   __TMS_USBPUMP_XHCI_TRB_COMMAND_COMPLETION
# define USBPUMP_XHCI_TRB_PTSC	\
   __TMS_USBPUMP_XHCI_TRB_PTSC
# define USBPUMP_XHCI_TRB_BW_REQUEST	\
   __TMS_USBPUMP_XHCI_TRB_BW_REQUEST
# define USBPUMP_XHCI_TRB_DOORBELL	\
   __TMS_USBPUMP_XHCI_TRB_DOORBELL
# define USBPUMP_XHCI_TRB_HOST_CONTROLLER	\
   __TMS_USBPUMP_XHCI_TRB_HOST_CONTROLLER
# define USBPUMP_XHCI_TRB_DEV_NOTIFICATION	\
   __TMS_USBPUMP_XHCI_TRB_DEV_NOTIFICATION
# define USBPUMP_XHCI_TRB_MFINDEX	\
   __TMS_USBPUMP_XHCI_TRB_MFINDEX
# define USBPUMP_XHCI_INVALID	\
   __TMS_USBPUMP_XHCI_INVALID
# define USBPUMP_XHCI_SUCCESS	\
   __TMS_USBPUMP_XHCI_SUCCESS
# define USBPUMP_XHCI_DATA_BUFFER_ERROR	\
   __TMS_USBPUMP_XHCI_DATA_BUFFER_ERROR
# define USBPUMP_XHCI_BABBLE_ERROR	\
   __TMS_USBPUMP_XHCI_BABBLE_ERROR
# define USBPUMP_XHCI_USB_TRANSACTION_ERROR	\
   __TMS_USBPUMP_XHCI_USB_TRANSACTION_ERROR
# define USBPUMP_XHCI_TRB_ERROR	\
   __TMS_USBPUMP_XHCI_TRB_ERROR
# define USBPUMP_XHCI_STALL	\
   __TMS_USBPUMP_XHCI_STALL
# define USBPUMP_XHCI_RESOURCE_ERROR	\
   __TMS_USBPUMP_XHCI_RESOURCE_ERROR
# define USBPUMP_XHCI_BW_ERROR	\
   __TMS_USBPUMP_XHCI_BW_ERROR
# define USBPUMP_XHCI_NO_SLOTS	\
   __TMS_USBPUMP_XHCI_NO_SLOTS
# define USBPUMP_XHCI_INVALID_STREAM_TYPE	\
   __TMS_USBPUMP_XHCI_INVALID_STREAM_TYPE
# define USBPUMP_XHCI_SLOT_NOT_ENABLE	\
   __TMS_USBPUMP_XHCI_SLOT_NOT_ENABLE
# define USBPUMP_XHCI_EP_NOT_ENABLE	\
   __TMS_USBPUMP_XHCI_EP_NOT_ENABLE
# define USBPUMP_XHCI_SHORT_PACKET	\
   __TMS_USBPUMP_XHCI_SHORT_PACKET
# define USBPUMP_XHCI_RING_UNDER_RUN	\
   __TMS_USBPUMP_XHCI_RING_UNDER_RUN
# define USBPUMP_XHCI_RING_OVER_RUN	\
   __TMS_USBPUMP_XHCI_RING_OVER_RUN
# define USBPUMP_XHCI_VF_EVENT_RING_FULL	\
   __TMS_USBPUMP_XHCI_VF_EVENT_RING_FULL
# define USBPUMP_XHCI_PARAMETER_ERROR	\
   __TMS_USBPUMP_XHCI_PARAMETER_ERROR
# define USBPUMP_XHCI_BW_OVERRUN	\
   __TMS_USBPUMP_XHCI_BW_OVERRUN
# define USBPUMP_XHCI_CTX_STATE_ERROR	\
   __TMS_USBPUMP_XHCI_CTX_STATE_ERROR
# define USBPUMP_XHCI_NO_PING	\
   __TMS_USBPUMP_XHCI_NO_PING
# define USBPUMP_XHCI_EVENT_RING_FULL	\
   __TMS_USBPUMP_XHCI_EVENT_RING_FULL
# define USBPUMP_XHCI_MISSED_SERVICE	\
   __TMS_USBPUMP_XHCI_MISSED_SERVICE
# define USBPUMP_XHCI_COMMAND_RING_STOPPED	\
   __TMS_USBPUMP_XHCI_COMMAND_RING_STOPPED
# define USBPUMP_XHCI_COMMAND_ABORT	\
   __TMS_USBPUMP_XHCI_COMMAND_ABORT
# define USBPUMP_XHCI_STOPPED	\
   __TMS_USBPUMP_XHCI_STOPPED
# define USBPUMP_XHCI_STOPPED_INVALID_LEN	\
   __TMS_USBPUMP_XHCI_STOPPED_INVALID_LEN
# define USBPUMP_XHCI_CTRL_ABORT	\
   __TMS_USBPUMP_XHCI_CTRL_ABORT
# define USBPUMP_XHCI_L1_ERROR	\
   __TMS_USBPUMP_XHCI_L1_ERROR
# define USBPUMP_XHCI_TRANSFER_RING_BUSY	\
   __TMS_USBPUMP_XHCI_TRANSFER_RING_BUSY
# define USBPUMP_XHCI_ISO_OVERRUN	\
   __TMS_USBPUMP_XHCI_ISO_OVERRUN
# define USBPUMP_XHCI_EVENT_LOST	\
   __TMS_USBPUMP_XHCI_EVENT_LOST
# define USBPUMP_XHCI_UNDEFINED	\
   __TMS_USBPUMP_XHCI_UNDEFINED
# define USBPUMP_XHCI_INVALID_STREAM_ID	\
   __TMS_USBPUMP_XHCI_INVALID_STREAM_ID
# define USBPUMP_XHCI_SPLIT_ERROR	\
   __TMS_USBPUMP_XHCI_SPLIT_ERROR
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_xhci.h ****/
#endif /* _USBPUMP_XHCI_H_ */
