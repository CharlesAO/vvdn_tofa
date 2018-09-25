# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : uart.tcl
# Description: uart support - MA2x8x specific
# Created on : July 19, 2016
# Author     : Luminita Daraban (luminita.daraban@movidius.com)
# ***************************************************************************


namespace eval mdbg::uart {
    set UART_INDEX 0
    namespace eval REGS {
        proc INIT { } {
            set ns [namespace current]
            set index $::mdbg::uart::UART_INDEX
            foreach NAME {
                MCR
                LCR
                DLH
                DLL
                FCR
                IER
                TFL
                RBR
                USR
                RFL
            } {
                set ${ns}::$NAME UART${index}_${NAME}_ADR
            }
        }
    }

    proc INIT_CLOCKS {} {
        variable UART_INDEX

        # Gate Bits are 35,36,37,38
        set gate0bit [expr {$UART_INDEX + 35}]

        # Gate Bit is in second 32-bit word
        set gate1bit [expr {$gate0bit - 32}]

        # Take it out of reset
        changeReg32 CPR_BLK_RST1_ADR |= 1 << $gate1bit

        # Give it clock
        changeReg32 CPR_CLK_EN1_ADR  |= 1 << $gate1bit

        # Aux enable bits are quite randomly spread
        set auxEnBit [lindex {6 14 30 31} $UART_INDEX]
        # Enable Aux clock for interface
        changeReg32 CPR_AUX_CLK_EN_ADR |= (1 << $auxEnBit)
    }

 }; # namespace eval mdbg::uart
