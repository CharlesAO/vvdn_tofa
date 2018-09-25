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
static UpfirdnCfg  updnCfg;
static uint8_t iCircBuffCmx5[I_CBUFF_H *IMG_W] SECTION(".cmx.data") ALIGNED(16);
static uint8_t oCircBuffCmx5[O_CBUFF_H *IMG_W] SECTION(".cmx.data") ALIGNED(16);

/// @spec      http://dub30/bugzilla/show_bug.cgi?id=20870#c18
///            "formula to go from register values in the range [0, 255] to 
///            floating point will be (x-64) / 128. "
///  so 1.0f = (x-64)/128, thus x = 128+64 = 192
///     0.0f = (y-64)/128, thus y = 64

// 8 because we will use memcpy sor set the registers, and in this will fit HW configuration
static uint8_t horzCoefs[8] ALIGNED(8) = {64, 64, 64,  192,  64, 64, 64,     64};
static uint8_t vertCoefs[8] ALIGNED(8) = {64, 64, 64,  192,  64, 64, 64,     64};


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
static void appSetParams05(Opipe *p)
{
    //Global stuff
    p->width   = IMG_W;
    p->height  = IMG_H;
 
    //PolyFir params"
    if(UPFIRDN_ID == SIPP_UPFIRDN0_ID) p->pUpfirdn0Cfg  = &updnCfg;
    else                               p->pUpfirdn12Cfg = &updnCfg;
  
    updnCfg.vN     = updnCfg.vD = 1;
    updnCfg.hN     = updnCfg.hD = 1;
    updnCfg.kerSz  = 7;
    updnCfg.vCoefs = vertCoefs;
    updnCfg.hCoefs = horzCoefs;
}

void buildAndCfgP5(Opipe *p, uint8_t *iBuf, uint8_t *oBuf)
{
    DBuffer *pIn;
    DBuffer *pOut;

    //1)Init pipe an do application specific filter configuration
    OpipeInit(p);
    appSetParams05(p);
   
    //2)Specify enables for filters used in this pipe
    p->enMask = 1<<UPFIRDN_ID;

    //3) Specify CMX buffers:
    pIn = OpipeCfgBuff (p, UPFIRDN_ID, D_DMA|D_IN, (uint32_t)iCircBuffCmx5, I_CBUFF_H, BPP(1));
    pOut = OpipeCfgBuff (p, UPFIRDN_ID, D_DMA|D_OUT,(uint32_t)oCircBuffCmx5, O_CBUFF_H, BPP(1));
    
    //Actually set the main buffer pointers
    pIn->ddr.base = (uint32_t)iBuf;
    pOut->ddr.base = (uint32_t)oBuf;
}
