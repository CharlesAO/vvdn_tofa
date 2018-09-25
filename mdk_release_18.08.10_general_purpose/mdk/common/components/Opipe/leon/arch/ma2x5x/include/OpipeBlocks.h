///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Opipe - SIPP blocks config data structs
///
/// @ingroup  Opipe
/// @{
///

#ifndef __OPIPE_BLOCKS_H__
#define __OPIPE_BLOCKS_H__

#include <stdint.h>
#include <mv_types.h>

//#############################################################################
//Based on Bayer-Pattern, we'll figure out pixels positions
typedef struct {
    uint32_t    gr; // Bayer CFA positions:
    uint32_t    r;
    uint32_t    b;
    uint32_t    gb;
} BlcCfg;

//#############################################################################
typedef struct {
    uint32_t    noiseFloor;
    uint32_t    thresh1P0;  //Pixel positions:
    uint32_t    thresh2P0;  // P0 P1
    uint32_t    thresh1P1;  // P2 P3
    uint32_t    thresh2P1;
    uint32_t    thresh1P2;
    uint32_t    thresh2P2;
    uint32_t    thresh1P3;
    uint32_t    thresh2P3;
} SigmaDnsCfg;

//#############################################################################
typedef struct {
    uint32_t    lscWidth;  //[coefs]
    uint32_t    lscHeight; //[coefs]
    uint32_t    lscStride; //[coefs] must be multiple of 4 !
        /*
         * Mesh entries are in 8.8 Fixed Point format.  Mesh is
         * laid out in a Bayer pattern.  Note: the table that this
         * field points to should not be modified until the
         * ISP update structure has been released via the
         * IC_EVENT_TYPE_ISP_CONFIG_ACCEPTED event.
         */
    uint16_t    *pLscTable;
} LscCfg;


//#############################################################################
typedef struct {
    /* Digital Gain - 8.8 Fixed Point */
    uint32_t    gainGr;
    uint32_t    gainR;
    uint32_t    gainB;
    uint32_t    gainGb;

    /* Clamp - e.g. set to 0x3fff if RAW block outputs data in 14-bit range */
    uint32_t    clampGr;
    uint32_t    clampR;
    uint32_t    clampB;
    uint32_t    clampGb;

    /* GrGb imbalance */
    uint32_t    grgbImbalPlatDark;
    uint32_t    grgbImbalDecayDark;
    uint32_t    grgbImbalPlatBright;
    uint32_t    grgbImbalDecayBright;
    uint32_t    grgbImbalThr;

    /* Defect pixel correction */
    uint32_t    dpcAlphaHotG;
    uint32_t    dpcAlphaHotRb;
    uint32_t    dpcAlphaColdG;
    uint32_t    dpcAlphaColdRb;
    uint32_t    dpcNoiseLevel;

    /*
     * Bit range of the raw filter output.  Bit range may be
     * promoted in the Raw filter - e.g. from 10 bits to 14 bits -
     * by programming the gains appropriately (in the case of
     * promotion from 10 to 14 bits, multiply by 16).
     */
    uint32_t    outputBits;
} RawCfg;

//#############################################################################
// AE/AWB stats config
typedef struct {
    uint32_t    firstPatchX;        /* X coord of top-left pixel of first patch */
    uint32_t    firstPatchY;        /* Y coord of top-left pixel of first patch */
    uint32_t    patchWidth;
    uint32_t    patchHeight;
    uint32_t    patchGapX;          /* Number of pixels from top-left of one cell to top-left of next */
    uint32_t    patchGapY;
    uint32_t    nPatchesX;          /* Number of patches on Horizontal direction */
    uint32_t    nPatchesY;          /* Number of patches on Vertical   direction */
    uint16_t    darkThresh;
    uint16_t    brightThresh;
} AeAwbCfg;

//AutoFocus(AF) stats config. Note: All values are relative to the full field of view,
//i.e. they are specified in terms of the Still Image resolution, and not the Video/Preview resolution.
typedef struct {
    uint32_t    firstPatchX;        /* X coord of top-left pixel of first patch */
    uint32_t    firstPatchY;        /* Y coord of top-left pixel of first patch */
    uint32_t    patchWidth;
    uint32_t    patchHeight;
    uint32_t    nPatchesX;          /* Number of patches on Horizontal direction */
    uint32_t    nPatchesY;          /* Number of patches on Vertical   direction */
  //uint32_t    nSkipRows;          /* Number of skipped lines in calc; 0 = no skipping */
    int32_t     initialSubtractionValue;
    int32_t     f1Threshold;        /* f1 min threshold */
    int32_t     f2Threshold;        /* f1 min threshold */
    int32_t     f1Coeffs[11];
    int32_t     f2Coeffs[11];
} AfCfg;

//#############################################################################
//RAW stats outputs
typedef struct {
    uint32_t count   [4]; // number of pixels in alternative accumulation.
    uint32_t accum   [4]; // accumulation of pixels within limits
    uint32_t altAccum[4]; // accumulation of pixels outside limits
} AeAwbPatchStats;

typedef struct{
  int32_t UNDEFINED; //so that entire struct is 8-Byte multiple
  int32_t sum_all_green;
  int32_t filter1_sum_max_green;
  int32_t filter2_sum_max_green;
  int32_t filter1_number_of_used_pixels_green;
  int32_t filter1_sum_green;
  int32_t filter2_number_of_used_pixels_green;
  int32_t filter2_sum_green;
} AfPatchStats;

//#############################################################################
typedef struct {
    int32_t     dewormGradientMul;  /* Fixed Point 1.7 */
    uint32_t    dewormSlope;        /* Fixed point 8.8 */
    int32_t     dewormOffset;       /* Signed integer */
    int32_t     lumaWeightR;
    int32_t     lumaWeightG;
    int32_t     lumaWeightB;
} DbyrCfg;

//#############################################################################
typedef struct {
    uint32_t  thr;
    uint16_t  curves[16*8];     /* U12f values */
} LtmCfg;

typedef struct {
    uint32_t thr;
    uint32_t strength;
    float    sigma11;           /* reference value for information/debug; IPIPE uses derived coeffs11 table below*/
    float    sigma15;           /* reference value for information/debug; IPIPE uses derived coeffs15 table below*/
    uint8_t  coeffs11[6];
    uint8_t  coeffs15[8];
} DogCfg;

//#############################################################################
typedef struct {
    float     strength;         /* reference value for information/debug; IPIPE uses derived bitpos and lut below*/
    uint32_t  alpha;
    uint32_t  bitpos;           /* derived from 'strength' */
    uint8_t   lut[32];          /* derived from 'strength' */
    uint32_t  f2;               /* Spatial weight control */
} LumaDnsCfg;

typedef struct {
    float     gamma;                /* reference value for information/debug; IPIPE uses derived lutGamma tables below*/
    float     angle_of_view;        /* reference value for information/debug; IPIPE uses derived lutDist table and shift below*/

    /*Actually an u8 array of 256 elements, but defined as u64 as HW requires so*/
    uint64_t  lutDist[256/8];       /* Derived from 'angle_of_view' */

    uint8_t   lutGamma0_32[9];      /* Derived from 'gamma' */
    uint8_t   lutGamma32_255[9];    /* Derived from 'gamma' */
    uint32_t  shift;                /* Derived from 'angle_of_view' */
} LumaDnsRefCfg;

//#############################################################################
typedef struct {
    float     sigma;              /* reference value */
    uint16_t  sharpenCoeffs[4];  /* fp16 format (derived from Sigma) */
    uint16_t  strengthDarken;    /* fp16 format */
    uint16_t  strengthLighten;   /* fp16 format */
    uint16_t  alpha;              /* fp16 format */
    uint16_t  overshoot;          /* fp16 format */
    uint16_t  undershoot;         /* fp16 format */
    uint16_t  rangeStop0;         /* fp16 format */
    uint16_t  rangeStop1;         /* fp16 format */
    uint16_t  rangeStop2;         /* fp16 format */
    uint16_t  rangeStop3;         /* fp16 format */
    uint16_t  minThr;             /* fp16 format */
} SharpenCfg;

//#############################################################################
typedef struct {
    uint32_t epsilon;           /* Chroma Generation */
    uint32_t kr;                /* Chroma Generation */
    uint32_t kg;                /* Chroma Generation */
    uint32_t kb;                /* Chroma Generation */
    uint32_t lumaCoeffR;        /* Luma generation */
    uint32_t lumaCoeffG;        /* Luma generation */
    uint32_t lumaCoeffB;        /* Luma generation */
    uint32_t pfrStrength;       /* Purple Flare Reduction */
    int32_t  desatOffset;       /* Dark Area Desaturation */
    uint32_t desatSlope;        /* Dark Area Desaturation */
} ChromaGenCfg;

//#############################################################################
typedef struct {
    uint32_t    kernelSize;     /* Must be 0, 3, 5 or 7 */
    uint32_t    slope;          /* Range is [0, 255] */
    int32_t     offset;         /* Range is [-128, 127] */
} MedianCfg;

//#############################################################################
typedef struct {
    uint32_t    th_r;
    uint32_t    th_g;
    uint32_t    th_b;
    uint32_t    limit;
    uint32_t    hEnab;
    uint32_t    greyDesatSlope;     /* Range is [0, 255] */
    int32_t     greyDesatOffset;    /* Range is [-128, 127] */
    uint32_t    greyCr;             /* Range is [0, 255] */
    uint32_t    greyCg;             /* Range is [0, 255] */
    uint32_t    greyCb;             /* Range is [0, 255] */
    uint32_t    convCoeffCenter;
    uint32_t    convCoeffEdge;
    uint32_t    convCoeffCorner;
} ChromaDnsCfg;

//#############################################################################
typedef struct {
   float    ccm[9];     //
   float    ccmOff[3];
   uint32_t kr;         /* U(4,8) format */
   uint32_t kg;         /* U(4,8) format */
   uint32_t kb;         /* U(4,8) format */
   uint16_t *lut3D;     /* U12f values: 3x16x16x16 */
} ColCombCfg;

//#############################################################################
typedef struct {
    /* Gamma table entries are within range [0, rangetop] */
    uint32_t    rangetop;
    uint32_t    size;        /* number of elements in "table", can be derived from rgnSize[] */
    uint32_t    rgnSize[2];  /* LUT region sizes */
    uint16_t    *table;      /* fp16 Channel-Mode setup (Rval,Gval,Bval,Pad) for each entry */
} LutCfg;

typedef struct {
   float  mat[3*3];
   float  offset[3]; //free term
} ColConvCfg;

//#############################################################################
typedef struct {
    uint32_t  kerSz;   //3,5 or 7
    uint32_t  hN, hD;  //horizontal ratio N/D
    uint32_t  vN, vD;  //vertical   ratio N/D
    uint8_t  *hCoefs;  //horizontal coefs (all phases)
    uint8_t  *vCoefs;  //vertical coefs (all phases)
    uint16_t  iW, iH;  //reference input resolution
    uint16_t  oW, oH;  //derived   output  resolution (based on N,D)
}UpfirdnCfg;

//#############################################################################
//Non-Opipe filter (no LLB)
//For 3x3 convolution, just set inner 3x3 coefs and outer coefs = 0
typedef struct {
    uint16_t mat5x5[5*5];
} ConvCfg;

//#############################################################################
typedef struct {
    uint32_t cfg;
    uint32_t xStartWidth [4];  //-> SIPP_MIPI_RX0_X?_ADR
    uint32_t yStartHeight[4];  //-> SIPP_MIPI_RX0_Y?_ADR
    uint32_t sel01, sel23;     //-> SIPP_MIPI_RX0_SEL??_ADR
    uint32_t mask[4];          //-> SIPP_MIPI_RX0_MASK?_ADR
    uint32_t black01, black23; //-> SIPP_MIPI_RX0_BLACK??_ADR
    uint32_t vbp;              //-> SIPP_MIPI_RX0_VBP_ADR
} MipiRxCfg;

//#############################################################################
typedef struct {
   uint32_t cfg;               //-> SIPP_HARRIS_CFG_ADR
   u32f32   kValue;            //-> SIPP_HARRIS_K_ADR
} HarrisCfg;

/// @}
#endif
