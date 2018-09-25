///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application initialization
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "usbpumpapi.h"
#include "mscapplib.h"
#include "uplatform.h"
#include "usbpumpobjectapi.h"
#include "usbpumpobjectname.h"
#include "usbpumproot.h"
#include "mscapplibcfg.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
VOID MscDemo_ClientCreate(UPLATFORM *pPlatform);

//Name: MscDemo_ClientCreate()
//
//Function:
//  Generate the leaf functions and bindings for the MSC demo app
//
//Definition:
//  VOID WmcDemo_ClientCreate(
//      UDEVICE *pPlatform
//      );
//
//Description:
//  This module scans the named functions of the datapump, looking
//  for objects named "storage.*.fn.mcci.com".  For each of these,
//  the module creates device instances and initializes the datastructures.
//
//Returns:
//  No explicit result.

VOID
MscDemo_ClientCreate(UPLATFORM *pPlatform)
{
    USBPUMP_OBJECT_ROOT * CONST pPumpRoot =
            UsbPumpObject_GetRoot(&pPlatform->upf_Header);
    USBPUMP_OBJECT_HEADER *pFunctionObject;

    MSCDEMO_DEVOBJ_RAMDISK RamDisk;
    PMSCDEMO_DEVOBJ_RAMDISK pRamDisk = &RamDisk;

    // enumerate the function objects
    pFunctionObject = NULL;

    //   Find all instances of the storage protocol stack and
    //   attach a ramdisk on top of it
    while ((pFunctionObject = UsbPumpObject_EnumerateMatchingNames(
            &pPumpRoot->Header,
            pFunctionObject,
            USBPUMP_OBJECT_NAME_ENUM_STORAGE
    )) != NULL)
    {
        MscDemo_CreateRamdiskV2(
                pPlatform,
                pFunctionObject,
                &pRamDisk,
                &gk_MscDemo_RamdiskAppConfig[1]
        );
    }
}
