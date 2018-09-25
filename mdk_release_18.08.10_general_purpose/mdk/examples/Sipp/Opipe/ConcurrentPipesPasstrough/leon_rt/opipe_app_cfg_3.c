///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    configure a pipeline
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
static SharpenCfg    sharpCfg;

static uint8_t iCircBuffCmx3[I_CBUFF_H *IMG_W] SECTION(".cmx.data") ALIGNED(16);
static uint8_t oCircBuffCmx3[O_CBUFF_H *IMG_W] SECTION(".cmx.data") ALIGNED(16);

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
static void appSetParams03(Opipe *p)
{
    //Luma Sharpen
    p->pSharpCfg = &sharpCfg;
    sharpCfg.strengthLighten = 0x0000;//fp16 (0 for passthrough)
    sharpCfg.strengthDarken  = 0x0000;//fp16 (0 for passthrough)
    sharpCfg.minThr          = 0x0000;//fp16
    sharpCfg.alpha           = 0x0000;//fp16
    sharpCfg.undershoot      = 0x0000;//fp16
    sharpCfg.overshoot       = 0x4000;//fp16
    
    sharpCfg.rangeStop0 =  sharpCfg.rangeStop1 = 
    sharpCfg.rangeStop2 =  sharpCfg.rangeStop3 = 0x0000;//fp16
    
    sharpCfg.sharpenCoeffs[0] = sharpCfg.sharpenCoeffs[1] =
    sharpCfg.sharpenCoeffs[2] = sharpCfg.sharpenCoeffs[3] = 0x0000; //fp16
}

void buildAndCfgP3(Opipe *p, uint8_t *iBuf, uint8_t *oBuf)
{
    DBuffer *pIn;
    DBuffer *pOut;

    //1)Init pipe an do application specific filter configuration
    OpipeInit(p);
    appSetParams03(p);
   
    //2)Specify enables for filters used in this pipe
    p->enMask =  1<<SIPP_SHARPEN_ID;
    p->width  = IMG_W;
    p->height = IMG_H;
    p->cfg[SIPP_SHARPEN_ID] =               5   //SHARPEN_KERNEL_SIZE
                              | (1      <<  3)  //clamp output
                              | (0x1a25 << 16); //fp trehshold

    //3) Specify CMX buffers:
    pIn = OpipeCfgBuff (p, SIPP_SHARPEN_ID, D_DMA|D_IN, (uint32_t)iCircBuffCmx3, I_CBUFF_H, BPP(1));
    pOut = OpipeCfgBuff (p, SIPP_SHARPEN_ID, D_DMA|D_OUT,(uint32_t)oCircBuffCmx3, O_CBUFF_H, BPP(1));
    
    //Actually set the main buffer pointers
    pIn->ddr.base = (uint32_t)iBuf;
    pOut->ddr.base = (uint32_t)oBuf;
}
