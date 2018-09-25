/* usbpump_usbd_class_uvc_params.h	Thu Mar 16 2017 16:55:51 chwon */

/*

Module:  usbpump_usbd_class_uvc_params.h

Function:
	Definition of the USBD Uvc driver parameters

Version:
	V3.19b	Thu Mar 16 2017 16:55:51 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2017

Revision history:
   3.19b  Thu Mar 16 2017 16:55:51  chwon
	20517: Module created.

*/

#ifndef _USBPUMP_USBD_CLASS_UVC_PARAMS_H_	/* prevent multiple includes */
#define _USBPUMP_USBD_CLASS_UVC_PARAMS_H_

/*

Macro:	USBPUMP_USBD_CLASS_UVC_PARAMS_PORT_INCLUDE

Function:
	Compile time override of DataPump FreeRTOS parameter macros.

Definition:
	CFLAGS_PORT += -DUSBPUMP_USBD_CLASS_UVC_PARAMS_PORT_INCLUDE=my_file.h

Description:
	The DataPump allow the platform build system to inject a header file
	to override the default definitions of DataPump FreeRTOS parameters.

	This header file will be included after #including "usbpumpenv.h"
	(and possibly after including other DataPump header files, depending 
	on the module).  

Notes:
	To avoid quoting issues in the build system the file name is
	given directly (without quotes). "usbpumpdebug.h" supplies
	the leading and trailing quotes. However, this means that you
	can't use an angle-bracketed include file.

*/

#ifdef USBPUMP_USBD_CLASS_UVC_PARAMS_PORT_INCLUDE

# if ! (__TMS_LIBPORT_VERSION > __TMS_LIBPORT_VERSION_CALC(3,11,2,0))
#  error USBPUMP_USBD_CLASS_UVC_PARAMS_PORT_INCLUDE needs a version of libport that defines __TMS_STRINGVAL.
# endif

/* __TMS_STRINGVAL expands the argument before stringifying */
# include __TMS_STRINGVAL(USBPUMP_USBD_CLASS_UVC_PARAMS_PORT_INCLUDE)
#endif

/****************************************************************************\
|
|	USBD UVC class driver parameters
|
\****************************************************************************/

#ifndef	USBPUMP_USBDI_CLASS_UVC_PARAM_MAX_INSTANCE		/* PARAM */
# define USBPUMP_USBDI_CLASS_UVC_PARAM_MAX_INSTANCE		8
#endif

#ifndef	USBPUMP_USBDI_CLASS_UVC_PARAM_CONFIG_MAX_SESSION	/* PARAM */
# define USBPUMP_USBDI_CLASS_UVC_PARAM_CONFIG_MAX_SESSION	\
	(USBPUMP_USBDI_CLASS_UVC_PARAM_MAX_INSTANCE + 1)
#endif

#ifndef	USBPUMP_USBDI_CLASS_UVC_PARAM_CONTROL_BUFFER_SIZE	/*PARAM*/
# define USBPUMP_USBDI_CLASS_UVC_PARAM_CONTROL_BUFFER_SIZE	64
#endif

#define	USBPUMP_USBDI_CLASS_UVC_MIN_CONTROL_BUFFER_SIZE		48
#if USBPUMP_USBDI_CLASS_UVC_PARAM_CONTROL_BUFFER_SIZE < USBPUMP_USBDI_CLASS_UVC_MIN_CONTROL_BUFFER_SIZE
# error USBPUMP_USBDI_CLASS_UVC_PARAM_CONTROL_BUFFER_SIZE is too small.
#endif

/**** end of usbpump_usbd_class_uvc_params.h ****/
#endif /* _USBPUMP_USBD_CLASS_UVC_PARAMS_H_ */
