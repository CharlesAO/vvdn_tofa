/* hostapp_msdclient_create.c Tue Mar 07 2017 10:28:51 chwon */

/*

Module:  hostapp_msdclient_create.c

Function:
  Home for UsbPumpHostApp_MsdClientCreate()

Version:
  V3.19b  Tue Mar 07 2017 10:28:51 chwon  Edit level 3

Copyright notice:
  This file copyright (C) 2017 by

    MCCI Corporation
    3520 Krums Corners Road
    Ithaca, NY  14850

  An unpublished work.  All rights reserved.

  This file is proprietary information, and may not be disclosed or
  copied without the prior permission of MCCI Corporation

  ChaeHee Won, MCCI Corporation February 2017

Revision history:
   3.19b  Fri Feb 10 2017 12:30:56  chwon
  20480: Module created.

   3.19b  Fri Feb 24 2017 16:54:31  chwon
  20480: Move UsbPumpMsdClient_DeviceDeparture to function notification.

   3.19b  Tue Mar 07 2017 10:28:51  chwon
  20480: Add performance test code.

*/

#include "usbpump_hostapp_msdclient.h"
#include "uplatform.h"
#include "uplatformapi.h"
#include "usbpumproot.h"
#include "usbpumpapi.h"
#include "usbpumpdebug.h"
#include "usbpumpobjectapi.h"
#include "usbpumplist.h"
#include "uhilalign.h"
#include "hilbusio.h"

/****************************************************************************\
|
|   Manifest constants & typedefs.
|
| This is strictly for private types and constants which will not
| be exported.
|
\****************************************************************************/

static USBPUMP_CLASSKIT_NOTIFICATION_FN
UsbPumpMsdClient_ClassNotification;

static USBPUMP_CLASSKIT_NOTIFICATION_FN
UsbPumpMsdClient_FunctionNotification;

static void
UsbPumpMsdClient_DeviceArrival(
  USBPUMP_CLASS_MSD_CLIENT *pMsdClient,
  CONST USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL_INFO *pInfo
  );

static void
UsbPumpMsdClient_DeviceDeparture(
  USBPUMP_CLASS_MSD_CLIENT *    pMsdClient,
  USBPUMP_CLASS_MSD_CLIENT_DEVICE * pMsdDevice
  );

static USTAT
UsbPumpMsdClient_OpenSession(
  USBPUMP_CLASS_MSD_CLIENT *  pMsdClient,
  USBPUMP_OBJECT_HEADER *   pMsdClassObject
  );

static USBPUMP_API_OPEN_CB_FN
UsbPumpMsdClient_OpenSessionCallback;

static USBPUMP_CLASSKIT_OPEN_FUNCTION_CB_FN
UsbPumpMsdClient_OpenCallback;

static USBPUMP_API_CLOSE_CB_FN
UsbPumpMsdClient_CloseCallback;

static void
UsbPumpMsdClient_StartRead(
  USBPUMP_CLASS_MSD_CLIENT_DEVICE * pMsdDevice,
  USBPUMP_CLASS_MSD_CLIENT_LUN *    pMsdLun
  );

static USBPUMP_USBDI_CLASS_MSD_REQUEST_DONE_FN
UsbPumpMsdClient_ReadDone;


/* rtems functions -- we can't include rtems header files */
UINT32
rtems_clock_get_ticks_per_second(
  void
  );

static
__TMS_forceinline
UINT32
rtems_clock_get_ticks_since_boot(
  void
  )
  {
  extern UINT32 _Watchdog_Ticks_since_boot;

  return _Watchdog_Ticks_since_boot;
  }


/****************************************************************************\
|
| Read-only data.
|
| If program is to be ROM-able, these must all be tagged read-only
| using the ROM storage class; they may be global.
|
\****************************************************************************/

static
CONST USBPUMP_CLASSKIT_CLASS_OUTCALL
  sk_UsbPumpUsbdiMsdClient_ClassOutCall =
  USBPUMP_CLASSKIT_CLASS_OUTCALL_INIT_V1(
    UsbPumpMsdClient_ClassNotification
    );
static
CONST USBPUMP_CLASSKIT_FUNCTION_OUTCALL
  sk_UsbPumpUsbdiMsdClient_FunctionOutCall =
  USBPUMP_CLASSKIT_FUNCTION_OUTCALL_INIT_V1(
    UsbPumpMsdClient_FunctionNotification
    );


/****************************************************************************\
|
| VARIABLES:
|
| If program is to be ROM-able, these must be initialized
| using the BSS keyword.  (This allows for compilers that require
| every variable to have an initializer.)  Note that only those
| variables owned by this module should be declared here, using the BSS
| keyword; this allows for linkers that dislike multiple declarations
| of objects.
|
\****************************************************************************/


/*

Name: UsbPumpHostApp_MsdClientCreate

Function:
  Sample function demonstrates how to find and use the mass storage class
  driver and open a object session to the mass storage class driver
  object.

Definition:
  USBPUMP_CLASS_MSD_CLIENT *
  UsbPumpHostApp_MsdClientCreate(
    UPLATFORM *   pPlatform,
    UINT32    DebugFlags,
    BYTES   DataBufferSize
    );

Description:
  This function finds the mass storage class drivers and open a class
  session to the mass storage class driver object.

Returns:
  Pointer of USBPUMP_CLASS_MSD_CLIENT if success, otherwise NULL

*/

#define FUNCTION "UsbPumpHostApp_MsdClientCreate"

USBPUMP_CLASS_MSD_CLIENT *
UsbPumpHostApp_MsdClientCreate(
  UPLATFORM *   pPlatform,
  UINT32    DebugFlags,
  BYTES   DataBufferSize
  )
  {
  USBPUMP_OBJECT_ROOT *   pRootObject;
  USBPUMP_OBJECT_HEADER *   pClassObject;
  USBPUMP_CLASS_MSD_CLIENT *  pMsdClient;
  USTAT       Status;

  pRootObject = UsbPumpObject_GetRoot(&pPlatform->upf_Header);

  /*
  || Find the Mass Storage Class Driver object
  */
  pClassObject = UsbPumpObject_EnumerateMatchingNames(
        &pRootObject->Header,
        NULL,
        USBPUMP_USBDI_CLASS_MSD_NAME
        );

  /*
  || If the MSD object is found, open a class session
  */
  if (pClassObject == NULL)
    {
    TTUSB_PLATFORM_PRINTF((
      pPlatform,
      UDMASK_ERRORS,
      "?" FUNCTION ":"
      " Failed to enumerate mass storage class driver\n"
      ));
    return NULL;
    }

  /*
  || Create a sample MSD client object
  */
  pMsdClient = UsbPumpPlatform_MallocZero(pPlatform, sizeof(*pMsdClient));
  if (pMsdClient == NULL)
    {
    TTUSB_PLATFORM_PRINTF((
      pPlatform,
      UDMASK_ANY | UDMASK_ERRORS,
      "?" FUNCTION ":"
      " Memory (%x bytes) allocation failed\n",
      sizeof(*pMsdClient)
      ));
    return NULL;
    }

  UsbPumpObject_Init(
    &pMsdClient->ObjectHeader,
    pPlatform->upf_Header.pClassParent,
    /* Mass Storage Class Driver Sample Client */
    UHIL_MEMTAG('M', 's', 'd', 'C'),
    sizeof(*pMsdClient),
    "sample.msd.client.mcci.com",
    &pPlatform->upf_Header,
    NULL
    );

  pMsdClient->pPlatform = pPlatform;
  pMsdClient->ObjectHeader.ulDebugFlags |= DebugFlags;
  pMsdClient->pClassObject = pClassObject;
  pMsdClient->DataBufferSize = DataBufferSize;

  Status = UsbPumpMsdClient_OpenSession(
      pMsdClient,
      pClassObject
      );

  if (Status != USTAT_OK)
    {
    TTUSB_OBJPRINTF((
      &pMsdClient->ObjectHeader,
      UDMASK_ERRORS,
      "?" FUNCTION ":"
      " Failed to open a class session(%s)\n",
      UsbPumpStatus_Name(Status)
      ));

    UsbPumpObject_DeInit(&pMsdClient->ObjectHeader);
    UsbPumpPlatform_Free(
      pPlatform,
      pMsdClient,
      sizeof(*pMsdClient)
      );
    return NULL;
    }

  return pMsdClient;
  }

#undef FUNCTION

/*

Name: UsbPumpMsdClient_OpenSession

Function:
  Sample function demonstrates how to open a object session to MSD object
  using UsbPumpObject_OpenSession()

Definition:
  USTAT
  UsbPumpMsdClient_OpenSession(
    USBPUMP_CLASS_MSD_CLIENT *  pMsdClient,
    USBPUMP_OBJECT_HEADER *   pMsdClassObject
    );

Description:
  This function open a class session to the Generic Class Driver object.

Returns:
  USTAT_OK if success
  USTAT_NO_MEMORY if allocating open request memory fails

*/

#define FUNCTION "UsbPumpMsdClient_OpenSession"

static USTAT
UsbPumpMsdClient_OpenSession(
  USBPUMP_CLASS_MSD_CLIENT *  pMsdClient,
  USBPUMP_OBJECT_HEADER *   pMsdClassObject
  )
  {
  void *    pOpenRequestMemory;
  CONST RECSIZE Size = UsbPumpObject_SizeOpenSessionRequestMemory(
          pMsdClassObject
          );

  /*
  || Allocate Open Request memory for UsbPumpObject_OpenSession()
  */
  pOpenRequestMemory = UsbPumpPlatform_Malloc(
        pMsdClient->pPlatform,
        Size
        );
  if (pOpenRequestMemory == NULL)
    {
    TTUSB_OBJPRINTF((
      &pMsdClient->ObjectHeader,
      UDMASK_ERRORS,
      "?" FUNCTION ":"
      " Memory (%x bytes) allocation failed\n",
      Size
      ));
    return USTAT_NO_MEMORY;
    }

  UsbPumpObject_OpenSession(
    pMsdClassObject,
    pOpenRequestMemory,
    USBPUMP_API_OPEN_REQUEST_MEMORY_SIZE,
    UsbPumpMsdClient_OpenSessionCallback,
    pMsdClient, /* pCallBackContext */
    &gk_UsbPumpUsbdiClassMsd_Guid,
    NULL,   /* pClientObject -- OPTIONAL */
    &pMsdClient->InCall.GenericCast,
    sizeof(pMsdClient->InCall),
    pMsdClient, /* pClientHandle */
    &sk_UsbPumpUsbdiMsdClient_ClassOutCall.GenericCast,
    sizeof(sk_UsbPumpUsbdiMsdClient_ClassOutCall)
    );

  return USTAT_OK;
  }

#undef FUNCTION

#define FUNCTION "UsbPumpMsdClient_OpenSessionCallback"

static void
UsbPumpMsdClient_OpenSessionCallback(
  void *      pClientContext,
  USBPUMP_SESSION_HANDLE  SessionHandle,
  UINT32      Status,
  void *      pOpenRequestMemory,
  RECSIZE     sizeOpenRequestMemory
  )
  {
  USBPUMP_CLASS_MSD_CLIENT * CONST  pMsdClient = pClientContext;

  if (Status == USBPUMP_CLASSKIT_STATUS_OK)
    {
    TTUSB_OBJPRINTF((
      &pMsdClient->ObjectHeader,
      UDMASK_ANY,
      " " FUNCTION ":"
      " OpenSession STATUS_OK %p\n",
      SessionHandle
      ));
    /*
    || Store the session handle to the MSD object. This will be
    || used to call Class In-Calls such as OpenFunction() and
    || GetNumDevices().
    */
    pMsdClient->SessionHandle = SessionHandle;
    }
  else
    {
    TTUSB_OBJPRINTF((
      &pMsdClient->ObjectHeader,
      UDMASK_ERRORS,
      "?" FUNCTION ":"
      " OpenSession failed(%d)\n",
      Status
      ));
    }

  if (pOpenRequestMemory)
    {
    UsbPumpPlatform_Free(
      pMsdClient->pPlatform,
      pOpenRequestMemory,
      sizeOpenRequestMemory
      );
    }
  }

#undef FUNCTION

/*

Name: UsbPumpMsdClient_ClassNotification

Function:
  Home of Class Out-Call Notification Function

Definition:
  USBPUMP_CLASSKIT_NOTIFICATION_FN
    UsbPumpMsdClient_ClassNotification;

  void
  UsbPumpMsdClient_ClassNotification(
    void *        pClientHandle,
    USBPUMP_CLASSKIT_NOTIFICATION NotificationId,
    CONST void *      pNotification,
    BYTES       nNotification
    );

Description:
  This function is called by Generic Class Driver to notify class
  event notifications to this client. This function should handle
  class event notifications such as DEVICE_ARRIVAL and DEVIDE_DEPARTURE.

Returns:
  No explicit result.

*/

static void
UsbPumpMsdClient_ClassNotification(
  void *        pClientHandle,
  USBPUMP_CLASSKIT_NOTIFICATION NotificationId,
  CONST void *      pNotification,
  BYTES       nNotification
  )
  {
  USBPUMP_CLASS_MSD_CLIENT * CONST  pMsdClient = pClientHandle;

  USBPUMP_UNREFERENCED_PARAMETER(nNotification);

  switch (NotificationId)
    {
  case USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL:
    UsbPumpMsdClient_DeviceArrival(pMsdClient, pNotification);
    break;

  case USBPUMP_CLASSKIT_EVENT_DEVICE_DEPARTURE:
    /* handled by function object */
    break;

  case USBPUMP_CLASSKIT_EVENT_FUNCTION_OPEN:
  case USBPUMP_CLASSKIT_EVENT_FUNCTION_CLOSE:
    break;

  default:
    break;
    }
  }

/*

Name: UsbPumpMsdClient_DeviceArrival

Function:
  Sample function demonstrates how to open a function session to
  a function instance using OpenFunction Class In-Call

Definition:
  void
  UsbPumpMsdClient_DeviceArrival(
    USBPUMP_CLASS_MSD_CLIENT *pMsdClient,
    CONST USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL_INFO *pInfo
    );

Description:
  This function opens a function session to the mass storage class driver
  function instance bound to a USB device.

Returns:
  No explicit result.

*/

#define FUNCTION "UsbPumpMsdClient_DeviceArrival"

static void
UsbPumpMsdClient_DeviceArrival(
  USBPUMP_CLASS_MSD_CLIENT *pMsdClient,
  CONST USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL_INFO *pInfo
  )
  {
  USBPUMP_CLASS_MSD_CLIENT_DEVICE * pMsdDevice;
  USBPUMP_USBDI_FUNCTION * CONST  pFunction = pInfo->pFunction;

  TTUSB_OBJPRINTF((
    &pMsdClient->ObjectHeader,
    UDMASK_ENTRY | UDMASK_ANY,
    "+" FUNCTION ":"
    " pFunction(%p) Speed(%u)\n",
    pFunction,
    pFunction->Function.PortInfo.OperatingSpeed
    ));

  pMsdDevice = UsbPumpPlatform_MallocZero(
      pMsdClient->pPlatform,
      sizeof(*pMsdDevice)
      );
  if (pMsdDevice == NULL)
    {
    TTUSB_OBJPRINTF((
      &pMsdClient->ObjectHeader,
      UDMASK_ERRORS,
      "?UsbPumpMsdClient_ClassNotification:"
      " pMsdDevice allocation failed.\n"
      ));
    return;
    }

  pMsdDevice->pMsdClient = pMsdClient;
  pMsdDevice->pFunction = pFunction;

  USBPUMPLIB_LIST_INSERT_NODE_TAIL(
    USBPUMP_CLASS_MSD_CLIENT_DEVICE,
    &pMsdClient->pMsdDeviceHead,
    pMsdDevice,
    pNext,
    pLast
    );

  (*pMsdClient->InCall.Msd.pOpenFunctionFn)(
    pMsdClient->SessionHandle,
    UsbPumpMsdClient_OpenCallback,
    pMsdDevice,
    pMsdDevice->pFunction,
    &pMsdDevice->InCall.ClassKitCast,
    sizeof(pMsdDevice->InCall),
    pMsdDevice,
    &sk_UsbPumpUsbdiMsdClient_FunctionOutCall,
    sizeof(sk_UsbPumpUsbdiMsdClient_FunctionOutCall)
    );
  }

#undef FUNCTION

#define FUNCTION "UsbPumpMsdClient_OpenCallback"

static void
UsbPumpMsdClient_OpenCallback(
  void *      pCallbackCtx,
  USBPUMP_CLASSKIT_STATUS ErrorCode,
  USBPUMP_SESSION_HANDLE  SessionHandle
  )
  {
  USBPUMP_CLASS_MSD_CLIENT_DEVICE * CONST pMsdDevice = pCallbackCtx;
  USBPUMP_CLASS_MSD_CLIENT_LUN *    pMsdLun;
  USBPUMP_CLASS_MSD_CLIENT * CONST
    pMsdClient = pMsdDevice->pMsdClient;
  UINT  i;

  if (ErrorCode != USBPUMP_CLASSKIT_STATUS_OK)
    {
    TTUSB_OBJPRINTF((
      &pMsdClient->ObjectHeader,
      UDMASK_ERRORS,
      "?" FUNCTION ": ErrorCode(%d)\n",
      ErrorCode
      ));
    return;
    }

  pMsdDevice->SessionHandle = SessionHandle;

  TTUSB_OBJPRINTF((
    &pMsdClient->ObjectHeader,
    UDMASK_FLOW,
    " " FUNCTION ":"
    " SessionHandle:%p\n",
    SessionHandle
    ));

  (*pMsdDevice->InCall.Msd.pGetDeviceInfoFn)(
    pMsdDevice->SessionHandle,
    &pMsdDevice->nLun,
    NULL, /* pSerialNo -- OPTIONAL */
    0 /* nSerialNo */
    );

  pMsdLun = UsbPumpPlatform_MallocZero(
      pMsdClient->pPlatform,
      sizeof(*pMsdLun) * pMsdDevice->nLun
      );
  if (pMsdLun == NULL)
    {
    TTUSB_OBJPRINTF((
      &pMsdClient->ObjectHeader,
      UDMASK_ERRORS,
      "?" FUNCTION ":"
      " %d MsdLun allocation failed.\n",
      pMsdDevice->nLun
      ));
    return;
    }

  pMsdDevice->pLun = pMsdLun;

  for (i = 0; i < pMsdDevice->nLun; ++pMsdLun, ++i)
    {
    pMsdLun->pMsdDevice = pMsdDevice;
    pMsdLun->Lun = i;

    (*pMsdDevice->InCall.Msd.pGetLunInfoFn)(
      pMsdDevice->SessionHandle,
      i,
      &pMsdLun->LunInfo
      );

    TTUSB_OBJPRINTF((
      &pMsdClient->ObjectHeader,
      UDMASK_FLOW,
      " " FUNCTION ":"
      " Lun(%x) Vendor(%s) Product(%s)\n",
      i,
      pMsdLun->LunInfo.VendorId,
      pMsdLun->LunInfo.ProductId
      ));

    (*pMsdDevice->InCall.Msd.pGetMediaInfoFn)(
      pMsdDevice->SessionHandle,
      i,
      &pMsdLun->MediaInfo
      );

    TTUSB_OBJPRINTF((
      &pMsdClient->ObjectHeader,
      UDMASK_FLOW,
      "  SizeOfBlock(%x) TotalBlocks(%x) fReadOnly(%x)\n",
      pMsdLun->MediaInfo.SizeOfBlock,
      pMsdLun->MediaInfo.TotalBlocks,
      pMsdLun->MediaInfo.fWriteProtected
      ));

    pMsdLun->nBuffer = pMsdClient->DataBufferSize;
    pMsdLun->hBuffer = UsbPumpPlatform_AllocateCommonBuffer(
          pMsdClient->pPlatform,
          pMsdLun->nBuffer,
          USBPUMP_ALIGNMENT,
          &pMsdLun->pBuffer
          );
    if (pMsdLun->hBuffer == NULL)
      {
      TTUSB_OBJPRINTF((
        &pMsdClient->ObjectHeader,
        UDMASK_ERRORS,
        "?" FUNCTION ":"
        " fail to allocate common buffer.\n"
        ));
      pMsdLun->pBuffer = UsbPumpPlatform_Malloc(
            pMsdClient->pPlatform,
            pMsdLun->nBuffer
            );
      if (pMsdLun->pBuffer == NULL)
        {
        TTUSB_OBJPRINTF((
          &pMsdClient->ObjectHeader,
          UDMASK_ERRORS,
          "?" FUNCTION ":"
          " MsdLun %d bytes buffer"
          " allocation failed.\n",
          pMsdLun->nBuffer
          ));
        continue;
        }
      }

    if (pMsdLun->MediaInfo.fHasMedia)
      {
      pMsdLun->StartLBA = 0;
      pMsdLun->NumberOfLBA =
          pMsdLun->nBuffer / pMsdLun->MediaInfo.SizeOfBlock;
      UsbPumpMsdClient_StartRead(pMsdDevice, pMsdLun);
      }
    }
  }

#undef FUNCTION

/*

Name: UsbPumpMsdClient_DeviceDeparture

Function:
  Sample function demonstrates how to close the function session that
  the client is currently using by calling CloseFunction Function In-Call

Definition:
  void
  UsbPumpMsdClient_DeviceDeparture(
    USBPUMP_CLASS_MSD_CLIENT *pMsdClient,
    CONST USBPUMP_CLASSKIT_EVENT_DEVICE_DEPARTURE_INFO *pInfo
    );

Description:
  This function closes the function session.

Returns:
  No explicit result.

*/

#define FUNCTION "UsbPumpMsdClient_DeviceDeparture"

static void
UsbPumpMsdClient_DeviceDeparture(
  USBPUMP_CLASS_MSD_CLIENT *pMsdClient,
  USBPUMP_CLASS_MSD_CLIENT_DEVICE *pMsdDevice
  )
  {
  USBPUMP_TRACE_PARAMETER(pMsdClient);

  TTUSB_OBJPRINTF((
    &pMsdClient->ObjectHeader,
    UDMASK_ENTRY | UDMASK_ANY,
    "+" FUNCTION ":"
    " pFunction(%p)\n",
    pMsdDevice->pFunction
    ));

  (*pMsdDevice->InCall.Msd.pCloseFn)(
    pMsdDevice->SessionHandle,
    UsbPumpMsdClient_CloseCallback,
    pMsdDevice
    );
  }

#undef FUNCTION

#define FUNCTION "UsbPumpMsdClient_CloseCallback"

static void
UsbPumpMsdClient_CloseCallback(
  void *      pCallbackCtx,
  USBPUMP_CLASSKIT_STATUS ErrorCode
  )
  {
  USBPUMP_CLASS_MSD_CLIENT_DEVICE * CONST pMsdDevice = pCallbackCtx;
  USBPUMP_CLASS_MSD_CLIENT_LUN *    pMsdLun;
  USBPUMP_CLASS_MSD_CLIENT * CONST
    pMsdClient = pMsdDevice->pMsdClient;

  if (ErrorCode != USBPUMP_CLASSKIT_STATUS_OK)
    {
    TTUSB_OBJPRINTF((
      &pMsdClient->ObjectHeader,
      UDMASK_ERRORS,
      "?" FUNCTION ":"
      " ErrorCode(%d)\n",
      ErrorCode
      ));
    }

  USBPUMPLIB_LIST_REMOVE_NODE(
    USBPUMP_CLASS_MSD_CLIENT_DEVICE,
    &pMsdClient->pMsdDeviceHead,
    pMsdDevice,
    pNext,
    pLast
    );

  if ((pMsdLun = pMsdDevice->pLun) != NULL)
    {
    UINT  i;

    for (i = 0; i < pMsdDevice->nLun; ++pMsdLun, ++i)
      {
      if (pMsdLun->hBuffer != NULL)
        {
        UsbPumpPlatform_FreeCommonBuffer(
          pMsdClient->pPlatform,
          pMsdLun->hBuffer
          );
        }
      else if (pMsdLun->pBuffer != NULL)
        {
        UsbPumpPlatform_Free(
          pMsdClient->pPlatform,
          pMsdLun->pBuffer,
          pMsdLun->nBuffer
          );
        }
      }

    UsbPumpPlatform_Free(
      pMsdClient->pPlatform,
      pMsdDevice->pLun,
      sizeof(*pMsdLun) * pMsdDevice->nLun
      );
    }

  UsbPumpPlatform_Free(
    pMsdClient->pPlatform,
    pMsdDevice,
    sizeof(*pMsdDevice)
    );
  }

#undef FUNCTION

/*

Name: UsbPumpMsdClient_FunctionNotification

Function:
  Home of Class Out-Call Notification Function

Definition:
  USBPUMP_CLASSKIT_NOTIFICATION_FN
    UsbPumpMsdClient_FunctionNotification;

  void
  UsbPumpMsdClient_FunctionNotification(
    void *        pClientHandle,
    USBPUMP_CLASSKIT_NOTIFICATION NotificationId,
    CONST void *      pNotification,
    BYTES       nNotification
    );

Description:
  This function is called by Generic Class Driver to notify class
  event notifications to this client. This function should handle
  class event notifications such as DEVICE_ARRIVAL and DEVIDE_DEPARTURE.

Returns:
  No explicit result.

*/

static void
UsbPumpMsdClient_FunctionNotification(
  void *        pClientHandle,
  USBPUMP_CLASSKIT_NOTIFICATION NotificationId,
  CONST void *      pNotification,
  BYTES       nNotification
  )
  {
  USBPUMP_CLASS_MSD_CLIENT_DEVICE * CONST pMsdDevice = pClientHandle;
  USBPUMP_CLASS_MSD_CLIENT * CONST pMsdClient = pMsdDevice->pMsdClient;
  USBPUMP_CLASS_MSD_CLIENT_LUN *    pMsdLun;

  USBPUMP_UNREFERENCED_PARAMETER(nNotification);

  switch (NotificationId)
    {
  case USBPUMP_CLASSKIT_EVENT_DEVICE_DEPARTURE:
    UsbPumpMsdClient_DeviceDeparture(pMsdClient, pMsdDevice);
    break;

  case USBPUMP_USBDI_MSD_EVENT_MEDIA_CHANGE:
    {
    CONST USBPUMP_USBDI_MSD_EVENT_MEDIA_CHANGE_INFO * CONST
      pEventInfo = pNotification;

    TTUSB_OBJPRINTF((
      &pMsdClient->ObjectHeader,
      UDMASK_ANY,
      " UsbPumpMsdClient_FunctionNotification:"
      " pFunction(%p) media changed. Lun(%x) fHasMedia(%x)\n",
      pEventInfo->pFunction,
      pEventInfo->Lun,
      pEventInfo->fHasMedia
      ));

    pMsdLun = pMsdDevice->pLun + pEventInfo->Lun;
    pMsdLun->MediaInfo.fHasMedia = pEventInfo->fHasMedia;
    pMsdLun->MediaInfo.SizeOfBlock = pEventInfo->SizeOfBlock;
    pMsdLun->MediaInfo.TotalBlocks = pEventInfo->TotalBlocks;

    if (pMsdLun->MediaInfo.fHasMedia)
      {
      pMsdLun->StartLBA = 0;
      pMsdLun->NumberOfLBA =
          pMsdLun->nBuffer / pMsdLun->MediaInfo.SizeOfBlock;
      UsbPumpMsdClient_StartRead(pMsdDevice, pMsdLun);
      }
    }
    break;

  default:
    break;
    }
  }

#define FUNCTION "UsbPumpMsdClient_StartRead"

static void
UsbPumpMsdClient_StartRead(
  USBPUMP_CLASS_MSD_CLIENT_DEVICE * pMsdDevice,
  USBPUMP_CLASS_MSD_CLIENT_LUN *    pMsdLun
  )
  {
  UINT32  StartLBA;
  UINT16  NumberOfLBA;

  NumberOfLBA = (UINT16) pMsdLun->NumberOfLBA;
  if ((StartLBA = pMsdLun->StartLBA) == 0)
    {
    TTUSB_OBJPRINTF((
      &pMsdDevice->pMsdClient->ObjectHeader,
      UDMASK_ANY | UDMASK_FLOW,
      " " FUNCTION ":"
      " Start reading blocks"
      " BufferSize(%u) TicksPerSecond(%u)\n",
      pMsdLun->nBuffer,
      rtems_clock_get_ticks_per_second()
      ));
    TTUSB_FLUSH(pMsdDevice->pMsdClient->pPlatform);

    pMsdLun->ReadBlocks = 0;
    pMsdLun->StartTimeTick =
      rtems_clock_get_ticks_since_boot();
    }

  if ((StartLBA + NumberOfLBA) > pMsdLun->MediaInfo.TotalBlocks)
    {
    pMsdLun->StartLBA = 1;
    if (StartLBA < pMsdLun->MediaInfo.TotalBlocks)
      NumberOfLBA = pMsdLun->MediaInfo.TotalBlocks - StartLBA;
    else
      StartLBA = pMsdLun->StartLBA;
    }
  else
    {
    pMsdLun->StartLBA += NumberOfLBA;
    }

  pMsdLun->ReadBlocks += NumberOfLBA;

  UsbPumpUsbdMsd_PrepareRequest(        \
    &pMsdLun->Request,        \
    USBPUMP_USBDI_CLASS_MSD_REQUEST_READ,   \
    pMsdLun->Lun,         \
    StartLBA,         \
    NumberOfLBA,          \
    5000, /* Timeout: 5 sec */      \
    pMsdLun->pBuffer,       \
    pMsdLun->hBuffer,       \
    NumberOfLBA * pMsdLun->MediaInfo.SizeOfBlock, \
    UsbPumpMsdClient_ReadDone,      \
    pMsdLun           \
    );

  (*pMsdDevice->InCall.Msd.pSubmitRequestFn)(
    pMsdDevice->SessionHandle,
    &pMsdLun->Request
    );
  }

#undef  FUNCTION

#define FUNCTION "UsbPumpMsdClient_ReadDone"

static void
UsbPumpMsdClient_ReadDone(
  USBPUMP_USBDI_FUNCTION *    pFunction,
  USBPUMP_USBDI_CLASS_MSD_REQUEST * pRequest,
  void *          pDoneInfo,
  USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT  Status
  )
  {
  USBPUMP_CLASS_MSD_CLIENT_LUN * CONST  pMsdLun = pDoneInfo;
  USBPUMP_CLASS_MSD_CLIENT_DEVICE * CONST
    pMsdDevice = pMsdLun->pMsdDevice;

  USBPUMP_UNREFERENCED_PARAMETER(pFunction);
  USBPUMP_UNREFERENCED_PARAMETER(pRequest);

  if (Status != USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_OK)
    {
    TTUSB_OBJPRINTF((
      &pMsdDevice->pMsdClient->ObjectHeader,
      UDMASK_ERRORS,
      "?" FUNCTION ":"
      " Status(%#x)\n",
      Status
      ));
    if (Status == USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_NO_DEVICE ||
        Status == USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT_NO_MEDIA)
      {
      TTUSB_OBJPRINTF((
        &pMsdDevice->pMsdClient->ObjectHeader,
        UDMASK_ERRORS,
        "?" FUNCTION ":"
        " No Device or media.\n"
        ));
      return;
      }
    }

  if (pMsdLun->ReadBlocks > (512 * 1024))
    {
    UINT32  TimeTick;
    UINT32  ReadBytes;
    UINT32  ReadBytesPerSecond;

    TimeTick = rtems_clock_get_ticks_since_boot();
    if (TimeTick > pMsdLun->StartTimeTick)
      TimeTick -= pMsdLun->StartTimeTick;
    else
      TimeTick += 0 - pMsdLun->StartTimeTick;

    ReadBytes = pMsdLun->ReadBlocks *
          pMsdLun->MediaInfo.SizeOfBlock;
    ReadBytesPerSecond = ReadBytes / TimeTick *
             rtems_clock_get_ticks_per_second();
    ReadBytesPerSecond /= 1024 * 1024;

    TTUSB_OBJPRINTF((
      &pMsdDevice->pMsdClient->ObjectHeader,
      UDMASK_ANY | UDMASK_FLOW,
      " " FUNCTION ":"
      " ReadBlocks(%u) TimeTicks(%u)"
      " ReadBytes(%u) %u MB/sec\n",
      pMsdLun->ReadBlocks,
      TimeTick,
      ReadBytes,
      ReadBytesPerSecond
      ));
    TTUSB_FLUSH(pMsdDevice->pMsdClient->pPlatform);

    pMsdLun->ReadBlocks = 0;
    pMsdLun->StartTimeTick =
      rtems_clock_get_ticks_since_boot();

    if (++pMsdLun->TestCount > 4)
      {
      TTUSB_OBJPRINTF((
        &pMsdDevice->pMsdClient->ObjectHeader,
        UDMASK_ENTRY | UDMASK_ANY,
        "-" FUNCTION ":"
        " performance test done.\n"
        ));
      return;
      }
    }

  UsbPumpMsdClient_StartRead(pMsdDevice, pMsdLun);
  }

#undef  FUNCTION

/**** end of hostapp_msdclient_create.c ****/
