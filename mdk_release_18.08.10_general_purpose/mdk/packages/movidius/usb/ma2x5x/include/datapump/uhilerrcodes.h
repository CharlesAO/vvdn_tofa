/* uhilerrcodes.h	Thu Jun  6 2002 11:09:27  tmm */

/*

Module:  uhilerrcodes.h

Function:
	The error codes defined by the UHIL.

Version:
	V1.79a	Thu Jun  6 2002 11:09:27	tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2002 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	June 2002

Revision history:
   1.79a  Thu Jun  6 2002 11:09:27  tmm
	Module created.

*/

#ifndef _UHILERRCODES_H_		/* prevent multiple includes */
#define _UHILERRCODES_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

/* at present, UHIL error codes are defined in terms of SWC codes... */
#ifndef _UHILSWC_H_
# include "uhilswc.h"
#endif

/* 
|| Generic-layer HIL codes
*/
#define __TMS_UHILERR_SUCCESS		__TMS_UHILSWC_HILBASE

/* ABend in platform code */
#define __TMS_UHILERR_PF_END		(__TMS_UHILSWC_HILBASE | 1)

/* bad descriptor type */
#define __TMS_UHILERR_BAD_DTYPE		(__TMS_UHILSWC_HILBASE | 2)

/* bad descriptor index */
#define __TMS_UHILERR_BAD_DINDEX	(__TMS_UHILSWC_HILBASE | 3)

/* bad descriptor lang. ID */
#define __TMS_UHILERR_BAD_DLID		(__TMS_UHILSWC_HILBASE | 4)

/* Unhappiness during init -- probably a compile error */
#define	__TMS_UHILERR_INIT_FAIL		(__TMS_UHILSWC_HILBASE | 5)

/* UHIL_dispatchevent failed -- null pointer? */
#define __TMS_UHILERR_DISPATCH_FAIL	(__TMS_UHILSWC_HILBASE | 6)

/* generic code to be used when 'not-implemented' code is called */
#define	__TMS_UHILERR_NOT_IMPLEMENTED_YET (__TMS_UHILSWC_HILBASE | 7)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UHILERR_SUCCESS		__TMS_UHILERR_SUCCESS
# define UHILERR_PF_END			__TMS_UHILERR_PF_END
# define UHILERR_BAD_DTYPE		__TMS_UHILERR_BAD_DTYPE
# define UHILERR_BAD_DINDEX		__TMS_UHILERR_BAD_DINDEX
# define UHILERR_BAD_DLID		__TMS_UHILERR_BAD_DLID
# define UHILERR_INIT_FAIL		__TMS_UHILERR_INIT_FAIL
# define UHILERR_DISPATCH_FAIL		__TMS_UHILERR_DISPATCH_FAIL
# define UHILERR_NOT_IMPLEMENTED_YET	__TMS_UHILERR_NOT_IMPLEMENTED_YET
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of uhilerrcodes.h ****/
#endif /* _UHILERRCODES_H_ */
