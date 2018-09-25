/*
 *  Auto-generated for 'moviCompile'
 *
 *  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   shave-builtin-pseudo-declarations.h
 *  Description:   This header contains pseudo function prototypes for all SHAVE
 *                 ISA builtins, it is provided for informative reasons for SHAVE
 *                 programmers and does not need to be included by an actual C
 *                 source file.
 *  ---------------------------------------------------------------------------
 *
 *  Each builtin has an exact correspondence to an instruction in the SHAVE ISA.
 *
 *  Naming convention:
 *        __builtin_shave_<functional-unit>_<instruction>_<instruction-type>_<operand-types>
 *
 *      For example: __builtin_shave_vau_add_i32s_ri
 *        This is the builtin for VAU.ADD.i32s <vrf:Dst> <vrf:srcA> <immu5:B>
 *  ---------------------------------------------------------------------------
 */

#ifndef __SHAVE_BUILTIN_PSEUDO_DECLARATIONS__
#define __SHAVE_BUILTIN_PSEUDO_DECLARATIONS__ (1)

#ifndef __shave__

#include <moviVectorUtils.h>

_MV_EXTERNC_BEGIN



/*  ========================================= */
/*  1.6.3 BRU Instructions */
/*  ========================================= */

/*  1.6.3.10 BRU.SWIH */
/*      Signal software interrupt and halt */
/*      The immediate operand has a value between 0 and 31 */
__attribute__((noreturn))	void	__builtin_shave_bru_swih_i ( unsigned char immediate_1 );	/* BRU.SWIH */


/*  ========================================= */
/*  1.6.4 CMU Instructions */
/*  ========================================= */

/*  1.6.4.1 CMU.ALIGNVEC */
/*      Long vector align with byte offset */
float4	__builtin_shave_cmu_alignvec_rri_float4 ( float4 variable_1, float4 variable_2, signed char immediate_1 );	/* CMU.ALIGNVEC */
int4	__builtin_shave_cmu_alignvec_rri_int4 ( int4 variable_1, int4 variable_2, signed char immediate_1 );	/* CMU.ALIGNVEC */
short8	__builtin_shave_cmu_alignvec_rri_short8 ( short8 variable_1, short8 variable_2, signed char immediate_1 );	/* CMU.ALIGNVEC */
schar16	__builtin_shave_cmu_alignvec_rri_char16 ( schar16 variable_1, schar16 variable_2, signed char immediate_1 );	/* CMU.ALIGNVEC */

/*  1.6.4.2 CMU.CLAMP */
/*      Clamp between zero and a positive value or between +/- a specified value */
float4	__builtin_shave_cmu_clamp_f32_rr_float4 ( float4 variable_1, float4 variable_2 );	/* CMU.CLAMP.f32 */
half8	__builtin_shave_cmu_clamp_f16_rr_half8 ( half8 variable_1, half8 variable_2 );	/* CMU.CLAMP.f16 */
int4	__builtin_shave_cmu_clamp_i32_rr_int4 ( int4 variable_1, int4 variable_2 );	/* CMU.CLAMP.i32 */
short8	__builtin_shave_cmu_clamp_i16_rr_short8 ( short8 variable_1, short8 variable_2 );	/* CMU.CLAMP.i16 */
schar16	__builtin_shave_cmu_clamp_i8_rr_char16 ( schar16 variable_1, schar16 variable_2 );	/* CMU.CLAMP.i8 */
float	__builtin_shave_cmu_clamp_f32_rr_float ( float variable_1, float variable_2 );	/* CMU.CLAMP.f32 */
half	__builtin_shave_cmu_clamp_f16_rr_half ( half variable_1, half variable_2 );	/* CMU.CLAMP.f16 */
int	__builtin_shave_cmu_clamp_i32_rr_int ( int variable_1, int variable_2 );	/* CMU.CLAMP.i32 */
short	__builtin_shave_cmu_clamp_i16_rr_short ( short variable_1, short variable_2 );	/* CMU.CLAMP.i16 */
signed char	__builtin_shave_cmu_clamp_i8_rr_char ( signed char variable_1, signed char variable_2 );	/* CMU.CLAMP.i8 */

float4	__builtin_shave_cmu_clamp0_f32_rr_float4 ( float4 variable_1, float4 variable_2 );	/* CMU.CLAMP0.f32 */
half8	__builtin_shave_cmu_clamp0_f16_rr_half8 ( half8 variable_1, half8 variable_2 );	/* CMU.CLAMP0.f16 */
int4	__builtin_shave_cmu_clamp0_i32_rr_int4 ( int4 variable_1, int4 variable_2 );	/* CMU.CLAMP0.i32 */
short8	__builtin_shave_cmu_clamp0_i16_rr_short8 ( short8 variable_1, short8 variable_2 );	/* CMU.CLAMP0.i16 */
schar16	__builtin_shave_cmu_clamp0_i8_rr_char16 ( schar16 variable_1, schar16 variable_2 );	/* CMU.CLAMP0.i8 */
float	__builtin_shave_cmu_clamp0_f32_rr_float ( float variable_1, float variable_2 );	/* CMU.CLAMP0.f32 */
half	__builtin_shave_cmu_clamp0_f16_rr_half ( half variable_1, half variable_2 );	/* CMU.CLAMP0.f16 */
int	__builtin_shave_cmu_clamp0_i32_rr_int ( int variable_1, int variable_2 );	/* CMU.CLAMP0.i32 */
short	__builtin_shave_cmu_clamp0_i16_rr_short ( short variable_1, short variable_2 );	/* CMU.CLAMP0.i16 */
signed char	__builtin_shave_cmu_clamp0_i8_rr_char ( signed char variable_1, signed char variable_2 );	/* CMU.CLAMP0.i8 */

/*  1.6.4.3 CMU.CLAMPAB */
/*      Clamp within range */
float4	__builtin_shave_cmu_clampab_f32_rrr_float4 ( float4 variable_1, float4 variable_2, float4 variable_3 );	/* CMU.CLAMPAB.f32 */
half8	__builtin_shave_cmu_clampab_f16_rrr_half8 ( half8 variable_1, half8 variable_2, half8 variable_3 );	/* CMU.CLAMPAB.f16 */
int4	__builtin_shave_cmu_clampab_i32_rrr_int4 ( int4 variable_1, int4 variable_2, int4 variable_3 );	/* CMU.CLAMPAB.i32 */
uint4	__builtin_shave_cmu_clampab_u32_rrr_uint4 ( uint4 variable_1, uint4 variable_2, uint4 variable_3 );	/* CMU.CLAMPAB.u32 */
short8	__builtin_shave_cmu_clampab_i16_rrr_short8 ( short8 variable_1, short8 variable_2, short8 variable_3 );	/* CMU.CLAMPAB.i16 */
ushort8	__builtin_shave_cmu_clampab_u16_rrr_ushort8 ( ushort8 variable_1, ushort8 variable_2, ushort8 variable_3 );	/* CMU.CLAMPAB.u16 */
schar16	__builtin_shave_cmu_clampab_i8_rrr_char16 ( schar16 variable_1, schar16 variable_2, schar16 variable_3 );	/* CMU.CLAMPAB.i8 */
uchar16	__builtin_shave_cmu_clampab_u8_rrr_uchar16 ( uchar16 variable_1, uchar16 variable_2, uchar16 variable_3 );	/* CMU.CLAMPAB.u8 */
float	__builtin_shave_cmu_clampab_f32_rrr_float ( float variable_1, float variable_2, float variable_3 );	/* CMU.CLAMPAB.f32 */
half	__builtin_shave_cmu_clampab_f16_rrr_half ( half variable_1, half variable_2, half variable_3 );	/* CMU.CLAMPAB.f16 */
int	__builtin_shave_cmu_clampab_i32_rrr_int ( int variable_1, int variable_2, int variable_3 );	/* CMU.CLAMPAB.i32 */
unsigned int	__builtin_shave_cmu_clampab_u32_rrr_uint ( unsigned int variable_1, unsigned int variable_2, unsigned int variable_3 );	/* CMU.CLAMPAB.u32 */
short	__builtin_shave_cmu_clampab_i16_rrr_short ( short variable_1, short variable_2, short variable_3 );	/* CMU.CLAMPAB.i16 */
unsigned short	__builtin_shave_cmu_clampab_u16_rrr_ushort ( unsigned short variable_1, unsigned short variable_2, unsigned short variable_3 );	/* CMU.CLAMPAB.u16 */
signed char	__builtin_shave_cmu_clampab_i8_rrr_char ( signed char variable_1, signed char variable_2, signed char variable_3 );	/* CMU.CLAMPAB.i8 */
unsigned char	__builtin_shave_cmu_clampab_u8_rrr_uchar ( unsigned char variable_1, unsigned char variable_2, unsigned char variable_3 );	/* CMU.CLAMPAB.u8 */

/*  1.6.4.4 CMU.CMASK */
/*      Generate VRF mask from IRF contents */
int4	__builtin_shave_cmu_cmask_r_int4 ( int variable_1 );	/* CMU.CMASK */
short8	__builtin_shave_cmu_cmask_r_short8 ( int variable_1 );	/* CMU.CMASK */
schar16	__builtin_shave_cmu_cmask_r_char16 ( int variable_1 );	/* CMU.CMASK */

/*  1.6.4.9 CMU.CPII */
/*      IRF copy with optional conversion. */
half	__builtin_shave_cmu_cpii_u8f_f16_r ( unsigned char variable_1 );	/* CMU.CPII.u8f.f16 */
unsigned char	__builtin_shave_cmu_cpii_f16_u8fs_r ( half variable_1 );	/* CMU.CPII.f16.u8fs */

/*  1.6.4.17 CMU.CPVV */
/*      VRF copy with optional conversion. */
uchar8	__builtin_shave_cmu_cpvv_f16_u8fs_r ( half8 variable_1 );	/* CMU.CPVV.f16.u8fs */
half8	__builtin_shave_cmu_cpvv_u8f_f16_r ( uchar8 variable_1 );	/* CMU.CPVV.u8f.f16 */
uchar8	__builtin_shave_cmu_cpvv_u16_u8s_r ( ushort8 variable_1 );	/* CMU.CPVV.u16.u8s */

/*  1.6.4.22 CMU.MAX */
/*      Maximum */
float4	__builtin_shave_cmu_max_f32_rr_float4 ( float4 variable_1, float4 variable_2 );	/* CMU.MAX.f32 */
half8	__builtin_shave_cmu_max_f16_rr_half8 ( half8 variable_1, half8 variable_2 );	/* CMU.MAX.f16 */
int4	__builtin_shave_cmu_max_i32_rr_int4 ( int4 variable_1, int4 variable_2 );	/* CMU.MAX.i32 */
uint4	__builtin_shave_cmu_max_u32_rr_uint4 ( uint4 variable_1, uint4 variable_2 );	/* CMU.MAX.u32 */
short8	__builtin_shave_cmu_max_i16_rr_short8 ( short8 variable_1, short8 variable_2 );	/* CMU.MAX.i16 */
ushort8	__builtin_shave_cmu_max_u16_rr_ushort8 ( ushort8 variable_1, ushort8 variable_2 );	/* CMU.MAX.u16 */
schar16	__builtin_shave_cmu_max_i8_rr_char16 ( schar16 variable_1, schar16 variable_2 );	/* CMU.MAX.i8 */
uchar16	__builtin_shave_cmu_max_u8_rr_uchar16 ( uchar16 variable_1, uchar16 variable_2 );	/* CMU.MAX.u8 */
float	__builtin_shave_cmu_max_f32_rr_float ( float variable_1, float variable_2 );	/* CMU.MAX.f32 */
half	__builtin_shave_cmu_max_f16_rr_half ( half variable_1, half variable_2 );	/* CMU.MAX.f16 */
int	__builtin_shave_cmu_max_i32_rr_int ( int variable_1, int variable_2 );	/* CMU.MAX.i32 */
unsigned int	__builtin_shave_cmu_max_u32_rr_uint ( unsigned int variable_1, unsigned int variable_2 );	/* CMU.MAX.u32 */
short	__builtin_shave_cmu_max_i16_rr_short ( short variable_1, short variable_2 );	/* CMU.MAX.i16 */
unsigned short	__builtin_shave_cmu_max_u16_rr_ushort ( unsigned short variable_1, unsigned short variable_2 );	/* CMU.MAX.u16 */
signed char	__builtin_shave_cmu_max_i8_rr_char ( signed char variable_1, signed char variable_2 );	/* CMU.MAX.i8 */
unsigned char	__builtin_shave_cmu_max_u8_rr_uchar ( unsigned char variable_1, unsigned char variable_2 );	/* CMU.MAX.u8 */

/*  1.6.4.23 CMU.MIN */
/*      Minimum */
float4	__builtin_shave_cmu_min_f32_rr_float4 ( float4 variable_1, float4 variable_2 );	/* CMU.MIN.f32 */
half8	__builtin_shave_cmu_min_f16_rr_half8 ( half8 variable_1, half8 variable_2 );	/* CMU.MIN.f16 */
int4	__builtin_shave_cmu_min_i32_rr_int4 ( int4 variable_1, int4 variable_2 );	/* CMU.MIN.i32 */
uint4	__builtin_shave_cmu_min_u32_rr_uint4 ( uint4 variable_1, uint4 variable_2 );	/* CMU.MIN.u32 */
short8	__builtin_shave_cmu_min_i16_rr_short8 ( short8 variable_1, short8 variable_2 );	/* CMU.MIN.i16 */
ushort8	__builtin_shave_cmu_min_u16_rr_ushort8 ( ushort8 variable_1, ushort8 variable_2 );	/* CMU.MIN.u16 */
schar16	__builtin_shave_cmu_min_i8_rr_char16 ( schar16 variable_1, schar16 variable_2 );	/* CMU.MIN.i8 */
uchar16	__builtin_shave_cmu_min_u8_rr_uchar16 ( uchar16 variable_1, uchar16 variable_2 );	/* CMU.MIN.u8 */
float	__builtin_shave_cmu_min_f32_rr_float ( float variable_1, float variable_2 );	/* CMU.MIN.f32 */
half	__builtin_shave_cmu_min_f16_rr_half ( half variable_1, half variable_2 );	/* CMU.MIN.f16 */
int	__builtin_shave_cmu_min_i32_rr_int ( int variable_1, int variable_2 );	/* CMU.MIN.i32 */
unsigned int	__builtin_shave_cmu_min_u32_rr_uint ( unsigned int variable_1, unsigned int variable_2 );	/* CMU.MIN.u32 */
short	__builtin_shave_cmu_min_i16_rr_short ( short variable_1, short variable_2 );	/* CMU.MIN.i16 */
unsigned short	__builtin_shave_cmu_min_u16_rr_ushort ( unsigned short variable_1, unsigned short variable_2 );	/* CMU.MIN.u16 */
signed char	__builtin_shave_cmu_min_i8_rr_char ( signed char variable_1, signed char variable_2 );	/* CMU.MIN.i8 */
unsigned char	__builtin_shave_cmu_min_u8_rr_uchar ( unsigned char variable_1, unsigned char variable_2 );	/* CMU.MIN.u8 */

/*  1.6.4.24 CMU.SHIV */
/*      Shift IRF left or right into VRF */
int4	__builtin_shave_cmu_shliv_x32_rr ( int4 variable_1, int variable_2 );	/* CMU.SHLIV.x32 */
short8	__builtin_shave_cmu_shliv_x16_rr ( short8 variable_1, short variable_2 );	/* CMU.SHLIV.x16 */
schar16	__builtin_shave_cmu_shliv_x8_rr ( schar16 variable_1, signed char variable_2 );	/* CMU.SHLIV.x8 */
int4	__builtin_shave_cmu_shriv_x32_rr ( int4 variable_1, int variable_2 );	/* CMU.SHRIV.x32 */
short8	__builtin_shave_cmu_shriv_x16_rr ( short8 variable_1, short variable_2 );	/* CMU.SHRIV.x16 */
schar16	__builtin_shave_cmu_shriv_x8_rr ( schar16 variable_1, signed char variable_2 );	/* CMU.SHRIV.x8 */

/*  1.6.4.27 CMU.VNZ */
/*      Generate IRF mask from VRF contents */
uchar4	__builtin_shave_cmu_vnz_x32_rri_int4 ( int4 variable_1, uchar4 variable_2, unsigned char immediate_1 );	/* CMU.VNZ.x32 */
uchar4	__builtin_shave_cmu_vnz_x32_rri_uint4 ( uint4 variable_1, uchar4 variable_2, unsigned char immediate_1 );	/* CMU.VNZ.x32 */
uchar4	__builtin_shave_cmu_vnz_x16_rri_short8 ( short8 variable_1, uchar4 variable_2, unsigned char immediate_1 );	/* CMU.VNZ.x16 */
uchar4	__builtin_shave_cmu_vnz_x16_rri_ushort8 ( ushort8 variable_1, uchar4 variable_2, unsigned char immediate_1 );	/* CMU.VNZ.x16 */
ushort2	__builtin_shave_cmu_vnz_x8_rri_char16 ( schar16 variable_1, ushort2 variable_2, unsigned char immediate_1 );	/* CMU.VNZ.x8 */
ushort2	__builtin_shave_cmu_vnz_x8_rri_uchar16 ( uchar16 variable_1, ushort2 variable_2, unsigned char immediate_1 );	/* CMU.VNZ.x8 */

/*  1.6.4.29 CMU.VSZM */
/*      Byte/Word swizzle with optional write disable and optional zeroing */
/*      The 4 immediate operands are enumerates and differ from the ISA document in the following ways: */
/*          the value 0 implies 'Z' meaning Zero */
/*          the value 1 implies 'D' meaning Disable */
/*          the value 4 implies '0' meaning Select Lane #0 */
/*          the value 5 implies '1' meaning Select Lane #1 */
/*          the value 6 implies '2' meaning Select Lane #2 */
/*          the value 7 implies '3' meaning Select Lane #3 */
/*          element number increases from left to right */
int4	__builtin_shave_cmu_vszmword_riiii_int4 ( int4 variable_1, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* CMU.VSZMWORD */
float4	__builtin_shave_cmu_vszmword_riiii_float4 ( float4 variable_1, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* CMU.VSZMWORD */
short8	__builtin_shave_cmu_vszmword_riiii_short8 ( short8 variable_1, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* CMU.VSZMWORD */
schar16	__builtin_shave_cmu_vszmword_riiii_char16 ( schar16 variable_1, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* CMU.VSZMWORD */
int4	__builtin_shave_cmu_vszmbyte_riiii_int4 ( int4 variable_1, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* CMU.VSZMBYTE */
short8	__builtin_shave_cmu_vszmbyte_riiii_short8 ( short8 variable_1, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* CMU.VSZMBYTE */
schar16	__builtin_shave_cmu_vszmbyte_riiii_char16 ( schar16 variable_1, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* CMU.VSZMBYTE */


/*  ========================================= */
/*  1.6.5 IAU Instructions */
/*  ========================================= */

/*  1.6.5.4 IAU.ADDSI */
/*      32-bit signed integer addition with saturation */
int	__builtin_shave_iau_addsi_rr ( int variable_1, int variable_2 );	/* IAU.ADDSI */
int	__builtin_shave_iau_addsi_ri ( int variable_1, unsigned char immediate_1 );	/* IAU.ADDSI */

/*  1.6.5.5 IAU.ADDSU */
/*      32-bit unsigned integer addition with saturation */
unsigned int	__builtin_shave_iau_addsu_rr ( unsigned int variable_1, unsigned int variable_2 );	/* IAU.ADDSU */
unsigned int	__builtin_shave_iau_addsu_ri ( unsigned int variable_1, unsigned char immediate_1 );	/* IAU.ADDSU */

/*  1.6.5.6 IAU.ALIGN */
/*      Align to byte boundary */
int	__builtin_shave_iau_align_rri ( int variable_1, int variable_2, unsigned char immediate_1 );	/* IAU.ALIGN */

/*  1.6.5.7 IAU.AND */
/*      Bitwise AND */
unsigned int	__builtin_shave_iau_and_rr ( unsigned int variable_1, unsigned int variable_2 );	/* IAU.AND */

/*  1.6.5.9 IAU.BSF */
/*      Find bit position of least significant bit set to 1/0 */
unsigned int	__builtin_shave_iau_bsf_32_r ( unsigned int variable_1 );	/* IAU.BSF.32 */
unsigned int	__builtin_shave_iau_bsfinv_32_r ( unsigned int variable_1 );	/* IAU.BSFINV.32 */

/*  1.6.5.10 IAU.BSR */
/*      Find bit position of most significant bit set to 1/0 */
unsigned int	__builtin_shave_iau_bsr_32_r ( unsigned int variable_1 );	/* IAU.BSR.32 */
unsigned int	__builtin_shave_iau_bsrinv_32_r ( unsigned int variable_1 );	/* IAU.BSRINV.32 */

/*  1.6.5.13 IAU.FEXTI */
/*      Signed integer field extract with immediate specifiers */
int	__builtin_shave_iau_fexti_rii ( int variable_1, unsigned char immediate_1, unsigned char immediate_2 );	/* IAU.FEXTI */

/*  1.6.5.14 IAU.FEXTJI */
/*      Signed integer field extract with indirect specifiers */
int	__builtin_shave_iau_fextji_rr ( int variable_1, unsigned int variable_2 );	/* IAU.FEXTJI */

/*  1.6.5.15 IAU.FEXTJU */
/*      Unsigned integer field extract with indirect specifiers */
unsigned int	__builtin_shave_iau_fextju_rr ( unsigned int variable_1, unsigned int variable_2 );	/* IAU.FEXTJU */

/*  1.6.5.16 IAU.FEXTU */
/*      Unsigned integer field extract with immediate specifiers */
unsigned int	__builtin_shave_iau_fextu_rii ( unsigned int variable_1, unsigned char immediate_1, unsigned char immediate_2 );	/* IAU.FEXTU */

/*  1.6.5.17 IAU.FINS */
/*      Field insert with immediate specifiers */
unsigned int	__builtin_shave_iau_fins_rrii ( unsigned int variable_1, unsigned int variable_2, unsigned char immediate_1, unsigned char immediate_2 );	/* IAU.FINS */

/*  1.6.5.18 IAU.FINSJ */
/*      Field insert with indirect specifiers */
unsigned int	__builtin_shave_iau_finsj_rrr ( unsigned int variable_1, unsigned int variable_2, unsigned int variable_3 );	/* IAU.FINSJ */

/*  1.6.5.27 IAU.MULSI */
/*      32-bit signed integer multiplication with saturation */
int	__builtin_shave_iau_mulsi_rr ( int variable_1, int variable_2 );	/* IAU.MULSI */
int	__builtin_shave_iau_mulsi_ri ( int variable_1, signed char immediate_1 );	/* IAU.MULSI */

/*  1.6.5.28 IAU.MULSU */
/*      32-bit unsigned integer multiplication with saturation */
unsigned int	__builtin_shave_iau_mulsu_rr ( unsigned int variable_1, unsigned int variable_2 );	/* IAU.MULSU */
unsigned int	__builtin_shave_iau_mulsu_ri ( unsigned int variable_1, unsigned char immediate_1 );	/* IAU.MULSU */

/*  1.6.5.30 IAU.ONES */
/*      Count number of bits set to 1/0 */
int	__builtin_shave_iau_ones_r ( int variable_1 );	/* IAU.ONES */
int	__builtin_shave_iau_zeros_r ( int variable_1 );	/* IAU.ZEROS */

/*  1.6.5.33 IAU.RVRS */
/*      Reverse bit order */
int	__builtin_shave_iau_rvrs_r ( int variable_1 );	/* IAU.RVRS */

/*  1.6.5.38 IAU.SUBSI */
/*      32-bit signed integer subtraction with saturation */
int	__builtin_shave_iau_subsi_rr ( int variable_1, int variable_2 );	/* IAU.SUBSI */
int	__builtin_shave_iau_subsi_ri ( int variable_1, unsigned char immediate_1 );	/* IAU.SUBSI */

/*  1.6.5.39 IAU.SUBSU */
/*      32-bit unsigned integer subtraction with saturation */
unsigned int	__builtin_shave_iau_subsu_rr ( unsigned int variable_1, unsigned int variable_2 );	/* IAU.SUBSU */
unsigned int	__builtin_shave_iau_subsu_ri ( unsigned int variable_1, unsigned char immediate_1 );	/* IAU.SUBSU */


/*  ========================================= */
/*  1.6.6 LSU Instructions */
/*  ========================================= */

/*  1.6.6.2 LSU.LD */
/*      Register indirect load of specified type from memory. */
half8	__builtin_shave_lsu_ld128_u8_f16_r ( const uchar8 * pointer_1 );	/* LSU.LD128.u8.f16 */
half8	__builtin_shave_lsu_ld128_u8f_f16_r ( const uchar8 * pointer_1 );	/* LSU.LD128.u8f.f16 */
uint4	__builtin_shave_lsu_ld128_rgb565_r ( const unsigned short * pointer_1 );	/* LSU.LD128.rgb565  : Myriad2.1 [ma2100], Myriad2.2 [ma2x5x] CPUs only */
uint4	__builtin_shave_lsu_ld128_rgb1555_r ( const unsigned short * pointer_1 );	/* LSU.LD128.rgb1555  : Myriad2.1 [ma2100], Myriad2.2 [ma2x5x] CPUs only */
uint4	__builtin_shave_lsu_ld128_rgb4444_r ( const unsigned short * pointer_1 );	/* LSU.LD128.rgb4444  : Myriad2.1 [ma2100], Myriad2.2 [ma2x5x] CPUs only */

/*      Register load - these are "exclusive" loads that do not permit a simultaneous load on the other LSU in the same cycle */
unsigned long long	__builtin_shave_lsu_ld64_r_exclusive ( const volatile unsigned long long * pointer_1 );	/* LSU.LD64 */
unsigned int	__builtin_shave_lsu_ld32_r_exclusive ( const volatile unsigned int * pointer_1 );	/* LSU.LD32 */
unsigned short	__builtin_shave_lsu_ld16_r_exclusive ( const volatile unsigned short * pointer_1 );	/* LSU.LD16 */
unsigned char	__builtin_shave_lsu_ld8_r_exclusive ( const volatile unsigned char * pointer_1 );	/* LSU.LD8 */

/*  1.6.6.12 LSU.ST */
/*      Register indirect store of specified type to memory. */
void	__builtin_shave_lsu_st128_f16_u8_rr ( half8 variable_1, uchar8 * pointer_1 );	/* LSU.ST128.f16.u8 */
void	__builtin_shave_lsu_st128_f16_u8f_rr ( half8 variable_1, uchar8 * pointer_1 );	/* LSU.ST128.f16.u8f */
void	__builtin_shave_lsu_st128_rgb565_rr ( uint4 variable_1, unsigned short * pointer_1 );	/* LSU.ST128.rgb565  : Myriad2.1 [ma2100], Myriad2.2 [ma2x5x] CPUs only */
void	__builtin_shave_lsu_st128_rgb1555_rr ( uint4 variable_1, unsigned short * pointer_1 );	/* LSU.ST128.rgb1555  : Myriad2.1 [ma2100], Myriad2.2 [ma2x5x] CPUs only */
void	__builtin_shave_lsu_st128_rgb4444_rr ( uint4 variable_1, unsigned short * pointer_1 );	/* LSU.ST128.rgb4444  : Myriad2.1 [ma2100], Myriad2.2 [ma2x5x] CPUs only */


/*  ========================================= */
/*  1.6.8 SAU Instructions */
/*  ========================================= */

/*  1.6.8.1 SAU.ABS */
/*      Short vector/scalar absolute value */
int	__builtin_shave_sau_abs_i32_r ( int variable_1 );	/* SAU.ABS.i32 */
short	__builtin_shave_sau_abs_i16_r ( short variable_1 );	/* SAU.ABS.i16 */
signed char	__builtin_shave_sau_abs_i8_r ( signed char variable_1 );	/* SAU.ABS.i8 */

float	__builtin_shave_sau_abs_f32_r ( float variable_1 );	/* SAU.ABS.f32 */
half	__builtin_shave_sau_abs_f16_r ( half variable_1 );	/* SAU.ABS.f16 */

/*  1.6.8.6 SAU.ADIFF */

int	__builtin_shave_sau_adiff_i32_rr ( int variable_1, int variable_2 );	/* SAU.ADIFF.i32 */
short	__builtin_shave_sau_adiff_i16_rr ( short variable_1, short variable_2 );	/* SAU.ADIFF.i16 */
signed char	__builtin_shave_sau_adiff_i8_rr ( signed char variable_1, signed char variable_2 );	/* SAU.ADIFF.i8 */
unsigned int	__builtin_shave_sau_adiff_u32_rr ( unsigned int variable_1, unsigned int variable_2 );	/* SAU.ADIFF.u32 */
unsigned short	__builtin_shave_sau_adiff_u16_rr ( unsigned short variable_1, unsigned short variable_2 );	/* SAU.ADIFF.u16 */
unsigned char	__builtin_shave_sau_adiff_u8_rr ( unsigned char variable_1, unsigned char variable_2 );	/* SAU.ADIFF.u8 */
float	__builtin_shave_sau_adiff_f32_rr ( float variable_1, float variable_2 );	/* SAU.ADIFF.f32 */
half	__builtin_shave_sau_adiff_f16_rr ( half variable_1, half variable_2 );	/* SAU.ADIFF.f16 */

/*  1.6.8.9 SAU.ANDX */
/*      Element-wise AND vector elements */
int	__builtin_shave_sau_andx_x32_r ( int4 variable_1 );	/* SAU.ANDX.x32 */
short	__builtin_shave_sau_andx_x16_r ( short8 variable_1 );	/* SAU.ANDX.x16 */
signed char	__builtin_shave_sau_andx_x8_r ( schar16 variable_1 );	/* SAU.ANDX.x8 */

/*  1.6.8.10 SAU.ATN */
/*      FP16 arctangent */
half	__builtin_shave_sau_atn_f16_l_r ( half variable_1 );	/* SAU.ATN.f16.l */
half	__builtin_shave_sau_atn_f16_h_r ( half variable_1 );	/* SAU.ATN.f16.h */

/*  1.6.8.13 SAU.COS */
/*      FP16 cosine */
half	__builtin_shave_sau_cos_f16_l_r ( half variable_1 );	/* SAU.COS.f16.l */
half	__builtin_shave_sau_cos_f16_h_r ( half variable_1 );	/* SAU.COS.f16.h */

/*  1.6.8.14 SAU.DIV */
/*      Short vector/scalar division */
float	__builtin_shave_sau_div_f32_rr ( float variable_1, float variable_2 );	/* SAU.DIV.f32 */
int	__builtin_shave_sau_div_i32_rr ( int variable_1, int variable_2 );	/* SAU.DIV.i32 */
unsigned int	__builtin_shave_sau_div_u32_rr ( unsigned int variable_1, unsigned int variable_2 );	/* SAU.DIV.u32 */
short	__builtin_shave_sau_div_i16_rr ( short variable_1, short variable_2 );	/* SAU.DIV.i16 */
unsigned short	__builtin_shave_sau_div_u16_rr ( unsigned short variable_1, unsigned short variable_2 );	/* SAU.DIV.u16 */

/*  1.6.8.15 SAU.EXP2 */
/*      FP16 base-2 exponentiation */
half	__builtin_shave_sau_exp2_f16_l_r ( half variable_1 );	/* SAU.EXP2.f16.l */
half	__builtin_shave_sau_exp2_f16_h_r ( half variable_1 );	/* SAU.EXP2.f16.h */

/*  1.6.8.16 SAU.FRAC */
/*      Fractional part of floating-point number */
float	__builtin_shave_sau_frac_f32_r ( float variable_1 );	/* SAU.FRAC.f32 */
half	__builtin_shave_sau_frac_f16_r ( half variable_1 );	/* SAU.FRAC.f16 */

/*  1.6.8.17 SAU.IADDS */
/*      Short vector/scalar integer addition with saturation. Optional unsigned immediate operand */
int	__builtin_shave_sau_iadds_i32_rr ( int variable_1, int variable_2 );	/* SAU.IADDS.i32 */
unsigned int	__builtin_shave_sau_iadds_u32_rr ( unsigned int variable_1, unsigned int variable_2 );	/* SAU.IADDS.u32 */
short	__builtin_shave_sau_iadds_i16_rr ( short variable_1, short variable_2 );	/* SAU.IADDS.i16 */
unsigned short	__builtin_shave_sau_iadds_u16_rr ( unsigned short variable_1, unsigned short variable_2 );	/* SAU.IADDS.u16 */
signed char	__builtin_shave_sau_iadds_i8_rr ( signed char variable_1, signed char variable_2 );	/* SAU.IADDS.i8 */
unsigned char	__builtin_shave_sau_iadds_u8_rr ( unsigned char variable_1, unsigned char variable_2 );	/* SAU.IADDS.u8 */

/*  1.6.8.18 SAU.ILMULH */
/*      Short vector/scalar multiple returning higher-order result */
int	__builtin_shave_sau_ilmulh_i32_rr ( int variable_1, int variable_2 );	/* SAU.ILMULH.i32 */
unsigned int	__builtin_shave_sau_ilmulh_u32_rr ( unsigned int variable_1, unsigned int variable_2 );	/* SAU.ILMULH.u32 */
short	__builtin_shave_sau_ilmulh_i16_rr ( short variable_1, short variable_2 );	/* SAU.ILMULH.i16 */
unsigned short	__builtin_shave_sau_ilmulh_u16_rr ( unsigned short variable_1, unsigned short variable_2 );	/* SAU.ILMULH.u16 */
signed char	__builtin_shave_sau_ilmulh_i8_rr ( signed char variable_1, signed char variable_2 );	/* SAU.ILMULH.i8 */
unsigned char	__builtin_shave_sau_ilmulh_u8_rr ( unsigned char variable_1, unsigned char variable_2 );	/* SAU.ILMULH.u8 */

/*  1.6.8.19 SAU.IMULS */
/*      Short vector/scalar integer multiplication with saturation. Optional immediate operand */
int	__builtin_shave_sau_imuls_i32_rr ( int variable_1, int variable_2 );	/* SAU.IMULS.i32 */
unsigned int	__builtin_shave_sau_imuls_u32_rr ( unsigned int variable_1, unsigned int variable_2 );	/* SAU.IMULS.u32 */
short	__builtin_shave_sau_imuls_i16_rr ( short variable_1, short variable_2 );	/* SAU.IMULS.i16 */
unsigned short	__builtin_shave_sau_imuls_u16_rr ( unsigned short variable_1, unsigned short variable_2 );	/* SAU.IMULS.u16 */
signed char	__builtin_shave_sau_imuls_i8_rr ( signed char variable_1, signed char variable_2 );	/* SAU.IMULS.i8 */
unsigned char	__builtin_shave_sau_imuls_u8_rr ( unsigned char variable_1, unsigned char variable_2 );	/* SAU.IMULS.u8 */

/*  1.6.8.20 SAU.ISUBS */
/*      Short vector/scalar integer subtraction with saturation. Optional unsigned immediate operand */
int	__builtin_shave_sau_isubs_i32_rr ( int variable_1, int variable_2 );	/* SAU.ISUBS.i32 */
unsigned int	__builtin_shave_sau_isubs_u32_rr ( unsigned int variable_1, unsigned int variable_2 );	/* SAU.ISUBS.u32 */
short	__builtin_shave_sau_isubs_i16_rr ( short variable_1, short variable_2 );	/* SAU.ISUBS.i16 */
unsigned short	__builtin_shave_sau_isubs_u16_rr ( unsigned short variable_1, unsigned short variable_2 );	/* SAU.ISUBS.u16 */
signed char	__builtin_shave_sau_isubs_i8_rr ( signed char variable_1, signed char variable_2 );	/* SAU.ISUBS.i8 */
unsigned char	__builtin_shave_sau_isubs_u8_rr ( unsigned char variable_1, unsigned char variable_2 );	/* SAU.ISUBS.u8 */

/*  1.6.8.21 SAU.LOG2 */
/*      FP16 base-2 logarithm */
half	__builtin_shave_sau_log2_f16_l_r ( half variable_1 );	/* SAU.LOG2.f16.l */
half	__builtin_shave_sau_log2_f16_h_r ( half variable_1 );	/* SAU.LOG2.f16.h */

/*  1.6.8.28 SAU.ONESX */
/*      Long vector ones or zeroes count reduction */
unsigned int	__builtin_shave_sau_onesx_x32_r ( int4 variable_1 );	/* SAU.ONESX.x32 */
unsigned int	__builtin_shave_sau_onesx_x64_r ( short8 variable_1 );	/* SAU.ONESX.x64 */
unsigned int	__builtin_shave_sau_onesx_x128_r ( schar16 variable_1 );	/* SAU.ONESX.x128 */

unsigned int	__builtin_shave_sau_zerosx_x32_r ( int4 variable_1 );	/* SAU.ZEROSX.x32 */
unsigned int	__builtin_shave_sau_zerosx_x64_r ( short8 variable_1 );	/* SAU.ZEROSX.x64 */
unsigned int	__builtin_shave_sau_zerosx_x128_r ( schar16 variable_1 );	/* SAU.ZEROSX.x128 */

/*  1.6.8.30 SAU.ORX */
/*      Element-wise OR of vector */
int	__builtin_shave_sau_orx_x32_r ( int4 variable_1 );	/* SAU.ORX.x32 */
short	__builtin_shave_sau_orx_x16_r ( short8 variable_1 );	/* SAU.ORX.x16 */
signed char	__builtin_shave_sau_orx_x8_r ( schar16 variable_1 );	/* SAU.ORX.x8 */

/*  1.6.8.31 SAU.RCP */
/*      FP16 reciprocal */
half	__builtin_shave_sau_rcp_f16_l_r ( half variable_1 );	/* SAU.RCP.f16.l */
half	__builtin_shave_sau_rcp_f16_h_r ( half variable_1 );	/* SAU.RCP.f16.h */

/*  1.6.8.32 SAU.ROL */
/*      Scalar rotate left with optional immediate rotate amount */
int	__builtin_shave_sau_rol_x32_rr ( int variable_1, int variable_2 );	/* SAU.ROL.x32 */
short	__builtin_shave_sau_rol_x16_rr ( short variable_1, short variable_2 );	/* SAU.ROL.x16 */
signed char	__builtin_shave_sau_rol_x8_rr ( signed char variable_1, signed char variable_2 );	/* SAU.ROL.x8 */

int	__builtin_shave_sau_rol_x32_ri ( int variable_1, unsigned char immediate_1 );	/* SAU.ROL.x32 */
short	__builtin_shave_sau_rol_x16_ri ( short variable_1, unsigned char immediate_1 );	/* SAU.ROL.x16 */
signed char	__builtin_shave_sau_rol_x8_ri ( signed char variable_1, unsigned char immediate_1 );	/* SAU.ROL.x8 */

/*  1.6.8.33 SAU.RQT */
/*      FP16 reciprocal square root */
half	__builtin_shave_sau_rqt_f16_l_r ( half variable_1 );	/* SAU.RQT.f16.l */
half	__builtin_shave_sau_rqt_f16_h_r ( half variable_1 );	/* SAU.RQT.f16.h */

/*  1.6.8.35 SAU.SHL */
/*      Short vector/scalar logical shift left with optional immediate shift amount */
int	__builtin_shave_sau_shl_x32_rr ( int variable_1, int variable_2 );	/* SAU.SHL.x32 */
short	__builtin_shave_sau_shl_x16_rr ( short variable_1, short variable_2 );	/* SAU.SHL.x16 */
signed char	__builtin_shave_sau_shl_x8_rr ( signed char variable_1, signed char variable_2 );	/* SAU.SHL.x8 */

/*  1.6.8.36 SAU.SHR */
/*      Short vector/scalar logical/arithmetic shift right with optional shift amount */
int	__builtin_shave_sau_shr_i32_rr ( int variable_1, int variable_2 );	/* SAU.SHR.i32 */
unsigned int	__builtin_shave_sau_shr_u32_rr ( unsigned int variable_1, unsigned int variable_2 );	/* SAU.SHR.u32 */
short	__builtin_shave_sau_shr_i16_rr ( short variable_1, short variable_2 );	/* SAU.SHR.i16 */
unsigned short	__builtin_shave_sau_shr_u16_rr ( unsigned short variable_1, unsigned short variable_2 );	/* SAU.SHR.u16 */
signed char	__builtin_shave_sau_shr_i8_rr ( signed char variable_1, signed char variable_2 );	/* SAU.SHR.i8 */
unsigned char	__builtin_shave_sau_shr_u8_rr ( unsigned char variable_1, unsigned char variable_2 );	/* SAU.SHR.u8 */

int	__builtin_shave_sau_shr_i32_ri ( int variable_1, unsigned char immediate_1 );	/* SAU.SHR.i32 */
unsigned int	__builtin_shave_sau_shr_u32_ri ( unsigned int variable_1, unsigned char immediate_1 );	/* SAU.SHR.u32 */
short	__builtin_shave_sau_shr_i16_ri ( short variable_1, unsigned char immediate_1 );	/* SAU.SHR.i16 */
unsigned short	__builtin_shave_sau_shr_u16_ri ( unsigned short variable_1, unsigned char immediate_1 );	/* SAU.SHR.u16 */
signed char	__builtin_shave_sau_shr_i8_ri ( signed char variable_1, unsigned char immediate_1 );	/* SAU.SHR.i8 */
unsigned char	__builtin_shave_sau_shr_u8_ri ( unsigned char variable_1, unsigned char immediate_1 );	/* SAU.SHR.u8 */

/*  1.6.8.37 SAU.SIN */
/*      FP16 sine */
half	__builtin_shave_sau_sin_f16_l_r ( half variable_1 );	/* SAU.SIN.f16.l */
half	__builtin_shave_sau_sin_f16_h_r ( half variable_1 );	/* SAU.SIN.f16.h */

/*  1.6.8.38 SAU.SQT */
/*      FP16 square root */
half	__builtin_shave_sau_sqt_f16_l_r ( half variable_1 );	/* SAU.SQT.f16.l */
half	__builtin_shave_sau_sqt_f16_h_r ( half variable_1 );	/* SAU.SQT.f16.h */
half	__builtin_shave_sau_sqt_f16_ri ( half8 variable_1, unsigned char immediate_1 );	/* SAU.SQT.f16 */

/*  1.6.8.42 SAU.SUMX */
/*      Sum vector elements to a scalar */
int	__builtin_shave_sau_sumx_i32_r ( int4 variable_1 );	/* SAU.SUMX.i32 */
int	__builtin_shave_sau_sumx_i16_r ( short8 variable_1 );	/* SAU.SUMX.i16 */
int	__builtin_shave_sau_sumx_i8_r ( schar16 variable_1 );	/* SAU.SUMX.i8 */
int	__builtin_shave_sau_sumx_v4i8_r ( schar4 variable_1 );	/* SAU.SUMX.v4i8 */

unsigned int	__builtin_shave_sau_sumx_u32_r ( uint4 variable_1 );	/* SAU.SUMX.u32 */
unsigned int	__builtin_shave_sau_sumx_u16_r ( ushort8 variable_1 );	/* SAU.SUMX.u16 */
unsigned int	__builtin_shave_sau_sumx_u8_r ( uchar16 variable_1 );	/* SAU.SUMX.u8 */
unsigned int	__builtin_shave_sau_sumx_v4u8_r ( uchar4 variable_1 );	/* SAU.SUMX.v4u8 */

float	__builtin_shave_sau_sumx_f32_r ( float4 variable_1 );	/* SAU.SUMX.f32 */
half	__builtin_shave_sau_sumx_f16_r ( half8 variable_1 );	/* SAU.SUMX.f16 */

/*  1.6.8.45 SAU.XORX */
/*      Element-wise XOR of vector */
int	__builtin_shave_sau_xorx_x32_r ( int4 variable_1 );	/* SAU.XORX.x32 */
short	__builtin_shave_sau_xorx_x16_r ( short8 variable_1 );	/* SAU.XORX.x16 */
signed char	__builtin_shave_sau_xorx_x8_r ( schar16 variable_1 );	/* SAU.XORX.x8 */


/*  ========================================= */
/*  1.6.9 VAU Instructions */
/*  ========================================= */

/*  1.6.9.1 VAU.ABS */
/*      Long vector absolute value */
int4	__builtin_shave_vau_abs_i32_r ( int4 variable_1 );	/* VAU.ABS.i32 */
short8	__builtin_shave_vau_abs_i16_r ( short8 variable_1 );	/* VAU.ABS.i16 */
schar16	__builtin_shave_vau_abs_i8_r ( schar16 variable_1 );	/* VAU.ABS.i8 */
float4	__builtin_shave_vau_abs_f32_r ( float4 variable_1 );	/* VAU.ABS.f32 */
half8	__builtin_shave_vau_abs_f16_r ( half8 variable_1 );	/* VAU.ABS.f16 */

/*  1.6.9.2 VAU.ACC */
/*      Vector add/subtract accumulate with optional zeroing of the accumulator */




/*  1.6.9.3 VAU.ACCW */
/*      Vector add/subtract accumulate with write to VRF */


/*  1.6.9.4 VAU.ADD */
/*      Vector addition with optional immediate operand */
float4	__builtin_shave_vau_add_f32_rr ( float4 variable_1, float4 variable_2 );	/* VAU.ADD.f32 */
int4	__builtin_shave_vau_add_i32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.ADD.i32 */
half8	__builtin_shave_vau_add_f16_rr ( half8 variable_1, half8 variable_2 );	/* VAU.ADD.f16 */
short8	__builtin_shave_vau_add_i16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.ADD.i16 */
schar16	__builtin_shave_vau_add_i8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.ADD.i8 */

int4	__builtin_shave_vau_add_i32_ri ( int4 variable_1, unsigned char immediate_1 );	/* VAU.ADD.i32 */
short8	__builtin_shave_vau_add_i16_ri ( short8 variable_1, unsigned char immediate_1 );	/* VAU.ADD.i16 */
schar16	__builtin_shave_vau_add_i8_ri ( schar16 variable_1, unsigned char immediate_1 );	/* VAU.ADD.i8 */

/*  1.6.9.6 VAU.ADIFF */
/*      Long vector absolute difference */
float4	__builtin_shave_vau_adiff_f32_rr ( float4 variable_1, float4 variable_2 );	/* VAU.ADIFF.f32 */
half8	__builtin_shave_vau_adiff_f16_rr ( half8 variable_1, half8 variable_2 );	/* VAU.ADIFF.f16 */
int4	__builtin_shave_vau_adiff_i32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.ADIFF.i32 */
uint4	__builtin_shave_vau_adiff_u32_rr ( uint4 variable_1, uint4 variable_2 );	/* VAU.ADIFF.u32 */
short8	__builtin_shave_vau_adiff_i16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.ADIFF.i16 */
ushort8	__builtin_shave_vau_adiff_u16_rr ( ushort8 variable_1, ushort8 variable_2 );	/* VAU.ADIFF.u16 */
schar16	__builtin_shave_vau_adiff_i8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.ADIFF.i8 */
uchar16	__builtin_shave_vau_adiff_u8_rr ( uchar16 variable_1, uchar16 variable_2 );	/* VAU.ADIFF.u8 */

/*  1.6.9.7 VAU.ALIGNBYTE */
/*      Long vector align bytes within words */
int4	__builtin_shave_vau_alignbyte_rri_int4 ( int4 variable_1, int4 variable_2, signed char immediate_1 );	/* VAU.ALIGNBYTE */
short8	__builtin_shave_vau_alignbyte_rri_short8 ( short8 variable_1, short8 variable_2, signed char immediate_1 );	/* VAU.ALIGNBYTE */
schar16	__builtin_shave_vau_alignbyte_rri_char16 ( schar16 variable_1, schar16 variable_2, signed char immediate_1 );	/* VAU.ALIGNBYTE */

/*  1.6.9.8 VAU.AND */
/*      Bitwise AND */
int4	__builtin_shave_vau_and_32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.AND.32 */
short8	__builtin_shave_vau_and_16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.AND.16 */
schar16	__builtin_shave_vau_and_8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.AND.8 */

/*  1.6.9.9 VAU.AVG */
/*      Integer average */
uint4	__builtin_shave_vau_avg_x32_rr ( uint4 variable_1, uint4 variable_2 );	/* VAU.AVG.x32 */
ushort8	__builtin_shave_vau_avg_x16_rr ( ushort8 variable_1, ushort8 variable_2 );	/* VAU.AVG.x16 */
uchar16	__builtin_shave_vau_avg_x8_rr ( uchar16 variable_1, uchar16 variable_2 );	/* VAU.AVG.x8 */

/*  1.6.9.10 VAU.CMB */
/*      Long vector combine */
/*      The 4 immediate operands are booleans and differ from the ISA document in the following ways: */
/*          the value 0 or 'false' implies 'D' */
/*          the value 1 or 'true' implies 'E' */
/*          element number increases from left to right */
int4	__builtin_shave_vau_cmbbyte_rriiii_int4 ( int4 variable_1, int4 variable_2, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* VAU.CMBBYTE */
short8	__builtin_shave_vau_cmbbyte_rriiii_short8 ( short8 variable_1, short8 variable_2, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* VAU.CMBBYTE */
schar16	__builtin_shave_vau_cmbbyte_rriiii_char16 ( schar16 variable_1, schar16 variable_2, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* VAU.CMBBYTE */
float4	__builtin_shave_vau_cmbword_rriiii_float4 ( float4 variable_1, float4 variable_2, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* VAU.CMBWORD */
int4	__builtin_shave_vau_cmbword_rriiii_int4 ( int4 variable_1, int4 variable_2, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* VAU.CMBWORD */
short8	__builtin_shave_vau_cmbword_rriiii_short8 ( short8 variable_1, short8 variable_2, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* VAU.CMBWORD */
schar16	__builtin_shave_vau_cmbword_rriiii_char16 ( schar16 variable_1, schar16 variable_2, signed char immediate_1, signed char immediate_2, signed char immediate_3, signed char immediate_4 );	/* VAU.CMBWORD */

/*  1.6.9.11 VAU.IADDS */
/*      Long vector integer addition with saturation. Optional unsigned immediate operand */
int4	__builtin_shave_vau_iadds_i32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.IADDS.i32 */
uint4	__builtin_shave_vau_iadds_u32_rr ( uint4 variable_1, uint4 variable_2 );	/* VAU.IADDS.u32 */
short8	__builtin_shave_vau_iadds_i16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.IADDS.i16 */
ushort8	__builtin_shave_vau_iadds_u16_rr ( ushort8 variable_1, ushort8 variable_2 );	/* VAU.IADDS.u16 */
schar16	__builtin_shave_vau_iadds_i8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.IADDS.i8 */
uchar16	__builtin_shave_vau_iadds_u8_rr ( uchar16 variable_1, uchar16 variable_2 );	/* VAU.IADDS.u8 */

int4	__builtin_shave_vau_iadds_i32_ri ( int4 variable_1, unsigned char immediate_1 );	/* VAU.IADDS.i32 */
uint4	__builtin_shave_vau_iadds_u32_ri ( uint4 variable_1, unsigned char immediate_1 );	/* VAU.IADDS.u32 */
short8	__builtin_shave_vau_iadds_i16_ri ( short8 variable_1, unsigned char immediate_1 );	/* VAU.IADDS.i16 */
ushort8	__builtin_shave_vau_iadds_u16_ri ( ushort8 variable_1, unsigned char immediate_1 );	/* VAU.IADDS.u16 */
schar16	__builtin_shave_vau_iadds_i8_ri ( schar16 variable_1, unsigned char immediate_1 );	/* VAU.IADDS.i8 */
uchar16	__builtin_shave_vau_iadds_u8_ri ( uchar16 variable_1, unsigned char immediate_1 );	/* VAU.IADDS.u8 */

/*  1.6.9.12 VAU.ILMULH */
/*      Long vector multiply returning higher-order result */
int4	__builtin_shave_vau_ilmulh_i32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.ILMULH.i32 */
short8	__builtin_shave_vau_ilmulh_i16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.ILMULH.i16 */
ushort8	__builtin_shave_vau_ilmulh_u16_rr ( ushort8 variable_1, ushort8 variable_2 );	/* VAU.ILMULH.u16 */
schar16	__builtin_shave_vau_ilmulh_i8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.ILMULH.i8 */
uchar16	__builtin_shave_vau_ilmulh_u8_rr ( uchar16 variable_1, uchar16 variable_2 );	/* VAU.ILMULH.u8 */

/*  1.6.9.13 VAU.IMULS */
/*      Long vector integer multiplication with saturation. Optional immediate operand */
int4	__builtin_shave_vau_imuls_i32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.IMULS.i32 */
uint4	__builtin_shave_vau_imuls_u32_rr ( uint4 variable_1, uint4 variable_2 );	/* VAU.IMULS.u32 */
short8	__builtin_shave_vau_imuls_i16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.IMULS.i16 */
ushort8	__builtin_shave_vau_imuls_u16_rr ( ushort8 variable_1, ushort8 variable_2 );	/* VAU.IMULS.u16 */
schar16	__builtin_shave_vau_imuls_i8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.IMULS.i8 */
uchar16	__builtin_shave_vau_imuls_u8_rr ( uchar16 variable_1, uchar16 variable_2 );	/* VAU.IMULS.u8 */

int4	__builtin_shave_vau_imuls_i32_ri ( int4 variable_1, unsigned char immediate_1 );	/* VAU.IMULS.i32 */
uint4	__builtin_shave_vau_imuls_u32_ri ( uint4 variable_1, unsigned char immediate_1 );	/* VAU.IMULS.u32 */
short8	__builtin_shave_vau_imuls_i16_ri ( short8 variable_1, unsigned char immediate_1 );	/* VAU.IMULS.i16 */
ushort8	__builtin_shave_vau_imuls_u16_ri ( ushort8 variable_1, unsigned char immediate_1 );	/* VAU.IMULS.u16 */
schar16	__builtin_shave_vau_imuls_i8_ri ( schar16 variable_1, unsigned char immediate_1 );	/* VAU.IMULS.i8 */
uchar16	__builtin_shave_vau_imuls_u8_ri ( uchar16 variable_1, unsigned char immediate_1 );	/* VAU.IMULS.u8 */

/*  1.6.9.14 VAU.ISUBS */
/*      Long vector integer subtraction with saturation. Optional unsigned immediate operand */
int4	__builtin_shave_vau_isubs_i32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.ISUBS.i32 */
uint4	__builtin_shave_vau_isubs_u32_rr ( uint4 variable_1, uint4 variable_2 );	/* VAU.ISUBS.u32 */
short8	__builtin_shave_vau_isubs_i16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.ISUBS.i16 */
ushort8	__builtin_shave_vau_isubs_u16_rr ( ushort8 variable_1, ushort8 variable_2 );	/* VAU.ISUBS.u16 */
schar16	__builtin_shave_vau_isubs_i8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.ISUBS.i8 */
uchar16	__builtin_shave_vau_isubs_u8_rr ( uchar16 variable_1, uchar16 variable_2 );	/* VAU.ISUBS.u8 */

int4	__builtin_shave_vau_isubs_i32_ri ( int4 variable_1, unsigned char immediate_1 );	/* VAU.ISUBS.i32 */
uint4	__builtin_shave_vau_isubs_u32_ri ( uint4 variable_1, unsigned char immediate_1 );	/* VAU.ISUBS.u32 */
short8	__builtin_shave_vau_isubs_i16_ri ( short8 variable_1, unsigned char immediate_1 );	/* VAU.ISUBS.i16 */
ushort8	__builtin_shave_vau_isubs_u16_ri ( ushort8 variable_1, unsigned char immediate_1 );	/* VAU.ISUBS.u16 */
schar16	__builtin_shave_vau_isubs_i8_ri ( schar16 variable_1, unsigned char immediate_1 );	/* VAU.ISUBS.i8 */
uchar16	__builtin_shave_vau_isubs_u8_ri ( uchar16 variable_1, unsigned char immediate_1 );	/* VAU.ISUBS.u8 */

/*  1.6.9.15 VAU.MAC */
/*      Vector multiply add/subtract accumulate with optional zeroing of the accumulator */




/*  1.6.9.16 MACNW */
/*      Vector multiply-subtract accumulate with write to VRF */

/*  1.6.9.17 MACPW */
/*      Vector multiply-add accumulate with write to VRF */

/*  1.6.9.18 VAU.MUL */
/*      Long vector multiplication with optional immediate operand */
float4	__builtin_shave_vau_mul_f32_rr ( float4 variable_1, float4 variable_2 );	/* VAU.MUL.f32 */
int4	__builtin_shave_vau_mul_i32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.MUL.i32 */
half8	__builtin_shave_vau_mul_f16_rr ( half8 variable_1, half8 variable_2 );	/* VAU.MUL.f16 */
short8	__builtin_shave_vau_mul_i16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.MUL.i16 */
schar16	__builtin_shave_vau_mul_i8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.MUL.i8 */

int4	__builtin_shave_vau_mul_i32_ri ( int4 variable_1, signed char immediate_1 );	/* VAU.MUL.i32 */
short8	__builtin_shave_vau_mul_i16_ri ( short8 variable_1, signed char immediate_1 );	/* VAU.MUL.i16 */
schar16	__builtin_shave_vau_mul_i8_ri ( schar16 variable_1, signed char immediate_1 );	/* VAU.MUL.i8 */

/*  1.6.9.19 VAU.NOT */
/*      Bitwise NOT */
int4	__builtin_shave_vau_not_i32_r ( int4 variable_1 );	/* VAU.NOT.i32 */
short8	__builtin_shave_vau_not_i16_r ( short8 variable_1 );	/* VAU.NOT.i16 */
schar16	__builtin_shave_vau_not_i8_r ( schar16 variable_1 );	/* VAU.NOT.i8 */

/*  1.6.9.20 VAU.OR */
/*      Bitwise OR */
int4	__builtin_shave_vau_or_32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.OR.32 */
short8	__builtin_shave_vau_or_16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.OR.16 */
schar16	__builtin_shave_vau_or_8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.OR.8 */

/*  1.6.9.21 VAU.ROL */
/*      Vector rotate left */
int4	__builtin_shave_vau_rol_x32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.ROL.x32 */
short8	__builtin_shave_vau_rol_x16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.ROL.x16 */
schar16	__builtin_shave_vau_rol_x8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.ROL.x8 */

int4	__builtin_shave_vau_rol_x32_ri ( int4 variable_1, unsigned char immediate_1 );	/* VAU.ROL.x32 */
short8	__builtin_shave_vau_rol_x16_ri ( short8 variable_1, unsigned char immediate_1 );	/* VAU.ROL.x16 */
schar16	__builtin_shave_vau_rol_x8_ri ( schar16 variable_1, unsigned char immediate_1 );	/* VAU.ROL.x8 */

/*  1.6.9.22 VAU.SHD */
/*      Long vector arithmetic shift right with set to zero on underflow. Optional immediate shift amount */
int4	__builtin_shave_vau_shd_i32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.SHD.i32 */
short8	__builtin_shave_vau_shd_i16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.SHD.i16 */
schar16	__builtin_shave_vau_shd_i8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.SHD.i8 */

int4	__builtin_shave_vau_shd_i32_ri ( int4 variable_1, unsigned char immediate_1 );	/* VAU.SHD.i32 */
short8	__builtin_shave_vau_shd_i16_ri ( short8 variable_1, unsigned char immediate_1 );	/* VAU.SHD.i16 */
schar16	__builtin_shave_vau_shd_i8_ri ( schar16 variable_1, unsigned char immediate_1 );	/* VAU.SHD.i8 */

/*  1.6.9.23 VAU.SHL */
/*      Long vector logical shift left with optional immediate shift amount */
uint4	__builtin_shave_vau_shl_x32_rr ( uint4 variable_1, uint4 variable_2 );	/* VAU.SHL.x32 */
ushort8	__builtin_shave_vau_shl_x16_rr ( ushort8 variable_1, ushort8 variable_2 );	/* VAU.SHL.x16 */
uchar16	__builtin_shave_vau_shl_x8_rr ( uchar16 variable_1, uchar16 variable_2 );	/* VAU.SHL.x8 */

uint4	__builtin_shave_vau_shl_x32_ri ( uint4 variable_1, unsigned char immediate_1 );	/* VAU.SHL.x32 */
ushort8	__builtin_shave_vau_shl_x16_ri ( ushort8 variable_1, unsigned char immediate_1 );	/* VAU.SHL.x16 */
uchar16	__builtin_shave_vau_shl_x8_ri ( uchar16 variable_1, unsigned char immediate_1 );	/* VAU.SHL.x8 */

/*  1.6.9.24 VAU.SHR */
/*      Long vector logical/arithmetic shift right with optional immediate shift amount */
int4	__builtin_shave_vau_shr_i32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.SHR.i32 */
uint4	__builtin_shave_vau_shr_u32_rr ( uint4 variable_1, uint4 variable_2 );	/* VAU.SHR.u32 */
short8	__builtin_shave_vau_shr_i16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.SHR.i16 */
ushort8	__builtin_shave_vau_shr_u16_rr ( ushort8 variable_1, ushort8 variable_2 );	/* VAU.SHR.u16 */
schar16	__builtin_shave_vau_shr_i8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.SHR.i8 */
uchar16	__builtin_shave_vau_shr_u8_rr ( uchar16 variable_1, uchar16 variable_2 );	/* VAU.SHR.u8 */

int4	__builtin_shave_vau_shr_i32_ri ( int4 variable_1, unsigned char immediate_1 );	/* VAU.SHR.i32 */
uint4	__builtin_shave_vau_shr_u32_ri ( uint4 variable_1, unsigned char immediate_1 );	/* VAU.SHR.u32 */
short8	__builtin_shave_vau_shr_i16_ri ( short8 variable_1, unsigned char immediate_1 );	/* VAU.SHR.i16 */
ushort8	__builtin_shave_vau_shr_u16_ri ( ushort8 variable_1, unsigned char immediate_1 );	/* VAU.SHR.u16 */
schar16	__builtin_shave_vau_shr_i8_ri ( schar16 variable_1, unsigned char immediate_1 );	/* VAU.SHR.i8 */
uchar16	__builtin_shave_vau_shr_u8_ri ( uchar16 variable_1, unsigned char immediate_1 );	/* VAU.SHR.u8 */

/*  1.6.9.25 VAU.SUB */
/*      Long vector subtraction with optional immediate operand */
float4	__builtin_shave_vau_sub_f32_rr ( float4 variable_1, float4 variable_2 );	/* VAU.SUB.f32 */
int4	__builtin_shave_vau_sub_i32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.SUB.i32 */
half8	__builtin_shave_vau_sub_f16_rr ( half8 variable_1, half8 variable_2 );	/* VAU.SUB.f16 */
short8	__builtin_shave_vau_sub_i16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.SUB.i16 */
schar16	__builtin_shave_vau_sub_i8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.SUB.i8 */

int4	__builtin_shave_vau_sub_i32_ri ( int4 variable_1, unsigned char immediate_1 );	/* VAU.SUB.i32 */
short8	__builtin_shave_vau_sub_i16_ri ( short8 variable_1, unsigned char immediate_1 );	/* VAU.SUB.i16 */
schar16	__builtin_shave_vau_sub_i8_ri ( schar16 variable_1, unsigned char immediate_1 );	/* VAU.SUB.i8 */

/*  1.6.9.27 VAU.XOR */
/*      Bitwise XOR */
int4	__builtin_shave_vau_xor_32_rr ( int4 variable_1, int4 variable_2 );	/* VAU.XOR.32 */
short8	__builtin_shave_vau_xor_16_rr ( short8 variable_1, short8 variable_2 );	/* VAU.XOR.16 */
schar16	__builtin_shave_vau_xor_8_rr ( schar16 variable_1, schar16 variable_2 );	/* VAU.XOR.8 */



/*  ============================================================================================================== */
/*      SHAVE has no native support for 64-bit vectors, but it can generate more efficient code for 64-bit vectors */
/*      using the native 128-bit instructions when the programmer uses explicit 64-bit vectors. */

/*      In some cases 'moviCompile' has internal support for 64-bit vectors, though not all 128-bit vectors have */
/*      equivalent 64-bit vector support.  But for those that are supported, the following built-ins provide 64-bit */
/*      type-safe variants of their 128-bit counterparts above */
float2	__builtin_shave_vau_abs_v2f32_r ( float2 variable_1 );	/* Meta-Instruction */
half4	__builtin_shave_vau_abs_v4f16_r ( half4 variable_1 );	/* Meta-Instruction */

float2	__builtin_shave_vau_add_v2f32_rr ( float2 variable_1, float2 variable_2 );	/* Meta-Instruction */
int2	__builtin_shave_vau_add_v2i32_rr ( int2 variable_1, int2 variable_2 );	/* Meta-Instruction */
half4	__builtin_shave_vau_add_v4f16_rr ( half4 variable_1, half4 variable_2 );	/* Meta-Instruction */
short4	__builtin_shave_vau_add_v4i16_rr ( short4 variable_1, short4 variable_2 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_add_v8i8_rr ( schar8 variable_1, schar8 variable_2 );	/* Meta-Instruction */

int2	__builtin_shave_vau_add_i32_ri_int2 ( int2 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
short4	__builtin_shave_vau_add_i16_ri_short4 ( short4 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_add_i8_ri_schar8 ( schar8 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */

uint2	__builtin_shave_vau_and_v2i32_rr ( uint2 variable_1, uint2 variable_2 );	/* Meta-Instruction */
ushort4	__builtin_shave_vau_and_v4i16_rr ( ushort4 variable_1, ushort4 variable_2 );	/* Meta-Instruction */
uchar8	__builtin_shave_vau_and_v8i8_rr ( uchar8 variable_1, uchar8 variable_2 );	/* Meta-Instruction */

float2	__builtin_shave_vau_mul_v2f32_rr ( float2 variable_1, float2 variable_2 );	/* Meta-Instruction */
int2	__builtin_shave_vau_mul_v2i32_rr ( int2 variable_1, int2 variable_2 );	/* Meta-Instruction */
half4	__builtin_shave_vau_mul_v4f16_rr ( half4 variable_1, half4 variable_2 );	/* Meta-Instruction */
short4	__builtin_shave_vau_mul_v4i16_rr ( short4 variable_1, short4 variable_2 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_mul_v8i8_rr ( schar8 variable_1, schar8 variable_2 );	/* Meta-Instruction */

int2	__builtin_shave_vau_mul_i32_ri_int2 ( int2 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
short4	__builtin_shave_vau_mul_i16_ri_short4 ( short4 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_mul_i8_ri_schar8 ( schar8 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */

uint2	__builtin_shave_vau_not_v2i32_r ( uint2 variable_1 );	/* Meta-Instruction */
ushort4	__builtin_shave_vau_not_v4i16_r ( ushort4 variable_1 );	/* Meta-Instruction */
uchar8	__builtin_shave_vau_not_v8i8_r ( uchar8 variable_1 );	/* Meta-Instruction */

uint2	__builtin_shave_vau_or_v2i32_rr ( uint2 variable_1, uint2 variable_2 );	/* Meta-Instruction */
ushort4	__builtin_shave_vau_or_v4i16_rr ( ushort4 variable_1, ushort4 variable_2 );	/* Meta-Instruction */
uchar8	__builtin_shave_vau_or_v8i8_rr ( uchar8 variable_1, uchar8 variable_2 );	/* Meta-Instruction */

int2	__builtin_shave_vau_rol_v2x32_rr ( int2 variable_1, int2 variable_2 );	/* Meta-Instruction */
short4	__builtin_shave_vau_rol_v4x16_rr ( short4 variable_1, short4 variable_2 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_rol_v8x8_rr ( schar8 variable_1, schar8 variable_2 );	/* Meta-Instruction */

int2	__builtin_shave_vau_rol_x32_ri_uint2 ( int2 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
short4	__builtin_shave_vau_rol_x16_ri_ushort4 ( short4 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_rol_x8_ri_uchar8 ( schar8 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */

int2	__builtin_shave_vau_shd_v2i32_rr ( int2 variable_1, int2 variable_2 );	/* Meta-Instruction */
short4	__builtin_shave_vau_shd_v4i16_rr ( short4 variable_1, short4 variable_2 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_shd_v8i8_rr ( schar8 variable_1, schar8 variable_2 );	/* Meta-Instruction */

int2	__builtin_shave_vau_shd_i32_ri_int2 ( int2 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
short4	__builtin_shave_vau_shd_i16_ri_short4 ( short4 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_shd_i8_ri_schar8 ( schar8 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */

int2	__builtin_shave_vau_shl_v2x32_rr ( int2 variable_1, int2 variable_2 );	/* Meta-Instruction */
short4	__builtin_shave_vau_shl_v4x16_rr ( short4 variable_1, short4 variable_2 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_shl_v8x8_rr ( schar8 variable_1, schar8 variable_2 );	/* Meta-Instruction */

int2	__builtin_shave_vau_shl_x32_ri_int2 ( int2 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
short4	__builtin_shave_vau_shl_x16_ri_short4 ( short4 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_shl_x8_ri_schar8 ( schar8 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */

int2	__builtin_shave_vau_shr_v2i32_rr ( int2 variable_1, int2 variable_2 );	/* Meta-Instruction */
uint2	__builtin_shave_vau_shr_v2u32_rr ( uint2 variable_1, uint2 variable_2 );	/* Meta-Instruction */
short4	__builtin_shave_vau_shr_v4i16_rr ( short4 variable_1, short4 variable_2 );	/* Meta-Instruction */
ushort4	__builtin_shave_vau_shr_v4u16_rr ( ushort4 variable_1, ushort4 variable_2 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_shr_v8i8_rr ( schar8 variable_1, schar8 variable_2 );	/* Meta-Instruction */
uchar8	__builtin_shave_vau_shr_v8u8_rr ( uchar8 variable_1, uchar8 variable_2 );	/* Meta-Instruction */

int2	__builtin_shave_vau_shr_i32_ri_int2 ( int2 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
uint2	__builtin_shave_vau_shr_u32_ri_int2 ( uint2 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
short4	__builtin_shave_vau_shr_i16_ri_short4 ( short4 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
ushort4	__builtin_shave_vau_shr_u16_ri_short4 ( ushort4 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_shr_i8_ri_schar8 ( schar8 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
uchar8	__builtin_shave_vau_shr_u8_ri_schar8 ( uchar8 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */

float2	__builtin_shave_vau_sub_v2f32_rr ( float2 variable_1, float2 variable_2 );	/* Meta-Instruction */
int2	__builtin_shave_vau_sub_v2i32_rr ( int2 variable_1, int2 variable_2 );	/* Meta-Instruction */
half4	__builtin_shave_vau_sub_v4f16_rr ( half4 variable_1, half4 variable_2 );	/* Meta-Instruction */
short4	__builtin_shave_vau_sub_v4i16_rr ( short4 variable_1, short4 variable_2 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_sub_v8i8_rr ( schar8 variable_1, schar8 variable_2 );	/* Meta-Instruction */

int2	__builtin_shave_vau_sub_i32_ri_int2 ( int2 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
short4	__builtin_shave_vau_sub_i16_ri_short4 ( short4 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */
schar8	__builtin_shave_vau_sub_i8_ri_schar8 ( schar8 variable_1, unsigned char immediate_1 );	/* Meta-Instruction */

uint2	__builtin_shave_vau_xor_v2i32_rr ( uint2 variable_1, uint2 variable_2 );	/* Meta-Instruction */
ushort4	__builtin_shave_vau_xor_v4i16_rr ( ushort4 variable_1, ushort4 variable_2 );	/* Meta-Instruction */
uchar8	__builtin_shave_vau_xor_v8i8_rr ( uchar8 variable_1, uchar8 variable_2 );	/* Meta-Instruction */


_MV_EXTERNC_END

#endif /* __shave__ */
#endif /* __SHAVE_BUILTIN_PSEUDO_DECLARATIONS__ */
