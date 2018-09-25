#ifndef ISPXUCONTROLSM2_H_
#define ISPXUCONTROLSM2_H_

#include "ispXUControlsCommon.h"
#include "IspCommon.h"

////////////////////////////////////////////////////////////////////////////////
//[General]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_general_data_t
{
    uint8_t    fileName[MAX_FILENAME_LEN];
    uint32_t   image_width;
    uint32_t   image_height;
    uint32_t   skip_lines;
    uint32_t   bits;
    uint32_t   bayer_order;
} uvcx_m2_general_data_t;

typedef struct _uvcx_m2_general_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_general_data_t cur;
    uvcx_m2_general_data_t min;
    uvcx_m2_general_data_t max;
    uvcx_m2_general_data_t res;
    uvcx_m2_general_data_t def;

} uvcx_m2_general_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Env]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_enviroment_data_t
{
    uint32_t    exp;
    uint32_t    gain;
    uint32_t    color_temp;
} uvcx_m2_enviroment_data_t;

typedef struct _uvcx_m2_enviroment_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_enviroment_data_t cur;
    uvcx_m2_enviroment_data_t min;
    uvcx_m2_enviroment_data_t max;
    uvcx_m2_enviroment_data_t res;
    uvcx_m2_enviroment_data_t def;

} uvcx_m2_enviroment_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Black level correction]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_blc_data_t
{
    uint32_t gr;
    uint32_t r;
    uint32_t b;
    uint32_t gb;
} uvcx_m2_blc_data_t;

typedef struct _uvcx_m2_blc_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_blc_data_t cur;
    uvcx_m2_blc_data_t min;
    uvcx_m2_blc_data_t max;
    uvcx_m2_blc_data_t res;
    uvcx_m2_blc_data_t def;

} uvcx_m2_blc_ctrl_t;
////////////////////////////////////////////////////////////////////////////////
//[Sigma denoise]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_sgmdns_data_t
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
} uvcx_m2_sgmdns_data_t;

typedef struct _uvcx_m2_sgmdns_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_sgmdns_data_t cur;
    uvcx_m2_sgmdns_data_t min;
    uvcx_m2_sgmdns_data_t max;
    uvcx_m2_sgmdns_data_t res;
    uvcx_m2_sgmdns_data_t def;

} uvcx_m2_sgmdns_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Lsc]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_lsc_data_t
{
    uint32_t    lscWidth;
    uint32_t    lscHeight;
    uint32_t    lscStride;
} uvcx_m2_lsc_data_t;

typedef struct _uvcx_m2_lsc_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_lsc_data_t cur;
    uvcx_m2_lsc_data_t min;
    uvcx_m2_lsc_data_t max;
    uvcx_m2_lsc_data_t res;
    uvcx_m2_lsc_data_t def;
} uvcx_m2_lsc_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Lsc table]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_lsctable_data_t
{
    uint16_t lscTable[MAX_LSC_SIZE] __attribute__((aligned(8)));
} uvcx_m2_lsctable_data_t;

typedef struct _uvcx_m2_lsctable_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_lsctable_data_t cur;
    uvcx_m2_lsctable_data_t min;
    uvcx_m2_lsctable_data_t max;
    uvcx_m2_lsctable_data_t res;
    uvcx_m2_lsctable_data_t def;
} uvcx_m2_lsctable_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Lsc table transfer]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_lsctabletransfer_data_t
{
    uint32_t header[2];
    uint8_t  payload[TABLE_CHUNK];
} uvcx_m2_lsctabletransfer_data_t;

typedef struct _uvcx_m2_lsctabletransfer_ctrl_t
{
    uint8_t    u8Info;
    uint16_t   u16Len;
    uvcx_m2_lsctabletransfer_data_t cur;
    uvcx_m2_lsctabletransfer_data_t min;
    uvcx_m2_lsctabletransfer_data_t max;
    uvcx_m2_lsctabletransfer_data_t res;
    uvcx_m2_lsctabletransfer_data_t def;
} uvcx_m2_lsctabletransfer_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Raw]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_raw_data_t
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
} uvcx_m2_raw_data_t;

typedef struct _uvcx_m2_raw_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_m2_raw_data_t cur;
    uvcx_m2_raw_data_t min;
    uvcx_m2_raw_data_t max;
    uvcx_m2_raw_data_t res;
    uvcx_m2_raw_data_t def;
} uvcx_m2_raw_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Demosaic]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_demosaic_data_t
{
    int32_t    dewormGradientMul;
    uint32_t   dewormSlope;
    int32_t    dewormOffset;
    int32_t    lumaWeightR;
    int32_t    lumaWeightG;
    int32_t    lumaWeightB;
} uvcx_m2_demosaic_data_t;

typedef struct _uvcx_m2_demosaic_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_m2_demosaic_data_t cur;
    uvcx_m2_demosaic_data_t min;
    uvcx_m2_demosaic_data_t max;
    uvcx_m2_demosaic_data_t res;
    uvcx_m2_demosaic_data_t def;
} uvcx_m2_demosaic_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[chroma generation]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_chromageneration_data_t
{
    uint32_t   epsilon;
    uint32_t   kr;
    uint32_t   kg;
    uint32_t   kb;
    uint32_t   lumaCoeffR;
    uint32_t   lumaCoeffG;
    uint32_t   lumaCoeffB;
    uint32_t   pfrStrength;
    int32_t    desatOffset;
    uint32_t   desatSlope;
} uvcx_m2_chromageneration_data_t;

typedef struct _uvcx_m2_chromageneration_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_chromageneration_data_t cur;
    uvcx_m2_chromageneration_data_t min;
    uvcx_m2_chromageneration_data_t max;
    uvcx_m2_chromageneration_data_t res;
    uvcx_m2_chromageneration_data_t def;
} uvcx_m2_chromageneration_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[median]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_median_data_t
{
    uint32_t   kernelSize;
    uint32_t   slope;
    int32_t   offset;
} uvcx_m2_median_data_t;

typedef struct _uvcx_m2_median_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_median_data_t cur;
    uvcx_m2_median_data_t min;
    uvcx_m2_median_data_t max;
    uvcx_m2_median_data_t res;
    uvcx_m2_median_data_t def;
} uvcx_m2_median_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[sharpen]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_sharpen_data_t
{
    float   sigma;
    uint16_t sharpenCoeffs[4];
    uint16_t strengthDarken;
    uint16_t strengthLighten;
    uint16_t alpha;
    uint16_t overshoot;
    uint16_t undershoot;
    uint16_t rangeStop0;
    uint16_t rangeStop1;
    uint16_t rangeStop2;
    uint16_t rangeStop3;
    uint16_t minThr;
} uvcx_m2_sharpen_data_t;

typedef struct _uvcx_m2_sharpen_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_sharpen_data_t cur;
    uvcx_m2_sharpen_data_t min;
    uvcx_m2_sharpen_data_t max;
    uvcx_m2_sharpen_data_t res;
    uvcx_m2_sharpen_data_t def;
} uvcx_m2_sharpen_ctrl_t;
////////////////////////////////////////////////////////////////////////////////
//[luma denoise]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_lumadns_data_t
{
    float strength;
    uint32_t alpha;
    uint32_t  bitpos;
    uint8_t   lut[32];
    uint32_t f2;
} uvcx_m2_lumadns_data_t;

typedef struct _uvcx_m2_lumadns_ctrl_t
{
    uint8_t u8Info;
    uint16_t u16Len;
    uvcx_m2_lumadns_data_t cur;
    uvcx_m2_lumadns_data_t min;
    uvcx_m2_lumadns_data_t max;
    uvcx_m2_lumadns_data_t res;
    uvcx_m2_lumadns_data_t def;
} uvcx_m2_lumadns_ctrl_t;
////////////////////////////////////////////////////////////////////////////////
//[luma denoise ref]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_lumadnsref_data_t
{
    float angle_of_view;
    float gamma;
    uint64_t  lutDist[256/8];
    uint8_t   lutGamma0_32[9];
    uint8_t   lutGamma32_255[9];
    uint32_t  shift;
} uvcx_m2_lumadnsref_data_t;

typedef struct _uvcx_m2_lumadnsref_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_lumadnsref_data_t cur;
    uvcx_m2_lumadnsref_data_t min;
    uvcx_m2_lumadnsref_data_t max;
    uvcx_m2_lumadnsref_data_t res;
    uvcx_m2_lumadnsref_data_t def;
} uvcx_m2_lumadnsref_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Local tone mapping]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_ltm__data_t
{
    uint32_t thr;
    uint16_t table[MAX_LTM_CURVES];
} uvcx_m2_ltm_data_t;

typedef struct _uvcx_m2_ltm_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_ltm_data_t cur;
    uvcx_m2_ltm_data_t min;
    uvcx_m2_ltm_data_t max;
    uvcx_m2_ltm_data_t res;
    uvcx_m2_ltm_data_t def;
} uvcx_m2_ltm_ctrl_t;
////////////////////////////////////////////////////////////////////////////////
//[Dog denoise]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_dogdenoise_data_t
{
    uint32_t thr;
    uint32_t strength;
    float sigma11;
    float sigma15;
    uint8_t  coeffs11[6];
    uint8_t  coeffs15[8];
} uvcx_m2_dogdenoise_data_t;

typedef struct _uvcx_m2_dogdenoise_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_dogdenoise_data_t cur;
    uvcx_m2_dogdenoise_data_t min;
    uvcx_m2_dogdenoise_data_t max;
    uvcx_m2_dogdenoise_data_t res;
    uvcx_m2_dogdenoise_data_t def;
} uvcx_m2_dogdenoise_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Chroma denoise]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_chromadenoise_data_t
{
    uint32_t thr_red;
    uint32_t thr_green;
    uint32_t thr_blue;
    uint32_t limit;
    uint32_t hEnable;
    uint32_t greydesat_slope;
    int32_t  greydesat_offset;
    uint32_t greyC_red;
    uint32_t greyC_green;
    uint32_t greyC_blue;
    uint32_t convCoeff_center;
    uint32_t convCoeff_edge;
    uint32_t convCoeff_corner;
} uvcx_m2_chromadenoise_data_t;


typedef struct _uvcx_m2_chromadenoise_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_chromadenoise_data_t cur;
    uvcx_m2_chromadenoise_data_t min;
    uvcx_m2_chromadenoise_data_t max;
    uvcx_m2_chromadenoise_data_t res;
    uvcx_m2_chromadenoise_data_t def;
} uvcx_m2_chromadenoise_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Color combine]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_colorcombine_data_t
{
    float ccm[9];
    float ccmoff[3];
    uint32_t kr;
    uint32_t kg;
    uint32_t kb;
} uvcx_m2_colorcombine_data_t;


typedef struct _uvcx_m2_colorcombine_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_colorcombine_data_t cur;
    uvcx_m2_colorcombine_data_t min;
    uvcx_m2_colorcombine_data_t max;
    uvcx_m2_colorcombine_data_t res;
    uvcx_m2_colorcombine_data_t def;
} uvcx_m2_colorcombine_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Lut3d table]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_lut3dtable_data_t
{
    uint16_t lut3dTable[MAX_LTM_SIZE];
} uvcx_m2_lut3dtable_data_t;

typedef struct _uvcx_m2_lut3dtable_ctrl_t
{
    uint8_t    u8Info;
    uint16_t   u16Len;
    uvcx_m2_lut3dtable_data_t cur;
    uvcx_m2_lut3dtable_data_t min;
    uvcx_m2_lut3dtable_data_t max;
    uvcx_m2_lut3dtable_data_t res;
    uvcx_m2_lut3dtable_data_t def;
} uvcx_m2_lut3dtable_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Lut3d table transfer]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_lut3dtabletransfer_data_t
{
    uint32_t header[4];
    uint8_t  payload[TABLE_CHUNK_SIZE];
} uvcx_m2_lut3dtabletransfer_data_t;

typedef struct _uvcx_m2_lut3dtabletransfer_ctrl_t
{
    uint8_t    u8Info;
    uint16_t   u16Len;
    uvcx_m2_lut3dtabletransfer_data_t cur;
    uvcx_m2_lut3dtabletransfer_data_t min;
    uvcx_m2_lut3dtabletransfer_data_t max;
    uvcx_m2_lut3dtabletransfer_data_t res;
    uvcx_m2_lut3dtabletransfer_data_t def;
} uvcx_m2_lut3dtabletransfer_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Color convert]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_colorconvert_data_t
{
    float mat[9];
    float off[3];
} uvcx_m2_colorconvert_data_t;


typedef struct _uvcx_m2_colorconvert_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_colorconvert_data_t cur;
    uvcx_m2_colorconvert_data_t min;
    uvcx_m2_colorconvert_data_t max;
    uvcx_m2_colorconvert_data_t res;
    uvcx_m2_colorconvert_data_t def;
} uvcx_m2_colorconvert_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Gamma]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_gamma_data_t
{
	uint32_t    size;
} uvcx_m2_gamma_data_t;


typedef struct _uvcx_m2_gamma_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_m2_gamma_data_t cur;
    uvcx_m2_gamma_data_t min;
    uvcx_m2_gamma_data_t max;
    uvcx_m2_gamma_data_t res;
    uvcx_m2_gamma_data_t def;
} uvcx_m2_gamma_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Gamma table]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_gammatable_data_t
{
    uint16_t gammaTable[MAX_GAMMA_SIZE];
} uvcx_m2_gammatable_data_t;

typedef struct _uvcx_m2_gammatable_ctrl_t
{
    uint8_t    u8Info;
    uint16_t   u16Len;
    uvcx_m2_gammatable_data_t cur;
    uvcx_m2_gammatable_data_t min;
    uvcx_m2_gammatable_data_t max;
    uvcx_m2_gammatable_data_t res;
    uvcx_m2_gammatable_data_t def;
} uvcx_m2_gammatable_ctrl_t;

////////////////////////////////////////////////////////////////////////////////
//[Gamma table transfer]
////////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_m2_gammatabletransfer_data_t
{
    uint32_t header[4];
    uint8_t  payload[TABLE_CHUNK_SIZE];
} uvcx_m2_gammatabletransfer_data_t;

typedef struct _uvcx_m2_gammatabletransfer_ctrl_t
{
    uint8_t    u8Info;
    uint16_t   u16Len;
    uvcx_m2_gammatabletransfer_data_t cur;
    uvcx_m2_gammatabletransfer_data_t min;
    uvcx_m2_gammatabletransfer_data_t max;
    uvcx_m2_gammatabletransfer_data_t res;
    uvcx_m2_gammatabletransfer_data_t def;
} uvcx_m2_gammatabletransfer_ctrl_t;

void Init_M2_Controls();
void onEventM2(uint8_t controlSelector, uint8_t request, uint16_t *pLen, uint8_t *pPayload);
void getConfigM2(icIspConfig *pCfg);

#endif //ISPXUCONTROLSM2_H_
