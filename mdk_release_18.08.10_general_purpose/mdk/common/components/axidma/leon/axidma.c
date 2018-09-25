#include <stdlib.h>
#include <stdio.h>

#include <registersMyriad.h>
#include <swcLeonUtils.h>
#include <swcWhoAmI.h>
#include <VcsHooksApi.h>
#include <DrvRegUtils.h>
#include <DrvIcb.h>
#include <DrvMutex.h>
#include <UnitTestApi.h>

#if defined(MA2480) || defined(MA2485) || defined(MA2490)

#define DEBUG_AXIDMA
/*****************************************************************************************
 * Private defines
 ****************************************************************************************/
#define AXIDMA_MUTEX 31

#include <axidma.h>
/*****************************************************************************************
 * Private types
 ****************************************************************************************/
struct axidma_ch_tr_ctl_regs {
    uint32_t cfg_0;
    uint32_t cfg_1;
    uint32_t ctl_0;
    uint32_t ctl_1;
    uint32_t blk_0;
    uint32_t blk_1;
    uint32_t sar_0;
    uint32_t sar_1;
    uint32_t dar_0;
    uint32_t dar_1;
};

/*****************************************************************************************
 * Private data
 ****************************************************************************************/
static volatile struct axidma_drv_ctl_s {
    uint8_t taken;
    uint8_t irq_en; /* Not really needed */
    uint8_t src_tr_width;
    uint8_t dst_tr_width;
    uint32_t blk_cnt;
    struct axidma_ch_tr_ctl_regs* tr_crl; /* latest channel configuration. For linked list or shadow registers transfer can be NULL */
    uint32_t cfg_0_reg;
    uint32_t cfg_1_reg;
    uint32_t ctl_0_reg;
    uint32_t ctl_1_reg;
    void(*handler)(uint32_t, uint32_t);
} axidma_drv_ctl[AXIDMA_CH_NUM];

/*****************************************************************************************
 * Private functions
 ****************************************************************************************/
static inline void axidma_ch_trf_start(uint32_t);
static inline void axidma_ch_trf_stop(uint32_t);
static inline void axidma_ch_susp_start(uint32_t);
static inline void axidma_ch_susp_stop(uint32_t);
static inline void axidma_ch_trf_cfg(uint32_t, void*, void*, size_t);
static inline void axidma_ch_cmn_cfg(uint32_t, struct axidma_ch_cmn_cfg_s*);
static inline void axidma_ch_dst_cfg(uint32_t, struct axidma_ch_blk_cfg_s*);
static inline void axidma_ch_src_cfg(uint32_t, struct axidma_ch_blk_cfg_s*);

static void axidma_irq_handler(uint32_t);

/**
 * AXI DMAC initialisation
 ****************************************************************************************/
void axidma_init(uint32_t dmac_cmn_irq_en, uint32_t doReset)
{
    uint32_t idx;
    #ifndef MA2490
    volatile uint32_t mutex;
    #endif
    volatile uint32_t reg_data;

    for (idx = 0; idx < AXIDMA_CH_NUM; idx ++) {
        axidma_drv_ctl[idx].taken = 0;
        axidma_drv_ctl[idx].handler = NULL;
    }

    /* Reset DMAC controller */
    if (doReset)
       SET_REG_WORD(AXIDMA0_RESET_0_ADR, (1 << AXIDMA_DMAC_RESETREG_DMAC_RST_POS));
    do {
        reg_data = GET_REG_WORD_VAL(AXIDMA0_RESET_0_ADR);
    } while ((reg_data & AXIDMA_DMAC_RESETREG_DMAC_RST_MASK) != 0);

    /* Enable DMAC interrupt */

    #ifdef MA2490
    if (PROCESS_LEON_RT0 == swcWhoAmI())
    {
        #ifdef DEBUG_AXIDMA
        vcsFastPuts("Running on LeonRT0. Setting Interrupts.");
        #endif
         //already done in DrvIcbSetupIrq(..)
        // DrvIcbDisableIrq(IRQ_DYNAMIC_BASE);
        // DrvIcbIrqClear(IRQ_DYNAMIC_BASE); //already done in the following function

        #ifdef DEBUG_AXIDMA
        vcsFastPuts("DrvIcbSetupIrq");
        #endif
        DrvIcbSetupIrq(IRQ_DYNAMIC_BASE, 1, POS_EDGE_INT, axidma_irq_handler);
        #ifdef DEBUG_AXIDMA
        vcsFastPuts("DrvIcbDynamicIrqConfig");
        #endif
        DrvIcbDynamicIrqConfig(ICB_A53, A53_IRQ_PBI_DMA_LRT, IRQ_DYNAMIC_BASE, 1);
        #ifdef DEBUG_AXIDMA
        vcsFastPuts("DrvIcbEnableIrq");
        #endif
        DrvIcbEnableIrq(IRQ_DYNAMIC_BASE);
    }
    else if (PROCESS_LEON_RT1 == swcWhoAmI())
    {
        DrvIcbSetupIrq(IRQ_DYNAMIC_BASE, 1, POS_EDGE_INT, axidma_irq_handler);
        DrvIcbDynamicIrqConfig(ICB_A53, A53_IRQ_PBI_DMA_LRT, IRQ_DYNAMIC_BASE, 1);
        DrvIcbEnableIrq(IRQ_DYNAMIC_BASE);

    }
    //TODO handle LRT1
    #else //all other platforms
    if (PROCESS_LEON_OS == swcWhoAmI()) {
        DrvIcbDisableIrq(LEON_CSS_IRQ_AXI_DMA);
        DrvIcbIrqClear(LEON_CSS_IRQ_AXI_DMA);
        DrvIcbSetupIrq(LEON_CSS_IRQ_AXI_DMA, 1, POS_EDGE_INT, axidma_irq_handler);
        DrvIcbEnableIrq(LEON_CSS_IRQ_AXI_DMA);
    }

    if (PROCESS_LEON_RT == swcWhoAmI()) {
        DrvIcbDisableIrq(IRQ_DYNAMIC_0);
        DrvIcbIrqClear(IRQ_DYNAMIC_0);

        DrvIcbSetupIrq(IRQ_DYNAMIC_0, 1, POS_EDGE_INT, axidma_irq_handler);
        DrvIcbDynamicIrqConfig(LOS_TO_LRT, LEON_CSS_IRQ_AXI_DMA_MSS, IRQ_DYNAMIC_0, 1);
        DrvIcbEnableIrq(IRQ_DYNAMIC_0);
    }
    #endif //defined (MA2490)

    /* Clear any status pending */
    SET_REG_WORD(AXIDMA0_COMMON_INT_CLEAR_0_ADR, AXIDMA_DMAC_INTSTATUS_MASK);

    /* Allow DMAC common events to be saved in common status register */
    SET_REG_WORD(AXIDMA0_COMMON_INT_STATUS_ENABLE_0_ADR, AXIDMA_DMAC_INTSTATUS_MASK);

    if (dmac_cmn_irq_en) {
        SET_REG_WORD(AXIDMA0_COMMON_INT_SIGNAL_ENABLE_0_ADR, AXIDMA_DMAC_INTSTATUS_MASK);
    }
    #ifndef MA2490

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_init: wait mutex lock");
    #endif

    do {
        mutex = DrvMutexTryLock(AXIDMA_MUTEX);
    } while (mutex != 1);
    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_init: mutex locked");
    #endif

    #endif

    reg_data = GET_REG_WORD_VAL(AXIDMA0_CFG_0_ADR);
    reg_data |= (1 << AXIDMA_DMAC_CFG_0_DMAC_EN_POS);
    reg_data |= (1 << AXIDMA_DMAC_CFG_0_INT_EN_POS);
    SET_REG_WORD(AXIDMA0_CFG_0_ADR, reg_data);

    #ifndef MA2490
    DrvMutexUnlock(AXIDMA_MUTEX);

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_init: mutex unlock");
    #endif

    #endif

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_init: Done");
    #endif
}

/**
 * Open a DMA channel
 ****************************************************************************************/
int32_t axidma_channel_open(uint32_t ch_id, uint32_t  irq_en, void(*irq_handler)(uint32_t, uint32_t))
{
    uint32_t reg;
    #ifndef MA2490
    uint32_t mutex;
    #endif
    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_channel_open: Entering.");
    #endif

    if (axidma_drv_ctl[ch_id].taken == 1)
    {
        #ifdef DEBUG_AXIDMA
        printMsgInt("axidma_channel_open - This channel is already taken:", (unsigned int)ch_id);
        #endif
        return -1;
    }
    else
    {
        #ifndef MA2490

        #ifdef DEBUG_AXIDMA
        vcsFastPuts("axidma_channel_open: wait mutex lock");
        #endif

        do
        {
            mutex = DrvMutexTryLock(AXIDMA_MUTEX);
        }
        while (mutex != 1);
        #endif


        #ifdef MA2490
        reg = GET_REG_WORD_VAL(AXIDMA0_CTRL_DMA_CFG_ADR);
        if (PROCESS_LEON_RT0 == swcWhoAmI()) {
            #ifdef DEBUG_AXIDMA
            vcsFastPuts("axidma_channel_open: Running on LRT");
            #endif
            reg |= (1 << ch_id) << 4;
        }
        //TODO handle LRT1
        SET_REG_WORD (AXIDMA0_CTRL_DMA_CFG_ADR, reg); //FIXME is this still true? Might move into AXIDMA APB slave

        #else // all other platforms
        reg = GET_REG_WORD_VAL(CPR_GEN_CTRL2_ADR);
        if (PROCESS_LEON_OS == swcWhoAmI()) {
            reg &= ~(1 << ch_id);
        }
        if (PROCESS_LEON_RT == swcWhoAmI()) {
            reg |= (1 << ch_id);
        }
        SET_REG_WORD (CPR_GEN_CTRL2_ADR, reg);
        #endif //defined MA2490

        #ifndef MA2490
        DrvMutexUnlock(AXIDMA_MUTEX);

        #ifdef DEBUG_AXIDMA
        vcsFastPuts("axidma_channel_open: mutex unlocked");
        #endif

        #endif

        axidma_drv_ctl[ch_id].taken = 1;
        axidma_drv_ctl[ch_id].handler = irq_handler;
        axidma_drv_ctl[ch_id].irq_en = irq_en;
        axidma_drv_ctl[ch_id].tr_crl = NULL;
        axidma_drv_ctl[ch_id].cfg_0_reg = 0;
        axidma_drv_ctl[ch_id].cfg_1_reg = 0;
        axidma_drv_ctl[ch_id].ctl_0_reg = 0;
        axidma_drv_ctl[ch_id].ctl_1_reg = 0;

        AXIDMA_CHANNEL_WRITE_REG(ch_id, INTSTATUS_ENABLE_0, AXIDMA_CHANNEL_ALL_STATUS_MASK);
        if (irq_en) {
            AXIDMA_CHANNEL_WRITE_REG(ch_id, INTCLEAR_0, AXIDMA_CHANNEL_ALL_STATUS_MASK);
            AXIDMA_CHANNEL_WRITE_REG(ch_id, INTSIGNAL_ENABLE_0, AXIDMA_CHANNEL_ALL_STATUS_MASK);
        }
    }

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_channel_open: Done.");
    #endif
    return 0;
}

/**
 * Close a DMA channel
 ****************************************************************************************/
void axidma_channel_close(uint32_t ch_id, uint32_t force)
{
    volatile uint32_t reg_data;

    if (force) {
        axidma_ch_trf_stop(ch_id);
    } else {
        /* wait until current transfer is finished */
        do {
            reg_data = GET_REG_WORD_VAL(AXIDMA0_CH_EN_0_ADR);
        } while ((reg_data & (1 << ch_id)) != 0);
    }

    AXIDMA_CHANNEL_WRITE_REG(ch_id, INTCLEAR_0, AXIDMA_CHANNEL_ALL_STATUS_MASK);
    AXIDMA_CHANNEL_WRITE_REG(ch_id, INTSIGNAL_ENABLE_0, 0);

    axidma_drv_ctl[ch_id].taken = 0;
    axidma_drv_ctl[ch_id].handler = NULL;
    axidma_drv_ctl[ch_id].tr_crl = NULL;
}

/**
 * Configure a DMA channel
 ****************************************************************************************/
int32_t axidma_channel_config(uint32_t ch_id, struct axidma_ch_cmn_cfg_s* cmn_cfg, struct axidma_ch_blk_cfg_s* dst_cfg, struct axidma_ch_blk_cfg_s* src_cfg)
{
    if (axidma_drv_ctl[ch_id].taken != 1) {
        #ifdef DEBUG_AXIDMA
        printMsgInt("axidma_channel_config - This channel is not taken:", (unsigned int)ch_id);
        #endif
        return -1;
    } else {
        #ifdef DEBUG_AXIDMA
        vcsFastPuts("axidma_channel_config: Entering");
        #endif
        axidma_ch_cmn_cfg(ch_id, cmn_cfg);
        axidma_ch_src_cfg(ch_id, src_cfg);
        axidma_ch_dst_cfg(ch_id, dst_cfg);

        AXIDMA_CHANNEL_WRITE_REG(ch_id, CONFIG_0, axidma_drv_ctl[ch_id].cfg_0_reg);
        AXIDMA_CHANNEL_WRITE_REG(ch_id, CONFIG_1, axidma_drv_ctl[ch_id].cfg_1_reg);
        AXIDMA_CHANNEL_WRITE_REG(ch_id, CONTROL_0, axidma_drv_ctl[ch_id].ctl_0_reg);
        AXIDMA_CHANNEL_WRITE_REG(ch_id, CONTROL_1, axidma_drv_ctl[ch_id].ctl_1_reg);
    }

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_channel_config: Done.");
    #endif

    return 0;
}

/**
 * Run a DMA channel
 ****************************************************************************************/
int32_t axidma_channel_run(uint32_t ch_id, void* dst, void* src, size_t size)
{
    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_channel_run: Entering");
    #endif
    if (axidma_drv_ctl[ch_id].taken != 1) {
        #ifdef DEBUG_AXIDMA
        printMsgInt("axidma_channel_run - This channel is not taken:", (unsigned int)ch_id);
        #endif
        return -1;
    } else {
        axidma_ch_trf_cfg(ch_id, dst, src, size);
        axidma_ch_trf_start(ch_id);
    }
    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_channel_run: Done");
    #endif

    return 0;
}

int32_t axidma_channel_stop(uint32_t ch_id)
{
    volatile uint32_t irq_en;
    volatile uint32_t stat_en;
    volatile uint32_t stat;

    if (axidma_drv_ctl[ch_id].taken != 1) {
        #ifdef DEBUG_AXIDMA
        printMsgInt("axidma_channel_stop - This channel is not taken:", (unsigned int)ch_id);
        #endif
        return -1;
    } else {
        // Disable channel suspend interrupt temporary
        AXIDMA_CHANNEL_READ_REG(ch_id, INTSIGNAL_ENABLE_0, irq_en);
        AXIDMA_CHANNEL_WRITE_REG(ch_id, INTSIGNAL_ENABLE_0, (irq_en & (~AXIDMA_CHANNEL_INTSIGNAL_ENABLE_0_CH_SUSPENDED_MASK)));


        // Enable channel suspend event in status register
        AXIDMA_CHANNEL_READ_REG(ch_id, INTSTATUS_ENABLE_0, stat_en);
        AXIDMA_CHANNEL_WRITE_REG(ch_id, INTSTATUS_ENABLE_0, (stat_en | (1 << AXIDMA_CHANNEL_INTSTATUS_ENABLE_0_CH_SUSPENDED_POS)));

        // Suspend the channel
        axidma_ch_susp_start(ch_id);

        // Wait until channel is suspended
        do
        {
            AXIDMA_CHANNEL_READ_REG(ch_id, INTSTATUS_0, stat);
        }
        while ((stat & AXIDMA_CHANNEL_INTSTATUS_0_CH_SUSPENDED_MASK) == 0);

        // Release suspend and disable the channel
        axidma_ch_susp_stop(ch_id);
        axidma_ch_trf_stop(ch_id);

        // Restore channel status and interrupt enable registers
        AXIDMA_CHANNEL_WRITE_REG(ch_id, INTSTATUS_ENABLE_0, stat_en);
        AXIDMA_CHANNEL_WRITE_REG(ch_id, INTSIGNAL_ENABLE_0, irq_en);
    }

    return 0;
}

/**
 * Memcopy using linked lists
 ****************************************************************************************/
void axidma_memcopy(void *dst, const void *src, size_t size)
{
    uint32_t idx;
    uint32_t reg_data;
    uint32_t ch_id;
    uint32_t blks;

    /* align link list to 64 bytes and place it non-cacheable section */
    static volatile struct axidma_ll_item_s ll[3] __attribute__ ((aligned (64))) __attribute__ ((section (".cmx_direct.data")));

    /* Get 1st available DMA channel */
    do {
        ch_id = 0xFFFFFFFF;
        reg_data = GET_REG_WORD_VAL(AXIDMA0_CH_EN_0_ADR);
        for (idx = 0; idx < AXIDMA_CH_NUM; idx ++) {
            if ((reg_data & (1 << idx)) != 0) {
                continue;
            } else {
                ch_id = idx;
                break;
            }
        }
    } while (ch_id == 0xFFFFFFFF);

    /* Initialise link list items */
    for (idx = 0; idx < 3; idx ++) {
        ll[idx].ch_ctl_0 = 0;
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, NONPOSTED_LAST_WRITE, 1, ll[idx].ch_ctl_0);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, SMS, AXIDMA_CTL_MASTER_1, ll[idx].ch_ctl_0);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, SINC, AXIDMA_CTL_INC, ll[idx].ch_ctl_0);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, SRC_MSIZE, AXIDMA_CTL_MSIZE_1, ll[idx].ch_ctl_0);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, DMS, AXIDMA_CTL_MASTER_2, ll[idx].ch_ctl_0);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, DINC, AXIDMA_CTL_INC, ll[idx].ch_ctl_0);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, DST_MSIZE, AXIDMA_CTL_MSIZE_1, ll[idx].ch_ctl_0);

        ll[idx].ch_ctl_1 = 0;
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, IOC_BLK_TFR, 0, ll[idx].ch_ctl_1);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, ARLEN_EN, 0, reg_data);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, ARLEN, 0, reg_data);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, SRC_STAT_EN, 0, reg_data);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, AWLEN_EN, 0, reg_data);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, AWLEN, 0, reg_data);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, DST_STAT_EN, 0, reg_data);

        if (idx == 2) {
            ll[idx].ll_next_0 = 0;
            ll[idx].ll_next_1 = 0;
        } else {
            ll[idx].ll_next_0 = (uint32_t)(&ll[idx + 1].ll_next_0);
            ll[idx].ll_next_1 = 0;
        }

        ll[idx].src_0 = (uint32_t)src;
        ll[idx].src_1 = 0;
        ll[idx].dst_0 = (uint32_t)dst;
        ll[idx].dst_1 = 0;
        ll[idx].src_stat = 0;
        ll[idx].dst_stat = 0;
        ll[idx].ll_stat_0 = 0;
        ll[idx].ll_stat_1 = 0;
        ll[idx].rsvd_0 = 0;
        ll[idx].rsvd_1 = 0;
        ll[idx].rsvd_2 = 0;
    }

    idx = 0;

    blks = size >> AXIDMA_CTL_TR_WIDTH_128;
    if (blks != 0) {
        ll[idx].blk_cnt = blks - 1;

        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, SRC_TR_WIDTH, AXIDMA_CTL_TR_WIDTH_128, ll[idx].ch_ctl_0);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, DST_TR_WIDTH, AXIDMA_CTL_TR_WIDTH_128, ll[idx].ch_ctl_0);

        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, SHADOW_OR_LLI_VALID, 1, ll[idx].ch_ctl_1);

        /* Update next link list item source and destination addresses */
        ll[idx + 1].src_0 = ll[idx].src_0 + (blks << AXIDMA_CTL_TR_WIDTH_128);
        ll[idx + 1].dst_0 = ll[idx].dst_0 + (blks << AXIDMA_CTL_TR_WIDTH_128);

        /* Update size */
        size -= (blks << AXIDMA_CTL_TR_WIDTH_128);

        idx ++;
    }

    blks = size >> AXIDMA_CTL_TR_WIDTH_64;
    if (blks != 0) {
        ll[idx].blk_cnt = blks - 1;

        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, SRC_TR_WIDTH, AXIDMA_CTL_TR_WIDTH_64, ll[idx].ch_ctl_0);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, DST_TR_WIDTH, AXIDMA_CTL_TR_WIDTH_64, ll[idx].ch_ctl_0);

        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, SHADOW_OR_LLI_VALID, 1, ll[idx].ch_ctl_1);

        /* Update next link list item source and destination addresses */
        ll[idx + 1].src_0 = ll[idx].src_0 + (blks << AXIDMA_CTL_TR_WIDTH_64);
        ll[idx + 1].dst_0 = ll[idx].dst_0 + (blks << AXIDMA_CTL_TR_WIDTH_64);

        /* Update size */
        size -= (blks << AXIDMA_CTL_TR_WIDTH_64);

        idx ++;
    }

    blks = size >> AXIDMA_CTL_TR_WIDTH_32;
    if (blks != 0) {
        ll[idx].blk_cnt = blks - 1;
        size -= (blks << AXIDMA_CTL_TR_WIDTH_32);

        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, SRC_TR_WIDTH, AXIDMA_CTL_TR_WIDTH_32, ll[idx].ch_ctl_0);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, DST_TR_WIDTH, AXIDMA_CTL_TR_WIDTH_32, ll[idx].ch_ctl_0);

        AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, SHADOW_OR_LLI_VALID, 1, ll[idx].ch_ctl_1);

        /* No need to update next link list item source and destination addresses since in any case this is last item in linked list */

        /* Update size */
        size -= (blks << AXIDMA_CTL_TR_WIDTH_32);

        idx ++;
    }

    if (size != 0) {
        /* Remaining size are not aligned to 32 bits. Should be copied by CPU */
        #ifdef DEBUG_AXIDMA
        printMsgInt("Copy with CPU [in bytes]:", size);
        #endif
    }

    /* Set last linked list item */
    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, SHADOW_OR_LLI_LAST, 1, ll[idx - 1].ch_ctl_1);

    AXIDMA_CHANNEL_READ_REG(ch_id, CONFIG_0, reg_data);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_0, SRC_MULTBLK_TYPE, AXIDMA_CFG_MULTBLK_LINKED_LIST, reg_data);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_0, DST_MULTBLK_TYPE, AXIDMA_CFG_MULTBLK_LINKED_LIST, reg_data);
    AXIDMA_CHANNEL_WRITE_REG(ch_id, CONFIG_0, reg_data);

    AXIDMA_CHANNEL_READ_REG(ch_id, CONFIG_1, reg_data);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, TT_FC, AXIDMA_CFG_TT_FC_D_M2M, reg_data);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, HS_SEL_SRC, AXIDMA_CFG_HS_SEL_SW, reg_data);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, HS_SEL_DST, AXIDMA_CFG_HS_SEL_SW, reg_data);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, CH_PRIOR, AXIDMA_MAX_CH_PRIO, reg_data);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, LOCK_CH, 0, reg_data);
    AXIDMA_CHANNEL_WRITE_REG(ch_id, CONFIG_1, reg_data);

    AXIDMA_CHANNEL_WRITE_REG(ch_id, LINKED_LIST_PTR_0, ((uint32_t)(&ll[0]) & (~0x3F)) | AXIDMA_LLP_MASTER_0);
    AXIDMA_CHANNEL_WRITE_REG(ch_id, LINKED_LIST_PTR_1, 0);

    axidma_ch_trf_start(ch_id);

    /* Wait transfer completed */
    //printf ("Wait transfer completed!\n");

    while ((ll[idx - 1].ll_stat_1 & AXIDMA_LLP_STATUS_1_DMA_TRF_DONE_MASK) != 0);

    //printf ("Done!\n");
}

/**
 *
 ****************************************************************************************/
void axidma_ch_cmn_cfg(uint32_t ch_id, struct axidma_ch_cmn_cfg_s* cfg)
{
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, TT_FC, cfg->transfer_type, axidma_drv_ctl[ch_id].cfg_1_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, CH_PRIOR, cfg->priority, axidma_drv_ctl[ch_id].cfg_1_reg);
    if ((cfg->transfer_type == AXIDMA_CFG_TT_FC_D_M2M) && cfg->lock_enable) {
        AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, LOCK_CH, 1, axidma_drv_ctl[ch_id].cfg_1_reg);
        AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, LOCK_CH_L, cfg->lock_level, axidma_drv_ctl[ch_id].cfg_1_reg);
    } else {
        AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, LOCK_CH, 0, axidma_drv_ctl[ch_id].cfg_1_reg);
    }

    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, NONPOSTED_LAST_WRITE, cfg->non_posted_write, axidma_drv_ctl[ch_id].ctl_0_reg);

    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, IOC_BLK_TFR, cfg->irq_on_block, axidma_drv_ctl[ch_id].ctl_1_reg);
}

/**
 *
 ****************************************************************************************/
void axidma_ch_src_cfg(uint32_t ch_id, struct axidma_ch_blk_cfg_s* cfg)
{
    axidma_drv_ctl[ch_id].src_tr_width =  cfg->tr_width;

    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_0, SRC_MULTBLK_TYPE, cfg->multi_blk_type, axidma_drv_ctl[ch_id].cfg_0_reg);

    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, HS_SEL_SRC, cfg->hs_sel, axidma_drv_ctl[ch_id].cfg_1_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, SRC_HWHS_POL, cfg->hs_pol, axidma_drv_ctl[ch_id].cfg_1_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, SRC_PER, cfg->hs_if, axidma_drv_ctl[ch_id].cfg_1_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, SRC_OSR_LMT, cfg->outstanding_req, axidma_drv_ctl[ch_id].cfg_1_reg);

    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, SMS, cfg->master_sel, axidma_drv_ctl[ch_id].ctl_0_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, SINC, cfg->adr_inc, axidma_drv_ctl[ch_id].ctl_0_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, SRC_TR_WIDTH, cfg->tr_width, axidma_drv_ctl[ch_id].ctl_0_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, SRC_MSIZE, cfg->tr_msize, axidma_drv_ctl[ch_id].ctl_0_reg);

    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, ARLEN_EN, cfg->axi_burst_len_en, axidma_drv_ctl[ch_id].ctl_1_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, ARLEN, cfg->axi_burst_len, axidma_drv_ctl[ch_id].ctl_1_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, SRC_STAT_EN, cfg->stat_fetch_en, axidma_drv_ctl[ch_id].ctl_1_reg);

    /* TODO: decide where you set cfg->stat_fetch_adr */
}

/**
 *
 ****************************************************************************************/
void axidma_ch_dst_cfg(uint32_t ch_id, struct axidma_ch_blk_cfg_s* cfg)
{
    axidma_drv_ctl[ch_id].dst_tr_width =  cfg->tr_width;

    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_0, DST_MULTBLK_TYPE, cfg->multi_blk_type, axidma_drv_ctl[ch_id].cfg_0_reg);

    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, HS_SEL_DST, cfg->hs_sel, axidma_drv_ctl[ch_id].cfg_1_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, DST_HWHS_POL, cfg->hs_pol, axidma_drv_ctl[ch_id].cfg_1_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, DST_PER, cfg->hs_if, axidma_drv_ctl[ch_id].cfg_1_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONFIG_1, DST_OSR_LMT, cfg->outstanding_req, axidma_drv_ctl[ch_id].cfg_1_reg);

    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, DMS, cfg->master_sel, axidma_drv_ctl[ch_id].ctl_0_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, DINC, cfg->adr_inc, axidma_drv_ctl[ch_id].ctl_0_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, DST_TR_WIDTH, cfg->tr_width, axidma_drv_ctl[ch_id].ctl_0_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_0, DST_MSIZE, cfg->tr_msize, axidma_drv_ctl[ch_id].ctl_0_reg);

    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, AWLEN_EN, cfg->axi_burst_len_en, axidma_drv_ctl[ch_id].ctl_1_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, AWLEN, cfg->axi_burst_len, axidma_drv_ctl[ch_id].ctl_1_reg);
    AXIDMA_CHANNEL_SET_REG_FIELD(CONTROL_1, DST_STAT_EN, cfg->stat_fetch_en, axidma_drv_ctl[ch_id].ctl_1_reg);

    /* TODO: decide where you set cfg->stat_fetch_adr */
}

void axidma_ch_trf_cfg(uint32_t ch_id, void* dst, void* src, size_t len)
{
    axidma_drv_ctl[ch_id].blk_cnt = len >> axidma_drv_ctl[ch_id].src_tr_width;

    AXIDMA_CHANNEL_WRITE_REG(ch_id, SAR_0, (uint32_t)src);
    AXIDMA_CHANNEL_WRITE_REG(ch_id, SAR_1, 0);

    AXIDMA_CHANNEL_WRITE_REG(ch_id, DAR_0, (uint32_t)dst);
    AXIDMA_CHANNEL_WRITE_REG(ch_id, DAR_1, 0);

    AXIDMA_CHANNEL_WRITE_REG(ch_id, BLOCK_TS_0, axidma_drv_ctl[ch_id].blk_cnt - 1);
    AXIDMA_CHANNEL_WRITE_REG(ch_id, BLOCK_TS_1, 0);
}

void axidma_ch_trf_start(uint32_t ch_id)
{
    uint32_t reg_data;
    #ifndef MA2490
    uint32_t mutex;

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_trf_start: wait mutex lock");
    #endif

    do {
        mutex = DrvMutexTryLock(AXIDMA_MUTEX);
    } while (mutex != 1);

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_trf_start: mutex locked");
    #endif

    #endif

    reg_data = GET_REG_WORD_VAL(AXIDMA0_CH_EN_0_ADR);
    reg_data |= (1 << (ch_id + AXIDMA_DMAC_CH_EN_0_CH_EN_POS));
    reg_data |= (1 << (ch_id + AXIDMA_DMAC_CH_EN_0_CH_EN_WE_POS));
    SET_REG_WORD(AXIDMA0_CH_EN_0_ADR, reg_data);

    #ifndef MA2490
    DrvMutexUnlock(AXIDMA_MUTEX);

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_trf_start: mutex unlocked");
    #endif

    #endif
}

void axidma_ch_trf_stop(uint32_t ch_id)
{
    uint32_t reg_data;
    #ifndef MA2490
    uint32_t mutex;

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_trf_stop: wait mutex lock");
    #endif

    do {
        mutex = DrvMutexTryLock(AXIDMA_MUTEX);
    } while (mutex != 1);

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_trf_stop: mutex locked");
    #endif

    #endif

    reg_data = GET_REG_WORD_VAL(AXIDMA0_CH_EN_0_ADR);
    reg_data &= ~(1 << (ch_id + AXIDMA_DMAC_CH_EN_0_CH_EN_POS));
    reg_data |= (1 << (ch_id + AXIDMA_DMAC_CH_EN_0_CH_EN_WE_POS));
    SET_REG_WORD(AXIDMA0_CH_EN_0_ADR, reg_data);

    #ifndef MA2490
    DrvMutexUnlock(AXIDMA_MUTEX);

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_trf_stop: mutex unlocked");
    #endif

    #endif
}

void axidma_ch_susp_start(uint32_t ch_id)
{
    uint32_t reg_data;
    #ifndef MA2490
    uint32_t mutex;

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_susp_start: wait mutex lock");
    #endif

    do {
        mutex = DrvMutexTryLock(AXIDMA_MUTEX);
    } while (mutex != 1);

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_susp_start: mutex locked");
    #endif

    #endif

    reg_data = GET_REG_WORD_VAL(AXIDMA0_CH_EN_0_ADR);
    reg_data |= (1 << (ch_id + AXIDMA_DMAC_CH_EN_0_CH_SUSP_POS));
    reg_data |= (1 << (ch_id + AXIDMA_DMAC_CH_EN_0_CH_SUSP_WE_POS));
    SET_REG_WORD(AXIDMA0_CH_EN_0_ADR, reg_data);

    #ifndef MA2490
    DrvMutexUnlock(AXIDMA_MUTEX);

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_susp_start: mutex unlocked");
    #endif

    #endif
}

void axidma_ch_susp_stop(uint32_t ch_id)
{
    uint32_t reg_data;
    #ifndef MA2490
    uint32_t mutex;

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_susp_stop: wait mutex lock");
    #endif

    do {
        mutex = DrvMutexTryLock(AXIDMA_MUTEX);
    } while (mutex != 1);

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_susp_stop: mutex locked");
    #endif

    #endif

    reg_data = GET_REG_WORD_VAL(AXIDMA0_CH_EN_0_ADR);
    reg_data &= ~(1 << (ch_id + AXIDMA_DMAC_CH_EN_0_CH_SUSP_POS));
    reg_data |= (1 << (ch_id + AXIDMA_DMAC_CH_EN_0_CH_SUSP_WE_POS));
    SET_REG_WORD(AXIDMA0_CH_EN_0_ADR, reg_data);

    #ifndef MA2490
    DrvMutexUnlock(AXIDMA_MUTEX);

    #ifdef DEBUG_AXIDMA
    vcsFastPuts("axidma_ch_susp_stop: mutex unlocked");
    #endif

    #endif
}

void axidma_irq_handler(uint32_t src)
{
    (void)src;

    uint32_t irq;
    uint32_t stat;
    uint32_t ch_idx;
    uint32_t proc_id;
    uint32_t ch_cfg;
    uint32_t irq_serve;

    /* The DMA interrupts is on edge so 1st clear it */
    #ifdef MA2490
    DrvIcbIrqClear(A53_IRQ_PBI_DMA_LRT);
    #else // all other platforms
    DrvIcbIrqClear(LEON_CSS_IRQ_AXI_DMA);
    #endif

    proc_id = swcWhoAmI();


    irq = GET_REG_WORD_VAL(AXIDMA0_INT_STATUS_0_ADR);

    #ifdef DEBUG_AXIDMA
    printMsgInt("axidma_irq_handler: interrupt source: ", (unsigned int)irq);
    #endif

    if ((irq & AXIDMA_DMAC_INT_STATUS_0_COMMON_INT_STAT_MASK) != 0)
    {
        stat = GET_REG_WORD_VAL(AXIDMA0_COMMON_INT_STATUS_0_ADR);

        /* clear errors */
        SET_REG_WORD(AXIDMA0_COMMON_INT_CLEAR_0_ADR, stat);

        /* inform all clients about the error */
        for (ch_idx = 0; ch_idx < AXIDMA_CH_NUM; ch_idx ++)
        {
            if (axidma_drv_ctl[ch_idx].handler != NULL)
            {
                axidma_drv_ctl[ch_idx].handler(AXIDMA_CH_NUM, stat);
            }
        }
    }

    if ((irq & AXIDMA_DMAC_INT_STATUS_0_CHANNELS_INT_STAT_MASK) != 0)
    {
        for (ch_idx = 0; ch_idx < AXIDMA_CH_NUM; ch_idx++)
        {
            irq_serve = 0;
            if ((irq & (1 << ch_idx)) != 0)
            {

                #ifdef MA2490
                ch_cfg = ( GET_REG_WORD_VAL(AXIDMA0_CTRL_DMA_CFG_ADR) & AXIDMA_CTRL_DMA_CFG_INT_SEL_MASK ) >> AXIDMA_CTRL_DMA_CFG_INT_SEL_POS;
                if ((proc_id == PROCESS_LEON_RT0) && ((ch_cfg & (1 << ch_idx)) != 0)) {
                    #ifdef DEBUG_AXIDMA
                    vcsFastPuts("Channel interrupt assigned to Leon RT0");
                    #endif
                    irq_serve = 1;
                }
                if ((proc_id == PROCESS_LEON_RT1) && ((ch_cfg & (1 << ch_idx)) != 0)) {
                    #ifdef DEBUG_AXIDMA
                    vcsFastPuts("Channel interrupt assigned to Leon RT1");
                    #endif
                    irq_serve = 1;
                }
                //TODO handle LRT1

                #else //all other platforms
                ch_cfg = GET_REG_WORD_VAL(CPR_GEN_CTRL2_ADR);
                if ((proc_id == PROCESS_LEON_OS) && ((ch_cfg & (1 << ch_idx)) == 0)) {
                    #ifdef DEBUG_AXIDMA
                    vcsFastPuts("Channel interrupt assigned to Leon OS");
                    #endif
                    irq_serve = 1;
                }

                if ((proc_id == PROCESS_LEON_RT) && ((ch_cfg & (1 << ch_idx)) != 0)) {
                    #ifdef DEBUG_AXIDMA
                    vcsFastPuts("Channel interrupt assigned to Leon RT");
                    #endif
                    irq_serve = 1;
                }
                #endif //defined (MA2490)

                if (1 == irq_serve) {
                    #ifdef DEBUG_AXIDMA
                    vcsFastPuts("Serving interrupt");
                    #endif

                    AXIDMA_CHANNEL_READ_REG(ch_idx, INTSTATUS_0, stat);

                    if (axidma_drv_ctl[ch_idx].handler != NULL)
                    {
                        axidma_drv_ctl[ch_idx].handler(ch_idx, stat);
                    }
                    else
                    {
                        #ifdef DEBUG_AXIDMA
                        vcsFastPuts("No Channel ISR provided. Clearing channel's INTSTATUS.");
                        #endif
                        /* No callback provided. Just clear. */
                        AXIDMA_CHANNEL_WRITE_REG(ch_idx, INTCLEAR_0, stat);
                    }
                }
            }
        }
    }
}

#endif /* MA2480 */
