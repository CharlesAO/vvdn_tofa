///
/// @file
/// @author    andrei.lupas@movidius.com
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Opipe - test utils
///

#include <DrvLeonL2C.h>
#include <DrvCpr.h>

#include "Opipe.h"
#include "OpipeInternals.h"

//#################################################################################################
//  SIPP MIPI Tx[0] -> SIPP MIPI Rx[1] loopback connection
//  SIPP MIPI Tx[1] -> SIPP MIPI Rx[3] loopback connection
//#################################################################################################

//#################################################################################################
// Test purpose mainly: push a full-frame towards mipi RX via loopback mode
// TX operates in no-WRAP mode !
void oCfgMipiTxLoopback(oMipiTxLoopbackParam *cfg)
{
   uint32_t data;
   uint32_t iBufOff;
   uint32_t paramOff;
   uint32_t unitNo = cfg->txID - SIPP_MIPI_TX0_ID;

 //Enable the required loopback mode via read-modify-write (so that we allow 2 loopbacks if needed)
   data  = GET_REG_WORD_VAL(MSS_LOOPBACK_CFG_ADR);
   data |= (1 << unitNo);
   SET_REG_WORD(MSS_LOOPBACK_CFG_ADR, data);

 //Config the input buffer
   iBufOff = unitNo * 0x1C;

   SET_REG_WORD(iBufOff + I_BASE(SIPP_MIPI_TX0_ID), cfg->imgAddr);
   SET_REG_WORD(iBufOff + I_CFG(SIPP_MIPI_TX0_ID) , cfg->bpp<<28);//nl = 0 => no_wrap mode
   SET_REG_WORD(iBufOff + I_LS(SIPP_MIPI_TX0_ID)  , cfg->imgW * cfg->bpp);

 //Set remaining params:
   paramOff = unitNo * 0x80;

   SET_REG_WORD(paramOff + SIPP_MIPI_TX0_FRM_DIM_ADR,          (cfg->imgH << 16)|cfg->imgW);
   SET_REG_WORD(paramOff + SIPP_MIPI_TX0_H_BACKPORCH_ADR,       cfg->hbp);    // horizontal back porch [pclk]
   SET_REG_WORD(paramOff + SIPP_MIPI_TX0_H_FRONTPORCH_ADR,      cfg->hfp);    // horizontal front porch [pclk]
   SET_REG_WORD(paramOff + SIPP_MIPI_TX0_HSYNC_WIDTH_ADR,       cfg->hsync);  // HSYNC [pclk]
   SET_REG_WORD(paramOff + SIPP_MIPI_TX0_VSYNC_WIDTH_ADR,       cfg->vsync);  // VSYNC [lines]
   SET_REG_WORD(paramOff + SIPP_MIPI_TX0_V_ACTIVEHEIGHT_ADR,    cfg->imgH-1); // Active height
   SET_REG_WORD(paramOff + SIPP_MIPI_TX0_H_ACTIVEWIDTH_ADR,     cfg->imgW-1); // Active width

   SET_REG_WORD(paramOff + SIPP_MIPI_TX0_CFG_ADR,               (1<<2)        // Use one-shot mode
                                                              | (1<<4));      // tx_clk_speed : 1/2
}
 //#################################################################################################
void oStartMipiTxLoopback(uint32_t txID)
{
   uint32_t enMask;
   uint32_t paramOff = (txID - SIPP_MIPI_TX0_ID) * 0x80;
   uint32_t iBufOff  = (txID - SIPP_MIPI_TX0_ID) * 0x1C;

 //Enable the TX of interest (RX is enabled as Opipe setup...)
   enMask = GET_REG_WORD_VAL(SIPP_CONTROL_ADR);
 //If not already enabled, enable next
   if((enMask &  (1<<txID))==0){
       enMask |= (1<<txID);
       SET_REG_WORD(SIPP_CONTROL_ADR, enMask);
   }

 //Prepare to run (this own't actually run till timing-generator is started)
   SET_REG_WORD(iBufOff + I_CTX(SIPP_MIPI_TX0_ID),  (1<<31)       // context update
                                                   |(1<<30));     // start (also kicks NOWRAP mode !)

 //The actual trigger:
   SET_REG_WORD(paramOff + SIPP_MIPI_TX0_TIMING_GEN_TRIG_ADR, 1); // One shot mode
}

//#################################################################################################
// Just sets some default TX params
void defaultMipiTxLoopParams(oMipiTxLoopbackParam *mipiTxCfg, uint32_t txID, uint32_t iBuf, uint32_t bpp, uint32_t imgW, uint32_t imgH)
{
   mipiTxCfg->txID    = txID; //SIPP_MIPI_TX0_ID or SIPP_MIPI_TX1_ID
   mipiTxCfg->imgAddr = (uint8_t*)iBuf;
   mipiTxCfg->imgW    = imgW;
   mipiTxCfg->imgH    = imgH;
   mipiTxCfg->bpp     =  bpp; //1 or 2
   mipiTxCfg->hbp     =   40; //pclk
   mipiTxCfg->hfp     =   60; //pclk
   mipiTxCfg->hsync   =   20; //pclk
   mipiTxCfg->vsync   =    8; //lines
}

//#################################################################################################
void OpipeTestInit()
{
   #ifdef BUP_PLL0_FREQ
    #ifndef BUP_MCLK_DIV_NUM
     #define BUP_MCLK_DIV_NUM (DONT_TOUCH_MASTER_DIVIDER)
    #endif
    #ifndef BUP_MCLK_DIV_DEN
     #define BUP_MCLK_DIV_DEN (DONT_TOUCH_MASTER_DIVIDER)
    #endif
    DrvCprInit();
    DrvCprConfigureSystemClk(BUP_PLL0_FREQ, BUP_MCLK_DIV_NUM, BUP_MCLK_DIV_DEN);
   #endif

   DrvCprStartAllClocks();
   SET_REG_WORD(CPR_AUX_CLK_EN_ADR, 0x0FFFFFFF & ~(AUX_CLK_MASK_DDR_CORE_CTRL)); //for mipi tests...
   DrvLL2CInitWriteThrough();
   swcLeonSetPIL(2); //Interrupts get enabled here, by lowering the priority
   unitTestInit();

  #if 0 //clock opt: kill all sipp clocks ("#if 0" lets everything enabled)
  //Disable all sipp clocks (except APB slave, so I can config filters)
   SET_REG_WORD(MSS_SIPP_CLK_CTRL_ADR, 1<<MSS_SP_SIPP_ABPSLV);
  #endif
}

//#################################################################################################
void OpipeDelay(uint32_t numx100)
{
   while(--numx100)
   {
      NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
      NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
      NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
      NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
      NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
      NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
      NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
      NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
      NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
      NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
   }
}

//#################################################################################################
//Bypass Leon's L1 and L2 caches on compares
static inline uint32_t bypassLeonCaches(uint32_t addr)
{
    if((addr >> 28) == 0x7) return addr | 0x08000000; //CMX addr
    if((addr >> 28) == 0x8) return addr | 0x40000000; //DDR addr
    return addr; //else ?
}
//=================================================================================================
void oMemCompare(void* _refA, void* _refB, int len)
{
   #if (VCSHOOKS_USE_SOFTWARE==1)
    uint32_t refA = bypassLeonCaches((uint32_t)_refA);
    uint32_t refB = bypassLeonCaches((uint32_t)_refB);
   #else
    uint32_t refA = (uint32_t)_refA;
    uint32_t refB = (uint32_t)_refB;
   #endif

    unitTestMemCompare((void*)refA, (void*)refB, len);
}
//#################################################################################################
void DrvSetSliceDbyrLumaBuff(Opipe *p, u32 sWidth, u32 firstSlc)
{
    CLR_REG_BITS_MASK(SIPP_CONTROL_ADR,p->enMask); //Temporally disable filters
	  //Note: imgW = 4208, will distribute as 2048, 2048, 112 pixels

    SET_REG_WORD(SIPP_SLC_SIZE_ADR,  (          128*1024)     // slice size
                                     |( (firstSlc  )<<24)    // 1st slice
                                     |( (firstSlc+2)<<28) ); // last slice

    //Set sliceWidth, FirstOutSlice, IRQ_RATE_POW
    SET_REG_WORD(O_IR(SIPP_DBYR_LUMA_ID), ((sWidth * BPP(2)) << 16) | (firstSlc << 8) | IRQ_RATE_POW);
    SET_REG_WORD(I_IR(SIPP_DOGL_ID)     , ((sWidth * BPP(2)) << 16) | (firstSlc << 8) | IRQ_RATE_POW);
    SET_REG_WORD(I_IR(SIPP_MED_LUMA_ID) , ((sWidth * BPP(2)) << 16) | (firstSlc << 8) | IRQ_RATE_POW);

	  //Must also update line-Strides
    SET_REG_WORD(O_LS(SIPP_DBYR_LUMA_ID), sWidth * BPP(2));
    SET_REG_WORD(I_LS(SIPP_DOGL_ID)     , sWidth * BPP(2));
    SET_REG_WORD(I_LS(SIPP_MED_LUMA_ID) , sWidth * BPP(2));

    SET_REG_BITS_MASK(SIPP_CONTROL_ADR,p->enMask);   //Re-enable filters
}
