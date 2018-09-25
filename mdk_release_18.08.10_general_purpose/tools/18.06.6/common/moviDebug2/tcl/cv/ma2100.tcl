proc mdbg::MV_SOC_REV {} { return ma2100 }
source [file join [file dirname [info script]] myriad2.tcl]
mdbg::cv::myriad2::REGISTERS_FROM_HEADER "registersMyriadMa2100.h"
source [file join [file dirname [info script]] ma2100 gpio.tcl]
source [file join [file dirname [info script]] ma2x5x uart.tcl]
mdbg::uart::AUTO_INIT
