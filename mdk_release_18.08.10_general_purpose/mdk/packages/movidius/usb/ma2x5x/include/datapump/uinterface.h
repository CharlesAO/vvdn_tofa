/* uinterface.h	Wed Jan 04 2012 19:14:54 chwon */

/*

Module:  uinterface.h

Function:
	Home for UINTERFACE structure.

Version:
	V3.01f	Wed Jan 04 2012 19:14:54 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2002, 2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.79a  Mon May 27 2002 22:58:30  tmm
	Module created from 1.63a usbpump.h.

   1.79a  5/28/2002  tmm
	600032/1:  add uifc_pDataPlaneNode and uifc_pIfcDesc

   1.79a  6/16/2002  tmm
	600032/1.1.16:  add links for use in forming the interface
	descriptors.

   1.79b  12/6/2002  chwon
	Use __TMS_CONCAT() instead of ##. This is more portable.

   3.01f  Wed Jan 04 2012 19:14:54  chwon
	14623: Added uifc_bStatus for super-speed device support.

*/

#ifndef _UINTERFACE_H_		/* prevent multiple includes */
#define _UINTERFACE_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

/****************************************************************************\
|
|	The standard "interface" structure -- there's one of these
|	for each interface.  Virtualization is thought to be rare, so
|	we use extension pointers for hw level.
|
\****************************************************************************/

#if WANT_HW_DERIVED_UINTERFACE
# define	__SDEF	struct TTUSB_UINTERFACE_HDR
# define	__F(p)	__TMS_CONCAT(uifchh_, p)

#else /* !WANT_HW_DERIVED_INTEFACE */
# define	__SDEF	struct TTUSB_UINTERFACE
# define	__F(p)	__TMS_CONCAT(uifc_, p)
#endif /* !WANT_HW_DERIVED_UINTERFACE */

/**** the contents of the embedded header ****/
__SDEF
	{
	__TMS_UINTERFACESET *__F(pInterfaceSet);	
						/* interface set that owns this ep */
	__TMS_UPIPE *__F(pPipes);		/* pointer to first endpoint */
	__TMS_UEVENTNODE *__F(noteq);		/* notification queue */
	__TMS_VOID *__F(pExtension);		/* application extension */

	/*
	|| The dataplane pointer is the linkage to the higher-level (structure
	|| agnostic) function drivers.  It might be null if the higher level
	|| doesn't use the dataplane abstraction.
	*/
	__TMS_UDATAPLANE	*__F(pDataPlane);

	/*
	|| Each UINTERFACE can be linked together with other, functionally
	|| equivalent UINTERFACEs.  This linking is not done by the core,
	|| but for convenience, the core provides functional next and last
	|| links.  These pointers will be NULL if the higher levels don't
	|| use the dataplane (or some other) abstraction.
	*/
	__TMS_UINTERFACE	*__F(pFunctionIfcNext);
	__TMS_UINTERFACE	*__F(pFunctionIfcLast);

	/*
	|| For convenience, we store a copy of the config descriptor here.
	*/
	__TMS_CONST __TMS_USBIF_IFCDESC_WIRE *__F(pIfcDesc);

	/*
	|| Depending on how we are configured, we either put a pointer to
	|| a chip-driver context block here, or else we make this a variable
	|| length structure.  The DERIVED_UINTERFACE code is untested and
	|| unsupported.
	*/
#if WANT_HW_DERIVED_UINTERFACE
	__TMS_UINT8	__F(Size);		/* true size of structure */
#else
	__TMS_VOID	*__F(pHwExtension);	/* hardware extension */
#endif
	__TMS_UINT8	__F(bNumPipes);		/* number of endpoints */
	__TMS_UINT8	__F(bAlternateSetting);	/* the alternate setting code */
	__TMS_UINT8	__F(bStatus);		/* interface status for USB3 */
	};

/*-------------- additional stuff if derived ---------------*/

#if WANT_HW_DERIVED_UINTERFACE

/**** the macro to embed in derived structures ****/
# define	UINTERFACE_HDR	struct TTUSB_UINTERFACE_HDR uifc_hh

/**** the generic structure ****/
struct TTUSB_UINTERFACE
	{
	UINTERFACE_HDR;
	};

/**** sane names ****/
# define	uifc_pInterfaceSet	uifc_hh.uifchh_pInterfaceSet
# define	uifc_bNumPipes		uifc_hh.uifchh_bNumPipes
# define	uifc_pPipes		uifc_hh.uifchh_pPipes
# define	uifc_noteq		uifc_hh.uifchh_noteq
# define	uifc_pExtension		uifc_hh.uifchh_pExtension
# define	uifc_pDataPlane		uifc_hh.uifchh_pDataPlane
# define	uifc_pFunctionIfcNext	uifc_hh.uifchh_pFunctionIfcNext
# define	uifc_pFunctionIfcLast	uifc_hh.uifchh_pFunctionIfcLast
# define	uifc_pIfcDesc		uifc_hh.uifchh_pIfcDesc
# define	uifc_bAlternateSetting	uifc_hh.uifchh_bAlternateSetting
# define	uifc_bStatus		uifc_hh.uifchh_bStatus
# define	uifc_Size		uifc_hh.uifchh_Size
#endif /* !WANT_HW_DERIVED_UINTERFACE */

/**** use UINTERFACE_INDEX to index to the next interface ****/
#if WANT_HW_DERIVED_UINTERFACE
# define	UINTERFACE_SIZE(p)	((p)->uifc_Size)
# define	UINTERFACE_SETSIZE(p,s)	((p)->uifc_Size = (s))
# define	UINTERFACE_INDEX(newp, p, i)	\
    do	{ \
	int __i = (i); \
	(newp) = (__TMS_UINTERFACE *) (p); \
	for (; __i > 0; --__i) \
		(newp) = (__TMS_UINTERFACE *) ((char *)(newp) + UINTERFACE_SIZE(newp)); \
	} while (0)
# define	UINTERFACE_NEXT(p) \
			((__TMS_UINTERFACE *) ((char *)(p) + UINTERFACE_SIZE(p)))
#else
# define	UINTERFACE_SIZE(p)		(sizeof(__TMS_UINTERFACE))
# define	UINTERFACE_INDEX(newp, p, i)	((newp) = (p) + (i))
# define	UINTERFACE_NEXT(p)		((p) + 1)
# define	UINTERFACE_SETSIZE(p, s)	__TMS_SKIP()
#endif

/**** all done *****/
#undef	__F
#undef	__SDEF

/**** end of uinterface.h ****/
#endif /* _UINTERFACE_H_ */
