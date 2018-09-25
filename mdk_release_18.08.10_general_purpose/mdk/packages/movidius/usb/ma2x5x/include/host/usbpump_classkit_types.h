/* usbpump_classkit_types.h	Tue Jul 15 2008 15:46:40 chwon */

/*

Module:  usbpump_classkit_types.h

Function:
	Scalar types for CLASSKIT.

Version:
	V1.97k	Tue Jul 15 2008 15:46:40 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	April 2008

Revision history:
   1.97k  Thu Apr 03 2008 10:34:00 chwon
	Module created.

   1.97k  Tue Jul 15 2008 15:46:40  chwon
	4265: include usbpump_usbdi_types.h instead of usbpump_types.h

*/

#ifndef _USBPUMP_CLASSKIT_TYPES_H_	/* prevent multiple includes */
#define _USBPUMP_CLASSKIT_TYPES_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION	(USBPUMP_CLASSKIT_CLASS);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_CLASS_CONTENTS);

__TMS_TYPE_DEF_UNION	(USBPUMP_CLASSKIT_FUNCTION);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_FUNCTION_CONTENTS);

__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_SESSION);

__TMS_TYPE_DEF_ARG	(USBPUMP_CLASSKIT_STATUS, UINT32);
__TMS_TYPE_DEF_ARG	(USBPUMP_CLASSKIT_NOTIFICATION, UINT32);

/**** end of usbpump_classkit_types.h ****/
#endif /* _USBPUMP_CLASSKIT_TYPES_H_ */
