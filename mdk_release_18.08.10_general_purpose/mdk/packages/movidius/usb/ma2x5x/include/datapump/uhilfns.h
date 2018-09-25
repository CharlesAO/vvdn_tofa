/* uhilfns.h	Thu Nov  6 2014 14:29:08 tmm */

/*

Module:  uhilfns.h

Function:
	UHIL standard functions.

Version:
	V3.13b	Thu Nov  6 2014 14:29:08 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	November 2014

Revision history:
   3.13b  Thu Nov  6 2014 14:29:08  tmm
	Module created.

*/

#ifndef _UHILFNS_H_		/* prevent multiple includes */
#define _UHILFNS_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

#ifdef __TMS_UHIL_GETPUT_OVERRIDE_FILE
# include __TMS_UHIL_GETPUT_OVERRIDE_FILE
#endif

#ifdef __TMS_INCLUDE_UHIL_GETPUT_MACROS
# include "uhil_getput_macros.h"
#endif

/*

Macro:	__TMS_USBPUMP_LOCAL_UHIL_INCLUDE__

Function:
	Compile time override of MCCI UHIL GET/PUT and UDIV64/UREM64 macros.

Definition:
	CFLAGS_PORT += -D__TMS_USBPUMP_LOCAL_UHIL_INCLUDE__=my_file.h

Description:
	The DataPump allow the platform build system to inject a header file
	to override the default UHIL GET/PUT and UDIV64/UREM64 macros. 

	This header file will be included after #including "basehilenv.h"
	(and possibly after including other DataPump header files, depending 
	on the module).

Notes:
	To avoid quoting issues in the build system the file name is
	given directly (without quotes). "uhilfns.h" supplies
	the leading and trailing quotes. However, this means that you
	can't use an angle-bracketed include file.

*/

#ifdef __TMS_USBPUMP_LOCAL_UHIL_INCLUDE__

# if ! (__TMS_LIBPORT_VERSION > __TMS_LIBPORT_VERSION_CALC(3,11,2,0))
#  error __TMS_USBPUMP_LOCAL_UHIL_INCLUDE__ needs a version of libport that defines __TMS_STRINGVAL.
# endif

/* __TMS_STRINGVAL expands the argument before stringifying */
# include __TMS_STRINGVAL(__TMS_USBPUMP_LOCAL_UHIL_INCLUDE__)
#endif

/****************************************************************************\
|
|	The API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UINT8
UHIL_GetCodeUint8(
	__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_VOID) *
	);

__TMS_UINT32
UHIL_GetCodeUint32(
	__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_UINT32) *
	);

__TMS_BOOL
UHIL_cmpbuf(
	__TMS_CONST __TMS_VOID *,
	__TMS_CONST __TMS_VOID *,
	__TMS_ARG_BYTES
	);

__TMS_BOOL
UHIL_cmpstr(
	__TMS_CONST __TMS_TEXT *,
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UHIL_cpybuf(
	__TMS_VOID *,
	__TMS_CONST __TMS_VOID *,
	__TMS_BYTES
	);

#if __TMS_LIBPORT_CPU_HARVARD
__TMS_BYTES
UHIL_cpybuf_p(
	__TMS_VOID *,
	__TMS_TYPE_CODESPACE(__TMS_CONST __TMS_VOID) *,
	__TMS_BYTES
	);
#else
# define UHIL_cpybuf_p(d, s, l) UHIL_cpybuf(d, s, l)
#endif

__TMS_BYTES
UHIL_fill(
	__TMS_VOID *,
	__TMS_ARG_BYTES,
	__TMS_ARG_UCHAR
	);

__TMS_BYTES
UHIL_lenstr(
	__TMS_CONST __TMS_TEXT *
	);

__TMS_BYTES
UHIL_lenstr_p(
	__TMS_CONST __TMS_TYPE_CODESPACE(__TMS_TEXT) *
	);

__TMS_BYTES
UHIL_cpynstr(
	__TMS_TEXT *,
	__TMS_BYTES,
	...
	);

/* little endian get and put */
__TMS_UINT16	UHIL_le_getuint16(
			__TMS_LIBPORT_PACKED1(__TMS_CONST __TMS_UINT8 *)
			);
__TMS_UINT32	UHIL_le_getuint32(
			__TMS_LIBPORT_PACKED1(__TMS_CONST __TMS_UINT8 *)
			);
__TMS_UINT64	UHIL_le_getuint64(
			__TMS_LIBPORT_PACKED1(__TMS_CONST __TMS_UINT8 *)
			);
__TMS_VOID	UHIL_le_getuint128_s(
			__TMS_UINT128_S *,
			__TMS_LIBPORT_PACKED1(__TMS_CONST __TMS_UINT8 *)
			);
__TMS_VOID	UHIL_le_getint128_s(
			__TMS_INT128_S *,
			__TMS_LIBPORT_PACKED1(__TMS_CONST __TMS_UINT8 *)
			);
__TMS_VOID	UHIL_le_putuint16(
			__TMS_LIBPORT_PACKED1(__TMS_UINT8 *),
			__TMS_UINT16
			);
__TMS_VOID	UHIL_le_putuint32(
			__TMS_LIBPORT_PACKED1(__TMS_UINT8 *),
			__TMS_UINT32
			);
__TMS_VOID	UHIL_le_putuint64(
			__TMS_LIBPORT_PACKED1(__TMS_UINT8 *),
			__TMS_UINT64
			);
__TMS_VOID	UHIL_le_putint128_s(
			__TMS_LIBPORT_PACKED1(__TMS_UINT8 *),
			__TMS_CONST __TMS_INT128_S *
			);
__TMS_VOID	UHIL_le_putuint128_s(
			__TMS_LIBPORT_PACKED1(__TMS_UINT8 *),
			__TMS_CONST __TMS_UINT128_S *
			);

/* big endian get and put */
__TMS_UINT16	UHIL_be_getuint16(
			__TMS_LIBPORT_PACKED1(__TMS_CONST __TMS_UINT8 *)
			);
__TMS_UINT32	UHIL_be_getuint32(
			__TMS_LIBPORT_PACKED1(__TMS_CONST __TMS_UINT8 *)
			);
__TMS_UINT64	UHIL_be_getuint64(
			__TMS_LIBPORT_PACKED1(__TMS_CONST __TMS_UINT8 *)
			);
__TMS_VOID	UHIL_be_getint128_s(
			__TMS_INT128_S *,
			__TMS_LIBPORT_PACKED1(__TMS_CONST __TMS_UINT8 *)
			);
__TMS_VOID	UHIL_be_getuint128_s(
			__TMS_UINT128_S *,
			__TMS_LIBPORT_PACKED1(__TMS_CONST __TMS_UINT8 *)
			);
__TMS_VOID	UHIL_be_putuint16(
			__TMS_LIBPORT_PACKED1(__TMS_UINT8 *),
			__TMS_UINT16
			);
__TMS_VOID	UHIL_be_putuint32(
			__TMS_LIBPORT_PACKED1(__TMS_UINT8 *),
			__TMS_UINT32
			);
__TMS_VOID	UHIL_be_putuint64(
			__TMS_LIBPORT_PACKED1(__TMS_UINT8 *),
			__TMS_UINT64
			);
__TMS_VOID	UHIL_be_putint128_s(
			__TMS_LIBPORT_PACKED1(__TMS_UINT8 *),
			__TMS_CONST __TMS_INT128_S *
			);
__TMS_VOID	UHIL_be_putuint128_s(
			__TMS_LIBPORT_PACKED1(__TMS_UINT8 *),
			__TMS_CONST __TMS_UINT128_S *
			);

/* 64-bit division and remainder function */
__TMS_UINT64
UHIL_udiv64(
	__TMS_UINT64	/* dividend */,
	__TMS_UINT64	/* divisor */
	);

__TMS_UINT64
UHIL_urem64(
	__TMS_UINT64	/* dividend */,
	__TMS_UINT64	/* divisor */
	);

__TMS_END_DECLS

/****************************************************************************\
|
|	Additional utility macros.
|
\****************************************************************************/

/*
|| To minimize the number of 2, 4, 8 values sprinkled through the code...
*/
#define	__TMS_UHIL_INT16_WIRE_SIZE	2
#define	__TMS_UHIL_UINT16_WIRE_SIZE	2
#define	__TMS_UHIL_INT32_WIRE_SIZE	4
#define	__TMS_UHIL_UINT32_WIRE_SIZE	4
#define	__TMS_UHIL_INT64_WIRE_SIZE	8
#define	__TMS_UHIL_UINT64_WIRE_SIZE	8
#define	__TMS_UHIL_INT128_WIRE_SIZE	16
#define	__TMS_UHIL_UINT128_WIRE_SIZE	16

/*
|| These might be optimized on a port basis....
*/
#ifndef	__TMS_UHIL_LE_GETUINT16
# define __TMS_UHIL_LE_GETUINT16(buf)		UHIL_le_getuint16(buf)
#endif

#ifndef	__TMS_UHIL_LE_GETUINT32
# define __TMS_UHIL_LE_GETUINT32(buf)		UHIL_le_getuint32(buf)
#endif

#ifndef	__TMS_UHIL_LE_GETUINT64
# define __TMS_UHIL_LE_GETUINT64(buf)		UHIL_le_getuint64(buf)
#endif

#ifndef	__TMS_UHIL_LE_GETINT128_S
# define __TMS_UHIL_LE_GETINT128_S(pv, buf)	UHIL_le_getint128_s(pv, buf)
#endif

#ifndef	__TMS_UHIL_LE_GETUINT128_S
# define __TMS_UHIL_LE_GETUINT128_S(pv, buf)	UHIL_le_getuint128_s(pv, buf)
#endif

#ifndef	__TMS_UHIL_LE_PUTUINT16
# define __TMS_UHIL_LE_PUTUINT16(buf, val)	UHIL_le_putuint16(buf, val)
#endif

#ifndef	__TMS_UHIL_LE_PUTUINT32
# define __TMS_UHIL_LE_PUTUINT32(buf, val)	UHIL_le_putuint32(buf, val)
#endif

#ifndef	__TMS_UHIL_LE_PUTUINT64
# define __TMS_UHIL_LE_PUTUINT64(buf, val)	UHIL_le_putuint64(buf, val)
#endif

#ifndef	__TMS_UHIL_LE_PUTINT128_S
# define __TMS_UHIL_LE_PUTINT128_S(buf, pval)	UHIL_le_putint128_s(buf, pval)
#endif

#ifndef	__TMS_UHIL_LE_PUTUINT128_S
# define __TMS_UHIL_LE_PUTUINT128_S(buf, pval)	UHIL_le_putuint128_s(buf, pval)
#endif

#ifndef	__TMS_UHIL_BE_GETUINT16
# define __TMS_UHIL_BE_GETUINT16(buf)		UHIL_be_getuint16(buf)
#endif

#ifndef __TMS_UHIL_BE_GETUINT32
# define __TMS_UHIL_BE_GETUINT32(buf)		UHIL_be_getuint32(buf)
#endif

#ifndef __TMS_UHIL_BE_GETUINT64
# define __TMS_UHIL_BE_GETUINT64(buf)		UHIL_be_getuint64(buf)
#endif

#ifndef __TMS_UHIL_BE_GETINT128_S
# define __TMS_UHIL_BE_GETINT128_S(pv, buf)	UHIL_be_getint128_s(pv, buf)
#endif

#ifndef __TMS_UHIL_BE_GETUINT128_S
# define __TMS_UHIL_BE_GETUINT128_S(pv, buf)	UHIL_be_getuint128_s(pv, buf)
#endif

#ifndef	__TMS_UHIL_BE_PUTUINT16
# define __TMS_UHIL_BE_PUTUINT16(buf, val)	UHIL_be_putuint16(buf, val)
#endif

#ifndef	__TMS_UHIL_BE_PUTUINT32
# define __TMS_UHIL_BE_PUTUINT32(buf, val)	UHIL_be_putuint32(buf, val)
#endif

#ifndef	__TMS_UHIL_BE_PUTUINT64
# define __TMS_UHIL_BE_PUTUINT64(buf, val)	UHIL_be_putuint64(buf, val)
#endif

#ifndef	__TMS_UHIL_BE_PUTINT128_S
# define __TMS_UHIL_BE_PUTINT128_S(buf, pval)	UHIL_be_putint128_s(buf, pval)
#endif

#ifndef	__TMS_UHIL_BE_PUTUINT128_S
# define __TMS_UHIL_BE_PUTUINT128_S(buf, pval)	UHIL_be_putuint128_s(buf, pval)
#endif

/* 64-bit division and remainder function */
#ifndef	__TMS_UHIL_UDIV64
# define __TMS_UHIL_UDIV64(dividend, divisor)	UHIL_udiv64(dividend, divisor)
#endif

#ifndef	__TMS_UHIL_UREM64
# define __TMS_UHIL_UREM64(dividend, divisor)	UHIL_urem64(dividend, divisor)
#endif
/****************************************************************************\
|
|	uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UHIL_INT16_WIRE_SIZE	\
   __TMS_UHIL_INT16_WIRE_SIZE
# define UHIL_UINT16_WIRE_SIZE	\
   __TMS_UHIL_UINT16_WIRE_SIZE
# define UHIL_INT32_WIRE_SIZE	\
   __TMS_UHIL_INT32_WIRE_SIZE
# define UHIL_UINT32_WIRE_SIZE	\
   __TMS_UHIL_UINT32_WIRE_SIZE
# define UHIL_INT64_WIRE_SIZE	\
   __TMS_UHIL_INT64_WIRE_SIZE
# define UHIL_UINT64_WIRE_SIZE	\
   __TMS_UHIL_UINT64_WIRE_SIZE
# define UHIL_INT128_WIRE_SIZE	\
   __TMS_UHIL_INT128_WIRE_SIZE
# define UHIL_UINT128_WIRE_SIZE	\
   __TMS_UHIL_UINT128_WIRE_SIZE
# define UHIL_LE_GETUINT16(buf)	\
   __TMS_UHIL_LE_GETUINT16(buf)
# define UHIL_LE_GETUINT32(buf)	\
   __TMS_UHIL_LE_GETUINT32(buf)
# define UHIL_LE_GETUINT64(buf)	\
   __TMS_UHIL_LE_GETUINT64(buf)
# define UHIL_LE_GETINT128_S(pv, buf)	\
   __TMS_UHIL_LE_GETINT128_S(pv, buf)
# define UHIL_LE_GETUINT128_S(pv, buf)	\
   __TMS_UHIL_LE_GETUINT128_S(pv, buf)
# define UHIL_LE_PUTUINT16(buf, val)	\
   __TMS_UHIL_LE_PUTUINT16(buf, val)
# define UHIL_LE_PUTUINT32(buf, val)	\
   __TMS_UHIL_LE_PUTUINT32(buf, val)
# define UHIL_LE_PUTUINT64(buf, val)	\
   __TMS_UHIL_LE_PUTUINT64(buf, val)
# define UHIL_LE_PUTINT128_S(buf, pval)	\
   __TMS_UHIL_LE_PUTINT128_S(buf, pval)
# define UHIL_LE_PUTUINT128_S(buf, pval)	\
   __TMS_UHIL_LE_PUTUINT128_S(buf, pval)
# define UHIL_BE_GETUINT16(buf)	\
   __TMS_UHIL_BE_GETUINT16(buf)
# define UHIL_BE_GETUINT32(buf)	\
   __TMS_UHIL_BE_GETUINT32(buf)
# define UHIL_BE_GETUINT64(buf)	\
   __TMS_UHIL_BE_GETUINT64(buf)
# define UHIL_BE_GETINT128_S(pv, buf)	\
   __TMS_UHIL_BE_GETINT128_S(pv, buf)
# define UHIL_BE_GETUINT128_S(pv, buf)	\
   __TMS_UHIL_BE_GETUINT128_S(pv, buf)
# define UHIL_BE_PUTUINT16(buf, val)	\
   __TMS_UHIL_BE_PUTUINT16(buf, val)
# define UHIL_BE_PUTUINT32(buf, val)	\
   __TMS_UHIL_BE_PUTUINT32(buf, val)
# define UHIL_BE_PUTUINT64(buf, val)	\
   __TMS_UHIL_BE_PUTUINT64(buf, val)
# define UHIL_BE_PUTINT128_S(buf, pval)	\
   __TMS_UHIL_BE_PUTINT128_S(buf, pval)
# define UHIL_BE_PUTUINT128_S(buf, pval)	\
   __TMS_UHIL_BE_PUTUINT128_S(buf, pval)
# define UHIL_UDIV64(dividend, divisor)	\
   __TMS_UHIL_UDIV64(dividend, divisor)
# define UHIL_UREM64(dividend, divisor)	\
   __TMS_UHIL_UREM64(dividend, divisor)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of uhilfns.h ****/
#endif /* _UHILFNS_H_ */
