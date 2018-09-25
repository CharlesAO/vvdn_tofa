#ifndef ISPXUCONTROLSMX_H_
#define ISPXUCONTROLSMX_H_

#include "ispXUControlsCommon.h"

////////////////////////////////////////////////////////////////////////////////
//[General]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_general_data_t
{
    uint8_t    fileName[MAX_FILENAME_LEN];
    uint32_t   image_width;
    uint32_t   image_height;
    uint32_t   skip_lines;
    uint32_t   bits;
    uint32_t   bayer_order;
} uvcx_mx_general_data_t;

typedef struct _uvcx_mx_general_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_mx_general_data_t cur;
    uvcx_mx_general_data_t min;
    uvcx_mx_general_data_t max;
    uvcx_mx_general_data_t res;
    uvcx_mx_general_data_t def;

} uvcx_mx_general_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Env]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_enviroment_data_t
{
    uint32_t    exp;
    uint32_t    gain;
    uint32_t    color_temp;
} uvcx_mx_enviroment_data_t;

typedef struct _uvcx_mx_enviroment_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_mx_enviroment_data_t cur;
    uvcx_mx_enviroment_data_t min;
    uvcx_mx_enviroment_data_t max;
    uvcx_mx_enviroment_data_t res;
    uvcx_mx_enviroment_data_t def;

} uvcx_mx_enviroment_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Black level correction]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_blc_data_t
{
    uint32_t gr;
    uint32_t r;
    uint32_t b;
    uint32_t gb;
} uvcx_mx_blc_data_t;

typedef struct _uvcx_mx_blc_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_mx_blc_data_t cur;
    uvcx_mx_blc_data_t min;
    uvcx_mx_blc_data_t max;
    uvcx_mx_blc_data_t res;
    uvcx_mx_blc_data_t def;

} uvcx_mx_blc_ctrl_t;
////////////////////////////////////////////////////////////////////////////////
//[Sigma denoise]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_sgmdns_data_t
{
    uint32_t    noiseFloor;
    uint32_t    thresh1P0;
    uint32_t    thresh2P0;
    uint32_t    thresh1P1;
    uint32_t    thresh2P1;
    uint32_t    thresh1P2;
    uint32_t    thresh2P2;
    uint32_t    thresh1P3;
    uint32_t    thresh2P3;
} uvcx_mx_sgmdns_data_t;

typedef struct _uvcx_mx_sgmdns_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_mx_sgmdns_data_t cur;
    uvcx_mx_sgmdns_data_t min;
    uvcx_mx_sgmdns_data_t max;
    uvcx_mx_sgmdns_data_t res;
    uvcx_mx_sgmdns_data_t def;

} uvcx_mx_sgmdns_ctrl_t;
////////////////////////////////////////////////////////////////////////////////
//[Lsc]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_lsc_data_t
{
    uint32_t    lscWidth;
    uint32_t    lscHeight;
    uint32_t    lscStride;
} uvcx_mx_lsc_data_t;

typedef struct _uvcx_mx_lsc_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_mx_lsc_data_t cur;
    uvcx_mx_lsc_data_t min;
    uvcx_mx_lsc_data_t max;
    uvcx_mx_lsc_data_t res;
    uvcx_mx_lsc_data_t def;
} uvcx_mx_lsc_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Lsc table]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_lsctable_data_t
{
    uint16_t lscTable[MAX_LSC_SIZE];
} uvcx_mx_lsctable_data_t;

typedef struct _uvcx_mx_lsctable_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_mx_lsctable_data_t cur;
    uvcx_mx_lsctable_data_t min;
    uvcx_mx_lsctable_data_t max;
    uvcx_mx_lsctable_data_t res;
    uvcx_mx_lsctable_data_t def;
} uvcx_mx_lsctable_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Lsc table transfer]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_lsctabletransfer_data_t
{
    uint32_t header[2];
    uint8_t  payload[TABLE_CHUNK];
} uvcx_mx_lsctabletransfer_data_t;

typedef struct _uvcx_mx_lsctabletransfer_ctrl_t
{
    uint8_t    u8Info;
    uint16_t   u16Len;
    uvcx_mx_lsctabletransfer_data_t cur;
    uvcx_mx_lsctabletransfer_data_t min;
    uvcx_mx_lsctabletransfer_data_t max;
    uvcx_mx_lsctabletransfer_data_t res;
    uvcx_mx_lsctabletransfer_data_t def;
} uvcx_mx_lsctabletransfer_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Raw]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_raw_data_t
{
    uint32_t    gainGr;
    uint32_t    gainR;
    uint32_t    gainB;
    uint32_t    gainGb;
    uint32_t    clampGr;
    uint32_t    clampR;
    uint32_t    clampB;
    uint32_t    clampGb;
    uint32_t    grgbImbalPlatDark;
    uint32_t    grgbImbalDecayDark;
    uint32_t    grgbImbalPlatBright;
    uint32_t    grgbImbalDecayBright;
    uint32_t    grgbImbalThr;
    uint32_t    dpcAlphaHotG;
    uint32_t    dpcAlphaHotRb;
    uint32_t    dpcAlphaColdG;
    uint32_t    dpcAlphaColdRb;
    uint32_t    dpcNoiseLevel;
    uint32_t    outputBits;
} uvcx_mx_raw_data_t;

typedef struct _uvcx_mx_raw_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_mx_raw_data_t cur;
    uvcx_mx_raw_data_t min;
    uvcx_mx_raw_data_t max;
    uvcx_mx_raw_data_t res;
    uvcx_mx_raw_data_t def;
} uvcx_mx_raw_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Demosaic]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_demosaic_data_t
{

    int32_t     greenInterpCoeff;
    uint32_t    gradientMulDark;
    uint32_t    gradientMulBright;
    int32_t     lumaWeightR;
    int32_t     lumaWeightG;
    int32_t     lumaWeightB;
} uvcx_mx_demosaic_data_t;

typedef struct _uvcx_mx_demosaic_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_mx_demosaic_data_t cur;
    uvcx_mx_demosaic_data_t min;
    uvcx_mx_demosaic_data_t max;
    uvcx_mx_demosaic_data_t res;
    uvcx_mx_demosaic_data_t def;
} uvcx_mx_demosaic_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Chroma generation]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_chromagen_data_t
{
    uint32_t    epsilon;            /* Chroma Generation */
    uint32_t    kr;                 /* Chroma Generation */
    uint32_t    kg;                 /* Chroma Generation */
    uint32_t    kb;                 /* Chroma Generation */
    uint32_t    lumaCoeffR;         /* Luma generation */
    uint32_t    lumaCoeffG;         /* Luma generation */
    uint32_t    lumaCoeffB;         /* Luma generation */
    uint32_t    pfrStrengthR;       /* Purple Flare Reduction */
    uint32_t    pfrStrengthB;       /* Purple Flare Reduction */
    uint32_t    pfrCoeff0;          /* Purple Flare Reduction */
    uint32_t    pfrCoeff1;          /* Purple Flare Reduction */
    uint32_t    pfrCoeff2;          /* Purple Flare Reduction */
    int32_t     desatOffset;        /* Dark Area Desaturation */
    uint32_t    desatSlope;         /* Dark Area Desaturation */
    uint32_t    emSlope;            /* Edge Median */
    int32_t     emOffset;           /* Edge Median */
    uint32_t    emLimit;            /* Edge Median */

} uvcx_mx_chromagen_data_t;

typedef struct _uvcx_mx_chromagen_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_mx_chromagen_data_t cur;
    uvcx_mx_chromagen_data_t min;
    uvcx_mx_chromagen_data_t max;
    uvcx_mx_chromagen_data_t res;
    uvcx_mx_chromagen_data_t def;
} uvcx_mx_chromagen_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[median]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_median_data_t
{
    uint32_t   kernelSize;
    uint32_t   slope;
    int32_t   offset;
} uvcx_mx_median_data_t;

typedef struct _uvcx_mx_median_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_mx_median_data_t cur;
    uvcx_mx_median_data_t min;
    uvcx_mx_median_data_t max;
    uvcx_mx_median_data_t res;
    uvcx_mx_median_data_t def;
} uvcx_mx_median_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[sharpen]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_sharpen_data_t
{
    float     lowpassSigma;     //Single precision floating point
    float     bandpassSigma0;   //Single precision floating point
    float     bandpassSigma1;   //Single precision floating point
    float     angleOfView;      //Single precision floating point
    uint32_t  hpGainCenter;     //U(3.8) fixed point
    uint32_t  hpGainCorner;     //U(3.8) fixed point
    uint32_t  bpGainCenter;     //U(3.8) fixed point
    uint32_t  bpGainCorner;     //U(3.8) fixed point
    uint32_t  darkenBiasGain;   // Legal value are [0, 256], which maps to [0, 1.0]
    uint32_t  nonlinearStops[3];// Range is [0, 4095]
    uint32_t  nonlinearGains[3];// Legal value are [0, 256], which maps to [0, 1.0]
    uint32_t  rangeStops[4];    // Values are in range [0, 4096]
    uint32_t  overshoot;        //U(1,8) fixed point format
    uint32_t  undershoot;       //U(1,8) fixed point format
    uint32_t  alpha;            //U(1,8) fixed point format
} uvcx_mx_sharpen_data_t;

typedef struct _uvcx_mx_sharpen_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_mx_sharpen_data_t cur;
    uvcx_mx_sharpen_data_t min;
    uvcx_mx_sharpen_data_t max;
    uvcx_mx_sharpen_data_t res;
    uvcx_mx_sharpen_data_t def;
} uvcx_mx_sharpen_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[luma denoise]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_lumadns_data_t
{
    float strength;
    uint32_t alpha;
    uint32_t bitpos;
    uint8_t  lut[32];
    uint32_t f2;
} uvcx_mx_lumadns_data_t;

typedef struct _uvcx_mx_lumadns_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_mx_lumadns_data_t cur;
    uvcx_mx_lumadns_data_t min;
    uvcx_mx_lumadns_data_t max;
    uvcx_mx_lumadns_data_t res;
    uvcx_mx_lumadns_data_t def;
} uvcx_mx_lumadns_ctrl_t;
////////////////////////////////////////////////////////////////////////////////
//[luma denoise ref]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_lumadnsref_data_t
{
    float angle_of_view;
    float gamma;
    uint64_t  lutDist[256/8];
    uint8_t   lutGamma0_32[9];
    uint8_t   lutGamma32_255[9];
    uint32_t  shift;
} uvcx_mx_lumadnsref_data_t;

typedef struct _uvcx_mx_lumadnsref_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_mx_lumadnsref_data_t cur;
    uvcx_mx_lumadnsref_data_t min;
    uvcx_mx_lumadnsref_data_t max;
    uvcx_mx_lumadnsref_data_t res;
    uvcx_mx_lumadnsref_data_t def;
} uvcx_mx_lumadnsref_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Local tone mapping]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_ltm__data_t
{
    uint32_t thr;
    uint16_t table[MAX_LTM_CURVES];
} uvcx_mx_ltm_data_t;

typedef struct _uvcx_mx_ltm_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_mx_ltm_data_t cur;
    uvcx_mx_ltm_data_t min;
    uvcx_mx_ltm_data_t max;
    uvcx_mx_ltm_data_t res;
    uvcx_mx_ltm_data_t def;
} uvcx_mx_ltm_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Dog denoise]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_dogdenoise_data_t
{
    uint32_t thr;
    uint32_t strength;
    float sigma11;
    float sigma15;
    uint8_t  coeffs11[6];
    uint8_t  coeffs15[8];
} uvcx_mx_dogdenoise_data_t;

typedef struct _uvcx_mx_dogdenoise_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_mx_dogdenoise_data_t cur;
    uvcx_mx_dogdenoise_data_t min;
    uvcx_mx_dogdenoise_data_t max;
    uvcx_mx_dogdenoise_data_t res;
    uvcx_mx_dogdenoise_data_t def;
} uvcx_mx_dogdenoise_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Chroma denoise]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_chromadenoise_data_t
{
    uint32_t    thrRDark;
    uint32_t    thrGDark;
    uint32_t    thrBDark;
    uint32_t    thrLumaDark;
    uint32_t    thrRBright;
    uint32_t    thrGBright;
    uint32_t    thrBBright;
    uint32_t    thrLumaBright;
    uint32_t    denoiseSlope;
    int32_t     denoiseOffset;
    uint32_t    greyCr;
    uint32_t    greyCg;
    uint32_t    greyCb;
    uint32_t    greyDesatSlope;
    int32_t     greyDesatOffset;
    uint32_t    limit;
    uint32_t    hEnab;
    uint32_t    convCoeffCenter;
    uint32_t    convCoeffEdge;
    uint32_t    convCoeffCorner;
} uvcx_mx_chromadenoise_data_t;


typedef struct _uvcx_mx_chromadenoise_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_mx_chromadenoise_data_t cur;
    uvcx_mx_chromadenoise_data_t min;
    uvcx_mx_chromadenoise_data_t max;
    uvcx_mx_chromadenoise_data_t res;
    uvcx_mx_chromadenoise_data_t def;
} uvcx_mx_chromadenoise_ctrl_t;
////////////////////////////////////////////////////////////////////////////////
//[Color combine]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_colorcombine_data_t
{
    float ccm[9];
    float ccmoff[3];
    uint32_t kr;
    uint32_t kg;
    uint32_t kb;
} uvcx_mx_colorcombine_data_t;


typedef struct _uvcx_mx_colorcombine_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_mx_colorcombine_data_t cur;
    uvcx_mx_colorcombine_data_t min;
    uvcx_mx_colorcombine_data_t max;
    uvcx_mx_colorcombine_data_t res;
    uvcx_mx_colorcombine_data_t def;
} uvcx_mx_colorcombine_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Lut3d table]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_lut3dtable_data_t
{
    uint16_t lut3dTable[MAX_LTM_SIZE];
} uvcx_mx_lut3dtable_data_t;

typedef struct _uvcx_mx_lut3dtable_ctrl_t
{
    uint8_t    u8Info;
    uint16_t   u16Len;
    uvcx_mx_lut3dtable_data_t cur;
    uvcx_mx_lut3dtable_data_t min;
    uvcx_mx_lut3dtable_data_t max;
    uvcx_mx_lut3dtable_data_t res;
    uvcx_mx_lut3dtable_data_t def;
} uvcx_mx_lut3dtable_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Lut3d table transfer]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_lut3dtabletransfer_data_t
{
    uint32_t header[2];
    uint8_t  payload[TABLE_CHUNK];
} uvcx_mx_lut3dtabletransfer_data_t;

typedef struct _uvcx_mx_lut3dtabletransfer_ctrl_t
{
    uint8_t    u8Info;
    uint16_t   u16Len;
    uvcx_mx_lut3dtabletransfer_data_t cur;
    uvcx_mx_lut3dtabletransfer_data_t min;
    uvcx_mx_lut3dtabletransfer_data_t max;
    uvcx_mx_lut3dtabletransfer_data_t res;
    uvcx_mx_lut3dtabletransfer_data_t def;
} uvcx_mx_lut3dtabletransfer_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Color convert]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_colorconvert_data_t
{
    float mat[9];
    float off[3];
} uvcx_mx_colorconvert_data_t;


typedef struct _uvcx_mx_colorconvert_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_mx_colorconvert_data_t cur;
    uvcx_mx_colorconvert_data_t min;
    uvcx_mx_colorconvert_data_t max;
    uvcx_mx_colorconvert_data_t res;
    uvcx_mx_colorconvert_data_t def;
} uvcx_mx_colorconvert_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Gamma]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_gamma_data_t
{
    uint32_t    size;
} uvcx_mx_gamma_data_t;


typedef struct _uvcx_mx_gamma_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_mx_gamma_data_t cur;
    uvcx_mx_gamma_data_t min;
    uvcx_mx_gamma_data_t max;
    uvcx_mx_gamma_data_t res;
    uvcx_mx_gamma_data_t def;
} uvcx_mx_gamma_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Gamma table]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_gammatable_data_t
{
    uint16_t gammaTable[MAX_GAMMA_SIZE];
} uvcx_mx_gammatable_data_t;

typedef struct _uvcx_mx_gammatable_ctrl_t
{
    uint8_t    u8Info;
    uint16_t   u16Len;
    uvcx_mx_gammatable_data_t cur;
    uvcx_mx_gammatable_data_t min;
    uvcx_mx_gammatable_data_t max;
    uvcx_mx_gammatable_data_t res;
    uvcx_mx_gammatable_data_t def;
} uvcx_mx_gammatable_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Gamma table transfer]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_mx_gammatabletransfer_data_t
{
    uint32_t header[4];
    uint8_t  payload[TABLE_CHUNK_SIZE];
} uvcx_mx_gammatabletransfer_data_t;

typedef struct _uvcx_mx_gammatabletransfer_ctrl_t
{
    uint8_t    u8Info;
    uint16_t   u16Len;
    uvcx_mx_gammatabletransfer_data_t cur;
    uvcx_mx_gammatabletransfer_data_t min;
    uvcx_mx_gammatabletransfer_data_t max;
    uvcx_mx_gammatabletransfer_data_t res;
    uvcx_mx_gammatabletransfer_data_t def;
} uvcx_mx_gammatabletransfer_ctrl_t;

void Init_MX_Controls();
void onEventMX(uint8_t controlSelector, uint8_t request, uint16_t *pLen, uint8_t *pPayload);
void getConfigMX();


#endif //ISPXUCONTROLSMX_H_
