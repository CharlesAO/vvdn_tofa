/* wmcskl.h	Fri Jul 26 2002 10:50:19 tmm */

/*

Module:  wmcskl.h

Function:
	Sample (skeletal) WMC TA SubClass header file.

Version:
	V1.79a	Fri Jul 26 2002 10:50:19 tmm	Edit level 5

Copyright notice:
	This file copyright (C) 2002 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2002

Revision history:
   1.79a  2002/03/08  tmm
	Cloned from MCPCMODE_SKL version 1.61j

*/

#ifndef _WMCSKL_H_		/* prevent multiple includes */
#define _WMCSKL_H_

#ifndef _PROTOWMC_H_
# include "protowmc.h"
#endif

#ifndef _WMCSUBCLASSLIB_H_
# include "wmcsubclasslib.h"
#endif

/****************************************************************************\
|
|	The parameters 
|
\****************************************************************************/

/* the absololute maximum payload with an OUT SETUP */
#define	UPROTO_WMCSUBCLASS_SKL_CONTROLBUF_SIZE	2048

/* the size of the encapsulated command queues -- 0 if not used */
#define	UPROTO_WMCSUBCLASS_SKL_ENCAPSQUEUE_SIZE	UPROTO_WMCTA_ENCAPSQUEUE_SIZE

/****************************************************************************\
|
|	The structures
|
\****************************************************************************/

/* forward types */
__TMS_TYPE_DEF_STRUCT(UPROTO_WMCSUBCLASS_SKL);

/*

Type:	UPROTO_WMCSUBCLASS_SKL

Function:
	The instance data structure for WMC sample subclass.

Description:
	One copy of this structure is allocated for each instance of
	a WMC sample subclass implemented by a device.  The associated code 
	terminates the subclass USB protocols, and calls an abstract upper 
	edge that is provided by the local operating system.

	This object is the upper edge of a USB DataPump protocol stack;
	it is designed to be located later by OS-specific init code.

Contents:
	UPROTO_WMCSUBCLASS_SKL is derived from UPROTO_WMCSUBCLASS, and
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

struct __TMS_STRUCTNAME(UPROTO_WMCSUBCLASS_SKL)
	{
	__TMS_UPROTO_WMCSUBCLASS_HDR;

	/* the specific contents for <this kind of subclass> */
	};

#define	UPROTO_WMCSUBCLASS_SKL_from_UPROTO_WMCSUBCLASS(p)	\
	((UPROTO_WMCSUBCLASS_SKL *) (&(p)->WmcSubClassHdr))

/* this expands to something like UHIL_MEMTAG('W', 's', 'S', 'K') */
#define	UPROTO_WMCSUBCLASS_SKL_TAG	UPROTO_WMCSUBCLASS_TAG('S', 'K')

/* this expands to something like skl.wmc.mcci.com */
#define	UPROTO_WMCSUBCLASS_SKL_NAME	UPROTO_WMCSUBCLASS_NAME("skl")

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

/* the upcall table from the class to the subclass */
extern CONST UPROTO_WMCSUBCLASS_UPCALLS			gk_WmcSubClass_Skl_Upcalls;

/* the IOCTL function we export */
__TMS_USBPUMP_OBJECT_IOCTL_FN				WmcSubClass_Skl_Ioctl;

/* the method functions */
UPROTOWMCSUBCLASS_ACTIVATE_FN				WmcSubClass_Skl_Activate;
UPROTOWMCSUBCLASS_BUS_STATE_CHANGE_FN			WmcSubClass_Skl_BusStateChange;
UPROTOWMCSUBCLASS_CREATE_FN				WmcSubClass_Skl_Create;
UPROTOWMCSUBCLASS_DEACTIVATE_FN				WmcSubClass_Skl_Deactivate;
UPROTOWMCSUBCLASS_DELETE_FN				WmcSubClass_Skl_Delete;
UPROTOWMCSUBCLASS_PROCESS_ENCAPSULATED_COMMAND_FN	WmcSubClass_Skl_ProcessEncapsulatedCommand;
UPROTOWMCSUBCLASS_PROBE_FN				WmcSubClass_Skl_Probe;
UPROTOWMCSUBCLASS_PROCESS_SETUP_FN			WmcSubClass_Skl_ProcessSetup;
UPROTOWMCSUBCLASS_PULL_ENCAPSULATED_REPLY_FN		WmcSubClass_Skl_PullEncapsulatedReply;
UPROTOWMCSUBCLASS_START_DATA_PLANE_FN		WmcSubClass_Skl_StartDataPlane;
UPROTOWMCSUBCLASS_STOP_DATA_PLANE_FN		WmcSubClass_Skl_StopDataPlane;

__TMS_END_DECLS

/**** end of wmcskl.h ****/
#endif /* _WMCSKL_H_ */
