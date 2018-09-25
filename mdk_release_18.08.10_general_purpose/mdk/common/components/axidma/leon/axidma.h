#ifndef _AXIDMA_H_
#define _AXIDMA_H_

#include <stdint.h>
#include <stddef.h>

#include "axidma_defs.h"

/**
 *
 ****************************************************************************************/
struct axidma_ch_cmn_cfg_s {
    uint8_t transfer_type;
    uint8_t priority;
    uint8_t lock_enable;
    uint8_t lock_level;
    uint8_t non_posted_write;
    uint8_t irq_on_block;
};

/**
 *
 ****************************************************************************************/
struct axidma_ch_blk_cfg_s {
    uint8_t multi_blk_type;
    uint8_t hs_sel;
    uint8_t hs_pol;
    uint8_t hs_if;
    uint8_t outstanding_req;
    uint8_t master_sel;
    uint8_t adr_inc;
    uint8_t tr_width;
    uint8_t tr_msize;
    uint8_t axi_burst_len_en;
    uint8_t axi_burst_len;
    uint8_t stat_fetch_en;
    uint8_t stat_fetch_adr;
};

/**
 *
 ****************************************************************************************/
struct axidma_ll_item_s {
    uint32_t src_0;
    uint32_t src_1;
    uint32_t dst_0;
    uint32_t dst_1;
    uint32_t blk_cnt;
    uint32_t rsvd_0;
    uint32_t ll_next_0;
    uint32_t ll_next_1;
    uint32_t ch_ctl_0;
    uint32_t ch_ctl_1;
    uint32_t src_stat;
    uint32_t dst_stat;
    uint32_t ll_stat_0;
    uint32_t ll_stat_1;
    uint32_t rsvd_1;
    uint32_t rsvd_2;
};

/**
 *
 ****************************************************************************************/
void axidma_init(uint32_t, uint32_t);

/**
 *
 ****************************************************************************************/
int32_t axidma_channel_open(uint32_t, uint32_t, void(*)(uint32_t, uint32_t));

/**
 *
 ****************************************************************************************/
void axidma_channel_close(uint32_t, uint32_t);

/**
 *
 ****************************************************************************************/
int32_t axidma_channel_config(uint32_t, struct axidma_ch_cmn_cfg_s*, struct axidma_ch_blk_cfg_s*, struct axidma_ch_blk_cfg_s*);

/**
 *
 ****************************************************************************************/
int32_t axidma_channel_run(uint32_t, void*, void*, size_t);

/**
 *
 ****************************************************************************************/
int32_t axidma_channel_stop(uint32_t);

/**
 *
 ****************************************************************************************/
void axidma_memcopy(void*, const void*, size_t);

#endif /*_AXIDMA_H_*/
