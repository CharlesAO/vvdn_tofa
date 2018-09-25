/* usbpumpobjectname.h	Mon Jun  1 2015 00:20:58 tmm */

/*

Module:  usbpumpobjectname.h

Function:
	Central definitions for USB DataPump IOCTL classes.

Version:
	V3.15b	Mon Jun  1 2015 00:20:58 tmm	Edit level 9

Copyright notice:
	This file copyright (C) 2004-2006, 2009, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Mats Webjorn, MCCI Corporation	March 2004

Revision history:
   1.87b  Tue March 23 2004 09:20:00 maw
	Module created.

   1.89a  Tue Jun 08 2004 11:43:59 maw
   	Added USBPUMP_OBJECT_NAME_DOWNLOAD

   1.91a  Fri Jan 28 2005 11:53:41  chwon
   	Added USBPUMP_OBJECT_NAME_SIC

   1.91b  Tue Mar 22 2005 15:23:15  chwon
   	Added USBPUMP_OBJECT_NAME_DIAG
   	
   1.91d  Fri Aug 19 2005 19:33:45  chunyi
   	Added USBPUMP_OBJECT_NAME_ETHER

   1.95a  Thu Mar 02 2006 23:00:57  chwon
	Added PictBridge and MTP object names

   1.97b  Sun Apr 30 2006 09:25:17  ssyang
	Added USBPUMP_OBJECT_NAME_AUDIO

   2.01a  Mon Mar 16 2009 15:39:57  chwon
	2829: move out object name from usbpumpobjectname.h to specific object
	header file. Add __TMS_PR2829_DISABLE as a compile parameter to restore
	old (incorrect) behavior.
	DO NOT ADD OBJECT NAME MACRO HERE. YOU SHOULD ADD OBJECT SPECIFIC FILE.

   3.15b  Mon Jun  1 2015 00:20:58  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _USBPUMPOBJECTNAME_H_		/* prevent multiple includes */
#define _USBPUMPOBJECTNAME_H_

#ifndef __TMS_CLOAKED_NAMES_ONLY
# define __TMS_CLOAKED_NAMES_ONLY 0
#endif

/*

MODULE DOCUMENTATION:  USB DataPump object enumeration

	******** To be completed *********

*/

/****************************************************************************\
|
|	Class matching strings
|
\****************************************************************************/

#define	__TMS_USBPUMP_OBJECT_NAME_FUNCTION	"fn"

#define	__TMS_USBPUMP_OBJECT_NAME_FN(Class,SubClass)	\
		Class    "."				\
		SubClass "."				\
		__TMS_USBPUMP_OBJECT_NAME_FUNCTION "."	\
		"mcci.com"

#define	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(Class)	\
	__TMS_USBPUMP_OBJECT_NAME_FN(Class,"*")


/*

Macro:	__TMS_PR2829_DISABLE

Function:
	Restore old compile-time behavior of usbkern/i/usbpumpobjectname.h

Description:
	Due to a bug (reported in PR2829), move out object names from
	usbpumpobjectname.h to the specific object header files.

	Removed object names from this header file are
		USBPUMP_OBJECT_NAME_CORE
		USBPUMP_OBJECT_NAME_DEBUG
		USBPUMP_OBJECT_NAME_DEVMGMT
		USBPUMP_OBJECT_NAME_DPS
		USBPUMP_OBJECT_NAME_ETHER
		USBPUMP_OBJECT_NAME_MODEM
		USBPUMP_OBJECT_NAME_MSS
		USBPUMP_OBJECT_NAME_MTP
		USBPUMP_OBJECT_NAME_NET
		USBPUMP_OBJECT_NAME_OBEX
		USBPUMP_OBJECT_NAME_PSS
		USBPUMP_OBJECT_NAME_PTP
		USBPUMP_OBJECT_NAME_SPECIAL
		USBPUMP_OBJECT_NAME_STORAGE
		USBPUMP_OBJECT_NAME_DOWNLOAD
		USBPUMP_OBJECT_NAME_SIC
		USBPUMP_OBJECT_NAME_DIAG
		USBPUMP_OBJECT_NAME_AUDIO

	Removed object enum names from this header file are
		USBPUMP_OBJECT_NAME_ENUM_DEVMGMT
		USBPUMP_OBJECT_NAME_ENUM_DPS
		USBPUMP_OBJECT_NAME_ENUM_ETHER
		USBPUMP_OBJECT_NAME_ENUM_MODEM
		USBPUMP_OBJECT_NAME_ENUM_MSS
		USBPUMP_OBJECT_NAME_ENUM_MTP
		USBPUMP_OBJECT_NAME_ENUM_NET
		USBPUMP_OBJECT_NAME_ENUM_OBEX
		USBPUMP_OBJECT_NAME_ENUM_PSS
		USBPUMP_OBJECT_NAME_ENUM_PTP
		USBPUMP_OBJECT_NAME_ENUM_STORAGE
		USBPUMP_OBJECT_NAME_ENUM_DOWNLOAD
		USBPUMP_OBJECT_NAME_ENUM_SIC
		USBPUMP_OBJECT_NAME_ENUM_DIAG
		USBPUMP_OBJECT_NAME_ENUM_AUDIO

	Rather than require that customers who have this problem change
	their code, the uncloaking for the above symbols is further 
	controlled by the compile-time parameter __TMS_PR2829_DISABLE.

	If this parameter is not defined, then i/usbpumpobjectname.h will
	properly handle cloaking for the above symbols.  If this parameter is
	defined to a non-zero value, then i/ubufqe.h will always define
	both the uncloaked and the cloaked names (but only for the above-
	mentioned symbols -- other symbols are not affected and will be
	uncloaked in the normal way.

*/

/* be careful to keep support for __TMS_PR2829_DISABLE! */
#ifndef __TMS_PR2829_DISABLE	/* PARAM */
# define __TMS_PR2829_DISABLE 0	/* if not overridden, remove all object names */
#endif /* __TMS_PR2829_DISABLE */

#if __TMS_PR2829_DISABLE
# define __TMS_USBPUMP_OBJECT_NAME_CORE		"core"
# define __TMS_USBPUMP_OBJECT_NAME_DEBUG	"debug"
# define __TMS_USBPUMP_OBJECT_NAME_DEVMGMT	"devmgmt"
# define __TMS_USBPUMP_OBJECT_NAME_DPS		"dps"
# define __TMS_USBPUMP_OBJECT_NAME_ETHER	"ether"
# define __TMS_USBPUMP_OBJECT_NAME_MODEM	"modem"
# define __TMS_USBPUMP_OBJECT_NAME_MSS		"mss"
# define __TMS_USBPUMP_OBJECT_NAME_MTP		"mtp"
# define __TMS_USBPUMP_OBJECT_NAME_NET		"net"
# define __TMS_USBPUMP_OBJECT_NAME_OBEX		"obex"
# define __TMS_USBPUMP_OBJECT_NAME_PSS		"pss"
# define __TMS_USBPUMP_OBJECT_NAME_PTP		"ptp"
# define __TMS_USBPUMP_OBJECT_NAME_SPECIAL	"special"
# define __TMS_USBPUMP_OBJECT_NAME_STORAGE	"storage"
# define __TMS_USBPUMP_OBJECT_NAME_DOWNLOAD	"download"
# define __TMS_USBPUMP_OBJECT_NAME_SIC		"sic"
# define __TMS_USBPUMP_OBJECT_NAME_DIAG		"diag"
# define __TMS_USBPUMP_OBJECT_NAME_AUDIO	"audio"
#endif /* __TMS_PR2829_DISABLE */

/*
|| Define strings to be used when enumerating functions
*/
#if __TMS_PR2829_DISABLE
# define __TMS_USBPUMP_OBJECT_NAME_ENUM_DEVMGMT		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_DEVMGMT)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_DPS		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_DPS)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_ETHER		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_ETHER)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_MODEM		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_MODEM)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_MSS		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_MSS)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_MTP		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_MTP)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_NET		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_NET)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_OBEX		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_OBEX)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_PSS		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_PSS)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_PTP		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_PTP)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_STORAGE		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_STORAGE)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_DOWNLOAD		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_DOWNLOAD)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_SIC		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_SIC)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_DIAG		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_DIAG)

# define __TMS_USBPUMP_OBJECT_NAME_ENUM_AUDIO		\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN(__TMS_USBPUMP_OBJECT_NAME_AUDIO)
#endif /* __TMS_PR2829_DISABLE */


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_OBJECT_NAME_FUNCTION	\
   __TMS_USBPUMP_OBJECT_NAME_FUNCTION
# define USBPUMP_OBJECT_NAME_FN(Class,SubClass)	\
   __TMS_USBPUMP_OBJECT_NAME_FN(Class,SubClass)
# define USBPUMP_OBJECT_NAME_ENUM_FN(Class)	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_FN(Class)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

#if !__TMS_CLOAKED_NAMES_ONLY && __TMS_PR2829_DISABLE
# define USBPUMP_OBJECT_NAME_CORE	\
   __TMS_USBPUMP_OBJECT_NAME_CORE
# define USBPUMP_OBJECT_NAME_DEBUG	\
   __TMS_USBPUMP_OBJECT_NAME_DEBUG
# define USBPUMP_OBJECT_NAME_DEVMGMT	\
   __TMS_USBPUMP_OBJECT_NAME_DEVMGMT
# define USBPUMP_OBJECT_NAME_DPS	\
   __TMS_USBPUMP_OBJECT_NAME_DPS
# define USBPUMP_OBJECT_NAME_ETHER	\
   __TMS_USBPUMP_OBJECT_NAME_ETHER
# define USBPUMP_OBJECT_NAME_MODEM	\
   __TMS_USBPUMP_OBJECT_NAME_MODEM
# define USBPUMP_OBJECT_NAME_MSS	\
   __TMS_USBPUMP_OBJECT_NAME_MSS
# define USBPUMP_OBJECT_NAME_MTP	\
   __TMS_USBPUMP_OBJECT_NAME_MTP
# define USBPUMP_OBJECT_NAME_NET	\
   __TMS_USBPUMP_OBJECT_NAME_NET
# define USBPUMP_OBJECT_NAME_OBEX	\
   __TMS_USBPUMP_OBJECT_NAME_OBEX
# define USBPUMP_OBJECT_NAME_PSS	\
   __TMS_USBPUMP_OBJECT_NAME_PSS
# define USBPUMP_OBJECT_NAME_PTP	\
   __TMS_USBPUMP_OBJECT_NAME_PTP
# define USBPUMP_OBJECT_NAME_SPECIAL	\
   __TMS_USBPUMP_OBJECT_NAME_SPECIAL
# define USBPUMP_OBJECT_NAME_STORAGE	\
   __TMS_USBPUMP_OBJECT_NAME_STORAGE
# define USBPUMP_OBJECT_NAME_DOWNLOAD	\
   __TMS_USBPUMP_OBJECT_NAME_DOWNLOAD
# define USBPUMP_OBJECT_NAME_SIC	\
   __TMS_USBPUMP_OBJECT_NAME_SIC
# define USBPUMP_OBJECT_NAME_DIAG	\
   __TMS_USBPUMP_OBJECT_NAME_DIAG
# define USBPUMP_OBJECT_NAME_AUDIO	\
   __TMS_USBPUMP_OBJECT_NAME_AUDIO
# define USBPUMP_OBJECT_NAME_ENUM_DEVMGMT	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_DEVMGMT
# define USBPUMP_OBJECT_NAME_ENUM_DPS	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_DPS
# define USBPUMP_OBJECT_NAME_ENUM_ETHER	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_ETHER
# define USBPUMP_OBJECT_NAME_ENUM_MODEM	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_MODEM
# define USBPUMP_OBJECT_NAME_ENUM_MSS	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_MSS
# define USBPUMP_OBJECT_NAME_ENUM_MTP	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_MTP
# define USBPUMP_OBJECT_NAME_ENUM_NET	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_NET
# define USBPUMP_OBJECT_NAME_ENUM_OBEX	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_OBEX
# define USBPUMP_OBJECT_NAME_ENUM_PSS	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_PSS
# define USBPUMP_OBJECT_NAME_ENUM_PTP	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_PTP
# define USBPUMP_OBJECT_NAME_ENUM_STORAGE	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_STORAGE
# define USBPUMP_OBJECT_NAME_ENUM_DOWNLOAD	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_DOWNLOAD
# define USBPUMP_OBJECT_NAME_ENUM_SIC	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_SIC
# define USBPUMP_OBJECT_NAME_ENUM_DIAG	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_DIAG
# define USBPUMP_OBJECT_NAME_ENUM_AUDIO	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_AUDIO
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpumpobjectname.h ****/
#endif /* _USBPUMPOBJECTNAME_H_ */
