///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Video demo application initialization
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "videodemo.h"

#include "usbappinit.h"
#include "usbprotoinit.h"
#include "usbvideo11.h"
#include "usbpumpdebug.h"
#include "usbpump_dcd_initvector_defines.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern CONST USBRC_ROOTTABLE gk_UsbDescriptorRoot;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static USB_DATAPUMP_APPLICATION_INIT_VECTOR_FINISH_FN VideoDemoI_AppInit_VectorFinish;

extern VOID VideoDemo_ClientCreate(UPLATFORM * pPlatform);

static CONST UPROTO_VIDEO_CONFIG    sk_VideoDemo_ProtoConfig =
        UPROTO_VIDEO_CONFIG_INIT_V1(
                &gk_UsbPumpVideo_InSwitch,
                VIDEODEMO_CONFIG_CONTROLBUFFER_SIZE,
                VIDEODEMO_CONFIG_MAXQUEUED_BUFFER,
                0,    /* no timeout */
                0    /* no timeout */
        );

// This table provides the initialization information for the protocols
// that we might load for this device.
static CONST USBPUMP_PROTOCOL_INIT_NODE InitNodes[] =
{
        USBPUMP_PROTOCOL_INIT_NODE_INIT_V2(                \
                /* dev class, subclass, proto */ -1, -1, -1,        \
                /* ifc class */ USB_bInterfaceClass_Video,        \
                /* subclass */ USB_bInterfaceSubClass_VideoControl,    \
                /* proto */ -1,                        \
                /* cfg, ifc, altset */ -1, -1, -1,            \
                /* speed */ -1,                        \
                /* uProbeFlags */ USBPUMP_PROTOCOL_INIT_FLAG_AUTO_ADD,    \
                /* probe */ UsbPumpVideo_ProtocolProbe,            \
                /* create */ UsbPumpVideo_ProtocolCreate,        \
                /* pQualifyAddInterface */ NULL,            \
                /* pAddInterface */ UsbPumpVideo_ProtocolAddInterface,    \
                /* optional info */ &sk_VideoDemo_ProtoConfig        \
        ),
};

static
CONST USBPUMP_PROTOCOL_INIT_NODE_VECTOR InitHeader =
        USBPUMP_PROTOCOL_INIT_NODE_VECTOR_INIT_V1(
                /* name of the vector */ InitNodes,
                /* prefunction */ NULL,
                /* postfunction */ NULL
        );

static
CONST USB_DATAPUMP_APPLICATION_INIT_VECTOR UsbPumpApplicationInitVector[] =
{
    USBPUMP_DCD_INIT_VECTOR_NODES_V1(
        /* DebugFlags */ UDMASK_ERRORS | UDMASK_PROTO,
        &InitHeader
        )
};

CONST USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR gk_UsbPumpApplicationInitHdr =
        USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR_INIT_V1(
                UsbPumpApplicationInitVector,
                /* pSetup */ NULL,
                /* pFinish */ VideoDemoI_AppInit_VectorFinish
        );

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
static VOID
VideoDemoI_AppInit_VectorFinish(
        UPLATFORM *                    pPlatform,
        CONST USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *pVecHdr,
        VOID *                        pAppInitContext,
        UINT                         nPorts
)
{
    USBPUMP_UNREFERENCED_PARAMETER(pVecHdr);
    USBPUMP_UNREFERENCED_PARAMETER(pAppInitContext);
    USBPUMP_UNREFERENCED_PARAMETER(nPorts);

    VideoDemo_ClientCreate(pPlatform);
}
