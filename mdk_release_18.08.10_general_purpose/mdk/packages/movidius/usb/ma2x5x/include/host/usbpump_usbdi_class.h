/* usbpump_usbdi_class.h	Wed Feb 23 2011 14:55:08 chwon */

/*

Module:  usbpump_usbdi_class.h

Function:
	Definitions related to USBPUMP_USBDI_DRIVER_CLASS objects.

Version:
	V3.01d	Wed Feb 23 2011 14:55:08 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2005, 2009-2011 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Thu Jun 30 2005 09:33:53  tmm
	Module created.

   3.01a  Tue Dec 22 2009 20:52:19  chwon
	9631: Add ";mf" for multi-function device

   3.01a  Sat Jan 16 2010 11:29:07  chwon
	9746: Remove obsolete USBPUMP_USBDI_DRIVER_CLASS_ENTRY_FN definition

   3.01a  Sat Jan 16 2010 12:09:19  tmm
	9751: Add information needed for dynamic function instance creation:
	USBPUMP_USBDI_DRIVER_CLASS_FUNCTION_INFO, and FunctionInfo in
	each class object.

   3.01d  Wed Feb 23 2011 14:55:08  chwon
	12782: Increase USBPUMP_USBDI_INSTANCE_ID_LENGTH from 64 to 128.

*/

#ifndef _USBPUMP_USBDI_CLASS_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_CLASS_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif


#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

/****************************************************************************\
|
|	The match list -- matches are done using strings.  The basic pattern
|	is:
|
|	  vid=XXXX/XXXX;r=XXXX;[dc=xx/yy/zz;][if=xx;][fc=xx/yy/zz;][ig=gg;][mf;]
|
|	All numbers are formatted in lower-case hex; the GUID is formatted 
|	in normal form, i.e. with '{', '}' and embedded '-'.
|
|	vid=XXXX/XXXX is the vendor ID/product ID
|
|	r=XXXX is the revision ID
|
|	dc=xx/yy/zz is the device class/subclass/protocol.  This is omitted
|		if the class/subclass/protocol are all zero, unless the
|		function class/subclass/protocol are also all zero.
|
|	if=xx is the interface number (in hex, two digits)
|
|	fc=xx/yy/zz is the function class/subclass/protoco.  This may be
|		taken from the interface descriptor or from an IAD
|
|	ig={xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx} is the GUID, and is
|		only present for functions whose primary interface is an
|		MDLM interface.
|
|	mf is the multi-function device.
|
|	NB: the device class/subclass/protocol will be omitted if they are
|	dc=00/00/00.  The interface number will be present only for 
|	true multi-function devices.  The interface class/subclass/protocol
|	will always be present.
|
|	vid=040e/f101;*;if=03;*
|		This matches interface 3 on device with vendor ID/product ID
|		040e/f101.
|
|	*;dc=02/\*;fc=02/02/01;*
|		Matches an interface with class 2, protocol 2, subclass 1 on
|		a device with device class 1.
|
|		[The backslash in the prior line is just for
|		the C compiler, it should not appear in printed docs]
|
|	*;ig={12345678-1234-1234-1234-123456789abc};*
|		Matches an interface GUID.  The ';*' at the end is for future-
|		proofing.  Note that you might want to also write this as
|		"*;fc=02/0a/03;ig=12345678-1234-1234-1234-123456789abc;*",
|		in order to match a specific protocol code.
|
|	The max overall length, by the way, of the formatted string will be
|	98 bytes, with the terminating NUL.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_MATCH_LIST_ENTRY);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_MATCH_LIST_ENTRY)
	{
	__TMS_CONST __TMS_TEXT *		pPattern;
	__TMS_UINT16				nPattern;
	__TMS_USBPUMP_USBDI_MATCH_PRIORITY	uPriority;
	};

/* the priority values for matches of various kinds */
#define	__TMS_USBPUMP_USBDI_PRIORITY_VIDPIDREV		0xF000
#define	__TMS_USBPUMP_USBDI_PRIORITY_VIDPID		0xE000
#define	__TMS_USBPUMP_USBDI_PRIORITY_VIDPIDREV_IF	0xD800
#define	__TMS_USBPUMP_USBDI_PRIORITY_VIDPID_IF		0xD000
#define	__TMS_USBPUMP_USBDI_PRIORITY_DEV_CSP		0xC000
#define	__TMS_USBPUMP_USBDI_PRIORITY_DEV_CS		0xB000
#define	__TMS_USBPUMP_USBDI_PRIORITY_DEV_C		0xA000
#define	__TMS_USBPUMP_USBDI_PRIORITY_VIDPIDREV_FN_CSP	0x9800
#define	__TMS_USBPUMP_USBDI_PRIORITY_VIDPID_FN_CSP	0x9000
#define	__TMS_USBPUMP_USBDI_PRIORITY_VIDPIDREV_FN_CS	0x8800
#define	__TMS_USBPUMP_USBDI_PRIORITY_VIDPID_FN_CS	0x8000
#define	__TMS_USBPUMP_USBDI_PRIORITY_VIDPIDREV_FN_C	0x7800
#define	__TMS_USBPUMP_USBDI_PRIORITY_VIDPID_FN_C	0x7000
#define	__TMS_USBPUMP_USBDI_PRIORITY_FN_GUID		0x6000
#define	__TMS_USBPUMP_USBDI_PRIORITY_FN_CSP		0x5000
#define	__TMS_USBPUMP_USBDI_PRIORITY_FN_CS		0x4000
#define	__TMS_USBPUMP_USBDI_PRIORITY_FN_C		0x3000
#define	__TMS_USBPUMP_USBDI_PRIORITY_WEAK		0x1000

/* __TMS_TYPE_DEF_STRUCT (USBPUMP_USBDI_MATCH_LIST_HEADER); */
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_MATCH_LIST_HEADER)
	{
	__TMS_USBPUMP_USBDI_MATCH_LIST_HEADER	*pNext;
	__TMS_USBPUMP_USBDI_MATCH_LIST_HEADER	*pLast;

	__TMS_BYTES				nPatterns;
	__TMS_USBPUMP_USBDI_MATCH_LIST_ENTRY	Entry[1];
	};

#define	__TMS_USBPUMP_USBDI_MATCH_LIST_HEADER_SIZE(nPatterns)		\
	(sizeof(__TMS_USBPUMP_USBDI_MATCH_LIST_HEADER) +		\
	 sizeof(__TMS_USBPUMP_USBDI_MATCH_LIST_ENTRY) * ((nPatterns) - 1))

#define __TMS_USBPUMP_USBDI_MATCH_ID_LENGTH	98
#define	__TMS_USBPUMP_USBDI_INSTANCE_ID_LENGTH	128


/****************************************************************************\
|
|	The USBDI driver class object
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_DRIVER_CLASS_FUNCTION_INFO);

struct __TMS_STRUCTNAME	(USBPUMP_USBDI_DRIVER_CLASS_FUNCTION_INFO)
	{
	/* the size of each instance, in bytes */
	__TMS_BYTES			Size;

	/* the name of each instance */
	__TMS_CONST __TMS_TEXT		*pName;

	/* the IOCTL function to be used for each instance. */
	__TMS_USBPUMP_OBJECT_IOCTL_FN	*pIoctlFn;
	};


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_DRIVER_CLASS_CONTENTS);

struct __TMS_STRUCTNAME	(USBPUMP_USBDI_DRIVER_CLASS_CONTENTS)
	{
	/*
	|| Class.pClassChildren points to the head of the list of
	|| known children (function objects).
	*/
	__TMS_USBPUMP_OBJECT_CLASS_CONTENTS__STRUCT;

	/*
	|| DriverClass.pMatchList points to the structure that is used for
	|| matching this driver.
	*/
	__TMS_USBPUMP_USBDI_MATCH_LIST_HEADER	*pMatchListHeader;

	/*
	|| The pointer to the abstract pool header
	*/
	__TMS_USBPUMP_ABSTRACT_POOL		*pDriverPool;

	/*
	|| In order to enable "after-the-fact" drivers, or dynamically
	|| loaded drivers, we include a VOID* for client use.  This is
	|| equivalent to the driver extension pointer in WDM.
	*/
	__TMS_VOID *				pDriverPrivateInfo;

	/*
	|| Information required for creating instances
	*/
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_FUNCTION_INFO FunctionInfo;
	};

union __TMS_UNIONNAME	(USBPUMP_USBDI_DRIVER_CLASS)
	{
	__TMS_USBPUMP_OBJECT_CLASS_CONTENTS__UNION;
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONTENTS	DriverClass;
	};

#define	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONTENTS__STRUCT		\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONTENTS	DriverClass

#define	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONTENTS__UNION		\
	__TMS_USBPUMP_OBJECT_CLASS_CONTENTS__UNION;			\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONTENTS	DriverClass;	\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS		DriverClassCast

#define	__TMS_USBPUMP_USBDI_DRIVER_CLASS_TAG				\
			__TMS_UHIL_MEMTAG('U', 'C', 'l', 's')

#define	__TMS_USBPUMP_USBDI_DRIVER_CLASS_NAME(x)			\
			x __TMS_USBPUMP_USBDI_NAME(".driver")

#define	__TMS_USBPUMP_USBDI_DRIVER_METACLASS_NAME			\
			__TMS_USBPUMP_USBDI_NAME("driver.class")

/****************************************************************************\
|
|	uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_PRIORITY_VIDPIDREV	\
   __TMS_USBPUMP_USBDI_PRIORITY_VIDPIDREV
# define USBPUMP_USBDI_PRIORITY_VIDPID	\
   __TMS_USBPUMP_USBDI_PRIORITY_VIDPID
# define USBPUMP_USBDI_PRIORITY_VIDPIDREV_IF	\
   __TMS_USBPUMP_USBDI_PRIORITY_VIDPIDREV_IF
# define USBPUMP_USBDI_PRIORITY_VIDPID_IF	\
   __TMS_USBPUMP_USBDI_PRIORITY_VIDPID_IF
# define USBPUMP_USBDI_PRIORITY_DEV_CSP	\
   __TMS_USBPUMP_USBDI_PRIORITY_DEV_CSP
# define USBPUMP_USBDI_PRIORITY_DEV_CS	\
   __TMS_USBPUMP_USBDI_PRIORITY_DEV_CS
# define USBPUMP_USBDI_PRIORITY_DEV_C	\
   __TMS_USBPUMP_USBDI_PRIORITY_DEV_C
# define USBPUMP_USBDI_PRIORITY_VIDPIDREV_FN_CSP	\
   __TMS_USBPUMP_USBDI_PRIORITY_VIDPIDREV_FN_CSP
# define USBPUMP_USBDI_PRIORITY_VIDPID_FN_CSP	\
   __TMS_USBPUMP_USBDI_PRIORITY_VIDPID_FN_CSP
# define USBPUMP_USBDI_PRIORITY_VIDPIDREV_FN_CS	\
   __TMS_USBPUMP_USBDI_PRIORITY_VIDPIDREV_FN_CS
# define USBPUMP_USBDI_PRIORITY_VIDPID_FN_CS	\
   __TMS_USBPUMP_USBDI_PRIORITY_VIDPID_FN_CS
# define USBPUMP_USBDI_PRIORITY_VIDPIDREV_FN_C	\
   __TMS_USBPUMP_USBDI_PRIORITY_VIDPIDREV_FN_C
# define USBPUMP_USBDI_PRIORITY_VIDPID_FN_C	\
   __TMS_USBPUMP_USBDI_PRIORITY_VIDPID_FN_C
# define USBPUMP_USBDI_PRIORITY_FN_GUID	\
   __TMS_USBPUMP_USBDI_PRIORITY_FN_GUID
# define USBPUMP_USBDI_PRIORITY_FN_CSP	\
   __TMS_USBPUMP_USBDI_PRIORITY_FN_CSP
# define USBPUMP_USBDI_PRIORITY_FN_CS	\
   __TMS_USBPUMP_USBDI_PRIORITY_FN_CS
# define USBPUMP_USBDI_PRIORITY_FN_C	\
   __TMS_USBPUMP_USBDI_PRIORITY_FN_C
# define USBPUMP_USBDI_PRIORITY_WEAK	\
   __TMS_USBPUMP_USBDI_PRIORITY_WEAK
# define USBPUMP_USBDI_MATCH_LIST_HEADER_SIZE(nPatterns)	\
   __TMS_USBPUMP_USBDI_MATCH_LIST_HEADER_SIZE(nPatterns)
# define USBPUMP_USBDI_MATCH_ID_LENGTH	\
   __TMS_USBPUMP_USBDI_MATCH_ID_LENGTH
# define USBPUMP_USBDI_INSTANCE_ID_LENGTH	\
   __TMS_USBPUMP_USBDI_INSTANCE_ID_LENGTH
# define USBPUMP_USBDI_DRIVER_CLASS_CONTENTS__STRUCT	\
   __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONTENTS__STRUCT
# define USBPUMP_USBDI_DRIVER_CLASS_CONTENTS__UNION	\
   __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONTENTS__UNION
# define USBPUMP_USBDI_DRIVER_CLASS_TAG	\
   __TMS_USBPUMP_USBDI_DRIVER_CLASS_TAG
# define USBPUMP_USBDI_DRIVER_CLASS_NAME(x)	\
   __TMS_USBPUMP_USBDI_DRIVER_CLASS_NAME(x)
# define USBPUMP_USBDI_DRIVER_METACLASS_NAME	\
   __TMS_USBPUMP_USBDI_DRIVER_METACLASS_NAME
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/**** end of usbpump_usbdi_class.h ****/
#endif /* _USBPUMP_USBDI_CLASS_H_ */
