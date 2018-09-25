# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : showPC.tcl
# Description: Show the PC registers for LEON and SHAVEs
# Created on : Jun 4, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    proc showPC { {nodeprecatedwarning 0} } {
        if {!$nodeprecatedwarning} {
            cli::DeprecatedWarning
        }
        foreach reg [concat [findReg *DSU_PC*] [findReg *INEXT*]] {
            puts [format {[%.8X] %-24s %.8X} [getRegAddr $reg] $reg [getReg32 $reg]]
        }
    }

    proc showPC_start {} {
        cli::DeprecatedWarning
        puts "-----------------------------"
        showPC
        variable showPC_ID
        set showPC_ID [after 1000 [namespace current]::showPC_start]
    }

    proc showPC_stop {} {
        cli::DeprecatedWarning
        variable showPC_ID
        after cancel $showPC_ID
        unset showPC_ID
    }

}
