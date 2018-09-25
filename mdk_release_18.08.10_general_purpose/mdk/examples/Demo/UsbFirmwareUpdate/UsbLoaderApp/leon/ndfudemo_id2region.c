/* ndfudemo_id2region.c	Wed Dec 24 2014 12:55:54 chwon */

/*

Module:  ndfudemo_id2region.c

Function:
	The NDFU client code for getting memory region coresponding
	to a string descriptor id

Version:
	V3.13b	Wed Dec 24 2014 12:55:54 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2004-2005, 2007, 2011, 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

	Mats Webjorn, MCCI Corporation	June 2004
*/

#include "ndfuapplib.h"
#include "usbpumpdebug.h"
#include "system.h"

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/

#ifndef	USBPUMP_NDFU_FLASH_REGION_SIZE		/* PARAM */
# define USBPUMP_NDFU_FLASH_REGION_SIZE		(APP_MAX_FILE_SIZE)
#endif

#ifndef	USBPUMP_NDFU_EEPROM_REGION_SIZE		/* PARAM */
# define USBPUMP_NDFU_EEPROM_REGION_SIZE	(1 * 1024)
#endif


/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only
|	using the ROM storage class; they may be global.
|
\****************************************************************************/


/****************************************************************************\
|
|	VARIABLES:
|
|	If program is to be ROM-able, these must be initialized
|	using the BSS keyword.  (This allows for compilers that require
|	every variable to have an initializer.)  Note that only those
|	variables owned by this module should be declared here, using the BSS
|	keyword; this allows for linkers that dislike multiple declarations
|	of objects.
|
\****************************************************************************/

/* Simulated memory regions */
UINT8	__attribute__((section(".ddr.bss"))) NDfuDemo_FlashRegion[USBPUMP_NDFU_FLASH_REGION_SIZE];
UINT8	__attribute__((section(".ddr.bss"))) NDfuDemo_EepromRegion[USBPUMP_NDFU_EEPROM_REGION_SIZE];
/*

Name:	NDfuDemo_Id2Region()

Function:
	Return memory region parameters corresponding to the string descriptor
	id, interface number and alternate setting number.

Definition:
	BOOL NDfuDemo_Id2Region;
		UDEVICE *	pDevice,
		UINT8		StringId,
		UINT8		bInterface,
		UINT8		bAlternateSetting,
		BYTES *		pnBytes,
		UINT8 *		pRegion
		);

Description:
	This module maps a string descriptor id to a memory region and
	returns a pointer to the region and its size in bytes.

	This function is just a way for the demonstration program to solve
	a code management issue. It is not required to use this technique in
	a real system.

Returns:
	No explicit result.

*/

BOOL NDfuDemo_Id2Region(
	UDEVICE *	pDevice,
	UINT8		StringId,
	UINT8		bInterface,
	UINT8		bAlternateSetting,
	BYTES *		pnBytes,
	UINT8 **	ppRegion
	)
	{
	USBPUMP_UNREFERENCED_PARAMETER(pDevice);
	USBPUMP_UNREFERENCED_PARAMETER(StringId);
	USBPUMP_UNREFERENCED_PARAMETER(bInterface);

	switch (bAlternateSetting)
		{
	case 0:
		/* alternate setting 0 is flash resgion */
		*ppRegion = NDfuDemo_FlashRegion;
		*pnBytes  = sizeof(NDfuDemo_FlashRegion);
		break;

	case 1:
		/* alternate setting 1 is eeprom resgion */
		*ppRegion = NDfuDemo_EepromRegion;
		*pnBytes  = sizeof(NDfuDemo_EepromRegion);
		break;

	default:
		TTUSB_PRINTF((
			pDevice,
			UDMASK_ERRORS,
			"?NDfuDemo_Id2Region:"
			" error invalid alternate setting %d\n",
			bAlternateSetting
			));

		return FALSE;
		}

	return TRUE;
	}
