///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     USB mass storage class example
///

// 1: Includes
// ----------------------------------------------------------------------------
#ifndef _MSCAPPLIB_H_
#define _MSCAPPLIB_H_

#include "usbpumpenv.h"
#include "usbpumpobject.h"
#include "usbioctl_storage.h"
#include "mscatapicfg.h"

/****************************************************************************\
|
|   parameters
|
\****************************************************************************/

#define MSCDEMO_RAMDISK_VENDOR_ID       "MCCI"
#define MSCDEMO_RAMDISK_PRODUCT_ID_0    "RAM-Disk-0"
#define MSCDEMO_RAMDISK_PRODUCT_ID_1    "RAM-Disk-1"
#define MSCDEMO_RAMDISK_PRODUCT_ID_2    "RAM-Disk-2"
#define MSCDEMO_RAMDISK_VERSION         "1.00"

#define MSCDEMO_RAMDISK_LBA_SIZE        512 /* bytes per lba */

#ifndef MSCDEMO_RAMDISK_NUM_LBA
#define MSCDEMO_RAMDISK_NUM_LBA         32 * 4096   /* last lba -> 64MB */
#endif

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
__TMS_TYPE_DEF_STRUCT(MSCDEMO_RAMDISK_LUN_CONFIG);
__TMS_TYPE_DEF_STRUCT(MSCDEMO_RAMDISK_CONFIG);
__TMS_TYPE_DEF_STRUCT(MSCDEMO_DEVOBJ);
__TMS_TYPE_DEF_STRUCT(MSCDEMO_DEVOBJ_RAMDISK_LUN);
__TMS_TYPE_DEF_STRUCT(MSCDEMO_DEVOBJ_RAMDISK);
__TMS_TYPE_DEF_STRUCT(MSCDEMO_RAMDISK_REQUEST);

// Conveniently, we can use one primary data structure for handling
// the interface to the lower level.  Because of the way things work, we
// can just record the configuration of the lower layers, and forward things
// to the appropriate application engine:  either an AT interpreter or
// an OBEX engine.
//
// When running in a real OS, this structure will probably be used without
// typecloaking, so we do it simply.

struct __TMS_STRUCTNAME(MSCDEMO_DEVOBJ)
    {
    // For convenience, and so we can recieve IOCTLs, we need
    // an object header.

    __TMS_USBPUMP_OBJECT_HEADER Header;

    // We need someplace to store the "object handle"
    __TMS_PUSBPUMP_OBJECT_HEADER    pIoObject;

    // For allocations, we really need a pDevice and a pPlatform
    __TMS_PUDEVICE          pDevice;
    __TMS_PUPLATFORM        pPlatform;

    };

#define MSCDEMO_DEVOBJ_TAG_RAMDISK  UHIL_MEMTAG('R','d','s','k')

#define MSCDEMO_DEVOBJ_NAME_RAMDISK "ramdisk.osdriver.mcci.com"

//  The RAMDISK structure
struct __TMS_STRUCTNAME(MSCDEMO_RAMDISK_REQUEST)
    {
    __TMS_UCALLBACKCOMPLETION       Event;
    __TMS_BYTES             Lba;
    __TMS_BYTES             LbaCount;
    __TMS_PUINT8                pBuffer;
    };

struct __TMS_STRUCTNAME(MSCDEMO_DEVOBJ_RAMDISK_LUN)
    {
    __TMS_MSCDEMO_DEVOBJ_RAMDISK *      udrdl_pRamdisk;

    __TMS_BYTES             udrdl_iLun;

    // the RAM-disk information
    __TMS_BYTES             udrdl_LbaOffs;
    __TMS_BYTES             udrdl_nLba;

    // RAM-disk request
    __TMS_MSCDEMO_RAMDISK_REQUEST       udrdl_ReadRequest;
    __TMS_MSCDEMO_RAMDISK_REQUEST       udrdl_WriteRequest;

    __TMS_USBPUMP_STORAGE_STATUS        udrdl_LastStatusReported;
    __TMS_BOOL              udrdl_fMediumLoaded;
    __TMS_BOOL              udrdl_fPreventRemoval;
    };

struct __TMS_STRUCTNAME(MSCDEMO_DEVOBJ_RAMDISK)
    {
    __TMS_MSCDEMO_DEVOBJ            udrd_DevObj;

    // the RAM-disk buffer
    __TMS_PUINT8                udrd_pMedia;
    __TMS_BYTES             udrd_nLba;

    // the LUN-array
    __TMS_MSCDEMO_DEVOBJ_RAMDISK_LUN *  udrd_pLun;
    __TMS_BYTES             udrd_nLun;
    };

// Exported variables
extern __TMS_CONST __TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG
            gk_MscDemoI_RamdiskConfigV2[];

extern __TMS_CONST __TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG
            gk_MscDemoI_RamdiskConfig;

extern __TMS_CONST __TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG
            gk_MscDemoI_RamdiskConfig_DataInStuff;

// Exported functions.
__TMS_BEGIN_DECLS

__TMS_BOOL MscDemo_CreateRamdisk(
        __TMS_PUPLATFORM                pPlatform,
        __TMS_PUSBPUMP_OBJECT_HEADER            pFunctionObject,
        __TMS_PMSCDEMO_DEVOBJ_RAMDISK *         ppRamDisk
        );

__TMS_BOOL MscDemo_CreateRamdiskV2(
        __TMS_PUPLATFORM                pPlatform,
        __TMS_PUSBPUMP_OBJECT_HEADER            pFunctionObject,
        __TMS_PMSCDEMO_DEVOBJ_RAMDISK *         ppRamDisk,
        __TMS_CONST __TMS_MSCDEMO_RAMDISK_CONFIG *  pConfigAll
        );

__TMS_END_DECLS

#endif // _MSCAPPLIB_H_
