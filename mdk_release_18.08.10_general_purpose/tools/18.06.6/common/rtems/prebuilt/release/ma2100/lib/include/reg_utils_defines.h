/*
 *  Register manipulatin macros
 *
 *  COPYRIGHT (c) 2013.
 *  Movidius.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#ifndef _RTEMS_REG_UTILS_DEF_H
#define _RTEMS_REG_UTILS_DEF_H

// To be able to use movidius types
#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

// 1: Defines
// ----------------------------------------------------------------------------

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

/*
 * SET_REG_DWORD is implemented as inline assembly because gcc for LEON replaces
 * a u64 store with two clr if x is zero which breaks the double-word store.
 * If/when this is fixed in the compiler, we can replace the inline assembly with the corresponding c code.
 */
#define SET_REG_DWORD(a,x)  asm volatile( "std\t%0, [%1]" :: "r"((u64)(x)), "r"((unsigned)(a)) : "memory")
#define SET_REG_WORD(a,x)   ((void)(*(volatile u32*)(((unsigned)(a))) = (u32)(x)))
#define SET_REG_HALF(a,x)   ((void)(*(volatile u16*)(((unsigned)(a))) = (u16)(x)))
#define SET_REG_BYTE(a,x)   ((void)(*(volatile u8*)(a) = (u8)(x)))

#define GET_REG_DWORD_VAL(a) (*(volatile u64*)(((unsigned)(a))))
#define GET_REG_WORD_VAL(a) (*(volatile u32*)(((unsigned)(a))))
#define GET_REG_HALF_VAL(a) (*(volatile u16*)(((unsigned)(a))))
#define GET_REG_BYTE_VAL(a) (*(volatile u8*)(a))

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

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // _RTEMS_REG_UTILS_DEF_H
