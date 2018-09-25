/* usbioctl_debug.h	Sun Nov 29 2015 01:25:17 tmm */

/*

Module:  usbioctl_debug.h

Function:
	IOCTLs for doing USB debugging prints

Version:
	V3.15d	Sun Nov 29 2015 01:25:17 tmm	Edit level 8

Copyright notice:
	This file copyright (C) 2002-2005, 2008-2009, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	August 2002

Revision history:
   1.79b  Fri Aug  9 2002 05:17:32  tmm
	Module created.

   1.79b  Sat Aug 24 2002  tmm
	Work around IAR va_list portability issue.

   1.79b  12/6/2002  chwon
	Use __TMS_CONCAT() instead of ##. This is more portable.

   1.93b  Wed Nov 16 2005 13:53:48  chwon
	1582: Added USBPUMP_IOCTL_OBJECT_DEBUG_PRINT

   1.97k  Sat Oct 25 2008 18:40:24  maw
	6706: Added USBPUMP_IOCTL_DEBUG_CREATE_CMD_IFC

   1.97k  Tue Mar 10 2009 15:09:03  maw
   	6706: Generate uncloaked defs with uncloak-defs.sh
   	corrected comment, removed CONST from CREATE_CMD_IFC_ARG
   	change USBPUMP_IOCTL_DEBUG_CREATE_CMD_IFC to
	USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE

   3.15d  Mon Nov 23 2015 19:06:24  skpark
	19666: Use __TMS_VA_LIST_COPY in
	__TMS_USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG_SETUP_V1

   3.15d  Sun Nov 29 2015 01:25:17  tmm
	19666: 19667: the suggested changes won't work. Instead, we're
	removing the IOCTLs and replacing with equivalent functionality that
	works using the "get device" and "get platform" requests.

*/

#ifndef _USBIOCTL_DEBUG_H_		/* prevent multiple includes */
#define _USBIOCTL_DEBUG_H_

#ifndef __TMS_USBPUMP_COMPAT_19666
# define __TMS_USBPUMP_COMPAT_19666 0
#endif

#if __TMS_USBPUMP_COMPAT_19666
# include <stdarg.h>
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBVALST_H_
# include "usbvalst.h"		/* va_list: clean up for IOS */
#endif

/****************************************************************************\
|
|	The IOCTL codes
|
\****************************************************************************/

/* the macro for defining entrie in this class */
#define __TMS_USBPUMP_IOCTL_DEBUG(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_DEBUG,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_, NAME, _ARG)		\
		)

#if __TMS_USBPUMP_COMPAT_19666

/* the device-based debug print */
#define __TMS_USBPUMP_IOCTL_DEVICE_DEBUG_PRINT				\
	__TMS_USBPUMP_IOCTL_DEBUG(0, W, DEVICE_DEBUG_PRINT)

/* the object-based debug print */
#define __TMS_USBPUMP_IOCTL_OBJECT_DEBUG_PRINT				\
	__TMS_USBPUMP_IOCTL_DEBUG(1, W, OBJECT_DEBUG_PRINT)

#endif /* __TMS_USBPUMP_COMPAT_19666 */

#define __TMS_USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE		\
	__TMS_USBPUMP_IOCTL_DEBUG(2, W, CREATE_DEBUG_COMMAND_INTERFACE)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_DEBUG_PRINT

Index:	Type:	USBPUMP_IOCTL_DEVICE_DEBUG_PRINT_ARG
	Name:	USBPUMP_IOCTL_DEVICE_DEBUG_PRINT_ARG_SETUP_V1()

Function:
	Send a debug print request to the UDEVICE. (DEPRECATED)

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_DEBUG_PRINT_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_DEBUG_PRINT_ARG structure
			(normally NULL)

	USBPUMP_IOCTL_DEVICE_DEBUG_PRINT_ARG has the following elements:

	UINT32 Mask;
		IN:  The debug mask

	CONST TEXT *pFmt;
		IN:  The format string

	va_list ArgPtr;
		IN:  The valist.

Description:
	This IOCTL, if directed to a UDEVICE, or object layered
	upon the same, and if the core has been built as a Checked build,
	does a diagnostic print.  If the mask is zero, this is an
	unconditional display; otherwise the mask anded with the debug
	flags must be non-zero.

Returns:
	USBPUMP_RESULT_SUCCESS: print message successfully.
	USBPUMP_RESULT_NOT_CLAIMED:  the object you sent the IOCTL to is
				not layered on a UDEVICE.

	Other failure codes are possible if the request is malformed.

Notes:
	This function is deprecated, and is only available if you compile
	with __TMS_USBPUMP_COMPAT_19666 defined and non-zero.
*/

#if __TMS_USBPUMP_COMPAT_19666
/*
|| USBPUMP_IOCTL_DEVICE_DEBUG_PRINT does a debug print (in checked
|| builds).
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_DEBUG_PRINT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_DEBUG_PRINT_ARG)
	{
	__TMS_UINT32	Mask;		/* IN: debug mask */
	__TMS_CONST __TMS_CHAR *pFmt;	/* IN: format string */
	__TMS_VA_LIST_REF ArgPtr;	/* IN: va_list */
	};

/* the setup for the .._DEVICE_DEBUG_PRINT_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_DEBUG_PRINT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_Mask,							\
	ARG_pFmt,							\
	ARG_ArgPtr							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_DEBUG_PRINT_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->Mask = (ARG_Mask);					\
	__pArg->pFmt = (ARG_pFmt);					\
	__pArg->ArgPtr = (ARG_ArgPtr);					\
   	} while (0)

#endif /* __TMS_USBPUMP_COMPAT_19666 */

/*

IOCTL:	USBPUMP_IOCTL_OBJECT_DEBUG_PRINT

Index:	Type:	USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG
	Name:	USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG_SETUP_V1()

Function:
	Send a debug print request to the object. (DEPRECATED)

Input:
	pInParam	points to a USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG structure
			(normally NULL)

	USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG has the following elements:

	CONST TEXT *pFmt;
		IN:  The format string

	va_list ArgPtr;
		IN:  The valist.

Description:
	This IOCTL, if directed to a UDEVICE, or object layered
	upon the same, and if the core has been built as a Checked build,
	does a diagnostic print.  If the mask is zero, this is an
	unconditional display; otherwise the mask anded with the debug
	flags must be non-zero.

Returns:
	USBPUMP_RESULT_SUCCESS:  print message successfully.
	USBPUMP_RESULT_NOT_CLAIMED:  the object you sent the IOCTL to is
				not layered on a UDEVICE.

	Other failure codes are possible if the request is malformed.

Notes:
	This IOCTL is deprecated and is only availble if you compile with
	__TMS_USBPUMP_COMPAT_19666 defined and non-zero.

*/

#if __TMS_USBPUMP_COMPAT_19666

/*
|| USBPUMP_IOCTL_OBJECT_DEBUG_PRINT does a debug print (in checked
|| builds).
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG)
	{
	__TMS_CONST __TMS_CHAR *pFmt;	/* IN: format string */
	__TMS_VA_LIST_REF ArgPtr;	/* IN: va_list */
	};

/* the setup for the .._OBJECT_DEBUG_PRINT_ARG struct */
#define __TMS_USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pFmt,							\
	ARG_ArgPtr							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pFmt = (ARG_pFmt);					\
	__TMS_VA_LIST_COPY(__pArg->ArgPtr, ARG_ArgPtr);			\
   	} while (0)

#endif /* __TMS_USBPUMP_COMPAT_19666 */

/*

IOCTL:	USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE

Index:	Type:	USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE_ARG
	Name:	USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE_ARG_SETUP_V1()

Function:
	Create a debug command line interface in recipient object

Input:
	pInParam	points to a USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE_ARG structure
			(normally NULL)

	USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE_ARG has the 
	following elements:

	USBPUMP_OBJECT_HEADER CONST *		pFunctionObject;

Description:
	This IOCTL will create a debug command line interface adapted
	for the particular object to which the IOCTL is directed.

Returns:
	USBPUMP_RESULT_SUCCESS:  Command line interface successfully created.
	USBPUMP_RESULT_NOT_CLAIMED:  the object you sent the IOCTL to is
				does not support this IOCTL.

	Other failure codes are possible if the request is malformed.

*/

/*
|| USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE creates a debug command line 
|| interface in recipient object
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pFunctionObject;
	};

/* the setup for the .._CREATE_DEBUG_COMMAND_INTERFACE_ARG struct */
#define __TMS_USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE_ARG_SETUP_V1( \
	pArg,								\
	ARG_pFunctionObject						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pFunctionObject = (ARG_pFunctionObject);		\
   	} while (0)

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_DEBUG(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_DEBUG(i, t, NAME)
# define USBPUMP_IOCTL_DEVICE_DEBUG_PRINT	\
   __TMS_USBPUMP_IOCTL_DEVICE_DEBUG_PRINT
# define USBPUMP_IOCTL_OBJECT_DEBUG_PRINT	\
   __TMS_USBPUMP_IOCTL_OBJECT_DEBUG_PRINT
# define USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE	\
   __TMS_USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE
# define USBPUMP_IOCTL_DEVICE_DEBUG_PRINT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_Mask,							\
	ARG_pFmt,							\
	ARG_ArgPtr							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_DEBUG_PRINT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_Mask,							\
	ARG_pFmt,							\
	ARG_ArgPtr							\
	)
# define USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pFmt,							\
	ARG_ArgPtr							\
	)	\
	__TMS_USBPUMP_IOCTL_OBJECT_DEBUG_PRINT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pFmt,							\
	ARG_ArgPtr							\
	)
# define USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pFunctionObject						\
	)	\
	__TMS_USBPUMP_IOCTL_CREATE_DEBUG_COMMAND_INTERFACE_ARG_SETUP_V1( \
	pArg,								\
	ARG_pFunctionObject						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

#endif /* _USBIOCTL_DEBUG_H_ */
