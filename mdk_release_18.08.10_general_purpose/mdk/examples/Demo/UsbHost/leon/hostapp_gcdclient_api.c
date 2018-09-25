/* hostapp_gcdclient_api.c  Fri May 26 2017 12:04:48 chwon */

/*

Module:  hostapp_gcdclient_api.c

Function:
  Home for UsbPump_Rtems_HostApp_GcdClientCreate()

Version:
  V3.21a  Fri May 26 2017 12:04:48 chwon  Edit level 1

Copyright notice:
  This file copyright (C) 2017 by

    MCCI Corporation
    3520 Krums Corners Road
    Ithaca, NY  14850

  An unpublished work.  All rights reserved.

  This file is proprietary information, and may not be disclosed or
  copied without the prior permission of MCCI Corporation

  ChaeHee Won, MCCI Corporation May 2017

Revision history:
   3.21a  Fri May 26 2017 12:04:48  chwon
  20480: Module created.

*/

#include "usbpumpdebug.h"
#include "usbpumproot.h"
#include "usbpumpobjectapi.h"
#include "uplatform.h"
#include "uplatformapi.h"
#include "ucallback.h"

#include "usbpump_hostapp_gcdclient.h"
#include "usbpump_application_rtems_host_api.h"

#include <stdio.h>
#include <malloc.h>

/****************************************************************************\
|
|   Manifest constants & typedefs.
|
| This is strictly for private types and constants which will not
| be exported.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT (USBPUMP_HOSTAPP_GCD_SYNC_CONTEXT);
struct __TMS_STRUCTNAME (USBPUMP_HOSTAPP_GCD_SYNC_CONTEXT)
  {
  UPLATFORM *   pPlatform;
  UINT32      DebugFlags;
  BYTES     GcdClientBufferSize;
  UINT      GcdClientLoopbackCount;
  BOOL      fGcdClientPerformanceMeasure;
  UCALLBACKCOMPLETION Callback;
  };


static
UCALLBACKFN
UsbPumpHostApp_GcdCreateCallback;


/****************************************************************************\
|
| Read-only data.
|
| If program is to be ROM-able, these must all be tagged read-only
| using the ROM storage class; they may be global.
|
\****************************************************************************/


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

Name: UsbPump_Rtems_HostApp_GcdClientCreate

Function:
  Create GCD driver client

Definition:
  unsigned int
  UsbPump_Rtems_HostApp_GcdClientCreate(
    void *    pPlatform,
    unsigned int  DebugFlags,
    unsigned int  GcdClientBufferSize,
    unsigned int  GcdClientLoopbackCount,
    unsigned int  fGcdClientPerformanceMeasure
    );

Description:
  This function creates host GCD driver client for demo purpose.

Returns:
  TRUE if success, otherwise FALSE.

*/

#define FUNCTION "UsbPump_Rtems_HostApp_GcdClientCreate"

unsigned int
UsbPump_Rtems_HostApp_GcdClientCreate(
  void *    pPlatform_Void,
  unsigned int  DebugFlags,
  unsigned int  GcdClientBufferSize,
  unsigned int  GcdClientLoopbackCount,
  unsigned int  fGcdClientPerformanceMeasure
  )
  {
  UPLATFORM * const pPlatform = pPlatform_Void;
  USBPUMP_HOSTAPP_GCD_SYNC_CONTEXT *  pSyncContext;

  if (pPlatform == NULL)
    {
    printf("?" FUNCTION ": pPlatform=NULL\n");
    return FALSE;
    }

  pSyncContext = malloc(sizeof(*pSyncContext));
  if (pSyncContext == NULL)
    {
    printf("?" FUNCTION ": No memory\n");
    return FALSE;
    }

  pSyncContext->pPlatform = pPlatform;
  pSyncContext->DebugFlags = DebugFlags;
  pSyncContext->GcdClientBufferSize = GcdClientBufferSize;
  pSyncContext->GcdClientLoopbackCount = GcdClientLoopbackCount;
  pSyncContext->fGcdClientPerformanceMeasure =
    fGcdClientPerformanceMeasure;

  USBPUMP_CALLBACKCOMPLETION_INIT(
    &pSyncContext->Callback,
    UsbPumpHostApp_GcdCreateCallback,
    pSyncContext
    );

  UsbPumpPlatform_PostEvent(
    pPlatform,
    &pSyncContext->Callback
    );

  return TRUE;
  }

#undef FUNCTION

static void
UsbPumpHostApp_GcdCreateCallback(
  void *  pContext
  )
  {
  USBPUMP_HOSTAPP_GCD_SYNC_CONTEXT * const pSyncContext = pContext;

  /*
  || Create Generic client object
  */
  UsbPumpHostApp_GcdClientCreate(
    pSyncContext->pPlatform,
    pSyncContext->DebugFlags,
    pSyncContext->GcdClientBufferSize,
    pSyncContext->GcdClientLoopbackCount,
    pSyncContext->fGcdClientPerformanceMeasure
    );

  free(pSyncContext);
  }

/**** end of hostapp_gcdclient_api.c ****/
