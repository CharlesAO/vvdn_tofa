/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2016-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   math.h
 *  Description:   Lightweight implementation of the '<math.h>' header for the
 *                 SHAVE library 'mlibm.a'.  This is mostly compliant to ISO C99,
 *                 but has been extended with additional support for the SHAVE
 *                 implementation.
 *  --------------------------------------------------------------------------- */

#ifndef _MCC_MATH_H_
#define _MCC_MATH_H_ (1)


#ifndef __shave__
# error "math.h: this header is for the SHAVE processor!"
#endif

#pragma GCC system_header

#include <sys/__moviconfig.h>


_MV_EXTERNC_BEGIN


/* Define the ISO C99 <math.h> types: */
#ifdef FLT_EVAL_METHOD
# define _MV_FLT_EVAL_METHOD FLT_EVAL_METHOD
#else
# define _MV_FLT_EVAL_METHOD __FLT_EVAL_METHOD__
#endif

#if _MV_FLT_EVAL_METHOD == 1
typedef double      float_t;
typedef double      double_t;
#elif _MV_FLT_EVAL_METHOD == 2
typedef long double float_t;
typedef long double double_t;
#else
typedef float       float_t;
typedef double      double_t;
#endif

#undef _MV_FLT_EVAL_METHOD


/* Define the ISO C99 <math.h> macro constants using CLang's builtins when possible: */
#define _HUGE_VALH   ((__half_t)HUGE_VALF)
#define  HUGE_VALF   (__builtin_huge_valf())
#define  HUGE_VAL    (__builtin_huge_val())
#define  HUGE_VALL   (__builtin_huge_vall())

#define INFINITY     (__builtin_inff())
#define NAN          (__builtin_nanf(""))

#define FP_INFINITE  (1)
#define FP_NAN       (0)
#define FP_NORMAL    (4)
#define FP_SUBNORMAL (3)
#define FP_ZERO      (2)

/* #define FP_FAST_FMAF */
/* #define FP_FAST_FMA */

#define FP_ILOGB0    (-__INT_MAX__)
#define FP_ILOGBNAN  (__INT_MAX__)

#define MATH_ERROR       (1)
#define MATH_ERREXCEPT   (2)
#define math_errhandling MATH_ERRNO


/* 7.12.3 - Classification macros: */
#define fpclassify(x) \
    ((int)((sizeof(x) == sizeof(long double)) ? __fpclassifyl(x) : \
          ((sizeof(x) == sizeof(double))      ? __fpclassifyd(x) : \
          ((sizeof(x) == sizeof(float))       ? __fpclassifyf(x) : \
          ((sizeof(x) == sizeof(__fp16))      ? __fpclassifys(x) : \
                                                __fpclassifyd(x))))))

#define isfinite(x) ((int)(__extension__({int tmp = fpclassify(x); ((tmp != FP_INFINITE) && (tmp != FP_NAN));})))
#define isinf(x)    ((int)(fpclassify(x) == FP_INFINITE))
#define isnan(x)    ((int)(fpclassify(x) == FP_NAN))
#define isnormal(x) ((int)(fpclassify(x) == FP_NORMAL))

#define signbit(x) \
    ((int)((sizeof(x) == sizeof(long double)) ? __signbitl(x) : \
          ((sizeof(x) == sizeof(double))      ? __signbitd(x) : \
          ((sizeof(x) == sizeof(float))       ? __signbitf(x) : \
          ((sizeof(x) == sizeof(__fp16))      ? __signbits(x) : \
                                                __signbitd(x))))))

/* Support functions for the above macros: */
extern               int __fpclassifys(__half_t);
extern               int __fpclassifyf(float);
extern               int __fpclassifyd(double);
extern               int __fpclassifyl(long double);

#ifndef __SOFTFP__
_MV_PURE_INLINE_DEFN int __signbits(__half_t x) { return (_MV_FP16_AS_UINT16(x) >> 15); }
#else /* __SOFTFP__ */
_MV_PURE_INLINE_DEFN int __signbits(__half_t x) { return __builtin_shave_iau_fextu_rii(_MV_FP32_AS_UINT32(x), 31, 1); }
#endif /* __SOFTFP__ */
_MV_PURE_INLINE_DEFN int __signbitf(float x)    { return __builtin_shave_iau_fextu_rii(_MV_FP32_AS_UINT32(x), 31, 1); }
_MV_PURE_INLINE_DEFN int __signbitd(double x)   { return __signbitf((float)x); }
extern               int __signbitl(long double);


/* 7.12.4 - Trigonometric functions: */
_MV_CONST_DECL        __half_t   __acoss(__half_t);
_MV_CONST_DECL        float        acosf(float);
_MV_CONST_DECL        double       acos (double);
_MV_CONST_DECL        long double  acosl(long double);

_MV_CONST_DECL        __half_t   __asins(__half_t);
_MV_CONST_DECL        float        asinf(float);
_MV_CONST_DECL        double       asin (double);
_MV_CONST_DECL        long double  asinl(long double);

_MV_CONST_DECL        __half_t   __atans(__half_t);
_MV_CONST_DECL        float        atanf(float);
_MV_CONST_DECL        double       atan (double);
_MV_CONST_DECL        long double  atanl(long double);

_MV_CONST_DECL        __half_t   __atan2s(__half_t, __half_t);
_MV_CONST_DECL        float        atan2f(float, float);
_MV_CONST_DECL        double       atan2 (double, double);
_MV_CONST_DECL        long double  atan2l(long double, long double);

_MV_CONST_DECL        __half_t   __coss(__half_t);
_MV_CONST_DECL        float        cosf(float);
_MV_CONST_DECL        double       cos (double);
_MV_CONST_DECL        long double  cosl(long double);

_MV_CONST_DECL        __half_t   __sins(__half_t);
_MV_CONST_DECL        float        sinf(float);
_MV_CONST_DECL        double       sin (double);
_MV_CONST_DECL        long double  sinl(long double);

_MV_CONST_DECL        __half_t   __tans(__half_t);
_MV_CONST_DECL        float        tanf(float);
_MV_CONST_DECL        double       tan (double);
_MV_CONST_DECL        long double  tanl(long double);

/* 7.12.5 - Hyperbolic functions: */
_MV_CONST_DECL        __half_t   __acoshs(__half_t);
_MV_CONST_DECL        float        acoshf(float);
_MV_CONST_DECL        double       acosh (double);
_MV_CONST_DECL        long double  acoshl(long double);

_MV_CONST_DECL        __half_t   __asinhs(__half_t);
_MV_CONST_DECL        float        asinhf(float);
_MV_CONST_DECL        double       asinh (double);
_MV_CONST_DECL        long double  asinhl(long double);

_MV_CONST_DECL        __half_t   __atanhs(__half_t);
_MV_CONST_DECL        float        atanhf(float);
_MV_CONST_DECL        double       atanh (double);
_MV_CONST_DECL        long double  atanhl(long double);

_MV_CONST_DECL        __half_t   __coshs(__half_t);
_MV_CONST_DECL        float        coshf(float);
_MV_CONST_DECL        double       cosh (double);
_MV_CONST_DECL        long double  coshl(long double);

_MV_CONST_DECL        __half_t   __sinhs(__half_t);
_MV_CONST_DECL        float        sinhf(float);
_MV_CONST_DECL        double       sinh (double);
_MV_CONST_DECL        long double  sinhl(long double);

_MV_CONST_DECL        __half_t   __tanhs(__half_t);
_MV_CONST_DECL        float        tanhf(float);
_MV_CONST_DECL        double       tanh (double);
_MV_CONST_DECL        long double  tanhl(long double);

/* 7.12.6 - Exponential and logarithmic functions: */
_MV_CONST_DECL        __half_t   __exps(__half_t);
_MV_CONST_DECL        float        expf(float);
_MV_CONST_DECL        double       exp (double);
_MV_CONST_DECL        long double  expl(long double);

_MV_CONST_DECL        __half_t   __exp2s(__half_t);
_MV_CONST_DECL        float        exp2f(float);
_MV_CONST_DECL        double       exp2 (double);
_MV_CONST_DECL        long double  exp2l(long double);

_MV_CONST_DECL        __half_t   __expm1s(__half_t);
_MV_CONST_DECL        float        expm1f(float);
_MV_CONST_DECL        double       expm1 (double);
_MV_CONST_DECL        long double  expm1l(long double);

_MV_EXTERN_DECL       __half_t   __frexps(__half_t, int*);
_MV_EXTERN_DECL       float        frexpf(float, int*);
_MV_EXTERN_DECL       double       frexp (double, int*);
_MV_EXTERN_DECL       long double  frexpl(long double, int*);

_MV_CONST_DECL        int        __ilogbs(__half_t);
_MV_CONST_DECL        int          ilogbf(float);
_MV_CONST_DECL        int          ilogb (double);
_MV_CONST_DECL        int          ilogbl(long double);

_MV_CONST_DECL        __half_t   __ldexps(__half_t, int);
_MV_CONST_DECL        float        ldexpf(float, int);
_MV_CONST_DECL        double       ldexp (double, int);
_MV_CONST_DECL        long double  ldexpl(long double, int);

_MV_CONST_DECL        __half_t   __logs(__half_t);
_MV_CONST_DECL        float        logf(float);
_MV_CONST_DECL        double       log (double);
_MV_CONST_DECL        long double  logl(long double);

_MV_CONST_DECL        __half_t   __log10s(__half_t);
_MV_CONST_DECL        float        log10f(float);
_MV_CONST_DECL        double       log10 (double);
_MV_CONST_DECL        long double  log10l(long double);

_MV_CONST_DECL        __half_t   __log1ps(__half_t);
_MV_CONST_DECL        float        log1pf(float);
_MV_CONST_DECL        double       log1p (double);
_MV_CONST_DECL        long double  log1pl(long double);

#ifndef __SOFTFP__
_MV_PURE_INLINE_DEFN  __half_t   __log2s(__half_t x) { return __builtin_shave_sau_log2_f16_l_r(x); }
#else /* __SOFTFP__ */
_MV_CONST_DECL        __half_t   __log2s(__half_t);
#endif /* __SOFTFP__ */
_MV_CONST_DECL        float        log2f(float);
_MV_CONST_DECL        double       log2 (double);
_MV_CONST_DECL        long double  log2l(long double);

_MV_CONST_DECL        __half_t   __logbs(__half_t);
_MV_CONST_DECL        float        logbf(float);
_MV_CONST_DECL        double       logb (double);
_MV_CONST_DECL        long double  logbl(long double);

_MV_EXTERN_DECL       __half_t   __modfs(__half_t, __half_t*);
_MV_EXTERN_DECL       float        modff(float, float*);
_MV_EXTERN_DECL       double       modf (double, double*);
_MV_EXTERN_DECL       long double  modfl(long double, long double*);

_MV_CONST_DECL        __half_t   __scalbns(__half_t, int);
_MV_CONST_DECL        float        scalbnf(float, int);
_MV_CONST_DECL        double       scalbn (double, int);
_MV_CONST_DECL        long double  scalbnl(long double, int);

_MV_CONST_DECL        __half_t   __scalblns(__half_t, long);
_MV_CONST_DECL        float        scalblnf(float, long);
_MV_CONST_DECL        double       scalbln (double, long);
_MV_CONST_DECL        long double  scalblnl(long double, long);

/* 7.12.7 - Power and absolute-value functions: */
_MV_CONST_DECL        __half_t   __cbrts(__half_t);
_MV_CONST_DECL        float        cbrtf(float);
_MV_CONST_DECL        double       cbrt (double);
_MV_CONST_DECL        long double  cbrtl(long double);

#ifndef __SOFTFP__
_MV_PURE_INLINE_DEFN  __half_t   __fabss(__half_t x) { return _MV_UINT16_AS_FP16(__builtin_shave_iau_fins_rrii(_MV_FP16_AS_UINT16(x), 0x0, 15, 1)); }
#else /* __SOFTFP__ */
_MV_CONST_DECL        __half_t   __fabss(__half_t);
#endif /* __SOFTFP__ */
_MV_CONST_DECL        float        fabsf(float);
_MV_CONST_DECL        double       fabs (double);
_MV_CONST_DECL        long double  fabsl(long double);

_MV_CONST_DECL        __half_t   __hypots(__half_t, __half_t);
_MV_CONST_DECL        float        hypotf(float, float);
_MV_CONST_DECL        double       hypot (double, double);
_MV_CONST_DECL        long double  hypotl(long double, long double);

_MV_CONST_DECL        __half_t   __pows(__half_t, __half_t);
_MV_CONST_DECL        float        powf(float, float);
_MV_CONST_DECL        double       pow (double, double);
_MV_CONST_DECL        long double  powl(long double, long double);

#ifndef __SOFTFP__
_MV_PURE_INLINE_DEFN  __half_t   __sqrts(__half_t x) { return __builtin_shave_sau_sqt_f16_l_r(x); }
#else /* __SOFTFP__ */
_MV_CONST_DECL        __half_t   __sqrts(__half_t);
#endif /* __SOFTFP__ */
_MV_CONST_DECL        float        sqrtf(float);
_MV_CONST_DECL        double       sqrt (double);
_MV_CONST_DECL        long double  sqrtl(long double);

/* 7.12.8 - Error and gamma functions: */
_MV_CONST_DECL        __half_t   __erfs(__half_t);
_MV_CONST_DECL        float        erff(float);
_MV_CONST_DECL        double       erf (double);
_MV_CONST_DECL        long double  erfl(long double);

_MV_CONST_DECL        __half_t   __erfcs(__half_t);
_MV_CONST_DECL        float        erfcf(float);
_MV_CONST_DECL        double       erfc (double);
_MV_CONST_DECL        long double  erfcl(long double);

_MV_CONST_DECL        __half_t   __lgammas(__half_t);
_MV_CONST_DECL        float        lgammaf(float);
_MV_CONST_DECL        double       lgamma (double);
_MV_CONST_DECL        long double  lgammal(long double);

_MV_CONST_DECL        __half_t   __tgammas(__half_t);
_MV_CONST_DECL        float        tgammaf(float);
_MV_CONST_DECL        double       tgamma (double);
_MV_CONST_DECL        long double  tgammal(long double);

/* 7.12.9 - Nearest integer functions: */
_MV_CONST_DECL        __half_t   __ceils(__half_t);
_MV_CONST_DECL        float        ceilf(float);
_MV_CONST_DECL        double       ceil (double);
_MV_CONST_DECL        long double  ceill(long double);

_MV_CONST_DECL        __half_t   __floors(__half_t);
_MV_CONST_DECL        float        floorf(float);
_MV_CONST_DECL        double       floor (double);
_MV_CONST_DECL        long double  floorl(long double);

_MV_CONST_DECL        __half_t   __nearbyints(__half_t);
_MV_CONST_DECL        float        nearbyintf(float);
_MV_CONST_DECL        double       nearbyint (double);
_MV_CONST_DECL        long double  nearbyintl(long double);

#ifndef __SOFTFP__
_MV_PURE_INLINE_DEFN  __half_t   __rints(__half_t x) { return  __nearbyints(x); }
#else /* __SOFTFP__ */
_MV_CONST_DECL        __half_t   __rints(__half_t);
#endif /* __SOFTFP__ */
_MV_CONST_DECL        float        rintf(float);
_MV_CONST_DECL        double       rint (double);
_MV_CONST_DECL        long double  rintl(long double);

#ifndef __SOFTFP__
_MV_PURE_INLINE_DEFN  long       __lrints(__half_t x) { return (long) __rints(x); }
#else /* __SOFTFP__ */
_MV_CONST_DECL        long       __lrints(__half_t);
#endif /* __SOFTFP__ */
_MV_CONST_DECL        long         lrintf(float);
_MV_CONST_DECL        long         lrint (double);
_MV_CONST_DECL        long         lrintl(long double);

#ifndef __SOFTFP__
_MV_PURE_INLINE_DEFN  long long  __llrints(__half_t x) { return (long long) __rints(x); }
#else /* __SOFTFP__ */
_MV_CONST_DECL        long long  __llrints(__half_t);
#endif /* __SOFTFP__ */
_MV_CONST_DECL        long long    llrintf(float);
_MV_CONST_DECL        long long    llrint (double);
_MV_CONST_DECL        long long    llrintl(long double);

_MV_CONST_DECL        __half_t   __rounds(__half_t);
_MV_CONST_DECL        float        roundf(float);
_MV_CONST_DECL        double       round (double);
_MV_CONST_DECL        long double  roundl(long double);

#ifndef __SOFTFP__
_MV_PURE_INLINE_DEFN  long       __lrounds(__half_t x) { return (long) __rounds(x); }
#else /* __SOFTFP__ */
_MV_CONST_DECL        long       __lrounds(__half_t);
#endif /* __SOFTFP__ */
_MV_CONST_DECL        long         lroundf(float);
_MV_CONST_DECL        long         lround (double);
_MV_CONST_DECL        long         lroundl(long double);

#ifndef __SOFTFP__
_MV_PURE_INLINE_DEFN  long long  __llrounds(__half_t x) { return (long long) __rounds(x); }
#else /* __SOFTFP__ */
_MV_CONST_DECL        long long  __llrounds(__half_t);
#endif /* __SOFTFP__ */
_MV_CONST_DECL        long long    llroundf(float);
_MV_CONST_DECL        long long    llround (double);
_MV_CONST_DECL        long long    llroundl(long double);

_MV_CONST_DECL        __half_t   __truncs(__half_t);
_MV_CONST_DECL        float        truncf(float);
_MV_CONST_DECL        double       trunc (double);
_MV_CONST_DECL        long double  truncl(long double);

/* 7.12.10 - Remainder functions: */
_MV_CONST_DECL        __half_t   __fmods(__half_t, __half_t);
_MV_CONST_DECL        float        fmodf(float, float);
_MV_CONST_DECL        double       fmod (double, double);
_MV_CONST_DECL        long double  fmodl(long double, long double);

_MV_CONST_DECL        __half_t   __remainders(__half_t, __half_t);
_MV_CONST_DECL        float        remainderf(float, float);
_MV_CONST_DECL        double       remainder (double, double);
_MV_CONST_DECL        long double  remainderl(long double, long double);

_MV_EXTERN_DECL       __half_t   __remquos(__half_t, __half_t, int*);
_MV_EXTERN_DECL       float        remquof(float, float, int*);
_MV_EXTERN_DECL       double       remquo (double, double, int*);
_MV_EXTERN_DECL       long double  remquol(long double, long double, int*);

/* 7.12.11 - Manipulation functions: */
_MV_CONST_DECL        __half_t   __copysigns(__half_t, __half_t);
_MV_CONST_DECL        float        copysignf(float, float);
_MV_CONST_DECL        double       copysign (double, double);
_MV_CONST_DECL        long double  copysignl(long double, long double);

#ifndef __SOFTFP__
_MV_INLINE_DEFN       __half_t   __nans(const char* _MV_UNUSED(str)) { return _MV_UINT16_AS_FP16(0x7E00); }
#else /* __SOFTFP__ */
_MV_EXTERN_DECL       __half_t   __nans(const char*);
#endif /* __SOFTFP__ */
_MV_EXTERN_DECL       float        nanf(const char*);
_MV_EXTERN_DECL       double       nan (const char*);
_MV_EXTERN_DECL       long double  nanl(const char*);

_MV_CONST_DECL        __half_t   __nextafters(__half_t, __half_t);
_MV_CONST_DECL        float        nextafterf(float, float);
_MV_CONST_DECL        double       nextafter (double, double);
_MV_CONST_DECL        long double  nextafterl(long double, long double);

_MV_CONST_DECL        __half_t   __nexttowards(__half_t, long double);
_MV_CONST_DECL        float        nexttowardf(float, long double);
_MV_CONST_DECL        double       nexttoward (double, long double);
_MV_CONST_DECL        long double  nexttowardl(long double, long double);

/* 7.12.12 - Maximum, minimum, and positive difference functions: */
_MV_CONST_DECL        __half_t   __fdims(__half_t, __half_t);
_MV_CONST_DECL        float        fdimf(float, float);
_MV_CONST_DECL        double       fdim (double, double);
_MV_CONST_DECL        long double  fdiml(long double, long double);

_MV_CONST_DECL        __half_t   __fmaxs(__half_t, __half_t);
_MV_CONST_DECL        float        fmaxf(float, float);
_MV_CONST_DECL        double       fmax (double, double);
_MV_CONST_DECL        long double  fmaxl(long double, long double);

_MV_CONST_DECL        __half_t   __fmins(__half_t, __half_t);
_MV_CONST_DECL        float        fminf(float, float);
_MV_CONST_DECL        double       fmin (double, double);
_MV_CONST_DECL        long double  fminl(long double, long double);

/* 7.12.13 - Floating multiply-add: */
_MV_CONST_DECL        __half_t   __fmas(__half_t, __half_t, __half_t);
_MV_CONST_DECL        float        fmaf(float, float, float);
_MV_CONST_DECL        double       fma (double, double, double);
_MV_CONST_DECL        long double  fmal(long double, long double, long double);

/* 7.12.14 - Comparison macros: */
#define isgreater(x,y)      ((int)(__extension__({__typeof__(x) tmpx = (x); __typeof__(y) tmpy = (y); !isunordered(tmpx, tmpy) && (tmpx >  tmpy);})))
#define isgreaterequal(x,y) ((int)(__extension__({__typeof__(x) tmpx = (x); __typeof__(y) tmpy = (y); !isunordered(tmpx, tmpy) && (tmpx >= tmpy);})))
#define isless(x,y)         ((int)(__extension__({__typeof__(x) tmpx = (x); __typeof__(y) tmpy = (y); !isunordered(tmpx, tmpy) && (tmpx <  tmpy);})))
#define islessequal(x,y)    ((int)(__extension__({__typeof__(x) tmpx = (x); __typeof__(y) tmpy = (y); !isunordered(tmpx, tmpy) && (tmpx <= tmpy);})))
#define islessgreater(x,y)  ((int)(__extension__({__typeof__(x) tmpx = (x); __typeof__(y) tmpy = (y); !isunordered(tmpx, tmpy) && ((tmpx < tmpy) || (tmpx > tmpy));})))

#define isunordered(x,y)    ((int)(isnan(x) || isnan(y)))


/* The following are extensions that are unique to the SHAVE implementation: */
#ifndef __SOFTFP__
_MV_PURE_INLINE_DEFN  __half_t    __clamps(__half_t x, __half_t lo, __half_t hi) { return __builtin_shave_cmu_clampab_f16_rrr_half(x, lo, hi); }
_MV_PURE_INLINE_DEFN  float       __clampf(float x, float lo, float hi)    { return __builtin_shave_cmu_clampab_f32_rrr_float(x, lo, hi); }
#else /* __SOFTFP__ */
_MV_CONST_DECL        __half_t    __clamps(__half_t, __half_t, __half_t);
_MV_CONST_DECL        float       __clampf(float, float, float);
#endif /* __SOFTFP__ */
_MV_PURE_INLINE_DEFN  double      __clamp (double x, double lo, double hi) { return __clampf((float)x, (float)lo, (float)hi); }
_MV_CONST_DECL        long double __clampl(long double, long double, long double);

_MV_CONST_DECL        __half_t    __powrs(__half_t, __half_t);
_MV_PURE_DECL         float       __powrf(float, float);
_MV_CONST_DECL        double      __powr (double, double);
_MV_CONST_DECL        long double __powrl(long double, long double);


/* Provide more convenient and symmetric names for the FP16 versions of the math functions */
#ifndef __STRICT_ANSI__
#define acoss(__a)               __acoss(__a)
#define asins(__a)               __asins(__a)
#define atans(__a)               __atans(__a)
#define atan2s(__a, __b)         __atan2s(__a, __b)
#define coss(__a)                __coss(__a)
#define sins(__a)                __sins(__a)
#define tans(__a)                __tans(__a)
#define acoshs(__a)              __acoshs(__a)
#define asinhs(__a)              __asinhs(__a)
#define atanhs(__a)              __atanhs(__a)
#define coshs(__a)               __coshs(__a)
#define sinhs(__a)               __sinhs(__a)
#define tanhs(__a)               __tanhs(__a)
#define exps(__a)                __exps(__a)
#define exp2s(__a)               __exp2s(__a)
#define expm1s(__a)              __expm1s(__a)
#define frexps(__a, __b)         __frexps(__a, __b)
#define ilogbs(__a)              __ilogbs(__a)
#define ldexps(__a, __b)         __ldexps(__a, __b)
#define logs(__a)                __logs(__a)
#define log10s(__a)              __log10s(__a)
#define log1ps(__a)              __log1ps(__a)
#define log2s(__a)               __log2s(__a)
#define logbs(__a)               __logbs(__a)
#define modfs(__a, __b)          __modfs(__a, __b)
#define scalbns(__a, __b)        __scalbns(__a, __b)
#define scalblns(__a, __b)       __scalblns(__a, __b)
#define cbrts(__a)               __cbrts(__a)
#define fabss(__a)               __fabss(__a)
#define hypots(__a, __b)         __hypots(__a, __b)
#define pows(__a, __b)           __pows(__a, __b)
#define sqrts(__a)               __sqrts(__a)
#define erfs(__a)                __erfs(__a)
#define erfcs(__a)               __erfcs(__a)
#define lgammas(__a)             __lgammas(__a)
#define tgammas(__a)             __tgammas(__a)
#define ceils(__a)               __ceils(__a)
#define floors(__a)              __floors(__a)
#define nearbyints(__a)          __nearbyints(__a)
#define rints(__a)               __rints(__a)
#define lrints(__a)              __lrints(__a)
#define llrints(__a)             __llrints(__a)
#define rounds(__a)              __rounds(__a)
#define lrounds(__a)             __lrounds(__a)
#define llrounds(__a)            __llrounds(__a)
#define truncs(__a)              __truncs(__a)
#define fmods(__a, __b)          __fmods(__a, __b)
#define remainders(__a, __b)     __remainders(__a, __b)
#define remquos(__a, __b, __c)   __remquos(__a, __b, __c)
#define copysigns(__a, __b)      __copysigns(__a, __b)
#define nans(__a)                __nans(__a)
#define nextafters(__a, __b)     __nextafters(__a, __b)
#define nexttowards(__a, __b)    __nexttowards(__a, __b)
#define fdims(__a, __b)          __fdims(__a, __b)
#define fmaxs(__a, __b)          __fmaxs(__a, __b)
#define fmins(__a, __b)          __fmins(__a, __b)
#define fmas(__a, __b, __c)      __fmas(__a, __b, __c)
#define clamps(__a, __b, __c)    __clamps(__a, __b, __c)
#define powrs(__a, __b)          __powrs(__a, __b)
#endif /* __STRICT_ANSI__ */


_MV_EXTERNC_END


#endif /* _MCC_MATH_H_ */
