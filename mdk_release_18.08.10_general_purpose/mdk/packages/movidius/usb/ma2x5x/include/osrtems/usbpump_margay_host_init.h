/* usbpump_margay_host_init.h	Fri May 26 2017 15:33:20 chwon */

/*

Module:  usbpump_margay_host_init.h

Function:
	Definition of host init vectors

Version:
	V3.21a	Fri May 26 2017 15:33:20 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	February 2017

Revision history:
   3.19b  Thu Feb 16 2017 15:33:11 chwon
	20480: Module created.

   3.19b  Fri Mar 03 2017 12:05:27  chwon
	20480: Add auto-mounter task information.

   3.21a  Fri May 26 2017 15:33:20  chwon
	20480: Move API prototypes to usbpump_application_rtems_host_api.h.

*/

#ifndef _USBPUMP_MARGAY_HOST_INIT_H_	/* prevent multiple includes */
#define _USBPUMP_MARGAY_HOST_INIT_H_

#ifndef _USBPUMP_USBDI_USBD_H_
# include "usbpump_usbdi_usbd.h"
#endif

#ifndef _USBPUMP_USBDI_TT_H_
# include "usbpump_usbdi_tt.h"
#endif

#ifndef _USBPUMP_USBDI_SUPERSPEED_H_
# include "usbpump_usbdi_superspeed.h"
#endif

/****************************************************************************\
|
|	Margay default host init vectors.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/*
|| Configuration for USBD
*/
extern
CONST
USBPUMP_USBDI_USBD_CONFIG
gk_UsbPumpMargay_Usbd_Config;

/*
|| Configuration for Transaction Translator
*/
extern
CONST
USBPUMP_USBDI_USBDTT_CONFIG
gk_UsbPumpMargay_UsbdTT_Config;

/*
|| Configuration for Super Speed
*/
extern
CONST
USBPUMP_USBDI_USBDSUPERSPEED_CONFIG
gk_UsbPumpMargay_UsbdSuperSpeed_Config;

__TMS_END_DECLS

/**** end of usbpump_margay_host_init.h ****/
#endif /* _USBPUMP_MARGAY_HOST_INIT_H_ */
