/* usbpumpioctl.h	Mon May 24 2010 12:36:11 chwon */

/*

Module:  usbpumpioctl.h

Function:
	Common definitions for USB DataPump IOCTL operations.

Version:
	V3.01c	Mon May 24 2010 12:36:11 chwon	Edit level 14

Copyright notice:
	This file copyright (C) 2001-2005, 2007, 2009-2010 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2001

Revision history:
   1.63b  Sun Jul 15 2001 09:46:30  tmm
	Module created.

   1.63d  11/8/2001  tmm
	Improve commentary.

   1.63i  5/18/2002  tmm
	Further improve commentary.

   1.87a  Mon Oct 27 2003 01:11:18  tmm
	Commentary changes only.

   1.91c  Tue May 03 2005 12:43:14  chwon
	Fix GCC 4.0 compiler warning.

   1.91d  2005-06-05  tmm
	Add asynchronous IOCTL definitions.

   1.91d  2005-06-28  tmm
	Correct error in the USBPUMP_DECLARE_IOCTL_FNTYPE (uncloaked) 
	definition -- I guess we aren't using it uncloaked anywhere at
	present.

   1.91h  Wed Aug 24 2005 14:13:41  chwon
	Add USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS code.

   1.93b  Wed Nov 02 2005 15:24:06  chwon
	Add USBPUMP_IOCTL_RESULT_NOT_TRANSPARENT_MODE code.

   1.97j  Tue Nov 27 2007 08:23:44  tmm
	3528:  clean compile for Microsoft Visual C V6.0 with /W4.
	Add USBPUMP_IOCTL_RESULT_NOT_IMPLEMENTED and recloak.

   2.01a  Thu Apr 02 2009 08:55:25 maw
   	7473: Add USBPUMP_IOCTL_RESULT_... and recloak

   3.01c  Mon Mar 29 2010 10:09:01  chwon
	10082: Add USBPUMP_IOCTL_RESULT_ALREADY_REGISTER code.

   3.01c  Mon May 24 2010 12:36:11  chwon
	10385: add USBPUMP_IOCTL_RESULT_HARDWARE_NOT_SUPPORT code.

*/

#ifndef _USBPUMPIOCTL_H_		/* prevent multiple includes */
#define _USBPUMPIOCTL_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/*

Type:  USBPUMP_IOCTL_CODE

Function:
	Abstract C scalar type, carries an IOCTL code.

Description:
	IOCTL codes are defined similarly to NetBSD.  We try to avoid
	colliding with existing BSD codes when we assign codes; the 
	codes we use have the high-bit set in the low order 16-bits.

	Therefore, our IOCTL codes (unique to the DataPump) all have the 
	following layout:

	 3 3 2 2          1 1 1
	 1 0 9 8          6 5 4         0
	+-+-+-+------------+-+-----------+
	| | | |            | |           |
	+-+-+-+------------+-+-----------+
	 | | |       |      |      |
	 | | |       |      |      |
	 | | |       |      |      +-->	Function code. Conventionally
	 | | |       |      |		the upper 7 bits are a group
	 | | |       |      |		code, and the lower 8 bits are
	 | | |       |      |		a function code within the group.
	 | | |       |      |
	 | | |       |      +---------> Set to 1 for synchronous IOCTLs
	 | | |	     |			Set to 0 for asynchronous IOCTLs
	 | | |       |
	 | | |       +---------------->	Length of parameters (in bytes).
	 | | |  
	 +-+-+------------------------> Parameter type:
					001 ->  no parameters (IN and
						OUT parameters are not
						pointers, and should be
						coded as NULL).
					010 ->	OUT parameter points to a
						buffer of given size for
						results from this function (IN
						paramter is not a pointer, 
						and should be coded as NULL).
					100 ->	IN parameter points to a
						buffer of given size, 
						containing parameters for 
						this IOCTL operation.
						(OUT parameter is not a
						pointer, and should be coded
						as NULL)
					110 ->	IN paramter points to input
						paramters, OUT pointer 
						points to a buffer that gets
						the results.  Both buffers
						must be of size indicated
						by the 'length' field.

	Frequently (in the DataPump) the codes are unexamined by the IOCTL
	dispatch functions; parameter blocks are fixed size, etc.  However,
	this design allows for the usual transparent filtering, and also
	allows for the use of variable length parameter blocks where needed.

See also:
	USBPUMP_IOCTL_RESULT
	USBPUMP_IOC(), USBPUMP_IOCTL_R(), USBPUMP_IOCTL_W(), 
	USBPUMP_IOCTL_RW()
	USBPUMP_IOC_ASYNC(), USBPUMP_IOCTL_R_ASYNC(), USBPUMP_IOCTL_W_ASYNC(), 
	USBPUMP_IOCTL_RW_ASYNC()

*/

__TMS_TYPE_DEF_ARG(USBPUMP_IOCTL_CODE, UINT32);

/*

Type:	USBPUMP_IOCTL_RESULT

Function:
	Abstract type for the functional result of any IOCTL operation.

Description:
	All IOCTL operations return a result of this kind.  There are 
	three categories of results:

	1) |USBPUMP_IOCTL_RESULT_NOT_CLAIMED| -- this means that the IOCTL
	   was not recognized by this IOCTL handler.  A single numeric code
	   is defined for this case.  This can be recognized by the macro
	   |USBPUMP_IOCTL_NOT_CLAIMED(code)|.

	2) |USBPUMP_IOCTL_RESULT_SUCCESS| -- this means that the IOCTL was
	   recognized, and it completed successfully.  A single numeric code
	   is defined for this case.  This can be recognized by the macro
	   |USBPUMP_IOCTL_SUCCESS(code)|.

	3) |USBPUMP_IOCTL_RESULT_FAILED| -- this means that the IOCTL was
	   recognized, but that an error occurred in processing.  A whole
	   family of error codes are defined to allow the type of failure
	   to be communicated to the caller.  These can be identified by the
	   boolean macro:  |USBPUMP_IOCTL_FAILED(code)|, which returns
	   TRUE for a failure, FALSE for either success or not-claimed.

	   Specific failure codes are constructed using the macro
	   |USBUPUMP_IOCTL_RESULT_FAILURE(int)|, which takes a positive 
	   integer, the index code of the failure.

*/

__TMS_TYPE_DEF_ARG(USBPUMP_IOCTL_RESULT, INT32);

#define	__TMS_USBPUMP_IOCTL_RESULT_NOT_CLAIMED	((__TMS_USBPUMP_IOCTL_RESULT) -1)
#define	__TMS_USBPUMP_IOCTL_RESULT_SUCCESS	((__TMS_USBPUMP_IOCTL_RESULT) 0)
#define	__TMS_USBPUMP_IOCTL_RESULT_FAILED	((__TMS_USBPUMP_IOCTL_RESULT) 1)

/* define a failure code */
#define	__TMS_USBPUMP_IOCTL_RESULT_FAILURE(x)	((__TMS_USBPUMP_IOCTL_RESULT) (x))

/*
|| Macros for checking status
*/
#define __TMS_USBPUMP_IOCTL_SUCCESS(result) \
	((result) == 0)

#define	__TMS_USBPUMP_IOCTL_NOT_CLAIMED(result) \
	((result) < 0)

#define	__TMS_USBPUMP_IOCTL_FAILED(result) \
	((result) > 0)

/*

Name:	USBPUMP_IOCTL_DECLARE_IOCTL_FNTYPE()

Function:
	C preprocessor macro, defines a specific function type for IOCTL 
	dispatching.

Definition:
	USBPUMP_IOCTL_DECLARE_IOCTL_FNTYPE(
		FunctionTypeName,
		ContextTypeExpression
		)

Description:
	This macro declares |FunctionTypeName| as a standard MCCI IOCTL
	dispatch function, with a context parameter of type 
	|ContextTypeExpression|.  The function type is defined according
	to the normal rules (i.e., with a 'P' prefix for a pointer-to-
	function type, and in cloaked form; and also in disclosed form
	depending on the effective state of __TMS_CLOAKED_NAMES_ONLY when
	the <def/typemacs.h> macro was first read at compile time.

Returns:
	Not applicable -- is a type-definition macro.

Notes:
	Any use of this macro should normally be followed by a ';'.

	We use this routine because frequently the IOCTL dispatch helper
	functions actually have context pointer types that are known at
	compile time (despite the formidable abstractness of this API).
	Using this mechanism allows for type-safey in those situations.

*/

#define __TMS_USBPUMP_DECLARE_IOCTL_FNTYPE(fn_type, context_type)	\
__TMS_FNTYPE_DEF(							\
	fn_type,							\
	__TMS_USBPUMP_IOCTL_RESULT,					\
		(							\
		context_type			/* the context ptr */,	\
		__TMS_USBPUMP_IOCTL_CODE	/* IoctlRequestCode */,	\
		__TMS_CONST __TMS_VOID *	/* pInParam */,		\
		__TMS_VOID *			/* pOutParam */		\
		)							\
	)

/*

Name:	USBPUMP_IOC()

Index:	Name:	USBPUMP_IOCTL_VOID()
	Name:	USBPUMP_IOCTL_R()
	Name:	USBPUMP_IOCTL_W()
	Name:	USBPUMP_IOCTL_RW()
	Name:	USBPUMP_IOCTL_IOC_EX()
	Name:	USBPUMP_IOCTL_VOID_ASYNC()
	Name:	USBPUMP_IOCTL_R_ASYNC()
	Name:	USBPUMP_IOCTL_W_ASYNC()
	Name:	USBPUMP_IOCTL_RW_ASYNC()
	Name:	USBPUMP_IOCPARM_LEN()

Function:
	Compute IOCTL codes in a standard way.

Definition:
	USBPUMP_IOCTL_CODE USBPUMP_IOC(
				UINT32 DirMask,
				UINT8  RawGroup,
				UINT8  Num,
				UINT   ArgSize
				)

	USBPUMP_IOCTL_CODE USBPUMP_IOCTL_VOID(
				UINT8 Group,
				UINT8 Num
				);
	USBPUMP_IOCTL_CODE USBPUMP_IOCTL_R(
				UINT8 Group,
				UINT8 Num,
				ArgumentTypeExpression
				);
	USBPUMP_IOCTL_CODE USBPUMP_IOCTL_W(
				UINT8 Group,
				UINT8 Num,
				ArgumentTypeExpression
				);
	USBPUMP_IOCTL_CODE USBPUMP_IOCTL_RW(
				UINT8 Group,
				UINT8 Num,
				ArgumentTypeExpression
				);

	USBPUMP_IOCTL_CODE USBPUMP_IOCTL_VOID_ASYNC(
				UINT8 Group,
				UINT8 Num
				);
	USBPUMP_IOCTL_CODE USBPUMP_IOCTL_R_ASYNC(
				UINT8 Group,
				UINT8 Num,
				ArgumentTypeExpression
				);
	USBPUMP_IOCTL_CODE USBPUMP_IOCTL_W_ASYNC(
				UINT8 Group,
				UINT8 Num,
				ArgumentTypeExpression
				);
	USBPUMP_IOCTL_CODE USBPUMP_IOCTL_RW_ASYNC(
				UINT8 Group,
				UINT8 Num,
				ArgumentTypeExpression
				);

	BYTES USBPUMP_IOCPARM_LEN(
			USBPUMP_IOCTL_CODE IoctlCode
			);

	BOOL USBPUMP_IOCTL_IS_ASYNC(
			USBPUMP_IOCTL_CODE IoctlCode
			);

Description:
	|USBPUMP_IOCTL_*| macros compute the appropriate IOCTL code based
	on |Group|, |Num| and |ArgumentTypeExpression|, setting the
	appropriate direction code.  For synchronous IOCTL codes,
	|Group| is always or-ed with 0x80; for asynchronous IOCTL codes,
	|Group| is always and-ed with 0x7F.

	|USBPUMP_IOC()| computes a fully general IOCTL code, based on
	|DirMask|, |RawGroup|, |RawNum|, and |ArgumentSize|.  Note that
	the |RawGroup| is completely unmodified, and that a numeric
	size is used rather than a type.

	|USBPUMP_IOCPARM_LEN()| extracts the parameter buffer length from
	an IoctlCode.  This macro is used when processing IOCTLs that
	pass variable-length buffers.

*/

#define	__TMS_USBPUMP_IOCPARM_MASK	0x1fff		/* parameter length, at most 13 bits */
#define	__TMS_USBPUMP_IOCPARM_SHIFT	16		/* how far to shift --
							|| defined for
							|| convenience of
							|| external users.
							|| If you change
							|| this, nothing good
							|| will result....
							*/
#define	__TMS_USBPUMP_IOCPARM_LEN(x)	(((x) >> 16) & __TMS_USBPUMP_IOCPARM_MASK)
#define	__TMS_USBPUMP_IOCBASECMD(x)	((x) & ~(__TMS_USBPUMP_IOCPARM_MASK << 16))
#define	__TMS_USBPUMP_IOCGROUP(x)	(((x) >> 8) & 0xff)

/* max size of ioctl args */
#define	__TMS_USBPUMP_IOCPARM_MAX	((__TMS_USBPUMP_IOCPARM_MASK + 1) >> 1)

/* no parameters */
#define	__TMS_USBPUMP_IOC_VOID		((__TMS_USBPUMP_IOCTL_CODE)0x20000000)

/* copy parameters out */
#define	__TMS_USBPUMP_IOC_OUT		((__TMS_USBPUMP_IOCTL_CODE)0x40000000)

/* copy parameters in */
#define	__TMS_USBPUMP_IOC_IN		((__TMS_USBPUMP_IOCTL_CODE)0x80000000)

/* copy paramters in and out */
#define	__TMS_USBPUMP_IOC_INOUT		(__TMS_USBPUMP_IOC_IN|__TMS_USBPUMP_IOC_OUT)

/* mask for IN/OUT/VOID */
#define	__TMS_USBPUMP_IOC_DIRMASK	((__TMS_USBPUMP_IOCTL_CODE)0xe0000000)

/* the asynch mask */
#define	__TMS_USBPUMP_IOCTL_SYNC_MASK	((__TMS_USBPUMP_IOCTL_CODE)0x00008000)
#define	__TMS_USBPUMP_IOCTL_IS_ASYNC(x)	(((x) & __TMS_USBPUMP_IOCTL_SYNC_MASK) == 0)

/* the macro to greate a general code */
#define	__TMS_USBPUMP_IOC(inout,group,num,len) \
	((__TMS_USBPUMP_IOCTL_CODE)(((inout) & __TMS_USBPUMP_IOC_DIRMASK) |	\
		(((len) & __TMS_USBPUMP_IOCPARM_MASK) << 16) |		\
		((group) << 8) | (num)))

#define	__TMS_USBPUMP_IOCTL_VOID(g,n)	\
		__TMS_USBPUMP_IOC(__TMS_USBPUMP_IOC_VOID, (g)|0x80, (n), 0)
#define	__TMS_USBPUMP_IOCTL_R(g,n,t)	\
		__TMS_USBPUMP_IOC(__TMS_USBPUMP_IOC_OUT,  (g)|0x80, (n), sizeof(t))
#define	__TMS_USBPUMP_IOCTL_W(g,n,t)	\
		__TMS_USBPUMP_IOC(__TMS_USBPUMP_IOC_IN,	  (g)|0x80, (n), sizeof(t))
#define	__TMS_USBPUMP_IOCTL_RW(g,n,t)	\
		__TMS_USBPUMP_IOC(__TMS_USBPUMP_IOC_INOUT,(g)|0x80, (n), sizeof(t))

#define	__TMS_USBPUMP_IOCTL_VOID_ASYNC(g,n)	\
		__TMS_USBPUMP_IOC(__TMS_USBPUMP_IOC_VOID, (g)&0x7F, (n), 0)
#define	__TMS_USBPUMP_IOCTL_R_ASYNC(g,n,t)	\
		__TMS_USBPUMP_IOC(__TMS_USBPUMP_IOC_OUT,  (g)&0x7F, (n), sizeof(t))
#define	__TMS_USBPUMP_IOCTL_W_ASYNC(g,n,t)	\
		__TMS_USBPUMP_IOC(__TMS_USBPUMP_IOC_IN,	  (g)&0x7F, (n), sizeof(t))
#define	__TMS_USBPUMP_IOCTL_RW_ASYNC(g,n,t)	\
		__TMS_USBPUMP_IOC(__TMS_USBPUMP_IOC_INOUT,(g)&0x7F, (n), sizeof(t))

/*

Name:	USBPUMP_IOCMATCH()

Function:
	Compare IOCTL codes for IOCTLs with variable-size parameters.

Definition:
	BOOL USBPUMP_IOCMATCH(
		USBPUMP_IOCTL_CODE InputCode,
		USBPUMP_IOCTL_CODE ReferenceCode
		);

Description:
	Compare an input IOCTL code against a reference code, and 
	return TRUE if |InputCode| is derived from |ReferenceCode|,
	and if the parameter length field in |InputCode| is at least
	as large as the minimum value specified in |ReferenceCode|.

Returns:
	|TRUE| if |InputCode| code matches, |FALSE| otherwise.

*/


#define	__TMS_USBPUMP_IOCMATCH(in, ref)							\
	(((((__TMS_USBPUMP_IOCTL_CODE)(in) ^ (__TMS_USBPUMP_IOCTL_CODE)(ref)) &		\
		~(__TMS_USBPUMP_IOCPARM_MASK << __TMS_USBPUMP_IOCPARM_SHIFT)) == 0) &&	\
	 (__TMS_USBPUMP_IOCPARM_LEN(in) >= __TMS_USBPUMP_IOCPARM_LEN(ref)))

/*

Name:	USBPUMP_IOCADJUST()

Function:
	Adjust an IOCTL code to reflect actual number of bytes passed.

Definition:
	USBPUMP_IOCTL_CODE USBPUMP_IOCADJUST(
		USBPUMP_IOCTL_CODE Code,
		UINT	ActualSize
		);

Description:
	The Length field of the IOCTL code |Code| is replace by
	|ActualSize|.

	For most IOCTLs, this macro is not used, because most IOCTLs
	have fixed-size parameter structures.  We provide this
	for completeness.

Returns:
	The adjusted IOCTL code.

Notes:
	This is a macro, and parameters may be evaluated multiple times.

*/

#define __TMS_USBPUMP_IOCADJUST(Code, ActualSize)			\
	((__TMS_USBPUMP_IOCTL_CODE)					\
	 ((__TMS_USBPUMP_IOCTL_CODE)(Code) & ~(__TMS_USBPUMP_IOCPARM_MASK << 16) | \
	  (((__TMS_USBPUMP_IOCTL_CODE)(ActualSize) &  __TMS_USBPUMP_IOCPARM_MASK) << 16)))

/****************************************************************************\
|
|	Some common failure codes.
|
\****************************************************************************/

/* 1 is used for __TMS_USBPUMP_IOCTL_RESULT_FAILED */

/* 2:  output buffer too small */
#define	__TMS_USBPUMP_IOCTL_RESULT_BUFFER_TOO_SMALL	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(2)
/* 3:  some implementation limit was exceeded */
#define __TMS_USBPUMP_IOCTL_RESULT_IMPLEMENTATION_LIMITATION \
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(3)
/* 4:  IN ptr omitted */
#define __TMS_USBPUMP_IOCTL_RESULT_IN_PARAM_NULL \
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(4)
/* 5:  OUT ptr omitted */
#define __TMS_USBPUMP_IOCTL_RESULT_OUT_PARAM_NULL \
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(5)
/* 6:  IOCTL code direction is not valid */
#define __TMS_USBPUMP_IOCTL_RESULT_DIRECTION_BAD \
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(6)
/* 7:  last result in a sequence has been returned */
#define __TMS_USBPUMP_IOCTL_RESULT_NO_MORE \
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(7)
/* 8:  an IN parameter value was invalid */
#define __TMS_USBPUMP_IOCTL_RESULT_INVALID_PARAMETER \
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(8)
/* 9:  function was already open */
#define	__TMS_USBPUMP_IOCTL_RESULT_ALREADY_OPEN	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(9)
/* 10: function was not opened */
#define	__TMS_USBPUMP_IOCTL_RESULT_NOT_OPEN	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(10)
/* 11: passed an asynch IOCTL to a synchronous path */
#define	__TMS_USBPUMP_IOCTL_RESULT_NOT_SYNCHRONOUS	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(11)
/* 12: cancelled an asynch IOCTL */
#define	__TMS_USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(12)
/* 13: mass storage protocol is not in transparent mode */
#define	__TMS_USBPUMP_IOCTL_RESULT_NOT_TRANSPARENT_MODE	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(13)
/* 14: request not implemented */
#define	__TMS_USBPUMP_IOCTL_RESULT_NOT_IMPLEMENTED	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(14)
/* 15: hardware in use */
#define	__TMS_USBPUMP_IOCTL_RESULT_HARDWARE_INUSE	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(15)
/* 16: hardware not in use */
#define	__TMS_USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(16)
/* 17: no hardware */
#define	__TMS_USBPUMP_IOCTL_RESULT_NO_HARDWARE	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(17)
/* 18: hardware IO-error */
#define	__TMS_USBPUMP_IOCTL_RESULT_HARDWARE_IOERR	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(18)
/* 19: already started */
#define	__TMS_USBPUMP_IOCTL_RESULT_ALREADY_STARTED	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(19)
/* 20: already stopped */
#define	__TMS_USBPUMP_IOCTL_RESULT_ALREADY_STOPPED	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(20)
/* 21: already register */
#define	__TMS_USBPUMP_IOCTL_RESULT_ALREADY_REGISTER	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(21)
/* 22: hardware not support */
#define	__TMS_USBPUMP_IOCTL_RESULT_HARDWARE_NOT_SUPPORT	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(22)
/* 23: already suspended */
#define	__TMS_USBPUMP_IOCTL_RESULT_ALREADY_SUSPENDED	\
		__TMS_USBPUMP_IOCTL_RESULT_FAILURE(23)

/****************************************************************************\
|
|	Common IOCTL checking macros
|
\****************************************************************************/

#define	__TMS_USBPUMP_IOCTL_CHECK_R(pInParam, pOutParam)		\
	((pOutParam) == __TMS_NULL ? __TMS_USBPUMP_IOCTL_RESULT_OUT_PARAM_NULL \
			     : __TMS_USBPUMP_IOCTL_RESULT_SUCCESS)

#define	__TMS_USBPUMP_IOCTL_CHECK_W(pInParam, pOutParam)		\
	((pInParam) == __TMS_NULL ? __TMS_USBPUMP_IOCTL_RESULT_IN_PARAM_NULL \
			    : __TMS_USBPUMP_IOCTL_RESULT_SUCCESS)

#define	__TMS_USBPUMP_IOCTL_CHECK_RW(Ioctl, pInParam, pOutParam, Memcpy) \
	((pInParam) == __TMS_NULL  ? __TMS_USBPUMP_IOCTL_RESULT_IN_PARAM_NULL : \
	 (pOutParam) == __TMS_NULL ? __TMS_USBPUMP_IOCTL_RESULT_OUT_PARAM_NULL : \
	 ((pOutParam) != (pInParam) && (Memcpy) != __TMS_NULL)		\
			     ? ((Memcpy)(				\
				   (pOutParam),				\
				   (pInParam),				\
				   __TMS_USBPUMP_IOCPARM_LEN(Ioctl)),	\
				 __TMS_USBPUMP_IOCTL_RESULT_SUCCESS)	\
			     : __TMS_USBPUMP_IOCTL_RESULT_SUCCESS)

/****************************************************************************\
|
|	uncloaked names...
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_RESULT_NOT_CLAIMED	\
   __TMS_USBPUMP_IOCTL_RESULT_NOT_CLAIMED
# define USBPUMP_IOCTL_RESULT_SUCCESS	\
   __TMS_USBPUMP_IOCTL_RESULT_SUCCESS
# define USBPUMP_IOCTL_RESULT_FAILED	\
   __TMS_USBPUMP_IOCTL_RESULT_FAILED
# define USBPUMP_IOCTL_RESULT_FAILURE(x)	\
   __TMS_USBPUMP_IOCTL_RESULT_FAILURE(x)
# define USBPUMP_IOCTL_SUCCESS(result)	\
   __TMS_USBPUMP_IOCTL_SUCCESS(result)
# define USBPUMP_IOCTL_NOT_CLAIMED(result)	\
   __TMS_USBPUMP_IOCTL_NOT_CLAIMED(result)
# define USBPUMP_IOCTL_FAILED(result)	\
   __TMS_USBPUMP_IOCTL_FAILED(result)
# define USBPUMP_DECLARE_IOCTL_FNTYPE(fn_type, context_type)	\
   __TMS_USBPUMP_DECLARE_IOCTL_FNTYPE(fn_type, context_type)
# define USBPUMP_IOCPARM_MASK	\
   __TMS_USBPUMP_IOCPARM_MASK
# define USBPUMP_IOCPARM_SHIFT	\
   __TMS_USBPUMP_IOCPARM_SHIFT
# define USBPUMP_IOCPARM_LEN(x)	\
   __TMS_USBPUMP_IOCPARM_LEN(x)
# define USBPUMP_IOCBASECMD(x)	\
   __TMS_USBPUMP_IOCBASECMD(x)
# define USBPUMP_IOCGROUP(x)	\
   __TMS_USBPUMP_IOCGROUP(x)
# define USBPUMP_IOCPARM_MAX	\
   __TMS_USBPUMP_IOCPARM_MAX
# define USBPUMP_IOC_VOID	\
   __TMS_USBPUMP_IOC_VOID
# define USBPUMP_IOC_OUT	\
   __TMS_USBPUMP_IOC_OUT
# define USBPUMP_IOC_IN	\
   __TMS_USBPUMP_IOC_IN
# define USBPUMP_IOC_INOUT	\
   __TMS_USBPUMP_IOC_INOUT
# define USBPUMP_IOC_DIRMASK	\
   __TMS_USBPUMP_IOC_DIRMASK
# define USBPUMP_IOCTL_SYNC_MASK	\
   __TMS_USBPUMP_IOCTL_SYNC_MASK
# define USBPUMP_IOCTL_IS_ASYNC(x)	\
   __TMS_USBPUMP_IOCTL_IS_ASYNC(x)
# define USBPUMP_IOC(inout,group,num,len)	\
   __TMS_USBPUMP_IOC(inout,group,num,len)
# define USBPUMP_IOCTL_VOID(g,n)	\
   __TMS_USBPUMP_IOCTL_VOID(g,n)
# define USBPUMP_IOCTL_R(g,n,t)	\
   __TMS_USBPUMP_IOCTL_R(g,n,t)
# define USBPUMP_IOCTL_W(g,n,t)	\
   __TMS_USBPUMP_IOCTL_W(g,n,t)
# define USBPUMP_IOCTL_RW(g,n,t)	\
   __TMS_USBPUMP_IOCTL_RW(g,n,t)
# define USBPUMP_IOCTL_VOID_ASYNC(g,n)	\
   __TMS_USBPUMP_IOCTL_VOID_ASYNC(g,n)
# define USBPUMP_IOCTL_R_ASYNC(g,n,t)	\
   __TMS_USBPUMP_IOCTL_R_ASYNC(g,n,t)
# define USBPUMP_IOCTL_W_ASYNC(g,n,t)	\
   __TMS_USBPUMP_IOCTL_W_ASYNC(g,n,t)
# define USBPUMP_IOCTL_RW_ASYNC(g,n,t)	\
   __TMS_USBPUMP_IOCTL_RW_ASYNC(g,n,t)
# define USBPUMP_IOCMATCH(in, ref)	\
   __TMS_USBPUMP_IOCMATCH(in, ref)
# define USBPUMP_IOCADJUST(Code, ActualSize)	\
   __TMS_USBPUMP_IOCADJUST(Code, ActualSize)
# define USBPUMP_IOCTL_RESULT_BUFFER_TOO_SMALL	\
   __TMS_USBPUMP_IOCTL_RESULT_BUFFER_TOO_SMALL
# define USBPUMP_IOCTL_RESULT_IMPLEMENTATION_LIMITATION	\
   __TMS_USBPUMP_IOCTL_RESULT_IMPLEMENTATION_LIMITATION
# define USBPUMP_IOCTL_RESULT_IN_PARAM_NULL	\
   __TMS_USBPUMP_IOCTL_RESULT_IN_PARAM_NULL
# define USBPUMP_IOCTL_RESULT_OUT_PARAM_NULL	\
   __TMS_USBPUMP_IOCTL_RESULT_OUT_PARAM_NULL
# define USBPUMP_IOCTL_RESULT_DIRECTION_BAD	\
   __TMS_USBPUMP_IOCTL_RESULT_DIRECTION_BAD
# define USBPUMP_IOCTL_RESULT_NO_MORE	\
   __TMS_USBPUMP_IOCTL_RESULT_NO_MORE
# define USBPUMP_IOCTL_RESULT_INVALID_PARAMETER	\
   __TMS_USBPUMP_IOCTL_RESULT_INVALID_PARAMETER
# define USBPUMP_IOCTL_RESULT_ALREADY_OPEN	\
   __TMS_USBPUMP_IOCTL_RESULT_ALREADY_OPEN
# define USBPUMP_IOCTL_RESULT_NOT_OPEN	\
   __TMS_USBPUMP_IOCTL_RESULT_NOT_OPEN
# define USBPUMP_IOCTL_RESULT_NOT_SYNCHRONOUS	\
   __TMS_USBPUMP_IOCTL_RESULT_NOT_SYNCHRONOUS
# define USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS	\
   __TMS_USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS
# define USBPUMP_IOCTL_RESULT_NOT_TRANSPARENT_MODE	\
   __TMS_USBPUMP_IOCTL_RESULT_NOT_TRANSPARENT_MODE
# define USBPUMP_IOCTL_RESULT_NOT_IMPLEMENTED	\
   __TMS_USBPUMP_IOCTL_RESULT_NOT_IMPLEMENTED
# define USBPUMP_IOCTL_RESULT_HARDWARE_INUSE	\
   __TMS_USBPUMP_IOCTL_RESULT_HARDWARE_INUSE
# define USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE	\
   __TMS_USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE
# define USBPUMP_IOCTL_RESULT_NO_HARDWARE	\
   __TMS_USBPUMP_IOCTL_RESULT_NO_HARDWARE
# define USBPUMP_IOCTL_RESULT_HARDWARE_IOERR	\
   __TMS_USBPUMP_IOCTL_RESULT_HARDWARE_IOERR
# define USBPUMP_IOCTL_RESULT_ALREADY_STARTED	\
   __TMS_USBPUMP_IOCTL_RESULT_ALREADY_STARTED
# define USBPUMP_IOCTL_RESULT_ALREADY_STOPPED	\
   __TMS_USBPUMP_IOCTL_RESULT_ALREADY_STOPPED
# define USBPUMP_IOCTL_RESULT_ALREADY_REGISTER	\
   __TMS_USBPUMP_IOCTL_RESULT_ALREADY_REGISTER
# define USBPUMP_IOCTL_RESULT_HARDWARE_NOT_SUPPORT	\
   __TMS_USBPUMP_IOCTL_RESULT_HARDWARE_NOT_SUPPORT
# define USBPUMP_IOCTL_RESULT_ALREADY_SUSPENDED	\
   __TMS_USBPUMP_IOCTL_RESULT_ALREADY_SUSPENDED
# define USBPUMP_IOCTL_CHECK_R(pInParam, pOutParam)	\
   __TMS_USBPUMP_IOCTL_CHECK_R(pInParam, pOutParam)
# define USBPUMP_IOCTL_CHECK_W(pInParam, pOutParam)	\
   __TMS_USBPUMP_IOCTL_CHECK_W(pInParam, pOutParam)
# define USBPUMP_IOCTL_CHECK_RW(Ioctl, pInParam, pOutParam, Memcpy)	\
   __TMS_USBPUMP_IOCTL_CHECK_RW(Ioctl, pInParam, pOutParam, Memcpy)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpumpioctl.h ****/
#endif /* _USBPUMPIOCTL_H_ */
