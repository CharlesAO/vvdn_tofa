/* usbrctab.h	Mon Oct 12 2015 11:28:06 chwon */

/*

Module:  usbrctab.h

Function:
	Sample header file for use when compiling files produced by
	usbrc.

Version:
	V3.43a	Mon Oct 12 2015 11:28:06 chwon	Edit level 20

Copyright notice:
	This file copyright (C) 1997-1998, 2005, 2007-2012, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	Distribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistribution of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. MCCI's name may not be used to endorse or promote products
	   derived from this software without specific prior written
	   permission.

Disclaimer:
	THIS FILE IS PROVIDED BY MCCI "AS IS" AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED.  IN NO EVENT SHALL MCCI BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
	GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Author:
	Terry Moore, MCCI Corporation	July 1997

Revision history:
   1.00a  Sun Jul 20 1997 13:18:55  tmm
	Module created.

   1.25b  8/6/1997  tmm
	Don't include stddef.h unless we need null.

   1.26b  9/25/1997  tmm
	Delete the trailing semicolon in the ROOTTAB macro.

   2.10a  9/19/1998  tmm
	Create ROOTABLE_V2 macro for use when user wants string handling.

   2.10b  9/29/98  june
	Change the protocol for __USBRCTAB_SETDESCRIPTOR_FN.

   2.10d  11/7/1998  tmm
	Create *_NOCLASS macros so we can keep Microsoft C happy, and
	not need empty macro args.

   2.23b  Sat Sep 24 2005 10:22:34 tmm
   	Add high-speed support to root descriptor table, and make _V3()
	macros.

   2.23e  Tue Nov 22 2005 09:47:40  tmm
	Integrate pointer to init function here, so that we can package
	everything with a single pointer.

   2.23f  Sat Nov 26 2005 11:12:48  tmm
	Reorg for backwards compatibility.  Create V2 and V3 versions of
	the structures, and make sure the V3 version is simply an extension
	of the V2 version.

   2.99a  Fri Dec 07 2007 13:19:25  yssaroha
	Added support for Wireless security and BOS descriptors. Speed
	specific descriptors will be maintained in the newly added structure.
	Remove support for __USBRCTAB_WANT_OTHERS.

   2.99a  Mon Jan 07 2008 16:12:15  yssaroha
	Removed space after line continuation character
	
   3.00  Mon Jan 21 2008 22:13:12  yssaroha
	Updated Version information

   3.06  Wed Apr 09 2008 12:39:38  yssaroha
	Updated Version information. In this version
	support is added for compiling urc file only with 
	mtp configuration information.

   3.10  Tue Oct 28 2008 15:56:00  muthukrishnan
	Updated Version information.

   3.11c  Thu May 21 2009 15:40:00  balaji
	Added V5 macro for super-speed.

   3.12  Fri Jun 05 2009 12:51:20  balaji
	8189: Added macro USBRCTAB_SPEED_DEPENDENT_DESCRIPTORS_T for
	speed specific descriptor table declaration.

   3.21a  Tue Jun 23 2009 17:37:20  balaji
	Updated Version information.

   3.24  Tue Sep 29 2009 20:25:00  balaji
	Updated Version information.

   3.32  Tue Feb 16 2010 12:40:00  balaji
	Updated Version information.

   3.34  Thu Oct 28 2010 12:41:35  balaji
	Updated Version information.

   3.35a  Fri May 13 2011 17:11:19  balaji
	13103: Changed the root table to include endpoint mapping info 
	that required for dynamic device initialization.

   3.35b  Thu May 19 2011 11:01:33  balaji
	13103: Added TTUSB_UDEVICESWITCH struct to use it in root table.

   3.41a  Tue May 01 2012 17:08:24  balaji
	Updated Version information.

   3.43a  Mon Oct 12 2015 11:28:06  chwon
	Update version to V3.44.

*/

#ifndef _USBRCTAB_H_		/* prevent multiple includes */
#define _USBRCTAB_H_

/**** USBRC sometimes emits a bare "NULL", so we need stddef.h ****/
#ifndef USBRCTAB_STDDEF_H
# define USBRCTAB_STDDEF_H	<stddef.h>
#endif

#ifndef NULL
# include USBRCTAB_STDDEF_H
#endif

/****************************************************************************\
|
|	Set the version
|
\****************************************************************************/

/*
|| The version is (major << 16) + ((minor << 8) + (letter - 'a' + 1);
|| write it as a hex number.
||
|| Prior to V2.23f, there was no version defined.
*/
#define	__USBRCTAB_VERSION	0x034400	/* 3.44 */

/****************************************************************************\
|
|	Set defaults for the adjustable parameters.
|
\****************************************************************************/

/*
|| on some compilers (for tiny machines) you need to specify the
|| storage class.  Here, all we care about is ROM.  By default,
|| we assume that a declaration of const is enough.
*/
#ifndef __USBRCTAB_ROM
# define __USBRCTAB_ROM			/* nothing */
#endif

/*
|| on most compilers, you need to do a lot of "const" decoration if
|| things are really const.  We define it this way to allow it to be
|| overridden if necesary
*/
#ifndef __USBRCTAB_CONST
# define __USBRCTAB_CONST	const
#endif

/*
|| USBRC will generate chip-information.  This is not used
|| on older datapumps, but we set it to 1 by default.
*/
#ifndef __USBRCTAB_WANT_V6
# define __USBRCTAB_WANT_V6	1
#endif

/*
|| USBRC will generate super-speed descriptors.  This is not used
|| on older datapumps, but we set it to 1 by default.
*/
#ifndef __USBRCTAB_WANT_V5
# define __USBRCTAB_WANT_V5	1
#endif

/*
|| USBRC will generate wireless descriptors.  This is not used
|| on older datapumps, but we set it to 1 by default.
*/
#ifndef __USBRCTAB_WANT_V4
# define __USBRCTAB_WANT_V4	1
#endif

/*
|| WANT_V5 && !WANT_V4 can only be obtained by explicit command-line
|| settings, and it doesn't make sense.  Check for it here.
*/
#if __USBRCTAB_WANT_V5 && !__USBRCTAB_WANT_V4
# error "__USBRCTAB_WANT_V5 cannot be used without __USBRCTAB_WANT_V4"
#endif

/*
|| USBRC will generate high-speed descriptors.  This is not used
|| on older datapumps, so we set it to 0 by default.
*/
#ifndef __USBRCTAB_WANT_V3
# if __USBRCTAB_WANT_V4 || __USBRCTAB_WANT_V5
#   define __USBRCTAB_WANT_V3	1
# else /* !(__USBRCTAB_WANT_V4 || __USBRCTAB_WANT_V5) */
#   define __USBRCTAB_WANT_V3	0
# endif /* __USBRCTAB_WANT_V4 || __USBRCTAB_WANT_V5 */
#endif /* __USBRCTAB_WANT_V3 */

/*
|| USBRC will generate a table of "other" descriptors.  This is not
|| normally used, so we suppress the generation of the data.
*/
#ifndef __USBRCTAB_WANT_OTHERS
#  define __USBRCTAB_WANT_OTHERS 0
#endif

#if __USBRCTAB_WANT_OTHERS
# error "__USBRCTAB_WANT_OTHERS is not supported"
#endif

/*
|| WANT_V5 && !WANT_V3 can only be obtained by explicit command-line
|| settings, and it doesn't make sense.  Check for it here.
*/
#if __USBRCTAB_WANT_V5 && !__USBRCTAB_WANT_V3
# error "__USBRCTAB_WANT_V5 cannot be used without __USBRCTAB_WANT_V3"
#endif

/*
|| WANT_V4 && !WANT_V3 can only be obtained by explicit command-line
|| settings, and it doesn't make sense.  Check for it here.
*/
#if __USBRCTAB_WANT_V4 && !__USBRCTAB_WANT_V3
# error "__USBRCTAB_WANT_V4 cannot be used without __USBRCTAB_WANT_V3"
#endif

/*
|| Code that uses the tables also wants to use this file, but may
|| not want the namespace polluted by the the generation macros.
|| If __USBRCTAB_TYPESONLY is defined to 1, we'll only define the various
|| structures.
*/
#ifndef __USBRCTAB_TYPESONLY
# define __USBRCTAB_TYPESONLY	0	/* everything by default */
#endif

/*
|| If you are using this file from your code, you probably want to do
|| something like this:
||
||	#define	__USBRCTAB_ROM		// whatever is appropriate
||	#define	__USBRCTAB_CONST	// whatever is appropriate
||	#define __USBRCTAB_TYPESONLY	1
||	#include "usbrctab.h"
||
||	typedef struct __USBRCTAB_RESPTR  USBRC_RESPTR;
||	typedef	struct __USBRCTAB_LANGIDPTR USBRC_LANGIDPTR;
||	typedef struct __USBRCTAB_ROOTTABLE USBRC_ROOTTABLE;
||
|| Probably you would put this in your project include file, rather than
|| inserting it into each .c file that needs the resource table; but this
|| is up to you!  Alternately, if you can live with the namespace
|| that is consumed by the USBRC definitions, you can omit TYPESONLY,
|| and use the "-i" switch on usbrc to cause the emitted file to use
|| your project's include file instead of the default.
*/

/****************************************************************************\
|
|	If we're going to do V3 tables, we need additional data types
|	from the datapump
|
\****************************************************************************/

#if __USBRCTAB_WANT_V3
# ifndef _USBPUMP_TYPES_H_

struct TTUSB_UDEVICE;
struct TTUSB_UDEVICESWITCH;
typedef void __TMS_UDEVICE_INITFN(struct TTUSB_UDEVICE *, unsigned long);

# endif
#endif

/****************************************************************************\
|
|	Define macros used for generating the tables.
|
\****************************************************************************/

/*
|| Define the keyword used for creating resource data
*/
#if !__USBRCTAB_TYPESONLY
# define RESCHAR __USBRCTAB_ROM __USBRCTAB_CONST unsigned char
#endif

/*
|| Define the basic resource pointer structure.
*/
struct __USBRCTAB_RESPTR
	{
	__USBRCTAB_ROM __USBRCTAB_CONST unsigned char	*pDescr;	/* pointer to descriptor */
	};

/*
|| Declare the macros used by USBRC to create resource tables.
*/
#if !__USBRCTAB_TYPESONLY

/* define the macro that creates a resource table */
# define RESPTRS	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_RESPTR

/* define the initializer for a resource table */
# define RESPTRVAL(name, offset)		{ (name)+(offset) }
# define RESPTR_NULL	((RESCHAR *) 0)

#endif /* __USBRCTAB_TYPESONLY */


/*
|| Define the structure that holds language mapping information.
*/
struct __USBRCTAB_LANGIDPTR
	{
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_RESPTR *pTable;	/* pointer to table */
	unsigned short	LangId;		/* the language ID */
	unsigned char	NumDescr;	/* number of descriptors */
	};

/*
|| Define the function type for initialization
*/
#if __USBRCTAB_WANT_V3
# ifndef __USBRCTAB_DCD_INIT_FN_DEFINED
#  define __USBRCTAB_DCD_INIT_FN_DEFINED
typedef __TMS_UDEVICE_INITFN __USBRCTAB_DCD_INIT_FN;
# endif /* __USBRCTAB_DCD_INIT_FN_DEFINED */
#endif /* __USBRCTAB_WANT_V3 */
/*
|| Define the function types for descriptor filtering:
*/
#ifndef __USBRCTAB_SETDESCRIPTOR_FN_DEFINED
# define __USBRCTAB_SETDESCRIPTOR_FN_DEFINED
struct TTUSB_UDEVICE;
typedef int __USBRCTAB_SETDESCRIPTOR_FN(
		struct TTUSB_UDEVICE *,
		unsigned char,		/* type */
		unsigned char,		/* index */
		unsigned short,		/* language id or 0 */
		unsigned short,		/* overall length */
		unsigned short,		/* index this call */
		const unsigned char *,	/* descriptor prototype */
		unsigned short,		/* length of descr proto */
		const unsigned char *,	/* buffer containing data from host */
		unsigned short		/* RAM buffer length */
		);
#endif /* __USBRCTAB_SETDESCRIPTOR_FN_DEFINED */

#ifndef __USBRCTAB_GETDESCRIPTOR_FN_DEFINED
# define __USBRCTAB_GETDESCRIPTOR_FN_DEFINED
struct TTUSB_UDEVICE;
typedef int __USBRCTAB_GETDESCRIPTOR_FN(
		struct TTUSB_UDEVICE *,
		unsigned char,		/* type */
		unsigned char,		/* index */
		unsigned short,		/* language id or 0 */
		unsigned short,		/* index this call */
		const unsigned char *,	/* descriptor prototype */
		unsigned short,		/* length of descr proto */
		unsigned char *,	/* RAM buffer */
		unsigned short,		/* RAM buffer length */
		const unsigned char **,	/* OUT:  result base */
		unsigned short *	/* OUT:  result length */
		);
#endif /* __USBRCTAB_GETDESCRIPTOR_FN_DEFINED */

#ifndef __USBRCTAB_BITMAP_ELEM_DEFINED
# define __USBRCTAB_BITMAP_ELEM_DEFINED
typedef	unsigned char __USBRCTAB_BITMAP_ELEM;
#endif /* ! __USBRCTAB_BITMAP_ELEM_DEFINED */

/*
|| struct __USBRCTAB_OS_FEATURE_ROOT is used to provide information that
|| is specific to implementing Microsoft's 0xEE string descriptor hack.
|| Unfortunately, the details of that hack are not publicly available, so
|| we do not describe it further here.
*/
#ifndef __USBRCTAB_OS_FEATURE_ROOT_DEFINED
# define __USBRCTAB_OS_FEATURE_ROOT_DEFINED
struct __USBRCTAB_OS_FEATURE_ROOT;
#endif /* ! __USBRCTAB_OS_FEATURE_ROOT_DEFINED */

#if !__USBRCTAB_TYPESONLY
# ifndef USBRCTAB_DECLARE_DCD_INIT_FN
/*
|| the following are defined with semicolons so you can make the
|| entire statement disappear if need be.
*/
#  define USBRCTAB_DECLARE_DCD_INIT_FN(storageClass, name) \
	storageClass __USBRCTAB_DCD_INIT_FN name;
# endif /* USBRCTAB_DECLARE_DCD_INIT_FN */

# ifndef USBRCTAB_DECLARE_DCD_INIT_FN_NOCLASS
/*
|| If an empty parameter is used, some compilers get
|| unhappy.  So we provide this version.
*/
#  define USBRCTAB_DECLARE_DCD_INIT_FN_NOCLASS(name) \
	__USBRCTAB_DCD_INIT_FN name;
# endif /* USBRCTAB_DECLARE_DCD_INIT_FN_NOCLASS */


# ifndef USBRCTAB_DECLARE_GETDESCRIPTOR_FN
/*
|| the following are defined with semicolons so you can make the
|| entire statement disappear if need be.
*/
#  define USBRCTAB_DECLARE_GETDESCRIPTOR_FN(name) \
		__USBRCTAB_GETDESCRIPTOR_FN name;
# endif
# ifndef USBRCTAB_DECLARE_SETDESCRIPTOR_FN
#  define USBRCTAB_DECLARE_SETDESCRIPTOR_FN(name) \
		__USBRCTAB_SETDESCRIPTOR_FN name;
# endif /* USBRCTAB_DECLARE_GETDESCRIPTOR_FN */

# ifndef USBRCTAB_DECLARE_BITMAP
#  define USBRCTAB_DECLARE_BITMAP(class, name) \
	class __USBRCTAB_ROM __USBRCTAB_CONST __USBRCTAB_BITMAP_ELEM (name)[]
# endif /* USBRCTAB_DECLARE_BITMAP */

# ifndef USBRCTAB_DECLARE_BITMAP_NOCLASS
#  define USBRCTAB_DECLARE_BITMAP_NOCLASS(name) \
	__USBRCTAB_ROM __USBRCTAB_CONST __USBRCTAB_BITMAP_ELEM (name)[]
# endif /* USBRCTAB_DECLARE_BITMAP_NOCLASS */


#endif /* __USBRCTAB_TYPESONLY */

/*
|| Define the macros used by USBRC to create language mapping tables
*/
#if !__USBRCTAB_TYPESONLY

/* use this macro to declarate a langid table */
# define LANGIDPTRS __USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_LANGIDPTR

/* use this macro to set a value for a language table */
# define LANGPTRVAL(id, n, table) 	{ (table), (id), (n) }

#endif /* !__USBRCTAB_TYPESONLY */

/*
|| Define the structure holding speed specific descriptors
|| Currently they will hold the Device, config, BOS and security
|| information.
|| Any other speed specific descriptor will be added when required
|| This gives us the advantage of adding more speeds in future
|| The future incorporation will be easy, code more maintainable
*/
#if __USBRCTAB_WANT_V4 || __USBRCTAB_WANT_V5
struct USBRCTAB_SPEED_DEPENDENT_DESCRIPTORS
	{
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_RESPTR *	pDeviceTable;
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_RESPTR *	pConfigTable;
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_RESPTR *	pBosTable;
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_RESPTR *	pSecurityTable;
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_RESPTR *	pContainerIdTable;

	/* for better alignement, these go at the end */
	unsigned char				nDeviceTable;
	unsigned char				nConfigTable;
	unsigned char				nBosTable;
	unsigned char				nSecurityTable;
	unsigned char				nContainerIdTable;
	};

# define USBRCTAB_SPEED_DEPENDENT_DESCRIPTORS_T	\
	__USBRCTAB_ROM __USBRCTAB_CONST struct USBRCTAB_SPEED_DEPENDENT_DESCRIPTORS

# define USBRCTAB_SPEED_DEPENDENT_DESCRIPTORS_INIT_V2(		\
		pDeviceTable, nDeviceTable,			\
		pConfigTable, nConfigTable,			\
		pBosTable, nBosTable,				\
		pSecurityTable, nSecurityTable,			\
		pContainerIdTable, nContainerIdTable		\
		)						\
		{						\
		(pDeviceTable), 				\
		(pConfigTable),					\
		(pBosTable),					\
		(pSecurityTable),				\
		(pContainerIdTable),				\
		(nDeviceTable), 				\
		(nConfigTable),					\
		(nBosTable),					\
		(nSecurityTable),				\
		(nContainerIdTable)				\
		}

# define USBRCTAB_SPEED_DEPENDENT_DESCRIPTORS_INIT_V1(		\
		pDeviceTable, nDeviceTable,			\
		pConfigTable, nConfigTable,			\
		pBosTable, nBosTable,				\
		pSecurityTable, nSecurityTable			\
		)						\
	USBRCTAB_SPEED_DEPENDENT_DESCRIPTORS_INIT_V2( 		\
		pDeviceTable, nDeviceTable,			\
		pConfigTable, nConfigTable,			\
		pBosTable, nBosTable,				\
		pSecurityTable, nSecurityTable,			\
		/* V2: pContainerIdTable*/ 0,			\
		/* V2: nContainerIdTable */ 0			\
		)

#endif /* __USBRCTAB_WANT_V4 || __USBRCTAB_WANT_V5 */

#if __USBRCTAB_WANT_V6
# define USBRCTAB_DEVICE_SPEED_MASK_LOW		(1u << 0)
# define USBRCTAB_DEVICE_SPEED_MASK_FULL	(1u << 1)
# define USBRCTAB_DEVICE_SPEED_MASK_HIGH	(1u << 2)
# define USBRCTAB_DEVICE_SPEED_MASK_WIRELESS	(1u << 3)
# define USBRCTAB_DEVICE_SPEED_MASK_SUPER	(1u << 4)

struct __USBRCTAB_ENDPOINTMAPTABLE
	{
	unsigned char bEndpointAddress;
	unsigned char iEndpoint;
	};

# define USBRCTAB_ENDPOINTMAPTABLE_T	\
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_ENDPOINTMAPTABLE

#endif /* __USBRCTAB_WANT_V6 */

/*
|| Define the structure of the root table, unless caller has defined
|| it  already.
*/
#ifndef __USBRCTAB_ROOTTABLE_DEFINED
# define __USBRCTAB_ROOTTABLE_DEFINED
struct __USBRCTAB_ROOTTABLE
	{
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_RESPTR *	pDevTbl;
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_RESPTR *	pConfigTbl;
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_LANGIDPTR *	pLangIdTbl;
	__USBRCTAB_GETDESCRIPTOR_FN *					pGetDescriptor;
	__USBRCTAB_SETDESCRIPTOR_FN *					pSetDescriptor;
	__USBRCTAB_ROM __USBRCTAB_CONST __USBRCTAB_BITMAP_ELEM *	pExternalStringBitmap;
	__USBRCTAB_ROM __USBRCTAB_CONST __USBRCTAB_BITMAP_ELEM *	pReadOnlyStringBitmap;

	/* for better alignement, these go at the end */
	unsigned char				nDevTbl;
	unsigned char				nConfigTbl;
	unsigned char				nLangIdTbl;
	unsigned char				nExternalStringBitmap;
	unsigned char				nReadOnlyStringBitmap;

	/*
	|| The following fields were added in V3.  They must be at the end
	|| of the structure in order to avoid binary interop problems.
	|| Old datapump versions will ignore these if present; new
	|| datapump versions will check signatures and ignore data if
	|| the signatures are wrong.
	*/
# if __USBRCTAB_WANT_V3
	unsigned				v3Magic;		/* the magic word */
	unsigned				v3Size;			/* the size */
	unsigned				sizeDcd;		/* V3: size of the DCD in bytes */
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_RESPTR *	pHighSpeedDevTbl;
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_RESPTR *	pHighSpeedConfigTbl;
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_OS_FEATURE_ROOT *
									pOsFeatureRoot;
	__USBRCTAB_DCD_INIT_FN *		pDcdInit;		/* V3: the function to initialize the DCD */
	unsigned char				nHighSpeedDevTbl;	/* V3: number of entries in HS Dev Table */
	unsigned char				nHighSpeedConfigTbl;	/* V3: number of entries in HS config table */
# endif /* __USBRCTAB_WANT_V3 */

# if __USBRCTAB_WANT_V4
	/*
	|| V4: New structure member to hold the pUdeviceSwitch and
	|| member holding the speed dependent descriptors for each
	|| speed. Though currently BOS and security descriptors are
	|| same across speed
	*/
	__USBRCTAB_ROM __USBRCTAB_CONST struct TTUSB_UDEVICESWITCH *	pUdeviceSwitch;

	__USBRCTAB_ROM __USBRCTAB_CONST struct USBRCTAB_SPEED_DEPENDENT_DESCRIPTORS
				* pDescFullSpeed;
	__USBRCTAB_ROM __USBRCTAB_CONST struct USBRCTAB_SPEED_DEPENDENT_DESCRIPTORS
				* pDescHighSpeed;
	__USBRCTAB_ROM __USBRCTAB_CONST struct USBRCTAB_SPEED_DEPENDENT_DESCRIPTORS
				* pDescWireless;
# endif /* __USBRCTAB_WANT_V4 */

# if __USBRCTAB_WANT_V5
	__USBRCTAB_ROM __USBRCTAB_CONST struct USBRCTAB_SPEED_DEPENDENT_DESCRIPTORS
				* pDescSuperSpeed;
# endif /* __USBRCTAB_WANT_V5 */

# if __USBRCTAB_WANT_V6
	__USBRCTAB_ROM __USBRCTAB_CONST char *	pDeviceName;
	unsigned char				bSupportedSpeeds;
	unsigned char				nEndpointMapTable;
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_ENDPOINTMAPTABLE
						* pEndpointMapTable;
# endif /* __USBRCTAB_WANT_V6 */
	};

/* the magic word that goes in the v3Size field */
# define __USBRCTAB_V3MAGIC	0x55726333u

/*
|| If the caller wants the USBRC macros, and hasn't defined a different
|| roottable format, we also need to define ROOTTABLE_V2. Therefore, if
|| the caller redefines the root table format, the caller must also
|| define ROOTABLE_V2.
*/
# if !__USBRCTAB_TYPESONLY
#  ifndef ROOTTABLE_V2
/* do not use NULL in the following, because NULL might not be defined */
#    define ROOTTABLE_V2(name, dev, ndev, cfg, ncfg, lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap \
		) \
	    ROOTTABLE_V3(name, 0, 0, dev, ndev, cfg, ncfg, \
	 	/* hsdev, nhsdev */ 0, 0, /* hscfg, nhscfg */ 0, 0, \
		/* pOsFeatureRoot */ 0, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap \
		)
#  endif /* ndef ROOTTABLE_V2 */
# endif /* !__USBRCTAB_TYPESONLY */

/*
|| If the caller wants the USBRC macros, and hasn't defined a different
|| roottable format, we also need to define ROOTTABLE_V4. Therefore, if
|| the caller redefines the root table format, the caller must also
|| define ROOTABLE_V3.
*/
# if !__USBRCTAB_TYPESONLY
#  ifndef ROOTTABLE_V3
/* do not use NULL in the following, because NULL might not be defined */
#    define ROOTTABLE_V3(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		/* V3 */ pOsFeatureRoot, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap \
		) \
	    ROOTTABLE_V4(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		/* V3 */ pOsFeatureRoot, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 udevice switch*/ 0, \
		/* V4 pDescFullSpeed */ 0, \
		/* V4 pDescHighSpeed */ 0, \
		/* V4 pDescWireless */ 0 \
		)
#  endif /* ndef ROOTTABLE_V3 */
# endif /* !__USBRCTAB_TYPESONLY */

/*
|| If the caller wants the USBRC macros, and hasn't defined a different
|| roottable format, we also need to define ROOTTABLE_V4. Therefore, if
|| the caller redefines the root table format, the caller must also
|| define ROOTABLE_V4.
*/
# if !__USBRCTAB_TYPESONLY
#  ifndef ROOTTABLE_V4
#    define ROOTTABLE_V4(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		/* V3 */ pOsFeatureRoot, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless \
		) \
	    ROOTTABLE_V5(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ 0 \
		)
#  endif /* ndef ROOTTABLE_V4 */
# endif /* !__USBRCTAB_TYPESONLY */

/*
|| If the caller wants the USBRC macros, and hasn't defined a different
|| roottable format, we also need to define ROOTTABLE_V5. Therefore, if
|| the caller redefines the root table format, the caller must also
|| define ROOTABLE_V5.
*/
# if !__USBRCTAB_TYPESONLY
#  ifndef ROOTTABLE_V5
#    define ROOTTABLE_V5(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ pDescSuperSpeed \
		) \
	    ROOTTABLE_V6(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ pDescSuperSpeed, \
		/* V6 */ 0, \
		/* V6 */ 0, \
		/* V6 */ 0, \
		/* V6 */ 0 \
		)
#  endif /* ndef ROOTTABLE_V5 */
# endif /* !__USBRCTAB_TYPESONLY */

/*
|| If the caller wants the USBRC macros, and hasn't defined a different
|| roottable format, we also need to define ROOTTABLE_V6. Therefore, if
|| the caller redefines the root table format, the caller must also
|| define ROOTABLE_V6.
*/
# if !__USBRCTAB_TYPESONLY
#  ifndef ROOTTABLE_V6
#   if __USBRCTAB_WANT_V6
#    define ROOTTABLE_V6(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ pDescSuperSpeed, \
		/* V6 */ pDeviceName, \
		/* V6 */ bSupportedSpeeds, \
		/* V6 */ nEndpointMapTable, \
		/* V6 */ pEndpointMapTable \
		) \
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_ROOTTABLE name = \
		{ \
		(dev), (cfg), \
		(lang), \
		(pGetfn), (pSetfn), (pExtmap), (pRomap), \
		(ndev), (ncfg), \
		(nlang), \
		(nExtmap), (nRomap), \
		/* V3 */ __USBRCTAB_V3MAGIC, \
		/* V3 */ sizeof(struct __USBRCTAB_ROOTTABLE), \
		/* V3 */ (a_sizeDcd), \
		/* V3 */ (hsdev), (hscfg), \
		/* V3 */ 0, \
		/* V3 */ (pDcdInitFn), \
		/* V3 */ (nhsdev), (nhscfg), \
		/* V4 */ (pUdeviceSwitch), \
		/* V4 */ (pDescFullSpeed), \
		/* V4 */ (pDescHighSpeed), \
		/* V4 */ (pDescWireless), \
		/* V5 */ (pDescSuperSpeed), \
		/* V6 */ (pDeviceName), \
		/* V6 */ (bSupportedSpeeds), \
		/* V6 */ (nEndpointMapTable), \
		/* V6 */ (pEndpointMapTable) \
		}
#   elif __USBRCTAB_WANT_V5
#    define ROOTTABLE_V6(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ pDescSuperSpeed, \
		/* V6 */ pDeviceName, \
		/* V6 */ bSupportedSpeeds, \
		/* V6 */ nEndpointMapTable, \
		/* V6 */ pEndpointMapTable \
		) \
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_ROOTTABLE name = \
		{ \
		(dev), (cfg), \
		(lang), \
		(pGetfn), (pSetfn), (pExtmap), (pRomap), \
		(ndev), (ncfg), \
		(nlang), \
		(nExtmap), (nRomap), \
		/* V3 */ __USBRCTAB_V3MAGIC, \
		/* V3 */ sizeof(struct __USBRCTAB_ROOTTABLE), \
		/* V3 */ (a_sizeDcd), \
		/* V3 */ (hsdev), (hscfg), \
		/* V3 */ 0, \
		/* V3 */ (pDcdInitFn), \
		/* V3 */ (nhsdev), (nhscfg), \
		/* V4 */ (pUdeviceSwitch), \
		/* V4 */ (pDescFullSpeed), \
		/* V4 */ (pDescHighSpeed), \
		/* V4 */ (pDescWireless), \
		/* V5 */ (pDescSuperSpeed) \
		}
#   elif __USBRCTAB_WANT_V4
#    define ROOTTABLE_V6(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ pDescSuperSpeed, \
		/* V6 */ pDeviceName, \
		/* V6 */ bSupportedSpeeds, \
		/* V6 */ nEndpointMapTable, \
		/* V6 */ pEndpointMapTable \
		) \
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_ROOTTABLE name = \
		{ \
		(dev), (cfg), \
		(lang), \
		(pGetfn), (pSetfn), (pExtmap), (pRomap), \
		(ndev), (ncfg), \
		(nlang), \
		(nExtmap), (nRomap), \
		/* V3 */ __USBRCTAB_V3MAGIC, \
		/* V3 */ sizeof(struct __USBRCTAB_ROOTTABLE), \
		/* V3 */ (a_sizeDcd), \
		/* V3 */ (hsdev), (hscfg), \
		/* V3 */ (0), \
		/* V3 */ (pDcdInitFn), \
		/* V3 */ (nhsdev), (nhscfg), \
		/* V4 */ (pUdeviceSwitch), \
		/* V4 */ (pDescFullSpeed), \
		/* V4 */ (pDescHighSpeed), \
		/* V4 */ (pDescWireless) \
		}
#   elif __USBRCTAB_WANT_V3
#    define ROOTTABLE_V6(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ pDescSuperSpeed, \
		/* V6 */ pDeviceName, \
		/* V6 */ bSupportedSpeeds, \
		/* V6 */ nEndpointMapTable, \
		/* V6 */ pEndpointMapTable \
		) \
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_ROOTTABLE name = \
		{ \
		(dev), (cfg), \
		(lang), \
		(pGetfn), (pSetfn), (pExtmap), (pRomap), \
		(ndev), (ncfg), \
		(nlang), \
		(nExtmap), (nRomap), \
		/* V3 */ __USBRCTAB_V3MAGIC, \
		/* V3 */ sizeof(struct __USBRCTAB_ROOTTABLE), \
		/* V3 */ (a_sizeDcd), \
		/* V3 */ (hsdev), (hscfg), \
		/* V3 */ (0), \
		/* V3 */ (pDcdInitFn), \
		/* V3 */ (nhsdev), (nhscfg) \
		}
#   else /* __USBRCTAB_WANT_V2 */
#    define ROOTTABLE_V6(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ pDescSuperSpeed, \
		/* V6 */ pDeviceName, \
		/* V6 */ bSupportedSpeeds, \
		/* V6 */ nEndpointMapTable, \
		/* V6 */ pEndpointMapTable \
		) \
	__USBRCTAB_ROM __USBRCTAB_CONST struct __USBRCTAB_ROOTTABLE name = \
		{ \
		(dev), (cfg), \
		(lang), \
		(pGetfn), (pSetfn), (pExtmap), (pRomap), \
		(ndev), (ncfg), \
		(nlang), \
		(nExtmap), (nRomap) \
		}
#   endif /* __USBRCTAB_WANT_V6 */
#  endif /* ROOTTABLE_V6 */
# endif /* !__USBRCTAB_TYPESONLY */

#endif /* __USBRCTAB_ROOTTABLE_DEFINED */

/*
|| Define the macro used by USBRC to create the root table.
|| These are all ultimately defined in terms of ROOTTABLE_V2,
|| and so are portable.
*/
#if !__USBRCTAB_TYPESONLY
# ifndef ROOTTABLE
#  define ROOTTABLE(name, dev, ndev, cfg, ncfg, lang, nlang, oth, noth) \
	ROOTTABLE_V2(name, dev, ndev, cfg, ncfg, lang, nlang, oth, noth, \
		0, 0, 0, 0, 0, 0)
# endif /* ROOTTABLE */

/*
|| The ..._WITH_SEMI forms are used by USBRC, so that the caller can
|| transform the output language completely w/o having to handle
|| a trailing semicolon.
*/

/* usbrc 1.26b and later will ask for this */
# ifndef ROOTTABLE_WITH_SEMI
#  define ROOTTABLE_WITH_SEMI(name, dev, ndev, cfg, ncfg, lang, nlang, oth, noth) \
	ROOTTABLE(name, dev, ndev, cfg, ncfg, lang, nlang, oth, noth);
# endif /* ROOTTABLE_WITH_SEMI */

/* usbrc 2.10a and later will create the root-table using this: */
# ifndef ROOTTABLE_V2_WITH_SEMI
#  define ROOTTABLE_V2_WITH_SEMI(name, dev, ndev, cfg, ncfg, lang, nlang, \
		oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap \
		) \
	ROOTTABLE_V2(name, dev, ndev, cfg, ncfg, lang, nlang, \
		oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap \
		);
# endif /* ROOTTABLE_V2_WITH_SEMI */

/* usbrc with high speed support may create the root-table using this: */
# ifndef ROOTTABLE_V3_WITH_SEMI
#  define ROOTTABLE_V3_WITH_SEMI(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		/* V3 */ pOsFeatureRoot, \
		lang, nlang, \
		oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap \
		) \
	ROOTTABLE_V3(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		/* V3 */ pOsFeatureRoot, \
		lang, nlang, \
		oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap \
		);
# endif /* ROOTTABLE_V3_WITH_SEMI */

/* usbrc with MTP and wireless support may create the root-table using this: */
# ifndef ROOTTABLE_V4_WITH_SEMI
#  define ROOTTABLE_V4_WITH_SEMI(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		/* V3 */ pOsFeatureRoot, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless \
		) \
	ROOTTABLE_V4(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		/* V3 */ pOsFeatureRoot, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless \
		);
# endif /* ROOTTABLE_V4_WITH_SEMI */

/* USBRC with super-speed and wireless support may create the root-table using this: */
# ifndef ROOTTABLE_V5_WITH_SEMI
#  define ROOTTABLE_V5_WITH_SEMI(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ pDescSuperSpeed \
		) \
	ROOTTABLE_V5(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ pDescSuperSpeed \
		);
# endif /* ROOTTABLE_V5_WITH_SEMI */

/* USBRC with dynamic initialzation support may create the root-table using this: */
# ifndef ROOTTABLE_V6_WITH_SEMI
#  define ROOTTABLE_V6_WITH_SEMI(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ pDescSuperSpeed, \
		/* V6 */ pDeviceName, \
		/* V6 */ bSupportedSpeeds, \
		/* V6 */ nEndpointMapTable, \
		/* V6 */ pEndpointMapTable \
		) \
	ROOTTABLE_V6(name, \
		/* V3 */ pDcdInitFn, a_sizeDcd, \
		dev, ndev, cfg, ncfg, \
		/* V3 */ hsdev, nhsdev, hscfg, nhscfg, \
		lang, nlang, oth, noth, \
		pGetfn, pSetfn, pExtmap, nExtmap, pRomap, nRomap, \
		/* V4 */ pUdeviceSwitch, \
		/* V4 */ pDescFullSpeed, \
		/* V4 */ pDescHighSpeed, \
		/* V4 */ pDescWireless, \
		/* V5 */ pDescSuperSpeed, \
		/* V6 */ pDeviceName, \
		/* V6 */ bSupportedSpeeds, \
		/* V6 */ nEndpointMapTable, \
		/* V6 */ pEndpointMapTable \
		);
# endif /* ROOTTABLE_V6_WITH_SEMI */
#endif /* !__USBRCTAB_TYPESONLY */

/**** end of usbrctab.h ****/
#endif /* _USBRCTAB_H_ */
