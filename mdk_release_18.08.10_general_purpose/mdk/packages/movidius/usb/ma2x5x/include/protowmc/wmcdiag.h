/* wmcdiag.h	Wed Mar 18 2009 12:28:21 chwon */

/*

Module:  wmcdiag.h

Function:
	WMC TA Diag SubClass header file.

Version:
	V2.01a	Wed Mar 18 2009 12:28:21 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2005, 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2005

Revision history:
   1.91b  Thu Mar 17 2005 11:58:22  chwon
	Module created.

   2.01a  Wed Mar 18 2009 12:28:21  chwon
	2829: move out object name from usbpumpobjectname.h to here.

*/

#ifndef _WMCDIAG_H_		/* prevent multiple includes */
#define _WMCDIAG_H_

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
#define	UPROTO_WMCSUBCLASS_DIAG_CONTROLBUF_SIZE		2048

/* the size of the encapsulated command queues -- 0 if not used */
#define	UPROTO_WMCSUBCLASS_DIAG_ENCAPSQUEUE_SIZE	0

/****************************************************************************\
|
|	The structures
|
\****************************************************************************/

/* forward types */
__TMS_TYPE_DEF_STRUCT(UPROTO_WMCSUBCLASS_DIAG);

/*

Type:	UPROTO_WMCSUBCLASS_DIAG

Function:
	The instance data structure for WMC DIAG subclass.

Description:
	One copy of this structure is allocated for each instance of
	a WMC DIAG subclass implemented by a device.  The associated code
	terminates the subclass USB protocols, and calls an abstract upper
	edge that is provided by the local operating system.

	This object is the upper edge of a USB DataPump protocol stack;
	it is designed to be located later by OS-specific init code.

Contents:
	UPROTO_WMCSUBCLASS_DIAG is derived from UPROTO_WMCSUBCLASS, and
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
	UDATASTREAM *pInStream;
		Points to the datastream for the IN pipe.  This is private.
		Clients should use USBPUMP_IOCTL_MODEM_QUEUE_IN to submit
		UBUFQEs.

	UDATASTREAM *pOutStream;
		Points to the datastream for the OUT pipe.  This is
		private; clients should use USBPUMP_IOCTL_MODEM_QUEUE_OUT
		to submit UBUFQEs.

Notes:
	Clients should attempt to avoid depending on internal implementation
	details documented above.

See Also:
	UPROTO_WMCSUBCLASS, UPROTO_WMCTA

*/

struct __TMS_STRUCTNAME(UPROTO_WMCSUBCLASS_DIAG)
	{
	__TMS_UPROTO_WMCSUBCLASS_HDR;

	/* the specific contents for DIAG TAs */

	/*
	|| The in datastream
	*/
	__TMS_UDATASTREAM			*pInStream;

	/*
	|| The OUT datastream
	*/
	__TMS_UDATASTREAM			*pOutStream;
	};

#define	UPROTO_WMCSUBCLASS_DIAG_from_UPROTO_WMCSUBCLASS(p)	\
	((__TMS_UPROTO_WMCSUBCLASS_DIAG *) (&(p)->WmcSubClassHdr))

/* this expands to something like UHIL_MEMTAG('W', 's', 'E', 'M') */
#define	UPROTO_WMCSUBCLASS_DIAG_TAG	UPROTO_WMCSUBCLASS_TAG('E', 'M')

/* this expands to something like diag.wmc.mcci.com */
#define	UPROTO_WMCSUBCLASS_DIAG_NAME	UPROTO_WMCSUBCLASS_NAME("diag")

/* backward compatibility for PR2829 */
#ifndef __TMS_USBPUMP_OBJECT_NAME_ENUM_DIAG
# define __TMS_USBPUMP_OBJECT_NAME_ENUM_DIAG	UPROTO_WMCSUBCLASS_DIAG_NAME
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

/* the upcall table from the class to the subclass */
extern __TMS_CONST __TMS_UPROTO_WMCSUBCLASS_UPCALLS	gk_WmcSubClass_Diag_Upcalls;

/* the IOCTL function we export */
__TMS_USBPUMP_OBJECT_IOCTL_FN				WmcSubClass_Diag_Ioctl;

/* the method functions */
__TMS_UPROTOWMCSUBCLASS_ACTIVATE_FN			WmcSubClass_Diag_Activate;
__TMS_UPROTOWMCSUBCLASS_BUS_STATE_CHANGE_FN		WmcSubClass_Diag_BusStateChange;
__TMS_UPROTOWMCSUBCLASS_CREATE_FN			WmcSubClass_Diag_Create;
__TMS_UPROTOWMCSUBCLASS_DEACTIVATE_FN			WmcSubClass_Diag_Deactivate;
__TMS_UPROTOWMCSUBCLASS_DELETE_FN			WmcSubClass_Diag_Delete;
__TMS_UPROTOWMCSUBCLASS_PROCESS_ENCAPSULATED_COMMAND_FN	WmcSubClass_Diag_ProcessEncapsulatedCommand;
__TMS_UPROTOWMCSUBCLASS_PROBE_FN			WmcSubClass_Diag_Probe;
__TMS_UPROTOWMCSUBCLASS_PROCESS_SETUP_FN		WmcSubClass_Diag_ProcessSetup;
__TMS_UPROTOWMCSUBCLASS_PULL_ENCAPSULATED_REPLY_FN	WmcSubClass_Diag_PullEncapsulatedReply;
__TMS_UPROTOWMCSUBCLASS_START_DATA_PLANE_FN		WmcSubClass_Diag_StartDataPlane;
__TMS_UPROTOWMCSUBCLASS_STOP_DATA_PLANE_FN		WmcSubClass_Diag_StopDataPlane;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_OBJECT_NAME_ENUM_DIAG	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_DIAG
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of wmcdiag.h ****/
#endif /* _WMCDIAG_H_ */
