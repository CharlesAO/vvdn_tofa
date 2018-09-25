///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Initialize the acmdemo(just modem) application - subset of wmcdemo.
///
// 1: Includes
// ----------------------------------------------------------------------------
#include "usbappinit.h"
#include "usbpumpobjectapi.h"
#include "usbpumproot.h"
#include "usbpumpdebug.h"

// the WMC things
#include "protowmc.h"
#include "wmctacfg.h"

// we need these for initialization
#include "wmcdevmgmtcfg.h"
#include "wmcmodemcfg.h"
#include "wmcobexcfg.h"

// and the local declarations
#include "wmcapplib.h"

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
extern VOID AcmDemo_ClientCreate(UPLATFORM *pPlatform);

// 4: Static Local Data
// ----------------------------------------------------------------------------
static USB_DATAPUMP_APPLICATION_INIT_VECTOR_FINISH_FN AcmDemoI_AppInit_VectorFinish;


// This table provides the initialization information for the protocols
// that we might load for this device.
static
CONST USBPUMP_PROTOCOL_INIT_NODE InitNodes[] =
    {
    // probe code for ACM
    USBPUMP_PROTOCOL_INIT_NODE_INIT_V2(                         \
        /* dev class, subclass, proto */ -1, -1, -1,            \
        /* ifc class */ USB_bInterfaceClass_Comm,               \
        /* subclass */ USB_bInterfaceSubClass_CommACM,          \
        /* proto */ -1,                                         \
        /* cfg, ifc, altset */ -1, -1, -1,                      \
        /* speed */ -1,                                         \
        /* uProbeFlags */ USBPUMP_PROTOCOL_INIT_FLAG_AUTO_ADD,  \
        /* probe */ WmcTA_ProtocolProbe,                        \
        /* create */ WmcTA_ProtocolCreate,                      \
        /* pQualifyAddInterface */ NULL,                        \
        /* pAddInterface */ WmcTA_AddDataInterface,             \
        /* optional info */ &gk_WmcSubClass_Modem_TaConfig      \
        )
    };

static
CONST USBPUMP_PROTOCOL_INIT_NODE_VECTOR InitHeader =
    USBPUMP_PROTOCOL_INIT_NODE_VECTOR_INIT_V1(
        /* name of the vector */ InitNodes,
        /* prefunction */ NULL,
        /* postfunction */ NULL
        );


// This module exists solely to provide the UsbPumpApplicationInitHdr,
// which provides the glue between the abstract DataPump, the chip layer,
// the descriptors, and the abstract applicaiton.
#define    INIT_VECTOR_NODE(a__UsbDescriptorRoot)          \
    USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V2(          \
        /* pDescriptorTable */ &a__UsbDescriptorRoot,      \
        /* DebugFlags */                                   \
            UDMASK_ANY | UDMASK_ERRORS,                    \
        /* pAppProbeFunction */ NULL,                      \
        /* pAppInitFunction */                             \
            UsbPump_GenericApplicationInit_Protocols,      \
        /* pAppInfo */ &InitHeader                         \
        )


// This module exists solely to provide the UsbPumpApplicationInitHdr,
// which provides the glue between the abstract datapump, the chip layer,
// the descriptors, and the abstract applicaiton.
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
        /* pFinish */ AcmDemoI_AppInit_VectorFinish
        );


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
static VOID
AcmDemoI_AppInit_VectorFinish(
    UPLATFORM *pPlatform,
    CONST USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *pVecHdr,
    VOID *pAppInitContext,
    UINT nPorts
    )
{
    USBPUMP_UNREFERENCED_PARAMETER(pVecHdr);
    USBPUMP_UNREFERENCED_PARAMETER(pAppInitContext);
    USBPUMP_UNREFERENCED_PARAMETER(nPorts);

    AcmDemo_ClientCreate(pPlatform);
}
