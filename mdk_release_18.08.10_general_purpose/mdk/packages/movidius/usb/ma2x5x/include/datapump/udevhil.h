/* udevhil.h	Sun May 31 2015 22:09:57 tmm */

/*

Module:  udevhil.h

Function:
	HIL functions that need knowledge of UDEVICE structure.

Version:
	V3.15b	Sun May 31 2015 22:09:57 tmm	Edit level 5

Copyright notice:
	This file copyright (C) 2000-2002, 2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2000

Revision history:
   1.52a  Wed Mar  1 2000 07:20:23  tmm
	Module created.

   1.61d  1/7/2001  tmm
	Cloak types.

   1.79a  7/12/2002  tmm
	Now need uhilevent.h.

   3.11b  Wed Feb 20 2013 04:08:39  tmm
	16761: Make it possible for the platform to override the definitions
	of UHIL_SynchronizeToDataPumpDevice() and UHIL_ReleaseEventLock().

   3.15b  Sun May 31 2015 22:09:57  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _UDEVHIL_H_		/* prevent multiple includes */
#define _UDEVHIL_H_

#ifndef _USBPUMP_H_
# include "usbpump.h"
#endif /* _USBPUMP_H_ */

#ifndef _UHILEVENT_H_
# include "uhilevent.h"
#endif /* _UHILEVENT_H_ */

#if defined(__TMS_USBPUMP_UDEVHIL_LOCAL_INCLUDE__)
# include __TMS_STRINGVAL(__TMS_USBPUMP_UDEVHIL_LOCAL_INCLUDE__)
#endif

/****************************************************************************\
|
|	Firmware stop functions from V1.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/* this seems to be architecturally dubious -- tmm */
__TMS_VOID		ttusb_FirmwareStop __TMS_P((__TMS_UDEVICE *));

/* 
|| This is a downcall to the hil, to indicate that a device knows that it
|| needs to be stopped.  (Previously it was an upcall from the hil.)
*/
__TMS_VOID		UHIL_Stop __TMS_P((__TMS_UPLATFORM *, __TMS_UDEVICE *));

__TMS_END_DECLS

/****************************************************************************\
|
| Synchronizing to the DataPump -- a set of primitives that allows the 
| DataPump to run without a thread, if desired.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(UHIL_SYNCHRONIZATION_BLOCK);

__TMS_FNTYPE_DEF(
UHIL_SYNCHRONIZED_FN,
__TMS_VOID,	(
	__TMS_UDEVICE *,
	__TMS_VOID *Context1,
	__TMS_VOID *Context2
	));

/*
|| You may need to define this yourself; if so, #define
|| __TTUSB_UHIL_SYNCHRONIZATION_BLOCK_DEFINED, and the rest of this
|| section becomes an abstract type specification.
*/
#ifndef __TTUSB_UHIL_SYNCHRONIZATION_BLOCK_DEFINED
# define __TTUSB_UHIL_SYNCHRONIZATION_BLOCK_DEFINED
struct __TMS_STRUCTNAME(UHIL_SYNCHRONIZATION_BLOCK)
	{
	__TMS_CALLBACKCOMPLETION	upsb_Event;
	__TMS_UDEVICE			*upsb_pDevice;
	__TMS_UHIL_SYNCHRONIZED_FN	*upsb_pSynchFn;
	__TMS_VOID			*upsb_pContext1;
	__TMS_VOID			*upsb_pContext2;
	};
#endif /* __TTUSB_UHIL_SYNCHRONIZATION_BLOCK_DEFINED */


/**** the functional APIs ****/
__TMS_BEGIN_DECLS

#ifndef __TMS_USBPUMP_USE_LOCAL_UHIL_SynchronizeToDataPumpDevice
# define __TMS_USBPUMP_USE_LOCAL_UHIL_SynchronizeToDataPumpDevice 0
#endif
__TMS_VOID UHIL_SynchronizeToDataPumpDevice(
	__TMS_UDEVICE *pDevice,
	__TMS_UHIL_SYNCHRONIZATION_BLOCK *,
	__TMS_UHIL_SYNCHRONIZED_FN *,
	__TMS_VOID *Context1,
	__TMS_VOID *Context2
	);

#ifndef __TMS_USBPUMP_USE_LOCAL_UHIL_ReleaseEventLock
# define __TMS_USBPUMP_USE_LOCAL_UHIL_ReleaseEventLock 0
#endif
__TMS_VOID UHIL_ReleaseEventLock __TMS_P((
	__TMS_UEVENTCONTEXT *pEvt
	));

__TMS_END_DECLS

/**** end of udevhil.h ****/
#endif /* _UDEVHIL_H_ */
