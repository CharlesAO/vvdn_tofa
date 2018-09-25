/* utaskmsg_codes.h	Fri May 27 2005 13:47:45 maw */

/*

Module:  utaskmsg_codes.h

Function:
	Define the common message codes

Version:
	V1.91d	Fri May 27 2005 13:47:45 maw	Edit level 2

Copyright notice:
	This file copyright (C) 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Mats Webjorn, MCCI Corporation	March 2005

Revision history:
   1.91b  Tue Mar 22 2005 08:11:11 maw
	Module created.

   1.91d  Fri May 27 2005 13:47:45 maw
	Def of UTASK_MSGCODE_TYPE moved to usbtask_types.h

*/

#ifndef _UTASKMSG_CODES_H_		/* prevent multiple includes */
#define _UTASKMSG_CODES_H_

#ifndef _UTASK_TYPES_H_
# include "utask_types.h"
#endif


/****************************************************************************\
|
|	Define macros to do bit-manipulation of message-code fields
|
\****************************************************************************/

/*
|| Message mode flag
*/
#define __TMS_UTASK_MSGCODE_MODE_SYNC		(0x0)
#define __TMS_UTASK_MSGCODE_MODE_ASYNC		(0x1)

/*
|| Message code fields
*/
#define __TMS_UTASK_MSGCODE_SERVICE_WIDTH	7
#define __TMS_UTASK_MSGCODE_SERVICE_SHIFT	0
#define __TMS_UTASK_MSGCODE_SERVICE_MASK	(((1 << __TMS_UTASK_MSGCODE_SERVICE_WIDTH)-1) \
						     << __TMS_UTASK_MSGCODE_SERVICE_SHIFT)
#define __TMS_UTASK_MSGCODE_SERVICE(c)		(((c) &  __TMS_UTASK_MSGCODE_SERVICE_MASK) \
						     >> (__TMS_UTASK_MSGCODE_SERVICE_SHIFT))

#define __TMS_UTASK_MSGCODE_INDEX_WIDTH		7
#define __TMS_UTASK_MSGCODE_INDEX_SHIFT		(__TMS_UTASK_MSGCODE_SERVICE_WIDTH + \
                                                 __TMS_UTASK_MSGCODE_SERVICE_SHIFT)
#define __TMS_UTASK_MSGCODE_INDEX_MASK		(((1 << __TMS_UTASK_MSGCODE_INDEX_WIDTH)-1) \
						     << __TMS_UTASK_MSGCODE_INDEX_SHIFT)
#define __TMS_UTASK_MSGCODE_INDEX(c)		(((c) &  __TMS_UTASK_MSGCODE_INDEX_MASK) \
						     >> (__TMS_UTASK_MSGCODE_INDEX_SHIFT))

#define __TMS_UTASK_MSGCODE_SIZE_WIDTH		10
#define __TMS_UTASK_MSGCODE_SIZE_SHIFT		(__TMS_UTASK_MSGCODE_INDEX_WIDTH + \
                                                 __TMS_UTASK_MSGCODE_INDEX_SHIFT)
#define __TMS_UTASK_MSGCODE_SIZE_MASK		(((1 << __TMS_UTASK_MSGCODE_SIZE_WIDTH)-1) \
						     << __TMS_UTASK_MSGCODE_SIZE_SHIFT)
#define __TMS_UTASK_MSGCODE_SIZE(c)		(((c) &  __TMS_UTASK_MSGCODE_SIZE_MASK) \
						     >> (__TMS_UTASK_MSGCODE_SIZE_SHIFT))

#define __TMS_UTASK_MSGCODE_NBUF_WIDTH		3
#define __TMS_UTASK_MSGCODE_NBUF_SHIFT		(__TMS_UTASK_MSGCODE_SIZE_WIDTH + \
                                                 __TMS_UTASK_MSGCODE_SIZE_SHIFT)
#define __TMS_UTASK_MSGCODE_NBUF_MASK		(((1 << __TMS_UTASK_MSGCODE_NBUF_WIDTH)-1) \
						     << __TMS_UTASK_MSGCODE_NBUF_SHIFT)
#define __TMS_UTASK_MSGCODE_NBUF(c)		(((c) &  __TMS_UTASK_MSGCODE_NBUF_MASK) \
						     >> (__TMS_UTASK_MSGCODE_NBUF_SHIFT))

#define __TMS_UTASK_MSGCODE_TYPE_WIDTH		2
#define __TMS_UTASK_MSGCODE_TYPE_SHIFT		(__TMS_UTASK_MSGCODE_NBUF_WIDTH + \
                                                 __TMS_UTASK_MSGCODE_NBUF_SHIFT)
#define __TMS_UTASK_MSGCODE_TYPE_MASK		(((1 << __TMS_UTASK_MSGCODE_TYPE_WIDTH)-1) \
						     << __TMS_UTASK_MSGCODE_TYPE_SHIFT)
#define __TMS_UTASK_MSGCODE_TYPE(c)		(((c) & (__TMS_UTASK_MSGCODE_TYPE_MASK)) \
						     >> (__TMS_UTASK_MSGCODE_TYPE_SHIFT))

#define __TMS_UTASK_MSGCODE_MODE_WIDTH		1
#define __TMS_UTASK_MSGCODE_MODE_SHIFT		(__TMS_UTASK_MSGCODE_TYPE_WIDTH + \
                                                 __TMS_UTASK_MSGCODE_TYPE_SHIFT)
#define __TMS_UTASK_MSGCODE_MODE_MASK		(((1 << __TMS_UTASK_MSGCODE_MODE_WIDTH)-1) \
						     << __TMS_UTASK_MSGCODE_MODE_SHIFT)
#define __TMS_UTASK_MSGCODE_MODE(c)		(((c) & (__TMS_UTASK_MSGCODE_MODE_MASK)) \
						     >> (__TMS_UTASK_MSGCODE_MODE_SHIFT))

#define __TMS_UTASK_MSGCODE_REQ_NE_REP(request, reply)	( \
						((request) & ~(__TMS_UTASK_MSGCODE_SIZE_MASK |		\
						               __TMS_UTASK_MSGCODE_TYPE_MASK)) !=	\
						((reply)   & ~(__TMS_UTASK_MSGCODE_SIZE_MASK |		\
						               __TMS_UTASK_MSGCODE_TYPE_MASK)) )
							             
/*
|| This is the macro that finally creates the message code
*/
#define __TMS_UTASK_MC(Service,Index,Type,Mode,Size,nBuf)	\
						((__TMS_UTASK_MSGCODE)					\
						 ((Service) << __TMS_UTASK_MSGCODE_SERVICE_SHIFT) |	\
						 ((Index)   << __TMS_UTASK_MSGCODE_INDEX_SHIFT)   |	\
						 ((Type)    << __TMS_UTASK_MSGCODE_TYPE_SHIFT)    |	\
						 ((Mode)    << __TMS_UTASK_MSGCODE_MODE_SHIFT)    |	\
						 ((Size)    << __TMS_UTASK_MSGCODE_SIZE_SHIFT)    |	\
						 ((nBuf)    << __TMS_UTASK_MSGCODE_NBUF_SHIFT))

/* 
|| A request and reply may carry attached buffers, as the reply is the feedback for when the 
|| buffer has been consumed, or it may contain requested data
*/
#define	__TMS_UTASK_MC_REQ(Service,Index,Size,nBuf)		\
		__TMS_UTASK_MC(					\
			Service, 				\
			Index, 					\
			__TMS_UTASK_MSGCODE_TYPE_REQ,		\
			__TMS_UTASK_MSGCODE_MODE_SYNC,		\
			Size, 					\
			nBuf)
#define	__TMS_UTASK_MC_REQ_ASYNC(Service,Index,Size,nBuf)	\
		__TMS_UTASK_MC(					\
			Service, 				\
			Index, 					\
			__TMS_UTASK_MSGCODE_TYPE_REQ, 		\
			__TMS_UTASK_MSGCODE_MODE_ASYNC, 	\
			Size, 					\
			nBuf)
#define	__TMS_UTASK_MC_REP(Service,Index,Size,nBuf)		\
		__TMS_UTASK_MC(					\
			Service, 				\
			Index, 					\
			__TMS_UTASK_MSGCODE_TYPE_REP, 		\
			__TMS_UTASK_MSGCODE_MODE_SYNC, 		\
			Size, 					\
			nBuf)
#define	__TMS_UTASK_MC_REP_ASYNC(Service,Index,Size,nBuf)	\
		__TMS_UTASK_MC(					\
			Service, 				\
			Index, 					\
			__TMS_UTASK_MSGCODE_TYPE_REP, 		\
			__TMS_UTASK_MSGCODE_MODE_ASYNC, 	\
			Size, 					\
			nBuf)

/* 
|| An indication may not carry any attached buffers, as there is no feedback for when the buffer-data
|| has been consumed
*/
#define	__TMS_UTASK_MC_IND(Service,Index,Size)			\
		__TMS_UTASK_MC(					\
			Service, 				\
			Index, 					\
			__TMS_UTASK_MSGCODE_TYPE_IND, 		\
			__TMS_UTASK_MSGCODE_MODE_SYNC,		\
			Size, 					\
			0)

/*
|| Define message-code types
*/

#if !__TMS_CLOAKED_NAMES_ONLY
# define UTASK_MSGCODE_TYPE_REQ			\
   __TMS_UTASK_MSGCODE_TYPE_REQ
# define UTASK_MSGCODE_TYPE_REP			\
   __TMS_UTASK_MSGCODE_TYPE_REP
# define UTASK_MSGCODE_TYPE_IND			\
   __TMS_UTASK_MSGCODE_TYPE_IND

# define UTASK_MSGCODE_MODE_SYNC		\
   __TMS_UTASK_MSGCODE_MODE_SYNC
# define UTASK_MSGCODE_MODE_ASYNC		\
   __TMS_UTASK_MSGCODE_MODE_ASYNC

# define UTASK_MSGCODE_SERVICE			\
   __TMS_UTASK_MSGCODE_SERVICE
# define UTASK_MSGCODE_SERVICE_WIDTH		\
   __TMS_UTASK_MSGCODE_SERVICE_WIDTH
# define UTASK_MSGCODE_SERVICE_SHIFT		\
   __TMS_UTASK_MSGCODE_SERVICE_SHIFT
# define UTASK_MSGCODE_SERVICE_MASK		\
   __TMS_UTASK_MSGCODE_SERVICE_MASK
# define UTASK_MSGCODE_SERVICE_READ		\
   __TMS_UTASK_MSGCODE_SERVICE_READ

# define UTASK_MSGCODE_INDEX			\
   __TMS_UTASK_MSGCODE_INDEX
# define UTASK_MSGCODE_INDEX_WIDTH		\
   __TMS_UTASK_MSGCODE_INDEX_WIDTH
# define UTASK_MSGCODE_INDEX_SHIFT		\
   __TMS_UTASK_MSGCODE_INDEX_SHIFT
# define UTASK_MSGCODE_INDEX_MASK		\
   __TMS_UTASK_MSGCODE_INDEX_MASK
# define UTASK_MSGCODE_INDEX_READ		\
   __TMS_UTASK_MSGCODE_INDEX_READ

# define UTASK_MSGCODE_SIZE			\
   __TMS_UTASK_MSGCODE_SIZE
# define UTASK_MSGCODE_SIZE_WIDTH		\
   __TMS_UTASK_MSGCODE_SIZE_WIDTH
# define UTASK_MSGCODE_SIZE_SHIFT		\
   __TMS_UTASK_MSGCODE_SIZE_SHIFT
# define UTASK_MSGCODE_SIZE_MASK		\
   __TMS_UTASK_MSGCODE_SIZE_MASK
# define UTASK_MSGCODE_SIZE_READ		\
   __TMS_UTASK_MSGCODE_SIZE_READ

# define UTASK_MSGCODE_NBUF			\
   __TMS_UTASK_MSGCODE_NBUF
# define UTASK_MSGCODE_NBUF_WIDTH		\
   __TMS_UTASK_MSGCODE_NBUF_WIDTH
# define UTASK_MSGCODE_NBUF_SHIFT		\
   __TMS_UTASK_MSGCODE_NBUF_SHIFT
# define UTASK_MSGCODE_NBUF_MASK		\
   __TMS_UTASK_MSGCODE_NBUF_MASK
# define UTASK_MSGCODE_NBUF_READ		\
   __TMS_UTASK_MSGCODE_NBUF_READ

# define UTASK_MSGCODE_TYPE			\
   __TMS_UTASK_MSGCODE_TYPE
# define UTASK_MSGCODE_TYPE_WIDTH		\
   __TMS_UTASK_MSGCODE_TYPE_WIDTH
# define UTASK_MSGCODE_TYPE_SHIFT		\
   __TMS_UTASK_MSGCODE_TYPE_SHIFT
# define UTASK_MSGCODE_TYPE_MASK		\
   __TMS_UTASK_MSGCODE_TYPE_MASK
# define UTASK_MSGCODE_TYPE_READ		\
   __TMS_UTASK_MSGCODE_TYPE_READ

# define UTASK_MSGCODE_MODE			\
   __TMS_UTASK_MSGCODE_MODE
# define UTASK_MSGCODE_MODE_WIDTH		\
   __TMS_UTASK_MSGCODE_MODE_WIDTH
# define UTASK_MSGCODE_MODE_SHIFT		\
   __TMS_UTASK_MSGCODE_MODE_SHIFT
# define UTASK_MSGCODE_MODE_MASK		\
   __TMS_UTASK_MSGCODE_MODE_MASK
# define UTASK_MSGCODE_MODE_READ		\
   __TMS_UTASK_MSGCODE_MODE_READ

# define UTASK_MSGCODE_REQ_NE_REP		\
   __TMS_UTASK_MSGCODE_REQ_NE_REP

# define UTASK_MC				\
   __TMS_UTASK_MC
# define UTASK_MC_REQ				\
   __TMS_UTASK_MC_REQ
# define UTASK_MC_REP				\
   __TMS_UTASK_MC_REP
# define UTASK_MC_IND				\
   __TMS_UTASK_MC_IND
#endif

/**** end of utaskmsg_codes.h ****/
#endif /* _UTASKMSG_CODES_H_ */
