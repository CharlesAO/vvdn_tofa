/* uintstruct.h	Mon May 27 2002 23:06:39  tmm */

/*

Module:  uintstruct.h

Function:
	Home for (deprecated) UINTSTRUCT and related structures.

Version:
	V1.79a	Mon May 27 2002 23:06:39	tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2002 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.79a  Mon May 27 2002 23:06:39  tmm
	Module created.

*/

#ifndef _UINTSTRUCT_H_		/* prevent multiple includes */
#define _UINTSTRUCT_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

/****************************************************************************\
|
|	Interrupt register structures can be used to simplify the handling
|	of interrupts.
|
\****************************************************************************/

__TMS_TYPE_DEF(UINTSTRUCT, struct TTUSB_UINTSTRUCT);
__TMS_TYPE_DEF(UINTHANDLE, __TMS_PUINTSTRUCT);

__TMS_FNTYPE_DEF(
UINTRELOADFN,
	__TMS_VOID,
		(
		__TMS_PUINTSTRUCT
		)
	);

struct TTUSB_UINTSTRUCT
	{
	__TMS_UINT32	uis_magic;		/* the signature */
	__TMS_UDEVICE	*uis_udev;		/* pointer to the device */
	__TMS_PUEVENTNODE uis_reloadq;	/* pointer to user event queue. */
	__TMS_UEVENTNODE uis_event;		/* for getting relevant events */
	__TMS_UBUFQE	uis_uqe;		/* the queue element we are to use */
	};

#define	UINTSTRUCT_MAGIC	((__TMS_UINT32) 0xCC970606)

__TMS_BEGIN_DECLS

__TMS_UINTHANDLE UsbCreateIntRegisters __TMS_P(( \
			__TMS_UINTSTRUCT *, \
			__TMS_BYTES /* sizeof UINTSTRUCT */, \
			__TMS_UDEVICE *, __TMS_UPIPE *, \
			__TMS_VOID * /*buf*/, \
			__TMS_BYTES /*sizeof buf*/, \
			__TMS_PUEVENTNODE reloadq
			));

__TMS_BOOL	UsbStartIntRegisters __TMS_P((__TMS_UINTHANDLE));
__TMS_VOID	UsbStopIntRegisters __TMS_P((__TMS_UINTHANDLE));
__TMS_END_DECLS

/**** end of uintstruct.h ****/
#endif /* _UINTSTRUCT_H_ */
