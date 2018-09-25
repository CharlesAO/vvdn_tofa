///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Application definitions
///
///

#ifndef _MOUSEDEFS_H_        // prevent multiple includes
#define _MOUSEDEFS_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "usbpump.h"

#ifndef _PROTOHID_H_
# include "protohid.h"
#endif

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern    CONST UPROTO_USBHID_CONFIG    HidFnDemo_BootMouse_ProtoConfig;

extern    CONST UPROTO_USBHID_CONFIG    HidFnDemo_Generic_ProtoConfig;

extern    ROM UCHAR    WheelMouseWithWakeupReportDescriptor[];
extern    ROM USHORT    sizeof_WheelMouseWithWakeupReportDescriptor;

extern    ROM UCHAR    WheelMouseReportDescriptor[];
extern    ROM USHORT    sizeof_WheelMouseReportDescriptor;

extern    ROM UCHAR    StandardMouseWithWakeupReportDescriptor[];
extern    ROM USHORT    sizeof_StandardMouseWithWakeupReportDescriptor;

extern    ROM UCHAR    StandardMouseReportDescriptor[];
extern    ROM USHORT    sizeof_StandardMouseReportDescriptor;

extern    ROM UCHAR    BootMouseReportDescriptor[];
extern    ROM USHORT    sizeof_BootMouseReportDescriptor;

#endif // _MOUSEDEFS_H_
