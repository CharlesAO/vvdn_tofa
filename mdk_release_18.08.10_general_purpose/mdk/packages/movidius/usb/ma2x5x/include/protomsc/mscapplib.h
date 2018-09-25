/* mscapplib.h	Fri Jan 09 2009 15:53:30 chwon */

/*

Module:  mscapplib.h

Function:
	Header file that puts together the necessary things for this
	application.

Version:
	V1.97k	Fri Jan 09 2009 15:53:30 chwon	Edit level 7

Copyright notice:
	This file copyright (C) 2002-2005, 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Mats Webjörn, MCCI Corporation	Oct 2002

Revision history:
   1.79b  Fri Oct 18 2002 16:00:00  maw
	Module created.

   1.83e  Thu Mar 13 2003 16:20:51  chwon
	Correct type cloaking.

   1.83e  Mon Apr 07 2003 10:33:16  chwon
   	Module created from app/mscdemo/mscdemo_osnone_ramdisk.h.

   1.89b  Wed Sep 15 2004 14:45:23 maw
   	Allowing for compile time configuration of RAM-disk size
   	Added support for dividing one ramdisk into several LUN's

   1.91a  Thu Jan 27 2005 11:34:44  chwon
	Bug 1388: Add MSCDEMO_RAMDISK_REQUEST

   1.93c  Thu Dec 08 2005 22:25:48  chwon
	Added MSCDEMO_RAMDISK_PRODUCT_ID_2

   1.97k  Fri Jan 09 2009 15:53:30  chwon
	Added mass storage configuration with fEnableDataInStuff flag --
	gk_MscDemoI_RamdiskConfig_DataInStuff

*/

#ifndef _MSCAPPLIB_H_		/* prevent multiple includes */
#define _MSCAPPLIB_H_

#include "usbpumpenv.h"
#include "usbpumpobject.h"
#include "usbioctl_storage.h"
#include "mscatapicfg.h"

/****************************************************************************\
|
|	parameters
|
\****************************************************************************/

#define MSCDEMO_RAMDISK_VENDOR_ID		"MCCI"
#define MSCDEMO_RAMDISK_PRODUCT_ID_0		"RAM-Disk-0"
#define MSCDEMO_RAMDISK_PRODUCT_ID_1		"RAM-Disk-1"
#define MSCDEMO_RAMDISK_PRODUCT_ID_2		"RAM-Disk-2"
#define MSCDEMO_RAMDISK_VERSION			"1.00"

#define MSCDEMO_RAMDISK_INQUIRY_INFO_SZ		36
#define MSCDEMO_RAMDISK_LBA_SIZE		512	/* bytes per lba */

#ifndef MSCDEMO_RAMDISK_NUM_LBA
#define MSCDEMO_RAMDISK_NUM_LBA			4096	/* last lba -> 2MB */
#endif

/****************************************************************************\
|
|	Forward type declarations.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(MSCDEMO_RAMDISK_LUN_CONFIG);
__TMS_TYPE_DEF_STRUCT(MSCDEMO_RAMDISK_CONFIG);
__TMS_TYPE_DEF_STRUCT(MSCDEMO_DEVOBJ);
__TMS_TYPE_DEF_STRUCT(MSCDEMO_DEVOBJ_RAMDISK_LUN);
__TMS_TYPE_DEF_STRUCT(MSCDEMO_DEVOBJ_RAMDISK);
__TMS_TYPE_DEF_STRUCT(MSCDEMO_RAMDISK_REQUEST);

/****************************************************************************\
|
|	The main instance data
|
\****************************************************************************/

/*
|| Conveniently, we can use one primary data structure for handling
|| the interface to the lower level.  Because of the way things work, we
|| can just record the configuration of the lower layers, and forward things
|| to the appropriate application engine:  either an AT interpreter or
|| an OBEX engine.
||
|| When running in a real OS, this structure will probably be used without
|| typecloaking, so we do it simply.
*/
struct __TMS_STRUCTNAME(MSCDEMO_DEVOBJ)
	{
	/*
	|| For convenience, and so we can recieve IOCTLs, we need
	|| an object header.
	*/
	__TMS_USBPUMP_OBJECT_HEADER	Header;

	/*
	|| We need someplace to store the "object handle"
	*/
	__TMS_PUSBPUMP_OBJECT_HEADER	pIoObject;

	/*
	|| For allocations, we really need a pDevice and a pPlatform
	*/
	__TMS_PUDEVICE			pDevice;
	__TMS_PUPLATFORM		pPlatform;

	};

#define	MSCDEMO_DEVOBJ_TAG_RAMDISK	UHIL_MEMTAG('R','d','s','k')

#define	MSCDEMO_DEVOBJ_NAME_RAMDISK	"ramdisk.osdriver.mcci.com"


/****************************************************************************\
|
|	The RAMDISK structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME(MSCDEMO_RAMDISK_REQUEST)
	{
	__TMS_UCALLBACKCOMPLETION		Event;
	__TMS_BYTES				Lba;
	__TMS_BYTES				LbaCount;
	__TMS_PUINT8				pBuffer;
	};

struct __TMS_STRUCTNAME(MSCDEMO_DEVOBJ_RAMDISK_LUN)
	{
	__TMS_MSCDEMO_DEVOBJ_RAMDISK *		udrdl_pRamdisk;

	__TMS_BYTES				udrdl_iLun;

	/* the RAM-disk information */
	__TMS_BYTES				udrdl_LbaOffs;
	__TMS_BYTES				udrdl_nLba;

	/* RAM-disk request */
	__TMS_MSCDEMO_RAMDISK_REQUEST		udrdl_ReadRequest;
	__TMS_MSCDEMO_RAMDISK_REQUEST		udrdl_WriteRequest;

	__TMS_USBPUMP_STORAGE_STATUS		udrdl_LastStatusReported;
	__TMS_BOOL				udrdl_fMediumLoaded;
	__TMS_BOOL				udrdl_fPreventRemoval;
	};

struct __TMS_STRUCTNAME(MSCDEMO_DEVOBJ_RAMDISK)
	{
	__TMS_MSCDEMO_DEVOBJ			udrd_DevObj;

	/* the RAM-disk buffer */
	__TMS_PUINT8				udrd_pMedia;
	__TMS_BYTES				udrd_nLba;

	/* the LUN-array */
	__TMS_MSCDEMO_DEVOBJ_RAMDISK_LUN *	udrd_pLun;
	__TMS_BYTES				udrd_nLun;
	};

/****************************************************************************\
|
|	The variables
|
\****************************************************************************/

extern __TMS_CONST __TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG
			gk_MscDemoI_RamdiskConfigV2[];

extern __TMS_CONST __TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG
			gk_MscDemoI_RamdiskConfig;

extern __TMS_CONST __TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG
			gk_MscDemoI_RamdiskConfig_DataInStuff;

/****************************************************************************\
|
|	The functions.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL MscDemo_CreateRamdisk(
		__TMS_PUPLATFORM				pPlatform,
		__TMS_PUSBPUMP_OBJECT_HEADER			pFunctionObject,
		__TMS_PMSCDEMO_DEVOBJ_RAMDISK *			ppRamDisk
		);

__TMS_BOOL MscDemo_CreateRamdiskV2(
		__TMS_PUPLATFORM				pPlatform,
		__TMS_PUSBPUMP_OBJECT_HEADER			pFunctionObject,
		__TMS_PMSCDEMO_DEVOBJ_RAMDISK *			ppRamDisk,
		__TMS_CONST __TMS_MSCDEMO_RAMDISK_CONFIG *	pConfigAll
		);

__TMS_END_DECLS

/**** end of mscapplib.h ****/
#endif /* _MSCAPPLIB_H_ */
