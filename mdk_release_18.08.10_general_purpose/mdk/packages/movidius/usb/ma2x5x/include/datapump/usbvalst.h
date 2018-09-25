/* usbvalst.h	Fri Jun  2 2000 09:48:10  tmm */

/*

Module:  usbvalst.h

Function:
	Home for va_list definition; needed to accommodate differences
	between IOS and other environments.

Version:
	V1.60e	Fri Jun  2 2000 09:48:10	tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2000 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	June 2000

Revision history:
   1.60e  Fri Jun  2 2000 09:48:10  tmm
	Module created.

*/

#ifndef _USBVALST_H_		/* prevent multiple includes */
#define _USBVALST_H_

#ifdef TARGET_CISCO
/* 
|| IOS's stdarg.h doesn't define va_list.  This is a headache,
|| because the file that does define it, types.h, pollutes
|| the namespace.  So we duplicate the necessary stuff here,
|| much to our discomfort.  It's meant to be textually 
|| identical, so that it will be easy to update if needed.
*/
#ifndef __PPC__
typedef char *va_list;          /* h/stdarg.h */
#else
  
typedef struct	
{
  char gpr;
  char fpr;
  char *overflow_arg_area;
  char *reg_save_area;
} __gnuc_va_list;

typedef __gnuc_va_list *va_list;
#endif
#endif /* TARGET_CISCO */

/**** end of usbvalst.h ****/
#endif /* _USBVALST_H_ */
