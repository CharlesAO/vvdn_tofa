/* uconfig.h	Fri Dec  6 2002 10:14:10 chwon */

/*

Module:  uconfig.h

Function:
	Home for UCONFIG structure.

Version:
	V1.79b	Fri Dec  6 2002 10:14:10 chwon	Edit level 3

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
   1.79a  Mon May 27 2002 22:54:45  tmm
	Module created.

   1.79a  5/28/2002  tmm
	Add ucfg_pConfigDesc.

   1.79b  12/6/2002  chwon
	Use __TMS_CONCAT() instead of ##. This is more portable.

*/

#ifndef _UCONFIG_H_		/* prevent multiple includes */
#define _UCONFIG_H_

/****************************************************************************\
|
|	UCONFIG structures represent a single configuration of the device.
|
\****************************************************************************/


#if WANT_HW_DERIVED_UCONFIG
# define __SDEF	struct TTUSB_UCONFIG_HDR
# define __F(p)	__TMS_CONCAT(ucfghh_, p)
#else
# define __SDEF	struct TTUSB_UCONFIG
# define __F(p)	__TMS_CONCAT(ucfg_, p)
#endif

/**** the contents of the embedded header ****/
__SDEF
	{
	__TMS_UDEVICE	*__F(pDevice);		/* pointer to device owning us */
	__TMS_UINTERFACESET *__F(pInterfaceSets);
						/* pointer to vector of interface
						|| set descriptions.
						*/
	__TMS_UEVENTNODE *__F(noteq);		/* notification queue */
	__TMS_VOID	*__F(pExtension);	/* app extension */

	/*
	|| For convenience, we store a pointer to our config descriptor
	|| here.
	*/
	__TMS_CONST __TMS_USBIF_CFGDESC_WIRE	*__F(pCfgDesc);

#if !WANT_HW_DERIVED_UCONFIG
	__TMS_VOID	*__F(pHwExtension);	/* hw extension */
#else
	__TMS_UINT8	_F(Size);		/* size of this structure */
#endif
	__TMS_UINT8	__F(bNumInterfaces);	/* number of interface sets */
	};

#undef	__SDEF
#undef	__F

/*---------- Derived configuration -- additional defs ----------*/
#if WANT_HW_DERIVED_UCONFIG
 /**** the macro to embed in derived structures ****/
# define	UCONFIG_HDR	struct TTUSB_UCONFIG_HDR ucfg_hh

 /**** the generic structure ****/
 struct TTUSB_UCONFIG
	{
	UCONFIG_HDR;
	};

/**** sane names ****/
# define	ucfg_pDevice		ucfg_hh.ucfghh_pDevice
# define	ucfg_pInterfaceSets	ucfg_hh.ucfghh_pInterfaceSets
# define	ucfg_pCfgDesc		ucfg_hh.ucfghh_pCfgDesc
# define	ucfg_Size		ucfg_hh.ucfghh_Size
# define	ucfg_bNumInterfaces	ucfg_hh.ucfghh_bNumInterfaces
# define	ucfg_noteq		ucfg_hh.ucfghh_noteq
#endif /* WANT_HW_DERIVED_UCONFIG */

/**** use UCONFIG_INDEX to index to the next configuration ****/
#if WANT_HW_DERIVED_UCONFIG
# define	UCONFIG_SIZE(p)	((p)->ucfg_Size)
# define	UCONFIG_SETSIZE(p,s)	((p)->ucfg_Size = (s))
# define	UCONFIG_INDEX(newp, p, i)	\
    do	{ \
	int __i = (i); \
	(newp) = (p); \
	for (; __i > 0; --__i) \
		(newp) = (__TMS_UCONFIG *) ((char *)(newp) + UCONFIG_SIZE(newp)); \
	} while (0)
# define	UCONFIG_NEXT(p)	((__TMS_UCONFIG *) ((char *)(p) + UCONFIG_SIZE(p)))

#else
# define	UCONFIG_SIZE(p)			(sizeof(__TMS_UCONFIG))
# define	UCONFIG_SETSIZE(p,s)		__TMS_SKIP()
# define	UCONFIG_INDEX(newp, p, i)	((newp) = (p) + (i))
# define	UCONFIG_NEXT(p)			((p) + 1)
#endif


/**** end of uconfig.h ****/
#endif /* _UCONFIG_H_ */
