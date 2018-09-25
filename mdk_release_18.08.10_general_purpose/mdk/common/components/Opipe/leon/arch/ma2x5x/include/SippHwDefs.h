///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Opipe - SIPP IDs and macros
///

#ifndef __SIPP_HW_DEFS_H__
#define __SIPP_HW_DEFS_H__

//#########################################################################################
//Ref: older http://dub30/svn/TRUNK/fragrak_v2/data/fragrak/sipp/MODELS/sipp_hw/include/sippHwCommon.h
//#########################################################################################
//Copy-pasted sections of interes here:
//a) to avoid depencency of always changing mdk folders and file names.
//b) svn export is deprecated

//SIPP Filter/buffer IDs
//Following are filter and input/output buffer IDs
//RAW processing
#define SIPP_SIGMA_ID      0    // Sigma denoise
#define SIPP_LSC_ID        1    // Lens shading filter
#define SIPP_RAW_ID        2    // RAW filter
#define SIPP_DBYR_ID       3    // Debayer
//Luma processing
#define SIPP_DOGL_ID       4    // Difference of Gaussians/Local tone mapping
#define SIPP_LUMA_ID       5    // Luma denoise in/out
#define SIPP_SHARPEN_ID    6    // Sharpening
//Chroma processing
#define SIPP_CGEN_ID       7    // Generate chroma filter
#define SIPP_MED_ID        8    // Median
#define SIPP_CHROMA_ID     9    // Chroma denoise
//RGB processing
#define SIPP_CC_ID         10   // Colour combination
#define SIPP_LUT_ID        11   // Look-up table
//General purpose/CV processing
#define SIPP_EDGE_OP_ID    12   // Edge operator
#define SIPP_CONV_ID       13   // Programmable convolution
#define SIPP_HARRIS_ID     14   // Harris corners
#define SIPP_UPFIRDN0_ID   15   // Polyphase FIR filter[0]
#define SIPP_UPFIRDN1_ID   16   // Polyphase FIR filter[1]
#define SIPP_UPFIRDN2_ID   17   // Polyphase FIR filter[2]

// Following are filter and input buffer IDs
#define SIPP_MIPI_TX0_ID   18   // MIPI Tx[0] filter (input buffer only)
#define SIPP_MIPI_TX1_ID   19   // MIPI Tx[1] filter (input buffer only)

// Following are filter and output buffer IDs
#define SIPP_MIPI_RX0_ID   20   // MIPI Rx[0] filter (output buffer only)
#define SIPP_MIPI_RX1_ID   21   // MIPI Rx[1] filter (output buffer only)
#define SIPP_MIPI_RX2_ID   22   // MIPI Rx[2] filter (output buffer only)
#define SIPP_MIPI_RX3_ID   23   // MIPI Rx[3] filter (output buffer only)

// Following are input buffer IDs
#define SIPP_LSC_GM_ID     20   // Lens shading correction - gain mesh buffer
#define SIPP_MED_LUMA_ID   21   // Median filter - chroma median reference luma buffer
#define SIPP_CC_CHROMA_ID  22   // Colour combination - chroma buffer
#define SIPP_LUT_LOAD_ID   23   // LUT filter - LUT buffer
#define SIPP_LUMA_C4LUT_ID 24   // Luma denoise - cosine-4th law LUT buffer
#define SIPP_CC_3DLUT_ID   25   // Colour combination - 3D LUT buffer
#define SIPP_RAW_DEFECT_ID 26   // RAW filter - defect pixel list for static defect correction

// Following are output buffer IDs
#define SIPP_DBYR_LUMA_ID  18   // Debayer luma buffer
#define SIPP_STATS_ID      19   // RAW statistics

// Following are output buffer IDs
#define SIPP_AF_STATS_ID   24  // RAW AF statistics
#define SIPP_LUMA_HIST_ID  25  // RAW Luma histogram
#define SIPP_RGB_HIST_ID   26  // RAW RGB histogram

//Number of filters
#define SIPP_F_NUM        (SIPP_MIPI_RX3_ID+1)

//"DoG/LTM - extra read client for DoG subtract"
#define SIPP_DOGL_SUB_ID   24

#define SIPP_MIPI_RX_MASK ((1<<SIPP_MIPI_RX0_ID)| \
                           (1<<SIPP_MIPI_RX1_ID)| \
                           (1<<SIPP_MIPI_RX2_ID)| \
                           (1<<SIPP_MIPI_RX3_ID))

//#########################################################################################
// Format encoding
#define PLANAR         0
#define BAYER          1

// Bayer pattern encoding
#define GRBG           0
#define RGGB           1
#define GBRG           2
#define BGGR           3

// Image out order encoding
#define P_RGB          0
#define P_BGR          1
#define P_RBG          2
#define P_BRG          3
#define P_GRB          4
#define P_GBR          5

//#########################################################################################
// Buffer fill level inc/dec and context update control bits of
// SIPP_I/OBUF_FC and SIPP_ICTX registers
#define SIPP_INCDEC_BIT 30
#define SIPP_START_BIT  30
#define SIPP_CTXUP_BIT  31
#define SIPP_INCDEC_BIT_MASK (1 << SIPP_INCDEC_BIT)
#define SIPP_START_BIT_MASK  (1 << SIPP_START_BIT)
#define SIPP_CTXUP_BIT_MASK  (1 << SIPP_CTXUP_BIT)

//#########################################################################################
// Offsets from bit 0 in packed APB buffer configuration registers
#define SIPP_NL_OFFSET 0
#define SIPP_SC_OFFSET 10
#define SIPP_SL_OFFSET 12
#define SIPP_OF_OFFSET 12
#define SIPP_NP_OFFSET 24
#define SIPP_FO_OFFSET 28

#endif