///
/// @file DrvRegUtilsDefines.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvRegUtils
/// @{
/// @brief     Definitions and types needed by Reg utils library
///

#ifndef REG_UTILS_DEF_H
#define REG_UTILS_DEF_H

// 1: Defines
// ----------------------------------------------------------------------------
#include "registersMyriad.h"
#include "mv_types.h"

// Register Manipulatin Macros
// ----------------------------------------------------------------------------

/*
 * GET/SET_REG_* macros.
 * These are used to access memory, and memory mapped IO in an ordered fashion.
 * These macros guarantee that the load and store instructions will be generated in the order
 * in which they appear in C code. These macros don't guarantee that in all cases the actual
 * effect of these are executed in order. This is due to system architecture. There is a number
 * of bus bridges in the Myriad system that implement posted writes. That means that even if a
 * write transaction has left the CPU core, it is not guaranteed that the write transaction is
 * completed before a following transaction to a different peripheral is completed, because
 * a write may be cached in a bus bridge for an undetermined amount of time, if there is high
 * traffic. Issuing a read to a particular peripheral will flush all pending writes to that peripheral.
 * When talking to a single peripheral, ordering of transactions is completely maintained.
 * When the user wants to maintain ordering across peripherals,  when first talking to peripheral
 * A and then to peripheral B, then the last transaction to peripheral A must be a read/GET_REG_*.
 * If the last effective transaction to peripheral A in the code is a write, then it is the
 * responsability of the user to add a dummy GET_REG_WORD to a register without side effects
 * in peripheral A.
 * NOTE: the guidelines here do not take into account the case in which real memories, and caches
 * are involved. Depending on scenario, on those cases cache invalidate and/or writeback operations
 * may need to be performed.
 */

#ifndef __shave__
/*
 * SET_REG_DWORD is implemented as inline assembly because gcc for LEON replaces
 * a u64 store with two clr if x is zero which breaks the double-word store.
 * If/when this is fixed in the compiler, we can replace the inline assembly with the corresponding c code.
 *
 * If more of these definitions need to have seperate implementation for shaves and leons, 
 * this file should be duplicated to shave/ and leon/ subdirectories, so we keep our code clean.
 */
#define SET_REG_DWORD(a,x)  asm volatile( "std\t%0, [%1]" :: "r"((u64)(x)), "r"((unsigned)(a)) : "memory")
#define SET_REG_WORD(a,x)   ((void)(*(volatile u32*)(((unsigned)(a))) = (u32)(x)))
#define SET_REG_HALF(a,x)   ((void)(*(volatile u16*)(((unsigned)(a))) = (u16)(x)))
#define SET_REG_BYTE(a,x)   ((void)(*(volatile u8*)(a) = (u8)(x)))

#define GET_REG_DWORD_VAL(a) (*(volatile u64*)(((unsigned)(a))))
#define GET_REG_WORD_VAL(a) (*(volatile u32*)(((unsigned)(a))))
#define GET_REG_HALF_VAL(a) (*(volatile u16*)(((unsigned)(a))))
#define GET_REG_BYTE_VAL(a) (*(volatile u8*)(a))
#else
/*
 * The semantics for dereferencing volatile pointers to different objects is not well defined in
 * C standards. For this reason the effective semantics implemented on SPARC GCC and moviCompile
 * are different. On sparc GCC, volatile pointer dereferences will never be reordered, but on
 * moviCompile volatile pointer dereferences may not only be reordered, but more then one such
 * memory access may be scheduled on the same cycle, because of the multiple load/store units.
 *
 * To preserve traditional semantics for GET/SET_REG* macros, these need to be implemented
 * in assembly. Please see the description at the top of this file for what these semantics are.
 */
#define SET_REG_DWORD(a, x) \
    ({ \
        u64 local_var_value = (u64)(x); \
        register int local_var_al_ asm("i15") = (int)local_var_value; \
        register int local_var_ah_ asm("i16") = (int)(local_var_value >> 32); \
        __asm volatile ( \
            "// 'SET_REG_DWORD()' defined in DrvRegUtilsDefines.h used in " __FILE__"\n\t" \
            ".nowarn \n\t" \
            "lsu1.st.64 %[al_], %[addr_]" "\n\t" \
            ".nowarnend"\
            : \
            : [addr_] "r" ((int)a), \
              [al_] "r" (local_var_al_), \
              [ah_] "r" (local_var_ah_) \
            : "memory" \
        ); \
    })
#define SET_REG_WORD(a, x) \
    ({ \
        __asm volatile ( \
            "// 'SET_REG_WORD()' defined in DrvRegUtilsDefines.h used in " __FILE__"\n\t" \
            ".nowarn \n\t" \
            "lsu1.st.32 %[val_], %[addr_]" "\n\t" \
            ".nowarnend"\
            : \
            : [addr_] "r" ((int)a), \
              [val_] "r" ((int)(x)) \
            : "memory" \
        ); \
    })
#define SET_REG_HALF(a, x) \
    ({ \
        __asm volatile ( \
            "// 'SET_REG_HALF()' defined in DrvRegUtilsDefines.h used in " __FILE__"\n\t" \
            ".nowarn \n\t" \
            "lsu1.st.16 %[val_], %[addr_]" "\n\t" \
            ".nowarnend"\
            : \
            : [addr_] "r" ((int)a), \
              [val_] "r" ((int)(x)) \
            : "memory" \
        ); \
    })
#define SET_REG_BYTE(a, x) \
    ({ \
        __asm volatile ( \
            "// 'SET_REG_BYTE()' defined in DrvRegUtilsDefines.h used in " __FILE__"\n\t" \
            ".nowarn \n\t" \
            "lsu1.st.8 %[val_], %[addr_]" "\n\t" \
            ".nowarnend"\
            : \
            : [addr_] "r" ((int)a), \
              [val_] "r" ((int)(x)) \
            : "memory" \
        ); \
    })

#define GET_REG_DWORD_VAL(address) \
    ({ \
        register u32 local_var_result_low asm("i15"); \
        register u32 local_var_result_high asm("i16"); \
        asm volatile ( \
            "// 'GET_REG_DWORD_VAL()' defined in DrvRegUtilsDefines.h used in " __FILE__"\n\t" \
            ".nowarn \n\t" \
            "lsu1.ld.64 %[result_low], %[address_]" "\n\t" \
            "nop 6\n\t" \
            ".nowarnend"\
            : [result_low] "=&r" (local_var_result_low), \
              [result_high] "=&r" (local_var_result_high) \
            : [address_] "r" ((int)address) \
            : "memory" \
        ); \
        ((u64)local_var_result_low) | (((u64)local_var_result_high) << 32); \
    })
#define GET_REG_WORD_VAL(address) \
    ({ \
        u32 local_var_result; \
        asm volatile ( \
            "// 'GET_REG_WORD_VAL()' defined in DrvRegUtilsDefines.h used in " __FILE__"\n\t" \
            ".nowarn \n\t" \
            "lsu1.ld.32 %[result], %[address_]" "\n\t" \
            "nop 6\n\t" \
            ".nowarnend"\
            : [result] "=r" (local_var_result) \
            : [address_] "r" ((int)address) \
            : "memory" \
        ); \
        local_var_result; \
    })
#define GET_REG_HALF_VAL(address) \
    ({ \
        u32 local_var_result; \
        asm volatile ( \
            "// 'GET_REG_HALF_VAL()' defined in DrvRegUtilsDefines.h used in " __FILE__"\n\t" \
            ".nowarn \n\t" \
            "lsu1.ld.32.U16.U32 %[result], %[address_]" "\n\t" \
            "nop 6\n\t" \
            ".nowarnend"\
            : [result] "=r" (local_var_result) \
            : [address_] "r" ((int)address) \
            : "memory" \
        ); \
        (u16)local_var_result; \
    })
#define GET_REG_BYTE_VAL(address) \
    ({ \
        u32 local_var_result; \
        asm volatile ( \
            "// 'GET_REG_BYTE_VAL()' defined in DrvRegUtilsDefines.h used in " __FILE__"\n\t" \
            ".nowarn \n\t" \
            "lsu1.ld.32.U8.U32 %[result], %[address_]" "\n\t" \
            "nop 6\n\t" \
            ".nowarnend"\
            : [result] "=r" (local_var_result) \
            : [address_] "r" ((int)address) \
            : "memory" \
        ); \
        (u8)local_var_result; \
    })
#endif


#define GET_REG_DWORD(a,x)  ((void)((x) = GET_REG_DWORD_VAL(a)))
#define GET_REG_WORD(a,x)   ((void)((x) = GET_REG_WORD_VAL(a)))
#define GET_REG_HALF(a,x)   ((void)((x) = GET_REG_HALF_VAL(a)))
#define GET_REG_BYTE(a,x)   ((void)((x) = GET_REG_BYTE_VAL(a)))


#define SET_REG_BIT_NUM(a,offset)     SET_REG_WORD(a, GET_REG_WORD_VAL(a) | (u32)(1<<(offset)))
#define CLR_REG_BIT_NUM(a,offset)     SET_REG_WORD(a, GET_REG_WORD_VAL(a) & (u32)~(1<<(offset)))
#define SET_REG_BITS_MASK(a,set_mask) SET_REG_WORD(a, GET_REG_WORD_VAL(a) | (u32)(set_mask))
#define CLR_REG_BITS_MASK(a,clr_mask) SET_REG_WORD(a, GET_REG_WORD_VAL(a) & ~(u32)(clr_mask))

#define MS_WORD_64(a) (*(((volatile u32*)(&(a))) + 1))
#define LS_WORD_64(a) (*(((volatile u32*)(&(a))) + 0))

#define GEN_BIT_MASK(numBits) ( (numBits) >= 32U ? 0xFFFFFFFF : ((1U << (numBits)) - 1U) )

// Special Macro to detect when we are in a hardware simulation
// Specially optimised to be fast
#define IS_PLATFORM_SILICON ((GET_REG_WORD_VAL(CPR_GEN_CTRL_ADR) & 0xE0000000) == 0x00000000)  // We are in VCS Sim iff CPR_GEN_CTRL[31..29] == 0
#define IS_PLATFORM_VCS     ((GET_REG_WORD_VAL(CPR_GEN_CTRL_ADR) & 0xE0000000) == 0x20000000)  // We are in VCS Sim iff CPR_GEN_CTRL[31..29] == 1
#define IS_PLATFORM_MOVISIM ((GET_REG_WORD_VAL(CPR_GEN_CTRL_ADR) & 0xE0000000) == 0x40000000)  // We are in Movi Sim iff CPR_GEN_CTRL[31..29] == 2
#define IS_PLATFORM_FPGA    ((GET_REG_WORD_VAL(CPR_GEN_CTRL_ADR) & 0xE0000000) == 0x60000000)  // We are in FPGA iff CPR_GEN_CTRL[31..29] == 3

#define GET_PLATFORM_TYPE   ((GET_REG_WORD_VAL(CPR_GEN_CTRL_ADR) & 0xE0000000) >> 29        )  // Return bits 31..29 as the chip TYPE

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef enum
{
    SET_WORD,     // Simply write the word (Write)
    SET_BITS,     // Set bits as per the mask value (Read modify write)
    CLR_BITS      // Clear bits as per the mask value (Read modify write)
} tyRegMaskOperation;

typedef enum
{
    PLATFORM_SILICON = 0,
    PLATFORM_VCS     = 1,
    PLATFORM_MOVISIM = 2,
    PLATFORM_FPGA    = 3
} tyHwPlatform;

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

#endif // REG_UTILS_DEF_H

///@}
