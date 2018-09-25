/* hostapp_msdclient_api.c  Fri May 26 2017 12:04:48 chwon */

/*

Module:  hostapp_msdclient_api.c

Function:
  Home for UsbPump_Rtems_HostApp_MsdClientCreate()

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

#include "usbpump_hostapp_msdclient.h"
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

__TMS_TYPE_DEF_STRUCT (USBPUMP_HOSTAPP_MSD_SYNC_CONTEXT);
struct __TMS_STRUCTNAME (USBPUMP_HOSTAPP_MSD_SYNC_CONTEXT)
  {
  UPLATFORM *   pPlatform;
  UINT32      DebugFlags;
  BYTES     MsdClientBufferSize;
  UCALLBACKCOMPLETION Callback;
  };


static
UCALLBACKFN
UsbPumpHostApp_MsdCreateCallback;


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

Name: UsbPump_Rtems_HostApp_MsdClientCreate

Function:
  Create MSD driver client

Definition:
  unsigned int
  UsbPump_Rtems_HostApp_MsdClientCreate(
    void *    pPlatform,
    unsigned int  DebugFlags,
    unsigned int  MsdClientBufferSize
    );

Description:
  This function creates host MSD driver client for demo purpose.

Returns:
  TRUE if success, otherwise FALSE.

*/

#define FUNCTION "UsbPump_Rtems_HostApp_MsdClientCreate"

unsigned int
UsbPump_Rtems_HostApp_MsdClientCreate(
  void *    pPlatform_Void,
  unsigned int  DebugFlags,
  unsigned int  MsdClientBufferSize
  )
  {
  UPLATFORM * CONST pPlatform = pPlatform_Void;
  USBPUMP_HOSTAPP_MSD_SYNC_CONTEXT *  pSyncContext;

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
  pSyncContext->MsdClientBufferSize = MsdClientBufferSize;

  USBPUMP_CALLBACKCOMPLETION_INIT(
    &pSyncContext->Callback,
    UsbPumpHostApp_MsdCreateCallback,
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
UsbPumpHostApp_MsdCreateCallback(
  void *  pContext
  )
  {
  USBPUMP_HOSTAPP_MSD_SYNC_CONTEXT * CONST pSyncContext = pContext;

  /*
  || Create MSD client object
  */
  UsbPumpHostApp_MsdClientCreate(
    pSyncContext->pPlatform,
    pSyncContext->DebugFlags,
    pSyncContext->MsdClientBufferSize
    );

  free(pSyncContext);
  }

/**** end of hostapp_msdclient_api.c ****/
