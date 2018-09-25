///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Mouse module

// 1: Includes
// ----------------------------------------------------------------------------
#include "mousedefs.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// the reports
#define    HIDMOUSE_MOUSEREPORT_TAG    USB_HID_ReportTypeIdToWValue(USB_HID_ReportType_Input, 0)
#define    HIDMOUSE_MOUSEREPORT_LEN    3

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
__TTUSB_FNTYPE_DEF(HIDMOUSE_MOUSEREPORT, UCHAR, [HIDMOUSE_MOUSEREPORT_LEN]);

// the context
__TTUSB_TYPE_DEF_STRUCT(HIDMOUSE_CONTEXT);

// 4: Static Local Data
// ----------------------------------------------------------------------------
struct __TTUSB_HIDMOUSE_CONTEXT
    {
    UPROTO_USBHID *              umouse_pUsbHid;
    ROM HIDMOUSE_MOUSEREPORT *   umouse_pReport;
    ROM UCHAR *                  umouse_pReportDescriptor;
    USHORT                       umouse_nReportDescriptor;
    USHORT                       umouse_nReports;
    UPROTO_USBHID_OUT_REPORT_QE  umouse_MouseSetReportQE;
    UBUFQE                       umouse_MouseIntReportQE;
    };

static UBUFIODONEFN    hidmouse_MouseIntReportDone;
static UPROTO_USBHID_OUT_REPORT_QE_DONE_FN    hidmouse_MouseReportDone;

// all the functions get declared, even if they're not used
UPROTO_USBHID_CLIENT_METHODS_DECLARE_FNS(hidmouse);

static ROM UPROTO_USBHID_CLIENT_METHODS hidmouse_switch =
    UPROTO_USBHID_CLIENT_METHODS_INIT_V1(
        hidmouse_Attach,
        hidmouse_Detach,
        hidmouse_Configure,
        hidmouse_Unconfigure,
        /* hidmouse_Suspend */ NULL,
        /* hidmouse_Resume */ NULL,
        hidmouse_GetNextReport,
        hidmouse_GetReport,
        /* hidmouse_SetReport */ NULL,
        /* hidmouse_GetIdle */ NULL,
        /* hidmouse_SetIdle */ NULL,
        /* hidmouse_SetProtocol */ NULL,
        hidmouse_GetReportDescriptor,
        /* hidmouse_SetReportDescriptor */ NULL,
        /* hidmouse_GetPhysicalDescriptor */ NULL,
        /* hidmouse_SetPhysicalDescriptor */ NULL,
        /* hidmouse_GetMiscellaneous */ NULL,
        /* hidmouse_SetMiscellaneous */ NULL,
        /* hidmouse_EventResponse -- NULL means use GetNextReport */ NULL
        );


// The array of reports...
ROM HIDMOUSE_MOUSEREPORT hidmouse_Reports[] =
    {
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0xFC },
    { 0x00, 0x00, 0x00 }
    };


CONST UPROTO_USBHID_CONFIG
HidFnDemo_BootMouse_ProtoConfig =
    UPROTO_USBHID_CONFIG_INIT_V1(
        UPROTO_USBHID_DERIVED_NAME("boot.mouse"),
        sizeof(HIDMOUSE_CONTEXT),
        &hidmouse_switch
        );

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
BOOL
hidmouse_Attach(
    VOID *pClientObject,
    PUPROTO_USBHID pHid
    )
    {
    HIDMOUSE_CONTEXT * CONST pMouse = pClientObject;
    VOID *pNonce;

    pMouse->umouse_pUsbHid = pHid;
    pMouse->umouse_pReportDescriptor = BootMouseReportDescriptor;
    pMouse->umouse_nReportDescriptor = sizeof_BootMouseReportDescriptor;

    if (pHid->uhid_usHostDataSize < pMouse->umouse_nReportDescriptor)
        pHid->uhid_usHostDataSize = pMouse->umouse_nReportDescriptor;


    // Set up the Set-Report mechanism for the MOUSE report
    pNonce = UsbAllocateDeviceBuffer(
            pHid->uhid_pDevice,
            HIDMOUSE_MOUSEREPORT_LEN
            );
    if (! pNonce)
        return FALSE;

    UHIL_fill(pNonce, HIDMOUSE_MOUSEREPORT_LEN, 0);
    UPROTO_USBHID_OUT_REPORT_QE_SETUP_V1(
        &pMouse->umouse_MouseSetReportQE,
        pNonce,
        HIDMOUSE_MOUSEREPORT_LEN,
        HIDMOUSE_MOUSEREPORT_TAG,
        hidmouse_MouseReportDone,
        pMouse
        );

    // Set up the Interrupt reporting mechanism for the MOUSE
    // report.
    pMouse->umouse_MouseIntReportQE.uqe_buf =
         pNonce = UsbAllocateDeviceBuffer(
            pHid->uhid_pDevice,
            HIDMOUSE_MOUSEREPORT_LEN
            );

    if (! pNonce)
        return FALSE;

    UHIL_fill(pNonce, HIDMOUSE_MOUSEREPORT_LEN, 0);
    pMouse->umouse_MouseIntReportQE.uqe_bufsize = HIDMOUSE_MOUSEREPORT_LEN;
    pMouse->umouse_MouseIntReportQE.uqe_donefn = hidmouse_MouseIntReportDone;
    pMouse->umouse_MouseIntReportQE.uqe_doneinfo = pMouse;

    return TRUE;
    }

VOID
hidmouse_Detach(
    VOID *pClientObject,
    PUPROTO_USBHID pHid
    )
    {
    HIDMOUSE_CONTEXT * CONST pKey = pClientObject;

    UsbFreeDeviceBuffer(
        pHid->uhid_pDevice,
        pKey->umouse_MouseSetReportQE.pBuffer,
        pKey->umouse_MouseSetReportQE.nBuffer
        );

    UsbFreeDeviceBuffer(
        pHid->uhid_pDevice,
        pKey->umouse_MouseIntReportQE.uqe_buf,
        pKey->umouse_MouseIntReportQE.uqe_bufsize
        );
    }

// Configure:  queue the out report requests.
VOID hidmouse_Configure(
    VOID *pClientObject,
    PUPROTO_USBHID pHid,
    UEVENT why
    )
    {
    PHIDMOUSE_CONTEXT CONST pMouse = pClientObject;

    USBPUMP_UNREFERENCED_PARAMETER(why);

    pMouse->umouse_pReport = &hidmouse_Reports[0];
    pMouse->umouse_nReports = LENOF(hidmouse_Reports);

    (*pHid->uhid_pPublicMethods->QueueForOutReportV2)(
        pHid,
        &pMouse->umouse_MouseSetReportQE
        );
    }

VOID hidmouse_Unconfigure(
    VOID *pClientObject,
    PUPROTO_USBHID pHid,
    UEVENT why
    )
    {
    // nothing to do
    USBPUMP_UNREFERENCED_PARAMETER(pClientObject);
    USBPUMP_UNREFERENCED_PARAMETER(pHid);
    USBPUMP_UNREFERENCED_PARAMETER(why);
    }

// only called when UP, and when host has drained interrupt queue
VOID hidmouse_GetNextReport(
    VOID *pClientObject,
    PUPROTO_USBHID pHid
    )
    {
    PHIDMOUSE_CONTEXT CONST pMouse = pClientObject;

    if (pMouse->umouse_nReports)
        {
        UHIL_cpybuf(
            pMouse->umouse_MouseIntReportQE.uqe_buf,
            pMouse->umouse_pReport,
            sizeof(HIDMOUSE_MOUSEREPORT)
            );

        if (--pMouse->umouse_nReports)
            ++pMouse->umouse_pReport;

        (*pHid->uhid_pPublicMethods->QueueInReport)(
            pHid,
            &pMouse->umouse_MouseIntReportQE
            );
        }
    }

// send the report back to the host, or stall
BOOL hidmouse_GetReport(
    VOID *pClientObject,
    PUPROTO_USBHID pHid,
    PUEVENTSETUP pSetup
    )
    {
    PHIDMOUSE_CONTEXT CONST pMouse = pClientObject;
    CONST UCHAR *pResult;
    USHORT nResult;

    TTUSB_PRINTF((
        pHid->uhid_pDevice,
        UDMASK_FLOW,
        "+hidmouse_GetReport (%p, %p): report 0x%04x\n",
        pHid,
        pSetup,
        pSetup->uec_ucp.uc_wValue
        ));

    switch (pSetup->uec_ucp.uc_wValue)
        {
    case HIDMOUSE_MOUSEREPORT_TAG:
        pResult = (CONST UCHAR *) pMouse->umouse_pReport;
        nResult = HIDMOUSE_MOUSEREPORT_LEN;
        break;

    default:
        pResult = NULL;
        nResult = 0;
        break;
        }

    if (pResult)
        {
        UHIL_cpybuf(pHid->uhid_pHostDataBuf, pResult, nResult);
        UsbDeviceReply(
            pHid->uhid_pDevice,
            pHid->uhid_pHostDataBuf,
            pSetup->uec_ucp.uc_wLength,
            nResult
            );

        TTUSB_PRINTF((
            pHid->uhid_pDevice,
            UDMASK_FLOW,
            "-hidmouse_GetReport:  returning $%x ($%x) bytes\n",
            nResult,
            pSetup->uec_ucp.uc_wLength
            ));

        return TRUE;
        }
    else
        {
        // respond with stall....
        return FALSE;
        }
    }

BOOL hidmouse_GetReportDescriptor(
    VOID *pClientObject,
    PUPROTO_USBHID pHid,
    PUEVENTSETUP pSetup
    )
    {
    PHIDMOUSE_CONTEXT CONST pMouse = pClientObject;

    CONST UCHAR *pResult;
    USHORT nResult;
    USHORT wValue = pSetup->uec_ucp.uc_wValue;

    TTUSB_PRINTF((
        pHid->uhid_pDevice,
        UDMASK_FLOW | UDMASK_CHAP9,
        "+hidmouse_GetReportDescriptor (%p, %p): report 0x%04x\n",
        pHid,
        pSetup,
        wValue
        ));

    switch (wValue)
        {
    case USB_bDescType_HID_REPORT << 8:
        pResult = pMouse->umouse_pReportDescriptor;
        nResult = pMouse->umouse_nReportDescriptor;
        break;

    default:
        pResult = NULL;
        nResult = 0;
        }

    if (pResult)
        {
        nResult = UsbCopyAndReply(
            pHid->uhid_pDevice,
            pHid->uhid_pHostDataBuf,
            pHid->uhid_usHostDataSize,
            pResult,
            nResult,
            pSetup->uec_ucp.uc_wLength
            );

        TTUSB_PRINTF((
            pHid->uhid_pDevice,
            UDMASK_FLOW | UDMASK_CHAP9,
            "-hidmouse_GetReportDescriptor: returning %x bytes\n",
            nResult
            ));
        return TRUE;
        }
    else
        {
        TTUSB_PRINTF((
            pHid->uhid_pDevice,
            UDMASK_FLOW | UDMASK_CHAP9 | UDMASK_ERRORS | UDMASK_ANY,
            "-hidmouse_GetReportDescriptor: returning stall to request for $%04x\n",
            wValue
            ));

        return FALSE;
        }
    }

static
BOOL hidmouse_MouseReportDone(
    UPROTO_USBHID_OUT_REPORT_QE *    pMouseSetReportQE,
    VOID *                pDoneInfo,
    USTAT                Status
    )
    {
    PHIDMOUSE_CONTEXT CONST pMouse = pDoneInfo;
    PUPROTO_USBHID CONST pHid = pMouse->umouse_pUsbHid;
    BOOL fResult;

    if (Status == USTAT_OK)
        {
        TTUSB_OBJPRINTF((
            &pHid->uhid_ObjectHeader,
            UDMASK_ANY,
            " hidmouse_MouseReportDone:  Mouse set report ignored\n"
            ));
        fResult = TRUE;
        }
    else
        {
        TTUSB_OBJPRINTF((
            &pHid->uhid_ObjectHeader,
            UDMASK_ANY,
            " hidmouse_MouseReportDone: failed with %s\n",
            UsbPumpStatus_Name(Status)
            ));
        fResult = FALSE;
        }

    if (Status != USTAT_NOTCFG)
        {
        (*pHid->uhid_pPublicMethods->QueueForOutReportV2)(
            pHid,
            pMouseSetReportQE
            );
        }

    return fResult;
    }

static
VOID hidmouse_MouseIntReportDone(
    PUDEVICE pDevice,
    PUENDPOINT pep,
    PUBUFQE pqe
    )
    {
    // PHIDMOUSE_CONTEXT CONST pMouse = pqe->uqe_doneinfo;
    // PUPROTO_USBHID CONST pHid = pMouse->umouse_pUsbHid;

    // we don't do anything; instead we use GetNextReport
    USBPUMP_UNREFERENCED_PARAMETER(pDevice);
    USBPUMP_UNREFERENCED_PARAMETER(pep);
    USBPUMP_UNREFERENCED_PARAMETER(pqe);
    }
