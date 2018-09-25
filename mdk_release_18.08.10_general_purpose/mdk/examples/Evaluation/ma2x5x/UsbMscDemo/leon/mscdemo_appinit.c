///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application initialization
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "usbappinit.h"
#include "usbioctl_storage.h"

// the MSC things
#include "mscatapi.h"

// and the MSC application lib declarations
#include "mscapplib.h"

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
#ifndef DESCRIPTOR_ROOT_0
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
extern VOID MscDemo_ClientCreate(UPLATFORM *pPlatform);

// 4: Static Local Data
// ----------------------------------------------------------------------------
static USB_DATAPUMP_APPLICATION_INIT_VECTOR_FINISH_FN MscDemoI_AppInit_VectorFinish;

// This table provides the initialization information for the protocols
// that we might load for this device.
static CONST USBPUMP_PROTOCOL_INIT_NODE InitNodes[] =
{
        USBPUMP_PROTOCOL_INIT_NODE_INIT_V2(                             \
                /* dev class, subclass, proto */ -1, -1, -1,            \
                /* ifc class */ USB_bInterfaceClass_MassStorage,        \
                /* subclass */ USB_bInterfaceSubClass_MassStorageScsi,  \
                /* proto */ -1,                                         \
                /* cfg, ifc, altset */ -1, -1, -1,                      \
                /* speed */ -1,                                         \
                /* uProbeFlags */ USBPUMP_PROTOCOL_INIT_FLAG_AUTO_ADD,  \
                /* probe */ NULL,                                       \
                /* create */ MscSubClass_Atapi_ProtocolCreate,          \
                /* pQualifyAddInterface */ NULL,                        \
                /* pAddInterface */ NULL,                               \
                /* optional info */ &gk_MscDemoI_RamdiskConfig          \
        )
};

static
CONST USBPUMP_PROTOCOL_INIT_NODE_VECTOR InitHeader =
        USBPUMP_PROTOCOL_INIT_NODE_VECTOR_INIT_V1(
                /* name of the vector */ InitNodes,
                /* prefunction */ NULL,
                /* postfunction */ NULL
        );

#define INIT_VECTOR_NODE(a__UsbDescriptorRoot)                  \
        USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V2(           \
                /* pDescriptorTable */ &a__UsbDescriptorRoot,   \
        /* DebugFlags */                                        \
        UDMASK_ANY | UDMASK_ERRORS | 0,                         \
        /* pAppProbeFunction */ NULL,                           \
        /* pAppInitFunction */                                  \
        UsbPump_GenericApplicationInit_Protocols,               \
        /* pAppInfo */ &InitHeader                              \
        )

// This module exists solely to provide the UsbPumpApplicationInitHdr,
// which provides the glue between the abstract datapump, the chip layer,
// the descriptors, and the abstract application.
//
// For prototype multi-DCD builds, the build system #defines the name
// of each init vector in the compile flags.
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
#ifndef DESCRIPTOR_ROOT_0
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
                /* pFinish */ MscDemoI_AppInit_VectorFinish
        );

// the only code in this module is a stub function
static VOID
MscDemoI_AppInit_VectorFinish(
        UPLATFORM *                 pPlatform,
        CONST USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *pVecHdr,
        VOID *                      pAppInitContext,
        UINT                        nPorts
)
{
    USBPUMP_UNREFERENCED_PARAMETER(pVecHdr);
    USBPUMP_UNREFERENCED_PARAMETER(pAppInitContext);
    USBPUMP_UNREFERENCED_PARAMETER(nPorts);

    MscDemo_ClientCreate(pPlatform);
}
