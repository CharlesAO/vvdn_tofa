///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    configure the second pipeline
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "app_config.h"
#include "Opipe.h"


// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
//Circular IO CMX buffers associated to main buffers
#define I_CBUFF_H (3*IRQ_RATE)
#define O_CBUFF_H (2*IRQ_RATE)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static SigmaDnsCfg   sigCfg;
static BlcCfg        blcCfg;

static uint16_t iCircBuffCmx2[I_CBUFF_H *IMG_W * 1] SECTION(".cmx.data") ALIGNED(16);  //10bit bayer
static uint16_t oCircBuffCmx2[O_CBUFF_H *IMG_W * 1] SECTION(".cmx.data") ALIGNED(16);  //10bit bayer

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
static void appSetParams02(Opipe *p)
{
    //Global stuff
    p->format       = BAYER;
    p->rawBits      = ISPC_BAYER_BITS;
    p->bayerPattern = BAYER_PATTERN;
  
    //BlackLevelCorrection
    p->pBlcCfg  = &blcCfg;
    blcCfg.r  = 0;
    blcCfg.b  = 0;
    blcCfg.gr = 0;
    blcCfg.gb = 0;
    
    //Sigma
    p->pSigmaCfg      = &sigCfg;
    sigCfg.noiseFloor = 0; //don't care
    sigCfg.thresh1P0  = 0; //don't care
    sigCfg.thresh2P0  = 0; //don't care 
    sigCfg.thresh1P1  = 0; //don't care
    sigCfg.thresh2P1  = 0; //don't care
    sigCfg.thresh1P2  = 0; //don't care
    sigCfg.thresh2P2  = 0; //don't care
    sigCfg.thresh1P3  = 0; //don't care
    sigCfg.thresh2P3  = 0; //don't care
}

void buildAndCfgP2(Opipe *p, uint16_t *iBuf, uint16_t *oBuf)
{
    DBuffer *pIn;
    DBuffer *pOut;

   //1)Init pipe an do application specific filter configuration
   OpipeInit(p);
   appSetParams02(p);
   
   //2)Specify enables for filters used in this pipe
    p->enMask =  1<<SIPP_SIGMA_ID;
    p->width  = IMG_W;
    p->height = IMG_H;
   
    //Override default cfg:
    p->cfg[SIPP_SIGMA_ID] = (             BAYER  << 0) |
                            (                 1  << 1) | //Passthrough mode !!!
                            ((ISPC_BAYER_BITS-1) << 4);

    //3) Specify CMX buffers:
    pIn  = OpipeCfgBuff (p, SIPP_SIGMA_ID, D_DMA|D_IN, (uint32_t)iCircBuffCmx2, I_CBUFF_H, BPP(2));
    pOut = OpipeCfgBuff (p, SIPP_SIGMA_ID, D_DMA|D_OUT,(uint32_t)oCircBuffCmx2, O_CBUFF_H, BPP(2));
    
    //Actually set the main buffer pointers
    pIn->ddr.base = (uint32_t)iBuf;
    pOut->ddr.base = (uint32_t)oBuf;
}
