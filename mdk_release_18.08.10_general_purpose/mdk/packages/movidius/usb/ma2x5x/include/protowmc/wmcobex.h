/* wmcobex.h	Wed Mar 18 2009 12:28:33 chwon */

/*

Module:  wmcobex.h

Function:
	WMC TA Obex SubClass header file.

Version:
	V2.01a	Wed Mar 18 2009 12:28:33 chwon	Edit level 7

Copyright notice:
	This file copyright (C) 2002, 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  2002/07/08  tmm
	Cloned from wmcskl.h

   2.01a  Wed Mar 18 2009 12:28:33  chwon
	2829: move out object name from usbpumpobjectname.h to here.

*/

#ifndef _WMCOBEX_H_		/* prevent multiple includes */
#define _WMCOBEX_H_

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
#define	UPROTO_WMCSUBCLASS_OBEX_CONTROLBUF_SIZE	2048

/* the size of the encapsulated command queues -- 0 if not used */
#define	UPROTO_WMCSUBCLASS_OBEX_ENCAPSQUEUE_SIZE	0

/****************************************************************************\
|
|	The structures
|
\****************************************************************************/

/* forward types */
__TMS_TYPE_DEF_STRUCT(UPROTO_WMCSUBCLASS_OBEX);

/*

Type:	UPROTO_WMCSUBCLASS_OBEX

Function:
	The instance data structure for WMC OBEX subclass.

Description:
	One copy of this structure is allocated for each instance of
	a WMC OBEX subclass implemented by a device.  The associated code 
	terminates the subclass USB protocols, and calls an abstract upper 
	edge that is provided by the local operating system.

	This object is the upper edge of a USB DataPump protocol stack;
	it is designed to be located later by OS-specific init code.

Contents:
	UPROTO_WMCSUBCLASS_OBEX is derived from UPROTO_WMCSUBCLASS, and
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

struct __TMS_STRUCTNAME(UPROTO_WMCSUBCLASS_OBEX)
	{
	__TMS_UPROTO_WMCSUBCLASS_HDR;

	/* the specific contents for OBEX TAs */

	/*
	|| The in datastream
	*/
	__TMS_UDATASTREAM			*pInStream;

	/*
	|| The OUT datastream
	*/
	__TMS_UDATASTREAM			*pOutStream;
	};

#define	UPROTO_WMCSUBCLASS_OBEX_from_UPROTO_WMCSUBCLASS(p)	\
	((UPROTO_WMCSUBCLASS_OBEX *) (&(p)->WmcSubClassHdr))

/* this expands to something like UHIL_MEMTAG('W', 's', 'O', 'X') */
#define	UPROTO_WMCSUBCLASS_OBEX_TAG	UPROTO_WMCSUBCLASS_TAG('O', 'X')

/* this expands to something like obex.wmc.mcci.com */
#define	UPROTO_WMCSUBCLASS_OBEX_NAME	UPROTO_WMCSUBCLASS_NAME("obex")

/* backward compatibility for PR2829 */
#ifndef __TMS_USBPUMP_OBJECT_NAME_ENUM_OBEX
# define __TMS_USBPUMP_OBJECT_NAME_ENUM_OBEX	UPROTO_WMCSUBCLASS_OBEX_NAME
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
extern CONST UPROTO_WMCSUBCLASS_UPCALLS			gk_WmcSubClass_Obex_Upcalls;

/* the IOCTL function we export */
__TMS_USBPUMP_OBJECT_IOCTL_FN				WmcSubClass_Obex_Ioctl;

/* the method functions */
UPROTOWMCSUBCLASS_ACTIVATE_FN				WmcSubClass_Obex_Activate;
UPROTOWMCSUBCLASS_BUS_STATE_CHANGE_FN			WmcSubClass_Obex_BusStateChange;
UPROTOWMCSUBCLASS_CREATE_FN				WmcSubClass_Obex_Create;
UPROTOWMCSUBCLASS_DEACTIVATE_FN				WmcSubClass_Obex_Deactivate;
UPROTOWMCSUBCLASS_DELETE_FN				WmcSubClass_Obex_Delete;
UPROTOWMCSUBCLASS_PROCESS_ENCAPSULATED_COMMAND_FN	WmcSubClass_Obex_ProcessEncapsulatedCommand;
UPROTOWMCSUBCLASS_PROBE_FN				WmcSubClass_Obex_Probe;
UPROTOWMCSUBCLASS_PROCESS_SETUP_FN			WmcSubClass_Obex_ProcessSetup;
UPROTOWMCSUBCLASS_PULL_ENCAPSULATED_REPLY_FN		WmcSubClass_Obex_PullEncapsulatedReply;
UPROTOWMCSUBCLASS_START_DATA_PLANE_FN			WmcSubClass_Obex_StartDataPlane;
UPROTOWMCSUBCLASS_STOP_DATA_PLANE_FN			WmcSubClass_Obex_StopDataPlane;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_OBJECT_NAME_ENUM_OBEX	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_OBEX
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of wmcobex.h ****/
#endif /* _WMCOBEX_H_ */
