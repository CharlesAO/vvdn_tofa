#include <mv_types.h>
#include <mvMacros.h>
#include <DrvRegUtilsDefines.h>
#include <registersMyriadMa2x5x.h>
#include <sippHwIds.h>
#include <sippHwCommon_ma2x5x.h>
#include <sippDefines.h>
#include <DrvIcb.h>
#include <Fp16Convert.h>
#include <stdio.h>
#include <semaphore.h>

sem_t sem_median;

// Convolution -----------------------------------------------------------------
typedef struct
{
    /// @sf_pfprivate yes
    u32 frmDim;           //see SIPP_CONV_FRM_DIM_ADR (Private)
	/// @sf_pfdesc configuration bit field(see SIPP_CONV_CFG_ADR)
    u32  cfg;              //see SIPP_CONV_CFG_ADR
    /// @sf_pfdesc Convolution kernel coefficients(see SIPP_CONV_COEFF_*_ADR (fp16 values))
    u32  kernel[15];       //see SIPP_CONV_COEFF_*_ADR (fp16 values)
    /// @sf_pfdesc Convolution kernel coefficients(see SIPP_CONV_COEFF_*_ADR (fp16 values))
    u32  shadowKernel[15];       //see SIPP_CONV_COEFF_*_ADR (fp16 values)
} ConvParam;

void packConv5x5CCMLocal(ConvParam* cfg, u16* ccm5x5)
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

void initConvHw(u32 width, u32 height)
{printf("init conv\n");
    const u32 paramSet = SIPP_CONV_BASE_ADR;

    // Just for testing
    const fp32 k[] =
    {
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
    };
    const u32 ksize = COUNT_OF(k);
    fp16 kfp16[ksize];

    for (u32 i = 0; i < ksize; i++)
        kfp16[i] = f16Tof32(k[i] / ksize);


    ConvParam param =
    {
        .frmDim = (height << 16) | width,
        .cfg    =       ksize  | //KERNEL SIZE
                     (1  << 3) | //OUTPUT_CLAMP
                     (0  << 4) | //ABS_BIT
                     (0  << 5) | //SQ_BIT
                     (0  << 6) | //ACCUM_BIT
                     (0  << 7) | //OUTPUT_DISABLE
                     (0  << 8),  //ACCUM_THR(fp16)
         .kernel = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         .shadowKernel = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    packConv5x5CCMLocal(&param, (u16*)kfp16);

    //Convolution specific params
    SET_REG_WORD(paramSet + 0x00,     param.frmDim);     //SIPP_CONV_FRM_DIM_ADR
    SET_REG_WORD(paramSet + 0x04,     param.cfg);        //SIPP_CONV_CFG_ADR

    //The 5x5 coefs matrix
    //(need to set all params, if the matrix is 3x3, it will be surrounded by zeros)
    SET_REG_WORD(paramSet + 0x10,     param.kernel[ 0]); //SIPP_CONV_COEFF_01_00_ADR
    SET_REG_WORD(paramSet + 0x14,     param.kernel[ 1]); //SIPP_CONV_COEFF_03_02_ADR
    SET_REG_WORD(paramSet + 0x18,     param.kernel[ 2]); //SIPP_CONV_COEFF_04_ADR
    SET_REG_WORD(paramSet + 0x1c,     param.kernel[ 3]); //SIPP_CONV_COEFF_11_10_ADR
    SET_REG_WORD(paramSet + 0x20,     param.kernel[ 4]); //SIPP_CONV_COEFF_13_12_ADR
    SET_REG_WORD(paramSet + 0x24,     param.kernel[ 5]); //SIPP_CONV_COEFF_14_ADR
    SET_REG_WORD(paramSet + 0x28,     param.kernel[ 6]); //SIPP_CONV_COEFF_21_20_ADR
    SET_REG_WORD(paramSet + 0x2c,     param.kernel[ 7]); //SIPP_CONV_COEFF_23_22_ADR
    SET_REG_WORD(paramSet + 0x30,     param.kernel[ 8]); //SIPP_CONV_COEFF_24_ADR
    SET_REG_WORD(paramSet + 0x34,     param.kernel[ 9]); //SIPP_CONV_COEFF_31_30_ADR
    SET_REG_WORD(paramSet + 0x38,     param.kernel[10]); //SIPP_CONV_COEFF_33_32_ADR
    SET_REG_WORD(paramSet + 0x3c,     param.kernel[11]); //SIPP_CONV_COEFF_34_ADR
    SET_REG_WORD(paramSet + 0x40,     param.kernel[12]); //SIPP_CONV_COEFF_41_40_ADR
    SET_REG_WORD(paramSet + 0x44,     param.kernel[13]); //SIPP_CONV_COEFF_43_42_ADR
    SET_REG_WORD(paramSet + 0x48,     param.kernel[14]); //SIPP_CONV_COEFF_44_ADR
}


// Median -----------------------------------------------------------------
typedef struct
{
    /// @sf_pfprivate yes
    u32 frmDim;          //see SIPP_MED_FRM_DIM_ADR  (Private)
    /// @sf_pfdesc configuration bit field, see SIPP_MED_CFG_ADR
    u32 cfg;             //see SIPP_MED_CFG_ADR
    /// @sf_pfdesc configuration bit field, see SIPP_MED_LUMA_ALPHA_ADR
    u32 lumaAlpha;       //see SIPP_MED_LUMA_ALPHA_ADR
} MedParam;

void setBuff(u32 isInput, u32 buffBase, u32 bufAddr, u32 lnStride, u32 height,
                 u32 nPlanes, u32 bpp)
{
    SET_REG_WORD(buffBase + 0x00 /*base*/, bufAddr);
    SET_REG_WORD(buffBase + 0x04 /*cfg */,            (0 <<  0)   //number of lines - no wrap
                                           |(          0 << 10)   //sync mode
                                           |((nPlanes-1) << 24)   //number of planes
                                           |( bpp        << 28) );//data type size
    SET_REG_WORD(buffBase + 0x08 /*ls */,  lnStride);
    SET_REG_WORD(buffBase + 0x0C /*ps */,  lnStride * height);
    SET_REG_WORD(buffBase + 0x10 /*ir */,  0); //clear potential left-overs, relevant regs
                                   //will get set later via cfgSources/cfgSinks/cfgSwLinks

    if (isInput)
    {
        SET_REG_WORD(buffBase + 0x14 /*fc */,  1 << 13); // input buff full
        SET_REG_WORD(buffBase + 0x18 /*ct */,  (1 << 30) //start in sync mode
                                              |(1 << 31));
    }
    else
    {
        SET_REG_WORD(buffBase + 0x14 /*fc */,  0);
        SET_REG_WORD(buffBase + 0x18 /*ct */,  0);
    }
}

volatile uint32_t irqEofReceived = 0;
void sippEofIsr(uint32_t irqSrc)
{
    UNUSED(irqSrc);
    uint32_t status, oldStatus;

    /*printf("IRQ\n");*/

    status = GET_REG_WORD_VAL(SIPP_INT0_STATUS_ADR);
    if (status & (1<<SIPP_MED_ID)){
        oldStatus = GET_REG_WORD_VAL(SIPP_INT0_CLEAR_ADR);
        SET_REG_WORD(SIPP_INT0_CLEAR_ADR, status | oldStatus);
        /*printf("IRQ0\n");*/
    }

    status = GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR);
    if (status & (1<<SIPP_MED_ID)){
        oldStatus = GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR);
        SET_REG_WORD(SIPP_INT1_CLEAR_ADR, status | oldStatus);
        /*printf("IRQ1\n");*/
    }

    status = GET_REG_WORD_VAL(SIPP_INT2_STATUS_ADR);
    if (status & (1<<SIPP_MED_ID)){
        oldStatus = GET_REG_WORD_VAL(SIPP_INT2_STATUS_ADR);
        SET_REG_WORD(SIPP_INT2_CLEAR_ADR, status | oldStatus);
        //irqEofReceived = 1;
        sem_post(&sem_median);
    }

//    DrvIcbIrqClear(irqSrc);
}

extern "C" void initMedHw(u8* dst, u8* src, u32 width, u32 height, u32 med_k)
{
    u32 status;
    static int oneTime = 0;
    // Median Enable EOF IRQ:
    /*SET_REG_BITS_MASK(SIPP_INT0_ENABLE_ADR, 1<<SIPP_MED_ID);*/
    /*SET_REG_BITS_MASK(SIPP_INT1_ENABLE_ADR, 1<<SIPP_MED_ID);*/
    if(!oneTime)
    {
        sem_init(&sem_median, 0, 1);
        status = GET_REG_WORD_VAL(SIPP_INT2_ENABLE_ADR);
        SET_REG_BITS_MASK(SIPP_INT2_ENABLE_ADR, (1<<SIPP_MED_ID) | status);

        /*DrvIcbDynamicIrqConfig(LRT_TO_LOS, IRQ_SIPP_0, IRQ_DYNAMIC_0, 1);*/
        /*DrvIcbDynamicIrqConfig(LRT_TO_LOS, IRQ_SIPP_1, IRQ_DYNAMIC_1, 1);*/
        DrvIcbDynamicIrqConfig(LRT_TO_LOS, IRQ_SIPP_2, IRQ_DYNAMIC_2, 1);

        /*DrvIcbSetupIrq(IRQ_DYNAMIC_0, 10, POS_LEVEL_INT, sippEofIsr);*/
        /*DrvIcbSetupIrq(IRQ_DYNAMIC_1, 10, POS_LEVEL_INT, sippEofIsr);*/
        DrvIcbSetupIrq(IRQ_DYNAMIC_2, 10, POS_LEVEL_INT, sippEofIsr);

        status = GET_REG_WORD_VAL(SIPP_CONTROL_ADR);
        SET_REG_WORD(SIPP_CONTROL_ADR, (1<<SIPP_MED_ID) | status); //enable filter
        oneTime++;
    }
    MedParam param =
    {
        .frmDim = (height << 16) | width,
        .cfg =(   (              med_k  <<  0)  // kernel size
                | (((med_k * med_k)>>1) <<  8)  // output selection - select central pixel after sort
                /*| (                 255 << 16)  //threshold : passthrgouth config*/
                | (                   0 << 28)),//Luma Alpha Blend enable = OFF
        .lumaAlpha = 0
    };

    // Set median specific params
    SET_REG_WORD(SIPP_MED_FRM_DIM_ADR,  param.frmDim);
    SET_REG_WORD(SIPP_MED_CFG_ADR,      param.cfg);
    /*SET_REG_WORD(SIPP_MED_LUMA_ALPHA_ADR,   param.lumaAlpha);   */

    // Set CMX buffers
    /*setBuff(0, 0x70A00000, (uint32_t)outMed, width, height, 1, 1);*/
    setBuff(0, SIPP_OBUF8_BASE_ADR, (uint32_t)dst, width, height, 1, 1);
    setBuff(1, SIPP_IBUF8_BASE_ADR, (uint32_t)src, width, height, 1, 1);
}

extern "C" void runMedHw()
{
    /*SET_REG_WORD(SIPP_START_ADR, 1 << SIPP_MED_ID);*/
    /*printf("SIPP_STATUS: 0x%x\n", GET_REG_WORD_VAL(SIPP_STATUS_ADR));*/
    /*while (irqEofReceived == 0)
    {
        //rtems_task_wake_after(0);
        NOP;NOP;NOP;NOP;NOP;
    }
    irqEofReceived = 0;*/
    sem_wait(&sem_median);

    /*printf("SIPP_OCTX8: 0x%x\n", GET_REG_WORD_VAL(SIPP_OCTX8_ADR));*/
}
