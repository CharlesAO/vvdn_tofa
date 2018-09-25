/* libport_128bit_help.h	Thu Apr 13 2006 01:11:52 tmm */

/*

Module:  libport_128bit_help.h

Function:
	Various help macros for doing 128 bit operations on UINT128_S and
	INT128_S objects.

Version:
	V3.05h	Thu Apr 13 2006 01:11:52 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2006 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	April 2006

Revision history:
   3.05h  Thu Apr 13 2006 01:11:52  tmm
	Module created.

*/

#ifndef _LIBPORT_128BIT_HELP_H_		/* prevent multiple includes */
#define _LIBPORT_128BIT_HELP_H_

#ifndef _DEF_CDEFS_H_
# include <def/cdefs.h>
#endif


#if ! __TMS_LIBPORT_HAS_64BIT
# error "libport_128bit_ops.h requires 64-bit compiler support"
#endif

/* the gross hack that finds us a dword in order of significance */
#define __TMS_UINT128_XQWORD(lv, i) \
	((lv).uq[__TMS_QQENDIAN == __TMS_QQENDIAN_LITTLE ? (i) : (1 - (i))])

#define __TMS_INT128_XQWORD(lv, i)	__TMS_UINT128_XQWORD(lv, i)
#define __TMS_INT128_XSQWORD(lv, i)	\
	((lv).sq[__TMS_QQENDIAN == __TMS_QQENDIAN_LITTLE ? (i) : (1 - (i))])


  /* define the increment primitive */
#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_INC(lv) (++(lv).qq)
#else
# define __TMS_UINT128_INC(lv) \
	((++__TMS_UINT128_XQWORD(lv, 0) == 0) ? ++__TMS_UINT128_XQWORD(lv, 1) : 0)
#endif

#define __TMS_INT128_INC(lv)	__TMS_UINT128_INC(lv)

  /* the decrement primitive */
#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_DEC(lv) (--(lv).qq)
#else
# define __TMS_UINT128_DEC(lv) \
	(__TMS_UINT128_XQWORD(lv, 0)-- == 0 ? --__TMS_UINT128_XQWORD(lv, 1) : 0)
#endif

#define __TMS_INT128_DEC(lv)	__TMS_UINT128_DEC(lv)
  

  /* define the compare primitives */
#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_EQ(lv1, lv2) ((lv1).qq == (lv2).qq)
#else
# define __TMS_UINT128_EQ(lv1, lv2) \
	(((lv1).uq[0] == (lv2).uq[0]) && (lv1).uq[1] == (lv2).uq[1])
#endif

#define __TMS_INT128_EQ(lv1, lv2)	__TMS_UINT128_EQ(lv1, lv2)

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_EQZ(lv)	((lv).qq == 0)
#else
# define __TMS_UINT128_EQZ(lv) \
	(((lv).uq[0] | (lv).uq[1]) == 0)
#endif

#define __TMS_INT128_EQZ(lv)		__TMS_UINT128_EQZ(lv)

#define __TMS_UINT128_NEZ(lv)		(! __TMS_UINT128_EQZ(lv))
#define __TMS_INT128_NEZ(lv)		(! __TMS_INT128_EQZ(lv))

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_INT128_LTZ(lv)	((lv).qq < 0)
#else
# define __TMS_INT128_LTZ(lv)	(__TMS_INT128_XSQWORD(lv, 1) < 0)
#endif

#define __TMS_INT128_GEZ(lv) 	(! __TMS_INT128_LTZ(lv))

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_INT128_GTZ(lv) 	((lv).qq > 0)
#else
# define __TMS_INT128_GTZ(lv) 	(__TMS_INT128_GEZ(lv) && __TMS_INT128_NEZ(lv))
#endif

#define __TMS_INT128_LEZ(lv)	(! __TMS_INT128_GTZ(lv))

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_LT(lv1, lv2)	((lv1).qq < (lv2).qq)
#else
# define __TMS_UINT128_LT(lv1, lv2) \
	(__TMS_UINT128_XQWORD(lv1, 1) < __TMS_UINT128_XQWORD(lv2, 1) ? 1 : \
	 __TMS_UINT128_XQWORD(lv1, 1) > __TMS_UINT128_XQWORD(lv2, 1) ? 0 : \
	 __TMS_UINT128_XQWORD(lv1, 0) < __TMS_UINT128_XQWORD(lv2, 0) ? 1 : 0)
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_INT128_LT(lv1, lv2)	((lv1).qq < (lv2).qq)
#else
# define __TMS_INT128_LT(lv1, lv2) \
	(__TMS_INT128_XSQWORD(lv1, 1) < __TMS_INT128_XSQWORD(lv2, 1) ? 1 : \
	 __TMS_INT128_XSQWORD(lv1, 1) > __TMS_INT128_XSQWORD(lv2, 1) ? 0 : \
	 __TMS_INT128_XQWORD(lv1, 0)  < __TMS_INT128_XQWORD(lv2, 0) ? 1 : 0)
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_LE(lv1, lv2)	((lv1).qq <= (lv2).qq)
#else
# define __TMS_UINT128_LE(lv1, lv2) \
	(__TMS_UINT128_XQWORD(lv1, 1) < __TMS_UINT128_XQWORD(lv2, 1) ? 1 : \
	 __TMS_UINT128_XQWORD(lv1, 1) > __TMS_UINT128_XQWORD(lv2, 1) ? 0 : \
	 __TMS_UINT128_XQWORD(lv1, 0) <= __TMS_UINT128_XQWORD(lv2, 0) ? 1 : 0)
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_INT128_LE(lv1, lv2)	((lv1).qq <= (lv2).qq)
#else
# define __TMS_INT128_LE(lv1, lv2) \
	(__TMS_INT128_XSQWORD(lv1, 1) < __TMS_INT128_XSQWORD(lv2, 1) ? 1 : \
	 __TMS_INT128_XSQWORD(lv1, 1) > __TMS_INT128_XSQWORD(lv2, 1) ? 0 : \
	 __TMS_UINT128_XQWORD(lv1, 0) <= __TMS_UINT128_XQWORD(lv2, 0) ? 1 : 0)
#endif

#define	__TMS_UINT128_NE(l1, l2) (! __TMS_UINT128_EQ(l1, l2))
#define	__TMS_INT128_NE(l1, l2)	(! __TMS_INT128_EQ(l1, l2))

#define	__TMS_UINT128_GT(l1, l2)	__TMS_UINT128_LT(l2, l1)
#define	__TMS_INT128_GT(l1, l2)	__TMS_INT128_LT(l2, l1)

#define	__TMS_UINT128_GE(l1, l2) __TMS_UINT128_LE(l2, l1)
#define	__TMS_INT128_GE(l1, l2)	__TMS_INT128_LE(l2, l1)

#if __TMS_LIBPORT_HAS_128BIT
# define	__TMS_UINT128_ADD3(lres, l1, l2) \
		do { (lres).qq = (l1).qq + (l2).qq; } while (0)
#else
# define	__TMS_UINT128_ADD3(lres, l1, l2) \
	do	{ \
		__TMS_UINT64 __r1; \
		__TMS_UINT128_XQWORD(lres, 0) = __r1 = __TMS_UINT128_XQWORD(l1, 0) + __TMS_UINT128_XQWORD(l2, 0); \
		__TMS_UINT128_XQWORD(lres, 1) = __TMS_UINT128_XQWORD(l1, 1) + __TMS_UINT128_XQWORD(l2, 1); \
		if (__r1 < __TMS_UINT128_XQWORD(l1, 0)) \
			++__TMS_UINT128_XQWORD(lres, 1); \
		} while (0)
#endif

#define	__TMS_INT128_ADD3(lres, l1, l2)	__TMS_UINT128_ADD3(lres, l1, l2)

#if __TMS_LIBPORT_HAS_128BIT
# define	__TMS_UINT128_SUB3(lres, l1, l2) \
		do { (lres).qq = (l1).qq - (l2).qq; } while (0)
#else
# define	__TMS_UINT128_SUB3(lres, l1, l2) \
	do	{ \
		__TMS_UINT64 __r1; \
		__TMS_UINT128_XQWORD(lres, 0) = __r1 = __TMS_UINT128_XQWORD(l1, 0) - __TMS_UINT128_XQWORD(l2, 0); \
		__TMS_UINT128_XQWORD(lres, 1) = __TMS_UINT128_XQWORD(l1, 1) - __TMS_UINT128_XQWORD(l2, 1); \
		if (__r1 > __TMS_UINT128_XQWORD(l1, 0)) \
			--__TMS_UINT128_XQWORD(lres, 1); \
		} while (0)
#endif

#define	__TMS_INT128_SUB3(lres, l1, l2)	__TMS_UINT128_SUB3(lres, l1, l2)

/* 128-bit assignment */
#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_SET(lv, rv) ((lv).qq = (rv).qq)
#else
# define __TMS_UINT128_SET(lv, rv) \
	(((lv).uq[0] = (rv).uq[0]), \
	 ((lv).uq[1] = (rv).uq[1]))
#endif

#define	__TMS_INT128_SET(lv, rv)		__TMS_UINT128_SET(lv, rv)

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_ZERO(lv)	((lv).qq = 0)
#else
# define __TMS_UINT128_ZERO(lv) \
	((lv).uq[1] = (lv).uq[0] = 0)
#endif

#define __TMS_INT128_ZERO(lv)		__TMS_UINT128_ZERO(lv)

/* 128-bit bitwise negation -- the cast in the native form helps with warnings */
#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_NOT2(lv, rv)	((lv).qq = ~(__TMS_UINT128)((rv).qq))
#else
# define __TMS_UINT128_NOT2(lv, rv) \
		(((lv).uq[0] = ~(rv).uq[0]), \
		 ((lv).uq[1] = ~(rv).uq[1]))
#endif

#define __TMS_UINT128_NOT1(lv)	__TMS_UINT128_NOT2(lv, lv)

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_NEG1(lv)	((lv).qq = (__TMS_UINT128)0 - (lv).qq)
#else
# define __TMS_UINT128_NEG1(lv) \
	(__TMS_UINT128_COMP1(lv), __TMS_UINT128_INC(lv))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_INT128_NEG1(lv)	((lv).qq = -(lv).qq)
#else
# define __TMS_INT128_NEG1(lv)	__TMS_UINT128_NEG1(lv)
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_AND3(lv, rv1, rv2)	((lv).qq = (rv1).qq & (rv2).qq)
#else
# define __TMS_UINT128_AND3(lv, rv1, rv2) \
	(((lv).uq[0] = (rv1).uq[0] & (rv2).uq[0]), \
	 ((lv).uq[1] = (rv1).uq[1] & (rv2).uq[1]))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_AND3PN(lv, rv1, rv2)	((lv).qq = (rv1).qq & ~(rv2).qq)
#else
# define __TMS_UINT128_AND3PN(lv, rv1, rv2) \
	(((lv).uq[0] = (rv1).uq[0] & ~(rv2).uq[0]), \
	 ((lv).uq[1] = (rv1).uq[1] & ~(rv2).uq[1]))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_AND3NP(lv, rv1, rv2)	((lv).qq = ~(rv1).qq & (rv2).qq)
#else
# define __TMS_UINT128_AND3NP(lv, rv1, rv2) \
	(((lv).uq[0] = ~(rv1).uq[0] & (rv2).uq[0]), \
	 ((lv).uq[1] = ~(rv1).uq[1] & (rv2).uq[1]))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_OR3(lv, rv1, rv2)	((lv).qq = (rv1).qq | (rv2).qq)
#else
# define __TMS_UINT128_OR3(lv, rv1, rv2) \
	(((lv).uq[0] = (rv1).uq[0] | (rv2).uq[0]), \
	 ((lv).uq[1] = (rv1).uq[1] | (rv2).uq[1]))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_OR3PN(lv, rv1, rv2)	((lv).qq = (rv1).qq | ~(rv2).qq)
#else
# define __TMS_UINT128_OR3PN(lv, rv1, rv2) \
	(((lv).uq[0] = (rv1).uq[0] | ~(rv2).uq[0]), \
	 ((lv).uq[1] = (rv1).uq[1] | ~(rv2).uq[1]))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_OR3NP(lv, rv1, rv2)	((lv).qq = ~(rv1).qq | (rv2).qq)
#else
# define __TMS_UINT128_OR3NP(lv, rv1, rv2) \
	(((lv).uq[0] = ~(rv1).uq[0] | (rv2).uq[0]), \
	 ((lv).uq[1] = ~(rv1).uq[1] | (rv2).uq[1]))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_XOR3(lv, rv1, rv2)	((lv).qq = (rv1).qq ^ (rv2).qq)
#else
# define __TMS_UINT128_XOR3(lv, rv1, rv2) \
	(((lv).uq[0] = (rv1).uq[0] ^ (rv2).uq[0]), \
	 ((lv).uq[1] = (rv1).uq[1] ^ (rv2).uq[1]))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_NAND3(lv, rv1, rv2)	((lv).qq = ~((rv1).qq & (rv2).qq))
#else
# define __TMS_UINT128_NAND3(lv, rv1, rv2) \
	(((lv).uq[0] = ~((rv1).uq[0] & (rv2).uq[0])), \
	 ((lv).uq[1] = ~((rv1).uq[1] & (rv2).uq[1])))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_NOR3(lv, rv1, rv2)	((lv).qq = ~((rv1).qq | (rv2).qq))
#else
# define __TMS_UINT128_NOR3(lv, rv1, rv2) \
	(((lv).uq[0] = ~((rv1).uq[0] | (rv2).uq[0])), \
	 ((lv).uq[1] = ~((rv1).uq[1] | (rv2).uq[1])))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_XNOR3(lv, rv1, rv2)	((lv).qq = ~((rv1).qq ^ (rv2).qq))
#else
# define __TMS_UINT128_XNOR3(lv, rv1, rv2) \
	(((lv).uq[0] = ~(rv1).uq[0] ^ (rv2).uq[0]), \
	 ((lv).uq[1] = ~(rv1).uq[1] ^ (rv2).uq[1]))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_LSR2(lv, n)	((lv).qq >>= (n))
#else
# define __TMS_UINT128_LSR2(lv, n) \
	 do	{ \
		__TMS_UINT64 __t; \
		__t = __TMS_UINT128_XQWORD(lv, 1); \
		if ((n) < 64) \
			{ \
			__TMS_UINT128_XQWORD(lv, 1) = __t >> (n); \
			__TMS_UINT128_XQWORD(lv, 0) >>= (n); \
			__TMS_UINT128_XQWORD(lv, 0) |= __t << (64 - (n)); \
			} \
		else  \
			{ \
			__TMS_UINT128_XQWORD(lv, 1) = 0; \
			__TMS_UINT128_XQWORD(lv, 0) = __t >> ((n) - 64); \
			} \
		} while (0)
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_LSL2(lv, n)	((lv).qq <<= (n))
#else
# define __TMS_UINT128_LSL2(lv, n) \
	 do	{ \
		__TMS_UINT64 __t; \
		__t = __TMS_UINT128_XQWORD(lv, 0); \
		if ((n) < 64) \
			{ \
			__TMS_UINT128_XQWORD(lv, 0) = __t << (n); \
			__TMS_UINT128_XQWORD(lv, 1) <<= (n); \
			__TMS_UINT128_XQWORD(lv, 0) |= __t >> (64 - (n)); \
			} \
		else  \
			{ \
			__TMS_UINT128_XQWORD(lv, 0) = 0; \
			__TMS_UINT128_XQWORD(lv, 1) = __t << ((n) - 64); \
			} \
		} while (0)
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT64toUINT128(lv, ullong_v)	((lv).qq = (ulong_v))
#else
# define __TMS_UINT64toUINT128(lv, ullong_v) \
	 ((__TMS_UINT128_XQWORD(lv, 0) = (ullong_v)), (__TMS_UINT128_XQWORD(lv, 1) = 0))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_INT64toINT128(lv, llong_v)		((lv).qq = (llong_v))
#else
# define __TMS_INT64toINT128(lv, llong_v) \
	 ((__TMS_UINT128_XQWORD(lv, 0) = (llong_v)), \
	  (__TMS_UINT128_XSQWORD(lv, 1) = (llong_v) < 0 ? -1, 0))
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128toUINT64(lv)	((__TMS_UINT64)(lv).qq)
#else
# define __TMS_UINT128toUINT64(lv)	__TMS_UINT128_XQWORD(lv, 0)
#endif

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_INT128toINT64(lv)	((__TMS_INT64)(lv).qq)
#else
# define __TMS_INT128toINT64(lv)	((__TMS_INT64) __TMS_UINT128_XQWORD(lv, 0))
#endif

/* 
|| to multiply uq x uq giving ul128, note:
||		(ah,al)
||		(bh,bl)
||	   ------------
|| 		  albl
||		ahbl
||		bhal
||	      ahbh
*/

/* multiply 2 uint64s to get a uint128 */

#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_MULU64_3(lv, a, b) \
	do { (lv).qq = ((__TMS_UINT128)(a) * (b)); } while (0)
#else
# define __TMS_UINT128_MULU64_3(lv, a, b) \
	do	{ \
		__TMS_UINT64 __t1;  \
		__TMS_UINT128_XQWORD(lv, 0) = (__TMS_UINT64)(a) * (__TMS_UINT64)(b); \
		__t1 = ((__TMS_UINT64)(a) & 0xFFFFFFF) * ((__TMS_UINT64)(b) >> 32); \
		__t1 += ((__TMS_UINT64)(b) & 0xFFFFFFFF) * ((__TMS_UINT64)(a) >> 32); \
		__t1 >>= 32; \
		__t1 += ((__TMS_UINT64)(a) >> 32) * ((__TMS_UINT64)(b) >> 32); \
		__TMS_UINT128_XQWORD(lv, 1) = __t1; \
		} while (0)
#endif

/* multiply two UINT128s */
#if __TMS_LIBPORT_HAS_128BIT
# define __TMS_UINT128_MUL3(lv, rv1, rv2) \
	do { (lv).qq = (rv1).qq * (rv2).qq; } while (0)
#else
# define __TMS_UINT128_MUL3(lv, rv1, rv2) \
	do	{ \
		__TMS_UINT128_MULU64_3(lv, __TMS_UINT128_XQWORD(rv1, 0), __TMS_UINT128_XQWORD(rv2, 0));	\
		__TMS_UINT128_XQWORD(lv, 1) += __TMS_UINT128_XQWORD(rv1, 0) * __TMS_UINT128_XQWORD(rv2, 1) \
					   +  __TMS_UINT128_XQWORD(rv1, 1) * __TMS_UINT128_XQWORD(rv2, 0); \
		} while (0)
#endif

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh -l | uniq ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UINT128_XQWORD(lv, i)	__TMS_UINT128_XQWORD(lv, i)
# define INT128_XQWORD(lv, i)	__TMS_INT128_XQWORD(lv, i)
# define INT128_XSQWORD(lv, i)	__TMS_INT128_XSQWORD(lv, i)
# define UINT128_INC(lv)	__TMS_UINT128_INC(lv)
# define INT128_INC(lv)	__TMS_INT128_INC(lv)
# define UINT128_DEC(lv)	__TMS_UINT128_DEC(lv)
# define INT128_DEC(lv)	__TMS_INT128_DEC(lv)
# define UINT128_EQ(lv1, lv2)	__TMS_UINT128_EQ(lv1, lv2)
# define INT128_EQ(lv1, lv2)	__TMS_INT128_EQ(lv1, lv2)
# define UINT128_EQZ(lv)	__TMS_UINT128_EQZ(lv)
# define INT128_EQZ(lv)	__TMS_INT128_EQZ(lv)
# define UINT128_NEZ(lv)	__TMS_UINT128_NEZ(lv)
# define INT128_NEZ(lv)	__TMS_INT128_NEZ(lv)
# define INT128_LTZ(lv)	__TMS_INT128_LTZ(lv)
# define INT128_GEZ(lv)	__TMS_INT128_GEZ(lv)
# define INT128_GTZ(lv)	__TMS_INT128_GTZ(lv)
# define INT128_LEZ(lv)	__TMS_INT128_LEZ(lv)
# define UINT128_LT(lv1, lv2)	__TMS_UINT128_LT(lv1, lv2)
# define INT128_LT(lv1, lv2)	__TMS_INT128_LT(lv1, lv2)
# define UINT128_LE(lv1, lv2)	__TMS_UINT128_LE(lv1, lv2)
# define INT128_LE(lv1, lv2)	__TMS_INT128_LE(lv1, lv2)
# define UINT128_NE(l1, l2)	__TMS_UINT128_NE(l1, l2)
# define INT128_NE(l1, l2)	__TMS_INT128_NE(l1, l2)
# define UINT128_GT(l1, l2)	__TMS_UINT128_GT(l1, l2)
# define INT128_GT(l1, l2)	__TMS_INT128_GT(l1, l2)
# define UINT128_GE(l1, l2)	__TMS_UINT128_GE(l1, l2)
# define INT128_GE(l1, l2)	__TMS_INT128_GE(l1, l2)
# define UINT128_ADD3(lres, l1, l2)	__TMS_UINT128_ADD3(lres, l1, l2)
# define INT128_ADD3(lres, l1, l2)	__TMS_INT128_ADD3(lres, l1, l2)
# define UINT128_SUB3(lres, l1, l2)	__TMS_UINT128_SUB3(lres, l1, l2)
# define INT128_SUB3(lres, l1, l2)	__TMS_INT128_SUB3(lres, l1, l2)
# define UINT128_SET(lv, rv)	__TMS_UINT128_SET(lv, rv)
# define INT128_SET(lv, rv)	__TMS_INT128_SET(lv, rv)
# define UINT128_ZERO(lv)	__TMS_UINT128_ZERO(lv)
# define INT128_ZERO(lv)	__TMS_INT128_ZERO(lv)
# define UINT128_NOT2(lv, rv)	__TMS_UINT128_NOT2(lv, rv)
# define UINT128_NOT1(lv)	__TMS_UINT128_NOT1(lv)
# define UINT128_NEG1(lv)	__TMS_UINT128_NEG1(lv)
# define INT128_NEG1(lv)	__TMS_INT128_NEG1(lv)
# define UINT128_AND3(lv, rv1, rv2)	__TMS_UINT128_AND3(lv, rv1, rv2)
# define UINT128_AND3PN(lv, rv1, rv2)	__TMS_UINT128_AND3PN(lv, rv1, rv2)
# define UINT128_AND3NP(lv, rv1, rv2)	__TMS_UINT128_AND3NP(lv, rv1, rv2)
# define UINT128_OR3(lv, rv1, rv2)	__TMS_UINT128_OR3(lv, rv1, rv2)
# define UINT128_OR3PN(lv, rv1, rv2)	__TMS_UINT128_OR3PN(lv, rv1, rv2)
# define UINT128_OR3NP(lv, rv1, rv2)	__TMS_UINT128_OR3NP(lv, rv1, rv2)
# define UINT128_XOR3(lv, rv1, rv2)	__TMS_UINT128_XOR3(lv, rv1, rv2)
# define UINT128_NAND3(lv, rv1, rv2)	__TMS_UINT128_NAND3(lv, rv1, rv2)
# define UINT128_NOR3(lv, rv1, rv2)	__TMS_UINT128_NOR3(lv, rv1, rv2)
# define UINT128_XNOR3(lv, rv1, rv2)	__TMS_UINT128_XNOR3(lv, rv1, rv2)
# define UINT128_LSR2(lv, n)	__TMS_UINT128_LSR2(lv, n)
# define UINT128_LSL2(lv, n)	__TMS_UINT128_LSL2(lv, n)
# define UINT64toUINT128(lv, ullong_v)	__TMS_UINT64toUINT128(lv, ullong_v)
# define INT64toINT128(lv, llong_v)	__TMS_INT64toINT128(lv, llong_v)
# define UINT128toUINT64(lv)	__TMS_UINT128toUINT64(lv)
# define INT128toINT64(lv)	__TMS_INT128toINT64(lv)
# define UINT128_MULU64_3(lv, a, b)	__TMS_UINT128_MULU64_3(lv, a, b)
# define UINT128_MUL3(lv, rv1, rv2)	__TMS_UINT128_MUL3(lv, rv1, rv2)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of libport_128bit_help.h ****/
#endif /* _LIBPORT_128BIT_HELP_H_ */
