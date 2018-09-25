/* usbappinit_types.h	Sat Sep 27 2014 20:58:12 tmm */

/*

Module:  usbappinit_types.h

Function:
	The abstract type names used by usbappinit.h.

Version:
	V3.13b	Sat Sep 27 2014 20:58:12 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	September 2014

Revision history:
   3.13b  Sat Sep 27 2014 20:58:12  tmm
	Module created.

*/

#ifndef _USBAPPINIT_TYPES_H_		/* prevent multiple includes */
#define _USBAPPINIT_TYPES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	The "forward types" for app init.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USB_DATAPUMP_APPLICATION_INIT_VECTOR);
__TMS_TYPE_DEF_STRUCT(USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR);
__TMS_TYPE_DEF_STRUCT(USB_DATAPUMP_PORT_INIT_VECTOR);
__TMS_TYPE_DEF_STRUCT(USB_DATAPUMP_PORT_INIT_VECTOR_HDR);

/****************************************************************************\
|
|	Function types for application init vector
|
\****************************************************************************/

/*
|| The setup function is called so that the application can do some
|| "pre-setup", including prompting for other things to do.  It is optional.
|| The result is passed (unchanged) to all the probe and setup functions.
*/
__TMS_FNTYPE_DEF(
USB_DATAPUMP_APPLICATION_INIT_VECTOR_SETUP_FN,
__TMS_VOID *,
	(
	__TMS_UPLATFORM	*,
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *
	));

/*
|| The finish function is called so that the application can do some
|| "post-setup", including prompting for other things to do.  It is optional.
*/
__TMS_FNTYPE_DEF(
USB_DATAPUMP_APPLICATION_INIT_VECTOR_FINISH_FN,
__TMS_VOID,
	(
	__TMS_UPLATFORM	*,
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *,
	__TMS_VOID * /* pAppInitContext */,
	__TMS_UINT /* nPorts */
	));


/*
|| The application probe function, if present, is called prior to
|| initializing the device, so that it can decide whether or not to init.
*/
__TMS_FNTYPE_DEF(
USB_DATAPUMP_APPLICATION_PROBE_FN,
__TMS_BOOL, (
	__TMS_UINT			/* PortIndex */,
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_CONST struct TTUSB_UDEVICESWITCH *,
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR *,
	__TMS_VOID * /*pContext */
	));

__TMS_FNTYPE_DEF(
USB_DATAPUMP_APPLICATION_INIT_FN,
__TMS_BOOL, (
	__TMS_UDEVICE *			/* initialized device */,
	__TMS_UINT			/* PortIndex */,
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR *,
	__TMS_VOID *			/*pContext */
	));

/****************************************************************************\
|
|	Function types for device-port-init vector
|
\****************************************************************************/


/*
|| The vector setup function is called so that the dev port detection logic 
|| can do some "pre-setup", including prompting for other things to do.  
|| It is optional. The result is passed (unchanged) to all the dev port probe 
|| and setup functions. It's normally called after calling the appinit stuff,
|| so it also gets the app init vector context pointer.  If not defined,
|| the dev port init context pointer will be NULL.
*/
__TMS_FNTYPE_DEF(
USB_DATAPUMP_PORT_INIT_VECTOR_SETUP_FN,
__TMS_VOID *,
	(
	__TMS_UPLATFORM *,
	__TMS_CONST __TMS_USB_DATAPUMP_PORT_INIT_VECTOR_HDR *,
	__TMS_VOID * /* pAppInitContext */
	));

/*
|| The finish function is called so that the device port detection logic can 
|| do some "post-setup", including prompting for other things to do.  It is 
|| optional. It's called after detecting all the device ports, before calling
|| the app init
*/
__TMS_FNTYPE_DEF(
USB_DATAPUMP_PORT_INIT_VECTOR_FINISH_FN,
__TMS_VOID,
	(
	__TMS_UPLATFORM *,
	__TMS_CONST __TMS_USB_DATAPUMP_PORT_INIT_VECTOR_HDR *,
	__TMS_VOID * /* pAppInitContext */,
	__TMS_VOID * /* pPortInitContext */,
	__TMS_UINT /* nUsbPorts */
	));


/*
|| The probe function, if not NULL, is called to check whether
|| there's really a device port present.  If it's not, then it returns
|| FALSE.  Otherwise it returns TRUE, which causes the default
|| init process to continue setting up.
*/
__TMS_FNTYPE_DEF(
USB_DATAPUMP_PORT_INIT_VECTOR_PROBE_FN,
__TMS_BOOL,
	(
	__TMS_UPLATFORM *,
	__TMS_CONST __TMS_USB_DATAPUMP_PORT_INIT_VECTOR *,
	__TMS_VOID * /* pPortInitContext */
	));

/**** end of usbappinit_types.h ****/
#endif /* _USBAPPINIT_TYPES_H_ */
