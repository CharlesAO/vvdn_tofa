/*
   This header contains function prototypes for all SHAVE ISA builtins

   Each builtin has an exact correspondence to an instruction in the SHAVE ISA.

   Naming convention:
       __builtin_shave_<functional-unit>_<instruction>_<instruction-type>_<operand-types>
       For example: __builtin_shave_vau_add_i32s_ri
         This is the builtin for VAU.ADD.i32s <vrf:Dst> <vrf:srcA> <immu5:B>
*/

#ifndef BUILTINFUNCTIONS_H_
#define BUILTINFUNCTIONS_H_
#include <mv_types.h>
#include <math.h>
#include <half.h>
#include "VectorTypes.h"

// defines for old builtin names

#define __builtin_shave_vau_cmbbyte_vrf_rriiii_int4 __builtin_shave_vau_cmbbyte_rriiii_int4
#define __builtin_shave_vau_cmbbyte_vrf_rriiii_short8 __builtin_shave_vau_cmbbyte_rriiii_short8
#define __builtin_shave_vau_cmbbyte_vrf_rriiii_char16 __builtin_shave_vau_cmbbyte_rriiii_char16
#define __builtin_shave_vau_cmbword_vrf_rriiii_float4 __builtin_shave_vau_cmbword_rriiii_float4
#define __builtin_shave_vau_cmbword_vrf_rriiii_int4 __builtin_shave_vau_cmbword_rriiii_int4
#define __builtin_shave_vau_cmbword_vrf_rriiii_short8 __builtin_shave_vau_cmbword_rriiii_short8
#define __builtin_shave_vau_cmbword_vrf_rriiii_char16 __builtin_shave_vau_cmbword_rriiii_char16

#define __builtin_shave_vau_alignbyte_vrf_rri_int4 __builtin_shave_vau_alignbyte_rri_int4
#define __builtin_shave_vau_alignbyte_vrf_rri_short8 __builtin_shave_vau_alignbyte_rri_short8
#define __builtin_shave_vau_alignbyte_vrf_rri_char16 __builtin_shave_vau_alignbyte_rri_char16

#define __builtin_shave_cmu_alignvec_vrf_rri_float4 __builtin_shave_cmu_alignvec_rri_float4
#define __builtin_shave_cmu_alignvec_vrf_rri_int4 __builtin_shave_cmu_alignvec_rri_int4
#define __builtin_shave_cmu_alignvec_vrf_rri_short8 __builtin_shave_cmu_alignvec_rri_short8
#define __builtin_shave_cmu_alignvec_vrf_rri_char16 __builtin_shave_cmu_alignvec_rri_char16

#define __builtin_shave_cmu_max_v4f32_rr __builtin_shave_cmu_max_f32_rr_float4
#define __builtin_shave_cmu_max_v8f16_rr __builtin_shave_cmu_max_f16_rr_half8
#define __builtin_shave_cmu_max_v4i32_rr __builtin_shave_cmu_max_i32_rr_int4
#define __builtin_shave_cmu_max_v4u32_rr __builtin_shave_cmu_max_u32_rr_uint4
#define __builtin_shave_cmu_max_v8i16_rr __builtin_shave_cmu_max_i16_rr_short8
#define __builtin_shave_cmu_max_v8u16_rr __builtin_shave_cmu_max_u16_rr_ushort8
#define __builtin_shave_cmu_max_v16i8_rr __builtin_shave_cmu_max_i8_rr_char16
#define __builtin_shave_cmu_max_v16u8_rr __builtin_shave_cmu_max_u8_rr_uchar16
#define __builtin_shave_cmu_max_f32_rr __builtin_shave_cmu_max_f32_rr_float
#define __builtin_shave_cmu_max_f16_rr __builtin_shave_cmu_max_f16_rr_half
#define __builtin_shave_cmu_max_i32_rr __builtin_shave_cmu_max_i32_rr_int
#define __builtin_shave_cmu_max_u32_rr __builtin_shave_cmu_max_u32_rr_uint
#define __builtin_shave_cmu_max_i16_rr __builtin_shave_cmu_max_i16_rr_short
#define __builtin_shave_cmu_max_u16_rr __builtin_shave_cmu_max_u16_rr_ushort
#define __builtin_shave_cmu_max_i8_rr __builtin_shave_cmu_max_i8_rr_char
#define __builtin_shave_cmu_max_u8_rr __builtin_shave_cmu_max_u8_rr_uchar

#define __builtin_shave_cmu_min_v4f32_rr __builtin_shave_cmu_min_f32_rr_float4
#define __builtin_shave_cmu_min_v8f16_rr __builtin_shave_cmu_min_f16_rr_half8
#define __builtin_shave_cmu_min_v4i32_rr __builtin_shave_cmu_min_i32_rr_int4
#define __builtin_shave_cmu_min_v4u32_rr __builtin_shave_cmu_min_u32_rr_uint4
#define __builtin_shave_cmu_min_v8i16_rr __builtin_shave_cmu_min_i16_rr_short8
#define __builtin_shave_cmu_min_v8u16_rr __builtin_shave_cmu_min_u16_rr_ushort8
#define __builtin_shave_cmu_min_v16i8_rr __builtin_shave_cmu_min_i8_rr_char16
#define __builtin_shave_cmu_min_v16u8_rr __builtin_shave_cmu_min_u8_rr_uchar16
#define __builtin_shave_cmu_min_f32_rr __builtin_shave_cmu_min_f32_rr_float
#define __builtin_shave_cmu_min_f16_rr __builtin_shave_cmu_min_f16_rr_half
#define __builtin_shave_cmu_min_i32_rr __builtin_shave_cmu_min_i32_rr_int
#define __builtin_shave_cmu_min_u32_rr __builtin_shave_cmu_min_u32_rr_uint
#define __builtin_shave_cmu_min_i16_rr __builtin_shave_cmu_min_i16_rr_short
#define __builtin_shave_cmu_min_u16_rr __builtin_shave_cmu_min_u16_rr_ushort
#define __builtin_shave_cmu_min_i8_rr __builtin_shave_cmu_min_i8_rr_char
#define __builtin_shave_cmu_min_u8_rr __builtin_shave_cmu_min_u8_rr_uchar

#define __builtin_shave_cmu_clampab_v4f32_rrr __builtin_shave_cmu_clampab_f32_rrr_float4
#define __builtin_shave_cmu_clampab_v8f16_rrr __builtin_shave_cmu_clampab_f16_rrr_half8
#define __builtin_shave_cmu_clampab_v4i32_rrr __builtin_shave_cmu_clampab_i32_rrr_int4
#define __builtin_shave_cmu_clampab_v4u32_rrr __builtin_shave_cmu_clampab_u32_rrr_uint4
#define __builtin_shave_cmu_clampab_v8i16_rrr __builtin_shave_cmu_clampab_i16_rrr_short8
#define __builtin_shave_cmu_clampab_v8u16_rrr __builtin_shave_cmu_clampab_u16_rrr_ushort8
#define __builtin_shave_cmu_clampab_v16i8_rrr __builtin_shave_cmu_clampab_i8_rrr_char16
#define __builtin_shave_cmu_clampab_v16u8_rrr __builtin_shave_cmu_clampab_u8_rrr_uchar16
#define __builtin_shave_cmu_clampab_f32_rrr __builtin_shave_cmu_clampab_f32_rrr_float
#define __builtin_shave_cmu_clampab_f16_rrr __builtin_shave_cmu_clampab_f16_rrr_half
#define __builtin_shave_cmu_clampab_i32_rrr __builtin_shave_cmu_clampab_i32_rrr_int
#define __builtin_shave_cmu_clampab_u32_rrr __builtin_shave_cmu_clampab_u32_rrr_uint
#define __builtin_shave_cmu_clampab_i16_rrr __builtin_shave_cmu_clampab_i16_rrr_short
#define __builtin_shave_cmu_clampab_u16_rrr __builtin_shave_cmu_clampab_u16_rrr_ushort
#define __builtin_shave_cmu_clampab_i8_rrr __builtin_shave_cmu_clampab_i8_rrr_char
#define __builtin_shave_cmu_clampab_u8_rrr __builtin_shave_cmu_clampab_u8_rrr_uchar

#define __builtin_shave_cmu_clamp_v4f32_rr __builtin_shave_cmu_clamp_f32_rr_float4
#define __builtin_shave_cmu_clamp_v8f16_rr __builtin_shave_cmu_clamp_f16_rr_half8
#define __builtin_shave_cmu_clamp_v4i32_rr __builtin_shave_cmu_clamp_i32_rr_int4
#define __builtin_shave_cmu_clamp_v8i16_rr __builtin_shave_cmu_clamp_i16_rr_short8
#define __builtin_shave_cmu_clamp_v16i8_rr __builtin_shave_cmu_clamp_i8_rr_char16
#define __builtin_shave_cmu_clamp_f32_rr __builtin_shave_cmu_clamp_f32_rr_float
#define __builtin_shave_cmu_clamp_f16_rr __builtin_shave_cmu_clamp_f16_rr_half
#define __builtin_shave_cmu_clamp_i32_rr __builtin_shave_cmu_clamp_i32_rr_int
#define __builtin_shave_cmu_clamp_i16_rr __builtin_shave_cmu_clamp_i16_rr_short
#define __builtin_shave_cmu_clamp_i8_rr __builtin_shave_cmu_clamp_i8_rr_char

#define __builtin_shave_cmu_clamp0_v4f32_rr __builtin_shave_cmu_clamp0_f32_rr_float4
#define __builtin_shave_cmu_clamp0_v8f16_rr __builtin_shave_cmu_clamp0_f16_rr_half8
#define __builtin_shave_cmu_clamp0_v4i32_rr __builtin_shave_cmu_clamp0_i32_rr_int4
#define __builtin_shave_cmu_clamp0_v8i16_rr __builtin_shave_cmu_clamp0_i16_rr_short8
#define __builtin_shave_cmu_clamp0_v16i8_rr __builtin_shave_cmu_clamp0_i8_rr_char16
#define __builtin_shave_cmu_clamp0_f32_rr __builtin_shave_cmu_clamp0_f32_rr_float
#define __builtin_shave_cmu_clamp0_f16_rr __builtin_shave_cmu_clamp0_f16_rr_half
#define __builtin_shave_cmu_clamp0_i32_rr __builtin_shave_cmu_clamp0_i32_rr_int
#define __builtin_shave_cmu_clamp0_i16_rr __builtin_shave_cmu_clamp0_i16_rr_short
#define __builtin_shave_cmu_clamp0_i8_rr __builtin_shave_cmu_clamp0_i8_rr_char

#define __builtin_shave_cmu_vszmword_vrf_riiii_int4 __builtin_shave_cmu_vszmword_riiii_int4
#define __builtin_shave_cmu_vszmword_vrf_riiii_float4 __builtin_shave_cmu_vszmword_riiii_float4
#define __builtin_shave_cmu_vszmword_vrf_riiii_short8 __builtin_shave_cmu_vszmword_riiii_short8
#define __builtin_shave_cmu_vszmword_vrf_riiii_char16 __builtin_shave_cmu_vszmword_riiii_char16
#define __builtin_shave_cmu_vszmbyte_vrf_riiii_int4 __builtin_shave_cmu_vszmbyte_riiii_int4
#define __builtin_shave_cmu_vszmbyte_vrf_riiii_short8 __builtin_shave_cmu_vszmbyte_riiii_short8
#define __builtin_shave_cmu_vszmbyte_vrf_riiii_char16 __builtin_shave_cmu_vszmbyte_riiii_char16

#define __builtin_shave_cmu_cmask_v4i32_r __builtin_shave_cmu_cmask_r_int4
#define __builtin_shave_cmu_cmask_v8i16_r __builtin_shave_cmu_cmask_r_short8
#define __builtin_shave_cmu_cmask_v16i8_r __builtin_shave_cmu_cmask_r_char16

#define __builtin_shave_cmu_alignvec_vrf_rri_float4 __builtin_shave_cmu_alignvec_rri_float4
#define __builtin_shave_cmu_alignvec_vrf_rri_int4 __builtin_shave_cmu_alignvec_rri_int4
#define __builtin_shave_cmu_alignvec_vrf_rri_short8 __builtin_shave_cmu_alignvec_rri_short8
#define __builtin_shave_cmu_alignvec_vrf_rri_char16 __builtin_shave_cmu_alignvec_rri_char16

/*mvuConvert*/

// Scalars
#define mvuConvert_int(x)          ((int)(x))
#define mvuConvert_uint(x)         ((uint)(x))
#define mvuConvert_short(x)        ((short)(x))
#define mvuConvert_ushort(x)       ((ushort)(x))
#define mvuConvert_char(x)         ((char)(x))
#define mvuConvert_uchar(x)        ((uchar)(x))
#define mvuConvert_float(x)        ((float)(x))
#define mvuConvert_half(x)         ((half)(x))

// Cardinal 2 vectors
#define mvuConvert_int2(x)         (convertor_type<int2>((x),                   (int)0))
#define mvuConvert_uint2(x)        (convertor_type<uint2>((x),         (unsigned int)0))
#define mvuConvert_short2(x)       (convertor_type<short2>((x),               (short)0))
#define mvuConvert_ushort2(x)      (convertor_type<ushort2>((x),     (unsigned short)0))
#define mvuConvert_char2(x)        (convertor_type<char2>((x),                 (char)0))
#define mvuConvert_uchar2(x)       (convertor_type<uchar2>((x),       (unsigned char)0))
#define mvuConvert_float2(x)       (convertor_type<float2>((x),               (float)0))
#define mvuConvert_half2(x)        (convertor_type<half2>((x),                 (half)0))

// Cardinal 3 vectors
#define mvuConvert_int3(x)         (convertor_type<int3>((x),                   (int)0))
#define mvuConvert_uint3(x)        (convertor_type<uint3>((x),         (unsigned int)0))
#define mvuConvert_short3(x)       (convertor_type<short3>((x),               (short)0))
#define mvuConvert_ushort3(x)      (convertor_type<ushort3>((x),     (unsigned short)0))
#define mvuConvert_char3(x)        (convertor_type<char3>((x),                 (char)0))
#define mvuConvert_uchar3(x)       (convertor_type<uchar3>((x),       (unsigned char)0))
#define mvuConvert_float3(x)       (convertor_type<float3>((x),               (float)0))
#define mvuConvert_half3(x)        (convertor_type<half3>((x),                 (half)0))

// Cardinal 4 vectors
#define mvuConvert_int4(x)         (convertor_type<int4>((x),                   (int)0))
#define mvuConvert_uint4(x)        (convertor_type<uint4>((x),         (unsigned int)0))
#define mvuConvert_short4(x)       (convertor_type<short4>((x),               (short)0))
#define mvuConvert_ushort4(x)      (convertor_type<ushort4>((x),     (unsigned short)0))
#define mvuConvert_char4(x)        (convertor_type<char4>((x),                 (char)0))
#define mvuConvert_uchar4(x)       (convertor_type<uchar4>((x),       (unsigned char)0))
#define mvuConvert_float4(x)       (convertor_type<float4>((x),               (float)0))
#define mvuConvert_half4(x)        (convertor_type<half4>((x),                 (half)0))

// Cardinal 8 vectors
#define mvuConvert_int8(x)         (convertor_type<int8>((x),                   (int)0))
#define mvuConvert_uint8(x)        (convertor_type<uint8>((x),         (unsigned int)0))
#define mvuConvert_short8(x)       (convertor_type<short8>((x),               (short)0))
#define mvuConvert_ushort8(x)      (convertor_type<ushort8>((x),     (unsigned short)0))
#define mvuConvert_char8(x)        (convertor_type<char8>((x),                 (char)0))
#define mvuConvert_uchar8(x)       (convertor_type<uchar8>((x),       (unsigned char)0))
#define mvuConvert_float8(x)       (convertor_type<float8>((x),               (float)0))
#define mvuConvert_half8(x)        (convertor_type<half8>((x),                 (half)0))

// Cardinal 16 vectors
#define mvuConvert_int16(x)        (convertor_type<int16>((x),                   (int)0))
#define mvuConvert_uint16(x)       (convertor_type<uint16>((x),         (unsigned int)0))
#define mvuConvert_short16(x)      (convertor_type<short16>((x),               (short)0))
#define mvuConvert_ushort16(x)     (convertor_type<ushort16>((x),     (unsigned short)0))
#define mvuConvert_char16(x)       (convertor_type<char16>((x),                 (char)0))
#define mvuConvert_uchar16(x)      (convertor_type<uchar16>((x),       (unsigned char)0))
#define mvuConvert_float16(x)      (convertor_type<float16>((x),               (float)0))
#define mvuConvert_half16(x)       (convertor_type<half16>((x),                 (half)0))



#ifndef _MSC_VER
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif

/*  ========================================= */
/*  3.3 SAU Instructions */
/*  ========================================= */


/*  3.3.6 SAU.DIV */
/*      Scalar floating point and integer division */
float	__builtin_shave_sau_div_f32_rr ( float, float );	/* SAU.DIV.f32 */
int	__builtin_shave_sau_div_i32_rr ( int, int );	/* SAU.DIV.i32 */
unsigned int	__builtin_shave_sau_div_u32_rr ( unsigned int, unsigned int );	/* SAU.DIV.u32 */
short	__builtin_shave_sau_div_i16_rr ( short, short );	/* SAU.DIV.i16 */
unsigned short	__builtin_shave_sau_div_u16_rr ( unsigned short, unsigned short );	/* SAU.DIV.u16 */

/*  3.3.7 SAU.IADDS */
/*      Scalar integer addition with saturation  */

int	__builtin_shave_sau_iadds_i32_rr ( int, int );	/* SAU.IADDS.i32 : Myriad2 only */
unsigned int	__builtin_shave_sau_iadds_u32_rr ( unsigned int, unsigned int );	/* SAU.IADDS.u32 : Myriad2 only */
short	__builtin_shave_sau_iadds_i16_rr ( short, short );	/* SAU.IADDS.i16 : Myriad2 only */
unsigned short	__builtin_shave_sau_iadds_u16_rr ( unsigned short, unsigned short );	/* SAU.IADDS.u16 : Myriad2 only */
char	__builtin_shave_sau_iadds_i8_rr ( char, char );	/* SAU.IADDS.i8 : Myriad2 only */
unsigned char	__builtin_shave_sau_iadds_u8_rr ( unsigned char, unsigned char );	/* SAU.IADDS.u8 : Myriad2 only */


/*  3.3.8 SAU.ISUBS */
/*      Scalar integer subtraction with saturation */

int	__builtin_shave_sau_isubs_i32_rr ( int, int );	/* SAU.ISUBS.i32 : Myriad2 only */
unsigned int	__builtin_shave_sau_isubs_u32_rr ( unsigned int, unsigned int );	/* SAU.ISUBS.u32 : Myriad2 only */
short	__builtin_shave_sau_isubs_i16_rr ( short, short );	/* SAU.ISUBS.i16 : Myriad2 only */
unsigned short	__builtin_shave_sau_isubs_u16_rr ( unsigned short, unsigned short );	/* SAU.ISUBS.u16 : Myriad2 only */
char	__builtin_shave_sau_isubs_i8_rr ( char, char );	/* SAU.ISUBS.i8 : Myriad2 only */
unsigned char	__builtin_shave_sau_isubs_u8_rr ( unsigned char, unsigned char );	/* SAU.ISUBS.u8 : Myriad2 only */


/*  3.3.9 SAU.IMULS */
/*      Scalar integer multiplication with saturation */

int	__builtin_shave_sau_imuls_i32_rr ( int, int );	/* SAU.IMULS.i32 : Myriad2 only */
unsigned int	__builtin_shave_sau_imuls_u32_rr ( unsigned int, unsigned int );	/* SAU.IMULS.u32 : Myriad2 only */
short	__builtin_shave_sau_imuls_i16_rr ( short, short );	/* SAU.IMULS.i16 : Myriad2 only */
unsigned short	__builtin_shave_sau_imuls_u16_rr ( unsigned short, unsigned short );	/* SAU.IMULS.u16 : Myriad2 only */
char	__builtin_shave_sau_imuls_i8_rr ( char, char );	/* SAU.IMULS.i8 : Myriad2 only */
unsigned char	__builtin_shave_sau_imuls_u8_rr ( unsigned char, unsigned char );	/* SAU.IMULS.u8 : Myriad2 only */


/*  3.3.12 SAU.FRAC */
/*      Return fractional part of floating point number */

float	__builtin_shave_sau_frac_f32_r ( float );	/* SAU.FRAC.f32 */
half	__builtin_shave_sau_frac_f16_r ( half );	/* SAU.FRAC.f16 */

/*  3.3.13 SAU.SQT */
/*      Square root function */

half	__builtin_shave_sau_sqt_f16_l_r ( half );	/* SAU.SQT.f16_l */
half	__builtin_shave_sau_sqt_f16_h_r ( half );	/* SAU.SQT.f16_h */

/*  3.3.14 SAU.RCP */
/*      Floating point reciprocal function */

half	__builtin_shave_sau_rcp_f16_l_r ( half );	/* SAU.RCP.f16_l */
half	__builtin_shave_sau_rcp_f16_h_r ( half );	/* SAU.RCP.f16_h */

/*  3.3.15 SAU.RQT */
/*      Floating point reciprocal square root function */

half	__builtin_shave_sau_rqt_f16_l_r ( half );	/* SAU.RQT.f16_l */
half	__builtin_shave_sau_rqt_f16_h_r ( half );	/* SAU.RQT.f16_h */

/*  3.3.16 SAU.SIN */
/*      Floating point trigonometric SINE function, input is in radians */

half	__builtin_shave_sau_sin_f16_l_r ( half );	/* SAU.SIN.f16_l */
half	__builtin_shave_sau_sin_f16_h_r ( half );	/* SAU.SIN.f16_h */

/*  3.3.17 SAU.COS */
/*      Floating point trigonometric COSINE function, input is in radians */

half	__builtin_shave_sau_cos_f16_l_r ( half );	/* SAU.COS.f16_l */
half	__builtin_shave_sau_cos_f16_h_r ( half );	/* SAU.COS.f16_h */

/*  3.3.18 SAU.ATN */
/*      Floating point ARCTANGENT function */

half	__builtin_shave_sau_atn_f16_l_r ( half );	/* SAU.ATN.f16_l */
half	__builtin_shave_sau_atn_f16_h_r ( half );	/* SAU.ATN.f16_h */

/*  3.3.19 SAU.LOG2 */
/*      Floating point Base-2 logarithm function */

half	__builtin_shave_sau_log2_f16_l_r ( half );	/* SAU.LOG2.f16_l */
half	__builtin_shave_sau_log2_f16_h_r ( half );	/* SAU.LOG2.f16_h */

/*  3.3.20 SAU.EXP2 */
/*      Floating point Base-2 exponentiation function */

half	__builtin_shave_sau_exp2_f16_l_r ( half );	/* SAU.EXP2.f16_l */
half	__builtin_shave_sau_exp2_f16_h_r ( half );	/* SAU.EXP2.f16_h */

/*  3.3.27 SAU.ILMULH */
/*      Integer multiply use upper half of result */
int	__builtin_shave_sau_ilmulh_i32_rr ( int, int );	/* SAU.ILMULH.i32 */
short	__builtin_shave_sau_ilmulh_i16_rr ( short, short );	/* SAU.ILMULH.i16 */
unsigned short	__builtin_shave_sau_ilmulh_u16_rr ( unsigned short, unsigned short );	/* SAU.ILMULH.u16 */
char	__builtin_shave_sau_ilmulh_i8_rr ( char, char );	/* SAU.ILMULH.i8 */
unsigned char	__builtin_shave_sau_ilmulh_u8_rr ( unsigned char, unsigned char );	/* SAU.ILMULH.u8 */

/*  3.3.35 SAU.ROL */
/*      Scalar rotate left */
int	__builtin_shave_sau_rol_x32_rr ( int, int );	/* SAU.ROL.x32 */
short	__builtin_shave_sau_rol_x16_rr ( short, short );	/* SAU.ROL.x16 */
char	__builtin_shave_sau_rol_x8_rr ( char, char );	/* SAU.ROL.x8 */

int	__builtin_shave_sau_rol_x32_ri ( int, unsigned char );	/* SAU.ROL.x32 */
short	__builtin_shave_sau_rol_x16_ri ( short, unsigned char );	/* SAU.ROL.x16 */
char	__builtin_shave_sau_rol_x8_ri ( char, unsigned char );	/* SAU.ROL.x8 */

/*  3.3.36 SAU.SHL */
/*      Scalar integer shift left */


int	__builtin_shave_sau_shl_x32_rr ( int, int );	/* SAU.SHL.x32 : Myriad2 only */
short	__builtin_shave_sau_shl_x16_rr ( short, short );	/* SAU.SHL.x16 : Myriad2 only */
char	__builtin_shave_sau_shl_x8_rr ( char, char );	/* SAU.SHL.x8 : Myriad2 only */

/*  3.3.37 SAU.SHR */
/*      Scalar integer shift right */
int	__builtin_shave_sau_shr_i32_rr ( int, int );	/* SAU.SHR.i32 */
unsigned int	__builtin_shave_sau_shr_u32_rr ( unsigned int, unsigned int );	/* SAU.SHR.u32 */
short	__builtin_shave_sau_shr_i16_rr ( short, short );	/* SAU.SHR.i16 */
unsigned short	__builtin_shave_sau_shr_u16_rr ( unsigned short, unsigned short );	/* SAU.SHR.u16 */
char	__builtin_shave_sau_shr_i8_rr ( char, char );	/* SAU.SHR.i8 */
unsigned char	__builtin_shave_sau_shr_u8_rr ( unsigned char, unsigned char );	/* SAU.SHR.u8 */

int	__builtin_shave_sau_shr_i32_ri ( int, unsigned char );	/* SAU.SHR.i32 */
unsigned int	__builtin_shave_sau_shr_u32_ri ( unsigned int, unsigned char );	/* SAU.SHR.u32 */
short	__builtin_shave_sau_shr_i16_ri ( short, unsigned char );	/* SAU.SHR.i16 */
unsigned short	__builtin_shave_sau_shr_u16_ri ( unsigned short, unsigned char );	/* SAU.SHR.u16 */
char	__builtin_shave_sau_shr_i8_ri ( char, unsigned char );	/* SAU.SHR.i8 */
unsigned char	__builtin_shave_sau_shr_u8_ri ( unsigned char, unsigned char );	/* SAU.SHR.u8 */

/*  3.3.39 SAU.ONES */
/*      Scalar integer ones count */

unsigned int	__builtin_shave_sau_onesx_x32_r ( int4 );	/* SAU.ONESX.x32 : Myriad2 only */
unsigned int	__builtin_shave_sau_onesx_x64_r ( short8 );	/* SAU.ONESX.x64 : Myriad2 only */
unsigned int	__builtin_shave_sau_onesx_x128_r ( char16 );	/* SAU.ONESX.x128 : Myriad2 only */

/*      Scalar integer zeroes count */

unsigned int	__builtin_shave_sau_zerosx_x32_r ( int4 );	/* SAU.ZEROSX.x32 : Myriad2 only */
unsigned int	__builtin_shave_sau_zerosx_x64_r ( short8 );	/* SAU.ZEROSX.x64 : Myriad2 only */
unsigned int	__builtin_shave_sau_zerosx_x128_r ( char16 );	/* SAU.ZEROSX.x128 : Myriad2 only */

/*  3.3.40 SAU.ABS */
int	__builtin_shave_sau_abs_i32_r ( int );	/* SAU.ABS.i32 */
short	__builtin_shave_sau_abs_i16_r ( short );	/* SAU.ABS.i16 */
char	__builtin_shave_sau_abs_i8_r ( char );	/* SAU.ABS.i8 */
/*      Scalar floating-point absolute value */
float	__builtin_shave_sau_abs_f32_r ( float );	/* SAU.ABS.f32 */
half	__builtin_shave_sau_abs_f16_r ( half );	/* SAU.ABS.f16 */

/*  3.3.53 SAU.SUMX */
/*      Horizontal sum of vector sum to scalar. Integer results are always 32 bit saturated  */



int	__builtin_shave_sau_sumx_i32_r ( int4 );	/* SAU.SUMX.i32 : Myriad2 only */
int	__builtin_shave_sau_sumx_i16_r ( short8 );	/* SAU.SUMX.i16 : Myriad2 only */
int	__builtin_shave_sau_sumx_i8_r ( char16 );	/* SAU.SUMX.i8 : Myriad2 only */

unsigned int	__builtin_shave_sau_sumx_u32_r ( int4 );	/* SAU.SUMX.u32 : Myriad2 only */
unsigned int	__builtin_shave_sau_sumx_u16_r ( short8 );	/* SAU.SUMX.u16 : Myriad2 only */
unsigned int	__builtin_shave_sau_sumx_u8_r ( char16 );	/* SAU.SUMX.u8 : Myriad2 only */

float	__builtin_shave_sau_sumx_f32_r ( float4 );	/* SAU.SUMX.f32 : Myriad2 only */
half	__builtin_shave_sau_sumx_f16_r ( half8 );	/* SAU.SUMX.f16 : Myriad2 only */

/*      Horizontal sum of vector sum to scalar. Integer results are always 32 bit saturated  */


/*  3.3.55 SAU.ANDX */
/*      Vector AND reduce all elements of a vector to a single element  */
int	__builtin_shave_sau_andx_x32_r ( int4 );	/* SAU.ANDX.x32 */
short	__builtin_shave_sau_andx_x16_r ( short8 );	/* SAU.ANDX.x16 */
char	__builtin_shave_sau_andx_x8_r ( char16 );	/* SAU.ANDX.x8 */

/*  3.3.56 SAU.ORX */
/*      Vector OR reduce all elements of a vector to a single element  */
int	__builtin_shave_sau_orx_x32_r ( int4 );	/* SAU.ORX.x32 */
short	__builtin_shave_sau_orx_x16_r ( short8 );	/* SAU.ORX.x16 */
char	__builtin_shave_sau_orx_x8_r ( char16 );	/* SAU.ORX.x8 */

/*  3.3.57 SAU.XORX */
/*      Vector XOR reduce all elements of a vector to a single element  */
int	__builtin_shave_sau_xorx_x32_r ( int4 );	/* SAU.XORX.x32 */
short	__builtin_shave_sau_xorx_x16_r ( short8 );	/* SAU.XORX.x16 */
char	__builtin_shave_sau_xorx_x8_r ( char16 );	/* SAU.XORX.x8 */

/*  ========================================= */
/*  3.3 VAU Instructions */
/*  ========================================= */


/*  3.3.3 VAU.ADD */
/*      Vector floating point and integer addition */
float4	__builtin_shave_vau_add_f32_rr ( float4, float4 );	/* VAU.ADD.f32 */
int4	__builtin_shave_vau_add_i32_rr ( int4, int4 );	/* VAU.ADD.i32 */
half8	__builtin_shave_vau_add_f16_rr ( half8, half8 );	/* VAU.ADD.f16 */
short8	__builtin_shave_vau_add_i16_rr ( short8, short8 );	/* VAU.ADD.i16 */
char16	__builtin_shave_vau_add_i8_rr ( char16, char16 );	/* VAU.ADD.i8 */

int4	__builtin_shave_vau_add_i32_ri ( int4, unsigned char );	/* VAU.ADD.i32 */
short8	__builtin_shave_vau_add_i16_ri ( short8, unsigned char );	/* VAU.ADD.i16 */
char16	__builtin_shave_vau_add_i8_ri ( char16, unsigned char );	/* VAU.ADD.i8 */

/*  3.3.4 VAU.SUB */
/*      Vector floating point and integer subtraction */
float4	__builtin_shave_vau_sub_f32_rr ( float4, float4 );	/* VAU.SUB.f32 */
int4	__builtin_shave_vau_sub_i32_rr ( int4, int4 );	/* VAU.SUB.i32 */
half8	__builtin_shave_vau_sub_f16_rr ( half8, half8 );	/* VAU.SUB.f16 */
short8	__builtin_shave_vau_sub_i16_rr ( short8, short8 );	/* VAU.SUB.i16 */
char16	__builtin_shave_vau_sub_i8_rr ( char16, char16 );	/* VAU.SUB.i8 */

int4	__builtin_shave_vau_sub_i32_ri ( int4, unsigned char );	/* VAU.SUB.i32 */
short8	__builtin_shave_vau_sub_i16_ri ( short8, unsigned char );	/* VAU.SUB.i16 */
char16	__builtin_shave_vau_sub_i8_ri ( char16, unsigned char );	/* VAU.SUB.i8 */

/*  3.3.5 VAU.MUL */
/*      Vector floating point and integer multiplication */
float4	__builtin_shave_vau_mul_f32_rr ( float4, float4 );	/* VAU.MUL.f32 */
int4	__builtin_shave_vau_mul_i32_rr ( int4, int4 );	/* VAU.MUL.i32 */
half8	__builtin_shave_vau_mul_f16_rr ( half8, half8 );	/* VAU.MUL.f16 */
short8	__builtin_shave_vau_mul_i16_rr ( short8, short8 );	/* VAU.MUL.i16 */
char16	__builtin_shave_vau_mul_i8_rr ( char16, char16 );	/* VAU.MUL.i8 */

int4	__builtin_shave_vau_mul_i32_ri ( int4, char );	/* VAU.MUL.i32 */
short8	__builtin_shave_vau_mul_i16_ri ( short8, char );	/* VAU.MUL.i16 */
char16	__builtin_shave_vau_mul_i8_ri ( char16, char );	/* VAU.MUL.i8 */

/*  3.3.7 VAU.IADDS */
/*      Vector integer addition with saturation */

int4	__builtin_shave_vau_iadds_i32_rr ( int4, int4 );	/* VAU.IADDS.i32 : Myriad2 only */
uint4	__builtin_shave_vau_iadds_u32_rr ( uint4, uint4 );	/* VAU.IADDS.u32 : Myriad2 only */
short8	__builtin_shave_vau_iadds_i16_rr ( short8, short8 );	/* VAU.IADDS.i16 : Myriad2 only */
ushort8	__builtin_shave_vau_iadds_u16_rr ( ushort8, ushort8 );	/* VAU.IADDS.u16 : Myriad2 only */
char16	__builtin_shave_vau_iadds_i8_rr ( char16, char16 );	/* VAU.IADDS.i8 : Myriad2 only */
uchar16	__builtin_shave_vau_iadds_u8_rr ( uchar16, uchar16 );	/* VAU.IADDS.u8 : Myriad2 only */


int4	__builtin_shave_vau_iadds_i32_ri ( int4, unsigned char );	/* VAU.IADDS.i32 : Myriad2 only */
uint4	__builtin_shave_vau_iadds_u32_ri ( uint4, unsigned char );	/* VAU.IADDS.u32 : Myriad2 only */
short8	__builtin_shave_vau_iadds_i16_ri ( short8, unsigned char );	/* VAU.IADDS.i16 : Myriad2 only */
ushort8	__builtin_shave_vau_iadds_u16_ri ( ushort8, unsigned char );	/* VAU.IADDS.u16 : Myriad2 only */
char16	__builtin_shave_vau_iadds_i8_ri ( char16, unsigned char );	/* VAU.IADDS.i8 : Myriad2 only */
uchar16	__builtin_shave_vau_iadds_u8_ri ( uchar16, unsigned char );	/* VAU.IADDS.u8 : Myriad2 only */

/*  3.3.8 VAU.ISUBS */
/*      Vector integer subtraction with saturation */

int4	__builtin_shave_vau_isubs_i32_rr ( int4, int4 );	/* VAU.ISUBS.i32 : Myriad2 only */
uint4	__builtin_shave_vau_isubs_u32_rr ( uint4, uint4 );	/* VAU.ISUBS.u32 : Myriad2 only */
short8	__builtin_shave_vau_isubs_i16_rr ( short8, short8 );	/* VAU.ISUBS.i16 : Myriad2 only */
ushort8	__builtin_shave_vau_isubs_u16_rr ( ushort8, ushort8 );	/* VAU.ISUBS.u16 : Myriad2 only */
char16	__builtin_shave_vau_isubs_i8_rr ( char16, char16 );	/* VAU.ISUBS.i8 : Myriad2 only */
uchar16	__builtin_shave_vau_isubs_u8_rr ( uchar16, uchar16 );	/* VAU.ISUBS.u8 : Myriad2 only */


int4	__builtin_shave_vau_isubs_i32_ri ( int4, unsigned char );	/* VAU.ISUBS.i32 : Myriad2 only */
uint4	__builtin_shave_vau_isubs_u32_ri ( uint4, unsigned char );	/* VAU.ISUBS.u32 : Myriad2 only */
short8	__builtin_shave_vau_isubs_i16_ri ( short8, unsigned char );	/* VAU.ISUBS.i16 : Myriad2 only */
ushort8	__builtin_shave_vau_isubs_u16_ri ( ushort8, unsigned char );	/* VAU.ISUBS.u16 : Myriad2 only */
char16	__builtin_shave_vau_isubs_i8_ri ( char16, unsigned char );	/* VAU.ISUBS.i8 : Myriad2 only */
uchar16	__builtin_shave_vau_isubs_u8_ri ( uchar16, unsigned char );	/* VAU.ISUBS.u8 : Myriad2 only */

/*  3.3.9 VAU.IMULS */
/*      Vector integer multiplication with saturation */

int4	__builtin_shave_vau_imuls_i32_rr ( int4, int4 );	/* VAU.IMULS.i32 : Myriad2 only */
uint4	__builtin_shave_vau_imuls_u32_rr ( uint4, uint4 );	/* VAU.IMULS.u32 : Myriad2 only */
short8	__builtin_shave_vau_imuls_i16_rr ( short8, short8 );	/* VAU.IMULS.i16 : Myriad2 only */
ushort8	__builtin_shave_vau_imuls_u16_rr ( ushort8, ushort8 );	/* VAU.IMULS.u16 : Myriad2 only */
char16	__builtin_shave_vau_imuls_i8_rr ( char16, char16 );	/* VAU.IMULS.i8 : Myriad2 only */
uchar16	__builtin_shave_vau_imuls_u8_rr ( uchar16, uchar16 );	/* VAU.IMULS.u8 : Myriad2 only */


int4	__builtin_shave_vau_imuls_i32_ri ( int4, unsigned char );	/* VAU.IMULS.i32 : Myriad2 only */
uint4	__builtin_shave_vau_imuls_u32_ri ( uint4, unsigned char );	/* VAU.IMULS.u32 : Myriad2 only */
short8	__builtin_shave_vau_imuls_i16_ri ( short8, unsigned char );	/* VAU.IMULS.i16 : Myriad2 only */
ushort8	__builtin_shave_vau_imuls_u16_ri ( ushort8, unsigned char );	/* VAU.IMULS.u16 : Myriad2 only */
char16	__builtin_shave_vau_imuls_i8_ri ( char16, unsigned char );	/* VAU.IMULS.i8 : Myriad2 only */
uchar16	__builtin_shave_vau_imuls_u8_ri ( uchar16, unsigned char );	/* VAU.IMULS.u8 : Myriad2 only */

/*  3.3.10 VAU.ADIFF */
/*      Computes the absolute difference of two vectors */
float4	__builtin_shave_vau_adiff_f32_rr ( float4, float4 );	/* VAU.ADIFF.f32 */
half8	__builtin_shave_vau_adiff_f16_rr ( half8, half8 );	/* VAU.ADIFF.f16 */
int4	__builtin_shave_vau_adiff_i32_rr ( int4, int4 );	/* VAU.ADIFF.i32 */
uint4	__builtin_shave_vau_adiff_u32_rr ( uint4, uint4 );	/* VAU.ADIFF.u32 */
short8	__builtin_shave_vau_adiff_i16_rr ( short8, short8 );	/* VAU.ADIFF.i16 */
ushort8	__builtin_shave_vau_adiff_u16_rr ( ushort8, ushort8 );	/* VAU.ADIFF.u16 */
char16	__builtin_shave_vau_adiff_i8_rr ( char16, char16 );	/* VAU.ADIFF.i8 */
uchar16	__builtin_shave_vau_adiff_u8_rr ( uchar16, uchar16 );	/* VAU.ADIFF.u8 */

/*  3.3.12 VAU.FRAC */
/*      Returns the fractional part of a floating point number */

/*  3.3.26 VAU.ILMULL */
/*      Vector multiplication. Use lower half of result */

/*  3.3.27 VAU.ILMULH */
/*      Vector integer multiplication. Use upper half of result */
int4	__builtin_shave_vau_ilmulh_i32_rr ( int4, int4 );	/* VAU.ILMULH.i32 */
short8	__builtin_shave_vau_ilmulh_i16_rr ( short8, short8 );	/* VAU.ILMULH.i16 */
ushort8	__builtin_shave_vau_ilmulh_u16_rr ( ushort8, ushort8 );	/* VAU.ILMULH.u16 */
char16	__builtin_shave_vau_ilmulh_i8_rr ( char16, char16 );	/* VAU.ILMULH.i8 */
uchar16	__builtin_shave_vau_ilmulh_u8_rr ( uchar16, uchar16 );	/* VAU.ILMULH.u8 */

/*  3.3.28 VAU.ADDCI */
/*      Vector integer addition with carry in */
int4	__builtin_shave_vau_addci_i32_rr ( int4, int4 );	/* VAU.ADDCI.i32 */

/*  3.3.29 VAU.SUBCI */
/*      Vector integer subtraction with carry in */
int4	__builtin_shave_vau_subci_i32_rr ( int4, int4 );	/* VAU.SUBCI.i32 */

/*  3.3.30 VAU.AVG */
/*      Integer average */

uint4	__builtin_shave_vau_avg_x32_rr ( uint4, uint4 );	/* VAU.AVG.x32 : Myriad2 only */
ushort8	__builtin_shave_vau_avg_x16_rr ( ushort8, ushort8 );	/* VAU.AVG.x16 : Myriad2 only */
uchar16	__builtin_shave_vau_avg_x8_rr ( uchar16, uchar16 );	/* VAU.AVG.x8 : Myriad2 only */

/*  3.3.31 VAU.CMB */
/*      Combine two vectors with word enables on the write */
/*      The 4 immediate operands are booleans and differ from the ISA document in the following ways: */
/*          the value 0 or 'false' implies 'D' */
/*          the value 1 or 'true' implies 'E' */
/*          element number increases from left to right */

int4	__builtin_shave_vau_cmbbyte_vrf_rriiii_int4 ( int4, int4, char, char, char, char );	/* VAU.CMBBYTE.i32 */
short8	__builtin_shave_vau_cmbbyte_vrf_rriiii_short8 ( short8, short8, char, char, char, char );	/* VAU.CMBBYTE.i16 */
char16	__builtin_shave_vau_cmbbyte_vrf_rriiii_char16 ( char16, char16, char, char, char, char );	/* VAU.CMBBYTE.i8 */
float4	__builtin_shave_vau_cmbword_vrf_rriiii_float4 ( float4, float4, char, char, char, char );	/* VAU.CMBWORD.f32 */
int4	__builtin_shave_vau_cmbword_vrf_rriiii_int4 ( int4, int4, char, char, char, char );	/* VAU.CMBWORD.i32 */
short8	__builtin_shave_vau_cmbword_vrf_rriiii_short8 ( short8, short8, char, char, char, char );	/* VAU.CMBWORD.i16 */
char16	__builtin_shave_vau_cmbword_vrf_rriiii_char16 ( char16, char16, char, char, char, char );	/* VAU.CMBWORD.i8 */

/*  3.3.32 VAU.SWZ */
/*      Combine vectors with input swizzle */
/*      The 4 immediate operands are enumerates and differ from the ISA document in the following ways: */
/*          these are values between 0 and 3, not the ASCII characters '0' .. '3' */
/*          element number increases from left to right */

/*  3.3.33 VAU.ALIGN */
/*      Align bytes within words */
int4	__builtin_shave_vau_alignbyte_vrf_rri_int4 ( int4, int4, char );	/* VAU.ALIGNBYTE.i32 */
short8	__builtin_shave_vau_alignbyte_vrf_rri_short8 ( short8, short8, char );	/* VAU.ALIGNBYTE.i16 */
char16	__builtin_shave_vau_alignbyte_vrf_rri_char16 ( char16, char16, char );	/* VAU.ALIGNBYTE.i8 */

/*  3.3.34 VAU.ALIGNVEC */
/*      Align vectors using byte offset */

/*  3.3.35 VAU.ROL */
/*      Vector rotate left */
int4	__builtin_shave_vau_rol_x32_rr ( int4, int4 );	/* VAU.ROL.x32 */
short8	__builtin_shave_vau_rol_x16_rr ( short8, short8 );	/* VAU.ROL.x16 */
char16	__builtin_shave_vau_rol_x8_rr ( char16, char16 );	/* VAU.ROL.x8 */

int4	__builtin_shave_vau_rol_x32_ri ( int4, unsigned char );	/* VAU.ROL.x32 */
short8	__builtin_shave_vau_rol_x16_ri ( short8, unsigned char );	/* VAU.ROL.x16 */
char16	__builtin_shave_vau_rol_x8_ri ( char16, unsigned char );	/* VAU.ROL.x8 */

/*  3.3.36 VAU.SHL */
/*      Vector integer shift left */

uint4	__builtin_shave_vau_shl_x32_rr ( uint4, uint4 );	/* VAU.SHL.x32 : Myriad2 only */
ushort8	__builtin_shave_vau_shl_x16_rr ( ushort8, ushort8 );	/* VAU.SHL.x16 : Myriad2 only */
uchar16	__builtin_shave_vau_shl_x8_rr ( uchar16, uchar16 );	/* VAU.SHL.x8 : Myriad2 only */


uint4	__builtin_shave_vau_shl_x32_ri ( uint4, unsigned char );	/* VAU.SHL.x32 : Myriad2 only */
ushort8	__builtin_shave_vau_shl_x16_ri ( ushort8, unsigned char );	/* VAU.SHL.x16 : Myriad2 only */
uchar16	__builtin_shave_vau_shl_x8_ri ( uchar16, unsigned char );	/* VAU.SHL.x8 : Myriad2 only */

/*  3.3.37 VAU.SHR */
/*      Vector integer shift right */
int4	__builtin_shave_vau_shr_i32_rr ( int4, int4 );	/* VAU.SHR.i32 */
uint4	__builtin_shave_vau_shr_u32_rr ( uint4, uint4 );	/* VAU.SHR.u32 */
short8	__builtin_shave_vau_shr_i16_rr ( short8, short8 );	/* VAU.SHR.i16 */
ushort8	__builtin_shave_vau_shr_u16_rr ( ushort8, ushort8 );	/* VAU.SHR.u16 */
char16	__builtin_shave_vau_shr_i8_rr ( char16, char16 );	/* VAU.SHR.i8 */
uchar16	__builtin_shave_vau_shr_u8_rr ( uchar16, uchar16 );	/* VAU.SHR.u8 */

int4	__builtin_shave_vau_shr_i32_ri ( int4, unsigned char );	/* VAU.SHR.i32 */
uint4	__builtin_shave_vau_shr_u32_ri ( uint4, unsigned char );	/* VAU.SHR.u32 */
short8	__builtin_shave_vau_shr_i16_ri ( short8, unsigned char );	/* VAU.SHR.i16 */
ushort8	__builtin_shave_vau_shr_u16_ri ( ushort8, unsigned char );	/* VAU.SHR.u16 */
char16	__builtin_shave_vau_shr_i8_ri ( char16, unsigned char );	/* VAU.SHR.i8 */
uchar16	__builtin_shave_vau_shr_u8_ri ( uchar16, unsigned char );	/* VAU.SHR.u8 */

/*  3.3.38 VAU.SHD */
/*      Vector integer shift right for signed arithmetic division */
int4	__builtin_shave_vau_shd_i32_rr ( int4, int4 );	/* VAU.SHD.i32 */
short8	__builtin_shave_vau_shd_i16_rr ( short8, short8 );	/* VAU.SHD.i16 */
char16	__builtin_shave_vau_shd_i8_rr ( char16, char16 );	/* VAU.SHD.i8 */

int4	__builtin_shave_vau_shd_i32_ri ( int4, unsigned char );	/* VAU.SHD.i32 */
short8	__builtin_shave_vau_shd_i16_ri ( short8, unsigned char );	/* VAU.SHD.i16 */
char16	__builtin_shave_vau_shd_i8_ri ( char16, unsigned char );	/* VAU.SHD.i8 */

/*  3.3.39 VAU.ONES */
/*      Vector integer ones count */

/*  3.3.39 VAU.ZEROS */
/*      Vector integer zeros count */

/*  3.3.40 VAU.ABS */
/*      Vector integer and floating point absolute value */
int4	__builtin_shave_vau_abs_i32_r ( int4 );	/* VAU.ABS.i32 */
short8	__builtin_shave_vau_abs_i16_r ( short8 );	/* VAU.ABS.i16 */
char16	__builtin_shave_vau_abs_i8_r ( char16 );	/* VAU.ABS.i8 */
float4	__builtin_shave_vau_abs_f32_r ( float4 );	/* VAU.ABS.f32 */
half8	__builtin_shave_vau_abs_f16_r ( half8 );	/* VAU.ABS.f16 */

/*  3.3.41 VAU.BSR */
/*      Integer bit scan reverse */

/*  3.3.42 VAU.BSF */
/*      Integer bit scan forward */

/*  3.3.43 VAU.SHFL */
/*      Bitwise "shuffle"\9D */

/*  3.3.44 VAU.DEAL */
/*      Bitwise de-interleave */

/*  3.3.45 VAU.REGX */
/*      Register Exchange */

/*  3.3.46 VAU.GMUL */
/*      Galois field multiplication */

/*  3.3.47 VAU.NOT */
/*      Bitwise NOT */
int4	__builtin_shave_vau_not_i32_r ( int4 );	/* VAU.NOT.i32 */
short8	__builtin_shave_vau_not_i16_r ( short8 );	/* VAU.NOT.i16 */
char16	__builtin_shave_vau_not_i8_r ( char16 );	/* VAU.NOT.i8 */

/*  3.3.48 VAU.AND */
/*      Bitwise AND */
int4	__builtin_shave_vau_and_32_rr ( int4, int4 );	/* VAU.AND.32 */
short8	__builtin_shave_vau_and_16_rr ( short8, short8 );	/* VAU.AND.16 */
char16	__builtin_shave_vau_and_8_rr ( char16, char16 );	/* VAU.AND.8 */

/*  3.3.49 VAU.OR */
/*      Bitwise OR */
int4	__builtin_shave_vau_or_32_rr ( int4, int4 );	/* VAU.OR.32 */
short8	__builtin_shave_vau_or_16_rr ( short8, short8 );	/* VAU.OR.16 */
char16	__builtin_shave_vau_or_8_rr ( char16, char16 );	/* VAU.OR.8 */

/*  3.3.50 VAU.XOR */
/*      Bitwise Exclusive-OR (XOR) */
int4	__builtin_shave_vau_xor_32_rr ( int4, int4 );	/* VAU.XOR.32 */
short8	__builtin_shave_vau_xor_16_rr ( short8, short8 );	/* VAU.XOR.16 */
char16	__builtin_shave_vau_xor_8_rr ( char16, char16 );	/* VAU.XOR.8 */

/*  3.3.51 VAU.XORT */
/*      Integer XOR reduce within SIMD word */

/*  3.3.52 VAU.RVRS */
/*      Bitwise reverse of 32 bit words */

/*  ========================================= */
/*  3.4 IAU Instructions */
/*  ========================================= */


/*  3.4.26 IAU.BSR */
/*      Bit scan reverse */
int	__builtin_shave_iau_bsr_32_r ( int );	/* IAU.BSR.32 */

/*  ========================================= */
/*  3.6 CMU Instructions */
/*  ========================================= */


/*  3.6.7 CMU.MAX */
/*      Maximum of two values */
float4	__builtin_shave_cmu_max_v4f32_rr ( float4, float4 );	/* CMU.MAX.v4f32 */
half8	__builtin_shave_cmu_max_v8f16_rr ( half8, half8 );	/* CMU.MAX.v8f16 */
int4	__builtin_shave_cmu_max_v4i32_rr ( int4, int4 );	/* CMU.MAX.v4i32 */
uint4	__builtin_shave_cmu_max_v4u32_rr ( uint4, uint4 );	/* CMU.MAX.v4u32 */
short8	__builtin_shave_cmu_max_v8i16_rr ( short8, short8 );	/* CMU.MAX.v8i16 */
ushort8	__builtin_shave_cmu_max_v8u16_rr ( ushort8, ushort8 );	/* CMU.MAX.v8u16 */
char16	__builtin_shave_cmu_max_v16i8_rr ( char16, char16 );	/* CMU.MAX.v16i8 */
uchar16	__builtin_shave_cmu_max_v16u8_rr ( uchar16, uchar16 );	/* CMU.MAX.v16u8 */
float	__builtin_shave_cmu_max_f32_rr ( float, float );	/* CMU.MAX.f32 */
half	__builtin_shave_cmu_max_f16_rr ( half, half );	/* CMU.MAX.f16 */
int	__builtin_shave_cmu_max_i32_rr ( int, int );	/* CMU.MAX.i32 */
unsigned int	__builtin_shave_cmu_max_u32_rr ( unsigned int, unsigned int );	/* CMU.MAX.u32 */
short	__builtin_shave_cmu_max_i16_rr ( short, short );	/* CMU.MAX.i16 */
unsigned short	__builtin_shave_cmu_max_u16_rr ( unsigned short, unsigned short );	/* CMU.MAX.u16 */
char	__builtin_shave_cmu_max_i8_rr ( char, char );	/* CMU.MAX.i8 */
unsigned char	__builtin_shave_cmu_max_u8_rr ( unsigned char, unsigned char );	/* CMU.MAX.u8 */

/*  3.6.8 CMU.MIN */
/*      Minimum of two values */
float4	__builtin_shave_cmu_min_v4f32_rr ( float4, float4 );	/* CMU.MIN.v4f32 */
half8	__builtin_shave_cmu_min_v8f16_rr ( half8, half8 );	/* CMU.MIN.v8f16 */
int4	__builtin_shave_cmu_min_v4i32_rr ( int4, int4 );	/* CMU.MIN.v4i32 */
uint4	__builtin_shave_cmu_min_v4u32_rr ( uint4, uint4 );	/* CMU.MIN.v4u32 */
short8	__builtin_shave_cmu_min_v8i16_rr ( short8, short8 );	/* CMU.MIN.v8i16 */
ushort8	__builtin_shave_cmu_min_v8u16_rr ( ushort8, ushort8 );	/* CMU.MIN.v8u16 */
char16	__builtin_shave_cmu_min_v16i8_rr ( char16, char16 );	/* CMU.MIN.v16i8 */
uchar16	__builtin_shave_cmu_min_v16u8_rr ( uchar16, uchar16 );	/* CMU.MIN.v16u8 */
float	__builtin_shave_cmu_min_f32_rr ( float, float );	/* CMU.MIN.f32 */
half	__builtin_shave_cmu_min_f16_rr ( half, half );	/* CMU.MIN.f16 */
int	__builtin_shave_cmu_min_i32_rr ( int, int );	/* CMU.MIN.i32 */
unsigned int	__builtin_shave_cmu_min_u32_rr ( unsigned int, unsigned int );	/* CMU.MIN.u32 */
short	__builtin_shave_cmu_min_i16_rr ( short, short );	/* CMU.MIN.i16 */
unsigned short	__builtin_shave_cmu_min_u16_rr ( unsigned short, unsigned short );	/* CMU.MIN.u16 */
char	__builtin_shave_cmu_min_i8_rr ( char, char );	/* CMU.MIN.i8 */
unsigned char	__builtin_shave_cmu_min_u8_rr ( unsigned char, unsigned char );	/* CMU.MIN.u8 */

/*  3.6.9 CMU.CLAMPAB */
/*      Clamp to between specified values */
float4	__builtin_shave_cmu_clampab_v4f32_rrr ( float4, float4, float4 );	/* CMU.CLAMPAB.v4f32 */
half8	__builtin_shave_cmu_clampab_v8f16_rrr ( half8, half8, half8 );	/* CMU.CLAMPAB.v8f16 */
int4	__builtin_shave_cmu_clampab_v4i32_rrr ( int4, int4, int4 );	/* CMU.CLAMPAB.v4i32 */
uint4	__builtin_shave_cmu_clampab_v4u32_rrr ( uint4, uint4, uint4 );	/* CMU.CLAMPAB.v4u32 */
short8	__builtin_shave_cmu_clampab_v8i16_rrr ( short8, short8, short8 );	/* CMU.CLAMPAB.v8i16 */
ushort8	__builtin_shave_cmu_clampab_v8u16_rrr ( ushort8, ushort8, ushort8 );	/* CMU.CLAMPAB.v8u16 */
char16	__builtin_shave_cmu_clampab_v16i8_rrr ( char16, char16, char16 );	/* CMU.CLAMPAB.v16i8 */
uchar16	__builtin_shave_cmu_clampab_v16u8_rrr ( uchar16, uchar16, uchar16 );	/* CMU.CLAMPAB.v16u8 */
float	__builtin_shave_cmu_clampab_f32_rrr ( float, float, float );	/* CMU.CLAMPAB.f32 : Myriad2 only */
half	__builtin_shave_cmu_clampab_f16_rrr ( half, half, half );	/* CMU.CLAMPAB.f16 : Myriad2 only */
int	__builtin_shave_cmu_clampab_i32_rrr ( int, int, int );	/* CMU.CLAMPAB.i32 */
unsigned int	__builtin_shave_cmu_clampab_u32_rrr ( unsigned int, unsigned int, unsigned int );	/* CMU.CLAMPAB.u32 */
short	__builtin_shave_cmu_clampab_i16_rrr ( short, short, short );	/* CMU.CLAMPAB.i16 */
unsigned short	__builtin_shave_cmu_clampab_u16_rrr ( unsigned short, unsigned short, unsigned short );	/* CMU.CLAMPAB.u16 */
char	__builtin_shave_cmu_clampab_i8_rrr ( char, char, char );	/* CMU.CLAMPAB.i8 */
unsigned char	__builtin_shave_cmu_clampab_u8_rrr ( unsigned char, unsigned char, unsigned char );	/* CMU.CLAMPAB.u8 */

/*  3.6.10 CMU.CLAMP */
/*      Clamp to between zero and a positive value */
float4	__builtin_shave_cmu_clamp_v4f32_rr ( float4, float4 );	/* CMU.CLAMP.v4f32 */
half8	__builtin_shave_cmu_clamp_v8f16_rr ( half8, half8 );	/* CMU.CLAMP.v8f16 */
int4	__builtin_shave_cmu_clamp_v4i32_rr ( int4, int4 );	/* CMU.CLAMP.v4i32 */
short8	__builtin_shave_cmu_clamp_v8i16_rr ( short8, short8 );	/* CMU.CLAMP.v8i16 */
char16	__builtin_shave_cmu_clamp_v16i8_rr ( char16, char16 );	/* CMU.CLAMP.v16i8 */
float	__builtin_shave_cmu_clamp_f32_rr ( float, float );	/* CMU.CLAMP.f32 */
half	__builtin_shave_cmu_clamp_f16_rr ( half, half );	/* CMU.CLAMP.f16 */
int	__builtin_shave_cmu_clamp_i32_rr ( int, int );	/* CMU.CLAMP.i32 */
short	__builtin_shave_cmu_clamp_i16_rr ( short, short );	/* CMU.CLAMP.i16 */
char	__builtin_shave_cmu_clamp_i8_rr ( char, char );	/* CMU.CLAMP.i8 */

float4	__builtin_shave_cmu_clamp0_v4f32_rr ( float4, float4 );	/* CMU.CLAMP0.v4f32 */
half8	__builtin_shave_cmu_clamp0_v8f16_rr ( half8, half8 );	/* CMU.CLAMP0.v8f16 */
int4	__builtin_shave_cmu_clamp0_v4i32_rr ( int4, int4 );	/* CMU.CLAMP0.v4i32 */
short8	__builtin_shave_cmu_clamp0_v8i16_rr ( short8, short8 );	/* CMU.CLAMP0.v8i16 */
char16	__builtin_shave_cmu_clamp0_v16i8_rr ( char16, char16 );	/* CMU.CLAMP0.v16i8 */
float	__builtin_shave_cmu_clamp0_f32_rr ( float, float );	/* CMU.CLAMP0.f32 */
half	__builtin_shave_cmu_clamp0_f16_rr ( half, half );	/* CMU.CLAMP0.f16 */
int	__builtin_shave_cmu_clamp0_i32_rr ( int, int );	/* CMU.CLAMP0.i32 */
short	__builtin_shave_cmu_clamp0_i16_rr ( short, short );	/* CMU.CLAMP0.i16 */
char	__builtin_shave_cmu_clamp0_i8_rr ( char, char );	/* CMU.CLAMP0.i8 */

/*  3.6.23 CMU.SHLIV */
/*      Shift integer value left into VRF */
int4	__builtin_shave_cmu_shliv_x32_rr ( int4, int );	/* CMU.SHLIV.x32 */
short8	__builtin_shave_cmu_shliv_x16_rr ( short8, short );	/* CMU.SHLIV.x16 */
char16	__builtin_shave_cmu_shliv_x8_rr ( char16, char );	/* CMU.SHLIV.x8 */

/*  3.6.24 CMU.SHLSV */
/*      Shift scalar value left into VRF */

/*  3.6.35 CMU.VSZM */
/*      Vector swizzle with write disable and optional zeroing */
/*      The 4 immediate operands are enumerates and differ from the ISA document in the following ways: */
/*          the value 0 implies 'D' */
/*          the value 1 implies 'E' */
/*          the value 4 implies '0' */
/*          the value 5 implies '1' */
/*          the value 6 implies '2' */
/*          the value 7 implies '3' */
/*          element number increases from left to right */
int4	__builtin_shave_cmu_vszmword_vrf_riiii_int4 ( int4, char, char, char, char );	/* CMU.VSZMWORD.v4i32 */
float4	__builtin_shave_cmu_vszmword_vrf_riiii_float4 ( float4, char, char, char, char );	/* CMU.VSZMWORD.v4f32 */
short8	__builtin_shave_cmu_vszmword_vrf_riiii_short8 ( short8, char, char, char, char );	/* CMU.VSZMWORD.v8i16 */
char16	__builtin_shave_cmu_vszmword_vrf_riiii_char16 ( char16, char, char, char, char );	/* CMU.VSZMWORD.v16i8 */
int4	__builtin_shave_cmu_vszmbyte_vrf_riiii_int4 ( int4, char, char, char, char );	/* CMU.VSZMBYTE.v4i32 */
float4	__builtin_shave_cmu_vszmbyte_vrf_riiii_float4 ( float4, char, char, char, char );	/* CMU.VSZMBYTE.v4f32 */
short8	__builtin_shave_cmu_vszmbyte_vrf_riiii_short8 ( short8, char, char, char, char );	/* CMU.VSZMBYTE.v8i16 */
char16	__builtin_shave_cmu_vszmbyte_vrf_riiii_char16 ( char16, char, char, char, char );	/* CMU.VSZMBYTE.v16i8 */

/*  3.6.36 CMU.CMASK */
/*      Create mask */
int4	__builtin_shave_cmu_cmask_v4i32_r ( int );	/* CMU.CMASK.v4i32 */

/*  4.4.1 CMU.ALIGNVEC */
float4	__builtin_shave_cmu_alignvec_vrf_rri_float4 ( float4, float4, char );	/* CMU.ALIGNVEC.f32 : Myriad2 only */
int4	__builtin_shave_cmu_alignvec_vrf_rri_int4 ( int4, int4, char );	/* CMU.ALIGNVEC.i32 : Myriad2 only */
short8	__builtin_shave_cmu_alignvec_vrf_rri_short8 ( short8, short8, char );	/* CMU.ALIGNVEC.i16 : Myriad2 only */
char16	__builtin_shave_cmu_alignvec_vrf_rri_char16 ( char16, char16, char );	/* CMU.ALIGNVEC.i8 : Myriad2 only */

#ifndef _MSC_VER
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

/*mvuConvert template*/
//convertor_type
template <class T1, class T2, class T3>
T1 convertor_type(T2 data, T3 aux)
{
	T1 x;
	int i;
	int size = sizeof(x)/sizeof(aux);
	for(i=0; i<size;i++)
		x[i] = (T3)data[i];
	return x;
		
}


#endif /* BUILTINFUNCTIONS_H_ */

