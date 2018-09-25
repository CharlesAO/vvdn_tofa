///
/// @file DrvLeonDefines.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvLeon
/// @{
/// @brief     Leon functionality defines
///
/// This implements Leon processors functionality
///

#ifndef DRV_LEON_DEFINES_H
#define DRV_LEON_DEFINES_H

// 1: Defines
// ----------------------------------------------------------------------------
#define LEON_IRQI_NONE                (0)
#define LEON_IRQI_RUN                 (1 << 0)
#define LEON_IRQI_RESET               (1 << 1)

// This bit signifies that the Leon Rt Debug Support Unit is enabled             
#define CPR_LRT_DSU_ENABLED         ( 1 << 17)

// DSU_CTRL register bits:
#define DSU_CTRL_POWER_DOWN_BIT             ( 1 << 11)
#define DSU_CTRL_PROC_HALT_BIT              ( 1 << 10)
#define DSU_CTRL_PROC_ERROR_BIT             ( 1 <<  9)
#define DSU_CTRL_DSUBRE_BIT                 ( 1 <<  8)
#define DSU_CTRL_DSUEN_BIT                  ( 1 <<  7)
#define DSU_CTRL_DEBUG_MODE_BIT             ( 1 <<  6)
#define DSU_CTRL_BREAK_ON_ERROR_TRAPS_BIT   ( 1 <<  5)
#define DSU_CTRL_BREAK_ON_TRAP_BIT          ( 1 <<  4)
#define DSU_CTRL_BREAK_ON_SW_WATCHPOINT_BIT ( 1 <<  3)
#define DSU_CTRL_BREAK_ON_IU_WATCHPOINT_BIT ( 1 <<  2)
#define DSU_CTRL_BREAK_ON_ERROR_BIT         ( 1 <<  1)
#define DSU_CTRL_TRACE_ENABLE_BIT           ( 1 <<  0)

// DSU_BRK_SS register bits:
#define DSU_BRK_SS_SINGLE_STEP_BIT          ( 1 << 16)
#define DSU_BRK_SS_BREAK_NOW_BIT            ( 1 <<  0)
// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef enum{
    LEON_NO_STATE = 1,
    LEON_RUNNING,
    LEON_BOOTED,
    LEON_FINISHED
}drvLeonStates_enum_t;

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

#endif // DRV_CPR_H  


///@}
