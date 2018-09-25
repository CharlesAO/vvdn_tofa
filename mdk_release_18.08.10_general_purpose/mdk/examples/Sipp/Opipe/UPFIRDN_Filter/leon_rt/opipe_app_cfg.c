///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Opipe application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <string.h>
#include "Opipe.h"
#include "app_config.h"

UpfirdnCfg  updnCfg;


// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
//Select a target UPFIRDN filter:
#define UPFIRDN_ID SIPP_UPFIRDN0_ID
//#define UPFIRDN_ID SIPP_UPFIRDN1_ID
//#define UPFIRDN_ID SIPP_UPFIRDN2_ID

//Circular IO CMX buffers associated to main buffers
#define I_CBUFF_H (3*IRQ_RATE)
#define O_CBUFF_H (2*IRQ_RATE)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
static uint8_t iCircBuffCmx[I_CBUFF_H *IMG_W]   SECTION(".cmx.data") ALIGNED(16);
static uint8_t oCircBuffCmx[O_CBUFF_H *IMG_W*2] SECTION(".cmx.data") ALIGNED(16);

/// @spec     
///            "formula to go from register values in the range [0, 255] to
///            floating point will be (x-64) / 128. "
///  so 1.0f = (x-64)/128, thus x = 128+64 = 192
///     0.0f = (y-64)/128, thus y = 64

// 8 because we will use memcpy sor set the registers, and in this will fit HW configuration
static uint8_t horzCoefs[] ALIGNED(8) = {64, 64, 64,  192,  64, 64, 64,     64, //phase1
        64, 64, 64,  192,  64, 64, 64,     64};//phase2

static uint8_t vertCoefs[] ALIGNED(8) = {64, 64, 64,  192,  64, 64, 64,     64, //phase1
        64, 64, 64,  192,  64, 64, 64,     64};//phase2


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
static void appSetParams(Opipe *p)
{
    // Global stuff
    p->width   = IMG_W;
    p->height  = IMG_H;

    // PolyFir params"
    if(UPFIRDN_ID == SIPP_UPFIRDN0_ID) p->pUpfirdn0Cfg  = &updnCfg;
    else                               p->pUpfirdn12Cfg = &updnCfg;

    updnCfg.vN     = 2; updnCfg.vD = 1;
    updnCfg.hN     = 2; updnCfg.hD = 1;
    updnCfg.kerSz  = 7;
    updnCfg.vCoefs = vertCoefs;
    updnCfg.hCoefs = horzCoefs;
}


void BuildAndCfgPipe(Opipe *p, uint8_t *iBuf, uint8_t *oBuf)
{
    DBuffer *pIn;
    DBuffer *pOut;

    // 1)Init pipe an do application specific filter configuration
    OpipeInit(p);
    appSetParams(p);

    // 2)Specify enables for filters used in this pipe
    p->enMask = 1<<UPFIRDN_ID;

    // 3) Specify CMX buffers:
    pIn  = OpipeCfgBuff (p, UPFIRDN_ID, D_DMA|D_IN, (uint32_t)iCircBuffCmx, I_CBUFF_H, BPP(1));
    pOut = OpipeCfgBuff (p, UPFIRDN_ID, D_DMA|D_OUT,(uint32_t)oCircBuffCmx, O_CBUFF_H, BPP(1));

    // Actually set the main buffer pointers
    pIn->ddr.base   = (uint32_t)iBuf;
    pOut->ddr.base   = (uint32_t)oBuf;
    pOut->ddr.height = IMG_H*2; //when height != p->height, must override manually
}
