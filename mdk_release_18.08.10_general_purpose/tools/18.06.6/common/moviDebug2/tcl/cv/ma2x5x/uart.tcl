# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : uart.tcl
# Description: uart support - MA2x[05]x specific
# Created on : July 19, 2016
# Author     : Luminita Daraban (luminita.daraban@movidius.com)
# ***************************************************************************

namespace eval mdbg::uart {

    namespace eval REGS {
        proc INIT {} {
            set ns [namespace current]
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
               set ${ns}::$NAME UART_${NAME}_ADR
            }
        }
    }

    proc INIT_CLOCKS {} {
        set cprClkEnable0Value [getReg32 CPR_CLK_EN0_ADR]
        set cprAuxClkEnableValue [getReg32 CPR_AUX_CLK_EN_ADR]

        setReg32 CPR_CLK_EN0_ADR $cprClkEnable0Value | (1 << 22)
        setReg32 CPR_AUX_CLK_EN_ADR $cprAuxClkEnableValue | (1 << 6)
    }

}; # namespace eval mdbg::uart

