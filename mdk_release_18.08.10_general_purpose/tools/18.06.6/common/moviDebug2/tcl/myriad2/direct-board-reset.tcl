# ***************************************************************************
# Copyright (C) 2018 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : myriad2/direct-board-reset.tcl
# Created on : May 7, 2018
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    proc directBoardReset { {clockAll false } } {
            mvproto::lock {
                if {[isPlatformMoviSim]} {
                    # stop all clocks
                    mdbg::setReg32 CPR_CLK_EN0_ADR 0x0
                    mdbg::setReg32 CPR_CLK_EN1_ADR 0x0
                    # do a Master Reset for moviSim
                    mdbg::setReg32 CPR_MAS_RESET_ADR 0x0
                    # wait for the simulator to get reset
                    after 1
                } else {
                    mvproto::reset
                    if {[catch {
                         switch [mvproto::getLeonHaltStatus] {
                             1       {set leonHaltStatus "ok" }
                             0       {set leonHaltStatus "failed"}
                             default {set leonHaltStatus "unknown" }
                         }
                     } error]} {
                        set leonHaltStatus $error
                    }
                    puts $::mdbg::Channels::info [format "Leon Halted: %s." $leonHaltStatus]
                }

                if {[info commands ::mdbg::MV_SOC_REV] != ""
                    && [string match "ma2?8?" [::mdbg::MV_SOC_REV]]
                } {
                    puts $::mdbg::Channels::warning "Warning: Direct Board reset fo MA2x8x is incomplete!"
                    return
                }

                after 100

                # Clock everything
                if $clockAll {
                    setReg32 CPR_CLK_EN0_ADR 0xFFFFFFFF
                    setReg32 CPR_CLK_EN1_ADR 0xFFFFFFFF
                }

                after 100

                # set L2 cache mode - no direct mode
                setReg32 L2C_MODE_ADR 0x6

                foreach leon {OS RT} {
                    # Put Leon $leon into debug mode
                    set val [getReg32 "LEON_${leon}_DSU_CTRL_ADR"]
                    setReg32 "LEON_${leon}_DSU_CTRL_ADR" [mdbg::FLAGS \
                     DSU_CTRL_REG_PE_MASK \
                     | DSU_CTRL_B_ON_ERR_OPT \
                     | DSU_CTRL_REG_BS_MASK \
                     | DSU_CTRL_REG_BW_MASK \
                     | DSU_CTRL_REG_TE_MASK \
                    ]
                    # Break now: force leon into debug mode
                    setReg32 "LEON_${leon}_DSU_BREAK_SINGLE_STEP_ADR" 1
                }
            }
        }
    } ; # namespace eval mdbg