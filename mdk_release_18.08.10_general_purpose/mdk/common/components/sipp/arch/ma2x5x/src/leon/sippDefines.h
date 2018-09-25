////// sippDefines.h

#define SIPP_CONTROL_PIPE_IDX   0x1F
#define SIPP_INVALID_HW_PIPE_ID 0xFF
#define SIPP_INVALID_PARENT_KS  0xBAD01

////////////////////////////////////////////////////////
//Nice macros to access addresses based on HW resources IDs
#define CONCAT3(A, B, C) A ## B ## C

#define I_BASE(X) CONCAT3(SIPP_IBUF, X, _BASE_ADR)
#define I_CFG(X)  CONCAT3(SIPP_IBUF, X, _CFG_ADR)
#define I_LS(X)   CONCAT3(SIPP_IBUF, X, _LS_ADR)
#define I_PS(X)   CONCAT3(SIPP_IBUF, X, _PS_ADR)
#define I_FC(X)   CONCAT3(SIPP_IBUF, X, _FC_ADR)
#define O_BASE(X) CONCAT3(SIPP_OBUF, X, _BASE_ADR)
#define O_CFG(X)  CONCAT3(SIPP_OBUF, X, _CFG_ADR)
#define O_LS(X)   CONCAT3(SIPP_OBUF, X, _LS_ADR)
#define O_FC(X)   CONCAT3(SIPP_OBUF, X, _FC_ADR)

#define I_CTX(X)  CONCAT3(SIPP_ICTX, X, _ADR)
#define O_CTX(X)  CONCAT3(SIPP_OCTX, X, _ADR)

#define I_SHADOW_BASE(X) CONCAT3(SIPP_IBUF, X, _BASE_SHADOW_ADR)
#define I_SHADOW_CFG(X)  CONCAT3(SIPP_IBUF, X, _CFG_SHADOW_ADR)
#define I_SHADOW_LS(X)   CONCAT3(SIPP_IBUF, X, _LS_SHADOW_ADR)
#define I_SHADOW_PS(X)   CONCAT3(SIPP_IBUF, X, _PS_SHADOW_ADR)
#define O_SHADOW_BASE(X) CONCAT3(SIPP_OBUF, X, _BASE_SHADOW_ADR)

////////////////////////////////////////////////////////
//Program a Input/Output buffer
#define PROG_IO_BUFF(target, src) \
   SET_REG_WORD((u32)&target->base,    src->base); \
   SET_REG_WORD((u32)&target->cfg,     src->cfg);  \
   SET_REG_WORD((u32)&target->ls,      src->ls);   \
   SET_REG_WORD((u32)&target->ps,      src->ps);   \
   SET_REG_WORD((u32)&target->irqRate, src->irqRate);

#define SIPP_BUFF_IRQ_RATE_MASK     0x0000000F
#define SIPP_LINE_BUFFER_ALIGNMENT  0x7

////////////////////////////////////////////////////////
//Commands from LEON to Shave
#define CMD_H_PAD 0x01
#define CMD_RUN   0x02
#define CMD_EXIT  0x04

////////////////////////////////////////////////////////
// ISR done flag bits

#define PFL_SIPP_DONE      (0x1 << 0)
#define PFL_DMA_DONE       (0x1 << 1)
#define PFL_SVU_DONE       (0x1 << 2)
#define PFL_SIPP_EOF_DONE  (0x1 << 3)

////////////////////////////////////////////////////////
// Runtime Stats

// Synchronous collection
#define SIPP_ITER_STAT_STARTED_POS     0x0
#define SIPP_ITER_STAT_COMPLETE_POS    0x1
#define SIPP_ITER_STAT_HW_CYCLE_POS    0x2
#define SIPP_ITER_STAT_CDMA_CYCLE_POS  0x3
#define SIPP_ITER_STAT_SW_CYCLE_POS    0x4

// Asynchronous collection
#define SIPP_ITER_STAT_ASYNC_START_POS 0x0
#define SIPP_ITER_STAT_ASYNC_HW_POS    0x1
#define SIPP_ITER_STAT_ASYNC_SW_POS    0x2
#define SIPP_ITER_STAT_ASYNC_CDMA_POS  0x3
#define SIPP_ITER_STAT_ASYNC_FW_POS    0x4

// Run time stats area config
#define SIPP_RT_PER_ITER_STATS_SIZE 5
