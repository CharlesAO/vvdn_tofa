///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Video demo application
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "videodemo.h"

#include "usbpumpapi.h"
#include "usbpumpobject.h"
#include "usbpumpobjectapi.h"
#include "usbpumproot.h"
#include "uplatform.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
VIDEODEMO_CONTEXT *    g_pVidemoDemoContext;

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

//Name:    VideoDemo_ClientCreate()
//
//Function:
//    Generate the leaf functions and bindings for the video demo app
//
//Definition:
//    VOID VideoDemo_ClientCreate(
//        UDEVICE * pPlatform
//        );
//
//Description:
//    This module scans the named functions of the datapump, looking
//    for objects named "video.*.fn.mcci.com".  For each of these,
//    the module creates device instances and initializes the datastructures.
//
//Returns:
//    No explicit result.

void VideoDemo_ClientCreate(UPLATFORM * pPlatform)
{
    USBPUMP_OBJECT_ROOT *    CONST pPumpRoot =
            UsbPumpObject_GetRoot(&pPlatform->upf_Header);

    USBPUMP_OBJECT_HEADER *    pFunctionObject = NULL;

    // Find all instances of the video protocol instance and
    // attach a loopback on top of it if it has in & out video stream
    while ((pFunctionObject = UsbPumpObject_EnumerateMatchingNames(
            &pPumpRoot->Header,
            pFunctionObject,
            USBPUMP_OBJECT_NAME_ENUM_VIDEO
    )) != NULL)
    {
        g_pVidemoDemoContext = VideoDemo_UncompressedInit(
                pPlatform,
                pFunctionObject
        );
    }
}
