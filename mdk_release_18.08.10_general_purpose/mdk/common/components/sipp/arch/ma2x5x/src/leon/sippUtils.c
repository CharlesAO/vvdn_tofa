///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

//#############################################################################
//Given a 5x5 typical conv matrix, this routine
//packs the coefficients as required by HW reg layout
//the ccm is a fp16 matrix
void packConv5x5CCM(ConvParam *cfg, u16 *ccm5x5)
{
    cfg->kernel[ 0] = (ccm5x5[ 1]<<16) | ccm5x5[ 0];
    cfg->kernel[ 1] = (ccm5x5[ 3]<<16) | ccm5x5[ 2];
    cfg->kernel[ 2] =                    ccm5x5[ 4];
    cfg->kernel[ 3] = (ccm5x5[ 6]<<16) | ccm5x5[ 5];
    cfg->kernel[ 4] = (ccm5x5[ 8]<<16) | ccm5x5[ 7];
    cfg->kernel[ 5] =                    ccm5x5[ 9];
    cfg->kernel[ 6] = (ccm5x5[11]<<16) | ccm5x5[10];
    cfg->kernel[ 7] = (ccm5x5[13]<<16) | ccm5x5[12];
    cfg->kernel[ 8] =                    ccm5x5[14];
    cfg->kernel[ 9] = (ccm5x5[16]<<16) | ccm5x5[15];
    cfg->kernel[10] = (ccm5x5[18]<<16) | ccm5x5[17];
    cfg->kernel[11] =                    ccm5x5[19];
    cfg->kernel[12] = (ccm5x5[21]<<16) | ccm5x5[20];
    cfg->kernel[13] = (ccm5x5[23]<<16) | ccm5x5[22];
    cfg->kernel[14] =                    ccm5x5[24];
}

//#############################################################################
//Given a 3x3 typical conv matrix, this routine
//packs the coefs as required by HW reg layout
//NOTE: the convolution coefs are 5x5, and the outer coefs need to be set as ZERO
void packConv3x3CCM(ConvParam *cfg, u16 *ccm3x3)
{
   //To check.. never tested...
    cfg->kernel[ 0] = 0;
    cfg->kernel[ 1] = 0;
    cfg->kernel[ 2] = 0;

    cfg->kernel[ 3] = (ccm3x3[0]<<16);
    cfg->kernel[ 4] = (ccm3x3[2]<<16) | ccm3x3[1];
    cfg->kernel[ 5] = 0;

    cfg->kernel[ 6] = (ccm3x3[3]<<16);
    cfg->kernel[ 7] = (ccm3x3[5]<<16) | ccm3x3[4];
    cfg->kernel[ 8] = 0;

    cfg->kernel[ 9] = (ccm3x3[6]<<16);
    cfg->kernel[10] = (ccm3x3[8]<<16) | ccm3x3[7];
    cfg->kernel[11] = 0;

    cfg->kernel[12] = 0;
    cfg->kernel[13] = 0;
    cfg->kernel[14] = 0;
}

//#############################################################################
//packing a typical 3x3 CCM into Octave format (as CC-HW expects)
#define CCM_COEF(x) (u16)(x*1024)
void packColCombCCM(ColCombParam *cfg, float *ccm3x3)
{
   cfg->ccm[0]  = CCM_COEF(ccm3x3[0]) | (CCM_COEF(ccm3x3[3])<<16);
   cfg->ccm[1]  = CCM_COEF(ccm3x3[6]) | (CCM_COEF(ccm3x3[1])<<16);
   cfg->ccm[2]  = CCM_COEF(ccm3x3[4]) | (CCM_COEF(ccm3x3[7])<<16);
   cfg->ccm[3]  = CCM_COEF(ccm3x3[2]) | (CCM_COEF(ccm3x3[5])<<16);
   cfg->ccm[4]  = CCM_COEF(ccm3x3[8]);
}

//#############################################################################
//Packing a 5x5 u8 gauss
void packLumaDnsGaussLut(YDnsParam *cfg, u8 *lut)
{
   u32 i;
   for(i=0; i<4; i++)
   {
      cfg->gaussLut[i] = (lut[i*8 + 0]      ) |
                         (lut[i*8 + 1] << 4 ) |
                         (lut[i*8 + 2] << 8 ) |
                         (lut[i*8 + 3] << 12) |
                         (lut[i*8 + 4] << 16) |
                         (lut[i*8 + 5] << 20) |
                         (lut[i*8 + 6] << 24) |
                         (lut[i*8 + 7] << 28);

   }
}

//#############################################################################
void sippUtilComputeFp16Lut(half (*formula)(half input), half *outLut, u32 lutSize)
{
    UNUSED (formula); //hush the compiler warning
    UNUSED (outLut);  //hush the compiler warning
    UNUSED (lutSize); //hush the compiler warning

  #if defined(SIPP_PC)
    u32 i;
    u32 offLsbTab[11] = {0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    u32 offLsb;
    u32 epr = lutSize/16;        //Entries / Range
    u32 rsi = (u32)(logf((float)epr)/logf(2.0f)); //Range Size Indicator
    half   fpNum;

    offLsb = offLsbTab[rsi];

    for(i=0; i<lutSize; i++)
    {
      //Build the fp16 number that corresponds to Range, Offset, Interp=0
      //Meaning: the number that would cause lut to select cell "i"
        *((u16*)&fpNum) = i<<offLsb;

      //Get the user transform result, and store it to location "i"
        outLut[i] = formula(fpNum);
    }
  #endif
}

//#############################################################################
//Same as above, but replicate the output 4xtimes
//"In Channel Mode the LBRC read client is set up to read from up to 4 different planes at a time."
void sippUtilComputeFp16LutChannelMode(half (*formula)(half input), half *outLut, u32 lutSize)
{
    UNUSED (formula); //hush the compiler warning
    UNUSED (outLut);  //hush the compiler warning
    UNUSED (lutSize); //hush the compiler warning

  #if defined(SIPP_PC)
    u32 i;
    u32 offLsbTab[11] = {0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    u32 offLsb;
    u32 epr = lutSize/16;        //Entries / Range
    u32 rsi = (u32)(logf((float)epr)/logf(2.0f)); //Range Size Indicator
    half   fpNum;

    offLsb = offLsbTab[rsi];

    for(i=0; i<lutSize; i++)
    {
      //Build the fp16 number that corresponds to Range, Offset, Interp=0
      //Meaning: the number that would cause lut to select cell "i"
        *((u16*)&fpNum) = i<<offLsb;

      //Get the user transform result, and store it to location "i"
        outLut[i*4+0] = formula(fpNum); //Red
        outLut[i*4+1] = formula(fpNum); //Green
        outLut[i*4+2] = formula(fpNum); //Blue
        outLut[i*4+3] = formula(fpNum); ; //Pad
    }
  #endif
}

//#############################################################################
void sippUtilPrintFp16Lut(half *fp16Lut, u32 lutSize, const char *fName)
{
    UNUSED (fp16Lut); //hush the compiler warning
    UNUSED (lutSize); //hush the compiler warning
    UNUSED (fName);   //hush the compiler warning

#if defined(SIPP_PC)
    u32 i;
    FILE *f;
    const char *fNameDef = "ispParam.c";

    if(fName == NULL)
        f = fopen(fNameDef, "wb");
    else
    {
        fNameDef = (char *)fName;
        f = fopen(fNameDef, "ab");
    }

    if (f == NULL)
    {
    perror(fNameDef);
    exit(2);
    }
    fprintf(f, "\nstatic uint16_t lutTable[%d] ALIGNED(8) =", lutSize);
    fprintf(f, "\n{");

    for (i = 0; i < lutSize; i++)
    {
        if(i%8 == 0)
            fprintf(f, "\n");

        fprintf(f, "0x%04x, ", *(u16 *)&fp16Lut[i]);

    }
    fseek(f, -2, SEEK_CUR);
    fprintf(f, "\n};\n");
    fclose(f);

#endif
}

//#############################################################################
// Generate a 7x1 1D guassian kernel for a given sigma.  Only 4 values
// are returned.  The rest can be generated by mirroring the coefficients.
// Values returned in "coeffs" array are register-ready FP16 values.
void sharpenSigmaToCoefficients(float sigma, u16 *coeffs)
{
    UNUSED (sigma);  //hush the compiler warning
    UNUSED (coeffs); //hush the compiler warning

  #if defined(SIPP_PC)
    int    i, j;
    float    denom = 2 * sigma*sigma;
    float    k[4], sum = 0;
    fp16    f16;

    for (i = 0; i < 4; i++) {
        j = 3 - i;
        k[i] = expf(-(float)(j*j) / denom);
        sum += k[i];
    }

    // Coefficients should sum to 1
    sum = sum + (sum - k[3]);
    for (i = 0; i < 4; i++) {
        f16 = k[i] / sum;
        coeffs[i] = f16.getPackedValue();
    }
  #endif
}

//#############################################################################
#if defined(_WIN32)
  // fix this issue with VS2013
#if (_MSC_VER != 1800)
    float log2(float n)
    {
        return logf(n)/logf(2);
    }
#endif
#endif

//#############################################################################
// Generate Luma Denoise register settings (lut and bitpos) based on strength
void lumaGenLut(float strength, u8 *lut, int *bitpos)
{
    UNUSED (strength); //hush the compiler warning
    UNUSED (lut);      //hush the compiler warning
    UNUSED (bitpos);   //hush the compiler warning

  #if defined(SIPP_PC)
    float    alpha, divisor, sigma, npot, l;
    int    i;

    if (strength < .001f) {
        // Avoid division by 0
        strength = .001f;
    }
    if (strength > 2047.0f) {
        // Limit to prevent 'bitpos' > 11
        strength = 2047.0f;
    }
    *bitpos = (int)floor(log2(strength));    // MSB position
    if (*bitpos < 0) {
        *bitpos = 0;
    }
    npot = (float)(1<<*bitpos);        // nearest power of two (rounding down)
    alpha = (strength - npot) / npot;
    divisor = 4*(1-alpha) + 8*(alpha);
    sigma = .05f;
    if (strength < 1) {
        // Reduce sigma when 0 < strength < 1
        sigma = sigma * strength;
    }
    for (i = 0; i < 32; i++) {
        l = (float)i / 31 / divisor;
        l = expf(-(powf(l, 2)) / (2*(powf(sigma,2))));    // Gaussian

        // LUT entries will be quantized to 16 possible values
        lut[i] = (u8)(l*16-1 + .5f);
    }
  #endif
}

//#############################################################################
void sippWait(u32 numx100)
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

//#############################################################################
#if defined(MYRIAD2)

  #include <DrvSvu.h>

  #define   DBG_TXN_TYPE_CPFL1           0x0
  #define   DBG_TXN_TYPE_CPFL1L          0x1
  #define   DBG_TXN_TYPE_CPFL2           0x2
  #define   DBG_TXN_TYPE_CFLUSH          0x3
  #define   DBG_TXN_TYPE_CINVAL          0x4
  #define   DBG_TXN_TYPE_INV_ALL         0x5
  #define   DBG_TXN_TYPE_CFLUSH_ALL      0x6
  #define   DBG_TXN_TYPE_CFLUSH_INV_ALL  0x7

  static INLINE void waitWhileShavesL1Busy(u32 first, u32 last)
  {
      u32 svu;

      // Wait for all L1 caches to stop :either due to FlushInv(bit1), or data transfers(bit0)
      for(svu = first; svu <= last; svu++)
      {
          while(GET_REG_WORD_VAL(SHAVE_CACHE_STATUS(svu)) & 0x3)
          {
              NOP;NOP;NOP;NOP;NOP;
          }//while
      }//for
  }

//#############################################################################
//Config Shaves L1 caches for a range of shaves.
// - invalidate caches
// - enable both INST and LSU caches
// - set in WRITE-THROUGH to keep things coherent
// - enable DDR addresses to be cached

#define   SLC_OFFSET_IL1            ( 0x6000 )
#define   SLC_OFFSET_DL1            ( 0x7000 )

#define   SVU_DL1_CACHE_CTRL         ( 0x000 )
#define   SVU_DL1_CACHE_SVU_CTRL     ( 0x004 )
#define   SVU_DL1_CACHE_CNT_IN       ( 0x008 )
#define   SVU_DL1_CACHE_CNT_OUT      ( 0x00c )
#define   SVU_DL1_CACHE_DBG_TXN_ADDR ( 0x010 )
#define   SVU_DL1_CACHE_DBG_TXN_TYPE ( 0x014 )
#define   SVU_DL1_CACHE_STATUS       ( 0x018 )
#define   SVU_DL1_CACHE_RAM_CFG      ( 0x01C )

#define   SVU_IL1_CACHE_CTRL         ( 0x000 )
#define   SVU_IL1_CACHE_HIT_CNT      ( 0x004 )
#define   SVU_IL1_CACHE_MISS_CNT     ( 0x008 )
#define   SVU_IL1_CACHE_RAM_CFG      ( 0x00C )
#define   SVU_IL1_CACHE_STATUS       ( 0x010 )

#define   SHAVE_IL1_CACHE(svu)        (SVU_CTRL_ADDR[svu] + SLC_OFFSET_IL1)
#define   SHAVE_DL1_CACHE(svu)        (SVU_CTRL_ADDR[svu] + SLC_OFFSET_DL1)

//#############################################################################
//WARNING: only try this if shaves are STOPPED
//         There is no stop bit for IL1!
void invalidateSvuInstL1Cache(u32 first, u32 last)
{
    u32 svu;

    for (svu = first; svu <= last; svu++)
    {
        //Fergal suggests it's better to wait for busy to be cleared
        // as there might be pending reads
        while(GET_REG_WORD_VAL(SHAVE_IL1_CACHE(svu) + SVU_IL1_CACHE_STATUS) & 0x1);

        //Invalidate & enable statistics (invalidate takes 1 cc so no need to wait ...)
        SET_REG_WORD(SHAVE_IL1_CACHE(svu) + SVU_IL1_CACHE_CTRL , (1<<5)|(1<<4));
    }
}

//#############################################################################
//Data L1 cache only chaches transactions towards DDR (throuhg cmxL2)
void invalidateSvuDataL1Cache(u32 first, u32 last)
{
    u32 svu, data;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //1) Stop all L1 caches: this bit must be set in order to invalidate
    //   On CACHE_CTRL_TXN_TYPE register, it's noted :
    //   "Access is only allowed when STOP bit of SLICE_CTRL register is set)
    for (svu = first; svu <= last; svu++)
    {
        //Set stop bit
        SET_REG_WORD(SHAVE_DL1_CACHE(svu) + SVU_DL1_CACHE_CTRL,  (1<<3)   //Stop (any activity)
                                                                |(1<<0) );//Enable
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //2) Wait for all L1 caches to stop :either due to FlushInv(bit1), or data transfers(bit0)
    for (svu = first; svu <= last; svu++)
    {
        //Wait while busy
        while(GET_REG_WORD_VAL(SHAVE_DL1_CACHE(svu) + SVU_DL1_CACHE_STATUS) & 0x3);
        //Invalidate
        SET_REG_WORD(SHAVE_DL1_CACHE(svu) + SVU_DL1_CACHE_DBG_TXN_TYPE,  DBG_TXN_TYPE_INV_ALL);
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //3) Wait for all invalidation to finish:
    for (svu = first; svu <= last; svu++)
    {
        //Wait while busy
        while(GET_REG_WORD_VAL(SHAVE_DL1_CACHE(svu) + SVU_DL1_CACHE_STATUS) & 0x3);
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //4) Start all L1 caches; writethrogh mode; both INSTR and LSU caches
    //     cache DDR addresses (L2 cacheable and non-cacheable)
    //     for now all transactions route through L2 partition 0
    for (svu = first; svu <= last; svu++)
    {
        //Windowed Cache Policy Config ? SLICE_WIN_CPC

        //Non-windowed Cache Policy Config (i.e. not 0x1C... 0x1D... )
        data  = GET_REG_WORD_VAL(SLICE_NWN_CPC(svu));
        data |=  (1<<( 9   )); //LSU enable DDRL2C L1 caching
        data |=  (1<<( 8   )); //LSU enable DDRL2B L1 caching
        SET_REG_WORD(SLICE_NWN_CPC(svu), data);

        SET_REG_WORD(SHAVE_DL1_CACHE(svu) + SVU_DL1_CACHE_CTRL, (0<<3) //stop
                                                               |(0<<2) //WRITE THROUGH !!! IMPORTANT !!!
                                                               |(0<<1) //lock
                                                               |(1<<0) //ENABLE
                    );
    }
}

//#############################################################################
//Note: swcResetShave DOES NOT RESET ShaveL1 cache, so can program independently
//      before configuring shaves !
void cfgMyriad2ShaveL1(u32 first, u32 last)
{
    invalidateSvuInstL1Cache(first, last);
    invalidateSvuDataL1Cache(first, last);
}

//#############################################################################
#include <DrvLeonL2C.h>
void leonL2CacheInitWrThrough( void )
{
    LL2CConfig_t ll2Config;

    // Invalidate entire L2Cache before enabling it
    DrvLL2CFlushOpOnAllLines (LL2C_OPERATION_INVALIDATE, /*disable cache?:*/ 0);

    ll2Config.LL2CEnable       = 1;
    ll2Config.LL2CLockedWaysNo = 0;
    ll2Config.LL2CWayToReplace = 0;
    ll2Config.busUsage         = BUS_WRAPPING_MODE;
    ll2Config.hitRate          = HIT_WRAPPING_MODE;
    ll2Config.replacePolicy    = LRU;
    ll2Config.writePolicy      = WRITE_THROUGH;

	  DrvLL2CInitialize(&ll2Config);
}

//#############################################################################
// Test purpose mainly: receive data from a mipi-TX block via loopback mode
//                      and store the data in a contiguous output buffer
// Possible loopback modes for the 2 TX units are:
//  SIPP MIPI Tx[0] -> SIPP MIPI Rx[1] loopback connection
//  SIPP MIPI Tx[1] -> SIPP MIPI Rx[3] loopback connection
// RX operates in no-WRAP mode !

void cfgMipiRxLoopback (MipiRxLoopbackParam * cfg)
{
    u32 data, oBufOff, paramOff;
    u32 unitNo = cfg->rxID - SIPP_MIPI_RX0_ID;  // Should result 1 or 3

    // Do some checks here: can only set RX1 and RX3 in loopback mode
    if ((cfg->rxID != SIPP_MIPI_RX1_ID) &&
        (cfg->rxID != SIPP_MIPI_RX3_ID))
    {
        sippError ((void *)NULL, E_INVLD_MIPI_RX_LOOPBACK);
    }

    // Enable proper loopback
    data  = GET_REG_WORD_VAL(MSS_LOOPBACK_CFG_ADR);

    if (cfg->rxID == SIPP_MIPI_RX1_ID) data |= (1<<0);
    else                               data |= (1<<1);

    SET_REG_WORD(MSS_LOOPBACK_CFG_ADR, data);

    // Config the output buffer
    oBufOff = unitNo * 0x1C;

    SET_REG_WORD(oBufOff + O_BASE(SIPP_MIPI_RX0_ID) ,     (u32)cfg->imgAddr);
    SET_REG_WORD(oBufOff + O_CFG(SIPP_MIPI_RX0_ID)  ,             cfg->bpp<<28); //(nl = 0 => no_wrap mode)
    SET_REG_WORD(oBufOff + O_LS(SIPP_MIPI_RX0_ID)   ,     cfg->imgW * cfg->bpp);

    // Set remaining params:
    paramOff = unitNo * 0x80;
    SET_REG_WORD(paramOff + SIPP_MIPI_RX0_CFG_ADR    ,                0x00000001); //Use half-rate pixel clock
    SET_REG_WORD(paramOff + SIPP_MIPI_RX0_FRM_DIM_ADR, (cfg->imgH<<16)|cfg->imgW);
    SET_REG_WORD(paramOff + SIPP_MIPI_RX0_X0_ADR,      (cfg->imgW<<16)|        0); //window Horiz: from zero to width
    SET_REG_WORD(paramOff + SIPP_MIPI_RX0_Y0_ADR,      (cfg->imgH<<16)|        0); //window Vert:  from zero to height
    SET_REG_WORD(paramOff + SIPP_MIPI_RX0_SEL01_ADR,                      0xF<<8); // select all pixels on all lines
    SET_REG_WORD(paramOff + SIPP_MIPI_RX0_MASK0_ADR,                  0x000000FF); //byte mask, TBD: do this right...

    // Timings don't care for RX block; timings are TX block concern
    // The RX will know when it needs to buffer the input data based on "valid" signal
}

//#############################################################################
// Test purpose mainly: push a full-frame towards mipi RX via loopback mode
// TX operates in no-WRAP mode !

void cfgMipiTxLoopback (MipiTxLoopbackParam *cfg)
{
    u32 data;
    u32 iBufOff;
    u32 paramOff;
    u32 unitNo = cfg->txID - SIPP_MIPI_TX0_ID;

    //Enable the required loopback mode via read-modify-write (so that we allow 2 loopbacks if needed)
    data  = GET_REG_WORD_VAL(MSS_LOOPBACK_CFG_ADR);
    data |= (1 << unitNo);
    SET_REG_WORD(MSS_LOOPBACK_CFG_ADR, data);

    //Config the input buffer
    iBufOff = unitNo * 0x1C;

    SET_REG_WORD(iBufOff + I_BASE(SIPP_MIPI_TX0_ID), cfg->imgAddr);
    SET_REG_WORD(iBufOff + I_CFG(SIPP_MIPI_TX0_ID) , cfg->bpp << 28);//nl = 0 => no_wrap mode
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
 //#############################################################################

void startMipiTxLoopback (MipiTxLoopbackParam * cfg)
{
    u32 enMask;
    u32 paramOff = (cfg->txID - SIPP_MIPI_TX0_ID) * 0x80;
    u32 iBufOff  = (cfg->txID - SIPP_MIPI_TX0_ID) * 0x1C;

    // If not already enabled, enable the TX of interest (RX is enabled during pipe setup...)
    enMask = GET_REG_WORD_VAL(SIPP_CONTROL_ADR);

    if ((enMask & (0x1 << cfg->txID)) == 0)
    {
        enMask |= (0x1 << cfg->txID);

        SET_REG_WORD(SIPP_CONTROL_ADR, enMask);
    }

    SET_REG_WORD(iBufOff + I_CTX(SIPP_MIPI_TX0_ID), (0x1 << 31)       // context update
                                                   |(0x1 << 30));     // start (also kicks NOWRAP mode !)

    // The actual trigger:
    SET_REG_WORD(paramOff + SIPP_MIPI_TX0_TIMING_GEN_TRIG_ADR, 0x1);
}

//#############################################################################
// Global SIPP HW clock gating
//  action = 1 : enable clocks
//  action = 0 : gate   clocks
void sippGlobalHwClockCfg (uint32_t enable)
{
    uint32_t data;

    data = GET_REG_WORD_VAL(MSS_CLK_CTRL_ADR);
    //bit 17 : "AMC (and SIPP global clock)"
    //bit 19 : "SIPP (APB slave clock)"

    if(enable)
    {
        data |=  ((1<<17)|(1<<19));
    }
    else
    {
        data &= ~((1<<17)|(1<<19)); //gate clocks
    }

    SET_REG_WORD(MSS_CLK_CTRL_ADR, data);
}

//#############################################################################
//Enable required individual SIPP clocks

void sippHwIndividualClkEn (uint32_t mask)
{
    SET_REG_WORD(MSS_SIPP_CLK_CTRL_ADR, mask);
}

//#############################################################################
//By default (after reset), Shave-IDC has lower priority than SIPP.
//So user can increase priority if Shave is the pipeline bottleneck

void sippUpShaveCmxPriority ()
{
    //Increase Shave IDC priority within current slice!
    //Slice priorities: 8 bits per slice, thus 16 slices grouped in 4x32bit regs
    //Cfg:  1:0 Inter-slice ports = 00
    //      3:2 SHAVE IDC ports   = 11
    //      5:4 AMC ports         = 00
    //      7:6 SHAVE LSU ports   = 11
    SET_REG_WORD(CMX_SLICE_LC_PRIORITY0_CTRL, 0xCCCCCCCC);
    SET_REG_WORD(CMX_SLICE_LC_PRIORITY1_CTRL, 0xCCCCCCCC);
    SET_REG_WORD(CMX_SLICE_LC_PRIORITY2_CTRL, 0xCCCCCCCC);
    SET_REG_WORD(CMX_SLICE_LC_PRIORITY3_CTRL, 0xCCCCCCCC);
}
#endif // Myriad2


//#############################################################################################################
//Remap input params to outpts based on Bayer order
// In: [inGr inR  inB inGb] -> |out[0] out[1]|
//                             |out[2] out[3]|
//Usage scenarios: reorder blc.blackLevel values, raw.gainSat values
void sippUtilOrderPixels (u32 bayerPattern,
                          u32 inGr,
                          u32 inR,
                          u32 inB,
                          u32 inGb,
                          u32 *out)
{
    switch (bayerPattern)
    {
      case GRBG:  out[0] = inGr;  out[1] = inR;   out[2] = inB;   out[3] = inGb; break;
      case GBRG:  out[0] = inGb;  out[1] = inB;   out[2] = inR;   out[3] = inGr; break;
      case BGGR:  out[0] = inB;   out[1] = inGb;  out[2] = inGr;  out[3] = inR;  break;
      case RGGB:  out[0] = inR;   out[1] = inGr;  out[2] = inGb;  out[3] = inB;  break;
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippListSort                                                     //
//                                                                                //
//  DESCRIPTION: Used to sort a list based on a corresponding sort indices        //
//                                                                                //
//  INPUTS:                                                                       //
//                                                                                //
//  OUTPUTS:     pnList - A sorted list                                           //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       The list of sort indices is similarly sorted                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippListSort (s32 * pnList,
                   s32 * pnSortIndices,
                   u32  uSize,
                   u8   descending)
{
    u32 j, k, uTemp, idc;

    if (uSize > 1)
    {
        for (j = 0; j < uSize - 1; j = j + 1)
        {
            for (k = j + 1; k < uSize; k = k + 1)
            {
                if ((descending & (pnSortIndices[j] < pnSortIndices[k])) |
                    ((!descending) & (pnSortIndices[j] > pnSortIndices[k])))
                {
                    uTemp            = pnSortIndices[k];
                    pnSortIndices[k] = pnSortIndices[j];
                    pnSortIndices[j] = uTemp;
                    idc              = pnList[k];
                    pnList[k]        = pnList[j];
                    pnList[j]        = idc;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippFindInList                                                   //
//                                                                                //
//  DESCRIPTION: Serach for an input element in a given list                      //
//                                                                                //
//  INPUTS:      pElement  - Pointer to element                                   //
//               ppList - list                                                    //
//               numList - number of elements in ppList                           //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     -1 if not found, otherwise list entry pos                        //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

s32 sippFindInList (void *  pElement,
                    void ** ppList,
                    u32     numList)
{
    u32 listIdx;
    s32  inList;

    for (listIdx = 0, inList = -1; listIdx < numList; listIdx++)
    {
        if (pElement == ppList[listIdx])
        {
            inList = listIdx;
            break;
        }
    }

    return inList;
}
