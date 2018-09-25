/* usbpump_usbdi_class_msd_api.h	Tue Feb 28 2017 12:08:59 tmm */

/*

Module:  usbpump_usbdi_class_msd_api.h

Function:
	Definition of API Msd Device functions for USBDI clients.

Version:
	V3.19b	Tue Feb 28 2017 12:08:59 tmm	Edit level 3

Copyright notice:
	This file copyright (C) 2013-2014, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	August 2013

Revision history:
   3.11d  Thu Aug 15 2013 11:43:58 chwon
	9010: Module created.

   3.13b  Thu Dec 04 2014 00:16:58  chwon
	18701: Add USBPUMP_USBDI_MSD_FUNCTION_SUBMIT_SCSI_REQUEST_FN in the
	in-call switch.

   3.19b  Tue Feb 28 2017 12:08:59  tmm
        Documentation changes only.

*/

#ifndef _USBPUMP_USBDI_CLASS_MSD_API_H_	/* prevent multiple includes */
#define _USBPUMP_USBDI_CLASS_MSD_API_H_

#ifndef _USBPUMP_USBDI_CLASS_H_
# include "usbpump_usbdi_class.h"
#endif

#ifndef _USBPUMP_USBDI_FUNCTION_H_
# include "usbpump_usbdi_function.h"
#endif

#ifndef _USBPUMP_USBDI_INIT_H_
# include "usbpump_usbdi_init.h"
#endif

#ifndef _USBPUMP_CLASSKIT_API_H_
# include "usbpump_classkit_api.h"
#endif

#ifndef _USBPUMP_USBDI_CLASS_MSD_REQUEST_H_
# include "usbpump_usbdi_class_msd_request.h"
#endif

/****************************************************************************\
|
|	Msd driver names
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_CLASS_MSD_NAME			\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_NAME("msd")

#define	__TMS_USBPUMP_USBDI_FUNCTION_MSD_NAME			\
	__TMS_USBPUMP_USBDI_FUNCTION_NAME("msd")

#define	__TMS_USBPUMP_USBDI_CLASS_MSD_GUID_INIT()		\
	__TMS_USBUUID_INIT(0x43acea13, 0xfca4, 0x4983,		\
		0xa7, 0x3c, 0x1a, 0x9a, 0xe8, 0xd5, 0xd7, 0x62)


/****************************************************************************\
|
|	Msd driver class in-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_USBDI_MSD_CLASS_GET_DRIVER_FEATURE_FN

Function:
	Learn capabilities of mass storage driver

Definition:
	typedef BOOL
	    (USBPUMP_USBDI_MSD_CLASS_GET_DRIVER_FEATURE_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		CONST TEXT *				pFeatureName,
		SIZE_T					sizeFeatureName
		);

Description:
	This is mass storage driver class in-call API. It will be called by
	client to determine a feature which the mass storage class driver
	supports.

Returns:
	TRUE if feature is present, otherwise FALSE.

Notes:
	Useful in order for clients to learn capabilities of mass storage
	class driver.
	Would be better (instead of version) to define "feature masks"
	- "I support Isoch"
	- "I support high-speed"
	- "I support wireless"
	- "I have bug-fix XXX"
	Could use getter methods feature-by-feature
	Could use getter methods based on string feature IDs

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_MSD_CLASS_GET_DRIVER_FEATURE_FN,
__TMS_BOOL,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_CONST __TMS_TEXT *	/* pFeatureName */,
	__TMS_SIZE_T			/* sizeFeatureName */
	));


/****************************************************************************\
|
|	Msd driver class in-call definition
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_CLASS_MSD_INCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_CLASS_MSD_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER;

	/* msd driver specific in-calls */
	__TMS_USBPUMP_USBDI_MSD_CLASS_GET_DRIVER_FEATURE_FN *
							pGetDriverFeatureFn;

	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER;
	};

/* __TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_CLASS_MSD_INCALL);	*/
union __TMS_UNIONNAME	(USBPUMP_USBDI_CLASS_MSD_INCALL)
	{
	__TMS_USBPUMP_USBDI_CLASS_MSD_INCALL_CONTENTS	Msd;
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS__UNION;
	};

#define	__TMS_USBPUMP_USBDI_CLASS_MSD_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)								\
	{{								\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_INIT_V1(		\
		sizeof(__TMS_USBPUMP_USBDI_CLASS_MSD_INCALL),		\
		a_pCloseFn,						\
		a_pOpenFunctionFn,					\
		a_pGetNumDevicesFn,					\
		a_pGetBoundDevicesFn					\
		),							\
	/* msd driver specific in-calls setup */			\
	(a_pGetDriverFeatureFn),					\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_INIT_V1(		\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_USBDI_CLASS_MSD_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)								\
    do	{								\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_SETUP_V1(		\
		a_pInCall,						\
		sizeof(*(a_pInCall)),					\
		a_pCloseFn,						\
		a_pOpenFunctionFn,					\
		a_pGetNumDevicesFn,					\
		a_pGetBoundDevicesFn					\
		);							\
	/* msd driver specific in-calls setup */			\
	(a_pInCall)->Msd.pGetDriverFeatureFn = (a_pGetDriverFeatureFn);	\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_SETUP_V1(		\
		a_pInCall,						\
		Msd,							\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		);							\
	} while (0)


/****************************************************************************\
|
|	Msd driver function in-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_USBDI_MSD_FUNCTION_GET_DEVICE_INFO_FN

Function:
	Get mass storage device information

Definition:
	typedef USBPUMP_CLASSKIT_STATUS
	    (USBPUMP_USBDI_MSD_FUNCTION_GET_DEVICE_INFO_FN)(
		USBPUMP_SESSION_HANDLE		FunctionHandle,
		UINT *				pNumberOfLun,
		UINT8 *				pSerialNo,	-- Optional
		BYTES				nSerialNo
		);

Description:
	This API gets attached mass storage device information.

Returns:
	USBPUMP_CLASSKIT_STATUS_OK
	USBPUMP_CLASSKIT_STATUS_INVALID_FUNCTION_HANDLE
	USBPUMP_CLASSKIT_STATUS_INVALID_PARAMETER

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_MSD_FUNCTION_GET_DEVICE_INFO_FN,
__TMS_USBPUMP_CLASSKIT_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* FunctionHandle */,
	__TMS_UINT *				/* pNumberOfLun */,
	__TMS_UINT8 *				/* pSerialNo -- OPTIONAL */,
	__TMS_BYTES				/* nSerialNo */
	));

/*

Type:  USBPUMP_USBDI_MSD_FUNCTION_GET_LUN_INFO_FN

Function:
	Get LUN information of attached mass storage device.

Definition:
	typedef USBPUMP_CLASSKIT_STATUS
	  (USBPUMP_USBDI_MSD_FUNCTION_GET_LUN_INFO_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		UINT					Lun,
		USBPUMP_USBDI_MSD_LUN_INFO *		pLunInfo
		);

Description:
	This API gets LUN information of attached mass storage device for given
	LUN number.

Returns:
	USBPUMP_CLASSKIT_STATUS_OK
	USBPUMP_CLASSKIT_STATUS_INVALID_FUNCTION_HANDLE
	USBPUMP_CLASSKIT_STATUS_INVALID_PARAMETER

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_MSD_LUN_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_MSD_LUN_INFO)
	{
	__TMS_CHAR	VendorId[8+1];
	__TMS_CHAR	ProductId[16+1];
	__TMS_CHAR	ProductRevision[4+1];

	__TMS_UINT8	DeviceType;
	__TMS_UINT8	MediaType;
	__TMS_BOOL	fRemovableMedia;
	};

/* DeviceType */
#define	__TMS_USBPUMP_MSD_DEVICE_TYPE_UNKNOWN		0
#define	__TMS_USBPUMP_MSD_DEVICE_TYPE_DISK		1
#define	__TMS_USBPUMP_MSD_DEVICE_TYPE_CD		2
#define	__TMS_USBPUMP_MSD_DEVICE_TYPE_OPTICAL		3
#define	__TMS_USBPUMP_MSD_DEVICE_TYPE_FLASH		4

#define __TMS_USBPUMP_MSD_DEVICE_TYPE__INIT		\
	"UNKNOWN",					\
	"DISK",						\
	"CD",						\
	"OPTICAL",					\
	"FLASH"

/* MediaType */
#define __TMS_USBPUMP_MSD_MEDIA_TYPE_NO_MEDIA		0
#define __TMS_USBPUMP_MSD_MEDIA_TYPE_DISK		1
#define __TMS_USBPUMP_MSD_MEDIA_TYPE_CD			2
#define __TMS_USBPUMP_MSD_MEDIA_TYPE_DVD		3
#define __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_SD		4
#define __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_COMPACTF	5
#define __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_MEMSTICK	6
#define __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_SMARTMD	7
#define __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_XD		8
#define __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_UNKNOWN	9

#define	__TMS_USBPUMP_MSD_MEDIA_TYPE__INIT		\
	"NO MEDIA",					\
	"DISK",						\
	"CD",						\
	"DVD",						\
	"FLASH_SD",					\
	"FLASH_COMPACTF",				\
	"FLASH_MEMSTICK",				\
	"FLASH_SMARTMD",				\
	"FLASH_XD",					\
	"FLASH_UNKNOWN"

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_MSD_FUNCTION_GET_LUN_INFO_FN,
__TMS_USBPUMP_CLASSKIT_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* FunctionHandle */,
	__TMS_UINT				/* Lun */,
	__TMS_USBPUMP_USBDI_MSD_LUN_INFO *	/* pLunInfo */
	));

/*

Type:  USBPUMP_USBDI_MSD_FUNCTION_GET_MEDIA_INFO_FN

Function:
	Get media information of the LUN of the mass storage device.

Definition:
	typedef USBPUMP_CLASSKIT_STATUS
	    (USBPUMP_USBDI_MSD_FUNCTION_GET_MEDIA_INFO_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		UINT					Lun,
		USBPUMP_USBDI_MSD_MEDIA_INFO *		pMediaInfo
		);

Description:
	This API gets the media information of the specified LUN of the mass
	storage device.

Returns:
	USBPUMP_CLASSKIT_STATUS_OK
	USBPUMP_CLASSKIT_STATUS_INVALID_FUNCTION_HANDLE
	USBPUMP_CLASSKIT_STATUS_INVALID_PARAMETER

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_MSD_MEDIA_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_MSD_MEDIA_INFO)
	{
	__TMS_BOOL		fHasMedia;
	__TMS_BOOL		fWriteProtected;
	__TMS_UINT32		SizeOfBlock;
	__TMS_UINT32		TotalBlocks;
	};

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_MSD_FUNCTION_GET_MEDIA_INFO_FN,
__TMS_USBPUMP_CLASSKIT_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* FunctionHandle */,
	__TMS_UINT				/* Lun */,
	__TMS_USBPUMP_USBDI_MSD_MEDIA_INFO *	/* pMediaInfo */
	));


/*

Type:  USBPUMP_USBDI_MSD_FUNCTION_SUBMIT_REQUEST_FN

Function:
	Submit MSD request

Definition:
	typedef VOID
	    (USBPUMP_USBDI_MSD_FUNCTION_SUBMIT_REQUEST_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_CLASS_MSD_REQUEST *	pMsdRequest
		);

Description:
	This API submits MSD request to the mass storage function driver.
	The USBPUMP_USBDI_CLASS_MSD_REQUEST::pDoneFn function will be called
	when this request is completed.

Returns:
	No explicit result.

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_MSD_FUNCTION_SUBMIT_REQUEST_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* FunctionHandle */,
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST *	/* pMsdRequest */
	));


/*

Type:  USBPUMP_USBDI_MSD_FUNCTION_SUBMIT_SCSI_REQUEST_FN

Function:
	Submit SCSI command request

Definition:
	typedef VOID
	    (USBPUMP_USBDI_MSD_FUNCTION_SUBMIT_SCSI_REQUEST_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_CLASS_MSD_REQUEST *	pMsdRequest
		);

Description:
	This API submits MSD SCSI request to the mass storage function driver.
	The USBPUMP_USBDI_CLASS_MSD_REQUEST::pDoneFn function will be called
	when this request is completed.

Returns:
	No explicit result.

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_MSD_FUNCTION_SUBMIT_SCSI_REQUEST_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* FunctionHandle */,
	__TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST *	/* pMsdRequest */
	));


/****************************************************************************\
|
|	Msd driver function in-call definition
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_FUNCTION_MSD_INCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_FUNCTION_MSD_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER;

	/* msd driver instance function in-call functions here */
	__TMS_USBPUMP_USBDI_MSD_FUNCTION_GET_DEVICE_INFO_FN *	pGetDeviceInfoFn;
	__TMS_USBPUMP_USBDI_MSD_FUNCTION_GET_LUN_INFO_FN *	pGetLunInfoFn;
	__TMS_USBPUMP_USBDI_MSD_FUNCTION_GET_MEDIA_INFO_FN *	pGetMediaInfoFn;
	__TMS_USBPUMP_USBDI_MSD_FUNCTION_SUBMIT_REQUEST_FN *	pSubmitRequestFn;
	__TMS_USBPUMP_USBDI_MSD_FUNCTION_SUBMIT_SCSI_REQUEST_FN *
								pSubmitScsiReqFn;

	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER;
	};

/* __TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_FUNCTION_MSD_INCALL);	*/
union __TMS_UNIONNAME	(USBPUMP_USBDI_FUNCTION_MSD_INCALL)
	{
	__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL_CONTENTS	Msd;
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS__UNION;
	};


#define	__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL_INIT_V2(		\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn,						\
	a_pSubmitScsiReqFn						\
	)								\
	{{								\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_INIT_V1(		\
		sizeof(__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL),	\
		a_pCloseFn						\
		),							\
	/* msd driver specific in-calls setup */			\
	(a_pGetDeviceInfoFn),						\
	(a_pGetLunInfoFn),						\
	(a_pGetMediaInfoFn),						\
	(a_pSubmitRequestFn),						\
	(a_pSubmitScsiReqFn),						\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_INIT_V1(		\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn						\
	)								\
	__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL_INIT_V2(		\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn,						\
	/* V2: pSubmitScsiReqFn */ __TMS_NULL				\
	)

#define	__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL_SETUP_V2(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn,						\
	a_pSubmitScsiReqFn						\
	)								\
    do	{								\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_SETUP_V1(		\
		a_pInCall,						\
		sizeof(*(a_pInCall)),					\
		a_pCloseFn						\
		);							\
	/* msd driver specific in-calls setup */			\
	(a_pInCall)->Msd.pGetDeviceInfoFn = (a_pGetDeviceInfoFn);	\
	(a_pInCall)->Msd.pGetLunInfoFn = (a_pGetLunInfoFn);		\
	(a_pInCall)->Msd.pGetMediaInfoFn = (a_pGetMediaInfoFn);		\
	(a_pInCall)->Msd.pSubmitRequestFn = (a_pSubmitRequestFn);	\
	(a_pInCall)->Msd.pSubmitScsiReqFn = (a_pSubmitScsiReqFn);	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_SETUP_V1(	\
		a_pInCall,						\
		Msd,							\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		);							\
	} while (0)

#define	__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn						\
	)								\
	__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL_SETUP_V2(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn,						\
	/* V2: pSubmitScsiReqFn */ __TMS_NULL				\
	)


/****************************************************************************\
|
|	Msd Class Driver Status Codes
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_MSD_STATUS_BASE			\
	__TMS_USBPUMP_CLASSKIT_STATUS__MAX


/****************************************************************************\
|
|	Msd Class Driver Notification Definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_MSD_EVENT_BASE			\
	__TMS_USBPUMP_CLASSKIT_EVENT__MAX

#define	__TMS_USBPUMP_USBDI_MSD_EVENT_MEDIA_CHANGE		\
	(__TMS_USBPUMP_USBDI_MSD_EVENT_BASE + 0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_MSD_EVENT_MEDIA_CHANGE_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_MSD_EVENT_MEDIA_CHANGE_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_UINT			Lun;
	__TMS_BOOL			fHasMedia;
	__TMS_UINT32			SizeOfBlock;
	__TMS_UINT32			TotalBlocks;
	};

#define	__TMS_USBPUMP_USBDI_MSD_EVENT_MEDIA_CHANGE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_Lun,								\
	a_fHasMedia,							\
	a_SizeOfBlock,							\
	a_TotalBlocks							\
	)								\
    do	{								\
	(a_pInfo)->pFunction = (a_pFunction);				\
	(a_pInfo)->Lun = (a_Lun);					\
	(a_pInfo)->fHasMedia = (a_fHasMedia);				\
	(a_pInfo)->SizeOfBlock = (a_SizeOfBlock);			\
	(a_pInfo)->TotalBlocks = (a_TotalBlocks);			\
	} while (0)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_CONST __TMS_USBGUID
gk_UsbPumpUsbdiClassMsd_Guid;

extern __TMS_CONST __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG
gk_UsbPumpUsbdiMsd_ClassConfigDefault;

__TMS_USBPUMP_USBDI_DRIVER_CLASS_INIT_FN
UsbPumpUsbdiClassMsd_Initialize;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_CLASS_MSD_NAME	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_NAME
# define USBPUMP_USBDI_FUNCTION_MSD_NAME	\
   __TMS_USBPUMP_USBDI_FUNCTION_MSD_NAME
# define USBPUMP_USBDI_CLASS_MSD_GUID_INIT()	\
   __TMS_USBPUMP_USBDI_CLASS_MSD_GUID_INIT()
# define USBPUMP_USBDI_CLASS_MSD_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)	\
	__TMS_USBPUMP_USBDI_CLASS_MSD_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)
# define USBPUMP_USBDI_CLASS_MSD_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)	\
	__TMS_USBPUMP_USBDI_CLASS_MSD_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetDriverFeatureFn						\
	)
# define USBPUMP_MSD_DEVICE_TYPE_UNKNOWN	\
   __TMS_USBPUMP_MSD_DEVICE_TYPE_UNKNOWN
# define USBPUMP_MSD_DEVICE_TYPE_DISK	\
   __TMS_USBPUMP_MSD_DEVICE_TYPE_DISK
# define USBPUMP_MSD_DEVICE_TYPE_CD	\
   __TMS_USBPUMP_MSD_DEVICE_TYPE_CD
# define USBPUMP_MSD_DEVICE_TYPE_OPTICAL	\
   __TMS_USBPUMP_MSD_DEVICE_TYPE_OPTICAL
# define USBPUMP_MSD_DEVICE_TYPE_FLASH	\
   __TMS_USBPUMP_MSD_DEVICE_TYPE_FLASH
# define USBPUMP_MSD_DEVICE_TYPE__INIT	\
   __TMS_USBPUMP_MSD_DEVICE_TYPE__INIT
# define USBPUMP_MSD_MEDIA_TYPE_NO_MEDIA	\
   __TMS_USBPUMP_MSD_MEDIA_TYPE_NO_MEDIA
# define USBPUMP_MSD_MEDIA_TYPE_DISK	\
   __TMS_USBPUMP_MSD_MEDIA_TYPE_DISK
# define USBPUMP_MSD_MEDIA_TYPE_CD	\
   __TMS_USBPUMP_MSD_MEDIA_TYPE_CD
# define USBPUMP_MSD_MEDIA_TYPE_DVD	\
   __TMS_USBPUMP_MSD_MEDIA_TYPE_DVD
# define USBPUMP_MSD_MEDIA_TYPE_FLASH_SD	\
   __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_SD
# define USBPUMP_MSD_MEDIA_TYPE_FLASH_COMPACTF	\
   __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_COMPACTF
# define USBPUMP_MSD_MEDIA_TYPE_FLASH_MEMSTICK	\
   __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_MEMSTICK
# define USBPUMP_MSD_MEDIA_TYPE_FLASH_SMARTMD	\
   __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_SMARTMD
# define USBPUMP_MSD_MEDIA_TYPE_FLASH_XD	\
   __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_XD
# define USBPUMP_MSD_MEDIA_TYPE_FLASH_UNKNOWN	\
   __TMS_USBPUMP_MSD_MEDIA_TYPE_FLASH_UNKNOWN
# define USBPUMP_MSD_MEDIA_TYPE__INIT	\
   __TMS_USBPUMP_MSD_MEDIA_TYPE__INIT
# define USBPUMP_USBDI_FUNCTION_MSD_INCALL_INIT_V2(		\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn,						\
	a_pSubmitScsiReqFn						\
	)	\
	__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL_INIT_V2(		\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn,						\
	a_pSubmitScsiReqFn						\
	)
# define USBPUMP_USBDI_FUNCTION_MSD_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_USBDI_FUNCTION_MSD_INCALL_SETUP_V2(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn,						\
	a_pSubmitScsiReqFn						\
	)	\
	__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL_SETUP_V2(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn,						\
	a_pSubmitScsiReqFn						\
	)
# define USBPUMP_USBDI_FUNCTION_MSD_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn						\
	)	\
	__TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pGetDeviceInfoFn,						\
	a_pGetLunInfoFn,						\
	a_pGetMediaInfoFn,						\
	a_pSubmitRequestFn						\
	)
# define USBPUMP_USBDI_MSD_STATUS_BASE	\
   __TMS_USBPUMP_USBDI_MSD_STATUS_BASE
# define USBPUMP_USBDI_MSD_EVENT_BASE	\
   __TMS_USBPUMP_USBDI_MSD_EVENT_BASE
# define USBPUMP_USBDI_MSD_EVENT_MEDIA_CHANGE	\
   __TMS_USBPUMP_USBDI_MSD_EVENT_MEDIA_CHANGE
# define USBPUMP_USBDI_MSD_EVENT_MEDIA_CHANGE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_Lun,								\
	a_fHasMedia,							\
	a_SizeOfBlock,							\
	a_TotalBlocks							\
	)	\
	__TMS_USBPUMP_USBDI_MSD_EVENT_MEDIA_CHANGE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_Lun,								\
	a_fHasMedia,							\
	a_SizeOfBlock,							\
	a_TotalBlocks							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_class_msd_api.h ****/
#endif /* _USBPUMP_USBDI_CLASS_MSD_API_H_ */
