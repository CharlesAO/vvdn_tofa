/* usbpump_hostapp_gcdclient.h  Tue Apr 11 2017 10:13:32 chwon */

/*

Module:  usbpump_hostapp_gcdclient.h

Function:
  Definition of host GCD application

Version:
  V3.21a  Tue Apr 11 2017 10:13:32 chwon  Edit level 2

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
   3.19b  Mon Mar 20 2017 10:57:46 chwon
  20480: Module created from host/app/applib/i/client_gcddemo.h.

   3.21a  Tue Apr 11 2017 10:13:32  chwon
  20480: Add performance measure support.

*/

#ifndef _USBPUMP_HOSTAPP_GCDCLIENT_H_ /* prevent multiple includes */
#define _USBPUMP_HOSTAPP_GCDCLIENT_H_

#ifndef _USBPUMP_USBDI_H_
# include "usbpump_usbdi.h"
#endif

#ifndef _USBPUMP_USBD_CLASS_GENERIC_API_H_
# include "usbpump_usbd_class_generic_api.h"
#endif

#ifndef _USBPUMPROOT_H_
# include "usbpumproot.h"
#endif

#ifndef _UPLATFORMAPI_H_
# include "uplatformapi.h"
#endif

#ifndef _USBPUMPOBJECTAPI_H_
# include "usbpumpobjectapi.h"
#endif

#ifndef _USBIOCTL_FUNCTION_H_
# include "usbioctl_function.h"
#endif

#ifndef _USBPUMPDEBUG_H_
# include "usbpumpdebug.h"
#endif

#ifndef _USBIOCTL_API_H_
# include "usbioctl_api.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

/****************************************************************************\
|
| Stream test FSM data structure
|
\****************************************************************************/

__TMS_TYPE_DEF    (USBPUMP_STREAMTEST_FSM_STATE, __TMS_UINT);
__TMS_TYPE_DEF_STRUCT   (USBPUMP_STREAMTEST_FSM);
__TMS_TYPE_DEF_STRUCT   (USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT);
__TMS_TYPE_DEF_STRUCT   (USBPUMP_CLASS_GCD_CLIENT_DATA);

#define __TMS_USBPUMP_STREAMTEST_FSM_stNoChange   0
#define __TMS_USBPUMP_STREAMTEST_FSM_stInitial    1
#define __TMS_USBPUMP_STREAMTEST_FSM_stSendData   2
#define __TMS_USBPUMP_STREAMTEST_FSM_stReceiveData  3
#define __TMS_USBPUMP_STREAMTEST_FSM_stSendDataDone 4
#define __TMS_USBPUMP_STREAMTEST_FSM_stDataArrived  5
#define __TMS_USBPUMP_STREAMTEST_FSM_stDone   6
#define __TMS_USBPUMP_STREAMTEST_FSM_stError    7
#define __TMS_USBPUMP_STREAMTEST_FSM_st___MAX   8

#define __TMS_USBPUMP_STREAMTEST_FSM_STATES__INIT \
  "stNoChange",         \
  "stInitial",          \
  "stSendData",         \
  "stReceiveData",        \
  "SendDataDone",         \
  "stDataArrived",        \
  "stDone",         \
  "stError",          \
  "st___MAX"


__TMS_TYPE_DEF_FUNCTION(
USBPUMP_STREAMTEST_FSM_FINALIZE_FN,
  __TMS_VOID,
    (
    __TMS_USBPUMP_CLASS_GCD_CLIENT_DATA *
    ));

struct __TMS_STRUCTNAME (USBPUMP_STREAMTEST_FSM)
  {
  __TMS_USBPUMP_STREAMTEST_FSM_STATE  CurrentState;

  __TMS_UINT        fRun: 1;
  __TMS_UINT        fBusy: 1;
  __TMS_UINT        fDeferred: 1;

  __TMS_UINT        fSendCompleteOK: 1;
  __TMS_UINT        fSendCompleteNG: 1;
  __TMS_UINT        fReceiveCompleteOK: 1;
  __TMS_UINT        fReceiveCompleteNG: 1;

  __TMS_USBPUMP_STREAMTEST_FSM_FINALIZE_FN * pFinalizeFn;
  };


/****************************************************************************\
|
| GCD client private data structure
|
\****************************************************************************/

#ifndef __TMS_USBPUMP_CLASS_GCD_CLIENT_SUPPORT_ISOCH  /* PARAM */
# define __TMS_USBPUMP_CLASS_GCD_CLIENT_SUPPORT_ISOCH 1
#endif

#ifndef __TMS_USBPUMP_CLASS_GCD_CLIENT_MAX_PIPES  /* PARAM */
# define __TMS_USBPUMP_CLASS_GCD_CLIENT_MAX_PIPES 30
#endif

#ifndef __TMS_USBPUMP_CLASS_GCD_CLIENT_BUFFER_SIZE    /* PARAM */
# define __TMS_USBPUMP_CLASS_GCD_CLIENT_BUFFER_SIZE (24 * 1024)
#endif

#ifndef __TMS_USBPUMP_CLASS_GCD_CLIENT_MAX_ISOCHDESCS /* PARAM */
# define __TMS_USBPUMP_CLASS_GCD_CLIENT_MAX_ISOCHDESCS  128
#endif

struct __TMS_STRUCTNAME (USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT)
  {
  __TMS_USBPUMP_USBDI_PIPE_NODE *     pPipe;
  __TMS_UINT8         bInterfaceNumber;
  __TMS_UINT8         bAlternateSetting;
  __TMS_UINT8         bEndpointAddress;
  __TMS_UINT8         bmAttributes;
  };

struct __TMS_STRUCTNAME (USBPUMP_CLASS_GCD_CLIENT_DATA)
  {
  __TMS_USBPUMP_OBJECT_HEADER       ObjectHeader;
  __TMS_USBPUMP_OBJECT_HEADER *     pClassObject;
  __TMS_UPLATFORM *       pPlatform;

  /* Generic driver session handlers */
  __TMS_USBPUMP_SESSION_HANDLE      SessionHandle;
  __TMS_USBPUMP_SESSION_HANDLE      FunctionHandle;
  __TMS_USBPUMP_USBDI_FUNCTION *      pFunction;
  __TMS_USBPUMP_USBDI_GENERIC_DEVICE_STATE  DeviceState;

  __TMS_USBPUMP_USBDI_CLASS_GENERIC_INCALL  ClassInCall;
  __TMS_USBPUMP_USBDI_FUNCTION_GENERIC_INCALL FunctionInCall;

  __TMS_USBPUMP_USBDI_CFG_NODE *      pConfigRoot;
  __TMS_USBPUMP_USBDI_PIPE_HANDLE     hDefaultPipe;
  __TMS_USBPUMP_CLASS_GCD_CLIENT_PIPE_CONTEXT
      PipeCtx[__TMS_USBPUMP_CLASS_GCD_CLIENT_MAX_PIPES];
  __TMS_BYTES         nPipes;
  __TMS_UINT          iCurrPipe;

  __TMS_UINT8 *         pReadBuffer;
  __TMS_UINT8 *         pWriteBuffer;
  __TMS_UINT8         iManufacturer;
  __TMS_UINT8         iProduct;
  __TMS_UINT8         iSerialNumber;
  __TMS_USBPUMP_TIMER       Timer;
  __TMS_USBPUMP_ISOCH_PACKET_DESCR
    IsochDesc[__TMS_USBPUMP_CLASS_GCD_CLIENT_MAX_ISOCHDESCS];

  /* For Loopback and ResetPipe Test */
  __TMS_UINT          NumIterations;
  __TMS_UINT32          StartTimeTick;
  __TMS_UINT32          EndTimeTick;

  __TMS_UCALLBACKCOMPLETION     Callback;
  __TMS_BOOL          fSetInterface;

  __TMS_BOOL          fPerformanceMeasure;
  __TMS_BYTES         BufferSize;
  __TMS_UINT          LoopbackCount;

  /* For stream test */
  __TMS_USBPUMP_STREAMTEST_FSM      StreamTestFsm;
  __TMS_UINT16          MaxStreamID;
  __TMS_UINT16          CurrentStreamID;
  __TMS_UINT8 *         pTestBuffer;
  __TMS_BYTES         sizeTestBuffer;
  __TMS_USBPUMP_USBDI_PIPE_HANDLE     OutStreamPipe;
  __TMS_USBPUMP_USBDI_PIPE_HANDLE     InStreamPipe;
  };


/****************************************************************************\
|
| API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_CONST __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG
gk_UsbPumpUsbdiGeneric_ClassConfig;

__TMS_USBPUMP_CLASS_GCD_CLIENT_DATA *
UsbPumpHostApp_GcdClientCreate(
  __TMS_UPLATFORM * pPlatform,
  __TMS_UINT32    DebugFlags,
  __TMS_BYTES   BufferSize,
  __TMS_UINT    LoopbackCount,
  __TMS_BOOL    fPerformanceMeasure
  );

__TMS_END_DECLS


/****************************************************************************\
|
| Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_STREAMTEST_FSM_stNoChange  \
   __TMS_USBPUMP_STREAMTEST_FSM_stNoChange
# define USBPUMP_STREAMTEST_FSM_stInitial \
   __TMS_USBPUMP_STREAMTEST_FSM_stInitial
# define USBPUMP_STREAMTEST_FSM_stSendData  \
   __TMS_USBPUMP_STREAMTEST_FSM_stSendData
# define USBPUMP_STREAMTEST_FSM_stReceiveData \
   __TMS_USBPUMP_STREAMTEST_FSM_stReceiveData
# define USBPUMP_STREAMTEST_FSM_stSendDataDone  \
   __TMS_USBPUMP_STREAMTEST_FSM_stSendDataDone
# define USBPUMP_STREAMTEST_FSM_stDataArrived \
   __TMS_USBPUMP_STREAMTEST_FSM_stDataArrived
# define USBPUMP_STREAMTEST_FSM_stDone  \
   __TMS_USBPUMP_STREAMTEST_FSM_stDone
# define USBPUMP_STREAMTEST_FSM_stError \
   __TMS_USBPUMP_STREAMTEST_FSM_stError
# define USBPUMP_STREAMTEST_FSM_st___MAX  \
   __TMS_USBPUMP_STREAMTEST_FSM_st___MAX
# define USBPUMP_STREAMTEST_FSM_STATES__INIT  \
   __TMS_USBPUMP_STREAMTEST_FSM_STATES__INIT
# define USBPUMP_CLASS_GCD_CLIENT_SUPPORT_ISOCH \
   __TMS_USBPUMP_CLASS_GCD_CLIENT_SUPPORT_ISOCH
# define USBPUMP_CLASS_GCD_CLIENT_MAX_PIPES \
   __TMS_USBPUMP_CLASS_GCD_CLIENT_MAX_PIPES
# define USBPUMP_CLASS_GCD_CLIENT_BUFFER_SIZE \
   __TMS_USBPUMP_CLASS_GCD_CLIENT_BUFFER_SIZE
# define USBPUMP_CLASS_GCD_CLIENT_MAX_ISOCHDESCS  \
   __TMS_USBPUMP_CLASS_GCD_CLIENT_MAX_ISOCHDESCS
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_hostapp_gcdclient.h ****/
#endif /* _USBPUMP_HOSTAPP_GCDCLIENT_H_ */
