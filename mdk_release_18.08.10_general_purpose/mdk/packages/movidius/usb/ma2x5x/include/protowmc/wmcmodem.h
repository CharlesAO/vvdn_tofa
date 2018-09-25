/* wmcmodem.h	Wed Mar 18 2009 12:28:29 chwon */

/*

Module:  wmcmodem.h

Function:
	WMC TA SubClass header file for Abstract Control Model modems.

Version:
	V2.01a	Wed Mar 18 2009 12:28:29 chwon	Edit level 12

Copyright notice:
	This file copyright (C) 2002-2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	March 2002

Revision history:
   1.63f  2002/03/08  tmm
	Created, with input from mcpcmode_modem.h V1_62c.

   1.81a  Thu Feb 20 2003 03:07:50  tmm
	Factor out common CommFeature definitions to wmcfeature.h.

   1.91c  Thu Apr 28 2005 10:52:35 maw
   	Bug 1463: Added Flags & PlaneStartState field to context

   1.97e  Wed Aug 09 2006 17:01:08  tmm
	1860: fix macros UPROTO_WMCSUBCLASS_MODEM_PLANESTART_GET
	and UPROTO_WMCSUBCLASS_MODEM_PLANESTART_SET to match
	style guidelines.

   1.97k  Wed Feb 27 2008 11:20:56  chwon
	3844: remove __value in the macro definition.
	Add __TMS_ for type cloaking support

   2.01a  Wed Mar 18 2009 12:28:29  chwon
	2829: move out object name from usbpumpobjectname.h to here.

*/

#ifndef _WMCMODEM_H_		/* prevent multiple includes */
#define _WMCMODEM_H_

#ifndef _PROTOWMC_H_
# include "protowmc.h"
#endif

#ifndef _WMCSUBCLASSLIB_H_
# include "wmcsubclasslib.h"
#endif

#ifndef _CDCLINECODING_H_
# include "cdclinecoding.h"
#endif

#ifndef _USBCDC11_H_
# include "usbcdc11.h"
#endif

#ifndef _WMCFEATURE_H_
# include "wmcfeature.h"
#endif

/****************************************************************************\
|
|	Some parameters
|
\****************************************************************************/

/* 
|| The API requires that we reset the country code to the default when we 
|| get a CLEAR_FEATURE, reset, etc.   This is supposed to be the ISO-3166
|| country code.  
||
|| XXX We get the default country code from the configuration
|| structure, or from an API; on a real modem, this would be stored in 
|| EEPROM
*/
#ifndef UPROTO_WMCSUBCLASS_MODEM_COUNTRY_CODE
# define UPROTO_WMCSUBCLASS_MODEM_COUNTRY_CODE	1	/* USA */
#endif

/* the absololute maximum payload with an OUT SETUP */
#define	UPROTO_WMCSUBCLASS_MODEM_CONTROLBUF_SIZE	2048

/* the size of the encapsulated command queues -- 0 if not used */
#define	UPROTO_WMCSUBCLASS_MODEM_ENCAPSQUEUE_SIZE	\
	__TMS_UPROTO_WMCTA_ENCAPSQUEUE_SIZE

/*
|| When a modem enumerates, then a DataPlane start followed by a stop event is
|| send up thru the system. 
|| This sometimes causes problems for clients where connections are brought up 
|| on the DataPlane start-event, but immediately dropped on the stop-event. 
|| Therefore the PlaneStartState variable is added to filter out StartDataPlane 
|| and StopDataPlane events before an application on the host really has opened 
|| the driver. This is detected by tracking control requests
*/
#define __TMS_UPROTO_WMCSUBCLASS_MODEM_PLANESTART_NOT_BLOCKED	0
#define __TMS_UPROTO_WMCSUBCLASS_MODEM_PLANESTART_CTRL_PENDING	1
#define __TMS_UPROTO_WMCSUBCLASS_MODEM_PLANESTART_DATA_PENDING	2

#define __TMS_UPROTO_WMCSUBCLASS_MODEM_PLANESTART_GET(a_p, a_f)			\
	(((a_p)->PlaneStartState >> (a_f)) & 1)

#define __TMS_UPROTO_WMCSUBCLASS_MODEM_PLANESTART_SET(a_p, a_f, a_v)		\
   do	{									\
	if (! (a_v))								\
		(a_p)->PlaneStartState &= ~(1u << (a_f));			\
	else									\
		(a_p)->PlaneStartState |= (1u << (a_f));			\
	} while (0)

/****************************************************************************\
|
|	The feature mapping macros
|
\****************************************************************************/


/*

Name:	UPROTO_WMCSUBCLASS_MODEM_FEATURE

Index:	Name:	UPROTO_WMCSUBCLASS_MODEM_iFEATURE
	Name:	UPROTO_WMCSUBCLASS_MODEM_pFEATURE
	Name:	UPROTO_WMCSUBCLASS_MODEM_IS_VALID_FEATURE
	Name:	UPROTO_WMCSUBCLASS_MODEM_NUM_FEATURES

Function:
	Macros for accessing abstract features in WMC MODEMs

Definition:
	UINT UPROTO_WMCSUBCLASS_MODEM_iFEATURE(
		UPROTO_WMCSUBCLASS_MODEM *pWmcModem, 
		UINT16 wValue
		);

	UINT16 *UPROTO_WMCSUBCLASS_MODEM_pFEATURE(
		UPROTO_WMCSUBCLASS_MODEM *pWmcModem, 
		UINT16 wValue
		);

	BOOL UPROTO_WMCSUBCLASS_MODEM_IS_VALID_FEATURE(
		UPROTO_WMCSUBCLASS_MODEM *pWmcModem, 
		UINT16 wValue
		);

	CONST UINT UPROTO_WMCSUBCLASS_MODEM_NUM_FEATURES;

Description:
	The CDC ACM specification defines a variety of abstract features,
	controlled by the SET/CLEAR/GET_COMM_FEATURE class command.

	The UPROTO_WMCSUBCLASS_MODEM implementation represents each feature
	as a location in an array of features.  Each feature has an index,
	and a slot.  The feature index is computed (from the SETUP packet
	selector wValue) using UPROTO_WMCSUBCLASS_MODEM_iFEATURE() macro.  A 
	SETUP packet wValue is checked for validity using
	UPROTO_WMCSUBCLASS_MODEM_IS_FEATURE_VALID().  The storage locaton 
	for the feature value is found using 
	UPROTO_WMCSUBCLASS_MODEM_pFEATURES.
	The number of distinct features is defined by
	UPROTO_WMCSUBCLASS_MODEM_NUM_FEATURES.

Returns:
	See description.

*/

#define	__TMS_UPROTO_WMCSUBCLASS_MODEM_iFEATURE(pWmcModem, wValue)	\
	__TMS_UPROTO_WMCSUBCLASS_ACM_iFEATURE(pWmcModem, wValue)

#define	__TMS_UPROTO_WMCSUBCLASS_MODEM_pFEATURE(pWmcModem, wValue)	\
	__TMS_UPROTO_WMCSUBCLASS_ACM_pFEATURE(pWmcModem, (pWmcModem)->CommFeatures, wValue)

#define	__TMS_UPROTO_WMCSUBCLASS_MODEM_IS_VALID_FEATURE(pWmcModem, wValue) \
	__TMS_UPROTO_WMCSUBCLASS_ACM_IS_VALID_FEATURE(pWmcModem, wValue)

#define	__TMS_UPROTO_WMCSUBCLASS_MODEM_NUM_FEATURES			\
	__TMS_UPROTO_WMCSUBCLASS_ACM_NUM_FEATURES

/****************************************************************************\
|
|	The structures
|
\****************************************************************************/

/* forward types */
__TMS_TYPE_DEF_STRUCT(UPROTO_WMCSUBCLASS_MODEM);

/*

Type:	UPROTO_WMCSUBCLASS_MODEM

Function:
	The instance data structure for WMC modems.

Description:
	One copy of this structure is allocated for each instance of
	a WMC modem implemented by a device.  The associated code terminates
	the modem USB protocols, and calls an abstract upper edge that
	is provided by the local operating system.

	This object is the upper edge of a USB DataPump protocol stack;
	it is designed to be located later by OS-specific init code.

Contents:
	UPROTO_WMCSUBCLASS_MODEM is derived from UPROTO_WMCSUBCLASS, and
	therefore contains all the elements defined there.  Additional
	elements are described below.

Client-Owned/Maintained Fields:
	USBPUMP_OBJECT_HEADER *WmcSubClassHeader.pClientObject;
		The pointer to the client, or NULL.  Do not set this directly;
		instead please use USBPUMP_IOCTL_FUNCTION_OPEN/CLOSE.

	VOID *WmcSubClassHeader.pClientContext;
		The client-supplied context given with the
		USBPUMP_IOCTL_FUNCTION_OPEN call.

Protocol-Owned/Maintained fields:
	UINT16 CommFeatures[UPROTO_WMCSUBCLASS_MODEM_NUM_FEATURES];
		An array that specifies the values for each of the
		features defined by the ACM spec.  These are 0-origin;
		ACM features are 1 origin.  Use 
		UPROTO_WMCSUBCLASS_MODEM_pFEATURE, etc., to access these.

	UINT16 DefaultCountryCode;
		The country code to be used by default after a USB reset
		or other adapter initialization event.  Clients: can change
		to an appropriate default value during initialization.

	USB_Comm_LINE_CODING LineCoding;
		Line coding most recently set by the host, as defined by
		the USB CDC ACM spec.  Clients: read-only.

	UINT8 ControlLineState;
		The simulated DTR and RTS lines, as defined by the USB CDC
		ACM spec.  Clients: read only.

	UINT8 BreakState;
		The current state of the break feature.  0 --> no break;
		non-zero --> break.  Clients: read-only.

	UINT8 UartState;
		The current UART state.  These bits are as defined by the
		CDC ACM Serial-State notification; however, the deltas are
		tracked separately.  Clients: read-only; use 
		USBPUMP_IOCTL_MODEM_SET_UART_STATE to change this.

	UINT8 UartStateDelta;
		This tracks the changes in the UartState.  Clients: read-only.
		Call USBPUMP_IOCTL_MODEM_SET_UART_STATE to change this.

	BITS Flags;
		Copy of modem-configuration Flags field
		
	UINT8 PlaneStartState;
		This tracks the PlaneStart state, so that CtrlPlaneStart/Stop &
		DataPlaneStart/Stop can be filtered out until a control-request
		other that SET/GET/CLEAR-COMMFEATURE(ABSTRACT_STATE ..) is found

Notes:
	Clients should attempt to avoid depending on internal implementation
	details documented above.

See Also:
	UPROTO_WMCSUBCLASS, UPROTO_WMCTA, UPROTO_WMCSUBCLASS_MODEM_FEATURE 
	macros

*/

struct __TMS_STRUCTNAME(UPROTO_WMCSUBCLASS_MODEM)
	{
	__TMS_UPROTO_WMCSUBCLASS_HDR;

	/* the specific contents for <this kind of mode> */

	/**** BEGIN client: read-only fields! ****/

	/* CommFeature array */
	__TMS_UINT16	CommFeatures[__TMS_UPROTO_WMCSUBCLASS_ACM_NUM_FEATURES];

	/* some default values */
	__TMS_UINT16				DefaultCountryCode;

	/* the line coding */
	__TMS_USB_Comm_LINE_CODING		LineCoding;

	/* the current control line state */
	__TMS_UINT8				ControlLineState;

	/* the current state of the break */
	__TMS_UINT8				BreakState;

	/* the current "uart state" */
	__TMS_UINT8				UartState;

	/* the current "delta tracker" */
	__TMS_UINT8				UartStateDelta;

	/* 
	|| We need a UBUFQE for the serial-state notification; the buffer is 
	|| allocated dynamically from device pool, and a pointer is left in
	|| SerialStateQe.uqe_buf.
	*/
	__TMS_UBUFQE				SerialStateQe;
	
	/*
	|| Copy of modem configuration Flags
	*/
	__TMS_BITS				Flags;
	
	/**** END client: read-only fields! ****/

	/*
	|| Start and stop filtering state
	*/
	__TMS_UINT8				PlaneStartState;

	/*
	|| The in datastream
	*/
	__TMS_UDATASTREAM			*pInStream;

	/*
	|| The OUT datastream
	*/
	__TMS_UDATASTREAM			*pOutStream;
	};

#define	UPROTO_WMCSUBCLASS_MODEM_from_UPROTO_WMCSUBCLASS(p)		\
	((__TMS_UPROTO_WMCSUBCLASS_MODEM *) (&(p)->WmcSubClassHdr))

/* this expands to something like UHIL_MEMTAG('W', 's', 'M', 'D') */
#define	UPROTO_WMCSUBCLASS_MODEM_TAG	__TMS_UPROTO_WMCSUBCLASS_TAG('M', 'D')

/* this expands to something like modem.wmc.mcci.com */
#define	UPROTO_WMCSUBCLASS_MODEM_NAME	__TMS_UPROTO_WMCSUBCLASS_NAME("modem")

/* backward compatibility for PR2829 */
#ifndef __TMS_USBPUMP_OBJECT_NAME_ENUM_MODEM
# define __TMS_USBPUMP_OBJECT_NAME_ENUM_MODEM	UPROTO_WMCSUBCLASS_MODEM_NAME
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
extern __TMS_CONST __TMS_UPROTO_WMCSUBCLASS_UPCALLS	gk_WmcSubClass_Modem_Upcalls;

/* the IOCTL function we export */
__TMS_USBPUMP_OBJECT_IOCTL_FN				WmcSubClass_Modem_Ioctl;

/* the method functions */
 __TMS_UPROTOWMCSUBCLASS_ACTIVATE_FN			WmcSubClass_Modem_Activate;
/* __TMS_UPROTOWMCSUBCLASS_BUS_STATE_CHANGE_FN		WmcSubClass_Modem_BusStateChange; */
__TMS_UPROTOWMCSUBCLASS_CREATE_FN			WmcSubClass_Modem_Create;
/* __TMS_UPROTOWMCSUBCLASS_DEACTIVATE_FN		WmcSubClass_Modem_Deactivate; */
__TMS_UPROTOWMCSUBCLASS_DELETE_FN			WmcSubClass_Modem_Delete;
__TMS_UPROTOWMCSUBCLASS_PROCESS_ENCAPSULATED_COMMAND_FN	WmcSubClass_Modem_ProcessEncapsulatedCommand;
__TMS_UPROTOWMCSUBCLASS_PROBE_FN			WmcSubClass_Modem_Probe;
__TMS_UPROTOWMCSUBCLASS_PROCESS_SETUP_FN		WmcSubClass_Modem_ProcessSetup;
__TMS_UPROTOWMCSUBCLASS_PULL_ENCAPSULATED_REPLY_FN	WmcSubClass_Modem_PullEncapsulatedReply;
__TMS_UPROTOWMCSUBCLASS_START_DATA_PLANE_FN		WmcSubClass_Modem_StartDataPlane;
__TMS_UPROTOWMCSUBCLASS_STOP_DATA_PLANE_FN		WmcSubClass_Modem_StopDataPlane;

__TMS_END_DECLS

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_WMCSUBCLASS_MODEM_PLANESTART_NOT_BLOCKED	\
   __TMS_UPROTO_WMCSUBCLASS_MODEM_PLANESTART_NOT_BLOCKED
# define UPROTO_WMCSUBCLASS_MODEM_PLANESTART_CTRL_PENDING	\
   __TMS_UPROTO_WMCSUBCLASS_MODEM_PLANESTART_CTRL_PENDING
# define UPROTO_WMCSUBCLASS_MODEM_PLANESTART_DATA_PENDING	\
   __TMS_UPROTO_WMCSUBCLASS_MODEM_PLANESTART_DATA_PENDING
# define UPROTO_WMCSUBCLASS_MODEM_PLANESTART_GET(a_p, a_f)	\
   __TMS_UPROTO_WMCSUBCLASS_MODEM_PLANESTART_GET(a_p, a_f)
# define UPROTO_WMCSUBCLASS_MODEM_PLANESTART_SET(a_p, a_f, a_v)	\
   __TMS_UPROTO_WMCSUBCLASS_MODEM_PLANESTART_SET(a_p, a_f, a_v)
# define UPROTO_WMCSUBCLASS_MODEM_iFEATURE(pWmcModem, wValue)	\
   __TMS_UPROTO_WMCSUBCLASS_MODEM_iFEATURE(pWmcModem, wValue)
# define UPROTO_WMCSUBCLASS_MODEM_pFEATURE(pWmcModem, wValue)	\
   __TMS_UPROTO_WMCSUBCLASS_MODEM_pFEATURE(pWmcModem, wValue)
# define UPROTO_WMCSUBCLASS_MODEM_IS_VALID_FEATURE(pWmcModem, wValue)	\
   __TMS_UPROTO_WMCSUBCLASS_MODEM_IS_VALID_FEATURE(pWmcModem, wValue)
# define UPROTO_WMCSUBCLASS_MODEM_NUM_FEATURES	\
   __TMS_UPROTO_WMCSUBCLASS_MODEM_NUM_FEATURES
# define USBPUMP_OBJECT_NAME_ENUM_MODEM	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_MODEM
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of wmcmodem.h ****/
#endif /* _WMCMODEM_H_ */
