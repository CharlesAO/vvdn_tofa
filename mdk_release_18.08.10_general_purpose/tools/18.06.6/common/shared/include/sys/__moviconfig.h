/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2016-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys/__moviconfig.h
 *  Description:   Set of utility definitions and macros used in the implementation
 *                 of the SHAVE libraries.
 *  --------------------------------------------------------------------------- */

#ifndef _MCC_SYS_MOVICONFIG_H_
#define _MCC_SYS_MOVICONFIG_H_ (1)

#pragma GCC system_header

#ifdef __shave__
/* The 'half' data type: */
#ifdef __SOFTFP__
typedef double __half_t;
#else /* __SOFTFP__ */
typedef __fp16 __half_t;
#endif /* __SOFTFP__ */

/* Conversion utilities: */
# define _MV_UINT64_AS_FP64(x)      (__builtin_astype((unsigned long long)(x), long double))
# define _MV_UINT32_AS_FP32(x)      (__builtin_astype((unsigned int)(x), float))
# define _MV_UINT16_AS_FP16(x)      (__builtin_astype((unsigned short)(x), __half_t))
# define _MV_FP64_AS_UINT64(x)      (__builtin_astype((long double)(x), unsigned long long))
# define _MV_FP32_AS_UINT32(x)      (__builtin_astype((float)(x), unsigned int))
# define _MV_FP16_AS_UINT16(x)      (__builtin_astype((__half_t)(x), unsigned short))
#elif defined(__leon__)
# define _MV_UINT64_AS_FP64(x)      (__mv_as_f64((unsigned long long)(x)))
# define _MV_UINT32_AS_FP32(x)      (__mv_as_f32((unsigned int)(x)))
# define _MV_FP64_AS_UINT64(x)      (__mv_as_u64((double)(x)))
# define _MV_FP32_AS_UINT32(x)      (__mv_as_u32((float)(x)))
#endif /* __leon__ */

/* Declaration forms: */
#define _MV_EXTERN_DECL             extern
#define _MV_CONST_DECL              extern __attribute__((const))
#define _MV_PURE_DECL               extern __attribute__((__pure__))
#define _MV_NORETURN_DECL           extern __attribute__((__noreturn__))

#define _MV_ENTRYPOINT_DEFN         __attribute__((__dllexport__,__no_instrument_function__))
#define _MV_NORETURN_DEFN           __attribute__((__noreturn__,__no_instrument_function__))

#ifdef __cplusplus
# define _MV_EXTERNC_DECL           extern "C"
# define _MV_OVERLOAD_DECL          extern

# define _MV_INLINE_DEFN            inline __attribute__((__always_inline__,__no_instrument_function__))
# define _MV_CONST_INLINE_DEFN      inline __attribute__((const, __always_inline__,__no_instrument_function__))
# define _MV_PURE_INLINE_DEFN       inline __attribute__((__pure__, __always_inline__,__no_instrument_function__))
# define _MV_OVERLOAD_INLINE_DEFN   inline __attribute__((__always_inline__,__no_instrument_function__))

# define _MV_EXTERNC_BEGIN          extern "C" {
# define _MV_EXTERNC_END            }

# define _MV_UNUSED(x)
#else /* __cplusplus */
# define _MV_EXTERNC_DECL           extern
# define _MV_OVERLOAD_DECL          extern __attribute__((__overloadable__))

# define _MV_INLINE_DEFN            __inline __attribute__((__always_inline__,__no_instrument_function__))
# define _MV_CONST_INLINE_DEFN      __inline __attribute__((const, __always_inline__,__no_instrument_function__))
# define _MV_PURE_INLINE_DEFN       __inline __attribute__((__pure__, __always_inline__,__no_instrument_function__))
# define _MV_OVERLOAD_INLINE_DEFN   __inline __attribute__((__always_inline__, __overloadable__,__no_instrument_function__))

# define _MV_EXTERNC_BEGIN
# define _MV_EXTERNC_END

# define _MV_UNUSED(x)              x __attribute__((__unused__))
#endif /* __cplusplus */


#ifdef __leon__
/* GCC on Leon does not have '__builtin_astype', so use a 'union' to achieve the intended purpose */
_MV_EXTERNC_BEGIN

_MV_INLINE_DEFN double              __mv_as_f64(unsigned long long __x) { union { unsigned long long __s; double __d; } __q; __q.__s = __x; return __q.__d; }
_MV_INLINE_DEFN float               __mv_as_f32(unsigned int __x)       { union { unsigned __s; float __d; } __q; __q.__s = __x; return __q.__d; }
_MV_INLINE_DEFN unsigned long long  __mv_as_u64(double __x)             { union { double __s; unsigned long long __d; } __q; __q.__s = __x; return __q.__d; }
_MV_INLINE_DEFN unsigned            __mv_as_u32(float __x)              { union { float __s; unsigned __d; } __q; __q.__s = __x; return __q.__d; }

_MV_EXTERNC_END
#endif /* __leon__ */


#endif /* _MCC_SYS_MOVICONFIG_H_ */
