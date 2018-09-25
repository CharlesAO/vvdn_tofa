/* usbpump_pathlib.h	Mon Mar 26 2007 18:23:52 yssaroha */

/*

Module:  usbpump_pathlib.h

Function:
	Definitions for the path library.

Version:
	V1.97h	Mon Mar 26 2007 18:23:52 yssaroha	Edit level 3

Copyright notice:
	This file copyright (C) 2005-2007 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	September 2005

Revision history:
   1.91h  Fri Sep  2 2005 03:30:39  tmm
	Module created.

   1.91h  Sun Sep 04 2005 17:35:01 maw
   	Added pointers to config info

   1.97h  Mon Mar 26 2007 18:23:52  yssaroha
	Fixed PR 2595

*/

#ifndef _USBPUMP_PATHLIB_H_		/* prevent multiple includes */
#define _USBPUMP_PATHLIB_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMP_PATHLIB_TYPES_H_
# include "usbpump_pathlib_types.h"
#endif

#ifndef _USBPUMP_UNICODE_H_
# include "usbpump_unicode.h"
#endif

/****************************************************************************\
|
|	The pathlib library context
|
\****************************************************************************/

/* __TMS_TYPE_DEF_STRUCT	(USBPUMP_PATHLIB_CONTEXT); */

struct __TMS_STRUCTNAME	(USBPUMP_PATHLIB_CONTEXT)
	{
	__TMS_CONST __TMS_TEXT *	pDirSep;
	__TMS_CONST __TMS_TEXT *	pDevSep;
	__TMS_CONST __TMS_TEXT *	pPathSep;
	__TMS_BOOL			fCaseSensitive;
	__TMS_UINT8			CharClass[256];
	};

/****************************************************************************\
|
|	Function prototypes
|
\****************************************************************************/


__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpPathlib_Init(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *pDirSep,
	__TMS_CONST __TMS_TEXT *pDevSep,
	__TMS_CONST __TMS_TEXT *pPathSep,
	__TMS_BOOL fCaseSensitive
	);

__TMS_VOID
UsbPumpPathlib_End(
	__TMS_USBPUMP_PATHLIB_CONTEXT *
	);

__TMS_BOOL
UsbPumpPathlib_IsPathRelative(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BOOL
UsbPumpPathlib_IsPathSep(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_TEXT
	);

__TMS_CONST __TMS_TEXT *
UsbPumpPathlib_FirstPathSep(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UsbPumpPathlib_FirstPathSepIndex(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_CONST __TMS_TEXT *
UsbPumpPathlib_LastPathSep(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UsbPumpPathlib_LastPathSepIndex(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BOOL
UsbPumpPathlib_IsDirSep(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_TEXT
	);

__TMS_CONST __TMS_TEXT *
UsbPumpPathlib_FirstDirSep(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UsbPumpPathlib_FirstDirSepIndex(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_CONST __TMS_TEXT *
UsbPumpPathlib_FirstDevSep(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UsbPumpPathlib_FirstDevSepIndex(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_CONST __TMS_TEXT *
UsbPumpPathlib_LastDirSep(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UsbPumpPathlib_LastDirSepIndex(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_CONST __TMS_TEXT *
UsbPumpPathlib_LastDevSep(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UsbPumpPathlib_LastDevSepIndex(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_CONST __TMS_TEXT *
UsbPumpPathlib_FirstNonDevice(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UsbPumpPathlib_FirstNonDeviceIndex(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_CONST __TMS_TEXT *
UsbPumpPathlib_LastExtSep(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UsbPumpPathlib_LastExtSepIndex(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_CONST __TMS_TEXT *
UsbPumpPathlib_FindExt(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UsbPumpPathlib_FindExtIndex(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BOOL
UsbPumpPathlib_IsPathDeviceBased(
	__TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *
	);

/* XXX: tmm: no ship: this belongs in the posix APIs */
__TMS_BOOL
UsbPumpPathlib_PathCompareBufferToString(
	__TMS_CONST __TMS_USBPUMP_PATHLIB_CONTEXT *,
	__TMS_CONST __TMS_TEXT *,
	__TMS_BYTES,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_END_DECLS

/**** end of usbpump_pathlib.h ****/
#endif /* _USBPUMP_PATHLIB_H_ */
