///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the source code of the SIPP pipeline
///            generated by the SIPP Graph Designer.
///
///
///  ************************ DO NOT EDIT, THIS IS A GENERATED FILE!!!! ************************
/// 

#include "monoPipe.h"

#include <include/sippHwDefs.h>
#include <arch/ma2x5x/include/sippHwDefs_ma2x5x.h>


//The full-frame buffers
UInt16     DDR_DATA MonoPipe_dmaIn0_buffer   [MONO_PIPE_INPUT_BUFFER_WIDTH* MONO_PIPE_INPUT_BUFFER_HEIGHT* 1  ]ALIGNED(8);
UInt8      DDR_DATA MonoPipe_dmaOut0_buffer  [MONO_PIPE_INPUT_BUFFER_WIDTH* MONO_PIPE_INPUT_BUFFER_HEIGHT* 1  ]ALIGNED(8);
	
///
void readMonoPipeInput(MonoPipe *pPl)
{
   sippRdFileU8((UInt8*)MonoPipe_dmaIn0_buffer,MONO_PIPE_INPUT_BUFFER_WIDTH * MONO_PIPE_INPUT_BUFFER_HEIGHT * 1 * sizeof(UInt16),"../../../../../../../resources/test_640x480_10b.raw");
   //Ensure parameter is used to avoid compiler warning
   (void)(pPl);
	
}

void writeMonoPipeOutput(MonoPipe *pPl)
{
   sippWrFileU8((UInt8*)MonoPipe_dmaOut0_buffer,MONO_PIPE_INPUT_BUFFER_WIDTH * MONO_PIPE_INPUT_BUFFER_HEIGHT * 1 * sizeof(UInt8),"output_640x480.raw");
   //Ensure parameter is used to avoid compiler warning
   (void)(pPl);

}
   
void buildMonoPipe(MonoPipe *pPl)
{
   // create new pipeline
   pPl->pl               = sippCreatePipeline(0, 3, SIPP_MBIN(mbinImgSipp));

   // create filters
   pPl->dmaIn0          =  sippCreateFilter(pPl->pl, 0x00,       MONO_PIPE_INPUT_BUFFER_WIDTH, MONO_PIPE_INPUT_BUFFER_HEIGHT, N_PL(1),  SZ(UInt16), SIPP_AUTO,          (FnSvuRun)SIPP_DMA_ID,        0);
   pPl->sigmaDnsMa2x5x0 =  sippCreateFilter(pPl->pl, 0x0,        MONO_PIPE_INPUT_BUFFER_WIDTH, MONO_PIPE_INPUT_BUFFER_HEIGHT, N_PL(1),  SZ(UInt16), SIPP_AUTO,          (FnSvuRun)SIPP_SIGMA_ID,      0);
   pPl->lscMa2x5x0      =  sippCreateFilter(pPl->pl, 0x0,        MONO_PIPE_INPUT_BUFFER_WIDTH, MONO_PIPE_INPUT_BUFFER_HEIGHT, N_PL(1),  SZ(UInt16), SIPP_AUTO,          (FnSvuRun)SIPP_LSC_ID,        0);
   pPl->rawMa2x5x0      =  sippCreateFilter(pPl->pl, 0x0,        MONO_PIPE_INPUT_BUFFER_WIDTH, MONO_PIPE_INPUT_BUFFER_HEIGHT, N_PL(1),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_RAW_ID,        0);
   pPl->ydnsMa2x5x0     =  sippCreateFilter(pPl->pl, 0x0,        MONO_PIPE_INPUT_BUFFER_WIDTH, MONO_PIPE_INPUT_BUFFER_HEIGHT, N_PL(1),  SZ(fp16), SIPP_AUTO,          (FnSvuRun)SIPP_LUMA_ID,       0);
   pPl->sharpenMa2x5x0  =  sippCreateFilter(pPl->pl, 0x0,        MONO_PIPE_INPUT_BUFFER_WIDTH, MONO_PIPE_INPUT_BUFFER_HEIGHT, N_PL(1),  SZ(fp16), SIPP_AUTO,          (FnSvuRun)SIPP_SHARPEN_ID,    0);
   pPl->lutMa2x5x0      =  sippCreateFilter(pPl->pl, 0x0,        MONO_PIPE_INPUT_BUFFER_WIDTH, MONO_PIPE_INPUT_BUFFER_HEIGHT, N_PL(1),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_LUT_ID,        0);
   pPl->dmaOut0         =  sippCreateFilter(pPl->pl, 0x00,       MONO_PIPE_INPUT_BUFFER_WIDTH, MONO_PIPE_INPUT_BUFFER_HEIGHT, N_PL(1),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_DMA_ID,        0);
	
   // link filters
   sippLinkFilter(pPl->sigmaDnsMa2x5x0, pPl->dmaIn0,        5,5                 );
   sippLinkFilter(pPl->lscMa2x5x0,    pPl->sigmaDnsMa2x5x0, 1,0                 );
   sippLinkFilter(pPl->rawMa2x5x0,    pPl->lscMa2x5x0,    5,5                 );
   sippLinkFilter(pPl->ydnsMa2x5x0,   pPl->rawMa2x5x0,    11,11               );
   sippLinkFilter(pPl->sharpenMa2x5x0, pPl->ydnsMa2x5x0,   7,7                 );
   sippLinkFilter(pPl->lutMa2x5x0,    pPl->sharpenMa2x5x0, 1,1                 );
   sippLinkFilter(pPl->dmaOut0,       pPl->lutMa2x5x0,    1,1                 );
	
}

  
void configMonoPipe(MonoPipe *pPl)
{
   DmaParam            *dmaIn0Cfg          = (DmaParam         *)pPl->dmaIn0->params;
   SigmaParam          *sigmaDnsMa2x5x0Cfg = (SigmaParam       *)pPl->sigmaDnsMa2x5x0->params;
   LscParam            *lscMa2x5x0Cfg      = (LscParam         *)pPl->lscMa2x5x0->params;
   RawParam            *rawMa2x5x0Cfg      = (RawParam         *)pPl->rawMa2x5x0->params;
   YDnsParam           *ydnsMa2x5x0Cfg     = (YDnsParam        *)pPl->ydnsMa2x5x0->params;
   UsmParam            *sharpenMa2x5x0Cfg  = (UsmParam         *)pPl->sharpenMa2x5x0->params;
   LutParam            *lutMa2x5x0Cfg      = (LutParam         *)pPl->lutMa2x5x0->params;
   DmaParam            *dmaOut0Cfg         = (DmaParam         *)pPl->dmaOut0->params;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // here go through all the filter instances and assign the value for all parameter
  // fields as defined in the filter property window
   sigmaDnsMa2x5x0Cfg->thresh[0]=(ISPC_SIGMA_THRESH1_P0 <<  0 | ISPC_SIGMA_THRESH2_P0 <<  8 | ISPC_SIGMA_THRESH1_P1 << 16 | ISPC_SIGMA_THRESH2_P1 << 24);
   sigmaDnsMa2x5x0Cfg->thresh[1]=(ISPC_SIGMA_THRESH1_P2 <<  0 | ISPC_SIGMA_THRESH2_P2 <<  8 | ISPC_SIGMA_THRESH1_P3 << 16 | ISPC_SIGMA_THRESH2_P3 << 24);
   sigmaDnsMa2x5x0Cfg->cfg= SIGMA_DNS_CFG(ISPC_SIGMA_NOISE_FLOOR, (ISPC_BAYER_BITS - 1), SIGMA_DNS_PASSTHRU_BIT, FORMAT);
   sigmaDnsMa2x5x0Cfg->bayerPattern=BAYER_PATTERN;
   sigmaDnsMa2x5x0Cfg->blcGR=ISPC_BLACK_LEVEL_P2;
   sigmaDnsMa2x5x0Cfg->blcR=ISPC_BLACK_LEVEL_P3;
   sigmaDnsMa2x5x0Cfg->blcB=ISPC_BLACK_LEVEL_P0;
   sigmaDnsMa2x5x0Cfg->blcGB=ISPC_BLACK_LEVEL_P1;
   lscMa2x5x0Cfg->gmBase=   ispcLscMesh;
   lscMa2x5x0Cfg->gmWidth=  ISPC_LSC_GAIN_MAP_W;
   lscMa2x5x0Cfg->gmHeight= ISPC_LSC_GAIN_MAP_H;
   lscMa2x5x0Cfg->dataFormat=SIPP_LSC_CFG_FORMAT(FORMAT);
   lscMa2x5x0Cfg->dataWidth=ISPC_BAYER_BITS;
   rawMa2x5x0Cfg->grgbPlat= RAW_GRGB_PLATO(GRGB_IMBAL_PLAT_DARK, GRGB_IMBAL_PLAT_BRIGHT);
   rawMa2x5x0Cfg->grgbDecay=RAW_GRGB_DECAY(GRGB_IMBAL_DECAY_DARK, GRGB_IMBAL_DECAY_BRIGHT);
   rawMa2x5x0Cfg->badPixCfg=RAW_BAD_PIX_CFG(ISPC_BAD_PIX_NOISE_LEVEL, ISPC_BAD_PIX_ALPHA_G_HOT, ISPC_BAD_PIX_ALPHA_G_COLD, ISPC_BAD_PIX_ALPHA_RB_HOT, ISPC_BAD_PIX_ALPHA_RB_COLD);
   rawMa2x5x0Cfg->cfg=      RAW_CFG(FORMAT, BAYER_PATTERN, GRGB_IMBAL_EN, 0, AE_PATCH_STATS_EN, AE_RGB_HIST_STATS_EN, (ISPC_BAYER_BITS - 1), ISPC_GRGB_IMBAL_THRESHOLD, 0, AF_PATCH_STATS_EN, BAYER_2x2_MODE, AE_Y_HIST_STATS_EN, HOT_COLD_PIX_SUPPRESS_EN);
   rawMa2x5x0Cfg->gainSat[0]= (DIGI_GAIN_GR | (DIGI_SAT_GR << 16));
   rawMa2x5x0Cfg->gainSat[1]= (DIGI_GAIN_R  | (DIGI_SAT_R  << 16));
   rawMa2x5x0Cfg->gainSat[2]= (DIGI_GAIN_B  | (DIGI_SAT_B  << 16));
   rawMa2x5x0Cfg->gainSat[3]= (DIGI_GAIN_GB | (DIGI_SAT_GB << 16));
   rawMa2x5x0Cfg->statsBase=0;
   rawMa2x5x0Cfg->statsPlanes=0;
   rawMa2x5x0Cfg->statsPatchCfg=0;
   rawMa2x5x0Cfg->statsPatchStart=0;
   rawMa2x5x0Cfg->statsPatchSkip=0;
   rawMa2x5x0Cfg->statsThresh=0;
   rawMa2x5x0Cfg->afF1coefs[0]=0;
   rawMa2x5x0Cfg->afF1coefs[1]=0;
   rawMa2x5x0Cfg->afF1coefs[2]=0;
   rawMa2x5x0Cfg->afF1coefs[3]=0;
   rawMa2x5x0Cfg->afF1coefs[4]=0;
   rawMa2x5x0Cfg->afF1coefs[5]=0;
   rawMa2x5x0Cfg->afF1coefs[6]=0;
   rawMa2x5x0Cfg->afF1coefs[7]=0;
   rawMa2x5x0Cfg->afF1coefs[8]=0;
   rawMa2x5x0Cfg->afF1coefs[9]=0;
   rawMa2x5x0Cfg->afF1coefs[10]=0;
   rawMa2x5x0Cfg->afF2coefs[0]=0;
   rawMa2x5x0Cfg->afF2coefs[1]=0;
   rawMa2x5x0Cfg->afF2coefs[2]=0;
   rawMa2x5x0Cfg->afF2coefs[3]=0;
   rawMa2x5x0Cfg->afF2coefs[4]=0;
   rawMa2x5x0Cfg->afF2coefs[5]=0;
   rawMa2x5x0Cfg->afF2coefs[6]=0;
   rawMa2x5x0Cfg->afF2coefs[7]=0;
   rawMa2x5x0Cfg->afF2coefs[8]=0;
   rawMa2x5x0Cfg->afF2coefs[9]=0;
   rawMa2x5x0Cfg->afF2coefs[10]=0;
   rawMa2x5x0Cfg->afMinThresh=0;
   rawMa2x5x0Cfg->afSubtract=0;
   rawMa2x5x0Cfg->afPatchCfg=0;
   rawMa2x5x0Cfg->afPatchStart=0;
   rawMa2x5x0Cfg->afStatsBase=0;
   rawMa2x5x0Cfg->histLumaBase=0;
   rawMa2x5x0Cfg->histRgbBase=0;
   ydnsMa2x5x0Cfg->cfg=     LUMA_DNS_CFG(0, ISPC_LUMA_DNS_REF_SHIFT, ISPC_LUMA_DNS_ALPHA, ISPC_LUMA_DNS_BITPOS);
   ydnsMa2x5x0Cfg->gaussLut[0]=gaussLut[0];
   ydnsMa2x5x0Cfg->gaussLut[1]=gaussLut[1];
   ydnsMa2x5x0Cfg->gaussLut[2]=gaussLut[2];
   ydnsMa2x5x0Cfg->gaussLut[3]=gaussLut[3];
   ydnsMa2x5x0Cfg->f2=      ISPC_LUMA_DNS_F2;
   ydnsMa2x5x0Cfg->gammaLut[0]=gammaLut[0];
   ydnsMa2x5x0Cfg->gammaLut[1]=gammaLut[1];
   ydnsMa2x5x0Cfg->gammaLut[2]=gammaLut[2];
   ydnsMa2x5x0Cfg->gammaLut[3]=gammaLut[3];
   ydnsMa2x5x0Cfg->gammaLut[4]=gammaLut[4];
   ydnsMa2x5x0Cfg->distCfg= (u32)ispcYDnsDistLut;
   ydnsMa2x5x0Cfg->distOffsets=LUMA_DNS_X_OFFSET | (LUMA_DNS_Y_OFFSET << 16);
   ydnsMa2x5x0Cfg->fullFrmDim=MONO_PIPE_INPUT_BUFFER_WIDTH | (MONO_PIPE_INPUT_BUFFER_HEIGHT  << 16);
   sharpenMa2x5x0Cfg->cfg=  SHARPEN_CFG(ISPC_SHARP_MIN_THR,KERNEL_SIZE, CLAMP_OUTPUT, SHARPEN_MODE, OUTPUT_DELTAS);
   sharpenMa2x5x0Cfg->strength=SHARPEN_STRENGTH_CFG(ISPC_SHARP_STRENGTH_LIGHTEN, ISPC_SHARP_STRENGTH_DARKEN);
   sharpenMa2x5x0Cfg->clip= ISPC_SHARP_ALPHA;
   sharpenMa2x5x0Cfg->limit=SHARPEN_LIMITS_CFG(ISPC_SHARP_UNDERSHOOT, ISPC_SHARP_OVERSHOOT);
   sharpenMa2x5x0Cfg->rgnStop01=SHARPEN_RANGESTOP01_CFG(ISPC_SHARP_RANGE_STOP_0, ISPC_SHARP_RANGE_STOP_1);
   sharpenMa2x5x0Cfg->rgnStop23=SHARPEN_RANGESTOP01_CFG(ISPC_SHARP_RANGE_STOP_2, ISPC_SHARP_RANGE_STOP_3);
   sharpenMa2x5x0Cfg->coef01=SHARPEN_COEFS01_CFG(ISPC_SHARP_COEF0, ISPC_SHARP_COEF1);
   sharpenMa2x5x0Cfg->coef23=SHARPEN_COEFS23_CFG(ISPC_SHARP_COEF2, ISPC_SHARP_COEF3);
   lutMa2x5x0Cfg->cfg=      LUT_CFG(0, 0, 0, 0, NUM_LUTS, 10, CHANNEL_MODE, FP16_MODE);
   lutMa2x5x0Cfg->sizeA=    0x55555555;
   lutMa2x5x0Cfg->sizeB=    0x55555555;
   lutMa2x5x0Cfg->lut=      gammaLutFp16;
   lutMa2x5x0Cfg->lutFormat=0;
   lutMa2x5x0Cfg->mat[0]=   0;
   lutMa2x5x0Cfg->mat[1]=   0;
   lutMa2x5x0Cfg->mat[2]=   0;
   lutMa2x5x0Cfg->mat[3]=   0;
   lutMa2x5x0Cfg->mat[4]=   0;
   lutMa2x5x0Cfg->mat[5]=   0;
   lutMa2x5x0Cfg->mat[6]=   0;
   lutMa2x5x0Cfg->mat[7]=   0;
   lutMa2x5x0Cfg->mat[8]=   0;
   lutMa2x5x0Cfg->offset[0]=0;
   lutMa2x5x0Cfg->offset[1]=0;
   lutMa2x5x0Cfg->offset[2]=0;
   dmaIn0Cfg->ddrAddr   = (UInt32)&MonoPipe_dmaIn0_buffer;
   dmaOut0Cfg->ddrAddr  = (UInt32)&MonoPipe_dmaOut0_buffer;
  
  
}
