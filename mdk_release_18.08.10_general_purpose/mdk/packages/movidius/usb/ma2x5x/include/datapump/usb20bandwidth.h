/* usb20bandwidth.h	Wed Feb 13 2008 10:29:57 djt */

/*

Module:  usb20bandwidth.h

Function:
	Macros for computing bus bandwidth for USB 2.0

Version:
	V1.97k	Wed Feb 13 2008 10:29:57 djt	Edit level 3

Copyright notice:
	This file copyright (C) 2005-2007 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Sat Jun 18 2005 21:45:05  tmm
	Module created.

   1.97j  Tue Jul 17 2007 07:07:23  djt
	3061: Removed TT Start plus Complete combination macros.
	3153: Removed ack from TT_INT_IN_CSPLIT_CLOCKS.

   1.97k  Wed Feb 13 2008 10:29:57  djt
	6900: Fix comments for TT
	Added best-case macros, which are worst-case minus the bit stuffing, 
	host delay, and host timeout.

*/

#ifndef _USB20BANDWIDTH_H_		/* prevent multiple includes */
#define _USB20BANDWIDTH_H_

/****************************************************************************\
|
|	This module defines macros and functions to calculate normalized
|	bus-clocks needed for various kinds of transactions.
|
\****************************************************************************/

#define	__TMS_USB20_HIGHSPEED_CLOCKS		60000	/* 480,000,000 / 8,000 */
#define	__TMS_USB20_HIGHSPEED_CLOCKS_PERIODIC	48000	/* hi-speed clocks * .8
							|| per 5.7.4
							*/
#define	__TMS_USB20_FULLSPEED_CLOCKS		12000	/* 12MHz * 1ms */
#define	__TMS_USB20_FULLSPEED_CLOCKS_PERIODIC	10800	/* full-speed clocks * .9
							|| per 5.7.4
							*/

/* convert low-speed clocks to full-speed clocks */
#define	__TMS_USB20_LOWSPEED_CLOCKS_TO_FULLSPEED(x)  ((x) * 8)

/* compute bitstuff */
#define	__TMS_USB20_BITSTUFF_BITS(nData)	(((nData) * 7 + 6) / 6)

/* calculate clocks for low-speed int */
/* what about inter-packet delay (7.1.18.2)? */
#define	__TMS_USB20_LOWSPEED_INT_CLOCKS(nPayloadBytes, hsu_clks)	\
	(								\
	/* fs SYNC */ 8 + /* fs PRE */ 8 + /* hub setup */ (hsu_clks) +	\
	/* ls SYNC */ 8*8 + /* ls PID */ 8*8 +				\
	/* ep */ 8 * __TMS_USB20_BITSTUFF_BITS(2*8) +			\
	/* ls eop */ 3*8 +						\
	/* host timeout */ 18 +						\
	/* ls SYNC + PID */ (2*8)*8 +					\
	/* ls DATA */ 8 * __TMS_USB20_BITSTUFF_BITS(8*(2+(nPayloadBytes))) + \
	/* ls eop */ 3*8 +						\
	/* fs SYNC */ 8 + /* fs PRE */ 8 + /* hub setup */ (hsu_clks) +	\
	/* ls SYNC */ 8*8 + /* ls PID */ 8*8 +				\
	/* ls eop */ 3*8						\
	)


/* calculate clocks for full-speed isoch (IN) */
/* what about inter-packet delay (7.1.18.2)? */
/* to this add host delay */
#define	__TMS_USB20_FULLSPEED_ISOCH_CLOCKS(nPayloadBytes)		\
	(								\
	/* fs SYNC */ 8 + /* fs PID */ 8 +				\
	/* ep */ __TMS_USB20_BITSTUFF_BITS(2*8) +			\
	/* eop */ 3 +							\
	/* worst case host timeout */ 18 +				\
	/* fs SYNC */ 8 + /* fs PID */ 8 +				\
	/* fs DATA */ __TMS_USB20_BITSTUFF_BITS(8*(2+(nPayloadBytes))) + \
	/* fs eop */ 3 + /* minimum gap */ 2				\
	)

/* calculate clocks for full-speed interrupt */
/*	(3*8 + 3*8 + __TMS_USB20_BITSTUFF_BITS(4*8) + 3*8 +	*/
/*		     __TMS_USB20_BITSTUFF_BITS((nPayloadBytes) * 8)) */
/* add host delay */
#define	__TMS_USB20_FULLSPEED_INT_CLOCKS(nPayloadBytes)			\
	(								\
	/* fs SYNC */ 8 + /* fs PID */ 8 +				\
	/* ep */ __TMS_USB20_BITSTUFF_BITS(2*8) +			\
	/* fs eop */ 3 +						\
	/* worst case host timeout */ 18 +				\
	/* fs SYNC */ 8 + /* fs PID */ 8 +				\
	/* fs DATA */ __TMS_USB20_BITSTUFF_BITS(8*(2+(nPayloadBytes))) + \
	/* fs eop */ 3 +						\
	/* worst case host delay */ 7 +					\
	/* fs SYNC */ 8 + /* fs PID */ 8 +				\
	/* fs eop */ 3 + /* minimum gap */ 2				\
	)

/* calculate clocks for high-speed isoch */
/* #define	__TMS_USB20_HIGHSPEED_ISOCH_CLOCKS(nPayloadBytes)	*/
/*	(2*8 + 2*88 + 2*32 + 2*8 + __TMS_USB20_BITSTUFF_BITS(2*8) +	*/
/*		__TMS_USB20_BITSTUFF_BITS((nPayloadBytes)*8+16))	*/
/* add host delay */
/* XXX worst case host timeout of of 816 clocks would seem to also be	*/
/* needed, but isn't included in the spec's formulae.			*/
#define	__TMS_USB20_HIGHSPEED_ISOCH_CLOCKS(nPayloadBytes)		\
	(								\
	/* hs SYNC */ 32 + /* PID */ 8 +				\
	/* ep */ __TMS_USB20_BITSTUFF_BITS(2*8) +			\
	/* eop */ 8 + /* delay */ 88 +					\
	/* avg prop */ (816-88) +					\
	/* SYNC */ 32 + /* hs PID */ 8 +				\
	/* DATA */ __TMS_USB20_BITSTUFF_BITS(8*(2+(nPayloadBytes))) +	\
	/* eop */ 8 + /* delay */ 88					\
	)

/* calculate clocks for high-speed int */
#define	__TMS_USB20_HIGHSPEED_INT_CLOCKS(nPayloadBytes)	\
	(/* OUT/IN */ 32 + 8 + __TMS_USB20_BITSTUFF_BITS(2*8) + 8 + 88 + \
	 /* DATA */   32 + 8 + __TMS_USB20_BITSTUFF_BITS((nPayloadBytes)*8 +  16) + \
				8 + 88 +				\
	 /* avg prop */ (816-88) +					\
	 /* ACK */    32 + 8 + 8 + 88)

/****************************************************************************\
|
|	Now define best-case macros.
|
\****************************************************************************/
/* calculate clocks for low-speed int */
#define	__TMS_USB20_LOWSPEED_BESTCASE_INT_CLOCKS(nPayloadBytes, hsu_clks)\
	(								\
	/* fs SYNC */ 8 + /* fs PRE */ 8 + /* hub setup */ (hsu_clks) +	\
	/* ls SYNC */ 8*8 + /* ls PID */ 8*8 +				\
	/* ep */ 8 * (2*8) +						\
	/* ls eop */ 3*8 +						\
	/* best case host timeout */ 0 +				\
	/* ls SYNC + PID */ (2*8)*8 +					\
	/* ls DATA */ 8 * (8*(2+(nPayloadBytes))) + 			\
	/* ls eop */ 3*8 +						\
	/* fs SYNC */ 8 + /* fs PRE */ 8 + /* hub setup */ (hsu_clks) +	\
	/* ls SYNC */ 8*8 + /* ls PID */ 8*8 +				\
	/* ls eop */ 3*8						\
	)


/* calculate clocks for full-speed isoch (IN) */
#define	__TMS_USB20_FULLSPEED_BESTCASE_ISOCH_CLOCKS(nPayloadBytes)	\
	(								\
	/* fs SYNC */ 8 + /* fs PID */ 8 +				\
	/* ep */ (2*8) +						\
	/* eop */ 3 +							\
	/* best case host timeout */ 0 +				\
	/* fs SYNC */ 8 + /* fs PID */ 8 +				\
	/* fs DATA */ (8*(2+(nPayloadBytes))) + 			\
	/* fs eop */ 3 + /* minimum gap */ 2				\
	)

/* calculate clocks for full-speed interrupt */
#define	__TMS_USB20_FULLSPEED_BESTCASE_INT_CLOCKS(nPayloadBytes)	\
	(								\
	/* fs SYNC */ 8 + /* fs PID */ 8 +				\
	/* ep */ (2*8) +						\
	/* fs eop */ 3 +						\
	/* best case host timeout */ 0 +				\
	/* fs SYNC */ 8 + /* fs PID */ 8 +				\
	/* fs DATA */ (8*(2+(nPayloadBytes))) + 			\
	/* fs eop */ 3 +						\
	/* best case host delay */ 0 +					\
	/* fs SYNC */ 8 + /* fs PID */ 8 +				\
	/* fs eop */ 3 + /* minimum gap */ 2				\
	)

/****************************************************************************\
|
|	The TT calculations that follow have NOT been verified.
|
\****************************************************************************/

/* calculate clocks for split transaction isoch out */
/* nPayloadBytes should never be set greater than 188 */
#define	__TMS_USB20_HIGHSPEED_TT_ISOCH_OUT_SSPLIT_CLOCKS(nPayloadBytes)	\
	(/* ssplit */ 32 + 8 + __TMS_USB20_BITSTUFF_BITS(3*8) + 8 + 88 + \
	 /* out */    32 + 8 + 8 + 88 + \
	 /* DATA */   32 + 8 + __TMS_USB20_BITSTUFF_BITS((nPayloadBytes)*8 + 16) +\
	/* EOP */     8 + 88)

/* calculate clocks for split transaction isoch IN */
/* nPayloadBytes should never be set greater than 188 */
#define	__TMS_USB20_HIGHSPEED_TT_ISOCH_IN_SSPLIT_CLOCKS(nPayloadBytes)	\
	(/* ssplit */ 32 + 8 + __TMS_USB20_BITSTUFF_BITS(3*8) + 8 + 88 + \
	 /* in */     32 + 8 + 8 + 88 )

#define	__TMS_USB20_HIGHSPEED_TT_ISOCH_IN_CSPLIT_CLOCKS(nPayloadBytes)	\
	(/* csplit */ 32 + 8 + __TMS_USB20_BITSTUFF_BITS(3*8) + 8 + 88 + \
	 /* in */     32 + 8 + 8 + 88 + \
	 /* data */   32 + 8 + __TMS_USB20_BITSTUFF_BITS((nPayloadBytes)*8 + 16) \
			     + 8 + 88)

/*
|| Int TT in/out are actually the same but let's leave them separate for now.
*/	
/* calculate clocks for split transaction int out */
#define	__TMS_USB20_HIGHSPEED_TT_INT_OUT_SSPLIT_CLOCKS(nPayloadBytes)	\
	(/* ssplit */ 32 + 8 + __TMS_USB20_BITSTUFF_BITS(3*8) + 8 + 88 + \
	 /* out */    32 + 8 + 8 + 88 + \
	 /* DATA */   32 + 8 + __TMS_USB20_BITSTUFF_BITS((nPayloadBytes)*8 + 16) \
			     + 8 + 88)

#define	__TMS_USB20_HIGHSPEED_TT_INT_OUT_CSPLIT_CLOCKS(nPayloadBytes)	\
	(/* csplit */ 32 + 8 + __TMS_USB20_BITSTUFF_BITS(3*8) + 8 + 88 + \
	 /* out */    32 + 8 + 8 + 88 + \
	 /* ack */    32 + 8 + 8 + 88)

/* calculate clocks for split transaction int in */
#define	__TMS_USB20_HIGHSPEED_TT_INT_IN_SSPLIT_CLOCKS(nPayloadBytes)	\
	(/* ssplit */ 32 + 8 + __TMS_USB20_BITSTUFF_BITS(3*8) + 8 + 88 + \
	 /* in */     32 + 8 + 8 + 88)

#define	__TMS_USB20_HIGHSPEED_TT_INT_IN_CSPLIT_CLOCKS(nPayloadBytes)	\
	(/* csplit */ 32 + 8 + __TMS_USB20_BITSTUFF_BITS(3*8) + 8 + 88 + \
	 /* in */     32 + 8 + 8 + 88 + \
	 /* DATA */   32 + 8 + __TMS_USB20_BITSTUFF_BITS((nPayloadBytes)*8 + 16) \
			     + 8 + 88) 

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USB20_HIGHSPEED_CLOCKS	\
   __TMS_USB20_HIGHSPEED_CLOCKS
# define USB20_HIGHSPEED_CLOCKS_PERIODIC	\
   __TMS_USB20_HIGHSPEED_CLOCKS_PERIODIC
# define USB20_FULLSPEED_CLOCKS	\
   __TMS_USB20_FULLSPEED_CLOCKS
# define USB20_FULLSPEED_CLOCKS_PERIODIC	\
   __TMS_USB20_FULLSPEED_CLOCKS_PERIODIC
# define USB20_LOWSPEED_CLOCKS_TO_FULLSPEED(x)	\
   __TMS_USB20_LOWSPEED_CLOCKS_TO_FULLSPEED(x)
# define USB20_BITSTUFF_BITS(nData)	\
   __TMS_USB20_BITSTUFF_BITS(nData)
# define USB20_LOWSPEED_INT_CLOCKS(nPayloadBytes, hsu_clks)	\
   __TMS_USB20_LOWSPEED_INT_CLOCKS(nPayloadBytes, hsu_clks)
# define USB20_FULLSPEED_ISOCH_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_FULLSPEED_ISOCH_CLOCKS(nPayloadBytes)
# define USB20_FULLSPEED_INT_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_FULLSPEED_INT_CLOCKS(nPayloadBytes)
# define USB20_HIGHSPEED_ISOCH_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_HIGHSPEED_ISOCH_CLOCKS(nPayloadBytes)
# define USB20_HIGHSPEED_INT_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_HIGHSPEED_INT_CLOCKS(nPayloadBytes)
# define USB20_LOWSPEED_BESTCASE_INT_CLOCKS(nPayloadBytes, hsu_clks)	\
   __TMS_USB20_LOWSPEED_BESTCASE_INT_CLOCKS(nPayloadBytes, hsu_clks)
# define USB20_FULLSPEED_BESTCASE_ISOCH_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_FULLSPEED_BESTCASE_ISOCH_CLOCKS(nPayloadBytes)
# define USB20_FULLSPEED_BESTCASE_INT_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_FULLSPEED_BESTCASE_INT_CLOCKS(nPayloadBytes)
# define USB20_HIGHSPEED_TT_ISOCH_OUT_SSPLIT_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_HIGHSPEED_TT_ISOCH_OUT_SSPLIT_CLOCKS(nPayloadBytes)
# define USB20_HIGHSPEED_TT_ISOCH_IN_SSPLIT_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_HIGHSPEED_TT_ISOCH_IN_SSPLIT_CLOCKS(nPayloadBytes)
# define USB20_HIGHSPEED_TT_ISOCH_IN_CSPLIT_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_HIGHSPEED_TT_ISOCH_IN_CSPLIT_CLOCKS(nPayloadBytes)
# define USB20_HIGHSPEED_TT_INT_OUT_SSPLIT_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_HIGHSPEED_TT_INT_OUT_SSPLIT_CLOCKS(nPayloadBytes)
# define USB20_HIGHSPEED_TT_INT_OUT_CSPLIT_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_HIGHSPEED_TT_INT_OUT_CSPLIT_CLOCKS(nPayloadBytes)
# define USB20_HIGHSPEED_TT_INT_IN_SSPLIT_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_HIGHSPEED_TT_INT_IN_SSPLIT_CLOCKS(nPayloadBytes)
# define USB20_HIGHSPEED_TT_INT_IN_CSPLIT_CLOCKS(nPayloadBytes)	\
   __TMS_USB20_HIGHSPEED_TT_INT_IN_CSPLIT_CLOCKS(nPayloadBytes)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/**** end of usb20bandwidth.h ****/
#endif /* _USB20BANDWIDTH_H_ */
