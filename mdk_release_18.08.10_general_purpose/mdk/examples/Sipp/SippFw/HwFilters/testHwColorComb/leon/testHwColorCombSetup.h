/*
 * testHwColorCombSetup.h
 *
 *  Created on: 30 Sep 2015
 *
 */

#ifndef _TESTHWCOLORCOMBSETUP_H_
#define _TESTHWCOLORCOMBSETUP_H_


////////////////////////////////////////////////////////////////////
// Test Macros

#define FORCE_LUMA_ONE      0
#define LUT_3D_BYPASS       DISABLED
#define LUT_3D_LOAD         ENABLED
#define U12F_OUTPUT         ENABLED
#define MUL                 255
#define T1                  2
#define OUTPUT_PLANE_MULT   3

#define K_R                 0x0303 // 256.f/85
#define K_G                 0x0303 // 256.f/85

#define K_B                 0x0303 // 256.f/85
#define EPSILON             5

#define R_OFF               0x0008 // (FLOAT_TO_FIXED1_12(+0.002f) & 0x1fff)
#define G_OFF               0x0018 // (FLOAT_TO_FIXED1_12(+0.006f) & 0x1fff)
#define B_OFF               0x1ff4 // (FLOAT_TO_FIXED1_12(-0.003f) & 0x1fff)

#define SIPP_HW_COL_COMB_LUMA_BUFFER_WIDTH       1584
#define SIPP_HW_COL_COMB_LUMA_BUFFER_HEIGHT      1290
#define SIPP_HW_COL_COMB_LUMA_BUFFER_NUM_PLANES  1
#define SIPP_HW_COL_COMB_LUMA_BUFFER_SIZE        (SIPP_HW_COL_COMB_LUMA_BUFFER_WIDTH * SIPP_HW_COL_COMB_LUMA_BUFFER_HEIGHT * SIPP_HW_COL_COMB_LUMA_BUFFER_NUM_PLANES)
#define SIPP_HW_COL_COMB_CHR_BUFFER_WIDTH        (1584 >> 1)
#define SIPP_HW_COL_COMB_CHR_BUFFER_HEIGHT       (1290 >> 1)
#define SIPP_HW_COL_COMB_CHR_BUFFER_NUM_PLANES   3
#define SIPP_HW_COL_COMB_CHR_BUFFER_SIZE         (SIPP_HW_COL_COMB_CHR_BUFFER_WIDTH * SIPP_HW_COL_COMB_CHR_BUFFER_HEIGHT * SIPP_HW_COL_COMB_CHR_BUFFER_NUM_PLANES)
#define SIPP_HW_COL_COMB_BUFFER_WIDTH            1584
#define SIPP_HW_COL_COMB_BUFFER_HEIGHT           1290
#define SIPP_HW_COL_COMB_BUFFER_NUM_PLANES       3
#define SIPP_HW_COL_COMB_BUFFER_SIZE             (SIPP_HW_COL_COMB_BUFFER_WIDTH * SIPP_HW_COL_COMB_BUFFER_HEIGHT * SIPP_HW_COL_COMB_BUFFER_NUM_PLANES)


////////////////////////////////////////////////////////////////////
// Test Globals

extern u16 colCorrMatrix[];
extern u16 Lut3D[16 * 16 * 16 * 4];

#endif /* _TESTHWCOLORCOMBSETUP_H_ */
