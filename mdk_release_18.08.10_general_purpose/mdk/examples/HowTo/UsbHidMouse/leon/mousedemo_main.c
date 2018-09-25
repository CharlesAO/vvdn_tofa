///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Main initializations: there's no code in this module, just tables

// 1: Includes
// ----------------------------------------------------------------------------
#include "mousedefs.h"
#include "usbappinit.h"
#include "usbpumpobjectapi.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define EXTERN_USBRC_ROOTTABLE(NAME)    \
    extern CONST USBRC_ROOTTABLE NAME;

#ifdef DESCRIPTOR_ROOT_0
    EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_0)
#endif
#ifdef DESCRIPTOR_ROOT_1
    EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_1)
#endif
#ifdef DESCRIPTOR_ROOT_2
    EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_2)
#endif
#ifdef DESCRIPTOR_ROOT_3
    EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_3)
#endif
#ifdef DESCRIPTOR_ROOT_4
    EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_4)
#endif
#ifdef DESCRIPTOR_ROOT_5
    EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_5)
#endif
#ifdef DESCRIPTOR_ROOT_6
    EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_6)
#endif
#ifdef DESCRIPTOR_ROOT_7
    EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_7)
#endif
#ifdef DESCRIPTOR_ROOT_8
    EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_8)
#endif
#ifdef DESCRIPTOR_ROOT_9
    EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_9)
#endif
#ifndef    DESCRIPTOR_ROOT_0
# ifndef DESCRIPTOR_ROOT_1
#  ifndef DESCRIPTOR_ROOT_2
#   ifndef DESCRIPTOR_ROOT_3
#    ifndef DESCRIPTOR_ROOT_4
#     ifndef DESCRIPTOR_ROOT_5
#      ifndef DESCRIPTOR_ROOT_6
#       ifndef DESCRIPTOR_ROOT_7
#        ifndef DESCRIPTOR_ROOT_8
#         ifndef DESCRIPTOR_ROOT_9
    EXTERN_USBRC_ROOTTABLE(gk_UsbDescriptorRoot)
#         endif
#        endif
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif
# endif
#endif

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern CONST USBRC_ROOTTABLE gk_UsbDescriptorRoot;


// 4: Static Local Data
// ----------------------------------------------------------------------------
// This module provides the UsbPumpApplicationInitHdr,
// which provides the glue between the abstract datapump, the chip layer,
// the descriptors, and the abstract application.  To avoid forward ref probs,
// things appear in reverse order.


// This table provides the initialization information for the protocols
// that we might load for this device.

static
CONST USBPUMP_PROTOCOL_INIT_NODE InitNodes[] =
    {
    USBPUMP_PROTOCOL_INIT_NODE_INIT_V1(
        /* dev class, subclass, proto */ -1, -1, -1,            \
        /* ifc class */ USB_bInterfaceClass_HID,                \
        /* subclass */ USB_bInterfaceSubClass_HID_Boot,         \
        /* proto */ USB_bInterfaceProtocol_HID_Mouse,           \
        /* cfg, ifc, altset */ -1, -1, -1,                      \
        /* speed */ -1,                                         \
        /* probe */ NULL,                                       \
        /* create */ UsbHid10_ProtocolCreate,                   \
        /* optional info */ &HidFnDemo_BootMouse_ProtoConfig    \
        )
    };

static
CONST USBPUMP_PROTOCOL_INIT_NODE_VECTOR InitHeader =
    USBPUMP_PROTOCOL_INIT_NODE_VECTOR_INIT_V1(
        /* name of the vector */ InitNodes,
        /* prefunction */ NULL,
        /* postfunction */ NULL
        );

/*
|| This module exists solely to provide the UsbPumpApplicationInitHdr,
|| which provides the glue between the abstract datapump, the chip layer,
|| the descriptors, and the abstract applicaiton.
*/
#define    INIT_VECTOR_NODE(a__UsbDescriptorRoot)         \
    USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V2(         \
        /* pDescriptorTable */ &a__UsbDescriptorRoot,     \
        /* DebugFlags */                                  \
            UDMASK_ANY | UDMASK_ERRORS | UDMASK_CHAP9 |   \
            UDMASK_PROTO,                                 \
        /* pAppProbeFunction */ NULL,                     \
        /* pAppInitFunction */                            \
            UsbPump_GenericApplicationInit_Protocols,     \
        /* pAppInfo */ &InitHeader                        \
        )

static
CONST USB_DATAPUMP_APPLICATION_INIT_VECTOR UsbPumpApplicationInitVector[] =
    {
#ifdef DESCRIPTOR_ROOT_0
    INIT_VECTOR_NODE(DESCRIPTOR_ROOT_0),
#endif
#ifdef DESCRIPTOR_ROOT_1
    INIT_VECTOR_NODE(DESCRIPTOR_ROOT_1),
#endif
#ifdef DESCRIPTOR_ROOT_2
    INIT_VECTOR_NODE(DESCRIPTOR_ROOT_2),
#endif
#ifdef DESCRIPTOR_ROOT_3
    INIT_VECTOR_NODE(DESCRIPTOR_ROOT_3),
#endif
#ifdef DESCRIPTOR_ROOT_4
    INIT_VECTOR_NODE(DESCRIPTOR_ROOT_4),
#endif
#ifdef DESCRIPTOR_ROOT_5
    INIT_VECTOR_NODE(DESCRIPTOR_ROOT_5),
#endif
#ifdef DESCRIPTOR_ROOT_6
    INIT_VECTOR_NODE(DESCRIPTOR_ROOT_6),
#endif
#ifdef DESCRIPTOR_ROOT_7
    INIT_VECTOR_NODE(DESCRIPTOR_ROOT_7),
#endif
#ifdef DESCRIPTOR_ROOT_8
    INIT_VECTOR_NODE(DESCRIPTOR_ROOT_8),
#endif
#ifdef DESCRIPTOR_ROOT_9
    INIT_VECTOR_NODE(DESCRIPTOR_ROOT_9),
#endif
#ifndef    DESCRIPTOR_ROOT_0
# ifndef DESCRIPTOR_ROOT_1
#  ifndef DESCRIPTOR_ROOT_2
#   ifndef DESCRIPTOR_ROOT_3
#    ifndef DESCRIPTOR_ROOT_4
#     ifndef DESCRIPTOR_ROOT_5
#      ifndef DESCRIPTOR_ROOT_6
#       ifndef DESCRIPTOR_ROOT_7
#        ifndef DESCRIPTOR_ROOT_8
#         ifndef DESCRIPTOR_ROOT_9
    INIT_VECTOR_NODE(gk_UsbDescriptorRoot)
#         endif
#        endif
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif
# endif
#endif
    };

CONST USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR gk_UsbPumpApplicationInitHdr =
    USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR_INIT_V1(
        UsbPumpApplicationInitVector,
        /* pSetup */ NULL,
        /* pFinish */ NULL
        );
