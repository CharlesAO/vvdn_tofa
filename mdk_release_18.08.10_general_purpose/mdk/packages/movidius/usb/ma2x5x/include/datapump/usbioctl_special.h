/* usbioctl_special.h	Fri Dec  6 2002 10:14:10 chwon */

/*

Module:  usbioctl_special.h

Function:
	IOCTLs for doing special customer request

Version:
	V1.79b	Fri Dec  6 2002 10:14:10 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2002 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	December 2002

Revision history:
   1.79b  Tue Dec  3 2002 15:12:10  chwon
	Module created.

   1.79b  12/6/2002  chwon
	Use __TMS_CONCAT() instead of ##. This is more portable.

*/

#ifndef _USBIOCTL_SPECIAL_H_		/* prevent multiple includes */
#define _USBIOCTL_SPECIAL_H_

#include <stdarg.h>

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _UENDPOINT_H_
# include "uendpoint.h"
#endif

/****************************************************************************\
|
|	The IOCTL codes
|
\****************************************************************************/

/* the macro for defining entry in this class */
#define __TMS_USBPUMP_IOCTL_SPECIAL(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_SPECIAL,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_, NAME, _ARG)		\
		)

/* the device-based flow control */
#define __TMS_USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL			\
	__TMS_USBPUMP_IOCTL_SPECIAL(0, W, DEVICE_SPECIAL_FLOWCTL)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL	__TMS_USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL

Index:	Type:	USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL_ARG
	Name:	USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL_ARG_SETUP_V1()

Function:
	Send a flow control request to the UDEVICE.

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL_ARG structure
			(normally NULL)

	USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL_ARG has the following elements:

	__TMS_UENDPOINT	*pUep;
		IN:  The endpoint pointer

	__TMS_BOOL	bStop;
		IN:  The flow control on/off

Description:
	This IOCTL, if directed to a UDEVICE, or object layered
	upon the same, does a flow control of endpoint.

Returns:
	USBPUMP_RESULT_SUCCESS:  pDevice is set to the UDEVICE pointer.
	USBPUMP_RESULT_NOT_CLAIMED:  the object you sent the IOCTL to is
				not layered on a UDEVICE.

	Other failure codes are possible if the request is malformed.

*/

/* 
|| USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL does a flow control of endpoint.
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL_ARG)
	{
	__TMS_UENDPOINT	*pUep;		/* IN: endpoint pointer */
	__TMS_BOOL	bStop;		/* IN: on/off flow control */
	};

/* the setup for the .._DEVICE_SPECIAL_FLOWCTL_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pUep,							\
	ARG_bStop							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pUep = (ARG_pUep);					\
	__pArg->bStop = (ARG_bStop);					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pUep,							\
	ARG_bStop							\
	)								\
   __TMS_USBPUMP_IOCTL_DEVICE_SPECIAL_FLOWCTL_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pUep,							\
	ARG_bStop							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_special.h ****/
#endif /* _USBIOCTL_SPECIAL_H_ */
