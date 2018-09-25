/* hostapp_gcdclient_create.c Tue Apr 11 2017 10:14:07 chwon */

/*

Module:  hostapp_gcdclient_create.c

Function:
  Home for Generic class driver(GCD) client initialization

Version:
  V3.21a  Tue Apr 11 2017 10:14:07 chwon  Edit level 2

Copyright notice:
  This file copyright (C) 2017 by

    MCCI Corporation
    3520 Krums Corners Road
    Ithaca, NY  14850

  An unpublished work.  All rights reserved.

  This file is proprietary information, and may not be disclosed or
  copied without the prior permission of MCCI Corporation

  ChaeHee Won, MCCI Corporation March 2017

Revision history:
   3.19b  Mon Mar 20 2017 11:00:10  chwon
  20480: Module created from host/app/applib/common/gcddemo_create.c.

   3.21a  Tue Apr 11 2017 10:14:07  chwon
  20480: Add performance measure support.

*/

#include "usbpump_hostapp_gcdclient.h"
#include "usbpump_devspeed.h"
#include "usbdesc_types.h"
#include "usbdesc.h"
#include "basehil.h"
#include "usbpumpapi.h"

/****************************************************************************\
|
| Manifest constants & typedefs.
|
| This is strictly for private types and constants which will not
| be exported.
|
\****************************************************************************/

/*
|| #define GCD_STREAM_TEST
*/
#ifndef USBPUMP_CLASS_GCD_CLIENT_SUSPEND_TIMEOUT  /* PARAM */
# define USBPUMP_CLASS_GCD_CLIENT_SUSPEND_TIMEOUT 5000
#endif

#define USBPUMP_CLASS_GCD_CLIENT_RESET_PIPE_FLAGS \
  (USBPUMP_URB_RESET_PIPE_FLAG_DO_CLEAR_FEATURE | \
   USBPUMP_URB_RESET_PIPE_FLAG_CLEAR_HOST_TOGGLE)

#define GCD_CLIENT_MAX_INT_COUNT  30


/****************************************************************************\
|
| Sample Functions to show how to use GCD API
|
\****************************************************************************/

static USTAT
UsbPumpGcdClient_OpenSession(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  USBPUMP_OBJECT_HEADER *     pGcdClassObjectHeader
  );

static void
UsbPumpGcdClient_OpenFunction(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  void *          pClientHandled
  );

static void
UsbPumpGcdClient_CloseFunction(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient
  );

static void
UsbPumpGcdClient_GetNumDevs(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient
  );

static void
UsbPumpGcdClient_GetBoundDevs(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  UINT32          ulNumOfInstances
  );

static void
UsbPumpGcdClient_GetDevState(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  USBPUMP_USBDI_GENERIC_DEVICE_STATE *  pDeviceState
  );

static void
UsbPumpGcdClient_GetDevDesc(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  void *          pBuffer,
  BYTES         sizeBuffer
  );

static void
UsbPumpGcdClient_GetConfigDesc(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  void *          pBuffer,
  BYTES         sizeBuffer,
  UINT8         iConfig
  );

static void
UsbPumpGcdClient_GetConfigTree(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  void *          pBuffer,
  UINT32          sizeBuffer
  );

static void
UsbPumpGcdClient_WriteCtrl(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE   hPipe,
  UINT8           bmRequestType,
  UINT8         bRequest,
  UINT16          wValue,
  UINT16          wIndex,
  void *          pBuffer,
  BYTES         sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT   milliseconds
  );

static void
UsbPumpGcdClient_ReadBulkInt(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE   hPipe,
  void *          pBuffer,
  BYTES         sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT   milliseconds
  );

static void
UsbPumpGcdClient_WriteBulkInt(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE   hPipe,
  void *          pBuffer,
  BYTES         sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT   milliseconds,
  BOOL          fFullTransfer
  );

static void
UsbPumpGcdClient_ReadIsoch(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  void *        pBuffer,
  BYTES       sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
  USBPUMP_ISOCH_PACKET_DESCR *  pIsochDescr,
  BYTES       IsochDescrSize,
  UINT32        IsochStartFrame,
  BOOL        fAsap
  );

static void
UsbPumpGcdClient_WriteIsoch(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  void *        pBuffer,
  BYTES       sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
  USBPUMP_ISOCH_PACKET_DESCR *  pIsochDescr,
  BYTES       IsochDescrSize,
  UINT32        IsochStartFrame,
  BOOL        fAsap
  );

static void
UsbPumpGcdClient_SuspendDev(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient
  );

static void
UsbPumpGcdClient_ResumeDev(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient
  );

static void
UsbPumpGcdClient_ResetPipe(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  UINT32        ResetPipeFlags
  );

static void
UsbPumpStreamTestFsm_Init(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
  );

static void
UsbPumpStreamTestFsm_Finalized(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
  );

static void
UsbPumpStreamTestFsm_Eval(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
  );

static USBPUMP_STREAMTEST_FSM_STATE
UsbPumpStreamTestFsm_Switch(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  BOOL        fEntry
  );

static void
UsbPumpStreamTestFsm_WriteStream(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  void *        pBuffer,
  BYTES       sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
  BOOL        fFullTransfer,
  UINT16        StreamID
  );

static void
UsbPumpStreamTestFsm_ReadStream(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  void *        pBuffer,
  BYTES       sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
  UINT16        StreamID
  );

#if USBPUMP_TRACE
static CONST TEXT *
UsbPumpStreamTestFsm_StateName(
  USBPUMP_STREAMTEST_FSM_STATE  state
  );
#endif


/****************************************************************************\
|
| Callback Functions
|
\****************************************************************************/

static USBPUMP_API_OPEN_CB_FN
UsbPumpGcdClient_OpenSession_Callback;

static USBPUMP_CLASSKIT_OPEN_FUNCTION_CB_FN
UsbPumpGcdClient_OpenFnCbFn;

static USBPUMP_API_CLOSE_CB_FN
UsbPumpGcdClient_CloseFnCbFn;

static USBPUMP_CLASSKIT_GET_NUM_DEVICES_CB_FN
UsbPumpGcdClient_GetNumDevsCbFn;

static USBPUMP_CLASSKIT_GET_BOUND_DEVICES_CB_FN
UsbPumpGcdClient_GetBoundDevsCbFn;

static USBPUMP_USBDI_GENERIC_GET_DEVICE_STATE_CB_FN
UsbPumpGcdClient_GetDevStateCbFn;

static USBPUMP_USBDI_GENERIC_GET_DEVICE_DESCRIPTOR_CB_FN
UsbPumpGcdClient_GetDevDescCbFn;

static USBPUMP_USBDI_GENERIC_GET_CONFIG_DESCRIPTOR_CB_FN
UsbPumpGcdClient_GetConfigDescCbFn;

static USBPUMP_USBDI_GENERIC_GET_CONFIG_TREE_CB_FN
UsbPumpGcdClient_GetConfigTreeCbFn;

static USBPUMP_USBDI_GENERIC_WRITE_CONTROL_PIPE_CB_FN
UsbPumpGcdClient_WriteCtrlCbFn;

static USBPUMP_USBDI_GENERIC_READ_BULKINT_PIPE_CB_FN
UsbPumpGcdClient_ReadBulkIntCbFn;

static USBPUMP_USBDI_GENERIC_WRITE_BULKINT_PIPE_CB_FN
UsbPumpGcdClient_WriteBulkIntCbFn;

static USBPUMP_USBDI_GENERIC_READ_ISOCH_PIPE_CB_FN
UsbPumpGcdClient_ReadIsochCbFn;

static USBPUMP_USBDI_GENERIC_WRITE_ISOCH_PIPE_CB_FN
UsbPumpGcdClient_WriteIsochCbFn;

static USBPUMP_USBDI_GENERIC_READ_STREAM_PIPE_CB_FN
UsbPumpStreamTestFsm_ReadStreamCbFn;

static USBPUMP_USBDI_GENERIC_WRITE_STREAM_PIPE_CB_FN
UsbPumpStreamTestFsm_WriteStreamCbFn;

static USBPUMP_USBDI_GENERIC_SUSPEND_DEVICE_CB_FN
UsbPumpGcdClient_SuspendDevCbFn;

static USBPUMP_USBDI_GENERIC_RESUME_DEVICE_CB_FN
UsbPumpGcdClient_ResumeDevCbFn;

static USBPUMP_USBDI_GENERIC_RESET_PIPE_CB_FN
UsbPumpGcdClient_ResetPipeCbFn;


/****************************************************************************\
|
| Event Notification Handling Function
|
\****************************************************************************/

static USBPUMP_CLASSKIT_NOTIFICATION_FN
UsbPumpGcdClient_Notification;


/****************************************************************************\
|
| Utility Functions
|
\****************************************************************************/

static void
UsbPumpGcdClient_StartLoopbackTest(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  BOOL          fSetInterface
  );

static
UCALLBACKFN
UsbPumpGcdClient_StartLoopback;

static void
UsbPumpGcdClient_ParseConfigTree(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_CFG_NODE * pConfigRoot
  );

static UINT
UsbPumpGcdClient_InitIsochPacketDesc(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT * pPipeCtx
  );

static void
UsbPumpGcdClient_GenerateTestData(
  UINT8 *   pBuffer,
  BYTES   nBuffer
  );

static void
UsbPumpGcdClient_ClearTestData(
  UINT8 *   pBuffer,
  BYTES   nBuffer
  );

static BOOL
UsbPumpGcdClient_CheckTestData(
  UINT8 *   pBuffer,
  BYTES   nBuffer
  );

static USBPUMP_TIMER_DONE_FN
UsbPumpGcdClient_SuspendTimeout;

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
sk_UsbPumpUsbdiClassGdi_ClientOutCall =
  USBPUMP_CLASSKIT_CLASS_OUTCALL_INIT_V1(
    UsbPumpGcdClient_Notification
    );
static
CONST USBPUMP_CLASSKIT_FUNCTION_OUTCALL
sk_UsbPumpUsbdiFunctionGdi_ClientOutCall =
  USBPUMP_CLASSKIT_FUNCTION_OUTCALL_INIT_V1(
    UsbPumpGcdClient_Notification
    );

#if USBPUMP_TRACE
static CONST CHAR * sk_EndpointTypes[] =
  {
  "Control",
  "Iso",
  "Bulk",
  "Interrupt"
  };

static CONST TEXT * CONST sk_vpStreamTestFsm_StateNames[] =
  {
  USBPUMP_STREAMTEST_FSM_STATES__INIT
  };

static CONST TEXT sk_StreamTestFsmUnknownState[] = "<<unknownState>>";
#endif


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

Name: UsbPumpHostApp_GcdClientCreate

Function:
  Sample function demonstrates how to find and use the Generic Class
  driver and open a object session to the Generic Class Driver object

Definition:
  USBPUMP_CLASS_GCD_CLIENT_DATA *
  UsbPumpHostApp_GcdClientCreate(
    UPLATFORM*  pPlatform,
    BYTES   BufferSize,
    UINT    LoopbackCount,
    BOOL    fPerformanceMeasure
    );

Description:
  This function finds the Generic Class drivers and open a class session
  to the Generic Class Driver object.

Returns:
  Pointer of USBPUMP_CLASS_GCD_CLIENT_DATA if success, otherwise NULL
*/

USBPUMP_CLASS_GCD_CLIENT_DATA *
UsbPumpHostApp_GcdClientCreate(
  UPLATFORM*  pPlatform,
  UINT32    DebugFlags,
  BYTES   BufferSize,
  UINT    LoopbackCount,
  BOOL    fPerformanceMeasure
  )
  {
  USBPUMP_OBJECT_ROOT *   pRootObject;
  USBPUMP_OBJECT_HEADER *   pClassObject;
  USBPUMP_CLASS_GCD_CLIENT_DATA * pClient;
  UINT8 *       pBuffer;
  USTAT       Status;

  pRootObject = UsbPumpObject_GetRoot(&pPlatform->upf_Header);

  /*
  || Create a sample GCD client object
  */
  pClient = UsbPumpPlatform_MallocZero(pPlatform, sizeof(*pClient));
  if (pClient == NULL)
    {
    TTUSB_OBJPRINTF((
      &pPlatform->upf_Header,
      UDMASK_ANY | UDMASK_ERRORS,
      "?UsbPumpHostApp_GcdClientCreate:"
      " pClient (%x bytes) allocation failed\n",
      sizeof(*pClient)
      ));
    return NULL;
    }

  BufferSize &= ~1023;
  pBuffer = UsbPumpPlatform_Malloc(pPlatform, BufferSize * 2);
  if (pBuffer == NULL)
    {
    TTUSB_OBJPRINTF((
      &pPlatform->upf_Header,
      UDMASK_ANY | UDMASK_ERRORS,
      "?UsbPumpHostApp_GcdClientCreate:"
      " Buffer (%x bytes) allocation failed\n",
      BufferSize * 2
      ));
    UsbPumpPlatform_Free(pPlatform, pClient, sizeof(*pClient));
    return NULL;
    }

  UsbPumpObject_Init(
    &pClient->ObjectHeader,
    pPlatform->upf_Header.pClassParent,
    /* Generic Class Driver Sample Client */
    UHIL_MEMTAG('G', 'c', 'd', 'S'),
    sizeof(*pClient),
    "sample.gcd.client.mcci.com",
    &pPlatform->upf_Header,
    NULL
    );
  pClient->pPlatform = pPlatform;
  pClient->pReadBuffer = pBuffer;
  pClient->pWriteBuffer = pBuffer + BufferSize;
  pClient->ObjectHeader.ulDebugFlags |= DebugFlags;
/*  pClient->FunctionHandle = NULL; */
  pClient->BufferSize = BufferSize;
  pClient->LoopbackCount = LoopbackCount;
  pClient->fPerformanceMeasure = fPerformanceMeasure;

  UsbPumpTimer_Initialize(
    pPlatform,
    &pClient->Timer,
    UsbPumpGcdClient_SuspendTimeout
    );

  /*
  || Find the Generic Class Driver object
  */
  pClassObject = UsbPumpObject_EnumerateMatchingNames(
        &pRootObject->Header,
        NULL,
        USBPUMP_OBJECT_NAME_ENUM_HOST_GCD
        );

  /*
  || If the GCD object is found, open a class session
  */
  if (pClassObject == NULL)
    {
    TTUSB_OBJPRINTF((
      &pClient->ObjectHeader,
      UDMASK_ERRORS,
      "?UsbPumpHostApp_GcdClientCreate:"
      " Failed to enumerate Generic Class driver\n"
      ));
    UsbPumpObject_DeInit(&pClient->ObjectHeader);
    UsbPumpPlatform_Free(pPlatform, pClient, sizeof(*pClient));
    UsbPumpPlatform_Free(pPlatform, pBuffer, BufferSize);
    return NULL;
    }

  pClient->pClassObject = pClassObject;
  Status = UsbPumpGcdClient_OpenSession(
      pClient,
      pClassObject
      );
  if (Status != USTAT_OK)
    {
    TTUSB_OBJPRINTF((
      &pClient->ObjectHeader,
      UDMASK_ERRORS,
      "?UsbPumpHostApp_GcdClientCreate:"
      " Failed to open a class session(%s)\n",
      UsbPumpStatus_Name(Status)
      ));
    UsbPumpObject_DeInit(&pClient->ObjectHeader);
    UsbPumpPlatform_Free(pPlatform, pClient, sizeof(*pClient));
    UsbPumpPlatform_Free(pPlatform, pBuffer, BufferSize);
    return NULL;
    }

  return pClient;
  }

/*

Name: UsbPumpGcdClient_OpenSession

Function:
  Sample function demonstrates how to open a object session to GCD object
  using UsbPumpObject_OpenSession()

Definition:
  static USTAT
  UsbPumpGcdClient_OpenSession(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    USBPUMP_OBJECT_HEADER *   pGcdClassObjectHeader
    );

Description:
  This function open a class session to the Generic Class Driver object.

Returns:
  USTAT_OK if success
  USTAT_NO_MEMORY if allocating open request memory fails
*/

static USTAT
UsbPumpGcdClient_OpenSession(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_OBJECT_HEADER *   pGcdClassObjectHeader
  )
  {
  void *  pOpenRequestMemory;

  /*
  || Allocate Open Request memory for UsbPumpObject_OpenSession()
  */
  pOpenRequestMemory = UsbPumpPlatform_Malloc(
        pGcdClient->pPlatform,
        USBPUMP_API_OPEN_REQUEST_MEMORY_SIZE
        );
  if (pOpenRequestMemory == NULL)
    {
    TTUSB_OBJPRINTF((
      pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_OpenSession:"
      " Memory (%x bytes) allocation failed\n",
      USBPUMP_API_OPEN_REQUEST_MEMORY_SIZE
      ));
    return USTAT_NO_MEMORY;
    }

  UsbPumpObject_OpenSession(
    pGcdClassObjectHeader,
    pOpenRequestMemory,
    USBPUMP_API_OPEN_REQUEST_MEMORY_SIZE,
    UsbPumpGcdClient_OpenSession_Callback,
    pGcdClient, /* pCallBackContext */
    &gk_UsbPumpUsbdiClassGeneric_Guid,
    NULL,   /* pClientObject -- OPTIONAL */
    &pGcdClient->ClassInCall.GenericCast,
    sizeof(pGcdClient->ClassInCall),
    pGcdClient, /* pClientHandle */
    &sk_UsbPumpUsbdiClassGdi_ClientOutCall.GenericCast,
    sizeof(sk_UsbPumpUsbdiClassGdi_ClientOutCall)
    );

  return USTAT_OK;
  }

static VOID
UsbPumpGcdClient_OpenSession_Callback(
  VOID *      pClientContext,
  USBPUMP_SESSION_HANDLE  SessionHandle,
  UINT32      Status,
  VOID *      pOpenRequestMemory,
  RECSIZE     sizeOpenRequestMemory
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pClientContext;

  if (Status == USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((
      pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_OpenSession_Callback:"
      " OpenSession STATUS_OK %p\n",
      SessionHandle
      ));
    /*
    || Store the session handle to the GCD object. This will be
    || used to call Class In-Calls such as OpenFunction() and
    || GetNumDevices().
    */
    pGcdClient->SessionHandle = SessionHandle;
    }
  else
    {
    TTUSB_OBJPRINTF((
      pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_OpenSession_Callback:"
      " OpenSession failed %s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(Status),
      Status
      ));
    }

  if (pOpenRequestMemory)
    {
    UsbPumpPlatform_Free(
      pGcdClient->pPlatform,
      pOpenRequestMemory,
      sizeOpenRequestMemory
      );
    }
  }

/*

Name: UsbPumpGcdClient_Notification

Function:
  Home of Class Out-Call Notification Function

Definition:
  static VOID
  UsbPumpGcdClient_Notification(
    VOID *        pClientHandle,
    USBPUMP_CLASSKIT_NOTIFICATION NotificationId,
    CONST VOID *      pNotification,
    BYTES       nNotification
    );

Description:
  This function is called by Generic Class Driver to notify class
  event notifications to this client. This function should handle
  class event notifications such as DEVICE_ARRIVAL and DEVIDE_DEPARTURE.

Returns:
  None.

*/

static VOID
UsbPumpGcdClient_Notification(
  VOID *        pClientHandle,
  USBPUMP_CLASSKIT_NOTIFICATION NotificationId,
  CONST VOID *      pNotification,
  BYTES       nNotification
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pClientHandle;
  USBPUMP_UNREFERENCED_PARAMETER(nNotification);

  switch (NotificationId)
    {
  case USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL:
    {
    CONST USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL_INFO * CONST
      pEventInfo = pNotification;

    TTUSB_OBJPRINTF((
      pGcdClient->pClassObject,
      UDMASK_FLOW | UDMASK_ANY,
      " UsbPumpGcdClient_Notification:"
      " pFunction(%p) is arriving.\n",
      pEventInfo->pFunction
      ));

    /*
    || If the client already opened a function session, just ignore
    || this event notification because this sample client cares
    || only one function instance.
    */
    if (pGcdClient->FunctionHandle)
      break;

    /*
    || Retrieve GCD function instance from Notification Information
    || and store it in the client context. This function instance
    || will be used to open a function session to use the function
    || interface of the Generic Class Driver
    */
    pGcdClient->pFunction = pEventInfo->pFunction;
    pGcdClient->hDefaultPipe =
        pGcdClient->pFunction->Function.PortInfo.hDefaultPipe;

    /*
    || Call OpenFunction Class In-Call to open a function session
    */
    UsbPumpGcdClient_OpenFunction(pGcdClient, pClientHandle);
    }
    break;

  case USBPUMP_CLASSKIT_EVENT_DEVICE_DEPARTURE:
    {
    CONST USBPUMP_CLASSKIT_EVENT_DEVICE_DEPARTURE_INFO * CONST
      pEventInfo = pNotification;

    /*
    || If the departing function instance is the same as
    || the function instance that this client has and this client
    || is using a function session to the function instance,
    || the client should close the function session by calling
    || CloseFunction Function In-Call.
    */
    if ((pEventInfo->pFunction == pGcdClient->pFunction) &&
      pGcdClient->FunctionHandle)
      {
      TTUSB_OBJPRINTF((
        pGcdClient->pClassObject,
        UDMASK_FLOW | UDMASK_ANY,
        " UsbPumpGcdClient_Notification:"
        " pFunction(%p) is departing.\n",
        pEventInfo->pFunction
        ));

      UsbPumpGcdClient_CloseFunction(pGcdClient);
      pGcdClient->pFunction = NULL;
      pGcdClient->hDefaultPipe = NULL;
      }
    }
    break;

  case USBPUMP_USBDI_GENERIC_EVENT_DEVICE_SUSPENDED:
    {
    CONST USBPUMP_USBDI_GENERIC_EVENT_DEVICE_SUSPENDED_INFO * CONST
      pEventInfo = pNotification;

    /*
    || If the suspended function instance is the same as
    || the function instance that this client has and this client
    || is using a function session to the function instance,
    || this sample appliaction tries to resume the device.
    */
    if ((pEventInfo->pFunction == pGcdClient->pFunction) &&
      pGcdClient->FunctionHandle)
      {
      USTAT TimerStat;
      USBPUMP_TIMER_TIMEOUT Timeout;

      TTUSB_OBJPRINTF((
        pGcdClient->pClassObject,
        UDMASK_FLOW,
        " UsbPumpGcdClient_Notification:"
        " pFunction(%p) is suspended.\n",
        pEventInfo->pFunction
        ));

      Timeout = USBPUMP_CLASS_GCD_CLIENT_SUSPEND_TIMEOUT;
      if (pGcdClient->iCurrPipe == 0)
        Timeout <<= 1;
      TimerStat = UsbPumpTimer_Start(
          pGcdClient->pPlatform,
          &pGcdClient->Timer,
          Timeout,
          NULL
          );

      if (TimerStat != USTAT_OK)
        {
        TTUSB_OBJPRINTF((
          pGcdClient->pClassObject,
          UDMASK_ERRORS,
          "?UsbPumpGcdClient_Notification:"
          " Starting timer failed(%d)\n",
          TimerStat
          ));
        }
      }
    }
    break;

  case USBPUMP_USBDI_GENERIC_EVENT_DEVICE_RESUMED:
    {
    CONST USBPUMP_USBDI_GENERIC_EVENT_DEVICE_RESUMED_INFO *
      pEventInfo = pNotification;

    /*
    || If the resumed function instance is the same as
    || the function instance that this client has and this client
    || is using a function session to the function instance,
    || this sample appliaction tries to resume the device.
    */
    if ((pEventInfo->pFunction == pGcdClient->pFunction) &&
      pGcdClient->FunctionHandle)
      {
      TTUSB_OBJPRINTF((
        pGcdClient->pClassObject,
        UDMASK_FLOW,
        " UsbPumpGcdClient_Notification:"
        " pFunction(%p) is resumed\n",
        pEventInfo->pFunction
        ));

      pGcdClient->NumIterations = pGcdClient->LoopbackCount;
      UsbPumpGcdClient_StartLoopbackTest(pGcdClient, TRUE);
      }
    }
    break;

  case USBPUMP_CLASSKIT_EVENT_FUNCTION_OPEN:
    {
    CONST USBPUMP_CLASSKIT_EVENT_FUNCTION_OPEN_INFO *
      pEventInfo = pNotification;

    USBPUMP_TRACE_PARAMETER(pEventInfo);
    TTUSB_OBJPRINTF((
      pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_Notification:"
      " pFunction(%p) has opened."
      " %d sessions are opened.\n",
      pEventInfo->pFunction,
      pEventInfo->nOpenClients
      ));
    }
    break;

  case USBPUMP_CLASSKIT_EVENT_FUNCTION_CLOSE:
    {
    CONST USBPUMP_CLASSKIT_EVENT_FUNCTION_CLOSE_INFO *
      pEventInfo = pNotification;

    USBPUMP_TRACE_PARAMETER(pEventInfo);
    TTUSB_OBJPRINTF((
      pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_Notification:"
      " pFunction(%p) has closed."
      " %d sessions are opened.\n",
      pEventInfo->pFunction,
      pEventInfo->nOpenClients
      ));
    }
    break;

  default:
    break;
    }
  }

static VOID
UsbPumpGcdClient_SuspendTimeout(
  UPLATFORM *     pPlatform,
  USBPUMP_TIMER *     pTimerObject,
  USBPUMP_MILLISECONDS    Now
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient =
    __TMS_CONTAINER_OF(pTimerObject, USBPUMP_CLASS_GCD_CLIENT_DATA, Timer);

  USBPUMP_UNREFERENCED_PARAMETER(pPlatform);
  USBPUMP_UNREFERENCED_PARAMETER(Now);

  UsbPumpGcdClient_ResumeDev(pGcdClient);
  }

/*

Name: UsbPumpGcdClient_OpenFunction

Function:
  Sample function demonstrates how to open a function session to
  a function instance using OpenFunction Class In-Call

Definition:
  static VOID
  UsbPumpGcdClient_OpenFunction(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    VOID *        pClientHandle
    );

Description:
  This function opens a function session to the Generic Class Driver
  function instance bound to a USB device.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_OpenFunction(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  VOID *        pClientHandle
  )
  {
  (*pGcdClient->ClassInCall.GenDrv.pOpenFunctionFn)(
    pGcdClient->SessionHandle,
    UsbPumpGcdClient_OpenFnCbFn,
    pGcdClient,
    pGcdClient->pFunction,
    &pGcdClient->FunctionInCall.ClassKitCast,
    sizeof(pGcdClient->FunctionInCall),
    pClientHandle,
    &sk_UsbPumpUsbdiFunctionGdi_ClientOutCall,
    sizeof(sk_UsbPumpUsbdiFunctionGdi_ClientOutCall)
    );
  }

static VOID
UsbPumpGcdClient_OpenFnCbFn(
  VOID *        pCallbackCtx,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  USBPUMP_SESSION_HANDLE    FunctionHandle
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_OpenFnCbFn: ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    pGcdClient->FunctionHandle = FunctionHandle;
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_OpenFnCbFn:"
      " FunctionHandle:%p\n",
      FunctionHandle
      ));

    /*
    || Get the number of devices (function instances) bound to
    || this Class Driver.
    */
    UsbPumpGcdClient_GetNumDevs(pGcdClient);
    }
  }

/*

Name: UsbPumpGcdClient_CloseFunction

Function:
  Sample function demonstrates how to close the function session that
  the client is currently using by calling CloseFunction Function In-Call

Definition:
  static VOID
  UsbPumpGcdClient_CloseFunction(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
    );

Description:
  This function closes the function session.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_CloseFunction(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
  )
  {
  /*
  || Call CloseFunction Function In-Call with the function handle
  || returned from OpenFunction Class In-Call.
  */
  (*pGcdClient->FunctionInCall.GenDrv.pCloseFn)(
    pGcdClient->FunctionHandle,
    UsbPumpGcdClient_CloseFnCbFn,
    pGcdClient
    );
  }

static VOID
UsbPumpGcdClient_CloseFnCbFn(
  VOID *        pCallbackCtx,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_CloseFnCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    pGcdClient->FunctionHandle = NULL;
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_CloseFnCbFn: OK\n"
      ));
    }
  }

/*

Name: UsbPumpGcdClient_GetNumDevs

Function:
  Sample function demonstrates how to get the number of devices (function
  instances) bound to the Generic Class Driver.

Definition:
  static VOID
  UsbPumpGcdClient_GetNumDevs(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
    );

Description:
  This function retrieves the number of devices bound to GCD currently.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_GetNumDevs(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
  )
  {
  (*pGcdClient->ClassInCall.GenDrv.pGetNumDevicesFn)(
    pGcdClient->SessionHandle,
    UsbPumpGcdClient_GetNumDevsCbFn,
    pGcdClient
    );
  }

static VOID
UsbPumpGcdClient_GetNumDevsCbFn(
  VOID *        pCallbackCtx,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  UINT32        ulNumOfInstances
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_GetNumDevsCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_GetNumDevsCbFn:"
      " %d(ulNumOfInstances) devices are bound to GCD.\n",
      ulNumOfInstances
      ));

    /*
    || Get the vector of devices (function instances) bound to
    || this Class Driver.
    */
    UsbPumpGcdClient_GetBoundDevs(
      pGcdClient,
      ulNumOfInstances
      );
    }
  }

/*

Name: UsbPumpGcdClient_GetBoundDevs

Function:
  Sample function demonstrates how to get the vector of devices (function
  instances) bound to the Generic Class Driver.

Definition:
  static VOID
  UsbPumpGcdClient_GetBoundDevs(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    UINT32        ulNumOfInstances
    );

Description:
  This function retrieves the vector of devices bound to GCD currently.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_GetBoundDevs(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  UINT32        ulNumOfInstances
  )
  {
  USBPUMP_USBDI_FUNCTION ** pFunctionVector;
  BYTES       sizeFunctionVector;

  sizeFunctionVector = sizeof(*pFunctionVector) * ulNumOfInstances;
  pFunctionVector = UsbPumpPlatform_Malloc(
        pGcdClient->pPlatform,
        sizeFunctionVector
        );

  if (pFunctionVector)
    {
    (*pGcdClient->ClassInCall.GenDrv.pGetBoundDevicesFn)(
      pGcdClient->SessionHandle,
      UsbPumpGcdClient_GetBoundDevsCbFn,
      pGcdClient,
      pFunctionVector,
      sizeFunctionVector
      );
    }
  else
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_GetBoundDevs:"
      " Memory (%x bytes) allocation failed\n",
      sizeFunctionVector
      ));
    }
  }

static VOID
UsbPumpGcdClient_GetBoundDevsCbFn(
  VOID *        pCallbackCtx,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  UINT32        nWrittenFunction,
  USBPUMP_USBDI_FUNCTION ** pFunctionVector,
  BYTES       sizeFunctionVector
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_GetBoundDevsCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    UINT32  i;

    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW | UDMASK_ANY,
      " UsbPumpGcdClient_GetBoundDevsCbFn:"
      " nFunction:%d sizeVector=%d\n",
      nWrittenFunction,
      sizeFunctionVector
      ));

    for (i = 0; i < nWrittenFunction; ++i)
      {
      USBPUMP_USBDI_FUNCTION * CONST  pFunction =
        pFunctionVector[i];
      CONST USBPUMP_DEVICE_SPEED OperatingSpeed =
        pFunction->Function.PortInfo.OperatingSpeed;
      TEXT *  pDeviceSpeedName;

      if (OperatingSpeed == USBPUMP_DEVICE_SPEED_HIGH)
        pDeviceSpeedName = "HIGH";
      else if (OperatingSpeed == USBPUMP_DEVICE_SPEED_FULL)
        pDeviceSpeedName = "FULL";
      else if (OperatingSpeed == USBPUMP_DEVICE_SPEED_LOW)
        pDeviceSpeedName = "LOW";
      else if (OperatingSpeed == USBPUMP_DEVICE_SPEED_WIRELESS)
        pDeviceSpeedName = "WIRELESS";
      else
        pDeviceSpeedName = "Unknown";

      USBPUMP_TRACE_PARAMETER(pDeviceSpeedName);

      TTUSB_OBJPRINTF((pGcdClient->pClassObject,
        UDMASK_FLOW | UDMASK_ANY,
        "  Vector[%d]=%p"
        " wMaxPower=%d OperatingSpeed=%s\n",
        i,
        pFunction,
        pFunction->Function.PortInfo.wMaxPower,
        pDeviceSpeedName
        ));
      }

    /*
    || Get the state of the device (function instance) that this
    || client opened the function session to.
    */
    UsbPumpGcdClient_GetDevState(
      pGcdClient,
      &pGcdClient->DeviceState
      );
    }

  UsbPumpPlatform_Free(
    pGcdClient->pPlatform,
    pFunctionVector,
    sizeFunctionVector
    );
  }

/*

Name: UsbPumpGcdClient_GetDevState

Function:
  Sample function demonstrates how to get the device state

Definition:
  static VOID
  UsbPumpGcdClient_GetDevState(
    USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
    USBPUMP_USBDI_GENERIC_DEVICE_STATE *  pDeviceState
    );

Description:
  This function gets the state of the device (function instance) which
  the client's function handle connects to.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_GetDevState(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  USBPUMP_USBDI_GENERIC_DEVICE_STATE *  pDeviceState
  )
  {
  /* Ignore request handle because it can be NULL even if success */
  (*pGcdClient->FunctionInCall.GenDrv.pGetDeviceStateFn)(
    pGcdClient->FunctionHandle,
    UsbPumpGcdClient_GetDevStateCbFn,
    pGcdClient,
    pDeviceState
    );
  }

static VOID
UsbPumpGcdClient_GetDevStateCbFn(
  VOID *          pCallbackCtx,
  VOID *          pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS    ErrorCode,
  USBPUMP_USBDI_GENERIC_DEVICE_STATE *  pDeviceState
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_TRACE_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(pDeviceState);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_GetDevStateCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW | UDMASK_ANY,
      " UsbPumpGcdClient_GetDevStateCbFn(%p):"
      " fRemoved:%d fStopped:%d"
      " fFunctionOpened:%d\n",
      pRequestHandle,
      pDeviceState->fRemoved,
      pDeviceState->fStopped,
      pDeviceState->fFunctionOpened
      ));

    /*
    || Get the device descriptor of the USB device
    */
    UsbPumpGcdClient_GetDevDesc(
      pGcdClient,
      pGcdClient->pReadBuffer,
      pGcdClient->BufferSize
      );
    }
  }

/*

Name: UsbPumpGcdClient_GetDevDesc

Function:
  Sample function demonstrates how to get the device descriptor

Definition:
  static VOID
  UsbPumpGcdClient_GetDevDesc(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    VOID *        pBuffer,
    BYTES       sizeBuffer
    );

Description:
  This function gets the device descriptor of the USB device

Returns:
  None

*/

static VOID
UsbPumpGcdClient_GetDevDesc(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  VOID *        pBuffer,
  BYTES       sizeBuffer
  )
  {
  VOID *  pRequestHandle;

  pRequestHandle =
    (*pGcdClient->FunctionInCall.GenDrv.pGetDeviceDescriptorFn)(
      pGcdClient->FunctionHandle,
      UsbPumpGcdClient_GetDevDescCbFn,
      pGcdClient,
      pBuffer,
      sizeBuffer
      );
  if (!pRequestHandle)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_GetDevDesc:"
      " Error - pRequestHandle is NULL!\n"
      ));
    return;
    }

  /*
  || This Function In-Call returns the pointer of the request handle
  || which is used to cancel the request.
  */
  TTUSB_OBJPRINTF((pGcdClient->pClassObject,
    UDMASK_FLOW,
    " UsbPumpGcdClient_GetDevDesc:"
    " pRequestHandle=%p\n",
    pRequestHandle
    ));
  }

static VOID
UsbPumpGcdClient_GetDevDescCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  BYTES       nBytesRead
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(sizeBuffer);
  USBPUMP_TRACE_PARAMETER(pBuffer);
  USBPUMP_TRACE_PARAMETER(nBytesRead);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_GetDevDescCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    BYTES i;
    USBIF_DEVDESC * CONST pDevDesc = pBuffer;

    pGcdClient->iManufacturer = pDevDesc->iManufacturer;
    pGcdClient->iProduct = pDevDesc->iProduct;
    pGcdClient->iSerialNumber = pDevDesc->iSerialNumber;

    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_GetDevDescCbFn:"
      " pBuffer:%p, sizeBuffer:%d, nBytesRead:%d\n",
      pBuffer, sizeBuffer, nBytesRead
      ));

    for (i = 0; i < nBytesRead; ++i)
      {
      TTUSB_OBJPRINTF((pGcdClient->pClassObject,
        UDMASK_FLOW,
        "%02x %s",
        ((UINT8 *) pBuffer)[i],
        (((i + 1) % 8)?"":"\n")
        ));
      }

    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      "\n"
      ));
    /*
    || Get the configuration descriptor of the USB device
    || iConfig is not bConfigurationValue but the index of the
    || configuration. iConfig zero means the 1st configuration.
    */
    UsbPumpGcdClient_GetConfigDesc(
      pGcdClient,
      pGcdClient->pReadBuffer,
      pGcdClient->BufferSize,
      0
      );
    }
  }

/*

Name: UsbPumpGcdClient_GetConfigDesc

Function:
  Sample function demonstrates how to get the configuration descriptor

Definition:
  static VOID
  UsbPumpGcdClient_GetConfigDesc(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    VOID *        pBuffer,
    BYTES       sizeBuffer,
    UINT8       iConfig
    );

Description:
  This function gets the configuration descriptor of the USB device.

Returns:
  None

Notes:
  iConfig indicates not bConfigurationValue but the index of the
  configuration. iConfig zero means the fist configuration and one
  means the second configuration.

*/

static VOID
UsbPumpGcdClient_GetConfigDesc(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  UINT8       iConfig
  )
  {
  VOID *  pRequestHandle;

  pRequestHandle =
    (*pGcdClient->FunctionInCall.GenDrv.pGetConfigDescriptorFn)(
      pGcdClient->FunctionHandle,
      UsbPumpGcdClient_GetConfigDescCbFn,
      pGcdClient,
      pBuffer,
      sizeBuffer,
      iConfig
      );
  if (!pRequestHandle)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_GetConfigDesc:"
      " Error - pRequestHandle is NULL!\n"
      ));

    return;
    }

  /*
  || This Function In-Call returns the pointer of the request handle
  || which is used to cancel the request.
  */
  TTUSB_OBJPRINTF((pGcdClient->pClassObject,
    UDMASK_FLOW,
    " UsbPumpGcdClient_GetConfigDesc:"
    " pRequestHandle=%p\n",
    pRequestHandle
    ));
  }

static VOID
UsbPumpGcdClient_GetConfigDescCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  BYTES       nBytesRead
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(pBuffer);
  USBPUMP_TRACE_PARAMETER(sizeBuffer);
  USBPUMP_TRACE_PARAMETER(nBytesRead);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_GetConfigDescCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    BYTES i;
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_GetConfigDescCbFn:"
      " pBuffer:%p, sizeBuffer:%d, nBytesRead:%d\n",
      pBuffer, sizeBuffer, nBytesRead
      ));
    for (i = 0; i < nBytesRead; ++i)
      {
      TTUSB_OBJPRINTF((pGcdClient->pClassObject,
        UDMASK_FLOW,
        "%02x %s",
        ((UINT8 *) pBuffer)[i],
        (((i + 1) % 8)?"":"\n")
        ));
      }
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      "\n"
      ));
    /*
    || Get the config tree of the device's configuration.
    || Because the client doesn't know the actual size of the
    || config tree, the client should retrieve the size of the
    || config tree by passing small buffer.
    */
    UsbPumpGcdClient_GetConfigTree(
      pGcdClient,
      pGcdClient->pConfigRoot,
      0
      );
    }
  }

/*

Name: UsbPumpGcdClient_GetConfigTree

Function:
  Sample function demonstrates how to get the config tree

Definition:
  static VOID
  UsbPumpGcdClient_GetConfigTree(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    VOID *        pBuffer,
    BYTES       sizeBuffer
    );

Description:
  This function gets the config tree of the specific configuration of
  the USB device. If sizeBuffer is not large enough, this function returns
  actual size of the config tree through callback routine.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_GetConfigTree(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  VOID *        pBuffer,
  UINT32        sizeBuffer
  )
  {
  /* Ignore request handle because it can be NULL even if success */
  (*pGcdClient->FunctionInCall.GenDrv.pGetConfigTreeFn)(
    pGcdClient->FunctionHandle,
    UsbPumpGcdClient_GetConfigTreeCbFn,
    pGcdClient,
    pBuffer,
    sizeBuffer
    );
  }

static VOID
UsbPumpGcdClient_GetConfigTreeCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  BYTES       nBytesRead
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(sizeBuffer);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_GetConfigTreeCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    if (ErrorCode == USBPUMP_USBDI_GENERIC_STATUS_BUFFER_TOO_SMALL)
      {
      /*
      || If the ErrorCode indicates "Buffer is too small",
      || nBytesRead is the actual size of the configuration
      || tree. Try to get the config tree again with the
      || actual size of the config tree.
      */
      pGcdClient->pConfigRoot =
        UsbPumpPlatform_Malloc(
          pGcdClient->pPlatform,
          nBytesRead
          );

      UsbPumpGcdClient_GetConfigTree(
        pGcdClient,
        pGcdClient->pConfigRoot,
        nBytesRead
        );
      }
    }
  else
    {
    USBPUMP_USBDI_CFG_NODE * CONST pConfigRoot = pBuffer;

    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_GetConfigTreeCbFn:"
      " pConfigRoot:%p, sizeBuffer:%d, nBytesRead:%d\n",
      pConfigRoot, sizeBuffer, nBytesRead
      ));

    UsbPumpGcdClient_ParseConfigTree(
      pGcdClient,
      pConfigRoot
      );

    pGcdClient->NumIterations = pGcdClient->LoopbackCount;
    UsbPumpGcdClient_StartLoopbackTest(pGcdClient, TRUE);
    }
  }


/*

Name: UsbPumpGcdClient_WriteCtrl

Function:
  Sample function demonstrates how to send Control OUT request to
  the USB device's control endpoint.

Definition:
  static VOID
  UsbPumpGcdClient_WriteCtrl(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    USBPUMP_USBDI_PIPE_HANDLE hPipe,
    UINT8         bmRequestType,
    UINT8       bRequest,
    UINT16        wValue,
    UINT16        wIndex,
    VOID *        pBuffer,
    BYTES       sizeBuffer,
    USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds
    );

Description:
  This function sends Control OUT request to write data to the control
  endpoint of the USB device.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_WriteCtrl(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  UINT8         bmRequestType,
  UINT8       bRequest,
  UINT16        wValue,
  UINT16        wIndex,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds
  )
  {
  VOID *  pRequestHandle;

  pRequestHandle =
    (*pGcdClient->FunctionInCall.GenDrv.pWriteControlPipeFn)(
      pGcdClient->FunctionHandle,
      UsbPumpGcdClient_WriteCtrlCbFn,
      pGcdClient,
      hPipe,
      bmRequestType,
      bRequest,
      wValue,
      wIndex,
      pBuffer,
      sizeBuffer,
      milliseconds
      );

  if (!pRequestHandle)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_WriteCtrl:"
      " ERROR - pRequestHandle is NULL!\n"
      ));
    return;
    }

  /*
  || This Function In-Call returns the pointer of the request handle
  || which is used to cancel the request.
  */
  TTUSB_OBJPRINTF((pGcdClient->pClassObject,
    UDMASK_FLOW,
    " UsbPumpGcdClient_WriteCtrl:"
    " pRequestHandle=%p\n",
    pRequestHandle
    ));
  }

static VOID
UsbPumpGcdClient_WriteCtrlCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  VOID CONST *      pBuffer,
  BYTES       sizeBuffer,
  BYTES       nBytesWritten
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(pBuffer);
  USBPUMP_TRACE_PARAMETER(sizeBuffer);
  USBPUMP_TRACE_PARAMETER(nBytesWritten);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_WriteCtrlCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_WriteCtrlCbFn:"
      " pBuffer:%p, sizeBuffer:%d, nBytesWritten:%d\n",
      pBuffer, sizeBuffer, nBytesWritten
      ));
    if (pGcdClient->NumIterations > 0)
      {
      UsbPumpGcdClient_StartLoopbackTest(pGcdClient, FALSE);
      }
    else
      {
      /*
      || All loopback and SetInterface(Alt=0) are done.
      || Make the USB device suspend
      */
      UsbPumpGcdClient_SuspendDev(pGcdClient);
      }
    }
  }

/*

Name: UsbPumpGcdClient_WriteBulkInt

Function:
  Sample function demonstrates how to write data to a specific bulk or
  interrupt endpoint.

Definition:
  static VOID
  UsbPumpGcdClient_WriteBulkInt(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    USBPUMP_USBDI_PIPE_HANDLE hPipe,
    VOID *        pBuffer,
    BYTES       sizeBuffer,
    USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
    BOOL        fFullTransfer
    );

Description:
  This function writes data to a specific bulk or interrupt endpoint.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_WriteBulkInt(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
  BOOL        fFullTransfer
  )
  {
  VOID *  pRequestHandle;

  pRequestHandle =
    (*pGcdClient->FunctionInCall.GenDrv.pWriteBulkIntPipeFn)(
      pGcdClient->FunctionHandle,
      UsbPumpGcdClient_WriteBulkIntCbFn,
      pGcdClient,
      hPipe,
      pBuffer,
      sizeBuffer,
      milliseconds,
      fFullTransfer
      );
  if (!pRequestHandle)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_WriteBulkInt:"
      " ERROR - pRequestHandle is NULL!\n"
      ));

    return;
    }

  /*
  || This Function In-Call returns the pointer of the request handle
  || which is used to cancel the request.
  */
  TTUSB_OBJPRINTF((pGcdClient->pClassObject,
    UDMASK_FLOW,
    " UsbPumpGcdClient_WriteBulkInt:"
    " pRequestHandle=%p, hPipe=%p, pBuffer=%p, sizeBuffer=%d\n",
    pRequestHandle,
    hPipe,
    pBuffer,
    sizeBuffer
    ));
  }

static VOID
UsbPumpGcdClient_WriteBulkIntCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  VOID CONST *      pBuffer,
  BYTES       sizeBuffer,
  BYTES       nBytesWritten
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(pBuffer);
  USBPUMP_TRACE_PARAMETER(sizeBuffer);
  USBPUMP_TRACE_PARAMETER(nBytesWritten);
  USBPUMP_TRACE_PARAMETER(pGcdClient);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_WriteBulkIntCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_WriteBulkIntCbFn:"
      " pBuffer:%p, sizeBuffer:%d, nBytesWritten:%d\n",
      pBuffer, sizeBuffer, nBytesWritten
      ));
    }
  }

/*

Name: UsbPumpGcdClient_WriteIsoch

Function:
  Sample function demonstrates how to write data to a specific isochronous
  endpoint.

Definition:
  static VOID
  UsbPumpGcdClient_WriteIsoch(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    USBPUMP_USBDI_PIPE_HANDLE hPipe,
    VOID *        pBuffer,
    BYTES       sizeBuffer,
    USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
    USBPUMP_ISOCH_PACKET_DESCR *  pIsochDescr,
    BYTES       IsochDescrSize,
    UINT32        IsochStartFrame,
    BOOL        fAsap
    );

Description:
  This function writes data to a specific isochronous endpoint.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_WriteIsoch(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
  USBPUMP_ISOCH_PACKET_DESCR *  pIsochDescr,
  BYTES       IsochDescrSize,
  UINT32        IsochStartFrame,
  BOOL        fAsap
  )
  {
  VOID *  pRequestHandle;

  pRequestHandle =
    (*pGcdClient->FunctionInCall.GenDrv.pWriteIsochPipeFn)(
      pGcdClient->FunctionHandle,
      UsbPumpGcdClient_WriteIsochCbFn,
      pGcdClient,
      hPipe,
      pBuffer,
      sizeBuffer,
      milliseconds,
      pIsochDescr,
      IsochDescrSize,
      IsochStartFrame,
      fAsap
      );
  if (!pRequestHandle)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_WriteIsoch:"
      " ERROR - pRequestHandle is NULL!\n"
      ));

    return;
    }

  /*
  || This Function In-Call returns the pointer of the request handle
  || which is used to cancel the request.
  */
  TTUSB_OBJPRINTF((pGcdClient->pClassObject,
    UDMASK_FLOW,
    " UsbPumpGcdClient_WriteIsoch:"
    " pRequestHandle=%p, hPipe=%p, pBuffer=%p, sizeBuffer=%d\n",
    pRequestHandle,
    hPipe,
    pBuffer,
    sizeBuffer
    ));
  }

static VOID
UsbPumpGcdClient_WriteIsochCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  VOID CONST *      pBuffer,
  BYTES       sizeBuffer,
  BYTES       nBytesWritten,
  USBPUMP_ISOCH_PACKET_DESCR *  pIsochDescr,
  BYTES       IsochDescrSize,
  UINT32        IsochStartFrame,
  BYTES       nIsochErrs
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(pBuffer);
  USBPUMP_TRACE_PARAMETER(sizeBuffer);
  USBPUMP_TRACE_PARAMETER(nBytesWritten);
  USBPUMP_UNREFERENCED_PARAMETER(pIsochDescr);
  USBPUMP_UNREFERENCED_PARAMETER(IsochDescrSize);
  USBPUMP_UNREFERENCED_PARAMETER(IsochStartFrame);
  USBPUMP_TRACE_PARAMETER(nIsochErrs);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_WriteIsochCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    UINT  nIsochDesc;
    USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT * CONST pPipeCtx =
      &pGcdClient->PipeCtx[pGcdClient->iCurrPipe + 1];

    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_WriteIsochCbFn:"
      " pBuffer:%p, sizeBuffer:%d, nBytesWritten:%d"
      " nIsochErrs:%d\n",
      pBuffer, sizeBuffer, nBytesWritten, nIsochErrs
      ));

    /*
    || The target USB device in this sample client implements
    || loopback protocol. So we can read the same data as the data
    || we wrote to the Isoch OUT endpoint from pair-IN endpoint.
    */
    nIsochDesc = UsbPumpGcdClient_InitIsochPacketDesc(
        pGcdClient,
        pPipeCtx
        );

    UsbPumpGcdClient_ReadIsoch(
      pGcdClient,
      pPipeCtx->pPipe->hPipe,
      pGcdClient->pReadBuffer,
      nIsochDesc * pPipeCtx->pPipe->wMaxPacketSize,
      /* Timeout */ 5000 /* ms */,
      pGcdClient->IsochDesc,
      nIsochDesc * sizeof(USBPUMP_ISOCH_PACKET_DESCR),
      0 /* IsochStartFrame */,
      TRUE /* fAsap */
      );
    }
  }

/*

Name: UsbPumpGcdClient_ReadBulkInt

Function:
  Sample function demonstrates how to read data from a specific bulk or
  interrupt endpoint.

Definition:
  static VOID
  UsbPumpGcdClient_ReadBulkInt(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    USBPUMP_USBDI_PIPE_HANDLE hPipe,
    VOID *        pBuffer,
    BYTES       sizeBuffer,
    USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds
    );

Description:
  This function reads data from a specific bulk or interrupt endpoint.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_ReadBulkInt(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds
  )
  {
  VOID *  pRequestHandle;

  pRequestHandle =
    (*pGcdClient->FunctionInCall.GenDrv.pReadBulkIntPipeFn)(
      pGcdClient->FunctionHandle,
      UsbPumpGcdClient_ReadBulkIntCbFn,
      pGcdClient,
      hPipe,
      pBuffer,
      sizeBuffer,
      milliseconds
      );
  if (!pRequestHandle)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_ReadBulkInt:"
      " ERROR - pRequestHandle is NULL!\n"
      ));

    return;
    }

  /*
  || This Function In-Call returns the pointer of the request handle
  || which is used to cancel the request.
  */
  TTUSB_OBJPRINTF((pGcdClient->pClassObject,
    UDMASK_FLOW,
    " UsbPumpGcdClient_ReadBulkInt:"
    " pRequestHandle=%p, hPipe=%p, pBuffer=%p, sizeBuffer=%d\n",
    pRequestHandle,
    hPipe,
    pBuffer,
    sizeBuffer
    ));
  }

static VOID
UsbPumpGcdClient_ReadBulkIntCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  BYTES       nBytesRead
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(sizeBuffer);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_ReadBulkIntCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    BOOL fResult;
    USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT * CONST pPipeCtx =
      &pGcdClient->PipeCtx[pGcdClient->iCurrPipe];

    if (! pGcdClient->fPerformanceMeasure)
      {
      fResult = UsbPumpGcdClient_CheckTestData(
          pBuffer,
          nBytesRead
          );
      }
    else
      {
      fResult = TRUE;
      }

    USBPUMP_TRACE_PARAMETER(fResult);

    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_ReadBulkIntCbFn:"
      " pBuffer:%p, sizeBuffer:%d, nBytesRead:%d,"
      " check:%d\n",
      pBuffer, sizeBuffer, nBytesRead, fResult
      ));

    /*
    || Issue RESET_PIPE for the Bulk OUT endpoint of
    || Interface #1's AlternativeSetting #2
    */
    UsbPumpGcdClient_ResetPipe(
      pGcdClient,
      pPipeCtx->pPipe->hPipe,
      USBPUMP_CLASS_GCD_CLIENT_RESET_PIPE_FLAGS
      );
    }
  }

/*

Name: UsbPumpGcdClient_ReadIsoch

Function:
  Sample function demonstrates how to read data from a specific bulk or
  interrupt endpoint.

Definition:
  static VOID
  UsbPumpGcdClient_ReadIsoch(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    USBPUMP_USBDI_PIPE_HANDLE hPipe,
    VOID *        pBuffer,
    BYTES       sizeBuffer,
    USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
    USBPUMP_ISOCH_PACKET_DESCR *  pIsochDescr,
    BYTES       IsochDescrSize,
    UINT32        IsochStartFrame,
    BOOL        fAsap
    );

Description:
  This function reads data from a specific bulk or interrupt endpoint.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_ReadIsoch(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
  USBPUMP_ISOCH_PACKET_DESCR *  pIsochDescr,
  BYTES       IsochDescrSize,
  UINT32        IsochStartFrame,
  BOOL        fAsap
  )
  {
  VOID *  pRequestHandle;

  pRequestHandle =
    (*pGcdClient->FunctionInCall.GenDrv.pReadIsochPipeFn)(
      pGcdClient->FunctionHandle,
      UsbPumpGcdClient_ReadIsochCbFn,
      pGcdClient,
      hPipe,
      pBuffer,
      sizeBuffer,
      milliseconds,
      pIsochDescr,
      IsochDescrSize,
      IsochStartFrame,
      fAsap
      );
  if (!pRequestHandle)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_ReadIsoch:"
      " ERROR - pRequestHandle is NULL!\n"
      ));

    return;
    }

  /*
  || This Function In-Call returns the pointer of the request handle
  || which is used to cancel the request.
  */
  TTUSB_OBJPRINTF((pGcdClient->pClassObject,
    UDMASK_FLOW,
    " UsbPumpGcdClient_ReadIsoch:"
    " pRequestHandle=%p, hPipe=%p, pBuffer=%p, sizeBuffer=%d\n",
    pRequestHandle,
    hPipe,
    pBuffer,
    sizeBuffer
    ));
  }

static VOID
UsbPumpGcdClient_ReadIsochCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  BYTES       nBytesRead,
  USBPUMP_ISOCH_PACKET_DESCR *  pIsochDescr,
  BYTES       IsochDescrSize,
  UINT32        IsochStartFrame,
  BYTES       nIsochErrs
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(sizeBuffer);
  USBPUMP_UNREFERENCED_PARAMETER(pIsochDescr);
  USBPUMP_UNREFERENCED_PARAMETER(IsochDescrSize);
  USBPUMP_UNREFERENCED_PARAMETER(IsochStartFrame);
  USBPUMP_TRACE_PARAMETER(nIsochErrs);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_ReadIsochCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    BOOL fResult;
    USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT * CONST pPipeCtx =
      &pGcdClient->PipeCtx[pGcdClient->iCurrPipe];

    if (! pGcdClient->fPerformanceMeasure)
      {
      fResult = UsbPumpGcdClient_CheckTestData(
          pBuffer,
          nBytesRead
          );
      }
    else
      {
      fResult = TRUE;
      }

    USBPUMP_TRACE_PARAMETER(fResult);

    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_ReadIsochCbFn:"
      " pBuffer:%p, sizeBuffer:%d, nBytesRead:%d,"
      " check:%d nIsochErrs:%d\n",
      pBuffer, sizeBuffer, nBytesRead, fResult, nIsochErrs
      ));

    /*
    || Issue RESET_PIPE for the Bulk OUT endpoint of
    || Interface #1's AlternativeSetting #2
    */
    UsbPumpGcdClient_ResetPipe(
      pGcdClient,
      pPipeCtx->pPipe->hPipe,
      USBPUMP_CLASS_GCD_CLIENT_RESET_PIPE_FLAGS
      );
    }
  }

/*

Name: UsbPumpGcdClient_SuspendDev

Function:
  Sample function demonstrates how to suspend the USB device

Definition:
  static VOID
  UsbPumpGcdClient_SuspendDev(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
    );

Description:
  This function suspends the USB device.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_SuspendDev(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
  )
  {
  TTUSB_OBJPRINTF((
    pGcdClient->pClassObject,
    UDMASK_FLOW | UDMASK_ANY,
    "--- Stop Loopback test for pipe %d and %d\n",
    pGcdClient->iCurrPipe,
    pGcdClient->iCurrPipe + 1
    ));

  if (pGcdClient->fPerformanceMeasure && pGcdClient->NumIterations == 0)
    {
    USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT * CONST pPipeCtx =
      &pGcdClient->PipeCtx[pGcdClient->iCurrPipe];
    UINT32  TimeTick;
    UINT32  Bytes;
    UINT32  BytesPerSecond;
    UINT  nLoopback;

    if (pGcdClient->EndTimeTick > pGcdClient->StartTimeTick)
      TimeTick = pGcdClient->EndTimeTick -
           pGcdClient->StartTimeTick;
    else
      TimeTick = pGcdClient->EndTimeTick +
           (0 - pGcdClient->StartTimeTick);

    if (pPipeCtx->bmAttributes == USB_bmAttr_INT &&
        pPipeCtx->pPipe->wMaxPacketSize < 512 &&
        pGcdClient->LoopbackCount > GCD_CLIENT_MAX_INT_COUNT
        )
      {
      nLoopback = GCD_CLIENT_MAX_INT_COUNT;
      }
    else
      {
      nLoopback = pGcdClient->LoopbackCount;
      }

    Bytes = pGcdClient->BufferSize * 2 * nLoopback;
    BytesPerSecond = Bytes / TimeTick *
         rtems_clock_get_ticks_per_second();

    TTUSB_OBJPRINTF((
      pGcdClient->pClassObject,
      UDMASK_ANY | UDMASK_FLOW,
      " BufferSize(%u) LoopbackCount(%u)\n"
      " WriteReadBytes(%u) TimeTicks(%u)",
      pGcdClient->BufferSize,
      nLoopback,
      Bytes,
      TimeTick
      ));

    if (BytesPerSecond > 1024)
      {
      BytesPerSecond /= 1024;
      TTUSB_OBJPRINTF((
        pGcdClient->pClassObject,
        UDMASK_ANY | UDMASK_FLOW,
        " -> %u KB/sec\n",
        BytesPerSecond
        ));
      }
    else
      {
      TTUSB_OBJPRINTF((
        pGcdClient->pClassObject,
        UDMASK_ANY | UDMASK_FLOW,
        " -> %u B/sec\n",
        BytesPerSecond
        ));
      }
    }

  pGcdClient->iCurrPipe += 2;
  if (pGcdClient->iCurrPipe >= pGcdClient->nPipes)
    {
    pGcdClient->iCurrPipe = 0;
    TTUSB_OBJPRINTF((
      pGcdClient->pClassObject,
      UDMASK_FLOW | UDMASK_ANY,
      "========== All Loopback test done ==========\n\n"
      ));
    }

  /* Ignore request handle because it can be NULL even if success */
  (*pGcdClient->FunctionInCall.GenDrv.pSuspendDeviceFn)(
    pGcdClient->FunctionHandle,
    UsbPumpGcdClient_SuspendDevCbFn,
    pGcdClient
    );
  }

static VOID
UsbPumpGcdClient_SuspendDevCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode
  )
        {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(pGcdClient);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_SuspendDevCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_SuspendDevCbFn: OK\n"
      ));
    }
  }

/*

Name: UsbPumpGcdClient_ResumeDev

Function:
  Sample function demonstrates how to resume the USB device

Definition:
  static VOID
  UsbPumpGcdClient_ResumeDev(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
    );

Description:
  This function resumes the USB device.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_ResumeDev(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
  )
  {
  /* Ignore request handle because it can be NULL even if success */
  (*pGcdClient->FunctionInCall.GenDrv.pResumeDeviceFn)(
    pGcdClient->FunctionHandle,
    UsbPumpGcdClient_ResumeDevCbFn,
    pGcdClient
    );
  }

static VOID
UsbPumpGcdClient_ResumeDevCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(pGcdClient);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_ResumeDevCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_ResumeDevCbFn: OK\n"
      ));
    }
  }

/*

Name: UsbPumpGcdClient_ResetPipe

Function:
  Sample function demonstrates how to reset a pipe

Definition:
  static VOID
  UsbPumpGcdClient_ResetPipe(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    USBPUMP_USBDI_PIPE_HANDLE hPipe,
    UINT32        ResetPipeFlags
    );

Description:
  This function resets a pipe.

Returns:
  None

*/

static VOID
UsbPumpGcdClient_ResetPipe(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  UINT32        ResetPipeFlags
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT * pPipeCtx;

  if (pGcdClient->NumIterations > 1)
    {
    --pGcdClient->NumIterations;
    UsbPumpGcdClient_StartLoopback(pGcdClient);
    return;
    }

  pGcdClient->EndTimeTick =
    rtems_clock_get_ticks_since_boot();

  pPipeCtx = &pGcdClient->PipeCtx[pGcdClient->iCurrPipe];
  pGcdClient->NumIterations = 0;
  if (pPipeCtx->bmAttributes == USB_bmAttr_ISO ||
      pPipeCtx->bmAttributes == USB_bmAttr_INT)
    {
    UsbPumpGcdClient_WriteCtrl(
      pGcdClient,
      pGcdClient->hDefaultPipe,
      /* bmReqType */ USB_bmRequestType_HSIFC,
      /* bRequest */ USB_bRequest_SET_INTERFACE,
      /* wValue */ 0,
      /* wIndex */ pPipeCtx->bInterfaceNumber,
      NULL,
      0,
      /* Timeout */ 500 /* ms */
      );
    }
  else
    {
    VOID *  pRequestHandle;

    pRequestHandle =
      (*pGcdClient->FunctionInCall.GenDrv.pResetPipeFn)(
        pGcdClient->FunctionHandle,
        UsbPumpGcdClient_ResetPipeCbFn,
        pGcdClient,
        hPipe,
        ResetPipeFlags
        );
    if (!pRequestHandle)
      {
      TTUSB_OBJPRINTF((pGcdClient->pClassObject,
        UDMASK_ERRORS,
        "?UsbPumpGcdClient_ResetPipe:"
        " ERROR - pRequestHandle is NULL!\n"
        ));
      }
    else
      {
      TTUSB_OBJPRINTF((pGcdClient->pClassObject,
        UDMASK_FLOW,
        " UsbPumpGcdClient_ResetPipe:"
        " pRequestHandle=%p, ResetPipeFlags=%x\n",
        pRequestHandle,
        ResetPipeFlags
        ));
      }
    }
  }

static VOID
UsbPumpGcdClient_ResetPipeCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  USBPUMP_USBDI_PIPE_HANDLE hPipe
  )
        {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(pGcdClient);
  USBPUMP_UNREFERENCED_PARAMETER(hPipe);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpGcdClient_ResetPipeCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    }
  else
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpGcdClient_ResetPipeCbFn: OK\n"
      ));

    /*
    || All loopback and reset_pipe are done.
    || Make the USB device suspend
    */
    UsbPumpGcdClient_SuspendDev(pGcdClient);
    }
  }

/*
|| Definition of Utility Functions
*/
static VOID
UsbPumpGcdClient_StartLoopbackTest(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  BOOL          fSetInterface
  )
  {
  pGcdClient->fSetInterface = fSetInterface;

  USBPUMP_CALLBACKCOMPLETION_INIT(
    &pGcdClient->Callback,
    UsbPumpGcdClient_StartLoopback,
    pGcdClient
    );

  UsbPumpPlatform_PostEvent(
    pGcdClient->pPlatform,
    &pGcdClient->Callback
    );
  }

static
VOID
UsbPumpGcdClient_StartLoopback(
  VOID *  pContext
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pContext;
  USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT * CONST pPipeCtx =
    &pGcdClient->PipeCtx[pGcdClient->iCurrPipe];

  if (pGcdClient->fSetInterface)
    {
    TTUSB_TRACE(
      USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT * CONST
        pPipe2Ctx = pPipeCtx + 1;
      )

    TTUSB_OBJPRINTF((
      pGcdClient->pClassObject,
      UDMASK_FLOW | UDMASK_ANY,
      "\n+++ Start Loopback Test for pipe %d and %d\n"
      " Interface %u AltSetting %u, %s %x <--> %s %x\n",
      pGcdClient->iCurrPipe,
      pGcdClient->iCurrPipe + 1,
      pPipeCtx->bInterfaceNumber,
      pPipeCtx->bAlternateSetting,
      sk_EndpointTypes[pPipeCtx->bmAttributes],
      pPipeCtx->bEndpointAddress,
      sk_EndpointTypes[pPipe2Ctx->bmAttributes],
      pPipe2Ctx->bEndpointAddress
      ));

    if (pPipeCtx->bmAttributes == USB_bmAttr_ISO &&
        ! USBPUMP_CLASS_GCD_CLIENT_SUPPORT_ISOCH)
      {
      TTUSB_OBJPRINTF((
        pGcdClient->pClassObject,
        UDMASK_FLOW | UDMASK_ANY,
        " Loopback test does not support isoch\n"
        ));
      UsbPumpGcdClient_SuspendDev(pGcdClient);
      return;
      }
    }

  if (pPipeCtx->pPipe->wMaxStreamID != 0)
    {
    /* Strart stream test FSM */
    UsbPumpStreamTestFsm_Init(pGcdClient);
    }
  else if (pGcdClient->fSetInterface)
    {
    /*
    || This is the sample code that send SetInterface control OUT
    || request to the control endpoint using WriteControlPipe
    || Function In-Call.
    */
    UsbPumpGcdClient_WriteCtrl(
      pGcdClient,
      pGcdClient->hDefaultPipe,
      /* bmReqType */ USB_bmRequestType_HSIFC,
      /* bRequest */ USB_bRequest_SET_INTERFACE,
      /* wValue */ pPipeCtx->bAlternateSetting,
      /* wIndex */ pPipeCtx->bInterfaceNumber,
      NULL,
      0,
      /* Timeout */ 500 /* ms */
      );
    }
  else if (pPipeCtx->bmAttributes == USB_bmAttr_ISO)
    {
    UINT  nIsochDesc;

    nIsochDesc = UsbPumpGcdClient_InitIsochPacketDesc(
        pGcdClient,
        pPipeCtx
        );

    if (! pGcdClient->fPerformanceMeasure)
      {
      UsbPumpGcdClient_GenerateTestData(
        pGcdClient->pWriteBuffer,
        pGcdClient->BufferSize
        );
      UsbPumpGcdClient_ClearTestData(
        pGcdClient->pReadBuffer,
        pGcdClient->BufferSize
        );
      }
    else if (pGcdClient->NumIterations == pGcdClient->LoopbackCount)
      {
      UsbPumpGcdClient_GenerateTestData(
        pGcdClient->pWriteBuffer,
        pGcdClient->BufferSize
        );
      TTUSB_FLUSH(pGcdClient->pPlatform);
      pGcdClient->StartTimeTick =
        rtems_clock_get_ticks_since_boot();
      }

    UsbPumpGcdClient_WriteIsoch(
      pGcdClient,
      pPipeCtx->pPipe->hPipe,
      pGcdClient->pWriteBuffer,
      nIsochDesc * pPipeCtx->pPipe->wMaxPacketSize,
      /* Timeout */ 5000 /* ms */,
      pGcdClient->IsochDesc,
      nIsochDesc * sizeof(USBPUMP_ISOCH_PACKET_DESCR),
      0 /* IsochStartFrame */,
      TRUE /* fAsap */
      );
    }
  else
    {
    USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT * CONST
      pPipe2Ctx = pPipeCtx + 1;

    if (! pGcdClient->fPerformanceMeasure)
      {
      UsbPumpGcdClient_GenerateTestData(
        pGcdClient->pWriteBuffer,
        pGcdClient->BufferSize
        );
      UsbPumpGcdClient_ClearTestData(
        pGcdClient->pReadBuffer,
        pGcdClient->BufferSize
        );
      }
    else if (pGcdClient->NumIterations == pGcdClient->LoopbackCount)
      {
      UsbPumpGcdClient_GenerateTestData(
        pGcdClient->pWriteBuffer,
        pGcdClient->BufferSize
        );
      TTUSB_FLUSH(pGcdClient->pPlatform);
      pGcdClient->StartTimeTick =
        rtems_clock_get_ticks_since_boot();
      }

    if (pPipeCtx->bmAttributes == USB_bmAttr_INT &&
        pPipeCtx->pPipe->wMaxPacketSize < 512 &&
        pGcdClient->NumIterations == pGcdClient->LoopbackCount &&
        pGcdClient->NumIterations > GCD_CLIENT_MAX_INT_COUNT
        )
      {
      pGcdClient->NumIterations = GCD_CLIENT_MAX_INT_COUNT;
      }

    /*
    || Write the generated test data to Bulk OUT endpoint of
    || Interface #1's AlternativeSetting #2
    */
    UsbPumpGcdClient_WriteBulkInt(
      pGcdClient,
      pPipeCtx->pPipe->hPipe,
      pGcdClient->pWriteBuffer,
      pGcdClient->BufferSize,
      /* Timeout */ 5000 /* ms */,
      FALSE /* fFullTransfer */
      );

    UsbPumpGcdClient_ReadBulkInt(
      pGcdClient,
      pPipe2Ctx->pPipe->hPipe,
      pGcdClient->pReadBuffer,
      pGcdClient->BufferSize,
      /* Timeout */ 5000 /* ms */
      );
    }
  }

static UINT
UsbPumpGcdClient_InitIsochPacketDesc(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient,
  USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT * pPipeCtx
  )
  {
  USBPUMP_ISOCH_PACKET_DESCR *  pIsochDesc;
  UINT  nIsochDesc;
  UINT  i;
  UINT16  wMaxPacketSize;

  wMaxPacketSize = pPipeCtx->pPipe->wMaxPacketSize;
  nIsochDesc = pGcdClient->BufferSize / wMaxPacketSize;
  if (nIsochDesc > USBPUMP_CLASS_GCD_CLIENT_MAX_ISOCHDESCS)
    nIsochDesc = USBPUMP_CLASS_GCD_CLIENT_MAX_ISOCHDESCS;

  pIsochDesc = pGcdClient->IsochDesc;
  for (i = 0; i < nIsochDesc; ++pIsochDesc, ++i)
    {
    pIsochDesc->uOffset = i * wMaxPacketSize;
    pIsochDesc->usLength = wMaxPacketSize;
    pIsochDesc->ucStatus = USTAT_BUSY;
    pIsochDesc->ucSpare = 0;
    }

  return nIsochDesc;
  }

static VOID
UsbPumpGcdClient_GenerateTestData(
  UINT8 * pBuffer,
  BYTES nBuffer
  )
  {
  BYTES i;
  for (i = 0; i < nBuffer; ++i)
    {
    pBuffer[i] = i % 100;
    }
  }

static VOID
UsbPumpGcdClient_ClearTestData(
  UINT8 * pBuffer,
  BYTES nBuffer
  )
  {
  UHIL_fill(pBuffer, nBuffer, 0);
  }

static BOOL
UsbPumpGcdClient_CheckTestData(
  UINT8 * pBuffer,
  BYTES nBuffer
  )
  {
  BYTES i;
  for (i = 0; i < nBuffer; ++i)
    {
    if (pBuffer[i] != i % 100)
      return FALSE;
    }
  return TRUE;
  }

static VOID
UsbPumpGcdClient_ParseConfigTree(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_CFG_NODE *  pConfigRoot
  )
  {
  UINT32    i;
  USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT * pPipeCtx;

  pPipeCtx = pGcdClient->PipeCtx;

  do
    {
    USBPUMP_USBDI_IFC_NODE * pIfcNode =
      USBPUMP_USBDI_CFG_NODE_IFC(
        pConfigRoot
        );
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW | UDMASK_ANY,
      " UsbPumpGcdClient_ParseConfigTree:"
      " bConfigurationValue:%x\n",
      pConfigRoot->bConfigurationValue
      ));
    do
      {
      USBPUMP_USBDI_ALTSET_NODE * pAltNode =
        USBPUMP_USBDI_IFC_NODE_ALTSET(pIfcNode);

      TTUSB_OBJPRINTF((pGcdClient->pClassObject,
        UDMASK_FLOW | UDMASK_ANY,
        "  pIfcNode:%p bInterfaceNumber:%x"
        " bNumAltSettings:%x\n",
        pIfcNode, pIfcNode->bInterfaceNumber,
        pIfcNode->bNumAltSettings
        ));
      do
        {
        USBPUMP_USBDI_PIPE_NODE * pPipeNode =
          USBPUMP_USBDI_ALTSET_NODE_PIPE(pAltNode);

        TTUSB_OBJPRINTF((pGcdClient->pClassObject,
          UDMASK_FLOW | UDMASK_ANY,
          "  pAltNode:%p bAlternateSetting:%x\n",
          pAltNode,
          pAltNode->bAlternateSetting
          ));

        for (i=0; i < pAltNode->bNumPipes; ++pPipeNode, ++i)
          {
          pPipeCtx->pPipe = pPipeNode;
          TTUSB_OBJPRINTF((pGcdClient->pClassObject,
            UDMASK_FLOW | UDMASK_ANY,
            "  pPipeNode:%p hPipe:%p wMaxPacketSize=%u\n"
            "   dwMaxTransferSize:%x wMaxStreams:%u\n",
            pPipeNode,
            pPipeNode->hPipe,
            pPipeNode->wMaxPacketSize,
            pPipeNode->dwMaxTransferSize,
            pPipeNode->wMaxStreamID
            ));

          pPipeCtx->bInterfaceNumber = pIfcNode->bInterfaceNumber;
          pPipeCtx->bAlternateSetting = pAltNode->bAlternateSetting;
          pPipeCtx->bEndpointAddress = pPipeNode->bEndpointAddress;
          pPipeCtx->bmAttributes =
            USBIF_EPDESC_bmAttributes_TRANSFER(pPipeNode->bmAttributes);
          ++pPipeCtx;

          if (pPipeNode->wMaxStreamID != 0)
            {
            pGcdClient->MaxStreamID =
                pPipeNode->wMaxStreamID;

            /* Save Out/In pipe index */
            if (pPipeNode->bEndpointAddress & 0x80)
              pGcdClient->InStreamPipe = pPipeNode->hPipe;
            else
              pGcdClient->OutStreamPipe = pPipeNode->hPipe;
            }
          else
            {
            pGcdClient->MaxStreamID = 0;
            }
          }
        } while ((pAltNode = USBPUMP_USBDI_ALTSET_NODE_NEXT(pAltNode)) != NULL);
      } while ((pIfcNode = USBPUMP_USBDI_IFC_NODE_NEXT(pIfcNode)) != NULL);
    } while ((pConfigRoot = USBPUMP_USBDI_CFG_NODE_NEXT(pConfigRoot)) != NULL);
  pGcdClient->nPipes = pPipeCtx - pGcdClient->PipeCtx;
  pGcdClient->iCurrPipe = 0;
  }

static VOID
UsbPumpStreamTestFsm_Init(
  USBPUMP_CLASS_GCD_CLIENT_DATA *   pGcdClient
  )
  {
  pGcdClient->sizeTestBuffer = 512;
  pGcdClient->CurrentStreamID = 1;

  UHIL_fill(&pGcdClient->StreamTestFsm, sizeof(pGcdClient->StreamTestFsm),0);

  pGcdClient->StreamTestFsm.CurrentState = USBPUMP_STREAMTEST_FSM_stInitial;
  pGcdClient->StreamTestFsm.fRun = TRUE;
  pGcdClient->StreamTestFsm.pFinalizeFn = UsbPumpStreamTestFsm_Finalized;

  /* start the FSM */
  UsbPumpStreamTestFsm_Eval(pGcdClient);
  }

/*

Name: UsbPumpStreamTestFsm_Eval

Function:
  FSM implementation for the Stream test FSM.

Definition:
  static VOID
  UsbPumpStreamTestFsm_Eval(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
    );

Description:
  This function runs the Stream test FSM.

Returns:
  None.

*/

static VOID
UsbPumpStreamTestFsm_Eval(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
  )
  {
  USBPUMP_STREAMTEST_FSM_STATE  nextState;
  BOOL        fEntry;

  nextState = USBPUMP_STREAMTEST_FSM_stNoChange;
  fEntry = FALSE;
  if (pGcdClient->StreamTestFsm.fBusy)
    {
    pGcdClient->StreamTestFsm.fDeferred = TRUE;
    return;
    }

  pGcdClient->StreamTestFsm.fBusy = TRUE;

  while (TRUE)
    {
    nextState = UsbPumpStreamTestFsm_Switch(
          pGcdClient,
          fEntry
          );

    if (nextState != USBPUMP_STREAMTEST_FSM_stNoChange)
      {
      TTUSB_OBJPRINTF((
        pGcdClient->pClassObject,
        UDMASK_FLOW,
        " UsbPumpStreamTestFsm_Eval(%p):"
        " change state: %s(%u) ==> %s(%u)\n",
        pGcdClient,
        UsbPumpStreamTestFsm_StateName(
          pGcdClient->StreamTestFsm.CurrentState),
        pGcdClient->StreamTestFsm.CurrentState,
        UsbPumpStreamTestFsm_StateName(nextState),
        nextState
        ));

      fEntry = TRUE;
      pGcdClient->StreamTestFsm.CurrentState = nextState;
      }
    else if (pGcdClient->StreamTestFsm.fDeferred)
      {
      fEntry = FALSE;
      pGcdClient->StreamTestFsm.fDeferred = FALSE;
      }
    else
      {
      break;
      }
    }

  pGcdClient->StreamTestFsm.fBusy = FALSE;
  }

/*

Name: UsbPumpStreamTestFsm_Switch

Function:
  Run a single step of the stream test FSM.

Definition:
  static USBPUMP_STREAMTEST_FSM_STATE
  UsbPumpStreamTestFsm_Switch(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    BOOL fEntry
    );

Description:
  This function contains the logic for a single evaluation of the
  FSM.  It is called with fNewState TRUE if and only if this evaluation
  represents a state change; the FSM will execute the "state entry"
  code for the current state if and only if fNewState is TRUE.

Returns:
  None.

*/

static USBPUMP_STREAMTEST_FSM_STATE
UsbPumpStreamTestFsm_Switch(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  BOOL fEntry
  )
  {
  USBPUMP_STREAMTEST_FSM_STATE newState;
  CONST USBPUMP_STREAMTEST_FSM_STATE currentState =
      pGcdClient->StreamTestFsm.CurrentState;

  newState = USBPUMP_STREAMTEST_FSM_stNoChange;

  switch (currentState)
    {
  case USBPUMP_STREAMTEST_FSM_stInitial:
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject, UDMASK_FLOW,
      " UsbPumpStreamTestFsm_Switch:"
      " CurrentStreamID(%d), TestBufferSize(%#x)\n",
      pGcdClient->CurrentStreamID,
      pGcdClient->sizeTestBuffer
      ));

    newState = USBPUMP_STREAMTEST_FSM_stSendData;
    }
    break;

  case USBPUMP_STREAMTEST_FSM_stSendData:
    {
    pGcdClient->pTestBuffer = UsbPumpPlatform_Malloc(
            pGcdClient->pPlatform,
            pGcdClient->sizeTestBuffer
            );
    if (!pGcdClient->pTestBuffer)
      {
      TTUSB_OBJPRINTF((pGcdClient->pClassObject, UDMASK_ERRORS,
        "?UsbPumpStreamTestFsm_Switch:"
        " Can't allocate test buffer(%d)\n",
        pGcdClient->sizeTestBuffer
        ));

      newState = USBPUMP_STREAMTEST_FSM_stError;
      }
    else
      {
      pGcdClient->StreamTestFsm.fSendCompleteOK = FALSE;
      pGcdClient->StreamTestFsm.fSendCompleteNG = FALSE;

      UsbPumpStreamTestFsm_WriteStream(
        pGcdClient,
        pGcdClient->OutStreamPipe,
        pGcdClient->pTestBuffer,
        pGcdClient->sizeTestBuffer,
        /* Timeout */ 5000 /* ms */,
        FALSE /* fFullTransfer */,
        pGcdClient->CurrentStreamID
        );

      newState = USBPUMP_STREAMTEST_FSM_stReceiveData;
      }
    }
    break;

  case USBPUMP_STREAMTEST_FSM_stReceiveData:
    if (fEntry)
      {
      pGcdClient->StreamTestFsm.fReceiveCompleteOK = FALSE;
      pGcdClient->StreamTestFsm.fReceiveCompleteNG = FALSE;

      UsbPumpStreamTestFsm_ReadStream(
        pGcdClient,
        pGcdClient->InStreamPipe,
        pGcdClient->pTestBuffer,
        pGcdClient->sizeTestBuffer,
        /* Timeout */ 5000 /* ms */,
        pGcdClient->CurrentStreamID
        );
      }

    if (pGcdClient->StreamTestFsm.fSendCompleteOK)
      {
      newState = USBPUMP_STREAMTEST_FSM_stSendDataDone;
      }
    else if (pGcdClient->StreamTestFsm.fSendCompleteNG)
      {
      newState = USBPUMP_STREAMTEST_FSM_stError;
      }
    break;

  case USBPUMP_STREAMTEST_FSM_stSendDataDone:
    {
    if (fEntry)
      {
      /* nothing */
      }
    if (pGcdClient->StreamTestFsm.fReceiveCompleteOK)
      {
      newState = USBPUMP_STREAMTEST_FSM_stDataArrived;
      }
    else if (pGcdClient->StreamTestFsm.fReceiveCompleteNG)
      {
      newState = USBPUMP_STREAMTEST_FSM_stError;
      }
    }
    break;

  case USBPUMP_STREAMTEST_FSM_stDataArrived:
    {
    if (fEntry)
      {
      /* Do nothing */
      }

    /* go back to the initial state by default */
    newState = USBPUMP_STREAMTEST_FSM_stInitial;

    /* Free buffer */
    UsbPumpPlatform_Free(
      pGcdClient->pPlatform,
      pGcdClient->pTestBuffer,
      pGcdClient->sizeTestBuffer
      );

    switch (pGcdClient->sizeTestBuffer)
      {
    case 512:
      pGcdClient->sizeTestBuffer = 1024;
      break;
    case 1024:
      pGcdClient->sizeTestBuffer = 2048;
      break;
    case 2048:
      pGcdClient->sizeTestBuffer = 64*1024;
      break;
    case 64*1024:
      {
      if (pGcdClient->CurrentStreamID >= pGcdClient->MaxStreamID)
        newState = USBPUMP_STREAMTEST_FSM_stDone;
      else
        {
        /* next stream ID */
        ++pGcdClient->CurrentStreamID;
        pGcdClient->sizeTestBuffer = 512;
        }
      }
      break;
    default:
      newState = USBPUMP_STREAMTEST_FSM_stError;
      break;
      }
    }
    break;

  case USBPUMP_STREAMTEST_FSM_stDone:
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject, UDMASK_FLOW,
      " UsbPumpStreamTestFsm_Switch:"
      " Test is done!\n"
      ));
    }
    break;

  case USBPUMP_STREAMTEST_FSM_stError:
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject, UDMASK_ERRORS,
      "?UsbPumpStreamTestFsm_Switch:"
      " Test is aborted due to the fatal error!\n"
      ));
    }
    break;

  case USBPUMP_STREAMTEST_FSM_st___MAX:
    {
    if (fEntry)
      {
      (*pGcdClient->StreamTestFsm.pFinalizeFn)(
        pGcdClient
        );
      }
    }
    break;

  default:
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject, UDMASK_ERRORS,
      "?UsbPumpStreamTestFsm_Switch:"
      " Enter default state!\n"
      ));

    newState = USBPUMP_STREAMTEST_FSM_stError;
    }
    break;
    }

  return newState;
  }

/*

Name: UsbPumpStreamTestFsm_WriteStream

Function:
  Sample function demonstrates how to write data to a specific bulk
  stream endpoint.

Definition:
  static VOID
  UsbPumpStreamTestFsm_WriteStream(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    USBPUMP_USBDI_PIPE_HANDLE hPipe,
    VOID *        pBuffer,
    BYTES       sizeBuffer,
    USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
    BOOL        fFullTransfer,
    UINT16        StreamID
    );

Description:
  This function write data to a specific bulk stream endpoint.

Returns:
  None

*/

static VOID
UsbPumpStreamTestFsm_WriteStream(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
  BOOL        fFullTransfer,
  UINT16        StreamID
  )
  {
  VOID *  pRequestHandle;

  /* Write test data with the predefined pattern */
  UsbPumpGcdClient_GenerateTestData(
      pBuffer,
      sizeBuffer
      );

  pRequestHandle =
    (*pGcdClient->FunctionInCall.GenDrv.pWriteStreamPipeFn)(
      pGcdClient->FunctionHandle,
      UsbPumpStreamTestFsm_WriteStreamCbFn,
      pGcdClient,
      hPipe,
      pBuffer,
      sizeBuffer,
      milliseconds,
      fFullTransfer,
      StreamID
      );
  if (!pRequestHandle)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpStreamTestFsm_WriteStream:"
      " ERROR - pRequestHandle is NULL!\n"
      ));
    return;
    }

  /*
  || This Function In-Call returns the pointer of the request handle
  || which is used to cancel the request.
  */
  TTUSB_OBJPRINTF((pGcdClient->pClassObject,
    UDMASK_FLOW,
    " UsbPumpStreamTestFsm_WriteStream:"
    " pRequestHandle=%p, %p, %p, %d, %d\n",
    pRequestHandle,
    hPipe,
    pBuffer,
    sizeBuffer,
    StreamID
    ));
  }

static VOID
UsbPumpStreamTestFsm_WriteStreamCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  VOID CONST *      pBuffer,
  BYTES       sizeBuffer,
  BYTES       nBytesWritten
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(pBuffer);
  USBPUMP_TRACE_PARAMETER(sizeBuffer);
  USBPUMP_TRACE_PARAMETER(nBytesWritten);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpStreamTestFsm_WriteStreamCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    pGcdClient->StreamTestFsm.fSendCompleteNG = TRUE;
    }
  else
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpStreamTestFsm_WriteStreamCbFn:"
      " pBuffer:%p, sizeBuffer:%d, nBytesWritten:%d\n",
      pBuffer, sizeBuffer, nBytesWritten
      ));

    pGcdClient->StreamTestFsm.fSendCompleteOK = TRUE;

    /* Trigger FSM */
    UsbPumpStreamTestFsm_Eval(pGcdClient);
    }
  }

/*

Name: UsbPumpStreamTestFsm_ReadStream

Function:
  Sample function demonstrates how to read data from a specific bulk
  stream endpoint.

Definition:
  static VOID
  UsbPumpStreamTestFsm_ReadStream(
    USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
    USBPUMP_USBDI_PIPE_HANDLE hPipe,
    VOID *        pBuffer,
    BYTES       sizeBuffer,
    USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
    UINT16        StreamID
    );

Description:
  This function reads data from a specific bulk stream endpoint.

Returns:
  None

*/

static VOID
UsbPumpStreamTestFsm_ReadStream(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient,
  USBPUMP_USBDI_PIPE_HANDLE hPipe,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  USBPUMP_USBDI_GENERIC_TIMEOUT milliseconds,
  UINT16        StreamID
  )
  {
  VOID *  pRequestHandle;

  pRequestHandle =
    (*pGcdClient->FunctionInCall.GenDrv.pReadStreamPipeFn)(
      pGcdClient->FunctionHandle,
      UsbPumpStreamTestFsm_ReadStreamCbFn,
      pGcdClient,
      hPipe,
      pBuffer,
      sizeBuffer,
      milliseconds,
      StreamID
      );
  if (!pRequestHandle)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpStreamTestFsm_ReadStream:"
      " ERROR - pRequestHandle is NULL!\n"
      ));
    return;
    }

  /*
  || This Function In-Call returns the pointer of the request handle
  || which is used to cancel the request.
  */
  TTUSB_OBJPRINTF((pGcdClient->pClassObject,
    UDMASK_FLOW,
    " UsbPumpStreamTestFsm_ReadStream:"
    " pRequestHandle=%p\n",
    pRequestHandle
    ));
  }

static VOID
UsbPumpStreamTestFsm_ReadStreamCbFn(
  VOID *        pCallbackCtx,
  VOID *        pRequestHandle,
  USBPUMP_USBDI_GENERIC_STATUS  ErrorCode,
  VOID *        pBuffer,
  BYTES       sizeBuffer,
  BYTES       nBytesWritten
  )
  {
  USBPUMP_CLASS_GCD_CLIENT_DATA * CONST pGcdClient = pCallbackCtx;

  USBPUMP_UNREFERENCED_PARAMETER(pRequestHandle);
  USBPUMP_TRACE_PARAMETER(sizeBuffer);

  if (ErrorCode != USBPUMP_USBDI_GENERIC_STATUS_OK)
    {
    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_ERRORS,
      "?UsbPumpStreamTestFsm_ReadStreamCbFn:"
      " ErrorCode:%s(%d)\n",
      UsbPumpUsbdiClassGeneric_StatusName(ErrorCode),
      ErrorCode
      ));
    pGcdClient->StreamTestFsm.fReceiveCompleteNG = TRUE;
    }
  else
    {
    BOOL fResult;

    fResult = UsbPumpGcdClient_CheckTestData(
        pBuffer,
        nBytesWritten
        );

    TTUSB_OBJPRINTF((pGcdClient->pClassObject,
      UDMASK_FLOW,
      " UsbPumpStreamTestFsm_ReadStreamCbFn:"
      " pBuffer:%p, sizeBuffer:%d, nBytesWritten:%d,"
      " check:%d\n",
      pBuffer, sizeBuffer, nBytesWritten, fResult
      ));

    if (fResult)
      {
      /* change async bit only when there is no data corruption*/
      pGcdClient->StreamTestFsm.fReceiveCompleteOK = TRUE;
      }
    }

  /* Trigger FSM */
  UsbPumpStreamTestFsm_Eval(pGcdClient);
  }

static VOID
UsbPumpStreamTestFsm_Finalized(
  USBPUMP_CLASS_GCD_CLIENT_DATA * pGcdClient
  )
  {
  pGcdClient->StreamTestFsm.fRun = FALSE;
  TTUSB_OBJPRINTF((pGcdClient->pClassObject,
    UDMASK_FLOW | UDMASK_ANY,
    "--- Stop Loopback test for pipe %d and %d\n",
    pGcdClient->iCurrPipe,
    pGcdClient->iCurrPipe + 1
    ));
  }

#if USBPUMP_TRACE
static CONST TEXT *
UsbPumpStreamTestFsm_StateName(
  USBPUMP_STREAMTEST_FSM_STATE  state
  )
  {
  if (state < LENOF(sk_vpStreamTestFsm_StateNames))
    {
    return sk_vpStreamTestFsm_StateNames[state];
    }
  else
    {
    return sk_StreamTestFsmUnknownState;
    }
  }
#endif

/**** end of hostapp_gcdclient_create.c ****/
