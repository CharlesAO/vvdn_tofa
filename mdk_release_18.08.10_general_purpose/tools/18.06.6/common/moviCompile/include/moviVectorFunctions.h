/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   moviVectorFunctions.h
 *  Description:   Vector Utilities Library function declarations
 *  --------------------------------------------------------------------------- */

#ifndef _MOVIVECTORFUNCTIONS_H_
#define _MOVIVECTORFUNCTIONS_H_ (1)

#ifndef __MOVICOMPILE__
# error "moviVectorFunctions.h: this header must be included only in a file compiled with moviCompile!"
#elif defined(__shavenn__)
# error "moviVectorFunctions.h: is not supported for SHAVE NN"
#endif


#pragma GCC system_header

#include <sys/__moviconfig.h>
#include <moviVectorTypes.h>


/* all */
_MV_OVERLOAD_INLINE_DEFN int        mvuAll(  long4 a)               { return ((__builtin_shave_sau_andx_x32_r(a) & 0x80000000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAll( ulong4 a)               { return ((__builtin_shave_sau_andx_x32_r(a) & 0x80000000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAll(   int4 a)               { return ((__builtin_shave_sau_andx_x32_r(a) & 0x80000000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAll(  uint4 a)               { return ((__builtin_shave_sau_andx_x32_r(a) & 0x80000000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAll( short8 a)               { return ((__builtin_shave_sau_andx_x16_r(a) &     0x8000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAll(ushort8 a)               { return ((__builtin_shave_sau_andx_x16_r(a) &     0x8000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAll(schar16 a)               { return ((__builtin_shave_sau_andx_x8_r(a)  &       0x80u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAll( char16 a)               { return ((__builtin_shave_sau_andx_x8_r(a)  &       0x80u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAll(uchar16 a)               { return ((__builtin_shave_sau_andx_x8_r(a)  &       0x80u) != 0); }

/* abs */
_MV_OVERLOAD_INLINE_DEFN ulong4     mvuAbs(  long4 a)               { return __builtin_shave_vau_abs_i32_r(a); }
_MV_OVERLOAD_INLINE_DEFN ulong4     mvuAbs( ulong4 a)               { return a; }
_MV_OVERLOAD_INLINE_DEFN uint4      mvuAbs(   int4 a)               { return __builtin_shave_vau_abs_i32_r(a); }
_MV_OVERLOAD_INLINE_DEFN uint4      mvuAbs(  uint4 a)               { return a; }
_MV_OVERLOAD_INLINE_DEFN ushort8    mvuAbs( short8 a)               { return __builtin_shave_vau_abs_i16_r(a); }
_MV_OVERLOAD_INLINE_DEFN ushort8    mvuAbs(ushort8 a)               { return a; }
_MV_OVERLOAD_INLINE_DEFN uchar16    mvuAbs(schar16 a)               { return __builtin_shave_vau_abs_i8_r(a); }
_MV_OVERLOAD_INLINE_DEFN uchar16    mvuAbs(uchar16 a)               { return a; }
#ifdef __CHAR_UNSIGNED__
_MV_OVERLOAD_INLINE_DEFN uchar16    mvuAbs( char16 a)               { return a; }
#else /* __CHAR_UNSIGNED__ */
_MV_OVERLOAD_INLINE_DEFN uchar16    mvuAbs( char16 a)               { return __builtin_shave_vau_abs_i8_r(a); }
#endif /* __CHAR_UNSIGNED__ */
_MV_OVERLOAD_INLINE_DEFN float4     mvuAbs( float4 a)               { return __builtin_shave_vau_abs_f32_r(a); }
_MV_OVERLOAD_INLINE_DEFN half8      mvuAbs(  half8 a)               { return __builtin_shave_vau_abs_f16_r(a); }

/* abs diff */
_MV_OVERLOAD_INLINE_DEFN ulong4     mvuAdiff(  long4 a,   long4 b)  { return __builtin_shave_vau_adiff_i32_rr(a, b); }
_MV_OVERLOAD_INLINE_DEFN ulong4     mvuAdiff( ulong4 a,  ulong4 b)  { return __builtin_shave_vau_adiff_u32_rr(a, b); }
_MV_OVERLOAD_INLINE_DEFN uint4      mvuAdiff(   int4 a,    int4 b)  { return __builtin_shave_vau_adiff_i32_rr(a, b); }
_MV_OVERLOAD_INLINE_DEFN uint4      mvuAdiff(  uint4 a,   uint4 b)  { return __builtin_shave_vau_adiff_u32_rr(a, b); }
_MV_OVERLOAD_INLINE_DEFN ushort8    mvuAdiff( short8 a,  short8 b)  { return __builtin_shave_vau_adiff_i16_rr(a, b); }
_MV_OVERLOAD_INLINE_DEFN ushort8    mvuAdiff(ushort8 a, ushort8 b)  { return __builtin_shave_vau_adiff_u16_rr(a, b); }
_MV_OVERLOAD_INLINE_DEFN uchar16    mvuAdiff(schar16 a, schar16 b)  { return __builtin_shave_vau_adiff_i8_rr(a, b); }
#ifdef __CHAR_UNSIGNED__
_MV_OVERLOAD_INLINE_DEFN uchar16    mvuAdiff( char16 a,  char16 b)  { return __builtin_shave_vau_adiff_u8_rr(a, b); }
#else /* __CHAR_UNSIGNED__ */
_MV_OVERLOAD_INLINE_DEFN uchar16    mvuAdiff( char16 a,  char16 b)  { return __builtin_shave_vau_adiff_i8_rr(a, b); }
#endif /* __CHAR_UNSIGNED__ */
_MV_OVERLOAD_INLINE_DEFN uchar16    mvuAdiff(uchar16 a, uchar16 b)  { return __builtin_shave_vau_adiff_u8_rr(a, b); }
_MV_OVERLOAD_INLINE_DEFN float4     mvuAdiff( float4 a,  float4 b)  { return __builtin_shave_vau_adiff_f32_rr(a, b); }
_MV_OVERLOAD_INLINE_DEFN half8      mvuAdiff(  half8 a,   half8 b)  { return __builtin_shave_vau_adiff_f16_rr(a, b); }

/* any */
_MV_OVERLOAD_INLINE_DEFN int        mvuAny(  long4 a)               { return ((__builtin_shave_sau_orx_x32_r(a)  & 0x80000000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAny( ulong4 a)               { return ((__builtin_shave_sau_orx_x32_r(a)  & 0x80000000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAny(   int4 a)               { return ((__builtin_shave_sau_orx_x32_r(a)  & 0x80000000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAny(  uint4 a)               { return ((__builtin_shave_sau_orx_x32_r(a)  & 0x80000000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAny( short8 a)               { return ((__builtin_shave_sau_orx_x16_r(a)  &     0x8000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAny(ushort8 a)               { return ((__builtin_shave_sau_orx_x16_r(a)  &     0x8000u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAny(schar16 a)               { return ((__builtin_shave_sau_orx_x8_r(a)   &       0x80u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAny( char16 a)               { return ((__builtin_shave_sau_orx_x8_r(a)   &       0x80u) != 0); }
_MV_OVERLOAD_INLINE_DEFN int        mvuAny(uchar16 a)               { return ((__builtin_shave_sau_orx_x8_r(a)   &       0x80u) != 0); }

/* cross */
_MV_OVERLOAD_INLINE_DEFN float4     mvuCross(float4 a, float4 b)
{
    float4 tmp1, tmp2, tmp3, dst;
    __asm(
                    "LSU0.SWZMV4.WORD [0021] [UUUU]\n"
            "            || VAU.CMB.WORD %[tmp1] %[a] [EEEE]\n"
            "            || LSU1.SWZMC4.WORD [0102] [UUUU]\n"
#if !defined(__myriad2__) || (__myriad2__ < 3)
            "            || CMU.CPVV %[tmp2] %[b]\n"
            "        VAU.MUL.f32 %[tmp3] %[tmp1] %[tmp2]\n"
            "            || LSU0.SWZMC4.WORD [0021] [UUUU]\n"
            "            || CMU.CPVV %[tmp1] %[b]\n"
            "        LSU0.SWZMC4.WORD [0102] [UUUU]\n"
            "            || CMU.CPVV %[tmp2] %[a]\n"
#else
            "            || CMU.ALIGNVEC %[result] %[b] %[b] 0\n"
            "        CMU.CP.128 %[tmp2] %[result]\n"
            "        VAU.MUL.f32 %[tmp3] %[tmp1] %[tmp2]\n"
            "            || LSU0.SWZMC4.WORD [0021] [UUUU]\n"
            "            || CMU.ALIGNVEC %[result] %[b] %[b] 0\n"
            "        CMU.CP.128 %[tmp1] %[result]\n"
            "        LSU0.SWZMC4.WORD [0102] [UUUU]\n"
            "            || CMU.ALIGNVEC %[result] %[a] %[a] 0\n"
            "        CMU.CP.128 %[tmp2] %[result]\n"
#endif
            "        VAU.MUL.f32 %[tmp1] %[tmp2] %[tmp1]\n"
            "        NOP 2\n"
            "        VAU.SUB.f32 %[result] %[tmp3] %[tmp1]\n"
            "        NOP 2"
            : [result] "=&r"(dst),                                          /* The results */
              [tmp1] "=&r"(tmp1), [tmp2] "=&r"(tmp2), [tmp3] "=&r"(tmp3)    /* Intermediate scratch registers */
            : [a] "r"(a), [b] "r"(b)                                        /* The input operands */
    );
    return dst;
}

/* dot */
_MV_OVERLOAD_INLINE_DEFN long       mvuDot(  long4 a,   long4 b)    { return __builtin_shave_sau_sumx_i32_r(a * b); }
_MV_OVERLOAD_INLINE_DEFN ulong      mvuDot( ulong4 a,  ulong4 b)    { return __builtin_shave_sau_sumx_u32_r(a * b); }
_MV_OVERLOAD_INLINE_DEFN int        mvuDot(   int4 a,    int4 b)    { return __builtin_shave_sau_sumx_i32_r(a * b); }
_MV_OVERLOAD_INLINE_DEFN uint       mvuDot(  uint4 a,   uint4 b)    { return __builtin_shave_sau_sumx_u32_r(a * b); }
_MV_OVERLOAD_INLINE_DEFN short      mvuDot( short8 a,  short8 b)    { return __builtin_shave_sau_sumx_i16_r(a * b); }
_MV_OVERLOAD_INLINE_DEFN ushort     mvuDot(ushort8 a, ushort8 b)    { return __builtin_shave_sau_sumx_u16_r(a * b); }
_MV_OVERLOAD_INLINE_DEFN schar      mvuDot(schar16 a, schar16 b)    { return __builtin_shave_sau_sumx_i8_r(a * b); }
# ifdef __CHAR_UNSIGNED__
_MV_OVERLOAD_INLINE_DEFN char       mvuDot( char16 a,  char16 b)    { return __builtin_shave_sau_sumx_u8_r(a * b); }
# else /* __CHAR_UNSIGNED__ */
_MV_OVERLOAD_INLINE_DEFN char       mvuDot( char16 a,  char16 b)    { return __builtin_shave_sau_sumx_i8_r(a * b); }
# endif /* __CHAR_UNSIGNED__ */
_MV_OVERLOAD_INLINE_DEFN uchar      mvuDot(uchar16 a, uchar16 b)    { return __builtin_shave_sau_sumx_u8_r(a * b); }
_MV_OVERLOAD_INLINE_DEFN float      mvuDot( float4 a,  float4 b)    { return __builtin_shave_sau_sumx_f32_r(a * b); }
_MV_OVERLOAD_INLINE_DEFN half       mvuDot(  half8 a,   half8 b)    { return __builtin_shave_sau_sumx_f16_r(a * b); }

/* max */
_MV_OVERLOAD_INLINE_DEFN long4      mvuMax(  long4 a,   long4 b)    { return __builtin_shave_cmu_max_i32_rr_int4(a, b); }
_MV_OVERLOAD_INLINE_DEFN ulong4     mvuMax( ulong4 a,  ulong4 b)    { return __builtin_shave_cmu_max_u32_rr_uint4(a, b); }
_MV_OVERLOAD_INLINE_DEFN int4       mvuMax(   int4 a,    int4 b)    { return __builtin_shave_cmu_max_i32_rr_int4(a, b); }
_MV_OVERLOAD_INLINE_DEFN uint4      mvuMax(  uint4 a,   uint4 b)    { return __builtin_shave_cmu_max_u32_rr_uint4(a, b); }
_MV_OVERLOAD_INLINE_DEFN short8     mvuMax( short8 a,  short8 b)    { return __builtin_shave_cmu_max_i16_rr_short8(a, b); }
_MV_OVERLOAD_INLINE_DEFN ushort8    mvuMax(ushort8 a, ushort8 b)    { return __builtin_shave_cmu_max_u16_rr_ushort8(a, b); }
_MV_OVERLOAD_INLINE_DEFN schar16    mvuMax(schar16 a, schar16 b)    { return __builtin_shave_cmu_max_i8_rr_char16(a, b); }
#ifdef __CHAR_UNSIGNED__
_MV_OVERLOAD_INLINE_DEFN char16     mvuMax( char16 a,  char16 b)    { return __builtin_shave_cmu_max_u8_rr_uchar16(a, b); }
#else /* __CHAR_UNSIGNED__ */
_MV_OVERLOAD_INLINE_DEFN char16     mvuMax( char16 a,  char16 b)    { return __builtin_shave_cmu_max_i8_rr_char16(a, b); }
#endif /* __CHAR_UNSIGNED__ */
_MV_OVERLOAD_INLINE_DEFN uchar16    mvuMax(uchar16 a, uchar16 b)    { return __builtin_shave_cmu_max_u8_rr_uchar16(a, b); }
_MV_OVERLOAD_INLINE_DEFN float4     mvuMax( float4 a,  float4 b)    { return __builtin_shave_cmu_max_f32_rr_float4(a, b); }
_MV_OVERLOAD_INLINE_DEFN half8      mvuMax(  half8 a,   half8 b)    { return __builtin_shave_cmu_max_f16_rr_half8(a, b); }

/* min */
_MV_OVERLOAD_INLINE_DEFN long4      mvuMin(  long4 a,   long4 b)    { return __builtin_shave_cmu_min_i32_rr_int4(a, b); }
_MV_OVERLOAD_INLINE_DEFN ulong4     mvuMin( ulong4 a,  ulong4 b)    { return __builtin_shave_cmu_min_u32_rr_uint4(a, b); }
_MV_OVERLOAD_INLINE_DEFN int4       mvuMin(   int4 a,    int4 b)    { return __builtin_shave_cmu_min_i32_rr_int4(a, b); }
_MV_OVERLOAD_INLINE_DEFN uint4      mvuMin(  uint4 a,   uint4 b)    { return __builtin_shave_cmu_min_u32_rr_uint4(a, b); }
_MV_OVERLOAD_INLINE_DEFN short8     mvuMin( short8 a,  short8 b)    { return __builtin_shave_cmu_min_i16_rr_short8(a, b); }
_MV_OVERLOAD_INLINE_DEFN ushort8    mvuMin(ushort8 a, ushort8 b)    { return __builtin_shave_cmu_min_u16_rr_ushort8(a, b); }
_MV_OVERLOAD_INLINE_DEFN schar16    mvuMin(schar16 a, schar16 b)    { return __builtin_shave_cmu_min_i8_rr_char16(a, b); }
#ifdef __CHAR_UNSIGNED__                                                                                              
_MV_OVERLOAD_INLINE_DEFN char16     mvuMin( char16 a,  char16 b)    { return __builtin_shave_cmu_min_u8_rr_uchar16(a, b); }
#else /* __CHAR_UNSIGNED__                                                                                             */
_MV_OVERLOAD_INLINE_DEFN char16     mvuMin( char16 a,  char16 b)    { return __builtin_shave_cmu_min_i8_rr_char16(a, b); }
#endif /* __CHAR_UNSIGNED__                                                                                            */
_MV_OVERLOAD_INLINE_DEFN uchar16    mvuMin(uchar16 a, uchar16 b)    { return __builtin_shave_cmu_min_u8_rr_uchar16(a, b); }
_MV_OVERLOAD_INLINE_DEFN float4     mvuMin( float4 a,  float4 b)    { return __builtin_shave_cmu_min_f32_rr_float4(a, b); }
_MV_OVERLOAD_INLINE_DEFN half8      mvuMin(  half8 a,   half8 b)    { return __builtin_shave_cmu_min_f16_rr_half8(a, b); }

/* 4x4 matrix rotate right by 90 degrees */
_MV_OVERLOAD_DECL        long4x4    mvuRotate_right( long4x4 matrix);
_MV_OVERLOAD_DECL        ulong4x4   mvuRotate_right(ulong4x4 matrix);
_MV_OVERLOAD_DECL        int4x4     mvuRotate_right(  int4x4 matrix);
_MV_OVERLOAD_DECL        uint4x4    mvuRotate_right( uint4x4 matrix);
_MV_OVERLOAD_DECL        float4x4   mvuRotate_right(float4x4 matrix);

/* 4x4 matrix rotate left by 90 degrees */
_MV_OVERLOAD_DECL        long4x4    mvuRotate_left( long4x4 matrix);
_MV_OVERLOAD_DECL        ulong4x4   mvuRotate_left(ulong4x4 matrix);
_MV_OVERLOAD_DECL        int4x4     mvuRotate_left(  int4x4 matrix);
_MV_OVERLOAD_DECL        uint4x4    mvuRotate_left( uint4x4 matrix);
_MV_OVERLOAD_DECL        float4x4   mvuRotate_left(float4x4 matrix);

/* 4x4 matrix transpose */
_MV_OVERLOAD_DECL        long4x4    mvuTranspose( long4x4 matrix);
_MV_OVERLOAD_DECL        ulong4x4   mvuTranspose(ulong4x4 matrix);
_MV_OVERLOAD_DECL        int4x4     mvuTranspose(  int4x4 matrix);
_MV_OVERLOAD_DECL        uint4x4    mvuTranspose( uint4x4 matrix);
_MV_OVERLOAD_DECL        float4x4   mvuTranspose(float4x4 matrix);

/* 8x8 matrix transpose */
_MV_OVERLOAD_DECL        short8x8   mvuTranspose( short8x8 matrix);
_MV_OVERLOAD_DECL        ushort8x8  mvuTranspose(ushort8x8 matrix);
_MV_OVERLOAD_DECL        half8x8    mvuTranspose(  half8x8 matrix);



#endif /* _MOVIVECTORFUNCTIONS_H_ */
