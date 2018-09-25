/* wmcdevmgmt.h	Wed Mar 18 2009 12:27:05 chwon */

/*

Module:  wmcdevmgmt.h

Function:
	WMC Device Management TA SubClass header file.

Version:
	V2.01a	Wed Mar 18 2009 12:27:05 chwon	Edit level 9

Copyright notice:
	This file copyright (C) 2002-2003, 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	March 2002

Revision history:
   1.79a  2002/03/08  tmm
	Cloned from MCPCMODE_DEVMGMT version 1.61j

   1.81a  Thu Feb 20 2003 03:12:05  tmm
	Add the abstract feature table.

   1.81a  Sat Feb 22 2003 13:53:44  tmm
	Add the default country code.

   2.01a  Wed Mar 18 2009 12:27:05  chwon
	2829: move out object name from usbpumpobjectname.h to here.

*/

#ifndef _WMCDEVMGMT_H_		/* prevent multiple includes */
#define _WMCDEVMGMT_H_

#ifndef _PROTOWMC_H_
# include "protowmc.h"
#endif

#ifndef _WMCSUBCLASSLIB_H_
# include "wmcsubclasslib.h"
#endif

#ifndef _WMCFEATURE_H_
# include "wmcfeature.h"
#endif

/****************************************************************************\
|
|	The parameters 
|
\****************************************************************************/

/* 
|| The API requires that we reset the country code to the default when we 
|| get a CLEAR_FEATURE, reset, etc.   This is supposed to be the ISO-3166
|| country code.  [In fact, for Device Management, the country code setting
|| really isn't used, but it's part of the API anyway.]
||
|| XXX We should get the default country code from the configuration
|| structure, or from an API; on a real modem, this would be stored in 
|| EEPROM
*/
#ifndef UPROTO_WMCSUBCLASS_DEVMGMT_COUNTRY_CODE
# define UPROTO_WMCSUBCLASS_DEVMGMT_COUNTRY_CODE	1	/* USA */
#endif


/* the absololute maximum payload with an OUT SETUP */
#define	UPROTO_WMCSUBCLASS_DEVMGMT_CONTROLBUF_SIZE	2048

/* the size of the encapsulated command queues -- 0 if not used */
#define	UPROTO_WMCSUBCLASS_DEVMGMT_ENCAPSQUEUE_SIZE	UPROTO_WMCTA_ENCAPSQUEUE_SIZE

/****************************************************************************\
|
|	The structures
|
\****************************************************************************/

/* forward types */
__TMS_TYPE_DEF_STRUCT(UPROTO_WMCSUBCLASS_DEVMGMT);

/*

Type:	UPROTO_WMCSUBCLASS_DEVMGMT

Function:
	The instance data structure for WMC device management subclass.

Description:
	One copy of this structure is allocated for each instance of
	a WMC sample subclass implemented by a device.  The associated code 
	terminates the subclass USB protocols, and calls an abstract upper 
	edge that is provided by the local operating system.

	This object is the upper edge of a USB DataPump protocol stack;
	it is designed to be located later by OS-specific init code.

Contents:
	UPROTO_WMCSUBCLASS_DEVMGMT is derived from UPROTO_WMCSUBCLASS, and
	therefore contains all the elements defined there.  Additional
	elements are described below.

Client-Owned/Maintained Fields:
	USBPUMP_OBJECT_HEADER *WmcSubClassHeader.pClientObject;
		The pointer to the client, or NULL.  Do not set this directly;
		instead please use USBPUMP_IOCTL_FUNCTION_OPEN.

	VOID *WmcSubClassHeader.pClientContext;
		The client-supplied context given with the
		USBPUMP_IOCTL_FUNCTION_OPEN call.

Protocol-Owned/Maintained fields:

Notes:
	Clients should attempt to avoid depending on internal implementation
	details documented above.

See Also:
	UPROTO_WMCSUBCLASS, UPROTO_WMCTA

*/

struct __TMS_STRUCTNAME(UPROTO_WMCSUBCLASS_DEVMGMT)
	{
	__TMS_UPROTO_WMCSUBCLASS_HDR;

	/* CommFeature array */
	__TMS_UINT16				CommFeatures[UPROTO_WMCSUBCLASS_ACM_NUM_FEATURES];
	};

#define	UPROTO_WMCSUBCLASS_DEVMGMT_from_UPROTO_WMCSUBCLASS(p)	\
	((UPROTO_WMCSUBCLASS_DEVMGMT *) (&(p)->WmcSubClassHdr))

/* this expands to something like UHIL_MEMTAG('W', 's', 'D', 'M') */
#define	UPROTO_WMCSUBCLASS_DEVMGMT_TAG	UPROTO_WMCSUBCLASS_TAG('D', 'M')

/* this expands to something like devmgmt.wmc.mcci.com */
#define	UPROTO_WMCSUBCLASS_DEVMGMT_NAME	UPROTO_WMCSUBCLASS_NAME("devmgmt")

/* backward compatibility for PR2829 */
#ifndef __TMS_USBPUMP_OBJECT_NAME_ENUM_DEVMGMT
# define __TMS_USBPUMP_OBJECT_NAME_ENUM_DEVMGMT	UPROTO_WMCSUBCLASS_DEVMGMT_NAME
#endif

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

extern CONST UPROTO_WMCSUBCLASS_UPCALLS			gk_WmcSubClass_DevMgmt_Upcalls;

/* the IOCTL function we export */
__TMS_USBPUMP_OBJECT_IOCTL_FN				WmcSubClass_DevMgmt_Ioctl;

/* the method functions */
UPROTOWMCSUBCLASS_ACTIVATE_FN				WmcSubClass_DevMgmt_Activate;
UPROTOWMCSUBCLASS_BUS_STATE_CHANGE_FN			WmcSubClass_DevMgmt_BusStateChange;
UPROTOWMCSUBCLASS_CREATE_FN				WmcSubClass_DevMgmt_Create;
UPROTOWMCSUBCLASS_DEACTIVATE_FN				WmcSubClass_DevMgmt_Deactivate;
UPROTOWMCSUBCLASS_DELETE_FN				WmcSubClass_DevMgmt_Delete;
UPROTOWMCSUBCLASS_PROCESS_ENCAPSULATED_COMMAND_FN	WmcSubClass_DevMgmt_ProcessEncapsulatedCommand;
UPROTOWMCSUBCLASS_PROBE_FN				WmcSubClass_DevMgmt_Probe;
UPROTOWMCSUBCLASS_PROCESS_SETUP_FN			WmcSubClass_DevMgmt_ProcessSetup;
UPROTOWMCSUBCLASS_PULL_ENCAPSULATED_REPLY_FN		WmcSubClass_DevMgmt_PullEncapsulatedReply;
UPROTOWMCSUBCLASS_START_DATA_PLANE_FN			WmcSubClass_DevMgmt_StartDataPlane;
UPROTOWMCSUBCLASS_STOP_DATA_PLANE_FN			WmcSubClass_DevMgmt_StopDataPlane;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_OBJECT_NAME_ENUM_DEVMGMT	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_DEVMGMT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of wmcdevmgmt.h ****/
#endif /* _WMCDEVMGMT_H_ */
