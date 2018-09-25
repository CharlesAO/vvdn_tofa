/*
 *  $Id: _default_types.h,v 1.6 2013/12/03 16:04:41 corinna Exp $
 */

#ifndef _MACHINE__DEFAULT_TYPES_H
#define _MACHINE__DEFAULT_TYPES_H

#include <sys/features.h>

/*
 * Guess on types by examining *_MIN / *_MAX defines.
 */
#if __GNUC_PREREQ (3, 3)
/* GCC >= 3.3.0 has __<val>__ implicitly defined. */
#define __EXP(x) __##x##__
#else
/* Fall back to POSIX versions from <limits.h> */
#define __EXP(x) x
#include <limits.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef signed char __int8_t ;
typedef unsigned char __uint8_t ;
#define ___int8_t_defined 1

typedef signed short __int16_t;
typedef unsigned short __uint16_t;
#define ___int16_t_defined 1

typedef signed int __int32_t;
typedef unsigned int __uint32_t;
#define ___int32_t_defined 1

typedef signed long long __int64_t;
typedef unsigned long long __uint64_t;
#define ___int64_t_defined 1


typedef __int8_t __int_least8_t;
typedef __uint8_t __uint_least8_t;
#define ___int_least8_t_defined 1

typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;
#define ___int_least16_t_defined 1

typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
#define ___int_least32_t_defined 1

typedef __int64_t __int_least64_t;
typedef __uint64_t __uint_least64_t;
#define ___int_least64_t_defined 1


typedef __int32_t __intptr_t;
typedef __uint32_t __uintptr_t;

typedef __int64_t __intmax_t;
typedef __uint64_t __uintmax_t;


#undef __EXP

#ifdef __cplusplus
}
#endif

#endif /* _MACHINE__DEFAULT_TYPES_H */
