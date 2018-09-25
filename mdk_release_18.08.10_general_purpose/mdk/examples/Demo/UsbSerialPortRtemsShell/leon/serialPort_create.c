///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     The OS/none client code for creating the WMC demonstration
///            bindings.
///
// 1: Includes
// ----------------------------------------------------------------------------
#include "usbpumpapi.h"
#include "wmcapplib.h"
#include "uplatform.h"
#include "usbpumpobjectapi.h"
#include "usbpumpobjectname.h"
#include "usbpumproot.h"
#include "wmcmodem.h"

#include "usbpump_usbseri.h"
#include "serialPort.h"
#include <rtems/libio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
VOID AcmDemo_ClientCreate(UPLATFORM *pPlatform);
// Name:    AcmDemo_ClientCreate()
//
// Function:
//    Generate the leaf functions and bindings for the WMC demo app
//
// Definition:
//     VOID AcmDemo_ClientCreate(
//         UDEVICE *pPlatform
//         );
//
// Description:
//     This module scans the named functions of the datapump, looking
//     for objects named "modem.*.fn.mcci.com", "obex.*.fn.mcci.com",
//     and "devmgmt.*.fn.mcci.com".  For each of these, the module creates
//     device instances and initializes the datastructures.
//
// Returns:
//     No explicit result.
VOID AcmDemo_ClientCreate(UPLATFORM *pPlatform)
{
    USBPUMP_OBJECT_ROOT * CONST pPumpRoot =
        UsbPumpObject_GetRoot(&pPlatform->upf_Header);
    USBPUMP_OBJECT_HEADER *pFunctionObject;
    USBSERI_WMC_OBJECT_MODEM_CONFIG    UsbSeri_WmcModem_Config;

    Usbseri_Initialize(pPlatform);

    USBSERI_WMC_OBJECT_MODEM_CONFIG_SETUP_V2(
        &UsbSeri_WmcModem_Config,
        /* NumCtrlRxQes */     USBSERI_WMCMODEM_CTRL_RX_NUM_UBUFQES,
        /* SizeCtrlRxQeBuffer */ USBSERI_WMCMODEM_QEBUFFER_SIZE,
        /* NumCtrlTxQes */     USBSERI_WMCMODEM_CTRL_TX_NUM_UBUFQES,
        /* SizeCtrlTxQeBuffer */ USBSERI_WMCMODEM_QEBUFFER_SIZE,
        /* NumDataRxQes */     USBSERI_WMCMODEM_DATA_RX_NUM_UBUFQES,
        /* SizeDataRxQeBuffer */ USBSERI_WMCMODEM_QEBUFFER_SIZE,
        /* NumDataTxQes */     USBSERI_WMCMODEM_DATA_TX_NUM_UBUFQES,
        /* SizeDataTxQeBuffer */ USBSERI_WMCMODEM_QEBUFFER_SIZE,
        /* fSupportCtsControl */ TRUE,
        /* fAdjustDataBufferSize */ TRUE,
        /* fSetSpecialRxQeFlag */ FALSE,
        /* fSupportNoDataCopy */ TRUE
        );

    // enumerate the function objects
    pFunctionObject = NULL;
    while ((pFunctionObject = UsbPumpObject_EnumerateMatchingNames(
                    &pPumpRoot->Header,
                    pFunctionObject,
                    USBPUMP_OBJECT_NAME_ENUM_MODEM
                    )) != NULL)
    {
        Usbseri_CreateModem_V2(
            pPlatform,
            pFunctionObject,
            NULL,
            &UsbSeri_WmcModem_Config
            );
    }

    struct stat file_status;

    int fd;
    if (stat("/dev/console", &file_status) == -1)
    {
        // If console is not present we are expecting the first file descriptor
        // return by open to be 0. If this is not the case, cycle through all the
        // descriptors until 0 is reached. This is needed because descriptors are
        // allocated consecutively even if an unused descriptor with a lower value
        // exists
        // TODO find a nicer way to do this
        do {
            fd = open("/dev/usb0", O_RDWR);
            if (fd != 0)
                close(fd);
        } while (fd);
        fd = open("/dev/usb0", O_WRONLY);
        fd = open("/dev/usb0", O_WRONLY);
    }
    else
    {
        // console is present, so we need to redirect
        // stdin, stdout and stderr to the USB device
        fd = open("/dev/usb0", O_RDWR);

        dup2(fd, fileno(stdin));
        dup2(fd, fileno(stdout));
        dup2(fd, fileno(stderr));
    }
}
