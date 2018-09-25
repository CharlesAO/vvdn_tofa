/* wmctacfg.h	Thu Apr 28 2005 10:52:35 maw */

/*

Module:  wmctacfg.h

Function:
	Data structures for tables that configure an WMC TA

Version:
	V1.91c	Thu Apr 28 2005 10:52:35 maw	Edit level 4

Copyright notice:
	This file copyright (C) 2001-2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	May 2001

Revision history:
   1.79a  6/23/2002  tmm
	Module created.

   1.91c  Thu Apr 28 2005 10:52:35 maw
   	Bug 1463: Added extension pointer to config structure
   	
*/

#ifndef _WMCTACFG_H_		/* prevent multiple includes */
#define _WMCTACFG_H_

#ifndef _PROTOWMC_H_
# include "protowmc.h"
#endif

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

Type:	UPROTO_WMCTA_CONFIG

Function:
	Carries configuration information into the portable TA configuration
	logic.

Contents:
	BYTES SizeSubClass;
		Sets the size of the subclass structure.

	UINT32 TagSubClass;	
		Sets the tag for the subclass structure.

	CONST TEXT *pName;
		Sets the name to be given to the subclass structure.
	
	BYTES ControlBufferSize;
		Sets the maximum SETUP data phase length; a buffer of
		specified size is allocated.

	BYTES InRingbufSize;
		Sets the ringbuffer size for control data being sent
		towards the host.

	BYTES OutRingbufSize;
		Sets the ringbuffer size for control data being
		received from the host.

	VOID * pSubClassConfig;
		Pointer to subclass configuration

Notes:
	This structure is normally initialized using
	UPROTO_WMCTA_CONFIG_INIT_Vx(), or UPROTO_WMCTA_CONFIG_SETUP_Vx.

See Also:
	UPROTO_WMCTA_CONFIG_SETUP_V2(),
	UPROTO_WMCTA_CONFIG_INIT_V2()

*/

struct __TMS_STRUCTNAME(UPROTO_WMCTA_CONFIG)
	{
	/* how large is the structure? */
	__TMS_BYTES				SizeSubClass;

	/* what is the tag? */
	__TMS_UINT32				TagSubClass;

	/* what's this TA's name? */
	__TMS_CONST __TMS_TEXT			*pName;

	/* What's the upcall switch? */
	__TMS_CONST __TMS_UPROTO_WMCSUBCLASS_UPCALLS	
						*pUpcallTable;

	/* how long for setup packets? */
	__TMS_BYTES				ControlBufferSize;

	/* how big are the encaps command ring buffers? */
	__TMS_BYTES				InRingbufSize;
	__TMS_BYTES				OutRingbufSize;

	/* optional pointer to sub-class configuration */
	__TMS_VOID *				pSubClassConfig;
	};

/*

Name:	UPROTO_WMCTA_CONFIG_SETUP_V2()

Function:
	Macro: runtime initialization code for UPROTO_WMCTA_CONFIG()

Definition:
	VOID UPROTO_WMCTA_CONFIG_SETUP_V2(
		UPROTO_WMCTA_CONFIG	*pConfig,
		BYTES			SizeSubClass,
		UINT32			TagSubClass,
		CONST TEXT		*pNameTA,
		CONST UPROTO_WMCSUBCLASS_UPCALLS *pUpcallTable,
		BYTES			ControlBufferSize,
		BYTES			InRingbufSize,
		BYTES			OutRingbufSize,
		VOID *			pSubClassConfig
		);

Description:
	This macro allows code to initialize a UPROTO_WMCTA_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_WMCTA_CONFIG_INIT_V2()

*/

/* don't add a parameter to this macro, create a _V3... instead */
#define	__TMS_UPROTO_WMCTA_CONFIG_SETUP_V2(				\
		pConfig,						\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_pUpcallTable,					\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize,					\
		ARG_pSubClassConfig					\
		)							\
	do	{							\
		(pConfig)->SizeSubClass = (ARG_SizeSubClass);		\
		(pConfig)->TagSubClass = (ARG_TagSubClass);		\
		(pConfig)->pName = (ARG_pName);				\
		(pConfig)->pUpcallTable = (ARG_pUpcallTable);		\
		(pConfig)->ControlBufferSize = (ARG_ControlBufferSize);	\
		(pConfig)->InRingbufSize = (ARG_InRingbufSize);		\
		(pConfig)->pSubClassConfig = (ARG_pSubClassConfig);	\
		} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_WMCTA_CONFIG_SETUP_V2(					\
		pConfig,						\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize,					\
		ARG_pSubClassConfig					\
		)							\
	__TMS_UPROTO_WMCTA_CONFIG_SETUP_V2(				\
		pConfig,						\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize,					\
		ARG_pSubClassConfig					\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

#define	__TMS_UPROTO_WMCTA_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_pUpcallTable,					\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize					\
		)							\
	__TMS_UPROTO_WMCTA_CONFIG_SETUP_V2(				\
		pConfig,						\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize,					\
		__TMS_NULL						\
		)
		
/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY

# define UPROTO_WMCTA_CONFIG_SETUP_V1(					\
		pConfig,						\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize					\
		)							\
	__TMS_UPROTO_WMCTA_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize					\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

Name:	UPROTO_WMCTA_CONFIG_INIT_V2()

Function:
	Macro: compile time initialization for UPROTO_WMCTA_CONFIG.

Definition:
	UPROTO_WMCTA_CONFIG_INIT_V2(
		BYTES			SizeSubClass,
		UINT32			TagSubClass,
		CONST TEXT		*pName,
		CONST UPROTO_WMCSUBCLASS_UPCALLS *pUpcallTable,
		BYTES			ControlBufferSize,
		BYTES			InRingbufSize,
		BYTES			OutRingbufSize,
		VOID *			pSubClassConfig
		);
	
Description:
	This macro is used when initializing a single UPROTO_WMCTA_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

	NameModeConfigVector must be the name of a vector whose size is
	known at compile time.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V3... instead */
#define	__TMS_UPROTO_WMCTA_CONFIG_INIT_V2(				\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_pUpcallTable,					\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize,					\
		ARG_pSubClassConfig					\
		)							\
	{								\
	(ARG_SizeSubClass),						\
	(ARG_TagSubClass),						\
	(ARG_pName),							\
	(ARG_pUpcallTable),						\
	(ARG_ControlBufferSize),					\
	(ARG_InRingbufSize),						\
	(ARG_OutRingbufSize),						\
	(ARG_pSubClassConfig)						\
	}


/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_WMCTA_CONFIG_INIT_V2(					\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_pUpcallTable,					\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize,					\
		ARG_pSubClassConfig					\
		)							\
	__TMS_UPROTO_WMCTA_CONFIG_INIT_V2(				\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_pUpcallTable,					\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize,					\
		ARG_pSubClassConfig					\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

#define	__TMS_UPROTO_WMCTA_CONFIG_INIT_V1(				\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_pUpcallTable,					\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize					\
		)							\
	__TMS_UPROTO_WMCTA_CONFIG_INIT_V2(				\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_pUpcallTable,					\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize,					\
		__TMS_NULL						\
		)
#

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_WMCTA_CONFIG_INIT_V1(					\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_pUpcallTable,					\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize					\
		)							\
	__TMS_UPROTO_WMCTA_CONFIG_INIT_V1(				\
		ARG_SizeSubClass,					\
		ARG_TagSubClass,					\
		ARG_pName,						\
		ARG_pUpcallTable,					\
		ARG_ControlBufferSize,					\
		ARG_InRingbufSize,					\
		ARG_OutRingbufSize					\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of wmctacfg.h ****/
#endif /* _WMCTACFG_H_ */
