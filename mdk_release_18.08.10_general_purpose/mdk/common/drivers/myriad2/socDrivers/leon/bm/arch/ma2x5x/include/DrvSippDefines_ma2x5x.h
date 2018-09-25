///
/// @file DrvSippDefines_ma2x5x.h
/// 
/// 
/// @ingroup DrvSipp_ma2x5x
/// @{
///

#ifndef DRV_SIPP_DEFINES_MA2x5x_H
#define DRV_SIPP_DEFINES_MA2x5x_H


#include<stdint.h>
#ifdef SIPP_PC
#include "registersMyriad.h"
#endif


//TODO(?): these defines could stand in registersMyriad.h; in this case, all the filters should have this privilege
#define SIPP_MIPI_TX0_BUF_BASE_ADR   SIPP_IBUF18_BASE_ADR
#define SIPP_MIPI_TX1_BUF_BASE_ADR   SIPP_IBUF19_BASE_ADR

#define SIPP_MIPI_RX0_BUF_BASE_ADR   SIPP_OBUF20_BASE_ADR
#define SIPP_MIPI_RX1_BUF_BASE_ADR   SIPP_OBUF21_BASE_ADR
#define SIPP_MIPI_RX2_BUF_BASE_ADR   SIPP_OBUF22_BASE_ADR
#define SIPP_MIPI_RX3_BUF_BASE_ADR   SIPP_OBUF23_BASE_ADR
//
#define SIPP_MIPI_RX0_FC_ADR         SIPP_OBUF20_FC_ADR
#define SIPP_MIPI_RX1_FC_ADR         SIPP_OBUF21_FC_ADR
#define SIPP_MIPI_RX2_FC_ADR         SIPP_OBUF22_FC_ADR
#define SIPP_MIPI_RX3_FC_ADR         SIPP_OBUF23_FC_ADR


// Basic enumeration that defines the chip revision
#define REV1_DEF 0
#define REV2_DEF 1

// Buffer fill level inc/dec and context update control bits of
// SIPP_I/OBUF_FC and SIPP_ICTX registers
#define SIPP_INCDEC_BIT 30
#define SIPP_START_BIT  30
#define SIPP_CTXUP_BIT  31
#define SIPP_INCDEC_BIT_MASK (1 << SIPP_INCDEC_BIT)
#define SIPP_START_BIT_MASK  (1 << SIPP_START_BIT)
#define SIPP_CTXUP_BIT_MASK  (1 << SIPP_CTXUP_BIT)

// Offset for working cbl field of SIPP_I/OBUF_FC registers
#define SIPP_CBL_OFFSET 16

// Mask and offset for working with SIPP_[FILT]_FRM_DIM registers
#define SIPP_IMGDIM_SIZE 16
#define SIPP_IMGDIM_MASK 0xffff

// Mask for working with kl field of filter configuration registers (offset generally 0)
#define SIPP_KL_MASK 0xf

// Masks for various APB registers
#define SIPP_NL_MASK  0x3ff
#define SIPP_SC_MASK  0x1
#define SIPP_OF_MASK  0x7
#define SIPP_NP_MASK  0xf
#define SIPP_FO_MASK  0x7
#define SIPP_LS_MASK  0x1ffffff
#define SIPP_PS_MASK  0x1ffffff
#define SIPP_IR_MASK  0xf
#define SIPP_IC_MASK  0x3
#define SIPP_SS_MASK  0xf
#define SIPP_CS_MASK  0xfff8
#define SIPP_SB_MASK  0x1

// Offsets from bit 0 in packed APB buffer configuration registers
#define SIPP_NL_OFFSET 0
#define SIPP_SC_OFFSET 10
#define SIPP_SL_OFFSET 12
#define SIPP_OF_OFFSET 12
#define SIPP_NP_OFFSET 24
#define SIPP_FO_OFFSET 28

// Offsets from bit 0 in packed APB interrupt configuration registers
#define SIPP_IR_OFFSET 0
#define SIPP_IC_OFFSET 4
#define SIPP_SS_OFFSET 8
#define SIPP_CS_OFFSET 16

// Automatic register names
#define CONCAT5(A,  B,C,D,  E)  A ## B ## C ## D ## E
#define CONCAT7(A,B,C,D,E,F,G)  A ## B ## C ## D ## E ## F ## G

// General enable/disable/active macros
#define ENABLED   1
#define DISABLED  0
#define ACTIVE    ENABLED

// Default/shadow registers indices
#define DEFAULT   0
#define SHADOW    1

// Filter constant kernel sizes
#define SIGMA_KERNEL_SIZE       5
#define RAW_KERNEL_SIZE         5
#define HIST_KERNEL_SIZE        3
#define LSC_KERNEL_SIZE         1
#define DBYR_KERNEL_SIZE        11
#define DBYR_PPM_KERNEL_SIZE    3
#define CHROMA_V_KERNEL_SIZE    21
#define CHROMA_H0_KERNEL_SIZE   23
#define CHROMA_H1_KERNEL_SIZE   17
#define CHROMA_H2_KERNEL_SIZE   13
#define CHROMA_REF_KERNEL_SIZE  21
#define LUMA_KERNEL_SIZE        7
#define LUMA_REF_KERNEL_SIZE    11
#define MED_KERNEL_SIZE         7
#define MED_LUMA_KERNEL_SIZE    1
#define SHARPEN_KERNEL_SIZE     7
#define LUT_KERNEL_SIZE         1
#define UPFIRDN_KERNEL_SIZE     7
#define EDGE_OP_KERNEL_SIZE     3
#define CC_LUMA_KERNEL_SIZE     1
#define CC_CHROMA_KERNEL_SIZE   5
#define MIPI_RX_KERNEL_SIZE     1
#define MIPI_TX_KERNEL_SIZE     1

// MIPI Rx/Tx instances
#define SIPP_MIPI_RX_INSTANCES  4
#define SIPP_MIPI_TX_INSTANCES  2

// Filter constant read clients number
#define SIPP_RAW_NLBRC       2
#define SIPP_LSC_NLBRC       2
#define SIPP_DBYR_NLBRC      2
#define SIPP_CHROMA_NLBRC    3
#define SIPP_LUMA_NLBRC      4
#define SIPP_SHARPEN_NLBRC   2
#define SIPP_UPFIRDN_NLBRC   1
#define SIPP_MED_NLBRC       1
#define SIPP_LUT_NLBRC       1
#define SIPP_EDGE_OP_NLBRC   1
#define SIPP_CONV_NLBRC      2
#define SIPP_HARRIS_NLBRC    1
#define SIPP_CC_NLBRC        4
#define SIPP_MIPI_RX_NLBRC   0
#define SIPP_MIPI_TX_NLBRC   1

// Filter cosntant write clients number
#define SIPP_NLBWC           1

// Default slice size
#define DEF_SLICE_SIZE 128*1024

// Number of CMX slices
#define CMX_NSLICES    16

// Max number of planes
#define MAX_PLANES     16

// Default size of AMC bus
#define AMC_WIDTH      64

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

// Edge Operator General Use Defines
// Input modes
#define NORMAL_MODE                  0
#define PRE_FP16_GRAD                1
#define PRE_U8_GRAD                  2
// Output modes
#define SCALED_MAGN_16BIT            0
#define SCALED_MAGN_8BIT             1
#define MAGN_ORIENT_16BIT            2
#define ORIENT_8BIT                  3
#define SCALED_GRADIENTS_16BIT       4
#define SCALED_GRADIENTS_32BIT       5
// Theta modes
#define NORMAL_THETA                 0
#define X_AXIS_REFL                  1
#define XY_AXIS_REFL                 2


//Statistics buffers
#define STATS_AE_BASE_ADR         O_BASE(SIPP_STATS_ID)
#define STATS_AF_BASE_ADR         O_BASE(SIPP_AF_STATS_ID)
#define STATS_HIST_LUMA_BASE_ADR  O_BASE(SIPP_LUMA_HIST_ID)
#define STATS_HIST_RGB_BASE_ADR   O_BASE(SIPP_RGB_HIST_ID)

// %%%%%%%%%%%%TEMP%%%%%%%%%%%%%
#define SIPP_DOG_NLBRC          1
#define DOG_INNER_KERNEL_SIZE   11
#define DOGLTM_KERNEL_SIZE      15
#define MULTI_UP_KERNEL_SIZE    (((DOGLTM_KERNEL_SIZE + 1) >> 1) + 1)
#define DOG_SUB_KERNEL_SIZE     1

#define SIPP_GCHR_NLBRC         3
#define RSZBHF_KERNEL_SIZE      2
#define PFLARE_KERNEL_SIZE      3
#define DARK_DESAT_KERNEL_SIZE  1
#define GEN_CHROMA_KERNEL_SIZE  1

#define CHRGAUSS_KERNEL_SIZE    3
#define GREYDESAT_KERNEL_SIZE   1

#define SIPP_DOGL_NLBRC         1
#define BGGEN_KERNEL_SIZE       11
#define RSZBTWO_KERNEL_SIZE     2
#define LTMAPPLY_KERNEL_SIZE    1

#define SIPP_SIGMA_NLBRC        1

// %%%%%%%%%%%%TEMP%%%%%%%%%%%%%


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
#define SIPP_LUMA_REF_ID   18   /*   Luma denoise reference in */

// Maximum IDs
//#define SIPP_MAX_ID        26
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



#endif // DRV_SIPP_DEFINES_MA2x5x_H

///@}
