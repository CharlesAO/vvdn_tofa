/* ndfudemo_create.c	Tue Apr 14 2009 12:12:56  maw */

/*

Module:  ndfudemo_create.c

Function:
	The NDFU client code for creating the NDFU demonstration
	bindings.

Version:
	V2.01a	Tue Apr 14 2009 12:12:56  maw	Edit level 2

Copyright notice:
	This file copyright (C) 2004 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

	Mats Webjorn, MCCI Corporation	June 2004
*/

#include "usbpumpapi.h"
#include "ndfuapplib.h"
#include "uplatform.h"
#include "usbpumpobjectapi.h"
#include "protondfu.h"
#include "usbpumproot.h"

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not 
|	be exported.
|
\****************************************************************************/

VOID NDfuDemo_ClientCreate(UPLATFORM *pPlatform);


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

/*

Name:	NDfuDemo_ClientCreate()

Function:
	Generate the leaf functions and bindings for the NDFU demo app

Definition:
	VOID NDfuDemo_ClientCreate(
		UDEVICE *pPlatform
		);

Description:
	This module scans the named functions of the datapump, looking
	for objects named "download.*.fn.mcci.com".  For each of these, 
	the module creates device instances and initializes the datastructures.	

Returns:
	No explicit result.

*/

VOID
NDfuDemo_ClientCreate(
	UPLATFORM *pPlatform
	)
	{
	USBPUMP_OBJECT_ROOT * CONST pPumpRoot =
		UsbPumpObject_GetRoot(&pPlatform->upf_Header);
	USBPUMP_OBJECT_HEADER *pFunctionObject = NULL;

	/* enumerate the function objects */
	while ((pFunctionObject = UsbPumpObject_EnumerateMatchingNames(
					&pPumpRoot->Header,
					pFunctionObject,
					USBPUMP_OBJECT_NAME_ENUM_DOWNLOAD
					)) != NULL)
		{
		NDfuDemo_CreateRamLoad(
			pPlatform,
			pFunctionObject,
			NULL
			);
		}
	}
