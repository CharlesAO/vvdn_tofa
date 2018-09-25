#ifndef __ISP_COMMON_H__
#define __ISP_COMMON_H__

#include "OpipeBlocks.h"

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

#endif
