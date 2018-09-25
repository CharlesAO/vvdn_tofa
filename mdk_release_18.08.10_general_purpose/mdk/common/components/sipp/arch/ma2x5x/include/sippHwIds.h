///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     HW filter related macros
///

#ifndef _SIPP_HW_IDS_H_
#define _SIPP_HW_IDS_H_

#ifdef SIPP_PC
#include "registersMyriad.h"
#endif

// SIPP Filter/buffer IDs
// Following are filter and input/output buffer IDs
#define SIPP_OPIPE_ID     -1    /* Convention to mark the oPipe */
// **** RAW processing
#define SIPP_SIGMA_ID      0    /* Sigma denoise */
#define SIPP_LSC_ID        1    /* Lens shading filter */
#define SIPP_RAW_ID        2    /* RAW filter */
#define SIPP_DBYR_ID       3    /* Debayer */
// **** Luma processing
#define SIPP_DOGL_ID       4    /* Difference of Gaussians/Local tone mapping */
#define SIPP_LUMA_ID       5    /* Luma denoise in/out */
#define SIPP_SHARPEN_ID    6    /* Sharpening */
// **** Chroma processing
#define SIPP_CGEN_ID       7    /* Generate chroma filter */
#define SIPP_MED_ID        8    /* Median */
#define SIPP_CHROMA_ID     9    /* Chroma denoise */
// **** RGB processing
#define SIPP_CC_ID         10   /* Colour combination */
#define SIPP_LUT_ID        11   /* Look-up table */
// **** General purpose/CV processing
#define SIPP_EDGE_OP_ID    12   /* Edge operator */
#define SIPP_CONV_ID       13   /* Programmable convolution */
#define SIPP_HARRIS_ID     14   /* Harris corners */
#define SIPP_UPFIRDN0_ID   15   /* Polyphase FIR filter[0] */
#define SIPP_UPFIRDN1_ID   16   /* Polyphase FIR filter[1] */
#define SIPP_UPFIRDN2_ID   17   /* Polyphase FIR filter[2] */

// Following are filter and input buffer IDs
#define SIPP_MIPI_TX0_ID   18   /* MIPI Tx[0] filter (input buffer only) */
#define SIPP_MIPI_TX1_ID   19   /* MIPI Tx[1] filter (input buffer only) */

// Following are filter and output buffer IDs
#define SIPP_MIPI_RX0_ID   20   /* MIPI Rx[0] filter (output buffer only) */
#define SIPP_MIPI_RX1_ID   21   /* MIPI Rx[1] filter (output buffer only) */
#define SIPP_MIPI_RX2_ID   22   /* MIPI Rx[2] filter (output buffer only) */
#define SIPP_MIPI_RX3_ID   23   /* MIPI Rx[3] filter (output buffer only) */

// Following are input buffer IDs
#define SIPP_LSC_GM_ID     20   /* Lens shading correction - gain mesh buffer */
#define SIPP_MED_LUMA_ID   21   /* Median filter - chroma median reference luma buffer */
#define SIPP_CC_CHROMA_ID  22   /* Colour combination - chroma buffer */
#define SIPP_LUT_LOAD_ID   23   /* LUT filter - LUT buffer */
#define SIPP_LUMA_C4LUT_ID 24   /* Luma denoise - cosine-4th law LUT buffer */
#define SIPP_CC_3DLUT_ID   25   /* Colour combination - 3D LUT buffer */
#define SIPP_RAW_DEFECT_ID 26   /* RAW filter - defect pixel list for static defect correction */

// Following are output buffer IDs
#define SIPP_DBYR_LUMA_ID  18   /* Debayer luma buffer */
#define SIPP_STATS_ID      19   /* RAW statistics */

// Following are output buffer IDs
#define SIPP_AF_STATS_ID   24  /* RAW AF statistics */
#define SIPP_LUMA_HIST_ID  25  /* RAW Luma histogram */
#define SIPP_RGB_HIST_ID   26  /* RAW RGB histogram */

// V1 legacy
#define SIPP_CHROMA_REF_ID 17   /* Chroma denoise reference in */
#define SIPP_LUMA_REF_ID   18   /* Luma denoise reference in   */

// Maximum IDs
#define SIPP_MAX_ID        26
#define SIPP_MIN_FILTER_ID SIPP_SIGMA_ID
#define SIPP_MAX_FILTER_ID SIPP_MIPI_RX3_ID

// Reserved IDs
#define SIPP_RESERVED_ID     13                /* 13 -  Reserved */
#define SIPP_V2_RESERVED_ID  SIPP_V2_MAX_ID+1  /* No ID Reserved */

#define SIPP_SIGMA_ID_MASK               (1 << SIPP_SIGMA_ID)
#define SIPP_RAW_ID_MASK                 (1 << SIPP_RAW_ID)
#define SIPP_STATS_MASK                  (1 << SIPP_STATS_ID)
#define SIPP_LSC_ID_MASK                 (1 << SIPP_LSC_ID)
#define SIPP_LSC_GM_ID_MASK              (1 << SIPP_LSC_GM_ID)
#define SIPP_DBYR_ID_MASK                (1 << SIPP_DBYR_ID)
#define SIPP_CHROMA_ID_MASK              (1 << SIPP_CHROMA_ID)
#define SIPP_LUMA_ID_MASK                (1 << SIPP_LUMA_ID)
#define SIPP_LUMA_REF_ID_MASK            (1 << SIPP_LUMA_REF_ID)
#define SIPP_SHARPEN_ID_MASK             (1 << SIPP_SHARPEN_ID)
#define SIPP_UPFIRDN0_ID_MASK            (1 << SIPP_UPFIRDN0_ID)
#define SIPP_UPFIRDN1_ID_MASK            (1 << SIPP_UPFIRDN1_ID)
#define SIPP_UPFIRDN2_ID_MASK            (1 << SIPP_UPFIRDN2_ID)
#define SIPP_MED_ID_MASK                 (1 << SIPP_MED_ID)
#define SIPP_LUT_ID_MASK                 (1 << SIPP_LUT_ID)
#define SIPP_LUT_LOAD_MASK               (1 << SIPP_LUT_LOAD_ID)
#define SIPP_EDGE_OP_ID_MASK             (1 << SIPP_EDGE_OP_ID)
#define SIPP_CONV_ID_MASK                (1 << SIPP_CONV_ID)
#define SIPP_HARRIS_ID_MASK              (1 << SIPP_HARRIS_ID)
#define SIPP_CC_ID_MASK                  (1 << SIPP_CC_ID)
#define SIPP_CC_CHROMA_ID_MASK           (1 << SIPP_CC_CHROMA_ID)
#define SIPP_DOGL_ID_MASK                (1 << SIPP_DOGL_ID)
#define SIPP_CGEN_ID_MASK                (1 << SIPP_CGEN_ID)
#define SIPP_MIPI_TX0_ID_MASK            (1 << SIPP_MIPI_TX0_ID)
#define SIPP_MIPI_TX1_ID_MASK            (1 << SIPP_MIPI_TX1_ID)
#define SIPP_MIPI_RX0_ID_MASK            (1 << SIPP_MIPI_RX0_ID)
#define SIPP_MIPI_RX1_ID_MASK            (1 << SIPP_MIPI_RX1_ID)
#define SIPP_MIPI_RX2_ID_MASK            (1 << SIPP_MIPI_RX2_ID)
#define SIPP_MIPI_RX3_ID_MASK            (1 << SIPP_MIPI_RX3_ID)

#define SIPP_DMA_ID                      (SIPP_MAX_ID+1  )
#define SIPP_SVU_ID                      (SIPP_MAX_ID+2  )
#define EXE_NUM                          (SIPP_MAX_ID+2+1)
#define SIPP_FAKE_ID                     (EXE_NUM + 1)

#define SIPP_HW_FILTER_MASK_SIZE         ((EXE_NUM+31)/32)

// Mask of HW filters which have LLBs and which should delay
// processing until the full kernel is in the LLB
// DoG, CGen and CComb are not in this list because they are
// not regularly scheduled and so may be handled some other way
#define SIPP_FILTER_LLB_MASK             (SIPP_SIGMA_ID_MASK | \
                                          SIPP_RAW_ID_MASK   | \
                                          SIPP_DBYR_ID_MASK  | \
                                          SIPP_LUMA_ID_MASK  | \
                                          SIPP_SHARPEN_ID_MASK | \
                                          SIPP_MED_ID_MASK   | \
                                          SIPP_CHROMA_ID_MASK)

#define SIPP_FILTER_FULL_LLB_MASK        (SIPP_SIGMA_ID_MASK | \
                                          SIPP_RAW_ID_MASK   | \
                                          SIPP_DBYR_ID_MASK  | \
                                          SIPP_DOGL_ID_MASK  | \
                                          SIPP_LUMA_ID_MASK  | \
                                          SIPP_SHARPEN_ID_MASK | \
                                          SIPP_CGEN_ID_MASK  | \
                                          SIPP_MED_ID_MASK   | \
                                          SIPP_CHROMA_ID_MASK | \
                                          SIPP_CC_ID_MASK)

#endif // _SIPP_HW_IDS_H_

