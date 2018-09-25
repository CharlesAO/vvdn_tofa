/* wmcfeature.h	Sun Feb 23 2003 00:32:39 tmm */

/*

Module:  wmcfeature.h

Function:
	WMC header file for handling ACM features.

Version:
	V1.83a	Sun Feb 23 2003 00:32:39 tmm	Edit level 3

Copyright notice:
	This file copyright (C) 2002-2003 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	February 2003

Revision history:
   1.81a  Thu Feb 20 2003 03:01:27  tmm
	Created from wmcmodem.h V1.79a

   1.81a  Sat Feb 22 2003 13:43:18  tmm
	Change APIs to centralize everything in a single SETUP-packet processor.

   1.83a  Sun Feb 23 2003 00:32:39  tmm
	Add the activate API.

*/

#ifndef _WMCFEATURE_H_		/* prevent multiple includes */
#define _WMCFEATURE_H_

#ifndef _PROTOWMC_H_
# include "protowmc.h"
#endif

#ifndef _WMCSUBCLASSLIB_H_
# include "wmcsubclasslib.h"
#endif

#ifndef _USBCDC11_H_
# include "usbcdc11.h"
#endif

/****************************************************************************\
|
|	Some parameters
|
\****************************************************************************/


/****************************************************************************\
|
|	The feature mapping macros
|
\****************************************************************************/


/*

Name:	UPROTO_WMCSUBCLASS_ACM_FEATURE

Index:	Name:	UPROTO_WMCSUBCLASS_ACM_iFEATURE
	Name:	UPROTO_WMCSUBCLASS_ACM_pFEATURE
	Name:	UPROTO_WMCSUBCLASS_ACM_IS_VALID_FEATURE
	Name:	UPROTO_WMCSUBCLASS_ACM_NUM_FEATURES

Function:
	Macros for accessing abstract features in WMC MODEMs

Definition:
	UINT UPROTO_WMCSUBCLASS_ACM_iFEATURE(
		UPROTO_WMCSUBCLASS_MODEM *pSubClass, 
		UINT16 wValue
		);

	UINT16 *UPROTO_WMCSUBCLASS_ACM_pFEATURE(
		UPROTO_WMCSUBCLASS_MODEM *pSubClass, 
		
		UINT16 wValue
		);

	BOOL UPROTO_WMCSUBCLASS_ACM_IS_VALID_FEATURE(
		UPROTO_WMCSUBCLASS_MODEM *pSubClass, 
		UINT16 wValue
		);

	CONST UINT UPROTO_WMCSUBCLASS_ACM_NUM_FEATURES;

Description:
	The CDC ACM specification defines a variety of abstract features,
	controlled by the SET/CLEAR/GET_COMM_FEATURE class command.

	The UPROTO_WMCSUBCLASS_MODEM implementation represents each feature
	as a location in an array of features.  Each feature has an index,
	and a slot.  The feature index is computed (from the SETUP packet
	selector wValue) using UPROTO_WMCSUBCLASS_ACM_iFEATURE() macro.  A 
	SETUP packet wValue is checked for validity using
	UPROTO_WMCSUBCLASS_ACM_IS_FEATURE_VALID().  The storage locaton 
	for the feature value is found using 
	UPROTO_WMCSUBCLASS_ACM_pFEATURES.
	The number of distinct features is defined by
	UPROTO_WMCSUBCLASS_ACM_NUM_FEATURES.

Returns:
	See description.

*/

#define	__TMS_UPROTO_WMCSUBCLASS_ACM_iFEATURE(pSubClass, wValue)	\
		((wValue) -						\
		 USB_Comm_COMM_FEATURE_wValue_ABSTRACT_STATE)

#define	__TMS_UPROTO_WMCSUBCLASS_ACM_pFEATURE(pSubClass, pCommFeatures, wValue)	\
	((pCommFeatures) +						\
		__TMS_UPROTO_WMCSUBCLASS_ACM_iFEATURE(pSubClass, wValue) \
	)

#define	__TMS_UPROTO_WMCSUBCLASS_ACM_IS_VALID_FEATURE(pSubClass, wValue) \
	(USB_Comm_COMM_FEATURE_wValue_ABSTRACT_STATE <= (wValue) &&	\
	 (wValue) <= USB_Comm_COMM_FEATURE_wValue_COUNTRY_SETTING)

#define	__TMS_UPROTO_WMCSUBCLASS_ACM_NUM_FEATURES			\
	(USB_Comm_COMM_FEATURE_wValue_COUNTRY_SETTING -			\
	 USB_Comm_COMM_FEATURE_wValue_ABSTRACT_STATE + 1)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_WMCSUBCLASS_ACM_iFEATURE(pSubClass,wValue)		\
   __TMS_UPROTO_WMCSUBCLASS_ACM_iFEATURE(pSubClass,wValue)
# define UPROTO_WMCSUBCLASS_ACM_pFEATURE(pSubClass,pCommFeatures,wValue) \
   __TMS_UPROTO_WMCSUBCLASS_ACM_pFEATURE(pSubClass,pCommFeatures,wValue)
# define UPROTO_WMCSUBCLASS_ACM_IS_VALID_FEATURE(pSubClass,wValue)	\
   __TMS_UPROTO_WMCSUBCLASS_ACM_IS_VALID_FEATURE(pSubClass,wValue)
# define UPROTO_WMCSUBCLASS_ACM_NUM_FEATURES				\
   __TMS_UPROTO_WMCSUBCLASS_ACM_NUM_FEATURES
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/****************************************************************************\
|
|	Exported entry points
|
\****************************************************************************/

/* none: all entry points are reached by IOCTL_MODEM or IOCTL_FUNCTION operations */

/****************************************************************************\
|
|	Function Prototypes
|
\****************************************************************************/

__TMS_BEGIN_DECLS

BOOL
WmcSubClassLib_CommFeature_ProcessSetup(
	UPROTO_WMCSUBCLASS *	pSubClass_Abstract,
	CONST USETUP *		pSetup,
	CONST UINT8 *		pDataFromHost,
	BYTES			lengthDataFromHost,
	UINT8 *			pInDataBuffer,
	BYTES			nInDataBuffer,
	WMCSUBCLASS_SETUP_RESULT *pResultData,
	UINT16			*pCommFeatures,
	CONST UINT16		*pvDefaults
	);

BOOL
WmcSubClassLib_CommFeature_Activate(
	UPROTO_WMCSUBCLASS *pSubClass,
	UINT16 *pvCommFeatures,
	CONST UINT16 *pvDefaults
	);

__TMS_END_DECLS

/**** end of wmcfeature.h ****/
#endif /* _WMCFEATURE_H_ */
