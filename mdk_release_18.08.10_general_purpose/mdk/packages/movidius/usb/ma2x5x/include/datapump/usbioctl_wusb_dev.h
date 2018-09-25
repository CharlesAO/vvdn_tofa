/* usbioctl_wusb_dev.h	Fri Nov 14 2008 11:19:32 ssatish */

/*

Module:  usbioctl_wusb_dev.h

Function:
	Define the commonly-recognized WUSB device async IOCTL codes

Version:
	V1.97j	Fri Nov 14 2008 11:19:32 ssatish	Edit level 3

Copyright notice:
	This file copyright (C) 2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Balakumar, MCCI Corporation	January 2008

Revision history:
   1.97j  Wed Jan 28 2008 17:28:36  balakumar
	UDP-6287: Module created.

   1.97j  Tue Mar 11 2008 15:52:16  balakumar
	IOCTL name change based on ConnSAPI name change.

   1.97j  Fri Nov 14 2008 11:19:32  ssatish
	UDP-6287: Filename changed to usbioctl_wusb_dev.h from usbioctl_wusb.h
	IOCTL names and IOCTL ARGS structure modified.

*/

#ifndef _USBIOCTL_WUSB_DEV_H_		/* prevent multiple includes */
#define _USBIOCTL_WUSB_DEV_H_

/* Necessary Includes */
#ifndef _UTASK_TYPES_H_
# include "utask_types.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _WUSB_DEV_DEFINES_H_
# include "usbpump_wusb_dev_defines.h"
#endif


/****************************************************************************\
|
|	Define helper macros.
|
\****************************************************************************/

#define	__TMS_USBPUMP_IOCTL_CLASS_WUSB		'w'

#define __TMS_USBPUMP_IOCTL_WUSB_DEV(t, i, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_WUSB,				\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_WUSB_DEV_, NAME, _ARG) \
		)


/****************************************************************************\
|
|	Define the WUSB device IOCTL codes. The IOCTL codes are defined
|	together so as to keep a track on the IOCTL index. The actual
|	parameters are defined later
|
\****************************************************************************/

/* scan hosts */
#define	__TMS_USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ASYNC		\
	__TMS_USBPUMP_IOCTL_WUSB_DEV(RW_ASYNC, 0, SCAN_FOR_HOSTS)

/* connect to host */
#define	__TMS_USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ASYNC		\
	__TMS_USBPUMP_IOCTL_WUSB_DEV(RW_ASYNC, 1, CONNECT_TO_HOST)

/* disconnect from host */
#define	__TMS_USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ASYNC		\
	__TMS_USBPUMP_IOCTL_WUSB_DEV(R_ASYNC, 2, DISCONNECT_FROM_HOST)

/* start a new connection - numeric association */
#define	__TMS_USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ASYNC	\
	__TMS_USBPUMP_IOCTL_WUSB_DEV(RW_ASYNC, 3, NUMERIC_ASSOCIATE_WITH_HOST)

/* confirm numeric association */
#define	__TMS_USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ASYNC \
	__TMS_USBPUMP_IOCTL_WUSB_DEV(RW_ASYNC, 4,			\
		NUMERIC_ASSOCIATE_USER_CONFIRM)


/****************************************************************************\
|
|	The IOCTL arguments and documentation
|
\****************************************************************************/

/*

IOCTL:	USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ASYNC

Function:
	Issue scan host and get the available host information.

Input:
	USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ARG has the following
	elements:

	USBPUMP_OBJECT_HEADER * 		pObject
		IN: object header

	USBPUMP_WUSB_DEV_SCAN_HOST_INFO * 	pHostInfo
		IN: buffer to fill host information

	BYTES 					sizeHostInfo
		IN: size of pHostInfo in bytes

	USBPUMP_WUSB_DEV_SCAN_MODE_WIRE 	ScanMode
		IN: scan mode

	BYTES					nHostsFound;
		OUT: Number of Hosts Found

	BYTES 					sizeActualHostInfo
		OUT: Number of bytes copied to pHostInfo

Description:
	This IOCTL issues the scan host command and fetches the available
	host info based on the scan mode. The host data is filled in the
	pHostInfo and the size of this buffer is specified in sizeHostInfo

	To fetch the information, issue the IOCTL and read the result
	available in pHostInfo buffer based on sizeActualHostInfo.

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS: if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ASYNC sends the scan host request
|| and then fills in the pHostInfo buffer with the available host info.
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to
							|| object header
							*/
	__TMS_USBPUMP_WUSB_DEV_SCAN_HOST_INFO *	pHostInfo;/* IN: buffer to
							|| fill the host
							|| data
							*/
	__TMS_BYTES			sizeHostInfo;	/* IN: size of
							|| pHostInfo
							*/
	__TMS_USBPUMP_WUSB_DEV_SCAN_MODE_WIRE	ScanMode;/* IN: Scan Mode */
	__TMS_BYTES			nHostsFound;	/* OUT: Number of
							|| Hosts Found
							*/
	__TMS_BYTES			sizeActualHostInfo ;/* OUT: No of
							|| bytes copied to
							|| pHostInfo buffer
							*/
	};

/* setup for the USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ARG struct */
#define __TMS_USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pHostInfo,							\
	ARG_sizeHostInfo,						\
	ARG_ScanMode							\
	)								\
   do	{								\
	USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pHostInfo      = (ARG_pHostInfo);			\
	__pArg->sizeHostInfo   = (ARG_sizeHostInfo);			\
	__pArg->ScanMode       = (ARG_ScanMode);			\
	__pArg->nHostsFound    = 0;					\
	__pArg->sizeActualHostInfo = 0;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ASYNC

Function:
	Connects to the specified host.

Input:
	USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ARG has the following
	elements:

	USBPUMP_OBJECT_HEADER *			pObject
		IN: object header

	CONST USBPUMP_WUSB_DEV_CONN_CONTEXT *	pCCBuffer
		IN: points to a valid connection context

	BYTES					sizeCCBuffer
		IN: size of connection context

	USBPUMP_WUSB_DEV_CONN_STATUS		ConnStatus;
		OUT: Connection Status

Description:
	This IOCTL connects the device with the host specified in the
	argument.

	To fetch the information, issue the IOCTL and read the status.

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ASYNC sends the connect host request
|| with the specified host and gives the connection status as result
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to
							|| object header
							*/
	__TMS_CONST __TMS_USBPUMP_WUSB_DEV_CONN_CONTEXT *pCCBuffer;
							/* IN:
							||connection
							||context
							*/
	__TMS_BYTES				sizeCCBuffer; /* IN:
							|| connection
							|| context size
							*/
	__TMS_USBPUMP_WUSB_DEV_CONN_STATUS	ConnStatus;/* OUT: connection
							|| status
							*/
	};

/* the setup for the USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ARG struct */
#define __TMS_USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pCCBuffer,							\
	ARG_sizeCCBuffer						\
	)								\
   do	{								\
	USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pCCBuffer      = (ARG_pCCBuffer);			\
	__pArg->sizeCCBuffer   = (ARG_sizeCCBuffer);			\
	__pArg->ConnStatus     = 0;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ASYNC

Function:
	Disconnects the wireless connection.

Input:
	USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ARG has the following
	elements:

	USBPUMP_OBJECT_HEADER *			pObject
		IN: object header

	USBPUMP_WUSB_DEV_CONN_STATUS		ConnStatus;
		OUT: Connection Status

Description:
	This IOCTL disconnects the current active connection with
	the host.

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ASYNC sends the disconnect
|| host request
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *		pObject;/* IN: pointer to
							|| object header
							*/
	__TMS_USBPUMP_WUSB_DEV_CONN_STATUS	ConnStatus;/* OUT: connection
							|| status
							*/
	};

/* the setup for USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ARG struct */
#define __TMS_USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->ConnStatus     = 0;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ASYNC

Function:
	Starts numeric association with a host.

Input:
	USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ARG has the
	following elements:

	USBPUMP_OBJECT_HEADER *		pObject
		IN: object header

	CONST USBPUMP_UTF8_TEXT *	pDeviceFriendlyName
		IN: device name

	BYTES				sizeDeviceFriendlyNameLength
		IN: device name	length

	USBPUMP_WUSB_DEV_NUMERIC_VERIFICATION_NUM	NumericVerificationNum
		OUT: Numeric association verification number

	USBPUMP_WUSB_DEV_NUMERIC_CONN_STATUS		NumericConnStatus
		OUT: Numeric Connection Status

Description:
	This IOCTL sends numeric association request to a wireless host.

	To fetch the information, issue the IOCTL and read the status.

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ASYNC starts numeric
|| association with a wireless host
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *		pObject;/* IN: pointer to
							|| object header
							*/
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *	pDeviceFriendlyName;
							/* IN: device
							|| friendly name
							*/
	__TMS_BYTES				sizeDeviceFriendlyName;
							/* IN: device
							|| name length
							*/
	__TMS_USBPUMP_WUSB_DEV_NUMERIC_VERIFICATION_NUM
						NumericVerificationNum;
							/* OUT: Numeric
							|| association
							|| verification number
							*/
	__TMS_USBPUMP_WUSB_DEV_NUMERIC_CONN_STATUS NumericConnStatus;
							/* OUT: Numeric
							|| connection status
							*/

	};

/* the setup for the USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ARG
|| struct
*/
#define __TMS_USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ARG_SETUP_V1(\
	pArg,								\
	ARG_pObject,							\
	ARG_pDeviceFriendlyName,					\
	ARG_sizeDeviceFriendlyName					\
	)								\
   do	{								\
	USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ARG * __TMS_CONST 	\
		__pArg = (pArg);						\
	__pArg->pObject                   = (ARG_pObject);			\
	__pArg->pDeviceFriendlyName       = (ARG_pDeviceFriendlyName);		\
	__pArg->sizeDeviceFriendlyName	  = (ARG_sizeDeviceFriendlyName);	\
	__pArg->NumericVerificationNum	  = 0;					\
	__pArg->NumericConnStatus	  = 0;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ASYNC

Function:
	Sends the user confirmation.

Input:
	USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ARG has the
	following elements:

	USBPUMP_OBJECT_HEADER *				pObject
		IN: object header

	USBPUMP_WUSB_DEV_NUMERIC_ASSOC_CC_INFO * 	pNumericCCInfoBuf
		IN: buffer to fill host	CC information

	BYTES						sizeNumericCCInfoBuf
		IN: size of pNumericCCInfoBuf in bytes

	BOOL						fUserConfirm
		IN: user confirmation

	USBPUMP_WUSB_DEV_NUMERIC_CONN_STATUS		NumericConnStatus;
		OUT: Numeric Connection Status

Description:
	This IOCTL confirms/denies a new association made with a wireless
	host

Implementation Requirements:
	All implementations must set all fields, unless the parameters
	are not well-formed.

Returns:
	USBPUMP_RESULT_SUCCESS:  if the information was successfully
	returned.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ASYNC
|| confirms/denies numeric association with a wireless host
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to
							|| object header
							*/
	__TMS_USBPUMP_WUSB_DEV_NUMERIC_ASSOC_CC_INFO * 	pNumericCCInfoBuf;
							/* IN: buffer to fill
							|| the CC info
							*/
	__TMS_BYTES			sizeNumericCCInfoBuf;
							/* IN: size of
							|| pNumericCCInfoBuf
							|| buffer
							*/
	__TMS_BOOL			fUserConfirm;	/* IN:
							|| user confirmation
							*/
	__TMS_USBPUMP_WUSB_DEV_NUMERIC_CONN_STATUS 	NumericConnStatus;
							/* OUT: Numeric
							|| connection status
							*/
	};

/* the setup for the USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ARG
|| struct
*/
#define __TMS_USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ARG_SETUP_V1(\
	pArg,								\
	ARG_pObject,							\
	ARG_pNumericCCInfoBuf,						\
	ARG_sizeNumericCCInfoBuf,					\
	ARG_fUserConfirm						\
	)								\
   do	{								\
	USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pObject                   = (ARG_pObject);		\
	__pArg->pNumericCCInfoBuf         = (ARG_pNumericCCInfoBuf);	\
	__pArg->sizeNumericCCInfoBuf      = (ARG_sizeNumericCCInfoBuf);	\
	__pArg->fUserConfirm              = (ARG_fUserConfirm);		\
	__pArg->NumericConnStatus         = 0;				\
   	} while (0)

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_CLASS_WUSB	\
   __TMS_USBPUMP_IOCTL_CLASS_WUSB
# define USBPUMP_IOCTL_WUSB_DEV(t, i, NAME)	\
   __TMS_USBPUMP_IOCTL_WUSB_DEV(t, i, NAME)
# define USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ASYNC	\
   __TMS_USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ASYNC
# define USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ASYNC	\
   __TMS_USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ASYNC
# define USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ASYNC	\
   __TMS_USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ASYNC
# define USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ASYNC	\
   __TMS_USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ASYNC
# define USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ASYNC	\
   __TMS_USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ASYNC
# define USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pHostInfo,							\
	ARG_sizeHostInfo,						\
	ARG_ScanMode							\
	)	\
	__TMS_USBPUMP_IOCTL_WUSB_DEV_SCAN_FOR_HOSTS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pHostInfo,							\
	ARG_sizeHostInfo,						\
	ARG_ScanMode							\
	)
# define USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pCCBuffer,							\
	ARG_sizeCCBuffer						\
	)	\
	__TMS_USBPUMP_IOCTL_WUSB_DEV_CONNECT_TO_HOST_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pCCBuffer,							\
	ARG_sizeCCBuffer						\
	)
# define USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject							\
	)	\
	__TMS_USBPUMP_IOCTL_WUSB_DEV_DISCONNECT_FROM_HOST_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject							\
	)
# define USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ARG_SETUP_V1(\
	pArg,								\
	ARG_pObject,							\
	ARG_pDeviceFriendlyName,					\
	ARG_sizeDeviceFriendlyName					\
	)	\
	__TMS_USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_WITH_HOST_ARG_SETUP_V1(\
	pArg,								\
	ARG_pObject,							\
	ARG_pDeviceFriendlyName,					\
	ARG_sizeDeviceFriendlyName					\
	)
# define USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ARG_SETUP_V1(\
	pArg,								\
	ARG_pObject,							\
	ARG_pNumericCCInfoBuf,						\
	ARG_sizeNumericCCInfoBuf,					\
	ARG_fUserConfirm						\
	)	\
	__TMS_USBPUMP_IOCTL_WUSB_DEV_NUMERIC_ASSOCIATE_USER_CONFIRM_ARG_SETUP_V1(\
	pArg,								\
	ARG_pObject,							\
	ARG_pNumericCCInfoBuf,						\
	ARG_sizeNumericCCInfoBuf,					\
	ARG_fUserConfirm						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_wusb_dev.h ****/
#endif /* _USBIOCTL_WUSB_DEV_H_ */
