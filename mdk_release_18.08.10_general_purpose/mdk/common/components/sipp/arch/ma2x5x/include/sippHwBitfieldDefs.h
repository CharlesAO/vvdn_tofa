/*
 * sippHwBitfieldDefs.h
 *
 *  Created on: Apr 14, 2015
 */

#include "sippPlatform.h"

#ifndef _SIPPHWBITFIELDDEFS_H_
#define _SIPPHWBITFIELDDEFS_H_

typedef enum
{
  eRawFormatPlanar = 0,
  eRawFormatBayer

} eRawInputFmt;


typedef enum
{
  eBayerOrderGRBG = 0,
  eBayerOrderRGGB,
  eBayerOrderGBRG,
  eBayerOrderBGGR

} eBayerOrder;

// Type definitions

/* SIPP HW Filter bitfield macros */

/* Convolution Filter */

#define CONV_CFG_KERNEL_SIZE_MASK       0x7
#define CONV_CFG_KERNEL_SIZE_SHIFT      0x0
#define CONV_CFG_OUTPUT_CLAMP_EN_MASK   0x1
#define CONV_CFG_OUTPUT_CLAMP_EN_SHIFT  0x3
#define CONV_CFG_OUTPUT_ABS_VAL_MASK    0x1
#define CONV_CFG_OUTPUT_ABS_VAL_SHIFT   0x4
#define CONV_CFG_OUTPUT_SQUARE_EN_MASK  0x1
#define CONV_CFG_OUTPUT_SQUARE_EN_SHIFT 0x5
#define CONV_CFG_ACCUMULATION_EN_MASK   0x1
#define CONV_CFG_ACCUMULATION_EN_SHIFT  0x6
#define CONV_CFG_FILTER_DISABLE_MASK    0x1
#define CONV_CFG_FILTER_DISABLE_SHIFT   0x7
#define CONV_CFG_ACCUM_THRESHOLD_MASK   0xFFFF
#define CONV_CFG_ACCUM_THRESHOLD_SHIFT  0x8
#define CONV_CFG_EVENODD_PIX_MASK       0x1
#define CONV_CFG_EVENODD_PIX_SHIFT      0x18
#define CONV_CFG_EVENODD_LINE_MASK      0x1
#define CONV_CFG_EVENODD_LINE_SHIFT     0x19

#define CONV_KERNEL_SIZE(val)     ((val & CONV_CFG_KERNEL_SIZE_MASK) << CONV_CFG_KERNEL_SIZE_SHIFT)
#define CONV_OUTPUT_CLAMP(val)    ((val & CONV_CFG_OUTPUT_CLAMP_EN_MASK) << CONV_CFG_OUTPUT_CLAMP_EN_SHIFT)
#define CONV_OUTPUT_ABS_VAL(val)  ((val & CONV_CFG_OUTPUT_ABS_VAL_MASK) << CONV_CFG_OUTPUT_ABS_VAL_SHIFT)
#define CONV_OUTPUT_SQUARE(val)   ((val & CONV_CFG_OUTPUT_SQUARE_EN_MASK) << CONV_CFG_OUTPUT_SQUARE_EN_SHIFT)
#define CONV_ACCUMULATION_EN(val) ((val & CONV_CFG_ACCUMULATION_EN_MASK) << CONV_CFG_ACCUMULATION_EN_SHIFT)
#define CONV_FILTER_DISABLE(val)  ((val & CONV_CFG_FILTER_DISABLE_MASK) << CONV_CFG_FILTER_DISABLE_SHIFT)
#define CONV_ACCUM_THRESHOLD(val) ((val & CONV_CFG_ACCUM_THRESHOLD_MASK) << CONV_CFG_ACCUM_THRESHOLD_SHIFT)
#define CONV_EVENODD_PIX(val)     ((val & CONV_CFG_EVENODD_PIX_MASK) << CONV_CFG_EVENODD_PIX_SHIFT)
#define CONV_EVENODD_LINE(val)    ((val & CONV_CFG_EVENODD_LINE_MASK) << CONV_CFG_EVENODD_LINE_SHIFT)

#define CONV_KSZ_3x3            CONV_KERNEL_SIZE(0x3)
#define CONV_KSZ_5x5            CONV_KERNEL_SIZE(0x5)

#define CONV_CFG(kernSize, outClamp, outAbsVal, outSquare,accEn, filterDis, accTresh, evenOddPix, evenOddLine) CONV_KERNEL_SIZE(kernSize) |\
                                                                                                               CONV_OUTPUT_CLAMP(outClamp) |\
                                                                                                               CONV_OUTPUT_ABS_VAL(outAbsVal)|\
                                                                                                               CONV_OUTPUT_SQUARE(outSquare)|\
                                                                                                               CONV_ACCUMULATION_EN(accEn)|\
                                                                                                               CONV_FILTER_DISABLE(filterDis)|\
                                                                                                               CONV_ACCUM_THRESHOLD(accTresh)|\
                                                                                                               CONV_EVENODD_PIX(evenOddPix)|\
                                                                                                               CONV_EVENODD_LINE(evenOddLine)
/* Debayer Filter */
//// Cfg Register
#define DEBAYER_CFG_BAYER_ORDER_MASK      0x3
#define DEBAYER_CFG_BAYER_ORDER_SHIFT     0x0
#define DEBAYER_CFG_LUMA_ONLY_MASK        0x1
#define DEBAYER_CFG_LUMA_ONLY_SHIFT       0x2
#define DEBAYER_CFG_FORCE_RB_ZERO_MASK    0x1
#define DEBAYER_CFG_FORCE_RB_ZERO_SHIFT   0x3
#define DEBAYER_CFG_IP_DATAWIDTH_M1_MASK  0xF
#define DEBAYER_CFG_IP_DATAWIDTH_M1_SHIFT 0x4
#define DEBAYER_CFG_OP_DATAWIDTH_M1_MASK  0xF
#define DEBAYER_CFG_OP_DATAWIDTH_M1_SHIFT 0x8
#define DEBAYER_CFG_OUTPLANE_ORDER_MASK   0x7
#define DEBAYER_CFG_OUTPLANE_ORDER_SHIFT  0xC
#define DEBAYER_CFG_NUM_OUTPLANES_MASK    0x3
#define DEBAYER_CFG_NUM_OUTPLANES_SHIFT   0xF
#define DEBAYER_CFG_GRAD_MULTIPLIER_MASK  0xFF
#define DEBAYER_CFG_GRAD_MULTIPLIER_SHIFT 0x18

#define DEBAYER_BAYER_ORDER(val)     ((val & DEBAYER_CFG_BAYER_ORDER_MASK) << DEBAYER_CFG_BAYER_ORDER_SHIFT)
#define DEBAYER_LUMA_ONLY(val)       ((val & DEBAYER_CFG_LUMA_ONLY_MASK) << DEBAYER_CFG_LUMA_ONLY_SHIFT)
#define DEBAYER_FORCE_RB_ZERO(val)   ((val & DEBAYER_CFG_FORCE_RB_ZERO_MASK) << DEBAYER_CFG_FORCE_RB_ZERO_SHIFT)
#define DEBAYER_IP_DATAWIDTH(val)    ((val & DEBAYER_CFG_IP_DATAWIDTH_M1_MASK) << DEBAYER_CFG_IP_DATAWIDTH_M1_SHIFT)
#define DEBAYER_OP_DATAWIDTH(val)    ((val & DEBAYER_CFG_OP_DATAWIDTH_M1_MASK) << DEBAYER_CFG_OP_DATAWIDTH_M1_SHIFT)
#define DEBAYER_OUTPLANE_ORDER(val)  ((val & DEBAYER_CFG_OUTPLANE_ORDER_MASK) << DEBAYER_CFG_OUTPLANE_ORDER_SHIFT)
#define DEBAYER_NUM_OUTPLANES(val)   ((val & DEBAYER_CFG_NUM_OUTPLANES_MASK) << DEBAYER_CFG_NUM_OUTPLANES_SHIFT)
#define DEBAYER_GRAD_MULTIPLIER(val) ((val & DEBAYER_CFG_GRAD_MULTIPLIER_MASK) << DEBAYER_CFG_GRAD_MULTIPLIER_SHIFT)

#define DEBAYER_CFG(bayerOrder,lumaOnly,forceRBZero,inDataWidth,outDataWidth,outplaneOrder,numOutplanes,gradMultiplier) \
                                    DEBAYER_BAYER_ORDER(bayerOrder) +\
                                    DEBAYER_LUMA_ONLY(lumaOnly) +\
                                    DEBAYER_FORCE_RB_ZERO(forceRBZero) +\
                                    DEBAYER_IP_DATAWIDTH((inDataWidth ? inDataWidth - 0x1 : inDataWidth)) +\
                                    DEBAYER_OP_DATAWIDTH((outDataWidth ? outDataWidth - 0x1 : outDataWidth)) +\
                                    DEBAYER_OUTPLANE_ORDER(outplaneOrder) +\
                                    DEBAYER_NUM_OUTPLANES((numOutplanes ? numOutplanes - 0x1 : numOutplanes)) +\
                                    DEBAYER_GRAD_MULTIPLIER(gradMultiplier)

#define BAYER_ORDER_GRBG            DEBAYER_BAYER_ORDER(0x0)
#define BAYER_ORDER_RGGB            DEBAYER_BAYER_ORDER(0x1)
#define BAYER_ORDER_GBRG            DEBAYER_BAYER_ORDER(0x2)
#define BAYER_ORDER_BGGR            DEBAYER_BAYER_ORDER(0x3)

#define OUTPLANE_ORDER_RGB          DEBAYER_OUTPLANE_ORDER(0x0)
#define OUTPLANE_ORDER_BGR          DEBAYER_OUTPLANE_ORDER(0x1)
#define OUTPLANE_ORDER_RBG          DEBAYER_OUTPLANE_ORDER(0x2)
#define OUTPLANE_ORDER_BRG          DEBAYER_OUTPLANE_ORDER(0x3)
#define OUTPLANE_ORDER_GRB          DEBAYER_OUTPLANE_ORDER(0x4)
#define OUTPLANE_ORDER_GBR          DEBAYER_OUTPLANE_ORDER(0x5)

//// Threshold Register
#define DEBAYER_THRESHOLD_ABS_THRESH1_MASK      0x1FFF
#define DEBAYER_THRESHOLD_ABS_THRESH1_SHIFT     0x0
#define DEBAYER_THRESHOLD_ABS_THRESH2_MASK      0xFFF
#define DEBAYER_THRESHOLD_ABS_THRESH2_SHIFT     0xD
#define DEBAYER_THRESHOLD_RGB_EN_MASK           0x1
#define DEBAYER_THRESHOLD_RGB_EN_SHIFT          0x19
#define DEBAYER_THRESHOLD_LUMA_EN_MASK          0x1
#define DEBAYER_THRESHOLD_LUMA_EN_SHIFT         0x1A
#define DEBAYER_THRESHOLD_LUMA_WC_EN_MASK       0x1
#define DEBAYER_THRESHOLD_LUMA_WC_EN_SHIFT      0x1B
#define DEBAYER_THRESHOLD_PREVIEW_EN_MASK       0x1
#define DEBAYER_THRESHOLD_PREVIEW_EN_SHIFT      0x1C


#define DEBAYER_ABS_THRESH1(val)      ((val & DEBAYER_THRESHOLD_ABS_THRESH1_MASK) << DEBAYER_THRESHOLD_ABS_THRESH1_SHIFT)
#define DEBAYER_ABS_THRESH2(val)      ((val & DEBAYER_THRESHOLD_ABS_THRESH2_MASK) << DEBAYER_THRESHOLD_ABS_THRESH2_SHIFT)
#define DEBAYER_RGB_EN(val)           ((val & DEBAYER_THRESHOLD_RGB_EN_MASK) << DEBAYER_THRESHOLD_RGB_EN_SHIFT)
#define DEBAYER_LUMA_EN(val)          ((val & DEBAYER_THRESHOLD_LUMA_EN_MASK) << DEBAYER_THRESHOLD_LUMA_EN_SHIFT)
#define DEBAYER_LUMA_WC_EN(val)       ((val & DEBAYER_THRESHOLD_LUMA_WC_EN_MASK) << DEBAYER_THRESHOLD_LUMA_WC_EN_SHIFT)
#define DEBAYER_PREVIEW_EN(val)       ((val & DEBAYER_THRESHOLD_PREVIEW_EN_MASK) << DEBAYER_THRESHOLD_PREVIEW_EN_SHIFT)

//// Deworm Register
#define DEBAYER_DEWORM_OFFSET_MASK      0xFFFF
#define DEBAYER_DEWORM_OFFSET_SHIFT     0x10
#define DEBAYER_DEWORM_SLOPE_MASK       0xFFFF
#define DEBAYER_DEWORM_SLOPE_SHIFT      0x0

#define DEBAYER_OFFSET(val)      ((val & DEBAYER_DEWORM_OFFSET_MASK) << DEBAYER_DEWORM_OFFSET_SHIFT)
#define DEBAYER_SLOPE(val)       ((val & DEBAYER_DEWORM_SLOPE_MASK) << DEBAYER_DEWORM_SLOPE_SHIFT)

#define DEBAYER_THRESH(absThresh1,absThresh2,rgbEn,lumaEn,lumaWc,prewEn)   DEBAYER_ABS_THRESH1(absThresh1) |\
                                                                           DEBAYER_ABS_THRESH2(absThresh2)|\
                                                                           DEBAYER_RGB_EN(rgbEn)|\
                                                                           DEBAYER_LUMA_EN(lumaEn)|\
                                                                           DEBAYER_LUMA_WC_EN(lumaWc)|\
                                                                           DEBAYER_PREVIEW_EN(prewEn)

#define DEBAYER_DEWORM(offset,slope)            DEBAYER_OFFSET(offset) |\
                                                DEBAYER_SLOPE(slope)

#define DEBAYER_LUMA_RED_COEF_MASK                0xFF
#define DEBAYER_LUMA_RED_COEF_SHIFT               0x10
#define DEBAYER_LUMA_BLUE_COEF_MASK               0xFF
#define DEBAYER_LUMA_BLUE_COEF_SHIFT              0x00
#define DEBAYER_LUMA_GREEN_COEF_MASK              0xFF
#define DEBAYER_LUMA_GREEN_COEF_SHIFT             0x8

#define DEBAYER_LUMA_RED_COEF(val)   ((val & DEBAYER_LUMA_RED_COEF_MASK ) << DEBAYER_LUMA_RED_COEF_SHIFT)
#define DEBAYER_LUMA_BLUE_COEF(val)  ((val & DEBAYER_LUMA_BLUE_COEF_MASK ) << DEBAYER_LUMA_BLUE_COEF_SHIFT)
#define DEBAYER_LUMA_GREEN_COEF(val) ((val & DEBAYER_LUMA_GREEN_COEF_MASK ) << DEBAYER_LUMA_GREEN_COEF_SHIFT)

#define DEBAYER_LUMA_COEFS_CFG(red, green, blue) DEBAYER_LUMA_RED_COEF(red)|\
		                                         DEBAYER_LUMA_GREEN_COEF(green)|\
		                                         DEBAYER_LUMA_BLUE_COEF(blue)

/* Color Combination Filter */

#define CC_CFG_FORCE_LUMA_MASK             0x1
#define CC_CFG_FORCE_LUMA_SHIFT            0x0
#define CC_CFG_CHROMA_SUB_SAMP_MASK        0x3
#define CC_CFG_CHROMA_SUB_SAMP_SHIFT       0x1
#define CC_CFG_3DLUT_BYPASS_MASK           0x1
#define CC_CFG_3DLUT_BYPASS_SHIFT          0x3
#define CC_CFG_3DLUT_LOAD_ENABLE_MASK      0x1
#define CC_CFG_3DLUT_LOAD_ENABLE_SHIFT     0x4
#define CC_CFG_ENABLE_U12_OUT_MASK         0x1
#define CC_CFG_ENABLE_U12_OUT_SHIFT        0x5
#define CC_CFG_NUM_PLANES_MINUS_1_MASK     0x3
#define CC_CFG_NUM_PLANES_MINUS_1_SHIFT    0x18


#define CC_CFG_FORCE_LUMA(val)          ((val & CC_CFG_FORCE_LUMA_MASK  ) << CC_CFG_FORCE_LUMA_SHIFT )
#define CC_CFG_CHROMA_SUB_SAMP(val)     ((val & CC_CFG_CHROMA_SUB_SAMP_MASK) << CC_CFG_CHROMA_SUB_SAMP_SHIFT )
#define CC_CFG_3DLUT_BYPASS(val)        ((val & CC_CFG_3DLUT_BYPASS_MASK) <<  CC_CFG_3DLUT_BYPASS_SHIFT )
#define CC_CFG_3DLUT_LOAD_ENABLE(val)   ((val & CC_CFG_3DLUT_LOAD_ENABLE_MASK ) << CC_CFG_3DLUT_LOAD_ENABLE_SHIFT)
#define CC_CFG_ENABLE_U12_OUT(val)      ((val & CC_CFG_ENABLE_U12_OUT_MASK) << CC_CFG_ENABLE_U12_OUT_SHIFT)
#define CC_CFG_NUM_PLANES_MINUS_1(val)  ((val & CC_CFG_NUM_PLANES_MINUS_1_MASK) << CC_CFG_NUM_PLANES_MINUS_1_SHIFT)

#define CC_CFG(forceLuma1,chrSubSamp,threeDLutBypass,threeDLutLoadEn,u12OutEn,numPlanes) CC_CFG_FORCE_LUMA(forceLuma1) |\
                                                                                         CC_CFG_CHROMA_SUB_SAMP(chrSubSamp) |\
                                                                                         CC_CFG_3DLUT_BYPASS(threeDLutBypass) |\
                                                                                         CC_CFG_3DLUT_LOAD_ENABLE(threeDLutLoadEn)  |\
                                                                                         CC_CFG_ENABLE_U12_OUT(u12OutEn)  |\
                                                                                         CC_CFG_NUM_PLANES_MINUS_1((numPlanes-1))


/* Harris corner Filter */

#define HARRIS_CORNER_CFG_OUTPUT_DETERMINANT_MASK       0x1
#define HARRIS_CORNER_CFG_OUTPUT_DETERMINANT_SHIFT      0x4
#define HARRIS_CORNER_CFG_KERNEL_SIZE_MASK              0xF
#define HARRIS_CORNER_CFG_KERNEL_SIZE_SHIFT             0x0
#define HARISS_CORNER_CFG_EXP_SUBTRAHEND_MASK           0xFF
#define HARRIS_CORNER_CFG_EXP_SUBTRAHEND_SHIFT          0x8


#define HARRIS_CORNER_CFG_OUTPUT_DETERMINANT(val)    ((val & HARRIS_CORNER_CFG_OUTPUT_DETERMINANT_MASK) << HARRIS_CORNER_CFG_OUTPUT_DETERMINANT_SHIFT)
#define HARRIS_CORNER_CFG_KERNEL_SIZE(val)           ((val & HARRIS_CORNER_CFG_KERNEL_SIZE_MASK)        << HARRIS_CORNER_CFG_KERNEL_SIZE_SHIFT)
#define HARISS_CORNER_CFG_EXP_SUBTRAHEND(val)        ((val & HARISS_CORNER_CFG_EXP_SUBTRAHEND_MASK)     << HARRIS_CORNER_CFG_EXP_SUBTRAHEND_SHIFT)

#define HARRIS_CORNER_CFG(kns,outDet,expSubtrahend)  HARRIS_CORNER_CFG_KERNEL_SIZE(kns)|\
                                                     HARRIS_CORNER_CFG_OUTPUT_DETERMINANT(outDet)|\
                                                     HARISS_CORNER_CFG_EXP_SUBTRAHEND(expSubtrahend)


#define HARRIS_CORNER_KSZ_5            0x5
#define HARRIS_CORNER_KSZ_7            0x7
#define HARRIS_CORNER_KSZ_9            0x9

/* LSC Filter */

#define SIPP_LSC_FRM_DIM_HEIGHT_MASK        0xFFFF
#define SIPP_LSC_FRM_DIM_HEIGHT_SHIFT       0x10
#define SIPP_LSC_FRM_DIM_WIDTH_MASK         0xFFFF
#define SIPP_LSC_FRM_DIM_WIDTH_SHIFT        0x0

#define SIPP_LSC_FRM_DIM_HEIGHT_CFG(val) ((val & SIPP_LSC_FRM_DIM_HEIGHT_MASK ) << SIPP_LSC_FRM_DIM_HEIGHT_SHIFT)
#define SIPP_LSC_FRM_DIM_WIDTH_CFG(val) ((val & SIPP_LSC_FRM_DIM_WIDTH_MASK   ) << SIPP_LSC_FRM_DIM_WIDTH_SHIFT )

#define SIPP_LSC_FRM_DIM_CFG(height, width) SIPP_LSC_FRM_DIM_HEIGHT_CFG(height)|\
		                                    SIPP_LSC_FRM_DIM_HEIGHT_CFG(width)

#define SIPP_LSC_GM_DIM_HEIGHT_MASK    0x3FF
#define SIPP_LSC_GM_DIM_HEIGHT_SHIFT   0x10

#define SIPP_LSC_GM_DIM_WIDTH_MASK     0x3FF
#define SIPP_LSC_GM_DIM_WIDTH_SHIFT    0x0

#define SIPP_LSC_GM_DIM_HEIGHT_CFG(val) ((val &  SIPP_LSC_GM_DIM_HEIGHT_MASK) <<  SIPP_LSC_GM_DIM_HEIGHT_SHIFT)
#define SIPP_LSC_GM_DIM_WIDTH_CFG(val)  ((val &  SIPP_LSC_GM_DIM_WIDTH_MASK  ) <<  SIPP_LSC_GM_DIM_WIDTH_SHIFT)

#define SIPP_LSC_GM_DIM_CFG(height, width) SIPP_LSC_GM_DIM_HEIGHT_CFG(height)|\
                                           SIPP_LSC_GM_DIM_WIDTH_CFG(width)

#define SIPP_LSC_CFG_FORMAT_MASK      0x1
#define SIPP_LSC_CFG_FORMAT_SHIFT     0x0

#define SIPP_LSC_CFG_FORMAT(val) ((val & SIPP_LSC_CFG_FORMAT_MASK ) << SIPP_LSC_CFG_FORMAT_SHIFT)

#define SIPP_LSC_CFG_DATA_WIDTH_MASK         0xF
#define SIPP_LSC_CFG_DATA_WIDTH_SHIFT        0x4

#define SIPP_LSC_CFG_DATA_WIDTH(val) ((val & SIPP_LSC_CFG_DATA_WIDTH_MASK) << SIPP_LSC_CFG_DATA_WIDTH_SHIFT)


/* Median Filter */

#define MED_CFG_KERNEL_SIZE_MASK       0x7
#define MED_CFG_KERNEL_SIZE_SHIFT      0x0
#define MED_CFG_OUTPUT_SELECT_MASK     0x3F
#define MED_CFG_OUTPUT_SELECT_SHIFT    0x8
#define MED_CFG_THRESHOLD_MASK         0x1FF
#define MED_CFG_THRESHOLD_SHIFT        0x10
#define MED_CFG_ALPHA_BLEND_EN_MASK    0x1
#define MED_CFG_ALPHA_BLEND_EN_SHIFT   0x1C
#define MED_CFG_LUM_SUBSAMP_EN_MASK    0x1
#define MED_CFG_LUM_SUBSAMP_EN_SHIFT   0x1D
#define MED_CFG_GO_FAST_MASK           0x1
#define MED_CFG_GO_FAST_SHIFT          0x1E

#define MED_CFG_KERNEL_SIZE(val)       ((val & MED_CFG_KERNEL_SIZE_MASK)     << MED_CFG_KERNEL_SIZE_SHIFT)
#define MED_CFG_OUTPUT_SELECT(val)     ((val & MED_CFG_OUTPUT_SELECT_MASK)   << MED_CFG_OUTPUT_SELECT_SHIFT)
#define MED_CFG_THRESHOLD(val)         ((val & MED_CFG_THRESHOLD_MASK)       << MED_CFG_THRESHOLD_SHIFT)
#define MED_CFG_ALPHA_BLEND_EN(val)    ((val & MED_CFG_ALPHA_BLEND_EN_MASK)  << MED_CFG_ALPHA_BLEND_EN_SHIFT)
#define MED_CFG_LUM_SUBSAMP_EN(val)    ((val & MED_CFG_LUM_SUBSAMP_EN_MASK)  << MED_CFG_LUM_SUBSAMP_EN_SHIFT)
#define MED_CFG_GO_FAST(val)           ((val & MED_CFG_GO_FAST_MASK)         << MED_CFG_GO_FAST_SHIFT)


#define MED_CFG(ks,outSelect,thresh,alphaBlend,lumaSubSamp,goFast)   MED_CFG_KERNEL_SIZE(ks) |\
                                                                     MED_CFG_OUTPUT_SELECT(outSelect) |\
                                                                     MED_CFG_THRESHOLD(thresh) |\
                                                                     MED_CFG_ALPHA_BLEND_EN(alphaBlend) |\
                                                                     MED_CFG_LUM_SUBSAMP_EN(lumaSubSamp) |\
                                                                     MED_CFG_GO_FAST(goFast)

#define MED_LUMA_ALPHA_SLOPE_MASK       0xFF
#define MED_LUMA_ALPHA_SLOPE_SHIFT      0x8
#define MED_LUMA_ALPHA_OFFSET_MASK      0xFF
#define MED_LUMA_ALPHA_OFFSET_SHIFT     0x0

#define MED_LUMA_ALPHA_SLOPE(val)       ((val & MED_LUMA_ALPHA_SLOPE_MASK)     << MED_LUMA_ALPHA_SLOPE_SHIFT)
#define MED_LUMA_ALPHA_OFFSET(val)     ((val & MED_LUMA_ALPHA_OFFSET_MASK)   << MED_LUMA_ALPHA_OFFSET_SHIFT)

#define MED_LUMA_ALPHA(slope,offset)      MED_LUMA_ALPHA_SLOPE(slope) |\
                                          MED_LUMA_ALPHA_OFFSET(offset)

/* Edge Operator Filter */

#define EDGE_OPERATOR_MAGNITUDE_SCALE_FACTOR_MASK      0xFFFF
#define EDGE_OPERATOR_MAGNITUDE_SCALE_FACTOR_SHIFT     0x10
#define EDGE_OPERATOR_THETA_OVX_MASK                   0x1
#define EDGE_OPERATOR_THETA_OVX_SHIFT                  0x7
#define EDGE_OPERATOR_THETA_MODE_MASK                  0x3
#define EDGE_OPERATOR_THETA_MODE_SHIFT                 0x5
#define EDGE_OPERATOR_OUTPUT_MODE_MASK                 0x7
#define EDGE_OPERATOR_OUTPUT_MODE_SHIFT                0x2
#define EDGE_OPERATOR_INPUT_MODE_MASK                  0x2
#define EDGE_OPERATOR_INPUT_MODE_SHIFT                 0x0
//SIPP_HW_EDGE_OPERATOR_BUFFER_HEIGHT
#define EDGE_OPERATOR_BUFFER_HEIGHT_MASK               0xFFFF
#define EDGE_OPERATOR_BUFFER_HEIGHT_SHIFT              0x10
#define EDGE_OPERATOR_BUFFER_WIDTH_MASK                0xFFFF
#define EDGE_OPERATOR_BUFFER_WIDTH_SHIFT               0x0


#define EDGE_OPERATOR_MAG_SCALE_FACTOR(val)        ((val & EDGE_OPERATOR_MAGNITUDE_SCALE_FACTOR_MASK) << EDGE_OPERATOR_MAGNITUDE_SCALE_FACTOR_SHIFT)
#define EDGE_OPERATOR_THETA_OVX(val)               ((val & EDGE_OPERATOR_THETA_OVX_MASK)    << EDGE_OPERATOR_THETA_OVX_SHIFT    )
#define EDGE_OPERATOR_THETA_MODE(val)              ((val & EDGE_OPERATOR_THETA_MODE_MASK)   << EDGE_OPERATOR_THETA_MODE_SHIFT  )
#define EDGE_OPERATOR_OUTPUT_MODE(val)             ((val & EDGE_OPERATOR_OUTPUT_MODE_MASK)  << EDGE_OPERATOR_OUTPUT_MODE_SHIFT)
#define EDGE_OPERATOR_INPUT_MODE(val)              ((val & EDGE_OPERATOR_INPUT_MODE_MASK)   << EDGE_OPERATOR_INPUT_MODE_SHIFT )
#define SIPP_HW_EDGE_OPERATOR_BUFFER__HEIGHT(val)   ((val & EDGE_OPERATOR_BUFFER_HEIGHT_MASK) <<EDGE_OPERATOR_BUFFER_HEIGHT_shift)
#define SIPP_HW_EDGE_OPERATOR_BUFFER__WIDTH(val)    ((val & EDGE_OPERATOR_BUFFER_WIDTH_MASK)  <<EDGE_OPERATOR_BUFFER_WIDTH_SHIFT)

#define EDGE_OPERATOR_CFG(inMode,outMode,thetaMode,magScaleFactor, thetaScaleFactor)   EDGE_OPERATOR_INPUT_MODE(inMode) |\
                                                                                       EDGE_OPERATOR_OUTPUT_MODE(outMode) |\
                                                                                       EDGE_OPERATOR_THETA_MODE(thetaMode) |\
                                                                                       EDGE_OPERATOR_MAG_SCALE_FACTOR(magScaleFactor)|\
                                                                                       EDGE_OPERATOR_THETA_OVX (thetaScaleFactor)

#define EDGE_OPERATOR_XCOEFF_F_MASK     0x1F
#define EDGE_OPERATOR_XCOEFF_F_SHIFT    0x19
#define EDGE_OPERATOR_XCOEFF_E_MASK     0x1F
#define EDGE_OPERATOR_XCOEFF_E_SHIFT    0x14
#define EDGE_OPERATOR_XCOEFF_D_MASK     0x1F
#define EDGE_OPERATOR_XCOEFF_D_SHIFT    0xF
#define EDGE_OPERATOR_XCOEFF_B_MASK     0x1F
#define EDGE_OPERATOR_XCOEFF_B_SHIFT    0x5
#define EDGE_OPERATOR_XCOEFF_A_MASK     0x1F
#define EDGE_OPERATOR_XCOEFF_A_SHIFT    0x0
#define EDGE_OPERATOR_XCOEFF_C_MASK     0x1F
#define EDGE_OPERATOR_XCOEFF_C_SHIFT    0xA


#define EDGE_OPERATOR_XCOEFF_A(val)       ((val & EDGE_OPERATOR_XCOEFF_A_MASK) << EDGE_OPERATOR_XCOEFF_A_SHIFT)
#define EDGE_OPERATOR_XCOEFF_B(val)       ((val & EDGE_OPERATOR_XCOEFF_B_MASK) << EDGE_OPERATOR_XCOEFF_B_SHIFT)
#define EDGE_OPERATOR_XCOEFF_C(val)       ((val & EDGE_OPERATOR_XCOEFF_C_MASK) << EDGE_OPERATOR_XCOEFF_C_SHIFT)
#define EDGE_OPERATOR_XCOEFF_D(val)       ((val & EDGE_OPERATOR_XCOEFF_D_MASK) << EDGE_OPERATOR_XCOEFF_D_SHIFT)
#define EDGE_OPERATOR_XCOEFF_E(val)       ((val & EDGE_OPERATOR_XCOEFF_E_MASK) << EDGE_OPERATOR_XCOEFF_E_SHIFT)
#define EDGE_OPERATOR_XCOEFF_F(val)       ((val & EDGE_OPERATOR_XCOEFF_F_MASK) << EDGE_OPERATOR_XCOEFF_F_SHIFT)




#define EDGE_OPERATOR_YCOEF_A_MASK        0x1F
#define EDGE_OPERATOR_YCOEF_A_SHIFT       0x0
#define EDGE_OPERATOR_YCOEF_B_MASK        0x1F
#define EDGE_OPERATOR_YCOEF_B_SHIFT       0x5
#define EDGE_OPERATOR_YCOEF_C_MASK        0x1F
#define EDGE_OPERATOR_YCOEF_C_SHIFT       0xA
#define EDGE_OPERATOR_YCOEF_D_MASK        0x1F
#define EDGE_OPERATOR_YCOEF_D_SHIFT       0xF
#define EDGE_OPERATOR_YCOEF_E_MASK        0x1F
#define EDGE_OPERATOR_YCOEF_E_SHIFT       0x14
#define EDGE_OPERATOR_YCOEF_F_MASK        0x1F
#define EDGE_OPERATOR_YCOEF_F_SHIFT       0x19

#define EDGE_OPERATOR_YCOEF_A(val)       ((val & EDGE_OPERATOR_YCOEF_A_MASK) << EDGE_OPERATOR_YCOEF_A_SHIFT)
#define EDGE_OPERATOR_YCOEF_B(val)       ((val & EDGE_OPERATOR_YCOEF_B_MASK) << EDGE_OPERATOR_YCOEF_B_SHIFT)
#define EDGE_OPERATOR_YCOEF_C(val)       ((val & EDGE_OPERATOR_YCOEF_C_MASK) << EDGE_OPERATOR_YCOEF_C_SHIFT)
#define EDGE_OPERATOR_YCOEF_D(val)       ((val & EDGE_OPERATOR_YCOEF_D_MASK) << EDGE_OPERATOR_YCOEF_D_SHIFT)
#define EDGE_OPERATOR_YCOEF_E(val)       ((val & EDGE_OPERATOR_YCOEF_E_MASK) << EDGE_OPERATOR_YCOEF_E_SHIFT)
#define EDGE_OPERATOR_YCOEF_F(val)       ((val & EDGE_OPERATOR_YCOEF_F_MASK) << EDGE_OPERATOR_YCOEF_F_SHIFT)



#define EDGE_OPERATOR_XCOEFS(a,b,c,d,e,f)  EDGE_OPERATOR_XCOEFF_A(a) |\
                                           EDGE_OPERATOR_XCOEFF_B(b) |\
                                           EDGE_OPERATOR_XCOEFF_C(c) |\
                                           EDGE_OPERATOR_XCOEFF_D(d) |\
                                           EDGE_OPERATOR_XCOEFF_E(e) |\
                                           EDGE_OPERATOR_XCOEFF_F(f) \



#define EDGE_OPERATOR_YCOEFS(a,b,c,d,e,f)  EDGE_OPERATOR_YCOEF_A(a) |\
                                           EDGE_OPERATOR_YCOEF_B(b) |\
                                           EDGE_OPERATOR_YCOEF_C(c) |\
                                           EDGE_OPERATOR_YCOEF_D(d) |\
                                           EDGE_OPERATOR_YCOEF_E(e) |\
                                           EDGE_OPERATOR_YCOEF_F(f) \




/* Chroma Denoise Filter */

#define CHROMA_DNS_HORZ_ENABLE_MASK               0x7
#define CHROMA_DNS_HORZ_ENABLE_SHIFT              0x0
#define CHROMA_DNS_LIMIT_MASK                     0xFF
#define CHROMA_DNS_LIMIT_SHIFT                    0x4
#define CHROMA_DNS_FORCE_WEIGHTS_HORIZONTAL_MASK  0x1
#define CHROMA_DNS_FORCE_WEIGHTS_HORIZONTAL_SHIFT 0xC
#define CHROMA_DNS_FORCE_WEIGHTS_VERTICAL_MASK    0x1
#define CHROMA_DNS_FORCE_WEIGHTS_VERTICAL_SHIFT   0xD
#define CHROMA_DNS_PLANES_MODE_MASK               0x3
#define CHROMA_DNS_PLANES_MODE_SHIFT              0xE
#define CHROMA_DNS_DESAT_OFFSET_MASK              0xFF
#define CHROMA_DNS_DESAT_OFFSET_SHIFT             0x10
#define CHROMA_DNS_DESAT_SLOPE_MASK               0xFF
#define CHROMA_DNS_DESAT_SLOPE_SHIFT              0x18


#define CHROMA_DNS_HORZ_ENABLE(val)               ((val & CHROMA_DNS_HORZ_ENABLE_MASK) << CHROMA_DNS_HORZ_ENABLE_SHIFT )
#define CHROMA_DNS_LIMIT(val)                     ((val & CHROMA_DNS_LIMIT_MASK) << CHROMA_DNS_LIMIT_SHIFT)
#define CHROMA_DNS_FORCE_WEIGHTS_HORIZONTAL(val)  ((val & CHROMA_DNS_FORCE_WEIGHTS_HORIZONTAL_MASK) << CHROMA_DNS_FORCE_WEIGHTS_HORIZONTAL_SHIFT)
#define CHROMA_DNS_FORCE_WEIGHTS_VERTICAL(val)    ((val & CHROMA_DNS_FORCE_WEIGHTS_VERTICAL_MASK) << CHROMA_DNS_FORCE_WEIGHTS_VERTICAL_SHIFT)
#define CHROMA_DNS_PLANES_MODE(val)               ((val & CHROMA_DNS_PLANES_MODE_MASK) << CHROMA_DNS_PLANES_MODE_SHIFT )
#define CHROMA_DNS_DESAT_OFFSET(val)              ((val & CHROMA_DNS_DESAT_OFFSET_MASK) << CHROMA_DNS_DESAT_OFFSET_SHIFT )
#define CHROMA_DNS_DESAT_SLOPE(val)               ((val & CHROMA_DNS_DESAT_SLOPE_MASK) << CHROMA_DNS_DESAT_SLOPE_SHIFT )

#define CHROMA_DNS_CFG(horzEnable,limit,forceWghtsHor,forceWghtsVer,planesMode,desatOffset,desatSlope) CHROMA_DNS_HORZ_ENABLE(horzEnable) |\
                                                                                                       CHROMA_DNS_LIMIT(limit) |\
                                                                                                       CHROMA_DNS_FORCE_WEIGHTS_HORIZONTAL(forceWghtsHor) |\
                                                                                                       CHROMA_DNS_FORCE_WEIGHTS_VERTICAL(forceWghtsVer) |\
                                                                                                       CHROMA_DNS_PLANES_MODE(planesMode) |\
                                                                                                       CHROMA_DNS_DESAT_OFFSET(desatOffset) |\
                                                                                                       CHROMA_DNS_DESAT_SLOPE(desatSlope)

// 3x3 separable/symmetrical Gaussian Filter coeffs

#define CHROMA_DNS_COEFFS_CENTRE_MASK             0xFF
#define CHROMA_DNS_COEFFS_CENTRE_SHIFT            0x0
#define CHROMA_DNS_COEFFS_CENTRE_EDGE_MASK        0xFF
#define CHROMA_DNS_COEFFS_CENTRE_EDGE_SHIFT       0x8
#define CHROMA_DNS_COEFFS_CORNER_MASK             0xFF
#define CHROMA_DNS_COEFFS_CORNER_SHIFT            0x10

#define CHROMA_DNS_COEFFS_CENTRE_SET(val)               ((val & CHROMA_DNS_COEFFS_CENTRE_MASK) << CHROMA_DNS_COEFFS_CENTRE_SHIFT )
#define CHROMA_DNS_COEFFS_CENTRE_EDGE_SET(val)          ((val & CHROMA_DNS_COEFFS_CENTRE_EDGE_MASK) << CHROMA_DNS_COEFFS_CENTRE_EDGE_SHIFT )
#define CHROMA_DNS_COEFFS_CORNER_SET(val)               ((val & CHROMA_DNS_COEFFS_CORNER_MASK) << CHROMA_DNS_COEFFS_CORNER_SHIFT )

#define CHROMA_DNS_COEFFS_SET(centre,centreEdge,corner) CHROMA_DNS_COEFFS_CENTRE_SET(centre) |\
                                                        CHROMA_DNS_COEFFS_CENTRE_EDGE_SET(centreEdge) |\
                                                        CHROMA_DNS_COEFFS_CORNER_SET(corner)

/* Grey Point */
#define CHORMA_GREY_PT_CB_MASK                   0xFF
#define CHORMA_GREY_PT_CB_SHIFT                  0x0
#define CHORMA_GREY_PT_CG_MASK                   0xFF
#define CHORMA_GREY_PT_CG_SHIFT                  0x8
#define CHORMA_GREY_PT_CR_MASK                   0xFF
#define CHORMA_GREY_PT_CR_SHIFT                  0x10
#define CHORMA_GREY_PT_PASSTHRU_MASK             0xFF
#define CHORMA_GREY_PT_PASSTHRU_SHIFT            0x1F

#define CHORMA_GREY_PT_CB_SET(val)               ((val & CHORMA_GREY_PT_CB_MASK) << CHORMA_GREY_PT_CB_SHIFT)
#define CHORMA_GREY_PT_CG_SET(val)               ((val & CHORMA_GREY_PT_CG_MASK) << CHORMA_GREY_PT_CG_SHIFT)
#define CHORMA_GREY_PT_CR_SET(val)               ((val & CHORMA_GREY_PT_CR_MASK) << CHORMA_GREY_PT_CR_SHIFT)
#define CHORMA_GREY_PT_PASSTHRU_SET(val)         ((val & CHORMA_GREY_PT_PASSTHRU_MASK) << CHORMA_GREY_PT_PASSTHRU_SHIFT)

#define CHORMA_GREY_PT_SET(cb,cg,cr,passthru)    CHORMA_GREY_PT_CB_SET(cb) |\
                                                 CHORMA_GREY_PT_CG_SET(cg) |\
                                                 CHORMA_GREY_PT_CR_SET(cr) |\
                                                 CHORMA_GREY_PT_PASSTHRU_SET(passthru)

#define CHROMA_DNS_HOR_T1_MASK        0xFF
#define CHROMA_DNS_HOR_T1_SHIFT       0x0
#define CHROMA_DNS_HOR_T2_MASK        0xFF
#define CHROMA_DNS_HOR_T2_SHIFT       0x8
#define CHROMA_DNS_VER_T1_MASK        0xFF
#define CHROMA_DNS_VER_T1_SHIFT       0x10
#define CHROMA_DNS_VER_T2_MASK        0xFF
#define CHROMA_DNS_VER_T2_SHIFT       0x18
#define CHROMA_DNS_HOR_T3_MASK        0xFF
#define CHROMA_DNS_HOR_T3_SHIFT       0x0
#define CHROMA_DNS_VER_T3_MASK        0xFF
#define CHROMA_DNS_VER_T3_SHIFT       0x10

#define CHROMA_DNS_HOR_T1(val) (( val & CHROMA_DNS_HOR_T1_MASK) << CHROMA_DNS_HOR_T1_SHIFT)
#define CHROMA_DNS_HOR_T2(val) (( val & CHROMA_DNS_HOR_T2_MASK) << CHROMA_DNS_HOR_T2_SHIFT)
#define CHROMA_DNS_VER_T1(val) (( val & CHROMA_DNS_VER_T1_MASK) << CHROMA_DNS_VER_T1_SHIFT)
#define CHROMA_DNS_VER_T2(val) (( val & CHROMA_DNS_VER_T2_MASK) << CHROMA_DNS_VER_T2_SHIFT)
#define CHROMA_DNS_HOR_T3(val) (( val & CHROMA_DNS_HOR_T3_MASK) << CHROMA_DNS_HOR_T3_SHIFT)
#define CHROMA_DNS_VER_T3(val) (( val & CHROMA_DNS_VER_T3_MASK) << CHROMA_DNS_VER_T3_SHIFT)

#define CHROMA_DNS_CFG_THRESH12(horzThr1,horzThr2,vertThr1,vertThr2) CHROMA_DNS_VER_T2 (vertThr2) +\
                                                                     CHROMA_DNS_HOR_T2 (horzThr2) +\
                                                                     CHROMA_DNS_HOR_T1 (horzThr1)  +\
                                                                     CHROMA_DNS_VER_T1 (vertThr1)


#define CHROMA_DNS_CFG_THRESH3(horzThr3,vertThr3) CHROMA_DNS_HOR_T3 (horzThr3) +\
                                                  CHROMA_DNS_VER_T3 (vertThr3)

/* Luma Denoise Filter */

#define LUMA_DNS_CFG_C4_LUT_LOAD_EN_MASK     0x1
#define LUMA_DNS_CFG_C4_LUT_LOAD_EN_SHIFT  0x18
#define LUMA_DNS_CFG_BITSHIFT_MASK   0x1F
#define LUMA_DNS_CFG_BITSHIFT_SHIFT   0x10
#define LUMA_DNS_CFG_ALPHA_MASK    0xFF
#define LUMA_DNS_CFG_ALPHA_SHIFT   0x8
#define LUMA_DNS_CFG_BITPOS_MASK   0xF
#define LUMA_DNS_CFG_BITPOS_SHIFT   0x0

#define LUMA_DNS_CFG_C4_LUT_LOAD_EN(val) (( val & LUMA_DNS_CFG_C4_LUT_LOAD_EN_MASK   ) << LUMA_DNS_CFG_C4_LUT_LOAD_EN_SHIFT )
#define LUMA_DNS_CFG_BITSHIFT(val)       (( val & LUMA_DNS_CFG_BITSHIFT_MASK         ) << LUMA_DNS_CFG_BITSHIFT_SHIFT       )
#define LUMA_DNS_CFG_ALPHA(val)          (( val & LUMA_DNS_CFG_ALPHA_MASK            ) << LUMA_DNS_CFG_ALPHA_SHIFT          )
#define LUMA_DNS_CFG_BITPOS(val)         (( val & LUMA_DNS_CFG_BITPOS_MASK           ) << LUMA_DNS_CFG_BITPOS_SHIFT         )

#define LUMA_DNS_CFG(loadEn, bitShift, cfgAlpha, cfgBitpos) LUMA_DNS_CFG_C4_LUT_LOAD_EN( loadEn ) |\
                                                            LUMA_DNS_CFG_BITSHIFT( bitShift )    |\
                                                            LUMA_DNS_CFG_ALPHA( cfgAlpha )  |\
                                                            LUMA_DNS_CFG_BITPOS(cfgBitpos)
/*LUT Filter */


#define LUT_CFG_COLOR_CONVERSION_EN_MASK  0x1
#define LUT_CFG_COLOR_CONVERSION_EN_SHIFT  0x10
#define LUT_CFG_APB_ACCESS_EN_MASK   0x1
#define LUT_CFG_APB_ACCESS_EN_SHIFT   0xF
#define LUT_CFG_LUT_LOAD_EN_MASK   0x1
#define LUT_CFG_LUT_LOAD_EN_SHIFT   0xE
#define LUT_CFG_NUM_CHANNELS_MASK   0x3
#define LUT_CFG_NUM_CHANNELS_SHIFT   0xC
#define LUT_CFG_NUM_LUTS_MASK    0xF
#define LUT_CFG_NUM_LUTS_SHIFT    0x8
#define LUT_CFG_INTEGER_MODE_WIDTH_MASK   0x1F
#define LUT_CFG_INTEGER_MODE_WIDTH_SHIFT  0x3
#define LUT_CFG_CHANNEL_MODE_MASK   0x1
#define LUT_CFG_CHANNEL_MODE_SHIFT   0x1
#define LUT_CFG_INTERPOLATE_FP16_MASK   0x1
#define LUT_CFG_INTERPOLATE_FP16_SHIFT   0x0


#define LUT_CFG_COLOR_CONVERSION_EN(val)     (( val &  LUT_CFG_COLOR_CONVERSION_EN_MASK  ) << LUT_CFG_COLOR_CONVERSION_EN_SHIFT )
#define LUT_CFG_APB_ACCESS_EN(val)           (( val &  LUT_CFG_APB_ACCESS_EN_MASK        ) << LUT_CFG_APB_ACCESS_EN_SHIFT       )
#define LUT_CFG_LUT_LOAD_EN(val)             (( val &  LUT_CFG_LUT_LOAD_EN_MASK          ) << LUT_CFG_LUT_LOAD_EN_SHIFT         )
#define LUT_CFG_NUM_CHANNELS(val)            (( val &  LUT_CFG_NUM_CHANNELS_MASK         ) << LUT_CFG_NUM_CHANNELS_SHIFT        )
#define LUT_CFG_NUM_LUTS(val)                (( val &  LUT_CFG_NUM_LUTS_MASK             ) << LUT_CFG_NUM_LUTS_SHIFT            )
#define LUT_CFG_INTEGER_MODE_WIDTH(val)      (( val &  LUT_CFG_INTEGER_MODE_WIDTH_MASK   ) << LUT_CFG_INTEGER_MODE_WIDTH_SHIFT  )
#define LUT_CFG_CHANNEL_MODE(val)            (( val &  LUT_CFG_CHANNEL_MODE_MASK         ) << LUT_CFG_CHANNEL_MODE_SHIFT        )
#define LUT_CFG_INTERPOLATE_FP16(val)        (( val &  LUT_CFG_INTERPOLATE_FP16_MASK     )  << LUT_CFG_INTERPOLATE_FP16_SHIFT   )

#define LUT_CFG(colorConv, apbAccesEN, loadEn, numChanels, numLuts, intModew, channelMode,interpoalateFp16) LUT_CFG_COLOR_CONVERSION_EN(colorConv) |\
                                                                                                           LUT_CFG_APB_ACCESS_EN(apbAccesEN)|\
                                                                                                           LUT_CFG_LUT_LOAD_EN(loadEn)|\
                                                                                                           LUT_CFG_NUM_CHANNELS(numChanels)|\
                                                                                                           LUT_CFG_NUM_LUTS(numLuts)|\
                                                                                                           LUT_CFG_INTEGER_MODE_WIDTH(intModew)|\
                                                                                                           LUT_CFG_CHANNEL_MODE(channelMode)|\
                                                                                                           LUT_CFG_INTERPOLATE_FP16(interpoalateFp16)

#define LUT_REGION0_SIZE_INDEX_MASK    0xF
#define LUT_REGION0_SIZE_INDEX_SHIFT   0x0
#define LUT_REGION1_SIZE_INDEX_MASK    0xF
#define LUT_REGION1_SIZE_INDEX_SHIFT   0x4
#define LUT_REGION2_SIZE_INDEX_MASK    0xF
#define LUT_REGION2_SIZE_INDEX_SHIFT   0x8
#define LUT_REGION3_SIZE_INDEX_MASK    0xF
#define LUT_REGION3_SIZE_INDEX_SHIFT   0xC
#define LUT_REGION4_SIZE_INDEX_MASK    0xF
#define LUT_REGION4_SIZE_INDEX_SHIFT   0x10
#define LUT_REGION5_SIZE_INDEX_MASK    0xF
#define LUT_REGION5_SIZE_INDEX_SHIFT   0x14
#define LUT_REGION6_SIZE_INDEX_MASK    0xF
#define LUT_REGION6_SIZE_INDEX_SHIFT   0x18
#define LUT_REGION7_SIZE_INDEX_MASK    0xF
#define LUT_REGION7_SIZE_INDEX_SHIFT   0x1C

#define LUT_REGION0_SIZE_INDEX(val)    ((val & LUT_REGION0_SIZE_INDEX_MASK) << LUT_REGION0_SIZE_INDEX_SHIFT)
#define LUT_REGION1_SIZE_INDEX(val)    ((val & LUT_REGION1_SIZE_INDEX_MASK) << LUT_REGION1_SIZE_INDEX_SHIFT)
#define LUT_REGION2_SIZE_INDEX(val)    ((val & LUT_REGION2_SIZE_INDEX_MASK) << LUT_REGION2_SIZE_INDEX_SHIFT)
#define LUT_REGION3_SIZE_INDEX(val)    ((val & LUT_REGION3_SIZE_INDEX_MASK) << LUT_REGION3_SIZE_INDEX_SHIFT)
#define LUT_REGION4_SIZE_INDEX(val)    ((val & LUT_REGION4_SIZE_INDEX_MASK) << LUT_REGION4_SIZE_INDEX_SHIFT)
#define LUT_REGION5_SIZE_INDEX(val)    ((val & LUT_REGION5_SIZE_INDEX_MASK) << LUT_REGION5_SIZE_INDEX_SHIFT)
#define LUT_REGION6_SIZE_INDEX(val)    ((val & LUT_REGION6_SIZE_INDEX_MASK) << LUT_REGION6_SIZE_INDEX_SHIFT)
#define LUT_REGION7_SIZE_INDEX(val)    ((val & LUT_REGION7_SIZE_INDEX_MASK) << LUT_REGION7_SIZE_INDEX_SHIFT)



#define LUT_REGION8_SIZE_INDEX_MASK     0xF
#define LUT_REGION8_SIZE_INDEX_SHIFT    0x0
#define LUT_REGION9_SIZE_INDEX_MASK     0xF
#define LUT_REGION9_SIZE_INDEX_SHIFT    0x4
#define LUT_REGION10_SIZE_INDEX_MASK    0xF
#define LUT_REGION10_SIZE_INDEX_SHIFT   0x8
#define LUT_REGION11_SIZE_INDEX_MASK    0xF
#define LUT_REGION11_SIZE_INDEX_SHIFT   0xC
#define LUT_REGION12_SIZE_INDEX_MASK    0xF
#define LUT_REGION12_SIZE_INDEX_SHIFT   0x10
#define LUT_REGION13_SIZE_INDEX_MASK    0xF
#define LUT_REGION13_SIZE_INDEX_SHIFT   0x14
#define LUT_REGION14_SIZE_INDEX_MASK    0xF
#define LUT_REGION14_SIZE_INDEX_SHIFT   0x18
#define LUT_REGION15_SIZE_INDEX_MASK    0xF
#define LUT_REGION15_SIZE_INDEX_SHIFT   0x1C

#define LUT_REGION8_SIZE_INDEX(val)    ((val & LUT_REGION8_SIZE_INDEX_MASK) << LUT_REGION8_SIZE_INDEX_SHIFT)
#define LUT_REGION9_SIZE_INDEX(val)    ((val & LUT_REGION9_SIZE_INDEX_MASK) << LUT_REGION9_SIZE_INDEX_SHIFT)
#define LUT_REGION10_SIZE_INDEX(val)   ((val & LUT_REGION10_SIZE_INDEX_MASK) << LUT_REGION10_SIZE_INDEX_SHIFT)
#define LUT_REGION11_SIZE_INDEX(val)   ((val & LUT_REGION11_SIZE_INDEX_MASK) << LUT_REGION11_SIZE_INDEX_SHIFT)
#define LUT_REGION12_SIZE_INDEX(val)   ((val & LUT_REGION12_SIZE_INDEX_MASK) << LUT_REGION12_SIZE_INDEX_SHIFT)
#define LUT_REGION13_SIZE_INDEX(val)   ((val & LUT_REGION13_SIZE_INDEX_MASK) << LUT_REGION13_SIZE_INDEX_SHIFT)
#define LUT_REGION14_SIZE_INDEX(val)   ((val & LUT_REGION14_SIZE_INDEX_MASK) << LUT_REGION14_SIZE_INDEX_SHIFT)
#define LUT_REGION15_SIZE_INDEX(val)   ((val & LUT_REGION15_SIZE_INDEX_MASK) << LUT_REGION15_SIZE_INDEX_SHIFT)


#define LUT_REGION_0_TO_7_SIZE(szR0,szR1,szR2,szR3,szR4,szR5,szR6,szR7)   LUT_REGION0_SIZE_INDEX(szR0) |\
                                                                          LUT_REGION1_SIZE_INDEX(szR1) |\
                                                                          LUT_REGION2_SIZE_INDEX(szR2) |\
                                                                          LUT_REGION3_SIZE_INDEX(szR3) |\
                                                                          LUT_REGION4_SIZE_INDEX(szR4) |\
                                                                          LUT_REGION5_SIZE_INDEX(szR5) |\
                                                                          LUT_REGION6_SIZE_INDEX(szR6) |\
                                                                          LUT_REGION7_SIZE_INDEX(szR7)

#define LUT_REGION_8_TO_15_SIZE(szR8,szR9,szR10,szR11,szR12,szR13,szR14,szR15)   LUT_REGION8_SIZE_INDEX(szR8) |\
                                                                                 LUT_REGION9_SIZE_INDEX(szR9) |\
                                                                                 LUT_REGION10_SIZE_INDEX(szR10) |\
                                                                                 LUT_REGION11_SIZE_INDEX(szR11) |\
                                                                                 LUT_REGION12_SIZE_INDEX(szR12) |\
                                                                                 LUT_REGION13_SIZE_INDEX(szR13) |\
                                                                                 LUT_REGION14_SIZE_INDEX(szR14) |\
                                                                                 LUT_REGION15_SIZE_INDEX(szR15)
/*Raw filter*/


#define RAW_CFG_SDC_EN_MASK                 0x1
#define RAW_CFG_SDC_EN_SHIFT                0x1B
#define RAW_CFG_RGB_HIST_EN_MASK            0x1
#define RAW_CFG_RGB_HIST_EN_SHIFT           0x18

#define RAW_CFG_BADP_THRES_MASK             0xFF
#define RAW_CFG_BADP_THRES_SHIFT            0x10
#define RAW_CFG_AF_STATS_EN_MASK            0x1
#define RAW_CFG_AF_STATS_EN_SHIFT           0xD

#define RAW_CFG_GAIN_MODE_MASK              0x1
#define RAW_CFG_GAIN_MODE_SHIFT             0xC
#define RAW_CFG_DATA_WIDTH_MASK             0xF
#define RAW_CFG_DATA_WIDTH_SHIFT            0x8
#define RAW_CFG_LUMA_HIST_EN_MASK           0x1
#define RAW_CFG_LUMA_HIST_EN_SHIFT          0x7

#define RAW_CFG_AWB_STATS_EN_MASK           0x1
#define RAW_CFG_AWB_STATS_EN_SHIFT          0x6
#define RAW_CFG_GREEN_FIX_ONLY_MASK         0x1
#define RAW_CFG_GREEN_FIX_ONLY_SHIFT        0x5
#define RAW_CFG_BAD_PIXEL_FIX_EN_MASK       0x1
#define RAW_CFG_BAD_PIXEL_FIX_EN_SHIFT      0x4
#define RAW_CFG_GRGB_IMB_EN_MASK            0x1
#define RAW_CFG_GRGB_IMB_EN_SHIFT           0x3
#define RAW_CFG_BAYER_PATTERN_MASK          0x3
#define RAW_CFG_BAYER_PATTERN_SHIFT         0x1
#define RAW_CFG_FORMAT_MASK                 0x1
#define RAW_CFG_FORMAT_SHIFT                0x0

#define RAW_CFG_SDC_EN(val)           (( val & RAW_CFG_SDC_EN_MASK) << RAW_CFG_SDC_EN_SHIFT )
#define RAW_CFG_RGB_HIST_EN(val)      (( val & RAW_CFG_RGB_HIST_EN_MASK) << RAW_CFG_RGB_HIST_EN_SHIFT)
#define RAW_CFG_BADP_THRES(val)       (( val & RAW_CFG_BADP_THRES_MASK)  << RAW_CFG_BADP_THRES_SHIFT)
#define RAW_CFG_AF_STATS_EN(val)      (( val & RAW_CFG_AF_STATS_EN_MASK) << RAW_CFG_AF_STATS_EN_SHIFT)
#define RAW_CFG_GAIN_MODE(val)        (( val & RAW_CFG_GAIN_MODE_MASK)   << RAW_CFG_GAIN_MODE_SHIFT)
#define RAW_CFG_DATA_WIDTH(val)       (( val & RAW_CFG_DATA_WIDTH_MASK)  << RAW_CFG_DATA_WIDTH_SHIFT)
#define RAW_CFG_LUMA_HIST_EN(val)     (( val & RAW_CFG_LUMA_HIST_EN_MASK) << RAW_CFG_LUMA_HIST_EN_SHIFT)
#define RAW_CFG_AWB_STATS_EN(val)     (( val & RAW_CFG_AWB_STATS_EN_MASK) << RAW_CFG_AWB_STATS_EN_SHIFT)
#define RAW_CFG_GREEN_FIX_ONLY(val)   (( val & RAW_CFG_GREEN_FIX_ONLY_MASK) << RAW_CFG_GREEN_FIX_ONLY_SHIFT)
#define RAW_CFG_BAD_PIXEL_FIX_EN(val) (( val & RAW_CFG_BAD_PIXEL_FIX_EN_MASK) << RAW_CFG_BAD_PIXEL_FIX_EN_SHIFT)
#define RAW_CFG_GRGB_IMB_EN(val)      (( val & RAW_CFG_GRGB_IMB_EN_MASK) << RAW_CFG_GRGB_IMB_EN_SHIFT)
#define RAW_CFG_BAYER_PATTERN(val)    (( val & RAW_CFG_BAYER_PATTERN_MASK) << RAW_CFG_BAYER_PATTERN_SHIFT)
#define RAW_CFG_FORMAT(val)           (( val & RAW_CFG_FORMAT_MASK       ) << RAW_CFG_FORMAT_SHIFT)



#define RAW_CFG(format,bayerPatern,grgbImbEn,greenFixOnly,awbStatsEn,rgbHistEn,dataWidth,badPThresh,sdcEn,afStatsEn,gainMode,lumaHist,badPixelFixEn)\
                                                                            RAW_CFG_FORMAT(format)|\
                                                                            RAW_CFG_BAYER_PATTERN (bayerPatern)|\
                                                                            RAW_CFG_GRGB_IMB_EN (grgbImbEn) |\
                                                                            RAW_CFG_GREEN_FIX_ONLY (greenFixOnly)|\
                                                                            RAW_CFG_AWB_STATS_EN (awbStatsEn)|\
                                                                            RAW_CFG_RGB_HIST_EN (rgbHistEn)|\
                                                                            RAW_CFG_DATA_WIDTH (dataWidth)|\
                                                                            RAW_CFG_BADP_THRES (badPThresh)|\
                                                                            RAW_CFG_SDC_EN(sdcEn)|\
                                                                            RAW_CFG_AF_STATS_EN (afStatsEn)|\
                                                                            RAW_CFG_GAIN_MODE  (gainMode)|\
                                                                            RAW_CFG_LUMA_HIST_EN (lumaHist)|\
                                                                            RAW_CFG_BAD_PIXEL_FIX_EN (badPixelFixEn)


#define RAW_BAD_PIXEL_CFG_NOISE_LEVEL_MASK      0xFFFF
#define RAW_BAD_PIXEL_CFG_NOISE_LEVEL_SHIFT     0x10
#define RAW_BAD_PIXEL_CFG_ALPHA_G_HOT_MASK      0xF
#define RAW_BAD_PIXEL_CFG_ALPHA_G_HOT_SHIFT     0xC
#define RAW_BAD_PIXEL_CFG_APLHA_G_COLD_MASK     0xF
#define RAW_BAD_PIXEL_CFG_APLHA_G_COLD_SHIFT    0x8
#define RAW_BAD_PIXEL_CFG_ALPHA_RB_HOT_MASK     0xF
#define RAW_BAD_PIXEL_CFG_ALPHA_RB_HOT_SHIFT    0x4
#define RAW_BAD_PIXEL_CFG_ALPHA_RB_COLD_MASK    0xF
#define RAW_BAD_PIXEL_CFG_ALPHA_RB_COLD_SHIFT   0x0


#define RAW_BAD_PIXEL_CFG_NOISE_LEVEL(val)   (( val & RAW_BAD_PIXEL_CFG_NOISE_LEVEL_MASK) << RAW_BAD_PIXEL_CFG_NOISE_LEVEL_SHIFT )
#define RAW_BAD_PIXEL_CFG_ALPHA_G_HOT(val)   (( val & RAW_BAD_PIXEL_CFG_ALPHA_G_HOT_MASK) << RAW_BAD_PIXEL_CFG_ALPHA_G_HOT_SHIFT )
#define RAW_BAD_PIXEL_CFG_APLHA_G_COLD(val)  (( val & RAW_BAD_PIXEL_CFG_APLHA_G_COLD_MASK) << RAW_BAD_PIXEL_CFG_APLHA_G_COLD_SHIFT)
#define RAW_BAD_PIXEL_CFG_ALPHA_RB_HOT(val)  (( val & RAW_BAD_PIXEL_CFG_ALPHA_RB_HOT_MASK) << RAW_BAD_PIXEL_CFG_ALPHA_RB_HOT_SHIFT)
#define RAW_BAD_PIXEL_CFG_ALPHA_RB_COLD(val) (( val &  RAW_BAD_PIXEL_CFG_ALPHA_RB_COLD_MASK) <<  RAW_BAD_PIXEL_CFG_ALPHA_RB_COLD_SHIFT)

#define RAW_BAD_PIX_CFG(noiseLevel, alphaGHot, alphaGCold, alphaRbHot, alphaRbCold) RAW_BAD_PIXEL_CFG_NOISE_LEVEL(noiseLevel) |\
                                                                                    RAW_BAD_PIXEL_CFG_ALPHA_G_HOT(alphaGHot) |\
                                                                                    RAW_BAD_PIXEL_CFG_APLHA_G_COLD(alphaGCold) |\
                                                                                    RAW_BAD_PIXEL_CFG_ALPHA_RB_HOT(alphaRbHot) |\
                                                                                    RAW_BAD_PIXEL_CFG_ALPHA_RB_COLD(alphaRbCold)



#define RAW_GRGB_PLATO_DARK_MASK         0x3FFF
#define RAW_GRGB_PLATO_DARK_SHIFT        0x0
#define RAW_GRGB_PLATO_BRIGHT_MASK       0x3FFF
#define RAW_GRGB_PLATO_BRIGHT_SHIFT      0x10

#define RAW_GRGB_PLATO_DARK(val)         ((val & RAW_GRGB_PLATO_DARK_MASK) << RAW_GRGB_PLATO_DARK_SHIFT)
#define RAW_GRGB_PLATO_BRIGHT(val)       ((val & RAW_GRGB_PLATO_BRIGHT_MASK) << RAW_GRGB_PLATO_BRIGHT_SHIFT)

#define RAW_GRGB_PLATO(dark,bright)       RAW_GRGB_PLATO_DARK(dark) + \
                                          RAW_GRGB_PLATO_BRIGHT(bright)

#define RAW_GRGB_SLOPE_BRIGHT_MASK   0x3FFF
#define RAW_GRGB_SLOPE_BRIGHT_SHIFT  0x10
#define RAW_GRGB_SLOPE_DARK_MASK     0x3FFF
#define RAW_GRGB_SLOPE_DARK_SHIFT    0x0


#define RAW_GRGB_SLOPE_BRIGHT(val) ((val & RAW_GRGB_SLOPE_BRIGHT_MASK) << RAW_GRGB_SLOPE_BRIGHT_SHIFT )
#define RAW_GRGB_SLOPE_DARK(val)   (( val & RAW_GRGB_SLOPE_DARK_MASK) << RAW_GRGB_SLOPE_DARK_SHIFT )

#define RAW_GRGB_DECAY(slopeBright, slopeDark) RAW_GRGB_SLOPE_BRIGHT (slopeBright) +\
                                                    RAW_GRGB_SLOPE_DARK (slopeDark)






#define RAW_PATCH_CFG_NUM_HORIZ_PATCHES_MASK      0x3F
#define RAW_PATCH_CFG_NUM_HORIZ_PATCHES_SHIFT     0x0
#define RAW_PATCH_CFG_NUM_VERT_PATCHES_MASK       0x3F
#define RAW_PATCH_CFG_NUM_VERT_PATCHES_SHIFT      0x8
#define RAW_PATCH_CFG_PATCH_WIDTH_MASK            0xFF
#define RAW_PATCH_CFG_PATCH_WIDTH_SHIFT           0x10
#define RAW_PATCH_CFG_PATCH_HEIGHT_MASK           0xFF
#define RAW_PATCH_CFG_PATCH_HEIGHT_SHIFT          0x18

#define RAW_PATCH_CFG_NUM_HORIZ_PATCHES(val)     ((val & RAW_PATCH_CFG_NUM_HORIZ_PATCHES_MASK) << RAW_PATCH_CFG_NUM_HORIZ_PATCHES_SHIFT)
#define RAW_PATCH_CFG_NUM_VERT_PATCHES(val)      ((val & RAW_PATCH_CFG_NUM_VERT_PATCHES_MASK) << RAW_PATCH_CFG_NUM_VERT_PATCHES_SHIFT)
#define RAW_PATCH_CFG_PATCH_WIDTH(val)           ((val & RAW_PATCH_CFG_PATCH_WIDTH_MASK) << RAW_PATCH_CFG_PATCH_WIDTH_SHIFT)
#define RAW_PATCH_CFG_PATCH_HEIGHT(val)          ((val & RAW_PATCH_CFG_PATCH_HEIGHT_MASK) << RAW_PATCH_CFG_PATCH_HEIGHT_SHIFT)

#define RAW_PATCH_CFG(numHorizPatches,numVertPatches,patchWidth,patchHeight) \
                                                 RAW_PATCH_CFG_NUM_HORIZ_PATCHES((numHorizPatches ?  numHorizPatches - 0x1 : 0x0)) +\
                                                 RAW_PATCH_CFG_NUM_VERT_PATCHES((numVertPatches ?  numVertPatches - 0x1 : 0x0)) +\
                                                 RAW_PATCH_CFG_PATCH_WIDTH((patchWidth ?  patchWidth - 0x1 : 0x0)) +\
                                                 RAW_PATCH_CFG_PATCH_HEIGHT((patchHeight ?  patchHeight - 0x1 : 0x0))

#define RAW_PATCH_START_X_POS_MASK      0xFFFF
#define RAW_PATCH_START_X_POS_SHIFT     0x0
#define RAW_PATCH_START_Y_POS_MASK      0xFFFF
#define RAW_PATCH_START_Y_POS_SHIFT     0x10

#define RAW_PATCH_START_X_POS(val)     ((val & RAW_PATCH_START_X_POS_MASK) << RAW_PATCH_START_X_POS_SHIFT)
#define RAW_PATCH_START_Y_POS(val)     ((val & RAW_PATCH_START_Y_POS_MASK) << RAW_PATCH_START_Y_POS_SHIFT)

#define RAW_PATCH_START(xCoord,yCoord)  RAW_PATCH_START_X_POS(xCoord) + \
                                        RAW_PATCH_START_Y_POS(yCoord)


#define RAW_STATS_PLANES_PLANE0_MASK         0xF
#define RAW_STATS_PLANES_PLANE0_SHIFT        0x0
#define RAW_STATS_PLANES_PLANE1_MASK         0xF
#define RAW_STATS_PLANES_PLANE1_SHIFT        0x4
#define RAW_STATS_PLANES_PLANE2_MASK         0xF
#define RAW_STATS_PLANES_PLANE2_SHIFT        0x8
#define RAW_STATS_PLANES_PLANE3_MASK         0xF
#define RAW_STATS_PLANES_PLANE3_SHIFT        0xC
#define RAW_STATS_PLANES_HGRAM_PLANE_MASK    0xF
#define RAW_STATS_PLANES_HGRAM_PLANE_SHIFT   0x10
#define RAW_STATS_PLANES_ACTIVE_PLANES_MASK  0x3
#define RAW_STATS_PLANES_ACTIVE_PLANES_SHIFT 0x14

#define RAW_STATS_PLANES_PLANE0(val)         ((val & RAW_STATS_PLANES_PLANE0_MASK) << RAW_STATS_PLANES_PLANE0_SHIFT)
#define RAW_STATS_PLANES_PLANE1(val)         ((val & RAW_STATS_PLANES_PLANE1_MASK) << RAW_STATS_PLANES_PLANE1_SHIFT)
#define RAW_STATS_PLANES_PLANE2(val)         ((val & RAW_STATS_PLANES_PLANE2_MASK) << RAW_STATS_PLANES_PLANE2_SHIFT)
#define RAW_STATS_PLANES_PLANE3(val)         ((val & RAW_STATS_PLANES_PLANE3_MASK) << RAW_STATS_PLANES_PLANE3_SHIFT)
#define RAW_STATS_PLANES_HGRAM_PLANE(val)    ((val & RAW_STATS_PLANES_HGRAM_PLANE_MASK) << RAW_STATS_PLANES_HGRAM_PLANE_SHIFT)
#define RAW_STATS_PLANES_ACTIVE_PLANES(val)  (((val ? val-1 : val) & RAW_STATS_PLANES_ACTIVE_PLANES_MASK) << RAW_STATS_PLANES_ACTIVE_PLANES_SHIFT)

#define RAW_STATS_PLANES(plane0,plane1,plane2,plane3,histPlane,activePlanes)   RAW_STATS_PLANES_PLANE0(plane0) + \
                                                                               RAW_STATS_PLANES_PLANE1(plane1) + \
                                                                               RAW_STATS_PLANES_PLANE2(plane2) + \
                                                                               RAW_STATS_PLANES_PLANE3(plane3) + \
                                                                               RAW_STATS_PLANES_HGRAM_PLANE(histPlane) + \
                                                                               RAW_STATS_PLANES_ACTIVE_PLANES(activePlanes)




/* Poly-phase scaler filter */

//Input Frame dimension

#define POLY_FRM_HEIGHT_IN_MASK   0xFFFF
#define POLY_FRM_HEIGHT_IN_SHIFT  0x10
#define POLY_FRM_WIDTH_IN_MASK    0xFFFF
#define POLY_FRM_WIDTH_IN_SHIFT   0x0



#define POLY_FRM_HEIGHT_IN(val) (( val & POLY_FRM_HEIGHT_IN_MASK ) << POLY_FRM_HEIGHT_IN_SHIFT)
#define POLY_FRM_WIDTH_IN(val)  (( val & POLY_FRM_WIDTH_IN_MASK ) << POLY_FRM_WIDTH_IN_SHIFT)

#define POLY_CFG_DIM_IN (width, height) POLY_FRM_HEIGHT_IN (height) |\
                                        POLY_FRM_WIDTH_IN (width)



//Output Frame dimension
#define POLY_FRM_HEIGHT_OUT_MASK     0xFFFF
#define POLY_FRM_HEIGHT_OUT_SHIFT    0x10
#define POLY_FRM_WIDTH_OUT_MASK      0xFFFF
#define POLY_FRM_WIDTH_OUT_SHIFT     0x0


#define POLY_FRM_HEIGHT_OUT(val) (( val & POLY_FRM_HEIGHT_OUT_MASK ) << POLY_FRM_HEIGHT_OUT_SHIFT)
#define POLY_FRM_WIDTH_OUT(val)  (( val & POLY_FRM_WIDTH_OUT_MASK ) << POLY_FRM_WIDTH_OUT_SHIFT)

#define POLY_CFG_DIM_OUT (width, height) POLY_FRM_HEIGHT_OUT (height) |\
                                         POLY_FRM_WIDTH_OUT (width)


#define POLY_CFG_UPFIRDN_VSCALE_FACTOR_NUM_MASK        0x1F
#define POLY_CFG_UPFIRDN_VSCALE_FACTOR_NUM_SHIFT       0x16
#define POLY_CFG_UPFIRDN_VSCALE_FACTOR_DENOM_MASK      0x3F
#define POLY_CFG_UPFIRDN_VSCALE_FACTOR_DENOM_SHIFT     0x10
#define POLY_CFG_UPFIRDN_HSCALE_FACTOR_NUM_MASK        0x7FFF
#define POLY_CFG_UPFIRDN_HSCALE_FACTOR_NUM_SHIFT       0xA
#define POLY_CFG_UPFIRDN_HSCALE_FACTOR_DENOM_MASK      0x3F
#define POLY_CFG_UPFIRDN_HSCALE_FACTOR_DENOM_SHIFT     0x4
#define POLY_CFG_UPFIRDN_OUTPUT_CLAMP_MASK             0x1
#define POLY_CFG_UPFIRDN_OUTPUT_CLAMP_SHIFT            0x3
#define POLY_CFG_KERNEL_SIZE_MASK                      0x7
#define POLY_CFG_KERNEL_SIZE_SHIFT                     0x0

#define SIPP_HW_POLY_FIR_CLAMP_DISABLE  0x0
#define SIPP_HW_POLY_FIR_CLAMP_ENABLE   0x1
#define SIPP_UPFIRDN_ID                 15


#define POLY_CFG_UPFIRDN_VSCALE_FACTOR_NUM(val)   ((val  & POLY_CFG_UPFIRDN_VSCALE_FACTOR_NUM_MASK   ) << POLY_CFG_UPFIRDN_VSCALE_FACTOR_NUM_SHIFT   )
#define POLY_CFG_UPFIRDN_VSCALE_FACTOR_DENOM(val) ((val  & POLY_CFG_UPFIRDN_VSCALE_FACTOR_DENOM_MASK ) << POLY_CFG_UPFIRDN_VSCALE_FACTOR_DENOM_SHIFT )
#define POLY_CFG_UPFIRDN_HSCALE_FACTOR_NUM(val)   (( val & POLY_CFG_UPFIRDN_HSCALE_FACTOR_NUM_MASK   ) << POLY_CFG_UPFIRDN_HSCALE_FACTOR_NUM_SHIFT   )
#define POLY_CFG_UPFIRDN_HSCALE_FACTOR_DENOM(val) (( val & POLY_CFG_UPFIRDN_HSCALE_FACTOR_DENOM_MASK ) << POLY_CFG_UPFIRDN_HSCALE_FACTOR_DENOM_SHIFT )
#define POLY_CFG_UPFIRDN_OUTPUT_CLAMP(val)        (( val & POLY_CFG_UPFIRDN_OUTPUT_CLAMP_MASK        ) << POLY_CFG_UPFIRDN_OUTPUT_CLAMP_SHIFT        )
#define POLY_CFG_KERNEL_SIZE(val)                 (( val & POLY_CFG_KERNEL_SIZE_MASK                 ) << POLY_CFG_KERNEL_SIZE_SHIFT                 )


#define POLY_CFG (vscaleFactorNum, vscaleFactorDnom, hscaleFactorNum, hscaleFactorDnom, outClamp, kernelSize)  POLY_CFG_UPFIRDN_VSCALE_FACTOR_NUM (vscaleFactorNum) |\
                                                                                                               POLY_CFG_UPFIRDN_VSCALE_FACTOR_DENOM (vscaleFactorDnom) |\
                                                                                                               POLY_CFG_UPFIRDN_HSCALE_FACTOR_NUM (hscaleFactorNum) |\
                                                                                                               POLY_CFG_UPFIRDN_HSCALE_FACTOR_DENOM (hscaleFactorDnom)|\
                                                                                                               POLY_CFG_UPFIRDN_OUTPUT_CLAMP (outClamp) |\
                                                                                                               POLY_CFG_KERNEL_SIZE  (kernelSize)


#define POLY_VERTICAL_COEFF0_MASK  0xFF
#define POLY_VERTICAL_COEFF0_SHIFT  0x0
#define POLY_VERTICAL_COEFF1_MASK  0xFF
#define POLY_VERTICAL_COEFF1_SHIFT  0x8
#define POLY_VERTICAL_COEFF2_MASK  0xFF
#define POLY_VERTICAL_COEFF2_SHIFT  0x10
#define POLY_VERTICAL_COEFF3_MASK  0xFF
#define POLY_VERTICAL_COEFF3_SHIFT  0x18

#define POLY_VERTICAL_COEFF4_MASK  0xFF
#define POLY_VERTICAL_COEFF4_SHIFT  0x0
#define POLY_VERTICAL_COEFF5_MASK  0xFF
#define POLY_VERTICAL_COEFF5_SHIFT  0x10
#define POLY_VERTICAL_COEFF6_MASK  0xFF
#define POLY_VERTICAL_COEFF6_SHIFT  0x10

#define POLY_VERTICAL_COEFF0 ( val & POLY_VERTICAL_COEFF0_MASK) << POLY_VERTICAL_COEFF0_SHIFT)
#define POLY_VERTICAL_COEFF1 ( val & POLY_VERTICAL_COEFF1_MASK) << POLY_VERTICAL_COEFF1_SHIFT)
#define POLY_VERTICAL_COEFF2 ( val & POLY_VERTICAL_COEFF2_MASK) << POLY_VERTICAL_COEFF2_SHIFT)
#define POLY_VERTICAL_COEFF3 ( val & POLY_VERTICAL_COEFF3_MASK) << POLY_VERTICAL_COEFF3_SHIFT)

#define POLY_CFG_VERTICAL_COEFFS_0_TO_3 (val0, val1,val2, val3) POLY_VERTICAL_COEFF0 (val0) |\
                                                                POLY_VERTICAL_COEFF1 (val1) |\
                                                                POLY_VERTICAL_COEFF2 (val2) |\
                                                                POLY_VERTICAL_COEFF3 (val3)


#define POLY_CFG_VERTICAL_COEFFS_4_TO_6 (val4, val5, val6) POLY_VERTICAL_COEFF4 (val4) |\
                                                           POLY_VERTICAL_COEFF5 (val5) |\
                                                           POLY_VERTICAL_COEFF6 (val6)



#define POLY_HORIZONTAL_COEFF0_MASK  0xFF
#define POLY_HORIZONTAL_COEFF0_SHIFT  0x0
#define POLY_HORIZONTAL_COEFF1_MASK  0xFF
#define POLY_HORIZONTAL_COEFF1_SHIFT  0x8
#define POLY_HORIZONTAL_COEFF2_MASK  0xFF
#define POLY_HORIZONTAL_COEFF2_SHIFT  0x10
#define POLY_HORIZONTAL_COEFF3_MASK  0xFF
#define POLY_HORIZONTAL_COEFF3_SHIFT  0x18

#define POLY_HORIZONTAL_COEFF4_MASK  0xFF
#define POLY_HORIZONTAL_COEFF4_SHIFT  0x0
#define POLY_HORIZONTAL_COEFF5_MASK  0xFF
#define POLY_HORIZONTAL_COEFF5_SHIFT  0x10
#define POLY_HORIZONTAL_COEFF6_MASK  0xFF
#define POLY_HORIZONTAL_COEFF6_SHIFT  0x10

#define POLY_HORIZONTAL_COEFF0 ( val & POLY_HORIZONTAL_COEFF0_MASK) << POLY_HORIZONTAL_COEFF0_SHIFT)
#define POLY_HORIZONTAL_COEFF1 ( val & POLY_HORIZONTAL_COEFF1_MASK) << POLY_HORIZONTAL_COEFF1_SHIFT)
#define POLY_HORIZONTAL_COEFF2 ( val & POLY_HORIZONTAL_COEFF2_MASK) << POLY_HORIZONTAL_COEFF2_SHIFT)
#define POLY_HORIZONTAL_COEFF3 ( val & POLY_HORIZONTAL_COEFF3_MASK) << POLY_HORIZONTAL_COEFF3_SHIFT)

#define POLY_CFG_HORIZONTAL_COEFFS_0_TO_3 (val0, val1,val2, val3) POLY_HORIZONTAL_COEFF0 (val0) |\
                                                                  POLY_HORIZONTAL_COEFF1 (val1) |\
                                                                  POLY_HORIZONTAL_COEFF2 (val2) |\
                                                                  POLY_HORIZONTAL_COEFF3 (val3)


#define POLY_CFG_HORIZONTAL_COEFFS_4_TO_6 (val4, val5, val6) POLY_VERICAL_COEFF4 (val4) |\
                                                             POLY_HORIZONTAL_COEFF5 (val5) |\
                                                             POLY_HORIZONTAL_COEFF6 (val6)



/*MIPI Rx Filter */

#define MIPI_RX_OUT_FRM_HEIGHT_MASK    0xFFFF
#define MIPI_RX_OUT_FRM_HEIGHT_SHIFT  0x10
#define MIPI_RX_OUT_FRM_WIDTH_MASK    0xFFFF
#define MIPI_RX_OUT_FRM_WIDTH_SHIFT  0x0

#define MIPI_RX_OUT_FRM_HEIGHT(val) ((MIPI_RX_OUT_FRM_HEIGHT_MASK) << MIPI_RX_OUT_FRM_HEIGHT_SHIFT)
#define MIPI_RX_OUT_FRM_WIDTH(val)  ((MIPI_RX_OUT_FRM_WIDTH_MASK) << MIPI_RX_OUT_FRM_WIDTH_SHIFT)

#define MIPI_RX_CFG_OUT_FRM_DIM (width, height) MIPI_RX_OUT_FRM_WIDTH  (width) |\
                                                MIPI_RX_OUT_FRM_HEIGHT (height)


#define MIPI_RX_CFG_INPUT_BIT_DEPTH_MASK  0xF
#define MIPI_RX_CFG_INPUT_BIT_DEPTH_SHIFT  0x1C
#define MIPI_RX_CFG_PROMOTE_MASK   0x1
#define MIPI_RX_CFG_PROMOTE_SHIFT   0x1A
#define MIPI_RX_CFG_USE_PRIVATE_LCS_MASK  0x1
#define MIPI_RX_CFG_USE_PRIVATE_SHIFT   0x19
#define MIPI_RX_CFG_PACK_BUFFER_MASK   0x1
#define MIPI_RX_CFG_PACK_BUFFER_SHIFT   0x13
#define MIPI_RX_CFG_BAYER_MODE_MASK   0x1
#define MIPI_RX_CFG_BAYER_MODE_SHIFT   0x11
#define MIPI_RX_FMT_CONV_RS_MASK   0x1F
#define MIPI_RX_FMT_CONV_RS_SHIFT   0xC
#define MIPI_RX_FMT_CONV_EN_MASK   0xF
#define MIPI_RX_FMT_CONV_EN_SHIFT                       0x8

#define MIPI_RX_CFG_INPUT_BIT_DEPTH(val)  (( val & MIPI_RX_CFG_INPUT_BIT_DEPTH_MASK) << MIPI_RX_CFG_INPUT_BIT_DEPTH_SHIFT    )
#define MIPI_RX_CFG_PROMOTE(val)          (( val & MIPI_RX_CFG_PROMOTE_MASK)         << MIPI_RX_CFG_PROMOTE_SHIFT            )
#define MIPI_RX_CFG_USE_PRIVATE_LCS(val)  (( val & MIPI_RX_CFG_USE_PRIVATE_LCS_MASK) << MIPI_RX_CFG_USE_PRIVATE_LCS_SHIFT    )
#define MIPI_RX_CFG_PACK_BUFFER(val)      (( val & MIPI_RX_CFG_PACK_BUFFER_MASK)     << MIPI_RX_CFG_PACK_BUFFER_MASK_SHIFT   )
#define MIPI_RX_CFG_BAYER_MODE(val)       (( val & MIPI_RX_CFG_BAYER_MODE_MASK)      << MIPI_RX_CFG_BAYER_MODE_MASK_SHIFT    )
#define MIPI_RX_FMT_CONV_RS(val)          (( val & MIPI_RX_FMT_CONV_RS_MASK)         << MIPI_RX_FMT_CONV_RS_SHIFT            )
#define MIPI_RX_FMT_CONV_EN(val)          (( val & MIPI_RX_FMT_CONV_EN_MASK)         << MIPI_RX_FMT_CONV_EN_SHIFT            )

#define MIPI_RX_CFG(inBitDepth, promote, usePrivateLcs, packBuffer, bayerMode, convRs, convEn)  MIPI_RX_CFG_INPUT_BIT_DEPTH(inBitDepth) |\
                                                                                                MIPI_RX_CFG_PROMOTE(promote) |\
                                                                                                MIPI_RX_CFG_USE_PRIVATE_LCS(usePrivateLcs) |\
                                                                                                MIPI_RX_CFG_PACK_BUFFER(packBuffer) |\  MIPI_RX_CFG_BAYER_MODE(bayerMode) |\
                                                                                                MIPI_RX_FMT_CONV_RS(convRs) |\
                                                                                                MIPI_RX_FMT_CONV_EN(convEn)



#define MIPI_RX_WINDOW0_HORIZONTAL_WIDTH_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW0_HORIZONTAL_WIDTH_CFG_SHIFT 0x10
#define MIPI_RX_WINDOW0_VERTICAL_HEIGHT_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW0_VERTICAL_HEIGHT_CFG_SHIFT 0x10

#define MIPI_RX_WINDOW0_HORIZONTAL_START_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW0_HORIZONTAL_START_CFG_SHIFT 0x0
#define MIPI_RX_WINDOW0_VERTICAL_START_CFG_MASK         0xFFFF
#define MIPI_RX_WINDOW0_VERTICAL_START_CFG_SHIFT 0x0

#define MIPI_RX_WINDOW0_HORIZONTAL_WIDTH_CFG(val) (( val & MIPI_RX_WINDOW0_HORIZONTAL_WIDTH_CFG_MASK) << MIPI_RX_WINDOW0_HORIZONTAL_WIDTH_CFG_SHIFT)
#define MIPI_RX_WINDOW0_VERTICAL_WIDTH_CFG(val)   (( val & MIPI_RX_WINDOW0_VERTICAL_WIDTH_CFG_MASK  ) << MIPI_RX_WINDOW0_VERICAL_WIDTH_CFG_SHIFT   )

#define MIPI_RX_WINDOW0_HORIZONTAL_START_CFG(val) (( val & MIPI_RX_WINDOW0_HORIZONTAL_START_CFG_MASK) << MIPI_RX_WINDOW0_HORIZONTAL_START_CFG_SHIFT)
#define MIPI_RX_WINDOW0_VERTICAL_START_CFG(val)   (( val & MIPI_RX_WINDOW0_VERTICAL_START_CFG_MASK) << MIPI_RX_WINDOW0_VERTICAL_START_CFG_SHIFT    )

#define MIPI_RX_WINDOW1_HORIZONTAL_WIDTH_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW1_HORIZONTAL_WIDTH_CFG_SHIFT 0x10
#define MIPI_RX_WINDOW1_VERTICAL_HEIGHT_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW1_VERTICAL_HEIGHT_CFG_SHIFT 0x10

#define MIPI_RX_WINDOW1_HORIZONTAL_START_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW1_HORIZONTAL_START_CFG_SHIFT 0x0
#define MIPI_RX_WINDOW1_VERTICAL_START_CFG_MASK         0xFFFF
#define MIPI_RX_WINDOW1_VERTICAL_START_CFG_SHIFT 0x0

#define MIPI_RX_WINDOW1_HORIZONTAL_WIDTH_CFG(val) (( val & MIPI_RX_WINDOW1_HORIZONTAL_WIDTH_CFG_MASK) << MIPI_RX_WINDOW1_HORIZONTAL_WIDTH_CFG_SHIFT)
#define MIPI_RX_WINDOW1_VERTICAL_WIDTH_CFG(val)   (( val & MIPI_RX_WINDOW1_VERTICAL_WIDTH_CFG_MASK  ) << MIPI_RX_WINDOW1_VERICAL_WIDTH_CFG_SHIFT   )

#define MIPI_RX_WINDOW1_HORIZONTAL_START_CFG(val) (( val & MIPI_RX_WINDOW1_HORIZONTAL_START_CFG_MASK) << MIPI_RX_WINDOW1_HORIZONTAL_START_CFG_SHIFT)
#define MIPI_RX_WINDOW1_VERTICAL_START_CFG(val)   (( val & MIPI_RX_WINDOW1_VERTICAL_START_CFG_MASK) << MIPI_RX_WINDOW1_VERTICAL_START_CFG_SHIFT    )


#define MIPI_RX_WINDOW2_HORIZONTAL_WIDTH_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW2_HORIZONTAL_WIDTH_CFG_SHIFT 0x10
#define MIPI_RX_WINDOW2_VERTICAL_HEIGHT_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW2_VERTICAL_HEIGHT_CFG_SHIFT 0x10

#define MIPI_RX_WINDOW2_HORIZONTAL_START_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW2_HORIZONTAL_START_CFG_SHIFT 0x0
#define MIPI_RX_WINDOW2_VERTICAL_START_CFG_MASK         0xFFFF
#define MIPI_RX_WINDOW2_VERTICAL_START_CFG_SHIFT 0x0

#define MIPI_RX_WINDOW2_HORIZONTAL_WIDTH_CFG(val) (( val & MIPI_RX_WINDOW2_HORIZONTAL_WIDTH_CFG_MASK) << MIPI_RX_WINDOW2_HORIZONTAL_WIDTH_CFG_SHIFT)
#define MIPI_RX_WINDOW2_VERTICAL_WIDTH_CFG(val)   (( val & MIPI_RX_WINDOW2_VERTICAL_WIDTH_CFG_MASK  ) << MIPI_RX_WINDOW2_VERICAL_WIDTH_CFG_SHIFT   )

#define MIPI_RX_WINDOW2_HORIZONTAL_START_CFG(val) (( val & MIPI_RX_WINDOW2_HORIZONTAL_START_CFG_MASK) << MIPI_RX_WINDOW2_HORIZONTAL_START_CFG_SHIFT)
#define MIPI_RX_WINDOW2_VERTICAL_START_CFG(val)   (( val & MIPI_RX_WINDOW2_VERTICAL_START_CFG_MASK) << MIPI_RX_WINDOW2_VERTICAL_START_CFG_SHIFT    )


#define MIPI_RX_WINDOW3_HORIZONTAL_WIDTH_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW3_HORIZONTAL_WIDTH_CFG_SHIFT 0x10
#define MIPI_RX_WINDOW3_VERTICAL_HEIGHT_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW3_VERTICAL_HEIGHT_CFG_SHIFT 0x10

#define MIPI_RX_WINDOW3_HORIZONTAL_START_CFG_MASK 0xFFFF
#define MIPI_RX_WINDOW3_HORIZONTAL_START_CFG_SHIFT 0x0
#define MIPI_RX_WINDOW3_VERTICAL_START_CFG_MASK         0xFFFF
#define MIPI_RX_WINDOW3_VERTICAL_START_CFG_SHIFT 0x0

#define MIPI_RX_WINDOW3_HORIZONTAL_WIDTH_CFG(val) (( val & MIPI_RX_WINDOW3_HORIZONTAL_WIDTH_CFG_MASK) << MIPI_RX_WINDOW3_HORIZONTAL_WIDTH_CFG_SHIFT)
#define MIPI_RX_WINDOW3_VERTICAL_WIDTH_CFG(val)   (( val & MIPI_RX_WINDOW3_VERTICAL_WIDTH_CFG_MASK  ) << MIPI_RX_WINDOW3_VERICAL_WIDTH_CFG_SHIFT   )

#define MIPI_RX_WINDOW3_HORIZONTAL_START_CFG(val) (( val & MIPI_RX_WINDOW3_HORIZONTAL_START_CFG_MASK) << MIPI_RX_WINDOW3_HORIZONTAL_START_CFG_SHIFT)
#define MIPI_RX_WINDOW3_VERTICAL_START_CFG(val)   (( val & MIPI_RX_WINDOW3_VERTICAL_START_CFG_MASK) << MIPI_RX_WINDOW3_VERTICAL_START_CFG_SHIFT    )


#define MIPI_RX_WINDOW0_SELECTION_ENABLE_MASK  0xF
#define MIPI_RX_WINDOW0_SELECTION_ENABLE_SHIFT  0x8
#define MIPI_RX_WINDOW0_LEAST_SEGNIFICANT_BIT_MASK 0x1F
#define MIPI_RX_WINDOW0_LEAST_SEGNIFICANT_BIT_SHIFT     0x0


#define MIPI_RX_WINDOW1_SELECTION_ENABLE_MASK  0xF
#define MIPI_RX_WINDOW1_SELECTION_ENABLE_SHIFT  0x18
#define MIPI_RX_WINDOW1_LEAST_SEGNIFICANT_BIT_MASK 0x1F
#define MIPI_RX_WINDOW1_LEAST_SEGNIFICANT_BIT_SHIFT     0xF

#define MIPI_RX_WINDOW2_SELECTION_ENABLE_MASK  0xF
#define MIPI_RX_WINDOW2_SELECTION_ENABLE_SHIFT  0x8
#define MIPI_RX_WINDOW2_LEAST_SEGNIFICANT_BIT_MASK 0x1F
#define MIPI_RX_WINDOW2_LEAST_SEGNIFICANT_BIT_SHIFT     0x0

#define MIPI_RX_WINDOW3_SELECTION_ENABLE_MASK  0xF
#define MIPI_RX_WINDOW3_SELECTION_ENABLE_SHIFT  0x18
#define MIPI_RX_WINDOW3_LEAST_SEGNIFICANT_BIT_MASK 0x1F
#define MIPI_RX_WINDOW3_LEAST_SEGNIFICANT_BIT_SHIFT     0xF




/*MIPI Tx Filter */

#define MIPI_TX_FRM_HEIGHT_MASK   0xFFFF
#define MIPI_TX_FRM_HEIGHT_SHIFT  0x10
#define MIPI_TX_FRM_WIDTH_MASK   0xFFFF
#define MIPI_TX_FRM_WIDTH_SHIFT          0x0

#define MIPI_TX_FRM_HEIGHT(val)  (( val & MIPI_TX_FRM_HEIGHT_MASK) << MIPI_TX_FRM_HEIGHT_SHIFT )
#define MIPI_TX_FRM_WIDTH(val)   (( val & MIPI_TX_FRM_WIDTH_MASK)  << MIPI_TX_FRM_WIDTH_SHIFT  )

#define MIPI_TX_FRM_CFG (width, height) MIPI_TX_FRM_HEIGHT (height)|\
                                        MIPI_TX_FRM_WIDTH (width)

#define MIPI_TX_CFG_FRONTPORCH_MASK    0x1
#define MIPI_TX_CFG_FRONTPORCH_SHIFT   0x9
#define MIPI_TX_CFG_BACKPORCH_MASK   0x1
#define MIPI_TX_CFG_BACKPORCH_SHIFT   0x8
#define MIPI_TX_INT_SEL_MASK    0x3
#define MIPI_TX_INT_SEL_SHIFT    0x6
#define MIPI_TX_SYNC_IDLE_MASK    0x1
#define MIPI_TX_SYNC_IDLE_SHIFT    0x3
#define MIPI_TX_ONESHOT_MODE_MASK   0x1
#define MIPI_TX_ONESHOT_MODE_SHIFT   0x2
#define MIPI_TX_FIRST_FIELD_MASK   0x1
#define MIPI_TX_FIRST_FIELD_SHIFT   0x1
#define MIPI_TX_SCAN_MODE_MASK    0x1
#define MIPI_TX_SCAN_MODE_SHIFT    0x0


#define MIPI_TX_CFG_FRONTPORCH(val) (( val & MIPI_TX_CFG_FRONTPORCH_MASK) << MIPI_TX_CFG_FRONTPORCH_SHIFT  )
#define MIPI_TX_CFG_BACKPORCH(val)  (( val & MIPI_TX_CFG_BACKPORCH_MASK ) << MIPI_TX_CFG_BACKPORCH+_SHIFT  )
#define MIPI_TX_INT_SEL(val)        (( val & MIPI_TX_INT_SEL_MASK       ) << MIPI_TX_INT_SEL_SHIFT         )
#define MIPI_TX_SYNC_IDLE(val)      (( val & MIPI_TX_SYNC_IDLE_MASK  ) << MIPI_TX_SYNC_IDLE_SHIFT     )
#define MIPI_TX_ONESHOT(val)        (( val & MIPI_TX_ONESHOT_MASK  ) << MIPI_TX_ONESHOT_SHIFT     )
#define MIPI_TX_FIRST_FIELD(val)    (( val & MIPI_TX_FIRST_FIELD_MASK   ) << MIPI_TX_FIRST_FIELD_SHIFT     )
#define MIPI_TX_SCAN_MODE(val)      (( val & MIPI_TX_SCAN_MODE_MASK  ) << MIPI_TX_SCAN_MODE_SHIFT     )


/*Sigma denoise filter */

#define SIGMA_DNS_FRM_HEIGHT_MASK  0xFFFF
#define SIGMA_DNS_FRM_HEIGHT_SHIFT  0x10

#define SIGMA_DNS_FRM_HEIGHT(val) (( val & SIGMA_DNS_FRM_HEIGHT_MASK) << SIGMA_DNS_FRM_HEIGHT_SHIFT )
#define SIGMA_DNS_FRM_WIDTH(val)  (( val & SIGMA_DNS_FRM_WIDTH_MASK)  << SIGMA_DNS_FRM_WIDTH_SHIFT  )


#define SIGMA_DNS_CFG_FORMAT_MASK  0x1
#define SIGMA_DNS_CFG_FORMAT_SHIFT  0x0
#define SIGMA_DNS_CFG_PASSTHRU_EN_MASK  0x1
#define SIGMA_DNS_CFG_PASSTHRU_EN_SHIFT  0x1
#define SIGMA_DNS_CFG_DATA_WIDTH_MASK  0xF
#define SIGMA_DNS_CFG_DATA_WIDTH_SHIFT  0x4
#define SIGMA_DNS_CFG_NF_MASK    0x3FFF
#define SIGMA_DNS_CFG_NF_SHIFT   0x8


#define SIGMA_DNS_CFG_NF(val)          (( val & SIGMA_DNS_CFG_NF_MASK         )  << SIGMA_DNS_CFG_NF_SHIFT         )
#define SIGMA_DNS_CFG_DATA_WIDTH(val)  (( val & SIGMA_DNS_CFG_DATA_WIDTH_MASK )  << SIGMA_DNS_CFG_DATA_WIDTH_SHIFT )
#define SIGMA_DNS_CFG_PASSTHRU_EN(val) (( val & SIGMA_DNS_CFG_PASSTHRU_EN_MASK)  << SIGMA_DNS_CFG_PASSTHRU_EN_SHIFT)
#define SIGMA_DNS_CFG_FORMAT(val)      (( val & SIGMA_DNS_CFG_FORMAT_MASK     )  <<  SIGMA_DNS_CFG_FORMAT_SHIFT    )

#define SIGMA_DNS_CFG(nf,dataWidth,passthr,format) SIGMA_DNS_CFG_NF(nf) |\
                                                   SIGMA_DNS_CFG_DATA_WIDTH(dataWidth) |\
                                                   SIGMA_DNS_CFG_PASSTHRU_EN(passthr) |\
                                                   SIGMA_DNS_CFG_FORMAT(format)

#define SIGMA_DNS_TRESH_T2P1_MASK  0xFF
#define SIGMA_DNS_TRESH_T2P1_SHIFT  0x18
#define SIGMA_DNS_TRESH_T1P1_MASK  0xFF
#define SIGMA_DNS_TRESH_T1P1_SHIFT  0x10
#define SIGMA_DNS_TRESH_T2P0_MASK  0xFF
#define SIGMA_DNS_TRESH_T2P0_SHIFT  0x8

#define SIGMA_DNS_TRESH_T1P0_MASK  0xFF
#define SIGMA_DNS_TRESH_T1P0_SHIFT  0x0

#define SIGMA_DNS_TRESH_T2P3_MASK  0xFF
#define SIGMA_DNS_TRESH_T2P3_SHIFT  0x18

#define SIGMA_DNS_TRESH_T1P3_MASK  0xFF
#define SIGMA_DNS_TRESH_T1P3_SHIFT  0x10

#define SIGMA_DNS_TRESH_T2P2_MASK  0xFF
#define SIGMA_DNS_TRESH_T2P2_SHIFT  0x8

#define SIGMA_DNS_TRESH_T1P2_MASK  0xFF
#define SIGMA_DNS_TRESH_T1P2_SHIFT  0x0

#define SIGMA_DNS_TRESH_T2P1(val) (( val & SIGMA_DNS_TRESH_T2P1_MASK) << SIGMA_DNS_TRESH_T2P1_SHIFT )
#define SIGMA_DNS_TRESH_T1P1(val) (( val & SIGMA_DNS_TRESH_T1P1_MASK) << SIGMA_DNS_TRESH_T1P1_SHIFT )
#define SIGMA_DNS_TRESH_T2P0(val) (( val & SIGMA_DNS_TRESH_T2P0_MASK) << SIGMA_DNS_TRESH_T2P0_SHIFT )
#define SIGMA_DNS_TRESH_T2P3(val) (( val & SIGMA_DNS_TRESH_T2P3_MASK) << SIGMA_DNS_TRESH_T2P3_SHIFT )
#define SIGMA_DNS_TRESH_T1P3(val) (( val & SIGMA_DNS_TRESH_T1P3_MASK) << SIGMA_DNS_TRESH_T1P3_SHIFT )
#define SIGMA_DNS_TRESH_T2P2(val) (( val & SIGMA_DNS_TRESH_T2P2_MASK) << SIGMA_DNS_TRESH_T2P2_SHIFT )
#define SIGMA_DNS_TRESH_T1P2(val) (( val & SIGMA_DNS_TRESH_T1P2_MASK) << SIGMA_DNS_TRESH_T1P2_SHIFT )
#define SIGMA_DNS_TRESH_T1P0(val) (( val & SIGMA_DNS_TRESH_T1P0_MASK) << SIGMA_DNS_TRESH_T1P0_SHIFT )

#define SIGMA_DNS_TRESH_CFG(t1,t2,t3,t4,t5,t6,t7,t8) SIGMA_DNS_TRESH_T1P0(t1)|\
                                                     SIGMA_DNS_TRESH_T1P1(t2)|\
                                                     SIGMA_DNS_TRESH_T2P0(t3)|\
                                                     SIGMA_DNS_TRESH_T2P3(t4)|\
                                                     SIGMA_DNS_TRESH_T1P3(t5)|\
                                                     SIGMA_DNS_TRESH_T2P2(t6)|\
                                                     SIGMA_DNS_TRESH_T1P2(t7)|\
                                                     SIGMA_DNS_TRESH_T1P0(t8)








/*DoG/LTM */

#define DOGL_CFG_MODE_MASK              0x3
#define DOGL_CFG_MODE_SHIFT             0x0
#define DOGL_CFG_DOG_THRESHOLD_MASK     0xFF
#define DOGL_CFG_DOG_THRESHOLD_SHIFT    0x2
#define DOGL_CFG_OUTPUT_CLAMP_MASK      0x1
#define DOGL_CFG_OUTPUT_CLAMP_SHIFT     0xA
#define DOGL_CFG_LLB_DSMODE_MASK        0x3
#define DOGL_CFG_LLB_DSMODE_SHIFT       0xC
#define DOGL_CFG_BACK_GEN_THRESH_MASK   0xFF
#define DOGL_CFG_BACK_GEN_THRESH_SHIFT  0xE
#define DOGL_CFG_NUM_PLANES_MASK        0xF
#define DOGL_CFG_NUM_PLANES_SHIFT       0x16
#define DOGL_CFG_KERNEL_HEIGHT_MASK     0xF
#define DOGL_CFG_KERNEL_HEIGHT_SHIFT    0x1A

#define DOGL_CFG_KERNEL_HEIGHT(val)   (( val & DOGL_CFG_KERNEL_HEIGHT_MASK   ) << DOGL_CFG_KERNEL_HEIGHT_SHIFT   )
#define DOGL_CFG_NUM_PLANES(val)      (( val & DOGL_CFG_NUM_PLANES_MASK      ) << DOGL_CFG_NUM_PLANES_SHIFT      )
#define DOGL_CFG_BACK_GEN_THRESH(val) (( val & DOGL_CFG_BACK_GEN_THRESH_MASK ) << DOGL_CFG_BACK_GEN_THRESH_SHIFT )
#define DOGL_CFG_LLB_DSMODE(val)      (( val & DOGL_CFG_LLB_DSMODE_MASK      ) << DOGL_CFG_LLB_DSMODE_SHIFT      )
#define DOGL_CFG_OUTPUT_CLAMP(val)    (( val & DOGL_CFG_OUTPUT_CLAMP_MASK    ) << DOGL_CFG_OUTPUT_CLAMP_SHIFT    )
#define DOGL_CFG_DOG_THRESHOLD(val)   (( val & DOGL_CFG_DOG_THRESHOLD_MASK   ) << DOGL_CFG_DOG_THRESHOLD_SHIFT   )
#define DOGL_CFG_MODE(val)            (( val & DOGL_CFG_MODE_MASK            ) << DOGL_CFG_MODE_SHIFT            )

#define DOGL_MODE_DOG_ONLY    0x0
#define DOGL_MODE_LTM_ONLY    0x1
#define DOGL_MODE_DOG_DENOISE 0x2
#define DOGL_MODE_DOG_LTM     0x3

#define DOGL_CFG_SET(mode,thresh,outClamp,llbDSMode,backGenThresh,numPlanes,kernelHeight) DOGL_CFG_MODE(mode) |\
                                                                                          DOGL_CFG_DOG_THRESHOLD(thresh) |\
                                                                                          DOGL_CFG_OUTPUT_CLAMP(outClamp) |\
                                                                                          DOGL_CFG_LLB_DSMODE(llbDSMode) |\
                                                                                          DOGL_CFG_BACK_GEN_THRESH(backGenThresh) |\
                                                                                          DOGL_CFG_NUM_PLANES(numPlanes) |\
                                                                                          DOGL_CFG_KERNEL_HEIGHT(kernelHeight)

/* Unsharpen Filter */
#define SHARPEN_CFG_KERNEL_SIZE_MASK       0x7
#define SHARPEN_CFG_KERNEL_SIZE_SHIFT      0x0
#define SHARPEN_CFG_CLAMP_MASK             0x1
#define SHARPEN_CFG_CLAMP_SHIFT            0x3
#define SHARPEN_CFG_MODE_MASK              0x1
#define SHARPEN_CFG_MODE_SHIFT             0x4
#define SHARPEN_CFG_OPUT_DELTAS_ONLY_MASK  0x1
#define SHARPEN_CFG_OPUT_DELTAS_ONLY_SHIFT 0x5
#define SHARPEN_CFG_MIN_THRESHOLD_MASK     0xFFFF
#define SHARPEN_CFG_MIN_THRESHOLD_SHIFT    0x10

#define SHARPEN_CFG_KERNEL_SIZE(val)      ((val & SHARPEN_CFG_KERNEL_SIZE_MASK) << SHARPEN_CFG_KERNEL_SIZE_SHIFT)
#define SHARPEN_CFG_CLAMP(val)            ((val & SHARPEN_CFG_CLAMP_MASK) << SHARPEN_CFG_CLAMP_SHIFT)
#define SHARPEN_CFG_MODE(val)             ((val & SHARPEN_CFG_MODE_MASK) << SHARPEN_CFG_MODE_SHIFT)
#define SHARPEN_CFG_OPUT_DELTAS_ONLY(val) ((val & SHARPEN_CFG_OPUT_DELTAS_ONLY_MASK) << SHARPEN_CFG_OPUT_DELTAS_ONLY_SHIFT)
#define SHARPEN_CFG_MIN_THRESHOLD(val)    ((val & SHARPEN_CFG_MIN_THRESHOLD_MASK) << SHARPEN_CFG_MIN_THRESHOLD_SHIFT)

#define SHARPEN_CFG(thresh,ksz,clamp,mode,opDeltas) SHARPEN_CFG_MIN_THRESHOLD(thresh)|\
                                                    SHARPEN_CFG_KERNEL_SIZE(ksz)|\
                                                    SHARPEN_CFG_CLAMP(clamp)|\
                                                    SHARPEN_CFG_MODE(mode)|\
                                                    SHARPEN_CFG_OPUT_DELTAS_ONLY(opDeltas)



#define SHARPEN_COEF0_MASK 		0xFFFF
#define SHARPEN_COEF0_SHIFT		0x0
#define SHARPEN_COEF1_MASK		0xFFFF
#define SHARPEN_COEF1_SHIFT		0x10

#define SHARPEN_COEF2_MASK 		0xFFFF
#define SHARPEN_COEF2_SHIFT		0x0
#define SHARPEN_COEF3_MASK		0xFFFF
#define SHARPEN_COEF3_SHIFT		0x10

#define SHARPEN_COEF0_CFG(val) ((val & SHARPEN_COEF0_MASK) << SHARPEN_COEF0_SHIFT)
#define SHARPEN_COEF1_CFG(val) ((val & SHARPEN_COEF1_MASK) << SHARPEN_COEF1_SHIFT)
#define SHARPEN_COEF2_CFG(val) ((val & SHARPEN_COEF2_MASK) << SHARPEN_COEF2_SHIFT)
#define SHARPEN_COEF3_CFG(val) ((val & SHARPEN_COEF3_MASK) << SHARPEN_COEF3_SHIFT)

#define SHARPEN_COEFS01_CFG(coef0,coef1) SHARPEN_COEF0_CFG(coef0)|\
                                         SHARPEN_COEF1_CFG(coef1)

#define SHARPEN_COEFS23_CFG(coef2,coef3) SHARPEN_COEF2_CFG(coef2)|\
                                         SHARPEN_COEF3_CFG(coef3)

#define SHARPEN_RANGESTOP0_MASK		0xFFFF
#define SHARPEN_RANGESTOP0_SHIFT	0x0
#define SHARPEN_RANGESTOP1_MASK		0xFFFF
#define SHARPEN_RANGESTOP1_SHIFT	0x10
#define SHARPEN_RANGESTOP2_MASK        0xFFFF
#define SHARPEN_RANGESTOP2_SHIFT	0x0
#define SHARPEN_RANGESTOP3_MASK		0xFFFF
#define SHARPEN_RANGESTOP3_SHIFT	0x10

#define SHARPEN_RANGESTOP0_CFG(val) ((val & SHARPEN_RANGESTOP0_MASK) << SHARPEN_RANGESTOP0_SHIFT)
#define SHARPEN_RANGESTOP1_CFG(val) ((val & SHARPEN_RANGESTOP1_MASK) << SHARPEN_RANGESTOP1_SHIFT)
#define SHARPEN_RANGESTOP2_CFG(val) ((val & SHARPEN_RANGESTOP2_MASK) << SHARPEN_RANGESTOP2_SHIFT)
#define SHARPEN_RANGESTOP3_CFG(val) ((val & SHARPEN_RANGESTOP3_MASK) << SHARPEN_RANGESTOP3_SHIFT)

#define SHARPEN_RANGESTOP01_CFG(rangeStop0, rangeStop1)  SHARPEN_RANGESTOP0_CFG(rangeStop0)|\
                                                         SHARPEN_RANGESTOP1_CFG(rangeStop1)

#define SHARPEN_RANGESTOP23_CFG(rangeStop2, rangeStop3)  SHARPEN_RANGESTOP2_CFG(rangeStop2)|\
                                                         SHARPEN_RANGESTOP3_CFG(rangeStop3)


#define SHARPEN_LIMITS_UNDERSHOOT_MASK  0xFFFF
#define SHARPEN_LIMITS_UNDERSHOOT_SHIFT  0x0
#define SHARPEN_LIMITS_OVERSHOOT_MASK  0xFFFF
#define SHARPEN_LIMITS_OVERSHOOT_SHIFT  0x10

#define SHARPEN_LIMITS_UNDERSHOOT_CFG(val) ((val & SHARPEN_LIMITS_UNDERSHOOT_MASK) << SHARPEN_LIMITS_UNDERSHOOT_SHIFT)
#define SHARPEN_LIMITS_OVERSHOOT_CFG(val) ((val & SHARPEN_LIMITS_OVERSHOOT_MASK) << SHARPEN_LIMITS_OVERSHOOT_SHIFT)

#define SHARPEN_LIMITS_CFG(underShoot, overShoot) SHARPEN_LIMITS_UNDERSHOOT_CFG(underShoot)|\
                                                  SHARPEN_LIMITS_OVERSHOOT_CFG(overShoot)

#define SHARPEN_STRENGTH_POSITIVE_MASK	       0xFFFF
#define SHARPEN_STRENGTH_POSITIVE_SHIFT	       0x10
#define SHARPEN_STRENGTH_NEGATIVE_MASK	       0xFFFF
#define SHARPEN_STRENGTH_NEGATIVE_SHIFT	       0x0


#define SHARPEN_STRENGTH_POSITIVE_CFG(val) ((val & SHARPEN_STRENGTH_POSITIVE_MASK) << SHARPEN_STRENGTH_POSITIVE_SHIFT)
#define SHARPEN_STRENGTH_NEGATIVE_CFG(val) ((val & SHARPEN_STRENGTH_NEGATIVE_MASK) << SHARPEN_STRENGTH_NEGATIVE_SHIFT)

#define SHARPEN_STRENGTH_CFG(pos, neg) SHARPEN_STRENGTH_POSITIVE_CFG(pos)|\
                                       SHARPEN_STRENGTH_NEGATIVE_CFG(neg)


/* Chroma generation */

#define CGEN_CFG_PURP_FLARE_STR_MASK   0xFF
#define CGEN_CFG_PURP_FLARE_STR_SHIFT  0x0
#define CGEN_CFG_DESAT_OFFSET_MASK     0xFF
#define CGEN_CFG_DESAT_OFFSET_SHIFT    0x8
#define CGEN_CFG_DESAT_SLOPE_MASK      0xFF
#define CGEN_CFG_DESAT_SLOPE_SHIFT     0x10
#define CGEN_CFG_DATA_WIDTH_MASK       0xF
#define CGEN_CFG_DATA_WIDTH_SHIFT      0x18
#define CGEN_CFG_BYPASS_MASK           0x1
#define CGEN_CFG_BYPASS_SHIFT          0x1C
#define CGEN_CFG_LLB_DSMODE_MASK       0x3
#define CGEN_CFG_LLB_DSMODE_SHIFT      0x1D

#define CGEN_CFG_PURP_FLARE_STR(val)  (( val & CGEN_CFG_PURP_FLARE_STR_MASK   ) << CGEN_CFG_PURP_FLARE_STR_SHIFT   )
#define CGEN_CFG_DESAT_OFFSET(val)    (( val & CGEN_CFG_DESAT_OFFSET_MASK   ) << CGEN_CFG_DESAT_OFFSET_SHIFT   )
#define CGEN_CFG_DESAT_SLOPE(val)     (( val & CGEN_CFG_DESAT_SLOPE_MASK   ) << CGEN_CFG_DESAT_SLOPE_SHIFT   )
#define CGEN_CFG_DATA_WIDTH(val)      (((val-1) & CGEN_CFG_DATA_WIDTH_MASK   ) << CGEN_CFG_DATA_WIDTH_SHIFT   )
#define CGEN_CFG_BYPASS(val)          (( val & CGEN_CFG_BYPASS_MASK   ) << CGEN_CFG_BYPASS_SHIFT   )
#define CGEN_CFG_LLB_DSMODE(val)      (( val & CGEN_CFG_LLB_DSMODE_MASK   ) << CGEN_CFG_LLB_DSMODE_SHIFT   )

#define CGEN_CFG_SET(purpFlareStr,desatOffset,desatSlope,dataWidth,bypass,llbDSMode)  CGEN_CFG_PURP_FLARE_STR(purpFlareStr) |\
                                                                                      CGEN_CFG_DESAT_OFFSET(desatOffset) |\
                                                                                      CGEN_CFG_DESAT_SLOPE(desatSlope) |\
                                                                                      CGEN_CFG_DATA_WIDTH(dataWidth) |\
                                                                                      CGEN_CFG_BYPASS(bypass) |\
                                                                                      CGEN_CFG_LLB_DSMODE(llbDSMode)

#define CGEN_LUMA_COEFF_RED_MASK     0xFF
#define CGEN_LUMA_COEFF_RED_SHIFT    0x0
#define CGEN_LUMA_COEFF_GREEN_MASK   0xFF
#define CGEN_LUMA_COEFF_GREEN_SHIFT  0x8
#define CGEN_LUMA_COEFF_BLUE_MASK    0xFF
#define CGEN_LUMA_COEFF_BLUE_SHIFT   0x10

#define CGEN_LUMA_COEFF_RED(val)     (( val & CGEN_LUMA_COEFF_RED_MASK   ) << CGEN_LUMA_COEFF_RED_SHIFT )
#define CGEN_LUMA_COEFF_GREEN(val)   (( val & CGEN_LUMA_COEFF_GREEN_MASK   ) << CGEN_LUMA_COEFF_GREEN_SHIFT )
#define CGEN_LUMA_COEFF_BLUE(val)    (( val & CGEN_LUMA_COEFF_BLUE_MASK   ) << CGEN_LUMA_COEFF_BLUE_SHIFT )

#define CGEN_LUMA_COEFF_SET(red,green,blue) CGEN_LUMA_COEFF_RED(red) |\
                                            CGEN_LUMA_COEFF_GREEN(green) |\
                                            CGEN_LUMA_COEFF_BLUE(blue)


#define CGEN_CHROMA_COEFF_EPSILON_MASK   0xFF
#define CGEN_CHROMA_COEFF_EPSILON_SHIFT  0x0
#define CGEN_CHROMA_COEFF_KR_MASK        0xFF
#define CGEN_CHROMA_COEFF_KR_SHIFT       0x8
#define CGEN_CHROMA_COEFF_KG_MASK        0xFF
#define CGEN_CHROMA_COEFF_KG_SHIFT       0x10
#define CGEN_CHROMA_COEFF_KB_MASK        0xFF
#define CGEN_CHROMA_COEFF_KB_SHIFT       0x18

#define CGEN_CHROMA_COEFF_EPSILON(val) (( val & CGEN_CHROMA_COEFF_EPSILON_MASK ) << CGEN_CHROMA_COEFF_EPSILON_SHIFT )
#define CGEN_CHROMA_COEFF_RED(val)     (( val & CGEN_CHROMA_COEFF_KR_MASK      ) << CGEN_CHROMA_COEFF_KR_SHIFT )
#define CGEN_CHROMA_COEFF_GREEN(val)   (( val & CGEN_CHROMA_COEFF_KG_MASK      ) << CGEN_CHROMA_COEFF_KG_SHIFT )
#define CGEN_CHROMA_COEFF_BLUE(val)    (( val & CGEN_CHROMA_COEFF_KB_MASK      ) << CGEN_CHROMA_COEFF_KB_SHIFT )

#define CGEN_CHROMA_COEFF_SET(epsilon,kr,kg,kb) CGEN_CHROMA_COEFF_EPSILON(epsilon) |\
                                                CGEN_CHROMA_COEFF_RED(kr) |\
                                                CGEN_CHROMA_COEFF_GREEN(kg) |\
                                                CGEN_CHROMA_COEFF_BLUE(kb)


#endif /* _SIPPHWBITFIELDDEFS_H_ */
