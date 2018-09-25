/* hostapp_hostinit.c Tue Apr 11 2017 10:14:17 chwon */

/*

Module:  hostapp_hostinit.c

Function:
  Home for application INIT_NODEs for USBD, and Class drivers

Version:
  V3.21a  Tue Apr 11 2017 10:14:17 chwon  Edit level 2

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
   3.19b  Mon Mar 20 2017 10:35:20  chwon
  20480: Module created.

   3.21a  Tue Apr 11 2017 10:14:17  chwon
  20480: Add performance measure support.

*/

#include "usbpumpdebug.h"
#include "usbpumproot.h"
#include "usbpumpobjectapi.h"
#include "usbpump_host_init.h"
#include "usbpump_usbdi.h"
#include "usbpump_usbdi_tt.h"
#include "usbpump_usbdtti_api.h"

#include "usbpump_usbdi_superspeed.h"
#include "usbpump_usbdsuperspeedi_api.h"

#include "usbpump_usbd_class_composite_api.h"

#include "usbpump_usbd_class_generic_api.h"

#include "usbpump_usbdi_class_msd_api.h"
#include "usbpump_usbdi_class_msd_config.h"

#include "usbpump_margay_host_init.h"

/****************************************************************************\
|
|   Manifest constants & typedefs.
|
| This is strictly for private types and constants which will not
| be exported.
|
\****************************************************************************/


/****************************************************************************\
|
| Read-only data.
|
| If program is to be ROM-able, these must all be tagged read-only
| using the ROM storage class; they may be global.
|
\****************************************************************************/

/*
|| Configuration for composite device class driver
*/
static
CONST
USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG
sk_UsbPumpUsbdCD_Config =
  USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_INIT_V1(
    &gk_UsbPumpUsbdiClassComposite_InitMatchList,
    /* driver class name */ NULL, /* use implementation default */
    /* function instance name */ NULL, /* use default */
    /* number of instances */ 8,
    /* number of Composite Functions Total */ 64,
    /* number of Composite Sub Function per Function */ 32,
    /* number of Pipes */ 16,
    /* size of Configuration Desc */ 4096
    );

/*
|| Configuration for generic device class driver
*/
static
CONST USBPUMP_USBDI_CLASS_GENERIC_CONFIG
sk_UsbPumpUsbdGeneric_PrivateConfig =
  USBPUMP_USBDI_CLASS_GENERIC_CONFIG_INIT_V1(
    /* number of client sessions */ 4,
    /* number of configurations */ 1,
    /* number of interfaces */ 5,
    /* number of alternative settings */ 15,
    /* number of pipes */ 30,
    /* number of requests */ 4,
    /* maximum size of configuration bundle */ 512
    );

static CONST USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY
sk_vUsbPumpUsbdiGeneric_Matches[] =
  {
  USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY_INIT_V1(
    "vid=040e/f60f;*",  /* catena1610 loopback application */
    USBPUMP_USBDI_PRIORITY_VIDPID
    ),
  USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY_INIT_V1(
    "vid=040e/f602;*",  /* catena1650/xdci loopback application */
    USBPUMP_USBDI_PRIORITY_VIDPID
    )
  };

CONST USBPUMP_USBDI_INIT_MATCH_LIST
gk_UsbPumpUsbdiGeneric_InitMatchList =
  USBPUMP_USBDI_INIT_MATCH_LIST_INIT_V1(
    sk_vUsbPumpUsbdiGeneric_Matches
    );

CONST USBPUMP_USBDI_DRIVER_CLASS_CONFIG
gk_UsbPumpUsbdiGeneric_ClassConfig =
  USBPUMP_USBDI_DRIVER_CLASS_CONFIG_INIT_V1(
    &gk_UsbPumpUsbdiGeneric_InitMatchList,
    /* driver class name */ USBPUMP_USBDI_CLASS_GENERIC_NAME,
    /* function instance name */ USBPUMP_USBDI_FUNCTION_GENERIC_NAME,
    /* number of instances */ 2
    );

/*
|| This table provides the initialization information for the class drivers
*/
static
CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE
sk_ClassDriverInitNodes[] =
  {
  USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_INIT_V1(
    /* pProbeFn */ NULL,
    /* pInitFn */ UsbPumpUsbdiClassComposite_Initialize,
    /* pConfig */ &sk_UsbPumpUsbdCD_Config.ClassConfig,
    /* pPrivateConfig */ &sk_UsbPumpUsbdCD_Config.ClassPrivateConfig,
    /* DebugFlags */ 0
    ),
  USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_INIT_V1(
    /* pProbeFn */ NULL,
    /* pInitFn */ UsbPumpUsbdiClassMsd_Initialize,
    /* pConfig */ &gk_UsbPumpUsbdiMsd_ClassConfigDefault,
    /* pPrivateConfig */ &gk_UsbPumpUsbdiClassMsd_ConfigDefault,
    /* DebugFlags */ 0
    ),
  USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_INIT_V1(
    /* pProbeFn */ NULL,
    /* pInitFn */ UsbPumpUsbdiClassGeneric_Initialize,
    /* pConfig */ &gk_UsbPumpUsbdiGeneric_ClassConfig,
    /* pPrivateConfig */ &sk_UsbPumpUsbdGeneric_PrivateConfig,
    /* DebugFlags */ 0
    )
  };

static
CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR
sk_UsbPumpClassDriverInitHeader =
  USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_INIT_V1(
    /* name of the vector */ sk_ClassDriverInitNodes,
    /* prefunction */ NULL,
    /* postfunction */ NULL
    );

static
CONST USBPUMP_HOST_INIT_NODE
sk_UsbPumpHostInitNodes[] =
  {
  USBPUMP_HOST_INIT_NODE_INIT_V3(
    /* pProbeFn */ NULL,
    /* pUsbdInitFn */ UsbPumpUsbd_Initialize,
    /* pUsbdConfig */ &gk_UsbPumpMargay_Usbd_Config,
    /* pUsbdImplementation */
        &gk_UsbPumpUsbdiUsbdImplementation_Isoch,
    /* pUsbdTTInitFn */ UsbPumpUsbdTT_Initialize,
    /* pUsbdTTConfig */ &gk_UsbPumpMargay_UsbdTT_Config,
    /* pWirelessUsbdInitFn */ NULL,
    /* pWirelessUsbdConfig */ NULL,
    /* pSuperSpeedInitFn */ UsbPumpUsbdSuperSpeed_Initialize,
    /* pSuperSpeedConfig */ &gk_UsbPumpMargay_UsbdSuperSpeed_Config,
    /* DebugFlags*/ 0,
    /* fDoNotStartUsbd */ FALSE,
    /* pHcdInitVector */ &gk_UsbPumpHcd_GenericInitVector,
    /* pDriverClassInitVector */ &sk_UsbPumpClassDriverInitHeader
    )
  };

CONST USBPUMP_HOST_INIT_NODE_VECTOR
gk_UsbPumpHost_GenericInitVector =
  USBPUMP_HOST_INIT_NODE_VECTOR_INIT_V1(
    /* name of the vector */ sk_UsbPumpHostInitNodes,
    /* prefunction */ NULL,
    /* postfunction */ NULL
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

/**** end of hostapp_hostinit.c ****/
