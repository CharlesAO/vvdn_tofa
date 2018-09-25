/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2015-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   moviNeonBindings.h
 *  Description:   Much of the functionality provided by the ARM(R) NEON(TM) extensions are already
 *                 optimally provided by the Movidius Vector Utility library, so this is reused
 *                 where appropriate to implement the ARM NEON instruction intrinsics.  Also, the
 *                 compiler already knows how to optimise many of the arithmetic operations using
 *                 their natural infix form, so this approach is used whenever possible.
 *
 *                 In addition, the SHAVE ISA built-in intrinsics can access functionality not
 *                 directly available any other way, so these are used when necessary.
 *
 *                 The ARM specification for these instructions is at:
 *                     http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0472k/chr1359125040250.html
 *
 *                 The GCC specification for these instructions is at:
 *                     http://gcc.gnu.org/onlinedocs/gcc-4.3.3/gcc/ARM-NEON-Intrinsics.html
 *
 *  ---------------------------------------------------------------------------
 *  NOTE:          ARM is a registered trademark of ARM Limited (or its subsidiaries) in the EU and/or elsewhere.
 *                 NEON is a trademark of ARM Limited (or its subsidiaries) in the EU and/or elsewhere.
 *                 All rights reserved.
 *  --------------------------------------------------------------------------- */

#ifndef _MOVINEONBINDINGS_H_
#define _MOVINEONBINDINGS_H_ (1)

#ifndef __MOVICOMPILE__
# error "moviNeonBindings.h: this header must be included only in a file compiled with moviCompile!"
#elif defined(__shavenn__)
# error "moviNeonBindings.h: is not supported for SHAVE NN"
#endif


#pragma GCC system_header

#include <limits.h>
#include <moviVectorUtils.h>
#include <stdint.h>
#include <sys/__moviconfig.h>


/* Since the ARM NEON supports 64-bit integers, and for SHAVE this requires 'long long' */
/* which in turn requires at least ISO C99 or ISO C++11 */
#if ((defined(__STDC_VERSION__) && (__STDC_VERSION__ < 199901L)) || (defined(__cplusplus) && (__cplusplus < 201103L)))
# error '<shave_arm_neon_extensions.h>' uses 'long long' which requires ISO C99 or ISO C++11 or newer
#endif /* Test for 'long long' support */


_MV_EXTERNC_BEGIN


/* Declare the ARM NEON vector types in terms of the Movidius Vector Utility types */
typedef int2          int32x2_t;
typedef int4          int32x4_t;
typedef uint2        uint32x2_t;
typedef uint4        uint32x4_t;

typedef short4        int16x4_t;
typedef short8        int16x8_t;
typedef ushort4      uint16x4_t;
typedef ushort8      uint16x8_t;

typedef schar8         int8x8_t;
typedef schar16       int8x16_t;
typedef uchar8        uint8x8_t;
typedef uchar16      uint8x16_t;

typedef float2      float32x2_t;
typedef float4      float32x4_t;

typedef half4       float16x4_t;
typedef half8       float16x8_t;
typedef half          float16_t;

typedef int64_t       int64x1_t __attribute__((ext_vector_type(1)));
typedef longlong2     int64x2_t;
typedef uint64_t     uint64x1_t __attribute__((ext_vector_type(1)));
typedef ulonglong2   uint64x2_t;

typedef float         float32_t;

typedef int           poly8x8_t; /* This is a dummy until we know what it needs to be */
typedef long         poly8x16_t; /* This is a dummy until we know what it needs to be */
typedef long         poly16x4_t; /* This is a dummy until we know what it needs to be */
typedef long         poly16x8_t; /* This is a dummy until we know what it needs to be */

/* 2 dimensional vector types */
typedef struct int8x8x2_t {
    int8x8_t val[2];
} int8x8x2_t;

typedef struct int8x8x3_t {
    int8x8_t val[3];
} int8x8x3_t;

typedef struct int8x8x4_t {
    int8x8_t val[4];
} int8x8x4_t;

typedef struct int8x16x2_t {
    int8x16_t val[2];
} int8x16x2_t;

typedef struct int8x16x3_t {
    int8x16_t val[3];
} int8x16x3_t;

typedef struct int8x16x4_t {
    int8x16_t val[4];
} int8x16x4_t;

typedef struct int16x4x2_t {
    int16x4_t val[2];
} int16x4x2_t;

typedef struct int16x4x3_t {
    int16x4_t val[3];
} int16x4x3_t;

typedef struct int16x4x4_t {
    int16x4_t val[4];
} int16x4x4_t;

typedef struct int16x8x2_t {
    int16x8_t val[2];
} int16x8x2_t;

typedef struct int16x8x3_t {
    int16x8_t val[3];
} int16x8x3_t;

typedef struct int16x8x4_t {
    int16x8_t val[4];
} int16x8x4_t;

typedef struct int32x2x2_t {
    int32x2_t val[2];
} int32x2x2_t;

typedef struct int32x2x3_t {
    int32x2_t val[3];
} int32x2x3_t;

typedef struct int32x2x4_t {
    int32x2_t val[4];
} int32x2x4_t;

typedef struct int32x4x2_t {
    int32x4_t val[2];
} int32x4x2_t;

typedef struct int32x4x3_t {
    int32x4_t val[3];
} int32x4x3_t;

typedef struct int32x4x4_t {
    int32x4_t val[4];
} int32x4x4_t;

typedef struct int64x1x2_t {
    int64x1_t val[2];
} int64x1x2_t;

typedef struct int64x1x3_t {
    int64x1_t val[3];
} int64x1x3_t;

typedef struct int64x1x4_t {
    int64x1_t val[4];
} int64x1x4_t;

typedef struct int64x2x2_t {
    int64x2_t val[2];
} int64x2x2_t;

typedef struct int64x2x3_t {
    int64x2_t val[3];
} int64x2x3_t;

typedef struct int64x2x4_t {
    int64x2_t val[4];
} int64x2x4_t;


typedef struct uint8x8x2_t {
    uint8x8_t val[2];
} uint8x8x2_t;

typedef struct uint8x8x3_t {
    uint8x8_t val[3];
} uint8x8x3_t;

typedef struct uint8x8x4_t {
    uint8x8_t val[4];
} uint8x8x4_t;

typedef struct uint8x16x2_t {
    uint8x16_t val[2];
} uint8x16x2_t;

typedef struct uint8x16x3_t {
    uint8x16_t val[3];
} uint8x16x3_t;

typedef struct uint8x16x4_t {
    uint8x16_t val[4];
} uint8x16x4_t;

typedef struct uint16x4x2_t {
    uint16x4_t val[2];
} uint16x4x2_t;

typedef struct uint16x4x3_t {
    uint16x4_t val[3];
} uint16x4x3_t;

typedef struct uint16x4x4_t {
    uint16x4_t val[4];
} uint16x4x4_t;

typedef struct uint16x8x2_t {
    uint16x8_t val[2];
} uint16x8x2_t;

typedef struct uint16x8x3_t {
    uint16x8_t val[3];
} uint16x8x3_t;

typedef struct uint16x8x4_t {
    uint16x8_t val[4];
} uint16x8x4_t;

typedef struct uint32x2x2_t {
    uint32x2_t val[2];
} uint32x2x2_t;

typedef struct uint32x2x3_t {
    uint32x2_t val[3];
} uint32x2x3_t;

typedef struct uint32x2x4_t {
    uint32x2_t val[4];
} uint32x2x4_t;

typedef struct uint32x4x2_t {
    uint32x4_t val[2];
} uint32x4x2_t;

typedef struct uint32x4x3_t {
    uint32x4_t val[3];
} uint32x4x3_t;

typedef struct uint32x4x4_t {
    uint32x4_t val[4];
} uint32x4x4_t;

typedef struct uint64x1x2_t {
    uint64x1_t val[2];
} uint64x1x2_t;

typedef struct uint64x1x3_t {
    uint64x1_t val[3];
} uint64x1x3_t;

typedef struct uint64x1x4_t {
    uint64x1_t val[4];
} uint64x1x4_t;

typedef struct uint64x2x2_t {
    uint64x2_t val[2];
} uint64x2x2_t;

typedef struct uint64x2x3_t {
    uint64x2_t val[3];
} uint64x2x3_t;

typedef struct uint64x2x4_t {
    uint64x2_t val[4];
} uint64x2x4_t;


typedef struct float16x4x2_t {
    float16x4_t val[2];
} float16x4x2_t;

typedef struct float16x4x3_t {
    float16x4_t val[3];
} float16x4x3_t;

typedef struct float16x4x4_t {
    float16x4_t val[4];
} float16x4x4_t;

typedef struct float16x8x2_t {
    float16x8_t val[2];
} float16x8x2_t;

typedef struct float16x8x3_t {
    float16x8_t val[3];
} float16x8x3_t;

typedef struct float16x8x4_t {
    float16x8_t val[4];
} float16x8x4_t;

typedef struct float32x2x2_t {
    float32x2_t val[2];
} float32x2x2_t;

typedef struct float32x2x3_t {
    float32x2_t val[3];
} float32x2x3_t;

typedef struct float32x2x4_t {
    float32x2_t val[4];
} float32x2x4_t;

typedef struct float32x4x2_t {
    float32x4_t val[2];
} float32x4x2_t;

typedef struct float32x4x3_t {
    float32x4_t val[3];
} float32x4x3_t;

typedef struct float32x4x4_t {
    float32x4_t val[4];
} float32x4x4_t;


#ifdef _HAVE_NEON_TYPE_POLY
typedef struct poly8x8x2_t {
    poly8x8_t val[2];
} poly8x8x2_t;

typedef struct poly8x8x3_t {
    poly8x8_t val[3];
} poly8x8x3_t;

typedef struct poly8x8x4_t {
    poly8x8_t val[4];
} poly8x8x4_t;

typedef struct poly8x16x2_t {
    poly8x16_t val[2];
} poly8x16x2_t;

typedef struct poly8x16x3_t {
    poly8x16_t val[3];
} poly8x16x3_t;

typedef struct poly8x16x4_t {
    poly8x16_t val[4];
} poly8x16x4_t;

typedef struct poly16x4x3_t {
    poly16x4_t val[3];
} poly16x4x3_t;

typedef struct poly16x4x2_t {
    poly16x4_t val[2];
} poly16x4x2_t;

typedef struct poly16x4x4_t {
    poly16x4_t val[4];
} poly16x4x4_t;

typedef struct poly16x8x2_t {
    poly16x8_t val[2];
} poly16x8x2_t;

typedef struct poly16x8x3_t {
    poly16x8_t val[3];
} poly16x8x3_t;

typedef struct poly16x8x4_t {
    poly16x8_t val[4];
} poly16x8x4_t;

#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.4 - NEON intrinsics for addition */
/* */

/* Vector addition: */
/*		vadd{q}_<type>.Vr[i] := Va[i] + Vb[i] */
_MV_INLINE_DEFN int8x8_t      vadd_s8 (int8x8_t a, int8x8_t b)        { return a + b; }
_MV_INLINE_DEFN int16x4_t     vadd_s16(int16x4_t a, int16x4_t b)      { return a + b; }
_MV_INLINE_DEFN int32x2_t     vadd_s32(int32x2_t a, int32x2_t b)      { return a + b; }
_MV_INLINE_DEFN int64x1_t     vadd_s64(int64x1_t a, int64x1_t b)      { return a + b; }
_MV_INLINE_DEFN float32x2_t   vadd_f32(float32x2_t a, float32x2_t b)  { return a + b; }
_MV_INLINE_DEFN uint8x8_t     vadd_u8 (uint8x8_t a, uint8x8_t b)      { return a + b; }
_MV_INLINE_DEFN uint16x4_t    vadd_u16(uint16x4_t a, uint16x4_t b)    { return a + b; }
_MV_INLINE_DEFN uint32x2_t    vadd_u32(uint32x2_t a, uint32x2_t b)    { return a + b; }
_MV_INLINE_DEFN uint64x1_t    vadd_u64(uint64x1_t a, uint64x1_t b)    { return a + b; }

_MV_INLINE_DEFN int8x16_t     vaddq_s8 (int8x16_t a, int8x16_t b)     { return a + b; }
_MV_INLINE_DEFN int16x8_t     vaddq_s16(int16x8_t a, int16x8_t b)     { return a + b; }
_MV_INLINE_DEFN int32x4_t     vaddq_s32(int32x4_t a, int32x4_t b)     { return a + b; }
_MV_INLINE_DEFN int64x2_t     vaddq_s64(int64x2_t a, int64x2_t b)     { return a + b; }
_MV_INLINE_DEFN float32x4_t   vaddq_f32(float32x4_t a, float32x4_t b) { return a + b; }
_MV_INLINE_DEFN uint8x16_t    vaddq_u8 (uint8x16_t a, uint8x16_t b)   { return a + b; }
_MV_INLINE_DEFN uint16x8_t    vaddq_u16(uint16x8_t a, uint16x8_t b)   { return a + b; }
_MV_INLINE_DEFN uint32x4_t    vaddq_u32(uint32x4_t a, uint32x4_t b)   { return a + b; }
_MV_INLINE_DEFN uint64x2_t    vaddq_u64(uint64x2_t a, uint64x2_t b)   { return a + b; }

/* Vector addition with 64-bit operands and a 128-bit result: */
/*		vaddl_<type>.Vr[i] := Va[i] + Vb[i] */
_MV_INLINE_DEFN int16x8_t     vaddl_s8 (int8x8_t a, int8x8_t b)       { return __builtin_convertvector(a, int16x8_t) + __builtin_convertvector(b, int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vaddl_s16(int16x4_t a, int16x4_t b)     { return __builtin_convertvector(a, int32x4_t) + __builtin_convertvector(b, int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vaddl_s32(int32x2_t a, int32x2_t b)     { return __builtin_convertvector(a, int64x2_t) + __builtin_convertvector(b, int64x2_t); }
_MV_INLINE_DEFN uint16x8_t    vaddl_u8 (uint8x8_t a, uint8x8_t b)     { return __builtin_convertvector(a, uint16x8_t) + __builtin_convertvector(b, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vaddl_u16(uint16x4_t a, uint16x4_t b)   { return __builtin_convertvector(a, uint32x4_t) + __builtin_convertvector(b, uint32x4_t); }
_MV_INLINE_DEFN uint64x2_t    vaddl_u32(uint32x2_t a, uint32x2_t b)   { return __builtin_convertvector(a, uint64x2_t) + __builtin_convertvector(b, uint64x2_t); }

/* Widening vector addition, operands are mixed 64-bit and 128-bit, result is 128-bit: */
/*		vaddw_<type>.Vr[i] := Va[i] + Vb[i] */
_MV_INLINE_DEFN int16x8_t     vaddw_s8 (int16x8_t a, int8x8_t b)      { return a + __builtin_convertvector(b, int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vaddw_s16(int32x4_t a, int16x4_t b)     { return a + __builtin_convertvector(b, int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vaddw_s32(int64x2_t a, int32x2_t b)     { return a + __builtin_convertvector(b, int64x2_t); }
_MV_INLINE_DEFN uint16x8_t    vaddw_u8 (uint16x8_t a, uint8x8_t b)    { return a + __builtin_convertvector(b, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vaddw_u16(uint32x4_t a, uint16x4_t b)   { return a + __builtin_convertvector(b, uint32x4_t); }
_MV_INLINE_DEFN uint64x2_t    vaddw_u32(uint64x2_t a, uint32x2_t b)   { return a + __builtin_convertvector(b, uint64x2_t); }

/* Vector halving addition: */
/*		vhadd{q}_<type>.Vr[i] := (Va[i] + Vb[i]) >> 1 */
_MV_INLINE_DEFN int8x8_t      vhadd_s8 (int8x8_t a, int8x8_t b)       { int16x8_t sum = ((__builtin_convertvector(a, int16x8_t) + __builtin_convertvector(b, int16x8_t)) >> 1); return __builtin_convertvector(sum, int8x8_t); }
_MV_INLINE_DEFN int16x4_t     vhadd_s16(int16x4_t a, int16x4_t b)     { int32x4_t sum = ((__builtin_convertvector(a, int32x4_t) + __builtin_convertvector(b, int32x4_t)) >> 1); return __builtin_convertvector(sum, int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vhadd_s32(int32x2_t a, int32x2_t b)     { int64x2_t sum = ((__builtin_convertvector(a, int64x2_t) + __builtin_convertvector(b, int64x2_t)) >> 1); return __builtin_convertvector(sum, int32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vhadd_u8 (uint8x8_t a, uint8x8_t b)     { uint16x8_t sum = ((__builtin_convertvector(a, uint16x8_t) + __builtin_convertvector(b, uint16x8_t)) >> 1); return __builtin_convertvector(sum, uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vhadd_u16(uint16x4_t a, uint16x4_t b)   { uint32x4_t sum = ((__builtin_convertvector(a, uint32x4_t) + __builtin_convertvector(b, uint32x4_t)) >> 1); return __builtin_convertvector(sum, uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vhadd_u32(uint32x2_t a, uint32x2_t b)   { uint64x2_t sum = ((__builtin_convertvector(a, uint64x2_t) + __builtin_convertvector(b, uint64x2_t)) >> 1); return __builtin_convertvector(sum, uint32x2_t); }

_MV_INLINE_DEFN int8x16_t     vhaddq_s8 (int8x16_t a, int8x16_t b)    { int8x16_t result; result.lo = vhadd_s8(a.lo, b.lo); result.hi = vhadd_s8(a.hi, b.hi); return result; }
_MV_INLINE_DEFN int16x8_t     vhaddq_s16(int16x8_t a, int16x8_t b)    { int16x8_t result; result.lo = vhadd_s16(a.lo, b.lo); result.hi = vhadd_s16(a.hi, b.hi); return result; }
_MV_INLINE_DEFN int32x4_t     vhaddq_s32(int32x4_t a, int32x4_t b)    { int32x4_t result; result.lo = vhadd_s32(a.lo, b.lo); result.hi = vhadd_s32(a.hi, b.hi); return result; }
_MV_INLINE_DEFN uint8x16_t    vhaddq_u8 (uint8x16_t a, uint8x16_t b)  { uint8x16_t result; result.lo = vhadd_u8(a.lo, b.lo); result.hi = vhadd_u8(a.hi, b.hi); return result; }
_MV_INLINE_DEFN uint16x8_t    vhaddq_u16(uint16x8_t a, uint16x8_t b)  { uint16x8_t result; result.lo = vhadd_u16(a.lo, b.lo); result.hi = vhadd_u16(a.hi, b.hi); return result; }
_MV_INLINE_DEFN uint32x4_t    vhaddq_u32(uint32x4_t a, uint32x4_t b)  { uint32x4_t result; result.lo = vhadd_u32(a.lo, b.lo); result.hi = vhadd_u32(a.hi, b.hi); return result; }

/* Vector rounding halving addition: */
/*		vrhadd{q}_<type>.Vr[i] := (Va[i] + Vb[i] + 1) >> 1 */
extern          int8x8_t      vrhadd_s8 (int8x8_t, int8x8_t);
extern          int16x4_t     vrhadd_s16(int16x4_t, int16x4_t);
extern          int32x2_t     vrhadd_s32(int32x2_t, int32x2_t);
extern          uint8x8_t     vrhadd_u8 (uint8x8_t, uint8x8_t);
extern          uint16x4_t    vrhadd_u16(uint16x4_t, uint16x4_t);
extern          uint32x2_t    vrhadd_u32(uint32x2_t, uint32x2_t);

extern          int8x16_t     vrhaddq_s8 (int8x16_t, int8x16_t);
extern          int16x8_t     vrhaddq_s16(int16x8_t, int16x8_t);
extern          int32x4_t     vrhaddq_s32(int32x4_t, int32x4_t);
extern          uint8x16_t    vrhaddq_u8 (uint8x16_t, uint8x16_t);
extern          uint16x8_t    vrhaddq_u16(uint16x8_t, uint16x8_t);
extern          uint32x4_t    vrhaddq_u32(uint32x4_t, uint32x4_t);

/* Vector saturating add: */
/*		vqadd{q}_<type> */
_MV_INLINE_DEFN int8x8_t      vqadd_s8 (int8x8_t a, int8x8_t b)       { int8x16_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_iadds_i8_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN int16x4_t     vqadd_s16(int16x4_t a, int16x4_t b)     { int16x8_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_iadds_i16_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN int32x2_t     vqadd_s32(int32x2_t a, int32x2_t b)     { int32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_iadds_i32_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN int64x1_t     vqadd_s64(int64x1_t a, int64x1_t b)     { uint64_t uA = (uint64_t)a[0]; uint64_t uB = (uint64_t)b[0]; uint64_t result = uA + uB; uA = (uA >> 63) + (uint64_t)LLONG_MAX; if ((int64_t)((uA ^ uB) | ~(uB ^ result)) >= 0) result = uA; return result; }
_MV_INLINE_DEFN uint8x8_t     vqadd_u8 (uint8x8_t a, uint8x8_t b)     { uint8x16_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_iadds_u8_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN uint16x4_t    vqadd_u16(uint16x4_t a, uint16x4_t b)   { uint16x8_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_iadds_u16_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN uint32x2_t    vqadd_u32(uint32x2_t a, uint32x2_t b)   { uint32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_iadds_u32_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN uint64x1_t    vqadd_u64(uint64x1_t a, uint64x1_t b)   { const uint64_t _a = a[0]; uint64_t result = _a + b[0]; result |= -(result < _a); return result; }

_MV_INLINE_DEFN int8x16_t     vqaddq_s8 (int8x16_t a, int8x16_t b)    { return __builtin_shave_vau_iadds_i8_rr( a, b ); }
_MV_INLINE_DEFN int16x8_t     vqaddq_s16(int16x8_t a, int16x8_t b)    { return __builtin_shave_vau_iadds_i16_rr( a, b ); }
_MV_INLINE_DEFN int32x4_t     vqaddq_s32(int32x4_t a, int32x4_t b)    { return __builtin_shave_vau_iadds_i32_rr( a, b ); }
_MV_INLINE_DEFN int64x2_t     vqaddq_s64(int64x2_t a, int64x2_t b)    { int64x2_t result; for (int i = 0; i < 2; ++i) { uint64_t uA = (uint64_t)a[i]; uint64_t uB = (uint64_t)b[i]; uint64_t tmp = uA + uB; uA = (uA >> 63) + (uint64_t)LLONG_MAX; if ((int64_t)((uA ^ uB) | ~(uB ^ tmp)) >= 0) tmp = uA; result[i] = (int64_t)tmp; } return result; }
_MV_INLINE_DEFN uint8x16_t    vqaddq_u8 (uint8x16_t a, uint8x16_t b)  { return __builtin_shave_vau_iadds_u8_rr( a, b ); }
_MV_INLINE_DEFN uint16x8_t    vqaddq_u16(uint16x8_t a, uint16x8_t b)  { return __builtin_shave_vau_iadds_u16_rr( a, b ); }
_MV_INLINE_DEFN uint32x4_t    vqaddq_u32(uint32x4_t a, uint32x4_t b)  { return __builtin_shave_vau_iadds_u32_rr( a, b ); }
_MV_INLINE_DEFN uint64x2_t    vqaddq_u64(uint64x2_t a, uint64x2_t b)  { uint64x2_t result; for (int i = 0; i < 2; ++i) { const uint64_t _a = a[i]; uint64_t tmp = _a + b[i]; tmp |= -(tmp < _a); result[i] = tmp; } return result; }

/* Vector add high half: */
/*		vaddhn_<type>.Vr[i] := Va[i] + Vb[i] */
_MV_INLINE_DEFN int8x8_t      vaddhn_s16(int16x8_t a, int16x8_t b)    { return __builtin_convertvector(((a + b) >> 8), int8x8_t); }
_MV_INLINE_DEFN int16x4_t     vaddhn_s32(int32x4_t a, int32x4_t b)    { return __builtin_convertvector(((a + b) >> 16), int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vaddhn_s64(int64x2_t a, int64x2_t b)    { return __builtin_convertvector(((a + b) >> 32), int32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vaddhn_u16(uint16x8_t a, uint16x8_t b)  { return __builtin_convertvector(((a + b) >> 8), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vaddhn_u32(uint32x4_t a, uint32x4_t b)  { return __builtin_convertvector(((a + b) >> 16), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vaddhn_u64(uint64x2_t a, uint64x2_t b)  { return __builtin_convertvector(((a + b) >> 32), uint32x2_t); }

/* Vector rounding add high half: */
/*		vraddhn_<type> */
_MV_INLINE_DEFN int8x8_t      vraddhn_s16(int16x8_t a, int16x8_t b)   { int16x8_t round = (((a + b) >> 7)& 1); return __builtin_convertvector((((a + b) >> 8) + round), int8x8_t); }
_MV_INLINE_DEFN int16x4_t     vraddhn_s32(int32x4_t a, int32x4_t b)   { int32x4_t round = (((a + b) >> 15)& 1); return __builtin_convertvector((((a + b) >> 16) + round), int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vraddhn_s64(int64x2_t a, int64x2_t b)   { int64x2_t round = (((a + b) >> 31)& 1); return __builtin_convertvector((((a + b) >> 32) + round), int32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vraddhn_u16(uint16x8_t a, uint16x8_t b) { uint16x8_t round = (((a + b) >> 7)& 1); return __builtin_convertvector((((a + b) >> 8) + round), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vraddhn_u32(uint32x4_t a, uint32x4_t b) { uint32x4_t round = (((a + b) >> 15)& 1); return __builtin_convertvector((((a + b) >> 16) + round), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vraddhn_u64(uint64x2_t a, uint64x2_t b) { uint64x2_t round = (((a + b) >> 31)& 1); return __builtin_convertvector((((a + b) >> 32) + round), uint32x2_t); }


/* */
/* ARM Section 18.5 - NEON intrinsics for multiplication */
/* */

/* Vector multiply: */
/*		vmul{q}_<type>. Vr[i] := Va[i] * Vb[i] */
_MV_INLINE_DEFN int8x8_t      vmul_s8 (int8x8_t a, int8x8_t b)        { return a * b; }
_MV_INLINE_DEFN int16x4_t     vmul_s16(int16x4_t a, int16x4_t b)      { return a * b; }
_MV_INLINE_DEFN int32x2_t     vmul_s32(int32x2_t a, int32x2_t b)      { return a * b; }
_MV_INLINE_DEFN float32x2_t   vmul_f32(float32x2_t a, float32x2_t b)  { return a * b; }
_MV_INLINE_DEFN uint8x8_t     vmul_u8 (uint8x8_t a, uint8x8_t b)      { return a * b; }
_MV_INLINE_DEFN uint16x4_t    vmul_u16(uint16x4_t a, uint16x4_t b)    { return a * b; }
_MV_INLINE_DEFN uint32x2_t    vmul_u32(uint32x2_t a, uint32x2_t b)    { return a * b; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vmul_p8 (poly8x8_t, poly8x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vmulq_s8 (int8x16_t a, int8x16_t b)     { return a * b; }
_MV_INLINE_DEFN int16x8_t     vmulq_s16(int16x8_t a, int16x8_t b)     { return a * b; }
_MV_INLINE_DEFN int32x4_t     vmulq_s32(int32x4_t a, int32x4_t b)     { return a * b; }
_MV_INLINE_DEFN float32x4_t   vmulq_f32(float32x4_t a, float32x4_t b) { return a * b; }
_MV_INLINE_DEFN uint8x16_t    vmulq_u8 (uint8x16_t a, uint8x16_t b)   { return a * b; }
_MV_INLINE_DEFN uint16x8_t    vmulq_u16(uint16x8_t a, uint16x8_t b)   { return a * b; }
_MV_INLINE_DEFN uint32x4_t    vmulq_u32(uint32x4_t a, uint32x4_t b)   { return a * b; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vmulq_p8 (poly8x16_t, poly8x16_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Vector multiply accumulate: */
/*		vmla{q}_<type>. Vr[i] := Va[i] + Vb[i] * Vc[i] */
_MV_INLINE_DEFN int8x8_t      vmla_s8 (int8x8_t a, int8x8_t b, int8x8_t c)           { return a + b * c; }
_MV_INLINE_DEFN int16x4_t     vmla_s16(int16x4_t a, int16x4_t b, int16x4_t c)        { return a + b * c; }
_MV_INLINE_DEFN int32x2_t     vmla_s32(int32x2_t a, int32x2_t b, int32x2_t c)        { return a + b * c; }
_MV_INLINE_DEFN float32x2_t   vmla_f32(float32x2_t a, float32x2_t b, float32x2_t c)  { return a + b * c; }
_MV_INLINE_DEFN uint8x8_t     vmla_u8 (uint8x8_t a, uint8x8_t b, uint8x8_t c)        { return a + b * c; }
_MV_INLINE_DEFN uint16x4_t    vmla_u16(uint16x4_t a, uint16x4_t b, uint16x4_t c)     { return a + b * c; }
_MV_INLINE_DEFN uint32x2_t    vmla_u32(uint32x2_t a, uint32x2_t b, uint32x2_t c)     { return a + b * c; }

_MV_INLINE_DEFN int8x16_t     vmlaq_s8 (int8x16_t a, int8x16_t b, int8x16_t c)       { return a + b * c; }
_MV_INLINE_DEFN int16x8_t     vmlaq_s16(int16x8_t a, int16x8_t b, int16x8_t c)       { return a + b * c; }
_MV_INLINE_DEFN int32x4_t     vmlaq_s32(int32x4_t a, int32x4_t b, int32x4_t c)       { return a + b * c; }
_MV_INLINE_DEFN float32x4_t   vmlaq_f32(float32x4_t a, float32x4_t b, float32x4_t c) { return a + b * c; }
_MV_INLINE_DEFN uint8x16_t    vmlaq_u8 (uint8x16_t a, uint8x16_t b, uint8x16_t c)    { return a + b * c; }
_MV_INLINE_DEFN uint16x8_t    vmlaq_u16(uint16x8_t a, uint16x8_t b, uint16x8_t c)    { return a + b * c; }
_MV_INLINE_DEFN uint32x4_t    vmlaq_u32(uint32x4_t a, uint32x4_t b, uint32x4_t c)    { return a + b * c; }

/* Vector multiply accumulate long: */
/*		vmlal_<type>. Vr[i] := Va[i] + Vb[i] * Vc[i] */
_MV_INLINE_DEFN int16x8_t     vmlal_s8 (int16x8_t a, int8x8_t b, int8x8_t c)         { return a + __builtin_convertvector(b, int16x8_t) * __builtin_convertvector (c, int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vmlal_s16(int32x4_t a, int16x4_t b, int16x4_t c)       { return a + __builtin_convertvector(b, int32x4_t) * __builtin_convertvector (c, int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vmlal_s32(int64x2_t a, int32x2_t b, int32x2_t c)       { return a + __builtin_convertvector(b, int64x2_t) * __builtin_convertvector (c, int64x2_t); }
_MV_INLINE_DEFN uint16x8_t    vmlal_u8 (uint16x8_t a, uint8x8_t b, uint8x8_t c)      { return a + __builtin_convertvector(b, uint16x8_t) * __builtin_convertvector (c, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vmlal_u16(uint32x4_t a, uint16x4_t b, uint16x4_t c)    { return a + __builtin_convertvector(b, uint32x4_t) * __builtin_convertvector (c, uint32x4_t); }
_MV_INLINE_DEFN uint64x2_t    vmlal_u32(uint64x2_t a, uint32x2_t b, uint32x2_t c)    { return a + __builtin_convertvector(b, uint64x2_t) * __builtin_convertvector (c, uint64x2_t); }

/* Vector multiply subtract: */
/*		vmls{q}_<type>. Vr[i] := Va[i] - Vb[i] * Vc[i] */
_MV_INLINE_DEFN int8x8_t      vmls_s8 (int8x8_t a, int8x8_t b, int8x8_t c)           { return a - b * c; }
_MV_INLINE_DEFN int16x4_t     vmls_s16(int16x4_t a, int16x4_t b, int16x4_t c)        { return a - b * c; }
_MV_INLINE_DEFN int32x2_t     vmls_s32(int32x2_t a, int32x2_t b, int32x2_t c)        { return a - b * c; }
_MV_INLINE_DEFN float32x2_t   vmls_f32(float32x2_t a, float32x2_t b, float32x2_t c)  { return a - b * c; }
_MV_INLINE_DEFN uint8x8_t     vmls_u8 (uint8x8_t a, uint8x8_t b, uint8x8_t c)        { return a - b * c; }
_MV_INLINE_DEFN uint16x4_t    vmls_u16(uint16x4_t a, uint16x4_t b, uint16x4_t c)     { return a - b * c; }
_MV_INLINE_DEFN uint32x2_t    vmls_u32(uint32x2_t a, uint32x2_t b, uint32x2_t c)     { return a - b * c; }

_MV_INLINE_DEFN int8x16_t     vmlsq_s8 (int8x16_t a, int8x16_t b, int8x16_t c)       { return a - b * c; }
_MV_INLINE_DEFN int16x8_t     vmlsq_s16(int16x8_t a, int16x8_t b, int16x8_t c)       { return a - b * c; }
_MV_INLINE_DEFN int32x4_t     vmlsq_s32(int32x4_t a, int32x4_t b, int32x4_t c)       { return a - b * c; }
_MV_INLINE_DEFN float32x4_t   vmlsq_f32(float32x4_t a, float32x4_t b, float32x4_t c) { return a - b * c; }
_MV_INLINE_DEFN uint8x16_t    vmlsq_u8 (uint8x16_t a, uint8x16_t b, uint8x16_t c)    { return a - b * c; }
_MV_INLINE_DEFN uint16x8_t    vmlsq_u16(uint16x8_t a, uint16x8_t b, uint16x8_t c)    { return a - b * c; }
_MV_INLINE_DEFN uint32x4_t    vmlsq_u32(uint32x4_t a, uint32x4_t b, uint32x4_t c)    { return a - b * c; }

/* Vector multiply subtract long */
/*		vmlsl_<type> */
_MV_INLINE_DEFN int16x8_t     vmlsl_s8 (int16x8_t a, int8x8_t b, int8x8_t c)         { return a - __builtin_convertvector(b, int16x8_t) * __builtin_convertvector (c, int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vmlsl_s16(int32x4_t a, int16x4_t b, int16x4_t c)       { return a - __builtin_convertvector(b, int32x4_t) * __builtin_convertvector (c, int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vmlsl_s32(int64x2_t a, int32x2_t b, int32x2_t c)       { return a - __builtin_convertvector(b, int64x2_t) * __builtin_convertvector (c, int64x2_t); }
_MV_INLINE_DEFN uint16x8_t    vmlsl_u8 (uint16x8_t a, uint8x8_t b, uint8x8_t c)      { return a - __builtin_convertvector(b, uint16x8_t) * __builtin_convertvector (c, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vmlsl_u16(uint32x4_t a, uint16x4_t b, uint16x4_t c)    { return a - __builtin_convertvector(b, uint32x4_t) * __builtin_convertvector (c, uint32x4_t); }
_MV_INLINE_DEFN uint64x2_t    vmlsl_u32(uint64x2_t a, uint32x2_t b, uint32x2_t c)    { return a - __builtin_convertvector(b, uint64x2_t) * __builtin_convertvector (c, uint64x2_t); }

/* Vector saturating doubling multiply high */
/*		vqdmulh{q}_<type> */
extern          int16x4_t     vqdmulh_s16(int16x4_t, int16x4_t);
extern          int32x2_t     vqdmulh_s32(int32x2_t, int32x2_t);

extern          int16x8_t     vqdmulhq_s16(int16x8_t, int16x8_t);
extern          int32x4_t     vqdmulhq_s32(int32x4_t, int32x4_t);

/* Vector saturating rounding doubling multiply high */
/*		vqrdmulh{q}_<type> */
extern          int16x4_t     vqrdmulh_s16(int16x4_t, int16x4_t);
extern          int32x2_t     vqrdmulh_s32(int32x2_t, int32x2_t);

extern          int16x8_t     vqrdmulhq_s16(int16x8_t, int16x8_t);
extern          int32x4_t     vqrdmulhq_s32(int32x4_t, int32x4_t);

/* Vector saturating doubling multiply accumulate long */
/*		vqdmlal_<type> */
extern          int32x4_t     vqdmlal_s16(int32x4_t, int16x4_t, int16x4_t);
extern          int64x2_t     vqdmlal_s32(int64x2_t, int32x2_t, int32x2_t);

/* Vector saturating doubling multiply subtract long */
/*		vqdmlsl_<type> */
extern          int32x4_t     vqdmlsl_s16(int32x4_t, int16x4_t, int16x4_t);
extern          int64x2_t     vqdmlsl_s32(int64x2_t, int32x2_t, int32x2_t);

/* Vector long multiply */
/*		vmull_<type> */
_MV_INLINE_DEFN int16x8_t     vmull_s8 (int8x8_t a, int8x8_t b)       { return __builtin_convertvector(a, int16x8_t) * __builtin_convertvector(b, int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vmull_s16(int16x4_t a, int16x4_t b)     { return __builtin_convertvector(a, int32x4_t) * __builtin_convertvector(b, int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vmull_s32(int32x2_t a, int32x2_t b)     { return __builtin_convertvector(a, int64x2_t) * __builtin_convertvector(b, int64x2_t); }
_MV_INLINE_DEFN uint16x8_t    vmull_u8 (uint8x8_t a, uint8x8_t b)     { return __builtin_convertvector(a, uint16x8_t) * __builtin_convertvector(b, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vmull_u16(uint16x4_t a, uint16x4_t b)   { return __builtin_convertvector(a, uint32x4_t) * __builtin_convertvector(b, uint32x4_t); }
_MV_INLINE_DEFN uint64x2_t    vmull_u32(uint32x2_t a, uint32x2_t b)   { return __builtin_convertvector(a, uint64x2_t) * __builtin_convertvector(b, uint64x2_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly16x8_t    vmull_p8 (poly8x8_t, poly8x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Vector saturating doubling long multiply */
/*		vqdmull_<type> */
extern          int32x4_t     vqdmull_s16(int16x4_t, int16x4_t);
extern          int64x2_t     vqdmull_s32(int32x2_t, int32x2_t);


/* */
/* ARM Section 18.6 - NEON intrinsics for subtraction */
/* */

/* Vector subtract */
/*		vsub{q}_<type> */
_MV_INLINE_DEFN int8x8_t      vsub_s8 (int8x8_t a, int8x8_t b)        { return a - b; }
_MV_INLINE_DEFN int16x4_t     vsub_s16(int16x4_t a, int16x4_t b)      { return a - b; }
_MV_INLINE_DEFN int32x2_t     vsub_s32(int32x2_t a, int32x2_t b)      { return a - b; }
_MV_INLINE_DEFN int64x1_t     vsub_s64(int64x1_t a, int64x1_t b)      { return a - b; }
_MV_INLINE_DEFN float32x2_t   vsub_f32(float32x2_t a, float32x2_t b)  { return a - b; }
_MV_INLINE_DEFN uint8x8_t     vsub_u8 (uint8x8_t a, uint8x8_t b)      { return a - b; }
_MV_INLINE_DEFN uint16x4_t    vsub_u16(uint16x4_t a, uint16x4_t b)    { return a - b; }
_MV_INLINE_DEFN uint32x2_t    vsub_u32(uint32x2_t a, uint32x2_t b)    { return a - b; }
_MV_INLINE_DEFN uint64x1_t    vsub_u64(uint64x1_t a, uint64x1_t b)    { return a - b; }

_MV_INLINE_DEFN int8x16_t     vsubq_s8 (int8x16_t a, int8x16_t b)     { return a - b; }
_MV_INLINE_DEFN int16x8_t     vsubq_s16(int16x8_t a, int16x8_t b)     { return a - b; }
_MV_INLINE_DEFN int32x4_t     vsubq_s32(int32x4_t a, int32x4_t b)     { return a - b; }
_MV_INLINE_DEFN int64x2_t     vsubq_s64(int64x2_t a, int64x2_t b)     { return a - b; }
_MV_INLINE_DEFN float32x4_t   vsubq_f32(float32x4_t a, float32x4_t b) { return a - b; }
_MV_INLINE_DEFN uint8x16_t    vsubq_u8 (uint8x16_t a, uint8x16_t b)   { return a - b; }
_MV_INLINE_DEFN uint16x8_t    vsubq_u16(uint16x8_t a, uint16x8_t b)   { return a - b; }
_MV_INLINE_DEFN uint32x4_t    vsubq_u32(uint32x4_t a, uint32x4_t b)   { return a - b; }
_MV_INLINE_DEFN uint64x2_t    vsubq_u64(uint64x2_t a, uint64x2_t b)   { return a - b; }

/* Vector long subtract: */
/*		vsubl_<type>. Vr[i] := Va[i] + Vb[i] */
_MV_INLINE_DEFN int16x8_t     vsubl_s8 (int8x8_t a, int8x8_t b)       { return __builtin_convertvector(a, int16x8_t) - __builtin_convertvector(b, int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vsubl_s16(int16x4_t a, int16x4_t b)     { return __builtin_convertvector(a, int32x4_t) - __builtin_convertvector(b, int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vsubl_s32(int32x2_t a, int32x2_t b)     { return __builtin_convertvector(a, int64x2_t) - __builtin_convertvector(b, int64x2_t); }
_MV_INLINE_DEFN uint16x8_t    vsubl_u8 (uint8x8_t a, uint8x8_t b)     { return __builtin_convertvector(a, uint16x8_t) - __builtin_convertvector(b, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vsubl_u16(uint16x4_t a, uint16x4_t b)   { return __builtin_convertvector(a, uint32x4_t) - __builtin_convertvector(b, uint32x4_t); }
_MV_INLINE_DEFN uint64x2_t    vsubl_u32(uint32x2_t a, uint32x2_t b)   { return __builtin_convertvector(a, uint64x2_t) - __builtin_convertvector(b, uint64x2_t); }

/* Vector wide subtract: */
/*		vsubw_<type>. Vr[i] := Va[i] - Vb[i] */
_MV_INLINE_DEFN int16x8_t     vsubw_s8 (int16x8_t a, int8x8_t b)      { return a - __builtin_convertvector(b, int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vsubw_s16(int32x4_t a, int16x4_t b)     { return a - __builtin_convertvector(b, int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vsubw_s32(int64x2_t a, int32x2_t b)     { return a - __builtin_convertvector(b, int64x2_t); }
_MV_INLINE_DEFN uint16x8_t    vsubw_u8 (uint16x8_t a, uint8x8_t b)    { return a - __builtin_convertvector(b, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vsubw_u16(uint32x4_t a, uint16x4_t b)   { return a - __builtin_convertvector(b, uint32x4_t); }
_MV_INLINE_DEFN uint64x2_t    vsubw_u32(uint64x2_t a, uint32x2_t b)   { return a - __builtin_convertvector(b, uint64x2_t); }

/* Vector saturating subtract */
/*		vqsub{q}_<type> */
_MV_INLINE_DEFN int8x8_t      vqsub_s8 (int8x8_t a, int8x8_t b)       { int8x16_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_isubs_i8_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN int16x4_t     vqsub_s16(int16x4_t a, int16x4_t b)     { int16x8_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_isubs_i16_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN int32x2_t     vqsub_s32(int32x2_t a, int32x2_t b)     { int32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_isubs_i32_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN int64x1_t     vqsub_s64(int64x1_t a, int64x1_t b)     { uint64_t uA = (uint64_t)a[0], uB = (uint64_t)b[0], result = uA - uB; uA = (uA >> 63) + (uint64_t)LLONG_MAX; if ((int64_t)((uA ^ uB) & (uA ^ result)) < 0) result = uA; return (int64_t)result; }
_MV_INLINE_DEFN uint8x8_t     vqsub_u8 (uint8x8_t a, uint8x8_t b)     { uint8x16_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_isubs_u8_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN uint16x4_t    vqsub_u16(uint16x4_t a, uint16x4_t b)   { uint16x8_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_isubs_u16_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN uint32x2_t    vqsub_u32(uint32x2_t a, uint32x2_t b)   { uint32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_isubs_u32_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN uint64x1_t    vqsub_u64(uint64x1_t a, uint64x1_t b)   { uint64_t result = a[0] - b[0]; result &= -(result <= a[0]); return result; }

_MV_INLINE_DEFN int8x16_t     vqsubq_s8 (int8x16_t a, int8x16_t b)    { return __builtin_shave_vau_isubs_i8_rr(a, b); }
_MV_INLINE_DEFN int16x8_t     vqsubq_s16(int16x8_t a, int16x8_t b)    { return __builtin_shave_vau_isubs_i16_rr(a, b); }
_MV_INLINE_DEFN int32x4_t     vqsubq_s32(int32x4_t a, int32x4_t b)    { return __builtin_shave_vau_isubs_i32_rr(a, b); }
_MV_INLINE_DEFN int64x2_t     vqsubq_s64(int64x2_t a, int64x2_t b)    { int64x2_t result; for (int i = 0; i < 2; ++i) { uint64_t uA = (uint64_t)a[i], uB = (uint64_t)b[i], tmp = uA - uB; uA = (uA >> 63) + (uint64_t)LLONG_MAX; if ((int64_t)((uA ^ uB) & (uA ^ tmp)) < 0) tmp = uA; result[i] = (int64_t)tmp; } return result; }
_MV_INLINE_DEFN uint8x16_t    vqsubq_u8 (uint8x16_t a, uint8x16_t b)  { return __builtin_shave_vau_isubs_u8_rr(a, b); }
_MV_INLINE_DEFN uint16x8_t    vqsubq_u16(uint16x8_t a, uint16x8_t b)  { return __builtin_shave_vau_isubs_u16_rr(a, b); }
_MV_INLINE_DEFN uint32x4_t    vqsubq_u32(uint32x4_t a, uint32x4_t b)  { return __builtin_shave_vau_isubs_u32_rr(a, b); }
_MV_INLINE_DEFN uint64x2_t    vqsubq_u64(uint64x2_t a, uint64x2_t b)  { uint64x2_t result; for (int i = 0; i < 2; ++i) { uint64_t tmp = a[i] - b[i]; tmp &= -(tmp <= a[i]); result[i] = tmp; } return result; }

/* Vector halving subtract */
/*		vhsub{q}_<type> */
extern          int8x8_t      vhsub_s8 (int8x8_t, int8x8_t);
extern          int16x4_t     vhsub_s16(int16x4_t, int16x4_t);
extern          int32x2_t     vhsub_s32(int32x2_t, int32x2_t);
extern          uint8x8_t     vhsub_u8 (uint8x8_t, uint8x8_t);
extern          uint16x4_t    vhsub_u16(uint16x4_t, uint16x4_t);
extern          uint32x2_t    vhsub_u32(uint32x2_t, uint32x2_t);

extern          int8x16_t     vhsubq_s8 (int8x16_t, int8x16_t);
extern          int16x8_t     vhsubq_s16(int16x8_t, int16x8_t);
extern          int32x4_t     vhsubq_s32(int32x4_t, int32x4_t);
extern          uint8x16_t    vhsubq_u8 (uint8x16_t, uint8x16_t);
extern          uint16x8_t    vhsubq_u16(uint16x8_t, uint16x8_t);
extern          uint32x4_t    vhsubq_u32(uint32x4_t, uint32x4_t);

/* Vector subtract high half */
/*		vsubhn_<type> */
_MV_INLINE_DEFN int8x8_t      vsubhn_s16(int16x8_t a, int16x8_t b)    { return __builtin_convertvector((a - b) >> 8, int8x8_t); }
_MV_INLINE_DEFN int16x4_t     vsubhn_s32(int32x4_t a, int32x4_t b)    { return __builtin_convertvector((a - b) >> 16, int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vsubhn_s64(int64x2_t a, int64x2_t b)    { return __builtin_convertvector((a - b) >> 32, int32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vsubhn_u16(uint16x8_t a, uint16x8_t b)  { return __builtin_convertvector((a - b) >> 8, uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vsubhn_u32(uint32x4_t a, uint32x4_t b)  { return __builtin_convertvector((a - b) >> 16, uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vsubhn_u64(uint64x2_t a, uint64x2_t b)  { return __builtin_convertvector((a - b) >> 32, uint32x2_t); }

/* Vector rounding subtract high half */
/*		vrsubhn_<type> */
_MV_INLINE_DEFN int8x8_t      vrsubhn_s16(int16x8_t a, int16x8_t b)   { int16x8_t sat = (((a - b) >> 7) & 1); return __builtin_convertvector(((a - b) >> 8) + sat, int8x8_t); }
_MV_INLINE_DEFN int16x4_t     vrsubhn_s32(int32x4_t a, int32x4_t b)   { int32x4_t sat = (((a - b) >> 15) & 1); return __builtin_convertvector(((a - b) >> 16) + sat, int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vrsubhn_s64(int64x2_t a, int64x2_t b)   { int64x2_t sat = (((a - b) >> 31) & 1); return __builtin_convertvector(((a - b) >> 32) + sat, int32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vrsubhn_u16(uint16x8_t a, uint16x8_t b) { uint16x8_t sat = (((a - b) >> 7) & 1); return __builtin_convertvector(((a - b) >> 8) + sat, uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vrsubhn_u32(uint32x4_t a, uint32x4_t b) { uint32x4_t sat = (((a - b) >> 15) & 1); return __builtin_convertvector(((a - b) >> 16) + sat, uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vrsubhn_u64(uint64x2_t a, uint64x2_t b) { uint64x2_t sat = (((a - b) >> 31) & 1); return __builtin_convertvector(((a - b) >> 32) + sat, uint32x2_t); }


/* */
/* ARM Section 18.7 - NEON intrinsics for comparison */
/* */

/* Vector compare equal */
/*		vceq{q}_<type> */
_MV_INLINE_DEFN uint8x8_t     vceq_s8 (int8x8_t a, int8x8_t b)         { return __builtin_astype((a == b), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vceq_s16(int16x4_t a, int16x4_t b)       { return __builtin_astype((a == b), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vceq_s32(int32x2_t a, int32x2_t b)       { return __builtin_astype((a == b), uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vceq_f32(float32x2_t a, float32x2_t b)   { return __builtin_astype((a == b), uint32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vceq_u8 (uint8x8_t a, uint8x8_t b)       { return __builtin_astype((a == b), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vceq_u16(uint16x4_t a, uint16x4_t b)     { return __builtin_astype((a == b), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vceq_u32(uint32x2_t a, uint32x2_t b)     { return __builtin_astype((a == b), uint32x2_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint8x8_t     vceq_p8 (poly8x8_t, poly8x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN uint8x16_t    vceqq_s8 (int8x16_t a, int8x16_t b)      { return __builtin_astype((a == b), uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vceqq_s16(int16x8_t a, int16x8_t b)      { return __builtin_astype((a == b), uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vceqq_s32(int32x4_t a, int32x4_t b)      { return __builtin_astype((a == b), uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vceqq_f32(float32x4_t a, float32x4_t b)  { return __builtin_astype((a == b), uint32x4_t); }
_MV_INLINE_DEFN uint8x16_t    vceqq_u8 (uint8x16_t a, uint8x16_t b)    { return __builtin_astype((a == b), uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vceqq_u16(uint16x8_t a, uint16x8_t b)    { return __builtin_astype((a == b), uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vceqq_u32(uint32x4_t a, uint32x4_t b)    { return __builtin_astype((a == b), uint32x4_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint8x16_t    vceqq_p8 (poly8x16_t, poly8x16_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Vector compare greater-than or equal */
/*		vcge{q}_<type> */
_MV_INLINE_DEFN uint8x8_t     vcge_s8 (int8x8_t a, int8x8_t b)         { return __builtin_astype((a >= b), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vcge_s16(int16x4_t a, int16x4_t b)       { return __builtin_astype((a >= b), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vcge_s32(int32x2_t a, int32x2_t b)       { return __builtin_astype((a >= b), uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vcge_f32(float32x2_t a, float32x2_t b)   { return __builtin_astype((a >= b), uint32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vcge_u8 (uint8x8_t a, uint8x8_t b)       { return __builtin_astype((a >= b), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vcge_u16(uint16x4_t a, uint16x4_t b)     { return __builtin_astype((a >= b), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vcge_u32(uint32x2_t a, uint32x2_t b)     { return __builtin_astype((a >= b), uint32x2_t); }

_MV_INLINE_DEFN uint8x16_t    vcgeq_s8 (int8x16_t a, int8x16_t b)      { return __builtin_astype((a >= b), uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vcgeq_s16(int16x8_t a, int16x8_t b)      { return __builtin_astype((a >= b), uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vcgeq_s32(int32x4_t a, int32x4_t b)      { return __builtin_astype((a >= b), uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vcgeq_f32(float32x4_t a, float32x4_t b)  { return __builtin_astype((a >= b), uint32x4_t); }
_MV_INLINE_DEFN uint8x16_t    vcgeq_u8 (uint8x16_t a, uint8x16_t b)    { return __builtin_astype((a >= b), uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vcgeq_u16(uint16x8_t a, uint16x8_t b)    { return __builtin_astype((a >= b), uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vcgeq_u32(uint32x4_t a, uint32x4_t b)    { return __builtin_astype((a >= b), uint32x4_t); }

/* Vector compare less-than or equal */
/*		vcle{q}_<type> */
_MV_INLINE_DEFN uint8x8_t     vcle_s8 (int8x8_t a, int8x8_t b)         { return __builtin_astype((a <= b), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vcle_s16(int16x4_t a, int16x4_t b)       { return __builtin_astype((a <= b), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vcle_s32(int32x2_t a, int32x2_t b)       { return __builtin_astype((a <= b), uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vcle_f32(float32x2_t a, float32x2_t b)   { return __builtin_astype((a <= b), uint32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vcle_u8 (uint8x8_t a, uint8x8_t b)       { return __builtin_astype((a <= b), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vcle_u16(uint16x4_t a, uint16x4_t b)     { return __builtin_astype((a <= b), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vcle_u32(uint32x2_t a, uint32x2_t b)     { return __builtin_astype((a <= b), uint32x2_t); }

_MV_INLINE_DEFN uint8x16_t    vcleq_s8 (int8x16_t a, int8x16_t b)      { return __builtin_astype((a <= b), uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vcleq_s16(int16x8_t a, int16x8_t b)      { return __builtin_astype((a <= b), uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vcleq_s32(int32x4_t a, int32x4_t b)      { return __builtin_astype((a <= b), uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vcleq_f32(float32x4_t a, float32x4_t b)  { return __builtin_astype((a <= b), uint32x4_t); }
_MV_INLINE_DEFN uint8x16_t    vcleq_u8 (uint8x16_t a, uint8x16_t b)    { return __builtin_astype((a <= b), uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vcleq_u16(uint16x8_t a, uint16x8_t b)    { return __builtin_astype((a <= b), uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vcleq_u32(uint32x4_t a, uint32x4_t b)    { return __builtin_astype((a <= b), uint32x4_t); }

/* Vector compare greater-than */
/*		vcgt{q}_<type> */
_MV_INLINE_DEFN uint8x8_t     vcgt_s8 (int8x8_t a, int8x8_t b)         { return __builtin_astype((a > b), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vcgt_s16(int16x4_t a, int16x4_t b)       { return __builtin_astype((a > b), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vcgt_s32(int32x2_t a, int32x2_t b)       { return __builtin_astype((a > b), uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vcgt_f32(float32x2_t a, float32x2_t b)   { return __builtin_astype((a > b), uint32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vcgt_u8 (uint8x8_t a, uint8x8_t b)       { return __builtin_astype((a > b), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vcgt_u16(uint16x4_t a, uint16x4_t b)     { return __builtin_astype((a > b), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vcgt_u32(uint32x2_t a, uint32x2_t b)     { return __builtin_astype((a > b), uint32x2_t); }

_MV_INLINE_DEFN uint8x16_t    vcgtq_s8 (int8x16_t a, int8x16_t b)      { return __builtin_astype((a > b), uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vcgtq_s16(int16x8_t a, int16x8_t b)      { return __builtin_astype((a > b), uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vcgtq_s32(int32x4_t a, int32x4_t b)      { return __builtin_astype((a > b), uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vcgtq_f32(float32x4_t a, float32x4_t b)  { return __builtin_astype((a > b), uint32x4_t); }
_MV_INLINE_DEFN uint8x16_t    vcgtq_u8 (uint8x16_t a, uint8x16_t b)    { return __builtin_astype((a > b), uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vcgtq_u16(uint16x8_t a, uint16x8_t b)    { return __builtin_astype((a > b), uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vcgtq_u32(uint32x4_t a, uint32x4_t b)    { return __builtin_astype((a > b), uint32x4_t); }

/* Vector compare less-than */
/*		vclt{q}_<type> */
_MV_INLINE_DEFN uint8x8_t     vclt_s8 (int8x8_t a, int8x8_t b)         { return __builtin_astype((a < b), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vclt_s16(int16x4_t a, int16x4_t b)       { return __builtin_astype((a < b), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vclt_s32(int32x2_t a, int32x2_t b)       { return __builtin_astype((a < b), uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vclt_f32(float32x2_t a, float32x2_t b)   { return __builtin_astype((a < b), uint32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vclt_u8 (uint8x8_t a, uint8x8_t b)       { return __builtin_astype((a < b), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vclt_u16(uint16x4_t a, uint16x4_t b)     { return __builtin_astype((a < b), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vclt_u32(uint32x2_t a, uint32x2_t b)     { return __builtin_astype((a < b), uint32x2_t); }

_MV_INLINE_DEFN uint8x16_t    vcltq_s8 (int8x16_t a, int8x16_t b)      { return __builtin_astype((a < b), uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vcltq_s16(int16x8_t a, int16x8_t b)      { return __builtin_astype((a < b), uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vcltq_s32(int32x4_t a, int32x4_t b)      { return __builtin_astype((a < b), uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vcltq_f32(float32x4_t a, float32x4_t b)  { return __builtin_astype((a < b), uint32x4_t); }
_MV_INLINE_DEFN uint8x16_t    vcltq_u8 (uint8x16_t a, uint8x16_t b)    { return __builtin_astype((a < b), uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vcltq_u16(uint16x8_t a, uint16x8_t b)    { return __builtin_astype((a < b), uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vcltq_u32(uint32x4_t a, uint32x4_t b)    { return __builtin_astype((a < b), uint32x4_t); }

/* Vector compare absolute */
/*		A helper function for computing 'abs' for a 'float32x2' 64-bit vector */
_MV_INLINE_DEFN float32x2_t   __float32x2_abs(float32x2_t a)           { float32x4_t _a, result; _a.lo = a; result = __builtin_shave_vau_abs_f32_r(_a); return result.lo; }
/*		vcage{q}_<type>, vcale{q}_<type>, vcagt{q}_<type>, vcalt{q}_<type> */
_MV_INLINE_DEFN uint32x2_t    vcage_f32(float32x2_t a, float32x2_t b)  { float32x2_t abs_a = __float32x2_abs(a); float32x2_t abs_b = __float32x2_abs(b); return __builtin_astype((abs_a >= abs_b), uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vcale_f32(float32x2_t a, float32x2_t b)  { float32x2_t abs_a = __float32x2_abs(a); float32x2_t abs_b = __float32x2_abs(b); return __builtin_astype((abs_a <= abs_b), uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vcagt_f32(float32x2_t a, float32x2_t b)  { float32x2_t abs_a = __float32x2_abs(a); float32x2_t abs_b = __float32x2_abs(b); return __builtin_astype((abs_a > abs_b), uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vcalt_f32(float32x2_t a, float32x2_t b)  { float32x2_t abs_a = __float32x2_abs(a); float32x2_t abs_b = __float32x2_abs(b); return __builtin_astype((abs_a < abs_b), uint32x2_t); }

_MV_INLINE_DEFN uint32x4_t    vcageq_f32(float32x4_t a, float32x4_t b) { float32x4_t abs_a = __builtin_shave_vau_abs_f32_r(a); float32x4_t abs_b = __builtin_shave_vau_abs_f32_r(b); return __builtin_astype((abs_a >= abs_b), uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vcaleq_f32(float32x4_t a, float32x4_t b) { float32x4_t abs_a = __builtin_shave_vau_abs_f32_r(a); float32x4_t abs_b = __builtin_shave_vau_abs_f32_r(b); return __builtin_astype((abs_a <= abs_b), uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vcagtq_f32(float32x4_t a, float32x4_t b) { float32x4_t abs_a = __builtin_shave_vau_abs_f32_r(a); float32x4_t abs_b = __builtin_shave_vau_abs_f32_r(b); return __builtin_astype((abs_a > abs_b), uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vcaltq_f32(float32x4_t a, float32x4_t b) { float32x4_t abs_a = __builtin_shave_vau_abs_f32_r(a); float32x4_t abs_b = __builtin_shave_vau_abs_f32_r(b); return __builtin_astype((abs_a < abs_b), uint32x4_t); }

/* Vector test bits */
/*		vtst{q}_<type> */
_MV_INLINE_DEFN uint8x8_t     vtst_s8 (int8x8_t a, int8x8_t b)         { return __builtin_astype((a & b) != 0, uint8x8_t);  }
_MV_INLINE_DEFN uint16x4_t    vtst_s16(int16x4_t a, int16x4_t b)       { return __builtin_astype((a & b) != 0, uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vtst_s32(int32x2_t a, int32x2_t b)       { return __builtin_astype((a & b) != 0, uint32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vtst_u8 (uint8x8_t a, uint8x8_t b)       { return __builtin_astype((a & b) != 0, uint8x8_t);  }
_MV_INLINE_DEFN uint16x4_t    vtst_u16(uint16x4_t a, uint16x4_t b)     { return __builtin_astype((a & b) != 0, uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vtst_u32(uint32x2_t a, uint32x2_t b)     { return __builtin_astype((a & b) != 0, uint32x2_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint8x8_t     vtst_p8 (poly8x8_t, poly8x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN uint8x16_t    vtstq_s8 (int8x16_t a, int8x16_t b)      { return __builtin_astype((a & b) != 0, uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vtstq_s16(int16x8_t a, int16x8_t b)      { return __builtin_astype((a & b) != 0, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vtstq_s32(int32x4_t a, int32x4_t b)      { return __builtin_astype((a & b) != 0, uint32x4_t); }
_MV_INLINE_DEFN uint8x16_t    vtstq_u8 (uint8x16_t a, uint8x16_t b)    { return __builtin_astype((a & b) != 0, uint8x16_t); }
_MV_INLINE_DEFN uint16x8_t    vtstq_u16(uint16x8_t a, uint16x8_t b)    { return __builtin_astype((a & b) != 0, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vtstq_u32(uint32x4_t a, uint32x4_t b)    { return __builtin_astype((a & b) != 0, uint32x4_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint8x16_t    vtstq_p8 (poly8x16_t, poly8x16_t);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.8 - NEON intrinsics for absolute difference */
/* */

/* Absolute difference between the arguments */
/*		vabd{q}_<type>. Vr[i] = | Va[i] - Vb[i] | */
_MV_INLINE_DEFN int8x8_t      vabd_s8(int8x8_t a, int8x8_t b)         { int8x16_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_adiff_i8_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN int16x4_t     vabd_s16(int16x4_t a, int16x4_t b)      { int16x8_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_adiff_i16_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN int32x2_t     vabd_s32(int32x2_t a, int32x2_t b)      { int32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_adiff_i32_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN float32x2_t   vabd_f32(float32x2_t a, float32x2_t b)  { float32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_adiff_f32_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN uint8x8_t     vabd_u8(uint8x8_t a, uint8x8_t b)       { uint8x16_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_adiff_u8_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN uint16x4_t    vabd_u16(uint16x4_t a, uint16x4_t b)    { uint16x8_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_adiff_u16_rr(_a, _b); return result.lo; }
_MV_INLINE_DEFN uint32x2_t    vabd_u32(uint32x2_t a, uint32x2_t b)    { uint32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_vau_adiff_u32_rr(_a, _b); return result.lo; }

_MV_INLINE_DEFN int8x16_t     vabdq_s8 (int8x16_t a, int8x16_t b)     { return __builtin_shave_vau_adiff_i8_rr(a, b); }
_MV_INLINE_DEFN int16x8_t     vabdq_s16(int16x8_t a, int16x8_t b)     { return __builtin_shave_vau_adiff_i16_rr(a, b); }
_MV_INLINE_DEFN int32x4_t     vabdq_s32(int32x4_t a, int32x4_t b)     { return __builtin_shave_vau_adiff_i32_rr(a, b); }
_MV_INLINE_DEFN float32x4_t   vabdq_f32(float32x4_t a, float32x4_t b) { return __builtin_shave_vau_adiff_f32_rr(a, b); }
_MV_INLINE_DEFN uint8x16_t    vabdq_u8 (uint8x16_t a, uint8x16_t b)   { return __builtin_shave_vau_adiff_u8_rr(a, b); }
_MV_INLINE_DEFN uint16x8_t    vabdq_u16(uint16x8_t a, uint16x8_t b)   { return __builtin_shave_vau_adiff_u16_rr(a, b); }
_MV_INLINE_DEFN uint32x4_t    vabdq_u32(uint32x4_t a, uint32x4_t b)   { return __builtin_shave_vau_adiff_u32_rr(a, b); }

/*		A helper function for computing 'abs' for an int64x2_t vector. */
_MV_INLINE_DEFN int64x2_t     __int64x2_t_abs(int64x2_t a)              { int64x2_t result = { a.s0 >= 0 ? a.s0 : -a.s0, a.s1 >= 0 ? a.s1 : -a.s1 }; return result; }

/* Absolute difference long */
/*		vabdl_<type> */
_MV_INLINE_DEFN int16x8_t     vabdl_s8 (int8x8_t a, int8x8_t b)       { int16x8_t la = __builtin_convertvector(a, int16x8_t); int16x8_t lb = __builtin_convertvector(b, int16x8_t); return __builtin_shave_vau_adiff_i16_rr(la, lb); }
_MV_INLINE_DEFN int32x4_t     vabdl_s16(int16x4_t a, int16x4_t b)     { int32x4_t la = __builtin_convertvector(a, int32x4_t); int32x4_t lb = __builtin_convertvector(b, int32x4_t); return __builtin_shave_vau_adiff_i32_rr(la, lb); }
_MV_INLINE_DEFN int64x2_t     vabdl_s32(int32x2_t a, int32x2_t b)     { int64x2_t la = __builtin_convertvector(a, int64x2_t); int64x2_t lb = __builtin_convertvector(b, int64x2_t); return __int64x2_t_abs(la - lb); }
_MV_INLINE_DEFN uint16x8_t    vabdl_u8 (uint8x8_t a, uint8x8_t b)     { uint8x8_t lt = __builtin_astype((a < b), uint8x8_t); uint8x8_t diff = (a - b); uint8x8_t diff_abs = (-diff & lt) | (diff & (!lt)); return __builtin_convertvector(diff_abs, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vabdl_u16(uint16x4_t a, uint16x4_t b)   { uint16x4_t lt = __builtin_astype((a < b), uint16x4_t); uint16x4_t diff = (a - b); uint16x4_t diff_abs = (-diff & lt) | (diff & (!lt)); return __builtin_convertvector(diff_abs, uint32x4_t); }
_MV_INLINE_DEFN uint64x2_t    vabdl_u32(uint32x2_t a, uint32x2_t b)   { uint32x2_t lt = __builtin_astype((a < b), uint32x2_t); uint32x2_t diff = (a - b); uint32x2_t diff_abs = (-diff & lt) | (diff & (!lt)); return __builtin_convertvector(diff_abs, uint64x2_t); }

/* Absolute difference and accumulate */
/*		vaba{q}_<type>. Vr[i] = Va[i] + | Vb[i] - Vc[i] | */
_MV_INLINE_DEFN int8x8_t      vaba_s8 (int8x8_t a, int8x8_t b, int8x8_t c)        { return a + vabd_s8(b, c); }
_MV_INLINE_DEFN int16x4_t     vaba_s16(int16x4_t a, int16x4_t b, int16x4_t c)     { return a + vabd_s16(b, c); }
_MV_INLINE_DEFN int32x2_t     vaba_s32(int32x2_t a, int32x2_t b, int32x2_t c)     { return a + vabd_s32(b, c); }
_MV_INLINE_DEFN uint8x8_t     vaba_u8 (uint8x8_t a, uint8x8_t b, uint8x8_t c)     { return a + vabd_u8(b, c); }
_MV_INLINE_DEFN uint16x4_t    vaba_u16(uint16x4_t a, uint16x4_t b, uint16x4_t c)  { return a + vabd_u16(b, c); }
_MV_INLINE_DEFN uint32x2_t    vaba_u32(uint32x2_t a, uint32x2_t b, uint32x2_t c)  { return a + vabd_u32(b, c); }

_MV_INLINE_DEFN int8x16_t     vabaq_s8 (int8x16_t a, int8x16_t b, int8x16_t c)    { return a + __builtin_shave_vau_adiff_i8_rr(b, c); }
_MV_INLINE_DEFN int16x8_t     vabaq_s16(int16x8_t a, int16x8_t b, int16x8_t c)    { return a + __builtin_shave_vau_adiff_i16_rr(b, c); }
_MV_INLINE_DEFN int32x4_t     vabaq_s32(int32x4_t a, int32x4_t b, int32x4_t c)    { return a + __builtin_shave_vau_adiff_i32_rr(b, c); }
_MV_INLINE_DEFN uint8x16_t    vabaq_u8 (uint8x16_t a, uint8x16_t b, uint8x16_t c) { return a + __builtin_shave_vau_adiff_u8_rr(b, c); }
_MV_INLINE_DEFN uint16x8_t    vabaq_u16(uint16x8_t a, uint16x8_t b, uint16x8_t c) { return a + __builtin_shave_vau_adiff_u16_rr(b, c); }
_MV_INLINE_DEFN uint32x4_t    vabaq_u32(uint32x4_t a, uint32x4_t b, uint32x4_t c) { return a + __builtin_shave_vau_adiff_u32_rr(b, c); }

/* Absolute difference and accumulate - long */
/*		vabal_<type> */
_MV_INLINE_DEFN int16x8_t     vabal_s8 (int16x8_t a, int8x8_t b, int8x8_t c)      { return a + vabdl_s8(b, c); }
_MV_INLINE_DEFN int32x4_t     vabal_s16(int32x4_t a, int16x4_t b, int16x4_t c)    { return a + vabdl_s16(b, c); }
_MV_INLINE_DEFN int64x2_t     vabal_s32(int64x2_t a, int32x2_t b, int32x2_t c)    { return a + vabdl_s32(b, c); }
_MV_INLINE_DEFN uint16x8_t    vabal_u8 (uint16x8_t a, uint8x8_t b, uint8x8_t c)   { return a + vabdl_u8(b, c); }
_MV_INLINE_DEFN uint32x4_t    vabal_u16(uint32x4_t a, uint16x4_t b, uint16x4_t c) { return a + vabdl_u16(b, c); }
_MV_INLINE_DEFN uint64x2_t    vabal_u32(uint64x2_t a, uint32x2_t b, uint32x2_t c) { return a + vabdl_u32(b, c); }


/* */
/* ARM Section 18.9 - NEON intrinsics for maximum and minimum */
/* */

/*		vmax{q}_<type>. Vr[i] := (Va[i] >= Vb[i]) ? Va[i] : Vb[i] */
/*			gets the maximum element of each corresponding element from two vectors */
_MV_INLINE_DEFN int8x8_t      vmax_s8 (int8x8_t a, int8x8_t b)        { int8x16_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_max_i8_rr_char16(_a , _b); return result.lo; }
_MV_INLINE_DEFN int16x4_t     vmax_s16(int16x4_t a, int16x4_t b)      { int16x8_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_max_i16_rr_short8(_a , _b); return result.lo; }
_MV_INLINE_DEFN int32x2_t     vmax_s32(int32x2_t a, int32x2_t b)      { int32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_max_i32_rr_int4(_a , _b); return result.lo; }
_MV_INLINE_DEFN float32x2_t   vmax_f32(float32x2_t a, float32x2_t b)  { float32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_max_f32_rr_float4(_a , _b); return result.lo; }
_MV_INLINE_DEFN uint8x8_t     vmax_u8 (uint8x8_t a, uint8x8_t b)      { uint8x16_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_max_u8_rr_uchar16(_a , _b); return result.lo; }
_MV_INLINE_DEFN uint16x4_t    vmax_u16(uint16x4_t a, uint16x4_t b)    { uint16x8_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_max_u16_rr_ushort8(_a , _b); return result.lo; }
_MV_INLINE_DEFN uint32x2_t    vmax_u32(uint32x2_t a , uint32x2_t b)   { uint32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_max_u32_rr_uint4(_a , _b); return result.lo; }

_MV_INLINE_DEFN int8x16_t     vmaxq_s8 (int8x16_t a, int8x16_t b)     { return __builtin_shave_cmu_max_i8_rr_char16( a, b); }
_MV_INLINE_DEFN int16x8_t     vmaxq_s16(int16x8_t a, int16x8_t b)     { return __builtin_shave_cmu_max_i16_rr_short8( a, b); }
_MV_INLINE_DEFN int32x4_t     vmaxq_s32(int32x4_t a, int32x4_t b)     { return __builtin_shave_cmu_max_i32_rr_int4( a, b); }
_MV_INLINE_DEFN float32x4_t   vmaxq_f32(float32x4_t a, float32x4_t b) { return __builtin_shave_cmu_max_f32_rr_float4( a, b); }
_MV_INLINE_DEFN uint8x16_t    vmaxq_u8 (uint8x16_t a, uint8x16_t b)   { return __builtin_shave_cmu_max_u8_rr_uchar16( a, b); }
_MV_INLINE_DEFN uint16x8_t    vmaxq_u16(uint16x8_t a, uint16x8_t b)   { return __builtin_shave_cmu_max_u16_rr_ushort8( a, b); }
_MV_INLINE_DEFN uint32x4_t    vmaxq_u32(uint32x4_t a, uint32x4_t b)   { return __builtin_shave_cmu_max_u32_rr_uint4( a, b); }

/*		vmin{q}_<type>. Vr[i] := (Va[i] >= Vb[i]) ? Vb[i] : Va[i] */
/*			gets the minimum element of each corresponding element from two vectors */
_MV_INLINE_DEFN int8x8_t      vmin_s8 (int8x8_t a, int8x8_t b)        { int8x16_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_min_i8_rr_char16(_a , _b); return result.lo; }
_MV_INLINE_DEFN int16x4_t     vmin_s16(int16x4_t a, int16x4_t b)      { int16x8_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_min_i16_rr_short8(_a , _b); return result.lo; }
_MV_INLINE_DEFN int32x2_t     vmin_s32(int32x2_t a, int32x2_t b)      { int32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_min_i32_rr_int4(_a , _b); return result.lo; }
_MV_INLINE_DEFN float32x2_t   vmin_f32(float32x2_t a, float32x2_t b)  { float32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_min_f32_rr_float4(_a , _b); return result.lo; }
_MV_INLINE_DEFN uint8x8_t     vmin_u8 (uint8x8_t a, uint8x8_t b)      { uint8x16_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_min_u8_rr_uchar16(_a , _b); return result.lo; }
_MV_INLINE_DEFN uint16x4_t    vmin_u16(uint16x4_t a, uint16x4_t b)    { uint16x8_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_min_u16_rr_ushort8(_a , _b); return result.lo; }
_MV_INLINE_DEFN uint32x2_t    vmin_u32(uint32x2_t a , uint32x2_t b)   { uint32x4_t _a, _b, result; _a.lo = a; _b.lo = b; result = __builtin_shave_cmu_min_u32_rr_uint4(_a , _b); return result.lo; }

_MV_INLINE_DEFN int8x16_t     vminq_s8 (int8x16_t a, int8x16_t b)     { return __builtin_shave_cmu_min_i8_rr_char16( a, b); }
_MV_INLINE_DEFN int16x8_t     vminq_s16(int16x8_t a, int16x8_t b)     { return __builtin_shave_cmu_min_i16_rr_short8( a, b); }
_MV_INLINE_DEFN int32x4_t     vminq_s32(int32x4_t a, int32x4_t b)     { return __builtin_shave_cmu_min_i32_rr_int4( a, b); }
_MV_INLINE_DEFN float32x4_t   vminq_f32(float32x4_t a, float32x4_t b) { return __builtin_shave_cmu_min_f32_rr_float4( a, b); }
_MV_INLINE_DEFN uint8x16_t    vminq_u8 (uint8x16_t a, uint8x16_t b)   { return __builtin_shave_cmu_min_u8_rr_uchar16( a, b); }
_MV_INLINE_DEFN uint16x8_t    vminq_u16(uint16x8_t a, uint16x8_t b)   { return __builtin_shave_cmu_min_u16_rr_ushort8( a, b); }
_MV_INLINE_DEFN uint32x4_t    vminq_u32(uint32x4_t a, uint32x4_t b)   { return __builtin_shave_cmu_min_u32_rr_uint4( a, b); }


/* */
/* ARM Section 18.10 - NEON intrinsics for pairwise addition */
/* */

/* Pairwise add */
/*		vpadd_<type> */
_MV_INLINE_DEFN int8x8_t      vpadd_s8 (int8x8_t a, int8x8_t b)           { int8x8_t _lo = __builtin_shufflevector(a, b, 0, 2, 4, 6, 8, 10, 12, 14); int8x8_t _hi = __builtin_shufflevector(a, b, 1, 3, 5, 7, 9, 11, 13, 15); return _lo + _hi; }
_MV_INLINE_DEFN int16x4_t     vpadd_s16(int16x4_t a, int16x4_t b)         { int16x4_t _lo = __builtin_shufflevector(a, b, 0, 2, 4, 6); int16x4_t _hi = __builtin_shufflevector(a, b, 1, 3, 5, 7); return _lo + _hi; }
_MV_INLINE_DEFN int32x2_t     vpadd_s32(int32x2_t a, int32x2_t b)         { int32x2_t _lo = {a.x, b.x}; int32x2_t _hi = {a.y, b.y}; return _lo + _hi; }
_MV_INLINE_DEFN float32x2_t   vpadd_f32(float32x2_t a, float32x2_t b)     { float32x2_t _lo = {a.x, b.x}; float32x2_t _hi = {a.y, b.y}; return _lo + _hi; }
_MV_INLINE_DEFN uint8x8_t     vpadd_u8 (uint8x8_t a, uint8x8_t b)         { uint8x8_t _lo = __builtin_shufflevector(a, b, 0, 2, 4, 6, 8, 10, 12, 14); uint8x8_t _hi = __builtin_shufflevector(a, b, 1, 3, 5, 7, 9, 11, 13, 15); return _lo + _hi; }
_MV_INLINE_DEFN uint16x4_t    vpadd_u16(uint16x4_t a, uint16x4_t b)       { uint16x4_t _lo = __builtin_shufflevector(a, b, 0, 2, 4, 6); uint16x4_t _hi = __builtin_shufflevector(a, b, 1, 3, 5, 7); return _lo + _hi; }
_MV_INLINE_DEFN uint32x2_t    vpadd_u32(uint32x2_t a, uint32x2_t b)       { uint32x2_t _lo = {a.x, b.x}; uint32x2_t _hi = {a.y, b.y}; return _lo + _hi; }

/* Long pairwise add */
/*		vpaddl{q}_<type> */
_MV_INLINE_DEFN int16x4_t     vpaddl_s8 (int8x8_t a)                      { return __builtin_convertvector(a.s0246, int16x4_t) + __builtin_convertvector(a.s1357, int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vpaddl_s16(int16x4_t a)                     { return __builtin_convertvector(a.s02, int32x2_t) + __builtin_convertvector(a.s13, int32x2_t); }
_MV_INLINE_DEFN int64x1_t     vpaddl_s32(int32x2_t a)                     { return (int64_t)a.x + (int64_t)a.y; }
_MV_INLINE_DEFN uint16x4_t    vpaddl_u8 (uint8x8_t a)                     { return __builtin_convertvector(a.s0246, uint16x4_t) + __builtin_convertvector(a.s1357, uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vpaddl_u16(uint16x4_t a)                    { return __builtin_convertvector(a.s02, uint32x2_t) + __builtin_convertvector(a.s13, uint32x2_t); }
_MV_INLINE_DEFN uint64x1_t    vpaddl_u32(uint32x2_t a)                    { return (uint64_t)a.x + (uint64_t)a.y; }

_MV_INLINE_DEFN int16x8_t     vpaddlq_s8 (int8x16_t a)                    { return __builtin_convertvector(a.s02468ace, int16x8_t) + __builtin_convertvector(a.s13579bdf, int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vpaddlq_s16(int16x8_t a)                    { return __builtin_convertvector(a.s0246, int32x4_t) + __builtin_convertvector(a.s1357, int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vpaddlq_s32(int32x4_t a)                    { return __builtin_convertvector(a.s02, int64x2_t) + __builtin_convertvector(a.s13, int64x2_t); }
_MV_INLINE_DEFN uint16x8_t    vpaddlq_u8 (uint8x16_t a)                   { return __builtin_convertvector(a.s02468ace, uint16x8_t) + __builtin_convertvector(a.s13579bdf, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vpaddlq_u16(uint16x8_t a)                   { return __builtin_convertvector(a.s0246, uint32x4_t) + __builtin_convertvector(a.s1357, uint32x4_t); }
_MV_INLINE_DEFN uint64x2_t    vpaddlq_u32(uint32x4_t a)                   { return __builtin_convertvector(a.s02, uint64x2_t) + __builtin_convertvector(a.s13, uint64x2_t); }

/* Long pairwise add and accumulate */
/*		vpadal{q}_<type> */
_MV_INLINE_DEFN int16x4_t     vpadal_s8 (int16x4_t accum, int8x8_t a)     { return accum + vpaddl_s8(a); }
_MV_INLINE_DEFN int32x2_t     vpadal_s16(int32x2_t accum, int16x4_t a)    { return accum + vpaddl_s16(a); }
_MV_INLINE_DEFN int64x1_t     vpadal_s32(int64x1_t accum, int32x2_t a)    { return accum + vpaddl_s32(a); }
_MV_INLINE_DEFN uint16x4_t    vpadal_u8 (uint16x4_t accum, uint8x8_t a)   { return accum + vpaddl_u8(a); }
_MV_INLINE_DEFN uint32x2_t    vpadal_u16(uint32x2_t accum, uint16x4_t a)  { return accum + vpaddl_u16(a); }
_MV_INLINE_DEFN uint64x1_t    vpadal_u32(uint64x1_t accum, uint32x2_t a)  { return accum + vpaddl_u32(a); }

_MV_INLINE_DEFN int16x8_t     vpadalq_s8 (int16x8_t accum, int8x16_t a)   { return accum + vpaddlq_s8(a); }
_MV_INLINE_DEFN int32x4_t     vpadalq_s16(int32x4_t accum, int16x8_t a)   { return accum + vpaddlq_s16(a); }
_MV_INLINE_DEFN int64x2_t     vpadalq_s32(int64x2_t accum, int32x4_t a)   { return accum + vpaddlq_s32(a); }
_MV_INLINE_DEFN uint16x8_t    vpadalq_u8 (uint16x8_t accum, uint8x16_t a) { return accum + vpaddlq_u8(a); }
_MV_INLINE_DEFN uint32x4_t    vpadalq_u16(uint32x4_t accum, uint16x8_t a) { return accum + vpaddlq_u16(a); }
_MV_INLINE_DEFN uint64x2_t    vpadalq_u32(uint64x2_t accum, uint32x4_t a) { return accum + vpaddlq_u32(a); }


/* */
/* ARM Section 18.11 - NEON intrinsics for folding maximum */
/* */

/*		vpmax_<type> */
extern          int8x8_t      vpmax_s8 (int8x8_t, int8x8_t);
extern          int16x4_t     vpmax_s16(int16x4_t, int16x4_t);
extern          int32x2_t     vpmax_s32(int32x2_t, int32x2_t);
extern          float32x2_t   vpmax_f32(float32x2_t, float32x2_t);
extern          uint8x8_t     vpmax_u8 (uint8x8_t, uint8x8_t);
extern          uint16x4_t    vpmax_u16(uint16x4_t, uint16x4_t);
extern          uint32x2_t    vpmax_u32(uint32x2_t, uint32x2_t);


/* */
/* ARM Section 18.12 - NEON intrinsics for folding minimum */
/* */

/*		vpmin_<type> */
extern          int8x8_t      vpmin_s8 (int8x8_t, int8x8_t);
extern          int16x4_t     vpmin_s16(int16x4_t, int16x4_t);
extern          int32x2_t     vpmin_s32(int32x2_t, int32x2_t);
extern          float32x2_t   vpmin_f32(float32x2_t, float32x2_t);
extern          uint8x8_t     vpmin_u8 (uint8x8_t, uint8x8_t);
extern          uint16x4_t    vpmin_u16(uint16x4_t, uint16x4_t);
extern          uint32x2_t    vpmin_u32(uint32x2_t, uint32x2_t);


/* */
/* ARM Section 18.13 - NEON intrinsics for reciprocal and sqrt */
/* */

/* Reciprocal intrinsics using the Newton-Raphson method */
/*		vrecps{q}_<type> */
_MV_INLINE_DEFN float32x2_t   vrecps_f32(float32x2_t a, float32x2_t b)   { const float32x2_t two = 2.0f; return two - (a * b); }

_MV_INLINE_DEFN float32x4_t   vrecpsq_f32(float32x4_t a, float32x4_t b)  { const float32x4_t two = 2.0f; return two - (a * b); }

/* Square root intrinsics using the Newton-Raphson method */
/*		vrsqrts{q}_<type> */
_MV_INLINE_DEFN float32x2_t   vrsqrts_f32(float32x2_t a, float32x2_t b)  { const float32x2_t three = 3.0f; return (three - (a * b)) * 0.5f; }

_MV_INLINE_DEFN float32x4_t   vrsqrtsq_f32(float32x4_t a, float32x4_t b) { const float32x4_t three = 3.0f; return (three - (a * b)) * 0.5f; }


/* */
/* ARM Section 18.14 - NEON intrinsics for shifts by signed variable */
/* */

/* Vector shift left: (negative values shift right) */
/*		vshl{q}_<type>.Vr[i] := Va[i] << Vb[i] */
extern          int8x8_t      vshl_s8 (int8x8_t, int8x8_t);
extern          int16x4_t     vshl_s16(int16x4_t, int16x4_t);
extern          int32x2_t     vshl_s32(int32x2_t, int32x2_t);
extern          int64x1_t     vshl_s64(int64x1_t, int64x1_t);
extern          uint8x8_t     vshl_u8 (uint8x8_t, int8x8_t);
extern          uint16x4_t    vshl_u16(uint16x4_t, int16x4_t);
extern          uint32x2_t    vshl_u32(uint32x2_t, int32x2_t);
extern          uint64x1_t    vshl_u64(uint64x1_t, int64x1_t);

extern          int8x16_t     vshlq_s8 (int8x16_t, int8x16_t);
extern          int16x8_t     vshlq_s16(int16x8_t, int16x8_t);
extern          int32x4_t     vshlq_s32(int32x4_t, int32x4_t);
extern          int64x2_t     vshlq_s64(int64x2_t, int64x2_t);
extern          uint8x16_t    vshlq_u8 (uint8x16_t, int8x16_t);
extern          uint16x8_t    vshlq_u16(uint16x8_t, int16x8_t);
extern          uint32x4_t    vshlq_u32(uint32x4_t, int32x4_t);
extern          uint64x2_t    vshlq_u64(uint64x2_t, int64x2_t);

/* Vector saturating shift left: (negative values shift right) */
/*		vqshl{q}_<type> */
extern          int8x8_t      vqshl_s8 (int8x8_t, int8x8_t);
extern          int16x4_t     vqshl_s16(int16x4_t, int16x4_t);
extern          int32x2_t     vqshl_s32(int32x2_t, int32x2_t);
extern          int64x1_t     vqshl_s64(int64x1_t, int64x1_t);
extern          uint8x8_t     vqshl_u8 (uint8x8_t, int8x8_t);
extern          uint16x4_t    vqshl_u16(uint16x4_t, int16x4_t);
extern          uint32x2_t    vqshl_u32(uint32x2_t, int32x2_t);
extern          uint64x1_t    vqshl_u64(uint64x1_t, int64x1_t);

extern          int8x16_t     vqshlq_s8 (int8x16_t, int8x16_t);
extern          int16x8_t     vqshlq_s16(int16x8_t, int16x8_t);
extern          int32x4_t     vqshlq_s32(int32x4_t, int32x4_t);
extern          int64x2_t     vqshlq_s64(int64x2_t, int64x2_t);
extern          uint8x16_t    vqshlq_u8 (uint8x16_t, int8x16_t);
extern          uint16x8_t    vqshlq_u16(uint16x8_t, int16x8_t);
extern          uint32x4_t    vqshlq_u32(uint32x4_t, int32x4_t);
extern          uint64x2_t    vqshlq_u64(uint64x2_t, int64x2_t);

/* Vector rounding shift left: (negative values shift right) */
/*		vrshl{q}_<type> --- rounded shift left */
extern          int8x8_t      vrshl_s8 (int8x8_t, int8x8_t);
extern          int16x4_t     vrshl_s16(int16x4_t, int16x4_t);
extern          int32x2_t     vrshl_s32(int32x2_t, int32x2_t);
extern          int64x1_t     vrshl_s64(int64x1_t, int64x1_t);
extern          uint8x8_t     vrshl_u8 (uint8x8_t, int8x8_t);
extern          uint16x4_t    vrshl_u16(uint16x4_t, int16x4_t);
extern          uint32x2_t    vrshl_u32(uint32x2_t, int32x2_t);
extern          uint64x1_t    vrshl_u64(uint64x1_t, int64x1_t);

extern          int8x16_t     vrshlq_s8 (int8x16_t, int8x16_t);
extern          int16x8_t     vrshlq_s16(int16x8_t, int16x8_t);
extern          int32x4_t     vrshlq_s32(int32x4_t, int32x4_t);
extern          int64x2_t     vrshlq_s64(int64x2_t, int64x2_t);
extern          uint8x16_t    vrshlq_u8 (uint8x16_t, int8x16_t);
extern          uint16x8_t    vrshlq_u16(uint16x8_t, int16x8_t);
extern          uint32x4_t    vrshlq_u32(uint32x4_t, int32x4_t);
extern          uint64x2_t    vrshlq_u64(uint64x2_t, int64x2_t);

/* Vector saturating rounding shift left: (negative values shift right) */
/*		vqrshl{q}_<type> */
extern          int8x8_t      vqrshl_s8 (int8x8_t, int8x8_t);
extern          int16x4_t     vqrshl_s16(int16x4_t, int16x4_t);
extern          int32x2_t     vqrshl_s32(int32x2_t, int32x2_t);
extern          int64x1_t     vqrshl_s64(int64x1_t, int64x1_t);
extern          uint8x8_t     vqrshl_u8 (uint8x8_t, int8x8_t);
extern          uint16x4_t    vqrshl_u16(uint16x4_t, int16x4_t);
extern          uint32x2_t    vqrshl_u32(uint32x2_t, int32x2_t);
extern          uint64x1_t    vqrshl_u64(uint64x1_t, int64x1_t);

extern          int8x16_t     vqrshlq_s8 (int8x16_t, int8x16_t b);
extern          int16x8_t     vqrshlq_s16(int16x8_t, int16x8_t b);
extern          int32x4_t     vqrshlq_s32(int32x4_t, int32x4_t b);
extern          int64x2_t     vqrshlq_s64(int64x2_t, int64x2_t b);
extern          uint8x16_t    vqrshlq_u8 (uint8x16_t, int8x16_t);
extern          uint16x8_t    vqrshlq_u16(uint16x8_t, int16x8_t);
extern          uint32x4_t    vqrshlq_u32(uint32x4_t, int32x4_t);
extern          uint64x2_t    vqrshlq_u64(uint64x2_t, int64x2_t);

/* */
/* ARM Section 18.15 - NEON intrinsics for shifts by a constant */
/* */

/* Vector shift right by constant */
/*		vshr{q}_n_<type> */
_MV_INLINE_DEFN int8x8_t      vshr_n_s8 (int8x8_t a, int imm)    { return (imm == 8) ? (a < 0) : (a >> imm); }
_MV_INLINE_DEFN int16x4_t     vshr_n_s16(int16x4_t a, int imm)   { return (imm == 16) ? (a < 0) : (a >> imm); }
_MV_INLINE_DEFN int32x2_t     vshr_n_s32(int32x2_t a, int imm)   { return (imm == 32) ? (a < 0) : (a >> imm); }
_MV_INLINE_DEFN int64x1_t     vshr_n_s64(int64x1_t a, int imm)   { return (imm == 64) ? (a < 0) : (a >> imm); }
_MV_INLINE_DEFN uint8x8_t     vshr_n_u8 (uint8x8_t a, int imm)   { return (imm == 8) ? 0 : (a >> imm); }
_MV_INLINE_DEFN uint16x4_t    vshr_n_u16(uint16x4_t a, int imm)  { return (imm == 16) ? 0 : (a >> imm); }
_MV_INLINE_DEFN uint32x2_t    vshr_n_u32(uint32x2_t a, int imm)  { return (imm == 32) ? 0 : (a >> imm); }
_MV_INLINE_DEFN uint64x1_t    vshr_n_u64(uint64x1_t a, int imm)  { return (imm == 64) ? 0 : (a >> imm); }

_MV_INLINE_DEFN int8x16_t     vshrq_n_s8 (int8x16_t a, int imm)  { return (imm == 8) ? (a < 0) : (a >> imm); }
_MV_INLINE_DEFN int16x8_t     vshrq_n_s16(int16x8_t a, int imm)  { return (imm == 16) ? (a < 0) : (a >> imm); }
_MV_INLINE_DEFN int32x4_t     vshrq_n_s32(int32x4_t a, int imm)  { return (imm == 32) ? (a < 0) : (a >> imm); }
_MV_INLINE_DEFN int64x2_t     vshrq_n_s64(int64x2_t a, int imm)  { return (imm == 64) ? (a < 0) : (a >> imm); }
_MV_INLINE_DEFN uint8x16_t    vshrq_n_u8 (uint8x16_t a, int imm) { return (imm == 8) ? 0 : (a >> imm); }
_MV_INLINE_DEFN uint16x8_t    vshrq_n_u16(uint16x8_t a, int imm) { return (imm == 16) ? 0 : (a >> imm); }
_MV_INLINE_DEFN uint32x4_t    vshrq_n_u32(uint32x4_t a, int imm) { return (imm == 32) ? 0 : (a >> imm); }
_MV_INLINE_DEFN uint64x2_t    vshrq_n_u64(uint64x2_t a, int imm) { return (imm == 64) ? 0 : (a >> imm); }

/* Vector shift left by constant */
/*		vshl{q}_n_<type> */
_MV_INLINE_DEFN int8x8_t      vshl_n_s8 (int8x8_t a, int imm)    { return a << imm; }
_MV_INLINE_DEFN int16x4_t     vshl_n_s16(int16x4_t a, int imm)   { return a << imm; }
_MV_INLINE_DEFN int32x2_t     vshl_n_s32(int32x2_t a, int imm)   { return a << imm; }
_MV_INLINE_DEFN int64x1_t     vshl_n_s64(int64x1_t a, int imm)   { return a << imm; }
_MV_INLINE_DEFN uint8x8_t     vshl_n_u8 (uint8x8_t a, int imm)   { return a << imm; }
_MV_INLINE_DEFN uint16x4_t    vshl_n_u16(uint16x4_t a, int imm)  { return a << imm; }
_MV_INLINE_DEFN uint32x2_t    vshl_n_u32(uint32x2_t a, int imm)  { return a << imm; }
_MV_INLINE_DEFN uint64x1_t    vshl_n_u64(uint64x1_t a, int imm)  { return a << imm; }

_MV_INLINE_DEFN int8x16_t     vshlq_n_s8 (int8x16_t a, int imm)  { return a << imm; }
_MV_INLINE_DEFN int16x8_t     vshlq_n_s16(int16x8_t a, int imm)  { return a << imm; }
_MV_INLINE_DEFN int32x4_t     vshlq_n_s32(int32x4_t a, int imm)  { return a << imm; }
_MV_INLINE_DEFN int64x2_t     vshlq_n_s64(int64x2_t a, int imm)  { return a << imm; }
_MV_INLINE_DEFN uint8x16_t    vshlq_n_u8 (uint8x16_t a, int imm) { return a << imm; }
_MV_INLINE_DEFN uint16x8_t    vshlq_n_u16(uint16x8_t a, int imm) { return a << imm; }
_MV_INLINE_DEFN uint32x4_t    vshlq_n_u32(uint32x4_t a, int imm) { return a << imm; }
_MV_INLINE_DEFN uint64x2_t    vshlq_n_u64(uint64x2_t a, int imm) { return a << imm; }

/* Vector rounding shift right by constant */
/*		vrshr{q}_n_<type> */
extern          int8x8_t      vrshr_n_s8 (int8x8_t, int);
extern          int16x4_t     vrshr_n_s16(int16x4_t, int);
extern          int32x2_t     vrshr_n_s32(int32x2_t, int);
extern          int64x1_t     vrshr_n_s64(int64x1_t, int);
extern          uint8x8_t     vrshr_n_u8 (uint8x8_t, int);
extern          uint16x4_t    vrshr_n_u16(uint16x4_t, int);
extern          uint32x2_t    vrshr_n_u32(uint32x2_t, int);
extern          uint64x1_t    vrshr_n_u64(uint64x1_t, int);

extern          int8x16_t     vrshrq_n_s8 (int8x16_t, int);
extern          int16x8_t     vrshrq_n_s16(int16x8_t, int);
extern          int32x4_t     vrshrq_n_s32(int32x4_t, int);
extern          int64x2_t     vrshrq_n_s64(int64x2_t, int);
extern          uint8x16_t    vrshrq_n_u8 (uint8x16_t, int);
extern          uint16x8_t    vrshrq_n_u16(uint16x8_t, int);
extern          uint32x4_t    vrshrq_n_u32(uint32x4_t, int);
extern          uint64x2_t    vrshrq_n_u64(uint64x2_t, int);

/* Vector shift right by constant and accumulate */
/*		vsra{q}_n_<type> */
_MV_INLINE_DEFN int8x8_t      vsra_n_s8 (int8x8_t a, int8x8_t b, int n)      { return a + vshr_n_s8(b, n); }
_MV_INLINE_DEFN int16x4_t     vsra_n_s16(int16x4_t a, int16x4_t b, int n)    { return a + vshr_n_s16(b, n); }
_MV_INLINE_DEFN int32x2_t     vsra_n_s32(int32x2_t a, int32x2_t b, int n)    { return a + vshr_n_s32(b, n); }
_MV_INLINE_DEFN int64x1_t     vsra_n_s64(int64x1_t a, int64x1_t b, int n)    { return a + vshr_n_s64(b, n); }
_MV_INLINE_DEFN uint8x8_t     vsra_n_u8 (uint8x8_t a, uint8x8_t b, int n)    { return a + vshr_n_u8(b, n); }
_MV_INLINE_DEFN uint16x4_t    vsra_n_u16(uint16x4_t a, uint16x4_t b, int n)  { return a + vshr_n_u16(b, n); }
_MV_INLINE_DEFN uint32x2_t    vsra_n_u32(uint32x2_t a, uint32x2_t b, int n)  { return a + vshr_n_u32(b, n); }
_MV_INLINE_DEFN uint64x1_t    vsra_n_u64(uint64x1_t a, uint64x1_t b, int n)  { return a + vshr_n_u64(b, n); }

_MV_INLINE_DEFN int8x16_t     vsraq_n_s8 (int8x16_t a, int8x16_t b, int n)   { return a + vshrq_n_s8(b, n); }
_MV_INLINE_DEFN int16x8_t     vsraq_n_s16(int16x8_t a, int16x8_t b, int n)   { return a + vshrq_n_s16(b, n); }
_MV_INLINE_DEFN int32x4_t     vsraq_n_s32(int32x4_t a, int32x4_t b, int n)   { return a + vshrq_n_s32(b, n); }
_MV_INLINE_DEFN int64x2_t     vsraq_n_s64(int64x2_t a, int64x2_t b, int n)   { return a + vshrq_n_s64(b, n); }
_MV_INLINE_DEFN uint8x16_t    vsraq_n_u8 (uint8x16_t a, uint8x16_t b, int n) { return a + vshrq_n_u8(b, n); }
_MV_INLINE_DEFN uint16x8_t    vsraq_n_u16(uint16x8_t a, uint16x8_t b, int n) { return a + vshrq_n_u16(b, n); }
_MV_INLINE_DEFN uint32x4_t    vsraq_n_u32(uint32x4_t a, uint32x4_t b, int n) { return a + vshrq_n_u32(b, n); }
_MV_INLINE_DEFN uint64x2_t    vsraq_n_u64(uint64x2_t a, uint64x2_t b, int n) { return a + vshrq_n_u64(b, n); }

/* Vector rounding shift right by constant and accumulate */
/*		vrsra{q}_n_<type> */
_MV_INLINE_DEFN int8x8_t      vrsra_n_s8 (int8x8_t a, int8x8_t b, int n)     { return a + vrshr_n_s8(b, n); }
_MV_INLINE_DEFN int16x4_t     vrsra_n_s16(int16x4_t a, int16x4_t b, int n)   { return a + vrshr_n_s16(b, n); }
_MV_INLINE_DEFN int32x2_t     vrsra_n_s32(int32x2_t a, int32x2_t b, int n)   { return a + vrshr_n_s32(b, n); }
_MV_INLINE_DEFN int64x1_t     vrsra_n_s64(int64x1_t a, int64x1_t b, int n)   { return a + vrshr_n_s64(b, n); }
_MV_INLINE_DEFN uint8x8_t     vrsra_n_u8 (uint8x8_t a, uint8x8_t b, int n)   { return a + vrshr_n_u8(b, n); }
_MV_INLINE_DEFN uint16x4_t    vrsra_n_u16(uint16x4_t a, uint16x4_t b, int n) { return a + vrshr_n_u16(b, n); }
_MV_INLINE_DEFN uint32x2_t    vrsra_n_u32(uint32x2_t a, uint32x2_t b, int n) { return a + vrshr_n_u32(b, n); }
_MV_INLINE_DEFN uint64x1_t    vrsra_n_u64(uint64x1_t a, uint64x1_t b, int n) { return a + vrshr_n_u64(b, n); }

_MV_INLINE_DEFN int8x16_t     vrsraq_n_s8 (int8x16_t a, int8x16_t b, int n)  { return a + vrshrq_n_s8(b, n); }
_MV_INLINE_DEFN int16x8_t     vrsraq_n_s16(int16x8_t a, int16x8_t b, int n)  { return a + vrshrq_n_s16(b, n); }
_MV_INLINE_DEFN int32x4_t     vrsraq_n_s32(int32x4_t a, int32x4_t b, int n)  { return a + vrshrq_n_s32(b, n); }
_MV_INLINE_DEFN int64x2_t     vrsraq_n_s64(int64x2_t a, int64x2_t b, int n)  { return a + vrshrq_n_s64(b, n); }
_MV_INLINE_DEFN uint8x16_t    vrsraq_n_u8 (uint8x16_t a, uint8x16_t b,int n)  { return a + vrshrq_n_u8(b, n); }
_MV_INLINE_DEFN uint16x8_t    vrsraq_n_u16(uint16x8_t a, uint16x8_t b, int n) { return a + vrshrq_n_u16(b, n); }
_MV_INLINE_DEFN uint32x4_t    vrsraq_n_u32(uint32x4_t a, uint32x4_t b, int n) { return a + vrshrq_n_u32(b, n); }
_MV_INLINE_DEFN uint64x2_t    vrsraq_n_u64(uint64x2_t a, uint64x2_t b, int n) { return a + vrshrq_n_u64(b, n); }

/* Vector saturating shift left by constant */
/*		vqshl{q}_n_<type> */
_MV_INLINE_DEFN int8x8_t      vqshl_n_s8 (int8x8_t a, int imm)               { int8x16_t _a, result, two = 2, multiplier = (1 << ((imm < 7 ) ? imm : 6)); _a.lo = a; result = __builtin_shave_vau_imuls_i8_rr(_a, multiplier); if (imm == 7) result = __builtin_shave_vau_imuls_i8_rr(result, two); return result.lo; }
_MV_INLINE_DEFN int16x4_t     vqshl_n_s16(int16x4_t a, int imm)              { int16x8_t _a, result, two = 2, multiplier = (1 << ((imm < 15 ) ? imm : 14)); _a.lo = a; result = __builtin_shave_vau_imuls_i16_rr(_a, multiplier); if (imm == 15) result = __builtin_shave_vau_imuls_i16_rr(result, two); return result.lo; }
_MV_INLINE_DEFN int32x2_t     vqshl_n_s32(int32x2_t a, int imm)              { int32x4_t _a, result, two = 2, multiplier = (1 << ((imm < 31 ) ? imm : 30)); _a.lo = a; result = __builtin_shave_vau_imuls_i32_rr(_a, multiplier); if (imm == 31) result = __builtin_shave_vau_imuls_i32_rr(result, two); return result.lo; }
extern          int64x1_t     vqshl_n_s64(int64x1_t, int);
_MV_INLINE_DEFN uint8x8_t     vqshl_n_u8 (uint8x8_t a, int imm)              { uint8x16_t _a, result, multiplier = (1u << imm); _a.lo = a; result = __builtin_shave_vau_imuls_u8_rr(_a, multiplier); return result.lo; }
_MV_INLINE_DEFN uint16x4_t    vqshl_n_u16(uint16x4_t a, int imm)             { uint16x8_t _a, result, multiplier = (1u << imm); _a.lo = a; result = __builtin_shave_vau_imuls_u16_rr(_a, multiplier); return result.lo; }
_MV_INLINE_DEFN uint32x2_t    vqshl_n_u32(uint32x2_t a, int imm)             { uint32x4_t _a, result, multiplier = (1u << imm); _a.lo = a; result = __builtin_shave_vau_imuls_u32_rr(_a, multiplier); return result.lo; }
extern          uint64x1_t    vqshl_n_u64(uint64x1_t, int);

_MV_INLINE_DEFN int8x16_t     vqshlq_n_s8 (int8x16_t a, int imm)             { int8x16_t result, two = 2, multiplier = (1 << ((imm < 7 ) ? imm : 6)); result = __builtin_shave_vau_imuls_i8_rr(a, multiplier); if (imm == 7) result = __builtin_shave_vau_imuls_i8_rr(result, two); return result; }
_MV_INLINE_DEFN int16x8_t     vqshlq_n_s16(int16x8_t a, int imm)             { int16x8_t result, two = 2, multiplier = (1 << ((imm < 15 ) ? imm : 14)); result = __builtin_shave_vau_imuls_i16_rr(a, multiplier); if (imm == 15) result = __builtin_shave_vau_imuls_i16_rr(result, two); return result; }
_MV_INLINE_DEFN int32x4_t     vqshlq_n_s32(int32x4_t a, int imm)             { int32x4_t result, two = 2, multiplier = (1 << ((imm < 31 ) ? imm : 30)); result = __builtin_shave_vau_imuls_i32_rr(a, multiplier); if (imm == 31) result = __builtin_shave_vau_imuls_i32_rr(result, two); return result; }
extern          int64x2_t     vqshlq_n_s64(int64x2_t, int);
_MV_INLINE_DEFN uint8x16_t    vqshlq_n_u8 (uint8x16_t a, int imm)            { uint8x16_t multiplier = (1u << imm); return __builtin_shave_vau_imuls_u8_rr(a, multiplier); }
_MV_INLINE_DEFN uint16x8_t    vqshlq_n_u16(uint16x8_t a, int imm)            { uint16x8_t multiplier = (1u << imm); return __builtin_shave_vau_imuls_u16_rr(a, multiplier); }
_MV_INLINE_DEFN uint32x4_t    vqshlq_n_u32(uint32x4_t a, int imm)            { uint32x4_t multiplier = (1u << imm); return __builtin_shave_vau_imuls_u32_rr(a, multiplier); }
extern          uint64x2_t    vqshlq_n_u64(uint64x2_t, int);

/* Vector signed->unsigned saturating shift left by constant */
/*		vqshlu{q}_n_<type> */
_MV_INLINE_DEFN uint8x8_t     vqshlu_n_s8 (int8x8_t a, int imm)              { int8x16_t _a; _a.lo = a; uint8x16_t result = vqshlq_n_u8(__builtin_astype(__builtin_shave_vau_and_16_rr(_a, (_a >= 0)), uint8x16_t), imm); return result.lo; }
_MV_INLINE_DEFN uint16x4_t    vqshlu_n_s16(int16x4_t a, int imm)             { int16x8_t _a; _a.lo = a; uint16x8_t result = vqshlq_n_u16(__builtin_astype(__builtin_shave_vau_and_16_rr(_a, (_a >= 0)), uint16x8_t), imm); return result.lo; }
_MV_INLINE_DEFN uint32x2_t    vqshlu_n_s32(int32x2_t a, int imm)             { int32x4_t _a; _a.lo = a; uint32x4_t result = vqshlq_n_u32(__builtin_astype(__builtin_shave_vau_and_32_rr(_a, (_a >= 0)), uint32x4_t), imm); return result.lo; }
_MV_INLINE_DEFN uint64x1_t    vqshlu_n_s64(int64x1_t a, int imm)             { return (a.x < 0) ? 0ull : vqshl_n_u64((uint64x1_t)a, imm); }

_MV_INLINE_DEFN uint8x16_t    vqshluq_n_s8 (int8x16_t a, int imm)            { return vqshlq_n_u8(__builtin_astype(__builtin_shave_vau_and_16_rr(a, (a >= 0)), uint8x16_t), imm); }
_MV_INLINE_DEFN uint16x8_t    vqshluq_n_s16(int16x8_t a, int imm)            { return vqshlq_n_u16(__builtin_astype(__builtin_shave_vau_and_16_rr(a, (a >= 0)), uint16x8_t), imm); }
_MV_INLINE_DEFN uint32x4_t    vqshluq_n_s32(int32x4_t a, int imm)            { return vqshlq_n_u32(__builtin_astype(__builtin_shave_vau_and_32_rr(a, (a >= 0)), uint32x4_t), imm); }
_MV_INLINE_DEFN uint64x2_t    vqshluq_n_s64(int64x2_t a, int imm)            { uint64x2_t result = { vqshlu_n_s64((int64x1_t)a[0], imm).x, vqshlu_n_s64((int64x1_t)a[1], imm).x }; return result; }

/* Vector narrowing shift right by constant */
/*		vshrn_n_<type> */
_MV_INLINE_DEFN int8x8_t      vshrn_n_s16(int16x8_t a, int imm)              { return __builtin_convertvector(vshrq_n_s16(a, imm), int8x8_t); }
_MV_INLINE_DEFN int16x4_t     vshrn_n_s32(int32x4_t a, int imm)              { return __builtin_convertvector(vshrq_n_s32(a, imm), int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vshrn_n_s64(int64x2_t a, int imm)              { return __builtin_convertvector(vshrq_n_s64(a, imm), int32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vshrn_n_u16(uint16x8_t a, int imm)             { return __builtin_convertvector(vshrq_n_u16(a, imm), uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vshrn_n_u32(uint32x4_t a, int imm)             { return __builtin_convertvector(vshrq_n_u32(a, imm), uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vshrn_n_u64(uint64x2_t a, int imm)             { return __builtin_convertvector(vshrq_n_u64(a, imm), uint32x2_t); }

/* Vector signed->unsigned narrowing saturating shift right by constant */
/*		vqshrun_n_<type> */
_MV_INLINE_DEFN uint8x8_t     vqshrun_n_s16(int16x8_t a, int imm)            { int16x8_t result = a & ((a & 0x8000) == 0); result >>= imm; result = ((result & 0xFF00) != 0) | result; return __builtin_convertvector(result, uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vqshrun_n_s32(int32x4_t a, int imm)            { int32x4_t result = a & ((a & 0x80000000) == 0); result >>= imm; result = ((result & 0xFFFF0000) != 0) | result; return __builtin_convertvector(result, uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vqshrun_n_s64(int64x2_t a, int imm)            { int64x2_t result = a & ((a & 0x8000000000000000) == 0); result >>= imm; result = ((result & 0xFFFFFFFF00000000ll) != 0) | result; return __builtin_convertvector(result, uint32x2_t); }

/* Vector signed->unsigned rounding narrowing saturating shift right by constant */
/*		vqrshrun_n_<type> */
extern          uint8x8_t     vqrshrun_n_s16(int16x8_t, int);
extern          uint16x4_t    vqrshrun_n_s32(int32x4_t, int);
extern          uint32x2_t    vqrshrun_n_s64(int64x2_t, int);

/* Vector narrowing saturating shift right by constant */
/*		vqshrn_n_<type> */
extern          int8x8_t      vqshrn_n_s16(int16x8_t, int);
extern          int16x4_t     vqshrn_n_s32(int32x4_t, int);
extern          int32x2_t     vqshrn_n_s64(int64x2_t, int);
extern          uint8x8_t     vqshrn_n_u16(uint16x8_t, int);
extern          uint16x4_t    vqshrn_n_u32(uint32x4_t, int);
extern          uint32x2_t    vqshrn_n_u64(uint64x2_t, int);

/* Vector rounding narrowing shift right by constant */
/*		vrshrn_n_<type> */
extern          int8x8_t      vrshrn_n_s16(int16x8_t, int);
extern          int16x4_t     vrshrn_n_s32(int32x4_t, int);
extern          int32x2_t     vrshrn_n_s64(int64x2_t, int);
extern          uint8x8_t     vrshrn_n_u16(uint16x8_t, int);
extern          uint16x4_t    vrshrn_n_u32(uint32x4_t, int);
extern          uint32x2_t    vrshrn_n_u64(uint64x2_t, int);

/* Vector rounding narrowing saturating shift right by constant */
/*		vqrshrn_n_<type> */
extern          int8x8_t      vqrshrn_n_s16(int16x8_t, int);
extern          int16x4_t     vqrshrn_n_s32(int32x4_t, int);
extern          int32x2_t     vqrshrn_n_s64(int64x2_t, int);
extern          uint8x8_t     vqrshrn_n_u16(uint16x8_t, int);
extern          uint16x4_t    vqrshrn_n_u32(uint32x4_t, int);
extern          uint32x2_t    vqrshrn_n_u64(uint64x2_t, int);

/* Vector widening shift left by constant */
/*		vshll_n_<type> */
_MV_INLINE_DEFN int16x8_t     vshll_n_s8 (int8x8_t a, int imm)   { return __builtin_convertvector(a, int16x8_t) << imm; }
_MV_INLINE_DEFN int32x4_t     vshll_n_s16(int16x4_t a, int imm)  { return __builtin_convertvector(a, int32x4_t) << imm; }
_MV_INLINE_DEFN int64x2_t     vshll_n_s32(int32x2_t a, int imm)  { return __builtin_convertvector(a, int64x2_t) << imm; }
_MV_INLINE_DEFN uint16x8_t    vshll_n_u8 (uint8x8_t a, int imm)  { return __builtin_convertvector(a, uint16x8_t) << imm; }
_MV_INLINE_DEFN uint32x4_t    vshll_n_u16(uint16x4_t a, int imm) { return __builtin_convertvector(a, uint32x4_t) << imm; }
_MV_INLINE_DEFN uint64x2_t    vshll_n_u32(uint32x2_t a, int imm) { return __builtin_convertvector(a, uint64x2_t) << imm; }


/* */
/* ARM Section 18.16 - NEON intrinsics for shifts with insert */
/* */

/* Vector shift right and insert */
/*		vsri{q}_n_<type> */
_MV_INLINE_DEFN int8x8_t      vsri_n_s8 (int8x8_t a, int8x8_t b, int imm)      { if (imm == 8) return a; uint8x8_t _a = __builtin_astype(a, uint8x8_t), _b = __builtin_astype(b, uint8x8_t), mask = (~0u << (8 - imm)); return __builtin_astype(((_a & mask) | (_b >> imm)), int8x8_t); }
_MV_INLINE_DEFN int16x4_t     vsri_n_s16(int16x4_t a, int16x4_t b, int imm)    { if (imm == 16) return a; uint16x4_t _a = __builtin_astype(a, uint16x4_t), _b = __builtin_astype(b, uint16x4_t), mask = (~0u << (16 - imm)); return __builtin_astype(((_a & mask) | (_b >> imm)), int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vsri_n_s32(int32x2_t a, int32x2_t b, int imm)    { if (imm == 32) return a; uint32x2_t _a = __builtin_astype(a, uint32x2_t), _b = __builtin_astype(b, uint32x2_t), mask = (~0u << (32 - imm)); return __builtin_astype(((_a & mask) | (_b >> imm)), int32x2_t); }
_MV_INLINE_DEFN int64x1_t     vsri_n_s64(int64x1_t a, int64x1_t b, int imm)    { if (imm == 64) return a; uint64x1_t _a = __builtin_astype(a, uint64x1_t), _b = __builtin_astype(b, uint64x1_t), mask = (~0ull << (64 - imm)); return __builtin_astype(((_a & mask) | (_b >> imm)), int64x1_t); }
_MV_INLINE_DEFN uint8x8_t     vsri_n_u8 (uint8x8_t a, uint8x8_t b, int imm)    { if (imm == 8) return a; uint8x8_t mask = (~0u << (8 - imm)); return ((a & mask) | (b >> imm)); }
_MV_INLINE_DEFN uint16x4_t    vsri_n_u16(uint16x4_t a, uint16x4_t b, int imm)  { if (imm == 16) return a; uint16x4_t mask = (~0u << (16 - imm)); return ((a & mask) | (b >> imm)); }
_MV_INLINE_DEFN uint32x2_t    vsri_n_u32(uint32x2_t a, uint32x2_t b, int imm)  { if (imm == 32) return a; uint32x2_t mask = (~0u << (32 - imm)); return ((a & mask) | (b >> imm)); }
_MV_INLINE_DEFN uint64x1_t    vsri_n_u64(uint64x1_t a, uint64x1_t b, int imm)  { if (imm == 64) return a; uint64x1_t mask = (~0ull << (64 - imm)); return ((a & mask) | (b >> imm)); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vsri_n_p8 (poly8x8_t, poly8x8_t, int);
extern          poly16x4_t    vsri_n_p16(poly16x4_t, poly16x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vsriq_n_s8 (int8x16_t a, int8x16_t b, int imm)   { if (imm == 8) return a; uint8x16_t _a = __builtin_astype(a, uint8x16_t), _b = __builtin_astype(b, uint8x16_t), mask = (~0u << (8 - imm)); return __builtin_astype(((_a & mask) | (_b >> imm)), int8x16_t); }
_MV_INLINE_DEFN int16x8_t     vsriq_n_s16(int16x8_t a, int16x8_t b, int imm)   { if (imm == 16) return a; uint16x8_t _a = __builtin_astype(a, uint16x8_t), _b = __builtin_astype(b, uint16x8_t), mask = (~0u << (16 - imm)); return __builtin_astype(((_a & mask) | (_b >> imm)), int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vsriq_n_s32(int32x4_t a, int32x4_t b, int imm)   { if (imm == 32) return a; uint32x4_t _a = __builtin_astype(a, uint32x4_t), _b = __builtin_astype(b, uint32x4_t), mask = (~0u << (32 - imm)); return __builtin_astype(((_a & mask) | (_b >> imm)), int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vsriq_n_s64(int64x2_t a, int64x2_t b, int imm)   { if (imm == 64) return a; uint64x2_t _a = __builtin_astype(a, uint64x2_t), _b = __builtin_astype(b, uint64x2_t), mask = (~0ull << (64 - imm)); return __builtin_astype(((_a & mask) | (_b >> imm)), int64x2_t); }
_MV_INLINE_DEFN uint8x16_t    vsriq_n_u8 (uint8x16_t a, uint8x16_t b, int imm) { if (imm == 8) return a; uint8x16_t mask = (~0u << (8 - imm)); return ((a & mask) | (b >> imm)); }
_MV_INLINE_DEFN uint16x8_t    vsriq_n_u16(uint16x8_t a, uint16x8_t b, int imm) { if (imm == 16) return a; uint16x8_t mask = (~0u << (16 - imm)); return ((a & mask) | (b >> imm)); }
_MV_INLINE_DEFN uint32x4_t    vsriq_n_u32(uint32x4_t a, uint32x4_t b, int imm) { if (imm == 32) return a; uint32x4_t mask = (~0u << (32 - imm)); return ((a & mask) | (b >> imm)); }
_MV_INLINE_DEFN uint64x2_t    vsriq_n_u64(uint64x2_t a, uint64x2_t b, int imm) { if (imm == 64) return a; uint64x2_t mask = (~0ull << (64 - imm)); return ((a & mask) | (b >> imm)); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vsriq_n_p8 (poly8x16_t, poly8x16_t, int);
extern          poly16x8_t    vsriq_n_p16(poly16x8_t, poly16x8_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Vector shift left and insert */
/*		vsli{q}_n_<type> */
_MV_INLINE_DEFN int8x8_t      vsli_n_s8 (int8x8_t a, int8x8_t b, int imm)      { if (imm == 0) return a; uint8x8_t _a = __builtin_astype(a, uint8x8_t), _b = __builtin_astype(b, uint8x8_t), mask = ((1u << imm) - 1u); return __builtin_astype(((_a & mask) | (_b << imm)), int8x8_t); }
_MV_INLINE_DEFN int16x4_t     vsli_n_s16(int16x4_t a, int16x4_t b, int imm)    { if (imm == 0) return a; uint16x4_t _a = __builtin_astype(a, uint16x4_t), _b = __builtin_astype(b, uint16x4_t), mask = ((1u << imm) - 1u); return __builtin_astype(((_a & mask) | (_b << imm)), int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vsli_n_s32(int32x2_t a, int32x2_t b, int imm)    { if (imm == 0) return a; uint32x2_t _a = __builtin_astype(a, uint32x2_t), _b = __builtin_astype(b, uint32x2_t), mask = ((1u << imm) - 1u); return __builtin_astype(((_a & mask) | (_b << imm)), int32x2_t); }
_MV_INLINE_DEFN int64x1_t     vsli_n_s64(int64x1_t a, int64x1_t b, int imm)    { if (imm == 0) return a; uint64x1_t _a = __builtin_astype(a, uint64x1_t), _b = __builtin_astype(b, uint64x1_t), mask = ((1u << imm) - 1u); return __builtin_astype(((_a & mask) | (_b << imm)), int64x1_t); }
_MV_INLINE_DEFN uint8x8_t     vsli_n_u8 (uint8x8_t a, uint8x8_t b, int imm)    { if (imm == 0) return a; uint8x8_t mask = ((1u << imm) - 1u); return ((a & mask) | (b << imm)); }
_MV_INLINE_DEFN uint16x4_t    vsli_n_u16(uint16x4_t a, uint16x4_t b, int imm)  { if (imm == 0) return a; uint16x4_t mask = ((1u << imm) - 1u); return ((a & mask) | (b << imm)); }
_MV_INLINE_DEFN uint32x2_t    vsli_n_u32(uint32x2_t a, uint32x2_t b, int imm)  { if (imm == 0) return a; uint32x2_t mask = ((1u << imm) - 1u); return ((a & mask) | (b << imm)); }
_MV_INLINE_DEFN uint64x1_t    vsli_n_u64(uint64x1_t a, uint64x1_t b, int imm)  { if (imm == 0) return a; uint64x1_t mask = ((1ull << imm) - 1ull); return ((a & mask) | (b << imm)); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vsli_n_p8 (poly8x8_t, poly8x8_t, int);
extern          poly16x4_t    vsli_n_p16(poly16x4_t, poly16x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vsliq_n_s8 (int8x16_t a, int8x16_t b, int imm)   { if (imm == 0) return a; uint8x16_t _a = __builtin_astype(a, uint8x16_t), _b = __builtin_astype(b, uint8x16_t), mask = ((1u << imm) - 1u); return __builtin_astype(((_a & mask) | (_b << imm)), int8x16_t); }
_MV_INLINE_DEFN int16x8_t     vsliq_n_s16(int16x8_t a, int16x8_t b, int imm)   { if (imm == 0) return a; uint16x8_t _a = __builtin_astype(a, uint16x8_t), _b = __builtin_astype(b, uint16x8_t), mask = ((1u << imm) - 1u); return __builtin_astype(((_a & mask) | (_b << imm)), int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vsliq_n_s32(int32x4_t a, int32x4_t b, int imm)   { if (imm == 0) return a; uint32x4_t _a = __builtin_astype(a, uint32x4_t), _b = __builtin_astype(b, uint32x4_t), mask = ((1u << imm) - 1u); return __builtin_astype(((_a & mask) | (_b << imm)), int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vsliq_n_s64(int64x2_t a, int64x2_t b, int imm)   { if (imm == 0) return a; uint64x2_t _a = __builtin_astype(a, uint64x2_t), _b = __builtin_astype(b, uint64x2_t), mask = ((1u << imm) - 1u); return __builtin_astype(((_a & mask) | (_b << imm)), int64x2_t); }
_MV_INLINE_DEFN uint8x16_t    vsliq_n_u8 (uint8x16_t a, uint8x16_t b, int imm) { if (imm == 0) return a; uint8x16_t mask = ((1u << imm) - 1u); return ((a & mask) | (b << imm)); }
_MV_INLINE_DEFN uint16x8_t    vsliq_n_u16(uint16x8_t a, uint16x8_t b, int imm) { if (imm == 0) return a; uint16x8_t mask = ((1u << imm) - 1u); return ((a & mask) | (b << imm)); }
_MV_INLINE_DEFN uint32x4_t    vsliq_n_u32(uint32x4_t a, uint32x4_t b, int imm) { if (imm == 0) return a; uint32x4_t mask = ((1u << imm) - 1u); return ((a & mask) | (b << imm)); }
_MV_INLINE_DEFN uint64x2_t    vsliq_n_u64(uint64x2_t a, uint64x2_t b, int imm) { if (imm == 0) return a; uint64x2_t mask = ((1ull << imm) - 1ull); return ((a & mask) | (b << imm)); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vsliq_n_p8 (poly8x16_t, poly8x16_t, int);
extern          poly16x8_t    vsliq_n_p16(poly16x8_t, poly16x8_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.17 - NEON intrinsics for loading a single vector or lane */
/* */

/* Load a single vector from memory */
/*		vldl1{q}_<type> */
_MV_INLINE_DEFN int8x8_t      vld1_s8 (const int8_t *p)         { return *((int8x8_t*)p); }
_MV_INLINE_DEFN int16x4_t     vld1_s16(const int16_t *p)        { return *((int16x4_t*)p); }
_MV_INLINE_DEFN int32x2_t     vld1_s32(const int32_t *p)        { return *((int32x2_t*)p); }
_MV_INLINE_DEFN int64x1_t     vld1_s64(const int64_t *p)        { return *((int64x1_t*)p); }
_MV_INLINE_DEFN float16x4_t   vld1_f16(const float16_t *p)      { return *((float16x4_t*)p); }
_MV_INLINE_DEFN float32x2_t   vld1_f32(const float32_t *p)      { return *((float32x2_t*)p); }
_MV_INLINE_DEFN uint8x8_t     vld1_u8 (const uint8_t *p)        { return *((uint8x8_t*)p); }
_MV_INLINE_DEFN uint16x4_t    vld1_u16(const uint16_t *p)       { return *((uint16x4_t*)p); }
_MV_INLINE_DEFN uint32x2_t    vld1_u32(const uint32_t *p)       { return *((uint32x2_t*)p); }
_MV_INLINE_DEFN uint64x1_t    vld1_u64(const uint64_t *p)       { return *((uint64x1_t*)p); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vld1_p8 (const poly8_t *);
extern          poly16x4_t    vld1_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vld1q_s8 (const int8_t *p)        { return *((int8x16_t*)p); }
_MV_INLINE_DEFN int16x8_t     vld1q_s16(const int16_t *p)       { return *((int16x8_t*)p); }
_MV_INLINE_DEFN int32x4_t     vld1q_s32(const int32_t *p)       { return *((int32x4_t*)p); }
_MV_INLINE_DEFN int64x2_t     vld1q_s64(const int64_t *p)       { return *((int64x2_t*)p); }
_MV_INLINE_DEFN float16x8_t   vld1q_f16(const float16_t *p)     { return *((float16x8_t*)p); }
_MV_INLINE_DEFN float32x4_t   vld1q_f32(const float32_t *p)     { return *((float32x4_t*)p); }
_MV_INLINE_DEFN uint8x16_t    vld1q_u8 (const uint8_t *p)       { return *((uint8x16_t*)p); }
_MV_INLINE_DEFN uint16x8_t    vld1q_u16(const uint16_t *p)      { return *((uint16x8_t*)p); }
_MV_INLINE_DEFN uint32x4_t    vld1q_u32(const uint32_t *p)      { return *((uint32x4_t*)p); }
_MV_INLINE_DEFN uint64x2_t    vld1q_u64(const uint64_t *p)      { return *((uint64x2_t*)p); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vld1q_p8 (const poly8_t *);
extern          poly16x8_t    vld1q_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Load a single lane from memory */
/*		vld1{q}_lane_<type> */
_MV_INLINE_DEFN int8x8_t      vld1_lane_s8 (const int8_t *p, int8x8_t vec, int lane)        { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN int16x4_t     vld1_lane_s16(const int16_t *p, int16x4_t vec, int lane)      { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN int32x2_t     vld1_lane_s32(const int32_t *p, int32x2_t vec, int lane)      { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN int64x1_t     vld1_lane_s64(const int64_t *p, int64x1_t vec, int lane)      { return *p; }
_MV_INLINE_DEFN float16x4_t   vld1_lane_f16(const float16_t *p, float16x4_t vec, int lane)  { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN float32x2_t   vld1_lane_f32(const float32_t *p, float32x2_t vec, int lane)  { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN uint8x8_t     vld1_lane_u8 (const uint8_t *p, uint8x8_t vec, int lane)      { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN uint16x4_t    vld1_lane_u16(const uint16_t *p, uint16x4_t vec, int lane)    { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN uint32x2_t    vld1_lane_u32(const uint32_t *p, uint32x2_t vec, int lane)    { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN uint64x1_t    vld1_lane_u64(const uint64_t *p, uint64x1_t vec, int lane)    { return *p; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vld1_lane_p8 (const poly8_t *, poly8x8_t, int);
extern          poly16x4_t    vld1_lane_p16(const poly16_t *, poly16x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vld1q_lane_s8 (const int8_t *p, int8x16_t vec, int lane)      { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN int16x8_t     vld1q_lane_s16(const int16_t *p, int16x8_t vec, int lane)     { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN int32x4_t     vld1q_lane_s32(const int32_t *p, int32x4_t vec, int lane)     { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN int64x2_t     vld1q_lane_s64(const int64_t *p, int64x2_t vec, int lane)     { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN float16x8_t   vld1q_lane_f16(const float16_t *p, float16x8_t vec, int lane) { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN float32x4_t   vld1q_lane_f32(const float32_t *p, float32x4_t vec, int lane) { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN uint8x16_t    vld1q_lane_u8 (const uint8_t *p, uint8x16_t vec, int lane)    { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN uint16x8_t    vld1q_lane_u16(const uint16_t *p, uint16x8_t vec, int lane)   { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN uint32x4_t    vld1q_lane_u32(const uint32_t *p, uint32x4_t vec, int lane)   { vec[lane] = *p; return vec; }
_MV_INLINE_DEFN uint64x2_t    vld1q_lane_u64(const uint64_t *p, uint64x2_t vec, int lane)   { vec[lane] = *p; return vec; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vld1q_lane_p8 (const poly8_t *, poly8x16_t, int);
extern          poly16x8_t    vld1q_lane_p16(const poly16_t *, poly16x8_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Load all lanes of vector with same value from memory */
/*		vld1{q}_dup_<type> */
_MV_INLINE_DEFN int8x8_t      vld1_dup_s8 (const int8_t *p)     { return *p; }
_MV_INLINE_DEFN int16x4_t     vld1_dup_s16(const int16_t *p)    { return *p; }
_MV_INLINE_DEFN int32x2_t     vld1_dup_s32(const int32_t *p)    { return *p; }
_MV_INLINE_DEFN int64x1_t     vld1_dup_s64(const int64_t *p)    { return *p; }
_MV_INLINE_DEFN float16x4_t   vld1_dup_f16(const float16_t *p)  { return *p; }
_MV_INLINE_DEFN float32x2_t   vld1_dup_f32(const float32_t *p)  { return *p; }
_MV_INLINE_DEFN uint8x8_t     vld1_dup_u8 (const uint8_t *p)    { return *p; }
_MV_INLINE_DEFN uint16x4_t    vld1_dup_u16(const uint16_t *p)   { return *p; }
_MV_INLINE_DEFN uint32x2_t    vld1_dup_u32(const uint32_t *p)   { return *p; }
_MV_INLINE_DEFN uint64x1_t    vld1_dup_u64(const uint64_t *p)   { return *p; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vld1_dup_p8 (const poly8_t *);
extern          poly16x4_t    vld1_dup_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vld1q_dup_s8 (const int8_t *p)    { return *p; }
_MV_INLINE_DEFN int16x8_t     vld1q_dup_s16(const int16_t *p)   { return *p; }
_MV_INLINE_DEFN int32x4_t     vld1q_dup_s32(const int32_t *p)   { return *p; }
_MV_INLINE_DEFN int64x2_t     vld1q_dup_s64(const int64_t *p)   { return *p; }
_MV_INLINE_DEFN float16x8_t   vld1q_dup_f16(const float16_t *p) { return *p; }
_MV_INLINE_DEFN float32x4_t   vld1q_dup_f32(const float32_t *p) { return *p; }
_MV_INLINE_DEFN uint8x16_t    vld1q_dup_u8 (const uint8_t *p)   { return *p; }
_MV_INLINE_DEFN uint16x8_t    vld1q_dup_u16(const uint16_t *p)  { return *p; }
_MV_INLINE_DEFN uint32x4_t    vld1q_dup_u32(const uint32_t *p)  { return *p; }
_MV_INLINE_DEFN uint64x2_t    vld1q_dup_u64(const uint64_t *p)  { return *p; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vld1q_dup_p8 (const poly8_t *);
extern          poly16x8_t    vld1q_dup_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.18 - NEON intrinsics for storing a single vector or lane */
/* */

/* Store a single vector into memory */
/*		vst1{q}_<type> */
_MV_INLINE_DEFN void          vst1_s8 (int8_t *p, int8x8_t value)        { *((int8x8_t*)p) = value; }
_MV_INLINE_DEFN void          vst1_s16(int16_t *p, int16x4_t value)      { *((int16x4_t*)p) = value; }
_MV_INLINE_DEFN void          vst1_s32(int32_t *p, int32x2_t value)      { *((int32x2_t*)p) = value; }
_MV_INLINE_DEFN void          vst1_s64(int64_t *p, int64x1_t value)      { *((int64x1_t*)p) = value; }
_MV_INLINE_DEFN void          vst1_f16(float16_t *p, float16x4_t value)  { *((float16x4_t*)p) = value; }
_MV_INLINE_DEFN void          vst1_f32(float32_t *p, float32x2_t value)  { *((float32x2_t*)p) = value; }
_MV_INLINE_DEFN void          vst1_u8 (uint8_t *p, uint8x8_t value)      { *((uint8x8_t*)p) = value; }
_MV_INLINE_DEFN void          vst1_u16(uint16_t *p, uint16x4_t value)    { *((uint16x4_t*)p) = value; }
_MV_INLINE_DEFN void          vst1_u32(uint32_t *p, uint32x2_t value)    { *((uint32x2_t*)p) = value; }
_MV_INLINE_DEFN void          vst1_u64(uint64_t *p, uint64x1_t value)    { *((uint64x1_t*)p) = value; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst1_p8 (poly8_t *, poly8x8_t);
extern          void          vst1_p16(poly16_t *, poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN void          vst1q_s8 (int8_t *p, int8x16_t value)      { *((int8x16_t*)p) = value; }
_MV_INLINE_DEFN void          vst1q_s16(int16_t *p, int16x8_t value)     { *((int16x8_t*)p) = value; }
_MV_INLINE_DEFN void          vst1q_s32(int32_t *p, int32x4_t value)     { *((int32x4_t*)p) = value; }
_MV_INLINE_DEFN void          vst1q_s64(int64_t *p, int64x2_t value)     { *((int64x2_t*)p) = value; }
_MV_INLINE_DEFN void          vst1q_f16(float16_t *p, float16x8_t value) {*((float16x8_t*)p) = value; }
_MV_INLINE_DEFN void          vst1q_f32(float32_t *p, float32x4_t value) { *((float32x4_t*)p) = value; }
_MV_INLINE_DEFN void          vst1q_u8 (uint8_t *p, uint8x16_t value)    { *((uint8x16_t*)p) = value; }
_MV_INLINE_DEFN void          vst1q_u16(uint16_t *p, uint16x8_t value)   { *((uint16x8_t*)p) = value; }
_MV_INLINE_DEFN void          vst1q_u32(uint32_t *p, uint32x4_t value)   { *((uint32x4_t*)p) = value; }
_MV_INLINE_DEFN void          vst1q_u64(uint64_t *p, uint64x2_t value)   { *((uint64x2_t*)p) = value; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst1q_p8 (poly8_t *, poly8x16_t);
extern          void          vst1q_p16(poly16_t *, poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Store a lane of a vector into memory */
/*		vst1{q}_lane_<type> */
_MV_INLINE_DEFN void          vst1_lane_s8 (int8_t *p, int8x8_t vec, int lane)        { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1_lane_s16(int16_t *p, int16x4_t vec, int lane)      { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1_lane_s32(int32_t *p, int32x2_t vec, int lane)      { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1_lane_s64(int64_t *p, int64x1_t vec, int lane)      { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1_lane_f16(float16_t *p, float16x4_t vec, int lane)  { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1_lane_f32(float32_t *p, float32x2_t vec, int lane)  { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1_lane_u8 (uint8_t *p, uint8x8_t vec, int lane)      { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1_lane_u16(uint16_t *p, uint16x4_t vec, int lane)    { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1_lane_u32(uint32_t *p, uint32x2_t vec, int lane)    { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1_lane_u64(uint64_t *p, uint64x1_t vec, int lane)    { *p = vec[lane]; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst1_lane_p8 (poly8_t *, poly8x8_t, int);
extern          void          vst1_lane_p16(poly16_t *, poly16x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN void          vst1q_lane_s8(int8_t *p, int8x16_t vec, int lane)       { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1q_lane_s16(int16_t *p, int16x8_t vec, int lane)     { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1q_lane_s32(int32_t *p, int32x4_t vec, int lane)     { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1q_lane_s64(int64_t *p, int64x2_t vec, int lane)     { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1q_lane_f16(float16_t *p, float16x8_t vec, int lane) { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1q_lane_f32(float32_t *p, float32x4_t vec, int lane) { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1q_lane_u8(uint8_t *p, uint8x16_t vec, int lane)     { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1q_lane_u16(uint16_t *p, uint16x8_t vec, int lane)   { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1q_lane_u32(uint32_t *p, uint32x4_t vec, int lane)   { *p = vec[lane]; }
_MV_INLINE_DEFN void          vst1q_lane_u64(uint64_t *p, uint64x2_t vec, int lane)   { *p = vec[lane]; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst1q_lane_p8 (poly8_t *, poly8x16_t, int);
extern          void          vst1q_lane_p16(poly16_t *, poly16x8_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.19 - NEON intrinsics for loading an N-element structure */
/* */

/* Load N-element structure from memory */
/*		vld2{q}_<type> */
extern          int8x8x2_t    vld2_s8 (const int8_t *);
extern          int16x4x2_t   vld2_s16(const int16_t *);
extern          int32x2x2_t   vld2_s32(const int32_t *);
extern          int64x1x2_t   vld2_s64(const int64_t *);
extern          float16x4x2_t vld2_f16(const float16_t *);
extern          float32x2x2_t vld2_f32(const float32_t *);
extern          uint8x8x2_t   vld2_u8 (const uint8_t *);
extern          uint16x4x2_t  vld2_u16(const uint16_t *);
extern          uint32x2x2_t  vld2_u32(const uint32_t *);
extern          uint64x1x2_t  vld2_u64(const uint64_t *);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x2_t   vld2_p8 (const poly8_t *);
extern          poly16x4x2_t  vld2_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          int8x16x2_t   vld2q_s8 (const int8_t *);
extern          int16x8x2_t   vld2q_s16(const int16_t *);
extern          int32x4x2_t   vld2q_s32(const int32_t *);
extern          int64x2x2_t   vld2q_s64(const int64_t *);
extern          float16x8x2_t vld2q_f16(const float16_t *);
extern          float32x4x2_t vld2q_f32(const float32_t *);
extern          uint8x16x2_t  vld2q_u8 (const uint8_t *);
extern          uint16x8x2_t  vld2q_u16(const uint16_t *);
extern          uint32x4x2_t  vld2q_u32(const uint32_t *);
extern          uint64x2x2_t  vld2q_u64(const uint64_t *);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16x2_t  vld2q_p8 (const poly8_t *);
extern          poly16x8x2_t  vld2q_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vld3{q}_<type> */
extern          int8x8x3_t    vld3_s8 (const int8_t *);
extern          int16x4x3_t   vld3_s16(const int16_t *);
extern          int32x2x3_t   vld3_s32(const int32_t *);
extern          int64x1x3_t   vld3_s64(const int64_t *);
extern          float16x4x3_t vld3_f16(const float16_t *);
extern          float32x2x3_t vld3_f32(const float32_t *);
extern          uint8x8x3_t   vld3_u8 (const uint8_t *);
extern          uint16x4x3_t  vld3_u16(const uint16_t *);
extern          uint32x2x3_t  vld3_u32(const uint32_t *);
extern          uint64x1x3_t  vld3_u64(const uint64_t *);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x3_t   vld3_p8 (const poly8_t *);
extern          poly16x4x3_t  vld3_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          int8x16x3_t   vld3q_s8 (const int8_t *);
extern          int16x8x3_t   vld3q_s16(const int16_t *);
extern          int32x4x3_t   vld3q_s32(const int32_t *);
extern          int64x2x3_t   vld3q_s64(const int64_t *);
extern          float16x8x3_t vld3q_f16(const float16_t *);
extern          float32x4x3_t vld3q_f32(const float32_t *);
extern          uint8x16x3_t  vld3q_u8 (const uint8_t *);
extern          uint16x8x3_t  vld3q_u16(const uint16_t *);
extern          uint32x4x3_t  vld3q_u32(const uint32_t *);
extern          uint64x2x3_t  vld3q_u64(const uint64_t *);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16x3_t  vld3q_p8 (const poly8_t *);
extern          poly16x8x3_t  vld3q_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vld4{q}_<type> */
extern          int8x8x4_t    vld4_s8 (const int8_t *);
extern          int16x4x4_t   vld4_s16(const int16_t *);
extern          int32x2x4_t   vld4_s32(const int32_t *);
extern          int64x1x4_t   vld4_s64(const int64_t *);
extern          float16x4x4_t vld4_f16(const float16_t *);
extern          float32x2x4_t vld4_f32(const float32_t *);
extern          uint8x8x4_t   vld4_u8 (const uint8_t *);
extern          uint16x4x4_t  vld4_u16(const uint16_t *);
extern          uint32x2x4_t  vld4_u32(const uint32_t *);
extern          uint64x1x4_t  vld4_u64(const uint64_t *);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x4_t   vld4_p8 (const poly8_t *);
extern          poly16x4x4_t  vld4_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          int8x16x4_t   vld4q_s8 (const int8_t *);
extern          int16x8x4_t   vld4q_s16(const int16_t *);
extern          int32x4x4_t   vld4q_s32(const int32_t *);
extern          int64x2x4_t   vld4q_s64(const int64_t *);
extern          float16x8x4_t vld4q_f16(const float16_t *);
extern          float32x4x4_t vld4q_f32(const float32_t *);
extern          uint8x16x4_t  vld4q_u8 (const uint8_t *);
extern          uint16x8x4_t  vld4q_u16(const uint16_t *);
extern          uint32x4x4_t  vld4q_u32(const uint32_t *);
extern          uint64x2x4_t  vld4q_u64(const uint64_t *);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16x4_t  vld4q_p8 (const poly8_t *);
extern          poly16x8x4_t  vld4q_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Load all lanes of N-element structure with same value from memory */
/*		vld2_dup_<type> */
_MV_INLINE_DEFN int8x8x2_t    vld2_dup_s8 (const int8_t *p)    { int8x8x2_t  result; result.val[0] = p[0]; result.val[1] = p[1]; return result; }
_MV_INLINE_DEFN int16x4x2_t   vld2_dup_s16(const int16_t *p)   { int16x4x2_t result; result.val[0] = p[0]; result.val[1] = p[1]; return result; }
_MV_INLINE_DEFN int32x2x2_t   vld2_dup_s32(const int32_t *p)   { int32x2x2_t result; result.val[0] = p[0]; result.val[1] = p[1]; return result; }
_MV_INLINE_DEFN int64x1x2_t   vld2_dup_s64(const int64_t *p)   { int64x1x2_t result; result.val[0] = p[0]; result.val[1] = p[1]; return result; }
_MV_INLINE_DEFN float16x4x2_t vld2_dup_f16(const float16_t *p) { float16x4x2_t result; result.val[0] = p[0]; result.val[1] = p[1]; return result; }
_MV_INLINE_DEFN float32x2x2_t vld2_dup_f32(const float32_t *p) { float32x2x2_t result; result.val[0] = p[0]; result.val[1] = p[1]; return result; }
_MV_INLINE_DEFN uint8x8x2_t   vld2_dup_u8 (const uint8_t *p)   { uint8x8x2_t  result; result.val[0] = p[0]; result.val[1] = p[1]; return result; }
_MV_INLINE_DEFN uint16x4x2_t  vld2_dup_u16(const uint16_t *p)  { uint16x4x2_t result; result.val[0] = p[0]; result.val[1] = p[1]; return result; }
_MV_INLINE_DEFN uint32x2x2_t  vld2_dup_u32(const uint32_t *p)  { uint32x2x2_t result; result.val[0] = p[0]; result.val[1] = p[1]; return result; }
_MV_INLINE_DEFN uint64x1x2_t  vld2_dup_u64(const uint64_t *p)  { uint64x1x2_t result; result.val[0] = p[0]; result.val[1] = p[1]; return result; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x2_t   vld2_dup_p8 (const poly8_t *);
extern          poly16x4x2_t  vld2_dup_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vld3_dup_<type> */
_MV_INLINE_DEFN int8x8x3_t    vld3_dup_s8 (const int8_t *p)    { int8x8x3_t  d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; return d; }
_MV_INLINE_DEFN int16x4x3_t   vld3_dup_s16(const int16_t *p)   { int16x4x3_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; return d; }
_MV_INLINE_DEFN int32x2x3_t   vld3_dup_s32(const int32_t *p)   { int32x2x3_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; return d; }
_MV_INLINE_DEFN int64x1x3_t   vld3_dup_s64(const int64_t *p)   { int64x1x3_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; return d; }
_MV_INLINE_DEFN float16x4x3_t vld3_dup_f16(const float16_t *p) { float16x4x3_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; return d; }
_MV_INLINE_DEFN float32x2x3_t vld3_dup_f32(const float32_t *p) { float32x2x3_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; return d; }
_MV_INLINE_DEFN uint8x8x3_t   vld3_dup_u8 (const uint8_t *p)   { uint8x8x3_t  d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; return d; }
_MV_INLINE_DEFN uint16x4x3_t  vld3_dup_u16(const uint16_t *p)  { uint16x4x3_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; return d; }
_MV_INLINE_DEFN uint32x2x3_t  vld3_dup_u32(const uint32_t *p)  { uint32x2x3_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; return d; }
_MV_INLINE_DEFN uint64x1x3_t  vld3_dup_u64(const uint64_t *p)  { uint64x1x3_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; return d; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x3_t   vld3_dup_p8 (const poly8_t *);
extern          poly16x4x3_t  vld3_dup_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vld4_dup_<type> */
_MV_INLINE_DEFN int8x8x4_t    vld4_dup_s8 (const int8_t *p)    { int8x8x4_t  d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; d.val[3] = p[3]; return d; }
_MV_INLINE_DEFN int16x4x4_t   vld4_dup_s16(const int16_t *p)   { int16x4x4_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; d.val[3] = p[3]; return d; }
_MV_INLINE_DEFN int32x2x4_t   vld4_dup_s32(const int32_t *p)   { int32x2x4_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; d.val[3] = p[3]; return d; }
_MV_INLINE_DEFN int64x1x4_t   vld4_dup_s64(const int64_t *p)   { int64x1x4_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; d.val[3] = p[3]; return d; }
_MV_INLINE_DEFN float16x4x4_t vld4_dup_f16(const float16_t *p) { float16x4x4_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; d.val[3] = p[3]; return d; }
_MV_INLINE_DEFN float32x2x4_t vld4_dup_f32(const float32_t *p) { float32x2x4_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; d.val[3] = p[3]; return d; }
_MV_INLINE_DEFN uint8x8x4_t   vld4_dup_u8 (const uint8_t *p)   { uint8x8x4_t  d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; d.val[3] = p[3]; return d; }
_MV_INLINE_DEFN uint16x4x4_t  vld4_dup_u16(const uint16_t *p)  { uint16x4x4_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; d.val[3] = p[3]; return d; }
_MV_INLINE_DEFN uint32x2x4_t  vld4_dup_u32(const uint32_t *p)  { uint32x2x4_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; d.val[3] = p[3]; return d; }
_MV_INLINE_DEFN uint64x1x4_t  vld4_dup_u64(const uint64_t *p)  { uint64x1x4_t d; d.val[0] = p[0]; d.val[1] = p[1]; d.val[2] = p[2]; d.val[3] = p[3]; return d; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x4_t   vld4_dup_p8 (const poly8_t *);
extern          poly16x4x4_t  vld4_dup_p16(const poly16_t *);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Load a single lane of N-element structure from memory */
/*		vld2{q}_lane_<type> */
_MV_INLINE_DEFN int8x8x2_t    vld2_lane_s8 (const int8_t *p, int8x8x2_t s, int lane)       { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN int16x4x2_t   vld2_lane_s16(const int16_t *p, int16x4x2_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN int32x2x2_t   vld2_lane_s32(const int32_t *p, int32x2x2_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN int64x1x2_t   vld2_lane_s64(const int64_t *p, int64x1x2_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN float16x4x2_t vld2_lane_f16(const float16_t *p, float16x4x2_t s, int lane) { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN float32x2x2_t vld2_lane_f32(const float32_t *p, float32x2x2_t s, int lane) { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN uint8x8x2_t   vld2_lane_u8 (const uint8_t *p, uint8x8x2_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN uint16x4x2_t  vld2_lane_u16(const uint16_t *p, uint16x4x2_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN uint32x2x2_t  vld2_lane_u32(const uint32_t *p, uint32x2x2_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN uint64x1x2_t  vld2_lane_u64(const uint64_t *p, uint64x1x2_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x2_t   vld2_lane_p8 (const poly8_t *, poly8x8x2_t, int);
extern          poly16x4x2_t  vld2_lane_p16(const poly16_t *, poly16x4x2_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16x2_t   vld2q_lane_s8 (const int8_t *p, int8x16x2_t s, int lane)      { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN int16x8x2_t   vld2q_lane_s16(const int16_t *p, int16x8x2_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN int32x4x2_t   vld2q_lane_s32(const int32_t *p, int32x4x2_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN int64x2x2_t   vld2q_lane_s64(const int64_t *p, int64x2x2_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN float16x8x2_t vld2q_lane_f16(const float16_t *p, float16x8x2_t s, int lane) { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN float32x4x2_t vld2q_lane_f32(const float32_t *p, float32x4x2_t s, int lane) { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN uint8x16x2_t  vld2q_lane_u8 (const uint8_t *p, uint8x16x2_t s, int lane)    { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN uint16x8x2_t  vld2q_lane_u16(const uint16_t *p, uint16x8x2_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN uint32x4x2_t  vld2q_lane_u32(const uint32_t *p, uint32x4x2_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
_MV_INLINE_DEFN uint64x2x2_t  vld2q_lane_u64(const uint64_t *p, uint64x2x2_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; return s;}
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16x2_t  vld2q_lane_p8 (const poly8_t *, poly8x16x2_t, int);
extern          poly16x8x2_t  vld2q_lane_p16(const poly16_t *, poly16x8x2_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vld3{q}_lane_<type> */
_MV_INLINE_DEFN int8x8x3_t    vld3_lane_s8 (const int8_t *p, int8x8x3_t s, int lane)        { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN int16x4x3_t   vld3_lane_s16(const int16_t *p, int16x4x3_t s, int lane)      { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN int32x2x3_t   vld3_lane_s32(const int32_t *p, int32x2x3_t s, int lane)      { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN int64x1x3_t   vld3_lane_s64(const int64_t *p, int64x1x3_t s, int lane)      { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN float16x4x3_t vld3_lane_f16(const float16_t *p, float16x4x3_t s, int lane)  { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN float32x2x3_t vld3_lane_f32(const float32_t *p, float32x2x3_t s, int lane)  { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN uint8x8x3_t   vld3_lane_u8 (const uint8_t *p, uint8x8x3_t s, int lane)      { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN uint16x4x3_t  vld3_lane_u16(const uint16_t *p, uint16x4x3_t s, int lane)    { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN uint32x2x3_t  vld3_lane_u32(const uint32_t *p, uint32x2x3_t s, int lane)    { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN uint64x1x3_t  vld3_lane_u64(const uint64_t *p, uint64x1x3_t s, int lane)    { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x3_t   vld3_lane_p8 (const poly8_t *, poly8x8x3_t, int);
extern          poly16x4x3_t  vld3_lane_p16(const poly16_t *, poly16x4x3_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16x3_t   vld3q_lane_s8 (const int8_t *p, int8x16x3_t s, int lane)      { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN int16x8x3_t   vld3q_lane_s16(const int16_t *p, int16x8x3_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN int32x4x3_t   vld3q_lane_s32(const int32_t *p, int32x4x3_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN int64x2x3_t   vld3q_lane_s64(const int64_t *p, int64x2x3_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN float16x8x3_t vld3q_lane_f16(const float16_t *p, float16x8x3_t s, int lane) { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN float32x4x3_t vld3q_lane_f32(const float32_t *p, float32x4x3_t s, int lane) { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN uint8x16x3_t  vld3q_lane_u8 (const uint8_t *p, uint8x16x3_t s, int lane)    { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN uint16x8x3_t  vld3q_lane_u16(const uint16_t *p, uint16x8x3_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN uint32x4x3_t  vld3q_lane_u32(const uint32_t *p, uint32x4x3_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
_MV_INLINE_DEFN uint64x2x3_t  vld3q_lane_u64(const uint64_t *p, uint64x2x3_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; return s;}
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16x3_t  vld3q_lane_p8 (const poly8_t *, poly8x16x3_t, int);
extern          poly16x8x3_t  vld3q_lane_p16(const poly16_t *, poly16x8x3_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vld4{q}_lane_<type> */
_MV_INLINE_DEFN int8x8x4_t    vld4_lane_s8 (const int8_t *p, int8x8x4_t s, int lane)        { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN int16x4x4_t   vld4_lane_s16(const int16_t *p, int16x4x4_t s, int lane)      { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN int32x2x4_t   vld4_lane_s32(const int32_t *p, int32x2x4_t s, int lane)      { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN int64x1x4_t   vld4_lane_s64(const int64_t *p, int64x1x4_t s, int lane)      { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN float16x4x4_t vld4_lane_f16(const float16_t *p, float16x4x4_t s, int lane)  { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN float32x2x4_t vld4_lane_f32(const float32_t *p, float32x2x4_t s, int lane)  { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN uint8x8x4_t   vld4_lane_u8 (const uint8_t *p, uint8x8x4_t s, int lane)      { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN uint16x4x4_t  vld4_lane_u16(const uint16_t *p, uint16x4x4_t s, int lane)    { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN uint32x2x4_t  vld4_lane_u32(const uint32_t *p, uint32x2x4_t s, int lane)    { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN uint64x1x4_t  vld4_lane_u64(const uint64_t *p, uint64x1x4_t s, int lane)    { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x4_t   vld4_lane_p8 (const poly8_t *, poly8x8x4_t, int);
extern          poly16x4x4_t  vld4_lane_p16(const poly16_t *, poly16x4x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16x4_t   vld4q_lane_s8 (const int8_t *p, int8x16x4_t s, int lane)      { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN int16x8x4_t   vld4q_lane_s16(const int16_t *p, int16x8x4_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN int32x4x4_t   vld4q_lane_s32(const int32_t *p, int32x4x4_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN int64x2x4_t   vld4q_lane_s64(const int64_t *p, int64x2x4_t s, int lane)     { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN float16x8x4_t vld4q_lane_f16(const float16_t *p, float16x8x4_t s, int lane) { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN float32x4x4_t vld4q_lane_f32(const float32_t *p, float32x4x4_t s, int lane) { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN uint8x16x4_t  vld4q_lane_u8 (const uint8_t *p, uint8x16x4_t s, int lane)    { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN uint16x8x4_t  vld4q_lane_u16(const uint16_t *p, uint16x8x4_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN uint32x4x4_t  vld4q_lane_u32(const uint32_t *p, uint32x4x4_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
_MV_INLINE_DEFN uint64x2x4_t  vld4q_lane_u64(const uint64_t *p, uint64x2x4_t s, int lane)   { s.val[0][lane] = p[0]; s.val[1][lane] = p[1]; s.val[2][lane] = p[2]; s.val[3][lane] = p[3]; return s;}
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16x4_t  vld4q_lane_p8 (const poly8_t *, poly8x16x4_t, int);
extern          poly16x8x4_t  vld4q_lane_p16(const poly16_t *, poly16x8x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Store N-element structure to memory */
/*		vst2{q}_<type> */
extern          void          vst2_s8 (int8_t *, int8x8x2_t);
extern          void          vst2_s16(int16_t *, int16x4x2_t);
extern          void          vst2_s32(int32_t *, int32x2x2_t);
extern          void          vst2_s64(int64_t *, int64x1x2_t);
extern          void          vst2_f16(float16_t *, float16x4x2_t);
extern          void          vst2_f32(float32_t *, float32x2x2_t);
extern          void          vst2_u8 (uint8_t *, uint8x8x2_t);
extern          void          vst2_u16(uint16_t *, uint16x4x2_t);
extern          void          vst2_u32(uint32_t *, uint32x2x2_t);
extern          void          vst2_u64(uint64_t *, uint64x1x2_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst2_p8 (poly8_t *, poly8x8x2_t);
extern          void          vst2_p16(poly16_t *, poly16x4x2_t);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          void          vst2q_s8 (int8_t *, int8x16x2_t);
extern          void          vst2q_s16(int16_t *, int16x8x2_t);
extern          void          vst2q_s32(int32_t *, int32x4x2_t);
extern          void          vst2q_f16(float16_t *, float16x8x2_t);
extern          void          vst2q_f32(float32_t *, float32x4x2_t);
extern          void          vst2q_u8 (uint8_t *, uint8x16x2_t);
extern          void          vst2q_u16(uint16_t *, uint16x8x2_t);
extern          void          vst2q_u32(uint32_t *, uint32x4x2_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst2q_p8 (poly8_t *, poly8x16x2_t);
extern          void          vst2q_p16(poly16_t *, poly16x8x2_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vst3{q}_<type> */
extern          void          vst3_s8 (int8_t *, int8x8x3_t);
extern          void          vst3_s16(int16_t *, int16x4x3_t);
extern          void          vst3_s32(int32_t *, int32x2x3_t);
extern          void          vst3_s64(int64_t *, int64x1x3_t);
extern          void          vst3_f16(float16_t *, float16x4x3_t);
extern          void          vst3_f32(float32_t *, float32x2x3_t);
extern          void          vst3_u8 (uint8_t *, uint8x8x3_t);
extern          void          vst3_u16(uint16_t *, uint16x4x3_t);
extern          void          vst3_u32(uint32_t *, uint32x2x3_t);
extern          void          vst3_u64(uint64_t *, uint64x1x3_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst3_p8 (poly8_t *, poly8x8x3_t);
extern          void          vst3_p16(poly16_t *, poly16x4x3_t);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          void          vst3q_s8 (int8_t *, int8x16x3_t);
extern          void          vst3q_s16(int16_t *, int16x8x3_t);
extern          void          vst3q_s32(int32_t *, int32x4x3_t);
extern          void          vst3q_f16(float16_t *, float16x8x3_t);
extern          void          vst3q_f32(float32_t *, float32x4x3_t);
extern          void          vst3q_u8 (uint8_t *, uint8x16x3_t);
extern          void          vst3q_u16(uint16_t *, uint16x8x3_t);
extern          void          vst3q_u32(uint32_t *, uint32x4x3_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst3q_p8 (poly8_t *, poly8x16x3_t);
extern          void          vst3q_p16(poly16_t *, poly16x8x3_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vst4{q}_<type> */
extern          void          vst4_s8 (int8_t *, int8x8x4_t);
extern          void          vst4_s16(int16_t *, int16x4x4_t);
extern          void          vst4_s32(int32_t *, int32x2x4_t);
extern          void          vst4_s64(int64_t *, int64x1x4_t);
extern          void          vst4_f16(float16_t *, float16x4x4_t);
extern          void          vst4_f32(float32_t *, float32x2x4_t);
extern          void          vst4_u8 (uint8_t *, uint8x8x4_t);
extern          void          vst4_u16(uint16_t *, uint16x4x4_t);
extern          void          vst4_u32(uint32_t *, uint32x2x4_t);
extern          void          vst4_u64(uint64_t *, uint64x1x4_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst4_p8 (poly8_t *, poly8x8x4_t);
extern          void          vst4_p16(poly16_t *, poly16x4x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          void          vst4q_s8 (int8_t *, int8x16x4_t);
extern          void          vst4q_s16(int16_t *, int16x8x4_t);
extern          void          vst4q_s32(int32_t *, int32x4x4_t);
extern          void          vst4q_f16(float16_t *, float16x8x4_t);
extern          void          vst4q_f32(float32_t *, float32x4x4_t);
extern          void          vst4q_u8 (uint8_t *, uint8x16x4_t);
extern          void          vst4q_u16(uint16_t *, uint16x8x4_t);
extern          void          vst4q_u32(uint32_t *, uint32x4x4_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst4q_p8 (poly8_t *, poly8x16x4_t);
extern          void          vst4q_p16(poly16_t *, poly16x8x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Store a single lane of N-element structure to memory */
/*		vst2{q}_lane_<type> */
_MV_INLINE_DEFN void          vst2_lane_s8 (int8_t *p, int8x8x2_t vec, int lane)        { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
_MV_INLINE_DEFN void          vst2_lane_s16(int16_t *p, int16x4x2_t vec, int lane)      { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
_MV_INLINE_DEFN void          vst2_lane_s32(int32_t *p, int32x2x2_t vec, int lane)      { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
_MV_INLINE_DEFN void          vst2_lane_f16(float16_t *p, float16x4x2_t vec, int lane)  { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
_MV_INLINE_DEFN void          vst2_lane_f32(float32_t *p, float32x2x2_t vec, int lane)  { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
_MV_INLINE_DEFN void          vst2_lane_u8 (uint8_t *p, uint8x8x2_t vec, int lane)      { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
_MV_INLINE_DEFN void          vst2_lane_u16(uint16_t *p, uint16x4x2_t vec, int lane)    { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
_MV_INLINE_DEFN void          vst2_lane_u32(uint32_t *p, uint32x2x2_t vec, int lane)    { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst2_lane_p8 (poly8_t *, poly8x8x2_t, int);
extern          void          vst2_lane_p16(poly16_t *, poly16x4x2_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          void          vst2q_lane_s8 (int8_t *, int8x16x2_t, int);
_MV_INLINE_DEFN void          vst2q_lane_s16(int16_t *p, int16x8x2_t vec, int lane)     { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
_MV_INLINE_DEFN void          vst2q_lane_s32(int32_t *p, int32x4x2_t vec, int lane)     { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
_MV_INLINE_DEFN void          vst2q_lane_f16(float16_t *p, float16x8x2_t vec, int lane) { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
_MV_INLINE_DEFN void          vst2q_lane_f32(float32_t *p, float32x4x2_t vec, int lane) { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
extern          void          vst2q_lane_u8 (uint8_t *, uint8x16x2_t, int);
_MV_INLINE_DEFN void          vst2q_lane_u16(uint16_t *p, uint16x8x2_t vec, int lane)   { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
_MV_INLINE_DEFN void          vst2q_lane_u32(uint32_t *p, uint32x4x2_t vec, int lane)   { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane];}
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst2q_lane_p16(poly16_t *, poly16x8x2_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vst3{q}_lane_<type> */
_MV_INLINE_DEFN void          vst3_lane_s8 (int8_t *p, int8x8x3_t vec, int lane)        { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
_MV_INLINE_DEFN void          vst3_lane_s16(int16_t *p, int16x4x3_t vec, int lane)      { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
_MV_INLINE_DEFN void          vst3_lane_s32(int32_t *p, int32x2x3_t vec, int lane)      { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
_MV_INLINE_DEFN void          vst3_lane_f16(float16_t *p, float16x4x3_t vec, int lane)  { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
_MV_INLINE_DEFN void          vst3_lane_f32(float32_t *p, float32x2x3_t vec, int lane)  { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
_MV_INLINE_DEFN void          vst3_lane_u8 (uint8_t *p, uint8x8x3_t vec, int lane)      { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
_MV_INLINE_DEFN void          vst3_lane_u16(uint16_t *p, uint16x4x3_t vec, int lane)    { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
_MV_INLINE_DEFN void          vst3_lane_u32(uint32_t *p, uint32x2x3_t vec, int lane)    { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst3_lane_p8 (poly8_t *, poly8x8x3_t, int);
extern          void          vst3_lane_p16(poly16_t *, poly16x4x3_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          void          vst3q_lane_s8 (int8_t *, int8x16x3_t, int);
_MV_INLINE_DEFN void          vst3q_lane_s16(int16_t *p, int16x8x3_t vec, int lane)     { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
_MV_INLINE_DEFN void          vst3q_lane_s32(int32_t *p, int32x4x3_t vec, int lane)     { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
_MV_INLINE_DEFN void          vst3q_lane_f16(float16_t *p, float16x8x3_t vec, int lane) { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
_MV_INLINE_DEFN void          vst3q_lane_f32(float32_t *p, float32x4x3_t vec, int lane) { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
extern          void          vst3q_lane_u8 (uint8_t *, uint8x16x3_t, int);
_MV_INLINE_DEFN void          vst3q_lane_u16(uint16_t *p, uint16x8x3_t vec, int lane)   { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
_MV_INLINE_DEFN void          vst3q_lane_u32(uint32_t *p, uint32x4x3_t vec, int lane)   { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst3q_lane_p16(poly16_t *, poly16x8x3_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vst4{q}_lane_<type> */
_MV_INLINE_DEFN void          vst4_lane_s8 (int8_t *p, int8x8x4_t vec, int lane)        { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
_MV_INLINE_DEFN void          vst4_lane_s16(int16_t *p, int16x4x4_t vec, int lane)      { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
_MV_INLINE_DEFN void          vst4_lane_s32(int32_t *p, int32x2x4_t vec, int lane)      { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
_MV_INLINE_DEFN void          vst4_lane_f16(float16_t *p, float16x4x4_t vec, int lane)  { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
_MV_INLINE_DEFN void          vst4_lane_f32(float32_t *p, float32x2x4_t vec, int lane)  { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
_MV_INLINE_DEFN void          vst4_lane_u8 (uint8_t *p, uint8x8x4_t vec, int lane)      { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
_MV_INLINE_DEFN void          vst4_lane_u16(uint16_t *p, uint16x4x4_t vec, int lane)    { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
_MV_INLINE_DEFN void          vst4_lane_u32(uint32_t *p, uint32x2x4_t vec, int lane)    { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst4_lane_p8 (poly8_t *, poly8x8x4_t, int);
extern          void          vst4_lane_p16(poly16_t *, poly16x4x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          void          vst4q_lane_s8 (int8_t *, int8x16x4_t, int);
_MV_INLINE_DEFN void          vst4q_lane_s16(int16_t *p, int16x8x4_t vec, int lane)     { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
_MV_INLINE_DEFN void          vst4q_lane_s32(int32_t *p, int32x4x4_t vec, int lane)     { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
_MV_INLINE_DEFN void          vst4q_lane_f16(float16_t *p, float16x8x4_t vec, int lane) { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
_MV_INLINE_DEFN void          vst4q_lane_f32(float32_t *p, float32x4x4_t vec, int lane) { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
extern          void          vst4q_lane_u8 (uint8_t *, uint8x16x4_t, int);
_MV_INLINE_DEFN void          vst4q_lane_u16(uint16_t *p, uint16x8x4_t vec, int lane)   { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
_MV_INLINE_DEFN void          vst4q_lane_u32(uint32_t *p, uint32x4x4_t vec, int lane)   { p[0] = vec.val[0][lane]; p[1] = vec.val[1][lane]; p[2] = vec.val[2][lane]; p[3] = vec.val[3][lane]; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          void          vst4q_lane_p16(poly16_t *, poly16x8x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.20 - NEON intrinsics for extracting lanes from a vector into a register */
/* */

/* Extract a single lane (element) from a vector */
/*		vget{q}_lane_<type> */
_MV_INLINE_DEFN int8_t        vget_lane_s8 (int8x8_t a, int imm)     { return a[imm]; }
_MV_INLINE_DEFN int16_t       vget_lane_s16(int16x4_t a, int imm)    { return a[imm]; }
_MV_INLINE_DEFN int32_t       vget_lane_s32(int32x2_t a, int imm)    { return a[imm]; }
_MV_INLINE_DEFN int64_t       vget_lane_s64(int64x1_t a, int imm)    { return a[imm]; }
_MV_INLINE_DEFN float32_t     vget_lane_f32(float32x2_t a, int imm)  { return a[imm]; }
_MV_INLINE_DEFN uint8_t       vget_lane_u8 (uint8x8_t a, int imm)    { return a[imm]; }
_MV_INLINE_DEFN uint16_t      vget_lane_u16(uint16x4_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN uint32_t      vget_lane_u32(uint32x2_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN uint64_t      vget_lane_u64(uint64x1_t a, int imm)   { return a[imm]; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8_t       vget_lane_p8 (poly8x8_t, int);
extern          poly16_t      vget_lane_p16(poly16x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8_t        vgetq_lane_s8 (int8x16_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN int16_t       vgetq_lane_s16(int16x8_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN int32_t       vgetq_lane_s32(int32x4_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN int64_t       vgetq_lane_s64(int64x2_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN float32_t     vgetq_lane_f32(float32x4_t a, int imm) { return a[imm]; }
_MV_INLINE_DEFN uint8_t       vgetq_lane_u8 (uint8x16_t a, int imm)  { return a[imm]; }
_MV_INLINE_DEFN uint16_t      vgetq_lane_u16(uint16x8_t a, int imm)  { return a[imm]; }
_MV_INLINE_DEFN uint32_t      vgetq_lane_u32(uint32x4_t a, int imm)  { return a[imm]; }
_MV_INLINE_DEFN uint64_t      vgetq_lane_u64(uint64x2_t a, int imm)  { return a[imm]; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8_t       vgetq_lane_p8 (poly8x16_t, int);
extern          poly16_t      vgetq_lane_p16(poly16x8_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.21 - NEON intrinsics for loading a single lane of a vector from a literal */
/* */

/* Set a single lane (element) within a vector */
/*		vset{q}_lane_<type> */
_MV_INLINE_DEFN int8x8_t      vset_lane_s8 (int8_t value, int8x8_t a, int imm)        { a[imm] = value; return a; }
_MV_INLINE_DEFN int16x4_t     vset_lane_s16(int16_t value, int16x4_t a, int imm)      { a[imm] = value; return a; }
_MV_INLINE_DEFN int32x2_t     vset_lane_s32(int32_t value, int32x2_t a, int imm)      { a[imm] = value; return a; }
_MV_INLINE_DEFN int64x1_t     vset_lane_s64(int64_t value, int64x1_t a, int imm)      { return value; }
_MV_INLINE_DEFN float32x2_t   vset_lane_f32(float32_t value, float32x2_t a, int imm)  { a[imm] = value; return a; }
_MV_INLINE_DEFN uint8x8_t     vset_lane_u8 (uint8_t value, uint8x8_t a, int imm)      { a[imm] = value; return a; }
_MV_INLINE_DEFN uint16x4_t    vset_lane_u16(uint16_t value, uint16x4_t a, int imm)    { a[imm] = value; return a; }
_MV_INLINE_DEFN uint32x2_t    vset_lane_u32(uint32_t value, uint32x2_t a, int imm)    { a[imm] = value; return a; }
_MV_INLINE_DEFN uint64x1_t    vset_lane_u64(uint64_t value, uint64x1_t a, int imm)    { return value; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vset_lane_p8 (poly8_t, poly8x8_t, int);
extern          poly16x4_t    vset_lane_p16(poly16_t, poly16x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vsetq_lane_s8 (int8_t value, int8x16_t a, int imm)      { a[imm] = value; return a; }
_MV_INLINE_DEFN int16x8_t     vsetq_lane_s16(int16_t value, int16x8_t a, int imm)     { a[imm] = value; return a; }
_MV_INLINE_DEFN int32x4_t     vsetq_lane_s32(int32_t value, int32x4_t a, int imm)     { a[imm] = value; return a; }
_MV_INLINE_DEFN int64x2_t     vsetq_lane_s64(int64_t value, int64x2_t a, int imm)     { a[imm] = value; return a; }
_MV_INLINE_DEFN float32x4_t   vsetq_lane_f32(float32_t value, float32x4_t a, int imm) { a[imm] = value; return a; }
_MV_INLINE_DEFN uint8x16_t    vsetq_lane_u8 (uint8_t value, uint8x16_t a, int imm)    { a[imm] = value; return a; }
_MV_INLINE_DEFN uint16x8_t    vsetq_lane_u16(uint16_t value, uint16x8_t a, int imm)   { a[imm] = value; return a; }
_MV_INLINE_DEFN uint32x4_t    vsetq_lane_u32(uint32_t value, uint32x4_t a, int imm)   { a[imm] = value; return a; }
_MV_INLINE_DEFN uint64x2_t    vsetq_lane_u64(uint64_t value, uint64x2_t a, int imm)   { a[imm] = value; return a; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vsetq_lane_p8 (poly8_t, poly8x16_t, int);
extern          poly16x8_t    vsetq_lane_p16(poly16_t, poly16x8_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.22 - NEON intrinsics for initializing a vector from a literal bit pattern */
/* */

/* Create a vector from a literal bit pattern */
/*		vcreate_<type> */
_MV_INLINE_DEFN int8x8_t      vcreate_s8 (uint64_t a) { return __builtin_astype(a, int8x8_t); }
_MV_INLINE_DEFN int16x4_t     vcreate_s16(uint64_t a) { return __builtin_astype(a, int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vcreate_s32(uint64_t a) { return __builtin_astype(a, int32x2_t); }
_MV_INLINE_DEFN int64x1_t     vcreate_s64(uint64_t a) { return __builtin_astype(a, int64x1_t); }
_MV_INLINE_DEFN float16x4_t   vcreate_f16(uint64_t a) { return __builtin_astype(a, float16x4_t); }
_MV_INLINE_DEFN float32x2_t   vcreate_f32(uint64_t a) { return __builtin_astype(a, float32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vcreate_u8 (uint64_t a) { return __builtin_astype(a, uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vcreate_u16(uint64_t a) { return __builtin_astype(a, uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vcreate_u32(uint64_t a) { return __builtin_astype(a, uint32x2_t); }
_MV_INLINE_DEFN uint64x1_t    vcreate_u64(uint64_t a) { return __builtin_astype(a, uint64x1_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vcreate_p8 (uint64_t);
extern          poly16x4_t    vcreate_p16(uint64_t);
#endif _HAVE_NEON_TYPE_POLY


/* */
/* ARM Section 18.23 - NEON intrinsics for setting all lanes to the same value */
/* */

/* Load all lanes of vector to the same literal value */
/*		vdup{q}_n_<type> */
_MV_INLINE_DEFN int8x8_t      vdup_n_s8 (int8_t value)     { return value; }
_MV_INLINE_DEFN int16x4_t     vdup_n_s16(int16_t value)    { return value; }
_MV_INLINE_DEFN int32x2_t     vdup_n_s32(int32_t value)    { return value; }
_MV_INLINE_DEFN int64x1_t     vdup_n_s64(int64_t value)    { return value; }
_MV_INLINE_DEFN float32x2_t   vdup_n_f32(float32_t value)  { return value; }
_MV_INLINE_DEFN uint8x8_t     vdup_n_u8 (uint8_t value)    { return value; }
_MV_INLINE_DEFN uint16x4_t    vdup_n_u16(uint16_t value)   { return value; }
_MV_INLINE_DEFN uint32x2_t    vdup_n_u32(uint32_t value)   { return value; }
_MV_INLINE_DEFN uint64x1_t    vdup_n_u64(uint64_t value)   { return value; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vdup_n_p8 (poly8_t);
extern          poly16x4_t    vdup_n_p16(poly16_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vdupq_n_s8 (int8_t value)    { return value; }
_MV_INLINE_DEFN int16x8_t     vdupq_n_s16(int16_t value)   { return value; }
_MV_INLINE_DEFN int32x4_t     vdupq_n_s32(int32_t value)   { return value; }
_MV_INLINE_DEFN int64x2_t     vdupq_n_s64(int64_t value)   { return value; }
_MV_INLINE_DEFN float32x4_t   vdupq_n_f32(float32_t value) { return value; }
_MV_INLINE_DEFN uint8x16_t    vdupq_n_u8 (uint8_t value)   { return value; }
_MV_INLINE_DEFN uint16x8_t    vdupq_n_u16(uint16_t value)  { return value; }
_MV_INLINE_DEFN uint32x4_t    vdupq_n_u32(uint32_t value)  { return value; }
_MV_INLINE_DEFN uint64x2_t    vdupq_n_u64(uint64_t value)  { return value; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vdupq_n_p8 (poly8_t);
extern          poly16x8_t    vdupq_n_p16(poly16_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vmov{q}_n_<type> */
_MV_INLINE_DEFN int8x8_t      vmov_n_s8 (int8_t value)     { return value; }
_MV_INLINE_DEFN int16x4_t     vmov_n_s16(int16_t value)    { return value; }
_MV_INLINE_DEFN int32x2_t     vmov_n_s32(int32_t value)    { return value; }
_MV_INLINE_DEFN int64x1_t     vmov_n_s64(int64_t value)    { return value; }
_MV_INLINE_DEFN float32x2_t   vmov_n_f32(float32_t value)  { return value; }
_MV_INLINE_DEFN uint8x8_t     vmov_n_u8 (uint8_t value)    { return value; }
_MV_INLINE_DEFN uint16x4_t    vmov_n_u16(uint16_t value)   { return value; }
_MV_INLINE_DEFN uint32x2_t    vmov_n_u32(uint32_t value)   { return value; }
_MV_INLINE_DEFN uint64x1_t    vmov_n_u64(uint64_t value)   { return value; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vmov_n_p8 (poly8_t);
extern          poly16x4_t    vmov_n_p16(poly16_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vmovq_n_s8 (int8_t value)    { return value; }
_MV_INLINE_DEFN int16x8_t     vmovq_n_s16(int16_t value)   { return value; }
_MV_INLINE_DEFN int32x4_t     vmovq_n_s32(int32_t value)   { return value; }
_MV_INLINE_DEFN int64x2_t     vmovq_n_s64(int64_t value)   { return value; }
_MV_INLINE_DEFN float32x4_t   vmovq_n_f32(float32_t value) { return value; }
_MV_INLINE_DEFN uint8x16_t    vmovq_n_u8 (uint8_t value)   { return value; }
_MV_INLINE_DEFN uint16x8_t    vmovq_n_u16(uint16_t value)  { return value; }
_MV_INLINE_DEFN uint32x4_t    vmovq_n_u32(uint32_t value)  { return value; }
_MV_INLINE_DEFN uint64x2_t    vmovq_n_u64(uint64_t value)  { return value; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vmovq_n_p8 (poly8_t);
extern          poly16x8_t    vmovq_n_p16(poly16_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Load all lanes of the vector to the value of a lane of a vector */
/*		vdup{q}_lane_<type> */
_MV_INLINE_DEFN int8x8_t      vdup_lane_s8 (int8x8_t a, int imm)     { return a[imm]; }
_MV_INLINE_DEFN int16x4_t     vdup_lane_s16(int16x4_t a, int imm)    { return a[imm]; }
_MV_INLINE_DEFN int32x2_t     vdup_lane_s32(int32x2_t a, int imm)    { return a[imm]; }
_MV_INLINE_DEFN int64x1_t     vdup_lane_s64(int64x1_t a, int imm)    { return a; }
_MV_INLINE_DEFN float32x2_t   vdup_lane_f32(float32x2_t a, int imm)  { return a[imm]; }
_MV_INLINE_DEFN uint8x8_t     vdup_lane_u8 (uint8x8_t a, int imm)    { return a[imm]; }
_MV_INLINE_DEFN uint16x4_t    vdup_lane_u16(uint16x4_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN uint32x2_t    vdup_lane_u32(uint32x2_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN uint64x1_t    vdup_lane_u64(uint64x1_t a, int imm)   { return a; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vdup_lane_p8 (poly8x8_t, int);
extern          poly16x4_t    vdup_lane_p16(poly16x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vdupq_lane_s8 (int8x8_t a, int imm)    { return a[imm]; }
_MV_INLINE_DEFN int16x8_t     vdupq_lane_s16(int16x4_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN int32x4_t     vdupq_lane_s32(int32x2_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN int64x2_t     vdupq_lane_s64(int64x1_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN float32x4_t   vdupq_lane_f32(float32x2_t a, int imm) { return a[imm]; }
_MV_INLINE_DEFN uint8x16_t    vdupq_lane_u8 (uint8x8_t a, int imm)   { return a[imm]; }
_MV_INLINE_DEFN uint16x8_t    vdupq_lane_u16(uint16x4_t a, int imm)  { return a[imm]; }
_MV_INLINE_DEFN uint32x4_t    vdupq_lane_u32(uint32x2_t a, int imm)  { return a[imm]; }
_MV_INLINE_DEFN uint64x2_t    vdupq_lane_u64(uint64x1_t a, int imm)  { return a[imm]; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vdupq_lane_p8 (poly8x8_t, int);
extern          poly16x8_t    vdupq_lane_p16(poly16x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.24 - NEON intrinsics for combining vectors */
/* */

/* Join two 64 bit vectors into a single 128 bit vector */
/*		vcombine_<type> */
_MV_INLINE_DEFN int8x16_t     vcombine_s8 (int8x8_t low, int8x8_t high)       { int8x16_t result; result.lo = low; result.hi = high; return result; }
_MV_INLINE_DEFN int16x8_t     vcombine_s16(int16x4_t low, int16x4_t high)     { int16x8_t result; result.lo = low; result.hi = high; return result; }
_MV_INLINE_DEFN int32x4_t     vcombine_s32(int32x2_t low, int32x2_t high)     { int32x4_t result; result.lo = low; result.hi = high; return result; }
_MV_INLINE_DEFN int64x2_t     vcombine_s64(int64x1_t low, int64x1_t high)     { int64x2_t result; result.lo = low[0]; result.hi = high[0]; return result; }
_MV_INLINE_DEFN float16x8_t   vcombine_f16(float16x4_t low, float16x4_t high) { float16x8_t result; result.lo = low; result.hi = high; return result; }
_MV_INLINE_DEFN float32x4_t   vcombine_f32(float32x2_t low, float32x2_t high) { float32x4_t result; result.lo = low; result.hi = high; return result; }
_MV_INLINE_DEFN uint8x16_t    vcombine_u8 (uint8x8_t low, uint8x8_t high)     { uint8x16_t result; result.lo = low; result.hi = high; return result; }
_MV_INLINE_DEFN uint16x8_t    vcombine_u16(uint16x4_t low, uint16x4_t high)   { uint16x8_t result; result.lo = low; result.hi = high; return result; }
_MV_INLINE_DEFN uint32x4_t    vcombine_u32(uint32x2_t low, uint32x2_t high)   { uint32x4_t result; result.lo = low; result.hi = high; return result; }
_MV_INLINE_DEFN uint64x2_t    vcombine_u64(uint64x1_t low, uint64x1_t high)   { uint64x2_t result; result.lo = low[0]; result.hi = high[0]; return result; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vcombine_p8 (poly8x8_t, poly8x8_t);
extern          poly16x8_t    vcombine_p16(poly16x4_t, poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.25 - NEON intrinsics for splitting vectors */
/* */

/* Split a 128 bit vector into 2 component 64 bit vectors */
/*		vget_high_<type> */
_MV_INLINE_DEFN int8x8_t      vget_high_s8 (int8x16_t a)   { return a.hi; }
_MV_INLINE_DEFN int16x4_t     vget_high_s16(int16x8_t a)   { return a.hi; }
_MV_INLINE_DEFN int32x2_t     vget_high_s32(int32x4_t a)   { return a.hi; }
_MV_INLINE_DEFN int64x1_t     vget_high_s64(int64x2_t a)   { return a.hi; }
_MV_INLINE_DEFN float16x4_t   vget_high_f16(float16x8_t a) { return a.hi; }
_MV_INLINE_DEFN float32x2_t   vget_high_f32(float32x4_t a) { return a.hi; }
_MV_INLINE_DEFN uint8x8_t     vget_high_u8 (uint8x16_t a)  { return a.hi; }
_MV_INLINE_DEFN uint16x4_t    vget_high_u16(uint16x8_t a)  { return a.hi; }
_MV_INLINE_DEFN uint32x2_t    vget_high_u32(uint32x4_t a)  { return a.hi; }
_MV_INLINE_DEFN uint64x1_t    vget_high_u64(uint64x2_t a)  { return a.hi; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vget_high_p8 (poly8x16_t);
extern          poly16x4_t    vget_high_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vget_low_<type> */
_MV_INLINE_DEFN int8x8_t      vget_low_s8 (int8x16_t a)    { return a.lo; }
_MV_INLINE_DEFN int16x4_t     vget_low_s16(int16x8_t a)    { return a.lo; }
_MV_INLINE_DEFN int32x2_t     vget_low_s32(int32x4_t a)    { return a.lo; }
_MV_INLINE_DEFN int64x1_t     vget_low_s64(int64x2_t a)    { return a.lo; }
_MV_INLINE_DEFN float16x4_t   vget_low_f16(float16x8_t a)  { return a.lo; }
_MV_INLINE_DEFN float32x2_t   vget_low_f32(float32x4_t a)  { return a.lo; }
_MV_INLINE_DEFN uint8x8_t     vget_low_u8 (uint8x16_t a)   { return a.lo; }
_MV_INLINE_DEFN uint16x4_t    vget_low_u16(uint16x8_t a)   { return a.lo; }
_MV_INLINE_DEFN uint32x2_t    vget_low_u32(uint32x4_t a)   { return a.lo; }
_MV_INLINE_DEFN uint64x1_t    vget_low_u64(uint64x2_t a)   { return a.lo; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vget_low_p8 (poly8x16_t);
extern          poly16x4_t    vget_low_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.26 - NEON intrinsics for converting vectors */
/* */

/* Convert from float */
/*		vcvt{q}_<type>_f32 */
_MV_INLINE_DEFN int32x2_t     vcvt_s32_f32(float32x2_t a)  { return __builtin_convertvector(a, int32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vcvt_u32_f32(float32x2_t a)  { return __builtin_convertvector(a, uint32x2_t); }

_MV_INLINE_DEFN int32x4_t     vcvtq_s32_f32(float32x4_t a) { return __builtin_convertvector(a, int32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vcvtq_u32_f32(float32x4_t a) { return __builtin_convertvector(a, uint32x4_t); }

/*		vcvt{q}_n_<type>_f32 */
#ifdef _HAVE_NEON_TYPE_FIXED_POINT
extern          int32x2_t     vcvt_n_s32_f32(float32x2_t, int);
extern          uint32x2_t    vcvt_n_u32_f32(float32x2_t, int);

extern          int32x4_t     vcvtq_n_s32_f32(float32x4_t, int);
extern          uint32x4_t    vcvtq_n_u32_f32(float32x4_t, int);
#endif /* _HAVE_NEON_TYPE_FIXED_POINT */

/* Convert to float */
/*		vcvt{q}_f32_<type> */
_MV_INLINE_DEFN float32x2_t   vcvt_f32_s32(int32x2_t a)   { return __builtin_convertvector(a, float32x2_t); }
_MV_INLINE_DEFN float32x2_t   vcvt_f32_u32(uint32x2_t a)  { return __builtin_convertvector(a, float32x2_t); }

_MV_INLINE_DEFN float32x4_t   vcvtq_f32_s32(int32x4_t a)  { return __builtin_convertvector(a, float32x4_t); }
_MV_INLINE_DEFN float32x4_t   vcvtq_f32_u32(uint32x4_t a) { return __builtin_convertvector(a, float32x4_t); }

#ifdef _HAVE_NEON_TYPE_FIXED_POINT
extern          float32x2_t   vcvt_n_f32_s32(int32x2_t, int);
extern          float32x2_t   vcvt_n_f32_u32(uint32x2_t, int);

extern          float32x4_t   vcvtq_n_f32_s32(int32x4_t, int);
extern          float32x4_t   vcvtq_n_f32_u32(uint32x4_t, int);
#endif /* _HAVE_NEON_TYPE_FIXED_POINT */

/* Convert between floats */
/*		vcvt_<dstType>_<srcType> */
_MV_INLINE_DEFN float16x4_t   vcvt_f16_f32(float32x4_t a) { return __builtin_convertvector(a, float16x4_t); }
_MV_INLINE_DEFN float32x4_t   vcvt_f32_f16(float16x4_t a) { return __builtin_convertvector(a, float32x4_t); }

/* Vector narrow integer */
/*		vmovn_<type> */
_MV_INLINE_DEFN int8x8_t      vmovn_s16(int16x8_t a)      { return __builtin_convertvector(a, int8x8_t); }
_MV_INLINE_DEFN int16x4_t     vmovn_s32(int32x4_t a)      { return __builtin_convertvector(a, int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vmovn_s64(int64x2_t a)      { return __builtin_convertvector(a, int32x2_t); }
_MV_INLINE_DEFN uint8x8_t     vmovn_u16(uint16x8_t a)     { return __builtin_convertvector(a, uint8x8_t); }
_MV_INLINE_DEFN uint16x4_t    vmovn_u32(uint32x4_t a)     { return __builtin_convertvector(a, uint16x4_t); }
_MV_INLINE_DEFN uint32x2_t    vmovn_u64(uint64x2_t a)     { return __builtin_convertvector(a, uint32x2_t); }

/* Vector long move */
/*		vmovl_<type> */
_MV_INLINE_DEFN int16x8_t     vmovl_s8 (int8x8_t a)       { return __builtin_convertvector(a, int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vmovl_s16(int16x4_t a)      { return __builtin_convertvector(a, int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vmovl_s32(int32x2_t a)      { return __builtin_convertvector(a, int64x2_t); }
_MV_INLINE_DEFN uint16x8_t    vmovl_u8 (uint8x8_t a)      { return __builtin_convertvector(a, uint16x8_t); }
_MV_INLINE_DEFN uint32x4_t    vmovl_u16(uint16x4_t a)     { return __builtin_convertvector(a, uint32x4_t); }
_MV_INLINE_DEFN uint64x2_t    vmovl_u32(uint32x2_t a)     { return __builtin_convertvector(a, uint64x2_t); }

/* Vector saturating narrow integer */
/*		vqmovn_<type> */
extern          int8x8_t      vqmovn_s16(int16x8_t);
extern          int16x4_t     vqmovn_s32(int32x4_t);
extern          int32x2_t     vqmovn_s64(int64x2_t);
extern          uint8x8_t     vqmovn_u16(uint16x8_t);
extern          uint16x4_t    vqmovn_u32(uint32x4_t);
extern          uint32x2_t    vqmovn_u64(uint64x2_t);

/* Vector saturating narrow integer signed->unsigned */
/*		vqmovun_<type> */
extern          uint8x8_t     vqmovun_s16(int16x8_t);
extern          uint16x4_t    vqmovun_s32(int32x4_t);
extern          uint32x2_t    vqmovun_s64(int64x2_t);


/* */
/* ARM Section 18.27 - NEON intrinsics for table look up */
/* */

/*		vtbl1_<type> */
extern          int8x8_t      vtbl1_s8(int8x8_t, int8x8_t);
extern          uint8x8_t     vtbl1_u8(uint8x8_t, uint8x8_t);

#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vtbl1_p8(poly8x8_t, poly8x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vtbl[2-4]_<type> */
extern          int8x8_t      vtbl2_s8(int8x8x2_t, int8x8_t);
extern          uint8x8_t     vtbl2_u8(uint8x8x2_t, uint8x8_t);
extern          int8x8_t      vtbl3_s8(int8x8x3_t, int8x8_t);
extern          uint8x8_t     vtbl3_u8(uint8x8x3_t, uint8x8_t);
extern          int8x8_t      vtbl4_s8(int8x8x4_t, int8x8_t);
extern          uint8x8_t     vtbl4_u8(uint8x8x4_t, uint8x8_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vtbl2_p8(poly8x8x2_t, poly8x8_t);
extern          poly8x8_t     vtbl3_p8(poly8x8x3_t, poly8x8_t);
extern          poly8x8_t     vtbl4_p8(poly8x8x4_t, poly8x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.28 - NEON intrinsics for extended table look up */
/* */

/*		vtbx1_<type> */
extern          int8x8_t      vtbx1_s8(int8x8_t, int8x8_t, int8x8_t);
extern          uint8x8_t     vtbx1_u8(uint8x8_t, uint8x8_t, uint8x8_t);

#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vtbx1_p8(poly8x8_t, poly8x8_t, poly8x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/*		vtbx[2-4]_<type> */
extern          int8x8_t      vtbx2_s8(int8x8_t, int8x8x2_t, int8x8_t);
extern          uint8x8_t     vtbx2_u8(uint8x8_t, uint8x8x2_t, uint8x8_t);
extern          int8x8_t      vtbx3_s8(int8x8_t, int8x8x3_t, int8x8_t);
extern          uint8x8_t     vtbx3_u8(uint8x8_t, uint8x8x3_t, uint8x8_t);
extern          int8x8_t      vtbx4_s8(int8x8_t, int8x8x4_t, int8x8_t);
extern          uint8x8_t     vtbx4_u8(uint8x8_t, uint8x8x4_t, uint8x8_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vtbx2_p8(poly8x8_t, poly8x8x2_t, poly8x8_t);
extern          poly8x8_t     vtbx3_p8(poly8x8_t, poly8x8x3_t, poly8x8_t);
extern          poly8x8_t     vtbx4_p8(poly8x8_t, poly8x8x4_t, poly8x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.29 - NEON intrinsics for operations with a scalar value */
/* */

/* Vector multiply accumulate with scalar */
/*		vmla{q}_lane_<type> */
_MV_INLINE_DEFN int16x4_t     vmla_lane_s16(int16x4_t a, int16x4_t b, int16x4_t c, int lane)        { return a + b * c[lane]; }
_MV_INLINE_DEFN int32x2_t     vmla_lane_s32(int32x2_t a, int32x2_t b, int32x2_t c, int lane)        { return a + b * c[lane]; }
_MV_INLINE_DEFN float32x2_t   vmla_lane_f32(float32x2_t a, float32x2_t b, float32x2_t c, int lane)  { return a + b * c[lane]; }
_MV_INLINE_DEFN uint16x4_t    vmla_lane_u16(uint16x4_t a, uint16x4_t b, uint16x4_t c, int lane)     { return a + b * c[lane]; }
_MV_INLINE_DEFN uint32x2_t    vmla_lane_u32(uint32x2_t a, uint32x2_t b, uint32x2_t c, int lane)     { return a + b * c[lane]; }

_MV_INLINE_DEFN int16x8_t     vmlaq_lane_s16(int16x8_t a, int16x8_t b, int16x4_t c, int lane)       { return a + b * c[lane]; }
_MV_INLINE_DEFN int32x4_t     vmlaq_lane_s32(int32x4_t a, int32x4_t b, int32x2_t c, int lane)       { return a + b * c[lane]; }
_MV_INLINE_DEFN float32x4_t   vmlaq_lane_f32(float32x4_t a, float32x4_t b, float32x2_t c, int lane) { return a + b * c[lane]; }
_MV_INLINE_DEFN uint16x8_t    vmlaq_lane_u16(uint16x8_t a, uint16x8_t b, uint16x4_t c, int lane)    { return a + b * c[lane]; }
_MV_INLINE_DEFN uint32x4_t    vmlaq_lane_u32(uint32x4_t a, uint32x4_t b, uint32x2_t c, int lane)    { return a + b * c[lane]; }

/* Vector widening multiply accumulate with scalar */
/*		vmlal_lane_<type> */
_MV_INLINE_DEFN int32x4_t     vmlal_lane_s16(int32x4_t a, int16x4_t b, int16x4_t c, int lane)       { int32x4_t _b = __builtin_convertvector(b, int32x4_t), _c = __builtin_convertvector(c, int32x4_t); return a + _b * _c[lane]; }
_MV_INLINE_DEFN int64x2_t     vmlal_lane_s32(int64x2_t a, int32x2_t b, int32x2_t c, int lane)       { int64x2_t _b = __builtin_convertvector(b, int64x2_t), _c = __builtin_convertvector(c, int64x2_t); return a + _b * _c[lane]; }
_MV_INLINE_DEFN uint32x4_t    vmlal_lane_u16(uint32x4_t a, uint16x4_t b, uint16x4_t c, int lane)    { uint32x4_t _b = __builtin_convertvector(b, uint32x4_t), _c = __builtin_convertvector(c, uint32x4_t); return a + _b * _c[lane]; }
_MV_INLINE_DEFN uint64x2_t    vmlal_lane_u32(uint64x2_t a, uint32x2_t b, uint32x2_t c, int lane)    { uint64x2_t _b = __builtin_convertvector(b, uint64x2_t), _c = __builtin_convertvector(c, uint64x2_t); return a + _b * _c[lane]; }

/* Vector widening saturating doubling multiply accumulate with scalar */
/*		vqdmlal_lane_<type> */
extern          int32x4_t     vqdmlal_lane_s16(int32x4_t, int16x4_t, int16x4_t, int);
extern          int64x2_t     vqdmlal_lane_s32(int64x2_t, int32x2_t, int32x2_t, int);

/* Vector multiply subtract with scalar */
/*		vmls{q}_lane_<type> */
_MV_INLINE_DEFN int16x4_t     vmls_lane_s16(int16x4_t a, int16x4_t b, int16x4_t c, int lane)        { return a - b * c[lane]; }
_MV_INLINE_DEFN int32x2_t     vmls_lane_s32(int32x2_t a, int32x2_t b, int32x2_t c, int lane)        { return a - b * c[lane]; }
_MV_INLINE_DEFN float32x2_t   vmls_lane_f32(float32x2_t a, float32x2_t b, float32x2_t c, int lane)  { return a - b * c[lane]; }
_MV_INLINE_DEFN uint16x4_t    vmls_lane_u16(uint16x4_t a, uint16x4_t b, uint16x4_t c, int lane)     { return a - b * c[lane]; }
_MV_INLINE_DEFN uint32x2_t    vmls_lane_u32(uint32x2_t a, uint32x2_t b, uint32x2_t c, int lane)     { return a - b * c[lane]; }

_MV_INLINE_DEFN int16x8_t     vmlsq_lane_s16(int16x8_t a, int16x8_t b, int16x4_t c, int lane)       { return a - b * c[lane]; }
_MV_INLINE_DEFN int32x4_t     vmlsq_lane_s32(int32x4_t a, int32x4_t b, int32x2_t c, int lane)       { return a - b * c[lane]; }
_MV_INLINE_DEFN float32x4_t   vmlsq_lane_f32(float32x4_t a, float32x4_t b, float32x2_t c, int lane) { return a - b * c[lane]; }
_MV_INLINE_DEFN uint16x8_t    vmlsq_lane_u16(uint16x8_t a, uint16x8_t b, uint16x4_t c, int lane)    { return a - b * c[lane]; }
_MV_INLINE_DEFN uint32x4_t    vmlsq_lane_u32(uint32x4_t a, uint32x4_t b, uint32x2_t c, int lane)    { return a - b * c[lane]; }

/* Vector widening multiply subtract with scalar */
/*		vmlsl_lane_<type> */
_MV_INLINE_DEFN int32x4_t     vmlsl_lane_s16(int32x4_t a, int16x4_t b, int16x4_t c, int lane)       { int32x4_t _b = __builtin_convertvector(b, int32x4_t), _c = __builtin_convertvector(c, int32x4_t); return a - _b * _c[lane]; }
_MV_INLINE_DEFN int64x2_t     vmlsl_lane_s32(int64x2_t a, int32x2_t b, int32x2_t c, int lane)       { int64x2_t _b = __builtin_convertvector(b, int64x2_t), _c = __builtin_convertvector(c, int64x2_t); return a - _b * _c[lane]; }
_MV_INLINE_DEFN uint32x4_t    vmlsl_lane_u16(uint32x4_t a, uint16x4_t b, uint16x4_t c, int lane)    { uint32x4_t _b = __builtin_convertvector(b, uint32x4_t), _c = __builtin_convertvector(c, uint32x4_t); return a - _b * _c[lane]; }
_MV_INLINE_DEFN uint64x2_t    vmlsl_lane_u32(uint64x2_t a, uint32x2_t b, uint32x2_t c, int lane)    { uint64x2_t _b = __builtin_convertvector(b, uint64x2_t), _c = __builtin_convertvector(c, uint64x2_t); return a - _b * _c[lane]; }

/* Vector widening saturating doubling multiply subtract with scalar */
/*		vqdmlsl_lane_<type> */
_MV_INLINE_DEFN int32x4_t     vqdmlsl_lane_s16(int32x4_t a, int16x4_t b, int16x4_t c, int lane)     { int32x4_t _b, _c; _b = __builtin_convertvector(b, int32x4_t); _c = __builtin_convertvector(c, int32x4_t); int32x4_t lmul = __builtin_shave_vau_imuls_i32_ri(_b * _c[lane], 2); return __builtin_shave_vau_isubs_i32_rr(a, lmul); }
extern          int64x2_t     vqdmlsl_lane_s32(int64x2_t, int32x2_t, int32x2_t, int);

/* Vector multiply by scalar */
/*		vmul{q}_n_<type> */
_MV_INLINE_DEFN int16x4_t     vmul_n_s16(int16x4_t a, int16_t b)      { return a * b; }
_MV_INLINE_DEFN int32x2_t     vmul_n_s32(int32x2_t a, int32_t b)      { return a * b; }
_MV_INLINE_DEFN float32x2_t   vmul_n_f32(float32x2_t a, float32_t b)  { return a * b; }
_MV_INLINE_DEFN uint16x4_t    vmul_n_u16(uint16x4_t a, uint16_t b)    { return a * b; }
_MV_INLINE_DEFN uint32x2_t    vmul_n_u32(uint32x2_t a, uint32_t b)    { return a * b; }

_MV_INLINE_DEFN int16x8_t     vmulq_n_s16(int16x8_t a, int16_t b)     { return a * b; }
_MV_INLINE_DEFN int32x4_t     vmulq_n_s32(int32x4_t a, int32_t b)     { return a * b; }
_MV_INLINE_DEFN float32x4_t   vmulq_n_f32(float32x4_t a, float32_t b) { return a * b; }
_MV_INLINE_DEFN uint16x8_t    vmulq_n_u16(uint16x8_t a, uint16_t b)   { return a * b; }
_MV_INLINE_DEFN uint32x4_t    vmulq_n_u32(uint32x4_t a, uint32_t b)   { return a * b; }

/* Vector long multiply with scalar */
/*		vmull_n_<type> */
_MV_INLINE_DEFN int32x4_t     vmull_n_s16(int16x4_t a, int16_t b)     { return __builtin_convertvector(a, int32x4_t) * b; }
_MV_INLINE_DEFN int64x2_t     vmull_n_s32(int32x2_t a, int32_t b)     { return __builtin_convertvector(a, int64x2_t) * b; }
_MV_INLINE_DEFN uint32x4_t    vmull_n_u16(uint16x4_t a, uint16_t b)   { return __builtin_convertvector(a, uint32x4_t) * b; }
_MV_INLINE_DEFN uint64x2_t    vmull_n_u32(uint32x2_t a, uint32_t b)   { return __builtin_convertvector(a, uint64x2_t) * b; }

/* Vector long multiply by scalar */
/*		vmull_lane_<type> */
_MV_INLINE_DEFN int32x4_t     vmull_lane_s16(int16x4_t a, int16x4_t b, int lane)   { return __builtin_convertvector(a, int32x4_t) * (int32_t)b[lane]; }
_MV_INLINE_DEFN int64x2_t     vmull_lane_s32(int32x2_t a, int32x2_t b, int lane)   { return __builtin_convertvector(a, int64x2_t) * (int64_t)b[lane]; }
_MV_INLINE_DEFN uint32x4_t    vmull_lane_u16(uint16x4_t a, uint16x4_t b, int lane) { return __builtin_convertvector(a, uint32x4_t) * (uint32_t)b[lane]; }
_MV_INLINE_DEFN uint64x2_t    vmull_lane_u32(uint32x2_t a, uint32x2_t b, int lane) { return __builtin_convertvector(a, uint64x2_t) * (uint64_t)b[lane]; }

/* Vector saturating doubling long multiply with scalar */
/*		vqdmull_n_<type> */
extern          int64x2_t     vqdmull_n_s32(int32x2_t, int32_t);
extern          int32x4_t     vqdmull_n_s16(int16x4_t, int16_t);

/* Vector saturating doubling long multiply by scalar */
/*		vqdmull_lane_<type> */
extern          int64x2_t     vqdmull_lane_s32(int32x2_t, int32x2_t, int);
extern          int32x4_t     vqdmull_lane_s16(int16x4_t, int16x4_t, int);

/* Vector saturating doubling multiply high with scalar */
/*		vqdmulh{q}_n_<type> */
extern          int16x4_t     vqdmulh_n_s16(int16x4_t, int16_t);
extern          int32x2_t     vqdmulh_n_s32(int32x2_t, int32_t);

extern          int16x8_t     vqdmulhq_n_s16(int16x8_t, int16_t);
extern          int32x4_t     vqdmulhq_n_s32(int32x4_t, int32_t);

/* Vector saturating doubling multiply high by scalar */
/*		vqdmulh{q}_lane_<type> */
extern          int16x4_t     vqdmulh_lane_s16(int16x4_t, int16x4_t, int);
extern          int32x2_t     vqdmulh_lane_s32(int32x2_t, int32x2_t, int);

extern          int16x8_t     vqdmulhq_lane_s16(int16x8_t, int16x4_t, int);
extern          int32x4_t     vqdmulhq_lane_s32(int32x4_t, int32x2_t, int);

/* Vector saturating rounding doubling multiply high with scalar */
/*		vqrdmulh{q}_n_<type> */
extern          int16x4_t     vqrdmulh_n_s16(int16x4_t, int16_t);
extern          int32x2_t     vqrdmulh_n_s32(int32x2_t, int32_t);

extern          int16x8_t     vqrdmulhq_n_s16(int16x8_t, int16_t);
extern          int32x4_t     vqrdmulhq_n_s32(int32x4_t, int32_t);

/* Vector rounding saturating doubling multiply high by scalar */
/*		vqrdmulh{q}_lane_<type> */
extern          int16x4_t     vqrdmulh_lane_s16(int16x4_t, int16x4_t, int);
extern          int32x2_t     vqrdmulh_lane_s32(int32x2_t, int32x2_t, int);

extern          int16x8_t     vqrdmulhq_lane_s16(int16x8_t, int16x4_t, int);
extern          int32x4_t     vqrdmulhq_lane_s32(int32x4_t, int32x2_t, int);

/* Vector multiply accumulate with scalar */
/*		vmla{q}_n_<type> */
_MV_INLINE_DEFN int16x4_t     vmla_n_s16(int16x4_t a, int16x4_t b, int16_t n)        { return a + b * n; }
_MV_INLINE_DEFN int32x2_t     vmla_n_s32(int32x2_t a, int32x2_t b, int32_t n)        { return a + b * n; }
_MV_INLINE_DEFN float32x2_t   vmla_n_f32(float32x2_t a, float32x2_t b, float32_t n)  { return a + b * n; }
_MV_INLINE_DEFN uint16x4_t    vmla_n_u16(uint16x4_t a, uint16x4_t b, uint16_t n)     { return a + b * n; }
_MV_INLINE_DEFN uint32x2_t    vmla_n_u32(uint32x2_t a, uint32x2_t b, uint32_t n)     { return a + b * n; }

_MV_INLINE_DEFN int16x8_t     vmlaq_n_s16(int16x8_t a, int16x8_t b, int16_t n)       { return a + b * n; }
_MV_INLINE_DEFN int32x4_t     vmlaq_n_s32(int32x4_t a, int32x4_t b, int32_t n)       { return a + b * n; }
_MV_INLINE_DEFN float32x4_t   vmlaq_n_f32(float32x4_t a, float32x4_t b, float32_t n) { return a + b * n; }
_MV_INLINE_DEFN uint16x8_t    vmlaq_n_u16(uint16x8_t a, uint16x8_t b, uint16_t n)    { return a + b * n; }
_MV_INLINE_DEFN uint32x4_t    vmlaq_n_u32(uint32x4_t a, uint32x4_t b, uint32_t n)    { return a + b * n; }

/* Vector widening multiply accumulate with scalar */
/*		vmlal_n_<type> */
_MV_INLINE_DEFN int32x4_t     vmlal_n_s16(int32x4_t a, int16x4_t b, int16_t n)       { return a + __builtin_convertvector(b, int32x4_t) * (int32_t)n; }
_MV_INLINE_DEFN int64x2_t     vmlal_n_s32(int64x2_t a, int32x2_t b, int32_t n)       { return a + __builtin_convertvector(b, int64x2_t) * (int64_t)n; }
_MV_INLINE_DEFN uint32x4_t    vmlal_n_u16(uint32x4_t a, uint16x4_t b, uint16_t n)    { return a + __builtin_convertvector(b, uint32x4_t) * (uint32_t)n; }
_MV_INLINE_DEFN uint64x2_t    vmlal_n_u32(uint64x2_t a, uint32x2_t b, uint32_t n)    { return a + __builtin_convertvector(b, uint64x2_t) * (uint64_t)n; }

/* Vector widening saturating doubling multiply accumulate with scalar */
/*		vqdmlal_n_<type> */
extern          int32x4_t     vqdmlal_n_s16(int32x4_t, int16x4_t, int16_t);
extern          int64x2_t     vqdmlal_n_s32(int64x2_t, int32x2_t, int32_t);

/* Vector multiply subtract with scalar */
/*		vmls{q}_n_<type> */
_MV_INLINE_DEFN int16x4_t     vmls_n_s16(int16x4_t a, int16x4_t b, int16_t n)        { return a - b * n; }
_MV_INLINE_DEFN int32x2_t     vmls_n_s32(int32x2_t a, int32x2_t b, int32_t n)        { return a - b * n; }
_MV_INLINE_DEFN float32x2_t   vmls_n_f32(float32x2_t a, float32x2_t b, float32_t n)  { return a - b * n; }
_MV_INLINE_DEFN uint16x4_t    vmls_n_u16(uint16x4_t a, uint16x4_t b, uint16_t n)     { return a - b * n; }
_MV_INLINE_DEFN uint32x2_t    vmls_n_u32(uint32x2_t a, uint32x2_t b, uint32_t n)     { return a - b * n; }

_MV_INLINE_DEFN int16x8_t     vmlsq_n_s16(int16x8_t a, int16x8_t b, int16_t n)       { return a - b * n; }
_MV_INLINE_DEFN int32x4_t     vmlsq_n_s32(int32x4_t a, int32x4_t b, int32_t n)       { return a - b * n; }
_MV_INLINE_DEFN float32x4_t   vmlsq_n_f32(float32x4_t a, float32x4_t b, float32_t n) { return a - b * n; }
_MV_INLINE_DEFN uint16x8_t    vmlsq_n_u16(uint16x8_t a, uint16x8_t b, uint16_t n)    { return a - b * n; }
_MV_INLINE_DEFN uint32x4_t    vmlsq_n_u32(uint32x4_t a, uint32x4_t b, uint32_t n)    { return a - b * n; }

/* Vector widening multiply subtract with scalar */
/*		vmlsl_n_<type> */
_MV_INLINE_DEFN int32x4_t     vmlsl_n_s16(int32x4_t a, int16x4_t b, int16_t n)       { return a - __builtin_convertvector(b, int32x4_t) * (int32_t)n; }
_MV_INLINE_DEFN int64x2_t     vmlsl_n_s32(int64x2_t a, int32x2_t b, int32_t n)       { return a - __builtin_convertvector(b, int64x2_t) * (int64_t)n; }
_MV_INLINE_DEFN uint32x4_t    vmlsl_n_u16(uint32x4_t a, uint16x4_t b, uint16_t n)    { return a - __builtin_convertvector(b, uint32x4_t) * (uint32_t)n; }
_MV_INLINE_DEFN uint64x2_t    vmlsl_n_u32(uint64x2_t a, uint32x2_t b, uint32_t n)    { return a - __builtin_convertvector(b, uint64x2_t) * (uint64_t)n; }

/* Vector widening saturating doubling multiply subtract with scalar */
/*		vqdmlsl_n_<type> */
extern          int32x4_t     vqdmlsl_n_s16(int32x4_t, int16x4_t, int16_t);
extern          int64x2_t     vqdmlsl_n_s32(int64x2_t, int32x2_t, int32_t);


/* */
/* ARM Section 18.30 - NEON intrinsics for vector extraction */
/* */

/* Vector extract */
/*		vext{q}_<type> */
extern          int8x8_t      vext_s8 (int8x8_t, int8x8_t, int);
extern          int16x4_t     vext_s16(int16x4_t, int16x4_t, int);
extern          int32x2_t     vext_s32(int32x2_t, int32x2_t, int);
extern          int64x1_t     vext_s64(int64x1_t, int64x1_t, int);
extern          uint8x8_t     vext_u8 (uint8x8_t, uint8x8_t, int);
extern          uint16x4_t    vext_u16(uint16x4_t, uint16x4_t, int);
extern          uint32x2_t    vext_u32(uint32x2_t, uint32x2_t, int);
extern          uint64x1_t    vext_u64(uint64x1_t, uint64x1_t, int);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vext_p8(poly8x8_t, poly8x8_t, int);
extern          poly16x4_t    vext_p16(poly16x4_t, poly16x4_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          int8x16_t     vextq_s8 (int8x16_t, int8x16_t, int);
extern          int16x8_t     vextq_s16(int16x8_t, int16x8_t, int);
extern          int32x4_t     vextq_s32(int32x4_t, int32x4_t, int);
extern          int64x2_t     vextq_s64(int64x2_t, int64x2_t, int);
extern          uint8x16_t    vextq_u8 (uint8x16_t, uint8x16_t, int);
extern          uint16x8_t    vextq_u16(uint16x8_t, uint16x8_t, int);
extern          uint32x4_t    vextq_u32(uint32x4_t, uint32x4_t, int);
extern          uint64x2_t    vextq_u64(uint64x2_t, uint64x2_t, int);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vextq_p8(poly8x16_t, poly8x16_t, int);
extern          poly16x8_t    vextq_p16(poly16x8_t, poly16x8_t, int);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.31 - NEON intrinsics for reversing vector elements (swap endianness) */
/* */

/* Vector Reverse */
/*		vrevN{q}_<type> */
_MV_INLINE_DEFN int8x8_t      vrev64_s8 (int8x8_t a)     { return a.s76543210; }
_MV_INLINE_DEFN int16x4_t     vrev64_s16(int16x4_t a)    { return a.s3210; }
_MV_INLINE_DEFN int32x2_t     vrev64_s32(int32x2_t a)    { return a.s10; }
_MV_INLINE_DEFN float32x2_t   vrev64_f32(float32x2_t a)  { return a.s10; }
_MV_INLINE_DEFN uint8x8_t     vrev64_u8 (uint8x8_t a)    { return a.s76543210; }
_MV_INLINE_DEFN uint16x4_t    vrev64_u16(uint16x4_t a)   { return a.s3210; }
_MV_INLINE_DEFN uint32x2_t    vrev64_u32(uint32x2_t a)   { return a.s10; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vrev64_p8(poly8x8_t);
extern          poly16x4_t    vrev64_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vrev64q_s8(int8x16_t a)    { return a.s32107654BA98FEDC; }
_MV_INLINE_DEFN int16x8_t     vrev64q_s16(int16x8_t a)   { return a.s32107654; }
_MV_INLINE_DEFN int32x4_t     vrev64q_s32(int32x4_t a)   { return a.s1032; }
_MV_INLINE_DEFN float32x4_t   vrev64q_f32(float32x4_t a) { return a.s1032; }
_MV_INLINE_DEFN uint8x16_t    vrev64q_u8(uint8x16_t a)   { return a.s32107654BA98FEDC; }
_MV_INLINE_DEFN uint16x8_t    vrev64q_u16(uint16x8_t a)  { return a.s32107654; }
_MV_INLINE_DEFN uint32x4_t    vrev64q_u32(uint32x4_t a)  { return a.s1032; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vrev64q_p8(poly8x16_t);
extern          poly16x8_t    vrev64q_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x8_t      vrev32_s8(int8x8_t a)      { return a.s32107654; }
_MV_INLINE_DEFN int16x4_t     vrev32_s16(int16x4_t a)    { return a.s1032; }
_MV_INLINE_DEFN uint8x8_t     vrev32_u8(uint8x8_t a)     { return a.s32107654; }
_MV_INLINE_DEFN uint16x4_t    vrev32_u16(uint16x4_t a)   { return a.s1032; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vrev32_p8(poly8x8_t);
extern          poly16x4_t    vrev32_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vrev32q_s8(int8x16_t a)    { return a.s32107654BA98FEDC; }
_MV_INLINE_DEFN int16x8_t     vrev32q_s16(int16x8_t a)   { return a.s10325476; }
_MV_INLINE_DEFN uint8x16_t    vrev32q_u8(uint8x16_t a)   { return a.s32107654BA98FEDC; }
_MV_INLINE_DEFN uint16x8_t    vrev32q_u16(uint16x8_t a)  { return a.s10325476; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vrev32q_p8(poly8x16_t);
extern          poly16x8_t    vrev32q_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x8_t      vrev16_s8(int8x8_t a)      { return a.s10325476; }
_MV_INLINE_DEFN uint8x8_t     vrev16_u8(uint8x8_t a)     { return a.s10325476; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vrev16_p8(poly8x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vrev16q_s8(int8x16_t a)    { return a.s1032547698BADCFE; }
_MV_INLINE_DEFN uint8x16_t    vrev16q_u8(uint8x16_t a)   { return a.s1032547698BADCFE; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vrev16q_p8(poly8x16_t);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.32 - NEON intrinsics for other single operand arithmetic */
/* */

/* Absolute */
/*		vabs{q}_<type>.Vd[i] = |Va[i]| */
_MV_INLINE_DEFN int8x8_t      vabs_s8 (int8x8_t a)     { int8x16_t _a, result; _a.lo = a; result = __builtin_shave_vau_abs_i8_r(_a); return result.lo; }
_MV_INLINE_DEFN int16x4_t     vabs_s16(int16x4_t a)    { int16x8_t _a, result; _a.lo = a; result = __builtin_shave_vau_abs_i16_r(_a); return result.lo; }
_MV_INLINE_DEFN int32x2_t     vabs_s32(int32x2_t a)    { int32x4_t _a, result; _a.lo = a; result = __builtin_shave_vau_abs_i32_r(_a); return result.lo; }
_MV_INLINE_DEFN float32x2_t   vabs_f32(float32x2_t a)  { float32x4_t _a, result; _a.lo = a; result = __builtin_shave_vau_abs_f32_r(_a); return result.lo; }

_MV_INLINE_DEFN int8x16_t     vabsq_s8 (int8x16_t a)   { return __builtin_shave_vau_abs_i8_r(a); }
_MV_INLINE_DEFN int16x8_t     vabsq_s16(int16x8_t a)   { return __builtin_shave_vau_abs_i16_r(a); }
_MV_INLINE_DEFN int32x4_t     vabsq_s32(int32x4_t a)   { return __builtin_shave_vau_abs_i32_r(a); }
_MV_INLINE_DEFN float32x4_t   vabsq_f32(float32x4_t a) { return __builtin_shave_vau_abs_f32_r(a); }

/* Saturating absolute */
/*		vqabs{q}_<type>.Vd[i] = sat(|Va[i]|) */
extern          int8x8_t      vqabs_s8 (int8x8_t);
extern          int16x4_t     vqabs_s16(int16x4_t);
extern          int32x2_t     vqabs_s32(int32x2_t);

extern          int8x16_t     vqabsq_s8 (int8x16_t);
extern          int16x8_t     vqabsq_s16(int16x8_t);
extern          int32x4_t     vqabsq_s32(int32x4_t);

/* Negate */
/*		vneg{q}_<type>.Vd[i] = -Va[i] */
_MV_INLINE_DEFN int8x8_t      vneg_s8 (int8x8_t a)     { return -a; }
_MV_INLINE_DEFN int16x4_t     vneg_s16(int16x4_t a)    { return -a; }
_MV_INLINE_DEFN int32x2_t     vneg_s32(int32x2_t a)    { return -a; }
_MV_INLINE_DEFN float32x2_t   vneg_f32(float32x2_t a)  { return -a; }

_MV_INLINE_DEFN int8x16_t     vnegq_s8 (int8x16_t a)   { return -a; }
_MV_INLINE_DEFN int16x8_t     vnegq_s16(int16x8_t a)   { return -a; }
_MV_INLINE_DEFN int32x4_t     vnegq_s32(int32x4_t a)   { return -a; }
_MV_INLINE_DEFN float32x4_t   vnegq_f32(float32x4_t a) { return -a; }

/* Saturating negate */
/*		vqneg{q}_<type>.sat(Vd[i] = -Va[i]) */
extern          int8x8_t      vqneg_s8 (int8x8_t);
extern          int16x4_t     vqneg_s16(int16x4_t);
extern          int32x2_t     vqneg_s32(int32x2_t);

extern          int8x16_t     vqnegq_s8 (int8x16_t);
extern          int16x8_t     vqnegq_s16(int16x8_t);
extern          int32x4_t     vqnegq_s32(int32x4_t);

/* Count leading sign bits */
/*		vcls{q}_<type> */
extern          int8x8_t      vcls_s8 (int8x8_t);
extern          int32x2_t     vcls_s32(int32x2_t);
extern          int16x4_t     vcls_s16(int16x4_t);

extern          int8x16_t     vclsq_s8 (int8x16_t);
extern          int32x4_t     vclsq_s32(int32x4_t);
extern          int16x8_t     vclsq_s16(int16x8_t);

/* Count leading zeros */
/*		vclz{q}_<type> */
extern          int8x8_t      vclz_s8 (int8x8_t);
extern          int16x4_t     vclz_s16(int16x4_t);
extern          int32x2_t     vclz_s32(int32x2_t);
extern          uint8x8_t     vclz_u8 (uint8x8_t);
extern          uint16x4_t    vclz_u16(uint16x4_t);
extern          uint32x2_t    vclz_u32(uint32x2_t);

extern          int8x16_t     vclzq_s8 (int8x16_t);
extern          int16x8_t     vclzq_s16(int16x8_t);
extern          int32x4_t     vclzq_s32(int32x4_t);
extern          uint8x16_t    vclzq_u8 (uint8x16_t);
extern          uint16x8_t    vclzq_u16(uint16x8_t);
extern          uint32x4_t    vclzq_u32(uint32x4_t);

/* Count number of set bits */
/*		vcnt{q}_<type> */
extern          int8x8_t      vcnt_s8(int8x8_t);
extern          uint8x8_t     vcnt_u8(uint8x8_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vcnt_p8(poly8x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          int8x16_t     vcntq_s8(int8x16_t);
extern          uint8x16_t    vcntq_u8(uint8x16_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vcntq_p8(poly8x16_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Reciprocal estimate */
/*		vrecpe{q}_<type> */
extern          float32x2_t   vrecpe_f32(float32x2_t);
#ifdef _HAVE_NEON_TYPE_FIXED_POINT
extern          uint32x2_t    vrecpe_u32(uint32x2_t);
#endif /* _HAVE_NEON_TYPE_FIXED_POINT */

extern          float32x4_t   vrecpeq_f32(float32x4_t);
#ifdef _HAVE_NEON_TYPE_FIXED_POINT
extern          uint32x4_t    vrecpeq_u32(uint32x4_t);
#endif /* _HAVE_NEON_TYPE_FIXED_POINT */

/* Reciprocal square root estimate */
/*		vrsqrte{q}_<type> */
extern          float32x2_t   vrsqrte_f32(float32x2_t);
extern          uint32x2_t    vrsqrte_u32(uint32x2_t);

extern          float32x4_t   vrsqrteq_f32(float32x4_t);
extern          uint32x4_t    vrsqrteq_u32(uint32x4_t);


/* */
/* ARM Section 18.33 - NEON intrinsics for logical operations */
/* */

/* Bitwise NOT */
/*		vmvn{q}_<type> */
_MV_INLINE_DEFN int8x8_t      vmvn_s8 (int8x8_t a)    { return ~a; }
_MV_INLINE_DEFN int16x4_t     vmvn_s16(int16x4_t a)   { return ~a; }
_MV_INLINE_DEFN int32x2_t     vmvn_s32(int32x2_t a)   { return ~a; }
_MV_INLINE_DEFN uint8x8_t     vmvn_u8 (uint8x8_t a)   { return ~a; }
_MV_INLINE_DEFN uint16x4_t    vmvn_u16(uint16x4_t a)  { return ~a; }
_MV_INLINE_DEFN uint32x2_t    vmvn_u32(uint32x2_t a)  { return ~a; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vmvn_p8 (poly8x8_t a);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vmvnq_s8 (int8x16_t a)  { return ~a; }
_MV_INLINE_DEFN int16x8_t     vmvnq_s16(int16x8_t a)  { return ~a; }
_MV_INLINE_DEFN int32x4_t     vmvnq_s32(int32x4_t a)  { return ~a; }
_MV_INLINE_DEFN uint8x16_t    vmvnq_u8 (uint8x16_t a) { return ~a; }
_MV_INLINE_DEFN uint16x8_t    vmvnq_u16(uint16x8_t a) { return ~a; }
_MV_INLINE_DEFN uint32x4_t    vmvnq_u32(uint32x4_t a) { return ~a; }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vmvnq_p8 (poly8x16_t a);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Bitwise AND */
/*		vand{q}_<type> */
_MV_INLINE_DEFN int8x8_t      vand_s8 (int8x8_t a, int8x8_t b)      { return a & b; }
_MV_INLINE_DEFN int16x4_t     vand_s16(int16x4_t a, int16x4_t b)    { return a & b; }
_MV_INLINE_DEFN int32x2_t     vand_s32(int32x2_t a, int32x2_t b)    { return a & b; }
_MV_INLINE_DEFN int64x1_t     vand_s64(int64x1_t a, int64x1_t b)    { return a & b; }
_MV_INLINE_DEFN uint8x8_t     vand_u8 (uint8x8_t a, uint8x8_t b)    { return a & b; }
_MV_INLINE_DEFN uint16x4_t    vand_u16(uint16x4_t a, uint16x4_t b)  { return a & b; }
_MV_INLINE_DEFN uint32x2_t    vand_u32(uint32x2_t a, uint32x2_t b)  { return a & b; }
_MV_INLINE_DEFN uint64x1_t    vand_u64(uint64x1_t a, uint64x1_t b)  { return a & b; }

_MV_INLINE_DEFN int8x16_t     vandq_s8 (int8x16_t a, int8x16_t b)   { return a & b; }
_MV_INLINE_DEFN int16x8_t     vandq_s16(int16x8_t a, int16x8_t b)   { return a & b; }
_MV_INLINE_DEFN int32x4_t     vandq_s32(int32x4_t a, int32x4_t b)   { return a & b; }
_MV_INLINE_DEFN int64x2_t     vandq_s64(int64x2_t a, int64x2_t b)   { return a & b; }
_MV_INLINE_DEFN uint8x16_t    vandq_u8 (uint8x16_t a, uint8x16_t b) { return a & b; }
_MV_INLINE_DEFN uint16x8_t    vandq_u16(uint16x8_t a, uint16x8_t b) { return a & b; }
_MV_INLINE_DEFN uint32x4_t    vandq_u32(uint32x4_t a, uint32x4_t b) { return a & b; }
_MV_INLINE_DEFN uint64x2_t    vandq_u64(uint64x2_t a, uint64x2_t b) { return a & b; }

/* Bitwise OR */
/*		vorr{q}_<type> */
_MV_INLINE_DEFN int8x8_t      vorr_s8 (int8x8_t a, int8x8_t b)      { return a | b; }
_MV_INLINE_DEFN int16x4_t     vorr_s16(int16x4_t a, int16x4_t b)    { return a | b; }
_MV_INLINE_DEFN int32x2_t     vorr_s32(int32x2_t a, int32x2_t b)    { return a | b; }
_MV_INLINE_DEFN int64x1_t     vorr_s64(int64x1_t a, int64x1_t b)    { return a | b; }
_MV_INLINE_DEFN uint8x8_t     vorr_u8 (uint8x8_t a, uint8x8_t b)    { return a | b; }
_MV_INLINE_DEFN uint16x4_t    vorr_u16(uint16x4_t a, uint16x4_t b)  { return a | b; }
_MV_INLINE_DEFN uint32x2_t    vorr_u32(uint32x2_t a, uint32x2_t b)  { return a | b; }
_MV_INLINE_DEFN uint64x1_t    vorr_u64(uint64x1_t a, uint64x1_t b)  { return a | b; }

_MV_INLINE_DEFN int8x16_t     vorrq_s8 (int8x16_t a, int8x16_t b)   { return a | b; }
_MV_INLINE_DEFN int16x8_t     vorrq_s16(int16x8_t a, int16x8_t b)   { return a | b; }
_MV_INLINE_DEFN int32x4_t     vorrq_s32(int32x4_t a, int32x4_t b)   { return a | b; }
_MV_INLINE_DEFN int64x2_t     vorrq_s64(int64x2_t a, int64x2_t b)   { return a | b; }
_MV_INLINE_DEFN uint8x16_t    vorrq_u8 (uint8x16_t a, uint8x16_t b) { return a | b; }
_MV_INLINE_DEFN uint16x8_t    vorrq_u16(uint16x8_t a, uint16x8_t b) { return a | b; }
_MV_INLINE_DEFN uint32x4_t    vorrq_u32(uint32x4_t a, uint32x4_t b) { return a | b; }
_MV_INLINE_DEFN uint64x2_t    vorrq_u64(uint64x2_t a, uint64x2_t b) { return a | b; }

/* Bitwise exclusive or (EOR or XOR) */
/*		veor{q}_<type> */
_MV_INLINE_DEFN int8x8_t      veor_s8 (int8x8_t a, int8x8_t b)      { return a ^ b; }
_MV_INLINE_DEFN int16x4_t     veor_s16(int16x4_t a, int16x4_t b)    { return a ^ b; }
_MV_INLINE_DEFN int32x2_t     veor_s32(int32x2_t a, int32x2_t b)    { return a ^ b; }
_MV_INLINE_DEFN int64x1_t     veor_s64(int64x1_t a, int64x1_t b)    { return a ^ b; }
_MV_INLINE_DEFN uint8x8_t     veor_u8 (uint8x8_t a, uint8x8_t b)    { return a ^ b; }
_MV_INLINE_DEFN uint16x4_t    veor_u16(uint16x4_t a, uint16x4_t b)  { return a ^ b; }
_MV_INLINE_DEFN uint32x2_t    veor_u32(uint32x2_t a, uint32x2_t b)  { return a ^ b; }
_MV_INLINE_DEFN uint64x1_t    veor_u64(uint64x1_t a, uint64x1_t b)  { return a ^ b; }

_MV_INLINE_DEFN int8x16_t     veorq_s8 (int8x16_t a, int8x16_t b)   { return a ^ b; }
_MV_INLINE_DEFN int16x8_t     veorq_s16(int16x8_t a, int16x8_t b)   { return a ^ b; }
_MV_INLINE_DEFN int32x4_t     veorq_s32(int32x4_t a, int32x4_t b)   { return a ^ b; }
_MV_INLINE_DEFN int64x2_t     veorq_s64(int64x2_t a, int64x2_t b)   { return a ^ b; }
_MV_INLINE_DEFN uint8x16_t    veorq_u8 (uint8x16_t a, uint8x16_t b) { return a ^ b; }
_MV_INLINE_DEFN uint16x8_t    veorq_u16(uint16x8_t a, uint16x8_t b) { return a ^ b; }
_MV_INLINE_DEFN uint32x4_t    veorq_u32(uint32x4_t a, uint32x4_t b) { return a ^ b; }
_MV_INLINE_DEFN uint64x2_t    veorq_u64(uint64x2_t a, uint64x2_t b) { return a ^ b; }

/*   Bit Clear */
/*      vbic{q}_<type> */
_MV_INLINE_DEFN int8x8_t      vbic_s8 (int8x8_t a, int8x8_t b)      { return a & ~b; }
_MV_INLINE_DEFN int16x4_t     vbic_s16(int16x4_t a, int16x4_t b)    { return a & ~b; }
_MV_INLINE_DEFN int32x2_t     vbic_s32(int32x2_t a, int32x2_t b)    { return a & ~b; }
_MV_INLINE_DEFN int64x1_t     vbic_s64(int64x1_t a, int64x1_t b)    { return a & ~b; }
_MV_INLINE_DEFN uint8x8_t     vbic_u8 (uint8x8_t a, uint8x8_t b)    { return a & ~b; }
_MV_INLINE_DEFN uint16x4_t    vbic_u16(uint16x4_t a, uint16x4_t b)  { return a & ~b; }
_MV_INLINE_DEFN uint32x2_t    vbic_u32(uint32x2_t a, uint32x2_t b)  { return a & ~b; }
_MV_INLINE_DEFN uint64x1_t    vbic_u64(uint64x1_t a, uint64x1_t b)  { return a & ~b; }

_MV_INLINE_DEFN int8x16_t     vbicq_s8 (int8x16_t a, int8x16_t b)   { return a & ~b; }
_MV_INLINE_DEFN int16x8_t     vbicq_s16(int16x8_t a, int16x8_t b)   { return a & ~b; }
_MV_INLINE_DEFN int32x4_t     vbicq_s32(int32x4_t a, int32x4_t b)   { return a & ~b; }
_MV_INLINE_DEFN int64x2_t     vbicq_s64(int64x2_t a, int64x2_t b)   { return a & ~b; }
_MV_INLINE_DEFN uint8x16_t    vbicq_u8 (uint8x16_t a, uint8x16_t b) { return a & ~b; }
_MV_INLINE_DEFN uint16x8_t    vbicq_u16(uint16x8_t a, uint16x8_t b) { return a & ~b; }
_MV_INLINE_DEFN uint32x4_t    vbicq_u32(uint32x4_t a, uint32x4_t b) { return a & ~b; }
_MV_INLINE_DEFN uint64x2_t    vbicq_u64(uint64x2_t a, uint64x2_t b) { return a & ~b; }

/*	Bitwise OR complement */
/*		vorn{q}_<type> */
_MV_INLINE_DEFN int8x8_t      vorn_s8 (int8x8_t a, int8x8_t b)      { return a | ~b; }
_MV_INLINE_DEFN int16x4_t     vorn_s16(int16x4_t a, int16x4_t b)    { return a | ~b; }
_MV_INLINE_DEFN int32x2_t     vorn_s32(int32x2_t a, int32x2_t b)    { return a | ~b; }
_MV_INLINE_DEFN int64x1_t     vorn_s64(int64x1_t a, int64x1_t b)    { return a | ~b; }
_MV_INLINE_DEFN uint8x8_t     vorn_u8 (uint8x8_t a, uint8x8_t b)    { return a | ~b; }
_MV_INLINE_DEFN uint16x4_t    vorn_u16(uint16x4_t a, uint16x4_t b)  { return a | ~b; }
_MV_INLINE_DEFN uint32x2_t    vorn_u32(uint32x2_t a, uint32x2_t b)  { return a | ~b; }
_MV_INLINE_DEFN uint64x1_t    vorn_u64(uint64x1_t a, uint64x1_t b)  { return a | ~b; }

_MV_INLINE_DEFN int8x16_t     vornq_s8 (int8x16_t a, int8x16_t b)   { return a | ~b; }
_MV_INLINE_DEFN int16x8_t     vornq_s16(int16x8_t a, int16x8_t b)   { return a | ~b; }
_MV_INLINE_DEFN int32x4_t     vornq_s32(int32x4_t a, int32x4_t b)   { return a | ~b; }
_MV_INLINE_DEFN int64x2_t     vornq_s64(int64x2_t a, int64x2_t b)   { return a | ~b; }
_MV_INLINE_DEFN uint8x16_t    vornq_u8 (uint8x16_t a, uint8x16_t b) { return a | ~b; }
_MV_INLINE_DEFN uint16x8_t    vornq_u16(uint16x8_t a, uint16x8_t b) { return a | ~b; }
_MV_INLINE_DEFN uint32x4_t    vornq_u32(uint32x4_t a, uint32x4_t b) { return a | ~b; }
_MV_INLINE_DEFN uint64x2_t    vornq_u64(uint64x2_t a, uint64x2_t b) { return a | ~b; }

/* Bitwise Select */
/*		vbsl{q}_<type> */
_MV_INLINE_DEFN int8x8_t      vbsl_s8 (uint8x8_t a, int8x8_t b, int8x8_t c)          { return __builtin_astype((a & b) | (~a & c), int8x8_t); }
_MV_INLINE_DEFN int16x4_t     vbsl_s16(uint16x4_t a, int16x4_t b, int16x4_t c)       { return __builtin_astype((a & b) | (~a & c), int16x4_t); }
_MV_INLINE_DEFN int32x2_t     vbsl_s32(uint32x2_t a, int32x2_t b, int32x2_t c)       { return __builtin_astype((a & b) | (~a & c), int32x2_t); }
_MV_INLINE_DEFN int64x1_t     vbsl_s64(uint64x1_t a, int64x1_t b, int64x1_t c)       { return __builtin_astype((a & b) | (~a & c), int64x1_t); }
_MV_INLINE_DEFN float32x2_t   vbsl_f32(uint32x2_t a, float32x2_t b, float32x2_t c)   { return mvuAs_float2(vbsl_s32(a, mvuAs_int2(b), mvuAs_int2(c))); }
_MV_INLINE_DEFN uint8x8_t     vbsl_u8 (uint8x8_t a, uint8x8_t b, uint8x8_t c)        { return (a & b) | (~a & c); }
_MV_INLINE_DEFN uint16x4_t    vbsl_u16(uint16x4_t a, uint16x4_t b, uint16x4_t c)     { return (a & b) | (~a & c); }
_MV_INLINE_DEFN uint32x2_t    vbsl_u32(uint32x2_t a, uint32x2_t b, uint32x2_t c)     { return (a & b) | (~a & c); }
_MV_INLINE_DEFN uint64x1_t    vbsl_u64(uint64x1_t a, uint64x1_t b, uint64x1_t c)     { return (a & b) | (~a & c); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vbsl_p8 (uint8x8_t, poly8x8_t, poly8x8_t);
extern          poly16x4_t    vbsl_p16(uint16x4_t, poly16x4_t, poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vbslq_s8 (uint8x16_t a, int8x16_t b, int8x16_t c)      { return __builtin_astype((a & b) | (~a & c), int8x16_t); }
_MV_INLINE_DEFN int16x8_t     vbslq_s16(uint16x8_t a, int16x8_t b, int16x8_t c)      { return __builtin_astype((a & b) | (~a & c), int16x8_t); }
_MV_INLINE_DEFN int32x4_t     vbslq_s32(uint32x4_t a, int32x4_t b, int32x4_t c)      { return __builtin_astype((a & b) | (~a & c), int32x4_t); }
_MV_INLINE_DEFN int64x2_t     vbslq_s64(uint64x2_t a, int64x2_t b, int64x2_t c)      { return __builtin_astype((a & b) | (~a & c), int64x2_t); }
_MV_INLINE_DEFN float32x4_t   vbslq_f32(uint32x4_t a, float32x4_t b, float32x4_t c)  { return mvuAs_float4(vbslq_s32(a, mvuAs_int4(b), mvuAs_int4(c))); }
_MV_INLINE_DEFN uint8x16_t    vbslq_u8 (uint8x16_t a, uint8x16_t b, uint8x16_t c)    { return (a & b) | (~a & c); }
_MV_INLINE_DEFN uint16x8_t    vbslq_u16(uint16x8_t a, uint16x8_t b, uint16x8_t c)    { return (a & b) | (~a & c); }
_MV_INLINE_DEFN uint32x4_t    vbslq_u32(uint32x4_t a, uint32x4_t b, uint32x4_t c)    { return (a & b) | (~a & c); }
_MV_INLINE_DEFN uint64x2_t    vbslq_u64(uint64x2_t a, uint64x2_t b, uint64x2_t c)    { return (a & b) | (~a & c); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16_t    vbslq_p8 (uint8x16_t, poly8x16_t, poly8x16_t);
extern          poly16x8_t    vbslq_p16(uint16x8_t, poly16x8_t, poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.34 - NEON intrinsics for transposition operations */
/* */

/* Transpose elements */
/*		vtrn{q}_<type> */
extern          int8x8x2_t    vtrn_s8 (int8x8_t, int8x8_t);
extern          int16x4x2_t   vtrn_s16(int16x4_t, int16x4_t);
extern          int32x2x2_t   vtrn_s32(int32x2_t, int32x2_t);
extern          float32x2x2_t vtrn_f32(float32x2_t, float32x2_t);
extern          uint8x8x2_t   vtrn_u8 (uint8x8_t, uint8x8_t);
extern          uint16x4x2_t  vtrn_u16(uint16x4_t, uint16x4_t);
extern          uint32x2x2_t  vtrn_u32(uint32x2_t, uint32x2_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x2_t   vtrn_p8 (poly8x8_t, poly8x8_t);
extern          poly16x4x2_t  vtrn_p16(poly16x4_t, poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          int8x16x2_t   vtrnq_s8 (int8x16_t, int8x16_t);
extern          int16x8x2_t   vtrnq_s16(int16x8_t, int16x8_t);
extern          int32x4x2_t   vtrnq_s32(int32x4_t, int32x4_t);
extern          float32x4x2_t vtrnq_f32(float32x4_t, float32x4_t);
extern          uint8x16x2_t  vtrnq_u8 (uint8x16_t, uint8x16_t);
extern          uint16x8x2_t  vtrnq_u16(uint16x8_t, uint16x8_t);
extern          uint32x4x2_t  vtrnq_u32(uint32x4_t, uint32x4_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16x2_t  vtrnq_p8 (poly8x16_t, poly8x16_t);
extern          poly16x8x2_t  vtrnq_p16(poly16x8_t, poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/* Interleave elements */
/*		vzip{q}_<type> */
extern          int8x8x2_t    vzip_s8 (int8x8_t, int8x8_t);
extern          int16x4x2_t   vzip_s16(int16x4_t, int16x4_t);
extern          int32x2x2_t   vzip_s32(int32x2_t, int32x2_t);
extern          float32x2x2_t vzip_f32(float32x2_t, float32x2_t);
extern          uint8x8x2_t   vzip_u8 (uint8x8_t, uint8x8_t);
extern          uint16x4x2_t  vzip_u16(uint16x4_t, uint16x4_t);
extern          uint32x2x2_t  vzip_u32(uint32x2_t, uint32x2_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x2_t   vzip_p8 (poly8x8_t, poly8x8_t);
extern          poly16x4x2_t  vzip_p16(poly16x4_t, poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

extern          int8x16x2_t   vzipq_s8 (int8x16_t, int8x16_t);
extern          int16x8x2_t   vzipq_s16(int16x8_t, int16x8_t);
extern          int32x4x2_t   vzipq_s32(int32x4_t, int32x4_t);
extern          float32x4x2_t vzipq_f32(float32x4_t, float32x4_t);
extern          uint8x16x2_t  vzipq_u8 (uint8x16_t, uint8x16_t);
extern          uint16x8x2_t  vzipq_u16(uint16x8_t, uint16x8_t);
extern          uint32x4x2_t  vzipq_u32(uint32x4_t, uint32x4_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16x2_t  vzipq_p8 (poly8x16_t, poly8x16_t);
extern          poly16x8x2_t  vzipq_p16(poly16x8_t, poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

/* De-Interleave elements */
/*		vuzp{q}_<type> */
extern          int8x8x2_t    vuzp_s8 (int8x8_t, int8x8_t);
extern          int16x4x2_t   vuzp_s16(int16x4_t, int16x4_t);
extern          int32x2x2_t   vuzp_s32(int32x2_t, int32x2_t);
extern          float32x2x2_t vuzp_f32(float32x2_t, float32x2_t);
extern          uint8x8x2_t   vuzp_u8 (uint8x8_t, uint8x8_t);
extern          uint16x4x2_t  vuzp_u16(uint16x4_t, uint16x4_t);
extern          uint32x2x2_t  vuzp_u32(uint32x2_t, uint32x2_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8x2_t   vuzp_p8 (poly8x8_t, poly8x8_t);
extern          poly16x4x2_t  vuzp_p16(poly16x4_t, poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */
extern          int8x16x2_t   vuzpq_s8 (int8x16_t, int8x16_t);
extern          int16x8x2_t   vuzpq_s16(int16x8_t, int16x8_t);
extern          int32x4x2_t   vuzpq_s32(int32x4_t, int32x4_t);
extern          float32x4x2_t vuzpq_f32(float32x4_t, float32x4_t);
extern          uint8x16x2_t  vuzpq_u8 (uint8x16_t, uint8x16_t);
extern          uint16x8x2_t  vuzpq_u16(uint16x8_t, uint16x8_t);
extern          uint32x4x2_t  vuzpq_u32(uint32x4_t, uint32x4_t);
#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x16x2_t  vuzpq_p8 (poly8x16_t, poly8x16_t);
extern          poly16x8x2_t  vuzpq_p16(poly16x8_t, poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */


/* */
/* ARM Section 18.35 - NEON intrinsics for vector cast operations */
/* */

/*		vreinterpret{q}_<dsttype>_<srctype> */
_MV_INLINE_DEFN int8x8_t      vreinterpret_s8_s16(int16x4_t a)     { return __builtin_astype(a, int8x8_t); }
_MV_INLINE_DEFN int8x8_t      vreinterpret_s8_s32(int32x2_t a)     { return __builtin_astype(a, int8x8_t); }
_MV_INLINE_DEFN int8x8_t      vreinterpret_s8_s64(int64x1_t a)     { return __builtin_astype(a, int8x8_t); }
_MV_INLINE_DEFN int8x8_t      vreinterpret_s8_f16(float16x4_t a)   { return __builtin_astype(a, int8x8_t); }
_MV_INLINE_DEFN int8x8_t      vreinterpret_s8_f32(float32x2_t a)   { return __builtin_astype(a, int8x8_t); }
_MV_INLINE_DEFN int8x8_t      vreinterpret_s8_u8 (uint8x8_t a)     { return __builtin_astype(a, int8x8_t); }
_MV_INLINE_DEFN int8x8_t      vreinterpret_s8_u16(uint16x4_t a)    { return __builtin_astype(a, int8x8_t); }
_MV_INLINE_DEFN int8x8_t      vreinterpret_s8_u32(uint32x2_t a)    { return __builtin_astype(a, int8x8_t); }
_MV_INLINE_DEFN int8x8_t      vreinterpret_s8_u64(uint64x1_t a)    { return __builtin_astype(a, int8x8_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          int8x8_t      vreinterpret_s8_p8 (poly8x8_t);
extern          int8x8_t      vreinterpret_s8_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int8x16_t     vreinterpretq_s8_s16(int16x8_t a)    { return __builtin_astype(a, int8x16_t); }
_MV_INLINE_DEFN int8x16_t     vreinterpretq_s8_s32(int32x4_t a)    { return __builtin_astype(a, int8x16_t); }
_MV_INLINE_DEFN int8x16_t     vreinterpretq_s8_s64(int64x2_t a)    { return __builtin_astype(a, int8x16_t); }
_MV_INLINE_DEFN int8x16_t     vreinterpretq_s8_f16(float16x8_t a)  { return __builtin_astype(a, int8x16_t); }
_MV_INLINE_DEFN int8x16_t     vreinterpretq_s8_f32(float32x4_t a)  { return __builtin_astype(a, int8x16_t); }
_MV_INLINE_DEFN int8x16_t     vreinterpretq_s8_u8 (uint8x16_t a)   { return __builtin_astype(a, int8x16_t); }
_MV_INLINE_DEFN int8x16_t     vreinterpretq_s8_u16(uint16x8_t a)   { return __builtin_astype(a, int8x16_t); }
_MV_INLINE_DEFN int8x16_t     vreinterpretq_s8_u32(uint32x4_t a)   { return __builtin_astype(a, int8x16_t); }
_MV_INLINE_DEFN int8x16_t     vreinterpretq_s8_u64(uint64x2_t a)   { return __builtin_astype(a, int8x16_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          int8x16_t     vreinterpretq_s8_p8 (poly8x16_t);
extern          int8x16_t     vreinterpretq_s8_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int16x4_t     vreinterpret_s16_s8 (int8x8_t a)     { return __builtin_astype(a, int16x4_t); }
_MV_INLINE_DEFN int16x4_t     vreinterpret_s16_s32(int32x2_t a)    { return __builtin_astype(a, int16x4_t); }
_MV_INLINE_DEFN int16x4_t     vreinterpret_s16_s64(int64x1_t a)    { return __builtin_astype(a, int16x4_t); }
_MV_INLINE_DEFN int16x4_t     vreinterpret_s16_f16(float16x4_t a)  { return __builtin_astype(a, int16x4_t); }
_MV_INLINE_DEFN int16x4_t     vreinterpret_s16_f32(float32x2_t a)  { return __builtin_astype(a, int16x4_t); }
_MV_INLINE_DEFN int16x4_t     vreinterpret_s16_u8 (uint8x8_t a)    { return __builtin_astype(a, int16x4_t); }
_MV_INLINE_DEFN int16x4_t     vreinterpret_s16_u16(uint16x4_t a)   { return __builtin_astype(a, int16x4_t); }
_MV_INLINE_DEFN int16x4_t     vreinterpret_s16_u32(uint32x2_t a)   { return __builtin_astype(a, int16x4_t); }
_MV_INLINE_DEFN int16x4_t     vreinterpret_s16_u64(uint64x1_t a)   { return __builtin_astype(a, int16x4_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          int16x4_t     vreinterpret_s16_p8 (poly8x8_t);
extern          int16x4_t     vreinterpret_s16_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int16x8_t     vreinterpretq_s16_s8 (int8x16_t a)   { return __builtin_astype(a, int16x8_t); }
_MV_INLINE_DEFN int16x8_t     vreinterpretq_s16_s32(int32x4_t a)   { return __builtin_astype(a, int16x8_t); }
_MV_INLINE_DEFN int16x8_t     vreinterpretq_s16_s64(int64x2_t a)   { return __builtin_astype(a, int16x8_t); }
_MV_INLINE_DEFN int16x8_t     vreinterpretq_s16_f16(float16x8_t a) { return __builtin_astype(a, int16x8_t); }
_MV_INLINE_DEFN int16x8_t     vreinterpretq_s16_f32(float32x4_t a) { return __builtin_astype(a, int16x8_t); }
_MV_INLINE_DEFN int16x8_t     vreinterpretq_s16_u8 (uint8x16_t a)  { return __builtin_astype(a, int16x8_t); }
_MV_INLINE_DEFN int16x8_t     vreinterpretq_s16_u16(uint16x8_t a)  { return __builtin_astype(a, int16x8_t); }
_MV_INLINE_DEFN int16x8_t     vreinterpretq_s16_u32(uint32x4_t a)  { return __builtin_astype(a, int16x8_t); }
_MV_INLINE_DEFN int16x8_t     vreinterpretq_s16_u64(uint64x2_t a)  { return __builtin_astype(a, int16x8_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          int16x8_t     vreinterpretq_s16_p8 (poly8x16_t);
extern          int16x8_t     vreinterpretq_s16_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int32x2_t     vreinterpret_s32_s8 (int8x8_t a)     { return __builtin_astype(a, int32x2_t); }
_MV_INLINE_DEFN int32x2_t     vreinterpret_s32_s16(int16x4_t a)    { return __builtin_astype(a, int32x2_t); }
_MV_INLINE_DEFN int32x2_t     vreinterpret_s32_s64(int64x1_t a)    { return __builtin_astype(a, int32x2_t); }
_MV_INLINE_DEFN int32x2_t     vreinterpret_s32_f16(float16x4_t a)  { return __builtin_astype(a, int32x2_t); }
_MV_INLINE_DEFN int32x2_t     vreinterpret_s32_f32(float32x2_t a)  { return __builtin_astype(a, int32x2_t); }
_MV_INLINE_DEFN int32x2_t     vreinterpret_s32_u8 (uint8x8_t a)    { return __builtin_astype(a, int32x2_t); }
_MV_INLINE_DEFN int32x2_t     vreinterpret_s32_u16(uint16x4_t a)   { return __builtin_astype(a, int32x2_t); }
_MV_INLINE_DEFN int32x2_t     vreinterpret_s32_u32(uint32x2_t a)   { return __builtin_astype(a, int32x2_t); }
_MV_INLINE_DEFN int32x2_t     vreinterpret_s32_u64(uint64x1_t a)   { return __builtin_astype(a, int32x2_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          int32x2_t     vreinterpret_s32_p8 (poly8x8_t);
extern          int32x2_t     vreinterpret_s32_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int32x4_t     vreinterpretq_s32_s8 (int8x16_t a)   { return __builtin_astype(a, int32x4_t); }
_MV_INLINE_DEFN int32x4_t     vreinterpretq_s32_s16(int16x8_t a)   { return __builtin_astype(a, int32x4_t); }
_MV_INLINE_DEFN int32x4_t     vreinterpretq_s32_s64(int64x2_t a)   { return __builtin_astype(a, int32x4_t); }
_MV_INLINE_DEFN int32x4_t     vreinterpretq_s32_f16(float16x8_t a) { return __builtin_astype(a, int32x4_t); }
_MV_INLINE_DEFN int32x4_t     vreinterpretq_s32_f32(float32x4_t a) { return __builtin_astype(a, int32x4_t); }
_MV_INLINE_DEFN int32x4_t     vreinterpretq_s32_u8 (uint8x16_t a)  { return __builtin_astype(a, int32x4_t); }
_MV_INLINE_DEFN int32x4_t     vreinterpretq_s32_u16(uint16x8_t a)  { return __builtin_astype(a, int32x4_t); }
_MV_INLINE_DEFN int32x4_t     vreinterpretq_s32_u32(uint32x4_t a)  { return __builtin_astype(a, int32x4_t); }
_MV_INLINE_DEFN int32x4_t     vreinterpretq_s32_u64(uint64x2_t a)  { return __builtin_astype(a, int32x4_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          int32x4_t     vreinterpretq_s32_p8 (poly8x16_t);
extern          int32x4_t     vreinterpretq_s32_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int64x1_t     vreinterpret_s64_s8 (int8x8_t a)     { return __builtin_astype(a, int64x1_t); }
_MV_INLINE_DEFN int64x1_t     vreinterpret_s64_s16(int16x4_t a)    { return __builtin_astype(a, int64x1_t); }
_MV_INLINE_DEFN int64x1_t     vreinterpret_s64_s32(int32x2_t a)    { return __builtin_astype(a, int64x1_t); }
_MV_INLINE_DEFN int64x1_t     vreinterpret_s64_f16(float16x4_t a)  { return __builtin_astype(a, int64x1_t); }
_MV_INLINE_DEFN int64x1_t     vreinterpret_s64_f32(float32x2_t a)  { return __builtin_astype(a, int64x1_t); }
_MV_INLINE_DEFN int64x1_t     vreinterpret_s64_u8 (uint8x8_t a)    { return __builtin_astype(a, int64x1_t); }
_MV_INLINE_DEFN int64x1_t     vreinterpret_s64_u16(uint16x4_t a)   { return __builtin_astype(a, int64x1_t); }
_MV_INLINE_DEFN int64x1_t     vreinterpret_s64_u32(uint32x2_t a)   { return __builtin_astype(a, int64x1_t); }
_MV_INLINE_DEFN int64x1_t     vreinterpret_s64_u64(uint64x1_t a)   { return __builtin_astype(a, int64x1_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          int64x1_t     vreinterpret_s64_p8 (poly8x8_t);
extern          int64x1_t     vreinterpret_s64_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN int64x2_t     vreinterpretq_s64_s8 (int8x16_t a)   { return __builtin_astype(a, int64x2_t); }
_MV_INLINE_DEFN int64x2_t     vreinterpretq_s64_s16(int16x8_t a)   { return __builtin_astype(a, int64x2_t); }
_MV_INLINE_DEFN int64x2_t     vreinterpretq_s64_s32(int32x4_t a)   { return __builtin_astype(a, int64x2_t); }
_MV_INLINE_DEFN int64x2_t     vreinterpretq_s64_f16(float16x8_t a) { return __builtin_astype(a, int64x2_t); }
_MV_INLINE_DEFN int64x2_t     vreinterpretq_s64_f32(float32x4_t a) { return __builtin_astype(a, int64x2_t); }
_MV_INLINE_DEFN int64x2_t     vreinterpretq_s64_u8 (uint8x16_t a)  { return __builtin_astype(a, int64x2_t); }
_MV_INLINE_DEFN int64x2_t     vreinterpretq_s64_u16(uint16x8_t a)  { return __builtin_astype(a, int64x2_t); }
_MV_INLINE_DEFN int64x2_t     vreinterpretq_s64_u32(uint32x4_t a)  { return __builtin_astype(a, int64x2_t); }
_MV_INLINE_DEFN int64x2_t     vreinterpretq_s64_u64(uint64x2_t a)  { return __builtin_astype(a, int64x2_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          int64x2_t     vreinterpretq_s64_p8 (poly8x16_t);
extern          int64x2_t     vreinterpretq_s64_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN float16x4_t   vreinterpret_f16_s8 (int8x8_t a)     { return __builtin_astype(a, float16x4_t); }
_MV_INLINE_DEFN float16x4_t   vreinterpret_f16_s16(int16x4_t a)    { return __builtin_astype(a, float16x4_t); }
_MV_INLINE_DEFN float16x4_t   vreinterpret_f16_s32(int32x2_t a)    { return __builtin_astype(a, float16x4_t); }
_MV_INLINE_DEFN float16x4_t   vreinterpret_f16_s64(int64x1_t a)    { return __builtin_astype(a, float16x4_t); }
_MV_INLINE_DEFN float16x4_t   vreinterpret_f16_f32(float32x2_t a)  { return __builtin_astype(a, float16x4_t); }
_MV_INLINE_DEFN float16x4_t   vreinterpret_f16_u8 (uint8x8_t a)    { return __builtin_astype(a, float16x4_t); }
_MV_INLINE_DEFN float16x4_t   vreinterpret_f16_u16(uint16x4_t a)   { return __builtin_astype(a, float16x4_t); }
_MV_INLINE_DEFN float16x4_t   vreinterpret_f16_u32(uint32x2_t a)   { return __builtin_astype(a, float16x4_t); }
_MV_INLINE_DEFN float16x4_t   vreinterpret_f16_u64(uint64x1_t a)   { return __builtin_astype(a, float16x4_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          float16x4_t   vreinterpret_f16_p8 (poly8x8_t);
extern          float16x4_t   vreinterpret_f16_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN float32x2_t   vreinterpret_f32_s8 (int8x8_t a)     { return __builtin_astype(a, float32x2_t); }
_MV_INLINE_DEFN float32x2_t   vreinterpret_f32_s16(int16x4_t a)    { return __builtin_astype(a, float32x2_t); }
_MV_INLINE_DEFN float32x2_t   vreinterpret_f32_s32(int32x2_t a)    { return __builtin_astype(a, float32x2_t); }
_MV_INLINE_DEFN float32x2_t   vreinterpret_f32_s64(int64x1_t a)    { return __builtin_astype(a, float32x2_t); }
_MV_INLINE_DEFN float32x2_t   vreinterpret_f32_f16(float16x4_t a)  { return __builtin_astype(a, float32x2_t); }
_MV_INLINE_DEFN float32x2_t   vreinterpret_f32_u8 (uint8x8_t a)    { return __builtin_astype(a, float32x2_t); }
_MV_INLINE_DEFN float32x2_t   vreinterpret_f32_u16(uint16x4_t a)   { return __builtin_astype(a, float32x2_t); }
_MV_INLINE_DEFN float32x2_t   vreinterpret_f32_u32(uint32x2_t a)   { return __builtin_astype(a, float32x2_t); }
_MV_INLINE_DEFN float32x2_t   vreinterpret_f32_u64(uint64x1_t a)   { return __builtin_astype(a, float32x2_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          float32x2_t   vreinterpret_f32_p8 (poly8x8_t);
extern          float32x2_t   vreinterpret_f32_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN float16x8_t   vreinterpretq_f16_s8 (int8x16_t a)   { return __builtin_astype(a, float16x8_t); }
_MV_INLINE_DEFN float16x8_t   vreinterpretq_f16_s16(int16x8_t a)   { return __builtin_astype(a, float16x8_t); }
_MV_INLINE_DEFN float16x8_t   vreinterpretq_f16_s32(int32x4_t a)   { return __builtin_astype(a, float16x8_t); }
_MV_INLINE_DEFN float16x8_t   vreinterpretq_f16_s64(int64x2_t a)   { return __builtin_astype(a, float16x8_t); }
_MV_INLINE_DEFN float16x8_t   vreinterpretq_f16_f32(float32x4_t a) { return __builtin_astype(a, float16x8_t); }
_MV_INLINE_DEFN float16x8_t   vreinterpretq_f16_u8 (uint8x16_t a)  { return __builtin_astype(a, float16x8_t); }
_MV_INLINE_DEFN float16x8_t   vreinterpretq_f16_u16(uint16x8_t a)  { return __builtin_astype(a, float16x8_t); }
_MV_INLINE_DEFN float16x8_t   vreinterpretq_f16_u32(uint32x4_t a)  { return __builtin_astype(a, float16x8_t); }
_MV_INLINE_DEFN float16x8_t   vreinterpretq_f16_u64(uint64x2_t a)  { return __builtin_astype(a, float16x8_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          float16x8_t   vreinterpretq_f16_p8 (poly8x16_t);
extern          float16x8_t   vreinterpretq_f16_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN float32x4_t   vreinterpretq_f32_s8 (int8x16_t a)   { return __builtin_astype(a, float32x4_t); }
_MV_INLINE_DEFN float32x4_t   vreinterpretq_f32_s16(int16x8_t a)   { return __builtin_astype(a, float32x4_t); }
_MV_INLINE_DEFN float32x4_t   vreinterpretq_f32_s32(int32x4_t a)   { return __builtin_astype(a, float32x4_t); }
_MV_INLINE_DEFN float32x4_t   vreinterpretq_f32_s64(int64x2_t a)   { return __builtin_astype(a, float32x4_t); }
_MV_INLINE_DEFN float32x4_t   vreinterpretq_f32_f16(float16x8_t a) { return __builtin_astype(a, float32x4_t); }
_MV_INLINE_DEFN float32x4_t   vreinterpretq_f32_u8 (uint8x16_t a)  { return __builtin_astype(a, float32x4_t); }
_MV_INLINE_DEFN float32x4_t   vreinterpretq_f32_u16(uint16x8_t a)  { return __builtin_astype(a, float32x4_t); }
_MV_INLINE_DEFN float32x4_t   vreinterpretq_f32_u32(uint32x4_t a)  { return __builtin_astype(a, float32x4_t); }
_MV_INLINE_DEFN float32x4_t   vreinterpretq_f32_u64(uint64x2_t a)  { return __builtin_astype(a, float32x4_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          float32x4_t   vreinterpretq_f32_p8 (poly8x16_t);
extern          float32x4_t   vreinterpretq_f32_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN uint8x8_t     vreinterpret_u8_s8 (int8x8_t a)      { return __builtin_astype(a, uint8x8_t); }
_MV_INLINE_DEFN uint8x8_t     vreinterpret_u8_s16(int16x4_t a)     { return __builtin_astype(a, uint8x8_t); }
_MV_INLINE_DEFN uint8x8_t     vreinterpret_u8_s32(int32x2_t a)     { return __builtin_astype(a, uint8x8_t); }
_MV_INLINE_DEFN uint8x8_t     vreinterpret_u8_s64(int64x1_t a)     { return __builtin_astype(a, uint8x8_t); }
_MV_INLINE_DEFN uint8x8_t     vreinterpret_u8_f16(float16x4_t a)   { return __builtin_astype(a, uint8x8_t); }
_MV_INLINE_DEFN uint8x8_t     vreinterpret_u8_f32(float32x2_t a)   { return __builtin_astype(a, uint8x8_t); }
_MV_INLINE_DEFN uint8x8_t     vreinterpret_u8_u16(uint16x4_t a)    { return __builtin_astype(a, uint8x8_t); }
_MV_INLINE_DEFN uint8x8_t     vreinterpret_u8_u32(uint32x2_t a)    { return __builtin_astype(a, uint8x8_t); }
_MV_INLINE_DEFN uint8x8_t     vreinterpret_u8_u64(uint64x1_t a)    { return __builtin_astype(a, uint8x8_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint8x8_t     vreinterpret_u8_p8 (poly8x8_t);
extern          uint8x8_t     vreinterpret_u8_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN uint8x16_t    vreinterpretq_u8_s8 (int8x16_t a)    { return __builtin_astype(a, uint8x16_t); }
_MV_INLINE_DEFN uint8x16_t    vreinterpretq_u8_s16(int16x8_t a)    { return __builtin_astype(a, uint8x16_t); }
_MV_INLINE_DEFN uint8x16_t    vreinterpretq_u8_s32(int32x4_t a)    { return __builtin_astype(a, uint8x16_t); }
_MV_INLINE_DEFN uint8x16_t    vreinterpretq_u8_s64(int64x2_t a)    { return __builtin_astype(a, uint8x16_t); }
_MV_INLINE_DEFN uint8x16_t    vreinterpretq_u8_f16(float16x8_t a)  { return __builtin_astype(a, uint8x16_t); }
_MV_INLINE_DEFN uint8x16_t    vreinterpretq_u8_f32(float32x4_t a)  { return __builtin_astype(a, uint8x16_t); }
_MV_INLINE_DEFN uint8x16_t    vreinterpretq_u8_u16(uint16x8_t a)   { return __builtin_astype(a, uint8x16_t); }
_MV_INLINE_DEFN uint8x16_t    vreinterpretq_u8_u32(uint32x4_t a)   { return __builtin_astype(a, uint8x16_t); }
_MV_INLINE_DEFN uint8x16_t    vreinterpretq_u8_u64(uint64x2_t a)   { return __builtin_astype(a, uint8x16_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint8x16_t    vreinterpretq_u8_p8 (poly8x16_t);
extern          uint8x16_t    vreinterpretq_u8_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN uint16x4_t    vreinterpret_u16_s8 (int8x8_t a)     { return __builtin_astype(a, uint16x4_t); }
_MV_INLINE_DEFN uint16x4_t    vreinterpret_u16_s16(int16x4_t a)    { return __builtin_astype(a, uint16x4_t); }
_MV_INLINE_DEFN uint16x4_t    vreinterpret_u16_s32(int32x2_t a)    { return __builtin_astype(a, uint16x4_t); }
_MV_INLINE_DEFN uint16x4_t    vreinterpret_u16_s64(int64x1_t a)    { return __builtin_astype(a, uint16x4_t); }
_MV_INLINE_DEFN uint16x4_t    vreinterpret_u16_f16(float16x4_t a)  { return __builtin_astype(a, uint16x4_t); }
_MV_INLINE_DEFN uint16x4_t    vreinterpret_u16_f32(float32x2_t a)  { return __builtin_astype(a, uint16x4_t); }
_MV_INLINE_DEFN uint16x4_t    vreinterpret_u16_u8 (uint8x8_t a)    { return __builtin_astype(a, uint16x4_t); }
_MV_INLINE_DEFN uint16x4_t    vreinterpret_u16_u32(uint32x2_t a)   { return __builtin_astype(a, uint16x4_t); }
_MV_INLINE_DEFN uint16x4_t    vreinterpret_u16_u64(uint64x1_t a)   { return __builtin_astype(a, uint16x4_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint16x4_t    vreinterpret_u16_p8 (poly8x8_t);
extern          uint16x4_t    vreinterpret_u16_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN uint16x8_t    vreinterpretq_u16_s8 (int8x16_t a)   { return __builtin_astype(a, uint16x8_t); }
_MV_INLINE_DEFN uint16x8_t    vreinterpretq_u16_s16(int16x8_t a)   { return __builtin_astype(a, uint16x8_t); }
_MV_INLINE_DEFN uint16x8_t    vreinterpretq_u16_s32(int32x4_t a)   { return __builtin_astype(a, uint16x8_t); }
_MV_INLINE_DEFN uint16x8_t    vreinterpretq_u16_s64(int64x2_t a)   { return __builtin_astype(a, uint16x8_t); }
_MV_INLINE_DEFN uint16x8_t    vreinterpretq_u16_f16(float16x8_t a) { return __builtin_astype(a, uint16x8_t); }
_MV_INLINE_DEFN uint16x8_t    vreinterpretq_u16_f32(float32x4_t a) { return __builtin_astype(a, uint16x8_t); }
_MV_INLINE_DEFN uint16x8_t    vreinterpretq_u16_u8 (uint8x16_t a)  { return __builtin_astype(a, uint16x8_t); }
_MV_INLINE_DEFN uint16x8_t    vreinterpretq_u16_u32(uint32x4_t a)  { return __builtin_astype(a, uint16x8_t); }
_MV_INLINE_DEFN uint16x8_t    vreinterpretq_u16_u64(uint64x2_t a)  { return __builtin_astype(a, uint16x8_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint16x8_t    vreinterpretq_u16_p8 (poly8x16_t);
extern          uint16x8_t    vreinterpretq_u16_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN uint32x2_t    vreinterpret_u32_s8 (int8x8_t a)     { return __builtin_astype(a, uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vreinterpret_u32_s16(int16x4_t a)    { return __builtin_astype(a, uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vreinterpret_u32_s32(int32x2_t a)    { return __builtin_astype(a, uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vreinterpret_u32_s64(int64x1_t a)    { return __builtin_astype(a, uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vreinterpret_u32_f16(float16x4_t a)  { return __builtin_astype(a, uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vreinterpret_u32_f32(float32x2_t a)  { return __builtin_astype(a, uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vreinterpret_u32_u8 (uint8x8_t a)    { return __builtin_astype(a, uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vreinterpret_u32_u16(uint16x4_t a)   { return __builtin_astype(a, uint32x2_t); }
_MV_INLINE_DEFN uint32x2_t    vreinterpret_u32_u64(uint64x1_t a)   { return __builtin_astype(a, uint32x2_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint32x2_t    vreinterpret_u32_p8 (poly8x8_t);
extern          uint32x2_t    vreinterpret_u32_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN uint32x4_t    vreinterpretq_u32_s8 (int8x16_t a)   { return __builtin_astype(a, uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vreinterpretq_u32_s16(int16x8_t a)   { return __builtin_astype(a, uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vreinterpretq_u32_s32(int32x4_t a)   { return __builtin_astype(a, uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vreinterpretq_u32_s64(int64x2_t a)   { return __builtin_astype(a, uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vreinterpretq_u32_f16(float16x8_t a) { return __builtin_astype(a, uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vreinterpretq_u32_f32(float32x4_t a) { return __builtin_astype(a, uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vreinterpretq_u32_u8 (uint8x16_t a)  { return __builtin_astype(a, uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vreinterpretq_u32_u16(uint16x8_t a)  { return __builtin_astype(a, uint32x4_t); }
_MV_INLINE_DEFN uint32x4_t    vreinterpretq_u32_u64(uint64x2_t a)  { return __builtin_astype(a, uint32x4_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint32x4_t    vreinterpretq_u32_p8 (poly8x16_t);
extern          uint32x4_t    vreinterpretq_u32_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN uint64x1_t    vreinterpret_u64_s8 (int8x8_t a)     { return __builtin_astype(a, uint64x1_t); }
_MV_INLINE_DEFN uint64x1_t    vreinterpret_u64_s16(int16x4_t a)    { return __builtin_astype(a, uint64x1_t); }
_MV_INLINE_DEFN uint64x1_t    vreinterpret_u64_s32(int32x2_t a)    { return __builtin_astype(a, uint64x1_t); }
_MV_INLINE_DEFN uint64x1_t    vreinterpret_u64_s64(int64x1_t a)    { return __builtin_astype(a, uint64x1_t); }
_MV_INLINE_DEFN uint64x1_t    vreinterpret_u64_f16(float16x4_t a)  { return __builtin_astype(a, uint64x1_t); }
_MV_INLINE_DEFN uint64x1_t    vreinterpret_u64_f32(float32x2_t a)  { return __builtin_astype(a, uint64x1_t); }
_MV_INLINE_DEFN uint64x1_t    vreinterpret_u64_u8 (uint8x8_t a)    { return __builtin_astype(a, uint64x1_t); }
_MV_INLINE_DEFN uint64x1_t    vreinterpret_u64_u16(uint16x4_t a)   { return __builtin_astype(a, uint64x1_t); }
_MV_INLINE_DEFN uint64x1_t    vreinterpret_u64_u32(uint32x2_t a)   { return __builtin_astype(a, uint64x1_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint64x1_t    vreinterpret_u64_p8 (poly8x8_t);
extern          uint64x1_t    vreinterpret_u64_p16(poly16x4_t);
#endif /* _HAVE_NEON_TYPE_POLY */

_MV_INLINE_DEFN uint64x2_t    vreinterpretq_u64_s8 (int8x16_t a)   { return __builtin_astype(a, uint64x2_t); }
_MV_INLINE_DEFN uint64x2_t    vreinterpretq_u64_s16(int16x8_t a)   { return __builtin_astype(a, uint64x2_t); }
_MV_INLINE_DEFN uint64x2_t    vreinterpretq_u64_s32(int32x4_t a)   { return __builtin_astype(a, uint64x2_t); }
_MV_INLINE_DEFN uint64x2_t    vreinterpretq_u64_s64(int64x2_t a)   { return __builtin_astype(a, uint64x2_t); }
_MV_INLINE_DEFN uint64x2_t    vreinterpretq_u64_f16(float16x8_t a) { return __builtin_astype(a, uint64x2_t); }
_MV_INLINE_DEFN uint64x2_t    vreinterpretq_u64_f32(float32x4_t a) { return __builtin_astype(a, uint64x2_t); }
_MV_INLINE_DEFN uint64x2_t    vreinterpretq_u64_u8 (uint8x16_t a)  { return __builtin_astype(a, uint64x2_t); }
_MV_INLINE_DEFN uint64x2_t    vreinterpretq_u64_u16(uint16x8_t a)  { return __builtin_astype(a, uint64x2_t); }
_MV_INLINE_DEFN uint64x2_t    vreinterpretq_u64_u32(uint32x4_t a)  { return __builtin_astype(a, uint64x2_t); }
#ifdef _HAVE_NEON_TYPE_POLY
extern          uint64x2_t    vreinterpretq_u64_p8 (poly8x16_t);
extern          uint64x2_t    vreinterpretq_u64_p16(poly16x8_t);
#endif /* _HAVE_NEON_TYPE_POLY */

#ifdef _HAVE_NEON_TYPE_POLY
extern          poly8x8_t     vreinterpret_p8_u8 (uint8x8_t);
extern          poly8x8_t     vreinterpret_p8_u16(uint16x4_t);
extern          poly8x8_t     vreinterpret_p8_s8 (int8x8_t);
extern          poly8x8_t     vreinterpret_p8_s16(int16x4_t);
extern          poly8x8_t     vreinterpret_p8_s32(int32x2_t);
extern          poly8x8_t     vreinterpret_p8_s64(int64x1_t);
extern          poly8x8_t     vreinterpret_p8_f16(floa16x4_t);
extern          poly8x8_t     vreinterpret_p8_f32(float32x2_t);
extern          poly8x8_t     vreinterpret_p8_u32(uint32x2_t);
extern          poly8x8_t     vreinterpret_p8_u64(uint64x1_t);
extern          poly8x8_t     vreinterpret_p8_p16(poly16x4_t);

extern          poly8x16_t    vreinterpretq_p8_s8 (int8x16_t);
extern          poly8x16_t    vreinterpretq_p8_s16(int16x8_t);
extern          poly8x16_t    vreinterpretq_p8_s32(int32x4_t);
extern          poly8x16_t    vreinterpretq_p8_s64(int64x2_t);
extern          poly8x16_t    vreinterpretq_p8_f16(float16x8_t);
extern          poly8x16_t    vreinterpretq_p8_f32(float32x4_t);
extern          poly8x16_t    vreinterpretq_p8_u8 (uint8x16_t);
extern          poly8x16_t    vreinterpretq_p8_u16(uint16x8_t);
extern          poly8x16_t    vreinterpretq_p8_u32(uint32x4_t);
extern          poly8x16_t    vreinterpretq_p8_u64(uint64x2_t);
extern          poly8x16_t    vreinterpretq_p8_p16(poly16x8_t);

extern          poly16x4_t    vreinterpret_p16_s8 (int8x8_t);
extern          poly16x4_t    vreinterpret_p16_s16(int16x4_t);
extern          poly16x4_t    vreinterpret_p16_s32(int32x2_t);
extern          poly16x4_t    vreinterpret_p16_s64(int64x1_t);
extern          poly16x4_t    vreinterpret_p16_f16(float16x4_t);
extern          poly16x4_t    vreinterpret_p16_f32(float32x2_t);
extern          poly16x4_t    vreinterpret_p16_u8 (uint8x8_t);
extern          poly16x4_t    vreinterpret_p16_u16(uint16x4_t);
extern          poly16x4_t    vreinterpret_p16_u32(uint32x2_t);
extern          poly16x4_t    vreinterpret_p16_u64(uint64x1_t);
extern          poly16x4_t    vreinterpret_p16_p8 (poly8x8_t);

extern          poly16x8_t    vreinterpretq_p16_s8 (int8x16_t);
extern          poly16x8_t    vreinterpretq_p16_s16(int16x8_t);
extern          poly16x8_t    vreinterpretq_p16_s32(int32x4_t);
extern          poly16x8_t    vreinterpretq_p16_s64(int64x2_t);
extern          poly16x8_t    vreinterpretq_p16_f16(float16x8_t);
extern          poly16x8_t    vreinterpretq_p16_f32(float32x4_t);
extern          poly16x8_t    vreinterpretq_p16_u8 (uint8x16_t);
extern          poly16x8_t    vreinterpretq_p16_u16(uint16x8_t);
extern          poly16x8_t    vreinterpretq_p16_u32(uint32x4_t);
extern          poly16x8_t    vreinterpretq_p16_u64(uint64x2_t);
extern          poly16x8_t    vreinterpretq_p16_p8 (poly8x16_t);
#endif /* _HAVE_NEON_TYPE_POLY */


_MV_EXTERNC_END


#endif /* _MOVINEONBINDINGS_H_ */
