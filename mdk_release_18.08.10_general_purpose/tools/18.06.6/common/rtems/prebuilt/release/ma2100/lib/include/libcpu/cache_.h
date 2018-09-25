/*
 *  Myriad2 Cache Manager Support
 */

#ifndef __MYRIAD2_CACHE_H
#define __MYRIAD2_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"
#include "myriad2_registers.h"
#include "reg_utils_defines.h"
#include "swcLeonUtilsDefines.h"

/********************************************************************************/
/* Definitions */
/********************************************************************************/
/* Masks for uncached memory access */
#define MYRIAD2_DRAM_CACHED_START_ADDR      (0x80000000)
#define MYRIAD2_DRAM_UNCACHED_MASK          (0x40000000)
#define MYRIAD2_CMX_UNCACHED_MASK           (0x08000000)
/* Replacement Policy */
#define L2C_REPL_LRU                        (0)
#define L2C_REPL_PSEUDO_RANDOM              (1)
#define L2C_REPL_MASTER_INDEX_REP           (2)
#define L2C_REPL_MASTER_INDEX_MOD           (3)
/* L2 cache Mode */
#define L2C_MODE_COPY_BACK                  (1)
#define L2C_MODE_WRITE_THROUGH              (2)
/* MTRR mode*/
#define L2C_MTRR_UNCACHED                   (0)
#define L2C_MTRR_WRITE_THROUGH              (1)

/* L2 cache definitions */
#define L2C_OP_INVALIDATE                   (1)
#define L2C_OP_WRITEBACK                    (2)

#define L2C_CTRL_EN                         (1 << 31)
#define L2C_CONTROL_OFF                     (L2C_LEON_OS_CONTROL - L2C_LEON_OS_BASE_ADR)
#define L2C_CTRL_REPL__LRU                  (0 << 28)
#define L2C_CTRL_NR_LOCKED_WAYS(number)     (((number) & 0xf) << 8)
#define L2C_CTRL_WRITE_POLICY_COPY_BACK     (0 << 1)

/********************************************************************************/
/* Variable prototypes */
/********************************************************************************/
extern uint32_t __l2c_base__;               /* L2C Base Address */
extern rtems_interrupt_lock lxc_cache_lock; /* Lock to protect concurrent L1/L2 cache accesses */
extern __BSP_L1_CACHE_CONFIG__ BSP_L1_CACHE_CONFIG; /* L1C configuration */
extern __BSP_L2_CACHE_CONFIG__ BSP_L2_CACHE_CONFIG; /* L2C configuration */

#define CPU_CACHE_SUPPORT_PROVIDES_RANGE_FUNCTIONS
#define CPU_CACHE_SUPPORT_PROVIDES_CACHE_SIZE_FUNCTIONS

#define CPU_INSTRUCTION_CACHE_ALIGNMENT     (64)
#define CPU_DATA_CACHE_ALIGNMENT            (64)

/********************************************************************************/
/* helper functions */
/********************************************************************************/

static inline size_t get_max_size(size_t a, size_t b) {
  return a < b ? b : a;
}

/********************************************************************************/
/* L1 Instruction cache operations */
/********************************************************************************/

static inline void cache_l1c_flush_instruction(void) {
    __asm__ volatile ("flush");
}

static inline void cache_l1c_disable_instruction(void) {
    rtems_interrupt_lock_context lock_context;
    rtems_interrupt_lock_acquire( &lxc_cache_lock, &lock_context);

    asm volatile(
        "lda [%%g0] %1, %%g1; "
        "bclr %0, %%g1; "
        "sta %%g1, [%%g0] %1"
        :: "r"(CCR_ICS_ENABLED), "I"(__CCR_ASI) : "%g1"
    );

    rtems_interrupt_lock_release( &lxc_cache_lock, &lock_context);
}

static inline void cache_l1c_enable_instruction(void) {
    rtems_interrupt_lock_context lock_context;
    rtems_interrupt_lock_acquire( &lxc_cache_lock, &lock_context);

    asm volatile(
        "lda [%%g0] %1, %%g1; "
        "bset %0, %%g1; "
        "sta %%g1, [%%g0] %1"
        :
        : "r"(CCR_ICS_ENABLED), "I"(__CCR_ASI)
        : "%g1"
   );

    rtems_interrupt_lock_release( &lxc_cache_lock, &lock_context);
}

static inline void cache_l1c_freeze_instruction(void) {
    rtems_interrupt_lock_context lock_context;
    rtems_interrupt_lock_acquire( &lxc_cache_lock, &lock_context);

    asm volatile(
        "lda [%%g0] %1, %%g1; "
        "bclr %0, %%g1; "
        "sta %%g1, [%%g0] %1"
        :: "r"(CCR_ICS_FROZEN), "I"(__CCR_ASI) : "%g1"
    );

    rtems_interrupt_lock_release( &lxc_cache_lock, &lock_context);
}

static inline uint32_t cache_l1c_get_instruction_cache_size(void) {
    if (__l2c_base__ == L2C_LEON_OS_BASE_ADR)
        return MYRIAD2_LEON_OS_L1_INST_CACHE_SIZE;
    else
        return MYRIAD2_LEON_RT_L1_INST_CACHE_SIZE;
}

static inline size_t cache_l1c_get_instruction_line_size(void) {
    if (__l2c_base__ == L2C_LEON_OS_BASE_ADR)
        return MYRIAD2_LEON_OS_L1_INSTRUCTION_LINE_SIZE;
    else
        return MYRIAD2_LEON_RT_L1_INSTRUCTION_LINE_SIZE;
}

/********************************************************************************/
/* L1 Instruction data operations */
/********************************************************************************/

static inline void cache_l1c_invalidate_data_line(uint32_t addr) {
    uint32_t data = 0;
    asm volatile(
        "lda [%[addr_]] 1, %[result]"
        : [result] "=r" (data)
        : [addr_] "r" (addr)
        : "memory" );
    data = data;
}

static inline void cache_l1c_flush_data(void) {
    asm volatile(
        "sta %%g0, [%%g0] %[dcache_flush_asi]"
        :
        : [dcache_flush_asi] "I" (__DCACHE_FLUSH_ASI)
        : "memory" );
}

static inline void cache_l1c_enable_data(uint32_t flush) {
    rtems_interrupt_lock_context lock_context;
    rtems_interrupt_lock_acquire( &lxc_cache_lock, &lock_context);

    asm volatile(
        "lda [%%g0] %1, %%g1; "
        "bset %0, %%g1; "
        "sta %%g1, [%%g0] %1"
        :
        : "r"((CCR_DCS_ENABLED)|((flush)?CCR_FD:0)), "I"(__CCR_ASI)
        : "%g1"
    );

    rtems_interrupt_lock_release( &lxc_cache_lock, &lock_context);

}

static inline void cache_l1c_disable_data(void) {
    rtems_interrupt_lock_context lock_context;
    rtems_interrupt_lock_acquire( &lxc_cache_lock, &lock_context);

    asm volatile(
        "lda [%%g0] %1, %%g1; "
        "bclr %0, %%g1; "
        "sta %%g1, [%%g0] %1"
        :: "r"(CCR_DCS_ENABLED), "I"(__CCR_ASI) : "%g1"
    );

    rtems_interrupt_lock_release( &lxc_cache_lock, &lock_context);
}

static inline void cache_l1c_freeze_data(void) {
    rtems_interrupt_lock_context lock_context;
    rtems_interrupt_lock_acquire( &lxc_cache_lock, &lock_context);

    asm volatile(
        "lda [%%g0] %1, %%g1; "
        "bclr %0, %%g1; "
        "sta %%g1, [%%g0] %1"
        :: "r"(CCR_DCS_FROZEN), "I"(__CCR_ASI) : "%g1"
    );

    rtems_interrupt_lock_release( &lxc_cache_lock, &lock_context);
}

static inline uint32_t cache_l1c_get_data_cache_size(void) {
    if (__l2c_base__ == L2C_LEON_OS_BASE_ADR)
        return MYRIAD2_LEON_OS_L1_DATA_CACHE_SIZE;
    else
        return MYRIAD2_LEON_RT_L1_DATA_CACHE_SIZE;
}

static inline size_t cache_l1c_get_data_line_size(void) {
    if (__l2c_base__ == L2C_LEON_OS_BASE_ADR)
        return MYRIAD2_LEON_OS_L1_DATA_LINE_SIZE;
    else
        return MYRIAD2_LEON_RT_L1_DATA_LINE_SIZE;
}

/* Allows to invalidate a memory range by forcing cache misses on all its associated lines
 * This function checks the size of the invalidation and flushes the whole L1 Data cache if
 * the size of the operation exceeds 32 lines
 */
static void cache_l1c_invalidate_data_address_range(uint32_t address, uint32_t bytes) {
    /* Do nothing if number of bytes to flush is zero */
    if( bytes == 0 )
      return;

    if (bytes >= L1_MAX_BYTES_INVALIDATION)
        cache_l1c_flush_data();
    else {
        const uint32_t final_address = address + bytes - 1;
        const uint32_t linesize = cache_l1c_get_data_line_size();

        for(address &= ~( linesize -1); address <= final_address; address+=linesize)
            cache_l1c_invalidate_data_line(address);
    }
}

/********************************************************************************/
/* L2 cache common operations */
/********************************************************************************/

static inline uint32_t cache_l2c_get_status(void) {
    return GET_REG_WORD_VAL(__l2c_base__ + (L2C_LEON_OS_STATUS - L2C_LEON_OS_BASE_ADR));
}

static inline void cache_l2c_operation_all(uint32_t operation, uint32_t disable_cache) {
    uint32_t reg = (__l2c_base__) + (L2C_LEON_OS_FLUSH_ADR - L2C_LEON_OS_BASE_ADR);
    uint32_t val = (((disable_cache) & 1) << 3) |((operation) & 3) |(1 << 2);
    SET_REG_WORD(reg, val);

    //Stall the processor if we executed an WRITEBACK operation
    if (operation & L2C_OP_WRITEBACK)
        GET_REG_WORD_VAL(__l2c_base__ + L2C_CONTROL_OFF);
}

static inline void cache_l2c_operation_address(uint32_t operation, uint32_t address) {
    uint32_t reg = (__l2c_base__) + (L2C_LEON_OS_FLUSH_ADR - L2C_LEON_OS_BASE_ADR);
    uint32_t val = ((address) & 0xffffffe0) | ((operation) & 3);
    SET_REG_WORD(reg, val);
}

static inline uint32_t cache_l2c_get_line_size(void) {
    uint32_t status = cache_l2c_get_status();
    if ((status >> 24) & 0x01)
        return 64;
    else
        return 32;
}

static void cache_l2c_operation_address_range(uint32_t operation,
                                              uint32_t address,
                                              uint32_t bytes) {
    const uint32_t final_address = address + bytes - 1;
    const uint32_t linesize = cache_l2c_get_line_size();

    /* Do nothing if number of bytes to flush is zero */
    if( bytes == 0 )
        return;

    for(address &= ~( linesize -1); address <= final_address; address+=linesize)
        cache_l2c_operation_address(operation, address);

    //Stall the processor if we executed an WRITEBACK operation
    if (operation & L2C_OP_WRITEBACK)
        GET_REG_WORD_VAL(__l2c_base__ + L2C_CONTROL_OFF);
}

static inline uint32_t cache_l2c_get_cache_size(void) {
    /* total cache size = 4 sets x cache set size (status [12:2]) */
    return cache_l2c_get_status() & 0x1FFC;
}

static inline void cache_l2c_disable(void) {
    // It is mandatory to flush the cache at the same time
    cache_l2c_operation_all(L2C_OP_INVALIDATE|L2C_OP_WRITEBACK, 1);
}

static inline void cache_l2c_enable(uint32_t repl, uint32_t locked_ways, uint32_t mode) {
    uint32_t reg = __l2c_base__ + L2C_CONTROL_OFF;
    uint32_t val = (L2C_CTRL_EN | (repl << 28) | L2C_CTRL_NR_LOCKED_WAYS(locked_ways) | mode);
    // Enable and Invalidate
    cache_l2c_operation_all(L2C_OP_INVALIDATE, 0);
    // Initialize
    SET_REG_WORD(reg, val);
}

static inline uint32_t cache_l2c_get_mtrr_count(void)
{
    uint32_t status = cache_l2c_get_status();
    return (((status >> 16) & 0x3f));
}

static inline uint32_t cache_l2c_program_mtrr(uint32_t address,
                                              uint32_t mask,
                                              uint8_t accessmode,
                                              uint8_t writeprotected,
                                              uint32_t number) {
    uint32_t reg = 0, val = 0;

    if (number < cache_l2c_get_mtrr_count()) {
        rtems_interrupt_lock_context lock_context;
        rtems_interrupt_lock_acquire( &lxc_cache_lock, &lock_context);

        reg = (__l2c_base__) + (L2C_LEON_OS_MTRR0 - L2C_LEON_OS_BASE_ADR) + (number << 2);
        val = address & 0xfffc0000;
        val |= (mask >> 16) & 0x0000fffc;
        val |= (accessmode & 3) << 16;
        val |= ((writeprotected) ? 1 : 0) << 1;
        val |= 1;
        SET_REG_WORD(reg, val);

        rtems_interrupt_lock_release( &lxc_cache_lock, &lock_context);
    }
    else
        number = cache_l2c_get_mtrr_count();


    return number;
}

/********************************************************************************/
/* Functions to export */
/********************************************************************************/

/* L1/L2 */
static inline size_t _CPU_cache_get_instruction_cache_size(const uint32_t level) {
  size_t size;

  switch(level)
  {
    case 0:
      size = get_max_size(cache_l1c_get_instruction_cache_size(), cache_l2c_get_cache_size());
      break;
    case 1:
      size = cache_l1c_get_instruction_cache_size();
      break;
    case 2:
      size = cache_l2c_get_cache_size();
      break;
    default:
      size = 0;
      break;
  }

  return size;
}

static inline size_t _CPU_cache_get_data_cache_size(const uint32_t level) {
  size_t size;

  switch(level)
  {
    case 0:
      size = get_max_size(cache_l1c_get_data_cache_size(), cache_l2c_get_cache_size());
      break;
    case 1:
      size = cache_l1c_get_data_cache_size();
      break;
    case 2:
      size = cache_l2c_get_cache_size();
      break;
    default:
      size = 0;
      break;
  }

  return size;
}

static inline size_t _CPU_cache_get_instruction_line_size(const uint32_t level) {
  size_t size;

  switch(level)
  {
    case 0:
      size = get_max_size(cache_l1c_get_instruction_line_size(), cache_l2c_get_line_size());
      break;
    case 1:
      size = cache_l1c_get_instruction_line_size();
      break;
    case 2:
      size = cache_l2c_get_line_size();
      break;
    default:
      size = 0;
      break;
  }

  return size;
}

static inline size_t _CPU_cache_get_data_line_size(const uint32_t level) {
  size_t size;

  switch(level)
  {
    case 0:
      size = get_max_size(cache_l1c_get_data_line_size(), cache_l2c_get_line_size());
      break;
    case 1:
      size = cache_l1c_get_data_line_size();
      break;
    case 2:
      size = cache_l2c_get_line_size();
      break;
    default:
      size = 0;
      break;
  }

  return size;
}

static inline void _CPU_cache_flush_data_range(const void *d_addr, size_t n_bytes) {
    // Flush (invalidate and write back) L2 cache
    if (BSP_L2_CACHE_CONFIG.enable)
        cache_l2c_operation_address_range(L2C_OP_INVALIDATE|L2C_OP_WRITEBACK, (uint32_t) d_addr, (uint32_t) n_bytes);

    // Invalidate L1 cache
    if (BSP_L1_CACHE_CONFIG.enableDCache)
        cache_l1c_invalidate_data_address_range((uint32_t) d_addr, (uint32_t) n_bytes);
}

static inline void _CPU_cache_invalidate_data_range(const void *d_addr, size_t n_bytes) {
    // Invalidate L2 cache
    if (BSP_L2_CACHE_CONFIG.enable)
        cache_l2c_operation_address_range(L2C_OP_INVALIDATE, (uint32_t) d_addr, (uint32_t) n_bytes);

    // Invalidate L1 cache
    if (BSP_L1_CACHE_CONFIG.enableDCache)
        cache_l1c_invalidate_data_address_range((uint32_t) d_addr, (uint32_t) n_bytes);
}

static inline void _CPU_cache_flush_instruction_range(const void *i_addr, size_t n_bytes) {
    // Invalidate L2 cache
    if (BSP_L2_CACHE_CONFIG.enable)
        cache_l2c_operation_address_range(L2C_OP_INVALIDATE|L2C_OP_WRITEBACK, (uint32_t) i_addr, (uint32_t) n_bytes);

    // Invalidate L1 cache
    if (BSP_L1_CACHE_CONFIG.enableICache)
        cache_l1c_flush_instruction();
}

static inline void _CPU_cache_invalidate_instruction_range(const void *i_addr, size_t n_bytes) {
    // Invalidate L2 cache
    if (BSP_L2_CACHE_CONFIG.enable)
        cache_l2c_operation_address_range(L2C_OP_INVALIDATE, (uint32_t) i_addr, (uint32_t) n_bytes);

    // Invalidate L1 cache
    if (BSP_L1_CACHE_CONFIG.enableICache)
        cache_l1c_flush_instruction();
}

/* L1 instruction */
static inline void _CPU_cache_invalidate_entire_l1_instruction(void) {
    if (BSP_L1_CACHE_CONFIG.enableICache)
        cache_l1c_flush_instruction();
}

static inline void _CPU_cache_freeze_l1_instruction (void) {
    if (BSP_L1_CACHE_CONFIG.enableICache)
        cache_l1c_freeze_instruction();
}

static inline void _CPU_cache_unfreeze_l1_instruction (void) {
    if (BSP_L1_CACHE_CONFIG.enableICache)
        cache_l1c_enable_instruction();
}

static inline void _CPU_cache_enable_l1_instruction (void) {
    if (BSP_L1_CACHE_CONFIG.enableICache == 0) {
        BSP_L1_CACHE_CONFIG.enableICache = 1;
        cache_l1c_enable_instruction();
    }
}

static inline void _CPU_cache_disable_l1_instruction (void) {
    if (BSP_L1_CACHE_CONFIG.enableICache) {
        BSP_L1_CACHE_CONFIG.enableICache = 0;
        cache_l1c_disable_instruction();
    }
}

/* L1 data */
static inline void _CPU_cache_enable_l1_data(void) {
    if (BSP_L1_CACHE_CONFIG.enableDCache == 0) {
        BSP_L1_CACHE_CONFIG.enableDCache = 1;
        cache_l1c_enable_data(1);
    }
}

static inline void _CPU_cache_disable_l1_data(void) {
    if (BSP_L1_CACHE_CONFIG.enableDCache) {
        BSP_L1_CACHE_CONFIG.enableDCache = 0;
        cache_l1c_disable_data();
    }
}

static inline void _CPU_cache_flush_entire_l1_data(void) {
    if (BSP_L1_CACHE_CONFIG.enableDCache)
        cache_l1c_flush_data();
}

static inline void _CPU_cache_invalidate_entire_l1_data(void) {
    if (BSP_L1_CACHE_CONFIG.enableDCache)
        cache_l1c_flush_data();
}

static inline void _CPU_cache_freeze_l1_data(void) {
    if (BSP_L1_CACHE_CONFIG.enableDCache)
        cache_l1c_freeze_data();
}

static inline void _CPU_cache_unfreeze_l1_data(void) {
    if (BSP_L1_CACHE_CONFIG.enableDCache)
        cache_l1c_enable_data(0);
}

static inline void _CPU_cache_invalidate_range_l1_data( const void *d_addr, size_t n_bytes) {
    if (BSP_L1_CACHE_CONFIG.enableDCache)
        cache_l1c_invalidate_data_address_range((uint32_t) d_addr, (uint32_t) n_bytes);
}

/* L2 */
static inline void _CPU_cache_flush_l2(void) {
    if (BSP_L2_CACHE_CONFIG.enable)
        cache_l2c_operation_all(L2C_OP_INVALIDATE|L2C_OP_WRITEBACK, 0);
}

static inline void _CPU_cache_invalidate_l2(void) {
    if (BSP_L2_CACHE_CONFIG.enable)
        cache_l2c_operation_all(L2C_OP_INVALIDATE, 0);
}

static inline void _CPU_cache_writeback_l2(void) {
    if (BSP_L2_CACHE_CONFIG.enable)
        cache_l2c_operation_all(L2C_OP_WRITEBACK, 0);
}

static inline void _CPU_cache_enable_l2(uint32_t repl, uint32_t locked_ways, uint32_t mode) {
    if (BSP_L2_CACHE_CONFIG.enable == 0) {
        BSP_L2_CACHE_CONFIG.enable = 1;
        cache_l2c_enable(repl, locked_ways, mode);
    }
}

static inline void _CPU_cache_disable_l2(void) {
    if (BSP_L2_CACHE_CONFIG.enable) {
        BSP_L2_CACHE_CONFIG.enable = 0;
        cache_l2c_disable();
    }
}

static inline void _CPU_cache_flush_range_l2(const void *addr, size_t n_bytes) {
    if (BSP_L2_CACHE_CONFIG.enable)
        cache_l2c_operation_address_range(L2C_OP_INVALIDATE|L2C_OP_WRITEBACK, (uint32_t) addr, (uint32_t) n_bytes);
}

static inline void _CPU_cache_invalidate_range_l2(const void *addr, size_t n_bytes) {
    if (BSP_L2_CACHE_CONFIG.enable)
        cache_l2c_operation_address_range(L2C_OP_INVALIDATE, (uint32_t) addr, (uint32_t) n_bytes);
}

static inline void _CPU_cache_writeback_range_l2(const void *addr, size_t n_bytes) {
    if (BSP_L2_CACHE_CONFIG.enable)
        cache_l2c_operation_address_range(L2C_OP_WRITEBACK, (uint32_t) addr, (uint32_t) n_bytes);
}

static inline uint32_t _CPU_cache_get_mtrr_count_l2(void) {
    return cache_l2c_get_mtrr_count();
}

static inline uint32_t _CPU_cache_program_mtrr_l2(uint32_t address,
                                                  uint32_t mask,
                                                  uint8_t accessmode,
                                                  uint8_t writeprotected,
                                                  uint32_t number) {
    return cache_l2c_program_mtrr(address, mask, accessmode, writeprotected, number);
}

/* Uncached/cached addresses utilities */
static inline uint32_t _CPU_cache_get_uncached_address(uint32_t addr) {
    if (addr & MYRIAD2_DRAM_CACHED_START_ADDR)
        return (addr | MYRIAD2_DRAM_UNCACHED_MASK);
    else
        return (addr | MYRIAD2_CMX_UNCACHED_MASK);
}

static inline uint32_t _CPU_cache_get_cached_address(uint32_t addr) {
    if (addr & MYRIAD2_DRAM_CACHED_START_ADDR)
        return (addr & ~MYRIAD2_DRAM_UNCACHED_MASK);
    else
        return (addr & ~MYRIAD2_CMX_UNCACHED_MASK);
}

/********************************************************************************/
/* Function mapping (for functions part of cache_manager) */
/********************************************************************************/
static inline void _CPU_cache_flush_entire_data(void) {
    // Flush L1 cache data
    _CPU_cache_flush_entire_l1_data();

    // Flush L2
    _CPU_cache_flush_l2();
}

static inline void _CPU_cache_invalidate_entire_data(void) {
    // Invalidate L2
    _CPU_cache_invalidate_l2();

    // Flush L1 cache data
    _CPU_cache_invalidate_entire_l1_data();
}

static inline void _CPU_cache_freeze_data(void) {
    _CPU_cache_freeze_l1_data();
}

static inline void _CPU_cache_unfreeze_data(void) {
    _CPU_cache_unfreeze_l1_data();
}

static inline void _CPU_cache_enable_data(void) {
    _CPU_cache_enable_l1_data();
}

static inline void _CPU_cache_disable_data(void) {
    _CPU_cache_disable_l1_data();
}

static inline void _CPU_cache_invalidate_entire_instruction(void) {
    _CPU_cache_invalidate_entire_l1_instruction();
}

static inline void _CPU_cache_enable_instruction(void) {
    _CPU_cache_enable_l1_instruction();
}

static inline void _CPU_cache_disable_instruction(void) {
    _CPU_cache_disable_l1_instruction();
}

static inline void _CPU_cache_freeze_instruction(void) {
    _CPU_cache_freeze_l1_instruction();
}

static inline void _CPU_cache_unfreeze_instruction(void) {
    _CPU_cache_unfreeze_l1_instruction();
}

/********************************************************************************/
/* Function mapping (for functions not part of cache_manager) */
/********************************************************************************/

/* L1 / L2 */
#define rtems_cache_flush_data_range(_d_addr, _n_bytes)                         _CPU_cache_flush_data_range(_d_addr, _n_bytes)                  //Backwards compatibility: Replaced by rtems_cache_flush_multiple_data_lines
#define rtems_cache_flush_1_data_line(_d_addr)                                  _CPU_cache_flush_data_range(_d_addr, 1)                         //Should not be used, instead use rtems_cache_flush_multiple_data_lines
#define rtems_cache_invalidate_data_range(_d_addr, _n_bytes)                    _CPU_cache_invalidate_data_range(_d_addr, _n_bytes)             //Backwards compatibility: Replaced by rtems_cache_invalidate_multiple_data_lines
#define rtems_cache_invalidate_1_data_line(_d_addr)                             _CPU_cache_invalidate_data_range(_d_addr, 1)                    //Should not be used, instead use rtems_cache_invalidate_multiple_data_lines
#define rtems_cache_flush_instruction_range(_i_addr, _n_bytes)                  _CPU_cache_flush_instruction_range(_i_addr, _n_bytes)
#define rtems_cache_flush_1_instruction_line(_i_addr)                           _CPU_cache_flush_instruction_range(_i_addr, 1)
#define rtems_cache_invalidate_instruction_range(_i_addr, _n_bytes)             _CPU_cache_invalidate_instruction_range(_i_addr, _n_bytes)
#define rtems_cache_invalidate_1_instruction_line(_i_addr)                      _CPU_cache_invalidate_instruction_range(_i_addr, 1)

#define rtems_cache_get_instruction_line_length(_level)                         _CPU_cache_get_instruction_line_size(_level)
#define rtems_cache_get_data_line_length(_level)                                _CPU_cache_get_data_line_size(_level)

/* L1 instruction */
#define rtems_cache_enable_l1_instruction()                                     _CPU_cache_enable_l1_instruction()
#define rtems_cache_disable_l1_instruction()                                    _CPU_cache_disable_l1_instruction()
#define rtems_cache_freeze_l1_instruction()                                     _CPU_cache_freeze_l1_instruction()
#define rtems_cache_unfreeze_l1_instruction()                                   _CPU_cache_unfreeze_l1_instruction()
#define rtems_cache_invalidate_entire_l1_instruction()                          _CPU_cache_invalidate_entire_l1_instruction()

/* L1 data */
#define rtems_cache_enable_l1_data()                                            _CPU_cache_enable_l1_data()
#define rtems_cache_disable_l1_data()                                           _CPU_cache_disable_l1_data()
#define rtems_cache_freeze_l1_data()                                            _CPU_cache_freeze_l1_data()
#define rtems_cache_unfreeze_l1_data()                                          _CPU_cache_unfreeze_l1_data()
#define rtems_cache_flush_entire_l1_data()                                      _CPU_cache_flush_entire_l1_data()
#define rtems_cache_invalidate_entire_l1_data()                                 _CPU_cache_invalidate_entire_l1_data()
#define rtems_cache_invalidate_l1_data_range(_d_addr, _n_bytes)                 _CPU_cache_invalidate_range_l1_data(_d_addr, _n_bytes)

/* L2 */
#define rtems_cache_flush_l2()                                                  _CPU_cache_flush_l2()
#define rtems_cache_invalidate_l2()                                             _CPU_cache_invalidate_l2()
#define rtems_cache_writeback_l2()                                              _CPU_cache_writeback_l2()
#define rtems_cache_enable_l2(_repl, _ways, _mode)                              _CPU_cache_enable_l2(_repl, _ways, _mode)
#define rtems_cache_disable_l2()                                                _CPU_cache_disable_l2()
#define rtems_cache_flush_range_l2(_addr, _bytes)                               _CPU_cache_flush_range_l2(_addr,_bytes)
#define rtems_cache_invalidate_range_l2(_addr, _bytes)                          _CPU_cache_invalidate_range_l2(_addr,_bytes)
#define rtems_cache_writeback_range_l2(_addr, _bytes)                           _CPU_cache_writeback_range_l2(_addr,_bytes)
#define rtems_cache_get_mtrr_count_l2()                                         _CPU_cache_get_mtrr_count_l2()
#define rtems_cache_program_mtrr_l2(_addr,_mask,_acc, _wp, _num)                _CPU_cache_program_mtrr_l2(_addr, _mask,_acc, _wp, _num)

/* get (un)cached address */
#define rtems_cache_get_uncached_address(_addr)                                 _CPU_cache_get_uncached_address(_addr)
#define rtems_cache_get_cached_address(_addr)                                   _CPU_cache_get_cached_address(_addr)

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
