/* usbpump_unicode.h	Mon Apr 06 2009 09:20:39 wkkim */

/*

Module:  usbpump_unicode.h

Function:
	Unicode processing macros and functions, for UTF32, UTF16 and UTF8.

Version:
	V2.01a	Mon Apr 06 2009 09:20:39 wkkim	Edit level 7

Copyright notice:
	This file copyright (C) 2005-2007, 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	September 2005

Revision history:
   1.91h  Fri Sep  9 2005 14:22:08  tmm
	Module created.

   1.91h  Thu Sep 15 2005 00:39:02  chwon
	Add USBPUMP_UTF16_TEXT_C macro, add prototypes. Fix typo.

   1.91i  Sun Sep 18 2005 14:12:14  tmm
	Add commentary and make UTF16_TEXT_C() generate an unsigned
	constant, which is correct for UTF16.

   1.93e  Fri Feb 17 2006 14:25:35  tmm
	Add UsbPumpUtf8_UpperFromUtf16Wire(); then rename to
	UsbPumpUtf16Wire_ToLowerUtf8() and add UsbPumpUtf16Wire_Get().

   1.97h  Thu Apr 12 2007 16:44:55  chwon
	Add UsbPumpUtf8_CompareBuffer(), UsbPumpUtf8_CompareBufferString(),
	and UsbPumpUtf8_CompareString()

   2.01a  Mon Apr 06 2009 09:20:39  wkkim
	7692: Add UsbPumpUtf16Wire_ToUtf8().

*/

#ifndef _USBPUMP_UNICODE_H_		/* prevent multiple includes */
#define _USBPUMP_UNICODE_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	Basic types.
|
\****************************************************************************/

/*
|| in usbpump_types.h:
|| __TMS_TYPE_DEF_ARG(USBPUMP_UTF32_TEXT, INT32);
|| __TMS_TYPE_DEF_ARG(USBPUMP_UTF16_TEXT, UINT16);
|| __TMS_TYPE_DEF_ARG(USBPUMP_UTF8_TEXT, TEXT);
*/

__TMS_TYPE_DEF_ARG(USBPUMP_UTF16_TEXT_WIRE, UINT8);

/*
|| Given a numeric constant, apply the suffices needed to generate a UTF-16
|| constant of the appropriate type.  Of course, this can only be used with
|| numbers, not with characters [i.e., USBPUMP_UTF16_TEXT_C('a') is wrong].
*/
#define	__TMS_USBPUMP_UTF16_TEXT_C(x)		x ## u

/*
|| Given a numeric constant, apply the suffices needed to generate a UTF-32
|| constant of the appropriate type.  Of course, this can only be used with
|| numbers, not with characters [i.e., USBPUMP_UTF32_TEXT_C('a') is wrong].
*/
#define	__TMS_USBPUMP_UTF32_TEXT_C(x)		x

/*
|| There are lots of invalid UTF-32 characters (at least when UTF-32
|| conveys Unicode), but only one of them is used intentionally:
|| USBPUMP_UTF32_TEXT_INVALID.
*/
#define	__TMS_USBPUMP_UTF32_TEXT_INVALID	(- __TMS_USBPUMP_UTF32_TEXT_C(1))

/*

Name:	USBPUMP_UTF8_UNICODE_NBYTES(c)

Function:
	Determine the number of bytes specified by a UTF8 lead-in byte,
	assuming UNICODE conformance.

Definition:
	unsigned USBPUMP_UNICODE_UTF8_NBYTES(
		USBPUMP_UTF8_TEXT Char
		);

Description:
	Char is looked up in the list of leagal UTF8 lead-in bytes.  If
	it's found, the appopriate overall length is returned.  If not found,
	then this is not the begining of a legal UTF8 sequence *in Unicode*,
	and 0 is returned.

Returns:
	An integer in the range 0..4.

Notes:
	The rules from Unicide 4.0 are followed.

	This is a macro, so Char may evaluated multiple times.

See also:
	USBPUMP_UTF8_ISO10646_NBYTES()

*/

#define	__TMS_USBPUMP_UNICODE_UTF8_NBYTES(c)	\
		((__TMS_UINT8)(c) < 0x80 ? 1 : \
		 (__TMS_UINT8)(c) < 0xC2 ? 0 : \
		 (__TMS_UINT8)(c) < 0xE0 ? 2 : \
		 (__TMS_UINT8)(c) < 0xF0 ? 3 : \
		 (__TMS_UINT8)(c) < 0xF5 ? 4 : \
					   0)
/*

Name:	USBPUMP_UTF8_ISO10646_NBYTES(c)

Function:
	Determine the number of bytes specified by a UTF8 lead-in byte,
	assuming ISO10646 conformance.

Definition:
	unsigned USBPUMP_ISO10646_UTF8_NBYTES(
		USBPUMP_UTF8_TEXT Char
		);

Description:
	Char is looked up in the list of leagal UTF8 lead-in bytes for ISO-
	10646.  If it's found, the appopriate overall length is returned.  If
	not found, then this is not the begining of a legal UTF8 sequence in
	ISO-10646, and 0 is returned.

Returns:
	An integer in the range 0..6.

See also:
	USBPUMP_UTF8_UNICODE_NBYTES()

*/

#define	__TMS_USBPUMP_ISO10646_UTF8_NBYTES(c)	\
		((__TMS_UINT8)(c) < 0x80 ? 1 : \
		 (__TMS_UINT8)(c) < 0xC2 ? 0 : \
		 (__TMS_UINT8)(c) < 0xE0 ? 2 : \
		 (__TMS_UINT8)(c) < 0xF0 ? 3 : \
		 (__TMS_UINT8)(c) < 0xF8 ? 4 : \
		 (__TMS_UINT8)(c) < 0xFC ? 5 : \
					   6)

/*
|| Maxima for each length.
*/
#define	__TMS_USBPUMP_UNICODE_UTF8_1BYTE __TMS_USBPUMP_UTF32_TEXT_C(    0x7f)
#define	__TMS_USBPUMP_UNICODE_UTF8_2BYTE __TMS_USBPUMP_UTF32_TEXT_C(   0x7ff)
#define	__TMS_USBPUMP_UNICODE_UTF8_3BYTE __TMS_USBPUMP_UTF32_TEXT_C(  0xffff)
#define	__TMS_USBPUMP_UNICODE_UTF8_4BYTE __TMS_USBPUMP_UTF32_TEXT_C(0x10ffff)

#define	__TMS_USBPUMP_ISO10646_UTF8_1BYTE __TMS_USBPUMP_UTF32_TEXT_C(      0x7f)
#define	__TMS_USBPUMP_ISO10646_UTF8_2BYTE __TMS_USBPUMP_UTF32_TEXT_C(     0x7ff)
#define	__TMS_USBPUMP_ISO10646_UTF8_3BYTE __TMS_USBPUMP_UTF32_TEXT_C(    0xffff)
#define	__TMS_USBPUMP_ISO10646_UTF8_4BYTE __TMS_USBPUMP_UTF32_TEXT_C(  0x1fffff)
#define	__TMS_USBPUMP_ISO10646_UTF8_5BYTE __TMS_USBPUMP_UTF32_TEXT_C( 0x3ffffff)
#define	__TMS_USBPUMP_ISO10646_UTF8_6BYTE __TMS_USBPUMP_UTF32_TEXT_C(0x7fffffff)

#define	__TMS_USBPUMP_UTF16_MAPBASE	__TMS_USBPUMP_UTF32_TEXT_C( 0x10000)
#define	__TMS_USBPUMP_UTF16_MAPTOP	__TMS_USBPUMP_UTF32_TEXT_C(0x10FFFF)
#define	__TMS_USBPUMP_UTF16_FIRSTMAP	__TMS_USBPUMP_UTF16_TEXT_C(  0xD800)
#define	__TMS_USBPUMP_UTF16_SECONDMAP	__TMS_USBPUMP_UTF16_TEXT_C(  0xDC00)
#define	__TMS_USBPUMP_UTF16_LASTMAP	__TMS_USBPUMP_UTF16_TEXT_C(  0xDFFF)

#define	__TMS_USBPUMP_UTF16_NWORDS(x)	\
	((__TMS_USBPUMP_UTF16_FIRSTMAP <= (__TMS_USBPUMP_UTF16_TEXT)(x) && \
	 (__TMS_USBPUMP_UTF16_TEXT)(x) < __TMS_USBPUMP_UTF16_SECONDMAP)	\
		? 2 : 1)

#define	__TMS_USBPUMP_UTF32_MAP16_FIRST(w)				\
	((__TMS_USBPUMP_UTF16_TEXT)					\
		(__TMS_USBPUMP_UTF16_FIRSTMAP +				\
		 (((w) - __TMS_USBPUMP_UTF16_MAPBASE) >> 10)		\
		))

#define	__TMS_USBPUMP_UTF32_MAP16_SECOND(w)				\
	((__TMS_USBPUMP_UTF16_TEXT)					\
		(__TMS_USBPUMP_UTF16_SECONDMAP + ((w) & 0x3FF)))

#define	__TMS_USBPUMP_UTF32_FROM_UTF16(x,y)				\
	(__TMS_USBPUMP_UTF16_MAPBASE +					\
	 (((__TMS_USBPUMP_UTF32_TEXT)(x) - __TMS_USBPUMP_UTF16_FIRSTMAP) \
			<< 10) +					\
	        (__TMS_USBPUMP_UTF32_TEXT)((y) & 0x3FF))

/****************************************************************************\
|
|	Structures
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_UTF32_MAP);
struct __TMS_STRUCTNAME	(USBPUMP_UTF32_MAP)
	{
	__TMS_USBPUMP_UTF32_TEXT	Input;
	__TMS_USBPUMP_UTF32_TEXT	Output;
	};

/****************************************************************************\
|
|	Functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_CONST __TMS_UINT16
gkUsbPumpUtf32_ToLowerHashTable[];

extern __TMS_CONST __TMS_USBPUMP_UTF32_MAP
gkUsbPumpUtf32_ToLowerMap[];

extern __TMS_CONST __TMS_BYTES
gkUsbPumpUtf32_nToLowerMap;

__TMS_BOOL
UsbPumpUtf8_BufferToUtf16(
	__TMS_USBPUMP_UTF16_TEXT *		/*pOutBuf*/,
	__TMS_BYTES				/*len_OutBuf*/,
	__TMS_BYTES				/*iOutBuf*/,
	__TMS_BYTES *				/*pnOutChars*/,
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *	/*pInBuf*/,
	__TMS_BYTES				/*nInBuf*/,
	__TMS_BYTES				/*iInBuf*/,
	__TMS_BYTES *				/*pnInChars*/
	);

__TMS_BOOL
UsbPumpUtf8_CompareBuffer(
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *	/*pName1*/,
	__TMS_BYTES				/*nName1*/,
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *	/*pName2*/,
	__TMS_BYTES				/*nName2*/,
	__TMS_BOOL				/*fCaseSensitive*/
	);

__TMS_BOOL
UsbPumpUtf8_CompareBufferString(
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *	/*pBuffer*/,
	__TMS_BYTES				/*nBuffer*/,
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *	/*pString*/,
	__TMS_BOOL				/*fCaseSensitive*/
	);

__TMS_BOOL
UsbPumpUtf8_CompareString(
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *	/*pName1*/,
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *	/*pName2*/,
	__TMS_BOOL				/*fCaseSensitive*/
	);

__TMS_BYTES
UsbPumpUtf8_Get(
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *,
	__TMS_BYTES /*insize*/,
	__TMS_BYTES /*index*/,
	__TMS_USBPUMP_UTF32_TEXT * /*pResult*/
	);

__TMS_BYTES
UsbPumpUtf8_Put(
	__TMS_USBPUMP_UTF8_TEXT *,
	__TMS_BYTES /*outsize*/,
	__TMS_BYTES /*outi*/,
	__TMS_USBPUMP_UTF32_TEXT
	);

__TMS_BOOL
UsbPumpUtf8_StringCountUtf16Elements(
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *	/*pUtf8String*/,
	__TMS_BYTES *				/*pnOutChars*/
	);

__TMS_BOOL
UsbPumpUtf16Wire_ToUtf8Lower(
	__TMS_CONST __TMS_USBPUMP_UTF16_TEXT_WIRE *	/*input string*/,
	__TMS_BYTES			/*nInputString, in chars not bytes*/,
	__TMS_USBPUMP_UTF8_TEXT *	/*pUtf8Buffer*/,
	__TMS_BYTES 			/*sizeUtf8Buffer*/,
	__TMS_BYTES *			/*pLengthResult*/
	);

__TMS_BOOL
UsbPumpUtf16Wire_ToUtf8(
	__TMS_CONST __TMS_USBPUMP_UTF16_TEXT_WIRE *	/*input string*/,
	__TMS_BYTES			/*nInputString, in chars not bytes*/,
	__TMS_USBPUMP_UTF8_TEXT *	/*pUtf8Buffer*/,
	__TMS_BYTES 			/*sizeUtf8Buffer*/,
	__TMS_BYTES *			/*pLengthResult*/
	);

__TMS_BYTES
UsbPumpUtf16_Get(
	__TMS_CONST __TMS_USBPUMP_UTF16_TEXT *,
	__TMS_BYTES /*in chars*/,
	__TMS_BYTES /*in char index*/,
	__TMS_USBPUMP_UTF32_TEXT * /*pResult*/
	);

__TMS_BYTES
UsbPumpUtf16Wire_Get(
	__TMS_CONST __TMS_USBPUMP_UTF16_TEXT_WIRE *,
	__TMS_BYTES /*in chars*/,
	__TMS_BYTES /*in char index*/,
	__TMS_USBPUMP_UTF32_TEXT * /*pResult*/
	);

__TMS_BYTES
UsbPumpUtf16_Put(
	__TMS_USBPUMP_UTF16_TEXT *,
	__TMS_BYTES /*out char size*/,
	__TMS_BYTES /*out char index */,
	__TMS_USBPUMP_UTF32_TEXT
	);

__TMS_USBPUMP_UTF32_TEXT
UsbPumpUtf32_ToLower(
	__TMS_USBPUMP_UTF32_TEXT /*input*/
	);

__TMS_END_DECLS

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_UTF32_TEXT_C(x)	__TMS_USBPUMP_UTF32_TEXT_C(x)
# define USBPUMP_UTF16_TEXT_C(x)	__TMS_USBPUMP_UTF16_TEXT_C(x)
# define USBPUMP_UTF32_TEXT_INVALID	__TMS_USBPUMP_UTF32_TEXT_INVALID
# define USBPUMP_UNICODE_UTF8_NBYTES(c)	__TMS_USBPUMP_UNICODE_UTF8_NBYTES(c)
# define USBPUMP_ISO10646_UTF8_NBYTES(c)	__TMS_USBPUMP_ISO10646_UTF8_NBYTES(c)
# define USBPUMP_UNICODE_UTF8_1BYTE	__TMS_USBPUMP_UNICODE_UTF8_1BYTE
# define USBPUMP_UNICODE_UTF8_2BYTE	__TMS_USBPUMP_UNICODE_UTF8_2BYTE
# define USBPUMP_UNICODE_UTF8_3BYTE	__TMS_USBPUMP_UNICODE_UTF8_3BYTE
# define USBPUMP_UNICODE_UTF8_4BYTE	__TMS_USBPUMP_UNICODE_UTF8_4BYTE
# define USBPUMP_ISO10646_UTF8_1BYTE	__TMS_USBPUMP_ISO10646_UTF8_1BYTE
# define USBPUMP_ISO10646_UTF8_2BYTE	__TMS_USBPUMP_ISO10646_UTF8_2BYTE
# define USBPUMP_ISO10646_UTF8_3BYTE	__TMS_USBPUMP_ISO10646_UTF8_3BYTE
# define USBPUMP_ISO10646_UTF8_4BYTE	__TMS_USBPUMP_ISO10646_UTF8_4BYTE
# define USBPUMP_ISO10646_UTF8_5BYTE	__TMS_USBPUMP_ISO10646_UTF8_5BYTE
# define USBPUMP_ISO10646_UTF8_6BYTE	__TMS_USBPUMP_ISO10646_UTF8_6BYTE
# define USBPUMP_UTF16_MAPBASE	__TMS_USBPUMP_UTF16_MAPBASE
# define USBPUMP_UTF16_MAPTOP	__TMS_USBPUMP_UTF16_MAPTOP
# define USBPUMP_UTF16_FIRSTMAP	__TMS_USBPUMP_UTF16_FIRSTMAP
# define USBPUMP_UTF16_SECONDMAP	__TMS_USBPUMP_UTF16_SECONDMAP
# define USBPUMP_UTF16_LASTMAP	__TMS_USBPUMP_UTF16_LASTMAP
# define USBPUMP_UTF16_NWORDS(x)	__TMS_USBPUMP_UTF16_NWORDS(x)
# define USBPUMP_UTF32_MAP16_FIRST(w)	__TMS_USBPUMP_UTF32_MAP16_FIRST(w)
# define USBPUMP_UTF32_MAP16_SECOND(w)	__TMS_USBPUMP_UTF32_MAP16_SECOND(w)
# define USBPUMP_UTF32_FROM_UTF16(x,y)	__TMS_USBPUMP_UTF32_FROM_UTF16(x,y)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_unicode.h ****/
#endif /* _USBPUMP_UNICODE_H_ */
