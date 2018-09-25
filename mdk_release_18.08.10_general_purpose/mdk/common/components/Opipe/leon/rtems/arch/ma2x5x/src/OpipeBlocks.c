///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Opipe - SIPP blocks programming routines
///

#include "Opipe.h"
#include "OpipeInternals.h"

//#################################################################################################
void progCmxBuff(uint32_t buffBase, uint32_t bufAddr, uint32_t lnStride, uint32_t height, uint32_t nPlanes, uint32_t format)
{
    SET_REG_WORD(buffBase + 0x00 /*base*/, bufAddr);
    SET_REG_WORD(buffBase + 0x04 /*cfg */, height                             //number of lines
                                           |(0           << SIPP_SC_OFFSET)   //0:fillCtrl; 1:sync mode
                                           |((nPlanes-1) << SIPP_NP_OFFSET)   //number of planes
                                           |( format     << SIPP_FO_OFFSET) );//format

    SET_REG_WORD(buffBase + 0x08 /*ls */,  lnStride);
    SET_REG_WORD(buffBase + 0x0C /*ps */,  lnStride * height);
    SET_REG_WORD(buffBase + 0x10 /*ir */,  0); //clear potential left-overs, relevant regs
                                   //will get set later via cfgSources/cfgSinks/cfgSwLinks
}

//#############################################################################################################
//Remap input params to outpts based on Bayer order
// In: [inGr inR  inB inGb] -> |out[0] out[1]|
//                             |out[2] out[3]|
//Usage scenarios: reorder blc.blackLevel values, raw.gainSat values
void orderPixels(uint32_t bayerPattern, uint32_t inGr, uint32_t inR, uint32_t inB, uint32_t inGb, uint32_t *out)
{
    switch (bayerPattern)
    {
      case GRBG:  out[0] = inGr;  out[1] = inR;   out[2] = inB;   out[3] = inGb; break;
      case GBRG:  out[0] = inGb;  out[1] = inB;   out[2] = inR;   out[3] = inGr; break;
      case BGGR:  out[0] = inB;   out[1] = inGb;  out[2] = inGr;  out[3] = inR;  break;
      case RGGB:  out[0] = inR;   out[1] = inGr;  out[2] = inGb;  out[3] = inB;  break;
    }
}

//#############################################################################################################
// _____       __          __              _   _
//|  __ \     /\ \        / /             | | | |
//| |__) |   /  \ \  /\  / /   _ __   __ _| |_| |__
//|  _  /   / /\ \ \/  \/ /   | '_ \ / _` | __| '_ \'
//| | \ \  / ____ \  /\  /    | |_) | (_| | |_| | | |
//|_|  \_\/_/    \_\/  \/     | .__/ \__,_|\__|_| |_|
//                            | |
//                            |_|
//#############################################################################################################

void cfgSigma(Opipe *pipe)
{
   uint32_t     b[4]; //black level
   SigmaDnsCfg *sigCfg = pipe->pSigmaCfg;
   BlcCfg      *blcCfg = pipe->pBlcCfg;

  //in multi-cam scenarios, Sigma will generally be reading data from CMX !
   SET_REG_WORD(I_CFG (SIPP_SIGMA_ID), (pipe->iPlanes[SIPP_SIGMA_ID]-1) << SIPP_NP_OFFSET);
   SET_REG_WORD(O_CFG (SIPP_SIGMA_ID), (pipe->iPlanes[SIPP_SIGMA_ID]-1) << SIPP_NP_OFFSET);

 //Filter specific registers
   SET_REG_WORD(SIPP_SIGMA_FRM_DIM_ADR     , (pipe->height<<16)|pipe->width);
   SET_REG_WORD(SIPP_SIGMA_THRESH_P0P1_ADR , sigCfg->thresh1P0       |
                                             sigCfg->thresh2P0 <<  8 |
                                             sigCfg->thresh1P1 << 16 |
                                             sigCfg->thresh2P1 << 24 );

   SET_REG_WORD(SIPP_SIGMA_THRESH_P2P3_ADR , sigCfg->thresh1P2       |
                                             sigCfg->thresh2P2 <<  8 |
                                             sigCfg->thresh1P3 << 16 |
                                             sigCfg->thresh2P3 << 24 );

   SET_REG_WORD(SIPP_SIGMA_CFG_ADR        ,  pipe->cfg[SIPP_SIGMA_ID]);

  //Figure out black level pixels order based on BayerPattern
   orderPixels(pipe->bayerPattern, blcCfg->gr, blcCfg->r, blcCfg->b, blcCfg->gb, b);
   SET_REG_WORD(SIPP_SIGMA_BLACK01_ADR, b[0] | b[1] << 16);
   SET_REG_WORD(SIPP_SIGMA_BLACK23_ADR, b[2] | b[3] << 16);
}

//#############################################################################################################
void cfgLsc(Opipe *pipe)
{
   LscCfg  *lscCfg = pipe->pLscCfg;
   uint32_t imgW   = pipe->width;
   uint32_t imgH   = pipe->height;
   uint32_t gmW    = lscCfg->lscWidth; //gain map W
   uint32_t gmH    = lscCfg->lscHeight;//gain map H
   uint32_t shift  = pipe->format; //0:Planar, 1:Bayer

 //LSC mesh is always in system memory (CMX/DDR)
   progCmxBuff(I_BASE(SIPP_LSC_GM_ID), (uint32_t)lscCfg->pLscTable, gmW*2, gmH, pipe->iPlanes[SIPP_LSC_ID], 2/*bpp 8.8 always*/);
   SET_REG_WORD(I_LS (SIPP_LSC_GM_ID), lscCfg->lscStride * 2); //patch line stride

   SET_REG_WORD(I_CFG (SIPP_LSC_ID), (pipe->iPlanes[SIPP_LSC_ID]-1) << SIPP_NP_OFFSET); //10bit
   SET_REG_WORD(O_CFG (SIPP_LSC_ID), (pipe->iPlanes[SIPP_LSC_ID]-1) << SIPP_NP_OFFSET); //10bit

 //Custom regs:
   SET_REG_WORD(SIPP_LSC_FRM_DIM_ADR  , (imgH<<16)|imgW);
   SET_REG_WORD(SIPP_LSC_FRACTION_ADR ,  (((uint32_t)(65536.0f*((gmH >> shift) - 1) / ((imgH>>shift) - 1))) << 16)
                                        |(((uint32_t)(65536.0f*((gmW >> shift) - 1) / ((imgW>>shift) - 1)))      ) );

   SET_REG_WORD(SIPP_LSC_GM_DIM_ADR   , (gmH<<16)|gmW);
   SET_REG_WORD(SIPP_LSC_CFG_ADR      ,    pipe->format               //format
                                       | ((pipe->rawBits - 1)<<4)  ); //bpp
}

//#################################################################################################
#define RAW_CFG_RGB_HIST_EN   (1<<24)
#define RAW_CFG_AF_STATS_EN   (1<<13)
#define RAW_CFG_LUMA_HIST_EN  (1<< 7)
#define RAW_CFG_AWB_STATS_EN  (1<< 6)

#define STATS_AE_BASE_ADR         O_BASE(SIPP_STATS_ID)
#define STATS_AF_BASE_ADR         O_BASE(SIPP_AF_STATS_ID)
#define STATS_HIST_LUMA_BASE_ADR  O_BASE(SIPP_LUMA_HIST_ID)
#define STATS_HIST_RGB_BASE_ADR   O_BASE(SIPP_RGB_HIST_ID)

static void getRawStatEn(Opipe *pipe)
{
  //Enable if config struct and output buffers are not NULL
   if((pipe->aeCfg) && (pipe->aeStats))  pipe->cfg[SIPP_RAW_ID] |= RAW_CFG_AWB_STATS_EN;
   if((pipe->afCfg) && (pipe->afStats))  pipe->cfg[SIPP_RAW_ID] |= RAW_CFG_AF_STATS_EN;
   if( pipe->histLuma                 )  pipe->cfg[SIPP_RAW_ID] |= RAW_CFG_LUMA_HIST_EN;
   if( pipe->histRgb                  )  pipe->cfg[SIPP_RAW_ID] |= RAW_CFG_RGB_HIST_EN;
}

//#################################################################################################
static void cfgRawStats(Opipe *pipe)
{
    AeAwbCfg   *aeCfg = pipe->aeCfg;
    AfCfg      *afCfg = pipe->afCfg;
    uint32_t    i;

   //=======================================================================
   //AE/AWB
   //=======================================================================
    if(pipe->cfg[SIPP_RAW_ID] & RAW_CFG_AWB_STATS_EN)
    {
        SET_REG_WORD(STATS_AE_BASE_ADR, pipe->aeStats); //output buff

        SET_REG_WORD(SIPP_STATS_PATCH_CFG_ADR, (aeCfg->nPatchesX  - 1)       |
                                               (aeCfg->nPatchesY  - 1) <<  8 |
                                               (aeCfg->patchWidth - 1) << 16 |
                                               (aeCfg->patchHeight- 1) << 24 );

        SET_REG_WORD(SIPP_STATS_PATCH_START_ADR, (aeCfg->firstPatchY    << 16) |  aeCfg->firstPatchX   );
        SET_REG_WORD(SIPP_STATS_PATCH_SKIP_ADR, ((aeCfg->patchGapY - 1) << 16) | (aeCfg->patchGapX - 1));

        SET_REG_WORD(SIPP_RAW_STATS_THRESHOLDS_ADR, (aeCfg->darkThresh <<16) | aeCfg->brightThresh);
    }

    //=======================================================================
    //AF
    //=======================================================================
    if(pipe->cfg[SIPP_RAW_ID] & RAW_CFG_AF_STATS_EN)
    {
        SET_REG_WORD(STATS_AF_BASE_ADR, pipe->afStats); //output buff

        for(i=0; i<11; i++) //11 coefs
        {
           SET_REG_WORD(SIPP_AF_F1_COEF_00_ADR + i*4, afCfg->f1Coeffs[i]);
           SET_REG_WORD(SIPP_AF_F2_COEF_00_ADR + i*4, afCfg->f2Coeffs[i]);
        }

        SET_REG_WORD(SIPP_AF_MIN_THRESHOLD_ADR,  ((afCfg->f2Threshold<<16) & 0xFFFF0000) |
                                                 ( afCfg->f1Threshold      & 0x0000FFFF) ); //0);
        SET_REG_WORD(SIPP_AF_SUBTRACT_ADR,        afCfg->initialSubtractionValue);          //512>>4);

        SET_REG_WORD(SIPP_AF_PATCH_CFG_ADR, ((afCfg->nPatchesX  - 1)      )|
                                            ((afCfg->nPatchesY  - 1) << 8 )|
                                            ((afCfg->patchWidth - 1) << 16)|
                                            ((afCfg->patchHeight- 1) << 24));

        SET_REG_WORD(SIPP_AF_PATCH_START_ADR, (afCfg->firstPatchX      )|
                                              (afCfg->firstPatchY << 16));
    }

    //=======================================================================
    //Histograms
    //=======================================================================
      if(pipe->cfg[SIPP_RAW_ID] & RAW_CFG_LUMA_HIST_EN) SET_REG_WORD(STATS_HIST_LUMA_BASE_ADR, pipe->histLuma);
      if(pipe->cfg[SIPP_RAW_ID] & RAW_CFG_RGB_HIST_EN ) SET_REG_WORD(STATS_HIST_RGB_BASE_ADR,  pipe->histRgb);
}

//#################################################################################################
void cfgRaw(Opipe *pipe)
{
   RawCfg  *rawCfg = pipe->pRawCfg;
   uint32_t gainSatGr, gainSatR, gainSatB, gainSatGb;
   uint32_t gainSat[4]; //what gets programmed in regs

   SET_REG_WORD(I_CFG (SIPP_RAW_ID), (pipe->iPlanes[SIPP_RAW_ID]-1) << SIPP_NP_OFFSET);
   SET_REG_WORD(O_CFG (SIPP_RAW_ID), (pipe->iPlanes[SIPP_RAW_ID]-1) << SIPP_NP_OFFSET);

 //Adjust cfg with stats enables
   getRawStatEn(pipe);
 //Filter specific registers
   SET_REG_WORD(SIPP_RAW_FRM_DIM_ADR,  (pipe->height<<16)|pipe->width);
   SET_REG_WORD(SIPP_RAW_CFG_ADR    ,   pipe->cfg[SIPP_RAW_ID]);

  //Reorder Gain/Sat values based on BayerPattern
   gainSatGr = (rawCfg->clampGr << 16) | rawCfg->gainGr;
   gainSatR  = (rawCfg->clampR  << 16) | rawCfg->gainR;
   gainSatB  = (rawCfg->clampB  << 16) | rawCfg->gainB;
   gainSatGb = (rawCfg->clampGb << 16) | rawCfg->gainGb;
   orderPixels(pipe->bayerPattern, gainSatGr, gainSatR, gainSatB, gainSatGb, gainSat);

   SET_REG_WORD(SIPP_GAIN_SATURATE_0_ADR, gainSat[0]);
   SET_REG_WORD(SIPP_GAIN_SATURATE_1_ADR, gainSat[1]);
   SET_REG_WORD(SIPP_GAIN_SATURATE_2_ADR, gainSat[2]);
   SET_REG_WORD(SIPP_GAIN_SATURATE_3_ADR, gainSat[3]);

   SET_REG_WORD(SIPP_BAD_PIXEL_CFG_ADR,   rawCfg->dpcNoiseLevel << 16
                                        | rawCfg->dpcAlphaHotG  << 12
                                        | rawCfg->dpcAlphaColdG <<  8
                                        | rawCfg->dpcAlphaHotRb <<  4
                                        | rawCfg->dpcAlphaColdRb);

   SET_REG_WORD(SIPP_GRGB_PLATO_ADR,   rawCfg->grgbImbalPlatBright  << 16 | rawCfg->grgbImbalPlatDark);
   SET_REG_WORD(SIPP_GRGB_SLOPE_ADR,   rawCfg->grgbImbalDecayBright << 16 | rawCfg->grgbImbalDecayDark);

   //Stats are too big, have separate func
   cfgRawStats(pipe);
}

//#################################################################################################
void cfgDbyr(Opipe *pipe)
{
   DbyrCfg  *dbyrCfg = pipe->pDbyrCfg;

   SET_REG_WORD(I_CFG (SIPP_DBYR_ID), (pipe->iPlanes[SIPP_DBYR_ID]-1) << SIPP_NP_OFFSET); //1pl, 10bit
   SET_REG_WORD(O_CFG (SIPP_DBYR_ID), (pipe->oPlanes[SIPP_DBYR_ID]-1) << SIPP_NP_OFFSET); //3pl, 12bit

 //Custom regs:
   SET_REG_WORD(SIPP_DBYR_FRM_DIM_ADR,  (pipe->height<<16)|pipe->width);    // Input resolution !!!

   SET_REG_WORD(SIPP_DBYR_CFG_ADR    ,   pipe->cfg[SIPP_DBYR_ID]);
   SET_REG_WORD(SIPP_DBYR_THRES_ADR  ,   (1<<25)  //en RGB out
                                       | (1<<26)  //en Luma out
                                       | (1<<27)  //wr Y data to CMX
                                       | ((pipe->flags & GEN_PREVIEW ? 1:0)<<28));//preview Gen

  //just in case luma gen is enabled...
   SET_REG_WORD(SIPP_DBYR_LUMA_COEFS_ADR,  dbyrCfg->lumaWeightR  << 16 |
                                           dbyrCfg->lumaWeightG  <<  8 |
                                           dbyrCfg->lumaWeightB        );

   SET_REG_WORD(SIPP_DBYR_DEWORM_ADR , dbyrCfg->dewormSlope | dbyrCfg->dewormOffset << 16);
}


//#############################################################################################################
//  _                                         _   _
// | |                                       | | | |
// | |    _   _ _ __ ___   __ _   _ __   __ _| |_| |__
// | |   | | | | '_ ` _ \ / _` | | '_ \ / _` | __| '_ \'
// | |___| |_| | | | | | | (_| | | |_) | (_| | |_| | | |
// |______\__,_|_| |_| |_|\__,_| | .__/ \__,_|\__|_| |_|
//                               | |
//                               |_|
//#############################################################################################################
void cfgDogLtm(Opipe *pipe)
{
   LtmCfg *ltmCfg = pipe->pLtmCfg;
   DogCfg *dogCfg = pipe->pDogCfg;

   int curve, point, regoff;

   SET_REG_WORD(I_CFG(SIPP_DOGL_ID), (pipe->iPlanes[SIPP_DOGL_ID]-1) << SIPP_NP_OFFSET); //1pl, fp16
   SET_REG_WORD(O_CFG(SIPP_DOGL_ID), (pipe->iPlanes[SIPP_DOGL_ID]-1) << SIPP_NP_OFFSET); //1pl, fp16

 //Custom regs:
 //TBD: if Dbyr mask is used and it's in preview mode, halve resolution !!!
   SET_REG_WORD(SIPP_DOGL_FRM_DIM_ADR, (pipe->height2<<16)|pipe->width2);
   SET_REG_WORD(SIPP_DOGL_CFG_ADR    ,  pipe->cfg[SIPP_DOGL_ID]);

   SET_REG_WORD(SIPP_DOGL_COEF0_ADR, dogCfg->coeffs11[0]       |
                                     dogCfg->coeffs11[1] <<  8 |
                                     dogCfg->coeffs11[2] << 16 |
                                     dogCfg->coeffs11[3] << 24 );

   SET_REG_WORD(SIPP_DOGL_COEF1_ADR, dogCfg->coeffs11[4]       |
                                     dogCfg->coeffs11[5] <<  8 |
                                     dogCfg->coeffs15[0] << 16 |
                                     dogCfg->coeffs15[1] << 24 );

   SET_REG_WORD(SIPP_DOGL_COEF2_ADR, dogCfg->coeffs15[2]       |
                                     dogCfg->coeffs15[3] <<  8 |
                                     dogCfg->coeffs15[4] << 16 |
                                     dogCfg->coeffs15[5] << 24 );

   SET_REG_WORD(SIPP_DOGL_COEF3_ADR, dogCfg->coeffs15[6]       |
                                     dogCfg->coeffs15[7] <<  8 |
                                     dogCfg->strength    << 16);

   regoff = 0;
   for (curve = 0; curve < 8; curve++) {
        for (point = 0; point < 16; point += 2) {
            int off = curve + point * 8;
            int a   = ltmCfg->curves[off];
            int b   = ltmCfg->curves[off+8];
            SET_REG_WORD(SIPP_LTM_COEF_ADR+regoff, (a & 0xfff) | (b & 0xfff) << 16);
            regoff += 4;
        }
    }
}

//#################################################################################################
void cfgLumaDns(Opipe *pipe)
{
   uint32_t data;
   LumaDnsCfg    *dnsCfg    = pipe->pLumaDnsCfg;
   LumaDnsRefCfg *dnsRefCfg = pipe->pLumaDnsRefCfg;

   SET_REG_WORD(I_CFG(SIPP_LUMA_ID), (pipe->iPlanes[SIPP_LUMA_ID]-1) << SIPP_NP_OFFSET); //1pl, fp16
   SET_REG_WORD(O_CFG(SIPP_LUMA_ID), (pipe->iPlanes[SIPP_LUMA_ID]-1) << SIPP_NP_OFFSET); //1pl, fp16

 //Filter specific registers
   SET_REG_WORD(SIPP_LUMA_FRM_DIM_ADR,      (pipe->height2<<16)|pipe->width2);
   SET_REG_WORD(I_BASE(SIPP_LUMA_C4LUT_ID), (uint32_t)dnsRefCfg->lutDist);

   SET_REG_WORD(SIPP_LUMA_CFG_ADR    ,     dnsCfg->bitpos         |
                                           dnsCfg->alpha    <<  8 |
                                           dnsRefCfg->shift << 16 );

  //and kick initial load of distance-lut
   data = GET_REG_WORD_VAL(SIPP_LUMA_CFG_ADR);
   SET_REG_WORD(SIPP_LUMA_CFG_ADR, data & (~(1<<24)));
   SET_REG_WORD(SIPP_LUMA_CFG_ADR, data | (  1<<24) );

   SET_REG_WORD(SIPP_LUMA_LUT0700_ADR,  dnsCfg->lut[0]       | dnsCfg->lut[1] <<  4 |
                                        dnsCfg->lut[2] <<  8 | dnsCfg->lut[3] << 12 |
                                        dnsCfg->lut[4] << 16 | dnsCfg->lut[5] << 20 |
                                        dnsCfg->lut[6] << 24 | dnsCfg->lut[7] << 28 );

   SET_REG_WORD(SIPP_LUMA_LUT1508_ADR,  dnsCfg->lut[8]        | dnsCfg->lut[ 9] <<  4 |
                                        dnsCfg->lut[10] <<  8 | dnsCfg->lut[11] << 12 |
                                        dnsCfg->lut[12] << 16 | dnsCfg->lut[13] << 20 |
                                        dnsCfg->lut[14] << 24 | dnsCfg->lut[15] << 28);

   SET_REG_WORD(SIPP_LUMA_LUT2316_ADR,  dnsCfg->lut[16]       | dnsCfg->lut[17] <<  4 |
                                        dnsCfg->lut[18] <<  8 | dnsCfg->lut[19] << 12 |
                                        dnsCfg->lut[20] << 16 | dnsCfg->lut[21] << 20 |
                                        dnsCfg->lut[22] << 24 | dnsCfg->lut[23] << 28);

   SET_REG_WORD(SIPP_LUMA_LUT3124_ADR,  dnsCfg->lut[24]       | dnsCfg->lut[25] <<  4 |
                                        dnsCfg->lut[26] <<  8 | dnsCfg->lut[27] << 12 |
                                        dnsCfg->lut[28] << 16 | dnsCfg->lut[29] << 20 |
                                        dnsCfg->lut[30] << 24 | dnsCfg->lut[31] << 28);

   SET_REG_WORD(SIPP_LUMA_F2LUT_ADR  ,  dnsCfg->f2);

   SET_REG_WORD(SIPP_LUMA_GAMMA_LUT0_ADR,  dnsRefCfg->lutGamma0_32[0]       | dnsRefCfg->lutGamma0_32[1] <<  8 |
                                           dnsRefCfg->lutGamma0_32[2] << 16 | dnsRefCfg->lutGamma0_32[3] << 24);
   SET_REG_WORD(SIPP_LUMA_GAMMA_LUT1_ADR,  dnsRefCfg->lutGamma0_32[4]       | dnsRefCfg->lutGamma0_32[5] <<  8 |
                                           dnsRefCfg->lutGamma0_32[6] << 16 | dnsRefCfg->lutGamma0_32[7] << 24);

   SET_REG_WORD(SIPP_LUMA_GAMMA_LUT2_ADR,  dnsRefCfg->lutGamma0_32[8]         | dnsRefCfg->lutGamma32_255[0] << 8 |
                                           dnsRefCfg->lutGamma32_255[1] << 16 | dnsRefCfg->lutGamma32_255[2] << 24);
   SET_REG_WORD(SIPP_LUMA_GAMMA_LUT3_ADR,  dnsRefCfg->lutGamma32_255[3]       | dnsRefCfg->lutGamma32_255[4] << 8 |
                                           dnsRefCfg->lutGamma32_255[5] << 16 | dnsRefCfg->lutGamma32_255[6] << 24);
   SET_REG_WORD(SIPP_LUMA_GAMMA_LUT4_ADR,  dnsRefCfg->lutGamma32_255[7]       | dnsRefCfg->lutGamma32_255[8] << 8);

 //
   SET_REG_WORD(SIPP_LUMA_DIST_OFFSETS_ADR, (pipe->offY <<16) | pipe->offX );
   SET_REG_WORD(SIPP_LUMA_FULL_FRM_DIM_ADR, (pipe->fullH<<16) | pipe->fullW);
}

//#################################################################################################
void cfgSharpen(Opipe *pipe)
{
   SharpenCfg *cfg = pipe->pSharpCfg;

 //http://dub30/bugzilla/show_bug.cgi?id=21574#c3
 //"lbuf.format : 0 (don't care, but must be ste for Sharpening filter)"
   SET_REG_WORD(I_CFG(SIPP_SHARPEN_ID), ((pipe->iPlanes[SIPP_SHARPEN_ID]-1) << SIPP_NP_OFFSET) | (2  << SIPP_FO_OFFSET)); //1pl, fp16
 //Note: Output is always in CMX !

 //Filter specific registers
   SET_REG_WORD(SIPP_SHARPEN_FRM_DIM_ADR     , (pipe->height2<<16)|pipe->width2);
   SET_REG_WORD(SIPP_SHARPEN_CFG_ADR         ,  pipe->cfg[SIPP_SHARPEN_ID]);

   SET_REG_WORD(SIPP_SHARPEN_STREN_ADR       , cfg->strengthDarken   << 16 | cfg->strengthLighten); //0 for pass through
   SET_REG_WORD(SIPP_SHARPEN_CLIP_ADR        , cfg->alpha);
   SET_REG_WORD(SIPP_SHARPEN_LIMIT_ADR       , cfg->overshoot        << 16 | cfg->undershoot);
   SET_REG_WORD(SIPP_SHARPEN_RANGETOP_1_0_ADR, cfg->rangeStop1       << 16 | cfg->rangeStop0);
   SET_REG_WORD(SIPP_SHARPEN_RANGETOP_3_2_ADR, cfg->rangeStop3       << 16 | cfg->rangeStop2);
   SET_REG_WORD(SIPP_SHARPEN_GAUSIAN_1_0_ADR,  cfg->sharpenCoeffs[1] << 16 | cfg->sharpenCoeffs[0]);
   SET_REG_WORD(SIPP_SHARPEN_GAUSIAN_3_2_ADR,  cfg->sharpenCoeffs[3] << 16 | cfg->sharpenCoeffs[2]);
}

//#############################################################################################################
//  _____ _                                             _   _
// / ____| |                                           | | | |
//| |    | |__  _ __ ___  _ __ ___   __ _   _ __   __ _| |_| |__
//| |    | '_ \| '__/ _ \| '_ ` _ \ / _` | | '_ \ / _` | __| '_ \'
//| |____| | | | | | (_) | | | | | | (_| | | |_) | (_| | |_| | | |
// \_____|_| |_|_|  \___/|_| |_| |_|\__,_| | .__/ \__,_|\__|_| |_|
//                                         | |
//                                         |_|
//#############################################################################################################
void cfgGenChr(Opipe *pipe)
{
   ChromaGenCfg *cfg = pipe->pChrGenCfg;

   SET_REG_WORD(I_CFG(SIPP_CGEN_ID), ((pipe->iPlanes[SIPP_CGEN_ID]-1) << SIPP_NP_OFFSET)); //3pl RGB, u8
   SET_REG_WORD(O_CFG(SIPP_CGEN_ID), ((pipe->oPlanes[SIPP_CGEN_ID]-1) << SIPP_NP_OFFSET)); //3pl Chroma, u8

 //Custom regs:
   SET_REG_WORD(SIPP_CGEN_FRM_DIM_ADR,     (pipe->height2<<16)|pipe->width2); //input resolution, omfg
   SET_REG_WORD(SIPP_CGEN_CFG_ADR     ,     pipe->cfg[SIPP_CGEN_ID]);

 //Luma = a*R + b*G + c*B, thus will be FF
   SET_REG_WORD(SIPP_CGEN_LUMA_COEFFS_ADR   ,  cfg->lumaCoeffR
                                             | cfg->lumaCoeffG <<  8
                                             | cfg->lumaCoeffB << 16);

 //R,G,B coefs are FF, thus out will be  X /ff * ff, so I expect passthrough
   SET_REG_WORD(SIPP_CGEN_GEN_CHR_COEFFS_ADR,   cfg->epsilon
                                              | cfg->kr      <<  8
                                              | cfg->kg      << 16
                                              | cfg->kb      << 24 );
}

//#################################################################################################
void cfgMedian(Opipe *pipe)
{
   MedianCfg *cfg = pipe->pMedCfg;

   SET_REG_WORD(I_CFG(SIPP_MED_ID), ((pipe->iPlanes[SIPP_MED_ID]-1) << SIPP_NP_OFFSET)); //3pl, u8
   SET_REG_WORD(O_CFG(SIPP_MED_ID), ((pipe->iPlanes[SIPP_MED_ID]-1) << SIPP_NP_OFFSET)); //3pl, u8

 //Special config override for Med.luma: buffer is actually 1 plane, but filter
 //reads each line once for each plane of chroma, set plane stride to 0
   SET_REG_WORD(I_PS(SIPP_MED_LUMA_ID), 0);

 //Custom regs:
   SET_REG_WORD(SIPP_MED_FRM_DIM_ADR,   ((pipe->height2>>1)<<16) | (pipe->width2>>1));
   SET_REG_WORD(SIPP_MED_CFG_ADR    ,     pipe->cfg[SIPP_MED_ID]);
   SET_REG_WORD(SIPP_MED_LUMA_ALPHA_ADR, (cfg->slope  & 0xff) << 8 |
                                         (cfg->offset & 0xff)      );
}

//#################################################################################################
void cfgChromaDns(Opipe *pipe)
{
   ChromaDnsCfg *cfg = pipe->pChromaDnsCfg;

   SET_REG_WORD(I_CFG(SIPP_CHROMA_ID), ((pipe->iPlanes[SIPP_CHROMA_ID]-1) << SIPP_NP_OFFSET)); //3pl, u8
   SET_REG_WORD(O_CFG(SIPP_CHROMA_ID), ((pipe->iPlanes[SIPP_CHROMA_ID]-1) << SIPP_NP_OFFSET)); //3pl, u8

 //Custom regs:
 //TBD: make this flexible !!! default quarered resolution specific to Chroma path
   SET_REG_WORD(SIPP_CHROMA_FRM_DIM_ADR,  ((pipe->height2>>1)<<16) | (pipe->width2>>1));
   SET_REG_WORD(SIPP_CHROMA_CFG_ADR    ,    pipe->cfg[SIPP_CHROMA_ID]);

   SET_REG_WORD(SIPP_CHROMA_THRESH_ADR, cfg->th_r       |
                                        cfg->th_r << 16 |
                                        cfg->th_g <<  8 |
                                        cfg->th_g << 24);

   SET_REG_WORD(SIPP_CHROMA_THRESH2_ADR, cfg->th_b |
                                         cfg->th_b << 16);

   SET_REG_WORD(SIPP_CHROMA_GREY_POINT_ADR, cfg->greyCb       |
                                            cfg->greyCg <<  8 |
                                            cfg->greyCr << 16 );

   SET_REG_WORD(SIPP_CHROMA_CHROMA_COEFFS_ADR, cfg->convCoeffCenter       |
                                               cfg->convCoeffEdge   <<  8 |
                                               cfg->convCoeffCorner << 16 );
}

//#################################################################################################
void cfgColComb(Opipe *pipe)
{
   ColCombCfg *cfg = pipe->pColCombCfg;

 //Luma is always in CMX ...

 //Chroma comes in general from LLB
   SET_REG_WORD(I_CFG(SIPP_CC_CHROMA_ID), ((pipe->iPlanes[SIPP_CC_CHROMA_ID]-1) << SIPP_NP_OFFSET)); //3pl-Chroma, u8
   SET_REG_WORD(O_CFG(SIPP_CC_ID),        ((pipe->oPlanes[SIPP_CC_ID       ]-1) << SIPP_NP_OFFSET) | ( 2 << SIPP_FO_OFFSET)); //3pl-Rgb, fp16

 //Filter specific registers
   SET_REG_WORD(SIPP_CC_FRM_DIM_ADR,  (pipe->height2<<16)|pipe->width2);
   SET_REG_WORD(SIPP_CC_CFG_ADR    ,   pipe->cfg[SIPP_CC_ID]);

   SET_REG_WORD(SIPP_CC_KRGB0_ADR  ,  cfg->kr | cfg->kg                   << 16); //kr(4.8), kg(4.8)
   SET_REG_WORD(SIPP_CC_KRGB1_ADR  ,  cfg->kb | pipe->pChrGenCfg->epsilon << 16); //kb(4.8), eps

  #define CCMC(f)   ((int)((f * 0x400)  + .5) & 0xffff) // CCM co-efficients are in S(6,10) format
  #define CCMO(f)   ((int)((f * 0x1000) + .5) & 0x1fff) // CCM offsets are in S(1,12) format

   SET_REG_WORD(SIPP_CC_CCM10_ADR  , CCMC(cfg->ccm[0])    | CCMC(cfg->ccm[3])    << 16);
   SET_REG_WORD(SIPP_CC_CCM32_ADR  , CCMC(cfg->ccm[6])    | CCMC(cfg->ccm[1])    << 16);
   SET_REG_WORD(SIPP_CC_CCM54_ADR  , CCMC(cfg->ccm[4])    | CCMC(cfg->ccm[7])    << 16);
   SET_REG_WORD(SIPP_CC_CCM76_ADR  , CCMC(cfg->ccm[2])    | CCMC(cfg->ccm[5])    << 16);
   SET_REG_WORD(SIPP_CC_CCM8_ADR   , CCMC(cfg->ccm[8])    | CCMO(cfg->ccmOff[0]) << 16);
   SET_REG_WORD(SIPP_CC_OFFSETS_ADR, CCMO(cfg->ccmOff[2]) | CCMO(cfg->ccmOff[1]) << 16);
}

//#################################################################################################
#define LOAD_EN       1
void cfgLut(Opipe *pipe)
{
   LutCfg        *lutCfg     = pipe->pLutCfg;
   ColConvCfg    *colConvCfg = pipe->pColConvCfg;
   uint32_t       data;

   SET_REG_WORD(I_CFG(SIPP_LUT_ID), ((pipe->iPlanes[SIPP_LUT_ID]-1) << SIPP_NP_OFFSET)); //1xCHANNEL (3planes), fp16
   SET_REG_WORD(O_CFG(SIPP_LUT_ID), ((pipe->oPlanes[SIPP_LUT_ID]-1) << SIPP_NP_OFFSET)); //          (3planes), u8

 //Filter specific registers
   SET_REG_WORD(SIPP_LUT_FRM_DIM_ADR ,  (pipe->height2<<16)|pipe->width2);
   SET_REG_WORD(SIPP_LUT_CFG_ADR     ,   pipe->cfg[SIPP_LUT_ID]);

   SET_REG_WORD(SIPP_LUT_SIZES7_0_ADR ,  lutCfg->rgnSize[0]);  // 2^5 = 32 entries/range; and 32x16 ranges = 512 entries
   SET_REG_WORD(SIPP_LUT_SIZES15_8_ADR,  lutCfg->rgnSize[1]);

 //Program LUT table base
   SET_REG_WORD(I_BASE(SIPP_LUT_LOAD_ID), (uint32_t)lutCfg->table);
 //Enable filter just before loading LUT
   SET_REG_BITS_MASK(SIPP_CONTROL_ADR, 1<<SIPP_LUT_ID);
 //Kick initial load
   data = GET_REG_WORD_VAL(SIPP_LUT_CFG_ADR);
   SET_REG_WORD(SIPP_LUT_CFG_ADR, data & (~(LOAD_EN<<14)));
   SET_REG_WORD(SIPP_LUT_CFG_ADR, data | (  LOAD_EN<<14) );

   if(colConvCfg != 0)
   {
      #define CCVT(f)   ((int)((f) * 1024 + .5) & 0xfff)
      #define OCVT(f)   ((int)((f) * 4096 + .5) & 0x1ffff)
      SET_REG_WORD(SIPP_LUT_COEFFS1_ADR, CCVT(colConvCfg->mat[0])    | CCVT(colConvCfg->mat[1])    << 16);
      SET_REG_WORD(SIPP_LUT_COEFFS2_ADR, CCVT(colConvCfg->mat[2])    | CCVT(colConvCfg->mat[3])    << 16);
      SET_REG_WORD(SIPP_LUT_COEFFS3_ADR, CCVT(colConvCfg->mat[4])    | CCVT(colConvCfg->mat[5])    << 16);
      SET_REG_WORD(SIPP_LUT_COEFFS4_ADR, CCVT(colConvCfg->mat[6])    | CCVT(colConvCfg->mat[7])    << 16);
      SET_REG_WORD(SIPP_LUT_COEFFS5_ADR, CCVT(colConvCfg->mat[8])    | OCVT(colConvCfg->offset[0]) << 16);
      SET_REG_WORD(SIPP_LUT_COEFFS6_ADR, OCVT(colConvCfg->offset[1]) | OCVT(colConvCfg->offset[2]) << 16);
   }
}
//#################################################################################################
//Return polyfir output resolution[pixels] (based on cfg params and input resolution)
UpfirdnCfg* getUpfirdnOutRes(Opipe *pipe, uint32_t unitID, uint32_t *outW, uint32_t *outH)
{
    UpfirdnCfg *cfg = (unitID == SIPP_UPFIRDN0_ID) ? pipe->pUpfirdn0Cfg :  pipe->pUpfirdn12Cfg;
    *outW = (cfg->hN * pipe->width2  - 1)/cfg->hD + 1;
    *outH = (cfg->vN * pipe->height2 - 1)/cfg->vD + 1;
    return cfg;
}

//#################################################################################################
void cfgUpfirdn(Opipe *pipe, uint32_t unitID)
{
    uint32_t i;
    uint32_t outW, outH;

    UpfirdnCfg *cfg = getUpfirdnOutRes(pipe, unitID, &outW, &outH);

  //Save output resolution to spare divisions and recalculations later...
    cfg->iW = pipe->width2;  cfg->oW = outW;
    cfg->iH = pipe->height2; cfg->oH = outH;

    uint32_t   *vCo = (uint32_t*)cfg->vCoefs; //using 32bit pointers...
    uint32_t   *hCo = (uint32_t*)cfg->hCoefs;

  //Units 1 and 2 use following shared regs
    uint32_t off     = (unitID == SIPP_UPFIRDN0_ID) ? 0 : SIPP_UPFIRDN12_BASE_ADR - SIPP_UPFIRDN_BASE_ADR;
    uint32_t FRM_IN  = off + SIPP_UPFIRDN0_FRM_IN_DIM_ADR;
    uint32_t FRM_OUT = off + SIPP_UPFIRDN0_FRM_OUT_DIM_ADR;
    uint32_t CFG     = off + SIPP_UPFIRDN0_CFG_ADR;
    uint32_t VP0     = off + SIPP_UPFIRDN0_VP0_3210_ADR;
    uint32_t HP0     = off + SIPP_UPFIRDN0_HP0_3210_ADR;

  // *_CFG = *_BASE + 0x4
    SET_REG_WORD(gSippNfo[unitID].iBase + 4, ((pipe->iPlanes[unitID]-1) << SIPP_NP_OFFSET));
    SET_REG_WORD(gSippNfo[unitID].oBase + 4, ((pipe->iPlanes[unitID]-1) << SIPP_NP_OFFSET));

  //Filter specific registers
    SET_REG_WORD(FRM_IN,     (pipe->height2<<16)|pipe->width2);
    SET_REG_WORD(FRM_OUT,    (        outH <<16)|        outW);

    SET_REG_WORD(CFG,  ((cfg->kerSz) <<  0) |
                       ((cfg->hD   ) <<  4) |
                       ((cfg->hN   ) << 10) |
                       ((cfg->vD   ) << 16) |
                       ((cfg->vN   ) << 22) );
 //Set coeficients
 //vertical phases
   for(i=0; i<((8*1)*cfg->vN)>>2; i++)
    SET_REG_WORD(VP0+i*4, vCo[i]);

 //horizontal phases
   for(i=0; i<((8*1)*cfg->hN)>>2; i++)
    SET_REG_WORD(HP0+i*4, hCo[i]);
}
//#################################################################################################
void cfgUpfirdn0(Opipe *pipe) {cfgUpfirdn(pipe, SIPP_UPFIRDN0_ID);}
void cfgUpfirdn1(Opipe *pipe) {cfgUpfirdn(pipe, SIPP_UPFIRDN1_ID);}
void cfgUpfirdn2(Opipe *pipe) {cfgUpfirdn(pipe, SIPP_UPFIRDN2_ID);}


//#################################################################################################
//Configuring MipiRx 0,1,2 or 3
void cfgMipiRx(Opipe *pipe, uint32_t rxId)
{
   uint32_t  unitNo   = rxId - SIPP_MIPI_RX0_ID; //results in 0,1,2,3
   uint32_t  paramOff = unitNo * 0x80;
   MipiRxCfg *param   = pipe->pMipiRxCfg[unitNo];

  // *_CFG = *_BASE + 0x4
   SET_REG_WORD(gSippNfo[rxId].oBase+0x4, (pipe->oPlanes[rxId]-1) << SIPP_NP_OFFSET);

 //Custom regs:
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_FRM_DIM_ADR ,   (pipe->height<<16) | pipe->width);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_CFG_ADR     ,    param->cfg); //clock mipi RX/2 (does this matter in loopback mode ?)  )

   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_X0_ADR      ,    param->xStartWidth [0]);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_X1_ADR      ,    param->xStartWidth [1]);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_X2_ADR      ,    param->xStartWidth [2]);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_X3_ADR      ,    param->xStartWidth [3]);

   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_Y0_ADR      ,    param->yStartHeight[0]);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_Y1_ADR      ,    param->yStartHeight[1]);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_Y2_ADR      ,    param->yStartHeight[2]);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_Y3_ADR      ,    param->yStartHeight[3]);

   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_SEL01_ADR   ,    param->sel01);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_SEL23_ADR   ,    param->sel23);

   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_MASK0_ADR   ,    param->mask[0]);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_MASK1_ADR   ,    param->mask[1]);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_MASK2_ADR   ,    param->mask[2]);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_MASK3_ADR   ,    param->mask[3]);

   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_BLACK01_ADR ,    param->black01);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_BLACK23_ADR ,    param->black23);
   SET_REG_WORD(paramOff + SIPP_MIPI_RX0_VBP_ADR     ,    param->vbp);
}

void cfgMipiRx0(Opipe *pipe) {cfgMipiRx(pipe, SIPP_MIPI_RX0_ID);};
void cfgMipiRx1(Opipe *pipe) {cfgMipiRx(pipe, SIPP_MIPI_RX1_ID);};
void cfgMipiRx2(Opipe *pipe) {cfgMipiRx(pipe, SIPP_MIPI_RX2_ID);};
void cfgMipiRx3(Opipe *pipe) {cfgMipiRx(pipe, SIPP_MIPI_RX3_ID);};



//#################################################################################################
//Non OPIPE filters:
//#################################################################################################
void cfgConv(Opipe *pipe)
{
   ConvCfg  *convCfg = pipe->pConvCfg;
   uint16_t *ccm5x5  = convCfg->mat5x5;

   SET_REG_WORD(I_CFG(SIPP_CONV_ID), ((pipe->iPlanes[SIPP_CONV_ID]-1) << SIPP_NP_OFFSET));
   SET_REG_WORD(O_CFG(SIPP_CONV_ID), ((pipe->iPlanes[SIPP_CONV_ID]-1) << SIPP_NP_OFFSET));

 //Filter specific registers
   SET_REG_WORD(SIPP_CONV_FRM_DIM_ADR,  (pipe->height<<16)|pipe->width);
   SET_REG_WORD(SIPP_CONV_CFG_ADR,       pipe->cfg[SIPP_CONV_ID]);

 //The 5x5 coefs matrix (packConv5x5CCM(convCfg, ker)
   SET_REG_WORD(SIPP_CONV_COEFF_01_00_ADR,  (ccm5x5[ 1]<<16) | ccm5x5[ 0]);
   SET_REG_WORD(SIPP_CONV_COEFF_03_02_ADR,  (ccm5x5[ 3]<<16) | ccm5x5[ 2]);
   SET_REG_WORD(SIPP_CONV_COEFF_04_ADR   ,                     ccm5x5[ 4]);
   SET_REG_WORD(SIPP_CONV_COEFF_11_10_ADR,  (ccm5x5[ 6]<<16) | ccm5x5[ 5]);
   SET_REG_WORD(SIPP_CONV_COEFF_13_12_ADR,  (ccm5x5[ 8]<<16) | ccm5x5[ 7]);
   SET_REG_WORD(SIPP_CONV_COEFF_14_ADR   ,                     ccm5x5[ 9]);
   SET_REG_WORD(SIPP_CONV_COEFF_21_20_ADR,  (ccm5x5[11]<<16) | ccm5x5[10]);
   SET_REG_WORD(SIPP_CONV_COEFF_23_22_ADR,  (ccm5x5[13]<<16) | ccm5x5[12]);
   SET_REG_WORD(SIPP_CONV_COEFF_24_ADR   ,                     ccm5x5[14]);
   SET_REG_WORD(SIPP_CONV_COEFF_31_30_ADR,  (ccm5x5[16]<<16) | ccm5x5[15]);
   SET_REG_WORD(SIPP_CONV_COEFF_33_32_ADR,  (ccm5x5[18]<<16) | ccm5x5[17]);
   SET_REG_WORD(SIPP_CONV_COEFF_34_ADR   ,                     ccm5x5[19]);
   SET_REG_WORD(SIPP_CONV_COEFF_41_40_ADR,  (ccm5x5[21]<<16) | ccm5x5[20]);
   SET_REG_WORD(SIPP_CONV_COEFF_43_42_ADR,  (ccm5x5[23]<<16) | ccm5x5[22]);
   SET_REG_WORD(SIPP_CONV_COEFF_44_ADR   ,                     ccm5x5[24]);
}

//#################################################################################################
void cfgHarris(Opipe *pipe)
{
   HarrisCfg *cfg = pipe->pHarrisCfg;

   SET_REG_WORD(I_CFG(SIPP_HARRIS_ID), ((pipe->iPlanes[SIPP_HARRIS_ID]-1) << SIPP_NP_OFFSET));
   SET_REG_WORD(O_CFG(SIPP_HARRIS_ID), ((pipe->iPlanes[SIPP_HARRIS_ID]-1) << SIPP_NP_OFFSET));

 //Custom regs:
   SET_REG_WORD(SIPP_HARRIS_FRM_DIM_ADR, ((pipe->height>>1)<<16) | (pipe->width>>1));
   SET_REG_WORD(SIPP_HARRIS_CFG_ADR,        cfg->cfg);
   SET_REG_WORD(SIPP_HARRIS_K_ADR,          cfg->kValue.u32);
}