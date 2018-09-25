///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @{
/// @brief    Definitions and types needed by software test library
///
/// This file contains all the definitions of constants, typedefs,
/// structures, enums and exported variables for the Test Utilities
///

#ifndef TEST_UTILS_API_DEFINES_H
#define TEST_UTILS_API_DEFINES_H

// 1. Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <DrvSvuDefines.h>
#include <DrvRegUtilsDefines.h>

// 2: Defines
// ----------------------------------------------------------------------------
#define DCR_DBGENABLE                   DCR_DBGE
#define OCR_HIST_REG                    OCR_TRACE_ENABLE
#define OPERATION_CTRL_REG              ( SLC_OFFSET_SVU + SVU_OCR  )
#define DEBUG_CTRL_REG                  ( SLC_OFFSET_SVU + SVU_DCR  )
#define PERF_COUNTER1_INSTR             ( SLC_OFFSET_SVU + SVU_PC0  )
#define PERF_COUNTER_CTRL_INSTR         ( SLC_OFFSET_SVU + SVU_PCC0 )
#define PERF_COUNTER2_STALLS            ( SLC_OFFSET_SVU + SVU_PC1  )
#define PERF_COUNTER_CTRL_STALLS        ( SLC_OFFSET_SVU + SVU_PCC1 )
#define PERF_COUNTER3_CLK_CYCLES        ( SLC_OFFSET_SVU + SVU_PC2  )
#define PERF_COUNTER_CTRL_CLK_CYCLES    ( SLC_OFFSET_SVU + SVU_PCC2 )
#define PERF_COUNTER4_BRANCHES          ( SLC_OFFSET_SVU + SVU_PC3  )
#define PERF_COUNTER_CTRL_BRANCHES      ( SLC_OFFSET_SVU + SVU_PCC3 )

// 3: Typedefs (types,enums,structs)
// ----------------------------------------------------------------------------
typedef struct
{
     /// @brief Counts the stalls cycles
     u32 perfCounterStall;
     /// @brief Counts the Shave execution cycles
     u32 perfCounterExec;
     /// @brief Counts the clock cycles
     u32 perfCounterClkCycles;
     /// @brief Counts the branches taken
     u32 perfCounterBranch;

     /// @brief Enables specific stalls from a given list to be counted
     u32 stallsTypes;
}performanceStruct;

typedef enum
{
     /// @brief Type selected for stalls
     STALL_COUNT,
     /// @brief Type selected for instruction cycles
     INSTRUCT_COUNT,
     /// @brief Type selected for clock cycles
     CLK_CYCLE_COUNT,
     /// @brief Type selected for branches taken
     BRANCH_COUNT,
}performanceCounterDef;

#undef GET_REG_WORD_VAL
#define GET_REG_WORD_VAL(address) \
({ \
    u32 local_var_result; \
    asm volatile ( \
        "lsu1.ld.32 %[result], %[address_]" "\n\t" \
        "nop 6" \
        : [result] "=r" (local_var_result) \
        : [address_] "r" (address) \
        : "memory" \
    ); \
    local_var_result; \
})

// workaround for control bus HW quirk
#undef SET_REG_WORD
#define SET_REG_WORD(address, data) \
({ \
    u32 local_var_tmp; \
    asm volatile ( \
        "lsu1.st.32 %[data_], %[address_]" "\n\t" \
        "lsu1.ld.32 %[tmp], %[dummy_ctrl_address]" "\n\t" \
        "nop 6" \
        : [tmp] "=r" (local_var_tmp) /* only written on the last non-nop
                                    instruction so no =&r required */ \
        : [address_] "r" (address), \
          [data_] "=r" (data) , \
          [dummy_ctrl_address] "r" (PMB_CFG_REG0_ADR) \
        : "memory" \
    ); \
})

/// @}
#endif // TEST_UTILS_API_DEFINES_H
