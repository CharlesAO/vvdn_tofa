/* uinterfaceset.h	Tue Mar 19 2013 15:54:58 chwon */

/*

Module:  uinterfaceset.h

Function:
	Home for UINTERFACESET

Version:
	V3.11c	Tue Mar 19 2013 15:54:58 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2002, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.79a  Mon May 27 2002 22:56:48  tmm
	Module created.

   1.79b  12/6/2002  chwon
	Use __TMS_CONCAT() instead of ##. This is more portable.

   3.11c  Tue Mar 19 2013 15:54:59  chwon
	16402: Added uifcset_bFlags in the UINTERFACESET and rearrange
	variables in the UINTERFACESET.

*/

#ifndef _UINTERFACESET_H_		/* prevent multiple includes */
#define _UINTERFACESET_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

/****************************************************************************\
|
|	Interface sets represet each interface; each interface has a
|	collection of "alternative settings".  There is one interface
|	set per interface.
|
|	Again, interface sets are not thought to be normally virtualized,
|	so we do the usual dodges.
|
\****************************************************************************/


#if WANT_HW_DERIVED_UIFCSET
# define	__SDEF	struct TTUSB_UINTERFACESET_HDR
# define	__F(p)	__TMS_CONCAT(uifcsethh_, p)
#else
# define	__SDEF	struct TTUSB_UINTERFACESET
# define	__F(p)	__TMS_CONCAT(uifcset_, p)
#endif /* WANT_HW_DERIVED_UIFCSET */

/**** the contents of the embedded header ****/
__SDEF
	{
	__TMS_UCONFIG	*__F(pConfig);		/* the config that owns us */
	__TMS_UINTERFACE *__F(pInterfaces);	/* vector of pointers to
						|| possible alternates.
						*/
	__TMS_UINTERFACE *__F(pCurrent);	/* currently selected ifc */
	__TMS_VOID	*__F(pExtension);	/* application extension */
	__TMS_UEVENTNODE *__F(noteq);		/* notification queue */
	__TMS_UINT8	__F(bNumAltSettings);	/* size of vector */
	__TMS_UINT8	__F(bFlags);		/* flags */
#if !WANT_HW_DERIVED_UIFCSET
	__TMS_VOID	*__F(pHwExtension);	/* hw extension */
#endif
	};

#undef	__SDEF
#undef	__F

/*---------- Things for derived types ----------*/
#if WANT_HW_DERIVED_UIFCSET

 /**** the macro to embed in derived structures ****/
# define	UINTERFACESET_HDR uifcset_hh

 /**** the generic structure ****/
 struct TTUSB_UINTERFACESET
	{
	UINTERFACESET_HDR;
	};

 /**** sane names ****/
# define	uifcset_pConfig		uifcset_hh.uifcsethh_pConfig
# define	uifcset_pInterfaces	uifcset_hh.uifcsethh_pInterfaces
# define	uifcset_pCurrent	uifcset_hh.uifcsethh_pCurrent
# define	uifcset_noteq		uifcset_hh.uifcsethh_noteq
# define	uifcset_pExtension	uifcset_hh.uifcsethh_pExtension
# define	uifcset_bNumAltSettings	uifcset_hh.uifcsethh_bNumAltSettings
# define	uifcset_bFlags		uifcset_hh.uifcsethh_bFlags
#endif /* WANT_HW_DERIVED_UIFCSET */

/**** use UINTERFACESET_INDEX to index to the next interface set ****/
#if WANT_HW_DERIVED_UIFCSET
# define	UINTERFACESET_SIZE(p)	((p)->uifcset_Size)
# define	UINTERFACESET_SETSIZE(p,s) ((p)->uifcset_Size = (s))
# define	UINTERFACESET_INDEX(newp, p, i)	\
    do	{ \
	int __i = (i); \
	(newp) = (p); \
	for (; __i > 0; --__i) \
		(newp) = (__TMS_UINTERFACESET *) ((char *)(newp) + UINTERFACESET_SIZE(newp)); \
	} while (0)
# define	UINTERFACESET_NEXT(p) \
			((__TMS_UINTERFACESET *)((char *)(p) + UINTERFACESET_SIZE(p)))
#else
# define	UINTERFACESET_SIZE(p)		(sizeof(__TMS_UINTERFACESET))
# define	UINTERFACESET_SETSIZE(p,s)	__TMS_SKIP()
# define	UINTERFACESET_INDEX(newp, p, i)	((newp) = (p) + (i))
# define	UINTERFACESET_NEXT(p)		((p) + 1)
#endif

/* definition of uifcset_bFlags */
#define UINTERFACESET_FLAG_PRIMARY	(1u << 0)
#define UINTERFACESET_FLAG_SECONDARY	(1u << 1)

/**** end of uinterfaceset.h ****/
#endif /* _UINTERFACESET_H_ */
