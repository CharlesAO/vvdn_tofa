// -----------------------------------------------------------------------------
// Copyright (C) 2013 Movidius Ltd. All rights reserved
//
// Company          : Movidius
// Description      : SIPP Accelerator HW model common definitions
//
// Mainly APB register map related
// -----------------------------------------------------------------------------

#ifndef __SIPP_HWCOMMON_MA2x5x_H__
#define __SIPP_HWCOMMON_MA2x5x_H__

#include<stdint.h>

// Basic enumeration that defines the chip revision
#define REV1_DEF 0
#define REV2_DEF 1
typedef enum {
    REV1 = REV1_DEF,
    REV2 = REV2_DEF
} Revision;

// Basic enumeration that defines the address type
typedef enum {
    OTHER,
    CMX_ADDRESS,
    CMX_MIRRORED_ADDRESS,
    DDR_ADDRESS,
    DDR_MIRRORED_ADDRESS
} AddressType;







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

// IO = Input/Output - call with I or with O, respectively
// BS =  Base/Shadow - leave it empty or call with _SHADOW, respectively
//#define BUF_BASE(IO,N,BS)       CONCAT7(SIPP_, IO, BUF, N, _BASE, BS, _ADR)
//#define BUF_CFG(IO,N,BS)        CONCAT7(SIPP_, IO, BUF, N, _CFG, BS, _ADR)
//#define BUF_LS(IO,N,BS)         CONCAT7(SIPP_, IO, BUF, N, _LS, BS, _ADR)
//#define BUF_PS(IO,N,BS)         CONCAT7(SIPP_, IO, BUF, N, _PS, BS, _ADR)
//#define BUF_IR(IO,N,BS)         CONCAT7(SIPP_, IO, BUF, N, _IR, BS, _ADR)
//#define BUF_FC(IO,N)            CONCAT5(SIPP_, IO, BUF, N, _FC_ADR)
//#define CTX(IO,N)               CONCAT5(SIPP_, IO, CTX, N, _ADR)

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
//#######################################################################################
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

//#######################################################################################
//RAW stats
//typedef struct {
//    uint32_t count    [4]; // number of pixels in alternative accumulation.
//    uint32_t accum    [4]; // accumulation of pixels within limits
//    uint32_t alt_accum[4]; // accumulation of pixels outside limits
//} ae_patch_stats;
//
//typedef struct{
//  int32_t UNDEFINED; //so that entire struct is 8-Byte multiple
//  int32_t sum_all_green;
//  int32_t filter1_sum_max_green;
//  int32_t filter2_sum_max_green;
//  int32_t filter1_number_of_used_pixels_green;
//  int32_t filter1_sum_green;
//  int32_t filter2_number_of_used_pixels_green;
//  int32_t filter2_sum_green;
//} AF_paxel_statistics;

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
#define GEN_CHROMA_KERNEL_SIZE  7

#define CHRGAUSS_KERNEL_SIZE    3
#define GREYDESAT_KERNEL_SIZE   1

#define SIPP_DOGL_NLBRC         1
#define BGGEN_KERNEL_SIZE       11
#define RSZBTWO_KERNEL_SIZE     2
#define LTMAPPLY_KERNEL_SIZE    1

#define SIPP_SIGMA_NLBRC        1

// %%%%%%%%%%%%TEMP%%%%%%%%%%%%%

#endif // __SIPP_HWCOMMON_MA2x5x_H__
