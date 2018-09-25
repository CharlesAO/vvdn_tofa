/* mscatapicfg.h	Wed Sep 15 2004 14:45:23 maw */

/*

Module:  mscatapicfg.h

Function:
	Data structures to configure the RAM-disk application

Version:
	V1.89b  Wed Sep 15 2004 14:45:23 maw	Edit level 1

Copyright notice:
	This file copyright (C) 2004 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Mats Webjörn, MCCI Corporation	Sep 2004

Revision history:
   1.89b  Wed Sep 15 2004 14:45:23  maw
	Module created.

*/

#ifndef _MSCAPPLIBCFG_H_		/* prevent multiple includes */
#define _MSCAPPLIBCFG_H_

#include "mscapplib.h"

/****************************************************************************\
|
|	Structures declared here
|
\****************************************************************************/


/****************************************************************************\
|
|	The configuration structure
|
\****************************************************************************/

/*

Type:	MSCDEMO_RAMDISK_LUN_CONFIG

Function:
	Carries configuration information for each LUN mapped onto the
	RAM-disk

Contents:
	BYTES LoBlock;
		First block of for this LUN on the RAM-disk

	BYTES HiBlock;
		Last block of for this LUN on the RAM-disk
Notes:
	This structure is normally initialized using
	MSCDEMO_RAMDISK_LUN_CONFIG_INIT_Vx(), or MSCDEMO_RAMDISK_LUN_CONFIG_SETUP_Vx.

See Also:

*/

struct __TMS_STRUCTNAME(MSCDEMO_RAMDISK_LUN_CONFIG)
	{
	__TMS_BYTES 		LoLba;			
	__TMS_BYTES 		HiLba;			
	};

/*

Name:	MSCDEMO_RAMDISK_LUN_CONFIG_SETUP_V1()

Function:
	Macro: runtime initialization code for MSCDEMO_RAMDISK_LUN_CONFIG()

Definition:
	VOID MSCDEMO_RAMDISK_LUN_CONFIG_SETUP_V1(
		MSCDEMO_RAMDISK_LUN_CONFIG *	pConfig,
		BYTES 				LoLba,			
		BYTES 				HiLba			
		);

Description:
	This macro allows code to initialize a MSCDEMO_RAMDISK_LUN_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	MSCDEMO_RAMDISK_LUN_CONFIG_INIT_V1()

*/

/* don't add a parameter to this macro, create a _V2... instead */
#define	__TMS_MSCDEMO_RAMDISK_LUN_CONFIG_SETUP_V1(			\
		pConfig,						\
		ARG_LoLba,						\
		ARG_HiLba						\
		)							\
	do	{							\
		(pConfig)->LoLba	   = (ARG_LoLba);		\
		(pConfig)->HiLba	   = (ARG_HiLba);		\
		} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define MSCDEMO_RAMDISK_LUN_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_LoLba,						\
		ARG_HiLba						\
		)							\
	__TMS_MSCDEMO_RAMDISK_LUN_CONFIG_SETUP_V1(			\
		pConfig,						\
		ARG_LoLba,						\
		ARG_HiLba						\
		)

#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

Name:	MSCDEMO_RAMDISK_LUN_CONFIG_INIT_V1()

Function:
	Macro: compile time initialization for MSCDEMO_RAMDISK_LUN_CONFIG.

Definition:
	MSCDEMO_RAMDISK_LUN_CONFIG_INIT_V1(
		BYTES 				LoLba,			
		BYTES 				HiLba			
		);

Description:
	This macro is used when initializing a single MSCDEMO_RAMDISK_LUN_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V2... instead */
#define	__TMS_MSCDEMO_RAMDISK_LUN_CONFIG_INIT_V1(			\
		ARG_LoLba,						\
		ARG_HiLba						\
		)							\
	{								\
	(ARG_LoLba),							\
	(ARG_HiLba)							\
	}


/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define MSCDEMO_RAMDISK_LUN_CONFIG_INIT_V1(				\
		ARG_LoLba,						\
		ARG_HiLba						\
		)							\
	__TMS_MSCDEMO_RAMDISK_LUN_CONFIG_INIT_V1(			\
		ARG_LoLba,						\
		ARG_HiLba						\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*
Type:	MSCDEMO_RAMDISK_CONFIG

Function:
	Carries configuration information for the entire RAM-disk

Contents:
	CONST MSCDEMO_RAMDISK_LUN_CONFIG * pLun;
		Pointer to vector with LUN configuration parameters

	BYTES nLun;
		Number of elements in LUN config vector

Notes:
	This structure is normally initialized using
	MSCDEMO_RAMDISK_CONFIG_INIT_V1(), or MSCDEMO_RAMDISK_CONFIG_SETUP_V1.

See Also:

*/

struct __TMS_STRUCTNAME(MSCDEMO_RAMDISK_CONFIG)
	{
	__TMS_BYTES					nLba;

	/*
	|| Vector of LUN configuration
	*/
	__TMS_CONST __TMS_MSCDEMO_RAMDISK_LUN_CONFIG *	pLun;
	__TMS_BYTES					nLun;
	};

/*

Name:	MSCDEMO_RAMDISK_CONFIG_SETUP_V1()

Function:
	Macro: runtime initialization code for MSCDEMO_RAMDISK_CONFIG()

Definition:
	VOID MSCDEMO_RAMDISK_CONFIG_SETUP_V1(
		MSCDEMO_RAMDISK_CONFIG *		pConfig,
		BYTES					nLba,
		CONST MSCDEMO_RAMDISK_LUN_CONFIG *	pLun
		);

Description:
	This macro allows code to initialize a MSCDEMO_RAMDISK_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	MSCDEMO_RAMDISK_CONFIG_INIT_V1()

*/

/* don't add a parameter to this macro, create a _V2... instead */
#define	__TMS_MSCDEMO_RAMDISK_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_nLba,						\
		ARG_pLun						\
		)							\
	do	{							\
		(pConfig)->nLba	= (ARG_nLba);				\
		(pConfig)->pLun	= (ARG_pLun);				\
		(pConfig)->nLun	= __TMS_LENOF(ARG_pLun); 		\
		} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define MSCDEMO_RAMDISK_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_nLba,						\
		ARG_pLun						\
		)							\
	__TMS_MSCDEMO_RAMDISK_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_nLba,						\
		ARG_pLun						\
		)

#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

Name:	MSCDEMO_RAMDISK_CONFIG_INIT_V1()

Function:
	Macro: compile time initialization for MSCDEMO_RAMDISK_CONFIG.

Definition:
	MSCDEMO_RAMDISK_CONFIG_INIT_V1(
		BYTES					nLba,
		CONST MSCDEMO_RAMDISK_LUN_CONFIG *	pLun
		);

Description:
	This macro is used when initializing a single MSCDEMO_RAMDISK_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V2... instead */
#define	__TMS_MSCDEMO_RAMDISK_CONFIG_INIT_V1(				\
		ARG_nLba,						\
		ARG_pLun						\
		)							\
	{								\
	(ARG_nLba),							\
	(ARG_pLun),							\
	__TMS_LENOF(ARG_pLun)						\
	}


/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define MSCDEMO_RAMDISK_CONFIG_INIT_V1(				\
		ARG_nLba,						\
		ARG_pLun						\
		)							\
	__TMS_MSCDEMO_RAMDISK_CONFIG_INIT_V1(				\
		ARG_nLba,						\
		ARG_pLun						\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/****************************************************************************\
|
|	The variables
|
\****************************************************************************/

extern __TMS_CONST __TMS_MSCDEMO_RAMDISK_CONFIG 
			gk_MscDemo_RamdiskAppConfig[];

/**** end of mscapplibcfg.h ****/
#endif /* _MSCAPPLIBCFG_H_ */
