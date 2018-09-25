/* usbuuid.h	Thu Jun 30 2005 14:36:52 tmm */

/*

Module:  usbuuid.h

Function:
	The UUID type

Version:
	V1.91d	Thu Jun 30 2005 14:36:52 tmm	Edit level 3

Copyright notice:
	This file copyright (C) 2000-2001, 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	September 2000

Revision history:
   1.60n  Sat Sep 23 2000 17:12:20  tmm
	Module created.

   1.61c  1/5/2001  tmm
	Bug 415:  add LENGTHOF_USBUUID_WIRE.

   1.91d  2005-06-30  tmm
	Add GUID as a synonym.

*/

#ifndef _USBUUID_H_		/* prevent multiple includes */
#define _USBUUID_H_

#include "def/cdefs.h"

/****************************************************************************\
|
|	Data structure definitions for UUIDs (known as GUIDs in other
|	contexts).  Taken from INTERNET-DRAFT <draft-leach-uuids-guids-01.txt>
|	which is in MCCI's uuidgen source directory.
|
\****************************************************************************/

/*
|| We have two forms:  one internal, one 'on the wire'.
*/
__TMS_TYPE_DEF_STRUCT(USBUUID);
__TMS_TYPE_DEF_STRUCT(USBUUID_WIRE);

__TMS_TYPE_DEF(USBGUID, __TMS_USBUUID);
__TMS_TYPE_DEF(USBGUID_WIRE, __TMS_USBUUID_WIRE);

/*
|| A uuid in internal form.
*/
struct __TMS_STRUCTNAME(USBUUID) {
	__TMS_UINT32	time_low;
	__TMS_UINT16	time_mid;
	__TMS_UINT16	time_hi_and_version;
	__TMS_UINT8	clock_seq_hi_and_reserved;
	__TMS_UINT8	clock_seq_low;
	__TMS_UINT8	node[6];
	};

#define __TMS_USBUUID_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	{ (t1), (t2), (t3), (c1), (c2), { (n1), (n2), (n3), (n4), (n5), (n6) }}

#define __TMS_USBGUID_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	__TMS_USBUUID_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6)

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBUUID_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	__TMS_USBUUID_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6)
# define USBGUID_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	__TMS_USBUUID_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6)
#endif

/*
|| A uuid on the wire.
*/

#include "usbpump_pushpack1.h"

__TMS_LIBPORT_PACKED1_DEFINITION(
struct __TMS_STRUCTNAME(USBUUID_WIRE)
)	{
	__TMS_UINT8	time_low[4];
	__TMS_UINT8	time_mid[2];
	__TMS_UINT8	time_hi_and_version[2];
	__TMS_UINT8	clock_seq_hi_and_reserved;
	__TMS_UINT8	clock_seq_low;
	__TMS_UINT8	node[6];
	} __TMS_LIBPORT_PACKED1_SUFFIX;

#include "usbpump_poppack.h"

#define	__TMS_LENGTHOF_USBUUID_WIRE	16
#define	__TMS_LENGTHOF_USBGUID_WIRE	__TMS_LENGTHOF_USBUUID_WIRE

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define LENGTHOF_USBUUID_WIRE	__TMS_LENGTHOF_USBUUID_WIRE
# define LENGTHOF_USBGUID_WIRE	__TMS_LENGTHOF_USBGUID_WIRE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/* initialize from a normal UUID initializer */
#define __TMS_USBUUID_WIRE_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	{ \
	{ (__TMS_UINT8) (((t1) >> 24) & 0xFF), \
	  (__TMS_UINT8) (((t1) >> 16) & 0xFF), \
	  (__TMS_UINT8) (((t1) >> 8) & 0xFF), \
	  (__TMS_UINT8)  ((t1) & 0xFF) }, \
	{ (__TMS_UINT8) (((t2) >> 8) & 0xFF), \
	  (__TMS_UINT8)  ((t2) & 0xFF) }, \
	{ (__TMS_UINT8) (((t3) >> 8) & 0xFF), \
	  (__TMS_UINT8)  ((t3) & 0xFF) }, \
	(c1), \
	(c2), \
	{ (n1), (n2), (n3), (n4), (n5), (n6) } \
	}

# define __TMS_USBGUID_WIRE_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	__TMS_USBUUID_WIRE_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6)

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBUUID_WIRE_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	__TMS_USBUUID_WIRE_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6)
# define USBGUID_WIRE_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6) \
	__TMS_USBGUID_WIRE_INIT(t1, t2, t3, c1, c2, n1, n2, n3, n4, n5, n6)
#endif

/**** end of usbuuid.h ****/
#endif /* _USBUUID_H_ */
