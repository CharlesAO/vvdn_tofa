///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     USB mass storage class example
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "mscapplibcfg.h"
#include "ubufqeapi.h"
#include "ueventnode.h"
#include "uplatform.h"
#include "uplatformapi.h"
#include "usbpumpapi.h"
#include "usbpumpdebug.h"
#include "usbpumplib.h"
#include "usbpumplist.h"
#include "usbpumpobjectapi.h"
#include "usbioctledgecodes.h"
#include "usbioctl_storage.h"
#include "usbpumpioctlapi.h"
#include "usbmsc10.h"
#include "ustat.h"
#include "ufnapistorage.h"
#include "usbpump_devspeed.h"

#include <stdio.h>
#include <rtems.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define STORAGE_SIZE (64 * 1024 * 1024)
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
UINT8 __attribute__((section(".ddr_direct.bss"))) ddrStorage[STORAGE_SIZE];
static USBPUMP_OBJECT_IOCTL_FN  MscDemoI_Ramdisk_Ioctl;


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// Generic Ioctls handlerss
static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_Activate(
        MSCDEMO_DEVOBJ *, PUSBPUMP_IOCTL_EDGE_ACTIVATE_ARG
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_Deactivate(
        MSCDEMO_DEVOBJ *, CONST USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG *
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_BusEvent(
        MSCDEMO_DEVOBJ *, CONST USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG *
);

/* STORAGE specific handlers */
static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_Read(
        MSCDEMO_DEVOBJ *, USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG *
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_ReadDone(
        MSCDEMO_DEVOBJ *, USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG *
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_Write(
        MSCDEMO_DEVOBJ *, USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG *
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_WriteData(
        MSCDEMO_DEVOBJ *, USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG *
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_GetStatus(
        MSCDEMO_DEVOBJ *, USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG *
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_ResetDevice(
        MSCDEMO_DEVOBJ *, USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG *
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_LoadOrEject(
        MSCDEMO_DEVOBJ *, USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG *
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_PreventRemoval(
        MSCDEMO_DEVOBJ *, USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG *
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_CustomCommand( /* 6099 */
        MSCDEMO_DEVOBJ *, USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG *
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_CustomSendDone(
        MSCDEMO_DEVOBJ *, USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG *
);

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_CustomReceiveDone(
        MSCDEMO_DEVOBJ *, USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG *
);

static UCALLBACKFN  MscDemoI_Ramdisk_ReadComplete;
static UCALLBACKFN  MscDemoI_Ramdisk_WriteComplete;

// 6: Functions Implementation
// ----------------------------------------------------------------------------
// Ramdisk 0 : Partition it into two parts, one for each LUN
static CONST MSCDEMO_RAMDISK_LUN_CONFIG RamdiskApp0_LunConfig[] =
{
        MSCDEMO_RAMDISK_LUN_CONFIG_INIT_V1(
                /* LoLba */     0,
                /* HiLba */     (MSCDEMO_RAMDISK_NUM_LBA/2)-1
        ),

        MSCDEMO_RAMDISK_LUN_CONFIG_INIT_V1(
                /* LoLba */     (MSCDEMO_RAMDISK_NUM_LBA/2),
                /* HiLba */     MSCDEMO_RAMDISK_NUM_LBA-1
        )
};


// Ramdisk 1 : Use all of it for LUN 0
static CONST MSCDEMO_RAMDISK_LUN_CONFIG RamdiskApp1_LunConfig[] =
{
        MSCDEMO_RAMDISK_LUN_CONFIG_INIT_V1(
                /* LoLba */     0,
                /* HiLba */     MSCDEMO_RAMDISK_NUM_LBA-1
        )
};

// Combined config for Ramdisk 1&2
CONST MSCDEMO_RAMDISK_CONFIG gk_MscDemo_RamdiskAppConfig[] =
{
        MSCDEMO_RAMDISK_CONFIG_INIT_V1(
                /* nLba */      MSCDEMO_RAMDISK_NUM_LBA,
                /* pLun */      RamdiskApp0_LunConfig
        ),

        MSCDEMO_RAMDISK_CONFIG_INIT_V1(
                /* nLba */      MSCDEMO_RAMDISK_NUM_LBA,
                /* pLun */      RamdiskApp1_LunConfig
        )
};


// Instance 0 with two LUN's
static CONST UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG MassStorage0_LunConfig[] =
{
        UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(
                /* DeviceType */    USBPUMP_STORAGE_DEVICE_TYPE_DIRECT_ACCESS,
                /* fRemovable */    TRUE,
                /* pVendorId */     MSCDEMO_RAMDISK_VENDOR_ID,
                /* pProductId */    MSCDEMO_RAMDISK_PRODUCT_ID_0,
                /* pVersion */      MSCDEMO_RAMDISK_VERSION
        ),

        UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(
                /* DeviceType */    USBPUMP_STORAGE_DEVICE_TYPE_DIRECT_ACCESS,
                /* fRemovable */    TRUE,
                /* pVendorId */     MSCDEMO_RAMDISK_VENDOR_ID,
                /* pProductId */    MSCDEMO_RAMDISK_PRODUCT_ID_1,
                /* pVersion */      MSCDEMO_RAMDISK_VERSION
        )
};

// Instance 1 with one LUN's
static CONST UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG MassStorage1_LunConfig[] =
{
        UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(
                /* DeviceType */    USBPUMP_STORAGE_DEVICE_TYPE_DIRECT_ACCESS,
                /* fRemovable */    TRUE,
                /* pVendorId */     MSCDEMO_RAMDISK_VENDOR_ID,
                /* pProductId */    MSCDEMO_RAMDISK_PRODUCT_ID_2,
                /* pVersion */      MSCDEMO_RAMDISK_VERSION
        )
};

// All MassStorage configs
CONST UPROTO_MSCSUBCLASS_ATAPI_CONFIG gk_MscDemoI_RamdiskConfigV2[] =
{
        UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V2(
                MassStorage0_LunConfig
        ),

        UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V2(
                MassStorage1_LunConfig
        )
};

// Old configuration of MassStorage protocol
CONST UPROTO_MSCSUBCLASS_ATAPI_CONFIG gk_MscDemoI_RamdiskConfig =
        UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V1(
                /* DeviceType */    USBPUMP_STORAGE_DEVICE_TYPE_DIRECT_ACCESS,
                /* fRemovable */    TRUE,
                /* pVendorId */     MSCDEMO_RAMDISK_VENDOR_ID,
                /* pProductId */    MSCDEMO_RAMDISK_PRODUCT_ID_0,
                /* pVersion */      MSCDEMO_RAMDISK_VERSION
        );

// Configuration of MassStorage protocol with fEnableDataInStuff
static CONST UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG sk_MassStorage_LunConfig[] =
{
        UPROTO_MSCSUBCLASS_ATAPI_LUN_CONFIG_INIT_V1(
                /* DeviceType */    USBPUMP_STORAGE_DEVICE_TYPE_DIRECT_ACCESS,
                /* fRemovable */    TRUE,
                /* pVendorId */     MSCDEMO_RAMDISK_VENDOR_ID,
                /* pProductId */    MSCDEMO_RAMDISK_PRODUCT_ID_0,
                /* pVersion */      MSCDEMO_RAMDISK_VERSION
        )
};

CONST UPROTO_MSCSUBCLASS_ATAPI_CONFIG gk_MscDemoI_RamdiskConfig_DataInStuff =
        UPROTO_MSCSUBCLASS_ATAPI_CONFIG_INIT_V3(
                sk_MassStorage_LunConfig,
                /* fEnableDataInStuff */ TRUE
        );

// FakeMbr - a Master Boot Record that contains enough info to allow Windows
// 2000 to be able to format the ram-disk.  If the ram disk is initialized
// to all 0x00's, Win2k will be unable to format it.  FDISK is required,
// but FDISK cannot be run at a point that the USB stack is loaded.  By
// placing a fake MBR in LBA 0, we fool Win2k into thinking the device
// can be formatted.
static CONST UINT8 MscDemo_FakeMbr[512]=
{
        0x02, 0x10, 0x00, 0x00, 0x48, 0x3A, 0x1F, 0x84,
        0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x2A,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48,
        0x38, 0x6E, 0x00, 0x7C, 0x09, 0x75, 0x13, 0x83,
        0xC5, 0x10, 0xE2, 0xF4, 0xCD, 0x18, 0x8B, 0xF5,
        0x83, 0xC6, 0x10, 0x49, 0x74, 0x19, 0x38, 0x2C,
        0x74, 0xF6, 0xA0, 0xB5, 0x07, 0xB4, 0x07, 0x8B,
        0xF0, 0xAC, 0x3C, 0x00, 0x74, 0xFC, 0xBB, 0x07,
        0x00, 0xB4, 0x0E, 0xCD, 0x10, 0xEB, 0xF2, 0x88,
        0x4E, 0x10, 0xE8, 0x46, 0x00, 0x73, 0x2A, 0xFE,
        0x46, 0x10, 0x80, 0x7E, 0x04, 0x0B, 0x74, 0x0B,
        0x80, 0x7E, 0x04, 0x0C, 0x74, 0x05, 0xA0, 0xB6,
        0x07, 0x75, 0xD2, 0x80, 0x46, 0x02, 0x06, 0x83,
        0x46, 0x08, 0x06, 0x83, 0x56, 0x0A, 0x00, 0xE8,
        0x21, 0x00, 0x73, 0x05, 0xA0, 0xB6, 0x07, 0xEB,
        0xBC, 0x81, 0x3E, 0xFE, 0x7D, 0x55, 0xAA, 0x74,
        0x0B, 0x80, 0x7E, 0x10, 0x00, 0x74, 0xC8, 0xA0,
        0xB7, 0x07, 0xEB, 0xA9, 0x8B, 0xFC, 0x1E, 0x57,
        0x8B, 0xF5, 0xCB, 0xBF, 0x05, 0x00, 0x8A, 0x56,
        0x00, 0xB4, 0x08, 0xCD, 0x13, 0x72, 0x23, 0x8A,
        0xC1, 0x24, 0x3F, 0x98, 0x8A, 0xDE, 0x8A, 0xFC,
        0x43, 0xF7, 0xE3, 0x8B, 0xD1, 0x86, 0xD6, 0xB1,
        0x06, 0xD2, 0xEE, 0x42, 0xF7, 0xE2, 0x39, 0x56,
        0x0A, 0x77, 0x23, 0x72, 0x05, 0x39, 0x46, 0x08,
        0x73, 0x1C, 0xB8, 0x01, 0x02, 0xBB, 0x00, 0x7C,
        0x8B, 0x4E, 0x02, 0x8B, 0x56, 0x00, 0xCD, 0x13,
        0x73, 0x51, 0x4F, 0x74, 0x4E, 0x32, 0xE4, 0x8A,
        0x56, 0x00, 0xCD, 0x13, 0xEB, 0xE4, 0x8A, 0x56,
        0x00, 0x60, 0xBB, 0xAA, 0x55, 0xB4, 0x41, 0xCD,
        0x13, 0x72, 0x36, 0x81, 0xFB, 0x55, 0xAA, 0x75,
        0x30, 0xF6, 0xC1, 0x01, 0x74, 0x2B, 0x61, 0x60,
        0x6A, 0x00, 0x6A, 0x00, 0xFF, 0x76, 0x0A, 0xFF,
        0x76, 0x08, 0x6A, 0x00, 0x68, 0x00, 0x7C, 0x6A,
        0x01, 0x6A, 0x10, 0xB4, 0x42, 0x8B, 0xF4, 0xCD,
        0x13, 0x61, 0x61, 0x73, 0x0E, 0x4F, 0x74, 0x0B,
        0x32, 0xE4, 0x8A, 0x56, 0x00, 0xCD, 0x13, 0xEB,
        0xD6, 0x61, 0xF9, 0xC3, 0x49, 0x6E, 0x76, 0x61,
        0x6C, 0x69, 0x64, 0x20, 0x70, 0x61, 0x72, 0x74,
        0x69, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x74, 0x61,
        0x62, 0x6C, 0x65, 0x00, 0x45, 0x72, 0x72, 0x6F,
        0x72, 0x20, 0x6C, 0x6F, 0x61, 0x64, 0x69, 0x6E,
        0x67, 0x20, 0x6F, 0x70, 0x65, 0x72, 0x61, 0x74,
        0x69, 0x6E, 0x67, 0x20, 0x73, 0x79, 0x73, 0x74,
        0x65, 0x6D, 0x00, 0x4D, 0x69, 0x73, 0x73, 0x69,
        0x6E, 0x67, 0x20, 0x6F, 0x70, 0x65, 0x72, 0x61,
        0x74, 0x69, 0x6E, 0x67, 0x20, 0x73, 0x79, 0x73,
        0x74, 0x65, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x44, 0x63,
        0xDB, 0xF0, 0x4C, 0xF0, 0x00, 0x00, 0x00, 0x01,
        0x01, 0x00, 0x0C, 0xFE, 0xFF, 0xFF, 0x3F, 0x00,
        0x00, 0x00, 0x81, 0x14, 0x2A, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
};


//Name: MscDemo_CreateRamdisk()
//
//Function:
//  Create a ramdisk instance at the OS-leaf level.
//
//Definition:
//  BOOL MscDemo_CreateRamdisk(
//      UPLATFORM *         pPlatform,
//      USBPUMP_OBJECT_HEADER *     pFunctionObject,
//      PMSCDEMO_DEVOBJ_RAMDISK *   ppRamDisk
//      );
//
//Description:
//  Allocate instance data for the specified ramdisk object.
//
//Returns:
//  TRUE for success, FALSE for failure.
//
//Notes:
//  Some awkwardness arises because we need to allocate our
//  instance data before we open the device.
BOOL
MscDemo_CreateRamdisk(
        UPLATFORM *         pPlatform,
        USBPUMP_OBJECT_HEADER *     pFunctionObject,
        PMSCDEMO_DEVOBJ_RAMDISK *   ppRamDisk
)
{
    return MscDemo_CreateRamdiskV2(
            pPlatform,
            pFunctionObject,
            ppRamDisk,
            NULL
    );
}



//Name: MscDemo_CreateRamdiskV2()
//
//Function:
//  Create a ramdisk instance at the OS-leaf level.
//
//Definition:
//  BOOL MscDemo_CreateRamdiskV2(
//      UPLATFORM *         pPlatform,
//      USBPUMP_OBJECT_HEADER *     pFunctionObject,
//      PMSCDEMO_DEVOBJ_RAMDISK *   ppRamDisk,
//      MSCDEMO_RAMDISK_CONFIG *    pConfig
//      );
//
//Description:
//  Allocate instance data for the specified ramdisk object.
//
//Returns:
//  TRUE for success, FALSE for failure.
//
//Notes:
//  Some awkwardness arises because we need to allocate our
//  instance data before we open the device.
BOOL
MscDemo_CreateRamdiskV2(
        UPLATFORM *         pPlatform,
        USBPUMP_OBJECT_HEADER *     pFunctionObject,
        PMSCDEMO_DEVOBJ_RAMDISK *   ppRamDisk,
        CONST MSCDEMO_RAMDISK_CONFIG *  pConfig
)
{
    UDEVICE * CONST pDevice = UsbPumpObject_GetDevice(
            pFunctionObject
    );
    MSCDEMO_DEVOBJ_RAMDISK * CONST pRamDisk =
            UsbPumpPlatform_MallocZero(
                    pPlatform,
                    sizeof(*pRamDisk)
            );
    MSCDEMO_DEVOBJ * CONST      pDevObj  = pRamDisk ? &pRamDisk->udrd_DevObj : NULL;
    BOOL                StatusOK = (pRamDisk != NULL);

    // Default if no config provided
    if (StatusOK)
    {
        if (pConfig)
        {
            pRamDisk->udrd_nLba = pConfig->nLba;
            pRamDisk->udrd_nLun = pConfig->nLun;
        }
        else
        {
            pRamDisk->udrd_nLba = MSCDEMO_RAMDISK_NUM_LBA;
            pRamDisk->udrd_nLun = 1;
        }

        pRamDisk->udrd_pLun = UsbPumpPlatform_MallocZero(
                pPlatform,
                pRamDisk->udrd_nLun*sizeof(*pRamDisk->udrd_pLun)
        );
        StatusOK = (pRamDisk->udrd_pLun != NULL);
    }

    if (StatusOK)
    {
        UsbPumpObject_Init(
                &pDevObj->Header,
                pPlatform->upf_Header.pClassParent,
                MSCDEMO_DEVOBJ_TAG_RAMDISK,
                sizeof(*pRamDisk),
                MSCDEMO_DEVOBJ_NAME_RAMDISK,
                /* IOCTL parent */ NULL,
                /* IOCTL fn */ MscDemoI_Ramdisk_Ioctl
        );

        // fill in a few more things
        pDevObj->pDevice   = pDevice;
        pDevObj->pPlatform = pPlatform;

        // now, try to open the object (storage protocol stack)
        pDevObj->pIoObject = UsbPumpObject_FunctionOpen(
                /* pFunctionObject */ pFunctionObject,
                /* pClientObject */  &pDevObj->Header,
                /* pClientContext */  pRamDisk,
                /* pIoctlResult */    NULL
        );

        if (pDevObj->pIoObject == NULL)
            StatusOK = FALSE;

        if (StatusOK)
        {
            TTUSB_OBJPRINTF((
                    pFunctionObject,
                    UDMASK_ANY,
                    " MscDemo_CreateRamdisk: "
                    "opened %s#%d\n",
                    pFunctionObject->pName,
                    pFunctionObject->InstanceNumber
            ));
        }
        else
        {
            TTUSB_OBJPRINTF((
                    pFunctionObject,
                    UDMASK_ANY,
                    " MscDemo_CreateRamdisk: "
                    "could not open %s#%d (%p)\n",
                    pFunctionObject->pName,
                    pFunctionObject->InstanceNumber,
                    pFunctionObject
            ));
        }

        // The followng major block allocates the UBUFQEs and
        // data buffers for this leaf driver.
        if (StatusOK)
        {
            /* Init our RAMDisk buffer */
            //          pRamDisk->udrd_pMedia = UsbAllocateDeviceBuffer(
            //              pDevice,
            //              pRamDisk->udrd_nLba*MSCDEMO_RAMDISK_LBA_SIZE
            //              );
            pRamDisk->udrd_pMedia = ddrStorage;

            StatusOK = (pRamDisk->udrd_pMedia != NULL);
        }

#if 0
        // This code isn't needed since we supply config parameters
        // to ATAPI thru the application node structure
        if (StatusOK)
        {
            USBPUMP_IOCTL_RESULT IoctlResult;

            IoctlResult = UsbFnApiStorage_SetDeviceProperties(
                    pDevObj->pIoObject,
                    USBPUMP_STORAGE_DEVICE_TYPE_DIRECT_ACCESS,
                    /* fRemovable */ TRUE,
                    /* pVendorId */  MSCDEMO_RAMDISK_VENDOR_ID,
                    /* pProductId */ MSCDEMO_RAMDISK_PRODUCT_ID,
                    /* pVersion */   MSCDEMO_RAMDISK_VERSION
            );

            StatusOK = USBPUMP_IOCTL_SUCCESS(IoctlResult);
        }
#endif


        if (StatusOK)
        {
            MSCDEMO_DEVOBJ_RAMDISK_LUN *    pLun = pRamDisk->udrd_pLun;
            USBPUMP_IOCTL_RESULT        IoctlResult;
            BYTES               i;

            // Initialize each partition of the disk
            for (i=0; i < pRamDisk->udrd_nLun && StatusOK; i++, pLun++)
            {
                pLun->udrdl_pRamdisk      = pRamDisk;
                pLun->udrdl_fMediumLoaded = TRUE;

                pLun->udrdl_iLun = i;
                USBPUMP_CALLBACKCOMPLETION_INIT(
                        &pLun->udrdl_ReadRequest.Event,
                        MscDemoI_Ramdisk_ReadComplete,
                        NULL
                );
                USBPUMP_CALLBACKCOMPLETION_INIT(
                        &pLun->udrdl_WriteRequest.Event,
                        MscDemoI_Ramdisk_WriteComplete,
                        NULL
                );

                if (pConfig)
                {
                    pLun->udrdl_LbaOffs = pConfig->pLun[i].LoLba;
                    pLun->udrdl_nLba    = pConfig->pLun[i].HiLba-pConfig->pLun[i].LoLba+1;
                }
                else
                {
                    pLun->udrdl_LbaOffs = 0;
                    pLun->udrdl_nLba    = MSCDEMO_RAMDISK_NUM_LBA;
                }


                // Copy our fake MBR into the first sector of LUN
                UHIL_cpybuf(
                        pRamDisk->udrd_pMedia+
                        MSCDEMO_RAMDISK_LBA_SIZE*pLun->udrdl_LbaOffs,
                        MscDemo_FakeMbr,
                        MSCDEMO_RAMDISK_LBA_SIZE
                );

                // and inform protocol about medium properties
                IoctlResult = UsbFnApiStorage_SetCurrentMediumV3(
                        pDevObj->pIoObject,
                        pLun->udrdl_iLun,   /* LUN */
                        pLun->udrdl_fMediumLoaded,
                        FALSE,      /* Not write-protected */
                        pLun->udrdl_nLba,   /* nLba */
                        MSCDEMO_RAMDISK_LBA_SIZE
                );

                StatusOK = USBPUMP_IOCTL_SUCCESS(IoctlResult);
            }
        }

        if (StatusOK && ppRamDisk)
        {
            *ppRamDisk = pRamDisk;
        }
    }


    if ((! StatusOK) && pDevObj)
    {
        // Something went wrong above. Close open objects
        if (pDevObj->pIoObject)
        {
            (VOID) UsbPumpObject_FunctionClose(
                    pDevObj->pIoObject,
                    &pDevObj->Header,
                    /* pIoctlResult */ NULL
            );
            pDevObj->pIoObject = NULL;
        }

        // DeInit the devobj
        UsbPumpObject_DeInit(
                &pDevObj->Header
        );

        if (pRamDisk->udrd_pLun)
        {
            UsbPumpPlatform_Free(
                    pPlatform,
                    pRamDisk->udrd_pLun,
                    pRamDisk->udrd_nLun*sizeof(*pRamDisk->udrd_pLun)
            );
        }

        UsbPumpPlatform_Free(
                pPlatform,
                pRamDisk,
                sizeof(*pRamDisk)
        );
    }

    return StatusOK;
}

//Name: MscDemoI_Ramdisk_Ioctl()
//
//Function:
//  Receives upcalls from the UPROTO_MSCSUBCLASS_ATAPI (or other
//  compatible) lower layer, and dispatches them.
//
//Definition:
//  USBPUMP_OBJECT_IOCTL_FN MscDemoI_Ramdisk_Ioctl;
//
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_Ioctl(
//      USBPUMP_OBJECT_HEADER *pDevObjHdr,
//      USBPUMP_IOCTL_CODE Ioctl,
//      CONST VOID *pInParam,
//      VOID *pOutParam
//      );
//
//Description:
//  When a low-level (USB-centric) event occurs, the low-level
//  MSC-subclass USB protocol layer prepares an appropriate IOCTL,
//  which is then directed to the client who has opened the
//  lower-level object.
//
//  The following IOCTLs may be received:
//
//  from "usbioctledgecodes.h":
//
//  USBPUMP_IOCTL_EDGE_ACTIVATE
//  USBPUMP_IOCTL_EDGE_DEACTIVATE
//  USBPUMP_IOCTL_EDGE_BUS_EVENT
//
//  From "usbioctl_storage.h":
//
//  USBPUMP_IOCTL_EDGE_STORAGE_READ
//  USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE
//
//  USBPUMP_IOCTL_EDGE_STORAGE_WRITE
//  USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA
//  USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS
//  USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE
//  USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT
//  USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL
//  USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND
//
//Returns:
//  The normal USBIOCTL result.
//
//Notes:
//  Unlike other IOCTL implementation routines, we know that we
//  actually are not likely to pass the operation upstream.  So we
//  decode the IOCTL before doing the switch, to save time and code.

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_Ioctl(
        USBPUMP_OBJECT_HEADER * pDevObjHdr,
        USBPUMP_IOCTL_CODE  Ioctl,
        CONST VOID *        pInParam,
        VOID *          pOutParam
)
{
    MSCDEMO_DEVOBJ * CONST pDevObj = __TMS_CONTAINER_OF(
            pDevObjHdr,
            MSCDEMO_DEVOBJ,
            Header
    );

    switch (Ioctl)
    {
    // Generic Ioctls here
    case USBPUMP_IOCTL_EDGE_ACTIVATE:
        return MscDemoI_Ramdisk_Activate(
                pDevObj,
                pOutParam
        );

    case USBPUMP_IOCTL_EDGE_DEACTIVATE:
        return MscDemoI_Ramdisk_Deactivate(
                pDevObj,
                pInParam
        );

    case USBPUMP_IOCTL_EDGE_BUS_EVENT:
        return MscDemoI_Ramdisk_BusEvent(
                pDevObj,
                pInParam
        );

        // STORAGE specific Ioctls here
    case USBPUMP_IOCTL_EDGE_STORAGE_READ:
        return MscDemoI_Ramdisk_Read(
                pDevObj,
                pOutParam
        );

    case USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE:
        return MscDemoI_Ramdisk_ReadDone(
                pDevObj,
                pOutParam
        );

    case USBPUMP_IOCTL_EDGE_STORAGE_WRITE:
        return MscDemoI_Ramdisk_Write(
                pDevObj,
                pOutParam
        );

    case USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA:
        return MscDemoI_Ramdisk_WriteData(
                pDevObj,
                pOutParam
        );

    case USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS:
        return MscDemoI_Ramdisk_GetStatus(
                pDevObj,
                pOutParam
        );

    case USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE:
        return MscDemoI_Ramdisk_ResetDevice(
                pDevObj,
                pOutParam
        );

    case USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT:
        return MscDemoI_Ramdisk_LoadOrEject(
                pDevObj,
                pOutParam
        );

    case USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL:
        return MscDemoI_Ramdisk_PreventRemoval(
                pDevObj,
                pOutParam
        );

    case USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND: /* 6099 */
        return MscDemoI_Ramdisk_CustomCommand(
                pDevObj,
                pOutParam
        );

    case USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE:
        return MscDemoI_Ramdisk_CustomSendDone(
                pDevObj,
                pOutParam
        );

    case USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE:
        return MscDemoI_Ramdisk_CustomReceiveDone(
                pDevObj,
                pOutParam
        );

    default:
        TTUSB_OBJPRINTF((
                pDevObj->pIoObject,
                UDMASK_ERRORS,
                " MscDemoI_Ramdisk_Ioctl: "
                "unknown IOCTL code %08x\n",
                Ioctl
        ));

        return USBPUMP_IOCTL_RESULT_NOT_CLAIMED;
    }
}

// Generic Ioctl handlers
static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_Activate(
        MSCDEMO_DEVOBJ *            pDevObj,
        USBPUMP_IOCTL_EDGE_ACTIVATE_ARG *   pOutArg
)
{
    USBPUMP_UNREFERENCED_PARAMETER(pDevObj);
    USBPUMP_UNREFERENCED_PARAMETER(pOutArg);

    MSCDEMO_DEVOBJ_RAMDISK * CONST      pRamDisk = pOutArg->pClientContext;
    TTUSB_OBJPRINTF((
            pOutArg->pObject, UDMASK_ENTRY,
            "+MscDemoI_Ramdisk_Activate: "
            "pDevObj(%p) pObject(%p) pClientContext(%p)\n",
            pDevObj,
            pOutArg->pObject,
            pOutArg->pClientContext
    ));

    // if you have parked buffers with the lower level, you don't need
    // to do anything, apart from notifying the OS client.  Set fReject
    // if you want to reject the activation, but don't count on it
    // actually being rejected on the bus.
    if (USBPUMP_IS_SUPER_SPEED(pDevObj->pDevice->udev_bCurrentSpeed))
    {
        printf("USB 3.0 storage device connected\n");
        printf("The drive should now be visible on the host PC. Please format "
               "and perform the speed benchmark (see the Manual for further details)\n");
    }
    else
    {
        printf("USB 2.0 storage device connected.\n");
        printf("The drive should now be visible on the host PC. Please format "
               "and perform the speed benchmark (see the Manual for further details)\n");
        printf("WARNING!!! This device is able to perform at higher speeds.\n");
        printf("Please connect the board to an USB 3 host to achieve USB 3 performance.\n");

    }
    printf("Storage size: %.2f MB\n",
            pRamDisk->udrd_nLun * pRamDisk->udrd_nLba * MSCDEMO_RAMDISK_LBA_SIZE / (1024 * 1024.0));

    return USBPUMP_IOCTL_RESULT_SUCCESS;
}

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_Deactivate(
        MSCDEMO_DEVOBJ *              pDevObj,
        CONST USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG * pInArg
)
{
    USBPUMP_UNREFERENCED_PARAMETER(pDevObj);
    USBPUMP_UNREFERENCED_PARAMETER(pInArg);

    TTUSB_OBJPRINTF((
            pInArg->pObject, UDMASK_ENTRY,
            "+MscDemoI_Ramdisk_Deactivate: "
            "pDevObj(%p) pObject(%p) pClientContext(%p)\n",
            pDevObj,
            pInArg->pObject,
            pInArg->pClientContext
    ));
    printf("Device disconnected\n\n");

    // if you have any local operations to cancel based on
    // deactivation (e.g., dropping any active calls) you
    // should do so here.
    return USBPUMP_IOCTL_RESULT_SUCCESS;
}

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_BusEvent(
        MSCDEMO_DEVOBJ *            pDevObj,
        CONST USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG *pInArg
)
{
    USBPUMP_UNREFERENCED_PARAMETER(pDevObj);    // In case debug printf isn't used

    TTUSB_OBJPRINTF((                   \
            pInArg->pObject, UDMASK_ENTRY,                  \
            "+MscDemoI_Ramdisk_BusEvent: "                  \
            "pDevObj(%p) pObject(%p) pClientContext(%p) "   \
            "Event %d(%s) pInfo(%p)\n",                     \
            pDevObj,                                        \
            pInArg->pObject,                                \
            pInArg->pClientContext,                         \
            pInArg->EventCode,                              \
            UsbPumpEvent_Name(pInArg->EventCode),           \
            pInArg->pEventSpecificInfo                      \
    ));

    // most likely, you'll want to decode as follows:
    switch (pInArg->EventCode)
    {
    case UEVENT_SUSPEND:
    case UEVENT_L1_SLEEP:
    case UEVENT_FUNCTION_SUSPEND:
        /* remember that we're now suspended */
        break;

    case UEVENT_RESUME:
    case UEVENT_FUNCTION_RESUME:
        /* remember that we're now active */
        break;

    case UEVENT_ATTACH:
        /* we're attached to a USB host */
        break;

    case UEVENT_DETACH:
        /* we're detached from the USB host */
        break;

    case UEVENT_RESET:
        /*
         we saw a bus reset -- normally not much to be done
         here that won't be done by deactivate
         */
        break;

    default:
        break;
    }

    return USBPUMP_IOCTL_RESULT_SUCCESS;
}

// Storage specific Ioctl handlers

//Name: MscDemoI_Ramdisk_Read()
//
//Function:
//  This function is called when the protocol has received
//  a command to read a block of data.
//
//Definition:
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_Read(
//      MSCDEMO_DEVOBJ *            pDevObj,
//      USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG *   pOutArg
//      );
//
//Description:
//  This function is called when the protocol needs to read
//  a block of data from the block device.
//  If the device can begin a block read, it should return
//  a STATUS_NONE code and begin to read the data block.
//
//  When the block is ready for the host, it should call
//  UsbFnApiStorage_QueueRead with the buffer, and a
//  count of how much data was read.
//
//  If an error occurs during a read, the device should call
//  UsbFnApiStorage_QueueRead with a size of 0, then fail
//  any subsequent calls to MscDemoI_Ramdisk_Read with the
//  appropriate code.
//
//Returns:
//  STATUS_NONE if the device can begin to read the data
//  block otherwise, an applicable STATUS_NONE should be returned.

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_Read(
        MSCDEMO_DEVOBJ *            pDevObj,
        USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG *   pOutArg
)
{
    MSCDEMO_DEVOBJ_RAMDISK * CONST      pRamDisk = pOutArg->pClientContext;
    MSCDEMO_DEVOBJ_RAMDISK_LUN * CONST  pLun     = pRamDisk->udrd_pLun+pOutArg->iLun;

    TTUSB_OBJPRINTF((
            &pDevObj->Header, UDMASK_ENTRY,
            "+MscDemoI_Ramdisk_Read: "
            "pDevObj(%p) pObject(%p) pClientContext(%p) Lba(%d) LbaCount(%d)\n",
            pDevObj,
            pOutArg->pObject,
            pOutArg->pClientContext,
            pOutArg->Lba,
            pOutArg->LbaCount
    ));

    TTUSB_OBJPRINTF((&pDevObj->Header, UDMASK_ANY,
            " MscDemoI_Ramdisk_Read: Request to read %d blocks starting at LBA %d on LUN %d\n",
            pOutArg->LbaCount, pOutArg->Lba, pOutArg->iLun
    ));

    pLun->udrdl_ReadRequest.Lba = pOutArg->Lba;
    pLun->udrdl_ReadRequest.LbaCount = pOutArg->LbaCount;
    pLun->udrdl_ReadRequest.pBuffer = NULL; /* we will use RAMDISK buffer */
    UsbPostIfNotBusy(
            pDevObj->pDevice,
            &pLun->udrdl_ReadRequest.Event,
            pLun
    );


    // Return STATUS_NONE if ok, or return an error code if there
    // was a problem and we must abort.  No Completion routine will
    // be called if anything but STATUS_NONE is returned
    pOutArg->Status = USBPUMP_STORAGE_STATUS_NONE;

    pLun->udrdl_LastStatusReported = pOutArg->Status;

    return USBPUMP_IOCTL_RESULT_SUCCESS;
}

static VOID
MscDemoI_Ramdisk_ReadComplete(
        VOID *pContext
)
{
    MSCDEMO_DEVOBJ_RAMDISK_LUN * CONST
    pLun = (MSCDEMO_DEVOBJ_RAMDISK_LUN *)pContext;
    MSCDEMO_DEVOBJ_RAMDISK * CONST
    pRamDisk = pLun->udrdl_pRamdisk;

    UsbMarkCompletionNotBusy(pRamDisk->udrd_DevObj.pDevice,
            &pLun->udrdl_ReadRequest.Event
    );

    (VOID)UsbFnApiStorage_QueueReadV2(
            /* pIoObject */ pRamDisk->udrd_DevObj.pIoObject,
            /* iLun */  pLun->udrdl_iLun,
            /* pBuf */  pRamDisk->udrd_pMedia +
            ((pLun->udrdl_LbaOffs +
                    pLun->udrdl_ReadRequest.Lba) *
                    MSCDEMO_RAMDISK_LBA_SIZE),
                    /* LbaCount */  pLun->udrdl_ReadRequest.LbaCount
    );
}

//Name: MscDemoI_Ramdisk_ReadDone()
//
//Function:
//  Notifies a block device that a block of data has been transmitted
//  to the host.
//
//Definition:
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_ReadDone(
//      MSCDEMO_DEVOBJ *                pDevObj,
//      USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG *  pOutArg
//      );
//
//Description:
//  This function is called after a buffer that had been sent to the host
//  is no longer being used by the protocol or the datapump layers and may
//  be reused.
//
//Returns:
//  None

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_ReadDone(
        MSCDEMO_DEVOBJ *                pDevObj,
        USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG *  pOutArg
)
{
    MSCDEMO_DEVOBJ_RAMDISK * CONST      pRamDisk = pOutArg->pClientContext;
    MSCDEMO_DEVOBJ_RAMDISK_LUN * CONST  pLun     = pRamDisk->udrd_pLun+pOutArg->iLun;

    USBPUMP_UNREFERENCED_PARAMETER(pDevObj);    /* In case debug printf isn't used */

    TTUSB_OBJPRINTF((
            pOutArg->pObject, UDMASK_ENTRY,
            "+MscDemoI_Ramdisk_ReadDone: "
            "pDevObj(%p) pObject(%p) pClientContext(%p)\n",
            pDevObj,
            pOutArg->pObject,
            pOutArg->pClientContext
    ));

    // free up the buffer that was passed down before, or whatever needs
    // to be done after a buffer has been successfully transmitted to
    // the host.
    // Don't automatically send another buffer, tho.  Another Read
    // call will be made if/when the protocol is ready for the device
    // to xmit another block

    pOutArg->Status = USBPUMP_STORAGE_STATUS_NONE;

    pLun->udrdl_LastStatusReported = pOutArg->Status;

    return USBPUMP_IOCTL_RESULT_SUCCESS;
}


//Name: MscDemoI_Ramdisk_Write()
//
//Function:
//  Called by the protocol layer to notify the block device
//  that we are going to be writing an LBA to the device.
//
//
//Definition:
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_Write(
//      MSCDEMO_DEVOBJ *            pDevObj,
//      USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG *  pOutArg
//      );
//
//
//Description:
//  The protocol layer will call this function to notify
//  the block device that it is going to be writing to
//  a logical block on the medium.  The lba and the
//  block count are provided by the parameters.
//
//  It is expected that at some time subsequent to this
//  call that the block device will call the protocol
//  UsbFnApiStorage_QueueWrite function with an lba
//  count, and a device-supplied buffer to let the protocol know
//  what block will be read, how much data the device will
//  need, and a buffer to store the data from the host.
//
//  If the device cannot sink all the data into a buffer
//  or cache in one call, it may specify how many blocks
//  it can receive at a time.  Minimally, we must receive
//  1 block at a time.
//
//
//Returns:
//  STATUS_NONE - If a block write can be initiated.
//
//  Otherwise, an applicable USBPUMP_STORAGE_STATUS as
//  to why a block write cannot begin.

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_Write(
        MSCDEMO_DEVOBJ *            pDevObj,
        USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG *  pOutArg
)
{
    MSCDEMO_DEVOBJ_RAMDISK * CONST      pRamDisk = pOutArg->pClientContext;
    MSCDEMO_DEVOBJ_RAMDISK_LUN * CONST  pLun     = pRamDisk->udrd_pLun+pOutArg->iLun;
    USBPUMP_IOCTL_RESULT            IoctlResult;

    USBPUMP_UNREFERENCED_PARAMETER(pDevObj);    // In case debug printf isn't used

    TTUSB_OBJPRINTF((
            &pDevObj->Header, UDMASK_ENTRY,
            "+MscDemoI_Ramdisk_Write: "
            "pDevObj(%p) pObject(%p) pClientContext(%p)\n",
            pDevObj,
            pOutArg->pObject,
            pOutArg->pClientContext
    ));

    TTUSB_OBJPRINTF((&pDevObj->Header, UDMASK_ANY,
            " MscDemoI_Ramdisk_Write: Request to write %d blocks starting at LBA %d on LUN %d\n",
            pOutArg->LbaCount, pOutArg->Lba, pOutArg->iLun
    ));

    // Do any prep work for beginning a LBA write. When done, call
    // UsbFnApiStorage_QueueWrite to fetch us an LBA (or more) to write.
    // For this example, we're going to do it one LBA at a time.
    // The protocol will keep calling MscDemoI_Ramdisk_Write until
    // the actual command has completed.

    pLun->udrdl_WriteRequest.Lba = pOutArg->Lba;
    pLun->udrdl_WriteRequest.LbaCount = pOutArg->LbaCount;
    pLun->udrdl_WriteRequest.pBuffer =
            pRamDisk->udrd_pMedia +
            ((pLun->udrdl_LbaOffs +
                    pOutArg->Lba) *
                    MSCDEMO_RAMDISK_LBA_SIZE);
    IoctlResult = UsbFnApiStorage_QueueWriteV2(
            /* pIoObject */ pRamDisk->udrd_DevObj.pIoObject,
            /* iLun */  pOutArg->iLun,
            /* pBuf */  pLun->udrdl_WriteRequest.pBuffer,
            /* LbaCount */  pOutArg->LbaCount
    );

    // Return STATUS_NONE if ok, or return an error code if there was a
    // problem and we must abort.  No Completion routine will be called
    // if anything but STATUS_NONE is returned
    if (USBPUMP_IOCTL_SUCCESS(IoctlResult))
        pOutArg->Status = USBPUMP_STORAGE_STATUS_NONE;
    else
        pOutArg->Status = USBPUMP_STORAGE_STATUS_WRITE_ERROR;

    pLun->udrdl_LastStatusReported = pOutArg->Status;

    return IoctlResult;
}


//Name: MscDemoI_Ramdisk_WriteData()
//
//Function:
//  Called when data is received from previous
//  UsbFnApiStorage_QueueWrite function completes,
//  containing requested data.
//
//Definition:
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_WriteData(
//      MSCDEMO_DEVOBJ *                pDevObj,
//      USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG * pOutArg
//      );
//
//Description:
//  This function is called when the block data has arrived in the buffer
//  provided by the block device.  Once the data has been written, either
//  this function, or some asynchronous function must call the
//  UsbFnApiStorage_WriteDone(...) function to let the protocol layer
//  know we have written the data.
//
//Returns:
//  None

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_WriteData(
        MSCDEMO_DEVOBJ *                pDevObj,
        USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG * pOutArg
)
{
    MSCDEMO_DEVOBJ_RAMDISK * CONST      pRamDisk = pOutArg->pClientContext;
    MSCDEMO_DEVOBJ_RAMDISK_LUN * CONST  pLun     = pRamDisk->udrd_pLun+pOutArg->iLun;

    USBPUMP_UNREFERENCED_PARAMETER(pDevObj);    /* In case debug printf isn't used */

    TTUSB_OBJPRINTF((
            pOutArg->pObject, UDMASK_ENTRY,
            "+MscDemoI_Ramdisk_WriteData: "
            "pDevObj(%p) pObject(%p) pClientContext(%p)\n",
            pDevObj,
            pOutArg->pObject,
            pOutArg->pClientContext
    ));

    // for the ram-disk, the buffer is actually the medium
    // so we can call UsbFnApiStorage_WriteDone immediately.
    // Otherwise we would begin a write to the medium here with the
    // data that is in our buffer.
    pLun->udrdl_WriteRequest.LbaCount =
            pOutArg->nBytes / MSCDEMO_RAMDISK_LBA_SIZE;
    pLun->udrdl_WriteRequest.pBuffer = pOutArg->pBuf;
    UsbPostIfNotBusy(pRamDisk->udrd_DevObj.pDevice,
            &pLun->udrdl_WriteRequest.Event,
            pLun
    );

    pOutArg->Status = USBPUMP_STORAGE_STATUS_NONE;

    pLun->udrdl_LastStatusReported = pOutArg->Status;

    return USBPUMP_IOCTL_RESULT_SUCCESS;
}

static VOID
MscDemoI_Ramdisk_WriteComplete(
        VOID *pContext
)
{
    MSCDEMO_DEVOBJ_RAMDISK_LUN * CONST
    pLun = (MSCDEMO_DEVOBJ_RAMDISK_LUN *)pContext;
    MSCDEMO_DEVOBJ_RAMDISK * CONST
    pRamDisk = pLun->udrdl_pRamdisk;

    UsbMarkCompletionNotBusy(pRamDisk->udrd_DevObj.pDevice,
            &pLun->udrdl_WriteRequest.Event
    );

    (VOID)UsbFnApiStorage_WriteDoneV2(
            pRamDisk->udrd_DevObj.pIoObject,
            pLun->udrdl_iLun,
            USBPUMP_STORAGE_STATUS_NONE
    );
}

//Name: MscDemoI_Ramdisk_GetStatus()
//
//Function:
//  This function is called by the protocol to determine
//  the status of a block device.
//
//Definition:
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_GetStatus(
//      MSCDEMO_DEVOBJ *                pDevObj,
//      USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG * pOutArg
//      );
//
//Description:
//  This function is called by the framework when it needs
//  to determine the status of the device.  A device should
//  report that it is ready if and only if it is powered up,
//  has medium, and that the medium is ready to read and write
//  blocks of data.  Otherwise, the device should return
//  an applicable code that is meaningful according to the
//  device type.
//
//Returns:
//  USBPUMP_STORAGE_STATUS - Coderepresenting the status of
//      the device. returns STATUS_NONE if the device is
//      READY, and is capable of read/writing to the
//      medium on the next command, if requested by the host.
//
//Notes:
//  This function is called specifically by a TEST-UNIT-READY
//  SCSI/ATAPI command to determine if an error condition
//  has occurred, NOT in response to a REQUEST_SENSE command.
//  The ATAPI Protocol layer maintains STATUS/ERROR sense codes
//  and responds appropriately, interacting with the device
//  at a low a level as possible.

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_GetStatus(
        MSCDEMO_DEVOBJ *                pDevObj,
        USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG * pOutArg
)
{
    MSCDEMO_DEVOBJ_RAMDISK * CONST      pRamDisk = pOutArg->pClientContext;
    MSCDEMO_DEVOBJ_RAMDISK_LUN * CONST  pLun     = pRamDisk->udrd_pLun+pOutArg->iLun;

    USBPUMP_UNREFERENCED_PARAMETER(pDevObj);    /* In case debug printf isn't used */

    TTUSB_OBJPRINTF((
            pOutArg->pObject, UDMASK_ENTRY,
            "+MscDemoI_Ramdisk_GetStatus: "
            "pDevObj(%p) pObject(%p) pClientContext(%p)\n",
            pDevObj,
            pOutArg->pObject,
            pOutArg->pClientContext
    ));

    if (pLun->udrdl_LastStatusReported == USBPUMP_STORAGE_STATUS_NO_MEDIA)
    {
        USBPUMP_IOCTL_RESULT    IoctlResult;

        // if last we reported no medium, but now medium is present,
        // report medium change
        pLun->udrdl_fMediumLoaded = TRUE;

        // "load" medium
        IoctlResult = UsbFnApiStorage_SetCurrentMediumV3(
                pRamDisk->udrd_DevObj.pIoObject,
                pOutArg->iLun,
                pLun->udrdl_fMediumLoaded,
                FALSE,
                pLun->udrdl_nLba,
                MSCDEMO_RAMDISK_LBA_SIZE
        );

        if (USBPUMP_IOCTL_SUCCESS(IoctlResult))
        {
            pOutArg->Status = USBPUMP_STORAGE_STATUS_MEDIA_CHANGE;
        }
        else
        {
            pOutArg->Status = USBPUMP_STORAGE_STATUS_NO_MEDIA;
        }
    }
    else if (pLun->udrdl_fMediumLoaded == FALSE)
    {
        pOutArg->Status = USBPUMP_STORAGE_STATUS_NO_MEDIA;
    }
    else
    {
        pOutArg->Status = USBPUMP_STORAGE_STATUS_NONE;
    }

    pLun->udrdl_LastStatusReported = pOutArg->Status;

    return USBPUMP_IOCTL_RESULT_SUCCESS;
}


//Name: MscDemoI_Ramdisk_ResetDevice()
//
//Function:
//  This function is called by the mass storage class
//  framework to reset the block device.
//
//Definition:
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_ResetDevice(
//      MSCDEMO_DEVOBJ *                pDevObj,
//      USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG *   pOutArg
//      );
//
//Description:
//  This function is called by the mass storage class
//  framework whenever a USB reset or a device reset
//  command arrives from the host so that we can issue
//  a reset command to the device.  This is usually
//  caused by the device becoming un-responsive during
//  use, to wake the device up initially, or to wake
//  the device up after being suspended.
//
//Returns:
//  USBPUMP_IOCTL_RESULT_SUCCESS

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_ResetDevice(
        MSCDEMO_DEVOBJ *                pDevObj,
        USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG *   pOutArg
)
{
    MSCDEMO_DEVOBJ_RAMDISK * CONST      pRamDisk = pOutArg->pClientContext;
    UINT                    i;

    USBPUMP_UNREFERENCED_PARAMETER(pDevObj);    /* In case debug printf isn't used */

    TTUSB_OBJPRINTF((
            pOutArg->pObject, UDMASK_ENTRY,
            "+MscDemoI_Ramdisk_ResetDevice: "
            "pDevObj(%p) pObject(%p) pClientContext(%p)\n",
            pDevObj,
            pOutArg->pObject,
            pOutArg->pClientContext
    ));

    // note:
    // the atapi protocol already has noted that
    // a BUS RESET was detected, and will respond
    // accordingly to the next REQUEST SENSE issued
    // by the host.  What needs to be done here
    // is any device specific reset, in case it
    // is responsible for the host issuing a reset.

    pOutArg->Status = USBPUMP_STORAGE_STATUS_NONE;

    for (i=0; i < pRamDisk->udrd_nLun ;i++)
        (pRamDisk->udrd_pLun+i)->udrdl_LastStatusReported = pOutArg->Status;

    return USBPUMP_IOCTL_RESULT_SUCCESS;
}

//Name: MscDemoI_Ramdisk_LoadOrEject()
//
//Function:
//  This function is called by the mass storage class
//  framework to eject or load medium in the block device.
//
//Definition:
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_LoadOrEject(
//      MSCDEMO_DEVOBJ *                pDevObj,
//      USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG *  pOutArg
//      );
//
//Description:
//  This function is called by the mass storage class
//  framework whenever a there is a request from the
//  host to load/eject a removable medium device
//
//Returns:
//  USBPUMP_IOCTL_RESULT_SUCCESS

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_LoadOrEject(
        MSCDEMO_DEVOBJ *                pDevObj,
        USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG *  pOutArg
)
{
    MSCDEMO_DEVOBJ_RAMDISK * CONST      pRamDisk = pOutArg->pClientContext;
    MSCDEMO_DEVOBJ_RAMDISK_LUN * CONST  pLun     = pRamDisk->udrd_pLun+pOutArg->iLun;
    USBPUMP_IOCTL_RESULT            IoctlResult;

    USBPUMP_UNREFERENCED_PARAMETER(pDevObj);    // In case debug printf isn't used

    TTUSB_OBJPRINTF((
            pOutArg->pObject, UDMASK_ENTRY,
            "+MscDemoI_Ramdisk_LoadOrEject: "
            "pDevObj(%p) pObject(%p) pClientContext(%p)\n",
            pDevObj,
            pOutArg->pObject,
            pOutArg->pClientContext
    ));

    // we'll toggle back and forth here
    if (pLun->udrdl_fMediumLoaded)
    {
        pLun->udrdl_fMediumLoaded = FALSE;

        // "eject" medium
        IoctlResult = UsbFnApiStorage_SetCurrentMediumV3(
                pRamDisk->udrd_DevObj.pIoObject,
                pOutArg->iLun,
                pLun->udrdl_fMediumLoaded,
                FALSE,
                0,
                0
        );

        pOutArg->Status = USBPUMP_STORAGE_STATUS_NONE;
    }
    else
    {
        pLun->udrdl_fMediumLoaded = TRUE;

        // "load" medium
        IoctlResult = UsbFnApiStorage_SetCurrentMediumV3(
                pRamDisk->udrd_DevObj.pIoObject,
                pOutArg->iLun,
                pLun->udrdl_fMediumLoaded,
                FALSE,                  /* Not write protected */
                pLun->udrdl_nLba,
                MSCDEMO_RAMDISK_LBA_SIZE
        );

        pOutArg->Status = USBPUMP_STORAGE_STATUS_NONE;
    }

    pLun->udrdl_LastStatusReported = pOutArg->Status;

    return IoctlResult;
}

//Name: MscDemoI_Ramdisk_PreventRemoval()
//
//Function:
//  This function is called by the mass storage class
//  framework to prevent or allow ejection of medium
//
//Definition:
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_PreventRemoval(
//      MSCDEMO_DEVOBJ *                pDevObj,
//      USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG *pOutArg
//      );
//
//Description:
//  This function is called by the mass storage class framework whenever
//  a there is a request from the host to prevent or allow medium eject.
//
//  It is usually used by the host during a write to indicate that there
//  are pending directory data that must be written to the medium before it
//  can be removed.
//
//Returns:
//  USBPUMP_IOCTL_RESULT_SUCCESS

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_PreventRemoval(
        MSCDEMO_DEVOBJ *                    pDevObj,
        USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG *    pOutArg
)
{
    MSCDEMO_DEVOBJ_RAMDISK * CONST      pRamDisk = pOutArg->pClientContext;
    MSCDEMO_DEVOBJ_RAMDISK_LUN * CONST  pLun     = pRamDisk->udrd_pLun+pOutArg->iLun;

    USBPUMP_UNREFERENCED_PARAMETER(pDevObj);    /* In case debug printf isn't used */

    TTUSB_OBJPRINTF((
            pOutArg->pObject, UDMASK_ENTRY,
            "+MscDemoI_Ramdisk_PreventRemoval: "
            "pDevObj(%p) pObject(%p) pClientContext(%p) %s removal\n",
            pDevObj,
            pOutArg->pObject,
            pOutArg->pClientContext,
            pOutArg->fPreventRemoval ? "prevent" : "allow"
    ));

    pLun->udrdl_fPreventRemoval = pOutArg->fPreventRemoval;

    pOutArg->Status = USBPUMP_STORAGE_STATUS_NONE;

    pLun->udrdl_LastStatusReported = pOutArg->Status;

    return USBPUMP_IOCTL_RESULT_SUCCESS;
}

//Name: MscDemoI_Ramdisk_CustomCommand()
//
//Function:
//  This function is called by the mass storage class
//  framework to notify a received custom command
//
//Definition:
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_CustomCommand(
//      MSCDEMO_DEVOBJ *                pDevObj,
//      USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG * pOutArg
//      );
//
//Description:
//  This function is called by the mass storage class framework whenever
//  a there is a request from the host to prevent or allow medium eject.
//
//  It is usually used by the host during a write to indicate that there
//  are pending directory data that must be written to the medium before it
//  can be removed.
//
//Returns:
//  USBPUMP_IOCTL_RESULT_SUCCESS

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_CustomCommand(
        MSCDEMO_DEVOBJ *                pDevObj,
        USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG * pOutArg
)
{
    MSCDEMO_DEVOBJ_RAMDISK * CONST  pRamDisk = pOutArg->pClientContext;
    USBPUMP_IOCTL_RESULT        Result;
    BOOL                fDataTransferFromDeviceToHost;
    UINT32              DataTransferLength;

    fDataTransferFromDeviceToHost = pOutArg->fDataTransferFromDeviceToHost;
    DataTransferLength =    pOutArg->DataTransferLength;

    USBPUMP_UNREFERENCED_PARAMETER(pDevObj);

    TTUSB_OBJPRINTF((                                       \
            pOutArg->pObject, UDMASK_ENTRY | UDMASK_PROTO,  \
            "+MscDemoI_Ramdisk_CustomCommand: "             \
            "pDevObj(%p) pObject(%p) pClientContext(%p) "   \
            "Length:%x dir:%x command %x\n",                \
            pDevObj,                                        \
            pOutArg->pObject,                               \
            pOutArg->pClientContext,                        \
            DataTransferLength,                             \
            fDataTransferFromDeviceToHost,                  \
            pOutArg->pCbwcbBuffer[0]                        \
    ));

    Result = USBPUMP_IOCTL_RESULT_SUCCESS;
    pOutArg->fReject = FALSE;

    // This is sample code for testing custom SCSI command without data phase
    if (pOutArg->pCbwcbBuffer[0] == 0xd6 && DataTransferLength == 0)
    {
        Result = UsbFnApiStorage_CustomSendStatus(
                pRamDisk->udrd_DevObj.pIoObject,
                pOutArg->iLun,
                UPROTO_MSCBOT_CSW_STATUS_SUCCESS,
                USBPUMP_STORAGE_STATUS_NONE
        );
    }

    // This is a sample code for testing Data-IN phase
    else if (pOutArg->pCbwcbBuffer[0] == 0xd7 &&
            DataTransferLength > 0 && fDataTransferFromDeviceToHost)
    {
        // Prepare buffer to send data
        UINT8 *pBuf;
        pBuf = UsbAllocateDeviceBuffer(
                pDevObj->pDevice,
                DataTransferLength
        );

        if (!pBuf)
        {
            Result = USBPUMP_IOCTL_RESULT_FAILED;
        }
        else
        {
            Result = UsbFnApiStorage_CustomSendData(
                    pRamDisk->udrd_DevObj.pIoObject,
                    pOutArg->iLun,
                    pBuf,
                    DataTransferLength
            );
        }
    }

    // This is a sample for testing Data-OUT phase
    else if (pOutArg->pCbwcbBuffer[0] == 0xd8 &&
            DataTransferLength > 0 && !fDataTransferFromDeviceToHost)
    {
        // Prepare buffer to receive data
        UINT8 *pBuf;
        pBuf = UsbAllocateDeviceBuffer(
                pDevObj->pDevice,
                DataTransferLength
        );

        if (!pBuf)
        {
            Result = USBPUMP_IOCTL_RESULT_FAILED;
        }
        else
            Result = UsbFnApiStorage_CustomReceiveData(
                    pRamDisk->udrd_DevObj.pIoObject,
                    pOutArg->iLun,
                    pBuf,
                    DataTransferLength
            );
    }

    else
    {
        pOutArg->fReject = TRUE;
        Result = USBPUMP_IOCTL_RESULT_SUCCESS;
    }

    return Result;
}

//Name: MscDemoI_Ramdisk_CustomSendDone()
//
//Function:
//  This function is called when data phase is done already in Custom SCSI
//  command.
//
//Definition:
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_CustomSendDone(
//      MSCDEMO_DEVOBJ *                pDevObj,
//      USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG *pOutArg
//      );
//
//Description:
//  This function is called after data is sent to host during Data-IN phase
//  in Custom SCSI command.
//
//  It is usually used by the client to finish the SCSI command, i.e., to
//  send status phase to the host by calling UsbFnApiStorage_CustomSendStatus.
//
//Returns:
//  USBPUMP_IOCTL_RESULT_SUCCESS

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_CustomSendDone(
        MSCDEMO_DEVOBJ *                pDevObj,
        USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG *pOutArg
)
{
    MSCDEMO_DEVOBJ_RAMDISK * CONST  pRamDisk = pOutArg->pClientContext;
    USBPUMP_IOCTL_RESULT        Result;

    // Free the transfer buffer
    UsbFreeDeviceBuffer(
            pDevObj->pDevice,
            pOutArg->pBuf,
            pOutArg->nBuf
    );

    Result = UsbFnApiStorage_CustomSendStatus(
            pRamDisk->udrd_DevObj.pIoObject,
            pOutArg->iLun,
            UPROTO_MSCBOT_CSW_STATUS_SUCCESS,
            USBPUMP_STORAGE_STATUS_NONE
    );

    pOutArg->fReject = FALSE;

    return Result;
}

//Name: MscDemoI_Ramdisk_CustomReceiveDone()
//
//Function:
//  This function is called when command data is received by MSC.
//
//Definition:
//  USBPUMP_IOCTL_RESULT MscDemoI_Ramdisk_CustomReceiveDone(
//      MSCDEMO_DEVOBJ *                pDevObj,
//      USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG *    pOutArg
//      );
//
//Description:
//  This function is called by the Client when command data is received by
//  MSC in Data-OUT of Custom SCSI Command procedure. Client will call
//  UsbFnApiStorage_CustomSendStatus to finish the status phase.
//
//Returns:
//  USBPUMP_IOCTL_RESULT_SUCCESS

static USBPUMP_IOCTL_RESULT
MscDemoI_Ramdisk_CustomReceiveDone(
        MSCDEMO_DEVOBJ *                    pDevObj,
        USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG *    pOutArg
)
{
    MSCDEMO_DEVOBJ_RAMDISK * CONST  pRamDisk = pOutArg->pClientContext;
    USBPUMP_IOCTL_RESULT        Result;

    // Free the transfer buffer
    UsbFreeDeviceBuffer(
            pDevObj->pDevice,
            pOutArg->pBuf,
            pOutArg->nBuf
    );

    Result = UsbFnApiStorage_CustomSendStatus(
            pRamDisk->udrd_DevObj.pIoObject,
            pOutArg->iLun,
            UPROTO_MSCBOT_CSW_STATUS_SUCCESS,
            USBPUMP_STORAGE_STATUS_NONE
    );

    pOutArg->fReject = FALSE;

    return Result;
}
