/* usbpump_usbd_class_composite_api.h	Wed Mar 19 2008 12:40:23 chwon */

/*

Module:  usbpump_usbd_class_composite_api.h

Function:
	Definition of API Composite Device functions for USBDI clients.

Version:
	V1.97k	Wed Mar 19 2008 12:40:23 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2007-2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Debbie Totsline, MCCI Corporation	December 2007

Revision history:
   1.97k  Thu Dec 13 2007 17:40:25 djt
	3049: Module created.

   1.97k  Mon Mar 10 2008 13:39:11  djt
	3049: Changed name of file
	Added usbpump_usbdi_cd.h.

   1.97k  Wed Mar 19 2008 12:40:23 chwon
	3049: change usbpump_usbdi_cd.h to usbpump_usbd_class_composite_config.h.
	Change composite driver API name.

*/

#ifndef _USBPUMP_USBD_CLASS_COMPOSITE_API_H_	/* prevent multiple includes */
#define _USBPUMP_USBD_CLASS_COMPOSITE_API_H_

#ifndef _USBPUMP_USBDI_COMPOSITE_H_
# include "usbpump_usbdi_composite.h"
#endif

#ifndef _USBPUMP_USBD_CLASS_COMPOSITE_CONFIG_H_
# include "usbpump_usbd_class_composite_config.h"
#endif

#ifndef _USBPUMP_USBDI_INIT_H_
# include "usbpump_usbdi_init.h"
#endif

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_USBDI_DRIVER_CLASS_INIT_FN
UsbPumpUsbdiClassComposite_Initialize;

__TMS_END_DECLS


/**** end of usbpump_usbd_class_composite_api.h ****/
#endif /* _USBPUMP_USBD_CLASS_COMPOSITE_API_H_ */
