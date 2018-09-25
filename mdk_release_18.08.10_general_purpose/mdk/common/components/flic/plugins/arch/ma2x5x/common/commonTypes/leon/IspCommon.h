///
/// @file      IspCommon.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IspCommon header.
///

#ifndef __ISP_COMMON_H__
#define __ISP_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#if(!defined(USE_ISP_WITH_SIPP) && !defined(USE_ISP_WITH_OPIPE))
    #define USE_ISP_WITH_OPIPE
#endif

#ifdef USE_ISP_WITH_OPIPE
#include "OpipeBlocks.h"
#endif
#ifdef USE_ISP_WITH_SIPP
#include "sipp_isp_types.h"
#endif

#include "ipipe.h"

#define IC_MAX_AEAWB_PATCHES    4096
#define IC_MAX_AF_PATCHES       4096

/* Main AE-AWB statistics data structure (patch based) */
typedef struct {
    AeAwbPatchStats     aeAwb[IC_MAX_AEAWB_PATCHES];
} icAeAwbStats;

/* Main AF statistics data structure (patch based) */
typedef struct {
    AfPatchStats        af[IC_MAX_AF_PATCHES];
} icAfStats;

typedef struct
{
    //uint32_t        ispId;
    uint32_t        frameCount;     /* Frame counter from the client */
    uint32_t        frameId;        /* Frame ID from the client */
    void            *userData;      /* Private data from the client */
    uint32_t        dirtyFlags;     /* OR'd IC_ISP_GROUP_* flags */
    uint32_t        enableFlags;    /* OR'd IC_ISP_ENABLE_* flags */
    uint32_t        pipeControl;    /* Pipeline Control - enable/disable various paths */
    int32_t         pipelineBits;   /* Number of bits in the data coming from the sensor */
    int32_t         bayerOrder;     /* Bayer order from sensor */

   //Bayer-domain
    BlcCfg          blc;
    SigmaDnsCfg     sigma;
    LscCfg          lsc;

    RawCfg          raw;
    AeAwbCfg        aeAwbConfig; //statistics
    AfCfg           afConfig;    //statistics

  //RAW-Stats get enabled if a non-null output pointer is associated
    AeAwbPatchStats *aeAwbStats;
    AfPatchStats    *afStats;
    uint32_t        *histLuma;
    uint32_t        *histRgb;

    DbyrCfg         demosaic;
    LtmCfg          ltm;
    DogCfg          dog;
    LumaDnsCfg      lumaDenoise;
    LumaDnsRefCfg   lumaDenoiseRef;
    SharpenCfg      sharpen;
    ChromaGenCfg    chromaGen;
    MedianCfg       median;
    ChromaDnsCfg    chromaDenoise;
    ColCombCfg      colorCombine;
    LutCfg          gamma;
    ColConvCfg      colorConvert;

    struct {
        uint32_t exp;
        uint32_t gain;
        uint32_t colour_temp;
    } env;
    UpfirdnCfg   updnCfg0;
    UpfirdnCfg   updnCfg12;
}icIspConfig;

#ifndef USE_ISP_WITH_OPIPE
// Pipeline configuration structure
typedef struct
{
  icIspConfig   ispCfg;
  uint16_t     *iBuf;
  uint8_t      *oBuf;

  /* Input/Output Frame sizes */
  struct {
    int     width;
    int     height;
  } iSize, oSize;
  // TODO clean up structure
  int         cropLeft;
  int         cropTop;
  int         cropRight;
  int         cropBottom;

  int         skipBytes;
  char        *inputFilename;

  // Add Raw filter stats addresses here to - should be passed in from app via this mechanism
  AeAwbPatchStats * aeStatsBuff;
  AfPatchStats *    afStatsBuff;
  uint8_t *         cc3DLut;

} PipeConfig;
#endif

#ifdef __cplusplus
}
#endif

#endif
