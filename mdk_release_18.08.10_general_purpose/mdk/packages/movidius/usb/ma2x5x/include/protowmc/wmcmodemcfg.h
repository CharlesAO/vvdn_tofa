/* wmcmodemcfg.h	Thu Apr 28 2005 12:45:45 maw */

/*

Module:  wmcmodemcfg.h

Function:
	WMC Modem SubClass configuration linkage header file.

Version:
	V1.91c	Thu Apr 28 2005 12:45:45 maw	Edit level 3

Copyright notice:
	This file copyright (C) 2002-2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Sun Jul  7 2002 09:01:52  tmm
	Module created.

   1.91c  Thu Apr 28 2005 12:45:45 maw
   	Bug 1463: Created modem specific configuration structure
 
*/

#ifndef _WMCMODEMCFG_H_		/* prevent multiple includes */
#define _WMCMODEMCFG_H_

#ifndef _WMCMODEM_H_
# include "wmcmodem.h"
#endif

#ifndef _WMCTACFG_H_
# include "wmctacfg.h"
#endif

/****************************************************************************\
|
|	Feature flags
|
\****************************************************************************/

#define __TMS_UPROTO_WMCSUBCLASS_MODEM_FLAGS_FILTER_CTRLPLANE	(1 << 0)
#define __TMS_UPROTO_WMCSUBCLASS_MODEM_FLAGS_FILTER_DATAPLANE	(1 << 1)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_WMCSUBCLASS_MODEM_FLAGS_FILTER_CTRLPLANE	\
   __TMS_UPROTO_WMCSUBCLASS_MODEM_FLAGS_FILTER_CTRLPLANE
# define UPROTO_WMCSUBCLASS_MODEM_FLAGS_FILTER_DATAPLANE	\
   __TMS_UPROTO_WMCSUBCLASS_MODEM_FLAGS_FILTER_DATAPLANE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/****************************************************************************\
|
|	The configuration structure
|
\****************************************************************************/

/*

Type:	UPROTO_WMC_MODEM_CONFIG

Function:
	Carries configuration information into the portable TA configuration
	logic.

Contents:
	BITS Flags;	
		Feature flags

Notes:
	This structure is normally initialized using
	UPROTO_WMC_CONFIG_MODEM_INIT_Vx(), or UPROTO_WMC_MODEM_CONFIG_SETUP_Vx.

See Also:
	UPROTO_WMC_MODEM_CONFIG_SETUP_V1(),
	UPROTO_WMC_MODEM_CONFIG_INIT_V1()

*/
__TMS_TYPE_DEF_STRUCT(  UPROTO_WMC_MODEM_CONFIG);

struct __TMS_STRUCTNAME(UPROTO_WMC_MODEM_CONFIG)
	{
	__TMS_BITS				Flags;	
	};

/*

Name:	UPROTO_WMC_MODEM_CONFIG_SETUP_V1()

Function:
	Macro: runtime initialization code for UPROTO_WMC_MODEM_CONFIG()

Definition:
	VOID UPROTO_WMC_MODEM_CONFIG_SETUP_V1(
		BYTES			Flags
		);

Description:
	This macro allows code to initialize a UPROTO_WMC_MODEM_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_WMC_MODEM_CONFIG_INIT_V1()

*/

/* don't add a parameter to this macro, create a _V2... instead */
#define	__TMS_UPROTO_WMC_MODEM_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_Flags						\
		)							\
	do	{							\
		(pConfig)->Flags = (ARG_Flags);				\
		} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_WMC_MODEM_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_Flags						\
		)							\
	__TMS_UPROTO_WMC_MODEM_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_Flags						\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

Name:	UPROTO_WMC_MODEM_CONFIG_INIT_V1()

Function:
	Macro: compile time initializatoin for UPROTO_WMC_MODEM_CONFIG.

Definition:
	UPROTO_WMC_MODEM_CONFIG_INIT_V1(
		BYTES			Flags
		);
	
Description:
	This macro is used when initializing a single UPROTO_WMC_MODEM_CONFIG
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

/* don't add a parameter to this macro, create a _V2... instead */
#define	__TMS_UPROTO_WMC_MODEM_CONFIG_INIT_V1(				\
		ARG_Flags						\
		)							\
		{							\
		(ARG_Flags)						\
		}

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_WMC_MODEM_CONFIG_INIT_V1(				\
		ARG_Flags						\
		)							\
	__TMS_UPROTO_WMC_MODEM_CONFIG_INIT_V1(				\
		ARG_Flags						\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/****************************************************************************\
|
|	Here is the linkage to the default configuration.
|
\****************************************************************************/

/* The Modem subclass's default TA configuration structure */
extern CONST UPROTO_WMCTA_CONFIG			
gk_WmcSubClass_Modem_TaConfig;

/**** end of wmcmodemcfg.h ****/
#endif /* _WMCMODEMCFG_H_ */
