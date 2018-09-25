# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : leonDefs.tcl
# Description: LEON definitions (flags)
# Created on : Jun 6, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::FLAGS {
    # (TE) Trace enable: Enables instruction tracing. If set, the instructions will be
    #                    stored in the trace buffer. Remains set when the processor enters
    #                    debug or error mode.
    set DSU_CTRL_REG_TE_MASK                             [expr 1 <<  0]

    set DSU_TRACE_ENABLE                                 $DSU_CTRL_REG_TE_MASK

    # (BE) Break on error: if set, will force the processor to debug mode when
    #                      the processor would have entered error condition (trap in trap)
    set DSU_CTRL_REG_BE_MASK                             [expr 1 <<  1]

    # (BW) Break on IU watchpoint: if set, debug mode will be forced on a IU watchpoint (trap 0xb)
    set DSU_CTRL_REG_BW_MASK                             [expr 1 <<  2]
    set DSU_BREAK_WATCHPOINT                             $DSU_CTRL_REG_BW_MASK

    # (BS) Break on SW breakpoint: if set, debug mode will be forced when a breakpoint
    #                              instruction (ta 1) is executed.
    set DSU_CTRL_REG_BS_MASK                             [expr 1 <<  3]
    set DSU_CTRL_REG_SW_BP_HIT_MASK                      $DSU_CTRL_REG_BS_MASK

    # (BX) Break on trap: if set, will force the processor into debug mode when any trap occurs
    set DSU_CTRL_REG_BX_MASK                             [expr 1 << 4]

    # (BZ) Break on error traps: if set, will force the processor into debug mode on all except the following
    #                            traps: privileged_instruction, fpu disabled, window_overflow, window_underflow,
    #                            asynchronous_interrupt, ticc_trap
    set DSU_CTRL_REG_BZ_MASK                             [expr 1 <<  5]
    set DSU_CTRL_BZ_MASK                                 $DSU_CTRL_REG_BZ_MASK

    # (DM )Debug mode: indicates when the processor has entered debug mode (read-only)
    set DSU_CTRL_REG_DEBUG_MODE_MASK                     [expr 1 <<  6]
    set DSU_CTRL_REG_DM_MASK                             [expr 1 <<  6]

    # (EE) Value of the external DSUEN signal (read-only)
    set DSU_CTRL_REG_EXT_DSUEN_MASK                      [expr 1 <<  7]
    # (EB) Value of the external DSUBRE signal (read-only)
    set DSU_CTRL_REG_EXT_DSUBRE_MASK                     [expr 1 <<  8]

    # (PE) Processor error mode. - returns `1` on read when processor is in error mode, else `0`
    #                            - if written, `1`, it will clear the error and halt mode
    set DSU_CTRL_REG_ERROR_MODE_MASK                     [expr 1 <<  9]
    set DSU_CTRL_REG_PE_MASK                             [expr 1 <<  9]

    # (HL) Processor halt - returns `1` if processor is in error mode
    #                     - if processor is in debug mode, setting this bit will put the processor
    #                       in halt mode.
    set DSU_CTRL_REG_HALT_MODE_MASK                      [expr 1 << 10]
    set DSU_CTRL_REG_HL_MASK                             [expr 1 << 10]

    # (PW) Power down. Returns `1` when processor is in power-down mode
    set DSU_CTRL_REG_POWER_DOWN_MASK                     [expr 1 << 11]

    # 12 bits = 3 x 4 bits
    set DSU_CTRL_REG_VALUE_MASK 0xfff

    # The following 2 lines control if the debugger will automatically break
    # when an Error Trap Occurs. This is not desirable for higher level OS's such as Linux
    # which rely on MMU traps for normal operation.
    set DSU_CTRL_B_ON_ERR_OPT                            [expr 0x1 << 1]
    set DSU_CTRL_B_ON_ERR_TRAPS_OPT                      $DSU_CTRL_BZ_MASK

    # (BNx) Break now - Force processor x into debug mode of the break watchpoint (BW) bit
    #                   in the DSU control register is set. If cleared, the processor x will resume
    #                   execution.
    set DSU_BSS_REG_BN0_MASK                              [expr 1 <<  0]
    set DSU_BSS_REG_BN1_MASK                              [expr 1 <<  1]
        # (SSx) Single step - If set, the processor will execute one instruction and return to debug mode. The
    #                     bit remains set after the processor goes out into the debug mode.
    set DSU_BSS_REG_SS0_MASK                              [expr 1 << 16]
    set DSU_BSS_REG_SS1_MASK                              [expr 1 << 17]
}



