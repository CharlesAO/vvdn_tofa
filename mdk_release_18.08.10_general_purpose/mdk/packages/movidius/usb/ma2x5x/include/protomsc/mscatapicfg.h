/* mscatapicfg.h	Sat Sep 02 2006 10:17:42 chwon */

/*

Module:  mscatapicfg.h

Function:
	Data structures to configure an ATAPI USB Mass-Storage subclass protocol

Version:
	V1.97e	Sat Sep 02 2006 10:17:42 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2001-2006 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Mats Webjörn, MCCI Corporation	October 2002

Revision history:
   1.79b  Fri Oct 18 2002 12:00:00  maw
	Module created.

   1.89b  Wed Sep 15 2004 14:45:23  maw
   	Added LUN config structure

   1.91a  Thu Dec 23 2004 11:27:55  chwon
	Improve documentation for autodoc purposes.

   1.97e  Sat Sep 02 2006 10:17:42  chwon
	Add data-in-stuff feature enable flag

*/

#ifndef _MSCATAPICFG_H_		/* prevent multiple includes */
#define _MSCATAPICFG_H_

#ifndef _USBIOCTL_STORAGE_H_	/* prevent multiple includes */
#include "usbioctl_storage.h"
#endif

#include "mscatapi.h"

/****************************************************************************\
|
|	Structures declared here
|
\****************************************************************************/


/****************************************************************************\
|
|	The configuration structure
|
\****************************************************************************/

/*

Type:	UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG

Function:
	Carries configuration information into the portable ATAPI configuration
	logic.

Contents:
	USBPUMP_STORAGE_DEVICE_TYPE DeviceType;
		What kind of device this is

	BOOL fRemovable;
		Is this device removable

	CONST TEXT * pVendorId;
		Vendor-id null terminated string (max __TMS_UPROTO_MSCSUBCLASS_ATAPI_VENDOR_ID_LENGTH bytes)

	CONST TEXT * pProductId;
		Product ID null terminated string (max __TMS_UPROTO_MSCSUBCLASS_ATAPI_PRODUCT_ID_LENGTH bytes)

	CONST TEXT * pVersion;
		Version info null terminated string (max __TMS_UPROTO_MSCSUBCLASS_ATAPI_VERSION_STRING_LENGTH bytes)

Notes:
	This structure is normally initialized using
	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_Vx(), or UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_Vx.

See Also:
	UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_SETUP_V1()
	UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1()

*/

struct __TMS_STRUCTNAME(UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG)
	{
	__TMS_USBPUMP_STORAGE_DEVICE_TYPE	DeviceType;
	__TMS_BOOL				fRemovable;
	__TMS_CONST __TMS_TEXT *		pVendorId;
	__TMS_CONST __TMS_TEXT *		pProductId;
	__TMS_CONST __TMS_TEXT *		pVersion;
	};

/*

Name:	UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_SETUP_V1()

Function:
	Macro: runtime initialization code for UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG()

Definition:
	VOID UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_SETUP_V1(
		UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG *	pConfig,
		USBPUMP_STORAGE_DEVICE_TYPE		DeviceType,
		BOOL					fRemovable,
		CONST TEXT *				pVendorId,
		CONST TEXT *				pProductId,
		CONST TEXT *				pVersion
		);

Description:
	This macro allows code to initialize a UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1()

*/

/* don't add a parameter to this macro, create a _V2... instead */
#define	__TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_SETUP_V1(		\
		pConfig,						\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)							\
	do	{							\
		(pConfig)->DeviceType	   = (ARG_DeviceType);		\
		(pConfig)->fRemovable	   = (ARG_fRemovable);		\
		(pConfig)->pVendorId	   = (ARG_pVendorId);		\
		(pConfig)->pProductId	   = (ARG_pProductId);		\
		(pConfig)->pVersion	   = (ARG_pVersion);		\
		} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_SETUP_V1(			\
		pConfig,						\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)							\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_SETUP_V1(		\
		pConfig,						\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)

#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

Name:	UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1()

Function:
	Macro: compile time initialization for UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG.

Definition:
	UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(
		USBPUMP_STORAGE_DEVICE_TYPE	DeviceType,
		BOOL				fRemovable,
		CONST TEXT *			pVendorId,
		CONST TEXT *			pProductId,
		CONST TEXT *			pVersion
		);

Description:
	This macro is used when initializing a single UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V2... instead */
#define	__TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(		\
		ARG_DeviceType,						\
		ARG_Removable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)							\
	{								\
	(ARG_DeviceType),						\
	(ARG_Removable),						\
	(ARG_pVendorId),						\
	(ARG_pProductId),						\
	(ARG_pVersion)							\
	}


/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(			\
		ARG_DeviceType,						\
		ARG_Removable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)							\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(		\
		ARG_DeviceType,						\
		ARG_Removable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*
Type:	UPROTO_MSCSUBCLASS_ATAPI_CONFIG

Function:
	Carries configuration information into the portable ATAPI configuration
	logic.

Contents:
	CONST UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG Lun0;
		LUN configuration if no LUN vector provided

	CONST UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG * pLun;
		Pointer to vector with LUN configuration parameters

	BYTES nLun;
		Number of elements in LUN config vector

	BOOL fEnableDataInStuff;
		Enable Data IN stuff feature in BOT. This is useful if USB
		device does not support endpoint stall feature.

Notes:
	This structure is normally initialized using
	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_Vx(), or UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_Vx.

See Also:
	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V2()
	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V2()

*/

struct __TMS_STRUCTNAME(UPROTO_MSCSUBCLASS_ATAPI_CONFIG)
	{
	/*
	|| This element is provided for backward compatibility and is only
	|| used if LUN array is not provided
	*/
	__TMS_CONST __TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG	Lun0;

	/*
	|| New LUN configuration vector
	*/
	__TMS_CONST __TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG *	pLun;
	__TMS_BYTES						nLun;

	__TMS_BOOL					fEnableDataInStuff;
	};

/*

Name:	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V3()

Function:
	Macro: runtime initialization code for UPROTO_MSCSUBCLASS_ATAPI_CONFIG()

Definition:
	VOID UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V3(
		UPROTO_MSCSUBCLASS_ATAPI_CONFIG *		pConfig,
		CONST UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG *	pLun,
		BOOL						fEnableDataInStuff
		);

Description:
	This macro allows code to initialize a UPROTO_MSCSUBCLASS_ATAPI_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V3()

*/

/* don't add a parameter to this macro, create a _V4... instead */
#define	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V3(			\
		pConfig,						\
		ARG_pLun,						\
		ARG_fEnableDataInStuff					\
		)							\
	do	{							\
		__TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_SETUP_V1(	\
			&(pConfig)->Lun0,				\
			__TMS_USBPUMP_STORAGE_DEVICE_TYPE_DIRECT_ACCESS,\
			__TMS_FALSE,					\
			__TMS_NULL,					\
			__TMS_NULL,					\
			__TMS_NULL					\
			);						\
		(pConfig)->pLun	= (ARG_pLun);				\
		(pConfig)->nLun	= __TMS_LENOF(ARG_pLun); 		\
		(pConfig)->fEnableDataInStuff = (ARG_fEnableDataInStuff); \
		} while (0)

/*

Name:	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V3()

Function:
	Macro: compile time initialization for UPROTO_MSCSUBCLASS_ATAPI_CONFIG.

Definition:
	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V2(
		CONST UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG *	pLun,
		BOOL						fEnableDataInStuff
		);

Description:
	This macro is used when initializing a single UPROTO_MSCSUBCLASS_ATAPI_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V4... instead */
#define	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V3(			\
		ARG_pLun,						\
		ARG_fEnableDataInStuff					\
		)							\
	{								\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(		\
			__TMS_USBPUMP_STORAGE_DEVICE_TYPE_DIRECT_ACCESS,\
			__TMS_FALSE,					\
			__TMS_NULL,					\
			__TMS_NULL,					\
			__TMS_NULL					\
			),						\
	(ARG_pLun),							\
	__TMS_LENOF(ARG_pLun),						\
	(ARG_fEnableDataInStuff)					\
	}

/*

Name:	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V2()

Function:
	Macro: runtime initialization code for UPROTO_MSCSUBCLASS_ATAPI_CONFIG()

Definition:
	VOID UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V2(
		UPROTO_MSCSUBCLASS_ATAPI_CONFIG *		pConfig,
		CONST UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG *	pLun
		);

Description:
	This macro allows code to initialize a UPROTO_MSCSUBCLASS_ATAPI_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V2()

*/

/* don't add a parameter to this macro, create a _V3... instead */
#define	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V2(			\
		pConfig,						\
		ARG_pLun						\
		)							\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V3(			\
		pConfig,						\
		ARG_pLun,						\
		/* fEnableDataInStuff */ __TMS_FALSE			\
		)

/*

Name:	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V2()

Function:
	Macro: compile time initialization for UPROTO_MSCSUBCLASS_ATAPI_CONFIG.

Definition:
	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V2(
		CONST UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG *	pLun
		);

Description:
	This macro is used when initializing a single UPROTO_MSCSUBCLASS_ATAPI_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V3... instead */
#define	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V2(			\
		ARG_pLun						\
		)							\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V3(			\
		ARG_pLun,						\
		/* fEnableDataInStuff */ __TMS_FALSE			\
		)

/*

Name:	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V1()

Function:
	Macro: runtime initialization code for UPROTO_MSCSUBCLASS_ATAPI_CONFIG()

Definition:
	VOID UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V1(
		UPROTO_MSCSUBCLASS_ATAPI_CONFIG *	pConfig,
		USBPUMP_STORAGE_DEVICE_TYPE	DeviceType,
		BOOL				fRemovable,
		CONST TEXT *			pVendorId,
		CONST TEXT *			pProductId,
		CONST TEXT *			pVersion
		);

Description:
	This macro allows code to initialize a UPROTO_MSCSUBCLASS_ATAPI_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V1()

Notes:
	The following macros are provided for backward compatibility

*/

#define	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V1(			\
		pConfig,						\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)							\
	do	{							\
		__TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_SETUP_V1(	\
			&(pConfig)->Lun0,				\
			ARG_DeviceType,					\
			ARG_fRemovable,					\
			ARG_pVendorId,					\
			ARG_pProductId,					\
			ARG_pVersion					\
			);						\
		(pConfig)->pLun = __TMS_NULL;				\
		(pConfig)->nLun = 1;					\
		(pConfig)->fEnableDataInStuff = __TMS_FALSE;		\
		} while (0)

/*

Name:	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V1()

Function:
	Macro: compile time initialization for UPROTO_MSCSUBCLASS_ATAPI_CONFIG.

Definition:
	UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V1(
		USBPUMP_STORAGE_DEVICE_TYPE	DeviceType,
		BOOL				fRemovable,
		CONST TEXT *			pVendorId,
		CONST TEXT *			pProductId,
		CONST TEXT *			pVersion
		);

Description:
	This macro is used when initializing a single UPROTO_MSCSUBCLASS_ATAPI_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

Notes:
	The following macros are provided for backward compatibility

*/

#define	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V1(			\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)							\
	{								\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(		\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		),							\
	__TMS_NULL,							\
	1,								\
	/* fEnableDataInStuff */ __TMS_FALSE				\
	}

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_SETUP_V1(		\
		pConfig,						\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)	\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_SETUP_V1(		\
		pConfig,						\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)
# define UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(		\
		ARG_DeviceType,						\
		ARG_Removable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)	\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(		\
		ARG_DeviceType,						\
		ARG_Removable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)
# define UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V3(			\
		pConfig,						\
		ARG_pLun,						\
		ARG_fEnableDataInStuff					\
		)	\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V3(			\
		pConfig,						\
		ARG_pLun,						\
		ARG_fEnableDataInStuff					\
		)
# define UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V3(			\
		ARG_pLun,						\
		ARG_fEnableDataInStuff					\
		)	\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V3(			\
		ARG_pLun,						\
		ARG_fEnableDataInStuff					\
		)
# define UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V2(			\
		pConfig,						\
		ARG_pLun						\
		)	\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V2(			\
		pConfig,						\
		ARG_pLun						\
		)
# define UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V2(			\
		ARG_pLun						\
		)	\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V2(			\
		ARG_pLun						\
		)
# define UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V1(			\
		pConfig,						\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)	\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_SETUP_V1(			\
		pConfig,						\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)
# define UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V1(			\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)	\
	__TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V1(			\
		ARG_DeviceType,						\
		ARG_fRemovable,						\
		ARG_pVendorId,						\
		ARG_pProductId,						\
		ARG_pVersion						\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of mscatapicfg.h ****/
#endif /* _MSCATAPICFG_H_ */
