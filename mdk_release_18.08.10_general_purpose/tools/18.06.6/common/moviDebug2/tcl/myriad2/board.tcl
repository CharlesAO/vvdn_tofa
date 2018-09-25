# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : board.tcl
# Description: Myriad2 board-related commands
# Created on : Jun 4, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    proc getPlatform {} {

        set value [cv::myriad2::getCprGenCtrlValue]

        set value [format "0x%x" [expr ($value >> 29) & 0x7]]
        switch -- $value {
            0x0 {return PLATFORM_SILICON }
            0x1 {return PLATFORM_VCS }
            0x2 {return PLATFORM_MOVISIM }
            0x3 {return PLATFORM_FPGA }
        }

        return
    }

    proc isPlatformMoviSim {} {
        string equal [getPlatform] PLATFORM_MOVISIM
    }

    proc isPlatformSilicon {} {
        string equal [getPlatform] PLATFORM_SILICON
    }

    proc isPlatformFPGA {} {
        string equal [getPlatform] PLATFORM_FPGA
    }

    proc isPlatformVCS {} {
        string equal [getPlatform] PLATFORM_VCS
    }

    namespace export isPlatform*

} ; # namespace eval mdbg
