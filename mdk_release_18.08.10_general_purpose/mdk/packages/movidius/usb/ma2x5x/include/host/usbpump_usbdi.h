/* usbpump_usbdi.h	Sun Aug 31 2008 14:56:56 djt */

/*

Module:  usbpump_usbdi.h

Function:
	The central USBD API header file

Version:
	V1.97k	Sun Aug 31 2008 14:56:56 djt	Edit level 6

Copyright notice:
	This file copyright (C) 2005-2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Sat Jun 18 2005 23:53:43  tmm
	Module created.

   1.97j  Tue Oct 16 2007 13:21:33  djt
	3265: Added usbpump_usbdtti_api.h, usbpump_usbdi_tt.h,
	usbpump_usbdi_ttclass.h

   1.97k  Thu Dec 13 2007 17:47:37  djt
	3049: Added usbpump_usbdcdi_api.h

   1.97k  Mon Mar 10 2008 19:17:59  djt
	3049: Undid previous change.

   1.97k  Tue Jul 15 2008 15:59:31  chwon
	4235: fix typo; _USBPUMP_USBDI_TTCLASS_H_ -> _USBPUMP_USBDI_TT_H_

   1.97k  Sun Aug 31 2008 14:56:56  djt
	UDP-6427: Removed tt header files.

*/

#ifndef _USBPUMP_USBDI_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

#ifndef _USBPUMP_USBDI_API_H_
# include "usbpump_usbdi_api.h"
#endif

#ifndef _USBPUMP_USBDI_CLASS_H_
# include "usbpump_usbdi_class.h"
#endif

#ifndef _USBPUMP_USBDI_DRVIMPL_H_
# include "usbpump_usbdi_drvimpl.h"
#endif

#ifndef _USBPUMP_USBDI_FUNCTION_H_
# include "usbpump_usbdi_function.h"
#endif

#ifndef _USBPUMP_USBDI_INIT_H_
# include "usbpump_usbdi_init.h"
#endif

#ifndef _USBPUMP_USBDI_IOCTLS_H_
# include "usbpump_usbdi_ioctls.h"
#endif

#ifndef _USBPUP_USBDI_PORT_H_
# include "usbpump_usbdi_port.h"
#endif

#ifndef _USBPUMP_USBDI_REQUEST_H_
# include "usbpump_usbdi_request.h"
#endif

#ifndef _USBPUMP_USBDI_USBD_H_
# include "usbpump_usbdi_usbd.h"
#endif

/**** end of usbpump_usbdi.h ****/
#endif /* _USBPUMP_USBDI_H_ */
