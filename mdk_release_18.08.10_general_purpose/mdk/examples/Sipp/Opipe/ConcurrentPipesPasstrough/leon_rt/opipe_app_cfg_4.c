///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    configure a pipeline
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <string.h>
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
static ConvCfg  convCfg;

static uint8_t iCircBuffCmx4[I_CBUFF_H *IMG_W] SECTION(".cmx.data") ALIGNED(16);
static uint8_t oCircBuffCmx4[O_CBUFF_H *IMG_W] SECTION(".cmx.data") ALIGNED(16);

//Identify 5x5 convolution matrix
static uint16_t ccm5x5[5*5] =
{
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x3C00, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
static void appSetParams04(Opipe *p)
{
    //Global stuff
    p->width   = IMG_W;
    p->height  = IMG_H;
 
    //Conv matrix
    p->pConvCfg  = &convCfg;
    p->cfg[SIPP_CONV_ID] =  5       | //kernel size
                           (1 << 3) ; //OUTPUT_CLAMP
    //Copy the conv matrix:
    memcpy(p->pConvCfg->mat5x5, ccm5x5, sizeof(ccm5x5));//the conv matrix (fp16)
}

void buildAndCfgP4(Opipe *p, uint8_t *iBuf, uint8_t *oBuf)
{
    DBuffer *pIn;
    DBuffer *pOut;

    //1)Init pipe an do application specific filter configuration
    OpipeInit(p);
    appSetParams04(p);
   
    //2)Specify enables for filters used in this pipe
    p->enMask =  1<<SIPP_CONV_ID;

    //3) Specify CMX buffers:
    pIn =  OpipeCfgBuff (p, SIPP_CONV_ID, D_DMA|D_IN, (uint32_t)iCircBuffCmx4, I_CBUFF_H, BPP(1));
    pOut = OpipeCfgBuff (p, SIPP_CONV_ID, D_DMA|D_OUT,(uint32_t)oCircBuffCmx4, O_CBUFF_H, BPP(1));
    
    //Actually set the main buffer pointers
    pIn->ddr.base = (uint32_t)iBuf;
    pOut->ddr.base = (uint32_t)oBuf;
}
