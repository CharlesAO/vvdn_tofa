/* usbpump_filter_types.h	Thu Sep 05 2013 10:53:39 chwon */

/*

Module:  usbpump_filter_types.h

Function:
	Definition of USB filter function type

Version:
	V3.11d	Thu Sep 05 2013 10:53:39 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	September 2013

Revision history:
   3.11d  Thu Sep 05 2013 10:53:39 chwon
	17377: Module created.

*/

#ifndef _USBPUMP_FILTER_TYPES_H_	/* prevent multiple includes */
#define _USBPUMP_FILTER_TYPES_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

/****************************************************************************\
|
|	Definition of "get descriptor" filter function type
|
\****************************************************************************/

/*

Type:	USBPUMP_FILTER_GETDESCRIPTOR_FN

Index:	Type:	PUSBPUMP_FILTER_GETDESCRIPTOR_FN

Function:
	Signature of generic "get descriptor" filter methods.

Definition:
	typedef int USBPUMP_FILTER_GETDESCRIPTOR_FN(
		UDEVICE *		pDevice,
		unsigned char		DescType,
		unsigned char		DescIndex,
		unsigned short		DescLangId,
		unsigned short		CallIndex,
		const unsigned char *	pRom,
		unsigned short		RomSize,
		unsigned char *		pRam,
		unsigned short		RamSize,
		const unsigned char **	ppResult, 
		unsigned short *	pResultLen
		);

	typedef USBPUMP_FILTER_GETDESCRIPTOR_FN 
		*PUSBPUMP_FILTER_GETDESCRIPTOR_FN;

Description:
	Functions of this type are used as the "get descriptor" function
	given in the URC file for a DataPump device application. 
	The DataPump calls the get descriptor function whenever it
	receives a "get descriptor" request on endpoint zero, after
	locating the precompiled response created by USBRC based on 
	the URC file, and before returning the result to the host.
	A single filter function is given in the URC file, but it 
	is common for the top level filter function to delegate 
	the computation to one or more built-in filter functions. To
	simplify this, all filter functions generally have the same
	function type.

	pDevice is the pointer to the UDEVICE for which descriptors
	are being returned.

	DescType, DescIndex and DescLangId come from the SETUP packet
	and have the obvious meaning.

	pRom is a pointer to the descriptor that USBRC prepared, and
	RomSize is the length of that descriptor.  

	pRam is a pointer to a RAM buffer that should be used by the 
	filter function to prepare any results.  RamSize is the allocated
	size of the buffer. If the buffer is too small, the filter must
	either return USBPUMP_GETDESRFILTER_FAIL or use some other 
	static buffer.

	If the function returns USBPUMP_GETDESCRFILTER_SEND, it must set 
	*ppResult to point to the data returned, and set *pResultLen to 
	the number of bytes to be transmitted. (Otherwise, it need not
	change *ppResult and *pResultLen).  The caller sets ppResult 
	to NULL and pResultLen to zero prior to caling this function.

	CallIndex is an argument that is provided for future
	use; it's intent is to allow the same user-function to be used
	in a future version of the DataPump that supports constructing
	the reply packet-by-packet, instead of in a single buffer.
	However, this is not implemented yet -- we don't have a good way
	to get a completion callback, or at least an easy one, so we'll
	just leave that for later. In the current implementation, 
	CallIndex will always be zero.

Returns:
	0 (USBPUMP_GETDESCRFILTER_PASS) if the core DataPump is to return the
		ROM descriptor, or query some other information provider.

	1 (USBPUMP_GETDESCRFILTER_SEND) if the core DataPump is to transmit
		the result from *ppResult, *pResultLen. 

	2 (USBPUMP_GETDESCRFILTER_FRAG) if the core DataPump is to transmit
		the result, and then ask for more data (with index-this-
		call suitably advanced by the number of bytes previously
		transmitted) [NOT YET SUPPORTED]

	3 (USBPUMP_GETDESCRFILTER_FAIL) if the core DataPump is to return
		an error (STALL endpoint)

	If the routine doesn't want to filter this descriptor, it can
	just return USBPUMP_GETDESCRFILTER_PASS (0), and the DataPump
	will take the appropriate action.

Notes:
	ppResult is initialized to NULL before this function is called. It's
	a programming error to return USBPUMP_GETDESCRFILTER_SEND
	without setting *ppResult to point to the result buffer. In this case,
	the DataPump will return STALL to the host (and won't crash). For 
	clarity, it's better to return USBPUMP_GETDESRFILTER_FAIL to request 
	a stall (checked builds may otherwise complain).

	This declaration must exactly match the declaration of 
	USBRC_GETDESCRIPTOR_FN. It's provided because
	it's often inconvenient to include all the usbrc header files.

*/

__TMS_TYPE_DEF_FUNCTION(				\
USBPUMP_FILTER_GETDESCRIPTOR_FN,			\
int,							\
	(						\
	__TMS_UDEVICE *		/* pDevice */,		\
	unsigned char		/* DescType */,		\
	unsigned char		/* DescIndex */,	\
	unsigned short		/* DescLangId */,	\
	unsigned short		/* CallIndex */,	\
	const unsigned char *	/* pRom */,		\
	unsigned short		/* RomSize */,		\
	unsigned char *		/* pRam */,		\
	unsigned short		/* RamSize */,		\
	const unsigned char **	/* ppResult */,		\
	unsigned short *	/* pResultLen */	\
	));


/****************************************************************************\
|
|	The numeric constants for filtering
|
\****************************************************************************/

#define	__TMS_USBPUMP_GETDESCRFILTER_PASS	0	/* pass the rom version on. */
#define	__TMS_USBPUMP_GETDESCRFILTER_SEND	1	/* send from buffer */
#define	__TMS_USBPUMP_GETDESCRFILTER_FRAG	2	/* send a fragment */
#define	__TMS_USBPUMP_GETDESCRFILTER_FAIL	3	/* fail -- send STALL */

#define __TMS_USBPUMP_SETDESCRFILTER_GETNEXT	0	/* get more data */
#define __TMS_USBPUMP_SETDESCRFILTER_DONE	1	/* get all the data needed */
#define __TMS_USBPUMP_SETDESCRFILTER_ERROR	2	/* error */


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_GETDESCRFILTER_PASS	\
   __TMS_USBPUMP_GETDESCRFILTER_PASS
# define USBPUMP_GETDESCRFILTER_SEND	\
   __TMS_USBPUMP_GETDESCRFILTER_SEND
# define USBPUMP_GETDESCRFILTER_FRAG	\
   __TMS_USBPUMP_GETDESCRFILTER_FRAG
# define USBPUMP_GETDESCRFILTER_FAIL	\
   __TMS_USBPUMP_GETDESCRFILTER_FAIL
# define USBPUMP_SETDESCRFILTER_GETNEXT	\
   __TMS_USBPUMP_SETDESCRFILTER_GETNEXT
# define USBPUMP_SETDESCRFILTER_DONE	\
   __TMS_USBPUMP_SETDESCRFILTER_DONE
# define USBPUMP_SETDESCRFILTER_ERROR	\
   __TMS_USBPUMP_SETDESCRFILTER_ERROR
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_filter_types.h ****/
#endif /* _USBPUMP_FILTER_TYPES_H_ */
