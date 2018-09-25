/* usbioctlcodes.h	Tue Nov 01 2016 21:53:37 chwon */

/*

Module:  usbioctlcodes.h

Function:
	Define the commonly-recognized IOCTL codes

Version:
	V3.19a	Tue Nov 01 2016 21:53:37 chwon	Edit level 39

Copyright notice:
	This file copyright (C) 2001-2013, 2015-2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	November 2001

Revision history:
   1.63d  Thu Nov  8 2001 08:24:38  tmm
	Module created.

   1.79a  7/11/2002  tmm
	Add USBPUMP_IOCTL_GET_UPLATFORM and USBPUMP_IOCTL_GET_OBJECT_ROOT.

   1.79b  8/9/2002  tmm
	Move the debug print IOCTL to usbioctl_debug.h
	Centralize the IOCTL class definitions.

   1.79b  Tue Oct 22 2002 12:50:00  maw
   	Add USBPUMP_IOCTL_GET_VIDPID and USBPUMP_IOCTL_GET_STRINGDESC

   1.79b  12/6/2002  chwon
	Use __TMS_CONCAT() instead of ##. This is more portable.

   1.81a  Wed Feb 19 2003 12:45:00 maw
   	Add USBPUMP_IOCTL_REMOTE_WAKEUP, add __TMS_USBPUMP_IOCTL_CORE2
   	to handle Ioctls with no IN or OUT parameters

   1.89b  Tue Aug 24 2004 17:12:21  chwon
   	Add USBPUMP_IOCTL_GET_VIDPIDMODE

   1.89e  Wed Nov 24 2004 13:50:39  chwon
   	Add USBPUMP_IOCTL_INIT_DEVMODEFN

   1.89e  Tue Dec 07 2004 15:44:22  chwon
   	Add USBPUMP_IOCTL_DEVICE_START

   1.91b  Fri Apr 15 2005 22:15:00 maw
   	Add USBPUMP_IOCTL_GET_TASKINFO

   1.91d  Tue May 31 2005 11:43:19 maw
   	Changed hTask to hProc in USBPUMP_IOCTL_GET_TASKINFO

   1.91h  2005-08-24  tmm
	Add USBPUMP_IOCTL_DCD_SESSION_REQUEST and
	USBPUMP_IOCTL_DCD_HNP_ENABLED, which are issued by core DataPump to
	initiate USB2.0 events defined by the USB OTG spec (but which are not
	really OTG related).

   1.93e  Wed Jan 11 2006 17:13:35  chwon
	Add USBPUMP_IOCTL_GET_CONFIG_POWER

   1.97f  Wed Dec 13 2006 18:36:25  chwon
	2164: add USBPUMP_IOCTL_GET_AUTO_REMOTE_WAKEUP_STATE and
	USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE

   1.97i  Thu Jun 07 2007 12:01:09  chwon
	2940: add USBPUMP_IOCTL_PROCESS_CONTROL_PACKET to send an
	unknown command to the UPLATFORM for processing.

   1.97j  Tue Nov 20 2007 08:37:36  tmm
	3505:  for virtual interrupt discipline, add
	USBPUMP_IOCTL_GET_INTERRUPT_SYSTEM.

   1.97k  Tue May 13 2008 11:06:54  chwon
	6100, 6110: add USBPUMP_IOCTL_GET_VIDPIDMODE_EX and create
	UsbDeviceIoctl_GetVidPidModeEx() library function

   1.97k  Tue Sep 23 2008 15:05:48  cccho
	6683: add USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT.

   2.01a  Mon Mar 09 2009 11:51:23  chwon
	7536: add USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN

   2.01a  Mon Sep 14 2009 16:57:43  chwon
	8556: Add USBPUMP_IOCTL_CALCULATE_SHA_1

   2.01a  Tue Oct 13 2009 16:27:58  chwon
	8556: Fix document error in USBPUMP_IOCTL_CALCULATE_SHA_1

   3.01d  Wed Oct 27 2010 12:12:52  cccho
	11425: add USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA and
	USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA.

   3.01d  Tue Dec 14 2010 23:11:57  maw
   	10093: Add USBPUMP_IOCTL_ALLOC_COMMON_BUFFER and
   	USBPUMP_IOCTL_FREE_COMMON_BUFFER

   3.01d  Wed Dec 15 2010 23:35:53  chwon
	10093: Re-add USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA and
	USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA definition.

   3.01d  Mon Jan 03 2011 12:32:09  cccho
	12155: add fValidateEndpoint flag for both the
	..._AUTO_REMOTE_WAKEUP_STATE ioctls.

   3.01f  Fri May 06 2011 12:50:16 rakesh
	6473: Added USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR

   3.01f  Thu Sep 08 2011 14:31:05  chwon
	14076: add USBPUMP_IOCTL_REGISTER_DUMP ioctl.

   3.01f  Sun Jan 08 2012 13:54:28  ssyang
	14706: add USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR ioctl.

   3.01f  Fri May 11 2012 10:48:12  chwon
	14821: added BESL related parameter to support LPM errata in the
	USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_ARG structure.

   3.01f  Thu Jun 28 2012 11:44:09  luxw
	15607: add USBPUMP_IOCTL_GET_PCI_CONFIG_DATA,
	USBPUMP_IOCTL_GET_PCI_INFO and USBPUMP_IOCTL_GET_XHCI_PORT_MAP.

   3.11a  Thu Aug 02 2012 12:04:14  chwon
	15685: 15684: add USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO.

   3.11a  Mon Aug 13 2012 12:17:36  chwon
	15804: Added USBPUMP_IOCTL_GET_VIDPIDMODE_V3.

   3.11a  Wed Aug 22 2012 10:23:44  wkkim
	15843: Add USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG.

   3.11b  Wed Oct 17 2012 14:12:38  chwon
	15804: Update document for USBPUMP_IOCTL_GET_VIDPIDMODE_V3: if client
	set fRenumberSingleConfig to TRUE, DataPump will renumber
	bConfigurationValue from 1 to NumConfigurations.

   3.11c  Thu Mar 14 2013 04:14:19  ssyang
	16893: Added USBPUMP_IOCTL_GET_CONTAINER_ID.

   3.15b  Mon Jun  1 2015 00:20:02  tmm
	19211: correct usages of "DataPump".

   3.19a  Tue Nov 01 2016 21:53:37  chwon
	20300: Add USBPUMP_IOCTL_DELAY_MICRO_SECONDS.

*/

#ifndef _USBIOCTLCODES_H_		/* prevent multiple includes */
#define _USBIOCTLCODES_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _USBUUID_H_
# include "usbuuid.h"
#endif

/****************************************************************************\
|
|	Define the IOCTL codes.  The actual parameters are defined later;
|	we keep these together so we can easily keep the numbers sequential.
|
\****************************************************************************/

/*
|| VOID can't be used as a parameter to a macro because MSC preprocessor
|| convers "VOID" to "void" and then concatenates this to __TMS_USBPUMP_IOCTL_
|| giving the symbol __TMS_USBPUMP_IOCTL_void that isn't defined
*/
#define __TMS_USBPUMP_IOCTL_CORE_VOID(i)				\
	__TMS_USBPUMP_IOCTL_VOID(					\
		__TMS_USBPUMP_IOCTL_CLASS_CORE,				\
		(i)							\
		)

#define __TMS_USBPUMP_IOCTL_CORE3(t, i, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_CORE,				\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_, NAME, _ARG)		\
		)



/* get VID & PID */
#define __TMS_USBPUMP_IOCTL_GET_VIDPID	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 0, GET_VIDPID)

/* get string  */
#define __TMS_USBPUMP_IOCTL_GET_STRING	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 1, GET_STRING)

/* get serial number */
#define __TMS_USBPUMP_IOCTL_GET_SERIALNO	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 2, GET_SERIALNO)

/* get mac address */
#define __TMS_USBPUMP_IOCTL_GET_MACADDRESS	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 3, GET_MACADDRESS)

/* get the UPLATFORM pointer */
#define __TMS_USBPUMP_IOCTL_GET_UPLATFORM	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 4, GET_UPLATFORM)

/* get the UPLATFORM pointer */
#define __TMS_USBPUMP_IOCTL_GET_OBJECT_ROOT	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 5, GET_OBJECT_ROOT)

/* add a child object */
#define __TMS_USBPUMP_IOCTL_ADD_CHILD	\
	__TMS_USBPUMP_IOCTL_CORE3(W, 6, ADD_CHILD)

/* add an object by name */
#define __TMS_USBPUMP_IOCTL_ADD_NAMED_OBJECT	\
	__TMS_USBPUMP_IOCTL_CORE3(W, 7, ADD_NAMED_OBJECT)

/* add a child object */
#define __TMS_USBPUMP_IOCTL_DELETE_CHILD	\
	__TMS_USBPUMP_IOCTL_CORE3(W, 8, DELETE_CHILD)

/* delete an object from name table */
#define __TMS_USBPUMP_IOCTL_DELETE_NAMED_OBJECT	\
	__TMS_USBPUMP_IOCTL_CORE3(W, 9, DELETE_NAMED_OBJECT)

/* find an object by name pattern */
#define __TMS_USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 10, ENUMERATE_MATCHING_OBJECTS)

/* get the UDEVICE pointer */
#define __TMS_USBPUMP_IOCTL_GET_UDEVICE	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 11, GET_UDEVICE)

/* request a remote wakeup */
#define __TMS_USBPUMP_IOCTL_REMOTE_WAKEUP	\
	__TMS_USBPUMP_IOCTL_CORE_VOID(12)

/* request that the device stop */
#define __TMS_USBPUMP_IOCTL_DEVICE_STOP	\
	__TMS_USBPUMP_IOCTL_CORE_VOID(13)

/* get VID & PID & mode */
#define __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 14, GET_VIDPIDMODE)

/* get VID & PID & mode */
/* it can have the same IOCTL index because the size will be different */
#define __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_EX	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 14, GET_VIDPIDMODE_EX)

/* get VID & PID & mode */
/* it can have the same IOCTL index because the size will be different */
#define __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_V3	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 14, GET_VIDPIDMODE_V3)

/* initialize device VID & PID & mode function */
#define __TMS_USBPUMP_IOCTL_INIT_DEVMODEFN	\
	__TMS_USBPUMP_IOCTL_CORE3(W, 15, INIT_DEVMODEFN)

/* request that the device start */
#define __TMS_USBPUMP_IOCTL_DEVICE_START	\
	__TMS_USBPUMP_IOCTL_CORE_VOID(16)

/* communicate session request to DCD or transceiver */
#define	__TMS_USBPUMP_IOCTL_DCD_SESSION_REQUEST	\
	__TMS_USBPUMP_IOCTL_CORE_VOID(17)

/* communicate HNP enabled status to DCD or transceiver */
#define	__TMS_USBPUMP_IOCTL_DCD_HNP_ENABLED	\
	__TMS_USBPUMP_IOCTL_CORE3(W, 18, DCD_HNP_ENABLED)

/* communicate test-mode status to DCD or transceiver */
#define	__TMS_USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED	\
	__TMS_USBPUMP_IOCTL_CORE3(W, 19, DCD_TEST_MODE_ENABLED)

/* get configuration power */
#define __TMS_USBPUMP_IOCTL_GET_CONFIG_POWER	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 20, GET_CONFIG_POWER)

/* get auto remote wakeup feature enable state */
#define __TMS_USBPUMP_IOCTL_GET_AUTO_REMOTE_WAKEUP_STATE	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 21, GET_AUTO_REMOTE_WAKEUP_STATE)

/* set auto remote wakeup feature enable state */
#define __TMS_USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE	\
	__TMS_USBPUMP_IOCTL_CORE3(W, 22, SET_AUTO_REMOTE_WAKEUP_STATE)

/* request control packet process */
#define __TMS_USBPUMP_IOCTL_PROCESS_CONTROL_PACKET	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 23, PROCESS_CONTROL_PACKET)

/* get the interrupt-system pointer */
#define __TMS_USBPUMP_IOCTL_GET_INTERRUPT_SYSTEM	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 24, GET_INTERRUPT_SYSTEM)

/* get link power management */
#define __TMS_USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 25, GET_LINK_POWER_MANAGEMENT)

/* find an class children object for given class object */
#define __TMS_USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 26, ENUMERATE_CLASS_CHILDREN)

/* calculate the SHA-1 */
#define __TMS_USBPUMP_IOCTL_CALCULATE_SHA_1	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 27, CALCULATE_SHA_1)

#define __TMS_USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA		\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 28, GET_PCI_POWER_MANAGEMENT_DATA)

#define __TMS_USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA	\
	__TMS_USBPUMP_IOCTL_CORE3(W, 29, SET_PCI_POWER_MANAGEMENT_DATA)

/* alloc and free common buffer */
#define	__TMS_USBPUMP_IOCTL_ALLOC_COMMON_BUFFER	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 30, ALLOC_COMMON_BUFFER)

#define	__TMS_USBPUMP_IOCTL_FREE_COMMON_BUFFER	\
	__TMS_USBPUMP_IOCTL_CORE3(W, 31, FREE_COMMON_BUFFER)

/* get override descriptor */
#define __TMS_USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 32, OVERRIDE_DESCRIPTOR)

/* dump chip register */
#define __TMS_USBPUMP_IOCTL_REGISTER_DUMP	\
	__TMS_USBPUMP_IOCTL_CORE_VOID(33)

/* override CDC network functional descriptors */
#define __TMS_USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 34, FILTER_FUNCTIONAL_DESCRIPTOR)

/* get PCI configuration space information */	
#define __TMS_USBPUMP_IOCTL_GET_PCI_INFO		\
	__TMS_USBPUMP_IOCTL_CORE3(R, 35, GET_PCI_INFO)
	
/* get PCI configuration space information */	
#define __TMS_USBPUMP_IOCTL_GET_PCI_CONFIG_DATA		\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 36, GET_PCI_CONFIG_DATA)

/* get XHCI port map information */	
#define __TMS_USBPUMP_IOCTL_GET_XHCI_PORT_MAP	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 37, GET_XHCI_PORT_MAP)

/* get Mass storage LUN information */	
#define __TMS_USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 38, GET_MASS_STORAGE_LUN_INFO)

/* get Abstract NIC packet loop test configuration */	
#define __TMS_USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG	\
	__TMS_USBPUMP_IOCTL_CORE3(R, 39, GET_ANIC_PKTLOOP_CONFIG)

/* get Container ID */	
#define __TMS_USBPUMP_IOCTL_GET_CONTAINER_ID	\
	__TMS_USBPUMP_IOCTL_CORE3(RW, 40, GET_CONTAINER_ID)

/* delay micro-seconds */
#define	__TMS_USBPUMP_IOCTL_DELAY_MICRO_SECONDS	\
	__TMS_USBPUMP_IOCTL_CORE3(W, 41, DELAY_MICRO_SECONDS)

/*

IOCTL:	USBPUMP_IOCTL_GET_VIDPID

Function:
	Obtain a VID, PID and device release number for this UDEVICE or
	UPLATFORM.

Input:
	pInParam	must be NULL.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_VIDPID_ARG structure.

	USBPUMP_IOCTL_GET_VIDPID_ARG has the following elements:

	UINT16	idVendor	-- OUT: vendor id

	UINT16	idProduct	-- OUT: product id

	UINT16	bcdDevice	-- OUT: device release number in BCD

Description:
	This IOCTL returns a VID, PID and device release number
	associated with the platform object.

	To fetch the information, issue the IOCTL and read the result.

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_GET_VIDPID fills in a buffer with
|| the serial number string.  The serial-number
|| is to be placed into the buffer in ANSI.  The actual
|| size of the buffer is carried in the IOCTL length code.
|| We make no provisions for a serial number longer than 32
|| characters, including the trailing NULL.
*/
__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_VIDPID_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_VIDPID_ARG)
	{
	__TMS_UINT16	idVendor;	/* OUT: vendor id */
	__TMS_UINT16	idProduct;	/* OUT: product id */
	__TMS_UINT16	bcdDevice;	/* OUT: device release number */
	};

/*

IOCTL:	USBPUMP_IOCTL_GET_STRING

Function:
	Obtain a UNICODE string for this UDEVICE or UPLATFORM.

Input:
	pInParam	points to a USBPUMP_IOCTL_GET_STRING_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_STRING_ARG structure.

	USBPUMP_IOCTL_STRING_ARG has the following elements:

	UCHAR		Index	-- IN: must be initialized to the string
				   index of the requested string

	UINT16		LangId	-- IN: must be initialized to the language
				   id of the requested string

	UINT16 *	pBuf;	-- IN: must be initialized to point to a
				   buffer that will receive the UNICODE string;
				   or else must be initialized to NULL.

	BYTES		nBuf;	-- IN: must be initialized to the number of
				   UNICODE-chars in the string buffer.

	BYTES		nStr;	-- OUT: set to the number of UNICODE-chars in the
				   resulting string.

Description:
	This IOCTL returns an UNICODE-string associated with the platform
	object.

	It can be used in two ways.  First, it can be used to determine the
	buffer size needed to hold the string.
	Second, it can be used to actually fetch the string into a caller-
	allocated buffer.

	To fetch the required size, set Index and LangId to the requested string.
	Then set pBuf and nBuf to zero and issue this request.  If the response
	is USBPUMP_RESULT_BUFFER_TOO_SMALL, then a string is available, and the
	number of UNICODE-chars is returned in nStr.

	To fetch the string, set pBuf to point to a buffer that is at least as
	large as indicated by nStr, and set nBuf to the size of that buffer,
	in UNICODE-chars.  Then issue the IOCTL.

Implementation Requirements:
	All implementations must set nStr to the number of UNICODE-chars in
	the string, unless the parameters are not well-formed.

Returns:
	USBPUMP_RESULT_BUFFER_TOO_SMALL:  this error code is returned
	if the input buffer pointer was NULL or if the buffer is too small
	to hold hold the string.  In this case nStr will be set to the
	number of UNICODE-chars required for the string.

	USBPUMP_RESULT_SUCCESS:  if the string was successfully returned.
	nStr is set to the number of UNICODE-chars in the string.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_GET_STRING fills in a buffer with
|| a UNICODE string.  The string is to be placed into the
|| buffer.  The actual size of the buffer is carried in
|| the IOCTL nStr code.
*/
__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_STRING_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_STRING_ARG)
	{
	__TMS_UCHAR		Index;	/* IN: string index */
	__TMS_UINT16		LangId;	/* IN: lanuage id */
	__TMS_UINT16 *		pBuf;	/* IN: pointer to buffer to fill */
	__TMS_BYTES		nBuf;	/* IN: size of buffer in UNICODE-chars */
	__TMS_BYTES		nStr;	/* OUT: actual number of UNICODE-chars in buffer */
	};



/*

IOCTL:	USBPUMP_IOCTL_GET_SERIALNO

Function:
	Obtain an appropriate serial number for this UDEVICE or UPLATFORM.

Input:
	pInParam	points to a USBPUMP_IOCTL_GET_SERIALNO_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_SERIALNO_ARG structure.

	USBPUMP_IOCTL_GETSERIALNO_ARG has the following elements:

	BYTES	nSerialNo;	-- OUT: set to strlen() of the resulting
				   serial number

	CHAR	*pSerialBuf;	-- IN: must be initialized to point to a
				   buffer that will receive the serial
				   number; or else must be initialized to
				   NULL.

	BYTES	nSerialBuf;	-- IN: must be initialized to the number of
				   bytes in the serial number buffer.

Description:
	This IOCTL, if directed to a UPLATFORM, returns the serial number
	associated with the platform object.  If directed to a UDEVICE, it
	returns the serial number associated with the UDEVICE, which is
	related to the UPLATORM serial number by appending a code based on
	the USB device port index.

	It can be used in two ways.  First, it can be used to determine the
	number of bytes needed to hold the serial number.  Second, it can
	be used to actually fetch the serial number into a caller-allocated
	buffer.

	To fetch the required size, set pSerialBuf and nSerialBuf to zero, and
	issue this request.  If the response is
	USBPUMP_RESULT_BUFFER_TOO_SMALL, then a serial number is available,
	and the serial number size is returned in nSerialNo.  (Note that
	the size is given exclusive of any trailing '\0'.)

	To fetch the serial number, set pSerialBuf to point to a buffer that
	is at least 1 byte larger than the indicated nSerialNo, and set
	nSerialBuf to the size of that buffer, in bytes.  Then issue the
	IOCTL.

Implementation Requirements:
	All implementations must set nSerialNo to the number of bytes in
	the serial number, unless the parameters are not well-formed.

Returns:
	USBPUMP_RESULT_BUFFER_TOO_SMALL:  this error code is returned
	if the input buffer pointer was NULL or if the buffer is too small
	to hold hold the entire serial number plus a trailing NULL.  In this
	case nSerialNo will be set to the number of bytes required for the
	serial number (NOT counting the trailing NULL).

	USBPUMP_RESULT_SUCCESS:  if the serial number was successfully
	returned.  nSerialBuf is set to the number of bytes in the serial
	number, NOT counting the trailing NULL.)

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_GET_SERIALNO fills in a buffer with
|| the serial number string.  The serial-number
|| is to be placed into the buffer in ANSI.  The actual
|| size of the buffer is carried in the IOCTL length code.
|| We make no provisions for a serial number longer than 32
|| characters, including the trailing NULL.
*/
__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_SERIALNO_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_SERIALNO_ARG)
	{
	__TMS_BYTES	nSerialNo;	/* OUT: strlen() of SerialNo */
	__TMS_CHAR	*pSerialBuf;	/* IN: pointer to buffer to fill */
	__TMS_BYTES	nSerialBuf;	/* IN: size of serial buffer */
	};

/*

IOCTL:	USBPUMP_IOCTL_GET_MACADDRESS

Function:
	Enumerate the MAC addresses associated with this platform.

Input:
	pInParam	points to a USBPUMP_IOCTL_GET_MACADDRESS_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_MACADDRESS_ARG structure.

	USBPUMP_IOCTL_GET_MACADDRESS_ARG has the following elements:

	UINT	iMacAddr;	-- IN: indicates the desired MAC address.
				   0 --> first, 1 --> second, etc.

	UCHAR	ucMacAddr[6];	-- OUT: if successful, set to the i'th
				   MAC address.

Description:
	This IOCTL, if directed to a UPLATFORM, UDEVICE or object layerd
	upon the same, returns the i'th Ethernet MAC address associated with
	the UPLATFORM.

	This IOCTL gives functions a way to discover all the mac addresses
	associated with this platform.  It doesn't address the problem of
	how a mac address is allocated to a given function.

	To fetch the i'th MAC address, prepare a
	USBPUMP_IOCTL_GET_MACADDRESS_ARG structure.  Set iMacAddr to i.
	Then issue the IOCTL.  If the response is
	USBPUMP_RESULT_NO_MORE, then there is no iMacAddr i (and none
	for any j >= i).  If the response is USBPUMP_RESULT_SUCCESS then
	ucMacAddr[] will be initialized to the value of that MAC address.

	Callers should not assume that the assigned MAC addresses are
	sequential (though they commonly will be).

Returns:
	USBPUMP_RESULT_SUCCESS:  ucMacAddr[] in the result buffer is set
	to the i'th MAC address.

	USBPUMP_RESULT_NO_MORE:  there are no more valid MAC indices
	for all values in [iMacAddr..UINT_MAX].

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_GET_MACADDRESS fills in a buffer with
|| the mac-address string.  The mac-address is placed
|| into the actual IOCTL buffer in binary.
*/
__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_MACADDRESS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_MACADDRESS_ARG)
	{
	__TMS_UINT	iMacAddr;	/* IN: which mac addr do you want? */
	__TMS_UINT8	ucMacAddr[6];	/* OUT: the MAC address */
	};

/*

IOCTL:	USBPUMP_IOCTL_GET_UPLATFORM

Index:	Type:	USBPUMP_IOCTL_GET_UPLATFORM_ARG

Function:
	Return a UPLATFORM pointer.

Input:
	pInParam	must be NULL.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_UPLATFORM_ARG structure.

	USBPUMP_IOCTL_GET_UPLATFORM_ARG has the following elements:

	UPLATFORM *pPlatform;	-- OUT: set to a valid UPLATFORM pointer.

Description:
	This IOCTL, if directed to a UPLATFORM, UDEVICE or object layered
	upon the same, returns the UPLATFORM pointer.  This is very useful
	when working at a high level in the object hierarchy.

	Callers should be aware that in special circumstances there may
	be multiple UPLATFORMs associated with a USBPUMP instance; this
	might be done in case different I/O methods or interrupt methods
	were needed for different chips.

	In this case, however, the basic UPLATFORM pMalloc, etc., functions
	will all be the same, so it doesn't much matter which UPLATFORM
	you get.  If you need a UPLATFORM that is associated with a given
	UDEVICE, route the request via that UDEVICE>

Returns:
	USBPUMP_RESULT_SUCCESS:  pPlatform is set to the UPLATFORM pointer.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_GET_UPLATFORM fills in a buffer with
|| a pointer to a UPLATFORM.
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_UPLATFORM_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_UPLATFORM_ARG)
	{
	__TMS_UPLATFORM	*pPlatform;	/* OUT: the platform pointer */
	};

/*

IOCTL:	USBPUMP_IOCTL_GET_OBJECT_ROOT

Index:	Type:	USBPUMP_IOCTL_GET_OBJECT_ROOT_ARG

Function:
	Return a USBPUMP_OBJECT_ROOT pointer.

Input:
	pInParam	must be NULL.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_OBJECT_ROOT_ARG structure.

	USBPUMP_IOCTL_GET_OBJECT_ROOT_ARG has the following elements:

	USBPUMP_OBJECT_ROOT *pRoot;	-- OUT: set to the
					   USBPUMP_OBJECT_ROOT pointer.

Description:
	This IOCTL, if directed to a USBPUMP_OBJECT_ROOT, UDEVICE or object
	layered upon the same, returns the USBPUMP_OBJECT_ROOT pointer.
	This is very useful when working at a high level in the object
	hierarchy.

Returns:
	USBPUMP_RESULT_SUCCESS:  pRoot is set to the USBPUMP_OBJECT_ROOT
	pointer.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_GET_OBJECT_ROOT fills in a buffer with
|| a pointer to the root-object.
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_OBJECT_ROOT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_OBJECT_ROOT_ARG)
	{
	__TMS_USBPUMP_OBJECT_ROOT	*pRoot;	/* OUT: the root pointer */
	};

/*

IOCTL:	USBPUMP_IOCTL_ADD_CHILD

Index:	Type:	USBPUMP_IOCTL_ADD_CHILD_ARG
	Name:	USBPUMP_IOCTL_ADD_CHILD_ARG_SETUP_V1()

Function:
	Add an instance structure to a class.

Input:
	pInParam	points to a USBPUMP_IOCTL_ADD_CHILD_ARG
			structure.
	pOutParam	must be NULL.

	USBPUMP_IOCTL_ADD_CHILD_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pNewChild;
		IN: points to the new child object.

Description:
	This IOCTL, if directed to a USBPUMP_OBJECT_ROOT or other
	"class-type" object, registers the object as a new child.

Returns:
	USBPUMP_RESULT_SUCCESS:  child was registered.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_ADD_CHILD_ARG_SETUP_V1(
		USBPUMP_IOCTL_ADD_CHILD_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pNewChild
		);

	This macro will correctly initialize a .._ADD_CHILD_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	In V1.8, we only have one class:  the root class.  All objects are
	instances of that class.  However we're architected to have a
	class hierarchy as well as an object hierarchy.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_ADD_CHILD_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_ADD_CHILD_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER	*pNewChild;	/* IN: new child */
	};

/* the setup for the .._ADD_CHILD_ARG struct */
#define __TMS_USBPUMP_IOCTL_ADD_CHILD_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pNewChild							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_ADD_CHILD_ARG * __TMS_CONST			\
		__pArg = (pArg);					\
	__pArg->pNewChild = (ARG_pNewChild);				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_ADD_NAMED_OBJECT

Index:	Type:	USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG
	Name:	USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG_SETUP_V1()

Function:
	Add a named object to the class object directory.

Input:
	pInParam	points to a USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG
			structure.
	pOutParam	must be NULL.

	USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pNewObject;
		IN: points to the new named object.

Description:
	This IOCTL, if directed to a USBPUMP_OBJECT_ROOT or other
	"class-type" object, places the object into the appropriate (closest)
	directory, and assigned the appropriate instance number to the
	object.

Returns:
	USBPUMP_RESULT_SUCCESS:  child was registered.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG_SETUP_V1(
		USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pNewChild
		);

	This macro will correctly initialize a .._ADD_NAMED_OBJECT_ARG
	structure.  If the layout of the argument structure changes in
	the future, MCCI will adjust this macro to initialize the new
	elements in a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER	*pNewObject;	/* IN: new item to
							|| add to directory
							*/
	};

/* the setup for the .._ADD_NAMED_OBJECT_ARG struct */
#define __TMS_USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pNewObject							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pNewObject = (ARG_pNewObject);				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DELETE_CHILD

Index:	Type:	USBPUMP_IOCTL_DELETE_CHILD_ARG
	Name:	USBPUMP_IOCTL_DELETE_CHILD_ARG_SETUP_V1()

Function:
	Delete a child object from its list.

Input:
	pInParam	points to a USBPUMP_IOCTL_DELETE_CHILD_ARG
			structure.
	pOutParam	must be NULL.

	USBPUMP_IOCTL_DELETE_CHILD_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pChild;
		IN: points to the child object to be deleted.

Description:
	This IOCTL, if directed to the class parent of an object,
	deletes the object from the list of instances of that class.

Returns:
	USBPUMP_RESULT_SUCCESS:  child was deregistered.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_DELETE_CHILD_ARG_SETUP_V1(
		USBPUMP_IOCTL_DELETE_CHILD_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pChildToDelete
		);

	This macro will correctly initialize a .._DELETE_CHILD_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DELETE_CHILD_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DELETE_CHILD_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER	*pChild;	/* IN: new child */
	};

/* the setup for the .._DELETE_CHILD_ARG struct */
#define __TMS_USBPUMP_IOCTL_DELETE_CHILD_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pChild							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DELETE_CHILD_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pChild = (ARG_pChild);					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DELETE_NAMED_OBJECT

Index:	Type:	USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG
	Name:	USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG_SETUP_V1()

Function:
	Remove a named object from the class object directory.

Input:
	pInParam	points to a USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG
			structure.
	pOutParam	must be NULL.

	USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object to be deleted from the dictionary

Description:
	This IOCTL, if directed to an objects class-parent, removes the
	object from the appropriate directory.

Returns:
	USBPUMP_RESULT_SUCCESS:  child was registered.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG_SETUP_V1(
		USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObject
		);

	This macro will correctly initialize a .._DELETE_NAMED_OBJECT_ARG
	structure.  If the layout of the argument structure changes in
	the future, MCCI will adjust this macro to initialize the new
	elements in a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER	*pObject;	/* IN: new item to
							|| add to directory
							*/
	};

/* the setup for the .._DELETE_NAMED_OBJECT_ARG struct */
#define __TMS_USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS

Index:	Type:	USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG
	Name:	USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG_SETUP_V1()

Function:
	Find an object by name pattern.

Input:
	pInParam	points to a USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG structure.

	USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pThisObject;
		IN:  starting point.  if NULL, starts at the beginning; if
		     non-NULL, this is the last value returned, and this
		     returns the next match.

	CONST TEXT *pPattern;
		IN:  pointer to the pattern string.

	BYTES nPattern;
		IN:  length of the pattern string.

	USBPUMP_OBJECT_HEADER *pNextObject;
		OUT:  next matching object.

Description:
	This IOCTL, if directed to an object's class-parent, can be used
	to traves the "appropriate" directory, looking for matches.  The
	pattern string is as defined for UsbPumpLib_MatchPattern().

Returns:
	USBPUMP_RESULT_SUCCESS:  the match is in pNextObject.
	USBPUMP_RESULT_NO_MORE:	 no more were found; pNextObject is NULL.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG_SETUP_V1(
		USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pThisObject,
		CONST TEXT *pPattern,
		BYTES nPattern
		);

	This macro will correctly initialize a .._ENUMERATE_MATCHING_OBJECTS_ARG
	structure.  If the layout of the argument structure changes in
	the future, MCCI will adjust this macro to initialize the new
	elements in a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER	*pThisObject;	/* IN: where to
							|| start searching.
							*/
	__TMS_CONST __TMS_TEXT		*pPattern;	/* IN: pattern */
	__TMS_BYTES			nPattern;	/* IN: length */
	__TMS_USBPUMP_OBJECT_HEADER	*pNextObject;	/* OUT: match */
	};

/* the setup for the .._ENUMERATE_MATCHING_OBJECTS_ARG struct */
#define __TMS_USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pThisObject,						\
	ARG_pPattern,							\
	ARG_nPattern							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pThisObject = (ARG_pThisObject);			\
	__pArg->pPattern = (ARG_pPattern);				\
	__pArg->nPattern = (ARG_nPattern);				\
	__pArg->pNextObject = __TMS_NULL;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_GET_UDEVICE

Index:	Type:	USBPUMP_IOCTL_GET_UDEVICE_ARG

Function:
	Return a UDEVICE pointer.

Input:
	pInParam	must be NULL.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_UDEVICE_ARG structure.

	USBPUMP_IOCTL_GET_UDEVICE_ARG has the following elements:

	UDEVICE *pDevice;	-- OUT: set to a valid UDEVICE pointer.

Description:
	This IOCTL, if directed to a UDEVICE, or object layered
	upon the same, returns the UDEVICE pointer.  This is very useful
	when working at a high level in the object hierarchy, particularly
	if diagnostic output is to be produced.

Returns:
	USBPUMP_RESULT_SUCCESS:  pDevice is set to the UDEVICE pointer.
	USBPUMP_RESULT_NOT_CLAIMED:  the object you sent the IOCTL to is
				not layered on a UDEVICE.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_GET_UDEVICE fills in a buffer with
|| a pointer to a UDEVICE.
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_UDEVICE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_UDEVICE_ARG)
	{
	__TMS_UDEVICE	*pDevice;	/* OUT: the platform pointer */
	};


/*

IOCTL:	USBPUMP_IOCTL_REMOTE_WAKEUP

Index:	Type:	USBPUMP_IOCTL_REMOTE_WAKEUP_ARG

Function:
	Start Resume signalling on USB

Input:
	pInParam	points to a USBPUMP_IOCTL_REMOTE_WAKEUP_ARG
			structure (always NULL).
	pOutParam	points to a USBPUMP_IOCTL_REMOTE_WAKEUP_ARG
			structure (always NULL).

	USBPUMP_IOCTL_REMOTE_WAKEUP_ARG has the following elements:

	VOID *	pDummy;	-- IN/OUT: not used.

Description:
	This IOCTL, if directed to a UDEVICE, or object layered
	upon the same, causes the physical layer to start resume signalling
	on USB.

Returns:
	USBPUMP_RESULT_SUCCESS:      Resume initiated
	USBPUMP_RESULT_NOT_CLAIMED:  the object you sent the IOCTL to is
				not layered on a UDEVICE.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_REMOTE_WAKEUP has no in or out parameter
*/
/*
__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_REMOTE_WAKEUP_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_REMOTE_WAKEUP_ARG)
	{
	__TMS_VOID	*pDummy;
	};
*/


/*

IOCTL:	USBPUMP_IOCTL_DEVICE_STOP

Index:	Type:	USBPUMP_IOCTL_DEVICE_STOP_ARG

Function:
	Stop USB device

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_STOP_ARG
			structure (always NULL).
	pOutParam	points to a USBPUMP_IOCTL_DEVICE_STOP_ARG
			structure (always NULL).

	USBPUMP_IOCTL_DEVICE_STOP_ARG has the following elements:

	VOID *	pDummy;	-- IN/OUT: not used.

Description:
	This IOCTL, if directed to a UDEVICE, or object layered
	upon the same, causes the DataPump to stop

Returns:
	USBPUMP_RESULT_SUCCESS:      Resume initiated
	USBPUMP_RESULT_NOT_CLAIMED:  the object you sent the IOCTL to is
				not layered on a UDEVICE.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_DEVICE_STOP has no in or out parameter
*/
/*
__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_STOP_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_STOP_ARG)
	{
	__TMS_VOID	*pDummy;
	};
*/


/*

IOCTL:	USBPUMP_IOCTL_GET_VIDPIDMODE

Function:
	Obtain a VID, PID, device release number and device mode for this
	UDEVICE or UPLATFORM.

Input:
	pInParam	must be NULL.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_VIDPIDMODE_ARG structure.

	USBPUMP_IOCTL_GET_VIDPIDMODE_ARG has the following elements:

	UINT16	idVendor	-- OUT: vendor id

	UINT16	idProduct	-- OUT: product id

	UINT16	bcdDevice	-- OUT: device release number in BCD

	INT16	sDeviceClass	-- OUT: device class (if -1, don't change)

	INT16	sDeviceSubClass	-- OUT: device subclass (if -1, don't change)

	INT16	sDeviceProtocol	-- OUT: device protocol (if -1, don't change)

	UINT32	ulMode		-- OUT: ulMode is used as the bConfigurationValue

Description:
	This IOCTL returns a VID, PID, device release number, device class,
	device subclass, protocol, and device mode associated with the
	platform object.

	To fetch the information, issue the IOCTL and read the result.

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_VIDPIDMODE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_VIDPIDMODE_ARG)
	{
	__TMS_UINT16	idVendor;	/* OUT: vendor id */
	__TMS_UINT16	idProduct;	/* OUT: product id */
	__TMS_UINT16	bcdDevice;	/* OUT: device release number */
	__TMS_INT16	sDeviceClass;	/* OUT: device class */
	__TMS_INT16	sDeviceSubClass;/* OUT: device subclass */
	__TMS_INT16	sDeviceProtocol;/* OUT: device protocol */
	__TMS_UINT32	ulMode;		/* OUT: device mode as the bConfigurationValue */
	};

#define	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_INIT_V1(			\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode							\
	)								\
	{								\
	(a_idVendor),							\
	(a_idProduct),							\
	(a_bcdDevice),							\
	(a_sDeviceClass),				        	\
	(a_sDeviceSubClass),						\
	(a_sDeviceProtocol),						\
	(a_ulMode)							\
	}

#define	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_SETUP_V1(			\
	a_pArg,								\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode							\
	)								\
   do	{								\
	(a_pArg)->idVendor = a_idVendor;				\
	(a_pArg)->idProduct = a_idProduct;				\
	(a_pArg)->bcdDevice = a_bcdDevice;				\
	(a_pArg)->sDeviceClass = a_sDeviceClass;		        \
	(a_pArg)->sDeviceSubClass = a_sDeviceSubClass;			\
	(a_pArg)->sDeviceProtocol = a_sDeviceProtocol;			\
	(a_pArg)->ulMode = a_ulMode;					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_GET_VIDPIDMODE_EX

Function:
	Obtain a VID, PID, device release number, device mode, and extra
	information for this UDEVICE or UPLATFORM.

Input:
	pInParam	must be NULL.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_VIDPIDMODE_EX_ARG structure.

	USBPUMP_IOCTL_GET_VIDPIDMODE_EX_ARG has the following elements:

	USBPUMP_IOCTL_GET_VIDPIDMODE_ARG	V1;
		OUT: result of USBPUMP_IOCTL_GET_VIDPIDMODE

	BOOL	fRenumberSingleConfig;
		OUT: renumber bConfigurationValue to 1 for the selected config
		descriptor selected by ulMode

	USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE OverrideRemoteWakeup;
		OUT: override bmAttributes.RemoteWakeup

Description:
	This IOCTL returns a VID, PID, device release number, device class,
	device subclass, protocol, and device mode associated with the
	platform object.

	To fetch the information, issue the IOCTL and read the result.

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_ARG(USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE, UINT8);

#define __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE_NONE	0
#define __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE_ON	1
#define __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE_OFF	2

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_VIDPIDMODE_EX_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_VIDPIDMODE_EX_ARG)
	{
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_ARG	V1;

	__TMS_BOOL				fRenumberSingleConfig;
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE
						OverrideRemoteWakeup;
	};

#define	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_EX_INIT_V1(			\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup						\
	)								\
	{								\
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_INIT_V1(			\
		a_idVendor,						\
		a_idProduct,						\
		a_bcdDevice,						\
		a_sDeviceClass,						\
		a_sDeviceSubClass,					\
		a_sDeviceProtocol,					\
		a_ulMode						\
		),							\
	(a_fRenumberSingleConfig),					\
	(a_OverrideRemoteWakeup)					\
	}

#define	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_EX_SETUP_V1(			\
	a_pArg,								\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_SETUP_V1(			\
		&(a_pArg)->V1,						\
		a_idVendor,						\
		a_idProduct,						\
		a_bcdDevice,						\
		a_sDeviceClass,						\
		a_sDeviceSubClass,					\
		a_sDeviceProtocol,					\
		a_ulMode						\
		);							\
	(a_pArg)->fRenumberSingleConfig = a_fRenumberSingleConfig;	\
	(a_pArg)->OverrideRemoteWakeup = a_OverrideRemoteWakeup;	\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_GET_VIDPIDMODE_V3

Function:
	Obtain a VID, PID, device release number, device mode, and extra
	information for this UDEVICE or UPLATFORM.

Input:
	pInParam	must be NULL.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_VIDPIDMODE_V3_ARG structure.

	USBPUMP_IOCTL_GET_VIDPIDMODE_V3_ARG has the following elements:

	USBPUMP_IOCTL_GET_VIDPIDMODE_EX_ARG	Ex;
		OUT: result of USBPUMP_IOCTL_GET_VIDPIDMODE_EX

	UINT32	NumConfigurations;
		OUT: number of configurations from starting configuration
		specified in the ulMode.  For example, if NumConfigurations = 2
		and ulMode = 2, then DataPump will return two configurations
		from start configuration with bConfigurationValue = 2.
		This value is only valid if ulMode is not zero.
		If client return NumConfigurations > 1 and client set
		Ex.fRenumberSingleConfig to TRUE, DataPump will renumber
		bConfigurationValue from 1 to NumConfigurations.

Description:
	This IOCTL returns a VID, PID, device release number, device class,
	device subclass, protocol, and device mode associated with the
	platform object.

	To fetch the information, issue the IOCTL and read the result.

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_VIDPIDMODE_V3_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_VIDPIDMODE_V3_ARG)
	{
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_EX_ARG	Ex;

	__TMS_UINT32					NumConfigurations;
	};

#define	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_V3_INIT_V1(			\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup,						\
	a_NumConfigurations						\
	)								\
	{								\
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_EX_INIT_V1(			\
		a_idVendor,						\
		a_idProduct,						\
		a_bcdDevice,						\
		a_sDeviceClass,						\
		a_sDeviceSubClass,					\
		a_sDeviceProtocol,					\
		a_ulMode,						\
		a_fRenumberSingleConfig,				\
		a_OverrideRemoteWakeup					\
		),							\
	(a_NumConfigurations)						\
	}

#define	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_V3_SETUP_V1(			\
	a_pArg,								\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup,						\
	a_NumConfigurations						\
	)								\
    do	{								\
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_EX_SETUP_V1(			\
		&(a_pArg)->Ex,						\
		a_idVendor,						\
		a_idProduct,						\
		a_bcdDevice,						\
		a_sDeviceClass,						\
		a_sDeviceSubClass,					\
		a_sDeviceProtocol,					\
		a_ulMode,						\
		a_fRenumberSingleConfig,				\
		a_OverrideRemoteWakeup					\
		);							\
	(a_pArg)->NumConfigurations = a_NumConfigurations;		\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_INIT_DEVMODEFN

Index:	Type:	USBPUMP_IOCTL_INIT_DEVMODEFN_ARG
	Name:	USBPUMP_IOCTL_INIT_DEVMODEFN_ARG_SETUP_V1()

Function:
	Initialize getting device mode function

Input:
	pInParam	points to a USBPUMP_IOCTL_INIT_DEVMODEFN_ARG
			structure.
	pOutParam	must be NULL.

	USBPUMP_IOCTL_INIT_DEVMODEFN_ARG has the following elements:

	USBPUMP_GET_DEVICEMODE_FN *pDeviceModeFn;
		IN: function pointer to get device mode.

	VOID *pDeviceModeCtx;
		IN: context of pDeviceModeFn.

Description:
	This IOCTL registers the function pointer to get device mode into the
	platform layer.

Returns:
	USBPUMP_RESULT_SUCCESS:  function pointer was registered.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_INIT_DEVMODEFN_ARG_SETUP_V1(
		USBPUMP_IOCTL_INIT_DEVMODEFN_ARG *pArg,
		USBPUMP_GET_DEVICEMODE_FN *pDeviceModeFn,
		VOID *pDeviceModeCtx
		);

	This macro will correctly initialize a .._INIT_DEVMODEFN_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

*/

/*
|| USBPUMP_IOCTL_GET_DEVICEMODE handling function prototype
*/
__TMS_FNTYPE_DEF(
	USBPUMP_GET_DEVICEMODE_FN,
	__TMS_VOID,
	(
	__TMS_VOID *					/* pContext */,
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_ARG *	/* pVidPidMode */
	));

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_INIT_DEVMODEFN_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_INIT_DEVMODEFN_ARG)
	{
	__TMS_USBPUMP_GET_DEVICEMODE_FN *pDeviceModeFn;	/* IN: function */
	__TMS_VOID *pDeviceModeCtx;			/* IN: context */
	};

/* the setup for the .._INIT_DEVMODEFN_ARG struct */
#define __TMS_USBPUMP_IOCTL_INIT_DEVMODEFN_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pDeviceModeFn,						\
	ARG_pDeviceModeCtx						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_INIT_DEVMODEFN_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pDeviceModeFn = (ARG_pDeviceModeFn);			\
	__pArg->pDeviceModeCtx = (ARG_pDeviceModeCtx);			\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_START

Function:
	Start USB device

Input:
	pInParam	always NULL.
	pOutParam	always NULL.

Description:
	This IOCTL, if directed to a UDEVICE, or object layered
	upon the same, causes the DataPump to start

Returns:
	USBPUMP_RESULT_SUCCESS:      Resume initiated
	USBPUMP_RESULT_NOT_CLAIMED:  the object you sent the IOCTL to is
				not layered on a UDEVICE.

	Other failure codes are possible if the request is malformed.

*/

/*

IOCTL:	USBPUMP_IOCTL_DCD_SESSION_REQUEST

Function:
	Ask the transceiver state machine to do an SRP if appropriate.

Input:
	pInParam	always NULL
	pOutParam	always NULL

Description:
	This IOCTL tells the lower layers that the system software
	wants to run a B-bus session, but doesn't want to be host.  This
	will cause SRP, etc., as needed.  This is a one-shot; the FSM
	will automatically reset this request after initiating SRP.

	Although this is defined by the OTG specification, it can be used
	on non "OTG" systems (i.e., things that are only peripherals).
	So it's part of the core definition set; but it's usually only
	implemented in OTG systems.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success -- which will always be
	the result unless there's some kind of system plumbing problem.
	The SRP (if any) proceeds asynchronously.

Notes:
	This IOCTL has no effect if the device is not a B device.

	SRP should not be attempted unless the device has
	an OTG descriptor that says the device actually does SRP.

	This IOCTL should only be issued by the DataPump core (which is
	supposed to know whether the device is allowed to do SRP.

*/

/*

IOCTL:	USBPUMP_IOCTL_DCD_HNP_ENABLED

Index:	Type:	USBPUMP_IOCTL_DCD_HNP_ENABLED_ARG
	Type:	PUSBPUMP_IOCTL_DCD_HNP_ENABLED_ARG
	Name:	USBPUMP_IOCTL_DCD_HNP_ENABLED_ARG_SETUP_V1()

Function:
	Informs the OTG FSM whether the B-device things that HNP is
	presently enabled.

Input:
	pInParam	points to a USBPUMP_IOCTL_DCD_HNP_ENABLED_ARG
			structure

	pOutParam	always NULL

	USBPUMP_IOCTL_DCD_HNP_ENABLED_ARG has the following input fields:

	BOOL fState;
		Logically TRUE means that HNP is now enabled.
		Logically FALSE means that HNP is now disabled.

Output:
	No output parameters.

Description:
	The core DataPump watches for SET_FEATURE(HNP), and if it
	sees it, it issues this IOCTL to the lower level transceiver.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS:	the lower level knows what to do
					with HNP.
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED: the lower level doesn't know what to
					do, or the targeted object isn't
					a DCD
	USBPUMP_IOCTL_RESULT_NOT_SUPPORTED:
					similar to not claimed, except that
					it was claimed but HNP is not
					supported for some reason.

Setup Macro:
	VOID USBPUMP_IOCTL_DCD_HNP_ENABLED_ARG_SETUP_V1(
		USBPUMP_IOCTL_DCD_HNP_ENABLED_ARG *pArg,
		BOOL fState
		);

	Initializes the parameter block for the IOCTL.  MCCI guarantees
	that if we add a parameter to this IOCTL, we'll do it in an upward
	compatible function so that old source code using the V1 macro
	won't have to be edited (though it may have to be recompiled).  New
	functionality will be available via a V2, etc. macro.

Notes:
	This IOCTL is to be issued only by the core DataPump, not by
	client protocols.  It is typically recieved (if at all) by the
	DCD or transciever object.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_DCD_HNP_ENABLED_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_DCD_HNP_ENABLED_ARG)
	{
	__TMS_BOOL	fState;
	};

#define	__TMS_USBPUMP_IOCTL_DCD_HNP_ENABLED_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fState							\
	)								\
   do	{								\
	(a_pArg)->fState = (a_fState);					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED

Index:	Type:	USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG
	Type:	PUSBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG
	Name:	USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG_SETUP_V1()

Function:
	Informs the DCD and transceiver that a test mode has been enabled.

Input:
	pInParam	points to a USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG
			structure

	pOutParam	always NULL

	USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG has the following input fields:

	UDEVICE *pDevice;
		Pointer to the UDEVICE in question.  Maybe helpful for
		transceivers.

	UINT8 bTestMode;
		Logically TRUE means that HNP is now enabled.
		Logically FALSE means that HNP is now disabled.

Output:
	No output parameters.

Description:
	The core DataPump watches for SET_FEATURE(HNP), and if it
	sees it, it issues this IOCTL to the lower level transceiver.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS:	the lower level knows what to do
					with test mode and has set things
					up for the test mode.

	USBPUMP_IOCTL_RESULT_NOT_CLAIMED: the lower level doesn't know what to
					do, or the targeted object isn't
					a DCD

	USBPUMP_IOCTL_RESULT_NOT_SUPPORTED:
					The vendor-specific test mode is
					not supported for this device.

Setup Macro:
	VOID USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG_SETUP_V1(
		USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG *pArg,
		UDEVICE *pDevice,
		UINT8 bTestMode
		);

	Initializes the parameter block for the IOCTL.  MCCI guarantees
	that if we add a parameter to this IOCTL, we'll do it in an upward
	compatible function so that old source code using the V1 macro
	won't have to be edited (though it may have to be recompiled).  New
	functionality will be available via a V2, etc. macro.

Notes:
	This IOCTL is to be issued only by the core DataPump, not by
	client protocols.  It is typically recieved (if at all) by the
	DCD or transciever object.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG)
	{
	__TMS_UDEVICE	*pDevice;
	__TMS_UINT8	bTestMode;
	};

#define	__TMS_USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pDevice,							\
	a_bTestMode							\
	)								\
   do	{								\
	(a_pArg)->pDevice = (a_pDevice);				\
	(a_pArg)->bTestMode = (a_bTestMode);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_GET_CONFIG_POWER

Function:
	Obtain a max power consumption of the config descriptor for this
	UDEVICE or UPLATFORM.

Input:
	pInParam	must be NULL.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_CONFIG_POWER_ARG structure.

	USBPUMP_IOCTL_GET_CONFIG_POWER_ARG has the following elements:

	UINT8	MaxPower	-- OUT: max power consumption

Description:
	This IOCTL returns a max power consumption of the config descriptor
	associated with the platform object.

	To fetch the information, issue the IOCTL and read the result.

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_CONFIG_POWER_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_CONFIG_POWER_ARG)
	{
	__TMS_UINT8	MaxPower;	/* OUT: max power consumption */
	};

/*

IOCTL:	USBPUMP_IOCTL_GET_AUTO_REMOTE_WAKEUP_STATE

Function:
	Obtain a automatic remote wakeup feature state.

Input:
	pInParam	must be NULL.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_AUTO_REMOTE_WAKEUP_STATE_ARG structure.

	USBPUMP_IOCTL_GET_AUTO_REMOTE_WAKEUP_STATE_ARG has the following elements:

	BOOL	fAutoRemoteWakeup	-- OUT: auto remote wakeup enable state
	BOOL	fSuspendState		-- OUT: current suspend state
	BOOL	fValidateEndpoint	-- OUT: validation of endpoints

Description:
	This IOCTL returns a current auto remote wakeup feature state.

	To fetch the information, issue the IOCTL and read the result.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_AUTO_REMOTE_WAKEUP_STATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_AUTO_REMOTE_WAKEUP_STATE_ARG)
	{
	__TMS_BOOL	fAutoRemoteWakeup;	/* OUT: auto remote wakeup enable state */
	__TMS_BOOL	fSuspendState;		/* OUT: current suspend state */
	__TMS_BOOL	fValidateEndpoint;	/* OUT: validation of endpoints */
	};

/*

IOCTL:	USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE

Function:
	Set a automatic remote wakeup feature state.

Input:
	pInParam	points to a USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG structure.

	USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG has the following elements:

	BOOL	fAutoRemoteWakeup	-- IN: auto remote wakeup enable state
	BOOL	fValidateEndpoint	-- IN: validate endpoint

Description:
	This IOCTL changes current auto remote wakeup feature state.

	To fetch the information, issue the IOCTL and read the result.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG)
	{
	__TMS_BOOL	fAutoRemoteWakeup;	/* IN: auto remote wakeup enable state */
	__TMS_BOOL	fValidateEndpoint;	/* validate endpoint */
	};

#define	__TMS_USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_fAutoRemoteWakeup						\
	)								\
	__TMS_USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG_SETUP_V2(	\
		a_pArg,							\
		a_fAutoRemoteWakeup,					\
		__TMS_TRUE						\
		)

#define	__TMS_USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG_SETUP_V2(	\
	a_pArg,								\
	a_fAutoRemoteWakeup,						\
	a_fValidateEndpoint						\
	)								\
   do	{								\
	(a_pArg)->fAutoRemoteWakeup = (a_fAutoRemoteWakeup);		\
	(a_pArg)->fValidateEndpoint = (a_fValidateEndpoint); 		\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_PROCESS_CONTROL_PACKET

Index:	Type:	USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG
	Name:	USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG_SETUP_V1()

Function:
	Process unrecognized control packet

Input:
	pInParam	points to a USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG structure.

	USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG has the following elements:

	UDEVICE *pDevice;
		IN:  pointer to the UDEVICE in question.

	UINT8 *pPacket;
		IN:  pointer to the control packet.

	UCHAR fAccept;
		OUT: must be set TRUE to accept

	UCHAR fReject;
		OUT: must be FALSE to accept

Description:
	This IOCTL can be used to ask processing of unrecognized control packet
	to the client.

	The client control packet processing routine is required to set
	uec_accept TRUE to accept; and to set uec_reject TRUE to reject.
	Both start out FALSE; and the client control packet processing routine
	should either set the accept field to accept, or set the reject field
	to reject; or else leave both fields alone, so that the accept field
	becomes the logical OR of all the accepts, and the reject field becomes
	the logical OR of all the rejects.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully returned.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG_SETUP_V1(
		USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG *pArg,
		UDEVICE *pDevice,
		UINT8 *pPacket
		);

	This macro will correctly initialize a .._PROCESS_CONTROL_PACKET_ARG
	structure.  If the layout of the argument structure changes in
	the future, MCCI will adjust this macro to initialize the new
	elements in a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG)
	{
	__TMS_UDEVICE *		pDevice;	/* IN: device */
	__TMS_UINT8 *		pPacket;	/* IN: packet */
	__TMS_UCHAR		fAccept;	/* OUT: accept */
	__TMS_UCHAR		fReject;	/* OUT: reject */
	};

/* the setup for the .._PROCESS_CONTROL_PACKET_ARG struct */
#define __TMS_USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_pPacket							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->pPacket = (ARG_pPacket);				\
	__pArg->fAccept = __TMS_FALSE;					\
	__pArg->fReject = __TMS_FALSE;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_GET_INTERRUPT_SYSTEM

Index:	Type:	USBPUMP_IOCTL_GET_INTERRUPT_SYSTEM_ARG

Function:
	Return a UHIL_INTERRUPT_SYSTEM_INTERFACE pointer.

Input:
	pInParam	must be NULL.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_INTERRUPT_SYSTEM_ARG structure.

	USBPUMP_IOCTL_GET_INTERRUPT_SYSTEM_ARG has the following elements:

	UHIL_INTERRUPT_SYSTEM_INTERFACE *
		pInterruptSystem;  // OUT: set to a valid 
				   // UHIL_INTERRUPT_SYSTEM_INTERFACE pointer.

Description:
	This IOCTL, if directed to any object layered above a UPLATFORM, 
	will return the appropriate UHIL_INTERRUPT_SYSTEM_INTERFACE pointer
	for that object.  This API allows intermediate providers to abstract
	the UHIL_INTERRUPT_SYSTEM_INTERFACE appropriately, especially in
	multi-DCD system.

Returns:
	USBPUMP_RESULT_SUCCESS:  pInterruptSystem is set to the
			UHIL_INTERRUPT_SYSTEM_INTERFACE pointer.
	USBPUMP_RESULT_NOT_CLAIMED:  the object you sent the IOCTL to has
			no associated UHIL_INTERRUPT_SYSTEM_INTERFACE.

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_INTERRUPT_SYSTEM_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_INTERRUPT_SYSTEM_ARG)
	{
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE	*pInterruptSystem;	
					/* OUT: the int-system pointer */
	};

/*

IOCTL:	USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT

Index:	Type:	USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_ARG

Function:
	Ask the platform whether LPM support should be enabled.

Input:
	pInParam	NULL.
	pOutParam	points to a USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_ARG
			structure.

	USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_ARG has the following elements:

	BOOL fEnable;
		TRUE if enable LPM, FALSE if LPM is disable.

	BOOL fBeslSupport;
		TRUE if BESL & Alternate HIRD definition supported, FALSE if
		not supported it. The fBeslSupported is valid only if fEnable
		is TRUE.

	BOOL fBaselineBeslValid;
	UINT8 bBaselineBeslValue;
		Recommended Baseline BESL valid flag and value.
		The bBaselineBeslValue is valid only if fBaselineBeslValid is
		TRUE. The bBaselineBeslValue is encoded value at range [0..15].

	BOOL fDeepBeslValid;
	UINT8 bDeepBeslValue;
		Recommended Deep BESL valid flag and value.
		The bDeepBeslValue is valid only if fDeepBeslValid is TRUE.
		The bDeepBeslValue is encoded value at range [0..15].

Description:
	This IOCTL can be used to ask the platform whether LPM support should
	be enabled or not.  The platform should return a consistent result to
	this for any given USB session.  The USB specification is not clear
	about what hosts might do if this value changes after enumeration, so
	platforms should assume that they should only change their answer when
	USB reset is received.

Returns:
	USBPUMP_RESULT_SUCCESS:  link power management mode is enabled/disabled.

	USBPUMP_IOCTL_RESULT_HW_NOT_SUPPORT:  chip doesn't support
	
	USBPUMP_IOCTL_RESULT_APP_NOT_SUPPORT:  aplication (.urc) doesn't support
	
	Other failure codes are possible if the request is malformed.

*/


__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_ARG)
	{
	__TMS_BOOL		fEnable;	/* OUT: fEnable */
	__TMS_BOOL		fBeslSupport;
	__TMS_BOOL		fBaselineBeslValid;
	__TMS_BOOL		fDeepBeslValid;
	__TMS_UINT8		bBaselineBeslValue;
	__TMS_UINT8		bDeepBeslValue;
	};

/*
|| To ensure future portability, platform code must use this macro to prepare
|| the results that they'll pass back in the
|| USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_ARG structure.
*/
#define __TMS_USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_SETUP_V2(	\
	a_pArg,							\
	a_fEnable,						\
	a_fBeslSupport,						\
	a_fBaselineBeslValid,					\
	a_fDeepBeslValid,					\
	a_bBaselineBeslValue,					\
	a_bDeepBeslValue					\
	)							\
   do	{							\
   	(a_pArg)->fEnable = (a_fEnable);			\
   	(a_pArg)->fBeslSupport = (a_fBeslSupport);		\
   	(a_pArg)->fBaselineBeslValid = (a_fBaselineBeslValid);	\
   	(a_pArg)->fDeepBeslValid = (a_fDeepBeslValid);		\
   	(a_pArg)->bBaselineBeslValue = (a_bBaselineBeslValue);	\
   	(a_pArg)->bDeepBeslValue = (a_bDeepBeslValue);		\
	} while (0)

#define __TMS_USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_SETUP_V1(	\
	a_pArg,							\
	a_fEnable						\
	)							\
	__TMS_USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_SETUP_V2(	\
	a_pArg,							\
	a_fEnable,						\
	/* V2: fBeslSupport */ __TMS_FALSE,			\
	/* V2: fBaselineBeslValid */ __TMS_FALSE,		\
	/* V2: fDeepBeslValid */ __TMS_FALSE,			\
	/* V2: bBaselineBeslValue */ 0,				\
	/* V2: bDeepBeslValue */ 0				\
	)

/*

IOCTL:	USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN

Index:	Type:	USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG
	Name:	USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG_SETUP_V1()

Function:
	Find a class child object from the class object directory.

Input:
	pInParam	points to a USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG structure.

	USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pThisObject;
		IN:  starting point.  if NULL, starts at the beginning; if
		     non-NULL, this is the last value returned, and this
		     returns the next match.

	USBPUMP_OBJECT_HEADER *pNextObject;
		OUT:  next matching object.

Description:
	This IOCTL, if directed to a class object, can be used to traverse the
	children objects.

Returns:
	USBPUMP_RESULT_SUCCESS:  the match is in pNextObject.
	USBPUMP_RESULT_NO_MORE:	 no more were found; pNextObject is NULL.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG_SETUP_V1(
		USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pThisObject
		);

	This macro will correctly initialize a .._ENUMERATE_CLASS_CHILDREN_ARG
	structure.  If the layout of the argument structure changes in
	the future, MCCI will adjust this macro to initialize the new
	elements in a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER	*pThisObject;	/* IN: where to
							|| start searching.
							*/
	__TMS_USBPUMP_OBJECT_HEADER	*pNextObject;	/* OUT: match */
	};

/* the setup for the .._ENUMERATE_CLASS_CHILDREN_ARG struct */
#define __TMS_USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pThisObject							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pThisObject = (ARG_pThisObject);			\
	__pArg->pNextObject = __TMS_NULL;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_CALCULATE_SHA_1

Index:	Type:	USBPUMP_IOCTL_CALCULATE_SHA_1_ARG
	Name:	USBPUMP_IOCTL_CALCULATE_SHA_1_ARG_SETUP_V1()

Function:
	Calculate SHA-1 with given buffer

Input:
	pInParam	points to a USBPUMP_IOCTL_CALCULATE_SHA_1_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_CALCULATE_SHA_1_ARG structure.

	USBPUMP_IOCTL_CALCULATE_SHA_1_ARG has the following elements:

	CONST UINT8 * pInputBuffer;
		IN:  the input buffer to calcuate SHA-1.

	BYTES InputBufferSizeInBytes;
		IN: size of the input buffer

	UINT8 * pOutputBuffer;
		OUT:  buffer to save calculated SHA-1. Size of output buffer
		should be greater than equal to 20 bytes.

Description:
	This IOCTL can be used to calcualte SHA-1 with pInputBuffer and
	InputBufferSizeInBytes.  Calculated SHA-1 result will be saved in the
	pOutputBuffer.  Caller should provide enough size of pOutputBuffer.

Returns:
	USBPUMP_RESULT_SUCCESS:  Successfully calculated
	USBPUMP_RESULT_NOT_CLAIMED:  not supported SHA-1 calculation.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_CALCULATE_SHA_1_ARG_SETUP_V1(
		USBPUMP_IOCTL_CALCULATE_SHA_1_ARG *pArg,
		CONST UINT8 * pInputBuffer,
		BYTES InputBufferSizeInBytes,
		UINT8 * pOutputBuffer
		);

	This macro will correctly initialize a .._CALCULATE_SHA_1_ARG
	structure.  If the layout of the argument structure changes in
	the future, MCCI will adjust this macro to initialize the new
	elements in a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_CALCULATE_SHA_1_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_CALCULATE_SHA_1_ARG)
	{
	__TMS_CONST __TMS_UINT8 *	pInputBuffer;
	__TMS_BYTES			InputBufferSizeInBytes;
	__TMS_UINT8 *			pOutputBuffer;
	};

/* the setup for the .._CALCULATE_SHA_1_ARG struct */
#define __TMS_USBPUMP_IOCTL_CALCULATE_SHA_1_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pInputBuffer,						\
	ARG_InputBufferSizeInBytes,					\
	ARG_pOutputBuffer						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_CALCULATE_SHA_1_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pInputBuffer = (ARG_pInputBuffer);			\
	__pArg->InputBufferSizeInBytes = (ARG_InputBufferSizeInBytes);	\
	__pArg->pOutputBuffer = (ARG_pOutputBuffer);			\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA

Index:	Type:	USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG

	Name:	USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1()

Function:
	Get the value of the indexed registier in PCI Power Management
	Register Blcok

Input:
	pInParam	points to a USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG
			structure.

	USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG has the following
	elements:

	UINT8	ucRegister:
		IN: The index of the requested register.  The valid value
		could be
		USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_CAP_ID:
			Capability Identifier
		USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_NEXT_ITEM_PTR:
			Next Item Pointer
		USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_PMC:
			Power Management Capabilities
		USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_PMCSR:
			Power Management Control/Status
		USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_BSE:
			PMCSR PCI-to-PCI Bridge Support Extension
		USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_DATA:
			Optional Data

	UINT16	usValue:
		OUT: The value read from the indicated register.

Description:
	Ceratin capabilities added to PCI after the publication of PCI local
	bus specification revision 2.1 by adding a set of registers to a
	linked likst called the Capabilities List.  The optional data 
	structure is indicated in the PCI Status Register by setting the 
	Capabilities List bit (bit 4) to indicate that the Capabiliteis linked
	list is available.

	This IOCTL can be used to get the value of the regsiter in the PCI
	Power Management Register Block.

Returns:
	USBPUMP_RESULT_SUCCESS:  Successfully return the content from the 
	indexed regsiter
	USBPUMP_RESULT_NOT_CLAIMED:  not supported PCI Power Management
	Register read access.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1
		USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG	*pArg,
		UINT8	ucRegister
		);

	This macro will correctly initialize a
	USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG
	structure.  If the layout of the argument structure changes in
	the future, MCCI will adjust this macro to initialize the new
	elements in a forward-compatible way.

See Also:
	USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA,
	USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG,
	USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1()
	
*/

#define __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_CAP_ID		0
#define __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_NEXT_ITEM_PTR	1
#define __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_PMC		2
#define __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_PMCSR		3
#define __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_BSE		4
#define __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_DATA		5

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG)
	{
	__TMS_UINT8			ucRegister;
	__TMS_UINT16			usValue;
	};

#define __TMS_USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_ucRegister							\
	)								\
   do	{								\
   	(pArg)->ucRegister = (ARG_ucRegister);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA

Index:	Type:	USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG
	Name:	USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1()

Function:
	Write to the indexed registier in PCI Power Management Register Blcok

Input:
	pInParam	points to a USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG
			structure.

	USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG has the following
	elements:

	UINT8	ucRegister:
		IN: The index of the requested register.  The valid value
		could be
		USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_PMCSR:
			Power Management Control/Status

	UINT16	usMaskToAnd:
		IN: Bit mask used to AND the register value (for the unchaning
		bits)

	UINT16	usMaskToOr:
		IN: bit mask used to OR the register value (for the changing
		bits)

Description:
	Ceratin capabilities added to PCI after the publication of PCI local
	bus specification revision 2.1 by adding a set of registers to a
	linked likst called the Capabilities List.  The optional data 
	structure is indicated in the PCI Status Register by setting the 
	Capabilities List bit (bit 4) to indicate that the Capabiliteis linked
	list is available.

	This IOCTL can be used to write the indexed regsiter.

Returns:
	USBPUMP_RESULT_SUCCESS:  Successfully write the content to
	indexed regsiter
	USBPUMP_RESULT_NOT_CLAIMED:  not supported PCI Power Management
	Register write access.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1(
		USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG *pArg,
		UINT8	ucRegister,
		UINT16	usMaskToAnd,
		UINT16	usMaskToOr
		);

	This macro will correctly initialize a
	USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG structure.  If the
	layout of the argument structure changes in the future, MCCI will
	adjust this macro to initialize the new elements in a 
	forward-compatible way.

See Also:
	USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA,
	USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG,
	USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1()

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG)
	{
	__TMS_UINT8			ucRegister;
	__TMS_UINT16			usMaskToAnd;
	__TMS_UINT16			usMaskToOr;
	};
#define __TMS_USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_ucRegister,							\
	ARG_usMaskToAnd,						\
	ARG_usMaskToOr							\
	)								\
   do	{								\
   	(pArg)->ucRegister = (ARG_ucRegister);				\
   	(pArg)->usMaskToAnd = (ARG_usMaskToAnd);			\
   	(pArg)->usMaskToOr = (ARG_usMaskToOr);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_ALLOC_COMMON_BUFFER

Index:	Type:	USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG
	Name:	USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG_SETUP_V1

Function:
	IOCTL issued to allocate memory elements accessible by the software 
	and hardware

Input:
	pInParam	points to a USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG structure.

	USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG has the following elements:

	BYTES SizeInBytes;
		IN: number of bytes to allocate

	BYTES AlignmentInBytes;
		IN: alignment of buffer in bytes

	USBPUMP_COMMON_BUFFER_ACCESS_TYPE_ENUM Access;
		IN: what this buffer is to be used for according
		    to USBPUMP_COMMON_BUFFER_ACCESS_TYPE_ENUM

	USBPUMP_BUFFER_HANDLE hBuffer;
		OUT: Buffer handle

	PVOID pBuffer;
		OUT: Buffer pointer

Description:
	This IOCTL is sent from application, protocol or USB chip interface to 
	allocate memory buffer accessible by the software and hardware.
	hBuffer may be NULL on return if memory allocator does not provide
	a buffer handle.

Setup Macro:
	VOID USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG_SETUP_V1(
		USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG *
							pArg,
		BYTES					SizeInBytes,
		BYTES					AlignmentInBytes,
		USBPUMP_COMMON_BUFFER_ACCESS_TYPE_ENUM	Access
		);

	This macro correctly initializes a .._ALLOC_COMMON_BUFFER_ARG
	structure. If the layout of the argument structure changes in the 
	future, MCCI will revise this macro to initialize the new elements 
	in a forward-compatible way.
*/

/*
|| Buffer access attributes. 
|| READ means that data will only be read from buffer by the one requesting 
|| the buffer, and consequently written to when passed down to hardware to 
|| be used. 
|| WRITE means that data will only be written to buffer by the one requesting 
|| the buffer, and consequently read from when passed down to hardware to 
|| be used.
|| MODIFY means that data may both read and written from buffer by the one 
|| requesting the buffer, and consequently both read and written to when 
|| passed down to hardware to be used.
*/
__TMS_TYPE_DEF_ARG (USBPUMP_COMMON_BUFFER_ACCESS_TYPE_ENUM, UINT32);
enum __TMS_ENUMNAME(USBPUMP_COMMON_BUFFER_ACCESS_TYPE_ENUM)
	{
	__TMS_USBPUMP_COMMON_BUFFER_ACCESS_READ = 0,
	__TMS_USBPUMP_COMMON_BUFFER_ACCESS_WRITE = 1,
	__TMS_USBPUMP_COMMON_BUFFER_ACCESS_MODIFY = 2
	};

/* Manual cloaking for enum type */
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_COMMON_BUFFER_ACCESS_READ	\
   __TMS_USBPUMP_COMMON_BUFFER_ACCESS_READ
# define USBPUMP_COMMON_BUFFER_ACCESS_WRITE	\
   __TMS_USBPUMP_COMMON_BUFFER_ACCESS_WRITE
# define USBPUMP_COMMON_BUFFER_ACCESS_MODIFY	\
   __TMS_USBPUMP_COMMON_BUFFER_ACCESS_MODIFY
#endif

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG)
	{
	__TMS_BYTES			SizeInBytes;
	__TMS_BYTES			AlignmentInBytes;
	__TMS_USBPUMP_COMMON_BUFFER_ACCESS_TYPE_ENUM
					Access;
	__TMS_USBPUMP_BUFFER_HANDLE	hBuffer;
	__TMS_PVOID			pBuffer;
	};

/* the setup for the .._ALLOC_BUFFER_ARG struct */
#define __TMS_USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG_SETUP_V1(		\
	pArg,								\
	ARG_SizeInBytes,						\
	ARG_AlignmentInBytes,						\
	ARG_Access							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG * __TMS_CONST 	\
		__pArg = (pArg);					\
	__pArg->SizeInBytes      = (ARG_SizeInBytes);			\
	__pArg->AlignmentInBytes = (ARG_AlignmentInBytes);		\
	__pArg->Access           = (ARG_Access);			\
	__pArg->hBuffer          = __TMS_NULL;				\
	__pArg->pBuffer          = __TMS_NULL;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_FREE_COMMON_BUFFER

Index:	Type:	USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG
	Name:	USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG_SETUP_V1

Function:
	IOCTL issued to free memory elements allocated with 
	USBPUMP_IOCTL_ALLOC_COMMON_BUFFER

Input:
	pInParam	points to a USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG structure.

	USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG has the following elements:

	USBPUMP_BUFFER_HANDLE hBuffer;
		IN: Buffer handle

	PVOID pBuffer;
		IN: Buffer pointer

Description:
	This IOCTL is sent from application, protocol or USB chip interface to 
	free memory buffer allocated using USBPUMP_IOCTL_FREE_COMMON_BUFFER.
	hBuffer and pBuffer should be provided as returned by 
	USBPUMP_IOCTL_FREE_COMMON_BUFFER when buffer allocated

Setup Macro:
	VOID USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG_SETUP_V1(
		USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG *
					pArg,
		USBPUMP_BUFFER_HANDLE	hBuffer,
		PVOID			pBuffer
		);

	This macro correctly initializes a .._FREE_COMMON_BUFFER_ARG
	structure. If the layout of the argument structure changes in the 
	future, MCCI will revise this macro to initialize the new elements 
	in a forward-compatible way.
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG)
	{
	__TMS_USBPUMP_BUFFER_HANDLE	hBuffer;
	__TMS_PVOID			pBuffer;
	};

/* the setup for the .._FREE_BUFFER_ARG struct */
#define __TMS_USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG_SETUP_V1(	\
	pArg,							\
	ARG_hBuffer,						\
	ARG_pBuffer						\
	)							\
   do	{							\
	__TMS_USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG * __TMS_CONST \
		__pArg = (pArg);				\
	__pArg->hBuffer          = (ARG_hBuffer);		\
	__pArg->pBuffer          = (ARG_pBuffer);		\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR

Index:	Type:	USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR_ARG
	Name:	USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR_ARG_SETUP_V1

Function:
	IOCTL issued to override descriptor for this UDEVICE or UPLATFORM.

Input:
	pInParam	points to a USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR_ARG
			structure
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR_ARG structure.

	USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR_ARG has the following elements:

	CONST UINT8	*pCurrentDesc;
		IN: pointer to current descriptor

	BYTES	nCurrentDesc;
		IN: number of bytes of current descriptor

	UINT8	ucRequestType;
		IN: Request Type

	UINT8	ucDescrType;
		IN: Descriptor Type

	UINT8	ucDescrIndex;
		IN: Descriptor Index

	USBPUMP_DEVICE_SPEED	bCurrentSpeed;
		IN: current device speed
	
	UINT16	wIndex;
		IN: wIndex
	
	CONST UINT8 *	pNewDesc;
		OUT: new config descriptor

	BYTES	nNewDesc;
		OUT: size of new descriptor

Description:
	This IOCTL returns the override descriptor associated with the
	platform object.

	To fetch the information, issue the IOCTL and read the result.

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR_ARG)
	{
	__TMS_CONST __TMS_UINT8 *	pCurrentDesc;	/* IN: current descriptor */
	__TMS_BYTES			nCurrentDesc;	/* IN: size of descriptor */
	__TMS_UINT8			ucRequestType;	/* IN: Request Type */
	__TMS_UINT8			ucDescrType;	/* IN: Descriptor Type */
	__TMS_UINT8			ucDescrIndex;	/* IN: Descriptor Index */
	__TMS_USBPUMP_DEVICE_SPEED	bCurrentSpeed;	/* IN: current device speed */
	__TMS_UINT16			wIndex;		/* IN: wIndex */
	__TMS_CONST __TMS_UINT8 *	pNewDesc;	/* OUT: new descriptor */
	__TMS_BYTES			nNewDesc;	/* OUT: size of new descriptor */
	};

#define	__TMS_USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR_SETUP_V1(	\
	a_pArg,							\
	a_pCurrentDesc,						\
	a_nCurrentDesc,						\
	a_ucRequestType,					\
	a_ucDescrType,						\
	a_ucDescrIndex,						\
	a_bCurrentSpeed,					\
	a_wIndex						\
	)							\
   do	{							\
	(a_pArg)->pCurrentDesc = (a_pCurrentDesc);		\
	(a_pArg)->nCurrentDesc = (a_nCurrentDesc);		\
	(a_pArg)->ucRequestType = (a_ucRequestType);		\
	(a_pArg)->ucDescrType = (a_ucDescrType);		\
	(a_pArg)->ucDescrIndex = (a_ucDescrIndex);		\
	(a_pArg)->bCurrentSpeed = (a_bCurrentSpeed);		\
	(a_pArg)->wIndex = (a_wIndex);				\
	(a_pArg)->pNewDesc = __TMS_NULL;			\
	(a_pArg)->nNewDesc = 0;					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_REGISTER_DUMP

Function:
	Ask the chip driver to dump register.

Input:
	pInParam	always NULL
	pOutParam	always NULL

Description:
	This IOCTL tells the chip driver that the system software wants to dump
	hardware register to debug output.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success -- which will always be
	the result unless there's some kind of system plumbing problem.

Notes:
	This IOCTL has no effect in free build.

*/

/*

IOCTL:	USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR

Index:	Type:	USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR_ARG
	Name:	USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR_ARG_SETUP_V1

Function:
	IOCTL issued to filter functional descriptor

Input:
	pInParam	points to a USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR_ARG
			structure
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR_ARG structure.

	USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR_ARG has the following elements:

	USBPUMP_DEVICE_SPEED	bCurrentSpeed;
		IN: current device speed

	UINT8	bConfigurationValue;
		IN: config value of the configuration that encompasses
		the functional descriptor.

	UINT8	bInterfaceNumber;
		IN: interface number of the interface that encompasses
		the functional descriptor

	UINT8	bAlternateSetting;
		IN: interface altsetting of the interface that encompasses
		the functional descriptor

	UINT8	bInterfaceClass;
		IN: interface class of the interface that encompasses
		the functional descriptor

	UINT8	bInterfaceSubClass;
		IN: interface subclass of the interface that encompasses
		the functional descriptor
	
	UINT8	bInterfaceProtocol;
		IN: interface protocol of the interface that encompasses
		the functional descriptor
	
	UINT8 *	pFuncDescWire;
		IN/OUT: buffer pointer of the wire format functional descriptor.
		client is supposed to change fields as necessary but should make
		sure that the filtered field values are compliant to the
		relevant specification and should be wire-format (little-endian)
		which will be transferred via USB wire.

Description:
	This IOCTL returns the filtered functional descriptor.
	Normally this IOCTL is issued by protocol layer or descriptor filter
	to fetch the functional descriptor filtered by client. client who
	implement this IOCTL would be usually the customer platform code.

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR_ARG)
	{
	__TMS_USBPUMP_DEVICE_SPEED	bCurrentSpeed;		/* IN: current device speed */
	__TMS_UINT8			bConfigurationValue;	/* IN: config value */
	__TMS_UINT8			bInterfaceNumber;	/* IN: interface number */
	__TMS_UINT8			bAlternateSetting;	/* IN: interface altset */
	__TMS_UINT8			bInterfaceClass	;	/* IN: interface class */
	__TMS_UINT8			bInterfaceSubClass;	/* IN: interface subclass */
	__TMS_UINT8			bInterfaceProtocol;	/* IN: interface protocol */
	__TMS_UINT8 *			pFuncDescWire;		/* IN/OUT: functional descriptor */
	};

#define	__TMS_USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR_SETUP_V1(	\
	a_pArg,								\
	a_bCurrentSpeed,						\
	a_bConfigurationValue,						\
	a_bInterfaceNumber,						\
	a_bAlternateSetting,						\
	a_bInterfaceClass,						\
	a_bInterfaceSubClass,						\
	a_bInterfaceProtocol,						\
	a_pFuncDescWire							\
	)								\
   do	{								\
	(a_pArg)->bCurrentSpeed = (a_bCurrentSpeed);			\
	(a_pArg)->bConfigurationValue = (a_bConfigurationValue);	\
	(a_pArg)->bInterfaceNumber = (a_bInterfaceNumber);		\
	(a_pArg)->bAlternateSetting = (a_bAlternateSetting);		\
	(a_pArg)->bInterfaceClass = (a_bInterfaceClass);		\
	(a_pArg)->bInterfaceSubClass = (a_bInterfaceSubClass);		\
	(a_pArg)->bInterfaceProtocol = (a_bInterfaceProtocol);		\
	(a_pArg)->pFuncDescWire = a_pFuncDescWire;			\
	} while (0)
	
/*

IOCTL:	USBPUMP_IOCTL_GET_PCI_INFO

Index:	Type:	USBPUMP_IOCTL_GET_PCI_INFO

Function:
	Ask the platform PCI information.

Input:
	pInParam	NULL.
	pOutParam	points to a USBPUMP_IOCTL_GET_PCI_INFO_ARG
			structure.

	USBPUMP_IOCTL_GET_PCI_INFO_ARG has the following elements:

	USHORT	VendorID; 
		Out: PCI vendor ID
	USHORT	DeviceID;  
		Out: PCI device ID
	UCHAR	RevisionID;
		Out: PCI revision ID
	UCHAR	ProgramInterface;
		Out: PCI ProgramInterface
	UCHAR	SubClass;  
		Out: PCI Subclass code
	UCHAR	BaseClass; 
		Out: PCI base class code
	USHORT	SubsystemVendorID
		Out: PCI SubsystemVendorID
	USHORT	SubsystemID;
		Out: PCI SubsystemID
	
Description:
	This IOCTL can be used to ask the platform basic PCI information. It
	includes Vendor ID, Device ID, Revision ID, Subclass code and base
	class code.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_PCI_INFO_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_PCI_INFO_ARG)
	{
	__TMS_USHORT		VendorID;
	__TMS_USHORT		DeviceID;
	__TMS_UCHAR		RevisionID;
	__TMS_UCHAR		ProgramInterface;
	__TMS_UCHAR		SubClass;
	__TMS_UCHAR		BaseClass;
	__TMS_USHORT		SubsystemVendorID;
	__TMS_USHORT		SubsystemID;
	};

#define __TMS_USBPUMP_IOCTL_GET_PCI_INFO_SETUP_V1(	\
	a_pArg,						\
	a_VendorID,					\
	a_DeviceID,					\
	a_RevisionID,					\
	a_ProgramInterface,				\
	a_SubClass,					\
	a_BaseClass,					\
	a_SubsystemVendorID,				\
	a_SubsystemID					\
	)						\
   do	{						\
   	(a_pArg)->VendorID = (a_VendorID);		\
   	(a_pArg)->DeviceID = (a_DeviceID);		\
   	(a_pArg)->RevisionID = (a_RevisionID);		\
   	(a_pArg)->ProgramInterface = (a_ProgramInterface); \
   	(a_pArg)->SubClass = (a_SubClass);		\
   	(a_pArg)->BaseClass = (a_BaseClass);		\
   	(a_pArg)->SubsystemVendorID = (a_SubsystemVendorID); \
   	(a_pArg)->SubsystemID = (a_SubsystemID);	\
	} while (0)


/*

IOCTL:	USBPUMP_IOCTL_GET_PCI_CONFIG_DATA

Index:	Type:	USBPUMP_IOCTL_GET_PCI_CONFIG_DATA_ARG
	Name:	USBPUMP_IOCTL_GET_PCI_CONFIG_DATA_ARG_SETUP_V1()

Function:
	Ask platform about PCI configuration data at the specific offet in
	PCI configuration space with given buffer.

Input:
	pInParam	points to a USBPUMP_IOCTL_GET_PCI_CONFIG_DATA_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_GET_PCI_CONFIG_DATA_ARG structure.

	USBPUMP_IOCTL_GET_PCI_CONFIG_DATA_ARG has the following elements:
	
	BYTES	Offset;
		IN: The offset in the PCI configuration space.

	BYTES nBuffer;
		IN: size of the input buffer

	UINT8 * pBuffer;
		OUT:  buffer to save PCI configuration data.

Description:
	This IOCTL can be used to retrieve PCI bus data at the specific
	offset in PCI configuration space with pBuffer and nBuffer.

Returns:
	USBPUMP_RESULT_SUCCESS:  Successfully calculated

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_PCI_CONFIG_DATA_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_PCI_CONFIG_DATA_ARG)
	{
	__TMS_VOID *		pBuffer;
	__TMS_BYTES		nBuffer;
	__TMS_BYTES		Offset;
	};

#define __TMS_USBPUMP_IOCTL_GET_PCI_CONFIG_DATA_SETUP_V1( \
	a_pArg,						\
	a_pBuffer,					\
	a_nBuffer,					\
	a_Offset					\
	)						\
   do	{						\
   	(a_pArg)->pBuffer = (a_pBuffer);		\
	(a_pArg)->nBuffer = (a_nBuffer);		\
	(a_pArg)->Offset = (a_Offset);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_GET_XHCI_PORT_MAP

Index:	Type:	USBPUMP_IOCTL_GET_XHCI_PORT_MAP_ARG
	Name:	USBPUMP_IOCTL_GET_XHCI_PORT_MAP_ARG_SETUP_V1()

Function:
	Ask about XHCI root port map information from platform if it is
	not standard designed.

Input:
	pInParam	points to a USBPUMP_IOCTL_GET_XHCI_PORT_MAP_ARG
			structure.
	pOutParam	points to a USBPUMP_IOCTL_GET_XHCI_PORT_MAP_ARG
			structure.

	USBPUMP_IOCTL_GET_XHCI_PORT_MAP_ARG has the following elements:
	
	BOOL	fUsb2Port;
		IN: USB2 port map or USB3 port map.
	
	UINT8 * pUsbHcPortMap;
		IN:  buffer to save port index.
		
	BYTES	nUsbHcPortMap;
		IN: size of pUsbHcPortMap.

	BYTES	nActualPortMap;
		OUT: Actual port map size.
		
Description:
	This IOCTL can be used to retrieve XHCI port map information from
	platform layer with pUsbHcPortMap and nUsbHcPortMap.
	
	For example, if port map information is like below:
	DataPumpXhcdUsb2PortMap = {2, 4, 3},
	DataPumpXhcdUsb3PortMap = {2, 1, 2}
	
	So for USB2 port, nUsbHcPortMap = 3, pUsbHcPortMap = {2, 4, 3}
	for USB3 port, nUsbHcPortMap = 3, pUsbHcPortMap = {2, 1, 2}

Returns:
	USBPUMP_RESULT_SUCCESS:  Successfully calculated

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_XHCI_PORT_MAP_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_XHCI_PORT_MAP_ARG)
	{
	__TMS_BOOL		fUsb2Port;		/* IN */
	__TMS_UINT8 *		pUsbHcPortMap;		/* IN */
	__TMS_BYTES		nUsbHcPortMap;		/* IN */
	__TMS_BYTES		nActualPortMap;		/* OUT */
	};

#define __TMS_USBPUMP_IOCTL_GET_XHCI_PORT_MAP_SETUP_V1( \
	a_pArg,						\
	a_fUsb2Port,					\
	a_pUsbHcPortMap,				\
	a_nUsbHcPortMap					\
	)						\
   do	{						\
   	(a_pArg)->fUsb2Port = (a_fUsb2Port);		\
   	(a_pArg)->pUsbHcPortMap = (a_pUsbHcPortMap);	\
	(a_pArg)->nUsbHcPortMap = (a_nUsbHcPortMap);	\
	(a_pArg)->nActualPortMap = 0;			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO

Index:	Type:	USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO_ARG
	Name:	USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO_ARG_SETUP_V1()

Function:
	Ask about mass storage LUN information from platform.

Input:
	pInParam	points to a USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO_ARG
			structure.
	pOutParam	points to a USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO_ARG
			structure.

	USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO_ARG has the following elements:

	UINT	iLun;
		IN: LUN number

	BYTES	LoLba;
		OUT: First logical block number for given LUN.

	BYTES	HiLba;
		OUT: Last logical block number for given LUN.

Description:
	This IOCTL can be used to retrieve mass storage LUN information from
	platform layer with iLun.

Returns:
	USBPUMP_RESULT_SUCCESS:  Successfully returned

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO_ARG)
	{
	__TMS_UINT		iLun;		/* IN */
	__TMS_BYTES		LoLba;		/* OUT */
	__TMS_BYTES		HiLba;		/* OUT */
	};

#define __TMS_USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO_SETUP_V1(	\
	a_pArg,							\
	a_iLun,							\
	a_LoLba,						\
	a_HiLba							\
	)							\
   do	{							\
   	(a_pArg)->iLun = (a_iLun);				\
	(a_pArg)->LoLba = (a_LoLba);				\
	(a_pArg)->HiLba = (a_HiLba);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG

Index:	Type:	USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_ARG
	Name:	USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_ARG_SETUP_V1()

Function:
	Ask about Abstract NIC packet loop test configuration from platform.

Input:
	pInParam	must be NULL.
	pOutParam	points to a USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_ARG
			structure.

	USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_ARG has the following elements:

	UINT	PacketMode;
		OUT: Define operating mode (IDLE, LOOP or GEN)

	UINT	SendFrameMode;
		OUT: Define how frames are to be generated when GEN mode
		selected

	BYTES	NumSendFrames;
		OUT: Define how many frames are to be generated when GEN mode
		selected (0 means unlimited)

	BYTES	SendFrameMinLen;
		OUT: Define min frame length when GEN mode selected

	BYTES	SendFrameMaxLen;
		OUT: Define max frame length when GEN mode selected

	BYTES	ConnectTimeout;
		OUT: Define the time (ms) before connect is issued

	BYTES	SendFrameQueueTimeout;
		OUT: Define the time (ms) that received frames may be in send
		queue before the queue is flushed to send

	UINT	SendFrameQueueSize;
		OUT: Define the number of received frames that may be in send
		queue before the queue is flushed to send

	UINT	NumSendQes;
		OUT: Define number of SEND_QE's to allocate

	UINT	NumReceiveQes;
		OUT: Define number of RECEIVE_QE's to allocate

	UINT	MaxFramesPerSendQe;
		OUT: Define number of FRAME_INFO blocks per SEND_QE

Description:
	This IOCTL can be used to retrieve Abstract NIC Packet Loop test
	configuration from platform layer.

Returns:
	USBPUMP_RESULT_SUCCESS:  Successfully returned

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_ARG);

#define __TMSUSBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_PACKETMODE_IDLE	0
#define __TMSUSBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_PACKETMODE_GEN	1
#define __TMSUSBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_PACKETMODE_LOOP	2

#define __TMSUSBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_SENDFRAMEMODE_FIXED	0
#define __TMSUSBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_SENDFRAMEMODE_RAMP	1
#define __TMSUSBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_SENDFRAMEMODE_RANDOM	2

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_ARG)
	{
	__TMS_UINT		PacketMode;		/* OUT */
	__TMS_UINT		SendFrameMode;		/* OUT */
	__TMS_BYTES		NumSendFrames;		/* OUT */
	__TMS_BYTES		SendFrameMinLen;	/* OUT */
	__TMS_BYTES		SendFrameMaxLen;	/* OUT */
	__TMS_BYTES		ConnectTimeout;		/* OUT */
	__TMS_BYTES		SendFrameQueueTimeout;	/* OUT */
	__TMS_UINT		SendFrameQueueSize;	/* OUT */
	__TMS_UINT		NumSendQes;		/* OUT */
	__TMS_UINT		NumReceiveQes;		/* OUT */
	__TMS_UINT		MaxFramesPerSendQe;	/* OUT */
	};

#define __TMS_USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_SETUP_V1(	\
	a_pArg,							\
	a_PacketMode,						\
	a_SendFrameMode,					\
	a_NumSendFrames,					\
	a_SendFrameMinLen,					\
        a_SendFrameMaxLen,					\
        a_ConnectTimeout,					\
        a_SendFrameQueueTimeout,				\
        a_SendFrameQueueSize,					\
        a_NumSendQes,						\
        a_NumReceiveQes,					\
        a_MaxFramesPerSendQe					\
	)							\
   do	{							\
 	(a_pArg)->PacketMode = (a_PacketMode);			\
	(a_pArg)->SendFrameMode = (a_SendFrameMode);		\
	(a_pArg)->NumSendFrames = (a_NumSendFrames);		\
	(a_pArg)->SendFrameMinLen = (a_SendFrameMinLen);	\
        (a_pArg)->SendFrameMaxLen = (a_SendFrameMaxLen);	\
        (a_pArg)->ConnectTimeout = (a_ConnectTimeout);		\
        (a_pArg)->SendFrameQueueTimeout = (a_SendFrameQueueTimeout);	\
        (a_pArg)->SendFrameQueueSize = (a_SendFrameQueueSize);	\
        (a_pArg)->NumSendQes = (a_NumSendQes);			\
        (a_pArg)->NumReceiveQes = (a_NumReceiveQes);		\
        (a_pArg)->MaxFramesPerSendQe = (a_MaxFramesPerSendQe);	\
	} while (0)

	
/*

IOCTL:	USBPUMP_IOCTL_GET_CONTAINER_ID

Index:	Type:	USBPUMP_IOCTL_GET_CONTAINER_ID_ARG
	Name:	USBPUMP_IOCTL_GET_CONTAINER_ID_ARG_SETUP_V1()

Function:
	IOCTL issued to retrieve 16 bytes Container ID data for BOS

Input:
	pInParam	points to a USBPUMP_IOCTL_GET_CONTAINER_ID_ARG
			structure
	pOutParam	points to a USBPUMP_IOCTL_GET_CONTAINER_ID_ARG
			structure.

	USBPUMP_IOCTL_GET_CONTAINER_ID_ARG has the following elements:
	
	USBPUMP_OBJECT_HEADER * pClientObject;
		In: IOCTL implementer (usually platform object) has to
		return the same unique container ID data for the same
		client object determined by this input parameter.
		
	USBUUID_WIRE * pContainerID;
		Out: Buffer for container ID in wire format.
	
Description:
	This IOCTL can be used to retrieve container ID from the platform.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_GET_CONTAINER_ID_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_GET_CONTAINER_ID_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pClientObject;
	__TMS_USBUUID_WIRE *		pContainerID;
	};

#define __TMS_USBPUMP_IOCTL_GET_CONTAINER_ID_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pClientObject,					\
	a_pContainerID						\
	)							\
   do	{							\
   	(a_pArg)->pClientObject = (a_pClientObject);		\
   	(a_pArg)->pContainerID = (a_pContainerID);		\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DELAY_MICRO_SECONDS

Index:	Type:	USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG
	Name:	USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to ask for micro-seconds delay.

Input:
	pInParam	points to a USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG
			structure.

	pOutParam	not used, should be NULL.

	USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG has the following elements:

	UINT32		uMicroSeconds;
		IN: required wait period in micro-second.

Description:
	This IOCTL is sent to delay required waiting micro-seconds period
	from client.

Setup Macro:
	VOID USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG_SETUP_V1(
		USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG *pArg,
		UINT32	uMicroSeconds
		);

	This macro correctly initializes a ..._DELAY_MICRO_SECONDS_ARG
	structure. If the layout of the argument structure changes in the 
	future, MCCI will revise this macro to initialize the new elements in
	a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG)
	{
	__TMS_UINT32	uMicroSeconds;
	};

/* the setup for the .._DEVICE_SUPPORT_DELAY_ARG struct */
#define __TMS_USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG_SETUP_V1(	\
	pArg,							\
	ARG_uMicroSeconds					\
	)							\
   do	{							\
	__TMS_USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG *		\
		__TMS_CONST	__pArg = (pArg);		\
	__pArg->uMicroSeconds = (ARG_uMicroSeconds);		\
   	} while (0)


/****************************************************************************\
|
|	The device IOCTL support routines
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_IOCTL_RESULT
UsbDeviceIoctl_GetVidPidModeEx(
		__TMS_UDEVICE *					pDevice,
		__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_EX_ARG *	pInfo
		);

__TMS_USBPUMP_IOCTL_RESULT
UsbDeviceIoctl_GetVidPidModeV3(
		__TMS_UDEVICE *					pDevice,
		__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_V3_ARG *	pInfo
		);

__TMS_END_DECLS


/****************************************************************************\
|
|	uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_CORE_VOID(i)	\
   __TMS_USBPUMP_IOCTL_CORE_VOID(i)
# define USBPUMP_IOCTL_CORE3(t, i, NAME)	\
   __TMS_USBPUMP_IOCTL_CORE3(t, i, NAME)
# define USBPUMP_IOCTL_GET_VIDPID	\
   __TMS_USBPUMP_IOCTL_GET_VIDPID
# define USBPUMP_IOCTL_GET_STRING	\
   __TMS_USBPUMP_IOCTL_GET_STRING
# define USBPUMP_IOCTL_GET_SERIALNO	\
   __TMS_USBPUMP_IOCTL_GET_SERIALNO
# define USBPUMP_IOCTL_GET_MACADDRESS	\
   __TMS_USBPUMP_IOCTL_GET_MACADDRESS
# define USBPUMP_IOCTL_GET_UPLATFORM	\
   __TMS_USBPUMP_IOCTL_GET_UPLATFORM
# define USBPUMP_IOCTL_GET_OBJECT_ROOT	\
   __TMS_USBPUMP_IOCTL_GET_OBJECT_ROOT
# define USBPUMP_IOCTL_ADD_CHILD	\
   __TMS_USBPUMP_IOCTL_ADD_CHILD
# define USBPUMP_IOCTL_ADD_NAMED_OBJECT	\
   __TMS_USBPUMP_IOCTL_ADD_NAMED_OBJECT
# define USBPUMP_IOCTL_DELETE_CHILD	\
   __TMS_USBPUMP_IOCTL_DELETE_CHILD
# define USBPUMP_IOCTL_DELETE_NAMED_OBJECT	\
   __TMS_USBPUMP_IOCTL_DELETE_NAMED_OBJECT
# define USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS	\
   __TMS_USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS
# define USBPUMP_IOCTL_GET_UDEVICE	\
   __TMS_USBPUMP_IOCTL_GET_UDEVICE
# define USBPUMP_IOCTL_REMOTE_WAKEUP	\
   __TMS_USBPUMP_IOCTL_REMOTE_WAKEUP
# define USBPUMP_IOCTL_DEVICE_STOP	\
   __TMS_USBPUMP_IOCTL_DEVICE_STOP
# define USBPUMP_IOCTL_GET_VIDPIDMODE	\
   __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE
# define USBPUMP_IOCTL_GET_VIDPIDMODE_EX	\
   __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_EX
# define USBPUMP_IOCTL_GET_VIDPIDMODE_V3	\
   __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_V3
# define USBPUMP_IOCTL_INIT_DEVMODEFN	\
   __TMS_USBPUMP_IOCTL_INIT_DEVMODEFN
# define USBPUMP_IOCTL_DEVICE_START	\
   __TMS_USBPUMP_IOCTL_DEVICE_START
# define USBPUMP_IOCTL_DCD_SESSION_REQUEST	\
   __TMS_USBPUMP_IOCTL_DCD_SESSION_REQUEST
# define USBPUMP_IOCTL_DCD_HNP_ENABLED	\
   __TMS_USBPUMP_IOCTL_DCD_HNP_ENABLED
# define USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED	\
   __TMS_USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED
# define USBPUMP_IOCTL_GET_CONFIG_POWER	\
   __TMS_USBPUMP_IOCTL_GET_CONFIG_POWER
# define USBPUMP_IOCTL_GET_AUTO_REMOTE_WAKEUP_STATE	\
   __TMS_USBPUMP_IOCTL_GET_AUTO_REMOTE_WAKEUP_STATE
# define USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE	\
   __TMS_USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE
# define USBPUMP_IOCTL_PROCESS_CONTROL_PACKET	\
   __TMS_USBPUMP_IOCTL_PROCESS_CONTROL_PACKET
# define USBPUMP_IOCTL_GET_INTERRUPT_SYSTEM	\
   __TMS_USBPUMP_IOCTL_GET_INTERRUPT_SYSTEM
# define USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT	\
   __TMS_USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT
# define USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN	\
   __TMS_USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN
# define USBPUMP_IOCTL_CALCULATE_SHA_1	\
   __TMS_USBPUMP_IOCTL_CALCULATE_SHA_1
# define USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA	\
   __TMS_USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA
# define USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA	\
   __TMS_USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA
# define USBPUMP_IOCTL_ALLOC_COMMON_BUFFER	\
   __TMS_USBPUMP_IOCTL_ALLOC_COMMON_BUFFER
# define USBPUMP_IOCTL_FREE_COMMON_BUFFER	\
   __TMS_USBPUMP_IOCTL_FREE_COMMON_BUFFER
# define USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR	\
   __TMS_USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR
# define USBPUMP_IOCTL_REGISTER_DUMP	\
   __TMS_USBPUMP_IOCTL_REGISTER_DUMP
# define USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR	\
   __TMS_USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR
# define USBPUMP_IOCTL_GET_PCI_INFO	\
   __TMS_USBPUMP_IOCTL_GET_PCI_INFO
# define USBPUMP_IOCTL_GET_PCI_CONFIG_DATA	\
   __TMS_USBPUMP_IOCTL_GET_PCI_CONFIG_DATA
# define USBPUMP_IOCTL_GET_XHCI_PORT_MAP	\
   __TMS_USBPUMP_IOCTL_GET_XHCI_PORT_MAP
# define USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO	\
   __TMS_USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO
# define USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG	\
   __TMS_USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG
# define USBPUMP_IOCTL_GET_CONTAINER_ID	\
   __TMS_USBPUMP_IOCTL_GET_CONTAINER_ID
# define USBPUMP_IOCTL_DELAY_MICRO_SECONDS	\
   __TMS_USBPUMP_IOCTL_DELAY_MICRO_SECONDS
# define USBPUMP_IOCTL_ADD_CHILD_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pNewChild							\
	)	\
	__TMS_USBPUMP_IOCTL_ADD_CHILD_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pNewChild							\
	)
# define USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pNewObject							\
	)	\
	__TMS_USBPUMP_IOCTL_ADD_NAMED_OBJECT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pNewObject							\
	)
# define USBPUMP_IOCTL_DELETE_CHILD_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pChild							\
	)	\
	__TMS_USBPUMP_IOCTL_DELETE_CHILD_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pChild							\
	)
# define USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject							\
	)	\
	__TMS_USBPUMP_IOCTL_DELETE_NAMED_OBJECT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject							\
	)
# define USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pThisObject,						\
	ARG_pPattern,							\
	ARG_nPattern							\
	)	\
	__TMS_USBPUMP_IOCTL_ENUMERATE_MATCHING_OBJECTS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pThisObject,						\
	ARG_pPattern,							\
	ARG_nPattern							\
	)
# define USBPUMP_IOCTL_GET_VIDPIDMODE_INIT_V1(			\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode							\
	)	\
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_INIT_V1(			\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode							\
	)
# define USBPUMP_IOCTL_GET_VIDPIDMODE_SETUP_V1(			\
	a_pArg,								\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode							\
	)	\
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_SETUP_V1(			\
	a_pArg,								\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode							\
	)
# define USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE_NONE	\
   __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE_NONE
# define USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE_ON	\
   __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE_ON
# define USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE_OFF	\
   __TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_RWUP_OVERRIDE_OFF
# define USBPUMP_IOCTL_GET_VIDPIDMODE_EX_INIT_V1(			\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup						\
	)	\
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_EX_INIT_V1(			\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup						\
	)
# define USBPUMP_IOCTL_GET_VIDPIDMODE_EX_SETUP_V1(			\
	a_pArg,								\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup						\
	)	\
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_EX_SETUP_V1(			\
	a_pArg,								\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup						\
	)
# define USBPUMP_IOCTL_GET_VIDPIDMODE_V3_INIT_V1(			\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup,						\
	a_NumConfigurations						\
	)	\
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_V3_INIT_V1(			\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup,						\
	a_NumConfigurations						\
	)
# define USBPUMP_IOCTL_GET_VIDPIDMODE_V3_SETUP_V1(			\
	a_pArg,								\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup,						\
	a_NumConfigurations						\
	)	\
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_V3_SETUP_V1(			\
	a_pArg,								\
	a_idVendor,							\
	a_idProduct,							\
	a_bcdDevice,							\
	a_sDeviceClass,							\
	a_sDeviceSubClass,						\
	a_sDeviceProtocol,						\
	a_ulMode,							\
	a_fRenumberSingleConfig,					\
	a_OverrideRemoteWakeup,						\
	a_NumConfigurations						\
	)
# define USBPUMP_IOCTL_INIT_DEVMODEFN_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pDeviceModeFn,						\
	ARG_pDeviceModeCtx						\
	)	\
	__TMS_USBPUMP_IOCTL_INIT_DEVMODEFN_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pDeviceModeFn,						\
	ARG_pDeviceModeCtx						\
	)
# define USBPUMP_IOCTL_DCD_HNP_ENABLED_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fState							\
	)	\
	__TMS_USBPUMP_IOCTL_DCD_HNP_ENABLED_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fState							\
	)
# define USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pDevice,							\
	a_bTestMode							\
	)	\
	__TMS_USBPUMP_IOCTL_DCD_TEST_MODE_ENABLED_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pDevice,							\
	a_bTestMode							\
	)
# define USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_fAutoRemoteWakeup						\
	)	\
	__TMS_USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_fAutoRemoteWakeup						\
	)
# define USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG_SETUP_V2(	\
	a_pArg,								\
	a_fAutoRemoteWakeup,						\
	a_fValidateEndpoint						\
	)	\
	__TMS_USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE_ARG_SETUP_V2(	\
	a_pArg,								\
	a_fAutoRemoteWakeup,						\
	a_fValidateEndpoint						\
	)
# define USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_pPacket							\
	)	\
	__TMS_USBPUMP_IOCTL_PROCESS_CONTROL_PACKET_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_pPacket							\
	)
# define USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_SETUP_V2(	\
	a_pArg,							\
	a_fEnable,						\
	a_fBeslSupport,						\
	a_fBaselineBeslValid,					\
	a_fDeepBeslValid,					\
	a_bBaselineBeslValue,					\
	a_bDeepBeslValue					\
	)	\
	__TMS_USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_SETUP_V2(	\
	a_pArg,							\
	a_fEnable,						\
	a_fBeslSupport,						\
	a_fBaselineBeslValid,					\
	a_fDeepBeslValid,					\
	a_bBaselineBeslValue,					\
	a_bDeepBeslValue					\
	)
# define USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_SETUP_V1(	\
	a_pArg,							\
	a_fEnable						\
	)	\
	__TMS_USBPUMP_IOCTL_GET_LINK_POWER_MANAGEMENT_SETUP_V1(	\
	a_pArg,							\
	a_fEnable						\
	)
# define USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pThisObject							\
	)	\
	__TMS_USBPUMP_IOCTL_ENUMERATE_CLASS_CHILDREN_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pThisObject							\
	)
# define USBPUMP_IOCTL_CALCULATE_SHA_1_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pInputBuffer,						\
	ARG_InputBufferSizeInBytes,					\
	ARG_pOutputBuffer						\
	)	\
	__TMS_USBPUMP_IOCTL_CALCULATE_SHA_1_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pInputBuffer,						\
	ARG_InputBufferSizeInBytes,					\
	ARG_pOutputBuffer						\
	)
# define USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_CAP_ID	\
   __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_CAP_ID
# define USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_NEXT_ITEM_PTR	\
   __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_NEXT_ITEM_PTR
# define USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_PMC	\
   __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_PMC
# define USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_PMCSR	\
   __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_PMCSR
# define USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_BSE	\
   __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_BSE
# define USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_DATA	\
   __TMS_USBPUMP_IOCTL_PCI_POWER_MANAGEMENT_DATA_DATA
# define USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_ucRegister							\
	)	\
	__TMS_USBPUMP_IOCTL_GET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_ucRegister							\
	)
# define USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_ucRegister,							\
	ARG_usMaskToAnd,						\
	ARG_usMaskToOr							\
	)	\
	__TMS_USBPUMP_IOCTL_SET_PCI_POWER_MANAGEMENT_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_ucRegister,							\
	ARG_usMaskToAnd,						\
	ARG_usMaskToOr							\
	)
# define USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG_SETUP_V1(		\
	pArg,								\
	ARG_SizeInBytes,						\
	ARG_AlignmentInBytes,						\
	ARG_Access							\
	)	\
	__TMS_USBPUMP_IOCTL_ALLOC_COMMON_BUFFER_ARG_SETUP_V1(		\
	pArg,								\
	ARG_SizeInBytes,						\
	ARG_AlignmentInBytes,						\
	ARG_Access							\
	)
# define USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG_SETUP_V1(	\
	pArg,							\
	ARG_hBuffer,						\
	ARG_pBuffer						\
	)	\
	__TMS_USBPUMP_IOCTL_FREE_COMMON_BUFFER_ARG_SETUP_V1(	\
	pArg,							\
	ARG_hBuffer,						\
	ARG_pBuffer						\
	)
# define USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR_SETUP_V1(	\
	a_pArg,							\
	a_pCurrentDesc,						\
	a_nCurrentDesc,						\
	a_ucRequestType,					\
	a_ucDescrType,						\
	a_ucDescrIndex,						\
	a_bCurrentSpeed,					\
	a_wIndex						\
	)	\
	__TMS_USBPUMP_IOCTL_OVERRIDE_DESCRIPTOR_SETUP_V1(	\
	a_pArg,							\
	a_pCurrentDesc,						\
	a_nCurrentDesc,						\
	a_ucRequestType,					\
	a_ucDescrType,						\
	a_ucDescrIndex,						\
	a_bCurrentSpeed,					\
	a_wIndex						\
	)
# define USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR_SETUP_V1(	\
	a_pArg,								\
	a_bCurrentSpeed,						\
	a_bConfigurationValue,						\
	a_bInterfaceNumber,						\
	a_bAlternateSetting,						\
	a_bInterfaceClass,						\
	a_bInterfaceSubClass,						\
	a_bInterfaceProtocol,						\
	a_pFuncDescWire							\
	)	\
	__TMS_USBPUMP_IOCTL_FILTER_FUNCTIONAL_DESCRIPTOR_SETUP_V1(	\
	a_pArg,								\
	a_bCurrentSpeed,						\
	a_bConfigurationValue,						\
	a_bInterfaceNumber,						\
	a_bAlternateSetting,						\
	a_bInterfaceClass,						\
	a_bInterfaceSubClass,						\
	a_bInterfaceProtocol,						\
	a_pFuncDescWire							\
	)
# define USBPUMP_IOCTL_GET_PCI_INFO_SETUP_V1(	\
	a_pArg,						\
	a_VendorID,					\
	a_DeviceID,					\
	a_RevisionID,					\
	a_ProgramInterface,				\
	a_SubClass,					\
	a_BaseClass,					\
	a_SubsystemVendorID,				\
	a_SubsystemID					\
	)	\
	__TMS_USBPUMP_IOCTL_GET_PCI_INFO_SETUP_V1(	\
	a_pArg,						\
	a_VendorID,					\
	a_DeviceID,					\
	a_RevisionID,					\
	a_ProgramInterface,				\
	a_SubClass,					\
	a_BaseClass,					\
	a_SubsystemVendorID,				\
	a_SubsystemID					\
	)
# define USBPUMP_IOCTL_GET_PCI_CONFIG_DATA_SETUP_V1( \
	a_pArg,						\
	a_pBuffer,					\
	a_nBuffer,					\
	a_Offset					\
	)	\
	__TMS_USBPUMP_IOCTL_GET_PCI_CONFIG_DATA_SETUP_V1( \
	a_pArg,						\
	a_pBuffer,					\
	a_nBuffer,					\
	a_Offset					\
	)
# define USBPUMP_IOCTL_GET_XHCI_PORT_MAP_SETUP_V1( \
	a_pArg,						\
	a_fUsb2Port,					\
	a_pUsbHcPortMap,				\
	a_nUsbHcPortMap					\
	)	\
	__TMS_USBPUMP_IOCTL_GET_XHCI_PORT_MAP_SETUP_V1( \
	a_pArg,						\
	a_fUsb2Port,					\
	a_pUsbHcPortMap,				\
	a_nUsbHcPortMap					\
	)
# define USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO_SETUP_V1(	\
	a_pArg,							\
	a_iLun,							\
	a_LoLba,						\
	a_HiLba							\
	)	\
	__TMS_USBPUMP_IOCTL_GET_MASS_STORAGE_LUN_INFO_SETUP_V1(	\
	a_pArg,							\
	a_iLun,							\
	a_LoLba,						\
	a_HiLba							\
	)
# define USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_SETUP_V1(	\
	a_pArg,							\
	a_PacketMode,						\
	a_SendFrameMode,					\
	a_NumSendFrames,					\
	a_SendFrameMinLen,					\
        a_SendFrameMaxLen,					\
        a_ConnectTimeout,					\
        a_SendFrameQueueTimeout,				\
        a_SendFrameQueueSize,					\
        a_NumSendQes,						\
        a_NumReceiveQes,					\
        a_MaxFramesPerSendQe					\
	)	\
	__TMS_USBPUMP_IOCTL_GET_ANIC_PKTLOOP_CONFIG_SETUP_V1(	\
	a_pArg,							\
	a_PacketMode,						\
	a_SendFrameMode,					\
	a_NumSendFrames,					\
	a_SendFrameMinLen,					\
        a_SendFrameMaxLen,					\
        a_ConnectTimeout,					\
        a_SendFrameQueueTimeout,				\
        a_SendFrameQueueSize,					\
        a_NumSendQes,						\
        a_NumReceiveQes,					\
        a_MaxFramesPerSendQe					\
	)
# define USBPUMP_IOCTL_GET_CONTAINER_ID_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pClientObject,					\
	a_pContainerID						\
	)	\
	__TMS_USBPUMP_IOCTL_GET_CONTAINER_ID_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pClientObject,					\
	a_pContainerID						\
	)
# define USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG_SETUP_V1(	\
	pArg,							\
	ARG_uMicroSeconds					\
	)	\
	__TMS_USBPUMP_IOCTL_DELAY_MICRO_SECONDS_ARG_SETUP_V1(	\
	pArg,							\
	ARG_uMicroSeconds					\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctlcodes.h ****/
#endif /* _USBIOCTLCODES_H_ */
