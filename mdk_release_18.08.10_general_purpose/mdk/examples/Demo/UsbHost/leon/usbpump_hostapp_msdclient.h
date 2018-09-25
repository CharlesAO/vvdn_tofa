/* usbpump_hostapp_msdclient.h  Tue Mar 07 2017 10:28:09 chwon */

/*

Module:  usbpump_hostapp_msdclient.h

Function:
  Definition of sample mass storage application

Version:
  V3.19b  Tue Mar 07 2017 10:28:09 chwon  Edit level 2

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
   3.19b  Fri Feb 10 2017 12:30:56 chwon
  20480: Module created.

   3.19b  Tue Mar 07 2017 10:28:09  chwon
  20480: Add performance test variables.

*/

#ifndef _USBPUMP_HOSTAPP_MSDCLIENT_H_ /* prevent multiple includes */
#define _USBPUMP_HOSTAPP_MSDCLIENT_H_

#ifndef _USBPUMP_HOST_INIT_H_
# include "usbpump_host_init.h"
#endif

#ifndef _USBPUMP_USBDI_CLASS_MSD_API_H_
# include "usbpump_usbdi_class_msd_api.h"
#endif

/****************************************************************************\
|
| MSD client types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT   (USBPUMP_CLASS_MSD_CLIENT);
__TMS_TYPE_DEF_STRUCT   (USBPUMP_CLASS_MSD_CLIENT_DEVICE);
__TMS_TYPE_DEF_STRUCT   (USBPUMP_CLASS_MSD_CLIENT_LUN);


/****************************************************************************\
|
| Definition of USBPUMP_CLASS_MSD_CLIENT structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME (USBPUMP_CLASS_MSD_CLIENT)
  {
  __TMS_USBPUMP_OBJECT_HEADER       ObjectHeader;

  __TMS_USBPUMP_OBJECT_HEADER *     pClassObject;
  __TMS_UPLATFORM *       pPlatform;

  /* mass storage driver session handle and incall */
  __TMS_USBPUMP_SESSION_HANDLE      SessionHandle;
  __TMS_USBPUMP_USBDI_CLASS_MSD_INCALL    InCall;

  __TMS_BYTES         DataBufferSize;

  /* mass storage function information */
  __TMS_USBPUMP_CLASS_MSD_CLIENT_DEVICE *   pMsdDeviceHead;
  };

struct __TMS_STRUCTNAME (USBPUMP_CLASS_MSD_CLIENT_DEVICE)
  {
  __TMS_USBPUMP_CLASS_MSD_CLIENT *    pMsdClient;

  __TMS_USBPUMP_USBDI_FUNCTION *      pFunction;
  __TMS_USBPUMP_SESSION_HANDLE      SessionHandle;
  __TMS_USBPUMP_USBDI_FUNCTION_MSD_INCALL   InCall;

  __TMS_UINT          nLun;
  __TMS_USBPUMP_CLASS_MSD_CLIENT_LUN *    pLun;

  __TMS_USBPUMP_CLASS_MSD_CLIENT_DEVICE *   pNext;
  __TMS_USBPUMP_CLASS_MSD_CLIENT_DEVICE *   pLast;
  };

struct __TMS_STRUCTNAME (USBPUMP_CLASS_MSD_CLIENT_LUN)
  {
  __TMS_USBPUMP_CLASS_MSD_CLIENT_DEVICE *   pMsdDevice;

  __TMS_UINT          Lun;
  __TMS_USBPUMP_USBDI_MSD_LUN_INFO    LunInfo;
  __TMS_USBPUMP_USBDI_MSD_MEDIA_INFO    MediaInfo;

  __TMS_UINT32          StartLBA;
  __TMS_UINT32          NumberOfLBA;

  __TMS_USBPUMP_USBDI_CLASS_MSD_REQUEST   Request;

  __TMS_USBPUMP_BUFFER_HANDLE     hBuffer;
  __TMS_VOID *          pBuffer;
  __TMS_BYTES         nBuffer;

  __TMS_UINT          TestCount;
  __TMS_UINT32          ReadBlocks;
  __TMS_UINT32          StartTimeTick;
  };


/****************************************************************************\
|
| API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern
__TMS_CONST
__TMS_USBPUMP_HOST_INIT_NODE_VECTOR
gk_UsbPumpHost_GenericInitVector;

__TMS_USBPUMP_CLASS_MSD_CLIENT *
UsbPumpHostApp_MsdClientCreate(
  __TMS_UPLATFORM *   /* pPlatform */,
  __TMS_UINT32    /* DebugFlags */,
  __TMS_BYTES   /* DataBufferSize */
  );

__TMS_END_DECLS

/**** end of usbpump_hostapp_msdclient.h ****/
#endif /* _USBPUMP_HOSTAPP_MSDCLIENT_H_ */
