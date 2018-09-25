///
/// @file ShDrvCmxFifo.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup ShDrvCmxFifo ShCmxFifo Driver
/// @{
/// @brief CMX FIFO functionality implementation for MA2x5x
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X5X_INCLUDE_SHDRVCMXFIFO_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X5X_INCLUDE_SHDRVCMXFIFO_H_

#include <assert.h>
#include "registersMyriadMa2x5x.h"
#include "scResMgrInternal.h"
#include "ShDrvCommon.h"

__asm(
".include ShDrvCmxFifoDefines.incl\n"
".include svuCommonDefinitions.incl\n");

#define CMXFIFO_MAX_DEPTH 16
#define CMXFIFOS_NUM CFG_NUM_SHAVES
#define CMXFIFO_MEM_SZ (CMXFIFO_MAX_DEPTH*CMXFIFOS_NUM*8)


/// @brief Check whether a FIFO is full or not
///
/// @param[in] fifo_nr -  The FIFO which should be checked
///
/// @return
///  - 0 - The FIFO is full. All the following writes won't have any effect
///        if the values that already reside there wouldn't be read.
///  - 1 - The FIFO is not full. This doesn't mean that the FIFO is empty.
///
static inline uint32_t ShDrvCmxFifoIsFull(uint32_t fifo_nr)
{
    assert(fifo_nr < CMXFIFOS_NUM);

    uint32_t local_var_result, local_var_tmp0;
    uint32_t local_var_tmp1, local_var_tmp2, local_var_tmp3;

    __asm volatile(                                                        \
        "// 'ShDrvCmxFifoIsFull()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t" \
        "lsu0.ldil %[tmp3], PFIF0_PTR_STAT2_ADR"                           \
        "    || lsu1.ldil %[tmp0], 3"                               "\n\t" \
        "lsu0.ldih %[tmp3], PFIF0_PTR_STAT2_ADR"                           \
        "    || lsu1.ldil %[tmp1], 7"                                      \
        "    || cmu.cmii.i32 %[fifo_nr_], %[tmp0]"                  "\n\t" \
        "PEU.PC1C GT"                                                      \
        "   || iau.add %[tmp3], %[tmp3], 4"                                \
        "   || cmu.cmii.i32 %[fifo_nr_] %[tmp1]"                    "\n\t" \
        "PEU.PC1C GT || iau.add %[tmp3], %[tmp3], 4"                "\n\t" \
        "lsu0.ld.32 %[tmp2], %[tmp3]"                               "\n\t" \
        "lsu1.ldil %[tmp1], 3"                                      "\n\t" \
        "iau.and %[tmp0], %[fifo_nr_], %[tmp1]"                     "\n\t" \
        "iau.mul %[tmp0], %[tmp0], 8"                               "\n\t" \
        "nop"                                                       "\n\t" \
        "iau.add %[tmp0], %[tmp0], 5"                               "\n\t" \
        "nop"                                                       "\n\t" \
        "iau.shr.u32 %[tmp2] %[tmp2] %[tmp0]"                              \
        "   || lsu0.ldil %[tmp0], 1"                                "\n\t" \
        "iau.and %[result], %[tmp2], %[tmp0]"                              \
         /* for this output operand =&r is not needed because it is written
         only in the final instruction */                                  \
        : [result] "=r" (local_var_result),                                \
          [tmp0] "=&r" (local_var_tmp0),                                   \
          [tmp1] "=&r" (local_var_tmp1),                                   \
          [tmp2] "=&r" (local_var_tmp2),                                   \
          [tmp3] "=&r" (local_var_tmp3)                                    \
        : [fifo_nr_] "r" (fifo_nr)                                         \
        : "cc", "memory"                                                   \
    );
    return local_var_result;
}

/// @brief Check whether a FIFO has reached the 'almost full' level.
///
/// @param[in] fifo_nr -  The FIFO which should be checked
///
/// @return
///  - 0 - The FIFO doesn't have enough elements in the FIFO to trigger the 'Almost full' bit.
///  - 1 - The FIFO has triggered the 'Almost full' level.
///
static inline uint32_t ShDrvCmxFifoIsAlmostFull(uint32_t fifo_nr)
{
    assert(fifo_nr < CMXFIFOS_NUM);

    uint32_t local_var_result, local_var_tmp0;
    uint32_t local_var_tmp1, local_var_tmp2, local_var_tmp3;

    __asm volatile(                                                        \
        "// 'ShDrvCmxFifoIsAlmostFull()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t" \
        "lsu0.ldil %[tmp3], PFIF0_PTR_STAT2_ADR"                           \
        "    || lsu1.ldil %[tmp0], 3"                               "\n\t" \
        "lsu0.ldih %[tmp3], PFIF0_PTR_STAT2_ADR"                           \
        "    || lsu1.ldil %[tmp1], 7"                                      \
        "    || cmu.cmii.i32 %[fifo_nr_], %[tmp0]"                  "\n\t" \
        "PEU.PC1C GT"                                                      \
        "    || iau.add %[tmp3], %[tmp3], 4"                               \
        "    || cmu.cmii.i32 %[fifo_nr_] %[tmp1]"                   "\n\t" \
        "PEU.PC1C GT || iau.add %[tmp3], %[tmp3], 4"                "\n\t" \
        "lsu0.ld.32 %[tmp2], %[tmp3]"                               "\n\t" \
        "lsu1.ldil %[tmp1], 3"                                      "\n\t" \
        "iau.and %[tmp0], %[fifo_nr_], %[tmp1]"                     "\n\t" \
        "iau.mul %[tmp0], %[tmp0], 8"                               "\n\t" \
        "nop"                                                       "\n\t" \
        "iau.add %[tmp0], %[tmp0], 6"                               "\n\t" \
        "nop"                                                       "\n\t" \
        "iau.shr.u32 %[tmp2] %[tmp2] %[tmp0]"                              \
        "   || lsu0.ldil %[tmp0], 1"                                "\n\t" \
        "iau.and %[result], %[tmp2], %[tmp0]"                              \
        /* for this output operand =&r is not needed because it is written
        only in the final instruction */                                   \
        : [result] "=r" (local_var_result),                                \
          [tmp0] "=&r" (local_var_tmp0),                                   \
          [tmp1] "=&r" (local_var_tmp1),                                   \
          [tmp2] "=&r" (local_var_tmp2),                                   \
          [tmp3] "=&r" (local_var_tmp3)                                    \
        : [fifo_nr_] "r" (fifo_nr)                                         \
        : "cc", "memory"                                                   \
    );
    return local_var_result;
}

/// @brief Check whether a FIFO is empty
///
/// @param[in] fifo_nr -  The FIFO which should be checked
///
/// @return
///  - 0 - The FIFO is not empty(i.e. there is at least one element in FIFO)
///  - 1 - The FIFO is empty
///
static inline uint32_t ShDrvCmxFifoIsEmpty(uint32_t fifo_nr)
{
    assert(fifo_nr < CMXFIFOS_NUM);

    uint32_t local_var_result, local_var_tmp0, local_var_tmp1;
    uint32_t local_var_tmp2, local_var_tmp3;

    __asm volatile(                                                        \
        "// 'ShDrvCmxFifoIsEmpty()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t" \
        "lsu0.ldil %[tmp3], PFIF0_PTR_STAT2_ADR"                           \
        "    || lsu1.ldil %[tmp0], 3"                               "\n\t" \
        "lsu0.ldih %[tmp3], PFIF0_PTR_STAT2_ADR"                           \
        "    || lsu1.ldil %[tmp1], 7"                                      \
        "    || cmu.cmii.i32 %[fifo_nr_], %[tmp0]"                  "\n\t" \
        "PEU.PC1C GT"                                                      \
        "   || iau.add %[tmp3], %[tmp3], 4"                                \
        "   || cmu.cmii.i32 %[fifo_nr_] %[tmp1]"                    "\n\t" \
        "PEU.PC1C GT || iau.add %[tmp3], %[tmp3], 4"                "\n\t" \
        "lsu0.ld.32 %[tmp2], %[tmp3]"                               "\n\t" \
        "lsu1.ldil %[tmp1], 3"                                      "\n\t" \
        "iau.and %[tmp0], %[fifo_nr_], %[tmp1]"                     "\n\t" \
        "iau.mul %[tmp0], %[tmp0], 8"                               "\n\t" \
        "nop"                                                       "\n\t" \
        "iau.add %[tmp0], %[tmp0], 7"                               "\n\t" \
        "nop"                                                       "\n\t" \
        "iau.shr.u32 %[tmp2] %[tmp2] %[tmp0]"                              \
        "   || lsu0.ldil %[tmp0], 1"                                "\n\t" \
        "iau.and %[result], %[tmp2], %[tmp0]"                              \
        /* for this output operand =&r is not needed because it is written
           only in the final instruction */                                \
        : [result] "=r" (local_var_result),                                \
          [tmp0] "=&r" (local_var_tmp0),                                   \
          [tmp1] "=&r" (local_var_tmp1),                                   \
          [tmp2] "=&r" (local_var_tmp2),                                   \
          [tmp3] "=&r" (local_var_tmp3)                                    \
        : [fifo_nr_] "r" (fifo_nr)                                         \
        : "cc", "memory"                                                   \
    );
    return local_var_result;
}

/// @brief Write a 64-bit value to a Shave FIFO
///
/// @param[in] fifo_nr - the FIFO number to which data should be written
/// @param[in] val     - The uint64_t value that should be written to FIFO
///
/// @warning The user has to check if the FIFO is not full before writing to it
///
/// @return void
///
static inline void ShDrvCmxFifoWriteDWord(uint32_t fifo_nr, uint64_t val)
{
    assert(fifo_nr < CMXFIFOS_NUM);
    uint32_t write_address = PFIFO_FIFO0_ADR + (fifo_nr << 3);
    __asm volatile (
        "// 'ShDrvCmxFifoWriteDWord()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.st.64 %[val], %[write_address]"
        :: [val] "r" (val),
           [write_address] "r" (write_address)
        : "memory", "cc"
    );
}

/// @brief Write a 32-bit value to a Shave FIFO
///
/// @param[in] fifo_nr - the FIFO number to which data should be written
/// @param[in] val     - The uint32_t value that should be written to FIFO
///
/// @warning The user has to check if the FIFO is not full before writing to it
///
/// @return void
///
static inline void ShDrvCmxFifoWriteWord(uint32_t fifo_nr, uint32_t val)
{
    uint64_t fifo_val = (uint64_t) val;

    ShDrvCmxFifoWriteDWord(fifo_nr, fifo_val);
}

/// @brief Read a 64-bit value from the FIFO of the current shave
///
/// @warning The user has to check if the FIFO is not empty before writing to it
///
/// @return The 64-bit value read from FIFO
///
static inline uint64_t ShDrvCmxFifoReadDWord()
{
    uint32_t local_var_shave_offset, local_var_shave_fif_adr;
    uint64_t local_var_result;

    __asm volatile (                                                           \
        "// 'ShDrvCmxFifoReadDWord()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t" \
        "cmu.cpti %[shave_offset], P_SVID"                              "\n\t" \
        "lsu0.ldil %[shave_fif_adr], PFIFO_FIFO0_ADR"                          \
        "    || lsu1.ldih %[shave_fif_adr], PFIFO_FIFO0_ADR"                   \
        "    || iau.shl %[shave_offset], %[shave_offset], 3"            "\n\t" \
        "iau.add %[shave_fif_adr], %[shave_fif_adr], %[shave_offset]"   "\n\t" \
        "lsu0.ld.64 %[result], %[shave_fif_adr]"                        "\n\t" \
        "nop 6"                                                                \
        /* there are no register input operands
           therefore we don't need =&r */                                      \
        : [result] "=r"(local_var_result),                                     \
          [shave_fif_adr] "=r" (local_var_shave_fif_adr),                      \
          [shave_offset] "=r" (local_var_shave_offset)                         \
        :                                                                      \
        : "cc", "memory"                                                       \
    );
    return local_var_result;
}

/// @brief Read a 32-bit value from the FIFO of the current shave
///
/// @warning The user has to check if the FIFO is not empty before reading
///
/// @return The 32-bit value read from FIFO
///
static inline uint32_t ShDrvCmxFifoReadWord(void)
{
    uint32_t local_var_result, local_var_shave_fif_adr, local_var_shave_offset;

    __asm volatile (                                                           \
        "// 'ShDrvCmxFifoReadWord()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t" \
        "cmu.cpti %[shave_offset], P_SVID"                              "\n\t" \
        "lsu0.ldil %[shave_fif_adr], PFIFO_FIFO0_ADR"                          \
        "    || lsu1.ldih %[shave_fif_adr], PFIFO_FIFO0_ADR"                   \
        "    || iau.shl %[shave_offset], %[shave_offset], 3"            "\n\t" \
        "iau.add %[shave_fif_adr], %[shave_fif_adr], %[shave_offset]"   "\n\t" \
        "lsu0.ld.32 %[result], %[shave_fif_adr]"                        "\n\t" \
        "nop 6"                                                                \
        : [result] "=r"(local_var_result),                                     \
          [shave_fif_adr] "=r" (local_var_shave_fif_adr),                      \
          [shave_offset] "=r" (local_var_shave_offset)                         \
        :                                                                      \
        : "cc", "memory"                                                       \
    );
    return local_var_result;
}

/// @brief Read a 64-bit value from the FIFO of the current shave
///
/// @param[in] fifo_nr - the FIFO number from which data should be read
///
/// @warning The user has to check if the FIFO is not empty before reading
///
/// @return The 64-bit value read from FIFO.
///
static inline uint64_t ShDrvCmxFifoNReadDWord(uint32_t fifo_nr)
{
    assert(fifo_nr < CMXFIFOS_NUM);

    uint32_t shave_fif_adr = PFIFO_FIFO0_ADR + (fifo_nr << 3);
    uint64_t local_var_result;

    __asm volatile (
        "// 'ShDrvCmxFifoNReadDWord()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.ld.64 %[result], %[shave_fif_adr]"                        "\n\t"
        "nop 6"
        : [result] "=r"(local_var_result)
        /* the address is not needed after the first instruction, so we don't need an "=&r" */
        : [shave_fif_adr] "r" (shave_fif_adr)
        : "cc", "memory"
    );
    return local_var_result;
}


/// @brief Read a 32-bit value from the FIFO of the shave given as parameter
///
/// @param[in] fifo_nr - the FIFO number from which data should be read
///
/// @warning The user has to check if the FIFO is not empty before reading
///
/// @return The 32-bit value read from FIFO
///
static inline uint32_t ShDrvCmxFifoNReadWord(uint32_t fifo_nr)
{
    assert(fifo_nr < CMXFIFOS_NUM);

    uint32_t shave_fif_adr = PFIFO_FIFO0_ADR + (fifo_nr << 3);
    uint32_t local_var_result;

    __asm volatile (
        "// 'ShDrvCmxFifoNReadWord()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.ld.32 %[result], %[shave_fif_adr]"                        "\n\t"
        "nop 6"
        : [result] "=r"(local_var_result)
        /* the address is not needed after the first instruction, so we don't need an "=&r" */
        : [shave_fif_adr] "r" (shave_fif_adr)
        : "cc", "memory"
    );
    return local_var_result;
}

/// @brief Write a 64-bit value to a Shave FIFO directly into the FIFO memory.
///
/// This permits to alter the values already contained in FIFO without changing the FIFO pointers.
///
/// @param[in] fifo_nr - the FIFO number to which data should be written
/// @param[in] index   - The entry index where the value should be written to.
///                      There is a total of 16 x 64-bit entries for EACH shave,
///                      so this number shouldn't be bigger than 15.
/// @param[in] val     - The uint64_t value that should be written to FIFO
///
/// @return void
///
static inline void ShDrvCmxFifoWriteDirectDWord(uint32_t fifo_nr, uint32_t index, uint64_t val)
{
    assert(fifo_nr < CMXFIFOS_NUM && index < CMXFIFO_MAX_DEPTH);

    uint32_t shave_fif_adr = PFIFO_DIRECT_ADR + (fifo_nr << 7) + (index << 3);

    __asm volatile (
        "// 'ShDrvCmxFifoWriteDirectDWord()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.st.64 %[val], %[shave_fif_adr]"
        :: [val] "r" (val),
           [shave_fif_adr] "r" (shave_fif_adr)
        : "cc", "memory"
    );
}

/// @brief Write a 32-bit value to a Shave FIFO directly into the FIFO memory.
///
/// This permits to alter the values already contained in FIFO without changing the FIFO pointers.
///
/// @param[in] fifo_nr - The FIFO which should be written
/// @param[in] index   - The entry index where the value should be written to.
///                      There is a total of 16 x 64-bit entries for EACH shave,
///                      so this number shouldn't be bigger than 15.
/// @param[in] val     - The uint32_t value that should be written to FIFO
///
/// @return void
///
static inline void ShDrvCmxFifoWriteDirectWord(uint32_t fifo_nr, uint32_t index, uint32_t val)
{
    uint64_t fifo_val = (uint64_t) val;

    ShDrvCmxFifoWriteDirectDWord(fifo_nr, index, fifo_val);
}

/// @brief Read a 64-bit value from a Shave FIFO directly from the FIFO memory.
///
/// This permits to read the values from FIFO without changing the FIFO pointers.
///
/// @param[in]fifo_nr - the FIFO number from which data should be read
/// @param[in]index   - The entry index from where the value should be read.
///                     There is a total of 16 x 64-bit entries for EACH shave,
///                     so this number shouldn't be bigger than 15.
///
/// @return The 64-bit value read directly from FIFO.
///
static inline uint64_t ShDrvCmxFifoReadDirectDWord(uint32_t fifo_nr, uint32_t index)
{
    assert(fifo_nr < CMXFIFOS_NUM && index < CMXFIFO_MAX_DEPTH);

    uint32_t shave_fif_adr = PFIFO_DIRECT_ADR + (fifo_nr << 7) + (index << 3);
    uint64_t local_var_result;
    __asm volatile (
        "// 'ShDrvCmxFifoReadDirectDWord()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.ld.64 %[result], %[shave_fif_adr]"                        "\n\t"
        // after the first line of this inline assembly shave_fif_adr is not needed, so it's okay to use "=r"
        "nop 6"
        : [result] "=r" (local_var_result)
        : [shave_fif_adr] "r" (shave_fif_adr)
        : "cc", "memory"
    );
    return local_var_result;
}

/// @brief Read a 32-bit value directly from the FIFO memory without affecting the FIFO pointers.
///
/// @param[in] fifo_nr - The FIFO which should be read.
/// @param[in] index   - The entry index from where the value should be read.
///                      There is a total of 16 x 64-bit entries for EACH shave,
///                      so this number shouldn't be bigger than 15.
///
/// @return The 32-bit value read directly from FIFO
///
static inline uint32_t ShDrvCmxFifoReadDirectWord(uint32_t fifo_nr, uint32_t index)
{
    assert(fifo_nr < CMXFIFOS_NUM && index < CMXFIFO_MAX_DEPTH);

    uint32_t local_var_addr, local_var_shave_offset;
    uint32_t local_var_entryOffset, local_var_result;

    __asm volatile(                                                        \
        "// 'ShDrvCmxFifoReadDirectWord()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t" \
        "iau.shl %[shave_offset], %[fifo_nr_], 7"                          \
        "   || lsu0.ldil %[addr], PFIFO_DIRECT_ADR"                 "\n\t" \
        "iau.shl %[entryOffset], %[index_], 3"                             \
        " || lsu1.ldih %[addr], PFIFO_DIRECT_ADR"                   "\n\t" \
        "iau.add %[addr], %[addr], %[shave_offset]"                 "\n\t" \
        "iau.add %[addr], %[addr], %[entryOffset]"                  "\n\t" \
        "lsu0.ld.32 %[result], %[addr]"                             "\n\t" \
        "nop 6"                                                            \
        /* for this output operand =&r is not needed because it is written
        only in the final instruction */                                   \
        : [result]      "=r" (local_var_result),                           \
          [addr]        "=&r" (local_var_addr),                            \
          [shave_offset] "=&r" (local_var_shave_offset),                   \
          /* for this operand =&r is not needed, because the first time
          it's written is in the same cycle
          as the last input operand is read*/                              \
          [entryOffset] "=r" (local_var_entryOffset)                       \
        : [fifo_nr_]    "r"  (fifo_nr),                                    \
          [index_]      "r"  (index)                                       \
        : "cc", "memory"                                                   \
    );
    return local_var_result;
}

/// @brief Configures the FIFO status bit to route through to the SHAVE
///        for direct monitoring.
///
/// This allows the monitored FIFO status to be read from the SHAVE's TRF register.
///
/// @note All SHAVEs monitor FIFO 0 by default
///
/// @param[in] shave_nr - The Shave for which to enable the direct monitoring
/// @param[in] fifo_nr  - The FIFO that will be monitored
///
/// @return
///
static inline void ShDrvCmxFifoMonitorSelect(uint32_t shave_nr, uint32_t fifo_nr)
{
    assert(shave_nr < CFG_NUM_SHAVES && fifo_nr < CMXFIFOS_NUM);

    register uint32_t local_reg_val_l asm("i4");
    register uint32_t local_reg_val_h asm("i5");
    uint32_t l_reg_addr, l_tmp0, l_tmp1, l_tmp2;

    __asm volatile (                                                      \
        "// 'ShDrvCmxFifoMonitorSelect()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t" \
        "lsu0.ldil %[reg_addr], PFIFO_CTRL_ADR"                           \
        "    || lsu1.ldih %[reg_addr], PFIFO_CTRL_ADR"             "\n\t" \
        "lsu0.ldil %[tmp0], 7"                                            \
        "    || lsu1.ldil %[tmp1], 0xF"                            "\n\t" \
        "lsu0.ld.64 %[reg_val_l], %[reg_addr]"                     "\n\t" \
        "iau.shl %[tmp2], %[shave_nr_], 2"                         "\n\t" \
        "iau.shl %[tmp1], %[tmp1], %[tmp2]"                        "\n\t" \
        "iau.not %[tmp1], %[tmp1]"                                 "\n\t" \
        "iau.shl %[tmp2], %[fifo_val], %[tmp2]"                    "\n\t" \
        "nop 2"                                                    "\n\t" \
        "cmu.cmii.i32 %[shave_nr_], %[tmp0]"                       "\n\t" \
        "PEU.PC1C GT"                                                     \
        "    || iau.and %[reg_val_h] %[reg_val_h] %[tmp1]"         "\n\t" \
        "PEU.PC1C GT"                                                     \
        "    || iau.or %[reg_val_h] %[reg_val_h] %[tmp2]"          "\n\t" \
        "PEU.PC1C LTE"                                                    \
        "    || iau.and %[reg_val_l] %[reg_val_l] %[tmp1]"         "\n\t" \
        "PEU.PC1C LTE"                                                    \
        "    || iau.or %[reg_val_l] %[reg_val_l] %[tmp2]"          "\n\t" \
        "lsu0.st.64 %[reg_val_l], %[reg_addr]"                     "\n\t" \
        : [reg_addr] "=&r" (l_reg_addr),                                  \
          [reg_val_l] "=&r" (local_reg_val_l),                            \
          [reg_val_h] "=&r" (local_reg_val_h),                            \
          [tmp0] "=&r" (l_tmp0),                                          \
          [tmp1] "=&r" (l_tmp1),                                          \
          [tmp2] "=&r" (l_tmp2)                                           \
        : [fifo_val] "r" (fifo_nr),                                       \
          [shave_nr_] "r" (shave_nr)                                      \
        : "memory", "cc"                                                  \
    );

}

/// @brief Wait for an element in the monitored FIFO
///
/// @note Should be called from the SHAVE which monitors the FIFO
/// @note All SHAVEs monitor FIFO 0 by default
///
/// @return
///
static inline void ShDrvCmxFifoMonitorWaitElement(void)
{
    uint32_t local_var_tmp;

    __asm volatile (                                                             \
        "// 'ShDrvCmxFifoMonitorWaitElement()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t" \
        "LSU0.LDIL     %[tmp], 0x0001"                                    "\n\t" \
        "CMU.CMTI.BITP %[tmp], P_GPI"                                     "\n\t" \
        "PEU.PC1C GT  || BRU.RPIM 0 || CMU.CMTI.BITP %[tmp], P_GPI"       "\n\t" \
        "//Need this nop to ensure absolutely that there is no portclash." "\n\t" \
        "nop"                                                             "\n\t" \
        ".nowarnend"                                                             \
        : [tmp] "=r" (local_var_tmp)                                             \
        :                                                                        \
        : "cc", "memory"                                                         \
    );
}

/// @brief Check whether the monitored FIFO is empty or not
///
/// @note Should be called from the SHAVE which monitors the FIFO
/// @note All SHAVEs monitor FIFO 0 by default
///
/// @return
///     - 1 : FIFO is empty
///     - 0 : FIFO is not empty
///
static inline uint32_t ShDrvCmxFifoMonitorIsEmpty(void)
{
    uint32_t local_var_tmp, local_var_result;

    __asm volatile (                                        \
        "// 'ShDrvCmxFifoMonitorIsEmpty()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"\
        "lsu0.ldil %[tmp], 0x0001"                   "\n\t" \
        "cmu.cpti  %[result], P_GPI"                 "\n\t" \
        "iau.and   %[result], %[result], %[tmp]"     "\n\t" \
        : [tmp] "=r" (local_var_tmp),                       \
          [result] "=r" (local_var_result)                  \
        :                                                   \
        : "cc", "memory"                                    \
    );
    return local_var_result;
}

/// @brief Check whether the monitored FIFO is almost full or not
///
/// @note Should be called from the SHAVE which monitors the FIFO
/// @note All SHAVEs monitor FIFO 0 by default
///
/// @return
///     - 1 : FIFO is almost full
///     - 0 : FIFO is not almost full
///
static inline uint32_t ShDrvCmxFifoMonitorIsAlmostFull(void)
{
    uint32_t local_var_tmp, local_var_result;

    __asm volatile (                                        \
        "// 'ShDrvCmxFifoMonitorIsAlmostFull()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t" \
        "lsu0.ldil %[tmp], 0x0004"                   "\n\t" \
        "cmu.cpti  %[result], P_GPI"                 "\n\t" \
        "iau.and   %[result], %[result], %[tmp]"     "\n\t" \
        "iau.shr.u32 %[result], %[result], 2"               \
        : [tmp] "=r" (local_var_tmp),                       \
          [result] "=r" (local_var_result)                  \
        :                                                   \
        : "cc", "memory"                                    \
    );
    return local_var_result;
}

/// @brief Check whether the monitored FIFO is full or not
///
/// @note Should be called from the SHAVE which monitors the FIFO
/// @note All SHAVEs monitor FIFO 0 by default
///
/// @return
///     - 1 : FIFO is full
///     - 0 : FIFO is not full
///
static inline uint32_t ShDrvCmxFifoMonitorIsFull(void)
{
    uint32_t local_var_tmp, local_var_result;

    __asm volatile (                                        \
        "// 'ShDrvCmxFifoMonitorIsFull()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t" \
        "lsu0.ldil %[tmp], 0x0002"                   "\n\t" \
        "cmu.cpti  %[result], P_GPI"                 "\n\t" \
        "iau.and   %[result], %[result], %[tmp]"     "\n\t" \
        "iau.shr.u32 %[result], %[result], 1"               \
        : [tmp] "=r" (local_var_tmp),                       \
          [result] "=r" (local_var_result)                  \
        :                                                   \
        : "cc", "memory"                                    \
    );
    return local_var_result;
}

/// @brief Control RAM power modes. This permits to switch the FIFO RAM
///        to different power states.
///
/// This can be achieved by passing an 8-bit value, where the most significant
/// three bits trigger one of the three power states.
///  - bit 5 (SD) - When set, this bit shuts down power to periphery and memory core,
///            no memory data retention.
///  - bit 6 (LS) - When set, this bit triggers the 'Light Sleep' mode:
///            the memory goes into low leakage mode, there is no change in the output state.
///  - bit 7 (DS) - When set, this bit triggers the 'Deep Sleep' mode:
///            it shuts down power to periphery and maintains memory contents.
///            The outputs of the memory are pulled low.
/// The function expects each of these bits to be set separately for each distinct power mode.
/// @note If these bits are not set separately: DS overrides LS, SD overrides DS and LS.
///
/// @param[in] value - The input value
///
/// @return void
///
static inline void ShDrvCmxFifoRAMControl(uint32_t value)
{
    assert(value >= 5 && value <= 7);

    uint32_t mode_onehot = 1 << value;
    uint32_t tmp1, tmp2;

    __asm volatile (
        "// 'ShDrvCmxFifoRAMControl()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.ld.32 %[tmp1], %[pfifo_gen_ctrl]"
        "    || iau.shl %[tmp2], %[mode_onehot], 24"                "\n\t"
        "nop 6"                                                     "\n\t"
        "iau.or %[tmp1], %[tmp1], %[tmp2]"                          "\n\t"
        "lsu0.st.32 %[tmp1], %[pfifo_gen_ctrl]"
        : [tmp1] "=&r" (tmp1),
          [tmp2] "=&r" (tmp2)
        : [mode_onehot] "r" (mode_onehot),
          [pfifo_gen_ctrl] "r" (PFIFO_GEN_CTRL_ADR)
        : "memory", "cc"
    );
}

/// @brief Get the value that was set for the "Almost full" FIFO fill level
///
/// @return the 'Almost full' level which is currently set for all the FIFOs
///
static inline uint32_t ShDrvCmxFifoGetAlmostFullLevel(void)
{
    uint32_t local_var_result, local_var_tmp;

    __asm volatile(
        "// 'ShDrvCmxFifoGetAlmostFullLevel;()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.ldil %[tmp], PFIFO_GEN_CTRL_ADR"
        "    || lsu1.ldih %[tmp], PFIFO_GEN_CTRL_ADR"               "\n\t"
        "lsu0.ld.32 %[result], %[tmp]"                              "\n\t"
        "nop 6"                                                     "\n\t"
        "lsu0.ldil %[tmp], 0x003F || lsu1.ldih %[tmp], 0x0000"      "\n\t"
        "iau.and %[result], %[result], %[tmp]"
          /* =&r not needed because there are no
          input-only register operands */
        : [result] "=r" (local_var_result),
          [tmp] "=r" (local_var_tmp)
        :
        : "cc", "memory"
    );
    return local_var_result;
}

/// @brief Get the read pointer value for a specific Shave
///
/// @note Writing to a FIFO increments its read pointer
///
/// @param[in] fifo_nr - The FIFO for which the pointer value should be read
///
/// @return The read pointer value.
///
static inline uint32_t ShDrvCmxFifoGetReadPtrValue(uint32_t fifo_nr)
{
    assert(fifo_nr < CMXFIFOS_NUM);

    uint32_t local_var_result, local_var_tmp0;
    uint32_t local_var_tmp1, local_var_tmp2, local_var_tmp3;

    __asm volatile(
        "// 'ShDrvCmxFifoGetReadPtrValue;()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.ldil %[tmp3], PFIFO_PTR_STAT0_ADR"
        "    || lsu1.ldil %[tmp0], 3"                               "\n\t"
        "lsu0.ldih %[tmp3], PFIFO_PTR_STAT0_ADR"
        "    || lsu1.ldil %[tmp1], 7"
        "    || cmu.cmii.i32 %[fifo_nr_], %[tmp0]"                  "\n\t"
        "PEU.PC1C GT"
        "    || iau.add %[tmp3], %[tmp3], 4"
        "    || cmu.cmii.i32 %[fifo_nr_] %[tmp1]"                   "\n\t"
        "PEU.PC1C GT || iau.add %[tmp3], %[tmp3], 4"                "\n\t"
        "lsu0.ld.32 %[tmp2], %[tmp3]"                               "\n\t"
        "lsu1.ldil %[tmp1], 3"                                      "\n\t"
        "iau.and %[tmp0], %[fifo_nr_], %[tmp1]"                     "\n\t"
        "iau.mul %[tmp0], %[tmp0], 8"                               "\n\t"
        "nop"                                                       "\n\t"
        "iau.add %[tmp0], %[tmp0], 4"                               "\n\t"
        "nop"                                                       "\n\t"
        "iau.shr.u32 %[tmp2] %[tmp2] %[tmp0]"
        "   || lsu0.ldil %[tmp1], 0xF"                              "\n\t"
        "iau.and %[result], %[tmp2], %[tmp1]"
         /* for this variable =&r is not needed because no input operands
         are read after this variable is written */
        : [result] "=r" (local_var_result),
          [tmp0] "=&r" (local_var_tmp0),
          [tmp1] "=&r" (local_var_tmp1),
          [tmp2] "=&r" (local_var_tmp2),
          [tmp3] "=&r" (local_var_tmp3)
        : [fifo_nr_] "r" (fifo_nr)
        : "cc", "memory"
    );
    return local_var_result;
}

/// @brief Get the write pointer value for a specific Shave
///
/// @note Reading from a FIFO increments its write pointer
///
/// @param[in] fifo_nr - The FIFO for which the pointer value should be read
///
/// @return The write pointer value.
///
static inline uint32_t ShDrvCmxFifoGetWritePtrValue(uint32_t fifo_nr)
{
    assert(fifo_nr < CMXFIFOS_NUM);

    uint32_t local_var_result, local_var_tmp0;
    uint32_t local_var_tmp1, local_var_tmp2, local_var_tmp3;

    __asm volatile(
        "// 'ShDrvCmxFifoGetWritePtrValue;()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.ldil %[tmp3], PFIFO_PTR_STAT0_ADR"
        "    || lsu1.ldil %[tmp0], 3"                               "\n\t"
        "lsu0.ldih %[tmp3], PFIFO_PTR_STAT0_ADR"
        "    || lsu1.ldil %[tmp1], 7"
        "    || cmu.cmii.i32 %[fifo_nr_], %[tmp0]"                  "\n\t"
        "PEU.PC1C GT"
        "   || iau.add %[tmp3], %[tmp3], 4"
        "   || cmu.cmii.i32 %[fifo_nr_] %[tmp1]"                    "\n\t"
        "PEU.PC1C GT || iau.add %[tmp3], %[tmp3], 4"                "\n\t"
        "lsu0.ld.32 %[tmp2], %[tmp3]"                               "\n\t"
        "lsu1.ldil %[tmp1], 3"                                      "\n\t"
        "iau.and %[tmp0], %[fifo_nr_], %[tmp1]"                     "\n\t"
        "iau.mul %[tmp0], %[tmp0], 8"                               "\n\t"
        "nop 3"                                                     "\n\t"
        "iau.shr.u32 %[tmp2] %[tmp2] %[tmp0]"
        "   || lsu0.ldil %[tmp1], 0xF"                              "\n\t"
        "iau.and %[result], %[tmp2], %[tmp1]"
         /* for this variable =&r is not needed because no input operands
         are read after this variable is written */
        : [result] "=r" (local_var_result),
          [tmp0] "=&r" (local_var_tmp0),
          [tmp1] "=&r" (local_var_tmp1),
          [tmp2] "=&r" (local_var_tmp2),
          [tmp3] "=&r" (local_var_tmp3)
        : [fifo_nr_] "r" (fifo_nr)
        : "cc", "memory"
    );
    return local_var_result;
}

/// @brief Get the current fill level for a specific shave
///
/// @param[in] fifo_nr - The FIFO for which the fill level should be read
///
/// @return The number of elements currently available in FIFO
///
static inline uint32_t ShDrvCmxFifoGetFillLevel(uint32_t fifo_nr)
{
    assert(fifo_nr < CMXFIFOS_NUM);

    uint32_t local_var_result, local_var_tmp0;
    uint32_t local_var_tmp1, local_var_tmp2, local_var_tmp3;

    __asm volatile(
        "// 'ShDrvCmxFifoGetFillLevel;()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.ldil %[tmp3], PFIF0_PTR_STAT2_ADR"
        "    || lsu1.ldil %[tmp0], 3"                               "\n\t"
        "lsu0.ldih %[tmp3], PFIF0_PTR_STAT2_ADR"
        "    || lsu1.ldil %[tmp1], 7"
        "    || cmu.cmii.i32 %[fifo_nr_], %[tmp0]"                  "\n\t"
        "PEU.PC1C GT"
        "   || iau.add %[tmp3], %[tmp3], 4"
        "   || cmu.cmii.i32 %[fifo_nr_] %[tmp1]"                    "\n\t"
        "PEU.PC1C GT || iau.add %[tmp3], %[tmp3], 4"                "\n\t"
        "lsu0.ld.32 %[tmp2], %[tmp3]"                               "\n\t"
        "lsu1.ldil %[tmp1], 3"                                      "\n\t"
        "iau.and %[tmp0], %[fifo_nr_], %[tmp1]"                     "\n\t"
        "iau.mul %[tmp0], %[tmp0], 8"                               "\n\t"
        "nop 3"                                                     "\n\t"
        "iau.shr.u32 %[tmp2] %[tmp2] %[tmp0]"
        "   || lsu0.ldil %[tmp1], 0x1F || lsu1.ldih %[tmp1], 0"     "\n\t"
        "iau.and %[result], %[tmp2], %[tmp1]"
        /* for this output operand =&r is not needed because it is written
        only in the final instruction */
        : [result] "=r" (local_var_result),
          [tmp0] "=&r" (local_var_tmp0),
          [tmp1] "=&r" (local_var_tmp1),
          [tmp2] "=&r" (local_var_tmp2),
          [tmp3] "=&r" (local_var_tmp3)
        : [fifo_nr_] "r" (fifo_nr)
        : "cc", "memory"
    );
    return local_var_result;
}

/// @brief Set the 'Almost full' level for all the FIFOs
///
/// @param[in] level - The number of elements which can be written in FIFO until the 'Almost full' bit is triggered.
///
/// @return void
///
static inline void ShDrvCmxFifoSetAlmostFullLevel(uint32_t level)
{
    assert(level <= CMXFIFO_MAX_DEPTH);

    uint32_t local_var_tmp;

    __asm volatile (
        "// 'ShDrvCmxFifoSetAlmostFullLevel;()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.ldil %[tmp], PFIFO_GEN_CTRL_ADR"
        "    || lsu1.ldih %[tmp], PFIFO_GEN_CTRL_ADR"               "\n\t"
        "lsu0.st.16 %[level_], %[tmp]"
        : [tmp] "=&r" (local_var_tmp)
        : [level_] "r" (level)
        : "memory"
    );
}

/// @brief Atomic read a 64-bit value from the FIFO of the current shave
///
/// This function has a return for any state of the FIFO
///
/// @param[in] fifo_nr - the FIFO which should be read
///
/// @return The 64-bit value read from FIFO.
///         The upper byte of the return value will be set to 0x00 in case of success,
///         or to 0xFF if the FIFO was empty
///
static inline uint64_t ShDrvCmxFifoNReadDWordAtomic(uint32_t fifo_nr)
{
    assert(fifo_nr < CMXFIFOS_NUM);

    uint32_t shave_fif_adr = PFIFO_ATM_FIFO0_ADR + (fifo_nr << 3);

    uint64_t local_var_result;

    __asm volatile (
        "// 'ShDrvCmxFifoNReadAtomicDWord;()' defined in ShDrvCmxFifo.h used in " __FILE__"\n\t"
        "lsu0.ld.64 %[result], %[shave_fif_adr]"                      "\n\t"
        "nop 6"
        : [result] "=r" (local_var_result)
        : [shave_fif_adr] "r" (shave_fif_adr)
        : "cc", "memory"
    );
    return local_var_result;
}

static inline void ShDrvCmxFifoFlush(uint32_t fifo_nr)
{
    volatile uint64_t flush_var;

    while(!ShDrvCmxFifoIsEmpty(fifo_nr))
    {
        flush_var = ShDrvCmxFifoNReadDWord(fifo_nr);
    }
}

#endif // COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X5X_INCLUDE_SHDRVCMXFIFO_H_

///@}
